#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <QObject>
#include <QVector>
#include <QCommandLineParser>

class Arguments : public QObject
{
    Q_OBJECT
    QCommandLineParser parser;

    QString _hostname;
    quint16 _port;
    QVector<QString> _address;

public:
    explicit Arguments(QCoreApplication *app);

    QString hostname();
    quint16 port();
    int concurrency();
    int requests();
    const QVector<QString>& address();
    bool help();
    bool verbose();

    QString helpText();

};

#endif // ARGUMENTS_H
