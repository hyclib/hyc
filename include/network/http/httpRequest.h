//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  http请求
//

#ifndef __BP_HTTPREQUEST_H_20101222112214__
#define __BP_HTTPREQUEST_H_20101222112214__
#ifdef HYC_COMPILE_MODULE_NET

#include "httpType.h"
#include "../socket/tcpTransceiver.h"
#include "../../os/thread.h"
#include "../../common/cnObjectPtrSafe.h"

namespace hyc
{
    class HttpResponse;

    //----------------------------------------------------------------------------------------------------
    //*	HttpRequest
    //      一个HttpRequest消息包括: 请求行/消息报头(可选)/请求正文(可选)
    //----------------------------------------------------------------------------------------------------

    class HYC_API HttpRequest : public INoncopyable, public MemoryAllocObject, public RefCountSafe
    {
        PRIVATE_CLASS_DECLARE;
    public:
        HttpRequest();
        virtual ~HttpRequest();

    public:

        /**
         * 超时.
         */
        void        setTimeout( u32 timeout );
        u32         getTimeout() const;

        /*---------------------------
          发送请求前的准备
        ---------------------------*/
        void        setHost( const String& host, PORT port = 80 );   // 设置服务器主机和端口 (必须第一个被调用)
        bool        setRequestLine( HttpMethod method, const char* URI, HttpVersion httpVersion = HttpVersion_1_1 );  // 设置请求行; 首页URI使用"/"

        /*---------------------------
          消息报头;
        ---------------------------*/
        void        setDefaultHeader();    // 使用默认的消息报头
        void        setHeader( const char* pName, const char* pValue ); // pValue = NULL 则清除指定的 Header
        const char* getHeader( const char* pName ) const;
        const char* getHeader( u32 index ) const;
        u32         getHeaderNum() const;

        /*---------------------------
          发送请求; 阻塞函数, 
          如果存在发送内容, 将自动加上Content-length头, 但Content-type头不会自动添加.
          return >= HttpResult_error 表示错误(包括超时).
                 >  HttpResult_timeout 表示超时.
        ---------------------------*/
        HttpResult  request( const char* pSendContent, u32 len, OUT HttpResponse* pResponse );
        HttpResult  requestContent( Byte* pBuf, u32 len, OUT u32& readLen );

    public:
        /**
         * @brief 连接成功后的回调.
         */
        virtual void    onConnected( TcpTransceiver& trans )    {}
        /**
         * @brief 关闭前的回调(只有连接成功后才会回调).
         */
        virtual void    onShutdown( TcpTransceiver& trans )     {}
        /**
         * @brief 发送的回调.
         */
        virtual bool    onWrite( TcpTransceiver& trans, ByteStream* buf )   { return trans.write(buf); }
        /**
         * @brief 读取的回调.
         */
        virtual s32     onReadPart( TcpTransceiver& trans, OUT char* pBuf, uinteger bufSize )   { return (s32)trans.readPart(pBuf, bufSize); }

    };
    typedef ObjectPtr<HttpRequest>  HttpRequestPtr;

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
#endif // end of define __BP_HTTPREQUEST_H_20101222112214__
