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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjGridStylePropertiesWdgt.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
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
#include <QtGui/qcheckbox.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qfontcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qcheckbox.h>
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
class CWdgtGraphObjGridStyleProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjGridStyleProperties::CWdgtGraphObjGridStyleProperties(
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
    m_gridSettings(i_strObjName),
    // Edit Controls
    m_pWdgtHeadline(nullptr),
    m_pLytWdgtHeadline(nullptr),
    m_pxmBtnDown(":/ZS/Button/ButtonArrowDown.png"),
    m_pxmBtnUp(":/ZS/Button/ButtonArrowUp.png"),
    m_pBtnCollapse(nullptr),
    m_pLblHeadlineIcon(nullptr),
    m_pLblHeadline(nullptr),
    m_pSepHeadline(nullptr),
    m_pWdgtGridStyleSettings(nullptr),
    m_pLytWdgtGridStyleSettings(nullptr),
    // Grid Lines
    m_pLblGridLines(nullptr),
    m_pChkGridLinesVisible(nullptr),
    m_pLblGridLinesDistMin(nullptr),
    m_pEdtGridLinesDistMin(nullptr),
    m_pLblGridLinesStyle(nullptr),
    m_pCmbGridLinesStyle(nullptr),
    m_pModelGridLinesStyles(nullptr),
    m_pEdtGridLinesWidth(nullptr),
    m_pPxmBtnGridLinesColor(nullptr),
    m_pBtnGridLinesColor(nullptr),
    // Grid Labels
    m_pLblGridScaleLabels(nullptr),
    m_pChkGridScaleLabelsVisible(nullptr),
    m_pCmbGridScaleLabelsFont(nullptr),
    m_pCmbGridScaleLabelsFontSize(nullptr),
    m_pPxmBtnGridScaleLabelsTextColor(nullptr),
    m_rctBtnGridScaleLabelsTextColor(0, 11, 16, 5),
    m_pBtnGridScaleLabelsTextColor(nullptr),
    m_pLblGridScaleLabelsTextStyle(nullptr),
    m_pBtnGridScaleLabelsFontStyleBold(nullptr),
    m_pBtnGridScaleLabelsFontStyleItalic(nullptr),
    m_pBtnGridScaleLabelsTextEffectUnderline(nullptr),
    m_pBtnGridScaleLabelsTextEffectStrikeout(nullptr)
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
        this, &CWdgtGraphObjGridStyleProperties::onBtnCollapseClicked);

    QPixmap pxmHeadline(":/ZS/Draw/DrawSettingsGridStyle16x16.png");
    m_pLblHeadlineIcon = new QLabel();
    m_pLblHeadlineIcon->setPixmap(pxmHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadlineIcon);

    m_pLblHeadline = new QLabel("Grid Style");
    QFont fntHeadline = m_pLblHeadline->font();
    fntHeadline.setPointSize(fntHeadline.pointSize() + 2);
    m_pLblHeadline->setFont(fntHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadline);

    m_pSepHeadline = new CSepLine();
    m_pLytWdgtHeadline->addWidget(m_pSepHeadline, 1);

    m_pLytWdgtHeadline->addStretch();

    // <Widget> Style
    //===============

    m_pWdgtGridStyleSettings = new QWidget();
    m_pLytWdgtGridStyleSettings = new QGridLayout();
    m_pLytWdgtGridStyleSettings->setContentsMargins(0, 0, 0, 0);
    m_pWdgtGridStyleSettings->setLayout(m_pLytWdgtGridStyleSettings);
    m_pLyt->addWidget(m_pWdgtGridStyleSettings);

    /* Grid Layout
         |       0     |1| 2 |3|      4       |5|      6       |7|      8       |9|      10         |
         |      50     |5| 20|5|     60       |5|     60       |5|     60       |5|      60         |
       --+-------------+-+---+-+--------------+-+--------------+-+--------------+-+-----------------+
       0 |Label Lines: | |Chk| |Label MinDist | |Edit MinDist  | |              | |                 |<-->
       2 |Label Style: | |   | |Edit LineStyle| |Edit LineWidth| |Btn LineColor | |                 |<-->
       3 |Label Labels:| |Chk| |Cmb Font                       | |Cmb FontSize  | |Btn TextColor    |<-->
       4 |Label Style: | |   | |Btn Bold      | |Btn Italic    | |Btn Underscore| |Btn StrikeThrough|<-->
    */
    int iClmCount = m_ariClmWidths.size();
    for (int idxClm = 0; idxClm < iClmCount; ++idxClm) {
        m_pLytWdgtGridStyleSettings->setColumnMinimumWidth(idxClm, m_ariClmWidths[idxClm]);
    }
    m_pLytWdgtGridStyleSettings->setColumnStretch(iClmCount, 1);

    // Row 0:
    const int iClmLblLines = 0;
    const int iClmChkVisible = 2;
    const int iClmLblLinesDistMin = 4;
    const int iClmEdtLinesDistMin = 6;
    // Row 1:
    const int iClmLblLineStyle = 0;
    const int iClmCmbLinesLineStyle = 4;
    const int iClmEdtLinesLineWidth = 6;
    const int iClmBtnLinesLineColor = 8;
    // Row 2:
    const int iClmLblLabels = 0;
    //const int iClmChkVisible = 2;
    const int iClmCmbLabelsFont = 4;
    const int iClmSpanCmbLabelsFont = 3;
    const int iClmCmbLabelsFontSize = 8;
    const int iClmBtnLabelsTextColor = 10;
    // Row 3:
    const int iClmLblTextStyle = 0;
    const int iClmBtnFontStyleBold = 4;
    const int iClmBtnFontStyleItalic = 6;
    const int iClmBtnFontEffectUnderline = 8;
    const int iClmBtnFontEffectStrikeout = 10;

    // <Row 0> Lines Min Distance
    //---------------------------

    int iRow = 0;

    int iClm = iClmLblLines;
    m_pLblGridLines = new QLabel("Lines:");
    m_pLytWdgtGridStyleSettings->addWidget(m_pLblGridLines, iRow, iClm);

    iClm = iClmChkVisible;
    m_pChkGridLinesVisible = new QCheckBox();
    m_pLytWdgtGridStyleSettings->addWidget(m_pChkGridLinesVisible, iRow, iClm);
    QObject::connect(
        m_pChkGridLinesVisible, &QCheckBox::stateChanged,
        this, &CWdgtGraphObjGridStyleProperties::onChkGridLinesVisibleStateChanged);

    iClm = iClmLblLinesDistMin;
    m_pLblGridLinesDistMin = new QLabel("Min Distance:");
    m_pLytWdgtGridStyleSettings->addWidget(m_pLblGridLinesDistMin, iRow, iClm);

    iClm = iClmEdtLinesDistMin;
    m_pEdtGridLinesDistMin = new QSpinBox();
    m_pEdtGridLinesDistMin->setMinimum(1);
    m_pEdtGridLinesDistMin->setMaximum(9999);
    m_pEdtGridLinesDistMin->setSuffix(" px");
    m_pLytWdgtGridStyleSettings->addWidget(m_pEdtGridLinesDistMin, iRow, iClm);
    QObject::connect(
        m_pEdtGridLinesDistMin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjGridStyleProperties::onEdtGridLinesDistMinValueChanged);

    // <Row 1> Lines Style, Width, Color
    //----------------------------------

    ++iRow;

    iClm = iClmLblLineStyle;
    m_pLblGridLinesStyle = new QLabel("Line Style:");
    m_pLytWdgtGridStyleSettings->addWidget(m_pLblGridLinesStyle, iRow, iClm);

    iClm = iClmCmbLinesLineStyle;
    m_pModelGridLinesStyles = new QStandardItemModel();
    QSize iconSize = fillGridLineStylesModel();
    m_pCmbGridLinesStyle = new QComboBox();
    m_pCmbGridLinesStyle->setModel(m_pModelGridLinesStyles);
    m_pCmbGridLinesStyle->setIconSize(iconSize);
    m_pLytWdgtGridStyleSettings->addWidget(m_pCmbGridLinesStyle, iRow, iClm);
    QObject::connect(
        m_pCmbGridLinesStyle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjGridStyleProperties::onCmbGridLinesStyleCurrentIndexChanged);

    iClm = iClmEdtLinesLineWidth;
    m_pEdtGridLinesWidth = new QSpinBox();
    m_pEdtGridLinesWidth->setMinimum(1);
    m_pEdtGridLinesWidth->setMaximum(9999);
    m_pEdtGridLinesWidth->setSuffix(" px");
    m_pLytWdgtGridStyleSettings->addWidget(m_pEdtGridLinesWidth, iRow, iClm);
    QObject::connect(
        m_pEdtGridLinesWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjGridStyleProperties::onEdtGridLinesWidthValueChanged);

    iClm = iClmBtnLinesLineColor;
    m_pPxmBtnGridLinesColor = new QPixmap(":/ZS/Draw/GraphObjLine16x16.png");
    m_pBtnGridLinesColor = new QPushButton();
    updateGridLinesColorButtonIcon();
    m_pLytWdgtGridStyleSettings->addWidget(m_pBtnGridLinesColor, iRow, iClm);
    QObject::connect(
        m_pBtnGridLinesColor, &QPushButton::clicked,
        this, &CWdgtGraphObjGridStyleProperties::onBtnGridLinesColorClicked);

    // <Row 2> Grid Scale Labels Visible and Font Size
    //------------------------------------------------

    ++iRow;

    iClm = iClmLblLabels;
    m_pLblGridScaleLabels = new QLabel("Labels:");
    m_pLytWdgtGridStyleSettings->addWidget(m_pLblGridScaleLabels, iRow, iClm);

    iClm = iClmChkVisible;
    m_pChkGridScaleLabelsVisible = new QCheckBox();
    m_pLytWdgtGridStyleSettings->addWidget(m_pChkGridScaleLabelsVisible, iRow, iClm);
    QObject::connect(
        m_pChkGridScaleLabelsVisible, &QCheckBox::stateChanged,
        this, &CWdgtGraphObjGridStyleProperties::onChkGridScaleLabelsVisibleStateChanged);

    iClm = iClmCmbLabelsFont;
    m_pCmbGridScaleLabelsFont = new QFontComboBox();
    m_pLytWdgtGridStyleSettings->addWidget(m_pCmbGridScaleLabelsFont, iRow, iClm, 1, iClmSpanCmbLabelsFont);
    QObject::connect(
        m_pCmbGridScaleLabelsFont, &QFontComboBox::currentFontChanged,
        this, &CWdgtGraphObjGridStyleProperties::onCmbGridScaleLabelsCurrentFontChanged);

    iClm = iClmCmbLabelsFontSize;
    m_pCmbGridScaleLabelsFontSize = new QComboBox();
    m_pLytWdgtGridStyleSettings->addWidget(m_pCmbGridScaleLabelsFontSize, iRow, iClm);
    for (int idx = 0; idx < ETextSizeCount; idx++) {
        m_pCmbGridScaleLabelsFontSize->addItem(
            textSize2Str(static_cast<ETextSize>(idx)),
            textSize2SizeInPixels(static_cast<ETextSize>(idx)));
    }
    QObject::connect(
        m_pCmbGridScaleLabelsFontSize, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjGridStyleProperties::onCmbGridScaleLabelsFontSizeCurrentIndexChanged);

    iClm = iClmBtnLabelsTextColor;
    m_pPxmBtnGridScaleLabelsTextColor = new QPixmap(":/ZS/Draw/DrawSettingsTextStyle16x16.png");
    m_pBtnGridScaleLabelsTextColor = new QPushButton();
    updateGridLabelsTextColorButtonIcon();
    m_pLytWdgtGridStyleSettings->addWidget(m_pBtnGridScaleLabelsTextColor, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsTextColor, &QPushButton::clicked,
        this, &CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsTextColorClicked);

    // <Line> Grid Scale Labels Font Style and Font Effect
    //----------------------------------------------------

    QFont font;

    ++iRow;

    iClm = iClmLblTextStyle;
    m_pLblGridScaleLabelsTextStyle = new QLabel("Text Style:");
    m_pLytWdgtGridStyleSettings->addWidget(m_pLblGridScaleLabelsTextStyle, iRow, iClm);

    iClm = iClmBtnFontStyleBold;
    m_pBtnGridScaleLabelsFontStyleBold = new QPushButton("F");
    font = m_pBtnGridScaleLabelsFontStyleBold->font();
    font.setBold(true);
    m_pBtnGridScaleLabelsFontStyleBold->setFont(font);
    m_pBtnGridScaleLabelsFontStyleBold->setCheckable(true);
    m_pLytWdgtGridStyleSettings->addWidget(m_pBtnGridScaleLabelsFontStyleBold, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsFontStyleBold, &QPushButton::toggled,
        this, &CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsFontStyleBoldToggled);

    iClm = iClmBtnFontStyleItalic;
    m_pBtnGridScaleLabelsFontStyleItalic = new QPushButton("I");
    font = m_pBtnGridScaleLabelsFontStyleItalic->font();
    font.setItalic(true);
    m_pBtnGridScaleLabelsFontStyleItalic->setFont(font);
    m_pBtnGridScaleLabelsFontStyleItalic->setCheckable(true);
    m_pLytWdgtGridStyleSettings->addWidget(m_pBtnGridScaleLabelsFontStyleItalic, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsFontStyleItalic, &QPushButton::toggled,
        this, &CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsFontStyleItalicToggled);

    iClm = iClmBtnFontEffectUnderline;
    m_pBtnGridScaleLabelsTextEffectUnderline = new QPushButton("U");
    font = m_pBtnGridScaleLabelsTextEffectUnderline->font();
    font.setUnderline(true);
    m_pBtnGridScaleLabelsTextEffectUnderline->setFont(font);
    m_pBtnGridScaleLabelsTextEffectUnderline->setCheckable(true);
    m_pLytWdgtGridStyleSettings->addWidget(m_pBtnGridScaleLabelsTextEffectUnderline, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsTextEffectUnderline, &QPushButton::toggled,
        this, &CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsTextEffectUnderlineToggled);

    iClm = iClmBtnFontEffectStrikeout;
    m_pBtnGridScaleLabelsTextEffectStrikeout = new QPushButton("ab");
    font = m_pBtnGridScaleLabelsTextEffectStrikeout->font();
    font.setStrikeOut(true);
    m_pBtnGridScaleLabelsTextEffectStrikeout->setFont(font);
    m_pBtnGridScaleLabelsTextEffectStrikeout->setCheckable(true);
    m_pLytWdgtGridStyleSettings->addWidget(m_pBtnGridScaleLabelsTextEffectStrikeout, iRow, iClm);
    QObject::connect(
        m_pBtnGridScaleLabelsTextEffectStrikeout, &QPushButton::toggled,
        this, &CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsTextEffectStrikeoutToggled);

    // Restore visibility
    //-------------------

    if (!s_bWdgtFillStyleVisible) {
        m_pWdgtGridStyleSettings->hide();
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjGridStyleProperties::~CWdgtGraphObjGridStyleProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try {
        delete m_pModelGridLinesStyles;
    }
    catch(...) {
    }
    try {
        delete m_pPxmBtnGridLinesColor;
    }
    catch(...) {
    }
    try {
        delete m_pPxmBtnGridScaleLabelsTextColor;
    }
    catch(...) {
    }

    //m_gridSettings;
    m_pWdgtHeadline = nullptr;
    m_pLytWdgtHeadline = nullptr;
    //m_pxmBtnDown;
    //m_pxmBtnUp;
    m_pBtnCollapse = nullptr;
    m_pLblHeadlineIcon = nullptr;
    m_pLblHeadline = nullptr;
    m_pSepHeadline = nullptr;
    m_pWdgtGridStyleSettings = nullptr;
    m_pLytWdgtGridStyleSettings = nullptr;
    // Grid Lines
    m_pLblGridLines = nullptr;
    m_pChkGridLinesVisible = nullptr;
    m_pLblGridLinesDistMin = nullptr;
    m_pEdtGridLinesDistMin = nullptr;
    m_pLblGridLinesStyle = nullptr;
    m_pCmbGridLinesStyle = nullptr;
    m_pModelGridLinesStyles = nullptr;
    m_pEdtGridLinesWidth = nullptr;
    m_pPxmBtnGridLinesColor = nullptr;
    m_pBtnGridLinesColor = nullptr;
    // Grid Scale Labels
    m_pLblGridScaleLabels = nullptr;
    m_pChkGridScaleLabelsVisible = nullptr;
    m_pCmbGridScaleLabelsFont = nullptr;
    m_pCmbGridScaleLabelsFontSize = nullptr;
    m_pPxmBtnGridScaleLabelsTextColor = nullptr;
    //m_rctBtnGridScaleLabelsTextColor;
    m_pBtnGridScaleLabelsTextColor = nullptr;
    m_pLblGridScaleLabelsTextStyle = nullptr;
    m_pBtnGridScaleLabelsFontStyleBold = nullptr;
    m_pBtnGridScaleLabelsFontStyleItalic = nullptr;
    m_pBtnGridScaleLabelsTextEffectUnderline = nullptr;
    m_pBtnGridScaleLabelsTextEffectStrikeout = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::expand(bool i_bExpand)
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

    if (i_bExpand && m_pWdgtGridStyleSettings->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtGridStyleSettings->show();
        s_bWdgtFillStyleVisible = true;
    }
    else if (!i_bExpand && !m_pWdgtGridStyleSettings->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtGridStyleSettings->hide();
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
bool CWdgtGraphObjGridStyleProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;

    if (m_pGraphObj == nullptr) {
        bHasChanges = (m_gridSettings != m_pDrawingScene->gridSettings());
    }
    else if (m_pGraphObj->type() == EGraphObjTypeGroup) {
        const CGraphObjGroup* pGraphObjGroup = dynamic_cast<const CGraphObjGroup*>(m_pGraphObj);
        bHasChanges = (m_gridSettings != pGraphObjGroup->gridSettings());
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
void CWdgtGraphObjGridStyleProperties::applySettings(bool i_bImmediatelyApplySettings)
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
            m_pDrawingScene->setGridSettings(m_gridSettings);
        }
        else if (m_pGraphObj->type() == EGraphObjTypeGroup) {
            CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pGraphObj);
            pGraphObjGroup->setGridSettings(m_gridSettings);
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
void CWdgtGraphObjGridStyleProperties::fillEditControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillEditControls",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CDrawGridSettings gridSettings(objectName());
    if (m_pGraphObj == nullptr) {
        gridSettings = m_pDrawingScene->gridSettings();
    }
    else if (m_pGraphObj->type() == EGraphObjTypeGroup) {
        CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pGraphObj);
        gridSettings = pGraphObjGroup->gridSettings();
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onBtnCollapseClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCollapseClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtGridStyleSettings->isHidden()) {
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
void CWdgtGraphObjGridStyleProperties::onChkGridLinesVisibleStateChanged(int i_iState)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkGridLinesVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLinesVisible(i_iState == Qt::Checked);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onEdtGridLinesDistMinValueChanged(int i_iVal)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtGridLinesDistMinValueChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLinesDistMin(i_iVal);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onCmbGridLinesStyleCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbGridLinesStyleCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if( i_idx >= 0 && i_idx < CEnumLineStyle::count() )
    {
        QVariant varData = m_pCmbGridLinesStyle->itemData(i_idx, Qt::UserRole);
        ELineStyle lineStyle = static_cast<ELineStyle>(varData.toInt());
        setGridLinesStyle(lineStyle);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onEdtGridLinesWidthValueChanged(int i_iVal)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridLinesColorClicked",
        /* strAddInfo   */ strMthInArgs );

    setGridLinesWidth(i_iVal);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onBtnGridLinesColorClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridLinesColorClicked",
        /* strAddInfo   */ strMthInArgs );

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_gridSettings.linesColor(),
        /* pWdgtParent */ m_pBtnGridLinesColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if (clr.isValid()) {
        setGridLinesColor(clr);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onChkGridScaleLabelsVisibleStateChanged(int i_iState)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCheckState2Str(i_iState);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkGridScaleLabelsVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLabelsVisible(i_iState == Qt::Checked);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onCmbGridScaleLabelsCurrentFontChanged(const QFont& i_fnt)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fnt.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbGridScaleLabelsCurrentFontChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLabelsFont(i_fnt);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onCmbGridScaleLabelsFontSizeCurrentIndexChanged(int i_iCurrentIndex)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iCurrentIndex);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbGridScaleLabelsFontSizeCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    setGridLabelsTextSize(static_cast<ETextSize>(i_iCurrentIndex));
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsTextColorClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsTextColorClicked",
        /* strAddInfo   */ strMthInArgs );

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_gridSettings.labelsTextColor(),
        /* pWdgtParent */ m_pBtnGridScaleLabelsTextColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if (clr.isValid()) {
        setGridLabelsTextColor(clr);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsFontStyleBoldToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsFontStyleBoldToggled",
        /* strAddInfo   */ strMthInArgs );

    CEnumTextStyle textStyle = ETextStyle::Normal;

    if (m_pBtnGridScaleLabelsFontStyleItalic->isChecked() && m_pBtnGridScaleLabelsFontStyleBold->isChecked()) {
        textStyle = ETextStyle::BoldItalic;
    }
    else if (m_pBtnGridScaleLabelsFontStyleItalic->isChecked()) {
        textStyle = ETextStyle::Italic;
    }
    else if (m_pBtnGridScaleLabelsFontStyleBold->isChecked()) {
        textStyle = ETextStyle::Bold;
    }
    setGridLabelsTextStyle(textStyle.enumerator());
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsFontStyleItalicToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsFontStyleItalicToggled",
        /* strAddInfo   */ strMthInArgs );

    CEnumTextStyle textStyle = ETextStyle::Normal;

    if (m_pBtnGridScaleLabelsFontStyleItalic->isChecked() && m_pBtnGridScaleLabelsFontStyleBold->isChecked()) {
        textStyle = ETextStyle::BoldItalic;
    }
    else if (m_pBtnGridScaleLabelsFontStyleItalic->isChecked()) {
        textStyle = ETextStyle::Italic;
    }
    else if (m_pBtnGridScaleLabelsFontStyleBold->isChecked()) {
        textStyle = ETextStyle::Bold;
    }
    setGridLabelsTextStyle(textStyle.enumerator());
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsTextEffectUnderlineToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsTextEffectUnderlineToggled",
        /* strAddInfo   */ strMthInArgs );

    CEnumTextEffect textEffect = ETextEffect::None;

    if (m_pBtnGridScaleLabelsTextEffectUnderline->isChecked() && m_pBtnGridScaleLabelsTextEffectStrikeout->isChecked()) {
        textEffect = ETextEffect::StrikeoutUnderline;
    }
    else if (m_pBtnGridScaleLabelsTextEffectUnderline->isChecked()) {
        textEffect = ETextEffect::Underline;
    }
    else if (m_pBtnGridScaleLabelsTextEffectStrikeout->isChecked()) {
        textEffect = ETextEffect::Strikeout;
    }
    setGridLabelsTextEffect(textEffect.enumerator());
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::onBtnGridScaleLabelsTextEffectStrikeoutToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    if (m_iContentChangedSignalBlockedCounter > 0) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnGridScaleLabelsTextEffectStrikeoutToggled",
        /* strAddInfo   */ strMthInArgs );

    CEnumTextEffect textEffect = ETextEffect::None;

    if (m_pBtnGridScaleLabelsTextEffectUnderline->isChecked() && m_pBtnGridScaleLabelsTextEffectStrikeout->isChecked()) {
        textEffect = ETextEffect::StrikeoutUnderline;
    }
    else if (m_pBtnGridScaleLabelsTextEffectUnderline->isChecked()) {
        textEffect = ETextEffect::Underline;
    }
    else if (m_pBtnGridScaleLabelsTextEffectStrikeout->isChecked()) {
        textEffect = ETextEffect::Strikeout;
    }
    setGridLabelsTextEffect(textEffect.enumerator());
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLinesVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bVisible);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesVisible",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.areLinesVisible() != i_bVisible) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesVisible(i_bVisible);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLinesDistMin(int i_iDistMin_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iDistMin_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesDistMin",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.linesDistMin() != i_iDistMin_px) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesDistMin(i_iDistMin_px);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLinesStyle(ELineStyle i_lineStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLineStyle(i_lineStyle).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesStyle",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.linesStyle() != i_lineStyle) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesStyle(i_lineStyle);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLinesColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_color.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesColor",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.linesColor() != i_color) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesColor(i_color);
        updateGridLinesColorButtonIcon();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLinesWidth(int i_iWidth_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iWidth_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLinesWidth",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.linesWidth() != i_iWidth_px) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLinesWidth(i_iWidth_px);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLabelsVisible(bool i_bVisible)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bVisible);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsVisible",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.areLabelsVisible() != i_bVisible) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsVisible(i_bVisible);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLabelsFont(const QFont& i_fnt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fnt.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsFont",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsFont() != i_fnt) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsFont(i_fnt);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLabelsTextSize(ETextSize i_textSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = textSize2Str(i_textSize);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextSize",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextSize() != i_textSize) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsTextSize(i_textSize);
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLabelsTextColor(const QColor& i_color)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_color.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextColor",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextColor() != i_color) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsTextColor(i_color);
        updateGridLabelsTextColorButtonIcon();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLabelsTextStyle(ETextStyle i_textStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumTextStyle(i_textStyle).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextStyle",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextStyle() != i_textStyle) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsTextStyle(i_textStyle);
        updateGridLabelsTextStyleBoldButton();
        updateGridLabelsTextStyleItalicButton();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::setGridLabelsTextEffect(const ETextEffect i_textEffect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumTextEffect(i_textEffect).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridLabelsTextEffect",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings.labelsTextEffect() != i_textEffect) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        m_gridSettings.setLabelsTextEffect(i_textEffect);
        updateGridLabelsTextEffectUnderlineButton();
        updateGridLabelsTextEffectStrikeoutButton();
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::updateGridSettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridSettings",
        /* strAddInfo   */ "" );

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    m_pChkGridLinesVisible->setChecked(m_gridSettings.areLinesVisible());
    m_pEdtGridLinesDistMin->setValue(m_gridSettings.linesDistMin());
    m_pEdtGridLinesWidth->setValue(m_gridSettings.linesWidth());
    m_pChkGridScaleLabelsVisible->setChecked(m_gridSettings.areLabelsVisible());
    int idx = m_pCmbGridLinesStyle->findData(m_gridSettings.linesStyle().enumeratorAsInt());
    if (idx >= 0) {
        m_pCmbGridLinesStyle->setCurrentIndex(idx);
    }
    m_pCmbGridScaleLabelsFont->setCurrentFont(m_gridSettings.labelsFont());
    m_pCmbGridScaleLabelsFontSize->setCurrentIndex(m_gridSettings.labelsTextSize());

    updateGridLinesColorButtonIcon();
    updateGridLabelsTextColorButtonIcon();
    updateGridLabelsTextStyleBoldButton();
    updateGridLabelsTextStyleItalicButton();
    updateGridLabelsTextEffectUnderlineButton();
    updateGridLabelsTextEffectStrikeoutButton();
}

//------------------------------------------------------------------------------
QSize CWdgtGraphObjGridStyleProperties::fillGridLineStylesModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillGridLineStylesModel",
        /* strAddInfo   */ "" );

    QSize iconSize(48, 16);
    QPixmap pxmLineStyle(iconSize);
    QPainter painter;
    QPen pen;

    pen.setColor(m_gridSettings.linesColor());
    pen.setWidth(m_gridSettings.linesWidth());

    for (CEnumLineStyle lineStyle = 0; lineStyle < CEnumLineStyle::count(); ++lineStyle) {
        if (lineStyle != ELineStyle::NoLine) {
            pxmLineStyle.fill(Qt::white);
            painter.begin(&pxmLineStyle);
            pen.setStyle(lineStyle2QtPenStyle(lineStyle.enumerator()));
            pen.setColor(Qt::black);
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawLine(4, pxmLineStyle.height()/2, pxmLineStyle.width()-4, pxmLineStyle.height()/2);
            painter.end();
            QStandardItem* pLineStyleItem = new QStandardItem();
            pLineStyleItem->setData(pxmLineStyle, Qt::DecorationRole);
            pLineStyleItem->setData(static_cast<int>(lineStyle.enumerator()), Qt::UserRole);
            m_pModelGridLinesStyles->appendRow(pLineStyleItem);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qSize2Str(iconSize));
    }
    return iconSize;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::updateGridLinesColorButtonIcon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLinesColorButtonIcon",
        /* strAddInfo   */ "" );

    QSize sizePxm = m_pPxmBtnGridLinesColor->size();
    QPainter painter(m_pPxmBtnGridLinesColor);
    QPen pen(m_gridSettings.linesColor(), 2);
    painter.setPen(pen);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.drawLine(3, 3, sizePxm.width()-4, sizePxm.height()-4);
    m_pBtnGridLinesColor->setIcon(*m_pPxmBtnGridLinesColor);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::updateGridLabelsTextColorButtonIcon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextColorButtonIcon",
        /* strAddInfo   */ "" );

    QPainter painter(m_pPxmBtnGridScaleLabelsTextColor);

    painter.setPen(m_gridSettings.labelsTextColor());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_gridSettings.labelsTextColor());
    painter.drawRect(m_rctBtnGridScaleLabelsTextColor);

    m_pBtnGridScaleLabelsTextColor->setIcon(*m_pPxmBtnGridScaleLabelsTextColor);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::updateGridLabelsTextStyleBoldButton()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextStyleBoldButton",
        /* strAddInfo   */ "" );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");

    if (isTextStyleBold(m_gridSettings.labelsTextStyle().enumerator())) {
        m_pBtnGridScaleLabelsFontStyleBold->setChecked(true);
        m_pBtnGridScaleLabelsFontStyleBold->setIcon(pxmCheckmark);
    } else {
        m_pBtnGridScaleLabelsFontStyleBold->setChecked(false);
        m_pBtnGridScaleLabelsFontStyleBold->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::updateGridLabelsTextStyleItalicButton()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextStyleItalicButton",
        /* strAddInfo   */ "" );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");

    if (isTextStyleItalic(m_gridSettings.labelsTextStyle().enumerator())) {
        m_pBtnGridScaleLabelsFontStyleItalic->setChecked(true);
        m_pBtnGridScaleLabelsFontStyleItalic->setIcon(pxmCheckmark);
    } else {
        m_pBtnGridScaleLabelsFontStyleItalic->setChecked(false);
        m_pBtnGridScaleLabelsFontStyleItalic->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::updateGridLabelsTextEffectUnderlineButton()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextEffectUnderlineButton",
        /* strAddInfo   */ "" );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");

    if (isTextEffectUnderline(m_gridSettings.labelsTextEffect().enumerator())) {
        m_pBtnGridScaleLabelsTextEffectUnderline->setChecked(true);
        m_pBtnGridScaleLabelsTextEffectUnderline->setIcon(pxmCheckmark);
    } else {
        m_pBtnGridScaleLabelsTextEffectUnderline->setChecked(false);
        m_pBtnGridScaleLabelsTextEffectUnderline->setIcon(QIcon());
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::updateGridLabelsTextEffectStrikeoutButton()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateGridLabelsTextEffectStrikeoutButton",
        /* strAddInfo   */ "" );

    QPixmap pxmCheckmark(":/ZS/App/Checkmark.png");

    if (isTextEffectStrikeout(m_gridSettings.labelsTextEffect().enumerator())) {
        m_pBtnGridScaleLabelsTextEffectStrikeout->setChecked(true);
        m_pBtnGridScaleLabelsTextEffectStrikeout->setIcon(pxmCheckmark);
    } else {
        m_pBtnGridScaleLabelsTextEffectStrikeout->setChecked(false);
        m_pBtnGridScaleLabelsTextEffectStrikeout->setIcon(QIcon());
    }
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjGridStyleProperties::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    QString strRuntimeInfo;
    if (i_methodDir == EMethodDir::Enter) {
        strRuntimeInfo = "-+ ";
    }
    else if (i_methodDir == EMethodDir::Leave) {
        strRuntimeInfo = "+- ";
    }
    else {
        strRuntimeInfo = "   ";
    }
    strRuntimeInfo +=
        "ContentChanged :" + bool2Str(m_bContentChanged) +
        ", SignalBlockCounter: " + QString::number(m_iContentChangedSignalBlockedCounter);
    mthTracer.trace(strRuntimeInfo);
    if (i_methodDir == EMethodDir::Enter) {
        strRuntimeInfo = "-+ ";
    }
    else if (i_methodDir == EMethodDir::Leave) {
        strRuntimeInfo = "+- ";
    }
    else {
        strRuntimeInfo = "   ";
    }
    strRuntimeInfo += "GridSettings {" + m_gridSettings.toString() + "}";
    mthTracer.trace(strRuntimeInfo);
}
