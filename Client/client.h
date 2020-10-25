#ifndef RPCCLIENT_H
#define RPCCLIENT_H
#include <QObject>
#include <QSharedPointer>
#include "Serilization/templatereaderswriters.h"
#include "Network/tcpstreamsocket.h"
#include "Network/networkreader.h"
#include "Network/networkwriter.h"
#include "Serilization/templatereaderswriters.h"
#include "QCryptographicHash"
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include "eventreceiver.h"

class Client : public QObject
{
    Q_OBJECT
    TcpStreamSocket m_tcpSocket;
    QString m_user_info;
    QString m_server_name;
    NetworkReader *m_reader = nullptr;
    NetworkWriter *m_writer  = nullptr;
    TemplateReadersWriters *m_templatereaderWriter = nullptr;
    QThread m_thread;
public:
    Client(const QString &_ip, const ushort & _port)
    {
        if(!this->m_tcpSocket.createSocket()){
            qDebug() << "socket did not create";
            exit(EXIT_FAILURE);
        }
        if(!this->m_tcpSocket.connect(_ip, _port)){
            qDebug() << "client did not connect to server check ip or port";
            exit(EXIT_FAILURE);
        }
        this->m_reader = new NetworkReader(&this->m_tcpSocket);
        this->m_writer = new NetworkWriter(&this->m_tcpSocket);

        this->m_user_info = QString(QCryptographicHash::hash(QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss").toUtf8(), QCryptographicHash::Md5).toHex());

        this->m_reader->read(this->m_server_name);
        qDebug() << "Server:" << this->m_server_name;
        this->m_writer->write(QString("login"));
        this->m_writer->write(this->m_user_info);
        QString l_connection_status;
        this->m_reader->read(l_connection_status);
        qDebug() << "Connection Status:" << l_connection_status;

        this->m_templatereaderWriter = new TemplateReadersWriters(this->m_writer, this->m_reader);

        moveToThread(&this->m_thread);
        QObject::connect(&this->m_thread, SIGNAL(started()), this, SLOT(listenServerRequest()));
        //         QObject::connect(current_thread, SIGNAL(finished()), &(this->m_thread), &QObject::deleteLater);
        this->m_thread.start();
    }
    virtual ~Client(){
        this->m_thread.exit(0);
        this->m_tcpSocket.close();
        if(this->m_reader != nullptr){
            delete this->m_reader;
        }
        if(this->m_writer != nullptr){
            delete this->m_writer;
        }
        if(this->m_templatereaderWriter != nullptr){
            delete this->m_templatereaderWriter;
        }
    }
    template<typename ...T>  bool beginCall(EventReceiverBase *ev, const QString &_service_name, const QString &_method_name, const T &...args){

        qDebug() << ev->id();
        return this->m_templatereaderWriter->marshall(QString("rpc"), _service_name, _method_name, ev->id(), args...);
    }
public slots:
    void listenServerRequest(){
        QString l_process_name;
        while (this->m_tcpSocket.isValid() && this->m_reader->read(l_process_name)) {
            if(l_process_name.compare("dummy") == 0){
                //todo
                QVector<QString> l_dummy;
                this->m_reader->read(l_dummy);
                qDebug() << "Method name:" << l_process_name << " data:" << l_dummy;
            } else if (l_process_name.compare("eventreceiver") == 0) {
                long long l_event_id;
                this->m_reader->read(l_event_id);
                if(EventReceiverBase::contains(l_event_id)){
                    EventReceiverBase *event_receiver = EventReceiverBase::getObjectFromId(l_event_id);
                    event_receiver->wakeUp();
                    event_receiver->readFromNetwork(this->m_templatereaderWriter);
                }
            }
        }
        if(l_process_name.isEmpty()){
            qDebug() << "server was down";
        }
    }

};

#endif // RPCCLIENT_H
