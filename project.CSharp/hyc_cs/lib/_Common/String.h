//
// Copyright(c) 1999-2011 hyrj. All rights reserved.
// business logic layer network proxy
// author:       lipengxiang
// description:  String.h
//

#pragma once

#include "hyc/include/hyc.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Hy
{

    /// <summary>
    /// 字符串
    /// </summary>
    public ref class String
    {
        hyc::String* _ptr;
    public:
        // 转换成System::String
        static operator System::String^ ( String^ val ) {   return val->ToString(); }

        static bool     operator ==( String^ val, System::String^ str )  { return val->ToString() == str; }
        static bool     operator !=( String^ val, System::String^ str )  { return val->ToString() != str; }
        void            operator =( System::String^ str )                { (*_ptr) = (char*)Marshal::StringToHGlobalAnsi(str).ToPointer(); }
        static String^  operator +( String^ val, System::String^ str )   { String^ pstr = gcnew String(); (*pstr->_ptr) = (char*)Marshal::StringToHGlobalAnsi(val->ToString()+str).ToPointer(); return pstr; }
        static String^  operator +=( String^ val, System::String^ str )  { (*val->_ptr) = (char*)Marshal::StringToHGlobalAnsi(val->ToString()+str).ToPointer(); return val; }

        static bool     operator ==( String^ val, const hyc::tchar* str )   { return (*val->_ptr)==str; }
        static bool     operator !=( String^ val, const hyc::tchar* str )   { return (*val->_ptr)!=str; }
        void            operator =( const hyc::tchar* str )                 { (*_ptr) = str; }
        static String^  operator +( String^ val, const hyc::tchar* str )    { String^ pstr = gcnew String(); (*pstr->_ptr) = val->ToCString(); (*pstr->_ptr) += str; return pstr; }
        static String^  operator +=( String^ val, const hyc::tchar* str )   { (*val->_ptr) = (*val->_ptr)+str; return val; }

        static bool     operator ==( String^ val, String^ str )         { return (*val->_ptr)==(*str->_ptr); }
        static bool     operator !=( String^ val, String^ str )         { return (*val->_ptr)!=(*str->_ptr); }
        void            operator =( String^ str )                       { (*_ptr) = (*str->_ptr); }
        static String^  operator +( String^ val, String^ str )          { String^ pstr = gcnew String(); (*pstr->_ptr) = (*val->_ptr)+(*str->_ptr); return pstr; }
        static String^  operator +=( String^ val, String^ str )         { (*val->_ptr) = (*val->_ptr)+(*str->_ptr); return val; }
    
    public:
        String()                        { _ptr = new hyc::String(); }
        String(System::String^ str)     { _ptr = new hyc::String(); (*this) = str; }
        String(const hyc::tchar* pStr)  { _ptr = new hyc::String(); (*this) = pStr; }
        ~String()   { this->!String(); }
        !String()   { SAFE_DELETE(_ptr); }

        // static function 
        static int              GetByteLength( System::String^ str )     { return System::Text::Encoding::Default->GetByteCount(str); }
        static int              GetByteLength( System::String^ str, int offset, int size )  { return System::Text::Encoding::Default->GetByteCount(((wchar_t*)Marshal::StringToHGlobalAuto(str).ToPointer())+offset, size); }

        // function
        IntPtr                  getPtr() { return (IntPtr) _ptr; }
        virtual System::String^ ToString() override { return Marshal::PtrToStringAnsi( (IntPtr)(hyc::tchar*)ToCString() ); }
        const hyc::tchar*       ToCString()         { return _ptr->c_str(); }
        int                     Size()              { return _ptr->size();  }
        int                     Length()            { return Size(); }
        bool                    IsEmpty()           { return _ptr->empty(); }
   };

} // end namespace of Hy