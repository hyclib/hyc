// Copyright(c) 1999-2010 lipengxiang. All rights reserved.
// bRAINpoint 3D Graphic Engine
//   

#include "../preheader.h"

// #include <algorithm>
// #include <string>

#include "../../include/common/cnMath.h"
#include "../../include/common/cnString.h"
#include "../../include/utils/stringUtils.h"
#include "../../include/utils/encodingUtils.h"

#include "cnStringKMP.hpp"

#if CN_PLATFORM == CN_PLATFORM_WINDOWS
#    pragma warning(disable: 4996)
#else
#include <ctype.h>
#include <wctype.h>
#endif // CN_PLATFORM == CN_PLATFROM_WINDOWS

#define MAX_CAPA    65536*10

inline static hyc::u32 _bestSize(hyc::u32 n) { n = hyc::Math::GetBestPowerOf2(n+1); return n > MAX_CAPA ? MAX_CAPA : n; }
//inline static hyc::u32 _bestSize(hyc::u32 n) { return n; }

namespace hyc
{
    const u32 cString::npos = (u32)(-1);
    const u32 wString::npos = (u32)(-1);


    //----------------------------------------------------------------------------------------------------
    //*	KMP_getNext
    //----------------------------------------------------------------------------------------------------
//     template<typename type>
//     inline void kmp_getNext(type* t, )
     

    //----------------------------------------------------------------------------------------------------
    //*	cString
    //----------------------------------------------------------------------------------------------------
//     class cString::PrivateClass : public MemoryAllocObject
//     {
//     public:
//         PrivateClass()  {}
//         PrivateClass(const char* s, u32 size) : m_str(s, size)    {}
// 
// 
// 		std::string		m_str;
//     };

    //-----------------------------------------------
    cString::cString()
        :
#ifdef DEBUG
    	    	pDebugStr(NULL),
#endif
         _pBuf(NULL)
        , _size(0)
        , _capa(0)
    {
    }

    cString::cString( const cString& str )
        : _pBuf(NULL)
        , _size(0)
        , _capa(0)
    {
        if (!str.empty())
            append(str.c_str(), str.size());
    }

    //-----------------------------------------------
    cString::cString( const char* str )
        : _pBuf(NULL)
        , _size(0)
        , _capa(0)
    {
        if (str != NULL)
            append(str, (u32)strlen(str));
    }

    //-----------------------------------------------
    cString::cString( const char* str, u32 size )
        : _pBuf(NULL)
        , _size(0)
        , _capa(0)
    {
        resize(size, 0);
        memcpy(_pBuf, str, size);

#ifdef _DEBUG
        pDebugStr = c_str();
#endif // _DEBUG
    }

    //-----------------------------------------------
    cString::~cString()
    {
        if (_pBuf)
            MemoryAlloc::Free(_pBuf);
    }

    //-----------------------------------------------
    cString& cString::operator=( const cString& str )
    {
        if (this == &str)
            return *this;

        clear();
        append(str.c_str(), str.size());

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

        return *this;
    }


    cString& cString::operator=( const char* str )
    {
        if (c_str() == str)
            return *this;

        if ( NULL == str )
            clear();
        else
        {
            clear();
            append(str, (u32)strlen(str));
        }

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

        return *this;
    }

    bool cString::operator==( const cString& str ) const
    {
        if (size() == str.size())
        {
            if (size() == 0)
                return true;
            return 0 == memcmp(c_str(), str.c_str(), size());
        }
        
        return false;
    }

    bool cString::operator==( const char* str ) const
    {
        if ( NULL == str )
        {
            return empty();
        }
        else if (size() == strlen(str))
        {
            if (size() == 0)
                return true;

            return 0 == memcmp(c_str(), str, size());
        }
        else
            return false;
    }

    bool cString::operator!=( const cString& str ) const
    {
        return !operator==(str);
    }

    bool cString::operator!=( const char* str ) const
    {
        return !((*this) == str);
    }

    cString cString::operator+( const cString& str ) const
    {
        cString s;
        s.reserve(_bestSize(size()+str.size()));
        s.append(c_str(), size());
        s.append(str.c_str(), str.size());

        return s;
    }

    cString cString::operator+( const char& c ) const
    {
        cString s;
        s.reserve(_bestSize(size()+1));
        s.append(c_str(), size());
        s.append(1, c);

        return s;
    }

    //-----------------------------------------------
    cString cString::operator+( const char* str ) const
    {
        if ( NULL == str )
        {
            return *this;
        }

        u32 l = (u32)strlen(str);
        cString s;
        s.reserve(_bestSize(size()+l));
        s.append(c_str(), size());
        s.append(str, l);
        return s;
    }

    cString& cString::operator+=( const cString& str )
    {
        append(str.c_str(), str.size());

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

        return *this;
    }

    cString& cString::operator+=( const char& c )
    {
        append(1, c);

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

        return *this;
    }

    //-----------------------------------------------
    cString& cString::operator+=( const char* str )
    {
        if ( NULL == str )
        {
            return *this;
        }
        
        append(str, (u32)strlen(str));

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

        return *this;
    }

    //----------------------------------------
    cString& cString::operator+=( const wchar_t& c )
    {
        cString temp;
        StringUtils::Wide2narrow(&c, 1, temp);

        append(temp.c_str(), temp.size());

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

        return *this;
    }

    //----------------------------------------
    cString& cString::operator+=( const wchar_t* str )
    {
        if ( NULL == str )
        {
            return *this;
        }

        cString temp;
        StringUtils::Wide2narrow(str, StringUtils::GetLength(str), temp);

        append(temp.c_str(), temp.size());

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

        return *this;
    }



    char cString::operator[]( u32 index ) const
    {
        return _pBuf[index];
    }

    //-----------------------------------------------
    char& cString::operator[]( u32 index )
    {
        return _pBuf[index];
    }

    //-----------------------------------------------
    bool cString::operator<( const cString& str ) const
    {
        return operator<(str.c_str());
    }

    //-----------------------------------------------
    bool cString::operator<( const char* str ) const
    {
        if (empty())
            return StringUtils::IsEmpty(str) ? false : true;

        if (NULL == str)
            return false;
        
        return 0 > strcmp(_pBuf, str);
    }

    //-----------------------------------------------
    bool cString::operator<=( const cString& str ) const
    {
        return operator<=(str.c_str());
    }

    //-----------------------------------------------
    bool cString::operator<=( const char* str ) const
    {
        if (empty())
            return true;
        
        if (NULL == str)
            return false;

        return 0 >= strcmp(_pBuf, str);
    }

    //-----------------------------------------------
    bool cString::operator>=( const cString& str ) const
    {
        return operator>=(str.c_str());
    }

    //-----------------------------------------------
    bool cString::operator>=( const char* str ) const
    {
        if (StringUtils::IsEmpty(str))
            return true;
        
        if (empty())
            return false;

        return 0 <= strcmp(_pBuf, str);
    }

    //-----------------------------------------------
    bool cString::operator>( const cString& str ) const
    {
        return operator>(str.c_str());
    }

    //-----------------------------------------------
    bool cString::operator>( const char* str ) const
    {
        if (StringUtils::IsEmpty(str))
            return empty() ? false : true;

        if (empty())
            return false;
        
        return 0 < strcmp(_pBuf, str);
    }

    //-----------------------------------------------
    const char& cString::at( u32 index ) const
    {
        return _pBuf[index];
    }

    //-----------------------------------------------
    void cString::append( u32 count, char c )
    {
        if (count > 0)
        {
            reserve(_bestSize(_size+count));
            memset(_pBuf+_size, c, count);
            _size+=count;
            *(_pBuf+_size) = 0;
            
#ifdef _DEBUG
            pDebugStr = (char*)c_str();
#endif // _DEBUG
        }
    }

    //-----------------------------------------------
    void cString::append( const cString& str )
    {
        if (!str.empty())
        {
            reserve(_bestSize(_size+str.size()));
            memcpy(_pBuf+_size, str.c_str(), str.size());
            _size += str.size();
            *(_pBuf+_size) = 0;
            
#ifdef _DEBUG
            pDebugStr = (char*)c_str();
#endif // _DEBUG
        }
    }

    //-----------------------------------------------
    void cString::append( const char* pstr, u32 count )
    {
        if (count > 0)
        {
            assert(pstr);
            reserve(_bestSize(_size+count));
            memcpy(_pBuf+_size, pstr, count);
            _size += count;
            *(_pBuf+_size) = 0;
            
#ifdef _DEBUG
            pDebugStr = (char*)c_str();
#endif // _DEBUG
        }
    }

    //-----------------------------------------------
    const char* cString::c_str() const
    {
        return _pBuf;
    }

    //-----------------------------------------------
    u32 cString::capacity() const
    {
        return _capa;
    }

    //-----------------------------------------------
    s32 cString::compare( u32 off, u32 count, const char* pStr ) const
    {
        return (s32)strncmp(_pBuf+off, pStr, count);
    }

    //-----------------------------------------------
    s32 cString::compare( u32 off, u32 count, const cString& str ) const
    {
        return (s32)strncmp(_pBuf+off, str.c_str(), count);
    }

    //-----------------------------------------------
    void cString::clear()
    {
        if (_pBuf)
            _pBuf[0] = 0;
        _size = 0;

#ifdef _DEBUG
        pDebugStr = "";
#endif // _DEBUG

    }

    //-----------------------------------------------
    bool cString::empty() const
    {
        return _size == 0;
    }

    //-----------------------------------------------
    void cString::erase( u32 off, u32 count /*= 1*/ )
    {
        if (count == -1)
            count = _size-off;

        if (off > _size)
            off = _size-count;

        assert(size() >= off+count);

        if (_size > off+count)
            memmove(_pBuf+off, _pBuf+off+count, (_size-off-count));
        _size -= count;
        _pBuf[_size] = 0;

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG


    }

    //-----------------------------------------------
    s32 cString::find( char c, u32 off /*= 0*/ ) const
    {
        for (u32 i = off; i < _size; i++)
        {
            if (_pBuf[i] == c)
                return i;
        }

        return npos;
    }

    //-----------------------------------------------
    s32 cString::find( const char* str, u32 off /*= 0*/ ) const
    {
        if (off >= _size)
            return npos;

        if (NULL == str)
            return npos;
        else
        {
            u32 l = (u32)strlen(str);
            if (l == 1)
                return find(str[0], off);
            else if (l > _size-off)
                return npos;
            else if (l == _size-off)
            {
                for(u32 i = 0; i < l; i++)
                {
                    if (_pBuf[i+off] != str[i])
                        return npos;
                }
                return off;
            }                
        }

        u32 index = KMP(_pBuf+off, str);
        return index == npos ? npos : index+off;
    }

    //-----------------------------------------------
    hyc::s32 cString::find_last_of( char c, u32 off /*= npos*/ ) const
    {
        if (off > _size)
            off = _size-1;
        for (s32 i = off; i >= 0; i--)
        {
            if (_pBuf[i] == c)
                return i;
        }

        return npos;
    }

    //-----------------------------------------------
    hyc::s32 cString::find_last_of( const char* str, u32 off /*= npos*/ ) const
    {
        if (NULL == str || empty())
            return npos;
        else
        {
            if (off > _size)
                off = _size-1;
            u32 l = (u32)strlen(str);
            if (l == 1)
                return find_last_of(str[0], off);
            else if (l > off+1)
                return npos;
            else if (l == off+1)
            {
                s32 j = 1;
                for(s32 i = off; i >= 0; i--, j++)
                {
                    if (_pBuf[i] != str[l-j])
                        return npos;
                }
                return off;
            }                
        }

        return KMPr(_pBuf, str, off);
    }


    //-----------------------------------------------
    void cString::vformat( const char* fmt, va_list arg_ptr )
    {
        if ( NULL == fmt )  return;
        assert(fmt != c_str());
        
        reserve( _bestSize((u32)strlen(fmt) * 2) );

        /*cString temp;
        temp.reserve(capacity());
        va_list arg_ptr;
        va_start(arg_ptr, vformat);
        vsnprintf(const_cast<char*>(temp.c_str()), temp.capacity(), vformat, arg_ptr);
        p->m_str = temp.c_str();
        va_end(arg_ptr);*/

        clear();
        cString temp;
        
        int num = 0;
        int prec = -1;
        
        char c = *fmt;
        while (0 != c) {
            
            if ('%' == c || num > 0 || prec > 0)
            {
                if ('%' == c)
                {
                    fmt++;
                    c = *fmt;
                }
                
                if (0 == c)  { error_all(_T("format error")); assert(0);return; }
                switch (c)
                {
                        //-----------------------------------------------
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    {
                        num = c-'0';
                        
                        while (true)
                        {
                            fmt++;
                            c = *fmt;
                            if (0 == c)  { error_all(_T("format error")); assert(0);return; }
                            if (!(c >= '0' && c <= '9'))
                            {
                                fmt--;
                                break;
                            }
                            else
                            {
                                num = num*10 + c-'0';
                            }
                        }
                    } break;
                        //-----------------------------------------------
                    case '.':
                    {
                        fmt++;
                        c = *fmt;
                        if (0 == c)  { error_all(_T("format error")); assert(0); return; }
                        switch (c)
                        {
                            case '0':
                            case '1':
                            case '2':
                            case '3':
                            case '4':
                            case '5':
                            case '6':
                            case '7':
                            case '8':
                            case '9':
                            {
								prec = c-'0';

								while (true)
								{
									fmt++;
									c = *fmt;
									if (0 == c)  { error_all(_T("format error")); assert(0);return; }
									if (!(c >= '0' && c <= '9'))
									{
										fmt--;
										break;
									}
									else
									{
										prec = prec*10 + c-'0';
									}
								}
							} break;
							default:
								fmt--;
								break;
						} // switch.
                    } break;
                        
                        //-----------------------------------------------
                    case '%':
						num = 0;
						prec = -1;
                        append(1, '%');
                        break;
                        //-----------------------------------------------
                    case 'c':
                    case 'C':
						num = 0;
						prec = -1;
                        append(1, va_arg(arg_ptr, int));
                        break;
                        //-----------------------------------------------
                    case 's':
                    case 'S':
						num = 0;
						prec = -1;
                        *this += va_arg(arg_ptr, const char*);
                        break;
                        //-----------------------------------------------
                    case 'x':
                    {
						num = 0;
						prec = -1;
                        int v = va_arg(arg_ptr, int);
                        EncodingUtils::ByteToHex((Byte*)&v, 1, temp);
                        *this += temp;
                    } break;
                    case 'X':
                    {
						num = 0;
						prec = -1;
                        int v = va_arg(arg_ptr, int);
                        EncodingUtils::ByteToHex((Byte*)&v, 1, temp);
                        temp.toUpper();
                        *this += temp;
                    } break;
                        //-----------------------------------------------
                    case 'd':
                    case 'D':
                    case 'i':
                    case 'I':
                        StringUtils::S32ToStr(va_arg(arg_ptr, int), temp);
                        if (num > temp.size())
                            this->append(num-temp.size(), '0');
						num = 0;
						prec = -1;
                        *this += temp;
                        break;
                        //-----------------------------------------------
                    case 'u':
                    case 'U':
                        StringUtils::U32ToStr(va_arg(arg_ptr, unsigned int), temp);
                        if (num > temp.size())
                            this->append(num-temp.size(), '0');
						num = 0;
						prec = -1;
                        *this += temp;
                        break;
                        //-----------------------------------------------
                    case 'f':
                    case 'F':
						if (prec == -1)
							prec = 6;
                        StringUtils::F64ToStr(va_arg(arg_ptr, double), temp, prec);
						if (num > temp.size()-prec-1)
							this->append(num-(temp.size()-prec-1), '0');
						num = 0;
						prec = -1;
						*this += temp;
                        break;
                        //-----------------------------------------------
                    case 'h':
                    case 'H':
                    {
                        fmt++;
                        c = *fmt;
                        if (0 == c)  { error_all(_T("format error")); assert(0); return; }
                        switch (c)
                        {
                            case 'd':
                            case 'D':
                            case 'i':
                            case 'I':
                                StringUtils::S32ToStr(va_arg(arg_ptr, int), temp);
                                if (num > temp.size())
                                    this->append(num-temp.size(), '0');
                                num = 0;
                                *this += temp;
                                break;
                            case 'u':
                            case 'U':
                                StringUtils::U32ToStr(va_arg(arg_ptr, unsigned int), temp);
                                if (num > temp.size())
                                    this->append(num-temp.size(), '0');
                                num = 0;
                                *this += temp;
                                break;
                            default:
                                error_all(_T("format error"));
                                assert(0);
                                return;
                                break;
                        }
                    } break;
                        //-----------------------------------------------
                    case 'l':
                    case 'L':
                    {
                        fmt++;
                        c = *fmt;
                        if (0 == c)  { error_all(_T("format error")); assert(0); return; }
                        switch (c)
                        {
                            case 'd':
                            case 'D':
                            case 'i':
                            case 'I':
                                StringUtils::IntegerToStr(va_arg(arg_ptr, long), temp);
                                if (num > temp.size())
                                    this->append(num-temp.size(), '0');
                                num = 0;
                                *this += temp;
                                break;
                            case 'u':
                            case 'U':
                                StringUtils::UIntegerToStr(va_arg(arg_ptr, unsigned long), temp);
                                if (num > temp.size())
                                    this->append(num-temp.size(), '0');
                                num = 0;
                                *this += temp;
                                break;
                            case 'f':
                            case 'F':
								if (prec == -1) prec = 6;
                                StringUtils::F64ToStr(va_arg(arg_ptr, double), temp, prec);
								if (num > temp.size()-prec-1)
									this->append(num-(temp.size()-prec-1), '0');
								num = 0;
								prec = -1;
								*this += temp;
                                break;
                            case 'l':
                            case 'L':
                            {
                                fmt++;
                                c = *fmt;
                                if (0 == c)  { error_all(_T("format error")); assert(0); return; }
                                switch (c)
                                {
                                    case 'd':
                                    case 'D':
                                    case 'i':
                                    case 'I':
                                        StringUtils::S64ToStr(va_arg(arg_ptr, long long), temp);
                                        if (num > temp.size())
                                            this->append(num-temp.size(), '0');
                                        num = 0;
                                        *this += temp;
                                        break;
                                    case 'u':
                                    case 'U':
                                        StringUtils::U64ToStr(va_arg(arg_ptr, unsigned long long), temp);
                                        if (num > temp.size())
                                            this->append(num-temp.size(), '0');
                                        num = 0;
                                        *this += temp;
                                        break;
                                    default:
                                        error_all(_T("format error"));
                                        assert(0);
                                        return;
                                        break;
                                }
                            } break;
                            default:
                                error_all(_T("format error"));
                                assert(0);
                                return;
                                break;
                        }
                    } break;
                        
                    default:
                        error_all(_T("format error"));
                        assert(0);
                        return;
                        break;
                }
            }
            else
            {
                append(1, c);
            } // if..else.
            
            fmt++;
            c = *fmt;
        } // while

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG
    }

    //-----------------------------------------------
    void cString::format( const char* fmt, ... )
    {
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        vformat(fmt, arg_ptr);
        va_end(arg_ptr);
    }

    //-----------------------------------------------
    void cString::insert( u32 pos, u32 count, char c )
    {
        if (pos > _size)
            pos = _size;

        reserve(_bestSize(_size+count));
        if (pos < _size)
        {
            memcpy(_pBuf+pos+count,_pBuf+pos, _size-pos);
            memset(_pBuf+pos, c, count);
        }
        else
        {
            memset(_pBuf+_size, c, count);
        }

        _size += count;

        if (_size > 0)
            _pBuf[_size] = 0;
        else if (_capa > 0)
            _pBuf[0] = 0;

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG
    }
 
    //-----------------------------------------------
    void cString::insert( u32 pos, const cString& str )
    {
        insert(pos, str.c_str());
    }

    //-----------------------------------------------
    void cString::insert( u32 pos, const char* str )
    {
        if (str)
        {
            s32 count = (u32)strlen(str);
            insert(pos, str, count);
        }
    }

    //-----------------------------------------------
    void cString::insert( u32 pos, const char* str, u32 count )
    {
        if (!str || count == 0)
            return;

        if (pos > _size)
            pos = _size;

        reserve(_bestSize(_size+count));
        if (pos < _size)
        {
            memmove(_pBuf+pos+count,_pBuf+pos, _size-pos);
            memcpy(_pBuf+pos, str, count);
        }
        else
        {
            memcpy(_pBuf+_size, str, count);
        }

        _size += count;
        if (_size > 0)
            _pBuf[_size] = 0;
        else if (_capa > 0)
            _pBuf[0] = 0;

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

    }
    //-----------------------------------------------
    void cString::resize( u32 newSize, char c )
    {
        reserve(_bestSize(newSize));

        if (newSize > _size)
        {
            memset(_pBuf+_size, c, newSize-_size);
        }

        _size = newSize;
        if (_size > 0)
            _pBuf[_size] = 0;
        else if (_capa > 0)
            _pBuf[0] = 0;

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void cString::replace( u32 startPos, u32 endPos, char oldC, char newC )
    {
        if (_size == 0)
            return;
        
        if (endPos >= _size) endPos = _size-1;
        for (u32 i = startPos; i <= endPos; i++)
        {
            if (_pBuf[i] == oldC)
                _pBuf[i] = newC;
        }

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void cString::replace( u32 startPos, u32 endPos, char oldC, const cString& newStr )
    {
        if (_size == 0)
            return;
        if (endPos >= _size) endPos = _size-1;
        
        for (u32 i = startPos; i < endPos; ++i)
        {
            if (_pBuf[i] == oldC)
            {
                if (newStr.empty())
                {
                    erase(i, 1);
                    i--;
                    endPos--;
                }
                else if (newStr.size() == 1)
                {
                    _pBuf[i] = newStr[0];
                }
                else
                {
                    _pBuf[i] = newStr[0];
                    insert(i+1, newStr.c_str()+1, newStr.size()-1);
                    i += newStr.size()-1;
                    endPos += newStr.size()-1;
                }
            }
        }

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void cString::replace( u32 startPos, u32 endPos, const cString& oldStr, char newC )
    {
        if (_size == 0)
            return;
        if (endPos >= _size) endPos = _size-1;
        
        u32 i = startPos;
        do 
        {
            i = find(oldStr.c_str(), i);
            if (npos != i && i < endPos)
            {
                _pBuf[i] = newC;
                erase(i+1, oldStr.size()-1);
                ++i;
                endPos -= oldStr.size()-1;
            }
            else
                break;
        } while (i < endPos);   // while

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void cString::replace( u32 startPos, u32 endPos, const cString& oldStr, const cString& newStr )
    {
        if (_size == 0)
            return;
        if (endPos >= _size) endPos = _size-1;
        
        u32 i = startPos;
        do 
        {
            i = find(oldStr.c_str(), i);
            if (npos != i && i < endPos)
            {
                erase(i, oldStr.size());
                insert(i, newStr.c_str(), newStr.size());
                i += newStr.size();  
                endPos += newStr.size();
                endPos -= oldStr.size();
            }
            else
                break;
        } while (i < endPos);   // while

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    u32 cString::size() const
    {
        return _size;
    }

    //-----------------------------------------------
    hyc::cString cString::substr( u32 off, u32 count ) const
    {
        if (off >= _size)
            count = 0;
        else if (count > _size-off)
            count = _size - off;

        return cString(_pBuf+off, count);
    }

    void cString::toLower()
    {
        for (u32 i = 0; i < _size; i++)
        {
            _pBuf[i] = tolower(_pBuf[i]);
        }
        
#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

    }

    void cString::toUpper()
    {
        for (u32 i = 0; i < _size; i++)
        {
            _pBuf[i] = toupper(_pBuf[i]);
        }

#ifdef _DEBUG
        pDebugStr = (char*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void cString::reserve( u32 reserveSize )
    {
        if (reserveSize > _capa)
        {
            if (reserveSize > MAX_CAPA)
            {
                error_all(_T("cString::reserve too long"));
                exit(-1);
            }

            if (_pBuf)
            {
                _pBuf = (char*)MemoryAlloc::Realloc(_pBuf, reserveSize);
            }
            else
            {
                _pBuf = (char*)MemoryAlloc::Malloc(reserveSize);
            }
            
            assert(_size < reserveSize);
            if (_size > 0)
                _pBuf[_size] = 0;
            else
                _pBuf[0] = 0;
            
            _capa = reserveSize;


#ifdef _DEBUG
            pDebugStr = (char*)c_str();
#endif // _DEBUG
        }
    }

	//-----------------------------------------------
	void cString::split( char delimiter, OUT Array<cString>* parrStr ) const
	{
        Array<cString>& arrStr = *parrStr;

		arrStr.clear();
        if (empty())
            return;

		u32 i = 0;
		u32 j = 0;
		cString strTemp;
		while (i != npos)
		{
			j = find(delimiter, i);
			if (j != npos)
			{
				strTemp = substr(i, j-i);
				arrStr.push_back(strTemp);
				i = j + 1;
			}
			else
			{
				strTemp = substr(i, size()-i);
				arrStr.push_back(strTemp);
				break;
			}				
		}
	}

	//-----------------------------------------------
	void cString::split( const char* delimiter, OUT Array<cString>* parrStr ) const
	{
        Array<cString>& arrStr = *parrStr;

		arrStr.clear();
        if (empty())
            return;

		u32 l = (u32)StringUtils::GetLength(delimiter);
		u32 i = 0;
		u32 j = 0;
		cString strTemp;
		while (i != npos)
		{
			j = find(delimiter, i);
			if (j != npos)
			{
				strTemp = substr(i, j-i);
				arrStr.push_back(strTemp);
				i = j + l;
			}
			else
			{
				strTemp = substr(i, size()-i);
				arrStr.push_back(strTemp);
				break;
			}
		}
	}
    
    //-----------------------------------------------
    void cString::trim(char c)
    {
        u32 i = 0;
        while (i < _size) {
            if (c == at(i))
                i++;
            else
                break;
        } // while
        
        if (i >= size())
        {
            clear();
        }
        else
        {
            s32 j = _size-1;
            while (j > i) {
                if (c == _pBuf[j])
                    j--;
                else
                    break;
            }
            
            *this = substr(i, j-i+1);
        } // if..else
    }

    //-----------------------------------------------
    bool cString::isBeginWith( const char* str ) const
    {
        u32 i = 0;
        while (*str && _size > i)
        {
            if (*(_pBuf+i) != *str)
                return false;

            i++;
            str++;
        }

        return size() > i;
    }


	//----------------------------------------------------------------------------------------------------
    //*	wString
    //----------------------------------------------------------------------------------------------------
    //-----------------------------------------------
    wString::wString()
    	:
#ifdef DEBUG
    	    	pDebugStr(NULL),
#endif
        _pBuf(NULL)
        , _size(0)
        , _capa(0)
    {
    }

    wString::wString( const wString& str )
        : _pBuf(NULL)
        , _size(0)
        , _capa(0)
    {
        if (!str.empty())
            append(str.c_str(), str.size());
    }

    //-----------------------------------------------
    wString::wString( const wchar_t* str )
        : _pBuf(NULL)
        , _size(0)
        , _capa(0)
    {
        if (str != NULL)
            append(str, (u32)wcslen(str));
    }

    //-----------------------------------------------
    wString::wString( const wchar_t* str, u32 size )
        : _pBuf(NULL)
        , _size(0)
        , _capa(0)
    {
        resize(size, 0);
        wmemcpy(_pBuf, str, size);

#ifdef _DEBUG
        pDebugStr = c_str();
#endif // _DEBUG
    }

    //-----------------------------------------------
    wString::~wString()
    {
        if (_pBuf)
            MemoryAlloc::Free(_pBuf);
    }

    //-----------------------------------------------
    wString& wString::operator=( const wString& str )
    {
        if (this == &str)
            return *this;

        clear();
        append(str.c_str(), str.size());

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

        return *this;
    }


    wString& wString::operator=( const wchar_t* str )
    {
        if (c_str() == str)
            return *this;

        if ( NULL == str )
            clear();
        else
        {
            clear();
            append(str, (u32)wcslen(str));
        }

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

        return *this;
    }

    bool wString::operator==( const wString& str ) const
    {
        if (size() == str.size())
        {
            if (size() == 0)
                return true;

            return 0 == wmemcmp(c_str(), str.c_str(), size());
        }
        
        return false;
    }

    bool wString::operator==( const wchar_t* str ) const
    {
        if ( NULL == str )
        {
            return empty();
        }
        else if (size() == wcslen(str))
        {
            if (size() == 0)
                return true;

            return 0 == wmemcmp(c_str(), str, size());
        }
        else
            return false;
    }

    bool wString::operator!=( const wString& str ) const
    {
        return !operator==(str);
    }

    bool wString::operator!=( const wchar_t* str ) const
    {
        return !((*this) == str);
    }

    wString wString::operator+( const wString& str ) const
    {
        wString s;
        s.reserve(_bestSize(size()+str.size()));
        s.append(c_str(), size());
        s.append(str.c_str(), str.size());

        return s;
    }

    wString wString::operator+( const wchar_t& c ) const
    {
        wString s;
        s.reserve(_bestSize(size()+1));
        s.append(c_str(), size());
        s.append(1, c);

        return s;
    }

    //-----------------------------------------------
    wString wString::operator+( const wchar_t* str ) const
    {
        if ( NULL == str )
        {
            return *this;
        }

        u32 l = (u32)wcslen(str);
        wString s;
        s.reserve(_bestSize(size()+l));
        s.append(c_str(), size());
        s.append(str, l);
        return s;
    }

    wString& wString::operator+=( const wString& str )
    {
        append(str.c_str(), str.size());

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

        return *this;
    }

    wString& wString::operator+=( const char& c )
    {
        wString temp;
        StringUtils::Narrow2wide(&c, 1, temp);

        append(temp.c_str(), temp.size());

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

        return *this;
    }

    //-----------------------------------------------
    wString& wString::operator+=( const char* str )
    {
        if ( NULL == str )
        {
            return *this;
        }

        wString temp;
        StringUtils::Narrow2wide(str, StringUtils::GetLength(str), temp);

        append(temp.c_str(), temp.size());

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

        return *this;
    }

    //----------------------------------------
    wString& wString::operator+=( const wchar_t& c )
    {
        append(1, c);

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

        return *this;
    }

    //----------------------------------------
    wString& wString::operator+=( const wchar_t* str )
    {
        if ( NULL == str )
        {
            return *this;
        }

        append(str, (u32)wcslen(str));

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

        return *this;
    }



    wchar_t wString::operator[]( u32 index ) const
    {
        return _pBuf[index];
    }

    //-----------------------------------------------
    wchar_t& wString::operator[]( u32 index )
    {
        return _pBuf[index];
    }

    //-----------------------------------------------
    bool wString::operator<( const wString& str ) const
    {
        return operator<(str.c_str());
    }

    //-----------------------------------------------
    bool wString::operator<( const wchar_t* str ) const
    {
        if (empty())
            return StringUtils::IsEmpty(str) ? false : true;

        if (NULL == str)
            return false;
        
        return 0 > wcscmp(_pBuf, str);
    }

    //-----------------------------------------------
    bool wString::operator<=( const wString& str ) const
    {
        return operator<=(str.c_str());
    }

    //-----------------------------------------------
    bool wString::operator<=( const wchar_t* str ) const
    {
        if (empty())
            return true;

        if (NULL == str)
            return false;
        
        return 0 >= wcscmp(_pBuf, str);
    }

    //-----------------------------------------------
    bool wString::operator>=( const wString& str ) const
    {
        return operator>=(str.c_str());
    }

    //-----------------------------------------------
    bool wString::operator>=( const wchar_t* str ) const
    {
        if (StringUtils::IsEmpty(str))
            return true;

        if (empty())
            return false;
        
        return 0 <= wcscmp(_pBuf, str);
    }

    //-----------------------------------------------
    bool wString::operator>( const wString& str ) const
    {
        return operator>(str.c_str());
    }

    //-----------------------------------------------
    bool wString::operator>( const wchar_t* str ) const
    {
        if (StringUtils::IsEmpty(str))
            return empty() ? false : true;

        if (empty())
            return false;
        
        return 0 < wcscmp(_pBuf, str);
    }

    //-----------------------------------------------
    const wchar_t& wString::at( u32 index ) const
    {
        return _pBuf[index];
    }

    //-----------------------------------------------
    void wString::append( u32 count, wchar_t c )
    {
        if (count > 0)
        {
            reserve(_bestSize(_size+count));
            wmemset(_pBuf+_size, c, count);
            _size+=count;
            *(_pBuf+_size) = 0;
            
#ifdef _DEBUG
            pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG
        }
    }

    //-----------------------------------------------
    void wString::append( const wString& str )
    {
        if (!str.empty())
        {
            reserve(_bestSize(_size+str.size()));
            wmemcpy(_pBuf+_size, str.c_str(), str.size());
            _size += str.size();
            *(_pBuf+_size) = 0;
            
#ifdef _DEBUG
            pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG
        }
    }

    //-----------------------------------------------
    void wString::append( const wchar_t* pstr, u32 count )
    {
        if (count > 0)
        {
            reserve(_bestSize(_size+count));
            wmemcpy(_pBuf+_size, pstr, count);
            _size += count;
            *(_pBuf+_size) = 0;
            
#ifdef _DEBUG
            pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG
        }
    }

    //-----------------------------------------------
    const wchar_t* wString::c_str() const
    {
        return _pBuf;
    }

    //-----------------------------------------------
    u32 wString::capacity() const
    {
        return _capa;
    }

    //-----------------------------------------------
    s32 wString::compare( u32 off, u32 count, const wchar_t* pStr ) const
    {
        return (s32)wcsncmp(_pBuf+off, pStr, count);
    }

    //-----------------------------------------------
    s32 wString::compare( u32 off, u32 count, const wString& str ) const
    {
        return (s32)wcsncmp(_pBuf+off, str.c_str(), count);
    }

    //-----------------------------------------------
    void wString::clear()
    {
        if (_pBuf)
            _pBuf[0] = 0;
        _size = 0;

#ifdef _DEBUG
        pDebugStr = L"";
#endif // _DEBUG

    }

    //-----------------------------------------------
    bool wString::empty() const
    {
        return _size == 0;
    }

    //-----------------------------------------------
    void wString::erase( u32 off, u32 count /*= 1*/ )
    {
        if (count == -1)
            count = _size-off;

        if (off > _size)
            off = _size-count;

        assert(size() >= off+count);

        if (_size > off+count)
            wmemmove(_pBuf+off, _pBuf+off+count, (_size-off-count));
        _size -= count;
        _pBuf[_size] = 0;

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    s32 wString::find( wchar_t c, u32 off /*= 0*/ ) const
    {
        for (u32 i = off; i < _size; i++)
        {
            if (_pBuf[i] == c)
                return i;
        }

        return npos;
    }

    //-----------------------------------------------
    s32 wString::find( const wchar_t* str, u32 off /*= 0*/ ) const
    {
        if (off >= _size)
            return npos;

        if (NULL == str)
            return npos;
        else
        {
            u32 l = (u32)wcslen(str);
            if (l == 1)
                return find(str[0], off);
            else if (l > _size-off)
                return npos;
            else if (l == _size-off)
            {
                for(u32 i = 0; i < l; i++)
                {
                    if (_pBuf[i+off] != str[i])
                        return npos;
                }
                return off;
            }                
        }

        u32 index = KMPw(_pBuf+off, str);
        return index == npos ? npos : index+off;
    }

    //-----------------------------------------------
    hyc::s32 wString::find_last_of( wchar_t c, u32 off /*= npos*/ ) const
    {
        if (off > _size)
            off = _size-1;
        for (s32 i = off; i >= 0; i--)
        {
            if (_pBuf[i] == c)
                return i;
        }

        return npos;
    }

    //-----------------------------------------------
    hyc::s32 wString::find_last_of( const wchar_t* str, u32 off /*= npos*/ ) const
    {
        if (NULL == str || empty())
            return npos;
        else
        {
            if (off > _size)
                off = _size-1;
            u32 l = (u32)wcslen(str);
            if (l == 1)
                return find_last_of(str[0], off);
            else if (l > off+1)
                return npos;
            else if (l == off+1)
            {
                s32 j = 1;
                for(s32 i = off; i >= 0; i--, j++)
                {
                    if (_pBuf[i] != str[l-j])
                        return npos;
                }
                return off;
            }                
        }

        return KMPwr(_pBuf, str, off);
    }

    //-----------------------------------------------
    void wString::vformat( const wchar_t* fmt, va_list arg_ptr )
    {
        if ( NULL == fmt )  return;
        assert(fmt != c_str());
        
        reserve( _bestSize((u32)wcslen(fmt) * 2) );
        
        /*cString temp;
         temp.reserve(capacity());
         va_list arg_ptr;
         va_start(arg_ptr, vformat);
         vsnprintf(const_cast<char*>(temp.c_str()), temp.capacity(), vformat, arg_ptr);
         p->m_str = temp.c_str();
         va_end(arg_ptr);*/
        
		clear();
		wString temp;

		int num = 0;
		int prec = -1;

		char c = *fmt;
		while (0 != c) {

			if (L'%' == c || num > 0 || prec > 0)
			{
				if (L'%' == c)
				{
					fmt++;
					c = *fmt;
				}

				if (0 == c)  { error_all(_T("format error")); assert(0);return; }
				switch (c)
				{
					//-----------------------------------------------
				case L'0':
				case L'1':
				case L'2':
				case L'3':
				case L'4':
				case L'5':
				case L'6':
				case L'7':
				case L'8':
				case L'9':
					{
						num = c-L'0';

						while (true)
						{
							fmt++;
							c = *fmt;
							if (0 == c)  { error_all(_T("format error")); assert(0);return; }
							if (!(c >= L'0' && c <= L'9'))
							{
								fmt--;
								break;
							}
							else
							{
								num = num*10 + c-L'0';
							}
						}
					} break;
					//-----------------------------------------------
				case L'.':
					{
						fmt++;
						c = *fmt;
						if (0 == c)  { error_all(_T("format error")); assert(0); return; }
						switch (c)
						{
						case L'0':
						case L'1':
						case L'2':
						case L'3':
						case L'4':
						case L'5':
						case L'6':
						case L'7':
						case L'8':
						case L'9':
							{
								prec = c-L'0';

								while (true)
								{
									fmt++;
									c = *fmt;
									if (0 == c)  { error_all(_T("format error")); assert(0);return; }
									if (!(c >= L'0' && c <= L'9'))
									{
										fmt--;
										break;
									}
									else
									{
										prec = prec*10 + c-L'0';
									}
								}
							} break;
						default:
							fmt--;
							break;
						} // switch.
					} break;

					//-----------------------------------------------
				case L'%':
					num = 0;
					prec = -1;
					append(1, L'%');
					break;
					//-----------------------------------------------
				case L'c':
				case L'C':
					num = 0;
					prec = -1;
					append(1, va_arg(arg_ptr, int));
					break;
					//-----------------------------------------------
				case L's':
				case L'S':
					num = 0;
					prec = -1;
					*this += va_arg(arg_ptr, const wchar_t*);
					break;
					//-----------------------------------------------
				case L'x':
					{
						num = 0;
						prec = -1;
						int v = va_arg(arg_ptr, int);
						EncodingUtils::ByteToHex((Byte*)&v, 1, temp);
						*this += temp;
					} break;
				case L'X':
					{
						num = 0;
						prec = -1;
						int v = va_arg(arg_ptr, int);
						EncodingUtils::ByteToHex((Byte*)&v, 1, temp);
						temp.toUpper();
						*this += temp;
					} break;
					//-----------------------------------------------
				case L'd':
				case L'D':
				case L'i':
				case L'I':
					StringUtils::S32ToStr(va_arg(arg_ptr, int), temp);
					if (num > temp.size())
						this->append(num-temp.size(), L'0');
					num = 0;
					prec = -1;
					*this += temp;
					break;
					//-----------------------------------------------
				case L'u':
				case L'U':
					StringUtils::U32ToStr(va_arg(arg_ptr, unsigned int), temp);
					if (num > temp.size())
						this->append(num-temp.size(), L'0');
					num = 0;
					prec = -1;
					*this += temp;
					break;
					//-----------------------------------------------
				case L'f':
				case L'F':
					if (prec == -1)
						prec = 6;
					StringUtils::F64ToStr(va_arg(arg_ptr, double), temp, prec);
					if (num > temp.size()-prec-1)
						this->append(num-(temp.size()-prec-1), L'0');
					num = 0;
					prec = -1;
					*this += temp;
					break;
					//-----------------------------------------------
				case L'h':
				case L'H':
					{
						fmt++;
						c = *fmt;
						if (0 == c)  { error_all(_T("format error")); assert(0); return; }
						switch (c)
						{
						case L'd':
						case L'D':
						case L'i':
						case L'I':
							StringUtils::S32ToStr(va_arg(arg_ptr, int), temp);
							if (num > temp.size())
								this->append(num-temp.size(), L'0');
							num = 0;
							*this += temp;
							break;
						case L'u':
						case L'U':
							StringUtils::U32ToStr(va_arg(arg_ptr, unsigned int), temp);
							if (num > temp.size())
								this->append(num-temp.size(), L'0');
							num = 0;
							*this += temp;
							break;
						default:
							error_all(_T("format error"));
							assert(0);
							return;
							break;
						}
					} break;
					//-----------------------------------------------
				case L'l':
				case L'L':
					{
						fmt++;
						c = *fmt;
						if (0 == c)  { error_all(_T("format error")); assert(0); return; }
						switch (c)
						{
						case L'd':
						case L'D':
						case L'i':
						case L'I':
							StringUtils::IntegerToStr(va_arg(arg_ptr, long), temp);
							if (num > temp.size())
								this->append(num-temp.size(), L'0');
							num = 0;
							*this += temp;
							break;
						case L'u':
						case L'U':
							StringUtils::UIntegerToStr(va_arg(arg_ptr, unsigned long), temp);
							if (num > temp.size())
								this->append(num-temp.size(), L'0');
							num = 0;
							*this += temp;
							break;
						case L'f':
						case L'F':
							if (prec == -1) prec = 6;
							StringUtils::F64ToStr(va_arg(arg_ptr, double), temp, prec);
							if (num > temp.size()-prec-1)
								this->append(num-(temp.size()-prec-1), L'0');
							num = 0;
							prec = -1;
							*this += temp;
							break;
						case L'l':
						case L'L':
							{
								fmt++;
								c = *fmt;
								if (0 == c)  { error_all(_T("format error")); assert(0); return; }
								switch (c)
								{
								case L'd':
								case L'D':
								case L'i':
								case L'I':
									StringUtils::S64ToStr(va_arg(arg_ptr, long long), temp);
									if (num > temp.size())
										this->append(num-temp.size(), L'0');
									num = 0;
									*this += temp;
									break;
								case L'u':
								case L'U':
									StringUtils::U64ToStr(va_arg(arg_ptr, unsigned long long), temp);
									if (num > temp.size())
										this->append(num-temp.size(), L'0');
									num = 0;
									*this += temp;
									break;
								default:
									error_all(_T("format error"));
									assert(0);
									return;
									break;
								}
							} break;
						default:
							error_all(_T("format error"));
							assert(0);
							return;
							break;
						}
					} break;

				default:
					error_all(_T("format error"));
					assert(0);
					return;
					break;
				}
			}
			else
			{
				append(1, c);
			} // if..else.

			fmt++;
			c = *fmt;
		} // while

#ifdef _DEBUG
		pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG
    }

    //-----------------------------------------------
    void wString::format( const wchar_t* fmt, ... )
    {
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        vformat(fmt, arg_ptr);
        va_end(arg_ptr);
    }

    //-----------------------------------------------
    void wString::insert( u32 pos, u32 count, wchar_t c )
    {
        if (pos > _size)
            pos = _size;

        reserve(_bestSize(_size+count));
        if (pos < _size)
        {
            wmemcpy(_pBuf+pos+count,_pBuf+pos, _size-pos);
            wmemset(_pBuf+pos, c, count);
        }
        else
        {
            wmemset(_pBuf+_size, c, count);
        }

        _size += count;

        if (_size > 0)
            _pBuf[_size] = 0;
        else if (_capa > 0)
            _pBuf[0] = 0;

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }
 
    //-----------------------------------------------
    void wString::insert( u32 pos, const wString& str )
    {
        insert(pos, str.c_str());
    }

    //-----------------------------------------------
    void wString::insert( u32 pos, const wchar_t* str )
    {
        if (str)
        {
            s32 count = (u32)wcslen(str);
            insert(pos, str, count);
        }
    }

    //-----------------------------------------------
    void wString::insert( u32 pos, const wchar_t* str, u32 count )
    {
        if (!str || count == 0)
            return;

        if (pos > _size)
            pos = _size;

        reserve(_bestSize(_size+count));
        if (pos < _size)
        {
            wmemmove(_pBuf+pos+count,_pBuf+pos, _size-pos);
            wmemcpy(_pBuf+pos, str, count);
        }
        else
        {
            wmemcpy(_pBuf+_size, str, count);
        }

        _size += count;
        if (_size > 0)
            _pBuf[_size] = 0;
        else if (_capa > 0)
            _pBuf[0] = 0;

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }
    //-----------------------------------------------
    void wString::resize( u32 newSize, wchar_t c )
    {
        reserve(_bestSize(newSize));

        if (newSize > _size)
        {
            wmemset(_pBuf+_size, c, newSize-_size);
        }

        _size = newSize;
        if (_size > 0)
            _pBuf[_size] = 0;
        else if (_capa > 0)
            _pBuf[0] = 0;

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void wString::replace( u32 startPos, u32 endPos, wchar_t oldC, wchar_t newC )
    {
        if (_size == 0)
            return;
        
        if (endPos >= _size) endPos = _size-1;
        for (u32 i = startPos; i <= endPos; i++)
        {
            if (_pBuf[i] == oldC)
                _pBuf[i] = newC;
        }

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void wString::replace( u32 startPos, u32 endPos, wchar_t oldC, const wString& newStr )
    {
        if (_size == 0)
            return;
        if (endPos >= _size) endPos = _size-1;
        
        for (u32 i = startPos; i < endPos; ++i)
        {
            if (_pBuf[i] == oldC)
            {
                if (newStr.empty())
                {
                    erase(i, 1);
                    i--;
                    endPos--;
                }
                else if (newStr.size() == 1)
                {
                    _pBuf[i] = newStr[0];
                }
                else
                {
                    _pBuf[i] = newStr[0];
                    insert(i+1, newStr.c_str()+1, newStr.size()-1);
                    i += newStr.size()-1;
                    endPos += newStr.size()-1;
                }
            }
        }

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void wString::replace( u32 startPos, u32 endPos, const wString& oldStr, wchar_t newC )
    {
        if (_size == 0)
            return;
        if (endPos >= _size) endPos = _size-1;
        
        u32 i = startPos;
        do 
        {
            i = find(oldStr.c_str(), i);
            if (npos != i && i < endPos)
            {
                _pBuf[i] = newC;
                erase(i+1, oldStr.size()-1);
                ++i;
                endPos -= oldStr.size()-1;
            }
            else
                break;
        } while (i < endPos);   // while

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void wString::replace( u32 startPos, u32 endPos, const wString& oldStr, const wString& newStr )
    {
        if (_size == 0)
            return;
        if (endPos >= _size) endPos = _size-1;
        
        u32 i = startPos;
        do 
        {
            i = find(oldStr.c_str(), i);
            if (npos != i && i < endPos)
            {
                erase(i, oldStr.size());
                insert(i, newStr.c_str(), newStr.size());
                i += newStr.size();  
                endPos += newStr.size();
                endPos -= oldStr.size();
            }
            else
                break;
        } while (i < endPos);   // while

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    u32 wString::size() const
    {
        return _size;
    }

    //-----------------------------------------------
    hyc::wString wString::substr( u32 off, u32 count ) const
    {
        if (off >= _size)
            count = 0;
        else if (count > _size-off)
            count = _size - off;

        return wString(_pBuf+off, count);
    }

    void wString::toLower()
    {
        for (u32 i = 0; i < _size; i++)
        {
            _pBuf[i] = towlower(_pBuf[i]);
        }
        
#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }

    void wString::toUpper()
    {
        for (u32 i = 0; i < _size; i++)
        {
            _pBuf[i] = towupper(_pBuf[i]);
        }

#ifdef _DEBUG
        pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG

    }

    //-----------------------------------------------
    void wString::reserve( u32 reserveSize )
    {
        if (reserveSize > _capa)
        {
            if (reserveSize > MAX_CAPA)
            {
                error_all(_T("wString::reserve too long"));
                exit(-1);
            }

            if (_pBuf)
            {
                _pBuf = (wchar_t*)MemoryAlloc::Realloc(_pBuf, reserveSize*sizeof(wchar_t));
            }
            else
            {
                _pBuf = (wchar_t*)MemoryAlloc::Malloc(reserveSize*sizeof(wchar_t));
            }
            
            assert(_size < reserveSize);
            if (_size > 0)
                _pBuf[_size] = 0;
            else
                _pBuf[0] = 0;
            
            _capa = reserveSize;


#ifdef _DEBUG
            pDebugStr = (wchar_t*)c_str();
#endif // _DEBUG
        }
    }

	//-----------------------------------------------
	void wString::split( wchar_t delimiter, OUT Array<wString>* parrStr ) const
	{
        Array<wString>& arrStr = *parrStr;

		arrStr.clear();
        if (empty())
            return;

		u32 i = 0;
		u32 j = 0;
		wString strTemp;
		while (i != npos)
		{
			j = find(delimiter, i);
			if (j != npos)
			{
				strTemp = substr(i, j-i);
				arrStr.push_back(strTemp);
				i = j + 1;
			}
			else
			{
				strTemp = substr(i, size()-i);
				arrStr.push_back(strTemp);
				break;
			}				
		}
	}

	//-----------------------------------------------
	void wString::split( const wchar_t* delimiter, OUT Array<wString>* parrStr ) const
	{
        Array<wString>& arrStr = *parrStr;

		arrStr.clear();
        if (empty())
            return;

		u32 l = (u32)StringUtils::GetLength(delimiter);
		u32 i = 0;
		u32 j = 0;
		wString strTemp;
		while (i != npos)
		{
			j = find(delimiter, i);
			if (j != npos)
			{
				strTemp = substr(i, j-i);
				arrStr.push_back(strTemp);
				i = j + l;
			}
			else
			{
				strTemp = substr(i, size()-i);
				arrStr.push_back(strTemp);
				break;
			}
		}
	}
    
    //-----------------------------------------------
    void wString::trim(wchar_t c)
    {
        u32 i = 0;
        while (i < _size) {
            if (c == at(i))
                i++;
            else
                break;
        } // while
        
        if (i >= _size)
        {
            clear();
        }
        else
        {
            s32 j = _size-1;
            while (j > i) {
                if (c == _pBuf[j])
                    j--;
                else
                    break;
            }
            
            *this = substr(i, j-i+1);
        } // if..else
    }

    //-----------------------------------------------
    bool wString::isBeginWith( const wchar_t* str ) const
    {
        u32 i = 0;
        while (*str && _size > i)
        {
            if (*(c_str()+i) != *str)
                return false;

            i++;
            str++;
        }

        return size() > i;
    }

    //-----------------------------------------------
    cString operator+( const char& c, const cString& right )
    {
        cString str;
        str.reserve(right.size()+1);
        str.append(1, c);
        str.append(right);
        return str;
    }

    //-----------------------------------------------
    cString operator+( const char* str, const cString& right )
    {
        if (str)
        {
            cString str1;
            str1.reserve(right.size()+(u32)StringUtils::GetLength(str));
            str1.append(str);
            str1.append(right);
            return str1;
        }
        else
        {
            return right;
        }
    }

    //-----------------------------------------------
    wString operator+( const wchar_t& c, const wString& right )
    {
        wString str;
        str.reserve(right.size()+1);
        str.append(1, c);
        str.append(right);
        return str;
    }

    //-----------------------------------------------
    wString operator+( const wchar_t* str, const wString& right )
    {
        if (str)
        {
            wString str1;
            str1.reserve(right.size()+(u32)StringUtils::GetLength(str));
            str1.append(str);
            str1.append(right);
            return str1;
        }
        else
        {
            return right;
        }
    }

    //-----------------------------------------------
    bool   operator==( const char* str, const cString& right )
    {
        return right == str;
    }

    //-----------------------------------------------
    bool    operator==( const wchar_t* str, const wString& right )
    {
        return right == str;

    }

    //-----------------------------------------------
    bool    operator!=( const char* str, const cString& right )
    {
        return right != str;
    }

    //-----------------------------------------------
    bool    operator!=( const wchar_t* str, const wString& right )
    {
        return right != str;
    }


} // end namespace hyc


#if defined(CN_PLATFORM_MOBILE)
#if !defined(__i386__) && defined(__arm__)
//-----------------------------------------------
void * memcpy( void * dst, const void * src, size_t len )
{
    char * pDst = (char *) dst;
    char const * pSrc = (char const *) src;
    
    while (len--)
    {
        *pDst++ = *pSrc++;
    }
    
    return (dst);
}


//-----------------------------------------------
wchar_t * wmemcpy( wchar_t * pDst, const wchar_t* pSrc, size_t len )
{
    wchar_t* s = pDst;
    while (len--)
    {
        *pDst++ = *pSrc++;
    }

    return s;
}

#endif
#endif
