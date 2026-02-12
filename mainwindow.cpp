#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


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

            // سوراخ نئون فضایی
            cell->setStyleSheet(
                "background-color: #0f2027;"
                "border-radius: 36px;"
                "border: 2px solid #00eaff;"
                );

            gridCF->addWidget(cell, row, col);
            connectFourCells[row][col] = cell;

            // تست کلیک - مهره قرمز نئون
            connect(cell, &QPushButton::clicked, this, [=](){
                cell->setStyleSheet(
                    "background-color: #ff073a;"
                    "border-radius: 36px;"
                    "border: 2px solid #ff6b81;"
                    );
            });
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnLogin_clicked()
{
    QString username = ui->txtUsername->text();
    QString password = ui->txtPassword->text();

    if(username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Fields cannot be empty!");
        return;
    }

    ui->stackedWidget->setCurrentWidget(ui->pageMainMenu);
}


void MainWindow::on_btnSignUp_clicked()
{
    QMessageBox::information(this, "Info", "Sign Up page will be implemented.");
}

void MainWindow::on_btnForgot_clicked()
{
    QMessageBox::information(this, "Info", "Password recovery will be implemented.");
}

void MainWindow::on_btnLogout_clicked()
{
    ui->txtUsername->clear();
    ui->txtPassword->clear();
    ui->stackedWidget->setCurrentWidget(ui->pageLogin);
}

void MainWindow::on_btnConnectFour_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageConnectFourBoard);
}

// void MainWindow::on_btnOthello_clicked()
// {
//     ui->stackedWidget->setCurrentWidget(ui->pageOthelloBoard);
// }

void MainWindow::on_btnBack_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageMainMenu);
}



void MainWindow::on_btnStartGame_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageGameSelection);
}

