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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLineStylePropertiesWdgt.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
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
class CWdgtGraphObjLineStyleProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjLineStyleProperties::CWdgtGraphObjLineStyleProperties(
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
    m_drawSettings(),
    // Edit Controls
    m_pWdgtHeadline(nullptr),
    m_pLytWdgtHeadline(nullptr),
    m_pxmBtnDown(":/ZS/Button/ButtonArrowDown.png"),
    m_pxmBtnUp(":/ZS/Button/ButtonArrowUp.png"),
    m_pBtnCollapse(nullptr),
    m_pLblHeadlineIcon(nullptr),
    m_pLblHeadline(nullptr),
    m_pSepHeadline(nullptr),
    m_pWdgtLineStyleSettings(nullptr),
    m_pLytWdgtLineStyleSettings(nullptr),
    // Whole Line
    m_pLblLineStyle(nullptr),
    m_pModelLineStyles(nullptr),
    m_pCmbLineStyle(nullptr),
    m_pEdtLineWidth(nullptr),
    m_pPxmBtnLineColor(nullptr),
    m_pBtnLineColor(nullptr),
    // Arrow Heads
    m_pLblLineEndStyles(nullptr),
    // P1 (LineStart)
    m_pLblP1LineEndStyles(nullptr),
    m_pModelP1LineEndStyles(nullptr),
    m_pCmbP1LineEndStyle(nullptr),
    m_pModelP1ArrowHeadWidth(nullptr),
    m_pCmbP1ArrowHeadWidth(nullptr),
    m_pModelP1ArrowHeadLength(nullptr),
    m_pCmbP1ArrowHeadLength(nullptr),
    // P2 (LineEnd)
    m_pLblP2LineEndStyles(nullptr),
    m_pModelP2LineEndStyles(nullptr),
    m_pCmbP2LineEndStyle(nullptr),
    m_pModelP2ArrowHeadWidth(nullptr),
    m_pCmbP2ArrowHeadWidth(nullptr),
    m_pModelP2ArrowHeadLength(nullptr),
    m_pCmbP2ArrowHeadLength(nullptr)
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
        this, &CWdgtGraphObjLineStyleProperties::onBtnCollapseClicked);

    QPixmap pxmHeadline(":/ZS/Draw/DrawSettingsLineStyle16x16.png");
    m_pLblHeadlineIcon = new QLabel();
    m_pLblHeadlineIcon->setPixmap(pxmHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadlineIcon);

    m_pLblHeadline = new QLabel("Line Style");
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

    m_pWdgtLineStyleSettings = new QWidget();
    m_pLytWdgtLineStyleSettings = new QGridLayout();
    m_pLytWdgtLineStyleSettings->setContentsMargins(0, 0, 0, 0);
    m_pWdgtLineStyleSettings->setLayout(m_pLytWdgtLineStyleSettings);
    m_pLyt->addWidget(m_pWdgtLineStyleSettings);

    /* Grid Layout (alternative 2)
         |     0    |1| 2 |3|  4  |5| 6   |7| 8    | 
       --+----------+-+---+-+-----+-+-----+-+------+----
       0 |Line:     | |   | |Style| |Width| |Color |<-->
       1 |End Points| |P1:| |Style| |Width| |Length|<-->
       2 |          | |P2:| |Style| |Width| |Length|<-->
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
         /*  8 */ 60
    };

    for (int idxClm = 0; idxClm < ariClmWidths.size(); ++idxClm) {
        m_pLytWdgtLineStyleSettings->setColumnMinimumWidth(idxClm, ariClmWidths[idxClm]);
    }
    m_pLytWdgtLineStyleSettings->setColumnStretch(ariClmWidths.size(), 1);

    // <Row 0> Line Style
    //-------------------

    int iRow = 0;

    m_pLblLineStyle = new QLabel("Line:");
    m_pLytWdgtLineStyleSettings->addWidget(m_pLblLineStyle, iRow, 0);

    m_pModelLineStyles = new QStandardItemModel();
    iconSize = fillLineStylesModel();
    m_pCmbLineStyle = new QComboBox();
    m_pCmbLineStyle->setModel(m_pModelLineStyles);
    m_pCmbLineStyle->setIconSize(iconSize);
    m_pLytWdgtLineStyleSettings->addWidget(m_pCmbLineStyle, iRow, 4);
    QObject::connect(
        m_pCmbLineStyle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjLineStyleProperties::onCmbLineStyleCurrentIndexChanged);

    m_pEdtLineWidth = new QSpinBox();
    m_pEdtLineWidth->setMinimum(1);
    m_pEdtLineWidth->setMaximum(9999);
    m_pEdtLineWidth->setSuffix(" px");
    m_pLytWdgtLineStyleSettings->addWidget(m_pEdtLineWidth, iRow, 6);
    QObject::connect(
        m_pEdtLineWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
        this, &CWdgtGraphObjLineStyleProperties::onEdtLineWidthValueChanged);

    m_pBtnLineColor = new QPushButton();
    m_pPxmBtnLineColor = new QPixmap(":/ZS/Draw/GraphObjLine16x16.png");
    m_pBtnLineColor->setIcon(*m_pPxmBtnLineColor);
    m_pLytWdgtLineStyleSettings->addWidget(m_pBtnLineColor, iRow, 8);
    QObject::connect(
        m_pBtnLineColor, &QPushButton::clicked,
        this, &CWdgtGraphObjLineStyleProperties::onBtnPenColorClicked);

    // <Row 1> P1 End Styles
    //----------------------

    iRow = 1;

    m_pLblLineEndStyles = new QLabel("End Points");
    m_pLytWdgtLineStyleSettings->addWidget(m_pLblLineEndStyles, iRow, 0);

    m_pLblP1LineEndStyles = new QLabel("P1:");
    m_pLytWdgtLineStyleSettings->addWidget(m_pLblP1LineEndStyles, iRow, 2, Qt::AlignRight);

    m_pModelP1LineEndStyles = new QStandardItemModel();
    iconSize = fillLineEndStylesModel(ELinePoint::Start);
    m_pCmbP1LineEndStyle = new QComboBox();
    m_pCmbP1LineEndStyle->setModel(m_pModelP1LineEndStyles);
    m_pCmbP1LineEndStyle->setIconSize(iconSize);
    m_pLytWdgtLineStyleSettings->addWidget(m_pCmbP1LineEndStyle, iRow, 4);
    QObject::connect(
        m_pCmbP1LineEndStyle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjLineStyleProperties::onCmbP1LineEndStyleCurrentIndexChanged);

    m_pModelP1ArrowHeadWidth = new QStandardItemModel();
    fillArrowHeadWidthModel(ELinePoint::Start);
    m_pCmbP1ArrowHeadWidth = new QComboBox();
    m_pCmbP1ArrowHeadWidth->setModel(m_pModelP1ArrowHeadWidth);
    m_pLytWdgtLineStyleSettings->addWidget(m_pCmbP1ArrowHeadWidth, iRow, 6);
    QObject::connect(
        m_pCmbP1ArrowHeadWidth, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjLineStyleProperties::onCmbP1ArrowHeadWidthClicked);

    m_pModelP1ArrowHeadLength = new QStandardItemModel();
    fillArrowHeadLengthModel(ELinePoint::Start);
    m_pCmbP1ArrowHeadLength = new QComboBox();
    m_pCmbP1ArrowHeadLength->setModel(m_pModelP1ArrowHeadLength);
    m_pLytWdgtLineStyleSettings->addWidget(m_pCmbP1ArrowHeadLength, iRow, 8);
    QObject::connect(
        m_pCmbP1ArrowHeadLength, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjLineStyleProperties::onCmbP1ArrowHeadLengthClicked);

    // <Row 2> P2 End Styles
    //----------------------

    iRow = 2;

    m_pLblP2LineEndStyles = new QLabel("P2:");
    m_pLytWdgtLineStyleSettings->addWidget(m_pLblP2LineEndStyles, iRow, 2, Qt::AlignRight);

    m_pModelP2LineEndStyles = new QStandardItemModel();
    iconSize = fillLineEndStylesModel(ELinePoint::End);
    m_pCmbP2LineEndStyle = new QComboBox();
    m_pCmbP2LineEndStyle->setModel(m_pModelP2LineEndStyles);
    m_pCmbP2LineEndStyle->setIconSize(iconSize);
    m_pLytWdgtLineStyleSettings->addWidget(m_pCmbP2LineEndStyle, iRow, 4);
    QObject::connect(
        m_pCmbP2LineEndStyle, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjLineStyleProperties::onCmbP2LineEndStyleCurrentIndexChanged);

    m_pModelP2ArrowHeadWidth = new QStandardItemModel();
    fillArrowHeadWidthModel(ELinePoint::End);
    m_pCmbP2ArrowHeadWidth = new QComboBox();
    m_pCmbP2ArrowHeadWidth->setModel(m_pModelP2ArrowHeadWidth);
    m_pLytWdgtLineStyleSettings->addWidget(m_pCmbP2ArrowHeadWidth, iRow, 6);
    QObject::connect(
        m_pCmbP2ArrowHeadWidth, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjLineStyleProperties::onCmbP2ArrowHeadWidthClicked);

    m_pModelP2ArrowHeadLength = new QStandardItemModel();
    fillArrowHeadLengthModel(ELinePoint::End);
    m_pCmbP2ArrowHeadLength = new QComboBox();
    m_pCmbP2ArrowHeadLength->setModel(m_pModelP2ArrowHeadLength);
    m_pLytWdgtLineStyleSettings->addWidget(m_pCmbP2ArrowHeadLength, iRow, 8);
    QObject::connect(
        m_pCmbP2ArrowHeadLength, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtGraphObjLineStyleProperties::onCmbP2ArrowHeadLengthClicked);

    // Restore visibility
    //-------------------

    if (!s_bWdgtLineStyleVisible) {
        m_pWdgtLineStyleSettings->hide();
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjLineStyleProperties::~CWdgtGraphObjLineStyleProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try {
        delete m_pModelLineStyles;
    }
    catch(...) {
    }
    try {
        delete m_pPxmBtnLineColor;
    }
    catch(...) {
    }
    try {
        delete m_pModelP1LineEndStyles;
    }
    catch(...) {
    }
    try {
        delete m_pModelP1ArrowHeadWidth;
    }
    catch(...) {
    }
    try {
        delete m_pModelP1ArrowHeadLength;
    }
    catch(...) {
    }
    try {
        delete m_pModelP2LineEndStyles;
    }
    catch(...) {
    }
    try {
        delete m_pModelP2ArrowHeadWidth;
    }
    catch(...) {
    }
    try {
        delete m_pModelP2ArrowHeadLength;
    }
    catch(...) {
    }

    //m_drawSettings;
    m_pWdgtHeadline = nullptr;
    m_pLytWdgtHeadline = nullptr;
    //m_pxmBtnDown;
    //m_pxmBtnUp;
    m_pBtnCollapse = nullptr;
    m_pLblHeadlineIcon = nullptr;
    m_pLblHeadline = nullptr;
    m_pSepHeadline = nullptr;
    m_pWdgtLineStyleSettings = nullptr;
    m_pLytWdgtLineStyleSettings = nullptr;
    // Whole Line
    m_pLblLineStyle = nullptr;
    m_pModelLineStyles = nullptr;
    m_pCmbLineStyle = nullptr;
    m_pEdtLineWidth = nullptr;
    m_pPxmBtnLineColor = nullptr;
    m_pBtnLineColor = nullptr;
    // Arrow Heads
    m_pLblLineEndStyles = nullptr;
    // P1 (LineStart)
    m_pLblP1LineEndStyles = nullptr;
    m_pModelP1LineEndStyles = nullptr;
    m_pCmbP1LineEndStyle = nullptr;
    m_pModelP1ArrowHeadWidth = nullptr;
    m_pCmbP1ArrowHeadWidth = nullptr;
    m_pModelP1ArrowHeadLength = nullptr;
    m_pCmbP1ArrowHeadLength = nullptr;
    // P2 (LineEnd)
    m_pLblP2LineEndStyles = nullptr;
    m_pModelP2LineEndStyles = nullptr;
    m_pCmbP2LineEndStyle = nullptr;
    m_pModelP2ArrowHeadWidth = nullptr;
    m_pCmbP2ArrowHeadWidth = nullptr;
    m_pModelP2ArrowHeadLength = nullptr;
    m_pCmbP2ArrowHeadLength = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::expand(bool i_bExpand)
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

    if (i_bExpand && m_pWdgtLineStyleSettings->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtLineStyleSettings->show();
        s_bWdgtLineStyleVisible = true;
    }
    else if (!i_bExpand && !m_pWdgtLineStyleSettings->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtLineStyleSettings->hide();
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
bool CWdgtGraphObjLineStyleProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (m_pGraphObj == nullptr) {
        CDrawSettings drawSettings = m_pDrawingScene->drawSettings();
        bHasChanges = (m_drawSettings != drawSettings);
    }
    else {
        CDrawSettings drawSettings = m_pGraphObj->getDrawSettings();
        bHasChanges = (m_drawSettings != drawSettings);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

/*==============================================================================
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::fillEditControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillEditControls",
        /* strAddInfo   */ "" );

    if (m_pGraphObj == nullptr) {
        m_drawSettings = m_pDrawingScene->drawSettings();
    }
    else {
        m_drawSettings = m_pGraphObj->getDrawSettings();
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    updateCmbLineStyle(m_drawSettings.getLineStyle());
    m_pEdtLineWidth->setValue(m_drawSettings.getPenWidth());
    updateBtnPenColor(m_drawSettings.getPenColor());

    ELinePoint linePoint = ELinePoint::Start;
    updateCmbLineEndStyle(
        linePoint,
        m_drawSettings.getLineEndStyle(linePoint),
        m_drawSettings.getLineEndFillStyle(linePoint),
        m_drawSettings.getLineEndBaseLineType(linePoint));
    updateCmbArrowHeadWidth(
        linePoint,
        m_drawSettings.getLineEndWidth(linePoint));
    updateCmbArrowHeadLength(
        linePoint,
        m_drawSettings.getLineEndLength(linePoint));

    linePoint = ELinePoint::End;
    updateCmbLineEndStyle(
        linePoint,
        m_drawSettings.getLineEndStyle(linePoint),
        m_drawSettings.getLineEndFillStyle(linePoint),
        m_drawSettings.getLineEndBaseLineType(linePoint));
    updateCmbArrowHeadWidth(
        linePoint,
        m_drawSettings.getLineEndWidth(linePoint));
    updateCmbArrowHeadLength(
        linePoint,
        m_drawSettings.getLineEndLength(linePoint));

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::applySettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applySettings",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    #pragma message(__TODO__"Cache and set only text style settings")
    if (!hasErrors() && hasChanges()) {
        if (m_pGraphObj == nullptr) {
            m_pDrawingScene->setDrawSettings(m_drawSettings);
        }
        else {
            m_pGraphObj->setDrawSettings(m_drawSettings);
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onDrawingSceneDrawSettingsChanged(const CDrawSettings& i_drawSettings)
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
        if (m_drawSettings != i_drawSettings && m_iContentChangedSignalBlockedCounter == 0)
        {
            m_drawSettings = i_drawSettings;

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
void CWdgtGraphObjLineStyleProperties::onBtnCollapseClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCollapseClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtLineStyleSettings->isHidden()) {
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
void CWdgtGraphObjLineStyleProperties::onCmbLineStyleCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbLineStyleCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        QVariant varData = m_pCmbLineStyle->itemData(i_idx, Qt::UserRole);
        ELineStyle lineStyle = static_cast<ELineStyle>(varData.toInt());
        m_drawSettings.setLineStyle(lineStyle);
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onEdtLineWidthValueChanged(int i_iVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtLineWidthValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        m_drawSettings.setPenWidth(i_iVal);
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onBtnPenColorClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnPenColorClicked",
        /* strAddInfo   */ "" );

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_drawSettings.getPenColor(),
        /* pWdgtParent */ m_pBtnLineColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (clr.isValid()) {
        if (m_drawSettings.getPenColor() != clr) {
            updateBtnPenColor(clr);
            if (m_iContentChangedSignalBlockedCounter > 0) {
                m_bContentChanged = (m_drawSettings.getPenColor() != clr);
                m_drawSettings.setPenColor(clr);
            }
            else {
                m_drawSettings.setPenColor(clr);
                emit_contentChanged();
            }
        }
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onCmbP1LineEndStyleCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbP1LineEndStyleCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CEnumLineEndStyle lineEndStyle = m_pCmbP1LineEndStyle->itemData(
            i_idx, static_cast<int>(EItemDataRoleModelLineEndStyle::LineEndStyle)).toInt();
        CEnumArrowHeadFillStyle arrowHeadFillStyle = m_pCmbP1LineEndStyle->itemData(
            i_idx, static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadFillStyle)).toInt();
        CEnumArrowHeadBaseLineType arrowHeadBaseLineType = m_pCmbP1LineEndStyle->itemData(
            i_idx, static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadBaseLineType)).toInt();
        m_drawSettings.setLineEndStyle(ELinePoint::Start, lineEndStyle.enumerator());
        m_drawSettings.setLineEndFillStyle(ELinePoint::Start, arrowHeadFillStyle.enumerator());
        m_drawSettings.setLineEndBaseLineType(ELinePoint::Start, arrowHeadBaseLineType.enumerator());
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onCmbP1ArrowHeadWidthClicked(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbP1ArrowHeadWidthClicked",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CEnumArrowHeadWidth arrowHeadWidth =
            m_pCmbP1ArrowHeadWidth->itemData(i_idx, Qt::DisplayRole).toString();
        m_drawSettings.setLineEndWidth(ELinePoint::Start, arrowHeadWidth.enumerator());
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onCmbP1ArrowHeadLengthClicked(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbP1ArrowHeadLengthClicked",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CEnumArrowHeadLength arrowHeadLength =
            m_pCmbP1ArrowHeadLength->itemData(i_idx, Qt::DisplayRole).toString();
        m_drawSettings.setLineEndLength(ELinePoint::Start, arrowHeadLength.enumerator());
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onCmbP2LineEndStyleCurrentIndexChanged(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbP2LineEndStyleCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CEnumLineEndStyle lineEndStyle = m_pCmbP2LineEndStyle->itemData(
            i_idx, static_cast<int>(EItemDataRoleModelLineEndStyle::LineEndStyle)).toInt();
        CEnumArrowHeadFillStyle arrowHeadFillStyle = m_pCmbP2LineEndStyle->itemData(
            i_idx, static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadFillStyle)).toInt();
        CEnumArrowHeadBaseLineType arrowHeadBaseLineType = m_pCmbP2LineEndStyle->itemData(
            i_idx, static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadBaseLineType)).toInt();
        m_drawSettings.setLineEndStyle(ELinePoint::End, lineEndStyle.enumerator());
        m_drawSettings.setLineEndFillStyle(ELinePoint::End, arrowHeadFillStyle.enumerator());
        m_drawSettings.setLineEndBaseLineType(ELinePoint::End, arrowHeadBaseLineType.enumerator());
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onCmbP2ArrowHeadWidthClicked(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbP2ArrowHeadWidthClicked",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CEnumArrowHeadWidth arrowHeadWidth =
            m_pCmbP2ArrowHeadWidth->itemData(i_idx, Qt::DisplayRole).toString();
        m_drawSettings.setLineEndWidth(ELinePoint::End, arrowHeadWidth.enumerator());
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::onCmbP2ArrowHeadLengthClicked(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbP2ArrowHeadLengthClicked",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_iContentChangedSignalBlockedCounter > 0) {
        m_bContentChanged = true;
    }
    else {
        CEnumArrowHeadLength arrowHeadLength =
            m_pCmbP2ArrowHeadLength->itemData(i_idx, Qt::DisplayRole).toString();
        m_drawSettings.setLineEndLength(ELinePoint::End, arrowHeadLength.enumerator());
        emit_contentChanged();
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
}

/*==============================================================================
private: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QSize CWdgtGraphObjLineStyleProperties::fillLineStylesModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillLineStylesModel",
        /* strAddInfo   */ "" );

    QSize iconSize(48, 16);
    QPixmap pxmLineStyle(iconSize);
    QPainter painter;
    QPen pen;

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
            pLineStyleItem->setData(lineStyle.enumeratorAsInt(), Qt::UserRole);
            m_pModelLineStyles->appendRow(pLineStyleItem);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qSize2Str(iconSize));
    }
    return iconSize;
}

//------------------------------------------------------------------------------
QSize CWdgtGraphObjLineStyleProperties::fillLineEndStylesModel(const CEnumLinePoint& i_linePoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillLineEndStylesModel",
        /* strAddInfo   */ strMthInArgs );

    QStandardItemModel* pModel = nullptr;
    if (i_linePoint == ELinePoint::Start) {
        pModel = m_pModelP1LineEndStyles;
    }
    else {
        pModel = m_pModelP2LineEndStyles;
    }

    QPixmap pxmLineEnd(48, 16);
    QRect   rctPxm = pxmLineEnd.rect();
    QLine   line;
    QPoint  ptArrowHead1;
    QPoint  ptArrowHead2;
    QPoint  ptArrowHead3;
    QPoint  ptArrowHead4;

    if (i_linePoint == ELinePoint::Start) {
        line = QLine(5, rctPxm.center().y(), rctPxm.right() - 4, rctPxm.center().y());
        ptArrowHead1 = QPoint(line.p1().x() + 7, line.p1().y() + 6);
        ptArrowHead2 = QPoint(line.p1().x() - 2, line.p1().y());
        ptArrowHead3 = QPoint(line.p1().x() + 7, line.p1().y() - 6);
        ptArrowHead4 = QPoint(line.p1().x() + 3, line.p1().y());
    }
    else {
        line = QLine(4, rctPxm.center().y(), rctPxm.right() - 5, rctPxm.center().y());
        ptArrowHead1 = QPoint(line.p2().x() - 7, line.p2().y() + 6);
        ptArrowHead2 = QPoint(line.p2().x() + 2, line.p2().y());
        ptArrowHead3 = QPoint(line.p2().x() - 7, line.p2().y() - 6);
        ptArrowHead4 = QPoint(line.p2().x() - 3, line.p2().y());
    }

    QPoint arPts[4] = {ptArrowHead1, ptArrowHead2, ptArrowHead3, ptArrowHead4};

    QPainter painter;
    QPen pen;
    QBrush brsh;

    // ELineEndStyle::Normal
    //======================

    CEnumLineEndStyle lineEndStyle = ELineEndStyle::Normal;
    CEnumArrowHeadFillStyle arrowHeadFillStyle = EArrowHeadFillStyle::NoFill;
    CEnumArrowHeadBaseLineType arrowHeadBaseLineType = EArrowHeadBaseLineType::NoLine;
    QString strItemId = lineEndStyle.toString()
        + ", " + arrowHeadFillStyle.toString()
        + ", " + arrowHeadBaseLineType.toString();

    int idxRow = 0;

    pxmLineEnd.fill(Qt::white);
    painter.begin(&pxmLineEnd);
    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(line);
    painter.end();

    QStandardItem* pLineEndItem = new QStandardItem();
    pLineEndItem->setData(pxmLineEnd, Qt::DecorationRole);
    pLineEndItem->setData(strItemId, static_cast<int>(EItemDataRoleModelLineEndStyle::Id));
    pLineEndItem->setData(lineEndStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::LineEndStyle));
    pLineEndItem->setData(arrowHeadFillStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadFillStyle));
    pLineEndItem->setData(arrowHeadBaseLineType.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadBaseLineType));
    pModel->setItem(idxRow, pLineEndItem);
    pLineEndItem = nullptr;

    // ELineEndStyle::ArrowHead
    //=========================

    lineEndStyle = ELineEndStyle::ArrowHead;

    // EArrowHeadFillStyle::NoFill && EArrowHeadBaseLineType::NoLine
    //--------------------------------------------------------------

    arrowHeadFillStyle = EArrowHeadFillStyle::NoFill;
    arrowHeadBaseLineType = EArrowHeadBaseLineType::NoLine;
    strItemId = lineEndStyle.toString()
        + ", " + arrowHeadFillStyle.toString()
        + ", " + arrowHeadBaseLineType.toString();

    ++idxRow;

    pxmLineEnd.fill(Qt::white);
    painter.begin(&pxmLineEnd);
    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(line);
    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPolyline(arPts, 3);
    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData(pxmLineEnd, Qt::DecorationRole);
    pLineEndItem->setData(strItemId, static_cast<int>(EItemDataRoleModelLineEndStyle::Id));
    pLineEndItem->setData(lineEndStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::LineEndStyle));
    pLineEndItem->setData(arrowHeadFillStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadFillStyle));
    pLineEndItem->setData(arrowHeadBaseLineType.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadBaseLineType));
    pModel->setItem(idxRow, pLineEndItem);
    pLineEndItem = nullptr;

    // EArrowHeadFillStyle::NoFill && EArrowHeadBaseLineType::Normal
    //--------------------------------------------------------------

    if (i_linePoint == ELinePoint::Start) {
        line.setP1(QPoint(ptArrowHead1.x(), rctPxm.center().y()));
    }
    else {
        line.setP2(QPoint(ptArrowHead1.x(), rctPxm.center().y()));
    }

    arrowHeadFillStyle = EArrowHeadFillStyle::NoFill;
    arrowHeadBaseLineType = EArrowHeadBaseLineType::Normal;
    strItemId = lineEndStyle.toString()
        + ", " + arrowHeadFillStyle.toString()
        + ", " + arrowHeadBaseLineType.toString();

    ++idxRow;

    pxmLineEnd.fill(Qt::white);
    painter.begin(&pxmLineEnd);
    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(line);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPolygon(arPts, 3);
    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData(pxmLineEnd, Qt::DecorationRole);
    pLineEndItem->setData(strItemId, static_cast<int>(EItemDataRoleModelLineEndStyle::Id));
    pLineEndItem->setData(lineEndStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::LineEndStyle));
    pLineEndItem->setData(arrowHeadFillStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadFillStyle));
    pLineEndItem->setData(arrowHeadBaseLineType.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadBaseLineType));
    pModel->setItem(idxRow, pLineEndItem);
    pLineEndItem = nullptr;

    // EArrowHeadFillStyle::SolidPattern && EArrowHeadBaseLineType::Normal
    //--------------------------------------------------------------------

    arrowHeadFillStyle = EArrowHeadFillStyle::SolidPattern;
    arrowHeadBaseLineType = EArrowHeadBaseLineType::Normal;
    strItemId = lineEndStyle.toString()
        + ", " + arrowHeadFillStyle.toString()
        + ", " + arrowHeadBaseLineType.toString();

    ++idxRow;

    pxmLineEnd.fill(Qt::white);
    painter.begin(&pxmLineEnd);
    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(line);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    brsh.setColor(Qt::black);
    brsh.setStyle(Qt::SolidPattern);
    painter.setBrush(brsh);
    painter.drawPolygon(arPts, 3);
    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData(pxmLineEnd, Qt::DecorationRole);
    pLineEndItem->setData(strItemId, static_cast<int>(EItemDataRoleModelLineEndStyle::Id));
    pLineEndItem->setData(lineEndStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::LineEndStyle));
    pLineEndItem->setData(arrowHeadFillStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadFillStyle));
    pLineEndItem->setData(arrowHeadBaseLineType.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadBaseLineType));
    pModel->setItem(idxRow, pLineEndItem);
    pLineEndItem = nullptr;

    // EArrowHeadFillStyle::NoFill && EArrowHeadBaseLineType::Indented
    //----------------------------------------------------------------

    arrowHeadFillStyle = EArrowHeadFillStyle::NoFill;
    arrowHeadBaseLineType = EArrowHeadBaseLineType::Indented;
    strItemId = lineEndStyle.toString()
        + ", " + arrowHeadFillStyle.toString()
        + ", " + arrowHeadBaseLineType.toString();

    ++idxRow;

    pxmLineEnd.fill(Qt::white);
    painter.begin(&pxmLineEnd);
    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(line);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPolygon(arPts,4);
    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData(pxmLineEnd, Qt::DecorationRole);
    pLineEndItem->setData(strItemId, static_cast<int>(EItemDataRoleModelLineEndStyle::Id));
    pLineEndItem->setData(lineEndStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::LineEndStyle));
    pLineEndItem->setData(arrowHeadFillStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadFillStyle));
    pLineEndItem->setData(arrowHeadBaseLineType.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadBaseLineType));
    pModel->setItem(idxRow, pLineEndItem);
    pLineEndItem = nullptr;

    // EArrowHeadFillStyle::SolidPattern && EArrowHeadBaseLineType::Indented
    //----------------------------------------------------------------------

    arrowHeadFillStyle = EArrowHeadFillStyle::SolidPattern;
    arrowHeadBaseLineType = EArrowHeadBaseLineType::Indented;
    strItemId = lineEndStyle.toString()
        + ", " + arrowHeadFillStyle.toString()
        + ", " + arrowHeadBaseLineType.toString();

    ++idxRow;

    pxmLineEnd.fill(Qt::white);
    painter.begin(&pxmLineEnd);
    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(line);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    brsh.setColor(Qt::black);
    brsh.setStyle(Qt::SolidPattern);
    painter.setBrush(brsh);
    painter.drawPolygon(arPts,4);
    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData(pxmLineEnd, Qt::DecorationRole);
    pLineEndItem->setData(strItemId, static_cast<int>(EItemDataRoleModelLineEndStyle::Id));
    pLineEndItem->setData(lineEndStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::LineEndStyle));
    pLineEndItem->setData(arrowHeadFillStyle.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadFillStyle));
    pLineEndItem->setData(arrowHeadBaseLineType.enumeratorAsInt(), static_cast<int>(EItemDataRoleModelLineEndStyle::ArrowHeadBaseLineType));
    pModel->setItem(idxRow, pLineEndItem);
    pLineEndItem = nullptr;

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qSize2Str(pxmLineEnd.size()));
    }
    return pxmLineEnd.size();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::fillArrowHeadWidthModel(const CEnumLinePoint& i_linePoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillArrowHeadWidthModel",
        /* strAddInfo   */ strMthInArgs );

    QStandardItemModel* pModel = nullptr;
    if (i_linePoint == ELinePoint::Start) {
        pModel = m_pModelP1ArrowHeadWidth;
    }
    else {
        pModel = m_pModelP2ArrowHeadWidth;
    }

    for (CEnumArrowHeadWidth arrowHeadWidth = 0; arrowHeadWidth < CEnumArrowHeadWidth::count(); arrowHeadWidth++) {
        QStandardItem* pLineEndItem = new QStandardItem();
        pLineEndItem->setData(arrowHeadWidth.toString(), Qt::DisplayRole);
        pModel->setItem(arrowHeadWidth.enumeratorAsInt(), pLineEndItem);
        pLineEndItem = nullptr;
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::fillArrowHeadLengthModel(const CEnumLinePoint& i_linePoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillArrowHeadLengthModel",
        /* strAddInfo   */ strMthInArgs );

    QStandardItemModel* pModel = nullptr;
    if (i_linePoint == ELinePoint::Start) {
        pModel = m_pModelP1ArrowHeadLength;
    }
    else {
        pModel = m_pModelP2ArrowHeadLength;
    }

    for (CEnumArrowHeadLength arrowHeadLength = 0; arrowHeadLength < CEnumArrowHeadLength::count(); arrowHeadLength++) {
        QStandardItem* pLineEndItem = new QStandardItem();
        pLineEndItem->setData(arrowHeadLength.toString(), Qt::DisplayRole);
        pModel->setItem(arrowHeadLength.enumeratorAsInt(), pLineEndItem);
        pLineEndItem = nullptr;
    }
}

/*==============================================================================
private: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::updateCmbLineStyle(const CEnumLineStyle& i_lineStyle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_lineStyle.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateCmbLineStyle",
        /* strAddInfo   */ strMthInArgs );

    int idx = m_pCmbLineStyle->findData(i_lineStyle.enumeratorAsInt(), Qt::UserRole);
    if (idx >= 0) {
        m_pCmbLineStyle->setCurrentIndex(idx);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::updateBtnPenColor(const QColor& i_clr)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qColor2Str(i_clr);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateBtnPenColor",
        /* strAddInfo   */ strMthInArgs );

    QSize sizePxm = m_pPxmBtnLineColor->size();
    QPainter painter(m_pPxmBtnLineColor);
    QPen pen(i_clr, 2);
    painter.setPen(pen);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.drawLine(3, 3, sizePxm.width()-4, sizePxm.height()-4);
    painter.setPen(Qt::red);
    painter.drawRect(sizePxm.width()/2 + 1, 1, 2, 2);
    painter.setPen(Qt::green);
    painter.drawRect(sizePxm.width()/2 + 3, 1, 2, 2);
    painter.setPen(Qt::yellow);
    painter.drawRect(sizePxm.width()/2 + 1, 3, 2, 2);
    painter.setPen(Qt::blue);
    painter.drawRect(sizePxm.width()/2 + 3, 3, 2, 2);
    m_pBtnLineColor->setIcon(*m_pPxmBtnLineColor);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::updateCmbLineEndStyle(
    const CEnumLinePoint& i_linePoint,
    const CEnumLineEndStyle& i_lineEndStyle,
    const CEnumArrowHeadFillStyle& i_arrowHeadFillStyle,
    const CEnumArrowHeadBaseLineType& i_arrowHeadBaseLineType)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString()
            + ", " + i_lineEndStyle.toString()
            + ", " + i_arrowHeadFillStyle.toString()
            + ", " + i_arrowHeadBaseLineType.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateCmbLineEndStyle",
        /* strAddInfo   */ strMthInArgs );

    QComboBox* pCmb = nullptr;
    if (i_linePoint == ELinePoint::Start) {
        pCmb = m_pCmbP1LineEndStyle;
    }
    else {
        pCmb = m_pCmbP2LineEndStyle;
    }
    QString strItemId = i_lineEndStyle.toString()
        + ", " + i_arrowHeadFillStyle.toString()
        + ", " + i_arrowHeadBaseLineType.toString();
    int idx = pCmb->findData(strItemId, static_cast<int>(EItemDataRoleModelLineEndStyle::Id));
    if (idx >= 0) {
        pCmb->setCurrentIndex(idx);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::updateCmbArrowHeadWidth(
    const CEnumLinePoint& i_linePoint,
    const CEnumArrowHeadWidth& i_lineEndWidth)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_lineEndWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateCmbArrowHeadWidth",
        /* strAddInfo   */ strMthInArgs );

    QComboBox* pCmb = nullptr;
    if (i_linePoint == ELinePoint::Start) {
        pCmb = m_pCmbP1ArrowHeadWidth;
    }
    else {
        pCmb = m_pCmbP2ArrowHeadWidth;
    }
    int idx = pCmb->findData(i_lineEndWidth.toString(), Qt::DisplayRole);
    if (idx >= 0) {
        pCmb->setCurrentIndex(idx);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::updateCmbArrowHeadLength(
    const CEnumLinePoint& i_linePoint,
    const CEnumArrowHeadLength& i_lineEndLength)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_lineEndLength.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateCmbArrowHeadLength",
        /* strAddInfo   */ strMthInArgs );

    QComboBox* pCmb = nullptr;
    if (i_linePoint == ELinePoint::Start) {
        pCmb = m_pCmbP1ArrowHeadLength;
    }
    else {
        pCmb = m_pCmbP2ArrowHeadLength;
    }
    int idx = pCmb->findData(i_lineEndLength.toString(), Qt::DisplayRole);
    if (idx >= 0) {
        pCmb->setCurrentIndex(idx);
    }
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjLineStyleProperties::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    CEnumLineStyle lineStyle = m_pCmbLineStyle->itemData(m_pCmbLineStyle->currentIndex(), Qt::UserRole).toInt();
    QString strLineEndStyleP1 = m_pCmbP1LineEndStyle->itemData(m_pCmbP1LineEndStyle->currentIndex(), static_cast<int>(EItemDataRoleModelLineEndStyle::Id)).toString();
    QString strLineEndStyleP2 = m_pCmbP2LineEndStyle->itemData(m_pCmbP2LineEndStyle->currentIndex(), static_cast<int>(EItemDataRoleModelLineEndStyle::Id)).toString();
    QString strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "ContentChanged {" + bool2Str(m_bContentChanged)
            + ", SignalBlockCounter: " + QString::number(m_iContentChangedSignalBlockedCounter) + "}";
    mthTracer.trace(strMthLog);
    strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "DrawSettings {" + m_drawSettings.toString() + "}";
    mthTracer.trace(strMthLog);
    strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        + "EditControls {"
            + "LineStyle: " + lineStyle.toString()
            + ", LineWidth: " + m_pEdtLineWidth->text()
            + ", P1 {" +
                + "Style: " + strLineEndStyleP1
                + ", Width: " + m_pCmbP1ArrowHeadWidth->currentText()
                + ", Length: " + m_pCmbP1ArrowHeadLength->currentText() + "}"
            + ", P2 {"
                + "Style: " + strLineEndStyleP2
                + ", Width: " + m_pCmbP1ArrowHeadWidth->currentText()
                + ", Length: " + m_pCmbP1ArrowHeadLength->currentText() + "}}";
    mthTracer.trace(strMthLog);
}
