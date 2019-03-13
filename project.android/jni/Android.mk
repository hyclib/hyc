LOCAL_PATH := $(call my-dir)/../..

include $(CLEAR_VARS)

LOCAL_MODULE    		:= hyc
LOCAL_MODULE_FILENAME 	:= libhyc
LOCAL_LDLIBS    		:= -llog

LOCAL_SRC_FILES := \
ext/nedmalloc/nedmalloc.cpp \
source/_android/ifaddrs.c \
source/common/cnMath.cpp \
source/common/cnString.cpp \
source/container/byteStream.cpp \
source/container/memoryAlloc.cpp \
source/io/fileStream.cpp \
source/io/jsonDoc.cpp \
source/io/jsonDocPrivat.cpp \
source/io/jsonObject.cpp \
source/io/jsonObjectPrivat.cpp \
source/io/jsonValue.cpp \
source/io/jsonValuePrivat.cpp \
source/io/log.cpp \
source/io/logListener.cpp \
source/io/logMgr.cpp \
source/io/mouse.cpp \
source/io/xmlDoc.cpp \
source/io/xmlNode.cpp \
source/io/xmlPrivateClass.cpp \
source/network/http/httpRequest.cpp \
source/network/http/httpResponse.cpp \
source/network/http/httpUtils.cpp \
source/network/selector2/selectSelector2.cpp \
source/network/smtp/smtp.cpp \
source/network/socket/netStatus.cpp \
source/network/socket/netUtils.cpp \
source/network/socket/socketUtils.cpp \
source/network/socket/tcpAcceptor.cpp \
source/network/socket/tcpTransceiver.cpp \
source/network/socket/udpTransceiver.cpp \
source/os/atomic.cpp \
source/os/conditionVariable.cpp \
source/os/dateTime.cpp \
source/os/process.cpp \
source/os/thread.cpp \
source/os/threadPool.cpp \
source/os/timer.cpp \
source/utils/byteUtils.cpp \
source/utils/cryptUtils_crc32.cpp \
source/utils/cryptUtils_md5.cpp \
source/utils/cryptUtils_rc4.cpp \
source/utils/cryptUtils_sha1.cpp \
source/utils/dynLibUtils.cpp \
source/utils/encodingUtils.cpp \
source/utils/fileUtils.cpp \
source/utils/identifierUtils.cpp \
source/utils/stringUtils.cpp \
source/_trace.cpp \
source/dllmain.cpp \
source/platform.cpp

include $(BUILD_SHARED_LIBRARY)
