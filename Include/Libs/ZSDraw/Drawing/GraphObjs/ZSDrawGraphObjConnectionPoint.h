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
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawSettings;
class CGraphObjConnectionLine;
class CGraphObjSelectionPoint;

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
    static double GetDefaultOuterCircleRadiusInPx() { return s_fOuterCircleRadius_px; }
    static double GetDefaultInnerCircleRadiusInPx() { return s_fInnerCircleRadius_px; }
public: // ctors and dtor
    CGraphObjConnectionPoint(
        CDrawingScene* i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString& i_strObjName = "");
    virtual ~CGraphObjConnectionPoint();
public: // overridables of base class QGraphicsItem
    virtual int type() const override { return static_cast<int>(EGraphObjTypeConnectionPoint); }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // overridables
    virtual bool appendConnectionLine( CGraphObjConnectionLine* i_pGraphObjCnctLine ); // appends the specified connection line to the list of connection lines. Return false if the line is already connected with the connection point.
    virtual void removeConnectionLine( CGraphObjConnectionLine* i_pGraphObjCnctLine );
    virtual int findConnectionLineIdx( CGraphObjConnectionLine* i_pGraphObjCnctLine );
    virtual int getConnectionLinesCount() const;
    virtual CGraphObjConnectionLine* getConnectionLine( int i_iLineIdx );
public: // replacing methods of QGraphicsEllipseItem
    void setRect( const QRectF& i_rct );
    void setRect( qreal i_x, qreal i_y, qreal i_width, qreal i_height );
public: // overridables
    void setInnerCircleWidthInPerCent( double i_fWidth_perCent );
    void setInnerCircleWidthInPx( double i_fWidth_px );
    double getInnerCircleWidthInPerCent();
    double getInnerCircleWidthInPx();
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
    virtual bool hasRotationSelectionPoints() const override { return true; }
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit ) override;
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr ) const override;
public: // reimplementing methods of base class QGraphicItem
    void setCursor( const QCursor& cursor );
public: // overridables of base class CGraphObj
    virtual QPointF getSelectionPointCoors( ESelectionPoint i_selPt ) const override; // returns coordinates of selection point in item's coordinate system
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override { i_selPts = i_selPts; }
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override { i_selPts = i_selPts; }
public: // overridables of base class CGraphObj
    virtual void onParentItemCoorsHasChanged( CGraphObj* i_pGraphObjParent ) override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    virtual bool sceneEvent( QEvent* i_pEv ) override;
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
protected: // class members
    /*!< Needed to set an initial unique name when creating a new instance. */
    static qint64 s_iInstCount;
protected: // class members
    static double s_fOuterCircleRadius_px;
    static double s_fInnerCircleRadius_px;
protected: // instance members
    QList<CGraphObjConnectionLine*> m_lstConnectionLines;
    double                          m_fInnerCircleWidth_perCent;

}; // class CGraphObjConnectionPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjConnectionPoint_h
