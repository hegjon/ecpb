#pragma once

#include <QCoreApplication>
#include <QThreadPool>
#include "Session.h"
#include "Arguments.h"

class App : public QCoreApplication
{
    Q_OBJECT

    QThreadPool sessions;
    void quitIfSessionsAreDone();
    QAtomicInt stoppedSessions = 0;

public:
    explicit App(int argc, char *argv[]);

public slots:
    void sessionDone(int id, const QString &result);
};
