//
// Copyright(c) 1999-2015 hyrj. All rights reserved.
// author:       lipengxiang
// description:  
//

#ifndef __BP_HYCEXCEPTION_H_20150513140456__
#define __BP_HYCEXCEPTION_H_20150513140456__


namespace hyc
{
    /**
     * @brief 异常枚举.
     *        不影响程序正常运行的结果不允许使用异常, 异常用于内存越界/空指针访问/系统错误等.
     */

    /**
     * 运行错误.表明是系统内部的错误.
     */
#define ExceptionType_Error 0

    /**
     * 参数错误.
     */
#define ExceptionType_ErrorParam 1

    /**
     * 空指针.
     */
#define ExceptionType_NullPointer 2

    /**
     * 未实现.
     */
#define ExceptionType_UnImplement 3

    /**
     * 禁止执行. 表明此刻不允许执行此语句.
     */
#define ExceptionType_Forbid 4

    /**
     * 越界.
     */
#define ExceptionType_OutOfRange 5

} // end namespace hyc

#endif // end of define __BP_HYCEXCEPTION_H_20150513140456__
