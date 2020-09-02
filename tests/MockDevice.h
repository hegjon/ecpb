#pragma once

#include <QObject>
#include <QIODevice>

class MockDevice : public QIODevice
{
    Q_OBJECT

public:
    MockDevice(QObject *parent = nullptr);
    QByteArray dataWritten;

protected:
    qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    qint64 writeData(const char *data, qint64 len) Q_DECL_OVERRIDE;
    bool waitForReadyRead(int msecs) Q_DECL_OVERRIDE;
    bool waitForBytesWritten(int msecs) Q_DECL_OVERRIDE;

};
