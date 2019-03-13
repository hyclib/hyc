//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  Universally Unique Identifier
//

#ifdef HYC_COMPILE_MODULE_IDENTIFIER

#ifndef __BP_IDENTIFIERUTILS_H_20111107112501__
#define __BP_IDENTIFIERUTILS_H_20111107112501__


namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	Unique Identifier utility
    //----------------------------------------------------------------------------------------------------
	class HYC_API IdentifierUtils
    {
    public:
        //! Universally Unique Identifier
        //! xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx (8-4-4-4-12)
        static String  generateUUID();
    };

} // end namespace hyc

#endif // end of define __BP_IDENTIFIERUTILS_H_20111107112501__
#endif // HYC_COMPILE_MODULE_IDENTIFIER


