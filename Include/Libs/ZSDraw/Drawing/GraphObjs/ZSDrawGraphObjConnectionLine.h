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
class CDrawSettings;
class CGraphObjConnectionPoint;
class CGraphObjSelectionPoint;

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
    CGraphObjConnectionLine(
        CDrawingScene* i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString& i_strObjName = "");
    virtual ~CGraphObjConnectionLine();
public: // overridables of base class QGraphicsItem
    virtual int type() const { return static_cast<int>(EGraphObjTypeConnectionLine); }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
public: // overridables
    virtual bool setConnectionPoint( ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pCnctPt ); // Connects the specified line point with the specified connection point by appending the line to the connection point. Returns false if the line is already connected with the specified connection point.
    virtual ELinePoint getConnectionLinePoint( CGraphObjConnectionPoint* i_pCnctPt );               // Returns the line point (start or end) which is connected to the specified connection point.
    virtual CGraphObjConnectionPoint* getConnectionPoint( ELinePoint i_linePoint );
public: // replacing methods of QGraphicsRectItem
    void setPolygon( const QPolygonF& i_plg );
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const override; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettings) override;
public: // overridables of base class CGraphObj
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    virtual void acceptCurrentAsOriginalCoors() override;
#endif
public: // must overridables of base class CGraphObj
    virtual void setWidth( const ZS::PhysVal::CPhysVal& i_physValWidth ) override;
    virtual void setHeight( const ZS::PhysVal::CPhysVal& i_physValHeight ) override;
    virtual void setSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight ) override;
    virtual void setSize( const CPhysValSize& i_physValSize ) override;
    virtual bool hasBoundingRect() const  override { return false; }
    virtual bool hasLineShapePoints() const  override { return true; }
    virtual bool hasRotationSelectionPoints() const  override { return true; }
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit ) override;
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr ) const override;
public: // reimplementing methods of base class QGraphicItem
    void setCursor( const QCursor& cursor );
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
public: // overridables of base class CGraphObj
    virtual void onParentItemCoorsHasChanged( CGraphObj* i_pGraphObjParent ) override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    virtual bool sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv ) override;
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
protected: // instance methods
    virtual void normalize(); // removes "unnecessary" points
    virtual void updateLineEndPolygonCoors();
protected: // class members
    /*!< Needed to set an initial unique name when creating a new instance. */
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

}; // class CGraphObjConnectionLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjConnectionLine_h
