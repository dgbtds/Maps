#include "MainWindow.h"
#include <QApplication>
#include <iostream>
#include<thread>
#include <functional>//bind
#include <unistd.h>

//#include "qringbuffer.h"
//#include<recvdata.h>
//#include<analyse.h>
//#include<recvdata.h>

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    //    QRingBuffer rbuffer;
    //    GETDT getDT;

    //   auto f =  std::bind(&GETDT::getdt, &getDT,&rbuffer);

    //    Analyse anaDT;
    //    auto f2 = std::bind(&Analyse::analysedt,&anaDT,&rbuffer);

    //        printf("startflag:%d\n",MainWindow::startflag);
    //        fflush(stdout);
    //        thread Gid1(f);
    //        thread Aid1(f2);


    return a.exec();
}
