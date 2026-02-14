#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGridLayout>
#include <QCryptographicHash>
#include <QRegularExpression> // اضافه شده برای اعتبارسنجی

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // مقداردهی مدیریت کاربران
    userManager = new UserManager();

    qApp->setStyleSheet(R"(
    QWidget {
        background-color: qradialgradient(
            cx:0.5, cy:0.35, radius:1.3,
            stop:0 #16222a,
            stop:0.4 #0f2027,
            stop:0.8 #090a0f,
            stop:1 #000000
        );
        color: #e6edf3;
        font-family: Segoe UI;
        font-size: 14px;
    }
    QFrame, QWidget#connectFourContainer {
        background-color: qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #141e30,
            stop:1 #243b55
        );
        border-radius: 20px;
        border: 1px solid #1f3c88;
    }
    QPushButton {
        background-color: #1a1d2b;
        color: #e6edf3;
        border: 1px solid #2f3640;
        border-radius: 12px;
        padding: 6px 16px;
    }
    QPushButton:hover {
        background-color: #2c3e50;
        border: 1px solid #00eaff;
    }
    QPushButton#btnBack,
    QPushButton#btnStartGame,
    QPushButton#btnLogin {
        background-color: #0d1117;
        color: #00eaff;
        border: 2px solid #00eaff;
        border-radius: 16px;
        font-weight: bold;
    }
    QPushButton#btnBack:hover,
    QPushButton#btnStartGame:hover,
    QPushButton#btnLogin:hover {
        background-color: #00eaff;
        color: black;
    }
    QLabel#label_6,
    QLabel#lblTitle {
        background-color: #0d1117;
        color: #00eaff;
        border: 2px solid #00eaff;
        border-radius: 18px;
        padding: 8px 20px;
        font-size: 26px;
        font-weight: bold;
        letter-spacing: 2px;
    }
    QLineEdit {
        background-color: #111827;
        border: 1px solid #2f3640;
        border-radius: 10px;
        padding: 6px;
        color: #e6edf3;
    }
    QLineEdit:focus {
        border: 1px solid #00eaff;
    }
    )");

    ui->stackedWidget->setCurrentWidget(ui->pageLogin);

    // --- تنظیمات دوز 4تایی ---
    QGridLayout* gridCF = new QGridLayout;
    gridCF->setSpacing(12);
    gridCF->setContentsMargins(25,25,25,25);
    ui->connectFourContainer->setLayout(gridCF);

    for(int row = 0; row < 6; row++)
    {
        for(int col = 0; col < 7; col++)
        {
            QPushButton* cell = new QPushButton;
            cell->setFixedSize(72,72);
            cell->setCursor(Qt::PointingHandCursor);
            cell->setStyleSheet(
                "background-color: #0f2027;"
                "border-radius: 36px;"
                "border: 2px solid #00eaff;"
                );
            gridCF->addWidget(cell, row, col);
            connectFourCells[row][col] = cell;

            connect(cell, &QPushButton::clicked, this, [=](){
                cell->setStyleSheet(
                    "background-color: #ff073a;"
                    "border-radius: 36px;"
                    "border: 2px solid #ff6b81;"
                    );
            });
        }
    }

    // --- تنظیمات اتلو ---
    QGridLayout* gridOthello = new QGridLayout(ui->othelloContainer);
    gridOthello->setSpacing(6);
    gridOthello->setContentsMargins(20,20,20,20);

    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            QPushButton* cell = new QPushButton(ui->othelloContainer);
            cell->setFixedSize(65,65);
            cell->setObjectName("othelloCell");
            cell->setCursor(Qt::PointingHandCursor);
            gridOthello->addWidget(cell, row, col);
            othelloCells[row][col] = cell;
        }
    }

    othelloCells[3][3]->setStyleSheet("background-color: white; border-radius: 30px;");
    othelloCells[4][4]->setStyleSheet("background-color: white; border-radius: 30px;");
    othelloCells[3][4]->setStyleSheet("background-color: black; border-radius: 30px;");
    othelloCells[4][3]->setStyleSheet("background-color: black; border-radius: 30px;");
}

MainWindow::~MainWindow()
{
    delete userManager;
    delete ui;
}

// --- توابع ورود و ثبت نام ---

void MainWindow::on_btnLogin_clicked()
{
    QString username = ui->txtUsername->text();
    QString password = ui->txtPassword->text();

    if(username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Fields cannot be empty!");
        return;
    }

    User* user = userManager->loginUser(username, password);
    if(user) {
        ui->stackedWidget->setCurrentWidget(ui->pageMainMenu);
    } else {
        QMessageBox::critical(this, "Error", "Invalid username or password!");
    }
}

void MainWindow::on_btnLoginAfterSignup_clicked()
{
    QString name = ui->textname->text();
    QString password = ui->textpass->text();
    QString id = ui->textid->text(); // به عنوان Username
    QString phone = ui->textphone->text();
    QString email = ui->textemail->text();

    if(name.isEmpty() || password.isEmpty() || id.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled!");
        return;
    }

    // 1. اعتبارسنجی پسورد (حداقل 8 کاراکتر)
    if(password.length() < 8) {
        QMessageBox::warning(this, "Error", "Password must be at least 8 characters!");
        return;
    }

    // 2. اعتبارسنجی ایمیل با Regex
    // الگوی استاندارد ایمیل
    QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if(!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid Email Address!");
        return;
    }

    // 3. اعتبارسنجی شماره تلفن با Regex
    // فرض بر این است که شماره باید فقط عدد باشد و مثلا 11 رقم (مثل 0912...)
    // اگر فرمت خاصی مد نظر نیست، فقط چک می‌کنیم که تماما عدد باشد.
    QRegularExpression phoneRegex(R"(^\d{10,11}$)");
    if(!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid Phone Number! (10-11 digits)");
        return;
    }

    // هش کردن پسورد
    QString hashed = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    User newUser(name, id, phone, email, hashed);

    if(userManager->registerUser(newUser)) {
        QMessageBox::information(this, "Success", "Registration successful!");
        ui->stackedWidget->setCurrentWidget(ui->pageLogin);
    } else {
        QMessageBox::warning(this, "Error", "Username already exists!");
    }
}

// --- توابع پروفایل و ویرایش ---

void MainWindow::on_btnProfile_clicked()
{
    User* current = userManager->getCurrentUser();
    if(current) {
        // فرض بر این است که در صفحه EditProfile نام lineEdit ها را می‌دانید
        // مطابق با اسامی که احتمالا در UI گذاشته‌اید (باید چک کنید)
        ui->lineEdit->setText(current->getName());
        ui->lineEdit_4->setText(current->getUsername());
        ui->lineEdit_2->setText(current->getPhone());
        ui->lineEdit_3->setText(current->getEmail());
    }
    ui->stackedWidget->setCurrentWidget(ui->pageEditProfile);
}

void MainWindow::on_SaveEdit_clicked()
{
    User* current = userManager->getCurrentUser();
    if(current) {
        current->setName(ui->lineEdit->text());
        current->setPhone(ui->lineEdit_2->text());
        current->setEmail(ui->lineEdit_3->text());
        userManager->saveUsers(); // ذخیره تغییرات در فایل
        QMessageBox::information(this, "Success", "Profile Updated!");
    }
    ui->stackedWidget->setCurrentWidget(ui->pageMainMenu);
}

// --- توابع ناوبری (Navigation) ---

void MainWindow::on_btnSignUp_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageSignup); }
void MainWindow::on_btnForgot_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageForget); }
void MainWindow::on_btnBack_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageMainMenu); }
void MainWindow::on_btnStartGame_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }
void MainWindow::on_BackformEdit_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageMainMenu); }

void MainWindow::on_btnLogout_clicked()
{
    userManager->logout();
    ui->txtUsername->clear();
    ui->txtPassword->clear();
    ui->stackedWidget->setCurrentWidget(ui->pageLogin);
}

void MainWindow::on_btnConnectFour_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageConnectFourHome); }
void MainWindow::on_btnOthello_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageOthelloHome); }

void MainWindow::on_NewGameOthello_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageOthelloBoard); }
void MainWindow::on_btnBackFromOthello_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }
void MainWindow::on_btnBackFromOtelloHome_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }

void MainWindow::on_NewGameConnectFour_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageConnectFourBoard); }
void MainWindow::on_btnBackFromConnectFour_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }
void MainWindow::on_btnBackFromConnectFourHome_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }

void MainWindow::on_btnLoginAfterForget_clicked()
{
    QString phone = ui->lineEdit_6->text(); // فیلد شماره تلفن در صفحه فراموشی
    QString newPass = ui->lineEdit_7->text(); // فیلد پسورد جدید

    if(phone.isEmpty() || newPass.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields!");
        return;
    }

    if(newPass.length() < 8) {
        QMessageBox::warning(this, "Error", "New password must be at least 8 characters!");
        return;
    }

    if(userManager->resetPassword(phone, newPass)) {
        QMessageBox::information(this, "Success", "Password changed successfully! Please Login.");
        ui->stackedWidget->setCurrentWidget(ui->pageLogin);
    } else {
        QMessageBox::critical(this, "Error", "Phone number not found!");
    }
}
