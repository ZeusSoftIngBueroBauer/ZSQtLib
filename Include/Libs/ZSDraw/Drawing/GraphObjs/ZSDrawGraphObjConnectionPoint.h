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

#ifndef ZSDraw_GraphObjConnectionPoint_h
#define ZSDraw_GraphObjConnectionPoint_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsEllipseItem>
#else
#include <QtWidgets/QGraphicsEllipseItem>
#endif

namespace ZS
{
namespace Draw
{
class CGraphObjConnectionLine;

//******************************************************************************
class ZSDRAWDLL_API CGraphObjConnectionPoint : public CGraphObj, public QGraphicsEllipseItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjConnectionPoint"; }
public: // class methods
    static QPainter::RenderHints painterRenderHints();
    static void setPainterRenderHints(QPainter::RenderHints i_renderHints);
    static void resetPainterRenderHints();
public: // class methods
    static double defaultWidthInPx();
    static QSizeF defaultSizeInPx();
public: // ctors and dtor
    CGraphObjConnectionPoint(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
    ~CGraphObjConnectionPoint() override;
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    CGraphObj* clone() override;
public: // must overridables of base class CGraphObj
    void openFormatGraphObjsDialog() override;
public: // instance methods
    QString pathNameOfLinkedObject() const;
    QString myPathName() const;
public: // instance methods
    SGraphObjSelectionPoint getSelectionPoint() const;
public: // overridables of base class CGraphObj
    void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // instance methods
    void appendConnectionLine(CGraphObjConnectionLine* i_pGraphObjCnctLine); // appends the specified connection line to the list of connection lines. Return false if the line is already connected with the connection point.
    void removeConnectionLine(CGraphObjConnectionLine* i_pGraphObjCnctLine);
    int getConnectionLineIdx(CGraphObjConnectionLine* i_pGraphObjCnctLine);
    int getConnectionLinesCount() const;
    CGraphObjConnectionLine* getConnectionLine( int i_iLineIdx );
public: // instance methods
    void setRect(const CPhysValRect& i_physValRect);
    void setRect(const QPointF& i_pTL, const QPointF& i_pBR, const ZS::PhysVal::CUnit& i_unit);
    void setRect(const QPointF& i_pTL, const QSizeF& i_size, const ZS::PhysVal::CUnit& i_unit);
    void setRect(const CPhysValPoint& i_physValTL, const CPhysValPoint& i_physValBR);
    void setRect(const CPhysValPoint& i_physValTL, const CPhysValSize& i_physValSize);
    CPhysValRect getRect() const;
    CPhysValRect getRect(const ZS::PhysVal::CUnit& i_unit) const;
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
    void setPosition(const CPhysValPoint& i_physValPos) override;
    void setRotationAngle(double i_fAngle_degree) override;
    void setRotationAngle(const ZS::PhysVal::CPhysVal& i_physValAngle) override;
public: // must overridables of base class CGraphObj
    QRectF getBoundingRect() const override;
    CPhysValRect getPhysValBoundingRect(const ZS::PhysVal::CUnit& i_unit) const override;
protected: // must overridables of base class CGraphObj
    void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll) override;
public: // overridables of base class CGraphObj (text labels)
    QList<SGraphObjSelectionPoint> getPossibleLabelAnchorPoints(const QString& i_strName) const override;
    bool labelHasDefaultValues(const QString& i_strName) const override;
public: // overridables of base class CGraphObj (geometry labels)
    bool geometryLabelHasDefaultValues(const QString& i_strName) const override;
public: // must overridables of base class QGraphicsItem
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr) override;
protected: // overridables of base class QGraphicsItem
    void hoverEnterEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    QVariant itemChange(GraphicsItemChange i_change, const QVariant& i_value) override;
protected: // overridable slots of base class CGraphObj
    void onGraphObjParentGeometryOnSceneChanged(CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged = false) override;
    void onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint) override;
public: // must overridables of base class CGraphObj
    void updateTransformedCoorsOnParentChanged(CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew) override;
    void updateTransformedCoorsOnParentGeometryChanged() override;
    void updateTransformedCoorsOnItemPositionChanged() override;
protected: // auxiliary instance methods
    QRectF getRectScaled(const QRectF& i_rectOrig) const;
    CPhysValRect getPhysValRectOrig(const QRectF& i_rectOrig) const;
    CPhysValRect getPhysValRectScaled(const CPhysValRect& i_physValRectOrig) const;
protected: // auxiliary instance methods (method tracing)
    QRectF setRectOrig(const QRectF& i_rect);
    QRectF QGraphicsEllipseItem_setRect(const QRectF& i_rect);
    QRectF QGraphicsEllipseItem_setRect(double i_fX, double i_fY, double i_fWidth, double i_fHeight);
    CPhysValRect setPhysValRectOrig(const CPhysValRect& i_physValRect);
    CPhysValRect setPhysValRectScaled(const CPhysValRect& i_physValRect);
    CPhysValRect setPhysValRectScaledAndRotated(const CPhysValRect& i_physValRect);
    void QGraphicsItem_prepareGeometryChange() override;
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    void traceThisPositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        const QString& i_strFilter = "",
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
    void traceGraphObjStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        const QString& i_strFilter = "",
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
protected: // auxiliary instance methods (method tracing)
    void traceConnectionLines(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined) const;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;
protected: // class members
    static QPainter::RenderHints s_painterRenderHints;
    static double s_fDefaultWidth_px;
protected: // instance members
    /*!< Defines the type of the selecton point, the linked object and the position at the linked
         object the selection point is linked to. */
    SGraphObjSelectionPoint m_selPt;
    /*!< List with connection lines linked to the connection point. */
    QList<CGraphObjConnectionLine*> m_lstConnectionLines;
    /*!< The original, untransformed (not scaled, not rotated) rectangle coordinates in local
         coordinates relative to the origin of the item's bounding rectangle.
         This member is set if any shape point is directly set via the method call "setRect"
         (which is implicitly called by all other methods modifying shape points) or at the
         time the item is added to a group.
         The rotation angle of the graphics item is separately stored in
         m_physValRotationAngle of base class CGraphObj.
         The scale factors are also stored separately in the members
         m_fParentGroupScaleX and m_fParentGroupScaleY of the base class CGraphObj.
         @see base class CGraphObj "Current and Original Coordinates". */
    QRectF m_rectOrig;
    /*!< The original, untransformed (not scaled, not rotated) rectangle coordinates with unit
         in parent coordinates relative to the top left or bottom left corner of the parent.
         The rotation angle of the graphics item is separately stored in
         m_physValRotationAngle of base class CGraphObj.
         The scale factors are also stored separately in the members
         m_fParentGroupScaleX and m_fParentGroupScaleY of the base class CGraphObj.
         @see base class CGraphObj "Current and Original Coordinates". */
    CPhysValRect m_physValRectOrig;
    /*!< The scaled but not rotated rectangle coordinates with unit in parent coordinates
         relative to the top left or bottom left corner of the parent.
         The scaled rectangle is returned by the "getBoundingRect" method
         to provide the bounding rectangle of the group in the current unit. */
    CPhysValRect m_physValRectScaled;
    /*!< The scaled and rotated rectangle coordinates with unit in parent coordinates
         relative to the top left or bottom left corner of the parent.
         The scaled and rotated rectangle is returned by the "getRect" method
         (and all other methods retrieving the resulting coordinates in the current unit). */
    CPhysValRect m_physValRectScaledAndRotated;
    /*!< Counter to block debug trace outputs for connection lines. */
    int m_iTraceConnectionLinesBlockedCounter = 0;

}; // class CGraphObjConnectionPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjConnectionPoint_h
