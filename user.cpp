#include "user.h"

User::User() {}

User::User(const QString& name,
           const QString& username,
           const QString& phone,
           const QString& email,
           const QString& hashedPassword)
    : name(name),
    username(username),
    phone(phone),
    email(email),
    hashedPassword(hashedPassword)
{}

QString User::getName() const { return name; }
QString User::getUsername() const { return username; }
QString User::getPhone() const { return phone; }
QString User::getEmail() const { return email; }
QString User::getHashedPassword() const { return hashedPassword; }
QList<GameRecord> User::getHistory() const { return history; }

void User::setName(const QString& name) { this->name = name; }
void User::setUsername(const QString& username) { this->username = username; }
void User::setPhone(const QString& phone) { this->phone = phone; }
void User::setEmail(const QString& email) { this->email = email; }
void User::setHashedPassword(const QString& hashedPassword) { this->hashedPassword = hashedPassword; }

void User::addGameRecord(const GameRecord& record) {
    history.append(record);
}
