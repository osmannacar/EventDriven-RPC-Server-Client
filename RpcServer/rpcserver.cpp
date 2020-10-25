#include "rpcserver.h"
#include <QDebug>
#include "Network/networkwriter.h"
#include "Network/networkreader.h"
#include <QMutexLocker>

extern QMutex mtx_dummy;
struct ClientInfo
{
    TcpStreamSocket _tcpSocket;
    QString m_user_info;
    bool operator ==(const ClientInfo &_info){
        return (this->m_user_info == _info.m_user_info);
    }
    bool operator !=(const ClientInfo &_info){
        return !this->operator ==(_info);
    }

};


class ClientProcess: public QThread
{
    ClientInfo m_info;
public:
    ClientProcess (ClientInfo _info) {
        this->m_info = _info;
    }
    void sendDummyData(){
        QMutexLocker locker(&mtx_dummy);
        NetworkWriter writer(&this->m_info._tcpSocket);
        TemplateReadersWriters readerWriter(&writer, nullptr);
        QString l_method_name = QString("dummy");
        QVector<QString> dummy = {"hi", "from", "server"};
        readerWriter.marshall(l_method_name, dummy);
        locker.unlock();
    }

protected:
    //process client request
    virtual void run() override{
        NetworkReader reader(&this->m_info._tcpSocket);
        NetworkWriter writer(&this->m_info._tcpSocket);
        QString process_name;
        bool flag;
        while (this->m_info._tcpSocket.isValid() && (flag = reader.read(process_name))) {
            if(process_name.compare("rpc") == 0){
                QString l_service_name;
                reader.read(l_service_name);
                if(RpcServer::instance()->isContainsServiceObject(l_service_name)){
                    this->sendDummyData();
                    QString l_method_name;
                    reader.read(l_method_name);
                    TemplateReadersWriters readerWriter(&writer, &reader);
                    RpcServer::instance()->rpcObject(l_service_name)->call(l_method_name, &readerWriter);
                }
            }
        }
    }
};

struct RpcServer::pimpl{
    TcpStreamSocket _tcpSocket;
    QMap<QString, RemoteObject *> _rpcObjects;
    QVector<ClientProcess*> _clientProcess;
    QString _deviceName;
    static RpcServer *_singletonRpcServer;
};

RpcServer *RpcServer::pimpl::_singletonRpcServer = nullptr;
RpcServer::RpcServer(QString &_device_name)
{
    if(RpcServer::pimpl::_singletonRpcServer == nullptr){
        this->_pimpl = QSharedPointer<pimpl>(new pimpl);
        this->_pimpl->_deviceName = _device_name;
        RpcServer::pimpl::_singletonRpcServer = this;
        this->_pimpl->_tcpSocket.createSocket();
    }else {
        Q_ASSERT("RPC server nesnesi sadece bir defa olusturulabilir");
    }

}

bool RpcServer::bind(const QString &_ip, const short &_port)
{
    bool flag = false;
    if(this->_pimpl->_tcpSocket.bind(_ip, _port))
        flag = true;
    return flag;
}

void RpcServer::listen()
{
    this->_pimpl->_tcpSocket.listen();
}

TcpStreamSocket &RpcServer::socket()
{
    return this->_pimpl->_tcpSocket;
}

bool RpcServer::addRpcObject(const QString &_service_name, RemoteObject *_object)
{
    bool flag = false;
    if(!this->_pimpl->_rpcObjects.contains(_service_name)){
        this->_pimpl->_rpcObjects.insert(_service_name, _object);
        flag = true;
    }
    return flag;
}

RemoteObject *RpcServer::rpcObject(QString &_service_name)
{
    return this->_pimpl->_rpcObjects.value(_service_name);
}

bool RpcServer::isContainsServiceObject(QString &_service_name)
{
    return this->_pimpl->_rpcObjects.contains(_service_name);
}

bool RpcServer::close()
{
    return this->_pimpl->_tcpSocket.close();
}

RpcServer *RpcServer::instance()
{
    return RpcServer::pimpl::_singletonRpcServer;
}

void RpcServer::run()
{
    qDebug() << "Activated";
    while (this->_pimpl->_tcpSocket.isValid()) {
        TcpStreamSocket _socket = this->_pimpl->_tcpSocket.accept();
        NetworkWriter writer(&_socket);
        writer.write(this->_pimpl->_deviceName);
        QString connection_str;
        NetworkReader reader(&_socket);
        reader.read(connection_str);
        if(connection_str.compare("login") == 0){
            ClientInfo _client_info;
            _client_info._tcpSocket = _socket;
            reader.read(_client_info.m_user_info);
            qDebug() << "Connected new Client : "<< _client_info.m_user_info ;
            ClientProcess *_clientProcess = new ClientProcess(_client_info);
            writer.write(QString("connected"));
            _clientProcess->start();
            this->_pimpl->_clientProcess.append(_clientProcess);
        }else {
            _socket.close();
        }
    }
    this->_pimpl->_clientProcess.clear();
}
