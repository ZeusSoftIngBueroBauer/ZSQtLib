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

#ifndef ZSDraw_GraphObjText_h
#define ZSDraw_GraphObjText_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsTextItem>
#else
#include <QtWidgets/QGraphicsTextItem>
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
class CGraphObjSelectionPoint;

//******************************************************************************
class ZSDRAWDLL_API CGraphObjText : public CGraphObj, public QGraphicsTextItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjText"; }
public: // ctors and dtor
    CGraphObjText(
        CDrawingScene* i_pDrawingScene,
        const CDrawSettings& i_drawSettings,
        const QString& i_strObjName = "");
    virtual ~CGraphObjText();
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // replacing methods of QGraphicsTextItem
    void setHtml( const QString& i_strText );
    void setPlainText( const QString& i_strText );
public: // instance methods
    QRectF rect() const; // returns "boundingRect" of QGraphicsTextItem
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const override; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // must overridables of base class CGraphObj
    virtual void setWidth( const ZS::PhysVal::CPhysVal& i_physValWidth ) override {};
    virtual void setHeight( const ZS::PhysVal::CPhysVal& i_physValHeight ) override {};
    virtual void setSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight ) override {};
    virtual void setSize( const CPhysValSize& i_physValSize ) override {};
    virtual bool hasBoundingRect() const override { return false; }
    virtual bool hasLineShapePoints() const override { return false; }
    virtual bool hasRotationSelectionPoints() const override { return true; }
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit ) override;
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr ) const override;
public: // reimplementing methods of base class QGraphicItem
    void setCursor( const QCursor& cursor );
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
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
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
    virtual void keyReleaseEvent( QKeyEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value ) override;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;

}; // class CGraphObjText

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjText_h
