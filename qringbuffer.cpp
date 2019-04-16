#include "qringbuffer.h"
#include <stdio.h>
#include <unistd.h>

/**
 * @brief QRingBuffer::QRingBuffer
 * @param buffersize Byte
 */

QRingBuffer::QRingBuffer(int size)
{
    bufferSize = size;
    rbCapacity = size;
    rbBuff = rbBuf;
    rHead = rbBuff;
    wTail = rbBuff+1;
}

QRingBuffer::~QRingBuffer()
{
    rbBuff = nullptr;
    rHead = nullptr;
    wTail = nullptr;
    rbCapacity = 0;
   // delete []rbBuf; //释放缓冲区
}
/**
 * @brief QRingBuffer::rbCanRead
 * @return 缓冲区可读字节数
 */
void QRingBuffer::clean()
{
    rHead = rbBuff;
    wTail = rbBuff+1;
}
int QRingBuffer::canRead()
{
    //ring buufer is null, return -1
    if((nullptr == rbBuff)||(nullptr == rHead)||(nullptr == wTail))
    {
        return -1;
    }


    if (1== rHead-wTail )
    {
        return rbCapacity-1;
    }

    if (rHead < wTail)
    {
        return wTail - rHead-1;
    }
    return rbCapacity - (rHead - wTail);
}

/**
 * @brief QRingBuffer::rbCanWrite  缓冲区剩余可写字节数
 * @return  可写字节数
 */
int QRingBuffer::canWrite()
{
    if((nullptr == rbBuff)||(nullptr == rHead)||(nullptr == wTail))
    {
        return -1;
    }

    return rbCapacity - canRead()-1;
}

/**
 * @brief QRingBuffer::read 从缓冲区读数据
 * @param 目标数组地址
 * @param 读的字节数
 * @return
 */
int fla=1;
int QRingBuffer::read(void *data, int count)
{

    int copySz = 0;  int tailAvailSz = 0;

    if((nullptr == rbBuff)||(nullptr == rHead)||(nullptr == wTail))
    {
        return -1;
    }
    if(nullptr == data)
    {
        return -1;
    }

    while (count > canRead() )
    {
       usleep(1);
    }

    if (rHead < wTail)
    {
        copySz = count;
        memcpy(data, rHead, copySz);
        rHead += copySz;
        return copySz;
    }
    else
    {
        tailAvailSz = rbCapacity - (rHead - rbBuff);
        if (count <= tailAvailSz)
        {
            memcpy(data, rHead, count);
            rHead += count;
            if (rHead == rbBuff+rbCapacity)
            {
                rHead= rbBuff;
            }
            return count;
        }
        else
        {
            memcpy(data, rHead, tailAvailSz);
            rHead= rbBuff;

            return tailAvailSz + read((char*)data+tailAvailSz, count-tailAvailSz);
        }
//        if (count <=canRead( ) )
//        {
//            copySz = count;
//            memcpy(data, rHead, copySz);
//            rHead += copySz;
//            return copySz;
//        }
//        else
//        {
//            copySz = canRead( );
//            memcpy(data, rHead, copySz);
//            rHead = rbBuff;
//            copySz += read((unsigned char *)data+copySz, count-copySz);
//            return copySz;
        }

}

/**
 * @brief QRingBuffer::write
 * @param 数据地址
 * @param 要写的字节数
 * @return 写入的字节数
 */
int fla2=1;
int QRingBuffer::write(const void *data, int count)
{
    int tailAvailSz = 0;

    if((nullptr == rbBuff)||(nullptr == rHead)||(nullptr == wTail))
    {
        return -1;
    }

    if(nullptr == data)
    {
        return -1;
    }

    while (count > canWrite() )
    {
        usleep(10);
    }

    if (rHead <wTail)
    {
        tailAvailSz = rbCapacity - (wTail - rbBuff);
        if (count <= tailAvailSz)
        {
            memcpy(wTail, data, count);
            wTail += count;
            if (wTail == rbBuff+rbCapacity)
            {
                wTail = rbBuff;
            }
            return count;
        }
        else
        {
            memcpy(wTail, data, tailAvailSz);
            wTail = rbBuff;

            return tailAvailSz + write((char*)data+tailAvailSz, count-tailAvailSz);
        }
    }
    else
    {
        memcpy(wTail, data, count);
        wTail += count;

        return count;
    }
}

/**
 * @brief QRingBuffer::size
 * @return 缓冲区大小
 */
int QRingBuffer::size()
{
    return bufferSize;
}
