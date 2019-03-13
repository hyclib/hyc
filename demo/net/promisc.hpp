//
//  main.cpp
//  promisc
//
//  Created by brainpoint on 10/30/15.
//  Copyright  2015 citong. All rights reserved.
//


//#   include <linux/sysctl.h>

#include <Mstcpip.h>

struct NetPkg_IP {
    unsigned int	ip_length:4;	/*little-endian*/
    unsigned int	ip_version:4;
    unsigned char	ip_tos;
    unsigned short	ip_id;
    unsigned short  ip_flags;
    unsigned char   ip_ttl;
    unsigned char   ip_protocol;
    unsigned short  ip_cksum;
    unsigned int	ip_source;
    unsigned int	ip_dest;
};

struct NetPkg_TCP {
    unsigned short	tcp_source_port;
    unsigned short  tcp_dest_port;
    unsigned int	tcp_seqno;
    unsigned int	tcp_ackno;
    unsigned int    tcp_res1:4,	/*little-endian*/
                    tcp_hlen:4,
                    tcp_fin:1,
                    tcp_syn:1,
                    tcp_rst:1,
                    tcp_psh:1,
                    tcp_ask:1,
                    tcp_urg:1,
                    tcp_res2:2;
    unsigned short  tcp_winsize;
    unsigned short  tcp_cksum;
    unsigned short  tcp_urgent;
};


/**
 * @return 失败返回INVALID_SOCKET, 否则返回socket
 */
int open_raw_socket()
{
    int sock;
    if ((sock=socket(AF_INET, SOCK_RAW, IPPROTO_IP)) < 0)
    {
        return INVALID_SOCKET;
    }

    return sock;
}

/**
 * 设置socket为混杂模式.
 */
bool set_promisc( const char* adapterName, int socket )
{
//     struct ifreq ifr;
//     strncpy(ifr.ifr_name, adapterName, strlen(adapterName)+1);
//     if (::ioctl(socket, SIOCGIFFLAGS, &ifr) == -1)
//     {
//         return false;
//     }
//     
//     ifr.ifr_flags |= IFF_PROMISC;
//     if (ioctl(socket, SIOCSIFFLAGS, &ifr) == -1)
//     {
//         return false;
//     }

    //设置为对IP头亲自操.
    BOOL bFlag = TRUE;
    int err = setsockopt(socket,IPPROTO_IP,IP_HDRINCL,(char *)&bFlag,sizeof(bFlag));
    if(err!=0)	
        return false;

    SocketUtils::Bind(socket, SocketUtils::AddrCreate("192.168.2.10", 0, ProtocolSupport_IPv4, false));

    // dwValue为输入输出参数，为1时执行，0时取消
    
    DWORD dwValue = 1; 

    // 设置 SOCK_RAW 为SIO_RCVALL，以便接收所有的IP包。其中SIO_RCVALL
    // 的定义为： #define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
    err = ioctlsocket(socket, SIO_RCVALL, &dwValue);
    if(err!=0)	
        return false;

    return true;
}


int testPromisc()
{
    int socket = open_raw_socket();
    if (socket == INVALID_SOCKET)
    {
        printf("can't create raw socket %d\r\n", (int)WSAGetLastError());
        return 0;
    }
    if (!set_promisc("en0", socket))
    {
        printf("can't set promisc %d\r\n", (int)WSAGetLastError());
        return 0;
    }
    
    printf("set promisc ok\r\n");
    
    struct sockaddr_in from;
    struct NetPkg_IP* ip;
    struct NetPkg_TCP* tcp;
    char buf[65536] = {0};
    while (1)
    {
        socklen_t fromlen = sizeof(from);
        int n = recvfrom(socket, buf, 65536, 0, (struct sockaddr*)&from, &fromlen);

        if (n > 0)
        {
//             printf("%d\r\n", n);
        }
        else
        {
            printf("nodata\r\n", n);
            continue;
        }
        
        ip = (struct NetPkg_IP*)buf;
        if (ip->ip_protocol == 6)
        {
            tcp = (struct NetPkg_TCP*)(buf+(4*ip->ip_length));

            //String str = SocketUtils::AddrToStr(from.sin_addr.s_addr);
            printf("IP: %d\n", (int)from.sin_addr.s_addr);
            printf("IP header length: %d\n", (int)ip->ip_length);
            printf("IP source: %s:%d\n", SocketUtils::AddrToStr(ntohl(ip->ip_source)).c_str(), (int)ntohs(tcp->tcp_source_port));
            printf("IP dest  : %s:%d\n", SocketUtils::AddrToStr(ntohl(ip->ip_dest)).c_str(), (int)ntohs(tcp->tcp_dest_port));
            printf("protocol: %d\n", (int)ip->ip_protocol);            
            printf("\r\n");
        }
    }
}
