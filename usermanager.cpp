#include "usermanager.h"
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QDebug>

UserManager::UserManager() {
    currentUser = nullptr;
    // مسیر فایل جیسون در کنار فایل اجرایی برنامه
    filePath = QCoreApplication::applicationDirPath() + "/users_data.json";
    loadUsers();
}

bool UserManager::registerUser(User newUser) {
    if (isUsernameTaken(newUser.getUsername())) {
        return false; // نام کاربری تکراری است
    }
    users.append(newUser);
    saveUsers(); // بلافاصله ذخیره کن
    return true;
}

User* UserManager::loginUser(QString username, QString password) {
    // پسورد ورودی را هش می‌کنیم تا با دیتابیس مقایسه شود
    QString inputHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    for (int i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == username && users[i].getHashedPassword() == inputHash) {
            currentUser = &users[i]; // ست کردن کاربر فعلی
            return currentUser;
        }
    }
    return nullptr; // پیدا نشد
}

bool UserManager::isUsernameTaken(QString username) {
    for (const User &u : users) {
        if (u.getUsername() == username) return true;
    }
    return false;
}

void UserManager::saveUsers() {
    QJsonArray jsonArray;
    for (const User &u : users) {
        QJsonObject obj;
        obj["name"] = u.getName();
        obj["username"] = u.getUsername();
        obj["phone"] = u.getPhone();
        obj["email"] = u.getEmail();
        obj["password"] = u.getHashedPassword(); // پسورد هش شده ذخیره می‌شود
        jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void UserManager::loadUsers() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray jsonArray = doc.array();

    users.clear();
    for (const auto &val : jsonArray) {
        QJsonObject obj = val.toObject();
        User u(obj["name"].toString(),
               obj["username"].toString(),
               obj["phone"].toString(),
               obj["email"].toString(),
               obj["password"].toString());
        users.append(u);
    }
    file.close();
}

User* UserManager::getCurrentUser() {
    return currentUser;
}

void UserManager::logout() {
    currentUser = nullptr;
}
