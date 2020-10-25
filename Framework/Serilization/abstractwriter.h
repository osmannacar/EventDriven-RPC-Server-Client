#ifndef WRITER_H
#define WRITER_H
#include <QString>
#include <QStringList>
#include <QMap>

class AbstractStructSerializaDeSerialize;

class AbstractWriter
{
public:
    AbstractWriter(){}
    virtual ~AbstractWriter() = default;
    //liste halinde verilerin boyutunu gonderir
    bool writeLength(int len);
    bool write(bool value);
    bool write(int value);
    bool write(long long value);
    bool write(const QString &value);
    bool write(const QByteArray &value);
    template <typename T> bool write(const QVector<T> &value);
protected:
    virtual bool writeData(const char *data, unsigned long int len) = 0;
};

template<typename T>bool AbstractWriter::write(const QVector<T> &value)
{
    bool flag = true;
    if(this->writeLength(value.length())){
        for(int i = 0; i < value.length(); ++i){
            if(!this->write(value[i])){
                flag = false;
                break;
            }
        }
    }else {
        flag = false;
    }
    return flag;
}

#endif // WRITER_H
