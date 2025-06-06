#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    initDatabase();
}

DatabaseManager* DatabaseManager::instance()
{
    static DatabaseManager inst;
    return &inst;
}

void DatabaseManager::initDatabase()
{
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dbPath);
    QString fullPath = dbPath + "/maildata.db";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fullPath);
    if (!db.open()) {
        qWarning() << "无法打开数据库:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS User ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "email TEXT NOT NULL,"
        "password TEXT NOT NULL)")) {
        qWarning() << "创建User表失败:" << query.lastError().text();
        }

        if (!query.exec("CREATE TABLE IF NOT EXISTS Mail ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "user_id INTEGER,"
            "box TEXT,"
            "subject TEXT,"
            "sender TEXT,"
            "receiver TEXT,"
            "time TEXT,"
            "starred INTEGER,"
            "content TEXT,"
            "FOREIGN KEY(user_id) REFERENCES User(id))")) {
            qWarning() << "创建Mail表失败:" << query.lastError().text();
            }
}

bool DatabaseManager::addUser(const QString &email, const QString &password)
{
    QSqlQuery query;
    // 密码哈希存储
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
    query.prepare("INSERT INTO User (email, password) VALUES (?, ?)");
    query.addBindValue(email);
    query.addBindValue(QString(hash));
    return query.exec();
}

int DatabaseManager::getUserId(const QString &email, const QString &password)
{
    QSqlQuery query;
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
    query.prepare("SELECT id FROM User WHERE email=? AND password=?");
    query.addBindValue(email);
    query.addBindValue(QString(hash));
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool DatabaseManager::addMail(int userId, const QString &box, const QString &subject,
                              const QString &sender, const QString &receiver,
                              const QString &time, int starred, const QString &content)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Mail (user_id, box, subject, sender, receiver, time, starred, content) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(box);
    query.addBindValue(subject);
    query.addBindValue(sender);
    query.addBindValue(receiver);
    query.addBindValue(time);
    query.addBindValue(starred);
    query.addBindValue(content);
    return query.exec();
}

QList<QVariantMap> DatabaseManager::getMails(int userId, const QString &box)
{
    QList<QVariantMap> mails;
    QSqlQuery query;
    query.prepare("SELECT * FROM Mail WHERE user_id=? AND box=? ORDER BY time DESC");
    query.addBindValue(userId);
    query.addBindValue(box);
    if (query.exec()) {
        while (query.next()) {
            QVariantMap mail;
            mail["id"] = query.value("id");
            mail["subject"] = query.value("subject");
            mail["sender"] = query.value("sender");
            mail["receiver"] = query.value("receiver");
            mail["time"] = query.value("time");
            mail["starred"] = query.value("starred");
            mail["content"] = query.value("content");
            mails.append(mail);
        }
    }
    return mails;
}
