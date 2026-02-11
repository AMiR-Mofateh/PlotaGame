#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        ui->stackedWidget->setCurrentWidget(ui->pageLogin);
}

