#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <loadconfig.h>
#include <sendcmd.h>
#include <recvdata.h>
#include <analyse.h>
#include<thread>
#include <functional>//bind

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void onPrompt(QString title);
    void onstate(QString status);
private slots:

    void datassmble( std::string str , int totaltime);
    void on_start_clicked();

    int on_stop_clicked();

    void on_ROOT_clicked();

    void on_ROOT_3_clicked();
    bool isIpFormatWrong(const char* ipAddress);
    int on_config1_clicked();

    int on_config2_clicked();

    int on_config3_clicked();

    int on_config4_clicked();

    int on_config5_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


    void on_ROOT_7_clicked();
    void event_rate();
    int stoptime();

    int assemble();

   int on_start_2_clicked();


   void on_reset_clicked();

signals:
    void prompt(QString title);
    void state(QString status);

private:
    Ui::MainWindow *ui;
    LoadConfig*loadconfig;
    Rcfgpck*rcfgpck;
    GETDT*getdt;
    Analyse*analysedt;

};

#endif // MAINWINDOW_H
