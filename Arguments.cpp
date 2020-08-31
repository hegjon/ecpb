#include <QDebug>
#include "Arguments.h"

Arguments::Arguments(QCoreApplication *app) : QObject(app)
{

    QCommandLineOption concurrency(QStringList() << "c" << "concurrency",
        "Number of multiple requests to perform at a time. Default is one request at a time.",
        "concurrency", "1");

    QCommandLineOption requests(QStringList() << "n" << "requests",
        "Number of total requests to perform for the benchmarking session. The default is to just perform a single request which usually leads to non-representative benchmarking results.",
        "requests", "1");

    QCommandLineOption address(QStringList() << "a" << "address",
        "File that contains all the bitcoin cash addresses to use. One address per line. Default is 'bitcoincash:qp3wjpa3tjlj042z2wv7hahsldgwhwy0rq9sywjpyy' as its content",
        "address");

    QCommandLineOption verbose(QStringList() << "v" << "verbose",
        "Print verbose debug information. Default false");

    QCommandLineOption help(QStringList() << "h" << "help",
        "Display usage information.");


    parser.addOption(concurrency);
    parser.addOption(requests);
    parser.addOption(address);
    parser.addOption(verbose);
    parser.addOption(help);
    parser.addPositionalArgument("host", "Hostname and port seperated by colon, default port is 50001", "hostname[:port]");

    parser.process(*app);
}

QString Arguments::hostname()
{
    return "192.168.128.3";

}

quint16 Arguments::port()
{
    return 50001;
}

int Arguments::concurrency()
{
    bool ok;
    int value = parser.value("concurrency").toInt(&ok);
    if(!ok || value <= 0) {
        qCritical() << parser.value("concurrency") << "is not valid";
        parser.showHelp(1);
    }

    return value;
}

int Arguments::requests()
{
    bool ok;
    int value = parser.value("requests").toInt(&ok);
    if(!ok) {
        parser.showHelp(1);
    }

    return value;
}

bool Arguments::help()
{
    return parser.isSet("help");
}

bool Arguments::verbose()
{
    return parser.isSet("verbose");
}

QString Arguments::helpText()
{
    return parser.helpText();
}
