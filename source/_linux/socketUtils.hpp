//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// YunFramework Gateway
// author:       lipengxiang
// description:  
//

#ifndef __BP_SOCKETUTILS_HPP_20110819140737__
#define __BP_SOCKETUTILS_HPP_20110819140737__


namespace hyc
{
	//-----------------------------------------------
	integer SocketUtils::GetAdapterNum( OUT Array<String>* pArrNames )
    {
        if (pArrNames)
            pArrNames->clear();

        struct ifreq *ifr, *ifend;
        struct ifconf ifc;
        struct ifreq ifs[16];
        int sockfd;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        ifc.ifc_len = sizeof(ifs);
        ifc.ifc_req = ifs;

        if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {

            CloseSocket(sockfd);
            return -1;
        } else {

            int num = 0;

            ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
            for (ifr = ifc.ifc_req; ifr < ifend; ++ifr) {
                if (ifr->ifr_addr.sa_family == AF_INET) {
                    if ( strncmp(ifr->ifr_name, "lo", strlen("lo")) ) 
                    {
                        if (pArrNames)
                        {
                            cString name = ifr->ifr_name;
#if defined(UNICODE) || defined(_UNICODE)
                            wString wname;
                            StringUtils::Narrow2wide( name, wname );
                            pArrNames->push_back( wname );
#else
                            pArrNames->push_back( name );
#endif
                        }
                        ++num; //printf(“[dev]%s\n”,ifr->ifr_name);
                    }
                }
            }

            CloseSocket(sockfd);

            return num;
        }
    }

    //-----------------------------------------------
    hyc::MacAddress SocketUtils::GetAdapterAddr( uinteger index )
    {
        MacAddress          addr;
        memset( addr.addr, 0, sizeof(addr.addr)/sizeof(addr.addr[0]) );

        // get the name of adapters
        Array<String> arrNetNames;
        if ( 0 >= GetAdapterNum(&arrNetNames) )
            return addr;
        if ( index >= arrNetNames.size() )
            return addr;

        // get the mac of adapter
        int sockfd;
        if (SOCKET_ERROR == (sockfd = socket(PF_INET, SOCK_STREAM, 0)))
        {
            return addr;            
        }

        struct ifreq req;

        ZeroMemory(&req, sizeof(struct ifreq));
#if defined(UNICODE) || defined(_UNICODE)
        cString name;
        StringUtils::Wide2narrow( arrNetNames[index], name );
        strcpy(req.ifr_name, name.c_str());
#else
        strcpy(req.ifr_name, arrNetNames[index].c_str());
#endif
        ioctl(sockfd, SIOCGIFHWADDR, &req);

        for (u32 i = 0; i < sizeof(MacAddress); ++i) {
            addr.addr[i] = req.ifr_hwaddr.sa_data[i];
        }

        CloseSocket(sockfd);
        return addr;
    }

} // end namespace hyc

#endif // end of define __BP_SOCKETUTILS_HPP_20110819140737__

