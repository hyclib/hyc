//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// base network libaray
// author:       lipengxiang
// description:  
//
#ifdef HYC_COMPILE_MODULE_NET

#ifndef __BP_NETSTATUS_H_20110617112323__
#define __BP_NETSTATUS_H_20110617112323__


namespace hyc
{
	//----------------------------------------------------------------------------------------------------
	//!	NetStatus 可以判断最后一次操作或当前的网络状态
	//----------------------------------------------------------------------------------------------------
    class HYC_API NetStatus
    {
    public:
        //! 错误码.
        static s32  ErrorCode();

        //! 超时.
        static bool IsTimeout( const s32& errorCode );
        //! 判断网络是否中断
        static bool IsInterrupted( const s32& errorCode );
        //! 判断接受连接是否中断
        static bool IsInterruptedAccept( const s32& errorCode );
        //! 连接失败
        static bool IsConnectFailed( const s32& errorCode );
        //! 连接拒绝
        static bool IsConnectRefused( const s32& errorCode );
        //! 正在连接
        static bool IsConnectInProgress( const s32& errorCode );
        //! 连接不可到达
        static bool IsConnectNetUnReach( const s32& errorCode );
        //! 连接丢失
        static bool IsConnectLost( const s32& errorCode );
        //! 接收中断
        static bool IsRecvTruncated( const s32& errorCode );
        //! 是否还在buffer
        static bool IsNoBuffers( const s32& errorCode );
        //! 是否将要阻塞.
        static bool IsWouldBlock( const s32& errorCode );
    };
	 

} // end namespace hyc

#endif // end of define __BP_NETSTATUS_H_20110617112323__
#endif // HYC_COMPILE_MODULE_NET