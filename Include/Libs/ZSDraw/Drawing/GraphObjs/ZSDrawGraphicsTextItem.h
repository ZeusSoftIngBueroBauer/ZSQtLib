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

#ifndef ZSDraw_GraphicsTextItem_h
#define ZSDraw_GraphicsTextItem_h

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
class ZSDRAWDLL_API CGraphicsTextItem : public QGraphicsTextItem
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "QGraphicsTextItem"; }
public: // ctors and dtor
    explicit CGraphicsTextItem(QGraphicsItem* i_pParentItem = nullptr);
    explicit CGraphicsTextItem(const QString& i_strText, QGraphicsItem* i_pParentItem = nullptr);
    ~CGraphicsTextItem() override;
signals:
    /*!< This signal is emitted if the object either got or lost the keyboard input focus. */
    void focusChanged(QFocusEvent* i_pEv);
public: // instance methods
    void setPath(const QString& i_strPath);
    QString path() const;
public: // instance methods
    QString toHtml() const;
    void setHtml(const QString& i_strHtml);
    QString toPlainText() const;
    void setPlainText(const QString& i_strText);
public: // instance methods
    QFont font() const;
    void setFont(const QFont& i_font);
    void setDefaultTextColor(const QColor& i_col);
    QColor defaultTextColor() const;
public: // instance methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    bool contains(const QPointF& i_pt) const override;
    void paint(QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pOption, QWidget* i_pWidget) override;
public: // instance methods
    bool isObscuredBy(const QGraphicsItem*i_pItem) const override;
    QPainterPath opaqueArea() const override;
    void setTextWidth(qreal i_fWidth_px);
    qreal textWidth() const;
    void adjustSize();
    void setDocument(QTextDocument* i_pDocument);
    QTextDocument* document() const;
    void setTextInteractionFlags(Qt::TextInteractionFlags i_flags);
    Qt::TextInteractionFlags textInteractionFlags() const;
    void setTabChangesFocus(bool i_bTabChangesFocus);
    bool tabChangesFocus() const;
    void setOpenExternalLinks(bool i_bOpen);
    bool openExternalLinks() const;
    void setTextCursor(const QTextCursor& i_cursor);
    QTextCursor textCursor() const;
protected: // instance methods
    bool sceneEvent(QEvent* i_pEv) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* i_pEv) override;
    void keyPressEvent(QKeyEvent* i_pEv) override;
    void keyReleaseEvent(QKeyEvent* i_pEv) override;
    void focusInEvent(QFocusEvent* i_pEv) override;
    void focusOutEvent(QFocusEvent* i_pEv) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent* i_pEv) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* i_pEv) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* i_pEv) override;
    void dropEvent(QGraphicsSceneDragDropEvent* i_pEv) override;
    void inputMethodEvent(QInputMethodEvent* i_pEv) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
protected: // instance methods
    QVariant inputMethodQuery(Qt::InputMethodQuery i_query) const override;
public: // call methods without method tracing
    qreal textWidthNoMethodTrace() const;
protected: // auxiliary instance methods (method tracing)
    void QGraphicsItem_setCursor(const QCursor& i_cursor);
    void QGraphicsItem_unsetCursor();
protected: // auxiliary instance methods (method tracing)
    void emit_focusChanged(QFocusEvent* i_pEv);
public: // auxiliary instance methods (method tracing)
    void traceGraphicsItemStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        const QString& i_strFilter = "",
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
protected: // instance members (method tracing)
    QString m_strPath;
protected: // instance members (method tracing)
    /*!< Counters to block debug trace outputs for internal position and state infos. */
    int m_iTraceItemStatesBlockedCounter;
    /*!< Trace admin object to control trace output. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjCtorsAndDtor;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjItemChange;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjBoundingRect;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjCursor;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjPaint;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjSceneEvent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjHoverEnterLeaveEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjHoverMoveEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseClickEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjKeyEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjFocusEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjDragDropEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjInputMethodEvents;
};

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphicsTextItem_h
