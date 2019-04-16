#ifndef QRINGBUFFER_H
#define QRINGBUFFER_H

#include <cstring>

#ifndef RB_MAX_LEN
#define RB_MAX_LEN 800000
#endif

class QRingBuffer
{
public:
    QRingBuffer(int size = RB_MAX_LEN);
    ~QRingBuffer();

    int canRead();    //how much can read
    int canWrite();   //how much can write
    int read(void *data, int count);  //read data frome ringbuffer
    int write(const void *data, int count);
    int size();
    void clean();
    unsigned char rbBuf  [RB_MAX_LEN];
private:
    int bufferSize;       //buffer size

    /*环形缓冲区变量*/
    int rbCapacity; //容量
    unsigned char  *wTail;
    unsigned char  *rHead;
    unsigned char  *rbBuff;

};

#endif // QRINGBUFFER_H
