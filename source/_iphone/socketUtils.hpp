/*
 *  SocketUtils.hpp
 *  hyc
 *
 *  Created by lipengxiang on 11-8-19.
 *  Copyright 2011 hy. All rights reserved.
 *
 */



namespace hyc
{
    
    //-----------------------------------------------
    integer SocketUtils::GetAdapterNum( OUT Array<String>* pArrNames )
    {
        if (pArrNames)
        {
            pArrNames->clear();
            pArrNames->push_back( _T("en0") );
        }
        return 1;
    }
    
    //-----------------------------------------------
    hyc::MacAddress SocketUtils::GetAdapterAddr( uinteger )
    {
        int                 mib[6];
        size_t              len;
        char*               buf;
        unsigned char*      ptr;
        struct if_msghdr*   ifm;
        struct sockaddr_dl* sdl;
        
        MacAddress addr;
        ZeroMemory(&addr, sizeof(MacAddress));
        
        mib[0] = CTL_NET;
        mib[1] = AF_ROUTE;
        mib[2] = 0;
        mib[3] = AF_LINK;
        mib[4] = NET_RT_IFLIST;
        
        if ((mib[5] = if_nametoindex("en0")) == 0) {
            return addr;
        }
        
        if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
            return addr;
        }
        
        if ((buf = (char*)malloc(len)) == NULL) {
            return addr;
        }
        
        if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
            free(buf);
            return addr;
        }
        
        ifm = (struct if_msghdr *)buf;
        sdl = (struct sockaddr_dl *)(ifm + 1);
        ptr = (unsigned char *)LLADDR(sdl);
        
        for (int i = 0; i < sizeof(MacAddress); ++i) {
            addr.addr[i] = *(ptr+i);
        }
        free(buf);
        
        return addr;
    }
    
	
} // end namespace hyc


