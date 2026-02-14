#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>
#include <QList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include "user.h"

class UserManager
{
public:
    UserManager();

    bool registerUser(User newUser);
    User* loginUser(QString username, QString password);
    bool resetPassword(QString phone, QString newPassword); // تابع جدید برای فراموشی رمز

    void saveUsers();
    void loadUsers();
    bool isUsernameTaken(QString username);

    User* getCurrentUser();
    void logout();

private:
    QList<User> users;
    User* currentUser;
    QString filePath;
};

#endif // USERMANAGER_H
