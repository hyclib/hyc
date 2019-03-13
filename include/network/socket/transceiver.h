//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET

#ifndef __BP_TRANSCEIVER_H_20110617162437__
#define __BP_TRANSCEIVER_H_20110617162437__

#include "../../common/cnObjectPtr.h"
#include "../../container/byteStream.h"
#include "socketUtils.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	Transceiver 收发器
    //----------------------------------------------------------------------------------------------------
    class HYC_API Transceiver : public RefCountSafe, public INoncopyable
    {
    public:
        Transceiver() : _socket(INVALID_SOCKET) {}
        virtual ~Transceiver()  {}

        virtual SOCKET          socket()    { return _socket; }

        //! 关闭socket.
        virtual void            close()     { if (_socket != INVALID_SOCKET) { SocketUtils::CloseSocket(_socket); _socket = INVALID_SOCKET; } }
        virtual bool            isClosed() const { return INVALID_SOCKET == _socket; }

        //! 返回建立通信还需要进行的操作.
        virtual SocketOperation initialize() = 0;

        //! 从buf的_curPos位置开始发送完buf.
        virtual bool            write( ByteStream* buf ) = 0;
        virtual bool            writeAll( ByteStream* buf ) = 0;    // 发送整个buf, [buf.begin~buf.end]
        //! 从buf的_curPos位置开始接收满buf.
        virtual bool            read( ByteStream* buf ) = 0;
        //! 接收部分buf, 返回接收到的字节数.
        virtual integer         readPart( OUT char* pBuf, uinteger size ) = 0;
        virtual integer         readPart( ByteStream* buf );

        //! 非连接状态socket, 从指定的地址接收数据. udp接受要求接受buf必须要大于发送的包, 否则报错.
        //! \param peerAddr 如若不筛选地址则将pPeerAddr置空 memset(pPeerAddr, 0, sizeof(struct sockaddr_storage))
        //!                  数据通信的对方地址将存储在 pPeerAddr上.
        virtual bool            writeTo( ByteStream* buf, const struct sockaddr_storage* peerAddr ) { return write(buf); }
        virtual integer         readPartFrom( OUT char* pBuf, uinteger size, INOUT struct sockaddr_storage* peerAddr ) { return readPart(pBuf, size); }

        //! 返回协议.
        virtual ProtocolType    protocol() const = 0;

    protected:
        SOCKET  _socket;
    };
    typedef ObjectPtr<Transceiver>      TransceiverPtr;


    //-----------------------------------------------
    inline hyc::integer Transceiver::readPart( ByteStream* buf )
    {
        char* pBuf = (char*)buf->_curPos; 
        if (!pBuf)  pBuf = (char*)buf->begin();

        uinteger size = (char*)buf->end()-pBuf;
        return readPart(pBuf, size);
    }

} // end namespace hyc

#endif // end of define __BP_TRANSCEIVER_H_20110617162437__
#endif // HYC_COMPILE_MODULE_NET
