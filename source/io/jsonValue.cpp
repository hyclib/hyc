#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_JSON

#include "../../include/io/jsonValue.h"
#include "jsonValuePrivat.hpp"

namespace hyc
{
    JsonValue::JsonValue()
    {
        m_pPrivate = new JsonValuePrivat;
    }

    JsonValue::~JsonValue()
    {        
        delete m_pPrivate;
    }
    
    JsonValue::TYPE JsonValue::GetType()const
    {
        return (JsonValue::TYPE)m_pPrivate->m_eType;
    }


    void JsonValue::SetType( JsonValue::TYPE eType )
    {
        m_pPrivate->SetType(eType);
    }


    const tchar* JsonValue::GetName()const
    {
        return m_pPrivate->GetName();
    }  

    void JsonValue::SetName( const tchar* strName )
    {
        m_pPrivate->SetName(strName);
    }

    f32 JsonValue::GetNumber_float( f32          defaultValue) const
    {
        return m_pPrivate->GetNumber_float(defaultValue);
    }

    //-----------------------------------------------
    hyc::s64 JsonValue::GetNumber_int( s64 defaultValue /*= 0 */ ) const
    {
        return m_pPrivate->GetNumber_int(defaultValue);
    }

    const tchar* JsonValue::GetString(const tchar* defaultValue) const
    {
        return m_pPrivate->GetString(defaultValue);
    }

    bool JsonValue::GetBoolean(bool        defaultValue) const
    {
        return m_pPrivate->GetBoolean(defaultValue);
    }
    JsonObject* JsonValue::GetObject() const
    {
        return m_pPrivate->GetObject();
    }


    void JsonValue::SetValue( f32 nNum )
    {
        m_pPrivate->SetValue(nNum);
    }

    //-----------------------------------------------
    void JsonValue::SetValue( s64 nNumber )
    {
        m_pPrivate->SetValue(nNumber);
    }

    void JsonValue::SetValue( const tchar* strValue )
    {
        m_pPrivate->SetValue(strValue);
    }

    void JsonValue::SetValue( JsonObject* pObject )
    {
        m_pPrivate->SetValue(pObject);
    }

    void JsonValue::SetValue( bool bValue )
    {
        m_pPrivate->SetValue(bValue);
    }


    void JsonValue::SetNullValue()
    {
        m_pPrivate->SetNullValue();
    }

    JsonValue* JsonValue::GetNextValue(const tchar* strName)
    {
        return m_pPrivate->FindNext(strName);
    }

    JsonValue* JsonValue::GetValue( u32 nIndex ) const
    {
        return m_pPrivate->GetValue(nIndex);
    }

    u32 JsonValue::GetArraySize() const
    {
        return m_pPrivate->GetArraySize();
    }

    void JsonValue::PushValue( JsonValue* pValue )
    {
        m_pPrivate->PushValue(pValue);
    }

    //-----------------------------------------------
    void JsonValue::PushValue( f32 number )
    {
        JsonValue* pValue = new JsonValue;
        pValue->SetValue(number);
        PushValue(pValue);
    }

    //-----------------------------------------------
    void JsonValue::PushValue( s64 number )
    {
        JsonValue* pValue = new JsonValue;
        pValue->SetValue(number);
        PushValue(pValue);
    }
    //-----------------------------------------------
    void JsonValue::PushValue( const tchar* strValue )
    {
        JsonValue* pValue = new JsonValue;
        pValue->SetValue(strValue);
        PushValue(pValue);
    }

    //-----------------------------------------------
    void JsonValue::PushValue( bool bValue )
    {
        JsonValue* pValue = new JsonValue;
        pValue->SetValue(bValue);
        PushValue(pValue);
    }
    
    //-----------------------------------------------
    void JsonValue::ClearArray()
    {
        m_pPrivate->ClearArray();
    }



}

#endif // HYC_COMPILE_MODULE_JSON
