//
// Copyright(c) 1999-2012 hyrj. All rights reserved.
// hyui Graphic Engine
// author:       lipengxiang
// description:  
//

#ifndef __BP_BEZIERCURVE2D_H_20120202175821__
#define __BP_BEZIERCURVE2D_H_20120202175821__

#include "icurve2d.hpp"

namespace hyc
{
namespace geom
{
    //----------------------------------------------------------------------------------------------------
    //!	采用3次曲线的bezier
    //----------------------------------------------------------------------------------------------------
    class BezierCurve2d : public ICurve2d
    {
    public:
        // Bezier上的控制点
        //  若想曲线连续, 三个点必须在同一直线上.
        struct CtrlPoint {
            CtrlPoint( 
                      const Vector2_f& ptCtrl1 = Vector2_f(),
                      const Vector2_f& ptInCurve = Vector2_f(),
                      const Vector2_f& ptCtrl2 = Vector2_f()
                      ) { this->ptInCurve = ptInCurve; this->ptCtrl1 = ptCtrl1; this->ptCtrl2 = ptCtrl2; }
            Vector2_f ptInCurve;           // 在曲线上的点.
            Vector2_f ptCtrl1 , ptCtrl2;   // 两个控制点.
        };

    public:
        BezierCurve2d();
        virtual ~BezierCurve2d();

    public:
        //-----------------------------------------------
        // override
        virtual s32                 getKeyPointNum() const;
        virtual void                setKeyPointNum(u32 iFrameNum);
        virtual const Vector2_f&    getKeyPoint(u32 index) const;

        //-----------------------------------------------
        // method
        virtual void        addControlPoint(const CtrlPoint& pt);
        virtual void        removeControlPoint(u32 index);
        virtual CtrlPoint*  getControlPoint(u32 index);
        virtual s32         getControlPointNum() const;
        virtual void        updateCurve();  // 在操作完数据后更新keyPoint.

    private:
        typedef Array<BezierCurve2d::CtrlPoint*>    CtrlPointVec;
        typedef Array<Vector2_f>                      PointVec;
        CtrlPointVec    m_vecCtrlPoints;    // 头尾为曲线两端.
        PointVec        m_vecKeyPoints;
    };

    //-----------------------------------------------
    inline BezierCurve2d::BezierCurve2d()
    {
        setKeyPointNum(10);
    }

    //-----------------------------------------------
    inline BezierCurve2d::~BezierCurve2d()
    {
        // dealloc control points
        for (u32 i = 0; i < m_vecCtrlPoints.size(); ++i)
        {
            m_vecCtrlPoints[i];
        }
        m_vecCtrlPoints.clear();
    }

    //-----------------------------------------------
    inline int BezierCurve2d::getKeyPointNum() const
    {
        return m_vecKeyPoints.size();
    }

    //-----------------------------------------------
    inline void BezierCurve2d::setKeyPointNum( u32 iFrameNum )
    {
        
        m_vecKeyPoints.clear();
        m_vecKeyPoints.reserve(iFrameNum);
        for (u32 i = 0; i < iFrameNum; ++i)
        {
            m_vecKeyPoints.push_back(Vector2_f());
        }
    }

    //-----------------------------------------------
    inline const Vector2_f& BezierCurve2d::getKeyPoint( u32 index ) const
    {
        return m_vecKeyPoints[index];
    }

    //-----------------------------------------------
    inline void BezierCurve2d::addControlPoint( const CtrlPoint& pt )
    {
        CtrlPoint* ppt = new CtrlPoint;
        ppt->ptCtrl1 = pt.ptCtrl1;
        ppt->ptCtrl2 = pt.ptCtrl2;
        ppt->ptInCurve = pt.ptInCurve;
        m_vecCtrlPoints.push_back(ppt);
    }

    //-----------------------------------------------
    inline void BezierCurve2d::removeControlPoint( u32 index )
    {
        if (index < m_vecCtrlPoints.size())
        {
            delete (m_vecCtrlPoints[index]);
            m_vecCtrlPoints.eraseByIndex(index);
        }
    }

    //-----------------------------------------------
    inline BezierCurve2d::CtrlPoint* BezierCurve2d::getControlPoint( u32 index )
    {
        return m_vecCtrlPoints[index];
    }

    //-----------------------------------------------
    inline int BezierCurve2d::getControlPointNum() const
    {
        return m_vecCtrlPoints.size();
    }

    //-----------------------------------------------
    inline void BezierCurve2d::updateCurve()
    {
        float t = 0.0f;
        float setp = 1.0f * (getControlPointNum()-1) / getKeyPointNum();

        // 采用3次曲线
        // B(t) = (1-t)^3 P0 + 3t(1-t)^2 P1 + 3t^2(1-t) P2 + t^3 P3, t=[0,1]
        u32 indexOfCtrl1 = 0;
        u32 indexOfCtrl2 = 1;
        for (u32 i = 0; i < m_vecKeyPoints.size(); ++i)
        {
            if (t > 1.0f)
            {
                ++indexOfCtrl1;
                ++indexOfCtrl2;
                t -= 1.0f;
            }

            float _p0 = pow(1-t, 3);
            float _p1 = 3*t*pow(1-t, 2);
            float _p2 = 3*t*t*(1-t);
            float _p3 = t*t*t;

            Vector2_f& p0 = m_vecCtrlPoints[indexOfCtrl1]->ptInCurve; // p0
            Vector2_f& p1 = m_vecCtrlPoints[indexOfCtrl1]->ptCtrl2;   // p1
            Vector2_f& p2 = m_vecCtrlPoints[indexOfCtrl2]->ptCtrl1;   // p2
            Vector2_f& p3 = m_vecCtrlPoints[indexOfCtrl2]->ptInCurve; // p3

            m_vecKeyPoints[i].x = _p0*p0.x + _p1*p1.x + _p2*p2.x + _p3*p3.x;
            m_vecKeyPoints[i].y = _p0*p0.y + _p1*p1.y + _p2*p2.y + _p3*p3.y;

            t += setp;
        }
    }


} // end namespace geom
} // end namespace hyc

#endif // end of define __BP_BEZIERCURVE2D_H_20120202175821__

