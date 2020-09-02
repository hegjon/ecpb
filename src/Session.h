#pragma once

#include "Arguments.h"
#include "RPC.h"

#include <QObject>
#include <QRunnable>
#include <QDateTime>
#include <QMutex>
#include <QSet>
#include <QSemaphore>

struct SessionResult
{
    QDateTime start;
    QDateTime end;
    int requests = 0;
    int failures = 0;
};

Q_DECLARE_METATYPE(SessionResult);

class Session : public QObject, public QRunnable
{
    Q_OBJECT
    Q_DISABLE_COPY(Session)

    Arguments* args;
    int requests = 1;

    void call(RPC &rpc);

    QEventLoop waitForReply;
    QAtomicInt responses = 0;

public:
    Session(QObject *parent = nullptr);

    void setArguments(Arguments *args);
    void setRequests(int requests);

    void run() override;

public slots:
    void result(QString id, QVariant result);
    void error(QString id, QVariant error);

signals:
    void resultReady(SessionResult result);
};
