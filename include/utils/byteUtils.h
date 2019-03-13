////////////////////////////////////////////////////////
// Copyright(c) 1986-2100, TT, All Rights Reserved
// Author:  Hu Tianzhi
// Created: 2011-3-17 11:05:58
// Describe: 字节操作
////////////////////////////////////////////////////////

#ifndef	_Tian_2011317110558_H_
#define	_Tian_2011317110558_H_

namespace hyc
{
    //----------------------------------------------------------------------------------------------------
    //!	位工具, 可以对位进行操作
    //----------------------------------------------------------------------------------------------------
    class HYC_API ByteUtils
    { 
    public:
        //! 读取指定数据中的第n位(1/0), 返回其他值表示错误
        //!  n从低位开始
        static u8   ReadBit( const u8& srcData, u8 n );
        static u8   ReadBit( const u16& srcData, u8 n );
        static u8   ReadBit( const uinteger& srcData, u8 n );

        //! 写指定数据中的第n位
        //!  n从低位开始
        static void WriteBit( u8& destData, u8 n, u8 value );
        static void WriteBit( u16& destData, u8 n, u8 value );
        static void WriteBit( uinteger& destData, u8 n, u8 value );

        //! 读取第n个字节
        //!  n从低位开始
        static Byte ReadByte( const u16& srcData, u8 n );
        static Byte ReadByte( const uinteger& srcData, u8 n );

        //! 写第n个字节值
        //!  n从低位开始
        static void WriteByte( u16& destData, u8 n, Byte value );
        static void WriteByte( uinteger& destData, u8 n, Byte value );

        //! 大尾小尾转换.
        inline static bool  Ntoh( const bool& v ) { return v;  }
        inline static bool  Hton( const bool& v ) { return v;  }
        inline static u8  Ntoh( const u8& v ) { return v;  }
        inline static u8  Hton( const u8& v ) { return v;  }
        inline static s8  Ntoh( const s8& v ) { return v;  }
        inline static s8  Hton( const s8& v ) { return v;  }

        inline static f32 Ntoh( const f32& v ) { return v;  }
        inline static f32 Hton( const f32& v ) { return v;  }
        inline static f64 Ntoh( const f64& v ) { return v;  }
        inline static f64 Hton( const f64& v ) { return v;  }

        inline static u16 Ntoh( const u16& v ) { return ntohs(v); }
        inline static u16 Hton( const u16& v ) { return htons(v); }
        inline static s16 Ntoh( const s16& v ) { return ntohs(v); }
        inline static s16 Hton( const s16& v ) { return htons(v); }
        inline static u32 Ntoh( const u32& v ) { return ntohl(v); }
        inline static u32 Hton( const u32& v ) { return htonl(v); }
        inline static s32 Ntoh( const s32& v ) { return ntohl(v); }
        inline static s32 Hton( const s32& v ) { return htonl(v); }
        inline static s64 Ntoh( const s64& v ) { return Ntoh((u64)v);  }
        inline static s64 Hton( const s64& v ) { return Hton((u64)v); }
        inline static u64 Ntoh( const u64& v ) { return Hton(v);  }
        inline static u64 Hton( const u64& v ) {
            if (CN_IS_BIG_ENDIAN) { return v; }
            else { 
                u64 r;
                Byte* p = ((Byte*)&v);
                Byte* pr = ((Byte*)&r);
                pr[0] = p[7]; pr[1] = p[6]; pr[2] = p[5]; pr[3] = p[4]; pr[4] = p[3]; pr[5] = p[2]; pr[6] = p[1]; pr[7] = p[0];
                return r;
            } 
        }


    };
}

#endif // _Tian_2011317110558_H_
