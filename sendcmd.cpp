#include "MainWindow.h"
#include "ui_MainWindow.h"
#include<sendcmd.h>
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

//#define SERVER_IP "192.168.10.16"
#define PORT 4660
#define command_size 1024
using namespace std;

Rcfgpck::Rcfgpck()
{

}

Rcfgpck::~Rcfgpck()
{

}


int Rcfgpck::rcfgpck(unsigned char *udp_config,unsigned char* header,int Rdatalength,QString s,string SERVER_IP)
{

    int sockfd, recvlength,send;
    unsigned char command[command_size];
    unsigned char ack[command_size];
    bzero(command,command_size);
    bzero(ack,command_size);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
         printf("create socket failed!\n");
          fflush(stdout);
        emit prompt("CONFIG ERROR: create socket failed");
        emit state("CONFIG ERR");
        qApp->processEvents();
        return 0;
    }
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    addr.sin_addr.s_addr = inet_addr(SERVER_IP.c_str());
    socklen_t addrlen= sizeof(addr);
    if(::connect( sockfd , (struct sockaddr*)&addr, addrlen) < 0)
    {
          printf("connect failed!\n");
          fflush(stdout);
          emit prompt("CANNOT CONNECT TO SERVER");
          qApp->processEvents();
          return 0;
    }
//    printf("socket success");
//    fflush(stdout);
//   return 0;

     //printf("socket success");
      // fflush(stdout);

        command[0]=header[0];
        command[1]=header[1];
        command[2]=header[2];
        command[3]=header[3];//0x01
        command[4]=header[4];
        command[5]=header[5];
        command[6]=header[6];
        command[7]=header[7];
        //printf("\n command[7]:%x,header[4]:%x,header[7]:%x\n",command[7],header[4],header[7]);
       // fflush(stdout);
        for(int i=8;i<8+Rdatalength;i++)
        {
            command[i]=udp_config[i-8];
           if ( (i-8<20)&&(i-8>14) )
           {
            printf(" i=%d; command[]:%d  %02x\n",i-8,command[i],command[i]);
              fflush(stdout);
           }

        }

        //fgets(command, command_size, stdin);
        if(  ( send=sendto(sockfd, command, 8+Rdatalength, 0, (struct sockaddr *)&addr, addrlen) ) <=0)
        {
            printf("send cmd failed!\n");
              fflush(stdout);
            emit prompt("CONFIG ERROR: send cmd failed");
            emit state("CONFIG ERR");
            qApp->processEvents();
            close(sockfd);
            return 0;
        }
//        else
//        {
//            printf("send cmd finish!\n");
//             fflush(stdout);
//        }
      if ( ( recvlength = recvfrom(sockfd, ack, 8+Rdatalength, 0, NULL, NULL) )<=0 )
      {
       //printf("recv ack failed!\n");
        //  fflush(stdout);
          emit prompt("CONFIG ERROR: recv ack failed");
          emit state("CONFIG ERR");
          qApp->processEvents();
          close(sockfd);
          return 0;
      }
    //    printf("recv ack succeed!\n");
      //  fflush(stdout);

        //check ack
        if(((ack[0]>>4)&0x0f)!=0x0f)
            {
            printf("ver error!\n");
             fflush(stdout);
            emit prompt("SCURVE ERROR: ver error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        if((ack[0]&0x0f)!=0x0f)
            {
            printf("type error!\n"); fflush(stdout);
            emit prompt("SCURVE ERROR: type error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        if(((ack[1]>>4)&0x0f)!=0x08)
            {
            printf("cmd error!\n"); fflush(stdout);
            emit prompt("SCURVE ERROR: cmd error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        if((ack[1]&0x0f)!=0x08)
            {
            printf("flag error!\n"); fflush(stdout);
            emit prompt("SCURVE ERROR: flag error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        if(ack[2]!=command[2])
            {
            printf("id not same!\n"); fflush(stdout);
            emit prompt("SCURVE ERROR: id error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }

        if(ack[4]!=command[4])
            {
            printf("ack 4 not same!\n"); fflush(stdout);
            emit prompt("SCURVE ERROR: addr error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        if(ack[5]!=command[5])
            {
            printf("ack 5 not same!\n"); fflush(stdout);
            emit prompt("SCURVE ERROR: addr error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        if(ack[6]!=command[6])
            {
            printf("ack 6 not same!\n"); fflush(stdout);
            emit prompt("SCURVE ERROR: addr error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        if(ack[7]!=command[7])
            {
            printf("ack 7  not same!\n");
            fflush(stdout);

            emit prompt("SCURVE ERROR: addr error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        if(ack[3]!=command[3])
            {
            printf("data length not same!\n"); fflush(stdout);
            emit prompt("SCURVE ERROR: data length error");
            emit state("SCURVE ERR");
            qApp->processEvents();
             return 0;
            }
        else
            {
            for(unsigned char temp=0;temp<ack[3];temp++)
                {
                    if(ack[temp+8]!=command[temp+8])
                    {
                        printf("cmd data error!\n"); fflush(stdout);
                        emit prompt("command data error");
                        emit state("SCURVE ERR");
                        qApp->processEvents();
                        return 0;
                     }
                 }
              }
        emit prompt(s);
        qApp->processEvents();
        return 1;
    close(sockfd);
}
