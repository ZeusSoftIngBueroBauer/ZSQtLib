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

#include "ZSDraw/Widgets/Drawing/ZSDrawingViewEditPropertyDlg.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingViewPropertiesWdgt.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qcoreapplication.h>

#if QT_VERSION < 0x050000
#include <QtGui/qspinbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CDlgDrawingViewEditProperty : public QDialog
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CTrcAdminObj* CDlgDrawingViewEditProperty::s_pTrcAdminObj = nullptr;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgDrawingViewEditProperty::CDlgDrawingViewEditProperty(
    CDrawingView* i_pDrawingView,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
    m_iContentChangedSignalBlockedCounter(0),
    m_strCoorPart1(),
    m_strCoorPart2(),
    m_pLyt(nullptr),
    m_pLytHeadLine(nullptr),
    m_pEdtPath(nullptr),
    m_pLytLineEdtVal(nullptr),
    m_pLblCoorPart1(nullptr),
    m_pLblCoorPart2(nullptr),
    m_pEdtCoor(nullptr),
    m_pLytLineBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnCancel(nullptr)
{
    setWindowTitle(QCoreApplication::applicationName() + ": Enter Value");

    s_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::GraphObjs", ClassName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "ctor",
        /* strInArgs    */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pLytHeadLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytHeadLine);

    m_pEdtPath = new QLineEdit();
    m_pEdtPath->setReadOnly(true);
    m_pLytHeadLine->addWidget(m_pEdtPath, 1);

    m_pLytLineEdtVal = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineEdtVal);

    m_pLblCoorPart1 = new QLabel();
    m_pLytLineEdtVal->addWidget(m_pLblCoorPart1);

    m_pLblCoorPart2 = new QLabel();
    m_pLytLineEdtVal->addWidget(m_pLblCoorPart2);

    m_pEdtCoor = new QDoubleSpinBox();
    m_pEdtCoor->setFixedWidth(120);
    m_pLytLineEdtVal->addWidget(m_pEdtCoor);
    QObject::connect(
        m_pEdtCoor, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
        this, &CDlgDrawingViewEditProperty::onEdtCoorValueChanged);

    m_pLytLineBtns = new QHBoxLayout();
    m_pLyt->addSpacing(10);
    m_pLyt->addLayout(m_pLytLineBtns);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytLineBtns->addWidget(m_pBtnOk);
    m_pBtnOk->setDefault(true);
    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgDrawingViewEditProperty::onBtnOkClicked);

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CDlgDrawingViewEditProperty::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytLineBtns->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CDlgDrawingViewEditProperty::onBtnResetClicked);

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytLineBtns->addWidget(m_pBtnCancel);
    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CDlgDrawingViewEditProperty::onBtnCancelClicked);

    m_pLytLineBtns->addStretch();

    QObject::connect(
        &Units.Length, &CUnitsLength::resolutionChanged,
        this, &CDlgDrawingViewEditProperty::onDrawUnitsLengthResolutionChanged );
    QObject::connect(
        m_pDrawingView, &CDrawingView::drawingSizeChanged,
        this, &CDlgDrawingViewEditProperty::onDrawingViewDrawingSizeChanged );

} // ctor

//------------------------------------------------------------------------------
CDlgDrawingViewEditProperty::~CDlgDrawingViewEditProperty()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "dtor",
        /* strInArgs    */ "" );

    CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObj);

    m_pDrawingView = nullptr;
    m_iContentChangedSignalBlockedCounter = 0;
    //m_strCoorPart1;
    //m_strCoorPart2;
    m_pLyt = nullptr;
    m_pLytHeadLine = nullptr;
    m_pEdtPath = nullptr;
    m_pLytLineEdtVal = nullptr;
    m_pLblCoorPart1 = nullptr;
    m_pLblCoorPart2 = nullptr;
    m_pEdtCoor = nullptr;
    m_pLytLineBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnCancel = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the graphical object and defines the coordinate which has to be edited.

    @param [in] i_pGraphObj
        Graphical object to be edited.
    @param [in] i_strCoorSpec
        The coordinate specifier has to be set as follows:
        <DimensionUnit>.<CoordinatePart1>[.<CoordinatePart2>]
        The sections of the object name will be used by the edit dialog
        to create the labels and to decide, which property has to be
        edited. E.g. for "Pixels.Point 1.X" the x coordinate of Point 1
        in dimension unit pixels will be edited.
*/
void CDlgDrawingViewEditProperty::setCoordinate(const QString& i_strCoorSpec)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(s_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strCoorSpec;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "setCoordinate",
        /* strInArgs    */ strMthInArgs );

    bool bIsValidCoordinate = false;

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    m_pEdtPath->setText(m_pDrawingView->objectName());

    QStringList strlstCoors = i_strCoorSpec.split(".");

    m_strCoorPart1 = "";
    m_strCoorPart2 = "";

    if (m_pDrawingView != nullptr && strlstCoors.size() > 0) {
        bIsValidCoordinate = true;
        m_strCoorPart1 = strlstCoors[0];
        if (strlstCoors.size() == 2) {
            m_strCoorPart2 = strlstCoors[1];
        }
        fillEditControls();
    }
    if (!bIsValidCoordinate) {
        m_pLblCoorPart1->setText("");
        m_pLblCoorPart2->setText("");
        m_pEdtCoor->setEnabled(false);
        m_pEdtCoor->setReadOnly(true);
        m_pEdtCoor->setValue(0.0);
    }
    updateButtonStates();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewEditProperty::onBtnOkClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onBtnOkClicked",
        /* strAddInfo   */ "" );

    acceptChanges();

    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgDrawingViewEditProperty::onBtnApplyClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onBtnApplyClicked",
        /* strAddInfo   */ "" );

    acceptChanges();
    updateButtonStates();
}

//------------------------------------------------------------------------------
void CDlgDrawingViewEditProperty::onBtnResetClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onBtnResetClicked",
        /* strAddInfo   */ "" );

    fillEditControls();
    updateButtonStates();
}

//------------------------------------------------------------------------------
void CDlgDrawingViewEditProperty::onBtnCancelClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onBtnCancelClicked",
        /* strAddInfo   */ "" );

    QDialog::reject();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewEditProperty::onDrawUnitsLengthResolutionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onDrawUnitsLengthResolutionChanged",
        /* strInArgs    */ "" );

    if (m_iContentChangedSignalBlockedCounter == 0) {
        fillEditControls();
    }
}

//------------------------------------------------------------------------------
void CDlgDrawingViewEditProperty::onDrawingViewDrawingSizeChanged(
    const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(s_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onDrawingViewDrawingSizeChanged",
        /* strInArgs    */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter == 0) {
        fillEditControls();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgDrawingViewEditProperty::onEdtCoorValueChanged(double i_fValue)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(s_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fValue);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onEdtCoorValueChanged",
        /* strInArgs    */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter == 0) {
    }
    updateButtonStates();
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief
*/
bool CDlgDrawingViewEditProperty::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;
    if (m_strCoorPart1 == CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter) {
        double fOrigValue = 1.0 / Units.Length.physValResolution(Units.Length.mm).getVal();
        bHasChanges = (fOrigValue != m_pEdtCoor->value());
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
/*! @brief
*/
void CDlgDrawingViewEditProperty::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "acceptChanges",
        /* strInArgs    */ "" );

    CDrawingSize drawingSize(objectName());
    drawingSize = m_pDrawingView->drawingSize();
    if (m_strCoorPart1 == CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter) {
        CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);
        double fRes_px_mm = m_pEdtCoor->value();
        Units.Length.setResolutionInPxPerMM(fRes_px_mm);
    }
}

//------------------------------------------------------------------------------
/*! @brief
*/
void CDlgDrawingViewEditProperty::updateButtonStates()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "updateButtonStates",
        /* strInArgs    */ "" );

    if (hasChanges()) {
        m_pBtnApply->setEnabled(true);
        m_pBtnReset->setEnabled(true);
    }
    else {
        m_pBtnApply->setEnabled(false);
        m_pBtnReset->setEnabled(false);
    }
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief
*/
void CDlgDrawingViewEditProperty::fillEditControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "fillEditControls",
        /* strInArgs    */ "" );

    bool bIsValidCoordinate = false;

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    m_pEdtPath->setText(m_pDrawingView->objectName());

    const CDrawingSize& drawingSize = m_pDrawingView->drawingSize();
    int cxLblWidthClm1 = 80;
    int cxLblWidthClm2 = 60;
    QString strLabelCoor1 = m_strCoorPart1;
    QString strLabelCoor2 = m_strCoorPart2;
    bool bEnabled = true;
    bool bReadOnly = false;
    int iDecimals = 2;
    double fSingleStep = 0.1;
    double fMinimum = 0.0;
    double fMaximum = 1000.0;
    double fValue = 0.0;

    if (m_strCoorPart1 == CDrawingScene::c_strXmlAttrScreenResolutionPxPerMilliMeter) {
        bIsValidCoordinate = true;
        cxLblWidthClm1 = 100;
        strLabelCoor1 = "Screen Resolution";
        strLabelCoor2 = "Pixels/mm";
        fMinimum = 0.1;
        fValue = 1.0 / Units.Length.physValResolution(Units.Length.mm).getVal();
    }

    if (bIsValidCoordinate) {
        m_pLblCoorPart1->setFixedWidth(cxLblWidthClm1);
        m_pLblCoorPart2->setFixedWidth(cxLblWidthClm2);
        if (strLabelCoor2.isEmpty()) {
            m_pLblCoorPart1->setText(strLabelCoor1 + ":");
            m_pLblCoorPart2->setText(strLabelCoor2);
        }
        else {
            m_pLblCoorPart1->setText(strLabelCoor1);
            m_pLblCoorPart2->setText(strLabelCoor2 + ":");
        }
        m_pEdtCoor->setEnabled(bEnabled);
        m_pEdtCoor->setReadOnly(bReadOnly);
        m_pEdtCoor->setDecimals(2);
        m_pEdtCoor->setSingleStep(0.1);
        m_pEdtCoor->setMinimum(fMinimum);
        m_pEdtCoor->setMaximum(fMaximum);
        m_pEdtCoor->setValue(fValue);
    }
    else {
        m_pLblCoorPart1->setText("");
        m_pLblCoorPart2->setText("");
        m_pEdtCoor->setEnabled(false);
        m_pEdtCoor->setReadOnly(true);
        m_pEdtCoor->setValue(0.0);
    }
}
