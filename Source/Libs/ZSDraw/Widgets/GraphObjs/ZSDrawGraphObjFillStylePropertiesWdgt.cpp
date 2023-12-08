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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjFillStylePropertiesWdgt.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>
#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcolordialog.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjFillStyleProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjFillStyleProperties::CWdgtGraphObjFillStyleProperties(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strNameSpace,
    const QString& i_strGraphObjType,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(
        i_pDrawingScene,
        i_strNameSpace, i_strGraphObjType,
        ClassName(), i_strObjName, i_pWdgtParent),
    // Caching values
    m_fillColor(Qt::white),
    m_fillStyle(EFillStyle::NoFill),
    // Edit Controls
    m_pWdgtHeadline(nullptr),
    m_pLytWdgtHeadline(nullptr),
    m_pxmBtnDown(":/ZS/Button/ButtonArrowDown.png"),
    m_pxmBtnUp(":/ZS/Button/ButtonArrowUp.png"),
    m_pBtnCollapse(nullptr),
    m_pLblHeadlineIcon(nullptr),
    m_pLblHeadline(nullptr),
    m_pSepHeadline(nullptr),
    m_pWdgtFillStyleSettings(nullptr),
    m_pLytWdgtFillStyleSettings(nullptr),
    // Fill Style
    m_pLblFillStyle(nullptr),
    m_pModelFillStyles(nullptr),
    m_pCmbFillStyle(nullptr),
    m_pPxmBtnFillColor(nullptr),
    m_pBtnFillColor(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // We need to fill the edit controls if the graphical object emits the drawSettingsChanged signal.
    // The flag is checked if "setKeyInTree" is called.
    m_bContentUpdateOnDrawSettingsChanged = true;

    // <Widget> Headline
    //==================

    m_pWdgtHeadline = new QWidget();
    m_pLytWdgtHeadline = new QHBoxLayout();
    m_pLytWdgtHeadline->setContentsMargins(0, 0, 0, 0);
    m_pWdgtHeadline->setLayout(m_pLytWdgtHeadline);
    m_pLyt->addWidget(m_pWdgtHeadline);

    m_pBtnCollapse = new QPushButton();
    if (s_bWdgtFillStyleVisible) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
    }
    else {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
    }
    m_pLytWdgtHeadline->addWidget(m_pBtnCollapse);

    QObject::connect(
        m_pBtnCollapse, &QPushButton::clicked,
        this, &CWdgtGraphObjFillStyleProperties::onBtnCollapseClicked);

    QPixmap pxmHeadline(":/ZS/Draw/DrawSettingsBrushStyle16x16.png");
    m_pLblHeadlineIcon = new QLabel();
    m_pLblHeadlineIcon->setPixmap(pxmHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadlineIcon);

    m_pLblHeadline = new QLabel("Fill Style");
    QFont fntHeadline = m_pLblHeadline->font();
    fntHeadline.setPointSize(fntHeadline.pointSize() + 2);
    m_pLblHeadline->setFont(fntHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadline);

    m_pSepHeadline = new CSepLine();
    m_pLytWdgtHeadline->addWidget(m_pSepHeadline, 1);

    m_pLytWdgtHeadline->addStretch();

    // <Widget> Style
    //===============

    QSize iconSize;

    m_pWdgtFillStyleSettings = new QWidget();
    m_pLytWdgtFillStyleSettings = new QGridLayout();
    m_pLytWdgtFillStyleSettings->setContentsMargins(0, 0, 0, 0);
    m_pWdgtFillStyleSettings->setLayout(m_pLytWdgtFillStyleSettings);
    m_pLyt->addWidget(m_pWdgtFillStyleSettings);

    /* Grid Layout (alternative 2)
         |     0    |1| 2 |3| 4   |5|  6  | 
       --+----------+-+---+-+-----+-+-----+----
       0 |Brush:    | |   | |Style| |Color|<-->
    */
    int iClmCount = 7;
    for (int idxClm = 0; idxClm < iClmCount; ++idxClm) {
        m_pLytWdgtFillStyleSettings->setColumnMinimumWidth(idxClm, m_ariClmWidths[idxClm]);
    }
    m_pLytWdgtFillStyleSettings->setColumnStretch(iClmCount, 1);

    // <Row 0> Fill Style
    //-------------------

    int iRow = 0;

    m_pLblFillStyle = new QLabel("Brush:");
    m_pLytWdgtFillStyleSettings->addWidget(m_pLblFillStyle, iRow, 0);

    QLabel* pLblDummy = new QLabel();
    m_pLytWdgtFillStyleSettings->addWidget(pLblDummy, iRow, 2, Qt::AlignRight);

    m_pModelFillStyles = new QStandardItemModel();
    iconSize = fillFillStylesModel();
    m_pCmbFillStyle = new QComboBox();
    m_pCmbFillStyle->setModel(m_pModelFillStyles);
    m_pCmbFillStyle->setIconSize(iconSize);
    m_pLytWdgtFillStyleSettings->addWidget(m_pCmbFillStyle, iRow, 4);
    QObject::connect(
        m_pCmbFillStyle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjFillStyleProperties::onCmbFillStyleCurrentIndexChanged);

    m_pBtnFillColor = new QPushButton();
    m_pPxmBtnFillColor = new QPixmap(":/ZS/Draw/DrawSettingsBrushStyle16x16.png");
    m_pBtnFillColor->setIcon(*m_pPxmBtnFillColor);
    m_pLytWdgtFillStyleSettings->addWidget(m_pBtnFillColor, iRow, 6);
    QObject::connect(
        m_pBtnFillColor, &QPushButton::clicked,
        this, &CWdgtGraphObjFillStyleProperties::onBtnFillColorClicked);

    // Restore visibility
    //-------------------

    if (!s_bWdgtFillStyleVisible) {
        m_pWdgtFillStyleSettings->hide();
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjFillStyleProperties::~CWdgtGraphObjFillStyleProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try {
        delete m_pModelFillStyles;
    }
    catch(...) {
    }
    try {
        delete m_pPxmBtnFillColor;
    }
    catch(...) {
    }

    //m_fillColor;
    m_fillStyle = static_cast<EFillStyle>(0);
    m_pWdgtHeadline = nullptr;
    m_pLytWdgtHeadline = nullptr;
    //m_pxmBtnDown;
    //m_pxmBtnUp;
    m_pBtnCollapse = nullptr;
    m_pLblHeadlineIcon = nullptr;
    m_pLblHeadline = nullptr;
    m_pSepHeadline = nullptr;
    m_pWdgtFillStyleSettings = nullptr;
    m_pLytWdgtFillStyleSettings = nullptr;
    // Fill Style
    m_pLblFillStyle = nullptr;
    m_pModelFillStyles = nullptr;
    m_pCmbFillStyle = nullptr;
    m_pPxmBtnFillColor = nullptr;
    m_pBtnFillColor = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::expand(bool i_bExpand)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bExpand);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expand",
        /* strAddInfo   */ strMthInArgs );

    if (i_bExpand && m_pWdgtFillStyleSettings->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtFillStyleSettings->show();
        s_bWdgtFillStyleVisible = true;
    }
    else if (!i_bExpand && !m_pWdgtFillStyleSettings->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtFillStyleSettings->hide();
        s_bWdgtFillStyleVisible = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Expanded: " + bool2Str(s_bWdgtFillStyleVisible));
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjFillStyleProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;

    CDrawSettings drawSettings;
    if (m_pGraphObj == nullptr) {
        drawSettings = m_pDrawingScene->drawSettings();
    }
    else {
        drawSettings = m_pGraphObj->getDrawSettings();
    }

    if (m_fillColor != drawSettings.getFillColor()) {
        bHasChanges = true;
    }
    else if (m_fillStyle != drawSettings.getFillStyle()) {
        bHasChanges = true;
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
/*! @brief Applies the changed settings at either the graphics scene or the
           graphical object.

    The widget is used to modify only part of the draw settings and is very
    likely a child of a widget used to modify also other draw setting attributes.
    The parent widget is responsibly for updating all the draw settings at
    the drawing scene or the graphical object. For this the parent widget
    will set the flag i_bImmediatelyApplySettings to false and will update the
    changed settings after all child widgets have forwarded the changes to
    either the graphics scene or the graphical object.

    @param [in] i_bImmediatelyApplySetting (default: true)
        Set this flag to false if further set<DrawAttribute> will follow and
        all changes have to be updated at once later on.
*/
void CWdgtGraphObjFillStyleProperties::applySettings(bool i_bImmediatelyApplySettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ImmediatelyApply: " + bool2Str(i_bImmediatelyApplySettings);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applySettings",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    if (!hasErrors() && hasChanges()) {
        if (m_pGraphObj == nullptr) {
            m_pDrawingScene->setFillColor(m_fillColor);
            m_pDrawingScene->setFillStyle(m_fillStyle);
            if (i_bImmediatelyApplySettings) {
                m_pDrawingScene->updateDrawSettings();
            }
        }
        else {
            m_pGraphObj->setFillColor(m_fillColor);
            m_pGraphObj->setFillStyle(m_fillStyle);
            if (i_bImmediatelyApplySettings) {
                m_pGraphObj->updateDrawSettings();
            }
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::fillEditControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillEditControls",
        /* strAddInfo   */ "" );

    CDrawSettings drawSettings;
    if (m_pGraphObj == nullptr) {
        drawSettings = m_pDrawingScene->drawSettings();
    }
    else {
        drawSettings = m_pGraphObj->getDrawSettings();
    }

    m_fillColor = drawSettings.getFillColor();
    m_fillStyle = drawSettings.getFillStyle();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    updateCmbFillStyle(m_fillStyle);
    updateBtnFillColor(m_fillColor);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::onDrawingSceneDrawSettingsChanged(const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    if( m_iContentChangedSignalBlockedCounter > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_pGraphObj == nullptr) {
        // When applying the changes from the edit controls by invoking "acceptChanges"
        // the ContentChangedSignalBlockedCounter is incremented to avoid that
        // "onDrawingSceneDrawSettingsChanged" overwrites settings in edit controls which
        // haven't been applied yet.
        if (m_iContentChangedSignalBlockedCounter == 0)
        {
            {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

                // Here the derived class should apply the properties from the graphical
                // object to the edit controls.
                fillEditControls();
            }

            // If the "contentChanged" signal is no longer blocked and the content of
            // properties widget has been changed ...
            if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
                // .. emit the contentChanged signal and update the enabled state
                // of the Apply and Reset buttons.
                emit_contentChanged();
                m_bContentChanged = false;
            }
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::onBtnCollapseClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCollapseClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtFillStyleSettings->isHidden()) {
        expand(true);
    }
    else {
        expand(false);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::onCmbFillStyleCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbFillStyleCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        QVariant varData = m_pCmbFillStyle->itemData(i_idx, Qt::UserRole);
        m_fillStyle = static_cast<EFillStyle>(varData.toInt());
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::onBtnFillColorClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnFillColorClicked",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_fillColor,
        /* pWdgtParent */ m_pBtnFillColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if (clr.isValid()) {
        if (m_fillColor != clr) {
            updateBtnFillColor(clr);
            if (m_iContentChangedSignalBlockedCounter > 0) {
                m_bContentChanged = true;
                m_fillColor = clr;
            }
            else {
                m_fillColor = clr;
                emit_contentChanged();
            }
        }
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
private: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QSize CWdgtGraphObjFillStyleProperties::fillFillStylesModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillFillStylesModel",
        /* strAddInfo   */ "" );

    QSize iconSize(48, 16);
    QPixmap pxmFillStyle(iconSize);
    QPainter painter;
    QBrush brsh;

    for (CEnumFillStyle fillStyle = 0; fillStyle < CEnumFillStyle::count(); ++fillStyle) {
        if (!isFillStyleGradientPattern(fillStyle.enumerator())) {
            QStandardItem* pFillStyleItem = nullptr;
            if (fillStyle == EFillStyle::NoFill) {
                pFillStyleItem = new QStandardItem(" No Fill");
            }
            else {
                pxmFillStyle.fill(Qt::white);
                painter.begin(&pxmFillStyle);
                if (fillStyle == EFillStyle::TexturePattern) {
                    brsh.setTexture(pxmFillStyle);
                }
                brsh.setStyle(fillStyle2QtBrushStyle(fillStyle.enumerator()));
                painter.setBrush(brsh);
                painter.drawRect(2, 2, pxmFillStyle.width() - 4, pxmFillStyle.height() - 4);
                painter.end();
                pFillStyleItem = new QStandardItem();
                pFillStyleItem->setData(pxmFillStyle, Qt::DecorationRole);
            }
            pFillStyleItem->setData(fillStyle.enumeratorAsInt(), Qt::UserRole);
            m_pModelFillStyles->appendRow(pFillStyleItem);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qSize2Str(iconSize));
    }
    return iconSize;
}

/*==============================================================================
private: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::updateCmbFillStyle(const CEnumFillStyle& i_fillStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fillStyle.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateCmbFillStyle",
        /* strAddInfo   */ strMthInArgs );

    int idx = m_pCmbFillStyle->findData(i_fillStyle.enumeratorAsInt(), Qt::UserRole);
    if (idx >= 0) {
        m_pCmbFillStyle->setCurrentIndex(idx);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::updateBtnFillColor(const QColor& i_clr)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qColor2Str(i_clr);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateBtnFillColor",
        /* strAddInfo   */ strMthInArgs );

    QSize sizePxm = m_pPxmBtnFillColor->size();
    QPainter painter(m_pPxmBtnFillColor);
    painter.setPen(i_clr);
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(QPoint(sizePxm.height() - 3, 5), QPoint(sizePxm.height() - 3, sizePxm.width() - 5));
    painter.drawLine(QPoint(sizePxm.height() - 2, 6), QPoint(sizePxm.height() - 2, sizePxm.width() - 6));
    painter.drawLine(QPoint(sizePxm.height() - 1, 7), QPoint(sizePxm.height() - 1, sizePxm.width() - 7));
    painter.setPen(Qt::NoPen);
    painter.setBrush(i_clr);
    painter.drawRect(QRect(0, sizePxm.height() - 5, sizePxm.width(), 5));
    m_pBtnFillColor->setIcon(*m_pPxmBtnFillColor);
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjFillStyleProperties::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    QString strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "ContentChanged {" + bool2Str(m_bContentChanged)
            + ", SignalBlockCounter: " + QString::number(m_iContentChangedSignalBlockedCounter) + "}";
    mthTracer.trace(strMthLog);
    strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "DrawSettings {"
            "Fill {"
                + "Color: " + m_fillColor.name()
                + ", Style: " + m_fillStyle.toString()
            + "}"
        + "}";
    mthTracer.trace(strMthLog);
}
