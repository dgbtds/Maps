#ifndef SENDCMD_H
#define SENDCMD_H
#include<QObject>
#include<QString>
#include <string>
class Rcfgpck: public QObject
{
Q_OBJECT

public:
    Rcfgpck();
    ~Rcfgpck();

//public:
    //void ldfile(unsigned char* rawudp);

public slots:
    int rcfgpck(unsigned char*  udp_config,unsigned char* header,int Rdatalength,QString s,std::string SERVER_IP);

   // void rbcupck(unsigned char* pckudpack, unsigned char* pckudp,unsigned char udpackflag,unsigned char* udpack);
signals:
    void state(QString status);
    void prompt(QString title);

};
#endif // SENDCMD_H
