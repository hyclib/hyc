//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_NET_UDP_S_H_20130402104309__
#define __BP_NET_UDP_S_H_20130402104309__


void net_udp_s()
{
    UdpTransceiver* pServer = new UdpTransceiver();
    if (pServer->doServer(_T("192.168.2.3"), 20005, ProtocolSupport_IPv4))
    {
        pServer->initialize();

        // 构造发送流.
        ByteStream bufSend(false);
        bufSend += "adfdgadgdge";
        bufSend._curPos = 0;    // 从启示位置开始发送.

        // 构造发送地址.
        struct sockaddr_storage peer;

        // 发送.
        peer = SocketUtils::AddrCreate(_T("255.255.255.255"), 2001, ProtocolSupport_IPv4, false);
        HY_VERIFY( pServer->writeTo( &bufSend, &peer ) );

        // 接收.
        memset(&peer, 0, sizeof(peer)); // 从任意地址开始接收.
        ByteStream bufRecv(false);
        bufRecv.resize(1024);
        s32 recvLen = pServer->readPartFrom( (char*)bufRecv.begin(), sizeof(bufRecv)/sizeof(bufRecv[0]), &peer );
    }

    SAFE_DELETE(pServer);
}


#endif // end of define __BP_NET_UDP_S_H_20130402104309__