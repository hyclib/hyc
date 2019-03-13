//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  閿洏鎿嶄綔
//

#ifndef __BP_KEYBORAD_H_20101105120213__
#define __BP_KEYBORAD_H_20101105120213__


namespace hyc
{
    //--------------------------------------------------
    //! 键盘管理.
    //--------------------------------------------------
#ifndef CN_PLATFORM_MOBILE
    class HYC_API Keyborad
    {
    public:
        /**
         * @brief 判断指定按键是否按下.
         * @param keyCode   0~9,A~Z 使用ASCII码; 其他使用KEY_CODE
         */
        static bool IsDown( int keyCode );

        /**
         * @brief 触发指定按键.
         */
        static void Click( int keyCode );       // Click == Down+Up;
        static void Down( int keyCode );
        static void Up( int keyCode );
    };
#endif

    //----------------------------------------------------------------------------------------------------
    //!	Windows
    //----------------------------------------------------------------------------------------------------
    // key code
    enum KeyCode {

        /**
         * Windows keycodes
         */
#if CN_PLATFORM == CN_PLATFORM_WINDOWS
        KC_Unkown       = 0,
        // borad
        KC_ESC          = VK_ESCAPE,
        KC_Backspace    = VK_BACK,
        KC_Enter        = VK_RETURN,
        KC_Space        = VK_SPACE,
        // function key
        KC_Insert       = VK_INSERT,
        KC_Delete       = VK_DELETE,
        KC_NumLock      = VK_NUMLOCK,
        KC_CapsLock     = VK_CAPITAL,
        KC_ScrollLock   = VK_SCROLL,
        KC_PrintScreen  = VK_SNAPSHOT,
        KC_Pause        = VK_PAUSE,
        KC_LWIN         = VK_LWIN,
        KC_RWIN         = VK_RWIN,
        // shift
        KC_Shift        = VK_SHIFT,
        KC_LShift       = VK_LSHIFT,
        KC_RShift       = VK_RSHIFT,
        // control
        KC_Control      = VK_CONTROL,
        KC_LControl     = VK_LCONTROL, 
        KC_RControl     = VK_RCONTROL,
        // alt
        KC_Alt          = VK_MENU, 
        KC_LAlt         = VK_LMENU, 
        KC_RAlt         = VK_RMENU,
        // F1~F12
        KC_F1 = VK_F1, KC_F2 = VK_F2, KC_F3 = VK_F3, KC_F4 = VK_F4, KC_F5 = VK_F5, KC_F6 = VK_F6, KC_F7 = VK_F7, KC_F8 = VK_F8, KC_F9 = VK_F9, KC_F10 = VK_F10, KC_F11 = VK_F11, KC_F12 = VK_F12,
        // direction control
        KC_PageUp       = VK_PRIOR,
        KC_PageDown     = VK_NEXT,
        KC_Home         = VK_HOME,
        KC_End          = VK_END,
        KC_Left         = VK_LEFT,
        KC_Up           = VK_UP,
        KC_Right        = VK_RIGHT,
        KC_Down         = VK_DOWN,
        // number pad
        KC_NUMPAD_0 = VK_NUMPAD0, KC_NUMPAD_1 = VK_NUMPAD1, KC_NUMPAD_2 = VK_NUMPAD2,
        KC_NUMPAD_3 = VK_NUMPAD3, KC_NUMPAD_4 = VK_NUMPAD4, KC_NUMPAD_5 = VK_NUMPAD5,
        KC_NUMPAD_6 = VK_NUMPAD6, KC_NUMPAD_7 = VK_NUMPAD7, KC_NUMPAD_8 = VK_NUMPAD8, KC_NUMPAD_9 = VK_NUMPAD9,
        KC_NUMPAD_ADD   = VK_ADD,
        KC_NUMPAD_SUB   = VK_SUBTRACT,
        KC_NUMPAD_MUL   = VK_MULTIPLY,
        KC_NUMPAD_DIV   = VK_DIVIDE,
        KC_NUMPAD_POINT = VK_DECIMAL,

        /**
         * Android keycodes
         */
#elif CN_PLATFORM == CN_PLATFORM_ANDROID
        KC_Back,
        KC_Menu,
        KC_Home,
        KC_Search,
#endif
    };

} // end namespace hyc

#endif // end of define __BP_KEYBORAD_H_20101105120213__