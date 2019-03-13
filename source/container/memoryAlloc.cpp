//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#include "../../include/container/memoryAlloc.h"

#ifdef CN_PLATFORM_MOBILE
#undef HYC_COMPILE_MODULE_MALLOC_EXT
#else
#include "../../ext/nedmalloc/nedmalloc.hpp"
#endif

#include "../globalVar.hpp"

namespace hyc
{
#ifdef _DEBUG
    //-----------------------------------------------
    void* MemoryAlloc::Malloc( uinteger size, const char* lpszFileName, int nLine )
    {
        void* p = Malloc(size);
        if (g_pDebugMap)
        {
            std::map<void*, DebugInfo>::iterator itr = g_pDebugMap->find(p);
            assert(itr == g_pDebugMap->end());
            g_pDebugMap->insert(std::make_pair(p, DebugInfo(lpszFileName, nLine, (int)size)));
        }
        return p;
    }
#endif // _DEBUG

    //-----------------------------------------------
    void* MemoryAlloc::Malloc( uinteger size )
    {
        return

#if defined(HYC_COMPILE_MODULE_MALLOC_EXT)
        nedalloc::nedmalloc( size );
#else
        malloc(size);
#endif
    }


    //-----------------------------------------------
    void* MemoryAlloc::Realloc( void* ptr, uinteger newSize )
    {
#ifdef _DEBUG
        void* k = ptr;
#endif // _DEBUG

#if defined(HYC_COMPILE_MODULE_MALLOC_EXT)
        if (newSize == 0)
        {
            Free(ptr);
            return NULL;
        }

#ifdef _DEBUG
        void* p =
#else
        return 
#endif
            nedalloc::nedrealloc( ptr, newSize );

#else


#ifdef DEBUG
        void* p =
#else
        return 
#endif
            realloc(ptr, newSize);

#endif

#ifdef _DEBUG
        if (g_pDebugMap)
        {
            std::map<void*, DebugInfo>::iterator itr = g_pDebugMap->find(k);
            if (itr != g_pDebugMap->end())
            {
                DebugInfo info(itr->second.filename.c_str(), itr->second.nLine, (u32)newSize);
                g_pDebugMap->erase(itr);
                g_pDebugMap->insert(std::make_pair(p, info));
            }        
        }
        return p;
#endif
    }

    //-----------------------------------------------
    void MemoryAlloc::Free( void* ptr )
    {
        if (ptr)
        {

            // operator delete( ptr );
#if defined(HYC_COMPILE_MODULE_MALLOC_EXT)
            nedalloc::nedfree( ptr );
#else
            free(ptr);
#endif

#ifdef _DEBUG
            if (g_pDebugMap)
            {
                std::map<void*, DebugInfo>::iterator itr = g_pDebugMap->find(ptr);
                if (itr != g_pDebugMap->end())
                {
                    g_pDebugMap->erase(itr);
                }
            }
#endif

        }
    }

    //-----------------------------------------------
    void MemoryAlloc::MemMove( void* pDest, const void* pSrc, uinteger size )
    {
        if (pDest && pSrc)
        {
            memmove( pDest, pSrc, size );
        }
    }

} // end namespace hyc
