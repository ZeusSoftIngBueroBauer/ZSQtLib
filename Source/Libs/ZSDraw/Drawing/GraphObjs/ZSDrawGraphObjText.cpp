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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjText.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjTextPropertiesDlg.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qevent.h>
#include <QtGui/QPainter>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/QMenu>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QStyleOption>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CGraphObjText : public CGraphObj, public QGraphicsItem
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

qint64 CGraphObjText::s_iInstCount = 0;

/*==============================================================================
protected: // class members
==============================================================================*/

QMargins CGraphObjText::s_marginsDefault(5, 5, 5, 5);
QPainter::RenderHints CGraphObjText::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QMargins CGraphObjText::defaultMarginsInPx()
//------------------------------------------------------------------------------
{
    return s_marginsDefault;
}

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjText::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjText::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjText::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a graphical text object.

    The number of text objects stored in s_iInstCount is increased.
    By passing an empty object name the number of created instances is used to
    create a unique object name.

    @param [in] i_pDrawingScene
        Pointer to drawing scene from which the object is created.
    @param [in] i_strObjName
        Name of the graphical object.
        Names of graphical objects must be unique below its parent.
        If an empty string is passed a unique name is created by adding the current
        number of objects taken from s_iInstCount to the graphical object type.
*/
CGraphObjText::CGraphObjText(CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeText,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeText),
        /* strObjName          */ i_strObjName.isEmpty() ? "Text" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsItem(),
    m_graphicsTextItem(this),
    m_margins(s_marginsDefault),
    m_rectOrig(),
    m_rectScaled(),
    m_physValRectOrig(*m_pDrawingScene),
    m_physValRectScaled(*m_pDrawingScene),
    m_physValRectScaledAndRotated(*m_pDrawingScene)
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    createTraceAdminObjs("StandardShapes::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ObjName: " + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_strlstPredefinedLabelNames.append(c_strLabelName);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameTopLeft);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameTopRight);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameBottomLeft);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameBottomRight);

    for (const QString& strLabelName : m_strlstPredefinedLabelNames) {
        if (!m_hshpLabels.contains(strLabelName)) {
            if (strLabelName == c_strGeometryLabelNameTopLeft) {
                addLabel(strLabelName, strLabelName, ESelectionPointType::BoundingRectangle, ESelectionPoint::TopLeft);
            }
            else if (strLabelName == c_strGeometryLabelNameTopRight) {
                addLabel(strLabelName, strLabelName, ESelectionPointType::BoundingRectangle, ESelectionPoint::TopRight);
            }
            else if (strLabelName == c_strGeometryLabelNameBottomRight) {
                addLabel(strLabelName, strLabelName, ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomRight);
            }
            else if (strLabelName == c_strGeometryLabelNameBottomLeft) {
                addLabel(strLabelName, strLabelName, ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomLeft);
            }
            else {
                addLabel(strLabelName, strLabelName, ESelectionPointType::BoundingRectangle, ESelectionPoint::Center);
            }
        }
    }

    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameTopLeft);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameTopRight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameBottomRight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameBottomLeft);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameCenter);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameWidth);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameHeight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameAngle);

    const CUnit& unit = m_pDrawingScene->drawingSize().unit();
    for (const QString& strLabelName : m_strlstGeometryLabelNames) {
        if (strLabelName == c_strGeometryLabelNameTopLeft) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::TopLeft);
        }
        else if (strLabelName == c_strGeometryLabelNameTopRight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::TopRight);
        }
        else if (strLabelName == c_strGeometryLabelNameBottomRight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::BottomRight);
        }
        else if (strLabelName == c_strGeometryLabelNameBottomLeft) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::BottomLeft);
        }
        else if (strLabelName == c_strGeometryLabelNameCenter) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::Center);
        }
        else if (strLabelName == c_strGeometryLabelNameWidth) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryLength, ESelectionPoint::LeftCenter, ESelectionPoint::RightCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameHeight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryLength, ESelectionPoint::TopCenter, ESelectionPoint::BottomCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameAngle) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryAngle, ESelectionPoint::LeftCenter, ESelectionPoint::RightCenter);
        }
    }

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable
            |QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
    QGraphicsItem_setAcceptHoverEvents(true);

    QGraphicsTextItem_setTextInteractionFlags(Qt::TextEditorInteraction);

    QObject::connect(
        &m_graphicsTextItem, &CGraphicsTextItem::focusChanged,
        this, &CGraphObjText::onGraphicsTextItemFocusChanged);
}

//------------------------------------------------------------------------------
CGraphObjText::~CGraphObjText()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;
    emit_aboutToBeDestroyed();
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjText::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeText;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjText::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjText* pGraphObj = new CGraphObjText(m_pDrawingScene, m_strName);
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    pGraphObj->setRect(getRect(drawingSize.unit()));
    pGraphObj->setDrawSettings(m_drawSettings);
    //pGraphObj->setPlainText(toPlainText());
    return pGraphObj;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObjText::openFormatGraphObjsDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "openFormatGraphObjsDialog",
        /* strAddInfo   */ "" );

    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Format Text";
    CDlgGraphObjTextProperties* pDlg = CDlgGraphObjTextProperties::GetInstance(this);
    if( pDlg == nullptr ) {
        pDlg = CDlgGraphObjTextProperties::CreateInstance(strDlgTitle, this);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->setModal(false);
        pDlg->show();
    }
    else {
        if( pDlg->isHidden() ) {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    bool bDrawSettingsChanged = (m_drawSettings != i_drawSettingsOld);
    if (bDrawSettingsChanged) {
        QFont fnt = m_drawSettings.textFont();
        fnt.setPixelSize(textSize2SizeInPixels(textSize()));
        fnt.setBold(isTextStyleBold(textStyle()));
        fnt.setItalic(isTextStyleItalic(textStyle()));
        fnt.setStrikeOut(isTextEffectStrikeout(textEffect()));
        fnt.setUnderline(isTextEffectUnderline(textEffect()));
        m_graphicsTextItem.setFont(fnt);
        m_graphicsTextItem.setDefaultTextColor(m_drawSettings.textColor());
        update();
        emit_drawSettingsChanged();
    }
}

/*==============================================================================
public: // providing methods of QGraphicsTextItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the color for unformatted text to col.
*/
void CGraphObjText::setTextColor(const QColor& i_col, bool i_bImmediatelyApplySetting)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_col.name() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTextColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        m_graphicsTextItem.setDefaultTextColor(i_col);
    }
    CGraphObj::setTextColor(i_col, i_bImmediatelyApplySetting);
}

//------------------------------------------------------------------------------
/*! @brief Sets the font used to render the text item to font.
*/
void CGraphObjText::setFont(const QFont& i_font, bool i_bImmediatelyApplySetting)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_font.family() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setFont",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        QFont fnt = i_font;
        fnt.setPixelSize(textSize2SizeInPixels(textSize()));
        fnt.setBold(isTextStyleBold(textStyle()));
        fnt.setItalic(isTextStyleItalic(textStyle()));
        fnt.setStrikeOut(isTextEffectStrikeout(textEffect()));
        fnt.setUnderline(isTextEffectUnderline(textEffect()));
        m_graphicsTextItem.setFont(fnt);
    }
    CGraphObj::setFont(i_font, i_bImmediatelyApplySetting);
}

/*==============================================================================
public: // providing methods of QGraphicsTextItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::setHtml( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHtml",
        /* strAddInfo   */ strMthInArgs );

    m_graphicsTextItem.setHtml(i_strText);
}

//------------------------------------------------------------------------------
QString CGraphObjText::toHtml() const
//------------------------------------------------------------------------------
{
    return m_graphicsTextItem.toHtml();
}

//------------------------------------------------------------------------------
void CGraphObjText::setPlainText( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPlainText",
        /* strAddInfo   */ strMthInArgs );

    m_graphicsTextItem.setPlainText(i_strText);
}

//------------------------------------------------------------------------------
QString CGraphObjText::toPlainText() const
//------------------------------------------------------------------------------
{
    return m_graphicsTextItem.toPlainText();
}

//------------------------------------------------------------------------------
/*! @brief Sets the text document document on the item.
*/
void CGraphObjText::setDocument(QTextDocument* i_pDoc)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pDoc == nullptr ? "null" : i_pDoc->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setDocument",
        /* strAddInfo   */ strMthInArgs );

    m_graphicsTextItem.setDocument(i_pDoc);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's text document.
*/
QTextDocument* CGraphObjText::document() const
//------------------------------------------------------------------------------
{
    return m_graphicsTextItem.document();
}

/*==============================================================================
public: // providing methods of QGraphicsTextItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief If i_bTabChangesFocus is true, the Tab key will cause the widget to
           change focus; otherwise, the tab key will insert a tab into the document.

    In some occasions text edits should not allow the user to input tabulators or
    change indentation using the Tab key, as this breaks the focus chain.
    The default is false.
*/
void CGraphObjText::setTabChangesFocus(bool i_bTabChangesFocus)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bTabChangesFocus);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTabChangesFocus",
        /* strAddInfo   */ strMthInArgs );

    m_graphicsTextItem.setTabChangesFocus(i_bTabChangesFocus);
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the Tab key will cause the widget to change focus;
           otherwise, false is returned.

    By default, this behavior is disabled, and this function will return false.
*/
bool CGraphObjText::tabChangesFocus() const
//------------------------------------------------------------------------------
{
    return m_graphicsTextItem.tabChangesFocus();
}

//------------------------------------------------------------------------------
/*! @brief Sets the flags flags to specify how the text item should react to user input.

    The default for a QGraphicsTextItem is Qt::NoTextInteraction. This function also
    affects the ItemIsFocusable QGraphicsItem flag by setting it if flags is different
    from Qt::NoTextInteraction and clearing it otherwise.

    By default, the text is read-only. To transform the item into an editor, set the Qt::TextEditable flag.
*/
void CGraphObjText::setTextInteractionFlags(Qt::TextInteractionFlags i_flags)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qTextInteractionFlags2Str(i_flags);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTextInteractionFlags",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsTextItem_setTextInteractionFlags(i_flags);
}

//------------------------------------------------------------------------------
/*! @brief Returns the current text interaction flags.
*/
Qt::TextInteractionFlags CGraphObjText::textInteractionFlags() const
//------------------------------------------------------------------------------
{
    return m_graphicsTextItem.textInteractionFlags();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjText::setMargins(const QMargins& i_margins_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qMargins2Str(i_margins_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setMargins",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    if (m_margins != i_margins_px) {
        m_margins = i_margins_px;
        QRectF rectBounding = getBoundingRect();
        QPointF ptTLTextItem(rectBounding.left() + m_margins.left(), rectBounding.top() + m_margins.top());
        QGraphicsTextItem_setPos(ptTLTextItem);
        double fTextWidth_px = rectBounding.width() - m_margins.left() - m_margins.right();
        QGraphicsTextItem_setTextWidth(fTextWidth_px > 0.0 ? fTextWidth_px : -1.0);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjText::setMargins(int i_iLeft_px, int i_iTop_px, int i_iRight_px, int i_iBottom_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Left: " + QString::number(i_iLeft_px) + "px" +
            ", Top: " + QString::number(i_iTop_px) + "px" +
            ", Right: " + QString::number(i_iRight_px) + "px" +
            ", Bottom: " + QString::number(i_iBottom_px) + "px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setMargins",
        /* strAddInfo   */ strMthInArgs );

    setMargins(QMargins(i_iLeft_px, i_iTop_px, i_iRight_px, i_iBottom_px));
}

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjText::setMarginLeft(int i_iLeft_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iLeft_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setMarginLeft",
        /* strAddInfo   */ strMthInArgs );

    QMargins margins = this->margins();
    margins.setLeft(i_iLeft_px);
    setMargins(margins);
}

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjText::setMarginTop(int i_iTop_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iTop_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setMarginTop",
        /* strAddInfo   */ strMthInArgs );

    QMargins margins = this->margins();
    margins.setTop(i_iTop_px);
    setMargins(margins);
}

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjText::setMarginRight(int i_iRight_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iRight_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setMarginRight",
        /* strAddInfo   */ strMthInArgs );

    QMargins margins = this->margins();
    margins.setRight(i_iRight_px);
    setMargins(margins);
}

//------------------------------------------------------------------------------
/*! @brief 
*/
void CGraphObjText::setMarginBottom(int i_iBottom_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iBottom_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setMarginBottom",
        /* strAddInfo   */ strMthInArgs );

    QMargins margins = this->margins();
    margins.setBottom(i_iBottom_px);
    setMargins(margins);
}

//------------------------------------------------------------------------------
/*! @brief 
*/
QMargins CGraphObjText::margins() const
//------------------------------------------------------------------------------
{
    return m_margins;
}

//------------------------------------------------------------------------------
/*! @brief 
*/
int CGraphObjText::marginLeft() const
//------------------------------------------------------------------------------
{
    return m_margins.left();
}

//------------------------------------------------------------------------------
/*! @brief 
*/
int CGraphObjText::marginTop() const
//------------------------------------------------------------------------------
{
    return m_margins.top();
}

//------------------------------------------------------------------------------
/*! @brief 
*/
int CGraphObjText::marginRight() const
//------------------------------------------------------------------------------
{
    return m_margins.right();
}

//------------------------------------------------------------------------------
/*! @brief 
*/
int CGraphObjText::marginBottom() const
//------------------------------------------------------------------------------
{
    return m_margins.bottom();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Adjusts the text item to a reasonable size.
*/
void CGraphObjText::adjustSize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "adjustSize",
        /* strAddInfo   */ "" );

    m_graphicsTextItem.adjustSize();

    QRectF rectBoundingTextItem = m_graphicsTextItem.boundingRect();
    QSizeF size = rectBoundingTextItem.size();
    if (!m_margins.isNull()) {
        size.setWidth(size.width() + m_margins.left() + m_margins.right());
        size.setHeight(size.height() + m_margins.top() + m_margins.bottom());
    }
    CPhysValSize physValSize(*m_pDrawingScene, size);
    setSize(physValSize);
}

//------------------------------------------------------------------------------
/*! @brief Sets the preferred width for the item's text.

    If the actual text is wider than the specified width then it will be broken
    into multiple lines. If width is set to -1 then the text will not be broken
    into multiple lines unless it is enforced through an explicit line break or a
    new paragraph.

    The default value is -1.

    Note that QGraphicsTextItem keeps a QTextDocument internally, which is used
    to calculate the text width.
*/
void CGraphObjText::setTextWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fWidth);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTextWidth",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsTextItem_setTextWidth(i_fWidth);

    //QRectF rectBoundingTextItem = m_graphicsTextItem.boundingRect();
    //QSizeF size = rectBoundingTextItem.size();
    //if (!m_margins.isNull()) {
    //    size.setWidth(m_margins.left() + m_margins.right());
    //    size.setHeight(m_margins.top() + m_margins.bottom());
    //}
    //CPhysValSize physValSize(*m_pDrawingScene, size);
    //setSize(physValSize);
}

//------------------------------------------------------------------------------
/*! @brief Returns the text width.

    The width is calculated with the QTextDocument that QGraphicsTextItem keeps internally.
*/
double CGraphObjText::textWidth() const
//------------------------------------------------------------------------------
{
    return m_graphicsTextItem.textWidth();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the item's rectangle to the given rectangle.

    Depending on the Y axis scale orientation of the drawing scene the coordinates
    must be passed either relative to the top left or relative to the bottom left
    corner of the parent item's bounding rectangle.

    @param [in] i_physValRect
        Rectangle to be set in parent coordinates, depending on the Y scale orientation
        relative to the top left or bottom left corner of parent item's bounding rectangle.
        The passed rectangle may be rotated (the angle may be in range 0 .. 360°).
*/
void CGraphObjText::setRect(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValRect.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    bool bGeometryOnSceneChanged = false;
    if (m_physValRectScaledAndRotated != i_physValRect) {
        // Depending on the Y scale orientation of the drawing scene the rectangle coordinates
        // have been passed either relative to the top left or bottom left corner of the
        // parent item's bounding rectangle.
        // The coordinates need to be transformed into the local coordinate system of the graphical
        // object whose origin point is the center of the objects bounding rectangle.
        QPointF ptPosPrev = pos();
        QRectF rectF;
        CPhysVal physValAngle;
        QPointF ptPos = getItemPosAndLocalCoors(i_physValRect, rectF, physValAngle);

        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItem_prepareGeometryChange();

        {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
            CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

            // Store physical coordinates.
            CPhysValRect physValRect(i_physValRect);
            physValRect.setAngle(0.0);
            setPhysValRectOrig(physValRect);
            setPhysValRectScaled(physValRect);
            setPhysValRectScaledAndRotated(i_physValRect);

            // Set the rectangle in local coordinate system.
            // Also note that itemChange must not overwrite the current values (refCountGuard).
            setRectOrig(rectF);
            setRectScaled(rectF);

            // Move the object to the parent position.
            // This has to be done after resizing the groups rectangle which updates the local coordinates
            // of the rectangle with origin (0/0) at the rectangle's center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current rectangle coordinates (refCountGuard).
            if (ptPos != ptPosPrev) {
                QGraphicsItem_setPos(ptPos);
            }
            if (physValAngle != m_physValRotationAngle) {
                m_physValRotationAngle = physValAngle;
                QGraphicsItem_setRotation(m_physValRotationAngle.getVal(Units.Angle.Degree));
            }

            // The aggregated graphics text item needs to be positioned relative to the top left corner.
            QRectF rectBounding = getBoundingRect();
            QPointF ptTLTextItem(rectBounding.left() + m_margins.left(), rectBounding.top() + m_margins.top());
            QGraphicsTextItem_setPos(ptTLTextItem);
            double fTextWidth_px = rectBounding.width() - m_margins.left() - m_margins.right();
            QGraphicsTextItem_setTextWidth(fTextWidth_px > 0.0 ? fTextWidth_px : -1.0);
        }
        // If the geometry of the parent on the scene of this item changes, also the geometry
        // on the scene of this item is changed.
        bGeometryOnSceneChanged = true;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
void CGraphObjText::setRect(const QPointF& i_pTL, const QPointF& i_pBR, const ZS::PhysVal::CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + qPoint2Str(i_pTL) + "}, BR {" + qPoint2Str(i_pBR) + "} " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    setRect(CPhysValRect(*m_pDrawingScene, i_pTL, i_pBR, i_unit));
}

//------------------------------------------------------------------------------
void CGraphObjText::setRect(const QPointF& i_pTL, const QSizeF& i_size, const ZS::PhysVal::CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + qPoint2Str(i_pTL) + "}, Size {" + qSize2Str(i_size) + "} " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    setRect(CPhysValRect(*m_pDrawingScene, i_pTL, i_size, i_unit));
}

//------------------------------------------------------------------------------
void CGraphObjText::setRect(const CPhysValPoint& i_physValTL, const CPhysValPoint& i_physValBR)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + i_physValTL.toString() + "}, BR {" + i_physValBR.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    setRect(CPhysValRect(i_physValTL, i_physValBR));
}

//------------------------------------------------------------------------------
void CGraphObjText::setRect(const CPhysValPoint& i_physValTL, const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + i_physValTL.toString() + "}, Size {" + i_physValSize.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    setRect(CPhysValRect(i_physValTL, i_physValSize));
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's rectangle coordinates either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the current unit of the drawing scene.
*/
CPhysValRect CGraphObjText::getRect() const
//------------------------------------------------------------------------------
{
    return getRect(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's rectangle coordinates either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the given unit.

    @param [in] i_unit
        Unit in which the line coordinates should be returned.
    @param [in] i_rowVersion
        If set to Current (default), the scaled and rotated coordinates are returned.
        If set to Original, the unscaled and not rotated coordinates are returned.
        The Original coordinates are used by childs to calculate the parent's scale factor.

    @return Physical rectangle (scaled and rotated) in parent or scene coordinates,
            if the object has no parent group.
*/
CPhysValRect CGraphObjText::getRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (parentGroup() != nullptr) {
        return parentGroup()->convert(m_physValRectScaledAndRotated, i_unit);
    }
    else {
        return m_pDrawingScene->convert(m_physValRectScaledAndRotated, i_unit);
    }
}

//------------------------------------------------------------------------------
void CGraphObjText::setCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getCenter() const
//------------------------------------------------------------------------------
{
    return getRect().center();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getCenter(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).center();
}

//------------------------------------------------------------------------------
void CGraphObjText::setSize(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qSize2Str(i_size) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setSize(i_size);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValSize.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setSize(i_physValSize);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjText::getSize() const
//------------------------------------------------------------------------------
{
    return getRect().size();
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjText::getSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).size();
}

//------------------------------------------------------------------------------
void CGraphObjText::setWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fWidth);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidth(i_fWidth);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setWidth(const CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidth(i_physValWidth);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setWidthByMovingLeftCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidthByMovingLeftCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidthByMovingLeftCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValPoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidthByMovingLeftCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidthByMovingLeftCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setWidthByMovingRightCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidthByMovingRightCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidthByMovingRightCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValPoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidthByMovingRightCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidthByMovingRightCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjText::getWidth() const
//------------------------------------------------------------------------------
{
    return getRect().width();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjText::getWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).width();
}

//------------------------------------------------------------------------------
void CGraphObjText::setHeight(double i_fHeight)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fHeight);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeight(i_fHeight);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setHeight(const CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeight(i_physValHeight);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setHeightByMovingTopCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeightByMovingTopCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeightByMovingTopCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValPoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeightByMovingTopCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeightByMovingTopCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setHeightByMovingBottomCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeightByMovingBottomCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeightByMovingBottomCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValPoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeightByMovingBottomCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeightByMovingBottomCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjText::getHeight() const
//------------------------------------------------------------------------------
{
    return getRect().height();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjText::getHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).height();
}

//------------------------------------------------------------------------------
void CGraphObjText::setTopLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTopLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopLeft(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setTopLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTopLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopLeft(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getTopLeft() const
//------------------------------------------------------------------------------
{
    return getRect().topLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getTopLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).topLeft();
}

//------------------------------------------------------------------------------
void CGraphObjText::setTopRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTopRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopRight(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setTopRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTopRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopRight(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getTopRight() const
//------------------------------------------------------------------------------
{
    return getRect().topRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getTopRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).topRight();
}

//------------------------------------------------------------------------------
void CGraphObjText::setBottomRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setBottomRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomRight(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setBottomRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setBottomRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomRight(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getBottomRight() const
//------------------------------------------------------------------------------
{
    return getRect().bottomRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getBottomRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).bottomRight();
}

//------------------------------------------------------------------------------
void CGraphObjText::setBottomLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setBottomLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomLeft(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjText::setBottomLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setBottomLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomLeft(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getBottomLeft() const
//------------------------------------------------------------------------------
{
    return getRect().bottomLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjText::getBottomLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).bottomLeft();
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overloaded method to set the clockwise rotation angle, in degrees,
           around the Z axis.

    @note This method must be overwritten. Otherwise because of implicit conversion
          instead of setRotationAngle(double) the overloaded method
          setRotationAngle(const CPhysVal&) would be called. And the physValAngle
          value would not contain the unit.

    @param [in] i_fAngle_degree
        Rotation angle in degree.
*/
void CGraphObjText::setRotationAngle(double i_fAngle_degree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fAngle_degree, 'f', 3);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRotationAngle",
        /* strAddInfo   */ strMthInArgs );

    setRotationAngle(CPhysVal(i_fAngle_degree, Units.Angle.Degree, 0.1));
}

//------------------------------------------------------------------------------
void CGraphObjText::setRotationAngle(const CPhysVal& i_physValAngle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValAngle.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRotationAngle",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setAngle(i_physValAngle);
    setRect(physValRect);
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the virtual method of base class CGraphObj.
*/
void CGraphObjText::setEditMode(const CEnumEditMode& i_eMode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eMode.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setEditMode",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    if (m_editMode != i_eMode) {
        CEnumEditMode modePrev = m_editMode;
        m_editMode = i_eMode;
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        if (m_editMode == EEditMode::None) {
            QGraphicsItem_setAcceptHoverEvents(true);
        }
        //else if (m_editMode == EEditMode::ModifyingContent) {
        //    //hideSelectionPoints();
        //}
        else {
            if (m_editMode == EEditMode::CreatingByMouseEvents) {
                // The object is under construction. Hover events will not be accepted.
                // Only when no drawing tool is selected in the drawing scene, hover
                // events may be accepted.
                QGraphicsItem_setAcceptHoverEvents(false);
            }
            // Immediately select the object to create the selection points.
            // Following mouse move press, mouse move and mouse release events will
            // be forwarded by the scene to the selection point responsible for
            // resizing the obejct (the top most selection point most recently created).
            if (!pGraphicsItemThis->isSelected()) {
                QGraphicsItem_setSelected(true);
            }
            else if (m_editMode == EEditMode::CreatingByMouseEvents || m_editMode == EEditMode::ModifyingPolygonPoints) {
                showSelectionPoints(c_uSelectionPointsPolygonPoints);
                hideSelectionPoints(c_uSelectionPointsBoundingRectAll);
            }
            else if (m_editMode == EEditMode::ModifyingBoundingRect || m_editMode == EEditMode::ModifyingContent) {
                hideSelectionPoints(c_uSelectionPointsPolygonPoints);
                showSelectionPoints(c_uSelectionPointsBoundingRectAllExcludingCenter);
            }
        }
        emit_editModeChanged(m_editMode, modePrev);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the virtual method of base class CGraphObj.
*/
QRectF CGraphObjText::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(m_rectScaled) + "}");
    }
    return m_rectScaled;
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotated, physical bounding rectangle.
*/
CPhysValRect CGraphObjText::getPhysValBoundingRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectBounding = m_physValRectScaledAndRotated;
    if (parentGroup() != nullptr) {
        physValRectBounding = parentGroup()->convert(physValRectBounding, i_unit);
    }
    else {
        physValRectBounding = m_pDrawingScene->convert(physValRectBounding, i_unit);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + m_physValRectScaledAndRotated.toString(true) + "}");
    }
    return physValRectBounding;
}

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::showSelectionPoints(TSelectionPointTypes i_selPts)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + selectionPointTypes2Str(i_selPts) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if (parentItem() == nullptr) {
        showSelectionPointsOfBoundingRect(getBoundingRect(), (i_selPts & c_uSelectionPointsBoundingRectAll));
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj (text labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the list of the possible anchor points for the given label name.

    For the predefined labels of the group the following applies:

    - The "Name" label may be anchored to the center point of the bounding rectangle
      and the center points of the border lines.
    - "TopLeft", "TopRight", "BottomRight" and "BottomLeft" may be anchored to their
       corresponding bounding rectangle corners.

    User defined labels may be anchored to any selection point at the bounding rectangle.

    Please note that the most common used selection points should be at the beginning
    of the list so that combo boxes to select the selection point start with those.

    @return List of possbile selection points.
*/
QList<SGraphObjSelectionPoint> CGraphObjText::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    static QList<SGraphObjSelectionPoint> s_arSelPtsUserDefined;
    if (s_arSelPtsUserDefined.isEmpty()) {
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopLeft));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopRight));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomRight));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomLeft));
    }
    static QHash<QString, QList<SGraphObjSelectionPoint>> s_hshSelPtsPredefined;
    if (s_hshSelPtsPredefined.isEmpty()) {
        QList<SGraphObjSelectionPoint> arSelPts;
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopCenter));
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomCenter));
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter));
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter));
        s_hshSelPtsPredefined.insert(c_strLabelName, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopLeft));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameTopLeft, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopRight));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameTopRight, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomRight));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameBottomRight, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjText*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomLeft));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameBottomLeft, arSelPts);
    }
    if (s_hshSelPtsPredefined.contains(i_strName)) {
        return s_hshSelPtsPredefined.value(i_strName);
    }
    return s_arSelPtsUserDefined;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the label with the passed name has been modified or still
           has its default values.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the label still has its default values, false otherwise.
*/
bool CGraphObjText::labelHasDefaultValues(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bHasDefaultValues = false;
    if (isPredefinedLabelName(i_strName)) {
        bHasDefaultValues = true;
        const SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
        if (labelDscr.m_bLabelIsVisible) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_bShowAnchorLine) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_polarCoorsToLinkedSelPt != SPolarCoors()) {
            bHasDefaultValues = false;
        }
        else if (i_strName == c_strLabelName) {
            if (labelDscr.m_strText != m_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::Center) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameTopLeft) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopLeft) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameTopRight) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopRight) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomRight) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomRight) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomLeft) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomLeft) {
                bHasDefaultValues = false;
            }
        }
    }
    return bHasDefaultValues;
}

/*==============================================================================
public: // overridables of base class CGraphObj (geometry labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether the label with the passed name has been modified or still
           has its default values.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the label still has its default values, false otherwise.
*/
bool CGraphObjText::geometryLabelHasDefaultValues(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bHasDefaultValues = false;
    if (m_strlstGeometryLabelNames.contains(i_strName)) {
        bHasDefaultValues = true;
        const SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
        if (labelDscr.m_bLabelIsVisible) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_bShowAnchorLine) {
            bHasDefaultValues = false;
        }
        else if (!labelDscr.m_strText.isEmpty()) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_polarCoorsToLinkedSelPt != SPolarCoors()) {
            bHasDefaultValues = false;
        }
        else if (i_strName == c_strGeometryLabelNameTopLeft) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopLeft) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameTopRight) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopRight) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomRight) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomRight) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomLeft) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomLeft) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameCenter) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::Center) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameWidth) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::LeftCenter) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::RightCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameHeight) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopCenter) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::BottomCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameAngle) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::Center) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::RightCenter) {
                bHasDefaultValues = false;
            }
        }
    }
    return bHasDefaultValues;
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjText::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = m_graphicsTextItem.boundingRect();
    if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
        // Half pen width of the selection rectangle would be enough.
        // But the whole pen width is also not a bad choice.
        rctBounding.adjust(-2.0, -2.0, 2.0, 2.0);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Reimplements QGraphicsItem::shape.
*/
QPainterPath CGraphObjText::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    QPainterPath painterPath = m_graphicsTextItem.shape();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        QString strMthRet = qPainterPath2Str(pVThis, painterPath);
        mthTracer.setMethodReturn(strMthRet);
    }
    return painterPath;
}

//------------------------------------------------------------------------------
void CGraphObjText::paint(
    QPainter* i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget* i_pWdgt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ZValue: " + QString::number(zValue());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceDrawSettings(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    i_pPainter->save();
    i_pPainter->setRenderHints(s_painterRenderHints);

    QPen pn;
    QBrush brush;
    QRectF rctBounding = getBoundingRect();
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
        if (isSelected()) {
            pn.setColor(s_selectionColor);
            pn.setWidth(3 + m_drawSettings.penWidth());
        }
        else {
            pn.setColor(s_highlightColor);
            pn.setWidth(3 + m_drawSettings.penWidth());
        }
        pn.setStyle(Qt::SolidLine);
        QPainterPath outline;
        outline.moveTo(rctBounding.topLeft());
        outline.lineTo(rctBounding.topRight());
        outline.lineTo(rctBounding.bottomRight());
        outline.lineTo(rctBounding.bottomLeft());
        outline.lineTo(rctBounding.topLeft());
        i_pPainter->strokePath(outline, pn);
    }

    pn.setColor(m_drawSettings.penColor());
    pn.setWidth(m_drawSettings.penWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.lineStyle().enumerator()));
    i_pPainter->setPen(pn);

    brush.setColor(m_drawSettings.fillColor());
    brush.setStyle(fillStyle2QtBrushStyle(m_drawSettings.fillStyle().enumerator()));
    i_pPainter->setBrush(brush);

    i_pPainter->drawRect(rctBounding);

    #if 0
    QRectF rectBoundingTextItem = m_graphicsTextItem.boundingRect();
    rectBoundingTextItem = m_graphicsTextItem.mapRectToParent(rectBoundingTextItem);
    QBrush brushTextItem;
    brushTextItem.setColor(Qt::lightGray);
    brushTextItem.setStyle(Qt::SolidPattern);
    i_pPainter->setBrush(brushTextItem);
    QPen penTextItem;
    penTextItem.setColor(Qt::red);
    penTextItem.setStyle(Qt::SolidLine);
    i_pPainter->setPen(penTextItem);
    i_pPainter->drawRect(rectBoundingTextItem);
    #endif

    i_pPainter->restore();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEnterLeaveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEnterLeaveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    // Ignore hover events if any object should be or is currently being created.
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        QGraphicsItem_setCursor(Qt::SizeAllCursor);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjText::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    // Ignore hover events if any object should be or is currently being created.
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        QGraphicsItem_setCursor(Qt::SizeAllCursor);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjText::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEnterLeaveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEnterLeaveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem_unsetCursor();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
        traceThisPositionInfo(mthTracer, EMethodDir::Enter, "Common");
    }

    bool bCallBaseMouseEventHandler = true;
    if (i_pEv->button() == Qt::LeftButton) {
        if (m_editMode == EEditMode::None) {
            setEditMode(EEditMode::ModifyingBoundingRect);
        }
    }
    else if (i_pEv->button() == Qt::RightButton) {
        showContextMenu(i_pEv);
        bCallBaseMouseEventHandler = false;
    }
    if (bCallBaseMouseEventHandler) {
        // Forward the mouse event to the base implementation.
        // This will select the item, creating selection points if not yet created.
        QGraphicsItem::mousePressEvent(i_pEv);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
        traceThisPositionInfo(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjText::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Enter, "Common");
    }

    if (m_editMode == EEditMode::CreatingByMouseEvents) {
        // The editMode changed signal will be emitted and received by the drawing scene.
        // The drawing scene is informed this way that creation of the object is finished
        // and will unselect the current drawing tool and will select the object under
        // construction showing the selection points at the bounding rectangle.
        setEditMode(EEditMode::ModifyingBoundingRect);
    }

    // Forward the mouse event to the items base implementation.
    QGraphicsItem::mouseReleaseEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjText::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    // When double clicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    //QGraphicsRectItem::mouseDoubleClickEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjText::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    // Forward the mouse event to the items base implementation.
    // This will move the item resulting in an itemChange call with PositionHasChanged.
    QGraphicsItem::mouseMoveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qKeyEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );

    //QGraphicsTextItem::keyPressEvent(i_pEv);
    QGraphicsItem::keyPressEvent(i_pEv);
}

//------------------------------------------------------------------------------
void CGraphObjText::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        qKeyEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    //QGraphicsTextItem::keyReleaseEvent(i_pEv);
    QGraphicsItem::keyReleaseEvent(i_pEv);
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjText::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if (m_bDtorInProgress) {
        return i_value;
    }
    if (m_iItemChangeBlockedCounter > 0) {
        return i_value;
    }

    CTrcAdminObj* pTrcAdminObj = selectTraceAdminObj(i_change);
    QString strMthInArgs;
    if (areMethodCallsActive(pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    CGraphObj* pGraphObjThis = dynamic_cast<CGraphObj*>(this);
    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);

    QVariant valChanged = i_value;

    bool bGeometryChanged = false;
    bool bSelectedChanged = false;
    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                tracePositionInfo(mthTracer, EMethodDir::Enter);
            }
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
        }
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemPositionHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                tracePositionInfo(mthTracer, EMethodDir::Enter);
            }
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // If the item is not a group and as long as the item is not added as a child to
            // a group, the current (transformed) and original coordinates are equal.
            // If the item is a child of a group, the current (transformed) coordinates are only
            // taken over as the original coordinates if initially creating the item or when
            // adding the item to or removing the item from a group.
            updateTransformedCoorsOnItemPositionChanged();
            bGeometryChanged = true;
        }
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemRotationHasChanged) {
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            tracePositionInfo(mthTracer, EMethodDir::Enter);
        }
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemSelectedHasChanged) {
        //QGraphicsItem_prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            if (m_editMode == EEditMode::CreatingByMouseEvents || m_editMode == EEditMode::ModifyingPolygonPoints) {
                hideSelectionPoints(c_uSelectionPointsPolygonPoints);
                showSelectionPoints(c_uSelectionPointsBoundingRectAllExcludingCenter);
            }
            else if (m_editMode == EEditMode::ModifyingBoundingRect || m_editMode == EEditMode::ModifyingContent) {
                hideSelectionPoints(c_uSelectionPointsPolygonPoints);
                showSelectionPoints(c_uSelectionPointsBoundingRectAllExcludingCenter);
            }
            else /*if (m_editMode == EEditMode::None)*/ {
                hideSelectionPoints();
            }
        }
        else {
            setEditMode(EEditMode::None);
            hideSelectionPoints();
            resetStackingOrderValueToOriginalValue(); // restore ZValue as before selecting the object
        }
        bSelectedChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemZValueHasChanged) {
        bZValueChanged = true;
        bTreeEntryChanged = true;
    }

    if (bGeometryChanged) {
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            tracePositionInfo(mthTracer, EMethodDir::Leave);
        }
        emit_geometryOnSceneChanged();
    }
    if (bSelectedChanged) {
        emit_selectedChanged(isSelected());
    }
    if (bZValueChanged) {
        emit_zValueChanged(zValue());
    }
    if (bTreeEntryChanged && m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsItem::itemChange(i_change, i_value);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;
}

/*==============================================================================
protected: // overridable slots of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjText::onGraphObjParentGeometryOnSceneChanged(
    CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged)
//------------------------------------------------------------------------------
{
    if (m_iIgnoreParentGeometryChange > 0) {
        return;
    }
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree() + ", ParentOfParentChanged: " + bool2Str(i_bParentOfParentChanged);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onGraphObjParentGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    bool bGeometryOnSceneChanged = false;

    {   CRefCountGuard refCountGuardTracePositionInfo(&m_iTracePositionInfoBlockedCounter);

        if (i_pGraphObjParent->isGroup()) {
            CGraphObjGroup* pGraphObjGroupParent = dynamic_cast<CGraphObjGroup*>(i_pGraphObjParent);
            if (i_bParentOfParentChanged) {
                initParentTransform();
                updateTransformedCoorsOnParentGeometryChanged();
            }
            CPhysValRect physValRectGroupParentCurr = pGraphObjGroupParent->getRect(m_physValRectParentGroupOrig.unit());
            if (m_physValRectParentGroupOrig.width().getVal() > 0.0) {
                setParentGroupScaleX(physValRectGroupParentCurr.width().getVal() / m_physValRectParentGroupOrig.width().getVal());
            }
            if (m_physValRectParentGroupOrig.height().getVal() > 0.0) {
                setParentGroupScaleY(physValRectGroupParentCurr.height().getVal() / m_physValRectParentGroupOrig.height().getVal());
            }

            // The relative distance of the center point to the top left or bottom left corner
            // of the parent's bounding rectangle should remain the same.
            CPhysValRect physValRect = getPhysValRectScaled(m_physValRectOrig);
            setPhysValRectScaled(physValRect);
            physValRect.setAngle(m_physValRotationAngle);
            setPhysValRectScaledAndRotated(physValRect);

            QPointF ptPosPrev = pos();

            QRectF rectF;
            CPhysVal physValAngle;
            QPointF ptPos = getItemPosAndLocalCoors(physValRect, rectF, physValAngle);

            // Prepare the item for a geometry change. This function must be called before
            // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
            QGraphicsItem_prepareGeometryChange();

            {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
                CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

                // Set the rectangle in local coordinate system.
                // Also note that itemChange must not overwrite the current coordinates (refCountGuard).
                setRectScaled(rectF);

                // Move the object to the parent position.
                // This has to be done after resizing the item which updates the local coordinates
                // of the item with origin (0/0) at the center point.
                // "setPos" will trigger an itemChange call which will update the position of the
                // selection points and labels. To position the selection points and labels correctly
                // the local coordinate system must be up-to-date.
                // Also note that itemChange must not overwrite the current coordinates (refCountGuard).
                // If the position is not changed, itemChange is not called with PositionHasChanged and
                // the position of the arrow heads will not be updated. We got to do this here "manually".
                if (ptPos != ptPosPrev) {
                    QGraphicsItem_setPos(ptPos);
                }

                // The aggregated graphics text item needs to be positioned relative to the top left corner.
                // But neither the text width nor the font size is changed so that the text and its new line
                // breaks remain the same.
                QRectF rectBounding = getBoundingRect();
                QPointF ptTLTextItem(rectBounding.left() + m_margins.left(), rectBounding.top() + m_margins.top());
                QGraphicsTextItem_setPos(ptTLTextItem);
            }
            // If the geometry of the parent on the scene of this item changes, also the geometry
            // on the scene of this item is changed.
            bGeometryOnSceneChanged = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }

    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjText::onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pSelectionPoint->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onSelectionPointGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QPointF ptPosThis = pos();
    CGraphObjSelectionPoint* pGraphObjSelPt = dynamic_cast<CGraphObjSelectionPoint*>(i_pSelectionPoint);
    QGraphicsItem* pGraphicsItemSelPt = dynamic_cast<QGraphicsItem*>(pGraphObjSelPt);
    QPointF ptScenePosSelPt = pGraphicsItemSelPt->scenePos();
    QPointF ptPosSelPt = mapFromScene(ptScenePosSelPt);
    QPointF ptParentPosSelPt = pGraphicsItemThis->mapToParent(ptPosSelPt);
    CPhysValPoint physValPointParentSelPt(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        physValPointParentSelPt = parentGroup()->convert(ptParentPosSelPt);
    }
    else {
        physValPointParentSelPt = m_pDrawingScene->convert(ptParentPosSelPt);
    }

    SGraphObjSelectionPoint selPt = pGraphObjSelPt->getSelectionPoint();
    if (selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
        // Moving a selection point will modify the shape of the object and the position
        // of all other selection points got to be updated. If the position of the other
        // selection points will be changed, those selection points are emitting the
        // geometryOnSceneChanged signal whereupon this slot method would be called again
        // for each other selection point. This will not end up in an endless loop but
        // is useless and anything else but performant. So the slot will be temporarily
        // disconnected from the geometryOnSceneChanged signal of the selection points.
        disconnectGeometryOnSceneChangedSlotFromSelectionPoints();

        switch (selPt.m_selPt) {
            case ESelectionPoint::TopLeft: {
                setTopLeft(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::TopRight: {
                setTopRight(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::BottomRight: {
                setBottomRight(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::BottomLeft: {
                setBottomLeft(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::TopCenter: {
                setHeightByMovingTopCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::RightCenter: {
                setWidthByMovingRightCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::BottomCenter: {
                setHeightByMovingBottomCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::LeftCenter: {
                setWidthByMovingLeftCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::Center: {
                setCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::RotateTop: {
                // The angle returned by getAngleDegree is counted counterclockwise with 0° at 3 o'clock.
                double fAngle_degree = ZS::Draw::getAngleDegree(ptPosThis, ptParentPosSelPt);
                // setRotationAngle expects the angle counted clockwise with 0° at 3 o'clock.
                fAngle_degree = ZS::System::Math::toClockWiseAngleDegree(fAngle_degree);
                // RotateTop is at 270°.
                fAngle_degree -= 270.0;
                fAngle_degree = ZS::System::Math::normalizeAngleInDegree(fAngle_degree);
                setRotationAngle(fAngle_degree);
                break;
            }
            case ESelectionPoint::RotateBottom: {
                double fAngle_degree = ZS::Draw::getAngleDegree(ptPosThis, ptParentPosSelPt);
                fAngle_degree = ZS::System::Math::toClockWiseAngleDegree(fAngle_degree);
                // RotateBottom is at 90°.
                fAngle_degree -= 90.0;
                fAngle_degree = ZS::System::Math::normalizeAngleInDegree(fAngle_degree);
                setRotationAngle(fAngle_degree);
                break;
            }
            default: {
                break;
            }
        }
        connectGeometryOnSceneChangedSlotWithSelectionPoints();
    }
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjText::updateTransformedCoorsOnParentChanged(
    CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "PrevGroup: " + QString(i_pGraphObjGroupPrev == nullptr ? "null" : i_pGraphObjGroupPrev->path()) +
            ", NewGroup: " + QString(i_pGraphObjGroupNew == nullptr ? "null" : i_pGraphObjGroupNew->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnParentChanged",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        setRectOrig(m_rectScaled);
        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        // The relative rotation angle on the screen should remain the same.
        if (i_pGraphObjGroupPrev != nullptr) {
            #pragma message(__TODO__"PhysVal: round2Resolution is different from round2Accuracy.")
            #pragma message(__TODO__"PhysVal: Keep both resolution and accuracy in CPhysVal.")
            #pragma message(__TODO__"PhysVal: Values with resolution may also have an accuracy.")
            #pragma message(__TODO__"PhysVal: Resolutions should not be added, but accuracies should be added.")
            CPhysValRes physValRes = m_physValRotationAngle.getRes();
            CPhysVal physValRotationAngleParentGroup = i_pGraphObjGroupPrev->rotationAngle();
            if (i_pGraphObjGroupNew == nullptr) {
                m_physValRotationAngle += i_pGraphObjGroupPrev->mapRotationAngleToScene(physValRotationAngleParentGroup);
            }
            else {
                m_physValRotationAngle += i_pGraphObjGroupPrev->mapRotationAngleTo(physValRotationAngleParentGroup, i_pGraphObjGroupNew);
            }
            // Keep the resolution.
            m_physValRotationAngle.setRes(physValRes);
        }
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjText::updateTransformedCoorsOnParentGeometryChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnParentGeometryChanged",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    // ItemChange will be called but should not emit the geometryOnSceneChanged signal.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        setRectOrig(m_rectScaled);
        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjText::updateTransformedCoorsOnItemPositionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnItemPositionChanged",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    // ItemChange will be called but should not emit the geometryOnSceneChanged signal.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    emit_geometryOnSceneChanged();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::onGraphicsTextItemFocusChanged(QFocusEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qFocusEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onGraphicsTextItemFocusChanged",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
    }

    // On setting the input focus on the text item (on starting the text editor) no other
    // graphics item than this text object should be selected. As the mouse press event
    // is eaten by the text item and the drawing scene does not deselect currently selected
    // items, that's been done here.
    QList<QGraphicsItem*> arpGraphicsItemsSelected = m_pDrawingScene->selectedItems();
    for (QGraphicsItem* pGraphicsItemSelected : arpGraphicsItemsSelected) {
        if (pGraphicsItemSelected != &m_graphicsTextItem) {
            pGraphicsItemSelected->setSelected(false);
        }
    }

    if (i_pEv->gotFocus() && !i_pEv->lostFocus()) {
        if (!isSelected()) {
            setEditMode(EEditMode::ModifyingContent);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
    }
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated rectangle in pixels in item coordinates
           relative to the origin (center point) of the orignal bounding rectangle.

    The relative distance of the center point of the scaled rectangle to the
    origin (center point) of the parent's bounding rectangle remains the same.
    The width and height are scaled to the scale factors of the parent group.

    @param [in] i_rectOrig
        Unscaled rectangle in the item's local coordinates system whose origin
        is the center point of the item.
    @return Scaled rectangle.
*/
QRectF CGraphObjText::getRectScaled(const QRectF& i_rectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qRect2Str(i_rectOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getRectScaled",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptCenter(m_fParentGroupScaleX * i_rectOrig.center().x(), m_fParentGroupScaleY * i_rectOrig.center().y());
    QSizeF sizeF(m_fParentGroupScaleX * i_rectOrig.width(), m_fParentGroupScaleY * i_rectOrig.height());
    QRectF rectF;
    rectF.setSize(sizeF);
    rectF.moveCenter(ptCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rectF) + "}");
    }
    return rectF;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the unscaled and not rotated physical rectangle in the
           current unit of the drawing scene relative to the top left or
           bottom left corner of the parent's bounding rectangle.

    @param [in] i_rectOrig
        Unscaled, not rotated rectangle in the item's local coordinates system
        whose origin is the center point of the item.
    @return Physical rectangle whose origin is either the top left or bottom
            left corner of the parent's bounding rectangle.
*/
CPhysValRect CGraphObjText::getPhysValRectOrig(const QRectF& i_rectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qRect2Str(i_rectOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValRectOrig",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjText* pVThis = const_cast<CGraphObjText*>(this);
    double fRotationAngle_degree = m_physValRotationAngle.getVal(Units.Angle.Degree);
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(0.0);
    }
    CPhysValPoint physValPointCenter(*m_pDrawingScene);
    CPhysValSize physValSize(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        QPointF ptCenter = pGraphicsItemThis->mapToParent(i_rectOrig.center());
        ptCenter = parentGroup()->mapToTopLeftOfBoundingRect(ptCenter);
        physValPointCenter = parentGroup()->convert(ptCenter);
        physValSize = parentGroup()->convert(i_rectOrig.size());
    }
    else {
        // Please note that "mapToScene" maps the local coordinates relative to the
        // top left corner of the item's bounding rectangle and there is no need to
        // call "mapToBoundingRectTopLeft" beforehand.
        QPointF ptCenter = pGraphicsItemThis->mapToScene(i_rectOrig.center());
        physValPointCenter = m_pDrawingScene->convert(ptCenter);
        physValSize = m_pDrawingScene->convert(i_rectOrig.size());
    }
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(fRotationAngle_degree);
    }

    CPhysValRect physValRect(*m_pDrawingScene);
    physValRect.setSize(physValSize);
    physValRect.setCenter(physValPointCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated physical rectangle in the current
           unit of the drawing scene relative to the top left or bottem left
           corner of the parent's bounding rectangle.

    The relative distance of the center point of the scaled rectangle to the
    top left or bottom left corner of the parent's bounding rectangle remains the same.
    The width and height are scaled to the scale factors of the parent group.

    @param [in] i_physValRectOrig
        Unscaled rectangle in the parent's, physical coordinates system whose origin
        is the top left or bottom right corner of the parent's bounding rectangle.
    @return Scaled rectangle.
*/
CPhysValRect CGraphObjText::getPhysValRectScaled(const CPhysValRect& i_physValRectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValRectOrig.toString() + "} " + i_physValRectOrig.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValRectScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPoint physValPointCenter(*m_pDrawingScene);
    physValPointCenter.setX(m_fParentGroupScaleX * i_physValRectOrig.center().x().getVal());
    physValPointCenter.setY(m_fParentGroupScaleY * i_physValRectOrig.center().y().getVal());
    CPhysValSize physValSize(*m_pDrawingScene);
    physValSize.setWidth(m_fParentGroupScaleX * i_physValRectOrig.width().getVal());
    physValSize.setHeight(m_fParentGroupScaleY * i_physValRectOrig.height().getVal());

    CPhysValRect physValRect(*m_pDrawingScene);
    physValRect.setSize(physValSize);
    physValRect.setCenter(physValPointCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Set the original, untransformed (not scaled, not rotated) rectangle
           coordinates in local coordinates relative to the origin of the
           groups bounding rectangle

    @param [in] i_rect
        Rectangle coordinates in local coordinates to be set.

    @return Previous original rectangle coordinates.
*/
QRectF CGraphObjText::setRectOrig(const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRectOrig",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = m_rectOrig;
    m_rectOrig = i_rect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qRect2Str(rectPrev) + "}");
    }
    return rectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated rectangle coordinates in local
           coordinates relative to the origin of the groups bounding rectangle.

    Other graphics items, like Line, provide methods to set and retrieve the local
    coordinates (e.g. "setLine", "line"). The text item does not have such methods
    (e.g. "setRect", "rect"). To provide the bounding rectangle of the text in
    local coordinates this member is maintained.

    The scaled rectangle is returned by the "boundingRect" method of the graphics
    item to provide the bounding rectangle of the text in local coordinates.

    @param [in] i_rect
        Rectangle coordinates in local coordinates to be set.

    @return Previous rectangle coordinates.
*/
QRectF CGraphObjText::setRectScaled(const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRectScaled",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = m_rectScaled;
    m_rectScaled = i_rect;

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qRect2Str(rectPrev) + "}");
    }
    return rectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the original, untransformed (not scaled, not rotated) rectangle
           coordinates with unit in parent coordinates relative to the top left
           or bottom left corner of the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous original rectangle coordinates.
*/
CPhysValRect CGraphObjText::setPhysValRectOrig(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectOrig;
    m_physValRectOrig = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated rectangle coordinates with unit in
           parent coordinates relative to the top left or bottom left corner of
           the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous rectangle coordinates.
*/
CPhysValRect CGraphObjText::setPhysValRectScaled(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectScaled;
    m_physValRectScaled = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled and rotated rectangle coordinates with unit in parent
           coordinates relative to the top left or bottom left corner of the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous rectangle coordinates.
*/
CPhysValRect CGraphObjText::setPhysValRectScaledAndRotated(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectScaledAndRotated",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectScaledAndRotated;
    m_physValRectScaledAndRotated = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the prepareGeometryChange method of
           graphics item to trace the method call.

    As the prepareGeometryChange method is a protected method of QGraphicsItem
    this method must be reimplemented by the derived classes.
*/
void CGraphObjText::QGraphicsItem_prepareGeometryChange()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsItem_prepareGeometryChange",
        /* strAddInfo   */ "" );

    prepareGeometryChange();
}

//------------------------------------------------------------------------------
QPointF CGraphObjText::QGraphicsTextItem_setPos(const QPointF& i_pos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qPoint2Str(i_pos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsTextItem_setPos",
        /* strAddInfo   */ strMthInArgs );

    QPointF posPrev = m_graphicsTextItem.pos();
    m_graphicsTextItem.setPos(i_pos);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qPoint2Str(i_pos) + "}");
    }
    return posPrev;
}

//------------------------------------------------------------------------------
double CGraphObjText::QGraphicsTextItem_setTextWidth(double i_fWidth_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New: " + QString::number(i_fWidth_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsTextItem_setTextWidth",
        /* strAddInfo   */ strMthInArgs );

    double fWidthPrev_px = m_graphicsTextItem.textWidth();
    m_graphicsTextItem.setTextWidth(i_fWidth_px);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fWidthPrev_px) + " px");
    }
    return fWidthPrev_px;
}

//------------------------------------------------------------------------------
Qt::TextInteractionFlags CGraphObjText::QGraphicsTextItem_setTextInteractionFlags(Qt::TextInteractionFlags i_flags)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qTextInteractionFlags2Str(i_flags);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsTextItem_setTextInteractionFlags",
        /* strAddInfo   */ strMthInArgs );

    Qt::TextInteractionFlags flagsPrev = m_graphicsTextItem.textInteractionFlags();
    m_graphicsTextItem.setTextInteractionFlags(i_flags);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + qTextInteractionFlags2Str(flagsPrev));
    }
    return flagsPrev;
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjText::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }

    CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter);

    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "GraphObjText {";
    i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = " . ";
        strRuntimeInfo += ". PhysValRect Orig {" + m_physValRectOrig.toString() + "} " + m_physValRectOrig.unit().symbol()
            + ", Scaled {" + m_physValRectScaled.toString() + "} " + m_physValRectScaled.unit().symbol();
            + ", ScaledRotated {" + m_physValRectScaledAndRotated.toString() + "} " + m_physValRectScaledAndRotated.unit().symbol();
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = " . ";
        strRuntimeInfo += ". ItemRect Orig {" + qRect2Str(m_rectOrig) + "}, Scaled {" + qRect2Str(m_rectScaled) + "}";
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = " . ";
        strRuntimeInfo += ". Margins {" + qMargins2Str(m_margins) + "}";
        i_mthTracer.trace(strRuntimeInfo);
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "}";
    i_mthTracer.trace(strRuntimeInfo);

    traceTextItemPositionInfo(i_mthTracer, i_mthDir, i_strFilter);
}

//------------------------------------------------------------------------------
void CGraphObjText::traceTextItemPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }
    QPointF ptPos = m_graphicsTextItem.pos();
    QPointF ptScenePos = m_graphicsTextItem.scenePos();
    QRectF rectBounding = m_graphicsTextItem.boundingRect();
    QPointF ptCenterPos = rectBounding.center();
    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "TextItem {";
    i_mthTracer.trace(strRuntimeInfo);
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += ". RotationAngle: " + QString::number(m_graphicsTextItem.rotation()) + " " + ZS::System::Math::c_chSymbolDegree;
    i_mthTracer.trace(strRuntimeInfo);
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += ". BoundingRect {" + qRect2Str(rectBounding) + "}, Center {" + qPoint2Str(ptCenterPos) + "}";
    i_mthTracer.trace(strRuntimeInfo);
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += ". Pos {" + qPoint2Str(ptPos) + "}, ScenePos {" + qPoint2Str(ptScenePos) + "}";
    i_mthTracer.trace(strRuntimeInfo);
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += ". TextWidth: " + QString::number(m_graphicsTextItem.textWidthNoMethodTrace()) + " px";
    i_mthTracer.trace(strRuntimeInfo);

    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = ". -+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = ". +- ";
    else strRuntimeInfo = " . . ";
    strRuntimeInfo += "Document {";
    i_mthTracer.trace(strRuntimeInfo);
    if (m_graphicsTextItem.document() == nullptr) {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = ". -+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = ". +- ";
        else strRuntimeInfo = " . . ";
        strRuntimeInfo += ". null";
        i_mthTracer.trace(strRuntimeInfo);
    }
    else {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = ". -+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = ". +- ";
        else strRuntimeInfo = " . . ";
        strRuntimeInfo += ". TextWidth: " + QString::number(m_graphicsTextItem.document()->textWidth()) + " px";
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = ". -+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = ". +- ";
        else strRuntimeInfo = " . . ";
        strRuntimeInfo += ". IdealWidth: " + QString::number(m_graphicsTextItem.document()->idealWidth()) + " px";
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = ". -+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = ". +- ";
        else strRuntimeInfo = " . . ";
        strRuntimeInfo += ". Size {" + qSize2Str(m_graphicsTextItem.document()->size()) + "} px";
        i_mthTracer.trace(strRuntimeInfo);
    }
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . . ";
    strRuntimeInfo += "}";
    i_mthTracer.trace(strRuntimeInfo);

    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "}";
    i_mthTracer.trace(strRuntimeInfo);
}
