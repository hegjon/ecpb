#include "App.h"
#include "Session.h"

#include <QDebug>
#include <QLoggingCategory>
#include <QQueue>
#include <QThreadPool>
#include <QTextStream>
#include "Arguments.h"

App::App(int argc, char *argv[]) : QCoreApplication (argc, argv)
{
    setApplicationName("ecpb");
    setApplicationVersion("0.1");

    Arguments args(this);
    if(args.help()) {
        QTextStream out(stdout);
        out << args.helpText();
        out.flush();
        std::exit(0);
    }

    if(args.verbose()) {
        QLoggingCategory::setFilterRules("*.debug=true");
    }

    qInfo() << "ecpb";
    qInfo() << "Verbose       " << args.verbose();
    qInfo() << "Concurrency   " << args.concurrency();
    qInfo() << "Total requests" << args.requests();
    qInfo() << "Addresses     " << args.address().count();

    int requestPerSession = args.requests() / args.concurrency();

    sessions.setMaxThreadCount(args.concurrency());
    sessions.setObjectName("Session Pool");

    qInfo() << "Starting";
    for(int i = 0; i < args.concurrency(); i++) {
        Session *s = new Session(this);
        s->setArguments(&args);
        s->setRequests(requestPerSession);
        connect(s, &Session::resultReady, this, &App::sessionDone, Qt::QueuedConnection);
        s->setAutoDelete(true);
        sessions.start(s);
    }

    sessions.waitForDone();

    qInfo() << "Done";
    std::exit(0);
}

void App::sessionDone(int id, const QString &result)
{
    qInfo().noquote() << "Session" << id << "done:" << result;
}
