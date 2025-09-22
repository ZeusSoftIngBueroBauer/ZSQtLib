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

#ifndef ZSDraw_GraphObjConnectionLine_h
#define ZSDraw_GraphObjConnectionLine_h

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
class CGraphObjConnectionPoint;

//******************************************************************************
class ZSDRAWDLL_API CGraphObjConnectionLine : public CGraphObj, public QGraphicsPolygonItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjConnectionLine"; }
public: // class methods
    static QPainter::RenderHints painterRenderHints();
    static void setPainterRenderHints(QPainter::RenderHints i_renderHints);
    static void resetPainterRenderHints();
public: // class methods
    static QString createPolygonPointLabelName(int i_idxPt);
    static bool isPolygonPointLabelName(const QString& i_strLabelName);
    static int extractIndexFromPolygonPointLabelName(const QString& i_strLabelName);
public: // ctors and dtor
    CGraphObjConnectionLine(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
    virtual ~CGraphObjConnectionLine();
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // must overridables of base class CGraphObj
    void createContextMenu() override;
    void showContextMenu(QGraphicsSceneMouseEvent* i_pEv) override;
    void openFormatGraphObjsDialog() override;
public: // overridables of base class CGraphObj
    void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // overridables
    virtual void setConnectionPoint(ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pGraphObjCnctPt);
    virtual ELinePoint getConnectionLinePoint(CGraphObjConnectionPoint* i_pGraphObjCnctPt) const;
    virtual CGraphObjConnectionPoint* getConnectionPoint(ELinePoint i_linePoint) const;
public: // instance methods
    void setPolygon(const CPhysValPolygon& i_physValPolygon);
    void setPolygon(const QPolygonF& i_polygon, const ZS::PhysVal::CUnit& i_unit);
    CPhysValPolygon getPolygon() const;
    CPhysValPolygon getPolygon(const ZS::PhysVal::CUnit& i_unit) const;
public: // instance methods
    bool isEmpty() const;
    int count() const;
    CPhysValPoint at(int i_idx) const;
    CPhysValPoint first() const;
    CPhysValPoint last() const;
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
public: // must overridables of base class CGraphObj
    QRectF getBoundingRect() const override;
    CPhysValRect getPhysValBoundingRect(const ZS::PhysVal::CUnit& i_unit) const override;
public: // overridables of base class CGraphObj
    QCursor getProposedCursor(const QPointF& i_pt) const override;
public: // overridables of base class CGraphObj
    CPhysValPoint getPositionOfSelectionPoint(ESelectionPointType i_selPtType, int i_idxPt, const ZS::PhysVal::CUnit& i_unit) const override;
    QPointF getPositionOfSelectionPointInSceneCoors(ESelectionPointType i_selPtType, int i_idxPt) const override;
public: // must overridables of base class CGraphObj
    SPolarCoors getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPointType i_selPtType, int i_idxPt) const override;
    QLineF getAnchorLineToSelectionPointFromPolarInSceneCoors(const SPolarCoors& i_polarCoors, ESelectionPointType i_selPtType, int i_idxPt) const override;
protected: // must overridables of base class CGraphObj
    void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll) override;
protected: // overridables of base class CGraphObj
    void showSelectionPointsOfPolygon(const QPolygonF& i_plg) override;
public: // overridables of base class CGraphObj (text labels)
    QList<SGraphObjSelectionPoint> getPossibleLabelAnchorPoints(const QString& i_strName) const override;
    bool labelHasDefaultValues(const QString& i_strName) const override;
public: // overridables of base class CGraphObj (geometry labels)
    bool geometryLabelHasDefaultValues(const QString& i_strName) const override;
public: // must overridables of base class QGraphicsItem
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
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
protected slots:
    void onGraphObjConnectionPointGeometryOnSceneChanged(CGraphObj* i_pGraphObjCnctPt, bool );
    //void onGraphObjConnectionPointZValueChanged(CGraphObj* i_pGraphObjCnctPt);
protected: // overridable slots of base class CGraphObj
    void onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint) override;
protected slots:
    void onActionModifyPointsTriggered();
    void onActionDeletePointTriggered();
public: // must overridables of base class CGraphObj
    void updateTransformedCoorsOnParentChanged(CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew) override;
    void updateTransformedCoorsOnParentGeometryChanged() override;
    void updateTransformedCoorsOnItemPositionChanged() override;
protected: // auxiliary instance methods
    bool lineEndArrowHeadPolygonsNeedUpdate(const CEnumLinePoint& i_linePoint, const CDrawSettings& i_drawSettingsOld) const;
    void updateLineEndArrowHeadPolygons(const CEnumLinePoint& i_linePoint = CEnumLinePoint());
    QPolygonF getPolygonScaled(const QPolygonF& i_polygonOrig) const;
    CPhysValPolygon getPhysValPolygonOrig(const QPolygonF& i_polygonOrig) const;
    CPhysValPolygon getPhysValPolygonScaled(const CPhysValPolygon& i_physValPolygonOrig) const;
    void normalize(); // removes "unnecessary" points
protected: // auxiliary instance methods
    int getNumberOfPredefinedPolygonPointLabelNames() const;
    int getNumberOfPolygonPointGeometryLabelNames() const;
    void updateLabelsOnPolygonPointsAdded();
    void updateLabelsOnPolygonPointsRemoved();
protected: // auxiliary instance methods (method tracing)
    QPolygonF setPolygonOrig(const QPolygonF& i_polygon);
    QPolygonF QGraphicsPolygonItem_setPolygon(const QPolygonF& i_polygon);
    CPhysValPolygon setPhysValPolygonOrig(const CPhysValPolygon& i_physValPolygon);
    CPhysValPolygon setPhysValPolygonScaledAndRotated(const CPhysValPolygon& i_physValPolygon);
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    void QGraphicsItem_prepareGeometryChange() override;
    void traceThisPositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        const QString& i_strFilter = "") const override;
protected: // auxiliary instance methods (method tracing)
    void traceConnectionPoints(
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
protected: // instance members
    QAction* m_pActionMenuContextModifyPoints = nullptr;
    QAction* m_pActionMenuContextDeletePoint = nullptr;
protected: // instance members
    /*!< Map with connection points of the connection line.
         The map has two elements: Start and End point. */
    QMap<ELinePoint, CGraphObjConnectionPoint*> m_arpCnctPts;
    QPolygonF m_polygonOrig;
    CPhysValPolygon m_physValPolygonOrig;
    CPhysValPolygon m_physValPolygonScaledAndRotated;
    QPolygonF m_plgLineStartArrowHead;
    QPolygonF m_plgLineEndArrowHead;
    /*!< On inserting points the polygon is retrieved, modified and afterwards set by invoking "setPolygon".
         The indices of the inserted points are stored before invoking setPolygon and cleared afterwards.
         This way the setPolygon method can update the labels correspondingly.
         The first element contains the index of the first inserted point,
         the second element the number of inserted points.
         Please note that points may not be added or removed at the same time by the same "setPolygon" call
         and either m_idxsAdded or m_idxsRemoved may refer to valid element indices but not both at the same time. */
    QPair<int, int> m_idxsAdded;
    /*!< On removing points the polygon is retrieved, modified and afterwards set by invoking "setPolygon".
         The indices of the removed points are stored before invoking setPolygon and cleared afterwards.
         This way the setPolygon method can update the labels correspondingly.
         The first element contains the index of the first removed point,
         the second element the number of removed points.
         Please note that points may not be added or removed at the same time by the same "setPolygon" call
         and either m_idxsAdded or m_idxsRemoved may refer to valid element indices but not both at the same time. */
    QPair<int, int> m_idxsRemoved;
    /*!< Counter to block debug trace outputs for connection points. */
    int m_iTraceConnectionPointsBlockedCounter = 0;

}; // class CGraphObjConnectionLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjConnectionLine_h
