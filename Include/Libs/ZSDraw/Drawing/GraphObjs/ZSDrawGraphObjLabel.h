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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSimpleTextItem>
#else
#include <QtWidgets/QGraphicsSimpleTextItem>
#endif

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Class to indicate text labels.

    Labels are used to indicate a descriptive text or coordinates of the object they
    are linked to. This class is also used as the base class for all geometry labels.

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
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjLabel"; }
public: // ctors and dtor
    CGraphObjLabel(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strKey,
        const QString& i_strText,
        const SGraphObjSelectionPoint& i_selPt);
protected: // ctor (used by derived classes, e.g. CGraphObjLabelGeometryPosition)
    CGraphObjLabel(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strKey,
        const QString& i_strText,
        EGraphObjType i_labelType,
        const SGraphObjSelectionPoint& i_selPt);
    CGraphObjLabel(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strKey,
        const QString& i_strText,
        EGraphObjType i_labelType,
        const SGraphObjSelectionPoint& i_selPt1,
        const SGraphObjSelectionPoint& i_selPt2);
public: // dtor
    virtual ~CGraphObjLabel();
public: // overridables of base class QGraphicsItem
    int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // overridables
    void setKey(const QString& i_strKey);
    QString key() const;
public: // replacing methods of QGraphicsSimpleTextItem
    void setText( const QString& i_strText );
    QString text() const;
public: // instance methods
    void setSelectionPoint1( const SGraphObjSelectionPoint& i_selPt );
    SGraphObjSelectionPoint selectionPoint1() const;
    void setSelectionPoint2( const SGraphObjSelectionPoint& i_selPt );
    SGraphObjSelectionPoint selectionPoint2() const;
public: // instance methods
    void setPolarCoorsToLinkedSelectionPoint(const SPolarCoors& i_polarCoors);
    SPolarCoors polarCoorsToLinkedSelectionPoint() const;
public: // instance methods
    void showAnchorLine();
    void hideAnchorLine();
    bool isAnchorLineVisible() const;
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const override; // for subsystem test
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
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
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override {};
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
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
public slots: // overridables of base class CGraphObj
    void onGraphObjParentGeometryChanged( CGraphObj* i_pGraphObjParent ) override;
    void onGraphObjParentZValueChanged( CGraphObj* i_pGraphObjParent ) override;
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value ) override;
protected: // overridable auxiliary instance methods
    virtual void updatePosition();
    virtual void updatePolarCoorsToLinkedSelPt();
    virtual void updateAnchorLines();
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    void traceInternalStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
protected: // instance members
    /*!< Key and selection point(s) of the label. */
    SLabelDscr m_labelDscr;
    /*!< Coordindates of the line segments forming the anchor lines. Each line segment is stored
         in local coordinates drawn from the label to the selection point(s) of the parent.
         The start point of the line at the label depend on the position of the label relative
         to the selection point(s) of the parent (see "updatePosition" for more details). */
    QList<QLineF> m_anchorLines;
    /*!< Flag used to avoid recursive calls of "updatePosition". */
    bool m_bUpdatePositionInProgress;
    /*!< Flag used to avoid that the relative distance in polar coordinates (length and angle)
         to the linked selection point is changed if the position is updated because the parent's
         geometry is changed. */
    bool m_bPositionUpdateOnParentGeometryChanged;

}; // class CGraphObjLabel

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabel_h
