////////////////////////////////////////////////////////
// Copyright(c) 1986-2100, TT, All Rights Reserved
// Author:  Hu Tianzhi
// Created: 2010-12-27 11:00:05
// Describe:
////////////////////////////////////////////////////////

#ifdef HYC_COMPILE_MODULE_JSON

#ifndef	_Tian_20101227110005_H_
#define	_Tian_20101227110005_H_

namespace hyc
{
    class JsonObject;
    class JsonValuePrivat;
    class HYC_API JsonValue
    {
    public:
        enum TYPE
        {
            TYPE_String,
            TYPE_Number,    // 数字有float/s64两种表现方式.
            TYPE_Object,
            TYPE_Array,
            TYPE_Boolean,
            TYPE_NULL,
        };
    public:
        JsonValue();
        virtual ~JsonValue();
    public:
        virtual JsonValue::TYPE  GetType()const;
        virtual void            SetType(JsonValue::TYPE eType);

        virtual const tchar*    GetName()const;
        virtual void            SetName(const tchar* strName);

        // Return default value if error type.
        virtual f32             GetNumber_float( f32      defaultValue = 0 )const;
        virtual s64             GetNumber_int( s64      defaultValue = 0 )const;
        virtual const tchar*    GetString( const tchar* defaultValue = NULL )const;
        virtual bool            GetBoolean( bool        defaultValue = false )const;
        virtual JsonObject*     GetObject()const;

        virtual void            SetValue(f32 nNumber);
        virtual void            SetValue(s64 nNumber);
        virtual void            SetValue(const tchar* strValue);
        virtual void            SetValue(JsonObject* pObject);
        virtual void            SetValue(bool bValue);
        virtual void            SetNullValue();

        // Return value only the type is TYPE_Array
        virtual JsonValue*      GetValue(u32 nIndex)const;
        virtual u32             GetArraySize()const;
        virtual void            PushValue(JsonValue* pValue);   // pValue由此对象释放.
        virtual void            PushValue(f32 number);
        virtual void            PushValue(s64 number);
        virtual void            PushValue(const tchar* strValue);
        virtual void            PushValue(bool bValue);
        virtual void            ClearArray();


        virtual JsonValue*      GetNextValue(const tchar* strName);

    public:
        JsonValuePrivat*   m_pPrivate;
    };
}
#endif // _Tian_20101227110005_H_

#endif // HYC_COMPILE_MODULE_JSON
