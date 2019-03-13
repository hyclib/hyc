
#include "../preheader.h"


#include "../../include/utils/byteUtils.h"

namespace hyc
{
    //-----------------------------------------------
	template<class T>
	inline u8 COMMON_GET_BIT(const T& srcData, u8 n)
	{
		u8 mov = sizeof(T)*8-1;
		if ((n) > mov )  {assert(0); return -1;}
		return (u8)( (T)((srcData)<<(mov-(n))) >> (mov) );
	}

    //-----------------------------------------------
	template<class T>
	inline void COMMON_SET_BIT(T& dest, u8 n, u8 v)
    {
		u8 mov = sizeof(T)*8-1;
		if (n > mov)  {assert(0);return;}
    	T vv = 1;
    	vv <<= n;
    	if (v){dest |= vv;}else{vv = ~vv; dest &= vv;}
    }

    //-----------------------------------------------
	template<class T>
	inline void COMMON_SET_BYTE(T& dest, u8 n, Byte v)
	{
	    if ((n) >= sizeof(T)) {assert(0);return;}
	    T x = 0xff;
	    x <<= ((n)*8);
	    x = ~x;
	    dest &= x;
	    x = (u16)(v);
	    x <<= ((n)*8);
	    dest |= x;
	}


    //-----------------------------------------------
    u8 ByteUtils::ReadBit( const u8& srcData, u8 n )
    {
        return COMMON_GET_BIT(srcData, n);
    }

    //-----------------------------------------------
    u8 ByteUtils::ReadBit( const u16& srcData, u8 n )
    {
        return COMMON_GET_BIT(srcData, n);
    }

    //-----------------------------------------------
    u8 ByteUtils::ReadBit( const uinteger& srcData, u8 n )
    {
        return COMMON_GET_BIT(srcData, n);
    }

    //-----------------------------------------------
    void ByteUtils::WriteBit( u8& destData, u8 n, u8 value )
    {
        COMMON_SET_BIT(destData, n, value);
    }

    //-----------------------------------------------
    void ByteUtils::WriteBit( u16& destData, u8 n, u8 value )
    {
        COMMON_SET_BIT(destData, n, value);
    }

    //-----------------------------------------------
    void ByteUtils::WriteBit( uinteger& destData, u8 n, u8 value )
    {
        COMMON_SET_BIT(destData, n, value);
    }



    //-----------------------------------------------
    Byte ByteUtils::ReadByte( const u16& srcData, u8 n )
    {
        if (n > sizeof(u16))  {assert(0);return 0;}
        u8 m = 0;
        if (0==n)
        {
            m = ((sizeof(u16)-1)*8);
            return (Byte) ( ((u16) ((srcData)<<m) ) >> m );
        }
        else
        {
            m = ((sizeof(u16)-1)*8);
            return (Byte)( srcData >> m );
        }
    }

    //-----------------------------------------------
    Byte ByteUtils::ReadByte( const uinteger& srcData, u8 n )
    {
        if (n > sizeof(uinteger))  {assert(0);return 0;}
        u8 m = 0;
        u8 m2 = ((sizeof(uinteger)-1)*8);
        if (0==n)
        {
            return (Byte) ( ((uinteger) ((srcData)<<m2) ) >> m2 );
        }
        else if (1==n)
        {
            m = ((sizeof(uinteger)-2)*8);
            return (Byte) ( ((uinteger) ((srcData)<<m) ) >> m2 );
        }
        else if (2==n)
        {
            m = ((sizeof(uinteger)-3)*8);
            return (Byte) ( ((uinteger) ((srcData)<<m) ) >> m2 );
        }
        else
        {
            return (Byte)( srcData >> m2 );
        }
   }

    //-----------------------------------------------
    void ByteUtils::WriteByte( u16& destData, u8 n, Byte value )
    {
       COMMON_SET_BYTE(destData, n, value);
    }

    //-----------------------------------------------
    void ByteUtils::WriteByte( uinteger& destData, u8 n, Byte value )
    {
        COMMON_SET_BYTE(destData, n, value);
    }
}
