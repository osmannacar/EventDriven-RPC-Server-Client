#include "tcpstreamsocket.h"

struct TcpStreamSocket::pimpl{
    //client or server
    int device_fd = -1;
    QString _ip;
    ushort _port;
    ~pimpl()
    {
        ::close(this->device_fd);
    }
};

TcpStreamSocket::TcpStreamSocket()
{
    this->_pimpl =  QSharedPointer<pimpl>(new pimpl);
}

bool TcpStreamSocket::createSocket()
{
    bool flag = true;
    // creating socket
    if((this->_pimpl->device_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        flag = false;
    }
    return flag;
}

bool TcpStreamSocket::bind(const QString &_ip, const ushort &_port)
{
    bool flag = true;
    static int opt = 1;
    if(::setsockopt(this->_pimpl->device_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        flag = false;
    }else {
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(_ip.toStdString().data());
        address.sin_port = htons(_port);
        // forcefully attaching socket to the port
        if(::bind(this->_pimpl->device_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
            flag = false;
        }
        this->_pimpl->_ip = _ip;
        this->_pimpl->_port = _port;
    }
    return flag;
}

bool TcpStreamSocket::listen()
{
    bool flag = true;
    // -1 ?
    if(::listen(this->_pimpl->device_fd, -1) < 0){
        flag = false;
        this->_pimpl->device_fd = -1;
    }
    return flag;
}

TcpStreamSocket TcpStreamSocket::accept()
{
    TcpStreamSocket sock;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    ::memset(&address, 0, addrlen);

    sock._pimpl->device_fd = ::accept(this->_pimpl->device_fd,(struct sockaddr *)&address, (socklen_t*)&addrlen);
    return sock;
}

bool TcpStreamSocket::connect(const QString &_ip, const ushort & _port)
{
    bool flag = true;
    struct  sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = ::htons(_port);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(::inet_pton(AF_INET, _ip.toStdString().data(), &serv_addr.sin_addr) <= 0){
        flag = false;
        this->_pimpl->device_fd = -1;
    }else {
        if (::connect(this->_pimpl->device_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
            flag = false;
            this->_pimpl->device_fd = -1;
        }
        this->_pimpl->_ip = _ip;
        this->_pimpl->_port = _port;
    }
    return flag;

}

int TcpStreamSocket::receive(void *buffer, int length)
{
    return ::recv(this->_pimpl->device_fd, buffer, length, 0);;
}

int TcpStreamSocket::send(void *buffer, int length)
{
    return ::send(this->_pimpl->device_fd, buffer, length, 0);
}

bool TcpStreamSocket::close()
{
    bool flag = false;
    if(this->_pimpl->device_fd != -1){
        if((::close(this->_pimpl->device_fd)) != -1){
            flag = true;
            while (this->_pimpl->device_fd != -1) {
                this->_pimpl->device_fd = -1;
            }
        }
    }
    return flag;
}

QString TcpStreamSocket::getIp()
{
    return this->_pimpl->_ip;
}

ushort TcpStreamSocket::getPort()
{
    return this->_pimpl->_port;
}

bool TcpStreamSocket::isValid()
{
    return  this->_pimpl->device_fd != -1;
}


