//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET

#ifndef __BP_ASYNCTRANSCEIVER_H_20110707153439__
#define __BP_ASYNCTRANSCEIVER_H_20110707153439__

#include "../transceiver.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	IOCP 异步收发器
    //----------------------------------------------------------------------------------------------------
    class HYC_API AsyncTransceiver : public Transceiver
    {
    public:
        typedef enum {
            Result_Error,
            // 缓冲区已经被读写完,等待下次消息.
            Result_WouldBlock,
            // 存在更多可读写的缓冲区.
            Result_moreData,
        } Result;

    public:
        virtual ~AsyncTransceiver()  {}

        /**
         * start: 在调用之后, 可能只会完成部分流的发送(效率原因)
         * finish: 在调用之后, 将会设置buf->_curPos或者pData->pPosWriteAll为当前流的位置,
         *         如若buf->_curPos大于流则表示发送完成, 否则需继续发送
         */
        virtual bool            startWrite( AsyncInfo* pInfo, AsyncData* pData ) = 0;
        virtual Result          finishWrite( AsyncInfo* pInfo, AsyncData* pData ) = 0;

        virtual bool            startRead( AsyncInfo* pInfo, AsyncData* pData ) = 0;
        virtual Result          finishRead( AsyncInfo* pInfo, AsyncData* pData ) = 0;

        // 非连接状态的socket读写.
        // \param pPeerAddr 数据通信的对方地址将存储在 pPeerAddr上.
        virtual bool            startWriteTo( AsyncInfo* pInfo, AsyncData* pData, const struct sockaddr_storage* peerAddr )  { return startWrite(pInfo, pData); }
        virtual bool            startReadFrom( AsyncInfo* pInfo, AsyncData* pData, INOUT struct sockaddr_storage* peerAddr )  { return startRead(pInfo, pData); }

    private:
        virtual bool            write( ByteStream* buf )    { return false; }
        virtual bool            writeAll( ByteStream* buf ) { return false; }
        virtual bool            writeTo( ByteStream* buf, const struct sockaddr_storage* peerAddr ) { return false; }
        virtual bool            read( ByteStream* buf )     { return false; }
        virtual integer         readPart( OUT char* pBuf, uinteger size )    { return -1; }
        virtual bool            readFrom( ByteStream* buf, INOUT struct sockaddr_storage* peerAddr ){ return false; }
        virtual integer         readPartFrom( OUT char* pBuf, uinteger size, INOUT struct sockaddr_storage* peerAddr ) { return -1; }
    };

    typedef ObjectPtr<AsyncTransceiver>  AsyncTransceiverPtr;

} // end namespace hyc

#endif // end of define __BP_IOCPTRANSCEIVER_H_20110707153439__
#endif // HYC_COMPILE_MODULE_NET

