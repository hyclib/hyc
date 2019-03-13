//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// YunFramework
// author:       lipengxiang
// description:  
//
#include "../preheader.h"

#include "../../include/common/cnObjectPtr.h"
#include "../../include/utils/dynLibUtils.h"

#if !defined(CN_PLATFORM_MOBILE)

#if CN_PLATFORM == CN_PLATFORM_LINUX || CN_PLATFORM == CN_PLATFORM_MACOSX
#include <dlfcn.h>
#endif

namespace hyc
{

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#    define DYNLIB_HANDLE HMODULE
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) FreeLibrary( a )

//     struct HINSTANCE__;
//     typedef struct HINSTANCE__* hInstance;

#elif CN_PLATFORM == CN_PLATFORM_LINUX || CN_PLATFORM == CN_PLATFORM_MACOSX
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

// #elif CN_PLATFORM == OGRE_PLATFORM_SYMBIAN
// #    define DYNLIB_HANDLE void*
// #    define DYNLIB_LOAD( a ) 0
// #    define DYNLIB_GETSYM( a, b ) 0
// #    define DYNLIB_UNLOAD( a ) 0

#endif



    //----------------------------------------------------------------------------------------------------
    //!	DynLib::PrivateClass
    //----------------------------------------------------------------------------------------------------
    class DynLib::PrivateClass
    {
    public:
        PrivateClass() : _hInst(NULL)  {}
        String          _name;
        DYNLIB_HANDLE   _hInst;
    };
    
    //----------------------------------------------------------------------------------------------------
    //!	DynLib
    //----------------------------------------------------------------------------------------------------
    
    //-----------------------------------------------
    DynLib::DynLib(const String& filename)
    {
        PRIVATE_CLASS_CREATE;
        PRIVATE_CLASS_GET(p);

        p->_name = filename;
#if CN_PLATFORM == CN_PLATFORM_LINUX
        // dlopen() does not add .so to the filename, like windows does for .dll
        if (p->_name.size() <= 3 || p->_name.substr(p->_name.size() - 3, 3) != _T(".so"))
            p->_name += _T(".so");
#elif CN_PLATFORM == CN_PLATFORM_MACOSX
        // dlopen() does not add .dylib to the filename, like windows does for .dll
        if (p->_name.size() <= 6 || p->_name.substr(p->_name.size() - 6, 6) != _T(".dylib"))
            p->_name += ".dylib";
#elif CN_PLATFORM == CN_PLATFORM_WINDOWS
        // Although LoadLibraryEx will add .dll itself when you only specify the library name,
        // if you include a relative path then it does not. So, add it to be sure.
        if (p->_name.size() <= 4 || p->_name.substr(p->_name.size() - 4, 4) != _T(".dll"))
            p->_name += _T(".dll");
#endif

        p->_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( p->_name.c_str() );
    }
    
    //-----------------------------------------------
    DynLib::~DynLib()
    {
        PRIVATE_CLASS_GET(p);
        if (p->_hInst)
        {
            DYNLIB_UNLOAD(p->_hInst);
        }

        PRIVATE_CLASS_DESTROY;
    }
    
    //-----------------------------------------------
    void* DynLib::getSymbol( const String& symbol ) const
    {
        PRIVATE_CLASS_GET_CONST(p);
        if (!p->_hInst) return NULL;

#if defined(_UNICODE) || defined(UNICODE)
        cString temp;
        return (void*)DYNLIB_GETSYM( p->_hInst, StringUtils::StrToStr(symbol, temp) );
#else
        return (void*)DYNLIB_GETSYM( p->_hInst, symbol.c_str() );
#endif
    }

    //-----------------------------------------------
    const String& DynLib::getName() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return p->_name;
    }

    //-----------------------------------------------
    bool DynLib::isLoaded() const
    {
        PRIVATE_CLASS_GET_CONST(p);
        return NULL != p->_hInst;
    }

} // end namespace hyc

#endif // !defined(CN_PLATFORM_MOBILE)
