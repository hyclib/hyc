//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// common
// author:       lipengxiang
// description:  单例模式模板; 使用单例类前, 需创建一个唯一对象, 在使用完后需释放
//

#ifndef __BP_CNSINGLETON_H_20110308115948__
#define __BP_CNSINGLETON_H_20110308115948__

//----------------------------------------------------------------------------------------------------
//*     单例模式模板
//*     继承此类模板即可实现单例模式
//----------------------------------------------------------------------------------------------------

template<class T>
class Singleton
{
public: // method
    Singleton()
    {
        if ( msSingleton )
            throw ExceptionType_Forbid;
        msSingleton = static_cast<T*>(this);
    }

    virtual ~Singleton()
    {
        msSingleton = NULL;
    }

    static T& getSingleton()
    {
        return ( *msSingleton );
    }

    static T* getSingletonPtr()
    {
        return msSingleton;
    }

protected:  // variable member
    static T* msSingleton;
};

//-----------------------------------------------
// 在类实现中使用以下宏, 将完成Singleton的接口实现
#define IMPLEMENT_INTERFACE_SINGLETON(runtimeClassName) template<> runtimeClassName* ::Singleton<runtimeClassName>::msSingleton = NULL; // add template<> for gcc4


#endif // end of define __BP_CNSINGLETON_H_20110308115948__
