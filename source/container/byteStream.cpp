//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#include "../preheader.h"

#include "../../include/container/byteStream.h"
#include "../../include/container/memoryAlloc.h"
#include "../../include/container/array.h"

#include "../../include/utils/stringUtils.h"
#include "../../include/utils/cryptUtils.h"
#include "../../include/utils/byteUtils.h"
#include "../../include/utils/encodingUtils.h"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#pragma warning(disable: 4018)
#endif

#define MAX_MALLOC_SIZE 6553600

namespace hyc
{

    //----------------------------------------------------------------------------------------------------
    //*	Buffer
    //----------------------------------------------------------------------------------------------------

    //-----------------------------------------------
    ByteStream::ByteStream( u32 maxCapacity )
        : _buf(0)
        , _size(0)
        , _capacity(0)
        , _maxCapacity(maxCapacity)
        , _extBuffer(false)
    	, _position(0)
    {
        _curPos = begin();
    }

    //-----------------------------------------------
    ByteStream::ByteStream( Byte* buffer, u32 size )
        : _buf(buffer)
        , _size(size)
        , _capacity(size)
        , _maxCapacity(size)
        , _extBuffer(true)
    	, _position(0)
    {
        _curPos = begin();
    }

    //-----------------------------------------------
    ByteStream::~ByteStream()
    {
        if (_buf && !_extBuffer)   MemoryAlloc::Free(_buf);
    }

    //-----------------------------------------------
    void ByteStream::swapBuffer( ByteStream& other )
    {
        Math::Swap(_buf, other._buf);
        Math::Swap(_size, other._size);
        Math::Swap(_capacity, other._capacity);
        Math::Swap(_curPos, other._curPos);
        Math::Swap(_position, other._position);
        Math::Swap(_extBuffer, other._extBuffer);
        Math::Swap(_maxCapacity, other._maxCapacity);
    }

    //-----------------------------------------------
    Byte& ByteStream::getAt( u32 n )
    {
        assert(n < _size);
        return _buf[n];
    }

    //-----------------------------------------------
    const Byte& ByteStream::getAt( u32 n ) const
    {
        assert(n < _size);
        return _buf[n];
    }

    //-----------------------------------------------
    void ByteStream::operator+=( const char* pStr )
    {
        //PRIVATE_CLASS_GET(p);
        if (pStr)
        {
            add( (Byte*)pStr, (u32)StringUtils::GetByteSize(pStr) );
        }
    }

    //-----------------------------------------------
    void ByteStream::operator+=( const cString& str )
    {
        //PRIVATE_CLASS_GET(p);
        add( (Byte*)str.c_str(), (u32)StringUtils::GetByteSize(str.c_str()) );
    }

    //-----------------------------------------------
    Byte* ByteStream::begin()
    {
        return _buf;
    }

    //-----------------------------------------------
    const Byte* ByteStream::begin() const
    {
        return _buf;
    }

    //-----------------------------------------------
    Byte* ByteStream::end()
    {
        return _buf + _size;
    }

    //-----------------------------------------------
    const Byte* ByteStream::end() const
    {
        return _buf + _size;
    }

    //-----------------------------------------------
    u32 ByteStream::size() const
    {
        return _size;
    }

    //-----------------------------------------------
    hyc::u32 ByteStream::capacity() const
    {
        return _capacity;
    }

    //-----------------------------------------------
    bool ByteStream::empty() const
    {
        return !_size;
    }

    //-----------------------------------------------
    void ByteStream::clear()
    {
        _size = 0;
        _position = 0;

		// add by htz
		_curPos = _buf;
    }

    //-----------------------------------------------
    void ByteStream::resize( u32 n )
    {
        if (n < _size && _position > n)
            _position = 0;

        if(n == 0)
        {
            clear();
        }
        else if(n > _capacity)
        {
            reserve(n); 
        }
        _size = Math::Min(n, _capacity);
    }

    //-----------------------------------------------
    void ByteStream::push_back( Byte v )
    {
        if (_extBuffer)
        {
            error_all(_T("ByteStream::push_back can't be called in _extBuffer"));
            throw ExceptionType_Forbid;
        }

        resize(_size + 1);
        _buf[_size - 1] = v;
    }

    //-----------------------------------------------
    void ByteStream::reserve( u32 n )
    {
        if(n > _capacity)
        {
            _capacity = Math::Max(n, Math::Min( (u32)(1.5f * _capacity), _maxCapacity));
            _capacity = Math::Max(static_cast<u32>(240), _capacity);
            _capacity = Math::Min(MAX_MALLOC_SIZE, _capacity);

            if (_extBuffer)
            {
                error_all(_T("ByteStream::reserve can't be called in _extBuffer"));
                throw ExceptionType_Forbid;
            }
        }
        else
        {
            return;
        }

        if(_buf)
        {
            _buf = reinterpret_cast<Byte*>(MemoryAlloc::Realloc(_buf, _capacity));
            ZeroMemory(_buf+_size, _capacity-_size);
        }
        else
        {
            _buf = reinterpret_cast<Byte*>(MemoryAlloc::Malloc(_capacity));
            ZeroMemory(_buf, _capacity);
        }

        if(!_buf)
        {
            error_all(_T("Error Buffer Malloc!"));
            throw ExceptionType_Error;
            return;
        }
    }

    //-----------------------------------------------
    hyc::s32 ByteStream::compare( u32 off, u32 count, const Byte* data ) const
    {
        for (u32 i = 0; i < count; i++)
        {
            if (off + i >= size())
                return -1;

            const Byte& b = (*this)[off+i];
            if (b != data[i])
                return b > data[i] ? 1 : -1;
        }

        return 0;
    }

    //-----------------------------------------------
    void ByteStream::add( const void* pAppendBuf, u32 n )
    {
        if (_extBuffer)
        {
            error_all(_T("ByteStream::add can't be called in _extBuffer"));
            throw ExceptionType_Forbid;
        }

        if (pAppendBuf && n > 0)
        {
            s32 si = size();
            resize( si + n );
            if (si + n == size())
            {
                memcpy( begin()+si, pAppendBuf, n );
                assert(size() <= _capacity && size() == si+n);
            }
        } // if
    }

    //-----------------------------------------------
    void ByteStream::write7BitValue( u32 v )
    {
        // get the 7 bit value size.
        // 7 bit value use 7 bit to present a value.
        u8 shift = (Get7BitValueSize(v)-1)*7;   // bits.

//         bool max = false;
//         if (shift < 28) {   // use 64 value.
//             if (shift >= 21) {
//                 shift = 22;
//                 max = true;
//             }
//         }

        while(shift>=7) {
            writeU8(0x80 | ((v>>shift)&0x7F));
            shift -= 7;
        }
        writeU8(/*max ? v&0xFF : */v&0x7F);
    }

    //-----------------------------------------------
    void ByteStream::write7BitLongValue( u64 value )
    {
        // get the 7 bit value size.
        // 7 bit value use 7 bit to present a value.
        u8 shift = (Get7BitValueSize(value)-1)*7;   // bits.

        bool max = shift>=63; // Can give 10 bytes!.
        if(max)
            ++shift;

        while(shift>=7) {
            writeU8(0x80 | ((value>>shift)&0x7F));
            shift -= 7;
        }
        writeU8(max ? value&0xFF : value&0x7F);
    }

    //-----------------------------------------------
    hyc::u32 ByteStream::read7BitValue() const
    {
//         u8 n = 0;
//         u8 b = readU8();
//         u32 result = 0;
//         while ((b&0x80) && n < 3) {
//             result <<= 7;
//             result |= (b&0x7F);
//             b = readU8();
//             ++n;
//         }
// 
//         result <<= ((n==3) ? 8 : 7); // Use all 8 bits from the 4th Byte.
//         result |= b;
//         return result;
        return (u32)read7BitLongValue();
    }

    //-----------------------------------------------
    hyc::u64 ByteStream::read7BitLongValue() const
    {
        u8 n = 0;
        u8 b = readU8();
        u64 result = 0;
        while ((b&0x80) && n < 8) {
            result <<= 7;
            result |= (b&0x7F);
            b = readU8();
            ++n;
        }
        result <<= ((n<8) ? 7 : 8); // Use all 8 bits from the 4th Byte.
        result |= b;
        return result;

    }

    //-----------------------------------------------
    hyc::u8 ByteStream::Get7BitValueSize( u64 value )
    {
        u64 limit = 0x80;
        u8 result=1;
        while(value>=limit) {
            limit<<=7;
            ++result;
        }
        return result;
    }

    //-----------------------------------------------
    void ByteStream::writeStringPure( const String& v )
    {
#if defined(UNICODE) || defined(_UNICODE)
        cString str; StringUtils::Wide2narrow(v, str);
        writeRaw((Byte*)str.c_str(), str.size());
#else
        writeRaw((Byte*)v.c_str(), StringUtils::GetByteSize(v.c_str()));
#endif
    }

    //-----------------------------------------------
    bool ByteStream::readStringPure( OUT String& str, u32 sizeStr ) const
    {
        if (sizeStr+getPosition() >= size())
            return false;

        str.clear();

#if defined(UNICODE) || defined(_UNICODE)
        cString cstr;
        cstr.resize(sizeStr, '0');
        assert(cstr.size() == sizeStr);
        readRaw((Byte*)cstr.c_str(), sizeStr);

        StringUtils::Narrow2wide(cstr, str);
#else
        str.resize(sizeStr, '0');
        assert(str.size() == sizeStr);
        readRaw((Byte*)str.c_str(), sizeStr);
#endif


#ifdef _DEBUG
        str.pDebugStr = str.c_str();
#endif // _DEBUG

        return true;
    }

    //-----------------------------------------------
    hyc::u32 ByteStream::find( Byte c, u32 off /*= 0*/ ) const
    {
        for (u32 i = off; i < size(); i++)
        {
            if ((*this)[i] == c)
                return i;
        }

        return npos;
    }

    //-----------------------------------------------
    hyc::u32 ByteStream::find_last_of( Byte c, u32 off /*= npos*/ ) const
    {
        if (empty())
            return npos;

        for (s32 i = size()-1; i >= 0; i--)
        {
            if ((*this)[i] == c)
                return i;
        }

        return npos;
    }

    //-----------------------------------------------
    void ByteStream::writeStringNative( const String& v )
    {
        if (v.empty()) {
            writeU16(0);
        } else {
#if defined(UNICODE) || defined(_UNICODE)
            cString str; StringUtils::Wide2narrow(v, str);
            writeU16( str.size() );
            writeRaw((Byte*)str.c_str(), str.size());
#else
            writeU16( v.size() );
            writeRaw((Byte*)v.c_str(), StringUtils::GetByteSize(v.c_str()));
#endif
            writeU8(0);
        }
    }

    //-----------------------------------------------
    bool ByteStream::readStringNative( OUT String& str ) const
    {
        str.clear();

        u16 l = readU16();
        if (0 != l)
        {
            u32 p = getPosition()+l;
            if (p+1 <= size() && getAt(getPosition()+l) == 0)
            {
                readStringPure(str, l);
                incPosition(1);
                return true;
            }

            return false;
        }

        return true;
    }

    //-----------------------------------------------
    void ByteStream::writeStringUtf8( const String& v )
    {
        if (v.size() == 0)
        {
            writeU16(0);
        }
        else
        {
#if defined(_UNICODE) || defined(UNICODE)
            utf8* strUtf8 = (utf8*)MemoryAlloc::Malloc(v.size()*4);
            u16 convSize = (u16)EncodingUtils::StrToUTF8(v.c_str(), v.size(), strUtf8, v.size()*4);
            if (convSize == 0)
            {
                MemoryAlloc::Free(strUtf8);
                trace(_T("writeStirng utf8 error"));
                return;
            }

            writeU16(convSize);
            writeRaw(strUtf8, convSize);
            MemoryAlloc::Free(strUtf8);
#else
            if (CN_IS_UTF8_CSTRING)
            {
                writeU16(v.size());
                writeRaw((Byte*)v.c_str(), v.size());
            }
            else
            {
                utf8* strUtf8 = (utf8*)MemoryAlloc::Malloc(v.size()*2);
                u16 convSize = (u16)EncodingUtils::StrToUTF8(v.c_str(), v.size(), strUtf8, v.size()*2);
                if (convSize == 0)
                {
                    MemoryAlloc::Free(strUtf8);
                    trace(_T("writeStirng utf8 error"));
                    return;
                }

                writeU16(convSize);
                writeRaw((Byte*)strUtf8, convSize);
                MemoryAlloc::Free(strUtf8);
            }
#endif
        } // if..else
    }

    //-----------------------------------------------
    bool ByteStream::readStringUtf8( OUT String& a ) const
    {
        a.clear();

        if (getPosition() >= this->size())
            return false;

        u16 size = readU16();
        if (size == 0)
            return true;
        else if (size+getPosition() > this->size())
            return false;


#if defined(_UNICODE) || defined(UNICODE)
        tchar* strbuf = (tchar*)MemoryAlloc::Malloc((size+1)*sizeof(tchar));
        u16 _0pos = EncodingUtils::UTF8ToStr(current(), size, strbuf, size);
        strbuf[_0pos] = _T('\0');
        a = strbuf;
        MemoryAlloc::Free(strbuf);
#else
        if (CN_IS_UTF8_CSTRING)
        {
            a.append((tchar*)(current()), size);
        }
        else
        {
            tchar* strbuf = (tchar*)MemoryAlloc::Malloc((size+1)*sizeof(tchar));
            u16 _0pos = EncodingUtils::UTF8ToStr(current(), size, strbuf, size);
            strbuf[_0pos] = _T('\0');
            a = strbuf;
            MemoryAlloc::Free(strbuf);
        }
#endif
        
        _position += size;
        return true;
    }

} // end namespace hyc
