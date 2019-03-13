//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_NET_SIMSERVER_H_20130402110236__
#define __BP_NET_SIMSERVER_H_20130402110236__

class SimpleServerDelegate
{
public:
    bool onConnect( TransceiverPtr newConnection ) { return true; }
    void onDisconnect( TransceiverPtr conn )   {}

    // 普通接收.
    void onMsg( TransceiverPtr ep )  { 
        char buf[255]; 
//         s32 len = ep->readPart(buf, 255);
//         if (len <= 0)
//             ep->close();
    }

    // 非阻塞接收.
    void onMsgUnBlock( TransceiverPtr ep )  { 
        char buf[255]; 
        SocketUtils::SetBlock(ep->socket(), false);

        while (true)
        {
            s32 len = ep->readPart(buf, 255);
            if (len <= 0)
            {
                if (NetStatus::IsWouldBlock(NetStatus::ErrorCode()))
                {
                    break;
                }
                else
                {
                    ep->close();
                    break;
                }
            }
        }
    }
    
};

void net_simple_server_tcp()
{
    SimpleServerDelegate& delegate = *(new SimpleServerDelegate);

    SimpleServer_tcp* pServer = new SimpleServer_tcp(new SelectSelector2, 100);
    SimpleServer_tcp& server = *pServer;
    server.event_connected().set(&delegate, &SimpleServerDelegate::onConnect);
    server.event_msg()      .set(&delegate, &SimpleServerDelegate::onMsg);
    server.event_disconnect().set(&delegate, &SimpleServerDelegate::onDisconnect);

    if (!server.go(_T("127.0.0.1"), 20001, ProtocolSupport_IPv4))
    {
        error_all(_T("run server error"));
        return;
    }

    trace_all(_T("ok"));



    Sleep(2000);
    server.stop();
}


#endif // end of define __BP_NET_SIMSERVER_H_20130402110236__