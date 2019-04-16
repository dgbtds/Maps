#ifndef ANALYSE_H
#define ANALYSE_H
#include <QObject>
#include "qringbuffer.h"
#include <string>
class Analyse: public QObject
{
Q_OBJECT

public:
    Analyse();
    ~Analyse();
int analysedt(QRingBuffer*rbuffer,int getdtNum,std::string File_name,std::string ip,int*ctrflag,int*starflag,int*ladheader,int*pack_num,QRingBuffer*rbufferdata);
signals:
    void prompt(QString title);

};
#endif // ANALYSE_H
