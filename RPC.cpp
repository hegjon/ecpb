#include "RPC.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <QObject>
#include <QUuid>
#include <QDebug>

RPC::RPC(QAbstractSocket *device, QObject *parent) : QObject(parent)
{
    this->socket = device;
    connect(socket, &QAbstractSocket::readyRead, this, &RPC::readyRead, Qt::DirectConnection);
}

RPC::~RPC()
{
}

QString RPC::send(const QString &method, const QVariantList &params)
{
    if(!socket->isOpen()) {
        qWarning() << "Socket is not open!";
        return QString();
    }

    if(!socket->isWritable()) {
        qWarning() << "Socket is not writable!";
        return QString();
    }

    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QJsonObject json = QJsonObject();
    json["id"] = id;
    json["jsonrpc"] = "2.0";
    json["method"] = method;
    json["params"] = QJsonArray::fromVariantList(params);

    const auto format = QJsonDocument::JsonFormat::Compact;
    const QByteArray data = QJsonDocument(json).toJson(format);

    qDebug().noquote() << "Writing:" << data;
    socket->write(data);
    socket->write("\n");
    socket->flush();

    if(!socket->waitForBytesWritten(2000)) {
        qWarning() << "Could not write to" << socket;
    }
    socket->waitForReadyRead(2000);

    return id;
}

void RPC::readyRead()
{
    QByteArray all = socket->readAll();

    qDebug() << "Ready read, size:" << all.length()
             << "Newline?" << all.contains("\n");

    buffer.append(all);

    int index = buffer.indexOf("\n");
    if(index == -1) {
        qDebug() << "No newline, only appending to buffer";
        return;
    }
    QByteArray line = buffer.left(index);
    buffer = buffer.mid(index+1);

    if(line.length() == 0) {
        return;
    }

    qDebug().noquote() << "Reading:" << line.trimmed().length() << "bytes";
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(line, &parseError);

    if(parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse:" << line << parseError.errorString();
    }

    if(!doc.isObject()) {
        qWarning() << "Result is not object" << doc;
        return;
    }

    QJsonObject response = doc.object();

    if(!response.contains("id")) {
        qWarning() << "Missing id";
        return;
    }


    QString id = response.value("id").toString();

    if(response.contains("error")) {
        qWarning() << "Server returned error:"
                   << response.value("error");

        emit error(id, response.value("error").toVariant());
        return;
    }

    if(response.contains("result")) {
        qDebug() << "XXXX Emit result";
        emit result(id, response.value("result").toVariant());
        return;
    }

    qWarning() << "Object is missing error or result";
}
