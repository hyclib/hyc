//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET

#ifndef __BP_TCPACCEPTOR_H_20110619130441__
#define __BP_TCPACCEPTOR_H_20110619130441__

#include "tcpTransceiver.h"

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	连接接受器
    //----------------------------------------------------------------------------------------------------
    class HYC_API TcpAcceptor : public Transceiver
    {
    public:
        /**
         * @brief 内部创建socket的方式来构建.
         */
        TcpAcceptor();
        /**
         * @brief 使用外部socket来创建 (不由析构自动关闭socket)
         * @param  bAlreadyBinded  指明socket是否已经绑定了地址,已经绑定的地址必须与将来监听的地址相同
         */
        TcpAcceptor( SOCKET socket, bool bAlreadyBinded );
        virtual ~TcpAcceptor();

        virtual SocketOperation initialize()    { return SocketOperation_None; }

        //! 返回false表示端口被占用or ip错误.
        virtual bool            listen( const String& host, int port, ProtocolSupport protocol );

        //! 返回的对象不使用前需close.
        virtual TransceiverPtr  accept();
        int                     effectivePort() const;

        virtual ProtocolType    protocol() const    { return Protocol_Tcp; }

    protected:
        virtual bool            write( ByteStream* buf )    { return false; }
        virtual bool            writeAll( ByteStream* buf ) { return false; }
        virtual bool            read( ByteStream* buf )     { return false; }
        virtual integer         readPart( OUT char* pBuf, uinteger size )   { return -1; }

    protected:
        virtual void            _initalize(SOCKET extS);

        struct sockaddr_storage         _addr;
        s32                             _backlog;
        bool                            _bAlreadyBinded;
        SOCKET                          _extSocket;
    };

} // end namespace hyc

#endif // end of define __BP_TCPACCEPTOR_H_20110619130441__
#endif // HYC_COMPILE_MODULE_NET
