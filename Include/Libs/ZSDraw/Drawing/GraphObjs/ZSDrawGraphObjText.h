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
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CGraphObjText : public CGraphObj, public QGraphicsItem
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjText"; }
public: // class methods
    static QPainter::RenderHints painterRenderHints();
    static void setPainterRenderHints(QPainter::RenderHints i_renderHints);
    static void resetPainterRenderHints();
public: // ctors and dtor
    CGraphObjText(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
    virtual ~CGraphObjText();
signals:
    /*!< This signal is emitted when the user clicks on a link on a text item that
         enables Qt::LinksAccessibleByMouse or Qt::LinksAccessibleByKeyboard.
         @param [in] i_strLink is the link that was clicked. */
    void linkActivated(CGraphObj* i_pGraphObj, const QString& i_strLink);
    /*!< This signal is emitted when the user hovers over a link on a text item that
         enables Qt::LinksAccessibleByMouse.
         @param [in] i_strLink is the link that was hovered over. */
    void linkHovered(CGraphObj* i_pGraphObj, const QString& i_strLink);
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // must overridables of base class CGraphObj
    void openFormatGraphObjsDialog() override;
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // overridables of base class CGraphObj
    void setTextColor(const QColor& i_col, bool i_bImmediatelyApplySetting = true) override;
    void setFont(const QFont& i_font, bool i_bImmediatelyApplySetting = true) override;
public: // providing methods of QGraphicsTextItem
    void setHtml(const QString& i_strText);
    QString toHtml() const;
    void setPlainText(const QString& i_strText);
    QString toPlainText() const;
public: // providing methods of QGraphicsTextItem
    void setDocument(QTextDocument* i_pDoc);
    QTextDocument* document() const;
public: // providing methods of QGraphicsTextItem
    void setTabChangesFocus(bool i_bTabChangesFocus);
    bool tabChangesFocus() const;
    void setTextInteractionFlags(Qt::TextInteractionFlags i_flags);
    Qt::TextInteractionFlags textInteractionFlags() const;
public: // instance methods
    void setMargins(const QMargins& i_margins_px);
    void setMargins(int i_fLeft_px, int i_fTop_px, int i_fRight_px, int i_fBottom_px);
    void setMarginLeft(int i_fLeft_px);
    void setMarginTop(int i_fTop_px);
    void setMarginRight(int i_fRight_px);
    void setMarginBottom(int i_fBottom_px);
    QMargins margins() const;
    int marginLeft() const;
    int marginTop() const;
    int marginRight() const;
    int marginBottom() const;
public: // instance methods
    void adjustSize();
    void setTextWidth(double i_fWidth);
    double textWidth() const;
public: // instance methods
    void setRect(const CPhysValRect& i_physValRect);
    CPhysValRect getRect() const;
    CPhysValRect getRect(const ZS::PhysVal::CUnit& i_unit) const;
    void setCenter(const QPointF& i_pt);
    void setCenter(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getCenter() const;
    CPhysValPoint getCenter(const ZS::PhysVal::CUnit& i_unit) const;
    void setSize(const QSizeF& i_size);
    void setSize(const CPhysValSize& i_physValSize);
    CPhysValSize getSize() const;
    CPhysValSize getSize(const ZS::PhysVal::CUnit& i_unit) const;
    void setWidth(double i_fWidth);
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void setWidthByMovingLeftCenter(const QPointF& i_pt);
    void setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint);
    void setWidthByMovingRightCenter(const QPointF& i_pt);
    void setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint);
    ZS::PhysVal::CPhysVal getWidth() const;
    ZS::PhysVal::CPhysVal getWidth(const ZS::PhysVal::CUnit& i_unit) const;
    void setHeight(double i_fHeight);
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    void setHeightByMovingTopCenter(const QPointF& i_pt);
    void setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint);
    void setHeightByMovingBottomCenter(const QPointF& i_pt);
    void setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint);
    ZS::PhysVal::CPhysVal getHeight() const;
    ZS::PhysVal::CPhysVal getHeight(const ZS::PhysVal::CUnit& i_unit) const;
    void setTopLeft(const QPointF& i_pt);
    void setTopLeft(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getTopLeft() const;
    CPhysValPoint getTopLeft(const ZS::PhysVal::CUnit& i_unit) const;
    void setTopRight(const QPointF& i_pt);
    void setTopRight(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getTopRight() const;
    CPhysValPoint getTopRight(const ZS::PhysVal::CUnit& i_unit) const;
    void setBottomRight(const QPointF& i_pt);
    void setBottomRight(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getBottomRight() const;
    CPhysValPoint getBottomRight(const ZS::PhysVal::CUnit& i_unit) const;
    void setBottomLeft(const QPointF& i_pt);
    void setBottomLeft(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getBottomLeft() const;
    CPhysValPoint getBottomLeft(const ZS::PhysVal::CUnit& i_unit) const;
public: // must overridables of base class CGraphObj
    void setRotationAngle(double i_fAngle_degree) override;
    void setRotationAngle(const ZS::PhysVal::CPhysVal& i_physValAngle) override;
public: // must overridables of base class CGraphObj
    QRectF getBoundingRect() const override;
    QRectF getEffectiveBoundingRectOnScene() const override;
    CPhysValRect getPhysValBoundingRect(const ZS::PhysVal::CUnit& i_unit) const override;
protected: // must overridables of base class CGraphObj
    void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll) override;
public: // overridables of base class CGraphObj (text labels)
    virtual QList<SGraphObjSelectionPoint> getPossibleLabelAnchorPoints(const QString& i_strName) const override;
    virtual bool labelHasDefaultValues(const QString& i_strName) const override;
public: // overridables of base class CGraphObj (geometry labels)
    virtual bool geometryLabelHasDefaultValues(const QString& i_strName) const override;
public: // must overridables of base class QGraphicsItem
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr) override;
protected: // overridables of base class QGraphicsItem
    void hoverEnterEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    void keyPressEvent( QKeyEvent* i_pEv ) override;
    void keyReleaseEvent( QKeyEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    QVariant itemChange(GraphicsItemChange i_change, const QVariant& i_value) override;
protected: // overridable slots of base class CGraphObj
    virtual void onGraphObjParentGeometryOnSceneChanged(CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged = false) override;
    virtual void onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint) override;
public: // must overridables of base class CGraphObj
    virtual void updateTransformedCoorsOnParentChanged(CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew) override;
    virtual void updateTransformedCoorsOnParentGeometryChanged() override;
    virtual void updateTransformedCoorsOnItemPositionChanged() override;
protected: // auxiliary instance methods
    QRectF getRectScaled(const QRectF& i_rectOrig) const;
    CPhysValRect getPhysValRectOrig(const QRectF& i_rectOrig) const;
    CPhysValRect getPhysValRectScaled(const CPhysValRect& i_physValRectOrig) const;
    QPointF getItemPosAndLocalCoors(const CPhysValRect& i_physValRect, QRectF& o_rect, ZS::PhysVal::CPhysVal& o_physValAngle) const;
protected: // auxiliary instance methods (method tracing)
    QRectF setRectOrig(const QRectF& i_rect);
    QRectF setRectScaled(const QRectF& i_rect);
    CPhysValRect setPhysValRectOrig(const CPhysValRect& i_physValRect);
    CPhysValRect setPhysValRectScaled(const CPhysValRect& i_physValRect);
    CPhysValRect setPhysValRectScaledAndRotated(const CPhysValRect& i_physValRect);
    void QGraphicsItem_prepareGeometryChange() override;
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    virtual void traceThisPositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        const QString& i_strFilter = "",
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
    virtual void traceTextItemPositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        const QString& i_strFilter = "",
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;
protected: // class members
    static QPainter::RenderHints s_painterRenderHints;
protected: // instance members
    /*!< The aggregated text item. The class could not have been derived from QGraphicsTextItem
         as CGraphObj and QGraphicsTextItem both are derviced from QObject. */
    QGraphicsTextItem m_graphicsTextItem;
    /*!< The margins to the surrounding rectangle. The margin must be at least one pixel.
         Otherwise mouse press events would be forwarded to the text editor of the text item
         and the text item graphic object cannot be selected or highlighted and cannot be
         moved, resized or rotated by mouse events. */
    QMargins m_margins;
    /*!< The original, untransformed (not scaled, not rotated) rectangle coordinates in local
         coordinates relative to the origin of the item's bounding rectangle.
         This member is set if any shape point is directly set via the method call "setRect"
         (which is implicitly called by all other methods modifying shape points) or at the
         time the item is added to a group.
         The rotation angle of the graphics item is separately stored in
         m_physValRotationAngle of base class CGraphObj.
         The scale factors are also stored separately in the members
         m_fParentGroupScaleX and m_fParentGroupScaleY of the base class CGraphObj.
         @see base class CGraphObj "Current and Original Coordinates". */
    QRectF m_rectOrig;
    /*!< The scaled but not rotated rectangle coordinates in local coordinates relative
         to the origin of the item's bounding rectangle.
         Other graphics items, like Line, provide methods to set and retrieve the
         local coordinates (e.g. "setLine", "line").
         The text item does not have such methods (e.g. "setRect", "rect").
         The scaled rectangle is returned by the "boundingRect" method of the graphics
         item to provide the bounding rectangle of the group in local coordinates. */
    QRectF m_rectScaled;
    /*!< The original, untransformed (not scaled, not rotated) rectangle coordinates with unit
         in parent coordinates relative to the top left or bottom left corner of the parent.
         The rotation angle of the graphics item is separately stored in
         m_physValRotationAngle of base class CGraphObj.
         The scale factors are also stored separately in the members
         m_fParentGroupScaleX and m_fParentGroupScaleY of the base class CGraphObj.
         @see base class CGraphObj "Current and Original Coordinates". */
    CPhysValRect m_physValRectOrig;
    /*!< The scaled but not rotated rectangle coordinates with unit in parent coordinates
         relative to the top left or bottom left corner of the parent.
         The scaled rectangle is returned by the "getBoundingRect" method
         to provide the bounding rectangle of the group in the current unit. */
    CPhysValRect m_physValRectScaled;
    /*!< The scaled and rotated rectangle coordinates with unit in parent coordinates
         relative to the top left or bottom left corner of the parent.
         The scaled and rotated rectangle is returned by the "getRect" method
         (and all other methods retrieving the resulting coordinates in the current unit). */
    CPhysValRect m_physValRectScaledAndRotated;

}; // class CGraphObjText

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjText_h
