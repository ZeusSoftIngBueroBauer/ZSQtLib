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

#ifndef ZSDraw_GraphObjLine_h
#define ZSDraw_GraphObjLine_h

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
class CGraphObjSelectionPoint;

//******************************************************************************
class ZSDRAWDLL_API CGraphObjLine : public CGraphObj, public QGraphicsLineItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjLine"; }
public: // ctors and dtor
    CGraphObjLine(
        CDrawingScene* i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString& i_strObjName,
        const CPhysValPoint& i_physValP1,
        const CPhysValPoint& i_physValP2 );
    virtual ~CGraphObjLine();
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // overridables of base class CGraphObj (for subsystem test)
    virtual QString getScenePolygonShapePointsString() const override;
public: // overridables of base class CGraphObj
    virtual void onCreateAndExecDlgFormatGraphObjs() override;
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // instance methods
    void setLine(const CPhysValLine& i_physValLine);
    void setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2, const ZS::PhysVal::CUnit& i_unit);
    void setLine(const QPointF& i_p1, const QPointF& i_p2, const ZS::PhysVal::CUnit& i_unit);
    void setLine(const QLineF& i_line, const ZS::PhysVal::CUnit& i_unit);
    void setLine(const CPhysValPoint& i_physValP1, const CPhysValPoint& i_physValP2);
    CPhysValLine getLine(const ZS::PhysVal::CUnit& i_unit) const;
    CPhysValPoint getP1(const ZS::PhysVal::CUnit& i_unit) const;
    CPhysValPoint getP2(const ZS::PhysVal::CUnit& i_unit) const;
public: // must overridables of base class CGraphObj
    virtual void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth) override;
    virtual void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight) override;
    virtual void setSize(const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight) override;
    virtual void setSize(const CPhysValSize& i_physValSize) override;
    virtual bool hasBoundingRect() const override;
    virtual bool hasLineShapePoints() const override;
    virtual bool hasRotationSelectionPoints() const override;
public: // overridables of base class CGraphObj
    virtual CPhysValPoint getPos(const ZS::PhysVal::CUnit& i_unit, ECoordinatesVersion i_version = ECoordinatesVersion::Transformed) const override;
    virtual ZS::PhysVal::CPhysVal getWidth(const ZS::PhysVal::CUnit& i_unit, ECoordinatesVersion i_version = ECoordinatesVersion::Transformed) const override;
    virtual ZS::PhysVal::CPhysVal getHeight(const ZS::PhysVal::CUnit& i_unit, ECoordinatesVersion i_version = ECoordinatesVersion::Transformed) const override;
    virtual CPhysValSize getSize(const ZS::PhysVal::CUnit& i_unit, ECoordinatesVersion i_version = ECoordinatesVersion::Transformed) const override;
public: // must overridables of base class CGraphObj
    virtual void setIsHit(bool i_bHit) override;
public: // overridables of base class CGraphObj
    virtual bool isHit(const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr) const override;
public: // reimplementing methods of base class QGraphicItem
    void setCursor( const QCursor& cursor );
public: // overridables of base class CGraphObj
    virtual QPointF getSelectionPointCoors(const SGraphObjSelectionPoint& i_selPt) const override;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints(unsigned char i_selPts = ESelectionPointsAll) override;
public: // overridables of base class CGraphObj
    virtual QList<SGraphObjSelectionPoint> getPossibleLabelAnchorPoints(const QString& i_strName) const override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr) override;
protected: // overridables of base class QGraphicsItem
    virtual bool sceneEventFilter(QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange(GraphicsItemChange i_change, const QVariant& i_value) override;
protected: // instance methods
    virtual bool lineEndArrowHeadPolygonsNeedUpdate(const CEnumLinePoint& i_linePoint, const CDrawSettings& i_drawSettingsOld) const;
    virtual void updateLineEndArrowHeadPolygons(const CEnumLinePoint& i_linePoint = CEnumLinePoint());
protected: // overridables of base class CGraphObj
    virtual void onDrawingSizeChanged(const CDrawingSize& i_drawingSize) override;
    virtual void updateToolTip() override;
protected: // auxiliary instance methods (method tracing)
    void setPhysValLine(const CPhysValLine& i_physValLine);
    void QGraphicsLineItem_setLine(const QLineF& i_line);
    void QGraphicsLineItem_setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2);
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    void traceInternalStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;
protected: // instance members
    /*!< The original, untransformed line coordinates with unit.
         The coordinates are relative to the parent item.
         If the item does not have another graphical object as a 
         parent, the coordinates are scene coordinates. */
    CPhysValLine m_physValLine;
    /*!< Polygon points for arrow head at P1 (line start) */
    QPolygonF m_plgP1ArrowHead;
    /*!< Polygon points for arrow head at P2 (line end) */
    QPolygonF m_plgP2ArrowHead;

}; // class CGraphObjLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLine_h
