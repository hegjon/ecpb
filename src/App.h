#pragma once

#include <QCoreApplication>
#include <QThreadPool>
#include <QMutex>
#include "Session.h"
#include "Arguments.h"

class App : public QCoreApplication
{
    Q_OBJECT

    QThreadPool sessions;
    void quitIfSessionsAreDone();
    QAtomicInt stoppedSessions = 0;
    Arguments *args;
    QMutex mutex;


public:
    explicit App(int argc, char *argv[]);
    void run();

public slots:
    void resultReady(SessionResult result);
};
