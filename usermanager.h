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

    // توابع اصلی
    bool registerUser(User newUser);
    User* loginUser(QString username, QString password);
    void saveUsers();  // ذخیره در فایل
    void loadUsers();  // خواندن از فایل
    bool isUsernameTaken(QString username);

    // نگهداری کاربر فعلی که لاگین کرده
    User* getCurrentUser();
    void logout();

private:
    QList<User> users;
    User* currentUser; // اشاره‌گر به کاربری که الان داخل سیستم
    QString filePath;
};

#endif // USERMANAGER_H
