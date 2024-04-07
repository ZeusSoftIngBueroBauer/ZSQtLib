/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#ifndef ZSDraw_PhysValRect_h
#define ZSDraw_PhysValRect_h

#include "ZSDraw/Common/ZSDrawPhysValPoint.h"
#include "ZSDraw/Common/ZSDrawPhysValLine.h"
#include "ZSDraw/Common/ZSDrawPhysValSize.h"
#include "QtCore/qrect.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Represents a rectangle on the drawing scene.

    Corresponds to QRectF but providing a physical unit and a resolution and
    the possibility to rotate the rectangle.

    Converting points (corners and center of the rectangle) from pixels to metric
    units is not supported. This conversion would have to be done by the parent
    group or scene to take the different Y-Scale-Axis orientation into account.
    But the line does not know about groups.
*/
class ZSDRAWDLL_API CPhysValRect
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValRect"; }
public: // ctors and dtor
    CPhysValRect(const CDrawingScene& i_drawingScene);
    CPhysValRect(const CDrawingScene& i_drawingScene, const QPointF& i_ptTL, const QPointF& i_ptBR);
    CPhysValRect(const CDrawingScene& i_drawingScene, const QPointF& i_ptTL, const QPointF& i_ptBR, const ZS::PhysVal::CUnit& i_unit);
    CPhysValRect(const CDrawingScene& i_drawingScene, const QRectF& i_rect);
    CPhysValRect(const CDrawingScene& i_drawingScene, const QRectF& i_rect, const ZS::PhysVal::CUnit& i_unit);
    CPhysValRect(const CDrawingScene& i_drawingScene, const CPhysValPoint& i_physValTopLeft, const CPhysValPoint& i_physValBottomRight);
    CPhysValRect(const CDrawingScene& i_drawingScene, const CPhysValPoint& i_physValTopLeft, const CPhysValSize& i_physValSize);
    CPhysValRect(const CPhysValRect& i_physValRectOther);
    virtual ~CPhysValRect();
public: // operators
    CPhysValRect& operator = (const CPhysValRect& i_physValRectOther);
    CPhysValRect& operator = (const QRectF& i_rect);
public: // operators
    bool operator == (const CPhysValRect& i_physValRectOther) const;
    bool operator != (const CPhysValRect& i_physValRectOther) const;
public: // instance methods
    bool isValid() const;
public: // instance methods
    CPhysValPoint center() const;
    CPhysValSize size() const;
    ZS::PhysVal::CPhysVal width() const;
    ZS::PhysVal::CPhysVal height() const;
    ZS::PhysVal::CPhysVal angle() const;
    CPhysValPoint topLeft() const;
    CPhysValPoint topRight() const;
    CPhysValPoint bottomRight() const;
    CPhysValPoint bottomLeft() const;
    CPhysValPoint topCenter() const;
    CPhysValPoint rightCenter() const;
    CPhysValPoint bottomCenter() const;
    CPhysValPoint leftCenter() const;
    double resolution() const;
    ZS::PhysVal::CUnit unit() const;
public: // instance methods
    void setCenter(const QPointF& i_pt);
    void setCenter(const CPhysValPoint& i_physValPoint);
    void setSize(const QSizeF& i_size);
    void setSize(const CPhysValSize& i_physValSize);
    void setWidth(double i_fWidth);
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void setWidthByMovingLeftCenter(const QPointF& i_pt);
    void setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint);
    void setWidthByMovingRightCenter(const QPointF& i_pt);
    void setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint);
    void setHeight(double i_fHeight);
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    void setHeightByMovingTopCenter(const QPointF& i_pt);
    void setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint);
    void setHeightByMovingBottomCenter(const QPointF& i_pt);
    void setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint);
    void setAngle(const ZS::PhysVal::CPhysVal& i_physValAngle);
    void setTopLeft(const QPointF& i_pt);
    void setTopLeft(const CPhysValPoint& i_physValPoint);
    void setTopRight(const QPointF& i_pt);
    void setTopRight(const CPhysValPoint& i_physValPoint);
    void setBottomRight(const QPointF& i_pt);
    void setBottomRight(const CPhysValPoint& i_physValPoint);
    void setBottomLeft(const QPointF& i_pt);
    void setBottomLeft(const CPhysValPoint& i_physValPoint);
public: // instance methods (to convert the values into another unit)
    QRectF toNotRotatedQRectF() const;
    QRectF toNotRotatedQRectF(const ZS::PhysVal::CUnit& i_unit) const;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ") const;
protected: // auxiliary math functions
    static double radius(const QSizeF& i_size);
    static double phi_rad(const QSizeF& i_size);
    void updateFromOppositeCorners(const CPhysValPoint& i_physValPtNew, const CPhysValPoint& i_physValPtOpposite);
protected: // instance members
    /*!< Reference to drawing scene. */
    const CDrawingScene* m_pDrawingScene;
    /*!< The rectangles center coordinates in the unit 'm_unit'. */
    QPointF m_ptCenter;
    /*!< The rectangles width and height in the unit 'm_unit'. */
    QSizeF m_size;
    /*!< The rotation angle. */
    ZS::PhysVal::CPhysVal m_physValAngle;
    /*!< Unit (either metric or pixels) in which the rectangle coordinates are in 'm_rect'. */
    ZS::PhysVal::CUnit m_unit;

}; // class CPhysValRect

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValRect_h
