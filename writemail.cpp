#include "writemail.h"
#include <QUuid>
#include <QDebug>
#include <QSslConfiguration>
#include <QFileInfo>
#include <QUrl>

writeMail::writeMail(QObject *parent, QByteArray username, QByteArray password)
    : QObject(parent)
    , username(username)
    , password(password)
{
    socket = new QSslSocket(this);
    connect(socket, &QSslSocket::readyRead, this, &writeMail::onReadyRead);
    connect(socket,
            QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this,
            &writeMail::onSslErrors);
}

void writeMail::send(QByteArray receiver, QString subject, QString content)
{
    sendWithAttachments(receiver, subject, content, QStringList());
}

void writeMail::sendWithAttachments(QByteArray receiver,
                                    QString subject,
                                    QString content,
                                    const QStringList &attachments)
{
    this->receiver = receiver;
    this->subject = subject;
    this->content = content;

    // 初始化SSL配置
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    socket->setSslConfiguration(sslConfig);

    // 连接服务器
    socket->connectToHostEncrypted("smtp.163.com", 465);
    if (!socket->waitForConnected(3000)) {
        emit errorOccurred("Connection timeout");
        return;
    }
    if (!socket->waitForEncrypted(3000)) {
        emit errorOccurred("SSL handshake failed: " + socket->errorString());
        return;
    }

    // SMTP协议交互
    sendCommand("EHLO localhost");
    sendCommand("AUTH LOGIN");
    sendCommand(username.toBase64());
    sendCommand(password.toBase64());
    sendCommand("MAIL FROM:<" + username + ">");
    sendCommand("RCPT TO:<" + receiver + ">");
    sendCommand("DATA");

    // 准备MIME邮件
    QString boundary = "BOUNDARY_" + QUuid::createUuid().toString(QUuid::WithoutBraces);

    // 邮件头
    socket->write("From: " + username + "\r\n");
    socket->write("To: " + receiver + "\r\n");
    socket->write("Subject: " + subject.toUtf8() + "\r\n");
    socket->write("MIME-Version: 1.0\r\n");
    socket->write("Content-Type: multipart/mixed; boundary=\"" + boundary.toUtf8() + "\"\r\n\r\n");

    // 正文部分
    socket->write("--" + boundary.toUtf8() + "\r\n");
    socket->write("Content-Type: text/plain; charset=\"utf-8\"\r\n\r\n");
    socket->write(content.toUtf8() + "\r\n\r\n");

    // 附件部分
    foreach (const QString &filePath, attachments) {
        addAttachment(filePath, boundary);
    }
    socket->write("--" + boundary.toUtf8() + "--\r\n");
    sendCommand(".");
    sendCommand("QUIT");
}

void writeMail::addAttachment(const QString &filePath, const QString &boundary)
{
    QFile file(QUrl(filePath).toLocalFile());
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << filePath;
        return;
    }

    // 写入附件头部
    socket->write("--" + boundary.toUtf8() + "\r\n");
    socket->write("Content-Type: application/octet-stream\r\n");
    socket->write("Content-Disposition: attachment; filename=\""
                  + QFileInfo(filePath).fileName().toUtf8() + "\"\r\n");
    socket->write("Content-Transfer-Encoding: base64\r\n\r\n");

    // 分块发送Base64数据
    QByteArray base64Data = file.readAll().toBase64();
    for (int i = 0; i < base64Data.size(); i += 76) {
        socket->write(base64Data.mid(i, 76) + "\r\n");
    }
    // 结束邮件
    socket->write("\r\n");
    file.close();
}

void writeMail::sendCommand(const QByteArray &command)
{
    socket->write(command + "\r\n");
    if (!socket->waitForReadyRead(3000)) {
        emit errorOccurred("Timeout waiting for response to: " + command);
        return;
    }
    QByteArray response = socket->readAll();
    qDebug() << "Command:" << command << "Response:" << response;

    if (response.startsWith("5")) {
        emit errorOccurred("SMTP error: " + response);
    }
}

void writeMail::onReadyRead()
{
    QByteArray response = socket->readAll();
    qDebug() << "Received:" << response;
    // 可以在这里添加更精细的响应处理
}

void writeMail::onSslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors) {
        qDebug() << "SSL Error:" << error.errorString();
    }
    // 生产环境应该验证证书，测试时可忽略
    socket->ignoreSslErrors();
}

//giter&seter
void writeMail::setusername(QByteArray username)
{
    this->username = username;
}
QByteArray writeMail::getusername()
{
    return this->username;
}
void writeMail::setpassword(QByteArray password)
{
    this->password = password;
}
QByteArray writeMail::getpassword()
{
    return this->password;
}
void writeMail::setreceiver(QByteArray receiver)
{
    this->receiver = receiver;
}
QByteArray writeMail::getreceiver()
{
    return this->receiver;
}
void writeMail::setsubject(QString subject)
{
    this->subject = subject;
}
QString writeMail::getsubject()
{
    return this->subject;
}
void writeMail::setcontent(QString content)
{
    this->content = content;
}
QString writeMail::getcontent()
{
    return this->content;
}

void writeMail::setattachments(QStringList attachments)
{
    this->attachments = attachments;
}
QStringList writeMail::getattachments()
{
    return this->attachments;
}
//changesigle

//析构函数
writeMail::~writeMail()
{
    if (socket) {
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState) {
            socket->waitForDisconnected(1000);
        }
        delete socket;
    }
}
