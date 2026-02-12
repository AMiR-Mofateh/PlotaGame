#ifndef USER_H
#define USER_H

#include <QString>

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

    void setName(const QString& name);
    void setUsername(const QString& username);
    void setPhone(const QString& phone);
    void setEmail(const QString& email);
    void setHashedPassword(const QString& hashedPassword);

private:
    QString name;
    QString username;
    QString phone;
    QString email;
    QString hashedPassword;
};

#endif
