//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  StringUtils的windows平台实现
//

#ifndef __BP_STRINGUTILS_HPP_2010120300236__
#define __BP_STRINGUTILS_HPP_2010120300236__


namespace hyc
{
    //-----------------------------------------------
    bool StringUtils::Narrow2wide( const char* mbstr, uinteger cSizeMbs, OUT wchar_t* vcstr, OUT uinteger& cSizeVcs )
    {
        // check parameters
        if ( NULL == mbstr || NULL == vcstr || !cSizeMbs || !cSizeVcs )
            return false;

        // convert (in windows Platform)
        integer c = ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, mbstr, cSizeMbs, vcstr, cSizeVcs );
        cSizeVcs = c;

        return ( c != 0 );
    }

    //-----------------------------------------------
    bool StringUtils::Wide2narrow( const wchar_t* vcstr, uinteger cSizeVcs, OUT char* mbstr, OUT uinteger& cSizeMbs )
    {
        // check parameters
        if ( NULL == mbstr || NULL == vcstr || !cSizeMbs || !cSizeVcs )
        {
            return false;
        }

        // convert (in windows Platform)
        BOOL r;
        integer c = WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, vcstr, cSizeVcs, mbstr, cSizeMbs, "?", &r );
        if ( r || c == 0 )
            return false;
        cSizeMbs = c;
        return true;
    }

} // end namespace hyc

#endif // end of define __BP_STRINGUTILS_HPP_2010120300236__