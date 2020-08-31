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
    qInfo() << qSetFieldWidth(5) << "Begin" << 0 << qSetFieldWidth(0) << this << QThread::currentThread();

    RPC rpc(nullptr, "192.168.128.3", 50001);
    QString result;
    QVariant serverVersion = rpc.call("server.version");
    qInfo() << "Server version:" << serverVersion;



    for(int i = 0; i < requests; i++) {
        int j = i % args->address().count();
        QString address = args->address()[j];
        rpc.call("blockchain.address.get_balance", QVariantList() << address);
        rpc.call("blockchain.address.listunspent", QVariantList() << address);
    }


    qInfo().noquote() << "Result:" << result;

    qInfo() << qSetFieldWidth(5) << "End" << 0 << qSetFieldWidth(0) << this;

    emit resultReady(0, result);
}
