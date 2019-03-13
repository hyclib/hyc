////////////////////////////////////////////////////////
// Copyright(c) 1986-2100, TT, All Rights Reserved
// Author:  Hu Tianzhi
// Created: 2010-12-28 9:15:36
// Describe:
////////////////////////////////////////////////////////

#ifdef HYC_COMPILE_MODULE_JSON

#ifndef	_Tian_2010122891536_H_
#define	_Tian_2010122891536_H_



namespace hyc
{
    class JsonValue;
    class JsonObject;
    class JsonObjectPrivat
    {
    public:
        JsonObjectPrivat();
        ~JsonObjectPrivat();

        // Return the value which the name is strName
        JsonValue*  GetFirstValue(const tchar* strName = NULL);

        JsonValue*  AddValue(const tchar* strName, f32 nNum);
        JsonValue*  AddValue(const tchar* strName, s64 nNum);
        JsonValue*  AddValue(const tchar* strName, const tchar* strValue);
        JsonValue*  AddValue(const tchar* strName, bool bBool);
        JsonValue*  AddValue(const tchar* strName, JsonObject* pObject);
        JsonValue*  AddValue(const tchar* strName);
        JsonValue*  AddArrayValue(const tchar* strName);
        JsonValue*  AddNullValue(const tchar* strName);

        void    SaveToString(String& str);

    public:
        void    _InsertValue(JsonValue* pValue);
    public:
        JsonValue*  m_pBeginValue;
        JsonValue*  m_pEndValue;

        JsonObject* m_pNextObject;
    };
}
#endif // _Tian_2010122891536_H_

#endif // HYC_COMPILE_MODULE_JSON
