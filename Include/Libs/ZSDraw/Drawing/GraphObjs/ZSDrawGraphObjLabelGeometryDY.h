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

#ifndef ZSDraw_GraphObjLabelGeometryDY_h
#define ZSDraw_GraphObjLabelGeometryDY_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief 
*/
class ZSDRAWDLL_API CGraphObjLabelGeometryDY : public CGraphObjLabel
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjLabelGeometryDY"; }
public: // ctors and dtor
    CGraphObjLabelGeometryDY(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strKey,
        const SGraphObjSelectionPoint& i_selPt1,
        const SGraphObjSelectionPoint& i_selPt2);
    virtual ~CGraphObjLabelGeometryDY();
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
    /*!< Draw settings for the arrow heads. */
    CDrawSettings m_drawSettingsArrowHeads;
    /*!< Polygon points for arrow head at P1 (line start) */
    QPolygonF m_plgP1ArrowHead;
    /*!< Polygon points for arrow head at P2 (line end) */
    QPolygonF m_plgP2ArrowHead;

}; // class CGraphObjLabelGeometryDY

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabelGeometryDY_h
