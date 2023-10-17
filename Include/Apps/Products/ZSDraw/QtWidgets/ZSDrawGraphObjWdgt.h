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

#ifndef ZSDraw_GraphObjWdgt_h
#define ZSDraw_GraphObjWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsProxyWidget>
#else
#include <QtWidgets/QGraphicsProxyWidget>
#endif

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"

class QCheckBox;

namespace ZS
{
namespace Draw
{
class CDrawSettings;
class CGraphObjSelectionPoint;

namespace QtWidgets
{
//******************************************************************************
class CGraphObjWdgt : public CGraphObj, public QGraphicsProxyWidget
//******************************************************************************
{
public: // class methods
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw::QtWidgets"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjWdgt"; }
public: // class members
    /*! Group name of the factories to create the standard shapes (Line, Point, Rect, etc).
        The group name together with the type as string of the graphical objects must be unique
        and is used to add the object factory to the index tree of all registered factories.
        Defined here as there is no base class for the widget factories. */
    static const QString c_strFactoryGroupName; // = "Widgets"
protected: // ctor
    CGraphObjWdgt(
        const QString&       i_strType,
        const QString&       i_strObjName,
        const CDrawSettings& i_drawSettings );
public: // dtor
    virtual ~CGraphObjWdgt();
public: // overridables of base class CGraphObj
    virtual int type() const override { return EGraphObjTypeUserDefined; }
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit ) override;
public: // overridables
    virtual QString getScenePolygonShapePointsString() const override; // for subsystem test
public: // must overridables of base class CGraphObj
    virtual void setWidth( const ZS::PhysVal::CPhysVal& i_physValWidth ) override;
    virtual void setHeight( const ZS::PhysVal::CPhysVal& i_physValHeight ) override;
    virtual void setSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight ) override;
    virtual void setSize( const CPhysValSize& i_physValSize ) override;
    virtual bool hasBoundingRect() const override { return false; }
    virtual bool hasLineShapePoints() const override { return false; }
    virtual bool hasRotationSelectionPoints() const override { return false; }
public: // must overridables of base class QGraphicsItem, overriding implementation of QGraphicsWidget
    virtual QRectF boundingRect() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
protected: // overridables of base class QGraphicsItem, overriding implementation of QGraphicsWidget
    virtual bool sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem, overriding implementation of QGraphicsProxyWidget
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem, overriding implementation of QGraphicsProxyWidget
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem, overriding implementation of QGraphicsProxyWidget
    virtual QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value ) override;

}; // class CGraphObjWdgt

} // namespace QtWidgets

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjWdgt_h
