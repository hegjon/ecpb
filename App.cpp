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

    this->args = new Arguments(this);
    if(args->help()) {
        QTextStream out(stdout);
        out << args->helpText();
        out.flush();
        std::exit(0);
    }

    if(args->verbose()) {
        QLoggingCategory::setFilterRules("*.debug=true");
    }
    qSetMessagePattern("[%{time process}][%{qthreadptr}] %{message}");
}

void App::run()
{
    qInfo() << "ecpb";
    qInfo() << "Verbose       " << args->verbose();
    qInfo() << "Concurrency   " << args->concurrency();
    qInfo() << "Total requests" << args->requests();
    qInfo() << "Addresses     " << args->address().count();

    int requestPerSession = args->requests() / args->concurrency();

    sessions.setMaxThreadCount(args->concurrency());
    sessions.setObjectName("Session Pool");

    qInfo() << "Starting" << args->concurrency() << "session(s)";
    for(int i = 0; i < args->concurrency(); i++) {
        Session *s = new Session(nullptr);
//        connect(s, &Session::resultReady, this, &App::resultReady, Qt::QueuedConnection);

        s->setObjectName("Session" + QString::number(i));
        s->setArguments(args);
        s->setRequests(requestPerSession);
        s->setAutoDelete(true);

        sessions.start(s);
    }

    sessions.waitForDone();

    qInfo() << "All" << args->concurrency() << "session(s) done";
}

void App::resultReady(SessionResult result)
{
    mutex.lock();

    Session* session = qobject_cast<Session *>(sender());
    if(!session) {
        qWarning() << "Sender is not Session type" << sender()->metaObject()->className();
        return;
    }

    stoppedSessions++;
    qInfo() << "Session" << session << "done!"
            << "Requests:" << result.requests
            << "Stopped sessions:" << stoppedSessions
            << "Active threads:" << sessions.activeThreadCount();

    if(stoppedSessions >= args->concurrency()) {
        qInfo() << "All " << args->concurrency() << "session(s) finished. Quiting";
        emit quit();
    }
    mutex.unlock();
}
