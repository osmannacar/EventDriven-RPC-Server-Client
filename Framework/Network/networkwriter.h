#ifndef NETWORKWRITER_H
#define NETWORKWRITER_H

#include <Serilization/abstractwriter.h>
#include <Network/tcpstreamsocket.h>
class NetworkWriter : public AbstractWriter
{
    TcpStreamSocket *m_socket;
public:
    NetworkWriter (TcpStreamSocket *_socket) {
        this->m_socket = _socket;
    }
protected:
    virtual bool writeData(const char *data, unsigned long int len) override final{
        return (this->m_socket->send((void *)data, len) > 0);
    }
};

#endif // NETWORKWRITER_H
