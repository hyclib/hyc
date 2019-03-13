////////////////////////////////////////////////////////
// Copyright(c) 1986-2100, TT, All Rights Reserved
// Author:  Hu Tianzhi
// Created: 2010-12-27 10:37:06
// Describe: Json 解析库
////////////////////////////////////////////////////////

#ifdef HYC_COMPILE_MODULE_JSON

#ifndef	_Tian_20101227103706_H_
#define	_Tian_20101227103706_H_

namespace hyc
{
    class JsonObject;
    class JsonDocPrivat;
    class HYC_API JsonDoc
    {
    public:
        JsonDoc();
        virtual ~JsonDoc();
        
        /*---------------------------
        解析;
        解析stream时, buf需以0结尾
        ---------------------------*/
        virtual bool            ParseFile( const tchar* pszFile );
        virtual bool            ParseStream( const tchar* buf )         { return ParseStream(buf, (u32)StringUtils::GetLength(buf)); }
        virtual bool            ParseStream( const tchar* buf, u32 size );

        // 获得发生解析错误的索引位置
        virtual int             GetErrorIndex();

        virtual const tchar*    SaveToStream();
        virtual JsonObject*     GetRootObject();
        virtual JsonObject*     AddObject();

    public:
        JsonDocPrivat* m_pPrivate;
    };
}

#endif // _Tian_20101227103706_H_

#endif // HYC_COMPILE_MODULE_JSON
