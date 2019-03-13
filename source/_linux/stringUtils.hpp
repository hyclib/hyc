//
// Copyright(c) 1999-2010 hyrj. All rights reserved.
// common libaray
// author:       lipengxiang
// description:  StringUtils鐨刲inux骞冲彴瀹炵幇
//

#ifndef __BP_STRINGUTILS_HPP_20101223153125__
#define __BP_STRINGUTILS_HPP_20101223153125__


namespace hyc
{
    //-----------------------------------------------
    bool StringUtils::Narrow2wide( const char* mbstr, uinteger cSizeMbs, OUT wchar_t* vcstr, OUT uinteger& cSizeVcs )
    {
        // check parameters
        if ( NULL == mbstr || NULL == vcstr || !cSizeMbs || !cSizeVcs )
            return false;

        // convert
        uinteger r = mbstowcs( vcstr, mbstr, cSizeVcs );
        if (-1 == r)
            trace(_T("Narrow2wide error"));
        cSizeVcs = r;
        return true;
    }

    //-----------------------------------------------
    bool StringUtils::Wide2narrow( const wchar_t* vcstr, uinteger cSizeVcs, OUT char* mbstr, OUT uinteger& cSizeMbs )
    {
        // check parameters
        if ( NULL == mbstr || NULL == vcstr || !cSizeMbs || !cSizeVcs )
        {
            return false;
        }

        // convert
        uinteger r = wcstombs( mbstr, vcstr, cSizeMbs );
        if (-1 == r)
            trace(_T("Wide2narrow error"));
        cSizeMbs = r;
        return true;

    }

} // end namespace hyc


#endif // end of define __BP_STRINGUTILS_HPP_20101223153125__