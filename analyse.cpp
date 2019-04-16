#include"analyse.h"
#include<qringbuffer.h>
#include <iostream>
#include<time.h>
#include <sys/stat.h>//mkdir
#include <unistd.h>
#include <res.h>

#define BUFFER_SIZE 20000
//#define ladheader 0x01cccccc
#define ladtrailer 0x99999999
#define eventnumberperfile 10000000
using namespace std;
Analyse::~Analyse()
{
}
Analyse::Analyse()
{
}

int Analyse::analysedt(QRingBuffer*rbuffer,int getdtNum,std::string File_name,string ip,int*ctrflag,int*starflag,int*ladheader,int*pack_num,QRingBuffer*rbufferdata)
{
    unsigned char buffer[BUFFER_SIZE];
    //unsigned char buffer[BUFFER_SIZE];
    //char FileName[200];
    bzero(buffer, BUFFER_SIZE);
    int packnum=0;
    int length =0,offset=0, canwrite=0;
    unsigned int datalength=0;
    unsigned int datalength0=0;
    unsigned int datalength1=0;
    unsigned long eventcount=0;
    int filenum=0;

    //   bzero(FileName,200 );
    //   strcpy(FileName,File_name);
    string ipfile=File_name+"/"+ip;
    const char* ipfileName=ipfile.c_str();
    mkdir( ipfileName,460);

//    string ipfiledat=ipfile+"/1.dat";
//    const char*ipfiledatName=ipfiledat.c_str();
//    FILE *fp= fopen(ipfiledatName, "ab+");

    sleep(1);
    while( (packnum<getdtNum)&&(*starflag==0))
    {

        //        pckFile++;
                eventcount=0;
                filenum++;
                char Filenum[100];
                 sprintf(Filenum,"/%d.dat",filenum);
                 string  FilenumName=Filenum;
                string ipfiledat=ipfile+FilenumName;
                const char*ipfiledatName=ipfiledat.c_str();
                FILE *fp= fopen(ipfiledatName, "ab+");



        while( (eventcount<eventnumberperfile)&&(packnum<getdtNum) )
         {
        length=rbuffer->read(buffer,4);

        offset=0;
        while ( ((buffer[0+offset]<<24)|(buffer[1+offset]<<16)|(buffer[2+offset]<<8)|(buffer[3+offset])) !=*ladheader)  //　校验头部，注意括号　移位并

        {
            length= rbuffer->read(&buffer[4+offset],1);
            if(offset==0)
                printf("****%02x %02x %02x ",(buffer[0+offset]),(buffer[1+offset]),(buffer[2+offset]));
            printf("%02x ",(buffer[3+offset]));
            fflush(stdout);
            offset++;
        }

        buffer[0]=(*ladheader&(0xff000000) )>>24;
        buffer[1]= (*ladheader&(0x00ff0000) )>>16 ;
        buffer[2]= (*ladheader&(0x0000ff00) )>>8 ;
        buffer[3]= *ladheader&(0x000000ff)  ;
        //校验８字节
        length= rbuffer->read(&buffer[4],4);


        length= rbuffer->read(&buffer[8],8);

        offset=8;


        if(length==8)
        {
            datalength0=((buffer[13]<<8)|(buffer[12]));
            datalength1=((buffer[15]<<8)|(buffer[14]));
            datalength=((datalength1<<16)|(datalength0));
            datalength=datalength*2;
            //printf("datalength is: %d bytes\n",datalength);
        }
        int writenum;

        length= rbuffer->read(&buffer[16], datalength+4);


        if(length==datalength+4)
        {
            if( ((buffer[16+datalength]<<24)|(buffer[17+datalength]<<16)|(buffer[18+datalength]<<8)|(buffer[19+datalength])) ==ladtrailer)
            {
                packnum++;
                eventcount++;
                if(packnum==1)
                {
                    cout<<endl;
                    cout<<ip<<" : ";
                    //   printf(" ***********stopsgn :%d ", stopsgn);
                    printf("*************************trigger :%d,framecount;%x \n", ((buffer[7]<<24)|(buffer[6]<<16)|(buffer[5]<<8)|(buffer[4])),((buffer[11]<<24)|(buffer[10]<<16)|(buffer[9]<<8)|(buffer[8])) );
                    fflush(stdout);
                }
                if(packnum%(getdtNum/getdtNum)==0)

                {
                    //  printf(" ***********stopsgn :%d ", stopsgn);
                    cout<<ip<<" : ";
                    printf("the %d package   is correct;  ",packnum);
                    canwrite=rbuffer->canWrite();
                    printf("read %dbytes<<<<    --canwrite--: %d B\n ", datalength+20,canwrite);
                    printf("****trigger :%d \n", ( (buffer[7]<<24)|(buffer[6]<<16)|(buffer[5]<<8)|(buffer[4])  ) );
                    fflush(stdout);
                }
                *pack_num=packnum;
                writenum=fwrite(buffer, sizeof(char),16+length , fp);

                //  printf("writenum: %d\n ",writenum);
                if(writenum !=datalength+20)
                {
                    printf("File Write Failed\n");
                    fflush(stdout);
                }
                // bzero(buffer, BUFFER_SIZE);

            }
            else
            {
                cout<<endl;
                cout<<ip<<" : ";
                printf("trailer of the %d package is error:  \n",packnum);

                for(int i=0;i<=16+length;i++)
                    printf(" %02x",buffer[i]);
                fflush(stdout);

            }
        }
         }
        fclose(fp);
    }

    //printf("**************Analyse stop*************");
    fflush(stdout);

    *ctrflag=1;
    return 1;
}
