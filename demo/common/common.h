//
//  _common.h
//  hyc
//
//  Created by 鏉?楣忕繑 on 12-1-14.
//  Copyright (c) 2012骞?鍗庤€€淇℃伅绉戞妧鏈夐檺鍏徃. All rights reserved.
//

#ifndef hyc__common_h
#define hyc__common_h

//////////////////////////////////////////
// common
void common()
{
    printf("\r\n====================================\r\n== common test start\r\n====================================\r\n");

    {
        void* p = MemoryAlloc::Malloc(10);
        p = MemoryAlloc::Realloc(p, 20);
        p = MemoryAlloc::Realloc(p, 30);
        p = MemoryAlloc::Realloc(p, 65);
        p = MemoryAlloc::Realloc(p, 30);
        p = MemoryAlloc::Realloc(p, 128);
        MemoryAlloc::Free(p);
    }

    /////////////////////////////  
    // math
    /////////////////////////////
    
    // degree & radian
    hy_assertTrue( Math::PI_HALF/3   == Math::Degree2Radian(30) );
    hy_assertTrue( 90.0f             == Math::Radian2Degree(Math::PI_HALF) );
    hy_assertTrue( 0.5f              == Math::MakeIn2Pi(Math::PI_2+0.5f) );
    hy_assertTrue( 0.5f-Math::PI     == Math::MakeInPi(Math::PI+0.5f) );
    
    // compare
    hy_assertTrue( Math::Equals(0.000001f, 0.000002f) );
    hy_assertTrue( Math::LessThan(0.000001f, 0.000003f) );
    hy_assertTrue( Math::LessEquals(0.000001f, 0.000002f) );
    hy_assertTrue( Math::LessEquals(0.000001f, 0.000003f) );
    hy_assertTrue( Math::MoreThan(0.000003f, 0.000001f) );
    hy_assertTrue( Math::MoreEquals(0.000003f, 0.000002f) );
    
    hy_assertTrue( 0.000003f == Math::Max(0.000003f, 0.000002f) );
    hy_assertTrue( 0.000002f == Math::Min(0.000003f, 0.000002f) );
    
    // math function
    hy_assertTrue( 0.000002f == Math::Abs(0.000002f) );
    hy_assertTrue( 0.000002f == Math::Abs(-0.000002f) );
    hy_assertTrue( 1.0f      == Math::Ceil(0.8f) );
    hy_assertTrue( 0.0f      == Math::Ceil(-0.8f) );
    hy_assertTrue( 0.0f      == Math::Floor(0.8f) );
    hy_assertTrue( -1.0f     == Math::Floor(-0.8f) );

    // sqrt
    hy_assertTrue( Math::Equals( Math::Sqrt( 2 ), Math::Sqrt_q( 2 ) ) );
    hy_assertTrue( Math::Equals( Math::InvSqrt( 2 ), Math::InvSqrt_q( 2 ), 0.01f ) );
    hy_assertTrue( Math::IsPowerOf2(1) );
    hy_assertTrue( Math::IsPowerOf2(2) );
    hy_assertTrue( Math::IsPowerOf2(4) );
    hy_assertTrue( Math::IsPowerOf2(128) );
    hy_assertTrue( Math::IsPowerOf2(256) );
    hy_assertFalse( Math::IsPowerOf2(3) );
    hy_assertFalse( Math::IsPowerOf2(255) );
    hy_assertFalse( Math::IsPowerOf2(0) );

    hy_assertTrue( Math::IsPowerOf2( Math::GetBestPowerOf2(154) ) );
    hy_assertTrue( Math::IsPowerOf2( Math::GetBestPowerOf2(4) ) );
    hy_assertTrue( Math::IsPowerOf2( Math::GetBestPowerOf2(128) ) );
    hy_assertTrue( Math::IsPowerOf2( Math::GetBestPowerOf2(129) ) );
    
    // swap
    
    f32 _1f32 = 0.7f, _2f32 = 0.8f;
    Math::Swap(_1f32, _2f32);
    hy_assertTrue( _2f32 == 0.7f );
    hy_assertTrue( _1f32 == 0.8f );
    
    s8 _1s8 = 7, _2s8 = 8;
    Math::Swap(_1s8, _2s8);
    hy_assertTrue( _1s8 == 8 );
    hy_assertTrue( _2s8 == 7 );
    
    s16 _1s16 = 7, _2s16 = 8;
    Math::Swap(_1s16, _2s16);
    hy_assertTrue( _1s16 == 8 );
    hy_assertTrue( _2s16 == 7 );
    
    s32 _1s32 = 7, _2s32 = 8;
    Math::Swap(_1s32, _2s32);
    hy_assertTrue( _1s32 == 8 );
    hy_assertTrue( _2s32 == 7 );
    
    s64 _1s64 = 7, _2s64 = 8;
    Math::Swap(_1s64, _2s64);
    hy_assertTrue( _1s64 == 8 );
    hy_assertTrue( _2s64 == 7 );
    
    u8 _1u8 = 7, _2u8 = 8;
    Math::Swap(_1u8, _2u8);
    hy_assertTrue( _1u8 == 8 );
    hy_assertTrue( _2u8 == 7 );
    
    u16 _1u16 = 7, _2u16 = 8;
    Math::Swap(_1u16, _2u16);
    hy_assertTrue( _1u16 == 8 );
    hy_assertTrue( _2u16 == 7 );
    
    u32 _1u32 = 7, _2u32 = 8;
    Math::Swap(_1u32, _2u32);
    hy_assertTrue( _1u32 == 8 );
    hy_assertTrue( _2u32 == 7 );
    
    u64 _1u64 = 7, _2u64 = 8;
    Math::Swap(_1u64, _2u64);
    hy_assertTrue( _1u64 == 8 );
    hy_assertTrue( _2u64 == 7 );
    
    // random
    Math::SRand(0);
    hy_assertTrue( Math::Rand01() >= 0.0f );
    hy_assertTrue( Math::Rand01() <= 1.0f );

    hy_assertTrue(Math::Rand01() <= 1.0f);
    hy_assertTrue(Math::Rand01() <= 1.0f);
    hy_assertTrue(Math::Rand01() <= 1.0f);
    hy_assertTrue(Math::Rand01() >= 0.0f);
    hy_assertTrue(Math::Rand01() >= 0.0f);
    hy_assertTrue(Math::Rand01() >= 0.0f);

    real ratios[] = { 0.1f, 0.3f, 0.8f };
    u32 rest[] = {0,0,0,0};
    Array<u32> arrResult(rest, sizeof(rest)/sizeof(u32));

    for (u32 i = 0; i < 100000; i++)
    {
        arrResult[Math::RandMulti(ratios, sizeof(ratios)/sizeof(real))+1]++;
    }
    real total = arrResult[0]+arrResult[1]+arrResult[2]+arrResult[3];
    printf("random multi (0.1f,0.3f,0.8f)-(n:%u,%f,%f,%f)\n", arrResult[0],arrResult[1]/total,arrResult[2]/total,arrResult[3]/total);
    
    
    ////////////////////////////////////////
    //  string
    ////////////////////////////////////////
    
    String str;
    String str2(_T("1234"));
    str = _T("sdfdb") + str2;
    hy_assertTrue(str == _T("sdfdb1234"));
    hy_assertTrue( String(_T("dfdfd")) == String(_T("dfdfd")).c_str() );
    hy_assertFalse( str == String(_T("afdfd")).c_str() );
    
    // format
    str.reserve(10);
    str.format(_T("%d"), 1092 );
    hy_assertTrue( _T("1092") == str );
    
    // to lower
    str = _T("abcDEF");
    str.toLower();
    hy_assertTrue( str == _T("abcdef") );
    
    str.toUpper();
    hy_assertTrue( str == _T("ABCDEF") );

    // find.
    hy_assertTrue(0 == str.find(_T("AB")));
    hy_assertTrue(0 == str.find(_T("ABC")));
    hy_assertTrue(-1 == str.find(_T("ABc")));
    hy_assertTrue(2 == str.find(_T("CD")));
    hy_assertTrue(-1 == str.find(_T("CDc")));
    hy_assertTrue(0 == str.find(_T("ABCDEF")));
    hy_assertTrue(-1 == str.find(_T("ABCDEFc")));
    hy_assertTrue(0 == str.find(_T("ABCDE")));
    hy_assertTrue(1 == str.find(_T("BCDEF")));
    hy_assertTrue(-1 == str.find(_T("ABCDE"), 1));
    hy_assertTrue(1 == str.find(_T("BCDEF"), 1));
    hy_assertTrue(1 == str.find_last_of(_T("AB")));
    hy_assertTrue(2 == str.find_last_of(_T("ABC")));
    hy_assertTrue(-1 == str.find_last_of(_T("ABc")));
    hy_assertTrue(3 == str.find_last_of(_T("CD")));
    hy_assertTrue(-1 == str.find_last_of(_T("CDc")));
    hy_assertTrue(5 == str.find_last_of(_T("ABCDEF")));
    hy_assertTrue(-1 == str.find_last_of(_T("ABCDEFc")));
    hy_assertTrue(4 == str.find_last_of(_T("ABCDE")));
    hy_assertTrue(5 == str.find_last_of(_T("BCDEF")));
    hy_assertTrue(4 == str.find_last_of(_T("ABCDE"), str.size()-2));
    hy_assertTrue(-1 == str.find_last_of(_T("BCDEF"), str.size()-2));

    // insert.
    str.insert(1, 2, _T('A'));
    hy_assertTrue( _T("AAABCDEF") == str );
    str.erase(1, 2);
    hy_assertTrue( _T("ABCDEF") == str );
    str.insert(str.npos, 2, _T('A'));
    hy_assertTrue( _T("ABCDEFAA") == str );
    str.erase(str.npos, 2);
    hy_assertTrue( _T("ABCDEF") == str );

    str.insert(1, _T("AB"), 2);
    hy_assertTrue( _T("AABBCDEF") == str );
    str.erase(1, 2);
    hy_assertTrue( _T("ABCDEF") == str );
    str.insert(str.npos, _T("AB"), 2);
    hy_assertTrue( _T("ABCDEFAB") == str );
    str.erase(str.npos, 2);
    hy_assertTrue( _T("ABCDEF") == str );

    // replace.
    str = _T("123456789");
    str.replace(0, str.npos, _T('9'), _T('0'));
    hy_assertTrue( _T("123456780") == str );
    str.replace(0, str.npos, _T('0'), _T("01"));
    hy_assertTrue( _T("1234567801") == str );
    str.replace(0, str.npos, _T("01"), _T('9'));
    hy_assertTrue( _T("123456789") == str );
    str.replace(0, str.npos, _T("567"), _T("aaa"));
    hy_assertTrue( _T("1234aaa89") == str );

    // split
    str = _T("1,2,3,4,5,6");
    Array<String>   arrStr;
    str.split(_T(','), &arrStr);
    hy_assertTrue( arrStr.size() == 6 );
    hy_assertTrue( arrStr[0] == _T("1") );
    hy_assertTrue( arrStr[1] == _T("2") );
    hy_assertTrue( arrStr[2] == _T("3") );
    hy_assertTrue( arrStr[3] == _T("4") );
    hy_assertTrue( arrStr[4] == _T("5") );
    hy_assertTrue( arrStr[5] == _T("6") );
    
    str = _T("1,2,");
    str.split(_T(','), &arrStr);
    hy_assertTrue( arrStr.size() == 3 );

    printf("\r\n====================================\r\n== common test end\r\n====================================\r\n");
}


#endif
