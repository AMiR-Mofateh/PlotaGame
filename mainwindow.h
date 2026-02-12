#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_btnLogin_clicked();
    void on_btnSignUp_clicked();
    void on_btnForgot_clicked();


    void on_btnLogout_clicked();

    void on_btnConnectFour_clicked();
    //void on_btnOthello_clicked();
    void on_btnBack_clicked();


    void on_btnStartGame_clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton* connectFourCells[6][7];

};
#endif // MAINWINDOW_H
