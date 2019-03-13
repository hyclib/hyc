//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hy demo
// author:       lipengxiang
// description:  
//

#ifndef __BP_NET_HTTP_H_20130402110236__
#define __BP_NET_HTTP_H_20130402110236__

void net_http()
{
    HttpRequest req;
    HttpResponse rsp;
    HttpResult re;

    req.setHost(_T("www.baidu.com"), 80, ProtocolSupport_IPv4);
    req.setRequestLine( HttpMethod_Get, _T("/") );

    re = req.request( NULL, 0, &rsp );

    // 超时.
    if (re > HttpResult_timeout)
    {   
    }
    // 错误.
    else if (re >= HttpResult_error) 
    {    
    }
    // 有内容需接收.
    else if (re == HttpResult_ok_haveContent) 
    {
        char buf[255]
        u32 readLen;

        while (re == HttpResult_ok_haveContent)
        {
            re = req.requestContent((Byte*)buf, sizeof(buf), readLen);
        }
    }
    // ok.
    else
    {

    }
}


#endif // end of define __BP_NET_HTTP_H_20130402110236__