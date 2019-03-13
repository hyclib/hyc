// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
//   the interface of common

#ifndef __BP_HYC_H_2010102403807__
#define __BP_HYC_H_2010102403807__

#include "hycPlatform.h"

// Utils
#include "utils/stringUtils.h"
#include "utils/fileUtils.h"
#include "utils/cryptUtils.h"
#include "utils/encodingUtils.h"
#include "utils/byteUtils.h"
#include "utils/identifierUtils.h"

#ifndef CN_PLATFORM_MOBILE
#   include "utils/dynLibUtils.h"
#endif
 
// IO
#include "io/keyborad.h"
#include "io/mouse.h"
#include "io/fileStream.h"
#include "io/xmlDoc.h"
#include "io/xmlNode.h"
#include "io/logListener.h"
#include "io/log.h"
#include "io/logMgr.h"
#include "io/jsonDoc.h"
#include "io/jsonObject.h"
#include "io/jsonValue.h"

// OS
#include "os/atomic.h"
#include "os/thread.h"
#include "os/conditionVariable.h"
#include "os/threadPool.h"
#include "os/timer.h"
#include "os/dateTime.h"
#include "os/process.h"
#include "os/multiSyncObject.hpp"

// Container
#include "container/array.h"
#include "container/arraySafe.h"
#include "container/fixArray.h"
#include "container/byteStream.h"
#include "container/hash.h"
#include "container/memoryPool.h"
#include "container/yunStreamType.h"
#include "container/yunStreamHelper.h"

#include "container/asyncContainer.h"

#include "common/cnObjectRef.h"
#include "common/cnObjectPtr.h"
#include "common/cnObjectPtrSafe.h"

// event
#include "event/event.h"

//--------------------------------
// Network
#include "network/netPlatform.h"

// socket
#include "network/socket/netStatus.h"
#include "network/socket/netUtils.h"
#include "network/socket/socketUtils.h"
#include "network/socket/tcpAcceptor.h"
#include "network/socket/tcpTransceiver.h"
#include "network/socket/udpTransceiver.h"

#include "network/selector2/selector2.h"
#include "network/selector2/selectSelector2.h"
#include "network/selector2/simpleServer_tcp.hpp"

#ifdef HYC_COMPILE_MODULE_NET_ASYNC
//    #include "network/socket/async/asyncTransceiver.h"
//    #include "network/selector/selector.h"
//	#include "network/selector/selectorEvent.h"
    #if defined(CN_NET_EPOLL)
        #include "network/selector/epollSelector.h"
		#include "network/socket/async/linux/epollTcpAcceptor.h"
		#include "network/socket/async/linux/epollTcpTransceiver.h"
		#include "network/socket/async/linux/epollUdpTransceiver.h"
    #elif defined(CN_NET_KQUEUE)
        #include "network/selector/kqueueSelector.h"
    #elif defined(CN_NET_IOCP)
        #include "network/selector/iocpSelector.h"
		#include "network/socket/async/win32/iocpTcpAcceptor.h"
        #include "network/socket/async/win32/iocpTcpTransceiver.h"
        #include "network/socket/async/win32/iocpUdpTransceiver.h"
    #endif
#endif // HYC_COMPILE_MODULE_NET_ASYNC

// http
#include "network/http/httpType.h"
#include "network/http/httpStatusCode.h"
#include "network/http/httpUtils.h"
#include "network/http/httpRequest.h"
#include "network/http/httpResponse.h"

// smtp
#include "network/smtp/smtpType.h"
#include "network/smtp/smtp.h"

//-----------------------------------------------
// Platform independent API

#include "_platform/platform.h"

// mobile
#include "_mobile/device.h"
#include "_mobile/camera.h"
#include "_mobile/connection.h"
#include "_mobile/contacts.h"
#include "_mobile/motion.h"



#endif // end of define __BP_HYC_H_2010102403807__
