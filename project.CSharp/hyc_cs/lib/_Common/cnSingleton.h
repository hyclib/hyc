//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyc
// author:       lipengxiang
// description:  
//

#pragma once
#include "hyc/include/hyc.h"
#include "cnInterface.h"

namespace Hy
{
    //----------------------------------------------------------------------------------------------------
    //*     单例模式模板
    //*     继承此类模板即可实现单例模式
    //----------------------------------------------------------------------------------------------------
	generic <class T>
    public ref class Singleton
    {
    public: // method
        static Singleton()
        {
            msCreated = false;
            msSingleton = T();
        }
        Singleton()
        {
            if ( true == msCreated )
            {
                error_all(_T("Cli Error: Singleton is already exist!"));
                throw 1; // 1 hint error
                return;
            }
            msSingleton = (T)(this);
        }
        ~Singleton()   { this->!Singleton();}
        !Singleton()   { msCreated = false; msSingleton = T(); }

        static T getSingletonPtr()
        {
            return msSingleton;
        }

    protected:  // variable member
        static T msSingleton;
        static bool msCreated;
    };

} // end namespace Hy
