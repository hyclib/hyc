//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_PLATFORM_H_20130325155055__
#define __BP_PLATFORM_H_20130325155055__


namespace hyc
{
	//----------------------------------------------------------------------------------------------------
    //!	platform independent API
    //----------------------------------------------------------------------------------------------------
    class HYC_API Platform {
    public:
        typedef struct {
            u64 totalVirtual;
            u64 availVirtual;
            u64 totalPhy;
            u64 availPhy;
        } MemoryStatus;
        
    public:
        static s32      GetLastError();
        static String   GetLastErrorMsg( s32 errCode );

        //! 获得处理器个数.
        static uinteger GetProcessorNum();

        //! 获得内存使用情况.
        static void     GetMemoryStatus( OUT MemoryStatus& status );

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        //! 显示/隐藏任务栏
        static void     ShowTray( bool show );
        static bool     IsShowTray();

        //! 显示/隐藏桌面
        static void     ShowDesktop( bool show );
        static bool     IsShowDesktop();

#endif // CN_PLATFORM == CN_PLATFORM_WINDOWS
    };


} // end namespace hyc

#endif // end of define __BP_PLATFORM_H_20130325155055__
