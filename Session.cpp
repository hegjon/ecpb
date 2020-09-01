#include "Session.h"
#include "RPC.h"

#include <QtDebug>
#include <QRunnable>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QThread>


Session::Session(QObject* parent): QObject(parent), QRunnable()
{
}

void Session::setArguments(Arguments *args)
{
    this->args = args;
}

void Session::setRequests(int requests)
{
    this->requests = requests;
}

void Session::run() {
    qInfo() << "Begin" << this << "on" << QThread::currentThread();

    RPC rpc(nullptr, "192.168.128.3", 50001);
    rpc.setObjectName("RPC session" + this->objectName());
    QString result;
    QVariant serverVersion = rpc.call("server.version");
    qDebug() << "Server version:" << serverVersion;

    for(int i = 0; i < requests/3; i++) {
        int j = i % args->address().count();
        QString address = args->address()[j];
        rpc.call("blockchain.address.get_balance", QVariantList() << address);
        rpc.call("blockchain.address.listunspent", QVariantList() << address);
        rpc.call("blockchain.address.get_history", QVariantList() << address);
    }

    qInfo() << "End" << this << "on" << QThread::currentThread();

    emit resultReady((int) 0, result);
}
