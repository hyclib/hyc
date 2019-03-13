//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// yun network libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_YUNSTREAMHELPER_H_20121003155537__
#define __BP_YUNSTREAMHELPER_H_20121003155537__


namespace hyc
{

#define YUN_CONTENT_MAX_LENGTH 65535-28   //! 最大长度.

    //----------------------------------------------------------------------------------------------------
    //!	用于数组流的助手.
    //! 用于YunStreamType的元素的操作.
    //----------------------------------------------------------------------------------------------------
    class HYC_API YunStreamHelper
    {
    public:
        /**
         * @brief 从buf的offPos位置读取出结构, 可用的流大小到endPos为止, endPos的位置不是流.
         */
        static bool get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT ByteStream& a );

        static bool get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT String& a );

        static bool get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT ArrayFind<String>& arr );

        template<class T>
        static bool get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT T& a )   { if (endPos-offPos < sizeof(T)) return false; memcpy(&a, (buf.begin()+offPos), sizeof(T)); a = ByteUtils::Ntoh(a); offPos+=sizeof(T); return true; }

        template<class T>
        static bool get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT ArrayFind<T>& arr );

        /**
         * @brief 将数据写入buf.
         */
        static bool set( OUT ByteStream& buf, const Byte* a, u32 size );
        static bool set( OUT ByteStream& buf, const ByteStream& a )     { return set(buf, a.begin(), a.size()); }

        static bool set( OUT ByteStream& buf, const tchar* a, u32 size );
        static bool set( OUT ByteStream& buf, const String& str )       { return set(buf, str.c_str(), (u32)StringUtils::GetByteSize(str.c_str())); }

        static bool set( OUT ByteStream& buf, const ArrayFind<String>& arr );

        template<class T>
        static bool set( OUT ByteStream& buf, const T& a )               { if ( buf.size()+sizeof(T) > YUN_CONTENT_MAX_LENGTH) return false; T aa = ByteUtils::Hton(a); buf.add(&aa, sizeof(T)); return true; }

        template<class T>
        static bool set( OUT ByteStream& buf, const ArrayFind<T>& arr )  { if ( buf.size()+sizeof(u16)+sizeof(T)*arr.size() > YUN_CONTENT_MAX_LENGTH) return false; u16 len = arr.size(); len = ByteUtils::Hton(len); buf.add(&len, sizeof(u16)); T aa; for (u32 i = 0; i < arr.size(); i++) { aa = ByteUtils::Hton(arr[i]); buf.add(&aa, sizeof(T)); } return true; } 
    };

    //-----------------------------------------------
    inline bool YunStreamHelper::set( OUT ByteStream& buf, const Byte* a, u32 size )
    {
        if ( buf.size()+sizeof(u16) + size > YUN_CONTENT_MAX_LENGTH)
            return false;

        // len
        u16 len = ByteUtils::Hton((u16)size);
        buf.add( &len, sizeof(u16) );
        // v
        buf.add( a, size );

        return true;
    }

    //-----------------------------------------------
    inline bool YunStreamHelper::set( OUT ByteStream& buf, const tchar* a, u32 size )
    {
        if ( buf.size()+sizeof(u16) > YUN_CONTENT_MAX_LENGTH)
            return false;

        if (size == 0)
        {
            u16 len = 0;
            len = ByteUtils::Hton(len);
            buf.add(&len, sizeof(len));
            return true;
        }
        else
        {
#if defined(_UNICODE) || defined(UNICODE)
            utf8* strUtf8 = (utf8*)MemoryAlloc::Malloc(size*4);
            u16 convSize = (u16)EncodingUtils::StrToUTF8(a, size, strUtf8, size*4);
            if (convSize == 0)
            {
                MemoryAlloc::Free(strUtf8);
                return false;
            }

            if ( buf.size()+convSize+sizeof(u16) > YUN_CONTENT_MAX_LENGTH)
            {
                MemoryAlloc::Free(strUtf8);
                return false;
            }

            u16 len = ByteUtils::Hton(convSize);
            buf.add(&len, sizeof(len));
            buf.add(strUtf8, convSize);
#else
            if (CN_IS_UTF8_CSTRING)
            {
                if ( buf.size()+size+sizeof(u16) > YUN_CONTENT_MAX_LENGTH)
                {
                    return false;
                }

                u16 len = ByteUtils::Hton((u16)size);
                buf.add(&len, sizeof(len));
                buf.add(a, size);
            }
            else
            {
                utf8* strUtf8 = (utf8*)MemoryAlloc::Malloc(size*2);
                u16 convSize = (u16)EncodingUtils::StrToUTF8(a, size, strUtf8, size*2);
                if (convSize == 0)
                {
                    MemoryAlloc::Free(strUtf8);
                    return false;
                }

                if ( buf.size()+convSize+sizeof(u16) > YUN_CONTENT_MAX_LENGTH)
                {
                    MemoryAlloc::Free(strUtf8);
                    return false;
                }

                u16 len = ByteUtils::Hton(convSize);
                buf.add(&len, sizeof(len));
                buf.add(strUtf8, convSize);
            }
#endif
        } // if..else

        return true;
    }

    //----------------------------------------
    inline bool YunStreamHelper::set( OUT ByteStream& buf, const ArrayFind<String>& arr )
    {
        if ( buf.size()+sizeof(u16) > YUN_CONTENT_MAX_LENGTH)
            return false;

        u16 len = arr.size(); 
        len = ByteUtils::Hton(len);
        buf.add(&len, sizeof(u16)); 

        for (u32 i = 0; i < arr.size(); i++)
        {
            if (!set(buf, arr[i]))
                return false;
        }

        return true;
    }

    //-----------------------------------------------
    inline bool YunStreamHelper::get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT ByteStream& a )
    {
        a.clear();

        if (endPos > buf.size() || offPos >= endPos)    return false;
        if (endPos-offPos < 2)  return false;

        // len.
        u16 size;
        memcpy(&size, (buf.begin()+offPos), sizeof(u16)); 
        size = ByteUtils::Ntoh(size);
        offPos += sizeof(u16);

        if (size == 0)
            return true;
        else if (endPos-offPos < size)
            return false;

        a.add((buf.begin()+offPos), size);
        offPos += size;
        return true;
    }

    //----------------------------------------
    inline bool YunStreamHelper::get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT String& a )
    {
        a.clear();

        if (endPos > buf.size() || offPos >= endPos)    return false;
        if (endPos-offPos < 2)  return false;

        // len.
        u16 size;
        memcpy(&size, (buf.begin()+offPos), sizeof(u16)); 
        size = ByteUtils::Ntoh(size);
        offPos += sizeof(u16);

        if (size == 0)
            return true;
        else if (endPos-offPos < size)
            return false;

#if defined(_UNICODE) || defined(UNICODE)
        tchar* strbuf = (tchar*)MemoryAlloc::Malloc((size+1)*sizeof(tchar));
        u16 _0pos = EncodingUtils::UTF8ToStr(buf.begin()+offPos, size, strbuf, size);
        strbuf[_0pos] = _T('\0');
        a = strbuf;
        MemoryAlloc::Free(strbuf);
#else
        if (CN_IS_UTF8_CSTRING)
        {
            a.append((tchar*)(buf.begin()+offPos), size);
        }
        else
        {
            tchar* strbuf = (tchar*)MemoryAlloc::Malloc((size+1)*sizeof(tchar));
            u16 _0pos = EncodingUtils::UTF8ToStr(buf.begin()+offPos, size, strbuf, size);
            strbuf[_0pos] = _T('\0');
            a = strbuf;
            MemoryAlloc::Free(strbuf);
        }
#endif
        offPos += size;
        return true;
    }

    //----------------------------------------
    inline bool YunStreamHelper::get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT ArrayFind<String>& arr )
    {
        arr.clear();
        if (endPos > buf.size() || offPos >= endPos)    return false;
        if (endPos-offPos < 2)  return false;

        u16 len;
        memcpy(&len, (buf.begin()+offPos), sizeof(u16)); 
        len = ByteUtils::Ntoh(len);
        offPos += sizeof(u16);

        String str;
        for (u16 i = 0; i < len; i++)
        {
            if (!get(buf, endPos, offPos, str))
                return false;

            arr.push_back(str);
        }

        return true;
    }
    
    //-----------------------------------------------
    template<class T>
    inline bool YunStreamHelper::get( const ByteStream& buf, const u32& endPos, OUT u32& offPos, OUT ArrayFind<T>& arr )
    {
        arr.clear();
        if (endPos > buf.size() || offPos >= endPos)    return false;
        if (endPos-offPos < 2)  return false;
        
        u16 len;
        memcpy(&len, (buf.begin()+offPos), sizeof(u16));
        len = ByteUtils::Ntoh(len);
        offPos += sizeof(u16);
        
        if (len * sizeof(T) > (endPos-offPos))   return false;
        
        T v;
        for (u16 i = 0; i < len; i++)
        {
            memcpy(&v, (buf.begin()+offPos), sizeof(T));
            v = ByteUtils::Ntoh(v);
            arr.push_back(v);
            offPos += sizeof(T);
        }
        return true;
    }




} // end namespace hyc

#endif // end of define __BP_YUNSTREAMHELPER_H_20121003155537__
