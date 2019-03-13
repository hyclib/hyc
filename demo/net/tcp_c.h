//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_NET_TCP_C_H_20130402105433__
#define __BP_NET_TCP_C_H_20130402105433__


inline void net_tcp_c_output(SOCKET s)
{
    int bFlag;
    int len;

    // SO_DONTROUTE.
    len = sizeof(bFlag);
    if(getsockopt(s, SOL_SOCKET, SO_DONTROUTE, (char*)&bFlag, &len) == 0)
        print_all_line(_T("SO_DONTROUTE %d"), (int)bFlag);
    else
        print_all_line(_T("SO_DONTROUTE err"));

    // SO_KEEPALIVE.
    len = sizeof(bFlag);
    if(getsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*)&bFlag, &len) == 0)
        print_all_line(_T("SO_KEEPALIVE %d"), (int)bFlag);
    else
        print_all_line(_T("SO_KEEPALIVE err"));

    // SO_LINGER.
    struct linger FAR lin;
    len = sizeof(lin);
    if(getsockopt(s, SOL_SOCKET, SO_LINGER, (char*)&lin, &len) == 0)
        print_all_line(_T("SO_LINGER %d %d"), (int)lin.l_onoff, (int)lin.l_linger);
    else
        print_all_line(_T("SO_LINGER err"));

    // SO_OOBINLINE.
    len = sizeof(bFlag);
    if(getsockopt(s, SOL_SOCKET, SO_OOBINLINE, (char*)&bFlag, &len) == 0)
        print_all_line(_T("SO_OOBINLINE %d"), (int)bFlag);
    else
        print_all_line(_T("SO_OOBINLINE err"));

    // SO_RCVBUF.
    len = sizeof(bFlag);
    if(getsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&bFlag, &len) == 0)
        print_all_line(_T("SO_RCVBUF %d"), (int)bFlag);
    else
        print_all_line(_T("SO_RCVBUF err"));

    // SO_REUSEADDR.
    len = sizeof(bFlag);
    if(getsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&bFlag, &len) == 0)
        print_all_line(_T("SO_REUSEADDR %d"), (int)bFlag);
    else
        print_all_line(_T("SO_REUSEADDR err"));

    // SO_SNDBUF.
    len = sizeof(bFlag);
    if(getsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&bFlag, &len) == 0)
        print_all_line(_T("SO_SNDBUF %d"), (int)bFlag);
    else
        print_all_line(_T("SO_SNDBUF err"));

    // SO_TYPE.
    len = sizeof(bFlag);
    if(getsockopt(s, SOL_SOCKET, SO_TYPE, (char*)&bFlag, &len) == 0)
        print_all_line(_T("SO_TYPE %d"), (int)bFlag);
    else
        print_all_line(_T("SO_TYPE err"));

    // SO_TYPE.
    len = sizeof(bFlag);
    if(getsockopt(s, SOL_SOCKET, SO_TYPE, (char*)&bFlag, &len) == 0)
        print_all_line(_T("SO_TYPE %d"), (int)bFlag);
    else
        print_all_line(_T("SO_TYPE err"));
}

void net_tcp_c()
{
    TcpTransceiver* pClient = new TcpTransceiver(ProtocolSupport_IPv4);

    struct sockaddr_storage peer;
    peer = SocketUtils::AddrCreate(_T("255.255.255.255"), 2001, ProtocolSupport_IPv4, false);
    if (ConnectResult_Ok == pClient->connect( peer ))
    {
        //pClient->initialize(); 过期操作.

        //-----------------------------------------------
        // @brief send.
        //-----------------------------------------------
        ByteStream buf;
        buf.add(_T("1234657"), 7);
        pClient->write(&buf);

        //-----------------------------------------------
        // @brief recv use buf.
        //-----------------------------------------------
        buf._curPos = NULL; // recv buf write to [_curPos, end()).
        if (!pClient->read(&buf))
        {
            if (pClient->isClosed())
            {
                // error.
            }
            else
            {
                // timeout.
            }
        }
        else
        {
            // read ok.
        }


        //-----------------------------------------------
        // @brief recv use char[].
        //-----------------------------------------------
        char bufs[255];
        while (1)
        {
            integer n = pClient->readPart(bufs, 255);
            if (n == 0)
            {
                // disconnect.
            }
            else if (n > 0)
            {
                // ok.
            }
            else
            {
                s32 err = NetStatus::ErrorCode();

                /////////
                // no err.
                if(NetStatus::IsInterrupted(err))
                {
                    continue;
                }

                /////////
                // timeout.
                if(NetStatus::IsWouldBlock(err))
                {
                    if (SocketUtils::GetRevTimeout(socket()) != -1)
                    {
                        // timeout.
                    }

                    continue;
                }
                if(NetStatus::IsTimeout(err))
                {
                    // timeout.
                }

                /////////
                // disconnect.
                if(NetStatus::IsConnectLost(err))
                {
                    pClient->close();
                    return false;
                }

                pClient->close();
                return false;
            }
        } // while.
       
    }

    SAFE_DELETE(pClient);
}


#endif // end of define __BP_NET_TCP_C_H_20130402105433__