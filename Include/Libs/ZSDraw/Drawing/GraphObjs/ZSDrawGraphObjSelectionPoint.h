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

#ifndef ZSDraw_GraphObjSelectionPoint_h
#define ZSDraw_GraphObjSelectionPoint_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsLineItem>
#else
#include <QtWidgets/QGraphicsLineItem>
#endif

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawSettings;

//******************************************************************************
// Please note that selection points should not belong as child to the graphics items
// for which the selection points are created. Otherwise the "boundingRect" call
// of groups (which implicitly calls childrenBoundingRect) does not work as the
// selection points of the bounding rectangle would be included.
class ZSDRAWDLL_API CGraphObjSelectionPoint : public CGraphObj, public QGraphicsEllipseItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjSelectionPoint"; }
public: // class methods
    static double GetDefaultRadiusInPx() { return s_fRadius_px; }
public: // ctors and dtor
    CGraphObjSelectionPoint(
        CDrawingScene* i_pDrawingScene,
        CGraphObj* i_pGraphObjSelected,
        ESelectionPoint i_selectionPoint );
    CGraphObjSelectionPoint(
        CDrawingScene* i_pDrawingScene,
        CGraphObj* i_pGraphObjSelected,
        int i_idxPt );
    virtual ~CGraphObjSelectionPoint();
public: // overridables of base class QGraphicsItem
    int type() const override { return static_cast<int>(EGraphObjTypeSelectionPoint); }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // instance methods
    ESelectionPointType getSelectionPointType() const { return m_selPtType.enumerator(); }
public: // instance methods
    CGraphObj* getSelectedGraphObj() { return m_pGraphObjSelected; }
    void setSelectionPoint( ESelectionPoint i_selPt );
    ESelectionPoint getSelectionPoint() const { return m_selPt.enumerator(); }
    void setShapePointIndex( int i_idxPt );
    int getShapePointIndex() const { return m_idxPt; }
public: // instance methods
    void setRadiusInPx( double i_fRadius_px );
    double getRadiusInPx() { return m_fRadius_px; }
public: // instance methods (replacing the methods of base class QGraphicsItem)
    void setSelected( bool i_bSelected );
    bool isSelected() const { return m_bSelected; }
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const override; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // must overridables of base class CGraphObj
    virtual void setWidth( const ZS::PhysVal::CPhysVal& i_physValWidth ) override;
    virtual void setHeight( const ZS::PhysVal::CPhysVal& i_physValHeight ) override;
    virtual void setSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight ) override;
    virtual void setSize( const CPhysValSize& i_physValSize ) override;
    virtual bool hasBoundingRect() const override { return false; }
    virtual bool hasLineShapePoints() const override { return false; }
    virtual bool hasRotationSelectionPoints() const override { return false; }
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool /*i_bHit*/ ) override {}
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr ) const override;
public: // reimplementing methods of base class QGraphicItem
    void setCursor( const QCursor& cursor );
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override { i_selPts = i_selPts; }
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override { i_selPts = i_selPts; }
protected: // overridables of base class CGraphObj
    virtual void updateToolTip() override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value ) override;
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    void traceInternalStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
protected: // class members
    /*!< Needed to set an initial unique name when creating a new instance. */
    static qint64 s_iInstCount;
protected: // class members
    static double s_fRadius_px;
protected: // instance members
    CGraphObj* m_pGraphObjSelected;
    CEnumSelectionPointType m_selPtType;
    CEnumSelectionPoint m_selPt;
    int m_idxPt;
    double m_fRadius_px;
    bool m_bSelected;

}; // class CGraphObjSelectionPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjSelectionPoint_h
