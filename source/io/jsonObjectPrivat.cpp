
#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_JSON

#include "../../include/io/jsonObject.h"
#include "../../include/io/jsonValue.h"
#include "jsonObjectPrivat.hpp"
#include "jsonValuePrivat.hpp"

namespace hyc
{
    JsonObjectPrivat::JsonObjectPrivat()
    {
        m_pBeginValue = m_pEndValue = NULL;
        m_pNextObject = NULL;
    }

    JsonObjectPrivat::~JsonObjectPrivat()
    {
        if (NULL != m_pNextObject)
        {
            delete m_pNextObject;
        }

        if (NULL != m_pBeginValue)
        {
            delete m_pBeginValue;
        }
    }

    JsonValue* JsonObjectPrivat::GetFirstValue( const tchar* strName /*= NULL*/ )
    {
        if (NULL == m_pBeginValue)
        {
            return NULL;
        }
        if (NULL == strName || m_pBeginValue->m_pPrivate->m_strName == strName)
        {
            return m_pBeginValue;
        }
        return m_pBeginValue->GetNextValue(strName);
    }

    JsonValue* JsonObjectPrivat::AddValue( const tchar* strName, f32 nNum )
    {
        JsonValue* pValue = new JsonValue;
        _InsertValue(pValue);
        pValue->SetName(strName);
        pValue->SetValue(nNum);
        return pValue;
    }

    JsonValue* JsonObjectPrivat::AddValue( const tchar* strName, const tchar* strValue )
    {
        JsonValue* pValue = new JsonValue;
        _InsertValue(pValue);
        pValue->SetName(strName);
        pValue->SetValue(strValue);
        return pValue;
    }

    JsonValue* JsonObjectPrivat::AddValue( const tchar* strName, bool bBool )
    {
        JsonValue* pValue = new JsonValue;
        _InsertValue(pValue);
        pValue->SetName(strName);
        pValue->SetValue(bBool);
        return pValue;
    }

    JsonValue* JsonObjectPrivat::AddValue( const tchar* strName, JsonObject* pObject )
    {
        JsonValue* pValue = new JsonValue;
        _InsertValue(pValue);
        pValue->SetName(strName);
        pValue->SetValue(pObject);
        return pValue;
    }

    JsonValue* JsonObjectPrivat::AddValue( const tchar* strName )
    {
        JsonValue* pValue = new JsonValue;
        _InsertValue(pValue);
        pValue->SetName(strName);
        return pValue;
    }

    //-----------------------------------------------
    JsonValue* JsonObjectPrivat::AddValue( const tchar* strName, s64 nNum )
    {
        JsonValue* pValue = new JsonValue;
        _InsertValue(pValue);
        pValue->SetName(strName);
        pValue->SetValue(nNum);
        return pValue;
    }

    JsonValue* JsonObjectPrivat::AddArrayValue( const tchar* strName )
    {
        JsonValue* pValue = new JsonValue;
        _InsertValue(pValue);
        pValue->SetName(strName);
        pValue->SetType(JsonValue::TYPE_Array);
        return pValue;
    }

    JsonValue* JsonObjectPrivat::AddNullValue( const tchar* strName )
    {
        JsonValue* pValue = new JsonValue;
        _InsertValue(pValue);
        pValue->SetName(strName);
        pValue->SetNullValue();
        return pValue;
    }
    
    void JsonObjectPrivat::_InsertValue( JsonValue* pValue )
    {        
        if (NULL == m_pBeginValue)
        {
            m_pBeginValue = m_pEndValue = pValue;
            return;
        }
        m_pEndValue->m_pPrivate->m_pNext = pValue;
        m_pEndValue = pValue;

    }

    void JsonObjectPrivat::SaveToString( String& str )
    {
        str += _T('{');
        JsonValue* pTemp = m_pBeginValue;

        while (NULL != pTemp)
        {
            pTemp->m_pPrivate->SaveToString(str);
            pTemp = pTemp->m_pPrivate->m_pNext;
            if (NULL != pTemp)
            {
                str += _T(',');
            }
            else
            {
                break;
            }
        }

        str += _T('}');
    }
}

#endif // HYC_COMPILE_MODULE_JSON
