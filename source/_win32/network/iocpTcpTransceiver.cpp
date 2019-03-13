//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
#if defined(CN_NET_IOCP) && defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/network/socket/tcpTransceiver.h"
#include "../../../include/network/socket/socketUtils.h"
#include "../../../include/network/socket/netStatus.h"
#include "../../../include/network/socket/async/win32/iocpTcpTransceiver.h"


namespace hyc
{
// #ifdef _DEBUG
//     AtomicBase<s32> g_tick = 0;
// #endif // _DEBUG
    //----------------------------------------------------------------------------------------------------
    //*	IOCPTcpTransceiver
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    IOCPTcpTransceiver::IOCPTcpTransceiver( Selector& selector, SOCKET socket )
    {
        //print_debug_line("IOCPTcpTransceiver create %d", ++g_tick);

        _socket = socket;

        //
        // On Windows, limiting the buffer size is important to prevent
        // poor throughput performances when transfering large amount of
        // data. See Microsoft KB article KB823764.
        //
        _maxSendPacketSize = SocketUtils::GetSendBufSize(socket) / 2;
        if(_maxSendPacketSize < 512)
        {
            _maxSendPacketSize = 0;
        }

        _maxReceivePacketSize = SocketUtils::GetRevBufSize(socket);
        if(_maxReceivePacketSize < 512)
        {
            _maxReceivePacketSize = 0;
        }
    }

    //-----------------------------------------------
    IOCPTcpTransceiver::~IOCPTcpTransceiver()
    {
        //print_debug_line("IOCPTcpTransceiver destory %d", --g_tick);
    }

    //-----------------------------------------------
    hyc::SocketOperation IOCPTcpTransceiver::initialize()
    {
        /*if(_status == SocketStatus_NeedConnect)
        {
            _status = SocketStatus_ConnectPending;
            return SocketOperation_Connect;
        }

        assert(_status == SocketStatus_Connected);*/
        return SocketOperation_None;
    }

    //-----------------------------------------------
    bool IOCPTcpTransceiver::startWrite( AsyncInfo* pInfo, AsyncData* pData )
    {
        if (!pData || !pInfo)   return false;

        ByteStream* buf = pData->pBuf;

        assert(!buf->empty());

        Byte* begin;

        if (pData->pPosWriteAll != NULL)
            begin = pData->pPosWriteAll;
        else
            begin = buf->_curPos;
        assert(begin < buf->end() && begin >= buf->begin());
 
        int packetSize = static_cast<int>(buf->end() - begin);
        if(_maxSendPacketSize > 0 && packetSize > _maxSendPacketSize)
        { 
            packetSize = _maxSendPacketSize;
        }

        pInfo->__inBuf.len = packetSize;
        pInfo->__inBuf.buf = reinterpret_cast<char*>(begin);
        int err = ::WSASend(_socket, &pInfo->__inBuf, 1, &pInfo->inCountBytes, 0, (LPWSAOVERLAPPED)pInfo, NULL);
        if(err == SOCKET_ERROR)
        {
            pInfo->inError = NetStatus::ErrorCode();

            if(!NetStatus::IsWouldBlock(pInfo->inError))
            {
                if(NetStatus::IsConnectLost(pInfo->inError))
                {
                    //trace(_T("Error IOCPTcpTransceiver::startWrite : ConnectLostException!");
                    return false;
                }
                else
                {
                    trace(_T("Error IOCPTcpTransceiver::startWrite : SocketException!"));
                    return false;
                }
            }
        }
        return  true; // packetSize == static_cast<int>(buf->end() - buf->_curPos);
    }

    //-----------------------------------------------
    AsyncTransceiver::Result IOCPTcpTransceiver::finishWrite( AsyncInfo* pInfo, AsyncData* pData )
    {
        if (!pData || !pInfo)   return Result_Error;

        ByteStream* buf = pData->pBuf;

        /*if(_status < SocketStatus_Connected)
        {
            return Result_Error;
        }*/

        if(pInfo->inCountBytes == SOCKET_ERROR)
        {
            if(NetStatus::IsConnectLost(pInfo->inError))
            {
                //trace(_T("Error IOCPTcpTransceiver::finishWrite : ConnectLostException!");
                return Result_Error;
            }
            else
            {
                trace(_T("Error IOCPTcpTransceiver::finishWrite : SocketException!"));
                return Result_Error;
            }
        }

        if (pData->pPosWriteAll == NULL)
            buf->_curPos += pInfo->inCountBytes;
        else
            pData->pPosWriteAll += pInfo->inCountBytes;

        //assert (buf->_curPos <= buf->end());
        return Result_WouldBlock;
    }

    //-----------------------------------------------
    bool IOCPTcpTransceiver::startRead( AsyncInfo* pInfo, AsyncData* pData )
    {
        if (!pData || !pInfo)   return false;

        ByteStream* buf = pData->pBuf;

        assert(!buf->empty() && buf->_curPos != buf->end());

        if (NULL == buf->_curPos || buf->end() <= buf->_curPos)    buf->_curPos = buf->begin();

        int packetSize = static_cast<int>(buf->end() - buf->_curPos);
//         if(_maxReceivePacketSize > 0 && packetSize > _maxReceivePacketSize)
//         {
//             packetSize = _maxReceivePacketSize;
//         }

        pInfo->__inBuf.len = packetSize;
        pInfo->__inBuf.buf = reinterpret_cast<char*>(&*buf->_curPos);
        int err = ::WSARecv(_socket, &pInfo->__inBuf, 1, &pInfo->inCountBytes, &pInfo->__inFlags, (LPWSAOVERLAPPED)pInfo, NULL);
        if(err == SOCKET_ERROR)
        {
            pInfo->inError = NetStatus::ErrorCode();

            if(!NetStatus::IsWouldBlock(pInfo->inError))
            {
                if(NetStatus::IsConnectLost(pInfo->inError))
                {
                    //trace(_T("Error IOCPTcpTransceiver::startRead : ConnectLostException!");
                    return false;
                }
                else
                {
                    error_all(_T("Error IOCPTcpTransceiver::startRead : SocketException!"));
                    return false;
                }
            }
        }
        return true;
    }

    //-----------------------------------------------
    AsyncTransceiver::Result IOCPTcpTransceiver::finishRead( AsyncInfo* pInfo, AsyncData* pData )
    {
        if (!pData || !pInfo)   return Result_Error;
        ByteStream* buf = pData->pBuf;

        if (NULL == buf->_curPos)    buf->_curPos = buf->begin();

        if(pInfo->inCountBytes == SOCKET_ERROR)
        {
            if(NetStatus::IsConnectLost(pInfo->inError))
            {
                //trace(_T("Error IOCPTcpTransceiver::finishRead : ConnectLostException!");
                return Result_Error;
            }
            else
            {
                error_all(_T("Error IOCPTcpTransceiver::finishRead : SocketException!"));
                return Result_Error;
            }
        }
        else if(pInfo->inCountBytes == 0)
        {
            trace(_T("Error IOCPTcpTransceiver::finishRead : ConnectLostException2!"));
            return Result_Error;
        }

        buf->_curPos += pInfo->inCountBytes;

        if ( buf->end() >= buf->_curPos )
        {
            return Result_WouldBlock;
        }
        else
        {
            return Result_Error;
        }
    }

    //-----------------------------------------------
    void IOCPTcpTransceiver::close()
    {
        if (_socket != INVALID_SOCKET)
        {
            // shutdown
            if(::shutdown(_socket, SD_BOTH) == SOCKET_ERROR)
            {
                error_all(_T("Error IOCPTcpTransceiver::close: shutdown!"));
                Transceiver::close();
                return;
            }

            // disconnectex
            LPFN_DISCONNECTEX DisconnectEx = NULL; // a pointer to the 'DisconnectEx()' function
            GUID GuidDisconnectEx = WSAID_DISCONNECTEX; // The Guid
            DWORD dwBytes;
            if(WSAIoctl(_socket, 
                SIO_GET_EXTENSION_FUNCTION_POINTER,
                &GuidDisconnectEx,
                sizeof(GuidDisconnectEx),
                &DisconnectEx,
                sizeof(DisconnectEx),
                &dwBytes,
                NULL, 
                NULL) == SOCKET_ERROR)
            {
                trace(_T("Error IOCPTcpTransceiver::close get DisconnectEx ptr!"));
                Transceiver::close();
                return;
            }

            if (!DisconnectEx(_socket, NULL, 0, 0))
            {
                trace(_T("Error IOCPTcpTransceiver::close: DisconnectEx!"));
                return;
            }
            Transceiver::close();
        } // if
    }

} // end namespace hyc

#endif // CN_NET_IOCP
#endif // HYC_COMPILE_MODULE_NET
