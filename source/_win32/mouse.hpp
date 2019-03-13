//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  mouse的win32平台实现
//

#ifndef __BP_MOUSE_HPP_2010110895855__
#define __BP_MOUSE_HPP_2010110895855__


namespace hyc
{
    //-----------------------------------------------
    void Mouse::GetPos( OUT integer& x, OUT integer& y )
    {
        POINT   p;
        ::GetCursorPos(&p);
        x = p.x;
        y = p.y;
    }

    //-----------------------------------------------
    void Mouse::SetPos( integer x, integer y )
    {
        ::SetCursorPos(x, y);
    }

    //-----------------------------------------------
    bool Mouse::IsLButtonDown()
    {
        return Keyborad::IsDown(VK_LBUTTON);
    }

    //-----------------------------------------------
    bool Mouse::IsMButtonDown()
    {
        return Keyborad::IsDown(VK_MBUTTON);
    }

    //-----------------------------------------------
    bool Mouse::IsRButtonDown()
    {
        return Keyborad::IsDown(VK_RBUTTON);
    }

    //-----------------------------------------------
    void Mouse::SwapButton( bool enable )
    {
        ::SystemParametersInfo(SPI_SETMOUSEBUTTONSWAP, enable, 0, 0);
    }

    //-----------------------------------------------
    bool Mouse::IsSwapButton()
    {
        return 0 != ::GetSystemMetrics(SM_SWAPBUTTON);
    }

    //-----------------------------------------------
    void Mouse::ClickLButton()
    {
        ::INPUT input[2];
        ZeroMemory(input, sizeof(INPUT)*2);
        input[0].type           = INPUT_MOUSE;
        input[0].mi.dwFlags     = MOUSEEVENTF_LEFTDOWN;
        input[0].mi.mouseData   = 0;
        input[1].type           = INPUT_MOUSE;
        input[1].mi.dwFlags     = MOUSEEVENTF_LEFTUP;
        input[1].mi.mouseData   = 0;

        ::SendInput(2, input, sizeof(input[0]));
    }

    //-----------------------------------------------
    void Mouse::DownLButton()
    {
        ::INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type           = INPUT_MOUSE;
        input.mi.dwFlags     = MOUSEEVENTF_LEFTDOWN;
        input.mi.mouseData   = 0;

        ::SendInput(1, &input, sizeof(input));
    }

    //-----------------------------------------------
    void Mouse::UpLButton()
    {
        ::INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type           = INPUT_MOUSE;
        input.mi.dwFlags     = MOUSEEVENTF_LEFTUP;
        input.mi.mouseData   = 0;

        ::SendInput(1, &input, sizeof(input));
    }

    //-----------------------------------------------
    void Mouse::ClickMButton()
    {
        ::INPUT input[2];
        ZeroMemory(input, sizeof(INPUT)*2);
        input[0].type           = INPUT_MOUSE;
        input[0].mi.dwFlags     = MOUSEEVENTF_MIDDLEDOWN;
        input[0].mi.mouseData   = 0;
        input[1].type           = INPUT_MOUSE;
        input[1].mi.dwFlags     = MOUSEEVENTF_MIDDLEUP;
        input[1].mi.mouseData   = 0;

        ::SendInput(2, input, sizeof(input[0]));
    }

    //-----------------------------------------------
    void Mouse::DownMButton()
    {
        ::INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type           = INPUT_MOUSE;
        input.mi.dwFlags     = MOUSEEVENTF_MIDDLEDOWN;
        input.mi.mouseData   = 0;

        ::SendInput(1, &input, sizeof(input));
    }

    //-----------------------------------------------
    void Mouse::UpMButton()
    {
        ::INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type           = INPUT_MOUSE;
        input.mi.dwFlags     = MOUSEEVENTF_MIDDLEUP;
        input.mi.mouseData   = 0;

        ::SendInput(1, &input, sizeof(input));
    }

    //-----------------------------------------------
    void Mouse::ClickRButton()
    {
        ::INPUT input[2];
        ZeroMemory(input, sizeof(INPUT)*2);
        input[0].type           = INPUT_MOUSE;
        input[0].mi.dwFlags     = MOUSEEVENTF_RIGHTDOWN;
        input[0].mi.mouseData   = 0;
        input[1].type           = INPUT_MOUSE;
        input[1].mi.dwFlags     = MOUSEEVENTF_RIGHTUP;
        input[1].mi.mouseData   = 0;

        ::SendInput(2, input, sizeof(input[0]));
    }

    //-----------------------------------------------
    void Mouse::DownRButton()
    {
        ::INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type           = INPUT_MOUSE;
        input.mi.dwFlags     = MOUSEEVENTF_RIGHTDOWN;
        input.mi.mouseData   = 0;

        ::SendInput(1, &input, sizeof(input));
    }

    //-----------------------------------------------
    void Mouse::UpRButton()
    {
        ::INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type           = INPUT_MOUSE;
        input.mi.dwFlags     = MOUSEEVENTF_RIGHTUP;
        input.mi.mouseData   = 0;

        ::SendInput(1, &input, sizeof(input));
    }

    //-----------------------------------------------
    void Mouse::Wheel( integer amount )
    {
        ::INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type           = INPUT_MOUSE;
        input.mi.dwFlags     = MOUSEEVENTF_WHEEL;
        input.mi.mouseData   = amount;

        ::SendInput(1, &input, sizeof(input));
    }

    //-----------------------------------------------
    void hyc::Mouse::SetCursor( MouseStyle ms )
    {
        switch ( ms ) {
            case MouseStyle_Appstatring:
                ::SetCursor( ::LoadCursor(NULL,IDC_APPSTARTING) );
                break;
            case MouseStyle_Arrow:
                ::SetCursor( ::LoadCursor(NULL,IDC_ARROW) );
                break;
            case MouseStyle_Cross:
                ::SetCursor( ::LoadCursor(NULL,IDC_CROSS) );
                break;
            case MouseStyle_Hand:
                ::SetCursor( ::LoadCursor(NULL,IDC_HAND) );
                break;
            case MouseStyle_Help:
                ::SetCursor( ::LoadCursor(NULL,IDC_HELP) );
                break;
            case MouseStyle_IBeam:
                ::SetCursor( ::LoadCursor(NULL,IDC_IBEAM) );
                break;
            case MouseStyle_No:
                ::SetCursor( ::LoadCursor(NULL,IDC_NO) );
                break;
            case MouseStyle_SizeAll:
                ::SetCursor( ::LoadCursor(NULL,IDC_SIZEALL) );
                break;
            case MouseStyle_SizeNESW:
                ::SetCursor( ::LoadCursor(NULL,IDC_SIZENESW) );
                break;
            case MouseStyle_SizeNS:
                ::SetCursor( ::LoadCursor(NULL,IDC_SIZENS) );
                break;
            case MouseStyle_SizeNWSE:
                ::SetCursor( ::LoadCursor(NULL,IDC_SIZENWSE) );
                break;
            case MouseStyle_SizeWE:
                ::SetCursor( ::LoadCursor(NULL,IDC_SIZEWE) );
                break;
            case MouseStyle_UpArrow:
                ::SetCursor( ::LoadCursor(NULL,IDC_UPARROW) );
                break;
            case MouseStyle_Wait:
                ::SetCursor( ::LoadCursor(NULL,IDC_WAIT) );
                break;
        }
    }

    //-----------------------------------------------
    void hyc::Mouse::ShowCursor( bool bShow )
    {
        ::ShowCursor( bShow );
    }

    //-----------------------------------------------
    void hyc::Mouse::SetSysCursor( MouseStyle newStyle, MouseStyle standardMouse )
    {
        HCURSOR hCur = NULL;

        switch ( newStyle ) {
            case MouseStyle_Appstatring:
                hCur = ::LoadCursor(NULL,IDC_APPSTARTING);
                break;
            case MouseStyle_Arrow:
                hCur = ::LoadCursor(NULL,IDC_ARROW);
                break;
            case MouseStyle_Cross:
                hCur = ::LoadCursor(NULL,IDC_CROSS);
                break;
            case MouseStyle_Hand:
                hCur = ::LoadCursor(NULL,IDC_HAND);
                break;
            case MouseStyle_Help:
                hCur = ::LoadCursor(NULL,IDC_HELP);
                break;
            case MouseStyle_IBeam:
                hCur = ::LoadCursor(NULL,IDC_IBEAM);
                break;
            case MouseStyle_No:
                hCur = ::LoadCursor(NULL,IDC_NO);
                break;
            case MouseStyle_SizeAll:
                hCur = ::LoadCursor(NULL,IDC_SIZEALL);
                break;
            case MouseStyle_SizeNESW:
                hCur = ::LoadCursor(NULL,IDC_SIZENESW);
                break;
            case MouseStyle_SizeNS:
                hCur = ::LoadCursor(NULL,IDC_SIZENS);
                break;
            case MouseStyle_SizeNWSE:
                hCur = ::LoadCursor(NULL,IDC_SIZENWSE);
                break;
            case MouseStyle_SizeWE:
                hCur = ::LoadCursor(NULL,IDC_SIZEWE);
                break;
            case MouseStyle_UpArrow:
                hCur = ::LoadCursor(NULL,IDC_UPARROW);
                break;
            case MouseStyle_Wait:
                hCur = ::LoadCursor(NULL,IDC_WAIT);
                break;
            default:
                return;
        } // switch

        if ( NULL == hCur ) return;

        DWORD id = -1;
        switch ( standardMouse ) {
            case MouseStyle_Appstatring:
                id = OCR_APPSTARTING;
                break;
            case MouseStyle_Arrow:
                id = OCR_NORMAL;
                break;
            case MouseStyle_Cross:
                id = OCR_CROSS;
                break;
            case MouseStyle_Hand:
                id = OCR_HAND;
                break;
            case MouseStyle_Help:
                id = OCR_HAND;
                break;
            case MouseStyle_IBeam:
                id = OCR_IBEAM;
                break;
            case MouseStyle_No:
                id = OCR_NO;
                break;
            case MouseStyle_SizeAll:
                id = OCR_SIZEALL;
                break;
            case MouseStyle_SizeNESW:
                id = OCR_SIZENESW;
                break;
            case MouseStyle_SizeNS:
                id = OCR_SIZENS;
                break;
            case MouseStyle_SizeNWSE:
                id = OCR_SIZENWSE;
                break;
            case MouseStyle_SizeWE:
                id = OCR_SIZEWE;
                break;
            case MouseStyle_UpArrow:
                id = OCR_UP;
                break;
            case MouseStyle_Wait:
                id = OCR_WAIT;
                break;
            default:
                return;
        } // switch

        ::SetSystemCursor( hCur, id );
    }

} // end namespace hyc

#endif // end of define __BP_MOUSE_HPP_2010110895855__