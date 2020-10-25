#ifndef RPCSERVER_H
#define RPCSERVER_H
#include <QThread>
#include <QString>
#include "Network/tcpstreamsocket.h"
#include "Rpc/remoteobject.h"
#include <QMutex>

class RpcServer : public QThread
{
    struct pimpl;
    QSharedPointer<pimpl> _pimpl;
public:
    explicit RpcServer(QString &_device_name);
    bool bind(const QString &_ip, const short &_port);
    void listen();
    TcpStreamSocket &socket();
    bool addRpcObject(const QString &_service_name, RemoteObject *_object);
    RemoteObject *rpcObject(QString &_service_name);
    bool isContainsServiceObject(QString &_service_name);
    bool close();
    static RpcServer *instance();
protected:
    //accept client
    virtual void run() override;
};

#endif // RPCSERVER_H
