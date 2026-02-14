#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <QDateTime>

struct GameRecord {
    QString gameName;   // نام بازی (Othello یا Connect4)
    QString opponent;   // نام حریف
    QString result;     // Win, Loss, Draw
    int score;          // امتیاز کسب شده
    QString date;       // تاریخ بازی
};

class User
{
public:
    User();
    User(const QString& name,
         const QString& username,
         const QString& phone,
         const QString& email,
         const QString& hashedPassword);

    QString getName() const;
    QString getUsername() const;
    QString getPhone() const;
    QString getEmail() const;
    QString getHashedPassword() const;
    QList<GameRecord> getHistory() const; // دریافت تاریخچه

    void setName(const QString& name);
    void setUsername(const QString& username);
    void setPhone(const QString& phone);
    void setEmail(const QString& email);
    void setHashedPassword(const QString& hashedPassword);
    void addGameRecord(const GameRecord& record); // افزودن بازی جدید به تاریخچه

private:
    QString name;
    QString username;
    QString phone;
    QString email;
    QString hashedPassword;
    QList<GameRecord> history; // لیست تاریخچه بازی‌ها
};

#endif // USER_H
