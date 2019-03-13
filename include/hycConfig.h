//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  the configure file of compile
//

#ifndef __BP_HYCCONFIG_H_20111008104025__
#define __BP_HYCCONFIG_H_20111008104025__


//----------------------------------------------------------------------------------------------------
//*	编译选项; 注释掉不需要支持的选项
//----------------------------------------------------------------------------------------------------

// windows平台下, 如果编译为静态库, 则需添加此定义.
//#define HYC_COMPILE_STATIC_LIB

// 支持错误跟踪; (自动打开log).
#define HYC_COMPLIE_SUPPORT_TRACE

// 在计算crc32及其他类似文件计算时, buf大小.
#define HYC_COMPILE_FILE_SEGMENT_BUF_SIZE 1024*1024*1
 
//----------------------------------------------------------------------------------------------------
//* 支持的模块
//----------------------------------------------------------------------------------------------------

//-----------------------------------------------
// 支持网络.
#define HYC_COMPILE_MODULE_NET
// 支持网络异步.
#define HYC_COMPILE_MODULE_NET_ASYNC
// 支持无线.
//#define HYC_COMPILE_MODULE_NET_WLAN

// 支持外部的内存分配.
#define HYC_COMPILE_MODULE_MALLOC_EXT

//-----------------------------------------------
// io
// 支持json
#define HYC_COMPILE_MODULE_JSON
// 支持xml
#define HYC_COMPILE_MODULE_XML
// 支持log
#define HYC_COMPILE_MODULE_LOG
// 支持io设备(mouse/keyborad).
#define HYC_COMPILE_MODULE_IO_DEVICE

//-----------------------------------------------
// os
// 支持process
#define HYC_COMPILE_MODULE_PROCESS
// 支持thread
#define HYC_COMPILE_MODULE_THREAD
// 支持threadPool
#define HYC_COMPILE_MODULE_THREADPOOL
// 支持timer
#define HYC_COMPILE_MODULE_TIMER

//-----------------------------------------------
//  utils
// 支持MD5
#define HYC_COMPILE_MODULE_CRYPT_MD5
// 支持RC4
#define HYC_COMPILE_MODULE_CRYPT_RC4
// 支持SHA1
#define HYC_COMPILE_MODULE_CRYPT_SHA1
// 支持Identifier

#if defined(__i386__) || !defined(__arm__)
#define HYC_COMPILE_MODULE_IDENTIFIER
#endif

//-----------------------------------------------
// mobile
//  打开这个选项后, ios项目需加入:
//   - SystemConfigurtion.framework
//   - AddressBook.framework
//   - UIKit.framework
#define HYC_COMPILE_MODULE_MOBILE


#endif // end of define __BP_HYCCONFIG_H_20111008104025__

