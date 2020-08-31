#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <QObject>
#include <QCommandLineParser>

class Arguments : public QObject
{
    Q_OBJECT
    QCommandLineParser parser;

public:
    explicit Arguments(QCoreApplication *app);

    QString hostname();
    quint16 port();
    int concurrency();
    int requests();
    bool help();
    bool verbose();

    QString helpText();

signals:

};

#endif // ARGUMENTS_H
