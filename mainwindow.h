#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>
#include "usermanager.h"
#include "connectfourgame.h"
#include "othellogame.h"

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

    void on_btnOthello_clicked();

    void on_btnBackFromOthello_clicked();

    void on_btnBackFromConnectFour_clicked();

    void on_btnLoginAfterSignup_clicked();

    void on_btnLoginAfterForget_clicked();



    void on_NewGameOthello_clicked();

    void on_btnBackFromOtelloHome_clicked();

    void on_NewGameConnectFour_clicked();

    void on_btnBackFromConnectFourHome_clicked();

    void on_btnProfile_clicked();

    void on_BackformEdit_clicked();

    void on_SaveEdit_clicked();

    void updateHistoryTables(); // تابع کلی برای بروزرسانی جداول
    void saveGameResult(QString gameName, QString opponent, QString role, QString result, int score);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    UserManager *userManager;
    QPushButton* connectFourCells[6][7];
    QPushButton* othelloCells[8][8];
    void updateProfilePage();

    // اضافه کردن شیء های بازی
    ConnectFourGame *c4Game;
    OthelloGame *othelloGame;


    // توابع کمکی برای بروزرسانی گرافیک
    void updateConnectFourUI();
    void updateOthelloUI();
    void handleConnectFourClick(int col); // فقط ستون مهم است
    void handleOthelloClick(int row, int col);

};
#endif // MAINWINDOW_H
