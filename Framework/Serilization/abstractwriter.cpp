#include "abstractwriter.h"

bool AbstractWriter::writeLength(int len)
{
    return this->write(len);
}

bool AbstractWriter::write(bool value)
{
    return this->writeData((char*) &value, 1);
}

bool AbstractWriter::write(int value)
{
    return this->writeData((char*) &value, 4);
}

bool AbstractWriter::write(long long value)
{
    return this->writeData((char*) &value, 8);
}

bool AbstractWriter::write(const QString &value)
{
    return this->write(value.toUtf8());
}

bool AbstractWriter::write(const QByteArray &value)
{
    int size = value.size();
    bool flag = this->writeLength(size);
    if(size > 0){
        flag = flag && this->writeData(value.data(), size);
    }
    return flag;
}
