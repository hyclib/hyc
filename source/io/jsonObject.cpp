#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_JSON

#include "../../include/io/jsonObject.h"
#include "../../include/io/jsonValue.h"
#include "jsonObjectPrivat.hpp"
#include "jsonValuePrivat.hpp"

namespace hyc
{
    JsonObject::JsonObject()
    {
        m_pPrivate = new JsonObjectPrivat;
    }

    JsonObject::~JsonObject()
    {
        delete m_pPrivate;
    }

    JsonValue* JsonObject::GetFirstValue( const tchar* strName /*= NULL*/ )
    {
        return m_pPrivate->GetFirstValue(strName);
    }   

    JsonValue* JsonObject::AddValue( const tchar* strName, f32 nNum )
    {
        return m_pPrivate->AddValue(strName, nNum);
    }

    JsonValue* JsonObject::AddValue( const tchar* strName, const tchar* strValue )
    {
        return m_pPrivate->AddValue(strName, strValue);
    }

    JsonValue* JsonObject::AddValue( const tchar* strName, bool bBool )
    {
        return m_pPrivate->AddValue(strName, bBool);
    }

    JsonValue* JsonObject::AddValue( const tchar* strName )
    {
        return m_pPrivate->AddValue(strName);
    }

    JsonValue* JsonObject::AddValue( const tchar* strName, JsonObject* pObject )
    {
        return m_pPrivate->AddValue(strName, pObject);
    }

    //-----------------------------------------------
    JsonValue* JsonObject::AddValue( const tchar* strName, s64 nNumber )
    {
        return m_pPrivate->AddValue(strName, nNumber);
    }

    JsonValue* JsonObject::AddArrayValue( const tchar* strName )
    {
        return m_pPrivate->AddArrayValue(strName); 
    }

    JsonValue* JsonObject::AddNullValue( const tchar* strName )
    {
        JsonValue* pValue = m_pPrivate->AddValue(strName);
        pValue->m_pPrivate->SetNullValue();
        return pValue;
    }

    JsonObject* JsonObject::GetNextObject()
    {
        return m_pPrivate->m_pNextObject;
    }
}

#endif // HYC_COMPILE_MODULE_JSON
