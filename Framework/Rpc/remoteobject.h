#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H
#include "Serilization/templatereaderswriters.h"

//add permission struct

class RemoteObject
{
public:
    RemoteObject() {}

    //call method and read arg from  and write to network
    virtual bool call(QString &_method_name, TemplateReadersWriters *readerWriter) = 0;
    //add hashAuthorization
};

#endif // ABSTRACTSERVICE_H
