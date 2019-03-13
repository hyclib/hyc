//
// Copyright(c) 1999-2014 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_YUNSTREAM_H_20140905104903__
#define __BP_YUNSTREAM_H_20140905104903__


void container_yunstream()
{
    f32 num[] = {
        11.1f, 
        0.f,
        12.35f,
        2333.34f,
        23434.f
    };
    ArrayFind<f32>  arrF32(num, sizeof(num)/sizeof(f32));

    String strs[] = {
        String(_T("你好")), 
        String(_T("")),
        String(_T("a")),
        String(_T("吗"))
    };
    ArrayFind<String> arrStr(strs, 4);
 
    ByteStream bs;
    bs.add("235d df", 5);

    YunStream ys;
    ys.addColBegin();
    {
        ys.addCol((u8)45);
        ys.addCol((u16)45);
        ys.addCol((s32)-45);
        ys.addColArr(arrF32);
        ys.addColArrString(arrStr);
        ys.addCol(_T("你utf8好"));
        ys.addCol(_T(""));
        ys.addCol(bs);
    }
    ys.addColEnd();

    u8 _u8;
    hy_assertTrue(ys.getColValue(0, _u8) && _u8 == 45);
    u16 _u16;
    hy_assertTrue(ys.getColValue(1, _u16) && _u16 == 45);
    s32 _s32;
    hy_assertTrue(ys.getColValue(2, _s32) && _s32 == -45);

    arrF32.clear();
    hy_assertTrue(ys.getColValueArr(3, arrF32) && arrF32.size() == 5);

    arrStr.clear();
    hy_assertTrue(ys.getColValueArrString(4, arrStr) && arrStr.size() == 4);

    String _str;
    hy_assertTrue(ys.getColValue(5, _str) && _str == _T("你utf8好"));
    hy_assertTrue(ys.getColValue(6, _str) && _str.empty());

    hy_assertTrue(ys.isLegalYun());

    YunStream ys2;
    ys2.add(ys.begin(), ys.size());

    s32 sur;
    ys2._curPos = ys2.end();
    hy_assertTrue(ys2.analyseSelf_b(sur, ys2.begin()) && sur == 0);

    ByteStream bs2;
    hy_assertTrue(ys.getColValue(7, bs2));

    hy_assertTrue(bs2.size() == bs.size());
    for (u32 i = 0; i < bs2.size(); i++)
    {
        hy_assertTrue( bs2.getAt(i) == bs.getAt(i) );
    }


    arrStr.clear();
}

#endif // end of define __BP_YUNSTREAM_H_20140905104903__