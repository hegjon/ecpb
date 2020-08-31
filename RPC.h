#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QAtomicInt>
#include <QVariant>

class RPC: public QObject
{
    Q_OBJECT
public:
    RPC(QObject *parent = nullptr, const QString &hostName = "localhost", quint16 port = 50001);
    ~RPC();
    QVariant call(const QString &method, const QVariantList &params = QVariantList());

private:
    QTcpSocket* socket;
    QAtomicInt counter;
};
