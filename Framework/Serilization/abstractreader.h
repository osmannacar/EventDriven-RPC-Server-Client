#ifndef READER_H
#define READER_H
#include <QString>
#include <QStringList>
#include <QMap>

class AbstractStructSerializaDeSerialize;

class AbstractReader
{
public:
    AbstractReader(){}
    virtual ~AbstractReader() = default;
    bool readLength(int &lenght);
    bool read(bool &value);
    bool read(int &value);
    bool read(long long &value);
    bool read(QString &value);
    bool read(QByteArray &value);
    template <typename T> bool read(QVector<T> &value);

protected:
    virtual bool readData(char *data, unsigned long int len) = 0;
};


template<typename T> bool AbstractReader::read(QVector<T> &value)
{
    int length;
    bool flag = true;
    value.clear();
    if(this->readLength(length)){
        for(int i = 0; i < length; ++i){
            T t;
            if(this->read(t)){
                value.append(t);
            }else {
                flag = false;
                break;
            }
        }
    }else {
        flag = false;
        value.clear();
    }
    return flag;
}


#endif // READER_H
