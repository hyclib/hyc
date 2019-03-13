//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  用于控制鼠标
//

#ifndef __BP_MOUSE_H_20101105113820__
#define __BP_MOUSE_H_20101105113820__

namespace hyc
{
    //-----------------------------------------------
    //  鼠标样式
    typedef enum {
        MouseStyle_Arrow,
        MouseStyle_IBeam,
        MouseStyle_Wait,
        MouseStyle_Cross,
        MouseStyle_UpArrow,
        MouseStyle_SizeNWSE,
        MouseStyle_SizeNESW,
        MouseStyle_SizeWE,
        MouseStyle_SizeNS,
        MouseStyle_SizeAll,
        MouseStyle_No,
        MouseStyle_Hand,
        MouseStyle_Appstatring,
        MouseStyle_Help,
    } MouseStyle;

    //-----------------------------------------------
    // 鼠标键
    typedef enum
    {
        MKC_No      = 0,
        MKC_Left    = 1<<0,
        MKC_Middle  = 1<<1,
        MKC_Right   = 1<<2,

    } MouseKeyCode;


#ifndef CN_PLATFORM_MOBILE

    //-----------------------------------------------
    //  鼠标
    class HYC_API Mouse
    {
    public:
        // 获得设置位置.
        static void GetPos( OUT integer& x, OUT integer& y );
        static void SetPos( integer x, integer y );

        // 判断鼠标键的状态.
        static bool IsLButtonDown();
        static bool IsMButtonDown();
        static bool IsRButtonDown();

        // 交换左右键.
        static void SwapButton( bool enable );
        static bool IsSwapButton();

        // 输入模拟.
        static void ClickLButton();        // click == down+up; 但效率高
        static void DownLButton();
        static void UpLButton();

        static void ClickMButton();
        static void DownMButton();
        static void UpMButton();

        static void ClickRButton();
        static void DownRButton();
        static void UpRButton();

        static void Wheel( integer amount );

        // 设置鼠标样式.
        static void SetCursor( MouseStyle ms );
        static void SetSysCursor( MouseStyle newStyle, MouseStyle standardMouse );    // standardMouse 指示要设置的鼠标状态; 谨慎使用, 设置新样式后, 默认的鼠标样式即改变
        static void ShowCursor( bool bShow );
    };
#endif
    
} // end namespace hyc

#endif // end of define __BP_MOUSE_H_20101105113820__
