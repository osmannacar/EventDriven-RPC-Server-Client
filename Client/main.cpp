#include <QCoreApplication>
#include "Controller/cnttest.h"
#include <QVector>
#include<QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<int> values = {15, 20, 35, 12, 45};
    QString hata;
    bool flag;
    int sum;

    CntTest test;
    flag = test.cntCalculater(hata, values, sum);
    qDebug() << "flag:" << flag;
    qDebug() << "hata:" << hata;
    qDebug() << "sum:" << sum;

    return a.exec();
}
