#include "recvdata.h"
#include<qringbuffer.h>
#include <res.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>
#include <QString>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"

#define ladheader 0xcc01cccc
#define ladtrailer 0x99999999
#define DAQPORT 24// not used
#define BUFFER_SIZE 8000
#define pckmax 8000 //used for prebuf length in getdt-getheader
#define filenamelength 200

using namespace std;

GETDT::GETDT()
{

}

GETDT::~GETDT()
{

}
int GETDT::getdt(  QRingBuffer*rbuffer,std::string SERVER_IP,int*ctrflag,int*starflag )//check header,length,trailer
{
    //connect with ladder

    emit prompt("START DAQ");
    qApp->processEvents();

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        emit prompt("Create Socket failed");
        exit(1);
    }
    cout<<SERVER_IP;
    printf("   Create tcp Socket successeed!\n");
    fflush(stdout);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(SERVER_IP.c_str());
    server_addr.sin_port = htons(DAQPORT);
    socklen_t server_addr_length = sizeof(server_addr);

    if(::connect(socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        emit prompt("Can Not Connect To Server ");
        exit(1);
    }
    cout<<SERVER_IP;
    printf("    tcp Socket connect successeed!\n\n");
    fflush(stdout);

    unsigned char gbuffer[BUFFER_SIZE];
    bzero(gbuffer, BUFFER_SIZE);
    int canwrite=0;int count=0;

    // FILE *fp = fopen("/home/wy/Desktop/Mapsdata/DataTest/1.dat", "ab+");

    while(*ctrflag!=1)
    {

        int length =0,recvlength=0;
        while(length<BUFFER_SIZE)
        {

            recvlength=recv(socket_fd, gbuffer+length, BUFFER_SIZE-length,0);

            if(recvlength==-1)
            {
               // printf("recv data over\n");
                break;
            }

            length+=recvlength;
            struct timeval timeout={30,0};//30s
            setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
//            printf("length %d, recvlength :%d \n",length,recvlength);
//            fflush(stdout);

        }


        //       int writenum=fwrite(gbuffer, sizeof(char),BUFFER_SIZE , fp);
        //        count++;
        //        if(writenum ==BUFFER_SIZE)
        //        {
        //            printf(" %d File Write \n",count);
        //            fflush(stdout);

        //        }
        int count= rbuffer->write(gbuffer,sizeof(gbuffer));
        canwrite=rbuffer->canWrite();
        if(count==-1)
        {
            printf("****write File Failed !>>>>  ");
            emit prompt("write rbuffer Failed !");
            qApp->processEvents();
        }
        else
        {
            //  cout<<SERVER_IP;
          //  printf("****write %dBytes succeed !>>>>    ",count);
            //             printf("write--canwrite-->>>>: %d B\n ",canwrite);
            //            qApp->processEvents();
            fflush(stdout);
        }
    }
    // fclose(fp);
    ::close(socket_fd);
    //printf("TCP DISCONNECTED!\n");
    stopsgn++;
   // *starflag==1;
    cout<<SERVER_IP<<" :DAQ Finish! "<<endl;
    fflush(stdout);
    return 1;
}
