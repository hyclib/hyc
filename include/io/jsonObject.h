////////////////////////////////////////////////////////
// Copyright(c) 1986-2100, TT, All Rights Reserved
// Author:  Hu Tianzhi
// Created: 2010-12-27 10:45:42
// Describe:
////////////////////////////////////////////////////////

#ifdef HYC_COMPILE_MODULE_JSON

#ifndef	_Tian_20101227104542_H_
#define	_Tian_20101227104542_H_

namespace hyc
{
    class JsonValue;
    class JsonObjectPrivat;
    class HYC_API JsonObject
    {
    public:
        JsonObject();
        virtual ~JsonObject();
    public:
        // Return the value which the name is strName
        virtual JsonValue*  GetFirstValue(const tchar* strName = NULL);
        virtual JsonObject* GetNextObject();

        virtual JsonValue*  AddValue(const tchar* strName, f32 nNumber);
        virtual JsonValue*  AddValue(const tchar* strName, s64 nNumber);
        virtual JsonValue*  AddValue(const tchar* strName, const tchar* strValue);
        virtual JsonValue*  AddValue(const tchar* strName, bool bBool);
        virtual JsonValue*  AddValue(const tchar* strName, JsonObject* pObject);
        virtual JsonValue*  AddValue(const tchar* strName);
        virtual JsonValue*  AddArrayValue(const tchar* strName);
        virtual JsonValue*  AddNullValue(const tchar* strName);

    public:
        JsonObjectPrivat*   m_pPrivate;
    };
}

#endif // _Tian_20101227104542_H_

#endif // HYC_COMPILE_MODULE_JSON
