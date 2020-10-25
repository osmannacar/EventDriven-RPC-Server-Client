#include "cnttest.h"
#include "utills.h"
#include "client.h"
bool CntTest::cntCalculater(QString &hata, const QVector<int> &values, int &result)
{
    EventReceiver<bool, QString, int> event;
    Utills::client()->beginCall(&event, "calculater", "sumGreaterThan100", values);
    event.wait(1000);
    result = event.get<2>();
    hata = event.get<1>();
    return event.get<0>();
}
