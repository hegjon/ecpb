#include "Session.h"
#include "RPC.h"

#include <QtDebug>
#include <QRunnable>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QThread>


Session::Session(Arguments* args, QObject* parent): QObject(parent), QRunnable()
{
    this->args = args;
}

void Session::run() {
    qInfo() << qSetFieldWidth(5) << "Begin" << 0 << qSetFieldWidth(0) << this << QThread::currentThread();

    RPC rpc(nullptr, "192.168.128.3", 50001);
    QString result;
    QVariant serverVersion = rpc.call("server.version");
    qInfo() << "Server version:" << serverVersion;

    rpc.call("blockchain.address.get_balance", QVariantList() << "bitcoincash:qqfc3lxxylme0w87c5j2wdmsqln6e844xcmsdssvzy");

    for(int i = 0; i < 10; i++) {
        QVariant result = rpc.call("server.ping");
        result = "OK " + QString::number(i);
    }


    qInfo().noquote() << "Result:" << result;

    qInfo() << qSetFieldWidth(5) << "End" << 0 << qSetFieldWidth(0) << this;

    emit resultReady(0, result);
}
