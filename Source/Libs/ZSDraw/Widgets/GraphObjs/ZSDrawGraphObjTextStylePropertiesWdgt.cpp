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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjTextStylePropertiesWdgt.h"
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
#include <QtGui/qfontcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qfontcombobox.h>
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
class CWdgtGraphObjTextStyleProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjTextStyleProperties::CWdgtGraphObjTextStyleProperties(
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
    m_textColor(Qt::black),
    m_font(),
    m_textSize(ETextSize11),
    m_textStyle(ETextStyle::Normal),
    m_textEffect(ETextEffect::None),
    // Edit Controls
    m_pWdgtHeadline(nullptr),
    m_pLytWdgtHeadline(nullptr),
    m_pxmBtnDown(":/ZS/Button/ButtonArrowDown.png"),
    m_pxmBtnUp(":/ZS/Button/ButtonArrowUp.png"),
    m_pBtnCollapse(nullptr),
    m_pLblHeadlineIcon(nullptr),
    m_pLblHeadline(nullptr),
    m_pSepHeadline(nullptr),
    m_pWdgtTextStyleSettings(nullptr),
    m_pLytWdgtTextStyleSettings(nullptr),
    // Text Style
    m_pLblFont(nullptr),
    m_pCmbFont(nullptr),
    m_pCmbFontSize(nullptr),
    m_pPxmBtnTextColor(nullptr),
    m_pBtnTextColor(nullptr),
    m_pLblEffects(nullptr),
    m_pBtnFontStyleBold(nullptr),
    m_pBtnFontStyleItalic(nullptr),
    m_pBtnTextEffectUnderline(nullptr),
    m_pBtnTextEffectStrikeout(nullptr)
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
    if (s_bWdgtLineStyleVisible) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
    }
    else {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
    }
    m_pLytWdgtHeadline->addWidget(m_pBtnCollapse);

    QObject::connect(
        m_pBtnCollapse, &QPushButton::clicked,
        this, &CWdgtGraphObjTextStyleProperties::onBtnCollapseClicked);

    QPixmap pxmHeadline(":/ZS/Draw/DrawSettingsTextStyle16x16.png");
    m_pLblHeadlineIcon = new QLabel();
    m_pLblHeadlineIcon->setPixmap(pxmHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadlineIcon);

    m_pLblHeadline = new QLabel("Text Style");
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

    m_pWdgtTextStyleSettings = new QWidget();
    m_pLytWdgtTextStyleSettings = new QGridLayout();
    m_pLytWdgtTextStyleSettings->setContentsMargins(0, 0, 0, 0);
    m_pWdgtTextStyleSettings->setLayout(m_pLytWdgtTextStyleSettings);
    m_pLyt->addWidget(m_pWdgtTextStyleSettings);

    /* Grid Layout (alternative 2)
         |     0    |1| 2 |3|  4  |5| 6    |7| 8    |9|  10   | 
       --+----------+-+---+-+-----+-+------+-+------+-+-------+----
       0 |Font:     | |   | |Font          | |Size  | |Color  |<-->
       1 |Effects:  | |   | |Bold | |Kursiv| |UnderL| |StrikeT|<-->
    */
    const int cxSpacing = 5;
    const QVector<int> ariClmWidths = {
         /*  0 */ 60,
         /*  1 */ cxSpacing,
         /*  2 */ 30,
         /*  3 */ cxSpacing,
         /*  4 */ 60,
         /*  5 */ cxSpacing,
         /*  6 */ 60,
         /*  7 */ cxSpacing,
         /*  8 */ 60,
         /*  9 */ cxSpacing,
         /* 10 */ 60
    };

    for (int idxClm = 0; idxClm < ariClmWidths.size(); ++idxClm) {
        m_pLytWdgtTextStyleSettings->setColumnMinimumWidth(idxClm, ariClmWidths[idxClm]);
    }
    m_pLytWdgtTextStyleSettings->setColumnStretch(ariClmWidths.size(), 1);

    // <Row 0> Font
    //-------------

    int iRow = 0;

    m_pLblFont = new QLabel("Font:");
    m_pLytWdgtTextStyleSettings->addWidget(m_pLblFont, iRow, 0);

    QLabel* pLbDummy = new QLabel();
    m_pLytWdgtTextStyleSettings->addWidget(pLbDummy, iRow, 2, Qt::AlignRight);

    m_pCmbFont = new QFontComboBox();
    m_pLytWdgtTextStyleSettings->addWidget(m_pCmbFont, iRow, 4, 1, 3);
    QObject::connect(
        m_pCmbFont, &QFontComboBox::currentFontChanged,
        this, &CWdgtGraphObjTextStyleProperties::onCmbFontCurrentFontChanged);

    m_pCmbFontSize = new QComboBox();
    m_pLytWdgtTextStyleSettings->addWidget(m_pCmbFontSize, iRow, 8);
    for (int idx = 0; idx < ETextSizeCount; idx++) {
        m_pCmbFontSize->addItem(
            textSize2Str(static_cast<ETextSize>(idx)),
            textSize2SizeInPixels(static_cast<ETextSize>(idx)));
    }
    QObject::connect(
        m_pCmbFontSize, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjTextStyleProperties::onCmbFontSizeCurrentIndexChanged);

    m_pPxmBtnTextColor = new QPixmap(":/ZS/Draw/DrawSettingsTextStyle16x16.png");
    m_pBtnTextColor = new QPushButton();
    m_pLytWdgtTextStyleSettings->addWidget(m_pBtnTextColor, iRow, 10);
    QObject::connect(
        m_pBtnTextColor, &QPushButton::clicked,
        this, &CWdgtGraphObjTextStyleProperties::onBtnTextColorClicked);

    // <Row 1> Text Effects
    //---------------------

    // <Line> Grid Scale Labels Font Style and Font Effect
    //----------------------------------------------------

    QFont font;

    ++iRow;

    m_pLblEffects = new QLabel("Effects:");
    m_pLytWdgtTextStyleSettings->addWidget(m_pLblEffects, iRow, 0);

    pLbDummy = new QLabel();
    m_pLytWdgtTextStyleSettings->addWidget(pLbDummy, iRow, 2, Qt::AlignRight);

    m_pBtnFontStyleBold = new QPushButton("F");
    font = m_pBtnFontStyleBold->font();
    font.setBold(true);
    m_pBtnFontStyleBold->setFont(font);
    m_pBtnFontStyleBold->setCheckable(true);
    m_pLytWdgtTextStyleSettings->addWidget(m_pBtnFontStyleBold, iRow, 4);
    QObject::connect(
        m_pBtnFontStyleBold, &QPushButton::toggled,
        this, &CWdgtGraphObjTextStyleProperties::onBtnFontStyleBoldToggled);

    m_pBtnFontStyleItalic = new QPushButton("I");
    font = m_pBtnFontStyleItalic->font();
    font.setItalic(true);
    m_pBtnFontStyleItalic->setFont(font);
    m_pBtnFontStyleItalic->setCheckable(true);
    m_pLytWdgtTextStyleSettings->addWidget(m_pBtnFontStyleItalic, iRow, 6);
    QObject::connect(
        m_pBtnFontStyleItalic, &QPushButton::toggled,
        this, &CWdgtGraphObjTextStyleProperties::onBtnFontStyleItalicToggled);

    m_pBtnTextEffectUnderline = new QPushButton("U");
    font = m_pBtnTextEffectUnderline->font();
    font.setUnderline(true);
    m_pBtnTextEffectUnderline->setFont(font);
    m_pBtnTextEffectUnderline->setCheckable(true);
    m_pLytWdgtTextStyleSettings->addWidget(m_pBtnTextEffectUnderline, iRow, 8);
    QObject::connect(
        m_pBtnTextEffectUnderline, &QPushButton::toggled,
        this, &CWdgtGraphObjTextStyleProperties::onBtnTextEffectUnderlineToggled);

    m_pBtnTextEffectStrikeout = new QPushButton("ab");
    font = m_pBtnTextEffectStrikeout->font();
    font.setStrikeOut(true);
    m_pBtnTextEffectStrikeout->setFont(font);
    m_pBtnTextEffectStrikeout->setCheckable(true);
    m_pLytWdgtTextStyleSettings->addWidget(m_pBtnTextEffectStrikeout, iRow, 10);
    QObject::connect(
        m_pBtnTextEffectStrikeout, &QPushButton::toggled,
        this, &CWdgtGraphObjTextStyleProperties::onBtnTextEffectStrikeoutToggled);

    // Restore visibility
    //-------------------

    if (!s_bWdgtTextStyleVisible) {
        m_pWdgtTextStyleSettings->hide();
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjTextStyleProperties::~CWdgtGraphObjTextStyleProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try {
        delete m_pPxmBtnTextColor;
    }
    catch(...) {
    }

    //m_textColor;
    //m_font;
    m_textSize = static_cast<ETextSize>(0);
    m_textStyle = static_cast<ETextStyle>(0);
    m_textEffect = static_cast<ETextEffect>(0);
    m_pWdgtHeadline = nullptr;
    m_pLytWdgtHeadline = nullptr;
    //m_pxmBtnDown;
    //m_pxmBtnUp;
    m_pBtnCollapse = nullptr;
    m_pLblHeadlineIcon = nullptr;
    m_pLblHeadline = nullptr;
    m_pSepHeadline = nullptr;
    m_pWdgtTextStyleSettings = nullptr;
    m_pLytWdgtTextStyleSettings = nullptr;
    // Text Style
    m_pLblFont = nullptr;
    m_pCmbFont = nullptr;
    m_pCmbFontSize = nullptr;
    m_pPxmBtnTextColor = nullptr;
    m_pBtnTextColor = nullptr;
    m_pLblEffects = nullptr;
    m_pBtnFontStyleBold = nullptr;
    m_pBtnFontStyleItalic = nullptr;
    m_pBtnTextEffectUnderline = nullptr;
    m_pBtnTextEffectStrikeout = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::expand(bool i_bExpand)
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

    if (i_bExpand && m_pWdgtTextStyleSettings->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtTextStyleSettings->show();
        s_bWdgtLineStyleVisible = true;
    }
    else if (!i_bExpand && !m_pWdgtTextStyleSettings->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtTextStyleSettings->hide();
        s_bWdgtLineStyleVisible = false;
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn("Expanded: " + bool2Str(s_bWdgtLineStyleVisible));
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjTextStyleProperties::hasChanges() const
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

    if (m_textColor != drawSettings.getTextColor()) {
        bHasChanges = true;
    }
    else if (m_font != drawSettings.getTextFont()) {
        bHasChanges = true;
    }
    else if (m_textStyle != drawSettings.getTextStyle()) {
        bHasChanges = true;
    }
    else if (m_textSize != drawSettings.getTextSize()) {
        bHasChanges = true;
    }
    else if (m_textEffect != drawSettings.getTextEffect()) {
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
void CWdgtGraphObjTextStyleProperties::applySettings(bool i_bImmediatelyApplySettings)
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
            m_pDrawingScene->setTextColor(m_textColor, false);
            m_pDrawingScene->setTextFont(m_font, false);
            m_pDrawingScene->setTextSize(m_textSize, false);
            m_pDrawingScene->setTextStyle(m_textStyle, false);
            m_pDrawingScene->setTextEffect(m_textEffect, false);
            if (i_bImmediatelyApplySettings) {
                m_pDrawingScene->updateDrawSettings();
            }
        }
        else {
            m_pGraphObj->setTextColor(m_textColor, false);
            m_pGraphObj->setTextFont(m_font, false);
            m_pGraphObj->setTextSize(m_textSize, false);
            m_pGraphObj->setTextStyle(m_textStyle, false);
            m_pGraphObj->setTextEffect(m_textEffect, false);
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
void CWdgtGraphObjTextStyleProperties::fillEditControls()
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

    m_textColor = drawSettings.getTextColor();
    m_font = drawSettings.getTextFont();
    m_textSize = drawSettings.getTextSize();
    m_textStyle = drawSettings.getTextStyle();
    m_textEffect = drawSettings.getTextEffect();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    updateCmbFont(m_font);
    updateCmbTextSize(m_textSize);
    updateBtnTextColor(m_textColor);
    updateBtnFontStyleBold(m_textStyle);
    updateBtnFontStyleItalic(m_textStyle);
    updateBtnTextEffectUnderline(m_textEffect);
    updateBtnTextEffectStrikeout(m_textEffect);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::onDrawingSceneDrawSettingsChanged(const CDrawSettings& i_drawSettings)
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
void CWdgtGraphObjTextStyleProperties::onBtnCollapseClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCollapseClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtTextStyleSettings->isHidden()) {
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
void CWdgtGraphObjTextStyleProperties::onCmbFontCurrentFontChanged(const QFont& i_font)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_font.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbFontCurrentFontChanged",
        /* strAddInfo   */ strMthInArgs );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        m_font = i_font;
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::onCmbFontSizeCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbFontSizeCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        m_textSize = (static_cast<ETextSize>(i_idx));
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::onBtnTextColorClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnTextColorClicked",
        /* strAddInfo   */ "" );

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_textColor,
        /* pWdgtParent */ m_pBtnTextColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    if (clr.isValid()) {
        if (m_textColor != clr) {
            updateBtnTextColor(clr);
            if (m_iContentChangedSignalBlockedCounter > 0) {
                m_bContentChanged = true;
                m_textColor = clr;
            }
            else {
                m_textColor = clr;
                emit_contentChanged();
            }
        }
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::onBtnFontStyleBoldToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnFontStyleBoldToggled",
        /* strAddInfo   */ strMthInArgs );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CEnumTextStyle textStyle = ETextStyle::Normal;
    if (m_pBtnFontStyleItalic->isChecked() && m_pBtnFontStyleBold->isChecked()) {
        textStyle = ETextStyle::BoldItalic;
    }
    else if (m_pBtnFontStyleItalic->isChecked()) {
        textStyle = ETextStyle::Italic;
    }
    else if (m_pBtnFontStyleBold->isChecked()) {
        textStyle = ETextStyle::Bold;
    }
    if (m_textStyle != textStyle) {
        updateBtnFontStyleBold(textStyle);
        if (m_iContentChangedSignalBlockedCounter > 0) {
            m_bContentChanged = true;
            m_textStyle = textStyle;
        }
        else {
            m_textStyle = textStyle;
            emit_contentChanged();
        }
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::onBtnFontStyleItalicToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnFontStyleItalicToggled",
        /* strAddInfo   */ strMthInArgs );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CEnumTextStyle textStyle = ETextStyle::Normal;
    if (m_pBtnFontStyleItalic->isChecked() && m_pBtnFontStyleBold->isChecked()) {
        textStyle = ETextStyle::BoldItalic;
    }
    else if (m_pBtnFontStyleItalic->isChecked()) {
        textStyle = ETextStyle::Italic;
    }
    else if (m_pBtnFontStyleBold->isChecked()) {
        textStyle = ETextStyle::Bold;
    }
    if (m_textStyle != textStyle) {
        updateBtnFontStyleItalic(textStyle);
        if (m_iContentChangedSignalBlockedCounter > 0) {
            m_bContentChanged = true;
            m_textStyle = textStyle;
        }
        else {
            m_textStyle = textStyle;
            emit_contentChanged();
        }
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::onBtnTextEffectUnderlineToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnTextEffectUnderlineToggled",
        /* strAddInfo   */ strMthInArgs );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CEnumTextEffect textEffect = ETextEffect::None;
    if (m_pBtnTextEffectUnderline->isChecked() && m_pBtnTextEffectStrikeout->isChecked()) {
        textEffect = ETextEffect::StrikeoutUnderline;
    }
    else if (m_pBtnTextEffectUnderline->isChecked()) {
        textEffect = ETextEffect::Underline;
    }
    else if (m_pBtnTextEffectStrikeout->isChecked()) {
        textEffect = ETextEffect::Strikeout;
    }
    if (m_textEffect != textEffect) {
        updateBtnTextEffectUnderline(textEffect);
        if (m_iContentChangedSignalBlockedCounter > 0) {
            m_bContentChanged = true;
            m_textEffect = textEffect;
        }
        else {
            m_textEffect = textEffect;
            emit_contentChanged();
        }
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::onBtnTextEffectStrikeoutToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnTextEffectStrikeoutToggled",
        /* strAddInfo   */ strMthInArgs );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CEnumTextEffect textEffect = ETextEffect::None;
    if (m_pBtnTextEffectUnderline->isChecked() && m_pBtnTextEffectStrikeout->isChecked()) {
        textEffect = ETextEffect::StrikeoutUnderline;
    }
    else if (m_pBtnTextEffectUnderline->isChecked()) {
        textEffect = ETextEffect::Underline;
    }
    else if (m_pBtnTextEffectStrikeout->isChecked()) {
        textEffect = ETextEffect::Strikeout;
    }
    if (m_textEffect != textEffect) {
        updateBtnTextEffectStrikeout(textEffect);
        if (m_iContentChangedSignalBlockedCounter > 0) {
            m_bContentChanged = true;
            m_textEffect = textEffect;
        }
        else {
            m_textEffect = textEffect;
            emit_contentChanged();
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
void CWdgtGraphObjTextStyleProperties::updateCmbFont(const QFont& i_font)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_font.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateCmbFont",
        /* strAddInfo   */ strMthInArgs );

    m_pCmbFont->setCurrentFont(i_font);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::updateCmbTextSize(ETextSize i_textSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = textSize2Str(i_textSize);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateCmbTextSize",
        /* strAddInfo   */ strMthInArgs );

    m_pCmbFontSize->setCurrentIndex(i_textSize);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::updateBtnTextColor(const QColor& i_clr)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qColor2Str(i_clr);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateBtnTextColor",
        /* strAddInfo   */ strMthInArgs );

    QSize sizePxm = m_pPxmBtnTextColor->size();
    QPainter painter(m_pPxmBtnTextColor);
    painter.setPen(i_clr);
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(QPoint(sizePxm.height() - 3, 5), QPoint(sizePxm.height() - 3, sizePxm.width() - 5));
    painter.drawLine(QPoint(sizePxm.height() - 2, 6), QPoint(sizePxm.height() - 2, sizePxm.width() - 6));
    painter.drawLine(QPoint(sizePxm.height() - 1, 7), QPoint(sizePxm.height() - 1, sizePxm.width() - 7));
    painter.setPen(Qt::NoPen);
    painter.setBrush(i_clr);
    painter.drawRect(QRect(0, sizePxm.height() - 5, sizePxm.width(), 5));
    m_pBtnTextColor->setIcon(*m_pPxmBtnTextColor);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::updateBtnFontStyleBold(const CEnumTextStyle& i_textStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textStyle.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateBtnFontStyleBold",
        /* strAddInfo   */ strMthInArgs );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");
    if (isTextStyleBold(i_textStyle.enumerator())) {
        m_pBtnFontStyleBold->setChecked(true);
        m_pBtnFontStyleBold->setIcon(pxmCheckmark);
    } else {
        m_pBtnFontStyleBold->setChecked(false);
        m_pBtnFontStyleBold->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::updateBtnFontStyleItalic(const CEnumTextStyle& i_textStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textStyle.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateBtnFontStyleItalic",
        /* strAddInfo   */ strMthInArgs );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");
    if (isTextStyleItalic(i_textStyle.enumerator())) {
        m_pBtnFontStyleItalic->setChecked(true);
        m_pBtnFontStyleItalic->setIcon(pxmCheckmark);
    } else {
        m_pBtnFontStyleItalic->setChecked(false);
        m_pBtnFontStyleItalic->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::updateBtnTextEffectUnderline(const CEnumTextEffect& i_textEffect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textEffect.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateBtnTextEffectUnderline",
        /* strAddInfo   */ strMthInArgs );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");
    if (isTextEffectUnderline(i_textEffect.enumerator())) {
        m_pBtnTextEffectUnderline->setChecked(true);
        m_pBtnTextEffectUnderline->setIcon(pxmCheckmark);
    } else {
        m_pBtnTextEffectUnderline->setChecked(false);
        m_pBtnTextEffectUnderline->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::updateBtnTextEffectStrikeout(const CEnumTextEffect& i_textEffect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textEffect.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateBtnTextEffectStrikeout",
        /* strAddInfo   */ strMthInArgs );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");
    if (isTextEffectStrikeout(i_textEffect.enumerator())) {
        m_pBtnTextEffectStrikeout->setChecked(true);
        m_pBtnTextEffectStrikeout->setIcon(pxmCheckmark);
    } else {
        m_pBtnTextEffectStrikeout->setChecked(false);
        m_pBtnTextEffectStrikeout->setIcon(QIcon());
    }
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjTextStyleProperties::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    QString strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "ContentChanged {" + bool2Str(m_bContentChanged)
            + ", SignalBlockCounter: " + QString::number(m_iContentChangedSignalBlockedCounter) + "}";
    mthTracer.trace(strMthLog);
    strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "DrawSettings {"
            "Text {"
                + "Color: " + m_textColor.name()
                + ", Font: " + m_font.family()
                + ", Size: " + textSize2Str(m_textSize)
                + ", Style: " + m_textStyle.toString()
                + ", Effect: " + m_textEffect.toString()
            + "}"
        + "}";
    mthTracer.trace(strMthLog);
}
