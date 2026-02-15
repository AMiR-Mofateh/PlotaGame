#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGridLayout>
#include <QCryptographicHash>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. مقداردهی اولیه سیستم‌ها
    userManager = new UserManager();
    c4Game = new ConnectFourGame();
    othelloGame = new OthelloGame();

    // 2. تنظیم استایل کلی برنامه
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
    QFrame, QWidget#connectFourContainer, QWidget#othelloContainer {
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

    // تنظیم صفحه پیش‌فرض
    ui->stackedWidget->setCurrentWidget(ui->pageLogin);

    // --- تنظیمات بورد دوز 4تایی (Connect Four) ---
    QGridLayout* gridCF = new QGridLayout;
    gridCF->setSpacing(10);
    gridCF->setContentsMargins(20,20,20,20);
    ui->connectFourContainer->setLayout(gridCF);

    for(int row = 0; row < 6; row++)
    {
        for(int col = 0; col < 7; col++)
        {
            QPushButton* cell = new QPushButton;
            cell->setFixedSize(70,70);
            cell->setCursor(Qt::PointingHandCursor);
            // استایل اولیه
            cell->setStyleSheet(
                "background-color: #0f2027;"
                "border-radius: 35px;"
                "border: 2px solid #00eaff;"
                );
            gridCF->addWidget(cell, row, col);
            connectFourCells[row][col] = cell;

            // اتصال سیگنال کلیک به منطق بازی
            // نکته: col با مقدار کپی می‌شود (by value)
            connect(cell, &QPushButton::clicked, this, [=](){
                handleConnectFourClick(col);
            });
        }
    }

    // --- تنظیمات بورد اتلو (Othello) ---
    QGridLayout* gridOthello = new QGridLayout(ui->othelloContainer);
    gridOthello->setSpacing(5);
    gridOthello->setContentsMargins(15,15,15,15);

    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            QPushButton* cell = new QPushButton(ui->othelloContainer);
            cell->setFixedSize(60,60);
            cell->setCursor(Qt::PointingHandCursor);
            gridOthello->addWidget(cell, row, col);
            othelloCells[row][col] = cell;

            // اتصال سیگنال کلیک
            connect(cell, &QPushButton::clicked, this, [=](){
                handleOthelloClick(row, col);
            });
        }
    }
    // نکته: رنگ‌دهی اولیه اتلو حالا توسط updateOthelloUI انجام می‌شود، نه دستی در اینجا.
}

MainWindow::~MainWindow()
{
    delete userManager;
    delete c4Game;
    delete othelloGame;
    delete ui;
}

// ==========================================
//           بخش منطق بازی‌ها
// ==========================================

// --- Connect Four Functions ---

void MainWindow::on_NewGameConnectFour_clicked() {
    c4Game->reset();
    updateConnectFourUI();
    ui->stackedWidget->setCurrentWidget(ui->pageConnectFourBoard);
}

void MainWindow::handleConnectFourClick(int col) {
    // تلاش برای انداختن مهره
    int row = c4Game->dropPiece(col);

    if (row != -1) { // حرکت موفق بود
        updateConnectFourUI();

        // بررسی وضعیت پایان بازی
        if (c4Game->checkWin(row, col)) {
            QString winner = (c4Game->getCurrentPlayer() == 1) ? "Yellow (Player 1)" : "Red (Player 2)";
            QMessageBox::information(this, "Game Over", winner + " Wins!");
            // TODO: ذخیره در تاریخچه کاربر
            ui->stackedWidget->setCurrentWidget(ui->pageConnectFourHome);
        }
        else if (c4Game->isFull()) {
            QMessageBox::information(this, "Game Over", "Draw! No winner.");
            ui->stackedWidget->setCurrentWidget(ui->pageConnectFourHome);
        }
        else {
            c4Game->switchTurn(); // تغییر نوبت
        }
    }
}

void MainWindow::updateConnectFourUI() {
    for(int r=0; r<6; r++) {
        for(int c=0; c<7; c++) {
            int val = c4Game->getCell(r, c);
            QString style;
            if (val == 0) { // خالی
                style = "background-color: #0f2027; border-radius: 35px; border: 2px solid #00eaff;";
            } else if (val == 1) { // بازیکن 1 (زرد)
                style = "background-color: #f1c40f; border-radius: 35px; border: 2px solid #fff;";
            } else { // بازیکن 2 (قرمز)
                style = "background-color: #e74c3c; border-radius: 35px; border: 2px solid #fff;";
            }
            connectFourCells[r][c]->setStyleSheet(style);
        }
    }
}

// --- Othello Functions ---

void MainWindow::on_NewGameOthello_clicked() {
    othelloGame->reset();
    updateOthelloUI();
    ui->stackedWidget->setCurrentWidget(ui->pageOthelloBoard);
}

void MainWindow::handleOthelloClick(int row, int col) {
    // انجام حرکت
    if (othelloGame->makeMove(row, col)) {
        othelloGame->switchTurn();
        updateOthelloUI();

        // بررسی نوبت نفر بعدی (پاس دادن یا پایان بازی)
        int nextPlayer = othelloGame->getCurrentPlayer();
        if (!othelloGame->hasValidMove(nextPlayer)) {
            QMessageBox::information(this, "Pass", "No valid moves for current player. Turn passes.");
            othelloGame->switchTurn();

            // بررسی مجدد: اگر نفر قبلی هم حرکت نداشت -> پایان بازی
            if (!othelloGame->hasValidMove(othelloGame->getCurrentPlayer())) {
                int s1 = othelloGame->getScore(1); // سیاه
                int s2 = othelloGame->getScore(2); // سفید
                QString msg = QString("Game Over!\nBlack: %1\nWhite: %2\n").arg(s1).arg(s2);

                if (s1 > s2) msg += "Winner: Black";
                else if (s2 > s1) msg += "Winner: White";
                else msg += "Result: Draw";

                QMessageBox::information(this, "Result", msg);
                // TODO: ذخیره در تاریخچه
                ui->stackedWidget->setCurrentWidget(ui->pageOthelloHome);
            }
        }
        updateOthelloUI(); // آپدیت نهایی برای نمایش راهنمای حرکت
    }
}

void MainWindow::updateOthelloUI() {
    int currentP = othelloGame->getCurrentPlayer();
    for(int r=0; r<8; r++) {
        for(int c=0; c<8; c++) {
            int val = othelloGame->getCell(r, c);
            QString style;
            QString base = "border-radius: 30px;";

            if (val == 1) { // سیاه
                style = "background-color: black; border: 2px solid #555; " + base;
            } else if (val == 2) { // سفید
                style = "background-color: white; border: 2px solid #ccc; " + base;
            } else {
                if (othelloGame->isValidMove(r, c, currentP)) {
                    style = "background-color: rgba(0, 234, 255, 0.2); border: 1px dashed #00eaff; " + base;
                } else {
                    style = "background-color: transparent; border: 1px solid #2f3640; " + base;
                }
            }
            // دقت کنید نام آرایه در اینجا باید othelloCells باشد:
            othelloCells[r][c]->setStyleSheet(style);
        }
    }
}

// ==========================================
//        بخش مدیریت کاربران (بدون تغییر)
// ==========================================

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
        //ui->lblWelcome->setText("Welcome " + user->getName()); // نمایش نام کاربر
        ui->stackedWidget->setCurrentWidget(ui->pageMainMenu);
    } else {
        QMessageBox::critical(this, "Error", "Invalid username or password!");
    }
}

void MainWindow::on_btnLoginAfterSignup_clicked()
{
    QString name = ui->textname->text();
    QString password = ui->textpass->text();
    QString id = ui->textid->text(); // Username
    QString phone = ui->textphone->text();
    QString email = ui->textemail->text();

    if(name.isEmpty() || password.isEmpty() || id.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be filled!");
        return;
    }

    if(password.length() < 8) {
        QMessageBox::warning(this, "Error", "Password must be at least 8 characters!");
        return;
    }

    QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if(!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid Email Address!");
        return;
    }

    QRegularExpression phoneRegex(R"(^\d{10,11}$)");
    if(!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Error", "Invalid Phone Number! (10-11 digits)");
        return;
    }

    QString hashed = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    User newUser(name, id, phone, email, hashed);

    if(userManager->registerUser(newUser)) {
        QMessageBox::information(this, "Success", "Registration successful!");
        ui->stackedWidget->setCurrentWidget(ui->pageLogin);
    } else {
        QMessageBox::warning(this, "Error", "Username already exists!");
    }
}

void MainWindow::on_btnProfile_clicked()
{
    User* current = userManager->getCurrentUser();
    if(current) {
        ui->lineEdit->setText(current->getName());
        ui->lineEdit_4->setText(current->getUsername());
        ui->lineEdit_4->setReadOnly(true); // نام کاربری معمولا تغییر نمیکند
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

        // تغییر رمز عبور اگر فیلد پر شده باشد
        QString newPass = ui->lineEdit_5->text();
        if(!newPass.isEmpty()) {
            if(newPass.length() < 8) {
                QMessageBox::warning(this, "Error", "New Password must be at least 8 chars!");
                return;
            }
            QString hashed = QString(QCryptographicHash::hash(newPass.toUtf8(), QCryptographicHash::Sha256).toHex());
            current->setHashedPassword(hashed);
        }

        userManager->saveUsers();
        QMessageBox::information(this, "Success", "Profile Updated!");
    }
    ui->stackedWidget->setCurrentWidget(ui->pageMainMenu);
}

void MainWindow::on_btnLoginAfterForget_clicked()
{
    QString phone = ui->lineEdit_6->text();
    QString newPass = ui->lineEdit_7->text();

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

void MainWindow::on_btnLogout_clicked()
{
    userManager->logout();
    ui->txtUsername->clear();
    ui->txtPassword->clear();
    ui->stackedWidget->setCurrentWidget(ui->pageLogin);
}

// --- توابع ساده ناوبری ---
void MainWindow::on_btnSignUp_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageSignup); }
void MainWindow::on_btnForgot_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageForget); }
void MainWindow::on_btnBack_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageMainMenu); }
void MainWindow::on_btnStartGame_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }
void MainWindow::on_BackformEdit_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageMainMenu); }
void MainWindow::on_btnConnectFour_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageConnectFourHome); }
void MainWindow::on_btnOthello_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageOthelloHome); }
void MainWindow::on_btnBackFromOthello_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }
void MainWindow::on_btnBackFromOtelloHome_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }
void MainWindow::on_btnBackFromConnectFour_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }
void MainWindow::on_btnBackFromConnectFourHome_clicked() { ui->stackedWidget->setCurrentWidget(ui->pageGameSelection); }
