#ifndef LOADCONFIG_H
#define LOADCONFIG_H



#include <QObject>
class LoadConfig: public QObject
{
Q_OBJECT

public:
    LoadConfig();
    ~LoadConfig();


public slots:
    void loadfile( unsigned char* udpconfig,int i,int volum);
    void   checkdata(QString s,std::string SERVER_IP);

signals:
    void prompt(QString title);
    void state(QString status);


};

#endif // LOADCONFIG_H
