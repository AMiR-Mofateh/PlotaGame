#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_Start_clicked();
    void on_Exit_clicked();

    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

private:

    QMap<QTcpSocket*, QString> onlineUsers;

    void broadcastUserList();

    void loadUsers();
    void saveUsers();
    QString usersFilePath = "users.json";

    bool checkLoginCredentials(QString username, QString password);

    void sendJson(QTcpSocket *socket, QJsonObject jsonObj);
    void stopServer();

    Ui::MainWindow *ui;
    QTcpServer *tcpServer;

    bool serverRunning = false;
    QList<QTcpSocket*> clients;

    QMap<QString, QString> userDatabase;

};
#endif // MAINWINDOW_H
