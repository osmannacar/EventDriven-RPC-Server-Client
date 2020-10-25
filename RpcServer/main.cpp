#include <QCoreApplication>
#include "rpcserver.h"
#include "Services/servicecalculater.h"
#include <QMutex>

QMutex mtx_dummy;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString device_name("VCServer");
    RpcServer server(device_name);
    server.bind("127.0.0.1", 1234);
    server.listen();

    ServiceCalculater calculter;
    server.addRpcObject("calculater", &calculter);

    server.start();

    int val = a.exec();
    server.wait();
    server.close();
    return val;
}
