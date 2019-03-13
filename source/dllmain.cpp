//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  用于初始化和析构 common 库
//

#include "preheader.h"

#include "../include/os/thread.h"
#include "../include/common/cnObjectPtrSafe.h"
#include "../include/utils/encodingUtils.h"
#include "../include/container/yunStreamHelper.h"
#include "globalVar.hpp"

using namespace hyc;

//#define CN_PLATFORM CN_PLATFORM_WINDOWS

namespace hyc
{
    Log*            g_pTraceLog = NULL;
    SyncObject*     g_pTraceSync = NULL;
    hyc::integer    g_mainThreadId = 0;

#ifdef DEBUG
    std::map<void*, DebugInfo>* g_pDebugMap = NULL;
#endif // DEBUG
}

static bool CN_IS_BIG_ENDIAN_ = false;
static bool CN_IS_ARCHITECTURE_64_ = false;
static bool CN_IS_UTF8_CSTRING_ = false;

bool _CN_IS_BIG_ENDIAN()         { return CN_IS_BIG_ENDIAN_; }
bool _CN_IS_ARCHITECTURE_64()    { return CN_IS_ARCHITECTURE_64_; }
bool _CN_IS_UTF8_CSTRING()       { return CN_IS_UTF8_CSTRING_; }

#if CN_PLATFORM != CN_PLATFORM_WINDOWS
#include <signal.h>
#include <locale.h>

int g_retval = 0;
void thread_handle_quit(int)
{
    if (hyc::Thread::CurThreadId() != hyc::g_mainThreadId)
        pthread_exit(&g_retval);
}

#endif


//-----------------------------------------------
inline void dll_init()
{
    // 全局对象.
    g_pTraceSync = new SyncObject;
#ifdef _DEBUG
    g_pDebugMap = new std::map<void*, DebugInfo>();
#endif // _DEBUG


    print_debug_line(_T("load hyc"));

    // big endian.
    if ( (*(unsigned short*)"\1\0")!=1 )
    {
        CN_IS_BIG_ENDIAN_ = true;
    }

    // arch 64.
    if ( sizeof(int) == 8 )
    {
        CN_IS_ARCHITECTURE_64_ = true;
    }

    // string utf8 encoding.
    if (0 == strcmp("严", (char*)"\xe4\xb8\xa5"))
    {
        CN_IS_UTF8_CSTRING_ = true;
    }

    //assert(0 == wcscmp(L"严", (wchar_t*)"\x4e\x25"));

    //----------------------------------------------------------------------------------------------------
    //*	initialize other
    //----------------------------------------------------------------------------------------------------
#ifdef DEBUG
    Trace_init( TraceType_console, NULL );
#endif

    //----------------------------------------------------------------------------------------------------
    //*	initialize Network
    //----------------------------------------------------------------------------------------------------
#ifdef HYC_COMPILE_MODULE_NET
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
    WSADATA	wsaData;

    // 初始化wsaData
    if ( NO_ERROR != ::WSAStartup(MAKEWORD(2,2), &wsaData) )
    {
        trace(_T("Network init error!"));
        return/*Log_ErrorHandle( _T("hyc init failure: WSAStartup !") )*/;
    }
#endif
#endif

    hyc::g_mainThreadId = Thread::CurThreadId();

    //----------------------------------------------------------------------------------------------------
    //*	initialize linux.
    //----------------------------------------------------------------------------------------------------
#if CN_PLATFORM != CN_PLATFORM_WINDOWS

    // 设置local.
    setlocale(LC_ALL, "zh_CN");

    // 设置线程退出信号.
    struct sigaction actQuit;
    sigemptyset(&actQuit.sa_mask);
    actQuit.sa_flags = SA_RESTART;
    actQuit.sa_handler = thread_handle_quit;
    //HY_VERIFY(0 == sigaction(SIGALRM, &actQuit, NULL));
    HY_VERIFY(0 == sigaction(SIGQUIT, &actQuit, NULL));

    // 忽略SIGPIPE信号.
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    act.sa_handler = SIG_IGN;
    HY_VERIFY(0 == sigaction(SIGPIPE, &act, NULL));
    HY_VERIFY(0 == sigaction(SIGCHLD, &act, NULL));
#endif
}

//-----------------------------------------------
inline void dll_destory()
{
#ifdef HYC_COMPILE_MODULE_NET
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
    //----------------------------------------------------------------------------------------------------
    //*	destroy Network
    //----------------------------------------------------------------------------------------------------
    ::WSACleanup();
#endif
#endif

    print_debug_line(_T("unload hyc"));

    // 全局对象.
    SAFE_DELETE(g_pTraceSync);
    SAFE_DELETE(g_pTraceLog);

#ifdef _DEBUG
    if (g_pDebugMap->size() > 0) // log.
    {
        for (std::map<void*, DebugInfo>::iterator itr = g_pDebugMap->begin(); itr != g_pDebugMap->end(); itr++)
        {
            cString str;
            str.format("%s(%d) : client block leak size [%d]\n", itr->second.filename.c_str(), itr->second.nLine, itr->second.size);
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
            OutputDebugStringA(str.c_str());
#else
            printf("%s", str.c_str());
#endif
        }
    }
    SAFE_DELETE(g_pDebugMap);
#endif // _DEBUG
}


#if CN_PLATFORM == CN_PLATFORM_WINDOWS
namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	dll初始化
    //----------------------------------------------------------------------------------------------------

    class DLLInit
    {
    public:
        /*---------------------------
         初始化
        ---------------------------*/
        DLLInit()
        {
            dll_init();
        }

        /*---------------------------
         析构
        ---------------------------*/
        ~DLLInit()
        {
            dll_destory();

        }
    };
    static DLLInit g_dllInit;

} // end namespace hyc

#else

void dll_main_init() __attribute__((constructor));
void dll_main_destory() __attribute__((destructor));

void dll_main_init() { 
    dll_init();
} 

void dll_main_destory() { 
    dll_destory();
} 
#endif
