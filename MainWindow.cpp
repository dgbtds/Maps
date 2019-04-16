#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <res.h>

#include "qringbuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>
#include <iostream>
#include <QString>
#include<pthread.h>
#include<thread>
#include <QTimer>//qtime
#include <qdatetime.h>//currentdatatime
#include <QtGlobal>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <ostream>
#include<QDebug>
#include <sys/stat.h>//mkdir
#include <stdint.h>
#include <vector>
#include <sstream>

using namespace std;

#define configdatalength 193
#define MAX_LADDER_PACK 100000
#define MAX_TOTAL_PACK 1000000
#define FEE_SUM 5

unsigned char udpconfig[1000];
QString qFileName;

unsigned char config1[configdatalength];
unsigned char config2[configdatalength];
unsigned char config3[configdatalength];
unsigned char config4[configdatalength];
unsigned char config5[configdatalength];
unsigned char config6[configdatalength];
unsigned char config7[configdatalength];
unsigned char config8[configdatalength];
unsigned char config9[configdatalength];
unsigned char config10[configdatalength];
unsigned char config11[configdatalength];
unsigned char config12[configdatalength];
unsigned char config13[configdatalength];
unsigned char config14[configdatalength];
unsigned char config15[configdatalength];
unsigned char config16[configdatalength];
unsigned char config17[configdatalength];
unsigned char config18[configdatalength];
unsigned char Wdata[10]={0};
unsigned char header[8];
unsigned int RUNNUM[1];
char datetime[200];
unsigned int runnum;
int getdtNum, totaltime;
int ctrflag1=0,ctrflag2=0,ctrflag3=0,ctrflag4=0,ctrflag5=0;
int starflag1=0,starflag2=0,starflag3=0,starflag4=0,starflag5=0;
int sendstar1=0,sendstar2=0,sendstar3=0,sendstar4=0,sendstar5=0;
int Rdatalength=0,ladder_1=0,ladder_2=0,ladder_3=0,ladder_4=0,ladder_5=0;
char FileName[200];
string fileName;
QString s="";
int EventA=0,EventB=0;
double Event_rate=0;
int ladheader1=0,ladheader2=0,ladheader3=0,ladheader4=0,ladheader5=0;
QRingBuffer* rbuffer[5];
QRingBuffer rbuffer_1,rbuffer_2,rbuffer_3,rbuffer_4,rbuffer_5;
QRingBuffer Brbuffer1,Brbuffer2,Brbuffer3,Brbuffer4,Brbuffer5;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadconfig= new LoadConfig();
    rcfgpck=new Rcfgpck();
    getdt=new GETDT();
    analysedt=new Analyse();
    connect(rcfgpck,SIGNAL(prompt(QString)),this,SLOT(onPrompt(QString)));
    connect(rcfgpck,SIGNAL(state(QString)),this,SLOT(onstate(QString)));
    connect(loadconfig,SIGNAL(prompt(QString)),this,SLOT(onPrompt(QString)));
    connect(analysedt,SIGNAL(prompt(QString)),this,SLOT(onPrompt(QString)));
    connect(loadconfig,SIGNAL(state(QString)),this,SLOT(onstate(QString)));
    connect(this,SIGNAL(prompt(QString)),this,SLOT(onPrompt(QString)));
    connect(this,SIGNAL(state(QString)),this,SLOT(onstate(QString)));
    connect(getdt,SIGNAL(prompt(QString)),this,SLOT(onPrompt(QString)));
    ui->status1->setText("WAITING");
    ui->status2->setText("WAITING");
    ui->status3->setText("WAITING");
    ui->status4->setText("WAITING");
    ui->status5->setText("WAITING");

    ui->ip1->setText("192.168.10.17");
    ui->sensor1->setText("0000000011");
    ui->ip2->setText("192.168.10.18");
    ui->sensor2->setText("0000000011");
    ui->ip3->setText("192.168.10.19");
    ui->sensor3->setText("0000000011");
    ui->ip4->setText("192.168.10.20");
    ui->sensor4->setText("0000000011");
    ui->ip5->setText("192.168.10.21");
    ui->sensor5->setText("0000000011");

    ui->threshold1->setText("7");
    ui->threshold2->setText("7");
    ui->threshold3->setText("7");
    ui->threshold4->setText("7");
    ui->threshold5->setText("7");

    ui->daqfilename->setText("10000");
     emit state("Waiting");

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onPrompt(QString title)
{
    // QMessageBox::critical(this,title,content,QMessageBox::Close);

    QString hi=title;
    ui->log->append(hi);
}
void MainWindow::onstate(QString status)
{
    QString hi=status;
    //ui->log->append(hi);
    ui->status ->setText(hi);
}

void MainWindow::on_start_clicked()
{
    //start
    ctrflag1=0;
    ctrflag2=0;
    ctrflag3=0;
    ctrflag4=0;
    ctrflag5=0;
    starflag1=0;
    starflag2=0;
    starflag3=0;
    starflag4=0;
    starflag5=0;
    stopsgn=0;
    packnum1=0;
    packnum2=0;
    packnum3=0;
    packnum4=0;
    packnum5=0;
    ui->config1->setEnabled(false);
    ui->config2->setEnabled(false);
    ui->config3->setEnabled(false);
    ui->config4->setEnabled(false);
    ui->config5->setEnabled(false);
    ui->start->setEnabled(false);
    ui->stoptime->setText("");
    emit state("DAQ start");
    emit prompt("DAQ START");

    QDateTime time_ = QDateTime::currentDateTime();
    QString startime = time_.toString(" hh:mm:ss ");
    ui->starttime->setText(startime);


    bzero(FileName,sizeof(FileName) );
    time_t now;
    struct tm *tm_now;


    FILE*fpr1=fopen("1","rb");
    fread(RUNNUM,sizeof(int),1,fpr1);
    fclose(fpr1);

    runnum=RUNNUM[0];
    printf("1 runnum:%d\n",runnum);
    fflush(stdout);
    runnum++;
    RUNNUM[0]=RUNNUM[0]+1;

    FILE*fpr2=fopen("1","wb");
    fwrite(RUNNUM,sizeof(int),1,fpr2);
    fclose(fpr2);

    printf("2 RUNNUM[0]:%d",RUNNUM[0]);
    fflush(stdout);


    QString therun;
    ui->runnum->setText(therun.setNum(runnum));

    time(&now);
    tm_now = localtime(&now);
    strftime(datetime, 50, "%Y-%m-%d-%H-%M-%S", tm_now);
    sprintf(FileName,"/home/daq/Desktop/Mapsdata/RUN%d_%s",runnum,datetime);
    mkdir(FileName,460);



    QString ip_;
    string ip;
    bool ok;
    QString datanum=ui->daqfilename->text();

    if(datanum!=NULL)
    {
        getdtNum=datanum.toUInt(&ok,10) ;

    }
     getdtNum=2*getdtNum;
    printf("\n mindows:getDTnum:%d",getdtNum);
    fflush(stdout);


    if(ladder_1==1)
    {

        ui->status1->setText("RUNNING");
        qApp->processEvents();
        ip_=ui->ip1->text();
        ip=ip_.toStdString();



        GETDT getDT;
        fileName=FileName;
        ladheader1=0x01cccccc;
        rbuffer_1.clean();
        //ladder_1=0;


        auto f =  std::bind(&GETDT::getdt, &getDT,&rbuffer_1,ip,&ctrflag1,&starflag1);

        Analyse anaDT;
        auto f2 = std::bind(&Analyse::analysedt,&anaDT,&rbuffer_1,getdtNum,fileName,ip,&ctrflag1,&starflag1,&ladheader1,&packnum1,&Brbuffer1);

        fflush(stdout);
        std::thread Gid1(f);
        std::thread Aid1(f2);


        Gid1.detach();
        Aid1.detach();

    }

    if(ladder_2==1)
    {
        ui->status2->setText("RUNNING");
        ip_=ui->ip2->text();
        ip=ip_.toStdString();



        GETDT getDT;
        fileName=FileName;
        ladheader2=0x02cccccc;
        rbuffer_2.clean();
        //ladder_2=0;


        auto f =  std::bind(&GETDT::getdt, &getDT,&rbuffer_2,ip,&ctrflag2,&starflag2);

        Analyse anaDT;
        auto f2 = std::bind(&Analyse::analysedt,&anaDT,&rbuffer_2,getdtNum,fileName,ip,&ctrflag2,&starflag2,&ladheader2,&packnum2,&Brbuffer2);


        std::thread Gid2(f);
        std::thread Aid2(f2);


        Gid2.detach();
        Aid2.detach();


    }



    if(ladder_3==1)
    {
        ui->status3->setText("RUNNING");
        ip_=ui->ip3->text();
        ip=ip_.toStdString();

        GETDT getDT;
        fileName=FileName;
        ladheader3=0x03cccccc;
        rbuffer_3.clean();
        // ladder_3==0;


        auto f =  std::bind(&GETDT::getdt, &getDT,&rbuffer_3,ip,&ctrflag3,&starflag3);

        Analyse anaDT;
        auto f2 = std::bind(&Analyse::analysedt,&anaDT,&rbuffer_3,getdtNum,fileName,ip,&ctrflag3,&starflag3,&ladheader3,&packnum3,&Brbuffer3);

        fflush(stdout);
        std::thread Gid3(f);
        std::thread Aid3(f2);


        Gid3.detach();
        Aid3.detach();

    }

    if(ladder_4==1)
    {
        ui->status4->setText("RUNNING");
        ip_=ui->ip4->text();
        ip=ip_.toStdString();



        GETDT getDT;
        fileName=FileName;
        ladheader4=0x04cccccc;
        rbuffer_4.clean();
        //ladder_4==0;

        auto f =  std::bind(&GETDT::getdt, &getDT,&rbuffer_4,ip,&ctrflag4,&starflag4);

        Analyse anaDT;
        auto f2 = std::bind(&Analyse::analysedt,&anaDT,&rbuffer_4,getdtNum,fileName,ip,&ctrflag4,&starflag4,&ladheader4,&packnum4,&Brbuffer4);

        fflush(stdout);
        std::thread Gid4(f);
        std::thread Aid4(f2);

        Gid4.detach();
        Aid4.detach();

    }

    if(ladder_5==1)
    {
        ui->status5->setText("RUNNING");
        ip_=ui->ip5->text();
        ip=ip_.toStdString();


        GETDT getDT;
        fileName=FileName;
        ladheader5=0x05cccccc;
        rbuffer_5.clean();
        //ladder_5==0;


        auto f =  std::bind(&GETDT::getdt, &getDT,&rbuffer_5,ip,&ctrflag5,&starflag5);

        Analyse anaDT;
        auto f2 = std::bind(&Analyse::analysedt,&anaDT,&rbuffer_5,getdtNum,fileName,ip,&ctrflag5,&starflag5,&ladheader5,&packnum5,&Brbuffer5);

        fflush(stdout);
        std::thread Gid5(f);
        std::thread Aid5(f2);



        Gid5.detach();
        Aid5.detach();
    }
   emit state("Running");

    sleep(1);
    printf("*********************SEND START SIGNAL**************************::\n");
    fflush(stdout);
    string ccip="192.168.10.16";
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;
    //start
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf5;
    s="start successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ccip);
    bzero(Wdata,10);

    usleep(1);
    //clear start
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf5;
    s="clear start successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ccip);
    bzero(Wdata,10);


    auto ERATE =  std::bind(&MainWindow::event_rate, this);
    std::thread eventrateshow(ERATE);
    eventrateshow.detach();

    auto Stime =  std::bind(&MainWindow::stoptime, this);
    std::thread stimeshow(Stime);
    stimeshow.detach();

    //        auto fbuilt=  std::bind(&MainWindow::assemble, this);
    //           std::thread DataBuilt(fbuilt);
    //           DataBuilt.detach();

}
int MainWindow::assemble()
{

}
int MainWindow::stoptime()
{
     totaltime=0;
    while(stopsgn!=(ladder_1+ladder_2+ladder_3+ladder_4+ladder_5))
    {
        QDateTime time_2 = QDateTime::currentDateTime();
        QString stoptime = time_2.toString(" hh:mm:ss ");
        ui->stoptime->setText(stoptime);
        sleep(1);
        totaltime++;
    }
         emit state("Waiting");
    std::string str=FileName;
    datassmble( str,totaltime);
       emit prompt("DAQ FINISH!!!");

          emit state("Ready");
       ui->config1->setEnabled(true);
       ui->config2->setEnabled(true);
       ui->config3->setEnabled(true);
       ui->config4->setEnabled(true);
       ui->config5->setEnabled(true);
       ui->start->setEnabled(true);
       ui->status1->setText("Finish!");
       ui->status2->setText("Finish!");
       ui->status3->setText("Finish!");
       ui->status4->setText("Finish!");
       ui->status5->setText("Finish!");


}

void MainWindow::event_rate()
{
    //sleep(10);
    while(packnum1<getdtNum)
    {

        int a=0;
        EventA=(packnum1+packnum2+packnum3+packnum4+packnum5)/(ladder_1+ladder_2+ladder_3+ladder_4+ladder_5);
        sleep(3);
        EventB=(packnum1+packnum2+packnum3+packnum4+packnum5)/(ladder_1+ladder_2+ladder_3+ladder_4+ladder_5);
       while(EventA==EventB)
        {
            sleep(1);
            EventB=(packnum1+packnum2+packnum3+packnum4+packnum5)/(ladder_1+ladder_2+ladder_3+ladder_4+ladder_5);
            a++;
        }

        Event_rate=(double)(EventB-EventA)/(double)(2*(3+a));
        QString eventnom;
        ui->currenteventnumber->setText(eventnom.setNum( Event_rate , 'g',  6));
        qApp->processEvents();
    }

}

int MainWindow::on_stop_clicked()
{
    //stop
//    ui->start->setEnabled(true);
//    ui->config1->setEnabled(true);
//    ui->config2->setEnabled(true);
//    ui->config3->setEnabled(true);
//    ui->config4->setEnabled(true);
//    ui->config5->setEnabled(true);
    string ccip="192.168.10.16";
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;
    //stop
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf6;
    s="stop successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ccip);
    bzero(Wdata,10);

    usleep(1);
    //clear start
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf6;
    s="clear stop successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ccip);
    bzero(Wdata,10);

    sleep(1);
    ctrflag1=1,ctrflag2=1,ctrflag3=1,ctrflag4=1,ctrflag5=1;
    starflag1=1,starflag2=1,starflag3=1,starflag4=1,starflag5=1;
    printf("*********************SEND STop SIGNAL**************************::\n");
    fflush(stdout);
    std::string str=FileName;
    datassmble( str,totaltime);
     emit state("Stop!please Restart!");




}

void MainWindow::on_ROOT_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,tr("open Directory"),
                                                      "/home/daq/Desktop/Mapsdata/");
        std::string str = dir.toStdString();

        QString ip_=ui->ip1->text();
        string ip=ip_.toStdString();

        string ipfile=str+"/"+ip;
        string ipfiledat=ipfile+"/";

        QString ip_2=ui->ip2->text();
        string ip2=ip_2.toStdString();

        string ipfile2=str+"/"+ip2;
        string ipfiledat2=ipfile2+"/";

        QString ip_3=ui->ip3->text();
        string ip3=ip_3.toStdString();

        string ipfile3=str+"/"+ip3;
        string ipfiledat3=ipfile3+"/";

        QString ip_4=ui->ip4->text();
        string ip4=ip_4.toStdString();

        string ipfile4=str+"/"+ip4;
        string ipfiledat4=ipfile4+"/";

        QString ip_5=ui->ip5->text();
        string ip5=ip_5.toStdString();
        string ipfile5=str+"/"+ip5;
        string ipfiledat5=ipfile5+"/";

        fflush(stdout);

        fstream file("photoshow.cc");
        string line;
        int count=0;
        ofstream outfile("temp.dat",ios::out|ios::trunc);
        while(!file.eof())
        {
            getline(file,line);
            if(count==107)
            {
                outfile<<"\""<< ipfiledat<<"\"";
                outfile<<endl;
            }
            if(count==109)
            {
                outfile<<"\""<< ipfiledat2<<"\"";
                outfile<<endl;
            }
            if(count==111)
            {
                outfile<<"\""<< ipfiledat3<<"\"";
                outfile<<endl;
            }
            if(count==113)
            {
                outfile<<"\""<< ipfiledat4<<"\"";
                outfile<<endl;
            }
            if(count==115)
            {
                outfile<<"\""<< ipfiledat5<<"\"";
                outfile<<endl;
            }
            if ( (count!=107)&&(count!=109)&&(count!=111) &&(count!=113)&&(count!=115))
                outfile<<line<<endl;
            count++;
        }
        outfile.close();
        file.close();
        ofstream outfile1("photoshow.cc",ios::out|ios::trunc);
        fstream file1("temp.dat");
        while(!file1.eof())
        {
            getline(file1,line);
            outfile1<<line<<endl;
        }
        outfile1.close();
        file1.close();
        system("rm temp.dat");
        system("./root-time.sh");
}


void MainWindow::on_ROOT_3_clicked()
{
    QString ip_=ui->ip1->text();
    string ip=ip_.toStdString();
    string checkfilename=FileName;
    string ipfile=checkfilename+"/"+ip;
    string ipfiledat=ipfile+"/";

    QString ip_2=ui->ip2->text();
    string ip2=ip_2.toStdString();
    string checkfilename2=FileName;
    string ipfile2=checkfilename2+"/"+ip2;
    string ipfiledat2=ipfile2+"/";

    QString ip_3=ui->ip3->text();
    string ip3=ip_3.toStdString();
    string checkfilename3=FileName;
    string ipfile3=checkfilename3+"/"+ip3;
    string ipfiledat3=ipfile3+"/";

    QString ip_4=ui->ip4->text();
    string ip4=ip_4.toStdString();
    string checkfilename4=FileName;
    string ipfile4=checkfilename4+"/"+ip4;
    string ipfiledat4=ipfile4+"/";

    QString ip_5=ui->ip5->text();
    string ip5=ip_5.toStdString();
    string checkfilename5=FileName;
    string ipfile5=checkfilename5+"/"+ip5;
    string ipfiledat5=ipfile5+"/";

    fflush(stdout);

    fstream file("photoshow.cc");
    string line;
    int count=0;
    ofstream outfile("temp.dat",ios::out|ios::trunc);
    while(!file.eof())
    {
        getline(file,line);
        if(count==107)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat<<"\"";
            outfile<<endl;
        }
        if(count==109)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat2<<"\"";
            outfile<<endl;
        }
        if(count==111)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat3<<"\"";
            outfile<<endl;
        }
        if(count==113)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat4<<"\"";
            outfile<<endl;
        }
        if(count==115)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat5<<"\"";
            outfile<<endl;
        }
        if ( (count!=107)&&(count!=109)&&(count!=111) &&(count!=113)&&(count!=115))
            outfile<<line<<endl;
        count++;
    }
    outfile.close();
    file.close();
    ofstream outfile1("photoshow.cc",ios::out|ios::trunc);
    fstream file1("temp.dat");
    while(!file1.eof())
    {
        getline(file1,line);
        outfile1<<line<<endl;
    }
    outfile1.close();
    file1.close();
    system("rm temp.dat");
    system("./root-time.sh");

}

bool MainWindow::isIpFormatWrong(const char*  ipAddress)
{
    //判断IP地址是否合法
    int a=-1,b=-1,c=-1,d=-1;
    if ((sscanf(ipAddress,"%d.%d.%d.%d",&a,&b,&c,&d)==4)
            &&(a>=0&&a<=255)
            &&(b>=0&&b<=255)
            &&(c>=0&&c<=255)
            &&(d>=0&&d<=255))
    {
        return false;
    }
    printf("a=%d,b=%d.c=%d.d=%d",a,b,c,d);
    fflush(stdout);
    return  true;
}

int MainWindow::on_config1_clicked()
{
      ui->config1->setEnabled(false);
    QString ip_=ui->ip1->text();
    string ip=ip_.toStdString();
    const char* ipAddress=ip.c_str();
    if (isIpFormatWrong(  ipAddress) )
    {
        emit prompt("LADDER 1 IP IS WRONG");
        return 0;
    }

    QString threshold=ui->threshold1->text();
    bool ok;
    int volum;
    if(threshold!=NULL)
    {
        volum=threshold.toUInt(&ok,10);
    }
    //----loadconfig
    ui->status1->setText("loadconfig");
    loadconfig->loadfile(config1,1,volum); emit prompt("load 1.dat,ok");
    loadconfig->loadfile(config2,2,volum);emit prompt("load 2.dat,ok");
    qApp->processEvents();

    //----reset
    ui->status1->setText("reset");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;

    //1:
    //reset
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    sleep(1);
    printf("reset\n");
    fflush(stdout);

    //clear reset
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="clear reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    printf("clear reset\n");
    fflush(stdout);

    //----config
    ui->status1->setText("config");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;

    //send command 1
    Rdatalength=193;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 1 successed ";
    rcfgpck->rcfgpck(config1,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 1
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 1 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 1
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 1 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 1 configed!";
    loadconfig->checkdata(s,ip);
    //2:
    //send command 2
    Rdatalength=193;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 2 successed ";
    rcfgpck->rcfgpck(config2,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 2
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 2 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //3
    //clear config 2
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 2 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 2 configed!";
    loadconfig->checkdata(s,ip);

    //------sensor select
    ui->status1->setText("sensor select");
    QString senID=ui->sensor1->text();
    unsigned int SenId=0;
    if(senID!=NULL)
    {
        SenId=senID.toUInt(&ok,2);
    }
    printf("senId: %x\n",SenId);
    fflush(stdout);


    //select sensor
    Wdata[0]=(char)(SenId&0xff);
    Wdata[1]=(char)((SenId&0xff00)>>8);
    printf("sen8=%x\n",Wdata[0]);
    fflush(stdout);
    printf("sen9=%x\n",Wdata[1]);
    fflush(stdout);
    Rdatalength=2;
    header[3]=Rdatalength;
    header[7]=0xf3;
    s="select sensor successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);

    ui->status1->setText("READ");
    ladder_1=1;
    return 1;
}

int MainWindow::on_config2_clicked()
{
     ui->config2->setEnabled(false);
    QString ip_=ui->ip2->text();
    string ip=ip_.toStdString();
    const char* ipAddress=ip.c_str();
    if (isIpFormatWrong(  ipAddress) )
    {
        emit prompt("LADDER 2 IP IS WRONG");
        return 0;
    }

    QString threshold=ui->threshold2->text();
    bool ok;
    int volum;
    if(threshold!=NULL)
    {
        volum=threshold.toUInt(&ok,10);
    }
    //----loadconfig
    ui->status2->setText("loadconfig");
    loadconfig->loadfile(config3,3,volum); emit prompt("load 3.dat,ok");
    loadconfig->loadfile(config4,4,volum);emit prompt("load 4.dat,ok");
    qApp->processEvents();

    //----reset
    ui->status2->setText("reset");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;

    //1:
    //reset
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    sleep(1);
    printf("reset\n");
    fflush(stdout);

    //clear reset
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="clear reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    printf("clear reset\n");
    fflush(stdout);

    //----config
    ui->status2->setText("config");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;

    //send command 3
    Rdatalength=193;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 3 successed ";
    rcfgpck->rcfgpck(config3,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 3
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 3 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 3
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 3 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 3 configed!";
    loadconfig->checkdata(s,ip);
    //2:
    //send command 4
    Rdatalength=193;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 4 successed ";
    rcfgpck->rcfgpck(config4,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 4
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 4 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //3
    //clear config 4
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 4 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 4 configed!";
    loadconfig->checkdata(s,ip);

    //------sensor select
    ui->status2->setText("sensor select");
    QString senID=ui->sensor2->text();
    unsigned int SenId=0;
    if(senID!=NULL)
    {
        SenId=senID.toUInt(&ok,2);
    }
    printf("senId: %x\n",SenId);
    fflush(stdout);
    //    unsigned char sen0=0;//lowbits
    //    unsigned char sen1=0;//highbits
    //    sen0=(char)(SenId&0xff);
    //    sen1=(char)((SenId&0xff00)>>8);
    //    printf("sen0=%x\n",sen0);
    //    fflush(stdout);
    //    printf("sen1=%x\n",sen1);
    //    fflush(stdout);

    //select sensor
    Wdata[0]=(char)(SenId&0xff);
    Wdata[1]=(char)((SenId&0xff00)>>8);
    printf("sen8=%x\n",Wdata[0]);
    fflush(stdout);
    printf("sen9=%x\n",Wdata[1]);
    fflush(stdout);
    Rdatalength=2;
    header[3]=Rdatalength;
    header[7]=0xf3;
    s="select sensor successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);

    ui->status2->setText("READ");
    ladder_2=1;
    return 1;
}

int MainWindow::on_config3_clicked()
{
     ui->config3->setEnabled(false);
    QString ip_=ui->ip3->text();
    string ip=ip_.toStdString();
    const char* ipAddress=ip.c_str();
    if (isIpFormatWrong(  ipAddress) )
    {
        emit prompt("LADDER 3 IP IS WRONG");
        return 0;
    }


    QString threshold=ui->threshold3->text();
    bool ok;
    int volum;
    if(threshold!=NULL)
    {
        volum=threshold.toUInt(&ok,10);
    }
    //----loadconfig
    ui->status1->setText("loadconfig");
    loadconfig->loadfile(config5,5,volum); emit prompt("load 5.dat,ok");
    loadconfig->loadfile(config6,6,volum);emit prompt("load 6.dat,ok");
    loadconfig->loadfile(config7,7,volum);emit prompt("load 7.dat,ok");
    loadconfig->loadfile(config8,8,volum);emit prompt("load 8.dat,ok");
    loadconfig->loadfile(config9,9,volum);emit prompt("load 9.dat,ok");
    loadconfig->loadfile(config10,10,volum);emit prompt("load 10.dat,ok");
    loadconfig->loadfile(config11,11,volum);emit prompt("load 11.dat,ok");
    loadconfig->loadfile(config12,12,volum);emit prompt("load 12.dat,ok");
    loadconfig->loadfile(config13,13,volum);emit prompt("load 13.dat,ok");
    loadconfig->loadfile(config14,14,volum);emit prompt("load 14.dat,ok");
    qApp->processEvents();

    //----reset
    ui->status3->setText("reset");
    qApp->processEvents();
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;

    //1:
    //reset
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;  fflush(stdout);
    s="reset successed ";
    printf("reset\n");
    fflush(stdout);
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    sleep(1);

    //clear reset
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="clear reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    printf("clear reset\n");
    fflush(stdout);

    //----config
    ui->status3->setText("config");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;

    //2:
    //send command 5
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 5 successed ";
    rcfgpck->rcfgpck(config5,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 5
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 5 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //3
    //clear config 5
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 5 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 5 configed!";
    loadconfig->checkdata(s,ip);

    //send command 6
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 6 successed ";
    rcfgpck->rcfgpck(config6,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 6
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 6 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 6
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 6 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 6 configed!";
    loadconfig->checkdata(s,ip);

    //send command 7
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 7 successed ";
    rcfgpck->rcfgpck(config7,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 7
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 7 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 7
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 7 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 7 configed!";
    loadconfig->checkdata(s,ip);

    //send command 8
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 8 successed ";
    rcfgpck->rcfgpck(config8,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 8
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 8 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 8
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 8 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 8 configed!";
    loadconfig->checkdata(s,ip);

    //send command 9
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 9  successed ";
    rcfgpck->rcfgpck(config9,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 9
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 9  successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config  9
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 9 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 9 configed!";
    loadconfig->checkdata(s,ip);

    //send command 10
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 10 successed ";
    rcfgpck->rcfgpck(config10,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 10
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 10 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 10
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 10 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 10 configed!";
    loadconfig->checkdata(s,ip);

    //send command 11
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 11 successed ";
    rcfgpck->rcfgpck(config11,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 11
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 11 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 11
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 11 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 11 configed!";
    loadconfig->checkdata(s,ip);

    //send command 12
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 12 successed ";
    rcfgpck->rcfgpck(config12,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 12
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 12 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 12
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 12 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 12 configed!";
    loadconfig->checkdata(s,ip);

    //send command 13
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 13 successed ";
    rcfgpck->rcfgpck(config13,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 13
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 13 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 13
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 13 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 13 configed!";
    loadconfig->checkdata(s,ip);

    //send command 14
    Rdatalength=113;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 14 successed ";
    rcfgpck->rcfgpck(config14,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 14
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 14 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 14
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 14 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 14 configed!";
    loadconfig->checkdata(s,ip);
    qApp->processEvents();

    //------sensor select
    ui->status3->setText("sensor select");
    QString senID=ui->sensor3->text();

    unsigned int SenId=0;
    if(senID!=NULL)
    {
        SenId=senID.toUInt(&ok,2);
    }
    printf("senId: %x\n",SenId);
    fflush(stdout);


    //select sensor
    Wdata[0]=(char)(SenId&0xff);
    Wdata[1]=(char)((SenId&0xff00)>>8);
    printf("sen8=%x\n",Wdata[0]);
    fflush(stdout);
    printf("sen9=%x\n",Wdata[1]);
    fflush(stdout);
    Rdatalength=2;
    header[3]=Rdatalength;
    header[7]=0xf3;
    s="select sensor successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    qApp->processEvents();

    ui->status3->setText("READ");
    ladder_3=1;
    return 1;
}

int MainWindow::on_config4_clicked()
{
     ui->config4->setEnabled(false);
    QString ip_=ui->ip4->text();
    string ip=ip_.toStdString();
    const char* ipAddress=ip.c_str();
    if (isIpFormatWrong(  ipAddress) )
    {
        emit prompt("LADDER 4 IP IS WRONG");
        return 0;
    }

    QString threshold=ui->threshold4->text();
    bool ok;
    int volum;
    if(threshold!=NULL)
    {
        volum=threshold.toUInt(&ok,10);
    }
    //----loadconfig
    ui->status4->setText("loadconfig");
    loadconfig->loadfile(config15,15,volum); emit prompt("load 15.dat,ok");
    loadconfig->loadfile(config16,16,volum);emit prompt("load 16.dat,ok");
    qApp->processEvents();

    //----reset
    ui->status4->setText("reset");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;

    //1:
    //reset
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    sleep(1);
    printf("reset\n");
    fflush(stdout);

    //clear reset
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="clear reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    printf("clear reset\n");
    fflush(stdout);

    //----config
    ui->status4->setText("config");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;


    //send command 15
    Rdatalength=193;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 15 successed ";
    rcfgpck->rcfgpck(config15,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 15
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 15 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 15
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 15 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 15 configed!";
    loadconfig->checkdata(s,ip);

    //2:
    //send command 16
    Rdatalength=193;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 16 successed ";
    rcfgpck->rcfgpck(config16,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 16
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 16 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //3
    //clear config 16
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 16 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 16 configed!";
    loadconfig->checkdata(s,ip);


    //------sensor select
    ui->status4->setText("sensor select");
    QString senID=ui->sensor4->text();

    unsigned int SenId=0;
    if(senID!=NULL)
    {
        SenId=senID.toUInt(&ok,2);
    }
    printf("senId: %x\n",SenId);
    fflush(stdout);
    //    unsigned char sen0=0;//lowbits
    //    unsigned char sen1=0;//highbits
    //    sen0=(char)(SenId&0xff);
    //    sen1=(char)((SenId&0xff00)>>8);
    //    printf("sen0=%x\n",sen0);
    //    fflush(stdout);
    //    printf("sen1=%x\n",sen1);
    //    fflush(stdout);

    //select sensor
    Wdata[0]=(char)(SenId&0xff);
    Wdata[1]=(char)((SenId&0xff00)>>8);
    printf("sen8=%x\n",Wdata[0]);
    fflush(stdout);
    printf("sen9=%x\n",Wdata[1]);
    fflush(stdout);
    Rdatalength=2;
    header[3]=Rdatalength;
    header[7]=0xf3;
    s="select sensor successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);

    ui->status4->setText("READ");
    ladder_4=1;
    return 1;
}

int MainWindow::on_config5_clicked()
{
     ui->config5->setEnabled(false);
    QString ip_=ui->ip5->text();
    string ip=ip_.toStdString();
    const char* ipAddress=ip.c_str();
    if (isIpFormatWrong(  ipAddress) )
    {
        emit prompt("LADDER 5 IP IS WRONG");
        return 0;
    }

    QString threshold=ui->threshold5->text();
    bool ok;
    int volum;
    if(threshold!=NULL)
    {
        volum=threshold.toUInt(&ok,10);
    }
    //----loadconfig
    ui->status5->setText("loadconfig");
    loadconfig->loadfile(config17,17,volum); emit prompt("load 17.dat,ok");
    loadconfig->loadfile(config18,18,volum);emit prompt("load 18.dat,ok");
    qApp->processEvents();

    //----reset
    ui->status5->setText("reset");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;

    //1:
    //reset
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    sleep(1);
    printf("reset\n");
    fflush(stdout);

    //clear reset
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf0;
    s="clear reset successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    printf("clear reset\n");
    fflush(stdout);

    //----config
    ui->status5->setText("config");
    header[0]=0xff;
    header[1]=0x80;
    header[2]=0x00;
    header[4]=0x00;
    header[5]=0x00;
    header[6]=0x00;


    //send command 17
    Rdatalength=193;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 17 successed ";
    rcfgpck->rcfgpck(config17,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 17
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 17 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //clear config 17
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 17 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 17 configed!";
    loadconfig->checkdata(s,ip);
    //2:
    //send command 18
    Rdatalength=193;
    header[3]=Rdatalength;
    header[7]=0x00;
    s="send command 18 successed ";
    rcfgpck->rcfgpck(config18,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    //config 18
    Wdata[0]=0x01,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="config 18 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    //3
    //clear config 18
    Wdata[0]=0x00,Rdatalength=1;
    header[3]=Rdatalength;
    header[7]=0xf1;
    s="clear config 18 successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);
    usleep(10);
    s="sesor 18 configed!";
    loadconfig->checkdata(s,ip);



    //------sensor select
    ui->status5->setText("sensor select");
    QString senID=ui->sensor5->text();

    unsigned int SenId=0;
    if(senID!=NULL)
    {
        SenId=senID.toUInt(&ok,2);
    }
    printf("senId: %x\n",SenId);
    fflush(stdout);
    //    unsigned char sen0=0;//lowbits
    //    unsigned char sen1=0;//highbits
    //    sen0=(char)(SenId&0xff);
    //    sen1=(char)((SenId&0xff00)>>8);
    //    printf("sen0=%x\n",sen0);
    //    fflush(stdout);
    //    printf("sen1=%x\n",sen1);
    //    fflush(stdout);

    //select sensor
    Wdata[0]=(char)(SenId&0xff);
    Wdata[1]=(char)((SenId&0xff00)>>8);
    printf("sen8=%x\n",Wdata[0]);
    fflush(stdout);
    printf("sen9=%x\n",Wdata[1]);
    fflush(stdout);
    Rdatalength=2;
    header[3]=Rdatalength;
    header[7]=0xf3;
    s="select sensor successed ";
    rcfgpck->rcfgpck(Wdata,header,Rdatalength,s,ip);
    bzero(Wdata,10);

    ui->status5->setText("READ");
    ladder_5=1;
    return 1;
}

void MainWindow::on_pushButton_clicked()
{
    qFileName=QFileDialog::getOpenFileName(this,tr("open file"),
                                           "/home/wy/Desktop/backup/MAPS_WUYE/MAPS_WUYE_DEbug/",tr("configfile (*.dat )"));
    ui->lineEdit->setText( qFileName);
    std::string str = qFileName.toStdString();
    const char* file_name = str.c_str();
    FILE * fp=fopen(file_name,"rb+");
    unsigned int count=0;
    if(fp!=NULL)
    {
        while(fscanf(fp,"%x",&udpconfig[count])!=EOF)
        {
            count=count+1;
            //  cout<<count<<udpconfig[count]<<" ";

        }
        if (count!=193)
        {
            ui->lineEdit->setText( "wrong data size");
            exit(1);
        }
    }
    char a[10];//weile udpconfig bu neicunyichu
    for(int i=0;i<5;i++)
    {
        sprintf(a,"%d",udpconfig[15+i]);
        QString vcount = QString(QLatin1String(a));
        if(i==0) ui->refd->setText(vcount);
        if(i==1) ui->refc->setText(vcount);
        if(i==2) ui->refb->setText(vcount);
        if(i==3) ui->refa->setText(vcount);
        if(i==4) ui->ref_v2->setText(vcount);
    }
    fclose(fp);
}


void MainWindow::on_pushButton_2_clicked()
{
    bool ok;
    QString vcounta=ui->refa->text();
    QString vcountb=ui->refb->text();
    QString vcountc=ui->refc->text();
    QString vcountd=ui->refd->text();
    QString vcounte=ui->ref_v2->text();

    unsigned long numa;
    if(vcounta!=NULL)
    {
        numa=vcounta.toUInt(&ok,10);
    }
    udpconfig[18]=numa;

    unsigned long numb;
    if(vcountb!=NULL)
    {
        numb=vcountb.toUInt(&ok,10);
    }
    udpconfig[17]=numb;

    unsigned long numc;
    if(vcountc!=NULL)
    {
        numc=vcountc.toUInt(&ok,10);
    }
    udpconfig[16]=numc;

    unsigned long numd;
    if(vcountd!=NULL)
    {
        numd=vcountd.toUInt(&ok,10);
    }
    udpconfig[15]=numd;

    unsigned long nume;
    if(vcounte!=NULL)
    {
        nume=vcounte.toUInt(&ok,10);
    }
    udpconfig[19]=nume;
    //    for(int i=0;i<4;i++)
    //    {
    //        printf("%i: %d\n",15+i,udpconfig[15+i]);
    //    }

    string filename;
    filename = qFileName.toStdString();

    fstream file(filename);
    //   fstream file("/home/wy/Desktop/MAPS/MAPS-DAQ/build-OnlineMaps-Desktop_Qt_5_10_1_GCC_64bit-Debug/1.dat");
    string line;
    int count=0;
    ofstream outfile("temp.dat",ios::out|ios::trunc);
    while(!file.eof())
    {
        getline(file,line);
        if(count==5)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            for(int i=15;i<20;i++)
            {
                char a[10];bzero(a,10);
                sprintf(a,"%x",udpconfig[i]);
                string s(a);
                outfile<<s<<" ";
            }
            outfile<<endl;
        }
        if(count!=5)
            outfile<<line<<endl;
        count++;
    }
    outfile.close();
    file.close();
    ofstream outfile1(filename,ios::out|ios::trunc);
    fstream file1("temp.dat");
    while(!file1.eof())
    {
        getline(file1,line);
        outfile1<<line<<endl;
    }
    outfile1.close();
    file1.close();
    system("rm temp.dat");
}



void MainWindow::on_ROOT_7_clicked()
{
    QString ip_=ui->ip1->text();
    string ip=ip_.toStdString();
    string checkfilename=FileName;
    string ipfile=checkfilename+"/"+ip;
    string ipfiledat=ipfile+"/";

    QString ip_2=ui->ip2->text();
    string ip2=ip_2.toStdString();
    string checkfilename2=FileName;
    string ipfile2=checkfilename2+"/"+ip2;
    string ipfiledat2=ipfile2+"/";

    QString ip_3=ui->ip3->text();
    string ip3=ip_3.toStdString();
    string checkfilename3=FileName;
    string ipfile3=checkfilename3+"/"+ip3;
    string ipfiledat3=ipfile3+"/";

    QString ip_4=ui->ip4->text();
    string ip4=ip_4.toStdString();
    string checkfilename4=FileName;
    string ipfile4=checkfilename4+"/"+ip4;
    string ipfiledat4=ipfile4+"/";

    QString ip_5=ui->ip5->text();
    string ip5=ip_5.toStdString();
    string checkfilename5=FileName;
    string ipfile5=checkfilename5+"/"+ip5;
    string ipfiledat5=ipfile5+"/";

    fflush(stdout);

    fstream file("photoshow.cc");
    string line;
    int count=0;
    ofstream outfile("temp.dat",ios::out|ios::trunc);
    while(!file.eof())
    {
        getline(file,line);
        if(count==107)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat<<"\"";
            outfile<<endl;
        }
        if(count==109)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat2<<"\"";
            outfile<<endl;
        }
        if(count==111)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat3<<"\"";
            outfile<<endl;
        }
        if(count==113)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat4<<"\"";
            outfile<<endl;
        }
        if(count==115)//如果要修改内容就在这修改line的内容,再存到文件中就行了
        {
            outfile<<"\""<< ipfiledat5<<"\"";
            outfile<<endl;
        }
        if ( (count!=107)&&(count!=109)&&(count!=111) &&(count!=113)&&(count!=115))
            outfile<<line<<endl;
        count++;
    }
    outfile.close();
    file.close();
    ofstream outfile1("photoshow.cc",ios::out|ios::trunc);
    fstream file1("temp.dat");
    while(!file1.eof())
    {
        getline(file1,line);
        outfile1<<line<<endl;
    }
    outfile1.close();
    file1.close();
    system("rm temp.dat");
    system("./root-time.sh");

}

int MainWindow::on_start_2_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,tr("open Directory"),
                                                  "/home/daq/Desktop/Mapsdata/");
    std::string str = dir.toStdString();

    uint8_t builtBuf[MAX_TOTAL_PACK];
    uint32_t builtL1id=0xffffffff;
    uint32_t builtFramecount;
    uint32_t builtLength =0;
    uint32_t lastBuiltL1id = 0xffffffff;

    struct lad_info_t{
        std::vector <uint32_t> frameCount;
    };

    lad_info_t lad_info[FEE_SUM];

    bool isFirst = true;
    bool toEnd[FEE_SUM];
    for(int i=0;i<FEE_SUM;i++)
    {
        toEnd[i] = false;
    }

    int curNum;

    FILE* fp[FEE_SUM];

    uint32_t fileCounter =0;

    while(1)
    {
        for(int i=0;i<FEE_SUM;i++)
        {
            toEnd[i] = false;
        }
        bool fileExist[FEE_SUM];
        for(int i=0;i<5;i++)
         {
                 fileExist[i]=true;
         }

        fileCounter = fileCounter+1;

        std::stringstream ss17;
        ss17<<str<<"/192.168.10.17/"<<fileCounter<<".dat";
        std::string str1_;
        ss17>>str1_;

        std::stringstream ss18;
        ss18<<str<<"/192.168.10.18/"<<fileCounter<<".dat";
        std::string str2_;
        ss18>>str2_;

        std::stringstream ss19;
        ss19<<str<<"/192.168.10.19/"<<fileCounter<<".dat";
        std::string str3_;
        ss19>>str3_;

        std::stringstream ss20;
        ss20<<str<<"/192.168.10.20/"<<fileCounter<<".dat";
        std::string str4_;
        ss20>>str4_;

        std::stringstream ss21;
        ss21<<str<<"/192.168.10.21/"<<fileCounter<<".dat";
        std::string str5_;
        ss21>>str5_;

        const char* str1 = str1_.c_str();
        const char* str2 = str2_.c_str();
        const char* str3 = str3_.c_str();
        const char* str4 = str4_.c_str();
        const char* str5 = str5_.c_str();

        fp[0]=fopen(str1,"rb+");
        fp[1]=fopen(str2,"rb+");
        fp[2]=fopen(str3,"rb+");
        fp[3]=fopen(str4,"rb+");
        fp[4]=fopen(str5,"rb+");

        bool notAllEnd = false;

        for (int i=0; i<=4 ;++i)
        {
            if(fp[i]!=NULL)
            {
                printf("ip:%d has been open;\n",17+i);
                fflush(stdout);
                notAllEnd = true;
            }

            else{
                printf("ip:%d doesn't exist;\n",17+i);
                toEnd[i]=true;
                fileExist[i]=false;
                fflush(stdout);
            }
        }

        if(!notAllEnd)
        {
            break;
        }
        else
        {
            //std::cout<<str1_<<std::endl;
            //std::cout<<str2_<<std::endl;
            //std::cout<<str3_<<std::endl;
            //std::cout<<str4_<<std::endl;
            //std::cout<<str5_<<std::endl;
        }

        uint8_t buf_1[MAX_LADDER_PACK];
        uint8_t buf_2[MAX_LADDER_PACK];
        uint8_t buf_3[MAX_LADDER_PACK];
        uint8_t buf_4[MAX_LADDER_PACK];
        uint8_t buf_5[MAX_LADDER_PACK];

        uint8_t * buf[FEE_SUM];

        buf[0]=&buf_1[0];
        buf[1]=&buf_2[0];
        buf[2]=&buf_3[0];
        buf[3]=&buf_4[0];
        buf[4]=&buf_5[0];

        std::vector <uint8_t > toBuild;
        for(uint8_t i=0;i<FEE_SUM;i++)
        {
            toBuild.push_back(i);
        }

        const char* str6 = str.c_str();
        char runname[50];
        strncpy(runname,str6+27,strlen(str6)-27);
        string rname=runname;

        string strbuild=str+"/"+rname+"_FullEvent.dat";
        const char* builtfileName = strbuild.c_str();

        FILE *fpr = fopen( builtfileName,"ab+");//FIXME
        //FILE * fpr = fopen("/home/daq/luxx/a/RUN27_2018-10-17-21-49-43/built","ab+");
        //std::cout<<"open a file"<<builtfileName<<std::endl;


        while(1)//FIXME
        {
            std::vector<uint8_t>::iterator ior;

            for(ior=toBuild.begin();ior!=toBuild.end();ior++)
            {

                uint8_t i = *ior;
                if(toEnd[i])
                {
                }
                else
                {
                    uint8_t count=fread(buf[i],1,16,fp[i]);

                    if(count!=16)
                    {
                        printf("%02x",i);
                        toEnd[i] = true;

                    }
                }
            }

            uint8_t endCount = 0;
            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                    endCount = endCount+1;
                }
            }
            if(endCount==FEE_SUM)
            {
                break;
            }

            struct info_t{
                uint32_t l1id;
                uint32_t frameCount;
                uint32_t length;
            };

            info_t info[FEE_SUM];

            for(ior=toBuild.begin();ior!=toBuild.end();ior++)
            {
                uint8_t i=*ior;
                if(toEnd[i])
                {
                    // break;
                }
                else
                {
                    info[i].l1id =( (*(uint8_t*)(buf[i]+7)) <<24)|((*(uint8_t*) (buf[i]+6))<<16) | ((*(uint8_t*)(buf[i]+5))<<8) | (*(uint8_t*)(buf[i]+4));//FIXME

                    info[i].frameCount  = ( (*(uint8_t*)(buf[i]+11)) <<24)|((*(uint8_t*) (buf[i]+10))<<16) | ((*(uint8_t*)(buf[i]+9))<<8) | (*(uint8_t*)(buf[i]+8));//FIXME

                    uint16_t length1 =( (*(uint8_t*)(buf[i]+13))<<8)|(*(uint8_t*)(buf[i]+12));
                    uint16_t length2 =( (*(uint8_t*)(buf[i]+15))<<8)|(*(uint8_t*)(buf[i]+14));

                    info[i].length = (length1+length2)*2+4;


                }
            }

            //std::cout<<"step 2"<<std::endl;

            for(ior=toBuild.begin();ior!=toBuild.end();ior++)
            {
                uint8_t i=*ior;
                if(toEnd[i])
                {
                    // break;
                }
                else
                {
                    uint32_t counter = fread((buf[i]+16),1,info[i].length,fp[i]);
                    if(counter!=info[i].length)
                    {
                        printf("%02x",i);
                        std::cout<<"to end"<<std::endl;
                        toEnd[i] = true;
                    }
                }
            }

            endCount = 0;;
            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                    endCount = endCount+1;
                }
            }
            if(endCount==FEE_SUM)
            {
                std::cout<<"to all file end"<<std::endl;
                break;
            }

            //printf("vector size 2 is %d\n",toBuild.size());
            //build policy
            toBuild.clear();
            uint32_t targetL1id ;
            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                }
                else
                {
                    targetL1id = info[i].l1id;
                    toBuild.push_back(i);
                    break;
                }
            }

            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                }
                else if (info[i].l1id<targetL1id)
                {
                    targetL1id = info[i].l1id;
                    toBuild.clear();//erase all toBuild
                    toBuild.push_back(i);
                }
                else if (info[i].l1id == targetL1id)
                {
                }
                else
                {
                }
            }


            //printf("vector size 3 is %d\n",toBuild.size());

            if(builtL1id!=targetL1id)
            {
                uint8_t headbuf[14];
                for(int i=0;i<4;i++)
                {
                    headbuf[i]=0xdd;
                }
                headbuf[4]=(uint8_t)(builtL1id&0x000000ff);
                headbuf[5]=(uint8_t)((builtL1id&0x0000ff00)>>8);
                headbuf[6]=(uint8_t)((builtL1id&0x00ff0000)>>16);
                headbuf[7]=(uint8_t)((builtL1id&0xff000000)>>24);
                headbuf[8]=(uint8_t)(builtLength);
                headbuf[9]=(uint8_t)(builtLength>>8);
                headbuf[10]=(uint8_t)(builtLength>>16);
                headbuf[11]=(uint8_t)(builtLength>>24);
                headbuf[12]=0x00;
                headbuf[13]=0x00;
                bool isAllLack = false;
                bool isLackOne = false;
                bool notContinue = false;
                bool notSame = false;
                //judge if frame for different IP not the same//FIXME: if 3 in one pack, not considered
                //uint32_t frame;// = lad_info[0].frameCount[0];
                std::vector <uint32_t > frame;
                for(int i=0;i<FEE_SUM;i++)
                {
                    if(lad_info[i].frameCount.size()==0)
                    {
                        isAllLack = true;//all lack
                    }
                    else if (lad_info[i].frameCount.size()==1)
                    {
                        isLackOne = true;
                    }
                    else if(lad_info[i].frameCount.size()!=2)
                    {
                        std::cout<<"strange pack num for one lad";
                        std::cout<<"lad builtL1id is ";
                        printf("%x\n",builtL1id);
                        std::cout<<"seq is";
                        printf("%d\n",i);
                        printf("%d\n",lad_info[i].frameCount.size());
                    }
                    else
                    {
                        if (lad_info[i].frameCount[1]!=(lad_info[i].frameCount[0]+1))
                        {
                            notContinue = true;
                        }
                        //if(lad_info[i].frameCount[0]!=frame)
                        else
                        {
                            frame.push_back(lad_info[i].frameCount[0]);
                        }
                    }
                }
                std::vector <uint32_t >::iterator itt;
                if(frame.size()>1)
                {
                    for(itt=frame.begin();itt!=frame.end()-1;itt++)
                    {
                        if((*itt)!=(*(itt+1)))
                        {
                            notSame = true;
                        }
                    }
                }
                //set status
                if(isAllLack)
                {
                    headbuf[13]=headbuf[13]|0x01;
                }
                if(isLackOne)
                {
                    headbuf[12]=headbuf[12]|0x08;
                }
                if(notContinue)
                {
                    headbuf[12]=headbuf[12]|0x02;
                }
                if(notSame)
                {
                    headbuf[12]=headbuf[12]|0x01;
                }
                for(int i=0;i<FEE_SUM;i++)
                {
                    lad_info[i].frameCount.clear();
                }
                if(!isFirst)
                {
                    int retV=fwrite(headbuf,1,14,fpr);
                    if(retV!=14)
                    {
                        std::cout<<"fwrite return wront val"<<retV<<std::endl;
                        std::cout<<"fpr is "<<fpr<<std::endl;
                    }
                    fwrite(builtBuf, builtLength,1,fpr);
                    uint8_t tailbuf[4];
                    for(int i=0;i<4;i++)
                    {
                        tailbuf[i]=0x66;
                    }
                    fwrite(tailbuf,4,1,fpr);
                    builtLength=0;
                }
                else
                {
                    isFirst = false;
                    builtLength = 0;
                }
            }

            //for toBuild
            //TODO:write header

            for(ior=toBuild.begin();ior!=toBuild.end();ior++)
            {
                uint8_t i = *ior;
                if(toEnd[i])
                {
                    break;
                }

                else
                {
                    builtL1id = info[i].l1id;
                    uint32_t tarLen=16+info[i].length;
                    memcpy(builtBuf+builtLength,(uint8_t*)(buf[i]),tarLen);
                    builtLength = builtLength+tarLen;
                    lad_info[i].frameCount.push_back(builtL1id);
                }
            }
            //TODO:write tail

            //std::cout<<"one done"<<std::endl;

            endCount = 0;;
            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                    endCount = endCount+1;
                }
            }
            if(endCount==FEE_SUM)
            {
                std::cout<<"all done"<<std::endl;
                break;
            }
            // printf("vector size 4 is %d\n",toBuild.size());

        }
        //write the last pack
        uint8_t headbuf[14];
        uint8_t tailbuf[4];
        for(int i=0;i<4;i++)
        {
            headbuf[i]=0xdd;
            tailbuf[i]=0x66;
        }
        headbuf[4]=(uint8_t)(builtL1id&0x000000ff);
        headbuf[5]=(uint8_t)((builtL1id&0x0000ff00)>>8);
        headbuf[6]=(uint8_t)((builtL1id&0x00ff0000)>>16);
        headbuf[7]=(uint8_t)((builtL1id&0xff000000)>>24);
        headbuf[8]=(uint8_t)(builtLength);
        headbuf[9]=(uint8_t)(builtLength>>8);
        headbuf[10]=(uint8_t)(builtLength>>16);
        headbuf[11]=(uint8_t)(builtLength>>24);
        headbuf[12]=0x00;
        headbuf[13]=0x00;
        fwrite(headbuf,14,1,fpr);
        fwrite(builtBuf, builtLength,1,fpr);
        fwrite(tailbuf,4,1,fpr);
        builtLength=0;
        fclose(fpr);

        for(int i=0;i<FEE_SUM;i++)
        {
            if(fileExist[i])
            {
                fclose(fp[i]);
             }
        }
        std::cout<<"one done"<<std::endl;
    }//while(1)for file looping
    //fclose(fpr);
    std::cout<<"built succeed"<<std::endl;

    return 1;
}
void MainWindow::datassmble( std::string str, int totaltime )
{
    uint8_t builtBuf[MAX_TOTAL_PACK];
    uint32_t builtL1id=0xffffffff;
    uint32_t builtFramecount;
    uint32_t builtLength =0;
    uint32_t lastBuiltL1id = 0xffffffff;

    struct lad_info_t{
        std::vector <uint32_t> frameCount;
    };

    lad_info_t lad_info[FEE_SUM];

    bool isFirst = true;
    bool toEnd[FEE_SUM];
    for(int i=0;i<FEE_SUM;i++)
    {
        toEnd[i] = false;
    }

    int curNum;

    FILE* fp[FEE_SUM];

    uint32_t fileCounter =0;

    while(1)
    {
        for(int i=0;i<FEE_SUM;i++)
        {
            toEnd[i] = false;
        }
        bool fileExist[FEE_SUM];
        for(int i=0;i<5;i++)
         {
                 fileExist[i]=true;
         }

        fileCounter = fileCounter+1;

        std::stringstream ss17;
        ss17<<str<<"/192.168.10.17/"<<fileCounter<<".dat";
        std::string str1_;
        ss17>>str1_;

        std::stringstream ss18;
        ss18<<str<<"/192.168.10.18/"<<fileCounter<<".dat";
        std::string str2_;
        ss18>>str2_;

        std::stringstream ss19;
        ss19<<str<<"/192.168.10.19/"<<fileCounter<<".dat";
        std::string str3_;
        ss19>>str3_;

        std::stringstream ss20;
        ss20<<str<<"/192.168.10.20/"<<fileCounter<<".dat";
        std::string str4_;
        ss20>>str4_;

        std::stringstream ss21;
        ss21<<str<<"/192.168.10.21/"<<fileCounter<<".dat";
        std::string str5_;
        ss21>>str5_;

        const char* str1 = str1_.c_str();
        const char* str2 = str2_.c_str();
        const char* str3 = str3_.c_str();
        const char* str4 = str4_.c_str();
        const char* str5 = str5_.c_str();

        fp[0]=fopen(str1,"rb+");
        fp[1]=fopen(str2,"rb+");
        fp[2]=fopen(str3,"rb+");
        fp[3]=fopen(str4,"rb+");
        fp[4]=fopen(str5,"rb+");

        bool notAllEnd = false;

        for (int i=0; i<=4 ;++i)
        {
            if(fp[i]!=NULL)
            {
                printf("ip:%d has been open;\n",17+i);
                fflush(stdout);
                notAllEnd = true;
            }

            else{
                printf("ip:%d doesn't exist;\n",17+i);
                toEnd[i]=true;
                fileExist[i]=false;
                fflush(stdout);
            }
        }

        if(!notAllEnd)
        {
            break;
        }
        else
        {
            //std::cout<<str1_<<std::endl;
            //std::cout<<str2_<<std::endl;
            //std::cout<<str3_<<std::endl;
            //std::cout<<str4_<<std::endl;
            //std::cout<<str5_<<std::endl;
        }

        uint8_t buf_1[MAX_LADDER_PACK];
        uint8_t buf_2[MAX_LADDER_PACK];
        uint8_t buf_3[MAX_LADDER_PACK];
        uint8_t buf_4[MAX_LADDER_PACK];
        uint8_t buf_5[MAX_LADDER_PACK];

        uint8_t * buf[FEE_SUM];

        buf[0]=&buf_1[0];
        buf[1]=&buf_2[0];
        buf[2]=&buf_3[0];
        buf[3]=&buf_4[0];
        buf[4]=&buf_5[0];

        std::vector <uint8_t > toBuild;
        for(uint8_t i=0;i<FEE_SUM;i++)
        {
            toBuild.push_back(i);
        }

        const char* str6 = str.c_str();
        char runname[50];
        strncpy(runname,str6+27,strlen(str6)-27);
        string rname=runname;

        string strbuild=str+"/"+rname+"_FullEvent.dat";
        const char* builtfileName = strbuild.c_str();

        FILE *fpr = fopen( builtfileName,"ab+");//FIXME
        //FILE * fpr = fopen("/home/daq/luxx/a/RUN27_2018-10-17-21-49-43/built","ab+");
        //std::cout<<"open a file"<<builtfileName<<std::endl;


        while(1)//FIXME
        {
            std::vector<uint8_t>::iterator ior;

            for(ior=toBuild.begin();ior!=toBuild.end();ior++)
            {

                uint8_t i = *ior;
                if(toEnd[i])
                {
                }
                else
                {
                    uint8_t count=fread(buf[i],1,16,fp[i]);

                    if(count!=16)
                    {
                        printf("%02x",i);
                        toEnd[i] = true;

                    }
                }
            }

            uint8_t endCount = 0;
            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                    endCount = endCount+1;
                }
            }
            if(endCount==FEE_SUM)
            {
                break;
            }

            struct info_t{
                uint32_t l1id;
                uint32_t frameCount;
                uint32_t length;
            };

            info_t info[FEE_SUM];

            for(ior=toBuild.begin();ior!=toBuild.end();ior++)
            {
                uint8_t i=*ior;
                if(toEnd[i])
                {
                    // break;
                }
                else
                {
                    info[i].l1id =( (*(uint8_t*)(buf[i]+7)) <<24)|((*(uint8_t*) (buf[i]+6))<<16) | ((*(uint8_t*)(buf[i]+5))<<8) | (*(uint8_t*)(buf[i]+4));//FIXME

                    info[i].frameCount  = ( (*(uint8_t*)(buf[i]+11)) <<24)|((*(uint8_t*) (buf[i]+10))<<16) | ((*(uint8_t*)(buf[i]+9))<<8) | (*(uint8_t*)(buf[i]+8));//FIXME

                    uint16_t length1 =( (*(uint8_t*)(buf[i]+13))<<8)|(*(uint8_t*)(buf[i]+12));
                    uint16_t length2 =( (*(uint8_t*)(buf[i]+15))<<8)|(*(uint8_t*)(buf[i]+14));

                    info[i].length = (length1+length2)*2+4;


                }
            }

            //std::cout<<"step 2"<<std::endl;

            for(ior=toBuild.begin();ior!=toBuild.end();ior++)
            {
                uint8_t i=*ior;
                if(toEnd[i])
                {
                    // break;
                }
                else
                {
                    uint32_t counter = fread((buf[i]+16),1,info[i].length,fp[i]);
                    if(counter!=info[i].length)
                    {
                        printf("%02x",i);
                        std::cout<<"to end"<<std::endl;
                        toEnd[i] = true;
                    }
                }
            }

            endCount = 0;;
            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                    endCount = endCount+1;
                }
            }
            if(endCount==FEE_SUM)
            {
                std::cout<<"to all file end"<<std::endl;
                break;
            }

            //printf("vector size 2 is %d\n",toBuild.size());
            //build policy
            toBuild.clear();
            uint32_t targetL1id ;
            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                }
                else
                {
                    targetL1id = info[i].l1id;
                    toBuild.push_back(i);
                    break;
                }
            }

            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                }
                else if (info[i].l1id<targetL1id)
                {
                    targetL1id = info[i].l1id;
                    toBuild.clear();//erase all toBuild
                    toBuild.push_back(i);
                }
                else if (info[i].l1id == targetL1id)
                {
                }
                else
                {
                }
            }


            //printf("vector size 3 is %d\n",toBuild.size());

            if(builtL1id!=targetL1id)
            {
                uint8_t headbuf[14];
                for(int i=0;i<4;i++)
                {
                    headbuf[i]=0xdd;
                }
                headbuf[4]=(uint8_t)(builtL1id&0x000000ff);
                headbuf[5]=(uint8_t)((builtL1id&0x0000ff00)>>8);
                headbuf[6]=(uint8_t)((builtL1id&0x00ff0000)>>16);
                headbuf[7]=(uint8_t)((builtL1id&0xff000000)>>24);
                headbuf[8]=(uint8_t)(builtLength);
                headbuf[9]=(uint8_t)(builtLength>>8);
                headbuf[10]=(uint8_t)(builtLength>>16);
                headbuf[11]=(uint8_t)(builtLength>>24);
                headbuf[12]=0x00;
                headbuf[13]=0x00;
                bool isAllLack = false;
                bool isLackOne = false;
                bool notContinue = false;
                bool notSame = false;
                //judge if frame for different IP not the same//FIXME: if 3 in one pack, not considered
                //uint32_t frame;// = lad_info[0].frameCount[0];
                std::vector <uint32_t > frame;
                for(int i=0;i<FEE_SUM;i++)
                {
                    if(lad_info[i].frameCount.size()==0)
                    {
                        isAllLack = true;//all lack
                    }
                    else if (lad_info[i].frameCount.size()==1)
                    {
                        isLackOne = true;
                    }
                    else if(lad_info[i].frameCount.size()!=2)
                    {
                        std::cout<<"strange pack num for one lad";
                        std::cout<<"lad builtL1id is ";
                        printf("%x\n",builtL1id);
                        std::cout<<"seq is";
                        printf("%d\n",i);
                        printf("%d\n",lad_info[i].frameCount.size());
                    }
                    else
                    {
                        if (lad_info[i].frameCount[1]!=(lad_info[i].frameCount[0]+1))
                        {
                            notContinue = true;
                        }
                        //if(lad_info[i].frameCount[0]!=frame)
                        else
                        {
                            frame.push_back(lad_info[i].frameCount[0]);
                        }
                    }
                }
                std::vector <uint32_t >::iterator itt;
                if(frame.size()>1)
                {
                    for(itt=frame.begin();itt!=frame.end()-1;itt++)
                    {
                        if((*itt)!=(*(itt+1)))
                        {
                            notSame = true;
                        }
                    }
                }
                //set status
                if(isAllLack)
                {
                    headbuf[13]=headbuf[13]|0x01;
                }
                if(isLackOne)
                {
                    headbuf[12]=headbuf[12]|0x08;
                }
                if(notContinue)
                {
                    headbuf[12]=headbuf[12]|0x02;
                }
                if(notSame)
                {
                    headbuf[12]=headbuf[12]|0x01;
                }
                for(int i=0;i<FEE_SUM;i++)
                {
                    lad_info[i].frameCount.clear();
                }
                if(!isFirst)
                {
                    int retV=fwrite(headbuf,1,14,fpr);
                    if(retV!=14)
                    {
                        std::cout<<"fwrite return wront val"<<retV<<std::endl;
                        std::cout<<"fpr is "<<fpr<<std::endl;
                    }
                    fwrite(builtBuf, builtLength,1,fpr);
                    uint8_t tailbuf[4];
                    for(int i=0;i<4;i++)
                    {
                        tailbuf[i]=0x66;
                    }
                    fwrite(tailbuf,4,1,fpr);
                    builtLength=0;
                }
                else
                {
                    isFirst = false;
                    builtLength = 0;
                }
            }

            //for toBuild
            //TODO:write header

            for(ior=toBuild.begin();ior!=toBuild.end();ior++)
            {
                uint8_t i = *ior;
                if(toEnd[i])
                {
                    break;
                }

                else
                {
                    builtL1id = info[i].l1id;
                    uint32_t tarLen=16+info[i].length;
                    memcpy(builtBuf+builtLength,(uint8_t*)(buf[i]),tarLen);
                    builtLength = builtLength+tarLen;
                    lad_info[i].frameCount.push_back(builtL1id);
                }
            }
            //TODO:write tail

            //std::cout<<"one done"<<std::endl;

            endCount = 0;;
            for(int i=0;i<FEE_SUM;i++)
            {
                if(toEnd[i])
                {
                    endCount = endCount+1;
                }
            }
            if(endCount==FEE_SUM)
            {
                std::cout<<"all done"<<std::endl;
                break;
            }
            // printf("vector size 4 is %d\n",toBuild.size());

        }
        //write the last pack
        uint8_t headbuf[14];
        uint8_t tailbuf[4];
        for(int i=0;i<4;i++)
        {
            headbuf[i]=0xdd;
            tailbuf[i]=0x66;
        }
        headbuf[4]=(uint8_t)(builtL1id&0x000000ff);
        headbuf[5]=(uint8_t)((builtL1id&0x0000ff00)>>8);
        headbuf[6]=(uint8_t)((builtL1id&0x00ff0000)>>16);
        headbuf[7]=(uint8_t)((builtL1id&0xff000000)>>24);
        headbuf[8]=(uint8_t)(builtLength);
        headbuf[9]=(uint8_t)(builtLength>>8);
        headbuf[10]=(uint8_t)(builtLength>>16);
        headbuf[11]=(uint8_t)(builtLength>>24);
        headbuf[12]=0x00;
        headbuf[13]=0x00;
        fwrite(headbuf,14,1,fpr);
        fwrite(builtBuf, builtLength,1,fpr);
        fwrite(tailbuf,4,1,fpr);
        builtLength=0;
        fclose(fpr);

        for(int i=0;i<FEE_SUM;i++)
        {
            if(fileExist[i])
            {
                fclose(fp[i]);
             }
        }
        std::cout<<"one done"<<std::endl;
    }//while(1)for file looping
    //fclose(fpr);
    std::cout<<"built succeed"<<std::endl;

    string runsum=str+"/"+"RunSummary.txt";
    const char* runsumy = runsum.c_str();


    time_t now;
    struct tm *tm_now;
    time(&now);
    tm_now = localtime(&now);
    char stoptime[200];
    strftime(stoptime, 50, "%Y-%m-%d-%H-%M-%S", tm_now);

    FILE *fps = fopen( runsumy ,"wt");//FIXME
     QString threshold1=ui->threshold1->text();
     string   threshold1_=threshold1.toStdString();
      const char* t1=threshold1_.c_str();


     QString threshold2=ui->threshold2->text();
     string   threshold2_=threshold2.toStdString();
           const char* t2=threshold2_.c_str();

     QString threshold3=ui->threshold3->text();
     string   threshold3_=threshold3.toStdString();
             const char* t3=threshold3_.c_str();

     QString threshold4=ui->threshold4->text();
     string   threshold4_=threshold4.toStdString();
           const char* t4=threshold4_.c_str();

     QString threshold5=ui->threshold5->text();
     string   threshold5_=threshold5.toStdString();
             const char* t5=threshold5_.c_str();

     QString senID1=ui->sensor1->text();
      string  senID1_=senID1.toStdString();
            const char* s1=senID1_.c_str();

      QString senID2=ui->sensor2->text();
       string  senID2_=senID2.toStdString();
          const char* s2=senID2_.c_str();

       QString senID3=ui->sensor3->text();
        string  senID3_=senID3.toStdString();
           const char* s3=senID3_.c_str();

        QString senID4=ui->sensor4->text();
         string  senID4_=senID4.toStdString();
            const char* s4=senID4_.c_str();

         QString senID5=ui->sensor5->text();
          string  senID5_=senID5.toStdString();
             const char* s5=senID5_.c_str();
                string  WQD="null";
            if( ladder_1==0)
            {
                      t1=WQD.c_str();
                       s1=WQD.c_str();
            }
            if( ladder_2==0)
            {
                      t2=WQD.c_str();
                       s2=WQD.c_str();
            }
            if( ladder_3==0)
            {
                      t3=WQD.c_str();
                       s3=WQD.c_str();
            }
            if( ladder_4==0)
            {
                      t4=WQD.c_str();
                       s4=WQD.c_str();
            }
            if( ladder_5==0)
            {
                      t5=WQD.c_str();
                       s5=WQD.c_str();
            }

            QString eventnom;
            ui->currenteventnumber->setText(eventnom.setNum(  (getdtNum/totaltime)/2));
            qApp->processEvents();

    fprintf(fps, "RunNum: %d\n", runnum);
    fprintf(fps, "Start_time: %s\n", datetime);
    fprintf(fps, "Stop_time: %s\n", stoptime);
    fprintf(fps, "Trigger_rate: %d\n", (getdtNum/totaltime)/2);
    fprintf(fps, "Event: %d\n", getdtNum/2);
    fprintf(fps, "Threshold: %s,%s,%s,%s,%s\n", t1, t2, t3, t4, t5);
    fprintf(fps, "Sensorid: %s,%s,%s,%s,%s\n", s1, s2, s3, s4, s5);


    fclose(fps);
}




void MainWindow::on_reset_clicked()
{
    ladder_1=0,ladder_2=0,ladder_3=0,ladder_4=0,ladder_5=0;
    emit state("Reset!Read!");
}
