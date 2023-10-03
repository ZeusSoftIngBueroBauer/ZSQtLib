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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qevent.h>
#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
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
    const QString& i_strParentClassName,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(
        i_pDrawingScene, i_strParentClassName + "::" + ClassName(), i_strObjName, i_pWdgtParent),
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
    m_pWdgtStyle(nullptr),
    m_pLytWdgtStyle(nullptr),
    m_pLytPenColor(nullptr),
    m_pLblPenColor(nullptr),
    m_pPxmBtnPenColor(nullptr),
    m_rctBtnPenColor(0, 12, 16, 4),
    m_pBtnPenColor(nullptr),
    m_pLytPenWidth(nullptr),
    m_pLblPenWidth(nullptr),
    m_pEdtPenWidth(nullptr),
    m_pWdgtLineStyle(nullptr),
    m_pLytWdgtLineStyle(nullptr),
    m_pLytLineStyle(nullptr),
    m_pLblLineStyle(nullptr),
    m_pBtnLineStyle(nullptr),
    m_pViewLineStyles(nullptr),
    m_pModelLineStyles(nullptr),
    m_pWdgtLineEndSettings(nullptr),
    m_pLytWdgtLineEndSettings(nullptr),
    m_pLytLineEndSettings(nullptr),
    m_pLblLineEndSettings(nullptr),
    m_pSepLineEndSettings(nullptr),
    m_pLytLineEndStyles(nullptr),
    m_pLblLineEndStylesLineStart(nullptr),
    m_pBtnLineEndStylesLineStart(nullptr),
    m_pViewLineEndStylesLineStart(nullptr),
    m_pModelLineEndStylesLineStart(nullptr),
    m_pLblLineEndStylesLineEnd(nullptr),
    m_pBtnLineEndStylesLineEnd(nullptr),
    m_pViewLineEndStylesLineEnd(nullptr),
    m_pModelLineEndStylesLineEnd(nullptr),
    m_pLytLineEndSizes(nullptr),
    m_pLblLineEndSizesLineStart(nullptr),
    m_pBtnLineEndSizesLineStart(nullptr),
    m_pViewLineEndSizesLineStart(nullptr),
    m_pModelLineEndSizesLineStart(nullptr),
    m_pLblLineEndSizesLineEnd(nullptr),
    m_pBtnLineEndSizesLineEnd(nullptr),
    m_pViewLineEndSizesLineEnd(nullptr),
    m_pModelLineEndSizesLineEnd(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // We need to fill the edit controls if the graphical object emits the geometryChanged signal:
    m_bContentUpdateOnDrawSettingsChanged = true;

    const QVector<int> ariClmWidths = {
         40,  // Color
         20,  // BtnColor
         120, // 
         30,  // 
         120  // 
    };
    int cxClmSpacing = 30;
    int idxClm = 0;

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

    m_pLblHeadline = new QLabel("Style");
    QFont fntHeadline = m_pLblHeadline->font();
    fntHeadline.setPointSize(fntHeadline.pointSize() + 2);
    m_pLblHeadline->setFont(fntHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadline);

    m_pSepHeadline = new CSepLine();
    m_pLytWdgtHeadline->addWidget(m_pSepHeadline, 1);

    m_pLytWdgtHeadline->addStretch();

    // <Widget> Style
    //===============

    m_pWdgtLineStyle = new QWidget();
    m_pLytWdgtLineStyle = new QVBoxLayout();
    m_pLytWdgtLineStyle->setContentsMargins(0, 0, 0, 0);
    m_pWdgtLineStyle->setLayout(m_pLytWdgtLineStyle);
    m_pLyt->addWidget(m_pWdgtLineStyle);

    // Restore visibility
    //===================

    if (!s_bWdgtLineStyleVisible) {
        m_pWdgtLineStyle->hide();
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
        delete m_pPxmBtnPenColor;
    }
    catch(...) {
    }
    try {
        delete m_pViewLineStyles;
    }
    catch(...) {
    }
    try {
        delete m_pModelLineStyles;
    }
    catch(...) {
    }
    try {
        delete m_pViewLineEndStylesLineStart;
    }
    catch(...) {
    }
    try {
        delete m_pModelLineEndStylesLineStart;
    }
    catch(...) {
    }
    try {
        delete m_pViewLineEndStylesLineEnd;
    }
    catch(...) {
    }
    try {
        delete m_pModelLineEndStylesLineEnd;
    }
    catch(...) {
    }
    try {
        delete m_pViewLineEndSizesLineStart;
    }
    catch(...) {
    }
    try {
        delete m_pModelLineEndSizesLineStart;
    }
    catch(...) {
    }
    try {
        delete m_pViewLineEndSizesLineEnd;
    }
    catch(...) {
    }
    try {
        delete m_pModelLineEndSizesLineEnd;
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
    m_pWdgtStyle = nullptr;
    m_pLytWdgtStyle = nullptr;
    m_pLytPenColor = nullptr;
    m_pLblPenColor = nullptr;
    m_pPxmBtnPenColor = nullptr;
    //m_rctBtnPenColor;
    m_pBtnPenColor = nullptr;
    m_pLytPenWidth = nullptr;
    m_pLblPenWidth = nullptr;
    m_pEdtPenWidth = nullptr;
    m_pWdgtLineStyle = nullptr;
    m_pLytWdgtLineStyle = nullptr;
    m_pLytLineStyle = nullptr;
    m_pLblLineStyle = nullptr;
    m_pBtnLineStyle = nullptr;
    m_pViewLineStyles = nullptr;
    m_pModelLineStyles = nullptr;
    m_pWdgtLineEndSettings = nullptr;
    m_pLytWdgtLineEndSettings = nullptr;
    m_pLytLineEndSettings = nullptr;
    m_pLblLineEndSettings = nullptr;
    m_pSepLineEndSettings = nullptr;
    m_pLytLineEndStyles = nullptr;
    m_pLblLineEndStylesLineStart = nullptr;
    m_pBtnLineEndStylesLineStart = nullptr;
    m_pViewLineEndStylesLineStart = nullptr;
    m_pModelLineEndStylesLineStart = nullptr;
    m_pLblLineEndStylesLineEnd = nullptr;
    m_pBtnLineEndStylesLineEnd = nullptr;
    m_pViewLineEndStylesLineEnd = nullptr;
    m_pModelLineEndStylesLineEnd = nullptr;
    m_pLytLineEndSizes = nullptr;
    m_pLblLineEndSizesLineStart = nullptr;
    m_pBtnLineEndSizesLineStart = nullptr;
    m_pViewLineEndSizesLineStart = nullptr;
    m_pModelLineEndSizesLineStart = nullptr;
    m_pLblLineEndSizesLineEnd = nullptr;
    m_pBtnLineEndSizesLineEnd = nullptr;
    m_pViewLineEndSizesLineEnd = nullptr;
    m_pModelLineEndSizesLineEnd = nullptr;
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

    CGraphObjLine* pGraphObjLine = nullptr;
    if (m_pGraphObj != nullptr) {
        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
    }

    if (pGraphObjLine != nullptr) {
        CDrawSettings drawSettings = pGraphObjLine->getDrawSettings();
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

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    CGraphObjLine* pGraphObjLine = nullptr;
    if (m_pGraphObj != nullptr) {
        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
    }

    if (pGraphObjLine == nullptr) {
        m_drawSettings = CDrawSettings();
    }
    else {
        m_drawSettings = pGraphObjLine->getDrawSettings();
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

    CGraphObjLine* pGraphObjLine = nullptr;
    if (m_pGraphObj != nullptr) {
        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
    }
    if (pGraphObjLine != nullptr && !hasErrors()) {
        pGraphObjLine->setDrawSettings(m_drawSettings);
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

    if (m_pWdgtLineStyle->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtLineStyle->show();
        s_bWdgtLineStyleVisible = true;
    }
    else {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtLineStyle->hide();
        s_bWdgtLineStyleVisible = false;
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

/*==============================================================================
private: // auxiliary instance methods
==============================================================================*/

