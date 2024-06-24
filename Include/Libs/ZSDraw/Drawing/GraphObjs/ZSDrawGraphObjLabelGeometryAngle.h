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

#ifndef ZSDraw_GraphObjLabelGeometryAngle_h
#define ZSDraw_GraphObjLabelGeometryAngle_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Label to show the angle of the line between two shape or selection points.

    The angle label does not use the common anchor lines but draws the anchor lines
    as a circle segment (pie).

    The pie is placed into a square whose center point is the center of the line between
    the two shape or selection points and whose width and height is the distance of the
    label to the end point of the line between the shape or selection points.

    The start angle of the is the 0° degree line of the linked object. This start angle
    must be calculated as the sum of all rotation angles of all parent groups of the
    linked object.

    The span of the pie is the rotation angle of the linked object.

    Examples for lines:

                        P2
                        +
                        |
                        |<--
                        |  90\°  Pie (circle segment)
                        |     |
                        x------
                        |
                        |
                        |
                        |
                        +
                        P1

                       P1
                       +
                        \
                      ---\----\ Pie
                     /    \       \
           (Span) 315°     \         \
                   |        \           |
                   |         x----------- (Start Angle of Pie)
                    |         \
                      \        \
                          \--- >\
                                 \
                                  \
                                   +
                                   P2

    Example for a Rectangle (rotated by 90°):

                 TopRight        BottomRight
                    +-----------------+
                    |                 |
                    |         |<---   |
                    |         |  90\° (Span of Pie)
                    |         |     | |
                    x         x-------x (Start Angle of Pie)
                    |                 |
                    |                 |
                    |                 |
                    |                 |
                    +-----------------+
                 TopLeft         BottomLeft
*/
class ZSDRAWDLL_API CGraphObjLabelGeometryAngle : public CGraphObjLabel
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjLabelGeometryAngle"; }
public: // ctors and dtor
    CGraphObjLabelGeometryAngle(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strKey,
        const SGraphObjSelectionPoint& i_selPt1,
        const SGraphObjSelectionPoint& i_selPt2);
    virtual ~CGraphObjLabelGeometryAngle();
public: // overridables of base class QGraphicsItem
    int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridable auxiliary instance methods of base class CGraphObjLabel
    virtual void updatePosition() override;
    virtual void updatePolarCoorsToLinkedSelPt() override;
    virtual void updateAnchorLines() override;
protected: // instance members
    /*!< The line between the two selection points the label is linked to.
         The line end points are in scene coordinates. */
    QLineF m_lineSelPtsSceneCoors;
    /*!< Rectangle to draw the circle segment. */
    QRectF m_rectPie;
    /*!< Start angle of the pie to draw the "anchor" line of the rotation angle.
         The start angle is the 0° horizontal line of the linked object and
         is calculated as the sum of all rotation angles of the parent groups
         of the linked object, excluding the rotation angle of the linked object. */
    double m_fPieStartAngle_degrees;
    /*!< Span of the pie  to draw the "anchor" line of the rotation angle.
         The span is equal to the rotation angle of the linked object. */
    double m_fPieSpanAngle_degrees;
    /*!< Draw settings used to draw the arrow heads at the ends of the circle segment. */
    CDrawSettings m_drawSettingsArrowHeads;
    /*!< Polygon points for arrow head at P2 (line end) */
    QPolygonF m_plgP2ArrowHead;

}; // class CGraphObjLabelGeometryAngle

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabelGeometryAngle_h
