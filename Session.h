#pragma once

#include "Arguments.h"

#include <QObject>
#include <QRunnable>

class Session : public QObject, public QRunnable
{
    Q_OBJECT

    Arguments* args;
    int requests = 1;

public:
    Session(QObject *parent = nullptr);

    void setArguments(Arguments *args);
    void setRequests(int requests);

    void run() override;

signals:
    void resultReady(int id, const QString &s);
};
