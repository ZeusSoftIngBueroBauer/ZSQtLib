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

#ifndef ZSDraw_GraphObjSelectionPoint_h
#define ZSDraw_GraphObjSelectionPoint_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsLineItem>
#else
#include <QtWidgets/QGraphicsLineItem>
#endif

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Selection points of graphical objects.

    Selection points are temporarily created when selecting objects to modify
    the shape of the objects.

    Please note that selection points should not belong as child to the graphics items
    for which the selection points are created. Otherwise the "boundingRect" call
    of groups (which implicitly calls childrenBoundingRect) does not work as the
    selection points of the bounding rectangle would be included.
*/
class ZSDRAWDLL_API CGraphObjSelectionPoint : public CGraphObj, public QGraphicsEllipseItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjSelectionPoint"; }
public: // class methods
    static double defaultWidthInPx();
public: // ctors and dtor
    CGraphObjSelectionPoint(CDrawingScene* i_pDrawingScene, const SGraphObjSelectionPoint& i_selPt);
    ~CGraphObjSelectionPoint() override;
public: // overridables of base class QGraphicsItem
    int type() const override;
public: // must overridables of base class CGraphObj
    CGraphObj* clone() override;
public: // instance methods
    QString pathNameOfLinkedObject() const;
    QString myPathName() const;
public: // instance methods
    SGraphObjSelectionPoint getSelectionPoint() const;
    //void setShapePoint( int i_idxPt );
public: // instance methods
    void setWidthInPx(double i_fWidth_px);
    double getWidthInPx() const;
public: // overridables of base class CGraphObj
    void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // must overridables of base class CGraphObj
    QRectF getBoundingRect() const override;
public: // overridables of base class CGraphObj
    QCursor getProposedCursor(const QPointF& i_pt) const override;
public: // overridables of base class QGraphicsItem
    bool contains(const QPointF& i_pt) const override;
protected: // must overridables of base class CGraphObj
    void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll) override {}
public: // must overridables of base class QGraphicsItem
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    void hoverEnterEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    QVariant itemChange(GraphicsItemChange i_change, const QVariant& i_value) override;
protected slots: // overridables of base class CGraphObj
    void onGraphObjParentGeometryOnSceneChanged(CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged = false) override;
    void onGraphObjParentZValueChanged(CGraphObj* i_pGraphObjParent) override;
protected: // auxiliary instance methods
    void updatePosition();
protected: // class members
    /*!< Default radius to be used for painting the selection points. */
    static double s_fDefaultWidth_px;
    /*!< Default pen width to be used for painting the border of the selection points. */
    static int s_iPenWidth_px;
    /*!< Default color to be used for painting the border of the selection points. */
    static QColor s_colPen;
protected: // instance members
    /*!< Defines the type of the selection point, the linked object and the position at the linked
         object the selection point is linked to.
         Selection points are differentiated into selection points on the bounding
         rectangle around the graphical object or into polygon shape points. */
    SGraphObjSelectionPoint m_selPt;
    /*!< Radius for drawing the selection point. */
    double m_fWidth_px;
    /*!< Flag used to avoid recursive calls of "updatePosition". */
    bool m_bUpdatePositionInProgress;

}; // class CGraphObjSelectionPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjSelectionPoint_h
