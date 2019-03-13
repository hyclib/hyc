////////////////////////////////////////////////////////
// Copyright(c) 1986-2100, TT, All Rights Reserved
// Author:  Hu Tianzhi
// Created: 2010-12-27 14:54:01
// Describe:
////////////////////////////////////////////////////////

#ifdef HYC_COMPILE_MODULE_JSON

#ifndef	_Tian_20101227145401_H_
#define	_Tian_20101227145401_H_


namespace hyc
{
    class JsonObject;
    class JsonValue;

    class JsonValuePrivat
    {
    public:
        JsonValuePrivat();
        ~JsonValuePrivat();

        void            SetName(const tchar* strName);
        const tchar*    GetName()const;

        f32             GetNumber_float(f32          defaultValue)const;
        s64             GetNumber_int(s64          defaultValue)const;
        const tchar*    GetString(const tchar* defaultValue)const;
        bool            GetBoolean(bool        defaultValue)const;
        JsonObject*     GetObject()const;

        void            SetValue(f32 nNum);
        void            SetValue(s64 nNum);
        void            SetValue(const tchar* strValue);
        void            SetValue(JsonObject* pObject);
        void            SetValue(bool bValue);
        void            SetNullValue();


        JsonValue*      FindNext(const tchar* strName);

        void            PushValue(JsonValue* pValue);
        JsonValue*      GetValue(u32 nIndex)const;
        u32             GetArraySize()const;
        void            ClearArray();
        
        void            SetType(s32 eType );
      

        void            SaveToString(String& str, bool bWriteName = true);
    protected:
        void        _ReleaseValue();
        void        _CreateValue();

    public:
        integer         m_eType;
        void*       m_pValue;
        String      m_strName;
        JsonValue*  m_pNext;

        f32 m_number_f32;
        s64 m_number_s64;

        enum NumberType {
            NumberType_none = -1,
            NumberType_f32 = 0,
            NumberType_s64 = 1,
        };
        u8          m_numberType;
    };
}

#endif // _Tian_20101227145401_H_

#endif // HYC_COMPILE_MODULE_JSON
