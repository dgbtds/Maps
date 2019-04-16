#include "loadconfig.h"
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
#include "MainWindow.h"
#include "ui_MainWindow.h"

#define PORT 4660
#define Rudpdatalength 193
int i=0;
using namespace std;

LoadConfig::LoadConfig()
{
}
LoadConfig::~LoadConfig()
{
}
void LoadConfig::loadfile( unsigned char*udpconfig,int i,int volum)
{

            emit ("CONFIG");
            char file_name[100];
            bzero(file_name, 100);
            if(volum==12)
                sprintf(file_name,"threshold12/%d.dat",i);
            if(volum==115)
                sprintf(file_name,"threshold115/%d.dat",i);
            if(volum==11)
                sprintf(file_name,"threshold11/%d.dat",i);
            if(volum==105)
                sprintf(file_name,"threshold105/%d.dat",i);
            if(volum==10)
                sprintf(file_name,"threshold10/%d.dat",i);
            if(volum==95)
                sprintf(file_name,"threshold95/%d.dat",i);
            if(volum==9)
                sprintf(file_name,"threshold9/%d.dat",i);
            if(volum==85)
                sprintf(file_name,"threshold85/%d.dat",i);
            if(volum==8)
                sprintf(file_name,"threshold8/%d.dat",i);
            if(volum==75)
                sprintf(file_name,"threshold75/%d.dat",i);
            if(volum==7)
                sprintf(file_name,"threshold7/%d.dat",i);
            if(volum==65)
                sprintf(file_name,"threshold65/%d.dat",i);
            if(volum==6)
                sprintf(file_name,"threshold6/%d.dat",i);
            if(volum==55)
                sprintf(file_name,"threshold55/%d.dat",i);
           if(volum==5)
               sprintf(file_name,"threshold5/%d.dat",i);
           if(volum==45)
               sprintf(file_name,"threshold45/%d.dat",i);
           if(volum==4)
               sprintf(file_name,"threshold4/%d.dat",i);
           if(volum==35)
               sprintf(file_name,"threshold35/%d.dat",i);
           if(volum==3)
               sprintf(file_name,"threshold3/%d.dat",i);
           if(volum==25)
               sprintf(file_name,"threshold25/%d.dat",i);
           if(volum==2)
               sprintf(file_name,"threshold2/%d.dat",i);

            FILE * fp=fopen(file_name,"rb+");
         // printf("%s",file_name);
           unsigned int count=0;
           //  unsigned char count=0;

            if(fp!=NULL)
            {
                while(fscanf(fp,"%02x",&udpconfig[count])!=EOF)
                {
                    count=count+1;
                  //  cout<<count<<udpconfig[count]<<" ";

                }
                if (count!=Rudpdatalength)
                {
        //               cout<<"wrong file size!"<<endl;
                    emit prompt("FILE ERROR:wrong file size");
                    emit state("LOAD CONFIG ERR");
                     qApp->processEvents();
                }
                    emit state("LOAD CONFIG SUCCESSED");
            }
            else
            {
        //           cout<<count;
        //           cout<<"open file error!"<<endl;
                emit prompt("FILE ERROR:open file error");
                emit state("LOAD CONFIG ERR");
                 qApp->processEvents();
            }
            fclose(fp);


}
void LoadConfig::checkdata(QString s,std::string SERVER_IP)
{
    int sockfd, recvlength,send;socklen_t  addrlen;
    unsigned char command[9];
    bzero(command,9);
    unsigned char ack[10];
    bzero(ack,10);
    struct sockaddr_in addr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("0xf2 create socket failed!\n");
         fflush(stdout);
        emit prompt("CONFIG ERROR: create socket failed");
        qApp->processEvents();
        exit(1);
    }
   // printf("0xf2 create socket successeed!\n");
     //fflush(stdout);
    addrlen = sizeof(struct sockaddr_in);
    bzero(&addr, addrlen);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP.c_str());

    command[0]=0xff;
    command[1]=0xc0;
    command[2]=0x00;
    command[3]=0x01;
    command[4]=0x00;
    command[5]=0x00;
    command[6]=0x00;
    command[7]=0xf2;
    command[8]=0x00;
    send=sendto(sockfd, command, 9, 0, (struct sockaddr *)&addr, addrlen);
    if(  send <=0)
    {
       // printf("send cmd failed!\n");
         // fflush(stdout);
        emit prompt("CONFIG ERROR: send cmd failed");
        emit state("CONFIG ERR");
        qApp->processEvents();
        close(sockfd);
    }
//    else
//    {
//        printf("send command finish!\n");
//         fflush(stdout);
//    }

  if ( ( recvlength = recvfrom(sockfd, ack, 9, 0, NULL, NULL) )<=0 )
  {
      printf("recv data from 0xf2 failed!\n");
      fflush(stdout);
      emit prompt("CONFIG ERROR: recv data from 0xf2 failed");
      qApp->processEvents();
      ::close(sockfd);
      exit(1);
  }
  i++;
  printf("recv data from 0xf2 succeed! %d sensors have configed \n",i);
  fflush(stdout);

  if(ack[8]!=1)
  {
      printf("0xf2 return data  ERROR!:%02x\n",ack[8]);
      fflush(stdout);
      emit prompt("0xf2 return data  ERROR");
      qApp->processEvents();
      ::close(sockfd);
      exit(1);
  }
//  printf("0xf2 return data right\n");
//  fflush(stdout);
  emit prompt(s);
   qApp->processEvents();
   ::close(sockfd);
}

