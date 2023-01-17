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
public: // class methods
    static int getCurrentInstCount() { return s_iInstCount; }
public: // ctors and dtor
    CGraphObjLabel( // for (bounding) rectangles
        CDrawingScene*  i_pDrawingScene,
        CGraphObj*      i_pGraphObjParent,
        const QString&  i_strKey,
        const QString&  i_strText,
        ESelectionPoint i_selPt );
    virtual ~CGraphObjLabel();
public: // overridables of base class QGraphicsItem
    int type() const { return static_cast<int>(EGraphObjTypeLabel); }
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // overridables
    virtual QString getKey() const { return m_strKey; }
public: // replacing methods of QGraphicsSimpleTextItem
    void setText( const QString& i_strText );
    QString getText() const;
public: // overridables
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
    virtual QString getScenePolygonShapePointsString() const; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged();
public: // must overridables of base class CGraphObj
    virtual void setWidth( double /*i_fWidth*/ ) {}
    virtual void setHeight( double /*i_fHeight*/ ) {}
    virtual void setSize( double /*i_fWidth*/, double /*i_fHeight*/ ) {}
    virtual void setSize( const QSizeF& /*i_size*/ ) {}
    virtual bool hasBoundingRect() const { return false; }
    virtual bool hasLineShapePoints() const { return false; }
    virtual bool hasRotationSelectionPoints() const { return false; }
public: // overridables of base class CGraphObj
    virtual void setRotationAngleInDegree( double /*i_fRotAngle_deg*/ ) {} // not supported for labels
public: // must overridables of base class CGraphObj
    virtual void setIsHit( bool i_bHit );
public: // overridables of base class CGraphObj
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const;
public: // reimplementing methods of base class QGraphicItem
    void setCursor( const QCursor& cursor );
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) { i_selPts = i_selPts; }
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) { i_selPts = i_selPts; }
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
    static qint64 s_iInstCount;
protected: // instance members
    QString         m_strKey;              /*!< Key of the label within the list of labels of the graphical objects. */
    CGraphObj*      m_pGraphObjParent;     /*!< Graphical parent object this label belongs to. */
    QGraphicsItem*  m_pGraphicsItemParent; /*!< Graphical parent object this label belongs to.
                                                Same as m_pGraphObjParent but stored to avoid unnecessary
                                                dynamic casts during runtime. */
    ESelectionPoint m_selPtLinked;         /*!< Selection point of parent item the label is linked and aligned to. */
    QSizeF          m_sizeLinkedSelPtDist; /*!< Distance to the selection point the label is aligned to.
                                                Calculated as:
                                                    width = this.x - LinkedSelPt.x
                                                    height = this.y - LinkedSelPt.y
                                                This means if the label is right of the parent items selection point the width is positive.
                                                If the label is above of the parent items selection point the height is positive. */
    bool            m_bShowAnchorLine;     /*! Flag to indicate whether the anchor line (line from label to parent's selection point the
                                               labe is linked to) should always be visible. */

}; // class CGraphObjLabel

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabel_h
