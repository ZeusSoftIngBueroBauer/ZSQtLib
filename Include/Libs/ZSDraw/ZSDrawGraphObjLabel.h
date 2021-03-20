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

#ifndef ZSDraw_GraphObjLabel_h
#define ZSDraw_GraphObjLabel_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSimpleTextItem>
#else
#include <QtWidgets/QGraphicsSimpleTextItem>
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

//******************************************************************************
class ZSDRAWDLL_API CGraphObjLabel : public CGraphObj, public QGraphicsSimpleTextItem
//******************************************************************************
{
// Please note that labels should not belong as child to the graphics items
// for which the selection points are created. Otherwise the "boundingRect" call
// of groups (which implicitly calls childrenBoundingRect) does not work as the
// label of the bounding rectangle would be included.
public: // ctors and dtor
    CGraphObjLabel( // for (bounding) rectangles
        CDrawingScene*  i_pDrawingScene,
        CGraphObj*      i_pGraphObj,
        const QString&  i_strKey,
        const QString&  i_strText,
        ESelectionPoint i_selPt );
    virtual ~CGraphObjLabel();
public: // overridables of base class QGraphicsItem
    int type() const { return EGraphObjTypeLabel; }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
public: // replacing methods of QGraphicsSimpleTextItem
    void setText( const QString& i_strText );
public: // overridables
    virtual QString getKey() const { return m_strKey; }
    virtual ESelectionPoint getGraphObjParentSelectionPoint() const { return m_selPtGraphObjParent; }
public: // overridables
    //virtual void saveGraphObjDistance();
    //virtual QSizeF getGraphObjDistance() const { return m_sizGraphObjDist; }
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged();
public: // must overridables of base class CGraphObj
    virtual void setWidth( double /*i_fWidth*/ ) {}
    virtual void setHeight( double /*i_fHeight*/ ) {}
    virtual void setSize( double /*i_fWidth*/, double /*i_fHeight*/ ) {}
    virtual void setSize( const QSizeF& /*i_size*/ ) {}
public: // overridables of base class CGraphObj
    virtual void setRotationAngleInDegree( double /*i_fRotAngle_deg*/ ) {} // not supported for labels
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit );
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) { i_selPts = i_selPts; }
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) { i_selPts = i_selPts; }
public: // overridables (to avoid endless recursion)
    virtual QRectF boundingRect( bool i_bSimpleTextItemOnly ) const;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr );
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
    static qint64                   s_iCtorsCount;
    static qint64                   s_iDtorsCount;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjCtorsAndDtor;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjBoundingRect;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjPaint;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjSceneEventFilter;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjHoverEvents;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjMouseEvents;
    static ZS::Trace::CTrcAdminObj* s_pTrcAdminObjItemChange;
protected: // instance members
    QString         m_strKey;
    CGraphObj*      m_pGraphObjParent;
    QGraphicsItem*  m_pGraphicsItemParent;
    ESelectionPoint m_selPtGraphObjParent;      // Selection point of parent item the label is aligned to.
    //QSizeF          m_sizGraphObjParentDist;  // With this distance to the scene position of the label.

}; // class CGraphObjLabel

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabel_h
