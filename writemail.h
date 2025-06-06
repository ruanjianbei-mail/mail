#ifndef WRITEMAIL_H
#define WRITEMAIL_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QSslSocket>

class writeMail : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray username READ getusername WRITE setusername NOTIFY usernameChanged)
    Q_PROPERTY(QByteArray password READ getpassword WRITE setpassword NOTIFY passwordChanged)
    Q_PROPERTY(QByteArray receiver READ getreceiver WRITE setreceiver NOTIFY receiverChanged)
    Q_PROPERTY(QString subject READ getsubject WRITE setsubject)
    Q_PROPERTY(QString content READ getcontent WRITE setcontent)
    Q_PROPERTY(QStringList attachments READ getattachments WRITE setattachments)

public:
    explicit writeMail(QObject *parent = nullptr,
                       QByteArray username = QByteArray(),
                       QByteArray password = QByteArray());
    ~writeMail();
public slots:
    void send(QByteArray receiver, QString subject, QString content);
    void sendWithAttachments(QByteArray receiver,
                             QString subject,
                             QString content,
                             const QStringList &attachments);

    Q_INVOKABLE void setusername(QByteArray username);
    Q_INVOKABLE QByteArray getusername();
    Q_INVOKABLE void setpassword(QByteArray password);
    Q_INVOKABLE QByteArray getpassword();
    Q_INVOKABLE void setreceiver(QByteArray receiver);
    Q_INVOKABLE QByteArray getreceiver();
    Q_INVOKABLE void setsubject(QString subject);
    Q_INVOKABLE QString getsubject();
    Q_INVOKABLE void setcontent(QString content);
    Q_INVOKABLE QString getcontent();
    Q_INVOKABLE void setattachments(QStringList attachments);
    Q_INVOKABLE QStringList getattachments();
signals:
    void errorOccurred(const QString &message);
    void finished();
signals:
    void usernameChanged();
    void passwordChanged();
    void receiverChanged();

private:
    void addAttachment(const QString &filePath, const QString &boundary);
    void sendCommand(const QByteArray &command);
    void onReadyRead();
    void onSslErrors(const QList<QSslError> &errors);

    QSslSocket *socket;
    QByteArray username;
    QByteArray password;
    QByteArray receiver;
    QString subject;
    QString content;
    QStringList attachments;
};

#endif
