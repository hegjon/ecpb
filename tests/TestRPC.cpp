#include <QTest>

#include "MockDevice.h"
#include "RPC.h"

class TestRPC : public QObject {
    Q_OBJECT
private slots:
    void writeServerVersion() {
        MockDevice device(this);
        RPC rpc(&device, this);

        rpc.send("server.version", QVariantList(), "1");

        QString expected = "{\"id\":\"1\",\"jsonrpc\":\"2.0\",\"method\":\"server.version\",\"params\":[]}\n";
        QCOMPARE(device.dataWritten, expected);
    }
};

QTEST_MAIN(TestRPC)

#include "TestRPC.moc"
