#include "MailModel.h"
#include "DatabaseManager.h"

MailModel::MailModel(QObject *parent) : QAbstractListModel(parent)
{
    refresh();
}

int MailModel::rowCount(const QModelIndex &) const
{
    return m_mails.size();
}

QVariant MailModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_mails.size())
        return QVariant();

    const QVariantMap &mail = m_mails.at(index.row());
    switch (role) {
        case IdRole: return mail.value("id");
        case SubjectRole: return mail.value("subject");
        case SenderRole: return mail.value("sender");
        case ReceiverRole: return mail.value("receiver");
        case TimeRole: return mail.value("time");
        case StarredRole: return mail.value("starred");
        case ContentRole: return mail.value("content");
        default: return QVariant();
    }
}

QHash<int, QByteArray> MailModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {SubjectRole, "subject"},
        {SenderRole, "sender"},
        {ReceiverRole, "receiver"},
        {TimeRole, "time"},
        {StarredRole, "starred"},
        {ContentRole, "content"}
    };
}

int MailModel::userId() const { return m_userId; }
void MailModel::setUserId(int id)
{
    if (m_userId != id) {
        m_userId = id;
        emit userIdChanged();
        refresh();
    }
}

QString MailModel::box() const { return m_box; }
void MailModel::setBox(const QString &box)
{
    if (m_box != box) {
        m_box = box;
        emit boxChanged();
        refresh();
    }
}

void MailModel::refresh()
{
    beginResetModel();
    if (m_userId > 0 && !m_box.isEmpty())
        m_mails = DatabaseManager::instance()->getMails(m_userId, m_box);
    else
        m_mails.clear();
    endResetModel();
}
