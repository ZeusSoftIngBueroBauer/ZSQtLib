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
public: // ctors and dtor
    CGraphObjConnectionLine(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
    virtual ~CGraphObjConnectionLine();
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // overridables
    virtual bool setConnectionPoint( ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pCnctPt );
    virtual ELinePoint getConnectionLinePoint( CGraphObjConnectionPoint* i_pCnctPt ) const;
    virtual CGraphObjConnectionPoint* getConnectionPoint( ELinePoint i_linePoint ) const;
public: // replacing methods of QGraphicsRectItem
    void setPolygon(const CPhysValPolygon& i_physValPolygon);
    void setPolygon(const QPolygonF& i_polygon, const ZS::PhysVal::CUnit& i_unit);
    CPhysValPolygon getPolygon() const;
    CPhysValPolygon getPolygon(const ZS::PhysVal::CUnit& i_unit) const;
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettings) override;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll) override;
public: // overridables of base class CGraphObj
    virtual void onGraphObjParentGeometryOnSceneChanged(CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged = false) override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value ) override;
protected: // instance methods
    virtual void normalize(); // removes "unnecessary" points
    virtual void updateLineEndPolygonCoors();
protected: // auxiliary instance methods (method tracing)
    void QGraphicsItem_prepareGeometryChange() override;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;
protected: // instance members
    bool m_bCoorsDirty;
    // First and lasst point may differ from polygon() depending on line end base line types
    QPolygonF m_plgCurr;
    // First and lasst point may differ from polygon() depending on line end base line types
    QPolygonF m_plgOrig;
    QPolygonF m_plgLineStart;
    QPolygonF m_plgLineEnd;
    QVector<CGraphObjConnectionPoint*> m_arpCnctPts;
protected: // instance members
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

}; // class CGraphObjConnectionLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjConnectionLine_h
