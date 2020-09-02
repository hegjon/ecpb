#pragma once

#include <QObject>
#include <QAbstractSocket>
#include <QVariant>

class RPC: public QObject
{
    Q_OBJECT

    QIODevice* socket;
    QByteArray buffer;

public:
    RPC(QIODevice *device, QObject *parent = nullptr);
    ~RPC();
    QString send(const QString &method, const QVariantList &params = QVariantList(), const QString &id = QString());

signals:
    void result(QString id, QVariant result);
    void error(QString id, QVariant error);

private slots:
    void readyRead();
};
