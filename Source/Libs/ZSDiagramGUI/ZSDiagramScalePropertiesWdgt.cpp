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

#include "ZSDiagramGUI/ZSDiagramScalePropertiesWdgt.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtGui/qcombobox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Diagram;
using namespace ZS::Diagram::GUI;


/*******************************************************************************
class CWdgtDiagramItemPropertiesBase : public CWdgtDiagramItemPropertiesBase
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDiagramScaleProperties::CWdgtDiagramScaleProperties(
    CWdgtDiagram* i_pDiagram,
    EMode i_mode,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtDiagramItemPropertiesBase(ClassName(), i_pDiagram, "CDiagScale", i_mode, i_pWdgtParent),
    m_pDiagScale(nullptr),
    // Caching values
    m_scaleDir(),
    m_spacing(),
    m_iMinVal_px(0),
    m_iMaxVal_px(0),
    m_ariDivLineDistMin_px(CEnumDivLineLayer::count(), 0),
    m_scale(),
    // Geometry
    m_pGrpGeometry(nullptr),
    m_pLytGrpGeometry(nullptr),
    m_pLytLineScaleDir(nullptr),
    m_pLblScaleDir(nullptr),
    m_pCmbScaleDir(nullptr),
    m_pLblSpacing(nullptr),
    m_pCmbSpacing(nullptr),
    m_pLblRange_px(nullptr),
    m_pEdtRange_px(nullptr),
    m_pLytSepLineDivLineDistMin(nullptr),
    m_pLblSepLineDivLineDistMin(nullptr),
    m_pSepLineDivLineDistMin(nullptr),
    m_pLytLineDivLineDistMin(nullptr),
    m_pLblDivLineDistMinLayerMain_px(nullptr),
    m_pEdtDivLineDistMinLayerMain_px(nullptr),
    m_pLblDivLineDistMinLayerSub_px(nullptr),
    m_pEdtDivLineDistMinLayerSub_px(nullptr),
    // Scale Values
    m_pGrpScaleValues(nullptr),
    m_pLytGrpScaleValues(nullptr),
    m_pLblScaleUnit(nullptr),
    m_pCmbScaleUnit(nullptr),
    m_pLblScaleRes(nullptr),
    m_pEdtScaleRes(nullptr),
    m_pLytLineScaleRange(nullptr),
    m_pLblScaleMinVal(nullptr),
    m_pEdtScaleMinVal(nullptr),
    m_pLblScaleMaxVal(nullptr),
    m_pEdtScaleMaxVal(nullptr),
    m_pLblScaleRangeVal(nullptr),
    m_pEdtScaleRangeVal(nullptr)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pDiagram->objectName() + ", " + CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    // <Group> Geometry
    //=========================

    m_pGrpGeometry = new QGroupBox("Geometry");
    m_pLyt->addWidget(m_pGrpGeometry);
    m_pLytGrpGeometry = new QVBoxLayout();
    m_pGrpGeometry->setLayout(m_pLytGrpGeometry);

    // <Line> ScaleDir and Spacing
    //----------------------------

    m_pLytLineScaleDir = new QHBoxLayout();
    m_pLytGrpGeometry->addLayout(m_pLytLineScaleDir);

    m_pLblScaleDir = new QLabel("Direction:");
    m_pLblScaleDir->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineScaleDir->addWidget(m_pLblScaleDir);
    m_pCmbScaleDir = new QComboBox();
    m_pCmbScaleDir->setFixedWidth(m_cxEdtWidthClm1);
    m_pCmbScaleDir->setEnabled(false);
    m_pLytLineScaleDir->addWidget(m_pCmbScaleDir);
    m_pLytLineScaleDir->addSpacing(m_cxClmSpacing);
    for( CEnumScaleDir scaleDir = 0; scaleDir < CEnumScaleDir::count(); ++scaleDir ) {
        m_pCmbScaleDir->addItem(scaleDir.toString());
    }
    /* The scale direction cannot be changed during runtime
    QObject::connect(
        m_pCmbScaleDir, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDiagramScaleProperties::onCmbScaleDirCurrentIndexChanged ); */

    m_pLblSpacing = new QLabel("Spacing:");
    m_pLblSpacing->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineScaleDir->addWidget(m_pLblSpacing);
    m_pCmbSpacing = new QComboBox();
    m_pCmbSpacing->setFixedWidth(m_cxEdtWidthClm2);
    m_pCmbSpacing->setEnabled(false);
    m_pLytLineScaleDir->addWidget(m_pCmbSpacing);
    m_pLytLineScaleDir->addSpacing(m_cxClmSpacing);
    for( CEnumSpacing spacing = 0; spacing < CEnumSpacing::count(); ++spacing ) {
        m_pCmbSpacing->addItem(spacing.toString());
    }
    QObject::connect(
        m_pCmbSpacing, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &CWdgtDiagramScaleProperties::onCmbSpacingCurrentIndexChanged );

    m_pLblRange_px = new QLabel("Range:");
    m_pLblRange_px->setFixedWidth(m_cxLblWidthClm3);
    m_pLytLineScaleDir->addWidget(m_pLblRange_px);
    m_pEdtRange_px = new QLineEdit();
    m_pEdtRange_px->setFixedWidth(m_cxEdtWidthClm3);
    m_pEdtRange_px->setEnabled(false);
    m_pLytLineScaleDir->addWidget(m_pEdtRange_px);
    m_pLytLineScaleDir->addStretch();

    // <Line> Division Lines
    //----------------------

    m_pLytSepLineDivLineDistMin = new QHBoxLayout();
    m_pLytGrpGeometry->addLayout(m_pLytSepLineDivLineDistMin);
    m_pLblSepLineDivLineDistMin = new QLabel("Division Line Minimum Distances");
    m_pLytSepLineDivLineDistMin->addWidget(m_pLblSepLineDivLineDistMin);
    m_pSepLineDivLineDistMin = new CSepLine(10);
    m_pLytSepLineDivLineDistMin->addWidget(m_pSepLineDivLineDistMin, 1);

    m_pLytLineDivLineDistMin = new QHBoxLayout();
    m_pLytGrpGeometry->addLayout(m_pLytLineDivLineDistMin);

    m_pLblDivLineDistMinLayerMain_px = new QLabel("Main Layer:");
    m_pLblDivLineDistMinLayerMain_px->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineDivLineDistMin->addWidget(m_pLblDivLineDistMinLayerMain_px);
    m_pEdtDivLineDistMinLayerMain_px = new QSpinBox();
    m_pEdtDivLineDistMinLayerMain_px->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtDivLineDistMinLayerMain_px->setEnabled(false);
    m_pEdtDivLineDistMinLayerMain_px->setReadOnly(true);
    m_pEdtDivLineDistMinLayerMain_px->setMinimum(0);
    m_pEdtDivLineDistMinLayerMain_px->setMaximum(9999);
    m_pEdtDivLineDistMinLayerMain_px->setSuffix(" px");
    m_pLytLineDivLineDistMin->addWidget(m_pEdtDivLineDistMinLayerMain_px);
    m_pLytLineDivLineDistMin->addSpacing(m_cxClmSpacing);
    QObject::connect(
        m_pEdtDivLineDistMinLayerMain_px, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtDiagramScaleProperties::onEdtDivLineDistMinMainLayerPixValueChanged);

    m_pLblDivLineDistMinLayerSub_px = new QLabel("Sub Layer:");
    m_pLblDivLineDistMinLayerSub_px->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLineDivLineDistMin->addWidget(m_pLblDivLineDistMinLayerSub_px);
    m_pEdtDivLineDistMinLayerSub_px = new QSpinBox();
    m_pEdtDivLineDistMinLayerSub_px->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtDivLineDistMinLayerSub_px->setEnabled(false);
    m_pEdtDivLineDistMinLayerSub_px->setReadOnly(true);
    m_pEdtDivLineDistMinLayerSub_px->setMinimum(0);
    m_pEdtDivLineDistMinLayerSub_px->setMaximum(9999);
    m_pEdtDivLineDistMinLayerSub_px->setSuffix(" px");
    m_pLytLineDivLineDistMin->addWidget(m_pEdtDivLineDistMinLayerSub_px);
    m_pLytLineDivLineDistMin->addStretch();
    QObject::connect(
        m_pEdtDivLineDistMinLayerSub_px, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtDiagramScaleProperties::onEdtDivLineDistMinSubLayerPixValueChanged);

    // <Group> Scale Values
    //=========================

    m_pGrpScaleValues = new QGroupBox("Scale Values");
    m_pLyt->addWidget(m_pGrpScaleValues);
    m_pLytGrpScaleValues = new QVBoxLayout();
    m_pGrpScaleValues->setLayout(m_pLytGrpScaleValues);

    // <Line> Unit and Scale Resolution
    //---------------------------------

    m_pLytLineUnitAndRes = new QHBoxLayout();
    m_pLytGrpScaleValues->addLayout(m_pLytLineUnitAndRes);

    m_pLblScaleUnit = new QLabel("Unit:");
    m_pLblScaleUnit->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineUnitAndRes->addWidget(m_pLblScaleUnit);
    m_pCmbScaleUnit = new QComboBox();
    m_pCmbScaleUnit->setFixedWidth(m_cxEdtWidthClm1);
    m_pCmbScaleUnit->setEnabled(false);
    m_pLytLineUnitAndRes->addWidget(m_pCmbScaleUnit);
    m_pLytLineUnitAndRes->addSpacing(m_cxClmSpacing);
    QObject::connect(
        m_pCmbScaleUnit, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &CWdgtDiagramScaleProperties::onCmbScaleUnitCurrentIndexChanged);

    m_pLblScaleRes = new QLabel("Res:");
    m_pLblScaleRes->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineUnitAndRes->addWidget(m_pLblScaleRes);
    m_pEdtScaleRes = new QLineEdit();
    m_pEdtScaleRes->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtScaleRes->setEnabled(false);
    m_pEdtScaleRes->setReadOnly(true);
    m_pLytLineUnitAndRes->addWidget(m_pEdtScaleRes);
    m_pLytLineUnitAndRes->addStretch();

    // <Line> Min and Max Value
    //--------------------------

    m_pLytLineScaleRange = new QHBoxLayout();
    m_pLytGrpScaleValues->addLayout(m_pLytLineScaleRange);

    m_pLblScaleMinVal = new QLabel("Min:");
    m_pLblScaleMinVal->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineScaleRange->addWidget(m_pLblScaleMinVal);
    m_pEdtScaleMinVal = new CWdgtEditPhysVal();
    m_pEdtScaleMinVal->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtScaleMinVal->setEnabled(false);
    m_pEdtScaleMinVal->setReadOnly(true);
    m_pLytLineScaleRange->addWidget(m_pEdtScaleMinVal);
    m_pLytLineScaleRange->addSpacing(m_cxClmSpacing);
    QObject::connect(
        m_pEdtScaleMinVal, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtDiagramScaleProperties::onEdtScaleMinValValueChanged);

    m_pLblScaleMaxVal = new QLabel("Max:");
    m_pLblScaleMaxVal->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLineScaleRange->addWidget(m_pLblScaleMaxVal);
    m_pEdtScaleMaxVal = new CWdgtEditPhysVal();
    m_pEdtScaleMaxVal->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtScaleMaxVal->setEnabled(false);
    m_pEdtScaleMaxVal->setReadOnly(true);
    m_pLytLineScaleRange->addWidget(m_pEdtScaleMaxVal);
    m_pLytLineScaleRange->addSpacing(m_cxClmSpacing);
    QObject::connect(
        m_pEdtScaleMaxVal, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtDiagramScaleProperties::onEdtScaleMaxValValueChanged);

    m_pLblScaleRangeVal = new QLabel("Range:");
    m_pLblScaleRangeVal->setFixedWidth(m_cxLblWidthClm3);
    m_pLytLineScaleRange->addWidget(m_pLblScaleRangeVal);
    m_pEdtScaleRangeVal = new CWdgtEditPhysVal();
    m_pEdtScaleRangeVal->setFixedWidth(m_cxEdtWidthClm3);
    m_pEdtScaleRangeVal->setEnabled(false);
    m_pEdtScaleRangeVal->setReadOnly(true);
    m_pLytLineScaleRange->addWidget(m_pEdtScaleRangeVal);
    m_pLytLineScaleRange->addStretch();
    QObject::connect(
        m_pEdtScaleRangeVal, &CWdgtEditPhysVal::valueChanged,
        this, &CWdgtDiagramScaleProperties::onEdtScaleRangeValueChanged);

    // <Buttons>
    //==========

    m_pLyt->addStretch();

    if (i_mode == EMode::View)
    {
        addButtonsWidget();
    }

    // <Mode>
    //==========

    setMode(i_mode);

} // ctor

//------------------------------------------------------------------------------
CWdgtDiagramScaleProperties::~CWdgtDiagramScaleProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    m_pDiagScale = nullptr;
    // Caching values
    m_scaleDir = static_cast<EScaleDir>(0);
    m_iMinVal_px = 0;
    m_iMaxVal_px = 0;
    //m_ariDivLineDistMin_px.clear();
    m_spacing = static_cast<ESpacing>(0);
    //m_scale;
    // Geometry
    m_pGrpGeometry = nullptr;
    m_pLytGrpGeometry = nullptr;
    m_pLytLineScaleDir = nullptr;
    m_pLblScaleDir = nullptr;
    m_pCmbScaleDir = nullptr;
    m_pLblSpacing = nullptr;
    m_pCmbSpacing = nullptr;
    m_pLblRange_px = nullptr;
    m_pEdtRange_px = nullptr;
    m_pLytSepLineDivLineDistMin = nullptr;
    m_pLblSepLineDivLineDistMin = nullptr;
    m_pSepLineDivLineDistMin = nullptr;
    m_pLytLineDivLineDistMin = nullptr;
    m_pLblDivLineDistMinLayerMain_px = nullptr;
    m_pEdtDivLineDistMinLayerMain_px = nullptr;
    m_pLblDivLineDistMinLayerSub_px = nullptr;
    m_pEdtDivLineDistMinLayerSub_px = nullptr;
    // Scale Values
    m_pGrpScaleValues = nullptr;
    m_pLytGrpScaleValues = nullptr;
    m_pLytLineUnitAndRes = nullptr;
    m_pLblScaleUnit = nullptr;
    m_pCmbScaleUnit = nullptr;
    m_pLblScaleRes = nullptr;
    m_pEdtScaleRes = nullptr;
    m_pLytLineScaleRange = nullptr;
    m_pLblScaleMinVal = nullptr;
    m_pEdtScaleMinVal = nullptr;
    m_pLblScaleMaxVal = nullptr;
    m_pEdtScaleMaxVal = nullptr;
    m_pLblScaleRangeVal = nullptr;
    m_pEdtScaleRangeVal = nullptr;

} // dtor

/*==============================================================================
protected: // overridable init methods of base class CWdgtDiagramItemPropertiesBase
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::setMode(EMode i_mode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMode",
        /* strAddInfo   */ strMthInArgs );

    if (m_mode != i_mode)
    {
        m_mode = i_mode;

        if (m_mode == EMode::Edit)
        {
            //m_pCmbScaleDir->setEnabled(m_pDiagScale != nullptr); remains disabled as scale dir cannot be changed
            m_pCmbSpacing->setEnabled(m_pDiagScale != nullptr);
            //m_pEdtRange_px->setEnabled(m_pDiagScale != nullptr); remains disabled
            m_pEdtDivLineDistMinLayerMain_px->setEnabled(m_pDiagScale != nullptr);
            m_pEdtDivLineDistMinLayerMain_px->setReadOnly(m_pDiagScale == nullptr);
            m_pEdtDivLineDistMinLayerSub_px->setEnabled(m_pDiagScale != nullptr);
            m_pEdtDivLineDistMinLayerSub_px->setReadOnly(m_pDiagScale == nullptr);
            m_pCmbScaleUnit->setEnabled(m_pDiagScale != nullptr);
            //m_pEdtScaleRes->setEnabled(m_pDiagScale != nullptr); remains disabled
            //m_pEdtScaleRes->setReadOnly(m_pDiagScale == nullptr); remains disabled
            m_pEdtScaleMinVal->setEnabled(m_pDiagScale != nullptr);
            m_pEdtScaleMinVal->setReadOnly(m_pDiagScale == nullptr);
            m_pEdtScaleMaxVal->setEnabled(m_pDiagScale != nullptr);
            m_pEdtScaleMaxVal->setReadOnly(m_pDiagScale == nullptr);
            m_pEdtScaleRangeVal->setEnabled(m_pDiagScale != nullptr);
            m_pEdtScaleRangeVal->setReadOnly(m_pDiagScale == nullptr);

            if (m_pWdgtButtons != nullptr)
            {
                m_pBtnEdit->setEnabled(false);
                m_pWdgtButtons->hide();
            }
        }
        else // if (m_mode == EMode::View)
        {
            m_pCmbSpacing->setEnabled(false);
            m_pEdtRange_px->setEnabled(false);
            m_pEdtDivLineDistMinLayerMain_px->setEnabled(false);
            m_pEdtDivLineDistMinLayerMain_px->setReadOnly(true);
            m_pEdtDivLineDistMinLayerSub_px->setEnabled(false);
            m_pEdtDivLineDistMinLayerSub_px->setReadOnly(true);
            m_pCmbScaleUnit->setEnabled(m_pDiagScale != nullptr);
            m_pEdtScaleRes->setEnabled(false);
            m_pEdtScaleRes->setReadOnly(true);
            m_pEdtScaleMinVal->setEnabled(false);
            m_pEdtScaleMinVal->setReadOnly(true);
            m_pEdtScaleMaxVal->setEnabled(false);
            m_pEdtScaleMaxVal->setReadOnly(true);
            m_pEdtScaleRangeVal->setEnabled(false);
            m_pEdtScaleRangeVal->setReadOnly(true);

            if (m_pWdgtButtons != nullptr)
            {
                m_pBtnEdit->setEnabled(m_pDiagScale != nullptr);
                m_pWdgtButtons->setVisible(m_pDiagScale != nullptr);
            }
        }
        emit modeChanged();
    }
}

/*==============================================================================
public: // overridables of base class CWdgtDiagramItemPropertiesBase
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::setDiagItemObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setDiagItemObjName",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_strDiagItemObjName != i_strObjName )
    {
        m_strDiagItemObjName = i_strObjName;

        if (m_pDiagScale != nullptr)
        {
            QObject::disconnect(
                m_pDiagScale, &QObject::destroyed,
                this, &CWdgtDiagramScaleProperties::onDiagScaleDestroyed);
            QObject::disconnect(
                m_pDiagScale, &CDiagScale::spacingChanged,
                this, &CWdgtDiagramScaleProperties::onDiagItemScaleSpacingChanged);
            QObject::disconnect(
                m_pDiagScale, &CDiagScale::scaleChanged,
                this, &CWdgtDiagramScaleProperties::onDiagItemScaleChanged);
            QObject::disconnect(
                m_pDiagScale, &CDiagScale::geometryChanged,
                this, &CWdgtDiagramScaleProperties::onDiagItemScaleGeometryChanged);
            QObject::disconnect(
                m_pDiagScale, &CDiagScale::divLineDistMinPixChanged,
                this, &CWdgtDiagramScaleProperties::onDiagItemScaleDivLineDistMinPixChanged);
        }

        QObject::disconnect(
            m_pCmbScaleUnit, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CWdgtDiagramScaleProperties::onCmbScaleUnitCurrentIndexChanged);

        m_pDiagScale = m_pDiagram->findDiagScale(i_strObjName);

        if (m_pDiagScale == nullptr)
        {
            m_pCmbScaleUnit->clear();

            m_pCmbSpacing->setEnabled(false);
            m_pEdtRange_px->setEnabled(false);
            m_pEdtDivLineDistMinLayerMain_px->setEnabled(false);
            m_pEdtDivLineDistMinLayerMain_px->setReadOnly(true);
            m_pEdtDivLineDistMinLayerSub_px->setEnabled(false);
            m_pEdtDivLineDistMinLayerSub_px->setReadOnly(true);
            m_pCmbScaleUnit->setEnabled(false);
            m_pEdtScaleRes->setEnabled(false);
            m_pEdtScaleRes->setReadOnly(true);
            m_pEdtScaleMinVal->setEnabled(false);
            m_pEdtScaleMinVal->setReadOnly(true);
            m_pEdtScaleMaxVal->setEnabled(false);
            m_pEdtScaleMaxVal->setReadOnly(true);
            m_pEdtScaleRangeVal->setEnabled(false);
            m_pEdtScaleRangeVal->setReadOnly(true);

            if (m_pWdgtButtons != nullptr)
            {
                m_pBtnEdit->setEnabled(false);
                m_pWdgtButtons->hide();
            }
        }
        else
        {
            //m_pCmbScaleDir->setEnabled(m_mode == EMode::Edit); remains disabled as scale dir cannot be changed
            m_pCmbSpacing->setEnabled(m_mode == EMode::Edit);
            //m_pEdtRange_px->setEnabled(m_mode == EMode::Edit); remains disabled
            //m_pEdtRange_px->setReadOnly(m_mode != EMode::Edit); remains disabled
            m_pEdtDivLineDistMinLayerMain_px->setEnabled(m_mode == EMode::Edit);
            m_pEdtDivLineDistMinLayerMain_px->setReadOnly(m_mode != EMode::Edit);
            m_pEdtDivLineDistMinLayerSub_px->setEnabled(m_mode == EMode::Edit);
            m_pEdtDivLineDistMinLayerSub_px->setReadOnly(m_mode != EMode::Edit);
            m_pCmbScaleUnit->setEnabled(true); // always enabled if scale object is available
            //m_pEdtScaleRes->setEnabled(m_mode == EMode::Edit); remains disabled
            //m_pEdtScaleRes->setReadOnly(m_mode != EMode::Edit); remains disabled
            m_pEdtScaleMinVal->setEnabled(m_mode == EMode::Edit);
            m_pEdtScaleMinVal->setReadOnly(m_mode != EMode::Edit);
            m_pEdtScaleMaxVal->setEnabled(m_mode == EMode::Edit);
            m_pEdtScaleMaxVal->setReadOnly(m_mode != EMode::Edit);
            m_pEdtScaleRangeVal->setEnabled(m_mode == EMode::Edit);
            m_pEdtScaleRangeVal->setReadOnly(m_mode != EMode::Edit);

            if (m_pWdgtButtons != nullptr)
            {
                m_pBtnEdit->setEnabled(m_mode == EMode::View);
                m_pWdgtButtons->setVisible(m_mode == EMode::View);
            }

            CScale scale = m_pDiagScale->getScale();

            m_pCmbScaleUnit->clear();
            QVector<CUnit> arUnits = scale.unit().getAllUnitsOfGroup();
            for (const CUnit& unit : arUnits) {
                m_pCmbScaleUnit->addItem(unit.symbol());
            }
            m_pCmbScaleUnit->setCurrentText(scale.unit().symbol());

            setScaleDir(m_pDiagScale->getScaleDir());

            onDiagItemScaleSpacingChanged(m_pDiagScale->getSpacing());
            onDiagItemScaleGeometryChanged(m_pDiagScale->getMinValPix(), m_pDiagScale->getMaxValPix());
            onDiagItemScaleDivLineDistMinPixChanged(EDivLineLayer::Main, m_pDiagScale->getDivLineDistMinPix(EDivLineLayer::Main));
            onDiagItemScaleDivLineDistMinPixChanged(EDivLineLayer::Sub, m_pDiagScale->getDivLineDistMinPix(EDivLineLayer::Sub));
            onDiagItemScaleChanged(scale);

            QObject::connect(
                m_pDiagScale, &QObject::destroyed,
                this, &CWdgtDiagramScaleProperties::onDiagScaleDestroyed);
            QObject::connect(
                m_pDiagScale, &CDiagScale::spacingChanged,
                this, &CWdgtDiagramScaleProperties::onDiagItemScaleSpacingChanged);
            QObject::connect(
                m_pDiagScale, &CDiagScale::scaleChanged,
                this, &CWdgtDiagramScaleProperties::onDiagItemScaleChanged);
            QObject::connect(
                m_pDiagScale, &CDiagScale::geometryChanged,
                this, &CWdgtDiagramScaleProperties::onDiagItemScaleGeometryChanged);
            QObject::connect(
                m_pDiagScale, &CDiagScale::divLineDistMinPixChanged,
                this, &CWdgtDiagramScaleProperties::onDiagItemScaleDivLineDistMinPixChanged);
        }

        QObject::connect(
            m_pCmbScaleUnit, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CWdgtDiagramScaleProperties::onCmbScaleUnitCurrentIndexChanged);

        emit_diagItemObjNameChanged();
    }
}

/*==============================================================================
public: // overridables of base class CWdgtDiagramItemPropertiesBase
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDiagramScaleProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;
    if (m_pDiagScale != nullptr)
    {
        bHasChanges = (m_scaleDir != m_pDiagScale->getScaleDir());
        if (!bHasChanges) {
            bHasChanges = (m_spacing != m_pDiagScale->getSpacing());
        }
        if (!bHasChanges) {
            bHasChanges = (m_iMinVal_px != m_pDiagScale->getMinValPix());
        }
        if (!bHasChanges) {
            bHasChanges = (m_iMaxVal_px != m_pDiagScale->getMaxValPix());
        }
        if (!bHasChanges) {
            bHasChanges = (m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Main)] != m_pDiagScale->getDivLineDistMinPix(EDivLineLayer::Main));
        }
        if (!bHasChanges) {
            bHasChanges = (m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Sub)] != m_pDiagScale->getDivLineDistMinPix(EDivLineLayer::Sub));
        }
        if (!bHasChanges) {
            bHasChanges = (m_scale != m_pDiagScale->getScale());
        }
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (m_pDiagScale != nullptr)
    {
        m_pDiagScale->setSpacing(m_spacing);
        m_pDiagScale->setDivLineDistMinPix(
            EDivLineLayer::Main, m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Main)]);
        m_pDiagScale->setDivLineDistMinPix(
            EDivLineLayer::Sub, m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Sub)]);
        m_pDiagScale->setScale(m_scale);
        m_pDiagram->update();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    if (m_pDiagScale != nullptr)
    {
        onDiagItemScaleSpacingChanged(m_pDiagScale->getSpacing());
        onDiagItemScaleGeometryChanged(m_pDiagScale->getMinValPix(), m_pDiagScale->getMaxValPix());
        onDiagItemScaleDivLineDistMinPixChanged(EDivLineLayer::Main, m_pDiagScale->getDivLineDistMinPix(EDivLineLayer::Main));
        onDiagItemScaleDivLineDistMinPixChanged(EDivLineLayer::Sub, m_pDiagScale->getDivLineDistMinPix(EDivLineLayer::Sub));
        onDiagItemScaleChanged(m_pDiagScale->getScale());
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onDiagItemScaleSpacingChanged(const CEnumSpacing& i_spacing)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_spacing.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDiagItemScaleSpacingChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_spacing != i_spacing ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_spacing = i_spacing;
        m_pCmbSpacing->setCurrentText(m_spacing.toString());
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onDiagItemScaleGeometryChanged(int i_iMinVal_px, int i_iMaxVal_px)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iMinVal_px) + ", " + QString::number(i_iMaxVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDiagItemScaleGeometryChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_iMinVal_px != i_iMinVal_px || m_iMaxVal_px != i_iMaxVal_px ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_iMinVal_px = i_iMinVal_px;
        m_iMaxVal_px = i_iMaxVal_px;
        int iRange_px = (m_iMaxVal_px > m_iMinVal_px) ?
            m_iMaxVal_px - m_iMinVal_px + 1 : m_iMinVal_px - m_iMaxVal_px + 1;
        m_pEdtRange_px->setText(QString::number(iRange_px) + " px");
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onDiagItemScaleDivLineDistMinPixChanged(EDivLineLayer i_layer, int i_iDivLineDistMin_px)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumDivLineLayer(i_layer).toString() + "," + QString::number(i_iDivLineDistMin_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDiagItemScaleDivLineDistMinPixChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_ariDivLineDistMin_px[static_cast<int>(i_layer)] != i_iDivLineDistMin_px ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_ariDivLineDistMin_px[static_cast<int>(i_layer)] = i_iDivLineDistMin_px;
        if (i_layer == EDivLineLayer::Main) {
            m_pEdtDivLineDistMinLayerMain_px->setValue(i_iDivLineDistMin_px);
        }
        else {
            m_pEdtDivLineDistMinLayerSub_px->setValue(i_iDivLineDistMin_px);
        }
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onDiagItemScaleChanged(const CScale& i_scale)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_scale.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDiagItemScaleChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_scale != i_scale ) {
        // The unit used to indicate the scale in the widget is set by the units combo box.
        // We need to convert the scale values into the indicated unit.
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        CUnit unitCurr = m_pCmbScaleUnit->currentText();
        double fScaleRes = m_pDiagScale->getScaleRes().getVal(unitCurr);
        m_scale = i_scale;
        m_scale.setUnit(unitCurr);
        m_pEdtScaleRes->setText(QString::number(fScaleRes) + " " + unitCurr.symbol());
        m_pEdtScaleMinVal->setUnit(unitCurr);
        m_pEdtScaleMinVal->setResolution(fScaleRes);
        m_pEdtScaleMinVal->setValue(m_scale.minVal().getVal());
        m_pEdtScaleMaxVal->setUnit(unitCurr);
        m_pEdtScaleMaxVal->setResolution(fScaleRes);
        m_pEdtScaleMaxVal->setValue(m_scale.maxVal().getVal());
        m_pEdtScaleRangeVal->setUnit(unitCurr);
        m_pEdtScaleRangeVal->setResolution(fScaleRes);
        m_pEdtScaleRangeVal->setValue(m_scale.rangeVal().getVal());
        emit_diagItemPropertyChanged();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onCmbSpacingCurrentIndexChanged( int i_idx )
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbSpacingCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_spacing != static_cast<ESpacing>(i_idx) ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_spacing = static_cast<ESpacing>(i_idx);
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onEdtDivLineDistMinMainLayerPixValueChanged(int i_iVal)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtDivLineDistMinMainLayerPixValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Main)] != i_iVal ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Main)] = i_iVal;
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onEdtDivLineDistMinSubLayerPixValueChanged(int i_iVal)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iVal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtDivLineDistMinSubLayerPixValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Sub)] != i_iVal ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Sub)] = i_iVal;
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onCmbScaleUnitCurrentIndexChanged( int i_idx )
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbScaleUnitCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    CUnit unitCurr = m_pCmbScaleUnit->currentText();

    if( m_scale.unit() != unitCurr ) {
        // The unit used to indicate the scale in the widget is set by the units combo box.
        // We need to convert the scale values into the indicated unit.
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        CUnit unitPrev = m_scale.unit();
        double fScaleRes = m_pDiagScale->getScaleRes().getVal(unitCurr);
        m_scale.setUnit(unitCurr);
        m_pEdtScaleRes->setText(QString::number(fScaleRes) + " " + unitCurr.symbol());
        m_pEdtScaleMinVal->setUnit(unitCurr);
        m_pEdtScaleMinVal->setResolution(fScaleRes);
        m_pEdtScaleMinVal->setValue(m_scale.minVal().getVal());
        m_pEdtScaleMaxVal->setUnit(unitCurr);
        m_pEdtScaleMaxVal->setResolution(fScaleRes);
        m_pEdtScaleMaxVal->setValue(m_scale.maxVal().getVal());
        m_pEdtScaleRangeVal->setUnit(unitCurr);
        m_pEdtScaleRangeVal->setResolution(fScaleRes);
        m_pEdtScaleRangeVal->setValue(m_scale.rangeVal().getVal());
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onEdtScaleMinValValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtScaleMinValValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_scale.minVal() != i_physVal ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_scale.setMinVal(i_physVal);
        m_pEdtScaleRangeVal->setValue(m_scale.rangeVal().getVal());
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onEdtScaleMaxValValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtScaleMaxValValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_scale.maxVal() != i_physVal ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_scale.setMaxVal(i_physVal);
        m_pEdtScaleRangeVal->setValue(m_scale.rangeVal().getVal());
        emit_diagItemPropertyChanged();
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onEdtScaleRangeValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked > 0 ) {
        return;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtScaleRangeValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_scale.maxVal() != i_physVal ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_scale.setRangeVal(i_physVal);
        m_pEdtScaleMaxVal->setValue(m_scale.maxVal().getVal());
        emit_diagItemPropertyChanged();
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/
//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::setScaleDir( const CEnumScaleDir& i_scaleDir )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_scaleDir.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleDir",
        /* strAddInfo   */ strMthInArgs );

    if( m_scaleDir != i_scaleDir ) {
        CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);
        m_scaleDir = i_scaleDir;
        m_pCmbScaleDir->setCurrentText(m_scaleDir.toString());
        if (m_scaleDir == EScaleDir::X) {
            m_pLblRange_px->setText("Width:");
        }
        else if (m_scaleDir == EScaleDir::Y) {
            m_pLblRange_px->setText("Height;");
        }
        else {
            m_pLblRange_px->setText("Range:");
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramScaleProperties::onDiagScaleDestroyed(QObject* i_pObj)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDiagScaleDestroyed",
        /* strAddInfo   */ strMthInArgs );

    m_pDiagScale = nullptr;

    setDiagItemObjName("");

    emit diagItemDestroyed(i_pObj);
}
