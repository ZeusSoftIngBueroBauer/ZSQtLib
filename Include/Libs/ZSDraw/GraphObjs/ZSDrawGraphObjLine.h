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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsLineItem>
#else
#include <QtWidgets/QGraphicsLineItem>
#endif

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"

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
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjLine"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // ctors and dtor
    CGraphObjLine(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString&       i_strObjName,
        const QPointF&       i_pt1,
        const QPointF&       i_pt2 );
    virtual ~CGraphObjLine();
public: // overridables of base class QGraphicsItem
    virtual int type() const override { return EGraphObjTypeLine; }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override ;
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // replacing methods of QGraphicsLineItem
    void setLine( const QLineF& i_lin );
    void setLine( qreal i_x1, qreal i_y1, qreal i_x2, qreal i_y2 );
    QLineF getLine() const;
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const override ; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged() override ;
public: // must overridables of base class CGraphObj
    virtual void setWidth( double i_fWidth ) override ;
    virtual void setHeight( double i_fHeight ) override ;
    virtual void setSize( double i_fWidth, double i_fHeight ) override ;
    virtual void setSize( const QSizeF& i_size ) override ;
    virtual bool hasBoundingRect() const  override { return false; }
    virtual bool hasLineShapePoints() const  override { return false; }
    virtual bool hasRotationSelectionPoints() const  override { return false; }
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit ) override ;
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const override ;
public: // reimplementing methods of base class QGraphicItem
    void setCursor( const QCursor& cursor );
public: // overridables of base class CGraphObj
    virtual QPointF getSelectionPointCoors( ESelectionPoint i_selPt ) const override;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
public: // overridables of base class CGraphObj
    virtual QList<ESelectionPoint> getPossibleLabelAnchorPoints() const override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    virtual bool sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value ) override;
protected: // instance methods
    virtual void updateLineEndPolygonCoors();
protected: // overridables of base class CGraphObj
    virtual void updateToolTip() override;
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    void traceInternalStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
protected: // class members
    /*!< Needed to set an initial unique name when creating a new instance. */
    static qint64 s_iInstCount;
protected: // instance members
    /*!< Start point and polygon points for arrow heads */
    QPolygonF m_plgLineStart;
    /*!< End point and polygon points for arrow heads */
    QPolygonF m_plgLineEnd;

}; // class CGraphObjLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLine_h
