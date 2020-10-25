#ifndef NETWORKREADER_H
#define NETWORKREADER_H

#include <Serilization/abstractreader.h>
#include <Network/tcpstreamsocket.h>
class NetworkReader : public AbstractReader
{
    TcpStreamSocket *m_socket;
public:
    NetworkReader (TcpStreamSocket *_socket) {
        this->m_socket = _socket;
    }
protected:
    virtual bool readData(char *data, unsigned long int len) override final{
        return (this->m_socket->receive((void *)data, len) > 0);
    }
};


#endif // NETWORKREADER_H
