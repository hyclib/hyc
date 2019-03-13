////////////////////////////////////////////////////////
// Copyright(c) 1986-2100, TT, All Rights Reserved
// Author:  Hu Tianzhi
// Created: 2010-12-27 11:35:38
// Describe:
////////////////////////////////////////////////////////

#ifdef HYC_COMPILE_MODULE_JSON

#ifndef	_Tian_20101227113538_H_
#define	_Tian_20101227113538_H_

#include "jsonValuePrivat.hpp"

namespace hyc
{
    class JsonValue;
    class JsonObject;
    class JsonDocPrivat
    {
    public:
        JsonDocPrivat();
        ~JsonDocPrivat();
    public:
        bool    Parse(const tchar* buf, u32 size);
        const tchar*    SaveToStream();
        JsonObject*     AddObject();

    public:
        JsonObject* m_pRoot;
        JsonObject* m_pEnd;

        int         m_nCurIndex;
        tchar*      m_pParseBuf;

        String      m_strJson;
        s32         m_strSize;

    protected:
        bool    _ObjectParse(JsonObject* pObject);
        bool    _ValuePairParse(JsonObject* pObject);   // 解析值对
        bool    _ValueParse(JsonValue* pValue);         // 解析:(冒号后的值)
        bool    _ArrayParse(JsonValue* pValue);

        // 解析 "" 对, 成功返回""中的数据，失败返回NULL
        const tchar*    _ParseEndChar(tchar endChar, String& strName);

        JsonValuePrivat::NumberType    _ParseEndNum(f32& nNum, s64& nNum2);

        // 忽略字符串
        void    _IgnoreChar();
    };
}

#endif // _Tian_20101227113538_H_

#endif // HYC_COMPILE_MODULE_JSON
