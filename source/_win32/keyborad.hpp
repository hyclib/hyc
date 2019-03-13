//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  keyborad的win32平台实现
//

#ifndef __BP_KEYBORAD_HPP_20101108100122__
#define __BP_KEYBORAD_HPP_20101108100122__


namespace hyc
{
    //-----------------------------------------------
    bool Keyborad::IsDown( int keyCode )
    {
#pragma warning(disable: 4800)
        return (0x8000 & ::GetAsyncKeyState( keyCode ) );
#pragma warning(default: 4800)
    }

    //-----------------------------------------------
    void Keyborad::Click( int keyCode )
    {
        ::INPUT input[2];
        input[0].type       = INPUT_KEYBOARD;
        input[0].ki.dwFlags = ~KEYEVENTF_KEYUP;	// Down
        input[0].ki.wVk     = keyCode;
        input[1].type       = INPUT_KEYBOARD;
        input[1].ki.dwFlags = KEYEVENTF_KEYUP;	// Up
        input[1].ki.wVk     = keyCode;

        ::SendInput(2, input, sizeof(input[0]));
    }

    //-----------------------------------------------
    void Keyborad::Down( int keyCode )
    {
        ::INPUT input;
        input.type          = INPUT_KEYBOARD;
        input.ki.dwFlags    = ~KEYEVENTF_KEYUP;
        input.ki.wVk        = keyCode;

        ::SendInput(1, &input, sizeof(input));
    }

    //-----------------------------------------------
    void Keyborad::Up( int keyCode )
    {
        ::INPUT input;
        input.type          = INPUT_KEYBOARD;
        input.ki.dwFlags    = KEYEVENTF_KEYUP;
        input.ki.wVk        = keyCode;

        ::SendInput(1, &input, sizeof(input));
    }


} // end namespace hyc

#endif // end of define __BP_KEYBORAD_H_20101108100122__