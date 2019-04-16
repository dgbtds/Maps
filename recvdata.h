#ifndef RECVDATA_H
#define RECVDATA_H
#include <QObject>
#include "qringbuffer.h"
#include <string>


class GETDT: public QObject
{
Q_OBJECT

public:
    GETDT();
    ~GETDT();
    int getdt(QRingBuffer* rbuffer,std::string SERVER_IP,int*ctrflag,int*starflag);

signals:
    void prompt(QString title);
    void state(QString status);
    void eventnum(unsigned long eventnow);



};


#endif // RECVDATA_H
