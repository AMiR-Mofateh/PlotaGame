#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::onNewConnection);
    ui->Ip->setText("127.0.0.1");
    ui->Port->setText("1234");

    // --- تعریف یوزرهای تستی ---
    // در نسخه نهایی این‌ها باید از فایل خوانده شوند
    loadUsers();

    if(userDatabase.isEmpty()){
        userDatabase.insert("admin", "1234");
        saveUsers();
    }
}

void MainWindow::loadUsers() {
    QFile file(usersFilePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    userDatabase.clear();
    for(auto key : obj.keys()) {
        userDatabase.insert(key, obj[key].toString());
    }
    file.close();
    ui->Log->addItem("User database loaded. Count: " + QString::number(userDatabase.size()));
}

void MainWindow::saveUsers() {
    QJsonObject obj;
    // تبدیل Map به JsonObject
    QMapIterator<QString, QString> i(userDatabase);
    while (i.hasNext()) {
        i.next();
        obj.insert(i.key(), i.value());
    }

    QJsonDocument doc(obj);
    QFile file(usersFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}
MainWindow::~MainWindow() { stopServer(); delete ui; }

// ... (توابع Start و Stop و Connection بدون تغییر می‌مانند) ...
// فقط توابع زیر را کپی/جایگزین کنید:

void MainWindow::on_Start_clicked() {
    // کد قبلی خود را حفظ کنید
    if (!serverRunning) {
        if (!tcpServer->listen(QHostAddress(ui->Ip->text()), ui->Port->text().toUShort())) return;
        serverRunning = true;
        ui->Start->setText("Stop");
        ui->Log->addItem("Server Started.");
    } else {
        stopServer();
    }
}

void MainWindow::onNewConnection() {
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        clients.append(socket);
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onClientDisconnected);
        ui->Log->addItem("New Client Connected.");
    }
}

void MainWindow::onClientDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(!socket) return;

    if (onlineUsers.contains(socket)) {
        QString user = onlineUsers[socket];
        onlineUsers.remove(socket);
        ui->Log->addItem("User " + user + " disconnected.");

        // خبر دادن به بقیه که این کاربر رفت
        broadcastUserList();
    }

    clients.removeAll(socket);
    socket->deleteLater();
}

void MainWindow::stopServer()
{
    for(auto c : clients) { c->disconnectFromHost(); c->deleteLater(); }
    clients.clear();
    tcpServer->close();
    serverRunning = false;
    ui->Start->setText("Start");
}

// --- منطق اصلی دریافت پیام ---
void MainWindow::onReadyRead()
{
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender());
    if (!senderSocket) return;

    QByteArray data = senderSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(doc.isNull() || !doc.isObject()) return;

    QJsonObject jsonObj = doc.object();
    QString type = jsonObj["type"].toString();

    if (type == "login")
    {
        QString user = jsonObj["username"].toString();
        QString pass = jsonObj["password"].toString();

        ui->Log->addItem("Login attempt: " + user);

        QJsonObject response;
        response["type"] = "login_result";

        if (checkLoginCredentials(user, pass))
        {
            if (onlineUsers.values().contains(user)) {
                response["success"] = false;
                response["message"] = "User is already logged in!";
            }
            else {
                response["success"] = true;
                response["message"] = "Login Successful";

                // ** اضافه کردن به لیست آنلاین‌ها **
                onlineUsers.insert(senderSocket, user);
                ui->Log->addItem("User " + user + " logged in.");

                // ارسال پاسخ لاگین به همین کاربر
                sendJson(senderSocket, response);

                // ** ارسال لیست جدید کاربران به همه **
                broadcastUserList();

                return; // چون پاسخ را فرستادیم، return می‌کنیم تا دوبار ارسال نشود
            }
        }
        else
        {
            response["success"] = false;
            response["message"] = "Invalid Username or Password";
            ui->Log->addItem("Failed login for " + user);
        }

        sendJson(senderSocket, response);
    }
    else if (type == "signup")
    {
        QString user = jsonObj["username"].toString();
        QString pass = jsonObj["password"].toString();
        // می‌توانید فیلدهای دیگر مثل ایمیل و نام را هم بگیرید ولی فعلا یوزر/پسورد مهم است

        ui->Log->addItem("Signup attempt: " + user);

        QJsonObject response;
        response["type"] = "signup_result";

        if (userDatabase.contains(user)) {
            response["success"] = false;
            response["message"] = "Username already exists!";
        } else {
            userDatabase.insert(user, pass);
            saveUsers(); // ذخیره در فایل

            response["success"] = true;
            response["message"] = "Account created successfully!";
            ui->Log->addItem("User " + user + " registered.");
        }
        sendJson(senderSocket, response);
    }

    // سایر دستورات مثل move بازی اینجا اضافه می‌شوند
}

bool MainWindow::checkLoginCredentials(QString username, QString password)
{
    if (userDatabase.contains(username))
    {
        if (userDatabase.value(username) == password)
            return true;
    }
    return false;
}

void MainWindow::sendJson(QTcpSocket *socket, QJsonObject jsonObj)
{
    QJsonDocument doc(jsonObj);
    socket->write(doc.toJson());
    socket->flush();
}

void MainWindow::on_Exit_clicked() { close(); }

void MainWindow::broadcastUserList()
{
    QJsonObject json;
    json["type"] = "user_list";

    QJsonArray userArray;
    // استخراج تمام نام‌ها از Map
    for(auto user : onlineUsers.values()) {
        userArray.append(user);
    }

    json["users"] = userArray;

    // ارسال به همه کلاینت‌های متصل
    for(auto socket : clients) {
        if(socket->state() == QAbstractSocket::ConnectedState) {
            sendJson(socket, json);
        }
    }
}
