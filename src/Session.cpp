#include "Session.h"

#include <QtDebug>
#include <QThread>
#include <QTcpSocket>


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

void Session::run()
{
    qInfo() << "Begin" << this << "on" << QThread::currentThread();
    //this->moveToThread(QThread::currentThread());

    QTcpSocket socket(nullptr);

    socket.connectToHost(args->hostname(), args->port());

    if(socket.waitForConnected(1000)) {
        qDebug().noquote().nospace() << "Connected to " << args->hostname() << ":" << args->port();
    } else {
        qCritical() << "Could not connect to" << args->hostname();
    }


    RPC rpc = RPC(&socket, nullptr);
    rpc.setObjectName("RPC session" + this->objectName());
    connect(&rpc, &RPC::result, this, &Session::result, Qt::DirectConnection);
    connect(&rpc, &RPC::error, this, &Session::error, Qt::DirectConnection);


    SessionResult result;
    result.start = QDateTime::currentDateTime();

    rpc.send("server.version");
    result.requests += 1;

    for(int i = 0; i < requests/3; i++) {
        int j = i % args->address().count();
        QString address = args->address()[j];

        rpc.send("blockchain.address.get_balance", QVariantList() << address);
        rpc.send("blockchain.address.listunspent", QVariantList() << address);
        rpc.send("blockchain.address.get_history", QVariantList() << address);

        result.requests += 3;
        socket.waitForReadyRead(500);
    }

    while(result.requests > responses) {
        qDebug() << "Waiting. reqests:" << result.requests << "responses:" << responses;
        socket.waitForReadyRead(500);
    }

    result.end = QDateTime::currentDateTime();

    emit resultReady(result);

    qInfo() << "End" << this << "on" << QThread::currentThread();
}

void Session::result(QString id, QVariant result)
{
    Q_UNUSED(result);
    responses++;
    qInfo() << "Session::result" << id;
}

void Session::error(QString id, QVariant error)
{
    responses++;
    qWarning() << "Session::error" << id << error;
}

