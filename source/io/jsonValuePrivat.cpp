#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_JSON

#include "../../include/io/jsonValue.h"
#include "../../include/io/jsonObject.h"
#include "../../include/utils/stringUtils.h"
#include "jsonValuePrivat.hpp"
#include "jsonObjectPrivat.hpp"

namespace hyc
{
    JsonValuePrivat::JsonValuePrivat()
        : m_pValue(NULL)
        , m_pNext(NULL)
    {
        m_number_s64 = 0;
        m_number_f32 = 0;
        m_eType = JsonValue::TYPE_Number;
    }

    JsonValuePrivat::~JsonValuePrivat()
    {
        //std::cout<<_T("delete:")<<m_strName.c_str()<<std::endl;
        _ReleaseValue();
        if (NULL != m_pNext)
        {
            delete m_pNext;
        }
    }
    
    void JsonValuePrivat::SetName( const tchar* strName )
    {
        m_strName = strName;
    }

    const tchar* JsonValuePrivat::GetName() const
    {
        return m_strName.c_str();
    }

    void JsonValuePrivat::_ReleaseValue()
    {
        if (NULL == m_pValue)
        {
            return;
        }
        switch(m_eType)
        {
        case JsonValue::TYPE_Array:
            {
            	Array<JsonValue*>& stlTemp = *((Array<JsonValue*>*)m_pValue);
            	Array<JsonValue*>::iterator it = stlTemp.begin();
                while (it != stlTemp.end())
                {
                    delete (*it);
                    it++;
                }
                delete &stlTemp;
                break;
            }
        case JsonValue::TYPE_String:
            {
// 
//                 String msg;
//                 msg.reserve(100);
//                 msg.format( "del %d\r\n", (s32)((String*)m_pValue) );
//                 Log_InfoHandle(msg.c_str());
// 
                delete (String*)(m_pValue);
                break;
            }
        case JsonValue::TYPE_Object:
            {
                delete (JsonObject*)(m_pValue);
                break;
            }
        case JsonValue::TYPE_Boolean:
        case JsonValue::TYPE_NULL:
        case JsonValue::TYPE_Number:
            {
                break;
            }
        }
        m_pValue = NULL;
        m_eType = JsonValue::TYPE_Number;
    }


    void JsonValuePrivat::_CreateValue()
    {
        switch(m_eType)
        {
        case JsonValue::TYPE_Array:
            {
                m_pValue = new Array<JsonValue*>;
                break;
            }
        case JsonValue::TYPE_String:
            {
                m_pValue = new String;
// 
//                 String msg;
//                 msg.reserve( 100 );
//                 msg.format( "create %d \r\n", (s32)m_pValue );
//                 Log_InfoHandle(msg.c_str());

                break;
            }
        case JsonValue::TYPE_Object:
        case JsonValue::TYPE_Boolean:
        case JsonValue::TYPE_NULL:
        case JsonValue::TYPE_Number:
            {
                break;
            }
        default:
            assert(0);  // Type error
            return;
        }
    }


    f32 JsonValuePrivat::GetNumber_float(f32          defaultValue)const
    {
        if (m_eType != JsonValue::TYPE_Number)
            return defaultValue;

        return m_number_f32;
    }

    //-----------------------------------------------
    hyc::s64 JsonValuePrivat::GetNumber_int( s64 defaultValue ) const
    {
        if (m_eType != JsonValue::TYPE_Number)
            return defaultValue;

        return m_number_s64;
    }
    const tchar* JsonValuePrivat::GetString(const tchar* defaultValue)const
    {
        if (m_eType != JsonValue::TYPE_String)
            return defaultValue;
// 
// 
//         String msg;
//         msg.reserve(100);
//         msg.format( "get %d\r\n", (s32)((String*)m_pValue) );
//         Log_InfoHandle(msg.c_str());

        return ((String*)m_pValue)->c_str();
    }

    bool JsonValuePrivat::GetBoolean(bool        defaultValue) const
    {
        if (m_eType != JsonValue::TYPE_Boolean)
            return defaultValue;

        return m_number_s64 != 0;
    }

    JsonObject* JsonValuePrivat::GetObject()const
    {
        assert(m_eType == JsonValue::TYPE_Object);
        return (JsonObject*)m_pValue;
    }

    void JsonValuePrivat::SetValue( f32 nNum )
    {
        if (m_eType != JsonValue::TYPE_Number)
        {
            _ReleaseValue();
            m_eType = JsonValue::TYPE_Number;
        }

        m_number_f32 = nNum;
        m_number_s64 = (s64)nNum;
        m_numberType = NumberType_f32;
    }

    //-----------------------------------------------
    void JsonValuePrivat::SetValue( s64 nNum )
    {
        if (m_eType != JsonValue::TYPE_Number)
        {
            _ReleaseValue();
            m_eType = JsonValue::TYPE_Number;
        }

        m_number_s64 = nNum;
        m_number_f32 = (f32)nNum;
        m_numberType = NumberType_s64;
    }

    void JsonValuePrivat::SetValue( const tchar* strValue )
    {
        SetType( JsonValue::TYPE_String );

        String& strTemp = *((String*)m_pValue);
// 
//         String msg;
//         msg.reserve(100);
//         msg.format( "set %d\r\n", (s32)((String*)m_pValue) );
//         Log_InfoHandle(msg.c_str());

//         u32 l = StringUtils::GetLength(strValue);
//         strTemp.clear();
//         strTemp.reserve(l);
// 
//         for (u32 i = 0; i < l; i++)
//         {
//             if (strValue[i] == _T('\\') || strValue[i] == _T('"'))
//             {
//                 strTemp += _T('\\');
//             }
//             strTemp += strValue[i];
//         }


       strTemp = strValue;

// 
//         if (m_eType != JsonValue::TYPE_String)
//         {
//             _ReleaseValue();
//             m_eType = JsonValue::TYPE_String;
//             _CreateValue();
//         }
//         String* pStr =  (String*)m_pValue;

    }

    void JsonValuePrivat::SetValue( JsonObject* pObject )
    {
        if (m_eType != JsonValue::TYPE_Object)
        {
            _ReleaseValue();
        }
		m_eType = JsonValue::TYPE_Object;
        m_pValue = pObject;
    }

    void JsonValuePrivat::SetValue( bool bValue )
    {
        if (m_eType != JsonValue::TYPE_Boolean)
        {
            _ReleaseValue();
        }
        m_eType = JsonValue::TYPE_Boolean;
        m_number_s64 = bValue?1:0;
    }



    void JsonValuePrivat::SetNullValue()
    {
        if (m_eType != JsonValue::TYPE_NULL)
        {
            _ReleaseValue();
            m_eType = JsonValue::TYPE_NULL;
        }
    }

    JsonValue* JsonValuePrivat::FindNext( const tchar* strName )
    {
        if (NULL == strName)
        {
            return m_pNext;
        }
        JsonValue* pTemp = m_pNext;
        while (NULL != pTemp)
        {
            if (pTemp->m_pPrivate->m_strName == strName)
            {
                return pTemp;
            }
            pTemp = pTemp->m_pPrivate->m_pNext;
        }
        return NULL;
    }

    void JsonValuePrivat::PushValue( JsonValue* pValue )
    {
        assert(m_eType == JsonValue::TYPE_Array);
        Array<JsonValue*>& stlTemp = *((Array<JsonValue*>*)m_pValue);
        stlTemp.push_back(pValue);
    }

    JsonValue* JsonValuePrivat::GetValue( u32 nIndex ) const
    {
        assert(m_eType == JsonValue::TYPE_Array);
        Array<JsonValue*>& stlTemp = *((Array<JsonValue*>*)m_pValue);
        return stlTemp[nIndex];
    }

    void JsonValuePrivat::SetType( s32 eType )
    {
        if (m_eType != eType)
        {
            _ReleaseValue();
            m_eType = eType;
            _CreateValue();
        }
    }

    u32 JsonValuePrivat::GetArraySize() const
    {
        assert(m_eType == JsonValue::TYPE_Array);
        Array<JsonValue*>& stlTemp = *((Array<JsonValue*>*)m_pValue);
        return (u32)stlTemp.size();
    }
    
    void JsonValuePrivat::ClearArray()
    {
        assert(m_eType == JsonValue::TYPE_Array);
        Array<JsonValue*>& stlTemp = *((Array<JsonValue*>*)m_pValue);
        return stlTemp.clear();
    }

    void JsonValuePrivat::SaveToString( String& str , bool bWriteName/* = true*/)
    {
        if (bWriteName)
        {
            str += _T('\"');
            str += m_strName;
            str += _T("\":");
        }
        switch(m_eType)
        {
        case JsonValue::TYPE_Array:
            {
                str += _T('[');
                Array<JsonValue*>& stlTemp = *((Array<JsonValue*>*)m_pValue);
                Array<JsonValue*>::iterator it = stlTemp.begin();
                while (it != stlTemp.end())
                {
                    (*it)->m_pPrivate->SaveToString(str, false);
                    it++;
                    if (it != stlTemp.end())
                    {
                        str += _T(',');
                    }
                    else
                    {
                        break;
                    }
                }
                str += _T(']');
                break;
            }
        case JsonValue::TYPE_String:
            {

                String& strValue = *((String*)m_pValue);
                str.reserve(str.size()+strValue.size());
                str += _T('\"');
                
                for (u32 i = 0; i < strValue.size(); i++)
                {
                    if (strValue[i] == _T('\\') || strValue[i] == _T('"'))
                    {
                        str += _T('\\');
                    }
                    str += strValue[i];
                }

                str += _T('\"');
                break;
            }
        case JsonValue::TYPE_Object:
            {
                ((JsonObject*)m_pValue)->m_pPrivate->SaveToString(str);
                break;
            }
        case JsonValue::TYPE_Boolean:
            {
                if (m_number_s64 == 0)
                    str += _T("false");
                else
                    str += _T("true");
                break;
            }
        case JsonValue::TYPE_NULL:
            {
                str += _T("null");
                break;
            }
        case JsonValue::TYPE_Number:
            {
                String sNum;

                switch (m_numberType)
                {
                case NumberType_f32:
                    if ((f32)(s32)m_number_f32 == m_number_f32)
                        StringUtils::S32ToStr((s32)m_number_f32, sNum);
                    else
                        StringUtils::F32ToStr(m_number_f32, sNum);
                    break;
                case NumberType_s64:
                    StringUtils::S64ToStr(m_number_s64, sNum);
                    break;
                default:
                    error_all(_T("json number type error"));
                    sNum = _T("0");
                    break;
                }

                str += sNum;
                break;
            }
        }
    }


}
#endif // HYC_COMPILE_MODULE_JSON
