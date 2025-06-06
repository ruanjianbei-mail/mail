#ifndef MAILMODEL_H
#define MAILMODEL_H

#include <QAbstractListModel>
#include <QVariantMap>

class MailModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString box READ box WRITE setBox NOTIFY boxChanged)
public:
    enum MailRoles {
        IdRole = Qt::UserRole + 1,
        SubjectRole,
        SenderRole,
        ReceiverRole,
        TimeRole,
        StarredRole,
        ContentRole
    };

    explicit MailModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int userId() const;
    void setUserId(int id);

    QString box() const;
    void setBox(const QString &box);

    Q_INVOKABLE void refresh();

signals:
    void userIdChanged();
    void boxChanged();

private:
    int m_userId = -1;
    QString m_box = "Inbox";
    QList<QVariantMap> m_mails;
};

#endif // MAILMODEL_H
