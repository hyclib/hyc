#include "../preheader.h"


#ifdef HYC_COMPILE_MODULE_JSON

#include "../../include/io/jsonObject.h"
#include "../../include/io/jsonValue.h"

#include "../../include/utils/stringUtils.h"
#include "../../include/utils/encodingUtils.h"

#include "jsonDocPrivat.hpp"
#include "jsonObjectPrivat.hpp"

namespace hyc
{
    JsonDocPrivat::JsonDocPrivat()
    {
        m_pRoot = NULL;
        m_pEnd = NULL;
        m_strSize = 0;
    }

    JsonDocPrivat::~JsonDocPrivat()
    {
        if(NULL != m_pRoot)
        {
            delete m_pRoot;
        }
    }

    bool JsonDocPrivat::Parse( const tchar* buf, u32 size )
    {
        if(NULL != m_pRoot)
        {
            delete m_pRoot;
        }
        m_pRoot = new JsonObject;
        m_pEnd = NULL;

        m_pParseBuf = (tchar*)buf;
        m_nCurIndex = 0;
        m_strSize = size;

        do 
        {
            _IgnoreChar();
            if (m_strSize <= m_nCurIndex)
                break;

            if (_T('\0') == m_pParseBuf[m_nCurIndex])
                return false;

            if (m_pParseBuf[m_nCurIndex] != _T('{'))
            {
                return false;
            }
            m_nCurIndex++;

            if (m_pEnd == NULL)
            {
                m_pEnd = m_pRoot;
            }
            else
            {
                JsonObject* pTemp = new JsonObject;
                m_pEnd->m_pPrivate->m_pNextObject = pTemp;
                if (m_pEnd == m_pRoot)
                {
                    m_pRoot->m_pPrivate->m_pNextObject = pTemp;
                }
                m_pEnd = pTemp;
            }

            if(!_ObjectParse(m_pEnd))
            {
                return false;
            }
        } while (true);
        
        

//         while (m_strSize > m_nCurIndex)
//         {
//             if (_T('\0') == m_pParseBuf[m_nCurIndex])
//                 return false;
// 
//             if (m_pParseBuf[m_nCurIndex] != _T('{'))
//             {
//                 return false;
//             }
//             m_nCurIndex++;
// 
//             if (m_pEnd == NULL)
//             {
//                 m_pEnd = m_pRoot;
//             }
//             else
//             {
//                 JsonObject* pTemp = new JsonObject;
//                 m_pEnd->m_pPrivate->m_pNextObject = pTemp;
//                 if (m_pEnd == m_pRoot)
//                 {
//                     m_pRoot->m_pPrivate->m_pNextObject = pTemp;
//                 }
//                 m_pEnd = pTemp;
//             }
// 
//             if(!_ObjectParse(m_pEnd))
//             {
//                 return false;
//             }
//         }

        return true;
    }

    bool JsonDocPrivat::_ObjectParse( JsonObject* pObject )
    {
        while (m_pParseBuf[m_nCurIndex] != _T('}') && m_nCurIndex < m_strSize)
        {
            _IgnoreChar();
            if (m_pParseBuf[m_nCurIndex] == _T('\0'))
            {
                return false;
            }
            if(!_ValuePairParse(pObject))
            {
                return false;
            }

            _IgnoreChar();
            if (m_pParseBuf[m_nCurIndex] == _T(','))
            {
                m_nCurIndex++;
            }
        }

        if (m_nCurIndex >= m_strSize)
            return false;

        m_nCurIndex++;
        return true;
    }

    bool JsonDocPrivat::_ValuePairParse( JsonObject* pObject )
    {
        _IgnoreChar();
        if (m_pParseBuf[m_nCurIndex] != _T('"'))
        {
            return false;
        }
        m_nCurIndex++;

        // 处理名字
        String str;
        const tchar* pStrName = _ParseEndChar(_T('"'), str);
        if (NULL == pStrName)
        {
            return false;
        }

        _IgnoreChar();

        if (m_pParseBuf[m_nCurIndex] != _T(':'))
        {
            return false;
        }
        m_nCurIndex++;
        JsonValue* pValue = pObject->AddValue(pStrName);

        // 解析值.
        return _ValueParse(pValue);
    }

    bool JsonDocPrivat::_ValueParse( JsonValue* pValue )
    {
        String str;

        _IgnoreChar();
        switch(m_pParseBuf[m_nCurIndex])
        {
        case _T('{'):   // object
            {
                m_nCurIndex++;
                JsonObject* pObject = new JsonObject;
                pValue->SetValue(pObject);
                return _ObjectParse(pObject);
            }
        case _T('['):   // array
            {
                m_nCurIndex++;
                pValue->SetType(JsonValue::TYPE_Array);
                return _ArrayParse(pValue);
            }
        case _T('"'):   // string
            {
                m_nCurIndex++;
                const tchar* pStrValue = _ParseEndChar(_T('"'), str);
                //if (NULL == pStrValue)
                //{
                //    return false;
                //}
                pValue->SetValue(str.c_str());

                break;
            }
        case _T('t'):   // true
        case _T('T'):
            {
                if ((m_pParseBuf[m_nCurIndex + 1] != _T('r') && m_pParseBuf[m_nCurIndex + 1] != _T('R')) ||
                    (m_pParseBuf[m_nCurIndex + 2] != _T('u') && m_pParseBuf[m_nCurIndex + 2] != _T('U')) ||
                    (m_pParseBuf[m_nCurIndex + 3] != _T('e') && m_pParseBuf[m_nCurIndex + 3] != _T('E')))                    
                {
                    return false;
                }
                pValue->SetValue(true);
                m_nCurIndex += 4;
                break;
            }
        case _T('f'):   // false
        case _T('F'):
            {
                if ((m_pParseBuf[m_nCurIndex + 1] != _T('a') && m_pParseBuf[m_nCurIndex + 1] != _T('A')) ||
                    (m_pParseBuf[m_nCurIndex + 2] != _T('l') && m_pParseBuf[m_nCurIndex + 2] != _T('L')) ||
                    (m_pParseBuf[m_nCurIndex + 3] != _T('s') && m_pParseBuf[m_nCurIndex + 3] != _T('S')) ||
                    (m_pParseBuf[m_nCurIndex + 4] != _T('e') && m_pParseBuf[m_nCurIndex + 4] != _T('E')))
                {
                    return false;
                }
                pValue->SetValue(false);
                m_nCurIndex += 5;
                break;
            }
        case _T('n'):   // null
        case _T('N'):
            {
                if ((m_pParseBuf[m_nCurIndex + 1] != _T('u') && m_pParseBuf[m_nCurIndex + 1] != _T('U')) ||
                    (m_pParseBuf[m_nCurIndex + 2] != _T('l') && m_pParseBuf[m_nCurIndex + 2] != _T('L')) ||
                    (m_pParseBuf[m_nCurIndex + 3] != _T('l') && m_pParseBuf[m_nCurIndex + 3] != _T('L')))
                {
                    return false;
                }
                pValue->SetNullValue();
                m_nCurIndex += 4;
                break;
            }
        default:
            {
                f32 nNum;
                s64 nNum2;
                JsonValuePrivat::NumberType type = _ParseEndNum(nNum, nNum2);
                if (type == JsonValuePrivat::NumberType_none)
                    return false;
                else if (type == JsonValuePrivat::NumberType_f32)
                {
                    pValue->SetValue(nNum);
                }
                else
                {
                    pValue->SetValue(nNum2);
                }
            }
        }
        return true;
    }

    bool JsonDocPrivat::_ArrayParse( JsonValue* pValue )
    {
        while (m_pParseBuf[m_nCurIndex] != _T(']') && m_nCurIndex < m_strSize)
        {
            _IgnoreChar();
            if (m_pParseBuf[m_nCurIndex] == _T('\0'))
            {
                return false;
            }
            JsonValue* pTemp = new JsonValue;
            pTemp->SetName(_T("Array"));
            pValue->PushValue(pTemp);
            if(!_ValueParse(pTemp))
            {
                return false;
            }
            _IgnoreChar();
            if (m_pParseBuf[m_nCurIndex] == _T(','))
            {
                m_nCurIndex++;
            }

        }

        if (m_nCurIndex >= m_strSize)
            return false;

        m_nCurIndex++;
        return true;
    }

    const tchar* JsonDocPrivat::_ParseEndChar(tchar endChar, String& strName)
    {
        strName = _T("");

#if defined(_UNICODE) || defined(UNICODE)
#else
        cString ansiStr;
#endif // defined(_UNICODE) || defined(UNICODE)

        Byte buf[5] = {0};

        while (m_nCurIndex < m_strSize)
        {
            // 检查是否到字符串末尾
            if (m_pParseBuf[m_nCurIndex] == _T('\0'))
            {
                return NULL;
            }

            // 处理转义符
            if (m_pParseBuf[m_nCurIndex] == _T('\\'))
            {
                if (m_nCurIndex+1 >= m_strSize)
                {
                    m_nCurIndex = m_strSize;
                    return NULL;
                }
                switch( m_pParseBuf[m_nCurIndex+1] )
                {
                case '"':
                    strName += '"';
                    break;
                case '\\':
                    strName += '\\';
                    break;
                case '/':
                    strName += '/';
                    break;
                case 'b':
                    strName += '\b';
                    break;
                case 'f':
                    strName += '\f';
                    break;
                case 'n':
                    strName += '\n';
                    break;
                case 'r':
                    strName += '\r';
                    break;
                case 't':
                    strName += '\t';
                    break;
                case 'u':
                    memset( buf, 0, sizeof buf );
                    EncodingUtils::HexToByte( m_pParseBuf+m_nCurIndex+2, 4, buf, sizeof buf );

#if defined(_UNICODE) || defined(UNICODE)
                    wchar_t t = buf[1];
                    t |= ((wchar_t)buf[0]) << 8;
                    strName += t;
#else
                    wchar_t t = buf[1];
                    t |= ((wchar_t)buf[0]) << 8;
                    StringUtils::Wide2narrow( &t, 1, ansiStr );
                    strName += ansiStr;
#endif // defined(_UNICODE) || defined(UNICODE)

                    m_nCurIndex += 4;
                    break;
                }

                m_nCurIndex += 2;
                continue;
            }

            // 结束
            if (m_pParseBuf[m_nCurIndex] == endChar)
            {
                m_nCurIndex ++;
                break;
            }

            strName += m_pParseBuf[m_nCurIndex];
            m_nCurIndex++;
        }

        return strName.c_str();
    }


    JsonValuePrivat::NumberType JsonDocPrivat::_ParseEndNum(f32& nNum, s64& nNum2)
    {
        String strNum;
        strNum = _T("");
        while (m_nCurIndex < m_strSize)
        {
            // 检查是否到字符串末尾
            if (m_pParseBuf[m_nCurIndex] == _T('\0'))
            {
                return JsonValuePrivat::NumberType_none;
            }

            // 处理转义符
            if (m_pParseBuf[m_nCurIndex] == _T('\\'))
            {                
                strNum += m_pParseBuf[m_nCurIndex + 1];
                m_nCurIndex += 2;
                continue;
            }

            // 结束
            if (m_pParseBuf[m_nCurIndex] == _T(','))
            {
                m_nCurIndex ++;
                break;
            }
            if (m_pParseBuf[m_nCurIndex] == _T(']') ||
                m_pParseBuf[m_nCurIndex] == _T('}'))
            {
                break;
            }

            strNum += m_pParseBuf[m_nCurIndex];
            m_nCurIndex++;
        }

        if (m_nCurIndex >= m_strSize)
            return JsonValuePrivat::NumberType_none;

        if (strNum.find(_T('.')) != String::npos)
        {
            nNum = StringUtils::StrToF32(strNum.c_str());
            return JsonValuePrivat::NumberType_f32;
        }
        else
        {
            nNum2 = StringUtils::StrToS64(strNum.c_str());
            return JsonValuePrivat::NumberType_s64;
        }
    }


    void JsonDocPrivat::_IgnoreChar()
    {
        while (m_nCurIndex < m_strSize)
        {
            tchar c = m_pParseBuf[m_nCurIndex];
            if (c == _T(' '))
            {
                m_nCurIndex++;
                continue;
            }
            else if (c == _T('\t'))
            {
                m_nCurIndex++;
                continue;
            }
            else if (c == _T('\r'))
            {
                m_nCurIndex++;
                continue;
            }
            else if (c == _T('\n'))
            {
                m_nCurIndex++;
                continue;
            }
            return;
        }
    }

    const tchar* JsonDocPrivat::SaveToStream()
    {
        m_strJson = _T("");

        JsonObject* pTemp = m_pRoot;
        while (NULL != pTemp)
        {
            pTemp->m_pPrivate->SaveToString(m_strJson);
            pTemp = pTemp->m_pPrivate->m_pNextObject;
        }
        return m_strJson.c_str();
    }

    JsonObject* JsonDocPrivat::AddObject()
    {
        JsonObject* pTemp = new JsonObject;
		if (NULL == m_pRoot)
		{
			m_pRoot = pTemp;
			m_pEnd = pTemp;
		}
		else
		{
			m_pEnd->m_pPrivate->m_pNextObject = pTemp;
			if (m_pEnd == m_pRoot)
			{
				m_pRoot->m_pPrivate->m_pNextObject = pTemp;
			}
			m_pEnd = pTemp;
		}
        return m_pEnd;
    }
}

#endif // HYC_COMPILE_MODULE_JSON
