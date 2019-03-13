//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include <map>

#include "../../../include/common/cnObjectRef.h"
#include "../../../include/network/http/httpRequest.h"
#include "../../../include/network/http/httpResponse.h"
#include "httpResponse_private.hpp"
#include "../../../include/network/socket/tcpTransceiver.h"
#include "../../../include/network/socket/socketUtils.h"
#include "../../../include/os/thread.h"
#include "../../../include/utils/stringUtils.h"
#include "../../../include/utils/encodingUtils.h"


namespace hyc
{
#define MAX_RECV_BUF    1024*512
#define CR              0xD
#define LF              0xA
    static const char CRLF[] = { CR, LF, 0 };
    static const char CRLFCRLF[] = { CR, LF, CR, LF, 0 };

    typedef std::map<cString, cString>  HeaderMap;

    //----------------------------------------------------------------------------------------------------
    //*	HttpRequest::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class HttpRequest::PrivateClass : public MemoryAllocObject
    {
    public:
        PrivateClass() 
            : m_contentChunkLen(0)
            , m_contentIsChunk(false)
    		, m_bufContent(false)
    		, m_bufPackage(false)
    		, m_host(0)
            , m_port(0)
    		, m_timeout(-1)
            , m_tcpTransceiver(0)
            , m_requestMethod(HttpMethod_Get)
        { m_bufRequestLine.reserve(512); m_bufPackage.reserve(1024); m_bufRecv.reserve(512); }

        void reset( HttpRequest* pc )
        {
            m_contentChunkLen = 0;
            m_contentIsChunk = false;
            m_bufContent.clear();
            m_bufPackage.clear();
            //m_bufRequestLine.clear();
            m_bufRecv.clear();
            //m_mapHeaders.clear();
            if (m_tcpTransceiver)
            {
                pc->onShutdown(*m_tcpTransceiver);
                m_tcpTransceiver.release();
            }
            
        }

        u32 readChunkSize(HttpRequest* pReq)
        {
            TcpTransceiver* pTcpTransceiver = m_tcpTransceiver.get();

            bool isCR = false;
            cString slenStr;
            char buf; 
            u32 bufOffset = 0;

            while (true)
            {
                s32 slen;

                if (bufOffset < m_bufContent.size())
                {
                    slen = 1;
                    buf = m_bufContent[bufOffset];
                    bufOffset++;
                }
                else
                {
                    slen = pReq->onReadPart(*pTcpTransceiver, &buf, 1);
                }

                if (slen == 1)
                {
                    // 前一次的数据.
                    if (isCR)
                    {
                        if (buf != LF)
                        {
                            slenStr += (char)CR;
                        }
                        // 读取完成.
                        else
                        {
                            u32 sizeSlenStr = slenStr.size();
                            if (sizeSlenStr&0x1)
                            {
                                if (sizeSlenStr == 5)
                                    slenStr = "000" + slenStr;
                                else
                                    slenStr = '0' + slenStr;
                            }

                            if (StringUtils::GetByteSize(slenStr.c_str()) > 8)
                            {
                                return HttpResult_error_contentTolong;
                            }

                            u32 sSize = StringUtils::GetByteSize(slenStr.c_str());
                            u32 len = 0;
                            if (sSize <= 2)
                            {
                                u8 tlen;
                                EncodingUtils::HexToByte(slenStr.c_str(), sSize, (Byte*)&tlen, 1);
                                len = tlen;
                            }
                            else if (sSize <= 4)
                            {
                                u16 tlen;
                                EncodingUtils::HexToByte(slenStr.c_str(), sSize, (Byte*)&tlen, 2);
                                len = ByteUtils::Ntoh(tlen);
                            }
                            else
                            {
                                EncodingUtils::HexToByte(slenStr.c_str(), sSize, (Byte*)&len, 4);
                                len = ByteUtils::Ntoh(len);
                            }

                            if (bufOffset != 0)
                            {
                                if (m_bufContent.size()-bufOffset > 0)
                                {
                                    memmove(m_bufContent.begin(), m_bufContent.begin()+bufOffset, m_bufContent.size()-bufOffset);
                                    m_bufContent.resize(m_bufContent.size()-bufOffset);
                                }
                            }
                            
                            return len;
                        }
                    }

                    // 读取.
                    if (buf == CR)
                    {
                        isCR = true;
                    }
                    else
                    {
                        slenStr += buf;
                    }
                }
                else
                {
                    pReq->onShutdown(*pTcpTransceiver);
                    pTcpTransceiver->close();
                    m_tcpTransceiver.release();
                    if (bufOffset != 0)
                    {
                        if (m_bufContent.size()-bufOffset > 0)
                        {
                            memmove(m_bufContent.begin(), m_bufContent.begin()+bufOffset, m_bufContent.size()-bufOffset);
                            m_bufContent.resize(m_bufContent.size()-bufOffset);
                        }
                    }
                    return -1;
                } // if..else.
            } // while.
        }

        
        HttpMethod  m_requestMethod;
        cString     m_requestURI;
        
        // buf
        u32         m_contentChunkLen;
        bool        m_contentIsChunk;
        ByteStream  m_bufContent;
        ByteStream  m_bufPackage;
        cString     m_bufRequestLine;
        cString     m_bufRecv;
        char        m_bufSzRecv[MAX_RECV_BUF];

        // socket
        String      m_host;
        PORT        m_port;

        u32         m_timeout;

        struct sockaddr_storage    m_connectAddr;
        ObjectRef<TcpTransceiver>  m_tcpTransceiver;

        // header map
        mutable HeaderMap   m_mapHeaders;

        // 同步锁
        mutable SyncObject          m_sync;
//         HttpRequestRecvThread   m_thread;
    };
    
    //----------------------------------------------------------------------------------------------------
    //*	HttpRequest
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    HttpRequest::HttpRequest()
    {
        PRIVATE_CLASS_CREATE;
    }
    
    //-----------------------------------------------
    HttpRequest::~HttpRequest()
    {
        PRIVATE_CLASS_GET(p);
        p->m_sync.lock();

        if (p->m_tcpTransceiver.get())
        {
            this->onShutdown(*(p->m_tcpTransceiver));
            p->m_tcpTransceiver->close();
        }

        p->m_sync.unlock();
        PRIVATE_CLASS_DESTROY;
    }

    //-----------------------------------------------
    void HttpRequest::setHost( const String& host, PORT port /*= 80 */ )
    {
        PRIVATE_CLASS_GET(p);
        SyncTokenPtr token = p->m_sync.getToken();

        p->m_host = host;
        p->m_port = port;

        // create address
        p->m_connectAddr = SocketUtils::AddrCreateAuto(host, port);

        setDefaultHeader();
    }

    //-----------------------------------------------
    void HttpRequest::setDefaultHeader()
    {
        PRIVATE_CLASS_GET(p);
        SyncTokenPtr token = p->m_sync.getToken();

        p->m_mapHeaders.clear();

        // Host
        cString cPort;  StringUtils::S32ToStr( p->m_port, cPort );
        cString cHost;  //SocketUtils::AddrToStr( p->m_host );
        StringUtils::StrToStr(p->m_host, cHost);

        if (p->m_port != 80)
        {
            cHost += ":";
            cHost += cPort;
        }
        setHeader( "Host", cHost.c_str() );

        // Connection
        setHeader( "Connection", "Keep-Alive" );
// 
//         // Cache-Control
//         setHeader( "Cache-Control", "max-age=0" );
// 
//         // Content-Type
//         setHeader( "Content-Type", "text/html; charset=utf-8" );
// 
//         // Accept-Encoding
//         setHeader( "Accept-Encoding", "gzip,deflate,sdch" );
// 
//         // Accept-Charset
//         setHeader( "Accept-Charset", "GBK,utf-8" );
// 
//         // Accept-Language
//         setHeader( "Accept-Language", "zh-CN,zh" );
// 
//         // Accept
//         setHeader( "Accept", "application/json, text/javascript, */*" );
// 
//         // user-agent
//         setHeader( "User-Agent", "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/534.30 (KHTML, like Gecko) Chrome/12.0.742.100 Safari/534.30" );

    }

    //-----------------------------------------------
    void HttpRequest::setHeader( const char* pName, const char* pValue )
    {
        PRIVATE_CLASS_GET(p);
        SyncTokenPtr token = p->m_sync.getToken();

        if ( NULL == pValue )
        {
            HeaderMap::iterator itr = p->m_mapHeaders.find( pName );
            if ( itr != p->m_mapHeaders.end() )
            {
                p->m_mapHeaders.erase( itr );
            }
        }
        else
            p->m_mapHeaders[pName] = pValue;
    }

    //-----------------------------------------------
    const char* HttpRequest::getHeader( const char* pName ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        SyncTokenPtr token = p->m_sync.getToken();

        HeaderMap::iterator itr = p->m_mapHeaders.find( pName );
        if ( itr != p->m_mapHeaders.end() )
            return NULL;
        else
            return (*itr).second.c_str();
    }

    //-----------------------------------------------
    const char* HttpRequest::getHeader( u32 index ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        SyncTokenPtr token = p->m_sync.getToken();

        if ( index > p->m_mapHeaders.size() )
        {
            return NULL;
        }

        u32 i = 0;
        HeaderMap::iterator itr = p->m_mapHeaders.begin();
        while ( itr != p->m_mapHeaders.end() )
        {
            ++itr;
            ++i;
        }

        return (*itr).second.c_str();
    }

    //-----------------------------------------------
    u32 HttpRequest::getHeaderNum() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        SyncTokenPtr token = p->m_sync.getToken();

        return (u32)p->m_mapHeaders.size();
    }

    //-----------------------------------------------
    bool HttpRequest::setRequestLine( HttpMethod method, const char* URI, HttpVersion httpVersion /*= HttpVersion_1_1 */ )
    {
        if ( !URI ) return false;

        PRIVATE_CLASS_GET(p);
        SyncTokenPtr token = p->m_sync.getToken();

        p->m_bufRequestLine.clear();

        /*---------------------------
        http method
        ---------------------------*/
        switch ( method )
        {
        case HttpMethod_Connect:
            p->m_bufRequestLine += "CONNECT ";
            break;
        case HttpMethod_Delete:
            p->m_bufRequestLine += "DELETE ";
            break;
        case HttpMethod_Get:
            p->m_bufRequestLine += "GET ";
            break;
        case HttpMethod_Head:
            p->m_bufRequestLine += "HEAD ";
            break;
        case HttpMethod_Options:
            p->m_bufRequestLine += "OPTIONS ";
            break;
        case HttpMethod_Post:
            p->m_bufRequestLine += "POST ";
            break;
        case HttpMethod_Put:
            p->m_bufRequestLine += "PUT ";
            break;
        case HttpMethod_Trace:
            p->m_bufRequestLine += "TRACE ";
            break;
        default:
            return false;
        }
        
        p->m_requestMethod = method;
        

        /*---------------------------
         URI
        ---------------------------*/
        p->m_requestURI = URI;
        p->m_bufRequestLine += URI;    p->m_bufRequestLine += " ";

        /*---------------------------
         HTTP-version
        ---------------------------*/
        switch ( httpVersion )
        {
        case HttpVersion_1:
            p->m_bufRequestLine += "HTTP/1.0";
            break;
        case HttpVersion_1_1:
            p->m_bufRequestLine += "HTTP/1.1";
            break;
        case HttpVersion_NG:
            p->m_bufRequestLine += "HTTP-NG";
            break;
        default:
            return false;
        }

        p->m_bufRequestLine += CRLF;

        return true;
    }

    //-----------------------------------------------
    HttpResult HttpRequest::request( const char* pContent, u32 len, OUT HttpResponse* pResponse )
    {
        PRIVATE_CLASS_GET(p);
        SyncTokenPtr token = p->m_sync.getToken();


        //-----------------------------------------------
        // 构造报头
        //------------------------------------------------
        p->m_bufContent.clear();
        p->m_contentChunkLen = 0;
        p->m_contentIsChunk = false;

        p->m_bufPackage.clear();
        p->m_bufPackage += p->m_bufRequestLine;  // request line
        
        // 1. host
        HeaderMap::iterator itrHost = p->m_mapHeaders.find("Host");
        if (itrHost == p->m_mapHeaders.end())   return HttpResult_error_header;

        p->m_bufPackage += (*itrHost).first;
        p->m_bufPackage += ": ";
        p->m_bufPackage += (*itrHost).second;
        p->m_bufPackage += CRLF;

        // other
        for ( HeaderMap::iterator itr = p->m_mapHeaders.begin(); itr != p->m_mapHeaders.end(); ++itr )
        {
            if (itrHost != itr)
            {
                p->m_bufPackage += (*itr).first;
                p->m_bufPackage += ": ";
                p->m_bufPackage += (*itr).second;
                p->m_bufPackage += CRLF;
            }
        }

        // 内容长度
        if ( len > 0 )
        {
            p->m_bufPackage += "Content-Length: ";
            cString cstrLen; StringUtils::S32ToStr(len, cstrLen);

            p->m_bufPackage += cstrLen;
            p->m_bufPackage += CRLF;
        }

        p->m_bufPackage += CRLF;

        if ( len > MAX_RECV_BUF-p->m_bufPackage.size() )
        {
            return HttpResult_error_contentTolong;
        }

        if ( len > 0 )
        {
            p->m_bufPackage.add( (Byte*)pContent, len );
        }

        //-----------------------------------------------
        // 连接
        //------------------------------------------------

        // create transceiver
        p->m_tcpTransceiver = ObjectRef<TcpTransceiver>(
            new TcpTransceiver()    // id no use
            );
        assert(p->m_tcpTransceiver.get());

        TcpTransceiver* pTcpTransceiver = p->m_tcpTransceiver.get();
        if (p->m_timeout != -1)
        {
            SocketUtils::SetRevTimeout(pTcpTransceiver->socket(), p->m_timeout);
        }

        if ( ConnectResult_Ok != pTcpTransceiver->connect(p->m_connectAddr) )
        {
            pTcpTransceiver->close();
            p->m_tcpTransceiver.release();
            return HttpResult_timeout_connect;
        }

        SocketUtils::SetKeepAlive(pTcpTransceiver->socket());
        SocketUtils::SetTcpNoDelay(pTcpTransceiver->socket());
        this->onConnected(*pTcpTransceiver);


        //------------------------------------------------
        // 发送
        //------------------------------------------------
        p->m_bufPackage._curPos = p->m_bufPackage.begin();
        bool rSend = this->onWrite(*pTcpTransceiver, &p->m_bufPackage); //pTcpTransceiver->write( &p->m_bufPackage );
        if ( !rSend )
        {
            this->onShutdown(*pTcpTransceiver);
            pTcpTransceiver->close();
            p->m_tcpTransceiver.release();
            return HttpResult_timeout_send;
        }

        // 如果发送不完.
//         if (len > 0)
//         {
// 
//         } // if

        //------------------------------------------------
        //  接收response
        //------------------------------------------------
        if ( NULL == pResponse )
        {
            return HttpResult_ok;
        }

        u32 totalLen = 0;
        cString bufTemp;
        memset( p->m_bufSzRecv, 0, MAX_RECV_BUF );
        HttpResponse::PrivateClass* ppResponse = pResponse->m_pPrivateClass;
        ppResponse->m_statusCode = HttpStatusCode_Error;

        s32 slen = this->onReadPart(*pTcpTransceiver, p->m_bufSzRecv, MAX_RECV_BUF-1); //(s32)pTcpTransceiver->readPart( p->m_bufSzRecv, MAX_RECV_BUF-1 );
        if ( slen <= 0 )
        {
            this->onShutdown(*pTcpTransceiver);
            pTcpTransceiver->close();
            p->m_tcpTransceiver.release();
            return HttpResult_timeout_recv;
        }
        totalLen += slen;

        p->m_bufRecv = p->m_bufSzRecv;

        //-----------------------------------------------
        // HTTP-Version
        u32 pos = (u32)p->m_bufRecv.find( ' ' );
        if ( cString::npos == pos )
        {
            this->onShutdown(*pTcpTransceiver);
            pTcpTransceiver->close();
            p->m_tcpTransceiver.release();
            return HttpResult_error;
        }
        bufTemp = p->m_bufRecv.substr( 0, pos );
        if ( bufTemp == "HTTP/1.0" )
        {
            ppResponse->m_httpVersion = HttpVersion_1;
        }
        else if ( bufTemp == "HTTP/1.1" )
        {
            ppResponse->m_httpVersion = HttpVersion_1_1;
        }
        else if ( bufTemp == "HTTP-NG" )
        {
            ppResponse->m_httpVersion = HttpVersion_NG;
        }
        else
        {
            this->onShutdown(*pTcpTransceiver);
            pTcpTransceiver->close();
            p->m_tcpTransceiver.release();
            return HttpResult_error;
        }

        //-----------------------------------------------
        // status-code
        ++pos;
        u32 pos2 = (u32)p->m_bufRecv.find( ' ', pos );
        if ( cString::npos == pos2 )
        {
            this->onShutdown(*pTcpTransceiver);
            pTcpTransceiver->close();
            p->m_tcpTransceiver.release();
            return HttpResult_error;
        }
        bufTemp = p->m_bufRecv.substr( pos, pos2-pos );
        ppResponse->m_statusCode = (HttpStatusCode)StringUtils::StrToS32( bufTemp.c_str() );

        //-----------------------------------------------
        // status-code description
        ++pos2;
        pos = (u32)p->m_bufRecv.find( CRLF, pos2 );
        if ( cString::npos == pos )
        {
            this->onShutdown(*pTcpTransceiver);
            pTcpTransceiver->close();
            p->m_tcpTransceiver.release();
            return HttpResult_error;
        }
        ppResponse->m_statusCodeDesc = p->m_bufRecv.substr( pos2, pos-pos2 );


        //----------------------------------------------------------------------------------------------------
        //*	接收报头
        //----------------------------------------------------------------------------------------------------

        u32 posEndHeader = 0;

        while (1) { // 确认报头接收完成

            pos2 = (u32)p->m_bufRecv.find( CRLFCRLF );   // 报头结束位置
            if ( cString::npos != pos2 )    // 计算报文的长度
            {
                if (totalLen > pos2) // 拷贝内容
                {
                    assert(p->m_bufContent.empty());
                    p->m_bufContent.add( (Byte*)(p->m_bufSzRecv+slen-(totalLen-pos2-4)), totalLen-pos2-4 );
                }

                posEndHeader = pos2;
                break;
            }

            // 继续接受
            memset( p->m_bufSzRecv, 0, MAX_RECV_BUF );
            slen = this->onReadPart(*pTcpTransceiver, p->m_bufSzRecv, MAX_RECV_BUF-1); //(s32)pTcpTransceiver->readPart( p->m_bufSzRecv, MAX_RECV_BUF-1 );
            if ( slen <= 0 )
            {
                this->onShutdown(*pTcpTransceiver);
                pTcpTransceiver->close();
                p->m_tcpTransceiver.release();
                return HttpResult_error;
            }
            totalLen += slen;
            p->m_bufRecv += p->m_bufSzRecv;

        } // while

        ppResponse->m_mapHeaders.clear();
        while ( cString::npos != pos && pos < posEndHeader )
        {
            ++pos;
            if ( pos >= p->m_bufRecv.size() )   break;

            pos2 = (u32)p->m_bufRecv.find( CRLF, pos );
            if ( cString::npos == pos2 )    break;

            bufTemp = p->m_bufRecv.substr( pos+1, pos2-pos );
            pos = pos2;

            // 取name/value
            pos2 = (u32)bufTemp.find( ": " );
            if ( cString::npos == pos2 )
            {
                this->onShutdown(*pTcpTransceiver);
                pTcpTransceiver->close();
                p->m_tcpTransceiver.release();
                return HttpResult_error;
            }
            cString bufTemp2 = bufTemp.substr( 0, pos2 );
            cString bufTemp3 = bufTemp.substr( pos2+2, bufTemp.size()-pos2-3 );
            ppResponse->m_mapHeaders.insert( std::make_pair( 
                bufTemp2.c_str(),
                bufTemp3.c_str()
                ));
        } // while
        
        //-----------------------------------------------
        // 处理301, 302.
        if (pResponse->getStatusCode() == HttpStatusCode_302_Found
            || pResponse->getStatusCode() == HttpStatusCode_301_MovedPermanently) {
            const char* pValueLocation = pResponse->getHeader( "Location" );
            if (!pValueLocation)    return HttpResult_error_header;
            
            cString value = pValueLocation;
            
            // remove http://
            if (value.size() < 7)   return HttpResult_error_header;
            s32 pos = value.find("http");
            if (pos != 0)          { trace(_T("302 no http started")); return HttpResult_error_header; }
            if (value[4] == 's')   { trace(_T("https is unsupported!")); return HttpResult_error; }
            value = value.substr(7, -1);
            
            // host. uri.
            pos = value.find('/');
            if (pos < 0)    return HttpResult_error_header;
            cString host = value.substr(0, pos);
            PORT    port = 80;
            pos = host.find(':');
            if (pos >= 0) {
                cString poststr = host.substr(pos+1, -1);
                port = StringUtils::StrToU32(poststr.c_str());
                host.erase(pos, -1);
            }
            cString uri  = value.substr(pos, -1);
            
            // request.
			String cHost;
			StringUtils::StrToStr(host, cHost);
            if (p->m_host != cHost
                || p->m_requestURI != uri
                || p->m_port != port) {
                
#ifdef DEBUG
                String debugHost, debugUri;
                StringUtils::StrToStr(host, debugHost);
                StringUtils::StrToStr(uri, debugUri);
                trace(_T("http %d %s:%d/%s"),
                      (int)pResponse->getStatusCode(),
                      debugHost.c_str(), (int)port, debugUri.c_str());
#endif
                
                // set host.
                StringUtils::StrToStr(host, p->m_host);
                p->m_port = port;
                p->m_connectAddr = SocketUtils::AddrCreateAuto(cHost, port);
                
                // request line.
                this->setRequestLine(p->m_requestMethod, uri.c_str());
                
                // request.
                return this->request(pContent, len, pResponse);
            } // if.
            
            return HttpResult_error_header;
        }
        

        //-----------------------------------------------
        // 处理接收buf不够长情况
        const char* pValue = pResponse->getHeader( "Content-Length" );
        if ( NULL == pValue )   // 无报文
        {
            if (NULL != pResponse->getHeader("Transfer-Encoding")) {
                p->m_contentIsChunk = true;

                s32 len = p->readChunkSize(this);
                p->m_contentChunkLen = len;
                if (len == -1)
                {
                    return HttpResult_timeout_recv;
                }

                return HttpResult_ok_haveContent;

            } else {
                return HttpResult_ok;
            }
        }
        else
        {
            pos = StringUtils::StrToS32( pValue );
            p->m_contentChunkLen = pos;

            if (p->m_contentChunkLen > 0)
                return HttpResult_ok_haveContent;
            else
            {
                p->reset(this);
                return HttpResult_ok;
            }
        }
    }

    //-----------------------------------------------
    HttpResult HttpRequest::requestContent( Byte* pBuf, u32 len, OUT u32& readLen )
    {
        PRIVATE_CLASS_GET(p);
        readLen = 0;

        char bufTemp[1024];

        // 从buf读取.
        if (p->m_bufContent.size() > 0)
        {
            readLen = Math::Min(len, p->m_bufContent.size());
            readLen = Math::Min(readLen, p->m_contentChunkLen);
			memmove(pBuf, p->m_bufContent.begin(), readLen);

			//
			u32 newsize = p->m_bufContent.size()-readLen;
			memmove(p->m_bufContent.begin(), p->m_bufContent.begin()+readLen, newsize);
			p->m_bufContent.resize(newsize);

			p->m_contentChunkLen -= readLen;

			if (p->m_contentIsChunk && p->m_contentChunkLen <= 0)
			{
				if (newsize < 2)
				{
					s32 rLen = this->onReadPart(*(p->m_tcpTransceiver), bufTemp, 2);
					if (rLen <= 0)
					{
						p->reset(this);
						return HttpResult_timeout_recv;
					}

					p->m_bufContent.add(bufTemp, rLen);
				} // if.

				char buf[2];
				buf[0] = p->m_bufContent[0];
				buf[1] = p->m_bufContent[1];

				if (buf[0] != CR && buf[1] != LF)
				{
					p->reset(this);
					return HttpResult_error;
				}

				u32 newsize = p->m_bufContent.size()-2;
				memmove(p->m_bufContent.begin(), p->m_bufContent.begin()+2, newsize);
				p->m_bufContent.resize(newsize);

				if (p->m_bufContent.size() < 5)
				{
					s32 rLen = this->onReadPart(*(p->m_tcpTransceiver), bufTemp, sizeof(bufTemp));
					if (rLen <= 0)
					{
						p->reset(this);
						return HttpResult_timeout_recv;
					}

					p->m_bufContent.add(bufTemp, rLen);
				}

				u32 lenTmp = p->readChunkSize(this);
				if (lenTmp == 0)
				{
					p->reset(this);
					return HttpResult_ok;
				}

				p->m_contentChunkLen = lenTmp;

				u32 readlen2 = 0;
				u32 len2 = len-readLen;
				Byte* pBuf2 = pBuf+readLen;

				HttpResult ret = requestContent(pBuf2, len2, readlen2);
				readLen += readlen2;
				return ret;
			} // if.

			return HttpResult_ok_haveContent;
        } // if.


        // 从网络读取.
        if (p->m_tcpTransceiver.isEmpty() || p->m_tcpTransceiver->isClosed())
        {
            p->reset(this);
            return HttpResult_error;
        }

        s32 slen = this->onReadPart(*(p->m_tcpTransceiver), bufTemp, sizeof(bufTemp)); //(s32)p->m_tcpTransceiver->readPart((char*)(pBuf+readLen), len-readLen);
        if (slen <= 0)
        {
            p->reset(this);
            return HttpResult_timeout_recv;
        }

        p->m_bufContent.add(bufTemp, slen);
        return requestContent(pBuf, len, readLen);
    }

    //-----------------------------------------------
    void HttpRequest::setTimeout( u32 timeout )
    {
        PRIVATE_CLASS_GET(p);
        p->m_timeout = timeout;
        if (p->m_tcpTransceiver.get()) 
        {
            SocketUtils::SetRevTimeout(p->m_tcpTransceiver->socket(), timeout);
        }
    }

    //-----------------------------------------------
    hyc::u32 HttpRequest::getTimeout() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->m_timeout;
    }

} // end namespace hyc

#endif // HYC_COMPILE_MODULE_NET
