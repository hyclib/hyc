////////////////////////////////////////////////////////
// Copyright(c) 1986-2100, TT, All Rights Reserved
// Author:  Hu Tianzhi
// Created: 2011-1-21 10:03:27
// Describe: hash 算法
////////////////////////////////////////////////////////

#ifndef	_Tian_20111211003271_H_
#define	_Tian_20111211003271_H_

#include "../hycPlatform.h"


namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	HashCode
    //----------------------------------------------------------------------------------------------------
    /*struct HashCode
    {
        HashCode()  { _code1 = _code2 = 0; }
        HashCode( const HashCode& rhs ) { _code1 = rhs._code1; _code2 = rhs._code2; }

        bool operator==( const HashCode& rhs ) const   { return _code1 == rhs._code1 && _code2 == rhs._code2; }
        bool operator!=( const HashCode& rhs ) const   { return !(*this==rhs); }
        bool operator<( const HashCode& rhs ) const    { return (_code1+_code2) < (rhs._code1+rhs._code2); }

        integer _code1;
        integer _code2;
    };*/

    //----------------------------------------------------------------------------------------------------
    //* hash
    //----------------------------------------------------------------------------------------------------
     
    class hash
    {
    public:

        //nSize: 大小
        template<typename T>
        static integer GetHash( const T& key, integer nSize )   { uinteger nHash = (uinteger)key; return nHash % nSize; }
        
        //nSize: 大小
        template<typename T>
        static integer GetHash2(const T& key, integer nSize)    { uinteger nHash = (uinteger)key * 2; return nHash % nSize; }
    };


    //----------------------------------------------------------------------------------------------------
    //*	特化
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    template<>
    inline integer hash::GetHash<String>(const String& key, integer nSize)
    {
        uinteger nHash = 1315423911;
        tchar* pChar = (tchar*)key.c_str();
        while (_T('\0') != *pChar)
        {
            nHash ^= ((nHash << 5) + (*pChar) + (nHash >> 2));
            pChar++;
        }
        return nHash % nSize;
    }

    //-----------------------------------------------
    template<>
    inline integer hash::GetHash2<String>(const String& key, integer nSize)
    {
        integer seed  = 131;
        uinteger nHash = 0;
        tchar* pChar = (tchar*)key.c_str();
        while (_T('\0') != *pChar)
        {
            nHash = (nHash * seed) + (*pChar);
            pChar++;
        }
        return nHash % nSize;
    }

} // end namespace of hyc

#endif // _Tian_20111211003271_H_