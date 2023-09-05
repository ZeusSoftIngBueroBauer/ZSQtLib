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

#ifndef ZSDraw_GraphObjLabel_h
#define ZSDraw_GraphObjLabel_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSimpleTextItem>
#else
#include <QtWidgets/QGraphicsSimpleTextItem>
#endif

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawSettings;

//******************************************************************************
/*!
    @note Labels should not belong as child to the graphics items for which the
          selection points are created. Otherwise the "boundingRect" call of groups
          (which implicitly calls childrenBoundingRect) does not work as the
          label of the bounding rectangle would be included.
*/
class ZSDRAWDLL_API CGraphObjLabel : public CGraphObj, public QGraphicsSimpleTextItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjLabel"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // ctors and dtor
    CGraphObjLabel( // for (bounding) rectangles
        CDrawingScene*  i_pDrawingScene,
        CGraphObj*      i_pGraphObjParent,
        const QString&  i_strKey,
        const QString&  i_strText,
        ESelectionPoint i_selPt );
    virtual ~CGraphObjLabel();
public: // overridables of base class QGraphicsItem
    int type() const override { return static_cast<int>(EGraphObjTypeLabel); }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // overridables
    virtual QString getKey() const { return m_strKey; }
public: // replacing methods of QGraphicsSimpleTextItem
    void setText( const QString& i_strText );
    QString getText() const;
public: // overridables
    virtual void setLinkedSelectionPoint(ESelectionPoint i_selPt);
    virtual ESelectionPoint getLinkedSelectionPoint() const { return m_selPtLinked; }
    QSizeF getLinkedSelectionPointDistance() const { return m_sizeLinkedSelPtDist; }
public: // overridables
    virtual void showAnchorLine();
    virtual void hideAnchorLine();
    virtual bool isAnchorLineVisible() const { return m_bShowAnchorLine; }
public: // overridables
    //virtual void saveGraphObjDistance();
    //virtual QSizeF getGraphObjDistance() const { return m_sizGraphObjDist; }
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const override; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged() override;
public: // must overridables of base class CGraphObj
    virtual void setWidth( const ZS::PhysVal::CPhysVal& /*i_physValWidth*/ ) override {}
    virtual void setHeight( const ZS::PhysVal::CPhysVal& /*i_physValHeight*/ ) override {}
    virtual void setSize( const ZS::PhysVal::CPhysVal& /*i_physValWidth*/, const ZS::PhysVal::CPhysVal& /*i_physValHeight*/ ) override {}
    virtual void setSize( const CPhysValSize& /*i_physValSize*/ ) override {}
    virtual bool hasBoundingRect() const override { return false; }
    virtual bool hasLineShapePoints() const override { return false; }
    virtual bool hasRotationSelectionPoints() const override { return false; }
public: // overridables of base class CGraphObj
    virtual void setRotationAngleInDegree( double /*i_fRotAngle_deg*/ ) override {} // not supported for labels
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit ) override;
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr ) const override;
public: // reimplementing methods of base class QGraphicItem
    void setCursor( const QCursor& cursor );
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
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
protected: // instance members
    /*!< Key of the label within the list of labels of the graphical objects. */
    QString m_strKey;
    /*!< Graphical parent object this label belongs to. */
    CGraphObj* m_pGraphObjParent;
    /*!< Graphical parent object this label belongs to. Same as m_pGraphObjParent
         but stored to avoid unnecessary dynamic casts during runtime. */
    QGraphicsItem* m_pGraphicsItemParent;
    /*!< Selection point of parent item the label is linked and aligned to. */
    ESelectionPoint m_selPtLinked;
    /*!< Distance to the selection point the label is aligned to.
         Calculated as:
         - width = this.x - LinkedSelPt.x
         - height = this.y - LinkedSelPt.y
         This means if the label is right of the parent items selection point the width is positive.
         If the label is above of the parent items selection point the height is positive. */
    QSizeF m_sizeLinkedSelPtDist;
    /*! Flag to indicate whether the anchor line (line from label to parent's selection point the
        label is linked to) should always be visible. */
    bool m_bShowAnchorLine;

}; // class CGraphObjLabel

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabel_h