// Copyright(c) 2010-2010 lipengxiang
// hyui Graphic Engine
//  <Geometric> define 3d vector of system

#ifndef __BP_VECTOR4D_H_20100316220516__
#define __BP_VECTOR4D_H_20100316220516__

namespace hyc
{
namespace geom
{
    //--------------------------------------------------
    //*	4 dimension vector
    //-------------------------------------------------  
    template<class T>
	struct Vector4
	{
        //--------------------------------------------------
        //*	    variable member
        //--------------------------------------------------
        T    x;
        T    y;
        T    z;
        T    w;
        
        //--------------------------------------------------
        //*     constructor & destructor
        //--------------------------------------------------
        
        Vector4();
        Vector4( const Vector4& rhs );
        Vector4( T x, T y, T z, T w );
    };

    typedef Vector4<real>       Vector4_f;
    typedef Vector4<integer>    Vector4_i;
    
    //-----------------------------------------------
    template<class T> 
    inline Vector4<T>::Vector4()   {}
    
    //-----------------------------------------------
    template<class T> 
    inline Vector4<T>::Vector4( const Vector4& rhs ) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)  {}
    
    //-----------------------------------------------
    template<class T> 
    inline Vector4<T>::Vector4( T x, T y, T z, T w ) : x(x), y(y), z(z), w(w)  {}
    
} // end namespace geom
} // end namespace hyc



#endif // __BP_VECTOR4D_H_20100316220516__