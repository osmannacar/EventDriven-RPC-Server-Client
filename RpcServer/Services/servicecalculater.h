#ifndef SERVICECALCULATER_H
#define SERVICECALCULATER_H
#include <QString>
#include "Rpc/remoteobject.h"
#include <QVector>

class ServiceCalculater : public RemoteObject
{
public:
    ServiceCalculater(){ }
    bool sumGreaterThan100(QString &hata, const QVector<int> &values, int &result);

    virtual bool call(QString &_method_name, TemplateReadersWriters *readerWriter) override;
};

#endif // SERVICECALCULATER_H
