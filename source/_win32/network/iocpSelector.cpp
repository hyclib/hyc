//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//

#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET
#if defined(CN_NET_IOCP) && defined(HYC_COMPILE_MODULE_NET_ASYNC)

#include "../../../include/network/selector/iocpSelector.h"

namespace hyc
{
    // 退出标记不能为0; 但收到新连接时为0
    // 设置为 SOCKET_ERROR-1
#define QUIT_MARK   SOCKET_ERROR-1

    //-----------------------------------------------
    IOCPSelector::IOCPSelector()
        : _handle(NULL)
    {
        int nProcessors = Platform::GetProcessorNum();

        _handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, nProcessors*2);
        if(_handle == NULL)
        {
            assert(0);
            error_all( _T("Error Selector::setup: CreateIoCompletionPort!" ));
            exit(-1);
        }
    }

    //-----------------------------------------------
    IOCPSelector::~IOCPSelector()
    {
        destroy();
    }

    //-----------------------------------------------
    void IOCPSelector::destroy()
    {
        if (_handle)
        {
            PostQueuedCompletionStatus(_handle, QUIT_MARK, NULL, NULL);
            CloseHandle(_handle);
            _handle = NULL;
        }
    }

    //-----------------------------------------------
    void IOCPSelector::initialize( SelectorEvent* handler )
    {
        assert(handler && _handle);

        HANDLE socket = reinterpret_cast<HANDLE>(handler->socket());
        if(::CreateIoCompletionPort(socket, _handle, reinterpret_cast<ULONG_PTR>(handler), 0) == NULL)
        {
            assert(0);
            error_all( _T("Error Selector::initialize: CreateIoCompletionPort!"));
            return;
        }
    }

    //-----------------------------------------------
    void IOCPSelector::remove( SelectorEvent* handler, AsyncInfo* pRemoveInfo )
    {
        //if (!handler || !pRemoveInfo)   return;

        if(_handle && !PostQueuedCompletionStatus(_handle, QUIT_MARK, reinterpret_cast<ULONG_PTR>(handler), (LPWSAOVERLAPPED)pRemoveInfo))
        {
            assert(0);
            error_all( _T("Error Selector::removeOp: PostQueuedCompletionStatus!"));
        }
    }

    //-----------------------------------------------
    void IOCPSelector::select( OUT Array<SelectorEventInfo>* parrFd, const s32& timeout )
    {
        Array<SelectorEventInfo>& arrFd = *parrFd;

        SelectorEventInfo sinfo;

        ULONG_PTR key;
        LPOVERLAPPED ol;
        DWORD count;

        if(!GetQueuedCompletionStatus(_handle, &count, &key, &ol, timeout))
        {
            int err = WSAGetLastError();
            // 
            //             if (err != ERROR_MORE_DATA)
            //             {
            //                 error_all(_T("iocp ");
            //             }

            if(ol == NULL)
            {
                if(err == WAIT_TIMEOUT)
                {
                    return;
                }
                else
                {
                    //exit(0);
                    error_all( _T("IOCPSelector::getNextHandler Selector!") );
                    return;
                }
            }

            if (err != WSA_OPERATION_ABORTED
                && err != ERROR_NETNAME_DELETED)
            {
                error_all(_T("iocp select error"));
                return;
            }

            assert(ol && key);

            //AsyncInfo* info = static_cast<AsyncInfo*>(ol);
			AsyncInfo* info = reinterpret_cast<AsyncInfo*>(ol);
            info->inCountBytes = SOCKET_ERROR;
            info->inError = err;

            sinfo.pInfo = info;
            sinfo.pEvent = reinterpret_cast<SelectorEvent*>(key);
            arrFd.push_back( sinfo );
            return;
        }

        // drop the message if marked QUIT_MARK.
        if (QUIT_MARK == count)
        {
            return;
        }

        assert(ol && key);
        AsyncInfo* info = reinterpret_cast<AsyncInfo*>(ol);
        info->inCountBytes = count;
        info->inError = 0;

        sinfo.pInfo = info;
        sinfo.pEvent = reinterpret_cast<SelectorEvent*>(key);
        arrFd.push_back( sinfo );
    }

} // end namespace hyc

#endif // defined(CN_NET_IOCP) && defined(HYC_COMPILE_MODULE_NET_ASYNC)
#endif