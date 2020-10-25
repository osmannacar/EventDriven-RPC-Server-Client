#include "abstractreader.h"

bool AbstractReader::readLength(int &lenght)
{
    return this->read(lenght);
}

bool AbstractReader::read(bool &value)
{
    return this->readData((char *)&value, 1);
}
bool AbstractReader::read(int &value)
{
    return this->readData((char *)&value, 4);
}

bool AbstractReader::read(long long &value)
{
     return this->readData((char *)&value, 8);
}

bool AbstractReader::read( QString &value)
{
    QByteArray arr;
    bool r = this->read(arr);
    value = QString(arr);
    return r;
}
bool AbstractReader::read(QByteArray &value)
{
    int length;
    bool flag = true;
    if(this->readLength(length)){
        if(length > 0){
            value.resize(length);
            if(!this->readData(value.data(), length)){
                flag = false;
            }
        }
    }else {
        flag = false;
    }
    return flag;
}

