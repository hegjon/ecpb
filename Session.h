#pragma once

#include "Arguments.h"

#include <QObject>
#include <QRunnable>

class Session : public QObject, public QRunnable
{
    Q_OBJECT

    Arguments* args;
public:
    Session(Arguments *args, QObject *parent = nullptr);
    void run() override;

signals:
    void resultReady(int id, const QString &s);
};
