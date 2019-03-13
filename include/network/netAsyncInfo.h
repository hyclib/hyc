//
//  netAsyncInfo.h
//  hyc
//
//  Created by brainpoint on 14-4-27.
//  Copyright (c) 2014年 hy. All rights reserved.
//

#ifndef hyc_netAsyncInfo_h
#define hyc_netAsyncInfo_h

#include "../container/byteStream.h"
#include "../template/concurrent/queue.hpp"

namespace hyc {

    class SelectorEvent;
    
    //----------------------------------------------------------------------------------------------------
    //!	AsyncData.
    //! 异步数据表示一次发送或接收的数据,系统可以继承此类存放自定义数据.
    //----------------------------------------------------------------------------------------------------
    struct AsyncData {
        AsyncData() : pBuf(NULL), pPosWriteAll(NULL)    {}
        virtual ~AsyncData() {}
        
        /**
         * @brief 发送接收流.
         */
        ByteStream* pBuf;
        
        /**
         * @brief 当发送整个流时,使用此标记记录要开始发送的位置.
         *      当为NULL时表示从流的_curPos位置开始发送.
         */
        Byte*   pPosWriteAll;
    };
    
    //----------------------------------------------------------------------------------------------------
    //!	AsyncInfo.
    //! 异步信息用于存放AsyncData
    //----------------------------------------------------------------------------------------------------
#if  defined(CN_NET_IOCP)
    struct HYC_API AsyncInfo
    {
    	AsyncInfo()   { ZeroMemory(&event, sizeof(WSAOVERLAPPED)); inError = 0; inCountBytes = 0; __info = NULL; __inFlags = 0; }
    	virtual ~AsyncInfo() {}
        
    	WSAOVERLAPPED		event;
        
        /**
         * @brief 是否失败.
         */
        inline bool        isFail() const              { return inError != 0; }
        
        /**
         * @brief 通信信息.
         */
        inline bool        pushBackInfo( AsyncData* data )   { assert(!haveInfo()); __info = data; return true; }
        inline bool        pushFrontInfo( AsyncData* data )  { assert(!haveInfo()); __info = data; return true; }
        inline bool        popInfo( OUT AsyncData*& data )   { bool r = false; if (__info) { data = __info; __info = NULL; r = true; } return r; }
        inline bool        haveInfo() const                  { return NULL != __info; }
        inline bool        isFinishBuf( AsyncData* data ) const { return data->pPosWriteAll == NULL ? data->pBuf->_curPos >= data->pBuf->end() : data->pPosWriteAll >= data->pBuf->end(); }
        
        s32					inError;
        
        // 异步io完成的bytes数. SOCKET_ERROR/0 则为错误.
        unsigned long       inCountBytes;
        
    	// don't care.
        AsyncData*          __info;
    	INT                 __inUdpFromLen;
    	WSABUF              __inBuf;
    	DWORD               __inFlags;
    };
    
#elif defined(CN_NET_EPOLL)
	struct HYC_API AsyncInfo
	{
    	AsyncInfo(SelectorEvent* pHandler) : __pHandler(pHandler) {  inError = 0; inCountBytes = 0; }
    	virtual ~AsyncInfo() {}
        
        inline bool        isFail() const                   { return inError != 0; }
        inline bool        pushBackInfo( AsyncData* data )  { return __info.push_back(data); }
        inline bool        pushFrontInfo( AsyncData* data ) { return __info.push_front(data); }
        inline bool        popInfo( OUT AsyncData*& data )  { return __info.pop_front(data); }
        inline bool        haveInfo() const                 { return !__info.empty(); }
        inline bool        isFinishBuf( AsyncData* data ) const { return data->pPosWriteAll == NULL ? data->pBuf->_curPos >= data->pBuf->end() : data->pPosWriteAll >= data->pBuf->end(); }
        
        s32					inError;
        unsigned long       inCountBytes;
        
        // don't care.
        SelectorEvent*      __pHandler;		// 指向SelectorEvent指针.
        concurrent::ConcurrentQueue<AsyncData*>	__info;     // 通信队列.
	};
    
#elif defined(CN_NET_KQUEUE)
    struct HYC_API AsyncInfo
	{
        typedef void* Info;
        
    	AsyncInfo() {  inError = 0; inCountBytes = 0; }
    	virtual ~AsyncInfo() {}
        
        inline bool        isFail() const              { return inError != 0; }
        inline bool        pushBackInfo( Info info )   { return __info.push_back(info); }
        inline bool        pushFrontInfo( Info info )  { return __info.push_front(info); }
        inline bool        popInfo( OUT Info& info )   { assert(haveInfo()); return __info.pop_front(info); }
        inline bool        haveInfo() const            { return !__info.empty(); }
        inline bool        isFinishBuf( ByteStream* buf ) const    { return buf->_curPos >= buf->end(); }
        
        s32					inError;
        unsigned long       inCountBytes;
        
        // don't care.
        concurrent::ConcurrentQueue<Info>	__info;     // 通信队列.
	};
    
#endif

    
}

#endif
