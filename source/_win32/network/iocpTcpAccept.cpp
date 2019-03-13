//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
#if defined(CN_NET_IOCP) && defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/container/array.h"
#include "../../../include/network/socket/tcpTransceiver.h"
#include "../../../include/network/socket/netStatus.h"

#include "../../../include/network/socket/async/win32/iocpTcpTransceiver.h"
#include "../../../include/network/socket/async/win32/iocpTcpAcceptor.h"

namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	IOCPTcpAcceptor
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    IOCPTcpAcceptor::IOCPTcpAcceptor(Selector& selector, bool isBlock)
        : _rSelector(selector)
        , _acceptFd(INVALID_SOCKET)
        , _block(isBlock)
    {
    }

    //-----------------------------------------------
    IOCPTcpAcceptor::IOCPTcpAcceptor( Selector& selector, bool isBlock, SOCKET socket, bool bAlreadyBinded )
        : TcpAcceptor(socket, bAlreadyBinded)
        , _rSelector(selector)
        , _acceptFd(INVALID_SOCKET)
        , _block(isBlock)
    {
    }

    //-----------------------------------------------
    IOCPTcpAcceptor::~IOCPTcpAcceptor()
    {
    }

    //-----------------------------------------------
    bool IOCPTcpAcceptor::listen( const String& host, int port, ProtocolSupport protocol )
    {
        if (!TcpAcceptor::listen(host, port, protocol))
            return false;

        if (SOCKET_ERROR != _socket && !_block)
        {
            SocketUtils::SetBlock(_socket, false);
        }

        return true;
    }

    //-----------------------------------------------
    AsyncTransceiverPtr IOCPTcpAcceptor::doAccept()
    {
        SOCKET fd;

        if(_acceptFd == INVALID_SOCKET)
        {
            return NULL; 
        }

        if(setsockopt(_acceptFd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&_acceptFd, sizeof(_acceptFd)) == 
            SOCKET_ERROR)
        {
            SocketUtils::CloseSocket(_acceptFd);
            _acceptFd = INVALID_SOCKET;
            return NULL;
        }

        fd = _acceptFd;
        _acceptFd = INVALID_SOCKET;

        AsyncTransceiverPtr pTrans = new IOCPTcpTransceiver(_rSelector, fd);
        //if (!pTrans.get())  return NULL;
        /*else */
        if (SocketOperation_None != pTrans->initialize())
        {
            pTrans->close();
            return NULL;
        }
        return pTrans;

    }

    //-----------------------------------------------
    hyc::AsyncTransceiverPtr IOCPTcpAcceptor::acceptBlock()
    {
        SOCKET fd;
        fd = SocketUtils::Accept(_socket);
        if (SOCKET_ERROR == fd)
        {
            return NULL;
        }
        return new IOCPTcpTransceiver(_rSelector,fd);
    }

    //-----------------------------------------------
    void IOCPTcpAcceptor::startAccept(AsyncInfo* pInfo)
    {
        LPFN_ACCEPTEX AcceptEx = NULL; // a pointer to the 'AcceptEx()' function
        GUID GuidAcceptEx = WSAID_ACCEPTEX; // The Guid
        DWORD dwBytes;
        if(WSAIoctl(_socket, 
            SIO_GET_EXTENSION_FUNCTION_POINTER,
            &GuidAcceptEx,
            sizeof(GuidAcceptEx),
            &AcceptEx,
            sizeof(AcceptEx),
            &dwBytes,
            NULL, 
            NULL) == SOCKET_ERROR)
        {
            error_all(_T("Error TcpAcceptor::startAccept get AcceptEx pt!"));
            return;
        }        

        assert(_acceptFd == INVALID_SOCKET);
        _acceptFd = SocketUtils::CreateSocket(false, _addr.ss_family);
        const int sz = static_cast<int>(_acceptBufSize);

		//ZeroMemory(pInfo, sizeof(AsyncInfo));
        if(!AcceptEx(_socket, _acceptFd, _acceptBuf, 0, sz, sz, &pInfo->inCountBytes, (LPWSAOVERLAPPED)pInfo))
        {
            s32 err = NetStatus::ErrorCode();

            if(!NetStatus::IsWouldBlock(err))
            {
                error_all(_T("Error TcpAcceptor::startAccept: IsWouldBlock!"));
                return;
            }
        }
    }


    //-----------------------------------------------
    void IOCPTcpAcceptor::finishAccept(AsyncInfo* pInfo)
    {
        if(pInfo->inCountBytes == SOCKET_ERROR || _socket == INVALID_SOCKET)
        {
            SocketUtils::CloseSocket(_acceptFd);
            _acceptFd = INVALID_SOCKET;
            _acceptError = pInfo->inError;
        }
    }

    //-----------------------------------------------
    void IOCPTcpAcceptor::close()
    {
//         if (_socket != INVALID_SOCKET)
//         {
//             // shutdown
//             if(::shutdown(_socket, SD_BOTH) == SOCKET_ERROR)
//             {
//                 error_all(_T("Error IOCPTcpAcceptor::close: shutdown!");
//                 TcpAcceptor::close();
//                 return;
//             }

            TcpAcceptor::close();
//         } // if
    }

} // end namespace hyc

#endif // CN_NET_IOCP
#endif // HYC_COMPILE_MODULE_NET
