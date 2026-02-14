#include "usermanager.h"
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QDebug>

UserManager::UserManager() {
    currentUser = nullptr;
    filePath = QCoreApplication::applicationDirPath() + "/users_data.json";
    loadUsers();
}

bool UserManager::registerUser(User newUser) {
    if (isUsernameTaken(newUser.getUsername())) {
        return false;
    }
    users.append(newUser);
    saveUsers();
    return true;
}

User* UserManager::loginUser(QString username, QString password) {
    QString inputHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    for (int i = 0; i < users.size(); ++i) {
        if (users[i].getUsername() == username && users[i].getHashedPassword() == inputHash) {
            currentUser = &users[i];
            return currentUser;
        }
    }
    return nullptr;
}

// پیاده‌سازی تابع فراموشی رمز عبور
bool UserManager::resetPassword(QString phone, QString newPassword) {
    QString newHash = QString(QCryptographicHash::hash(newPassword.toUtf8(), QCryptographicHash::Sha256).toHex());
    bool found = false;

    // جستجو بر اساس شماره تلفن
    for (int i = 0; i < users.size(); ++i) {
        if (users[i].getPhone() == phone) {
            users[i].setHashedPassword(newHash);
            found = true;
            break;
        }
    }

    if (found) {
        saveUsers(); // ذخیره تغییرات
        return true;
    }
    return false;
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
        obj["password"] = u.getHashedPassword();

        // ذخیره تاریخچه بازی‌ها
        QJsonArray histArray;
        for (const GameRecord &rec : u.getHistory()) {
            QJsonObject hObj;
            hObj["game"] = rec.gameName;
            hObj["opponent"] = rec.opponent;
            hObj["result"] = rec.result;
            hObj["score"] = rec.score;
            hObj["date"] = rec.date;
            histArray.append(hObj);
        }
        obj["history"] = histArray;

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

        // لود کردن تاریخچه
        if (obj.contains("history")) {
            QJsonArray histArray = obj["history"].toArray();
            for (const auto &hVal : histArray) {
                QJsonObject hObj = hVal.toObject();
                GameRecord rec;
                rec.gameName = hObj["game"].toString();
                rec.opponent = hObj["opponent"].toString();
                rec.result = hObj["result"].toString();
                rec.score = hObj["score"].toInt();
                rec.date = hObj["date"].toString();
                u.addGameRecord(rec);
            }
        }

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
