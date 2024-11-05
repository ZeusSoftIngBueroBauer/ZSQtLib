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

#ifndef ZSDraw_GraphObjPolyline_h
#define ZSDraw_GraphObjPolyline_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Common/ZSDrawPhysValPolygon.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsPolygonItem>
#else
#include <QtWidgets/QGraphicsPolygonItem>
#endif

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CGraphObjPolyline : public CGraphObj, public QGraphicsPolygonItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjPolyline"; }
public: // class methods
    static QPainter::RenderHints painterRenderHints();
    static void setPainterRenderHints(QPainter::RenderHints i_renderHints);
    static void resetPainterRenderHints();
public: // ctors
    CGraphObjPolyline(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
protected: // ctor (used by derived classes, e.g. CGraphObjPolygon)
    CGraphObjPolyline(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strFactoryGroupName,
        EGraphObjType i_type,
        const QString& i_strObjName);
    void initInstance();
public: // dtor
    virtual ~CGraphObjPolyline();
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // must overridables of base class CGraphObj
    virtual void onCreateAndExecDlgFormatGraphObjs() override;
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // instance methods
    void setPolyline(const CPhysValPolyline& i_physValPolyline);
    void setPolyline(const QPolygonF& i_polyline, const ZS::PhysVal::CUnit& i_unit);
    CPhysValPolyline getPolyline() const;
    CPhysValPolyline getPolyline(const ZS::PhysVal::CUnit& i_unit) const;
    void setCenter(const QPointF& i_pt);
    void setCenter(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getCenter() const;
    CPhysValPoint getCenter(const ZS::PhysVal::CUnit& i_unit) const;
    void setSize(const QSizeF& i_size);
    void setSize(const CPhysValSize& i_physValSize);
    CPhysValSize getSize() const;
    CPhysValSize getSize(const ZS::PhysVal::CUnit& i_unit) const;
    void setWidth(double i_fWidth);
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void setWidthByMovingLeftCenter(const QPointF& i_pt);
    void setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint);
    void setWidthByMovingRightCenter(const QPointF& i_pt);
    void setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint);
    ZS::PhysVal::CPhysVal getWidth() const;
    ZS::PhysVal::CPhysVal getWidth(const ZS::PhysVal::CUnit& i_unit) const;
    void setHeight(double i_fHeight);
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    void setHeightByMovingTopCenter(const QPointF& i_pt);
    void setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint);
    void setHeightByMovingBottomCenter(const QPointF& i_pt);
    void setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint);
    ZS::PhysVal::CPhysVal getHeight() const;
    ZS::PhysVal::CPhysVal getHeight(const ZS::PhysVal::CUnit& i_unit) const;
    void setTopLeft(const QPointF& i_pt);
    void setTopLeft(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getTopLeft() const;
    CPhysValPoint getTopLeft(const ZS::PhysVal::CUnit& i_unit) const;
    void setTopRight(const QPointF& i_pt);
    void setTopRight(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getTopRight() const;
    CPhysValPoint getTopRight(const ZS::PhysVal::CUnit& i_unit) const;
    void setBottomRight(const QPointF& i_pt);
    void setBottomRight(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getBottomRight() const;
    CPhysValPoint getBottomRight(const ZS::PhysVal::CUnit& i_unit) const;
    void setBottomLeft(const QPointF& i_pt);
    void setBottomLeft(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getBottomLeft() const;
    CPhysValPoint getBottomLeft(const ZS::PhysVal::CUnit& i_unit) const;
public: // must overridables of base class CGraphObj
    virtual void setRotationAngle(double i_fAngle_degree) override;
    virtual void setRotationAngle(const ZS::PhysVal::CPhysVal& i_physValAngle) override;
public: // instance methods
    bool isEmpty() const;
    int count() const;
    CPhysValPoint at(int i_idx) const;
    void replace(int i_idx, const CPhysValPoint& i_physValPoint);
    void append(const CPhysValPoint& i_physValPoint);
    void insert(int i_idx, const CPhysValPoint& i_physValPoint);
    void remove(int i_idx, int i_iCount);
    void removeAt(int i_idx);
    void removeFirst();
    void removeLast();
    CPhysValPoint takeAt(int i_idx);
    CPhysValPoint takeFirst();
    CPhysValPoint takeLast();
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll);
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    //virtual bool sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv ) override;
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
protected: // auxiliary instance methods
    virtual bool lineEndArrowHeadPolygonsNeedUpdate(const CEnumLinePoint& i_linePoint, const CDrawSettings& i_drawSettingsOld) const;
    virtual void updateLineEndArrowHeadPolygons(const CEnumLinePoint& i_linePoint = CEnumLinePoint());
    QPointF getItemPosAndLocalCoors(const CPhysValPolyline& i_physValPolyline, QPolygonF& o_polyline) const;
    virtual void normalize(); // removes "unnecessary" points
protected: // auxiliary instance methods (method tracing)
    QPolygonF setPolylineOrig(const QPolygonF& i_polyline);
    QPolygonF QGraphicsPolygonItem_setPolygon(const QPolygonF& i_polyline);
    CPhysValPolyline setPhysValPolylineOrig(const CPhysValPolyline& i_physValPolyline);
    CPhysValPolyline setPhysValPolylineScaled(const CPhysValPolyline& i_physValPolyline);
    CPhysValPolyline setPhysValPolylineScaledAndRotated(const CPhysValPolyline& i_physValPolyline);
    void QGraphicsItem_prepareGeometryChange() override;
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    virtual void traceThisPositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;
protected: // class members
    static QPainter::RenderHints s_painterRenderHints;
protected: // instance members
    /*!< First and lasst point may differ from polygon() depending on line end base line types. */
    QPolygonF m_polylineOrig;
    CPhysValPolyline m_physValPolylineOrig;
    CPhysValPolyline m_physValPolylineScaled;
    CPhysValPolyline m_physValPolylineScaledAndRotated;
    QPolygonF m_plgLineStartArrowHead;
    QPolygonF m_plgLineEndArrowHead;
    /*!< In item's coordinate system (original size before editing (resizing) the object by mouse events) */
    //QPolygonF m_plgOnMousePressEvent;

}; // class CGraphObjPolyline

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjPolyline_h
