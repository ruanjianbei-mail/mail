#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "writemail.h"
#include "mailuser.h"
#include "MailModel.h"
#include "DatabaseManager.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<writeMail>("writeMail", 1, 0, "WriteMail");
    qmlRegisterType<mailUser>("mailUser", 1, 0, "MailUser");

    DatabaseManager::instance();

    //register MailModel for QML
    qmlRegisterType<MailModel>("Mail", 1, 0, "MailModel");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
