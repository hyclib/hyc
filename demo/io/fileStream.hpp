//
// Copyright(c) 1999-2015 hyrj. All rights reserved.
// author:       lipengxiang
// description:  
//

#ifndef __BP_FILESTREAM_HPP_20150714145615__
#define __BP_FILESTREAM_HPP_20150714145615__


void io_fileStream()
{
    /**
     * 文件流对象.
     */
    FileStream fs;

    /**
     * 打开文件.
     * 打开文件的方式: 
     *      FileStream::FM_Binary为二进制, 无此标志为字符文件读取
     *      详见 FileMode
     */
    if (!fs.open(_T("c://xxx.xml"), FileStream::FM_Binary|FileStream::FM_ReadWrite))
    {
        trace(_T("file isn't existed!"));
        return;
    }

    /**
     * 读取.
     */
    u8 buf[256];
    integer len = fs.readByte(buf, 256);        // 返回读取的字节数.

    integer curPos = fs.tell();                 // 返回当前文件流的位置.
    fs.seek(curPos+10, FileStream::SO_Begin);   // 设置当前文件流的位置为从开始位置偏移curPos+10

    len = fs.writeByte(buf, 10);    // 返回写入的字节数.
    if (-1 == len)
    {
        trace(_T("write error"));
        return;
    }

    fs.flush(); // 立即写入硬盘.
}


#endif // end of define __BP_FILESTREAM_HPP_20150714145615__
