#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->pageLogin);

    QGridLayout* gridCF = new QGridLayout;
    gridCF->setSpacing(12);
    gridCF->setContentsMargins(25,25,25,25);
    ui->connectFourContainer->setLayout(gridCF);

    /* ====== 🪐 CREATE CELLS ====== */
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

