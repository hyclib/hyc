//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hy UI
// author:       lipengxiang
// description:  
//

#ifndef __BP_CURVE2D_H_20120202175757__
#define __BP_CURVE2D_H_20120202175757__

#include "../vector2.hpp"

namespace hyc
{
namespace geom
{
    //----------------------------------------------------------------------------------------------------
    //!	2d 曲线
    //----------------------------------------------------------------------------------------------------
    class ICurve2d
    {
    public:
        virtual ~ICurve2d() {}

    public:
        virtual s32                 getKeyPointNum() const = 0;
        virtual void                setKeyPointNum( u32 iFrameNum ) = 0;
        virtual const Vector2_f&    getKeyPoint( u32 index ) const = 0;
    };

} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_CURVE2D_H_20120202175757__

