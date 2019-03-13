// This is the main DLL file.

#include "stdafx.h"
#include "hyc/include/hyc.h"

// common
#include "_Common/cnMath.h"
#include "_Common/cnSingleton.h"
#include "_Common/String.h"

// container

// utils
#include "Utils/CryptUtils.h"
#include "Utils/EncodingUtils.h"
#include "Utils/FileUtils.h"

// IO
#include "IO/FileStream.h"

class DLLInit
{
public:
    /*---------------------------
    初始化
    ---------------------------*/
    DLLInit()
    {
        hyc::FileUtils::DirSetCurrent(hyc::FileUtils::DirGetRuningDir().c_str());
        hyc::Trace_init( hyc::TraceType_file, _T("log/hyc.log") );
    }

    /*---------------------------
    析构
    ---------------------------*/
    ~DLLInit()
    {
    }
};
static DLLInit g_dllInit;
