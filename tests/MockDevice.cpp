#include "MockDevice.h"

MockDevice::MockDevice(QObject *parent) : QIODevice(parent)
{
    this->setOpenMode(QIODevice::ReadWrite);
}

qint64 MockDevice::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    return 0;
}

qint64 MockDevice::writeData(const char *data, qint64 len)
{
    dataWritten.append(data, len);
    return len;
}

bool MockDevice::waitForReadyRead(int msecs)
{
    return true;
}

bool MockDevice::waitForBytesWritten(int msecs)
{
    return true;
}

