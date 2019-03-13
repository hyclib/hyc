//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_NET_TCP_S_H_20130402105439__
#define __BP_NET_TCP_S_H_20130402105439__

void net_tcp_s()
{
    TcpTransceiver* pServer = new TcpTransceiver(ProtocolSupport_IPv4);
    //pServer->initialize(); 过期操作.

    char buf[255];
    int len;

    while (true) {
        len = pServer->readPart(buf, 255);
    }

    SAFE_DELETE(pServer);
}

#endif // end of define __BP_NET_TCP_S_H_20130402105439__