/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsPolygonItem>
#else
#include <QtWidgets/QGraphicsPolygonItem>
#endif

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawGraphObj.h"

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
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return NameSpace(); } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // class methods
    static int getCurrentInstCount() { return s_iInstCount; }
public: // ctors and dtor
    CGraphObjConnectionLine(
        CDrawingScene*       i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString&       i_strObjName = "");
    virtual ~CGraphObjConnectionLine();
public: // overridables of base class QGraphicsItem
    virtual int type() const { return static_cast<int>(EGraphObjTypeConnectionLine); }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // overridables
    virtual bool setConnectionPoint( ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pCnctPt ); // Connects the specified line point with the specified connection point by appending the line to the connection point. Returns false if the line is already connected with the specified connection point.
    virtual ELinePoint getConnectionLinePoint( CGraphObjConnectionPoint* i_pCnctPt );               // Returns the line point (start or end) which is connected to the specified connection point.
    virtual CGraphObjConnectionPoint* getConnectionPoint( ELinePoint i_linePoint );
public: // replacing methods of QGraphicsRectItem
    void setPolygon( const QPolygonF& i_plg );
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged();
public: // overridables of base class CGraphObj
    virtual void acceptCurrentAsOriginalCoors();
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
public: // overridables of base class CGraphObj
    virtual void onParentItemCoorsHasChanged( CGraphObj* i_pGraphObjParent );
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
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
protected: // instance methods
    virtual void normalize(); // removes "unnecessary" points
    virtual void updateLineEndPolygonCoors();
protected: // class members
    static qint64 s_iInstCount;
protected: // instance members
    bool                               m_bCoorsDirty;
    QPolygonF                          m_plgCurr;        // First and lasst point may differ from polygon() depending on line end base line types
    QPolygonF                          m_plgOrig;        // First and lasst point may differ from polygon() depending on line end base line types
    QPolygonF                          m_plgLineStart;
    QPolygonF                          m_plgLineEnd;
    QVector<CGraphObjConnectionPoint*> m_arpCnctPts;

}; // class CGraphObjConnectionLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjConnectionLine_h
