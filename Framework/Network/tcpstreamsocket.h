#ifndef TCPSTREAMSOCKET_H
#define TCPSTREAMSOCKET_H
#include <QString>
#include <QSharedPointer>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

class TcpStreamSocket
{
    struct pimpl;
    QSharedPointer <pimpl> _pimpl;

public:
    TcpStreamSocket();
    bool createSocket();
    bool bind(const QString &_ip, const ushort &_port);
    bool listen();
    TcpStreamSocket accept();
    bool connect(const QString &_ip, const ushort &_port);
    int receive(void *buffer, int length);
    int send(void *buffer, int length);
    bool close();
    QString getIp();
    ushort getPort();
    bool isValid();
};

#endif // TCPSTREAMSOCKET_H
