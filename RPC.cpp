#include "RPC.h"

#include <QEventLoop>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <QObject>
#include <QAtomicInt>

RPC::RPC(QObject *parent, const QString &hostName, quint16 port) : QObject(parent)
{
    counter = QAtomicInt(0);
    qInfo() << "Connecting";

    socket = new QTcpSocket(this);

    socket->connectToHost(hostName, port);

    if(socket->waitForConnected(1000)) {
        qInfo() << "Connected";
    } else {
        qInfo() << "Not connected";
    }


    qInfo() << "Done";
}

RPC::~RPC()
{
    socket->disconnectFromHost();
}

QVariant RPC::call(const QString &method, const QVariantList &params)
{
    QJsonObject json = QJsonObject();
    json["id"] = counter++;
    json["jsonrpc"] = "2.0";
    json["method"] = method;
    //json.insert("params", QJsonArray::fromVariantList(params));
    json["params"] = QJsonArray::fromVariantList(params);

    const auto format = QJsonDocument::JsonFormat::Compact;
    const QByteArray data = QJsonDocument(json).toJson(format);

    qDebug().noquote() << "Sending:" << data;
    socket->write(data);
    socket->write("\n");
    socket->flush();

    QEventLoop wait(this);
    connect(socket, &QTcpSocket::readyRead, &wait, &QEventLoop::quit);

    wait.exec();

    QByteArray result = socket->readLine();
    qDebug().noquote() << "Result:" << result;
    QJsonDocument json2 = QJsonDocument::fromJson(result);

    return json2.object().value("result");
}
