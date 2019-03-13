//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_NET_UDP_C_H_20130402105106__
#define __BP_NET_UDP_C_H_20130402105106__

void net_udp_c()
{
    UdpTransceiver* pClient = new UdpTransceiver();
    pClient->doClient(_T("127.0.0.1"), port, ProtocolSupport_IPv4);
    {
        pClient->initialize();

        // 构造发送流.
        ByteStream bufSend(false);
        bufSend.add(_T("aaaaa1七"), 8);
        bufSend._curPos = 0;    // 从启示位置开始发送.

        // 接收流.
        char bufRecv[255];
        int len;

        // 发送.
        HY_VERIFY( pClient->write( &bufSend ) );

        // 接收.
        len = pClient->readPart( bufRecv, sizeof(bufRecv)/sizeof(bufRecv[0]) );
    }

    SAFE_DELETE(pClient);
}


#endif // end of define __BP_NET_UDP_C_H_20130402105106__