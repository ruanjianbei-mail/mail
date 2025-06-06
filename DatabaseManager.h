#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QVariantMap>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    static DatabaseManager* instance();

    bool addUser(const QString &email, const QString &password);
    int getUserId(const QString &email, const QString &password);

    bool addMail(int userId, const QString &box, const QString &subject,
                 const QString &sender, const QString &receiver,
                 const QString &time, int starred, const QString &content);

    QList<QVariantMap> getMails(int userId, const QString &box);

private:
    void initDatabase();
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
