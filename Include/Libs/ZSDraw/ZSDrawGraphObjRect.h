/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_GraphObjRect_h
#define ZSDraw_GraphObjRect_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsRectItem>
#else
#include <QtWidgets/QGraphicsRectItem>
#endif

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawGraphObj.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawSettings;
class CGraphObjSelectionPoint;

//******************************************************************************
class ZSDRAWDLL_API CGraphObjRect : public CGraphObj, public QGraphicsRectItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjRect"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // ctors and dtor
    CGraphObjRect(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString&       i_strObjName = "" );
    virtual ~CGraphObjRect();
public: // overridables of base class QGraphicsItem
    virtual int type() const { return EGraphObjTypeRect; }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // replacing methods of QGraphicsRectItem
    void setRect( const QRectF& i_rct );
    void setRect( qreal i_x, qreal i_y, qreal i_width, qreal i_height );
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged();
public: // must overridables of base class CGraphObj
    virtual void setWidth( double i_fWidth );
    virtual void setHeight( double i_fHeight );
    virtual void setSize( double i_fWidth, double i_fHeight );
    virtual void setSize( const QSizeF& i_size );
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit );
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll );
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll );
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr );
protected: // overridables of base class QGraphicsItem
    virtual bool sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv );
protected: // overridables of base class QGraphicsItem
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv );
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv );
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv );
protected: // overridables of base class QGraphicsItem
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv );
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv );
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value );
protected: // class members
    static qint64 s_iInstCount;

}; // class CGraphObjRect

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjRect_h