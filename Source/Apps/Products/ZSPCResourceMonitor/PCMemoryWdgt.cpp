/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qsettings.h>

#include <QtGui/qpainter.h>
#include <QtGui/qvalidator.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#else
#include <QtWidgets/qapplication.h>
#endif

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "PCMemoryWdgt.h"
#include "PCMemoryModel.h"
#include "PCResourceMonitor.h"

#include "ZSPhysSizes/Kinematics/ZSPhysSizes.h"
#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"
#include "ZSDiagram/ZSDiagram.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSDiagram/ZSDiagramLabelStyles.h"
#include "ZSDiagram/ZSDiagramLineStyles.h"
#include "ZSDiagram/ZSDiagramImageStyles.h"
#include "ZSDiagram/ZSDiagramToolTipStyles.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagObjAxisLabel.h"
#include "ZSDiagram/ZSDiagObjCurve.h"
#include "ZSDiagram/ZSDiagObjGrid.h"
#include "ZSDiagram/ZSDiagObjLine.h"
#include "ZSDiagram/ZSDiagObjMarker.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Diagram;
using namespace ZS::Apps::Products::PCResourceMonitor;


/*******************************************************************************
class CWdgtPCMemory : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtPCMemory::CWdgtPCMemory(
    CPCResourceMonitor* i_pPCResourceMonitor,
    QWidget*            i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pPCResourceMonitor(i_pPCResourceMonitor),
    m_pLyt(nullptr),
    m_pGrpEquipment(nullptr),
    m_pLytGrpEquipment(nullptr),
    m_pLytLinePhysicalMemory(nullptr),
    m_pLblPhysicalMemorySize(nullptr),
    m_pEdtPhysicalMemorySize(nullptr),
    m_pLytLineVirtualMemory(nullptr),
    m_pLblVirtualMemorySize(nullptr),
    m_pEdtVirtualMemorySize(nullptr),
    m_pModelMemory(nullptr),
    m_pGrpLoadValues(nullptr),
    m_pLytGrpLoadValues(nullptr),
    m_pSplitterLoadValues(nullptr),
    m_pWdgtMemoryValues(nullptr),
    m_pLytMemoryValues(nullptr),
    m_pLytMemoryValuesShowAll(nullptr),
    m_pLblMemoryValuesShowAll(nullptr),
    m_pChkMemoryValuesShowAll(nullptr),
    m_strDataQuantityUnit("B"),
    m_pLblMemoryValuesUnit(nullptr),
    m_pCmbMemoryValuesUnit(nullptr),
    m_pModelMemoryValues(nullptr),
    m_pTableViewMemoryValues(nullptr),
    m_pWdgtDiagram(nullptr),
    m_scaleX(
        /* fMin  */ 0.0,
        /* fMax  */ 60000.0,
        /* pUnit */ Kinematics::Time()->MilliSeconds() ),
    m_scaleY(
        /* fMin     */ 0.0,
        /* fMax     */ 4000000000.0,
        /* physUnit */ DataQuantity()->Byte() ),
    m_pDiagScaleX(nullptr),
    m_pDiagScaleY(nullptr),
    m_pDiagObjGrid(nullptr),
    m_pDiagObjAxisLabelX(nullptr),
    m_pDiagObjAxisLabelY(nullptr),
    m_pDiagObjLinePhysicalMemorySize(nullptr),
    m_pDiagTracePhysicalMemory(nullptr),
    m_pDiagObjCurvePhysicalMemory(nullptr),
    m_pDiagObjMarkerPhysicalMemory(nullptr),
    m_pDiagObjLineVirtualMemorySize(nullptr),
    m_pDiagTraceVirtualMemory(nullptr),
    m_pDiagObjCurveVirtualMemory(nullptr),
    m_pDiagObjMarkerVirtualMemory(nullptr)
{
    setObjectName("WdgtPCMemory");

    QPalette paletteWdgtCentral;
    paletteWdgtCentral.setColor(QPalette::Window,Qt::lightGray);
    setPalette(paletteWdgtCentral);
    setAutoFillBackground(true);

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // <GroupBox> Equipment
    //=======================

    m_pGrpEquipment = new QGroupBox("Equipment");
    m_pLytGrpEquipment = new QVBoxLayout();
    m_pGrpEquipment->setLayout(m_pLytGrpEquipment);
    m_pLyt->addWidget(m_pGrpEquipment);

    m_pModelMemory = m_pPCResourceMonitor->getModelMemory();

    // <Line> Physical Memory
    //-----------------------

    m_pLytLinePhysicalMemory = new QHBoxLayout();
    m_pLytGrpEquipment->addLayout(m_pLytLinePhysicalMemory);
    m_pLblPhysicalMemorySize = new QLabel("Physical Memory Size:");
    m_pLblPhysicalMemorySize->setFixedWidth(120);
    m_pLytLinePhysicalMemory->addWidget(m_pLblPhysicalMemorySize);
    m_pEdtPhysicalMemorySize = new QLabel(m_pModelMemory->getPhysicalMemorySize().toString());
    m_pLytLinePhysicalMemory->addWidget(m_pEdtPhysicalMemorySize);
    m_pLytLinePhysicalMemory->addStretch();

    // <Line> Virtual Memory
    //-----------------------

    m_pLytLineVirtualMemory = new QHBoxLayout();
    m_pLytGrpEquipment->addLayout(m_pLytLineVirtualMemory);
    m_pLblVirtualMemorySize = new QLabel("Virtual Memory Size:");
    m_pLblVirtualMemorySize->setFixedWidth(120);
    m_pLytLineVirtualMemory->addWidget(m_pLblVirtualMemorySize);
    m_pEdtVirtualMemorySize = new QLabel(m_pModelMemory->getVirtualMemorySize().toString());
    m_pLytLineVirtualMemory->addWidget(m_pEdtVirtualMemorySize);
    m_pLytLineVirtualMemory->addStretch();

    // <GroupBox> Load Values
    //=======================

    m_pGrpLoadValues = new QGroupBox("Values");
    m_pLytGrpLoadValues = new QHBoxLayout();
    m_pGrpLoadValues->setLayout(m_pLytGrpLoadValues);
    m_pLyt->addWidget(m_pGrpLoadValues);

    m_pSplitterLoadValues = new QSplitter(Qt::Vertical);
    m_pLytGrpLoadValues->addWidget(m_pSplitterLoadValues);

    // <Table> Memory Values
    //----------------------

    m_pWdgtMemoryValues = new QWidget();
    m_pSplitterLoadValues->addWidget(m_pWdgtMemoryValues);

    m_pLytMemoryValues = new QVBoxLayout();
    #if QT_VERSION >= 0x040501
    m_pLytMemoryValues->setContentsMargins(0,0,0,0);
    #endif
    m_pWdgtMemoryValues->setLayout(m_pLytMemoryValues);

    // <CheckBox> Show all values
    //---------------------------

    m_pLytMemoryValuesShowAll = new QHBoxLayout();
    m_pLytMemoryValues->addLayout(m_pLytMemoryValuesShowAll);

    m_pLblMemoryValuesShowAll = new QLabel("Show All Values");
    m_pLytMemoryValuesShowAll->addWidget(m_pLblMemoryValuesShowAll);
    m_pChkMemoryValuesShowAll = new QCheckBox();
    m_pLytMemoryValuesShowAll->addWidget(m_pChkMemoryValuesShowAll);
    m_pLytMemoryValuesShowAll->addSpacing(10);

    if( !connect(
        /* pObjSender   */ m_pChkMemoryValuesShowAll,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkMemoryValuesShowAllToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <ComboBox> Unit
    //----------------

    m_pLblMemoryValuesUnit = new QLabel("Unit:");
    m_pLytMemoryValuesShowAll->addWidget(m_pLblMemoryValuesUnit);
    m_pCmbMemoryValuesUnit = new QComboBox();
    m_pLytMemoryValuesShowAll->addWidget(m_pCmbMemoryValuesUnit);
    m_pLytMemoryValuesShowAll->addStretch();

    CUnitGrpDataQuantity* pUnitGrpDataQuantity = DataQuantity();
    int                   idxUnit;
    int                   idxCmbItem;

    for( idxUnit = 0; idxUnit < pUnitGrpDataQuantity->getUnitCount(); idxUnit++ )
    {
        m_pCmbMemoryValuesUnit->addItem(pUnitGrpDataQuantity->getUnit(idxUnit)->getSymbol());
    }
    idxCmbItem = m_pCmbMemoryValuesUnit->findText(m_strDataQuantityUnit);
    if( idxCmbItem >= 0 && idxCmbItem < m_pCmbMemoryValuesUnit->count() )
    {
        m_pCmbMemoryValuesUnit->setCurrentIndex(idxCmbItem);
    }

    if( !connect(
        /* pObjSender   */ m_pCmbMemoryValuesUnit,
        /* szSignal     */ SIGNAL( currentIndexChanged( const QString& ) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onCmbMemoryValuesUnitCurrentIndexChanged( const QString& ) ) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Table> Memory Values
    //----------------------

    m_pModelMemoryValues = m_pPCResourceMonitor->getModelMemoryValues();

    m_pTableViewMemoryValues = new QTableView(this);
    m_pLytMemoryValues->addWidget(m_pTableViewMemoryValues);

    m_pTableViewMemoryValues->setModel(m_pModelMemoryValues);

    QFont fntTableView = m_pTableViewMemoryValues->font();
    fntTableView.setPointSize(8);

    m_pTableViewMemoryValues->setFont(fntTableView);
    m_pTableViewMemoryValues->setShowGrid(false);
    m_pTableViewMemoryValues->setAlternatingRowColors(true);
    m_pTableViewMemoryValues->verticalHeader()->show();
    m_pTableViewMemoryValues->horizontalHeader()->show();
    m_pTableViewMemoryValues->setTextElideMode(Qt::ElideRight);
    m_pTableViewMemoryValues->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    // <Diagram> Memory Values (used space)
    //-------------------------------------

    QColor colCurve;
    QColor colMarker;

    m_pWdgtDiagram = new CWdgtDiagram(
        /* pWdgtParent */ nullptr,
        /* strObjName  */ "Diagram" );
    m_pWdgtDiagram->setMouseTracking(true);
    m_pWdgtDiagram->setEraseColor(Qt::lightGray);
    m_pWdgtDiagram->setColBgPartCenter(Qt::white);
    m_pWdgtDiagram->setFrameStylePartCenter(new SFrameStyle(1,Qt::darkGray));
    m_pWdgtDiagram->enableZooming();
    m_pWdgtDiagram->setMinimumHeightPartTop(10);
    m_pWdgtDiagram->setMinimumHeightPartBottom(20);
    m_pWdgtDiagram->setMinimumWidthPartLeft(0);
    m_pWdgtDiagram->setMinimumWidthPartRight(10);

    SFrameStyle3DSunken* pFrameStyleDiagram = new SFrameStyle3DSunken;
    m_pWdgtDiagram->setFrameStyle(pFrameStyleDiagram);
    pFrameStyleDiagram = nullptr;

    m_pSplitterLoadValues->addWidget(m_pWdgtDiagram);

    m_pDiagScaleX = new CDiagScale(
        /* strObjName */ "DiagScaleX",
        /* scaleDir   */ EScaleDirX,
        /* scale      */ m_scaleX );
    m_pDiagScaleX->setDivLineDistMinPix(EDivLineLayerMain,20);
    m_pWdgtDiagram->addDiagScale(m_pDiagScaleX);

    m_pDiagScaleY = new CDiagScale(
        /* strObjName */ "DiagScaleY",
        /* scaleDir   */ EScaleDirY,
        /* scale      */ m_scaleY );
    m_pDiagScaleY->setDivLineDistMinPix(EDivLineLayerMain,20);
    m_pWdgtDiagram->addDiagScale(m_pDiagScaleY);

    m_pDiagObjGrid = new CDiagObjGrid(
        /* strObjName  */ "DiagObjGrid",
        /* pDiagScaleX */ m_pDiagScaleX,
        /* pDiagScaleY */ m_pDiagScaleY );
    m_pDiagObjGrid->setCol(EDivLineLayerCount,Qt::darkGray);
    m_pWdgtDiagram->addDiagObj(m_pDiagObjGrid);

    m_pDiagObjAxisLabelX = new CDiagObjAxisLabel(
        /* strObjName  */ "DiagObjAxisLabelX",
        /* pDiagScaleX */ m_pDiagScaleX,
        /* layoutPos   */ ELayoutPosBottom );
    m_pDiagObjAxisLabelX->setUnit(Kinematics::Time()->Seconds());
    m_pWdgtDiagram->addDiagObj(m_pDiagObjAxisLabelX);

    m_pDiagObjAxisLabelY = new CDiagObjAxisLabel(
        /* strObjName  */ "DiagObjAxisLabelY",
        /* pDiagScaleX */ m_pDiagScaleY,
        /* layoutPos   */ ELayoutPosLeft );
    m_pWdgtDiagram->addDiagObj(m_pDiagObjAxisLabelY);

    // Limit line for physical memory
    //-------------------------------

    colCurve = m_pModelMemory->getColorPhysicalMemory();

    m_pDiagObjLinePhysicalMemorySize = new CDiagObjLine(
        /* strObjName  */ "DiagObjLinePhysicalMemorySize",
        /* pDiagScaleX */ m_pDiagScaleX,
        /* pDiagScaleY */ m_pDiagScaleY,
        /* pLineStyle  */ new SLineStyle(colCurve,Qt::DashDotLine,2) );
    m_pWdgtDiagram->addDiagObj(m_pDiagObjLinePhysicalMemorySize);

    // Trace for physical memory
    //--------------------------

    m_pDiagTracePhysicalMemory = new CDiagTrace(
        /* strObjName  */ "DiagTracePhysicalMemory",
        /* pDiagScaleX */ m_pDiagScaleX,
        /* pDiagScaleY */ m_pDiagScaleY );
    m_pWdgtDiagram->addDiagTrace(m_pDiagTracePhysicalMemory);

    m_pDiagObjCurvePhysicalMemory = new CDiagObjCurve(
        /* strObjName */ "DiagObjCurvePhysicalMemory",
        /* pDiagTrace */ m_pDiagTracePhysicalMemory );
    m_pDiagObjCurvePhysicalMemory->setCol(colCurve);
    m_pWdgtDiagram->addDiagObj(m_pDiagObjCurvePhysicalMemory);

    colMarker = m_pModelMemory->getColorPhysicalMemory();
    //colMarker.setAlpha(127);

    m_pDiagObjMarkerPhysicalMemory = new CDiagObjMarker(
        /* strObjName */ "DiagObjMarkerPhysicalMemory",
        /* pDiagTrace */ m_pDiagTracePhysicalMemory );
    m_pDiagObjMarkerPhysicalMemory->setVal(EScaleDirX,0.0);

    CLabelStyle* pLabelStyleMarkerPhysicalMemory = new CLabelStyle(
        /* strLabel      */ "P",
        /* brushStyle    */ Qt::SolidPattern,
        /* colFg         */ colMarker,
        /* colBg         */ m_pWdgtDiagram->getColBgPartCenter(),
        /* fntLabel      */ QFont(),
        /* iMarginTop    */ 0,
        /* iMarginBottom */ 0,
        /* iMarginLeft   */ 0,
        /* iMarginRight  */ 0 );
    m_pDiagObjMarkerPhysicalMemory->setLabelStyle(EDiagObjStateCount,pLabelStyleMarkerPhysicalMemory);
    pLabelStyleMarkerPhysicalMemory = 0;
    m_pDiagObjMarkerPhysicalMemory->showElement(EDiagObjStateCount,CDiagObjMarker::EElementLabel);

    SLineStyle* pLineStyleMarkerPhysicalMemory = new SLineStyle(colMarker,Qt::DotLine,1);
    m_pDiagObjMarkerPhysicalMemory->setLineStyleVer(EDiagObjStateCount,pLineStyleMarkerPhysicalMemory);
    pLineStyleMarkerPhysicalMemory = nullptr; //lint !e423
    m_pDiagObjMarkerPhysicalMemory->showElement(EDiagObjStateCount,CDiagObjMarker::EElementLineVer);

    CToolTipStyle* pToolTipStyleMarkerPhysicalMemory = new CToolTipStyle(
        /* colFg         */ Qt::white,
        /* colBg         */ colMarker,
        /* fnt           */ QFont(),
        /* cxOffs        */ 8,
        /* cyOffs        */ 8,
        /* pFrameStyle   */ nullptr,
        /* iMarginTop    */ 0,
        /* iMarginBottom */ 0,
        /* iMarginLeft   */ 0,
        /* iMarginRight  */ 0,
        /* cxArrowWidth  */ 6 );
    m_pDiagObjMarkerPhysicalMemory->setToolTipStyle(EDiagObjStateCount,pToolTipStyleMarkerPhysicalMemory);
    pToolTipStyleMarkerPhysicalMemory = nullptr;
    SValueFormatProvider* pValueFormatMarkerToolTipXVal = new SValueFormatProvider(
        /* pUnitVal                      */ Kinematics::Time()->Seconds(),
        /* unitFindVal                   */ EUnitFindNone,
        /* iValSubStrVisibility          */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
        /* iDigitsMantissa               */ -1,
        /* iDigitsExponent               */ -1,
        /* bUseEngineeringFormat         */ false,
        /* iDigitsPerDigitGroup          */ -1,
        /* pstrDigitsGroupDelimiter      */ nullptr,
        /* pstrDecimalPoint              */ nullptr,
        /* iDigitsAccuracy               */ 1,
        /* bDigitsAccuracyLimitsMantissa */ false );
    m_pDiagObjMarkerPhysicalMemory->setToolTipValueFormat(EScaleDirX,pValueFormatMarkerToolTipXVal);
    pValueFormatMarkerToolTipXVal = nullptr;
    m_pDiagObjMarkerPhysicalMemory->showElement(EDiagObjStateCount,CDiagObjMarker::EElementToolTip);

    CImageStyleHCursorFocused* pImageStyleHCursorFocusedPhysicalMemory = new CImageStyleHCursorFocused();
    pImageStyleHCursorFocusedPhysicalMemory->setColBg(Qt::black);
    m_pDiagObjMarkerPhysicalMemory->setImageStyleCursor(EDiagObjStateFocused,pImageStyleHCursorFocusedPhysicalMemory);
    pImageStyleHCursorFocusedPhysicalMemory = nullptr; //lint !e423
    m_pDiagObjMarkerPhysicalMemory->showElement(EDiagObjStateFocused,CDiagObjMarker::EElementCursor);

    CImageStyleHCursorEditing* pImageStyleHCursorEditingPhysicalMemory = new CImageStyleHCursorEditing();
    pImageStyleHCursorEditingPhysicalMemory->setColBg(Qt::black);
    m_pDiagObjMarkerPhysicalMemory->setImageStyleCursor(EDiagObjStateEditing,pImageStyleHCursorEditingPhysicalMemory);
    pImageStyleHCursorEditingPhysicalMemory = nullptr; //lint !e423
    m_pDiagObjMarkerPhysicalMemory->showElement(EDiagObjStateEditing,CDiagObjMarker::EElementCursor);

    m_pWdgtDiagram->addDiagObj(m_pDiagObjMarkerPhysicalMemory);

    // Limit line for virtual memory
    //-------------------------------

    colCurve = m_pModelMemory->getColorVirtualMemory();

    m_pDiagObjLineVirtualMemorySize = new CDiagObjLine(
        /* strObjName  */ "DiagObjLineVirtualMemorySize",
        /* pDiagScaleX */ m_pDiagScaleX,
        /* pDiagScaleY */ m_pDiagScaleY,
        /* pLineStyle  */ new SLineStyle(colCurve,Qt::DashDotLine,2) );
    m_pWdgtDiagram->addDiagObj(m_pDiagObjLineVirtualMemorySize);

    // Trace for virtual memory
    //--------------------------

    m_pDiagTraceVirtualMemory = new CDiagTrace(
        /* strObjName  */ "DiagTraceVirtualMemory",
        /* pDiagScaleX */ m_pDiagScaleX,
        /* pDiagScaleY */ m_pDiagScaleY );
    m_pWdgtDiagram->addDiagTrace(m_pDiagTraceVirtualMemory);

    m_pDiagObjCurveVirtualMemory = new CDiagObjCurve(
        /* strObjName */ "DiagObjCurveVirtualMemory",
        /* pDiagTrace */ m_pDiagTraceVirtualMemory );
    m_pDiagObjCurveVirtualMemory->setCol(colCurve);
    m_pWdgtDiagram->addDiagObj(m_pDiagObjCurveVirtualMemory);

    colMarker = m_pModelMemory->getColorVirtualMemory();
    //colMarker.setAlpha(127);

    m_pDiagObjMarkerVirtualMemory = new CDiagObjMarker(
        /* strObjName */ "DiagObjMarkerVirtualMemory",
        /* pDiagTrace */ m_pDiagTraceVirtualMemory );
    m_pDiagObjMarkerVirtualMemory->setVal(EScaleDirX,0.0);

    CLabelStyle* pLabelStyleMarkerVirtualMemory = new CLabelStyle(
        /* strLabel      */ "V",
        /* brushStyle    */ Qt::SolidPattern,
        /* colFg         */ colMarker,
        /* colBg         */ m_pWdgtDiagram->getColBgPartCenter(),
        /* fntLabel      */ QFont(),
        /* iMarginTop    */ 0,
        /* iMarginBottom */ 0,
        /* iMarginLeft   */ 0,
        /* iMarginRight  */ 0 );
    m_pDiagObjMarkerVirtualMemory->setLabelStyle(EDiagObjStateCount,pLabelStyleMarkerVirtualMemory);
    pLabelStyleMarkerVirtualMemory = 0;
    m_pDiagObjMarkerVirtualMemory->showElement(EDiagObjStateCount,CDiagObjMarker::EElementLabel);

    SLineStyle* pLineStyleMarkerVirtualMemory = new SLineStyle(colMarker,Qt::DotLine,1);
    m_pDiagObjMarkerVirtualMemory->setLineStyleVer(EDiagObjStateCount,pLineStyleMarkerVirtualMemory);
    pLineStyleMarkerVirtualMemory = nullptr; //lint !e423
    m_pDiagObjMarkerVirtualMemory->showElement(EDiagObjStateCount,CDiagObjMarker::EElementLineVer);

    CToolTipStyle* pToolTipStyleMarkerVirtualMemory = new CToolTipStyle(
        /* colFg         */ Qt::white,
        /* colBg         */ colMarker,
        /* fnt           */ QFont(),
        /* cxOffs        */ 8,
        /* cyOffs        */ 8,
        /* pFrameStyle   */ nullptr,
        /* iMarginTop    */ 0,
        /* iMarginBottom */ 0,
        /* iMarginLeft   */ 0,
        /* iMarginRight  */ 0,
        /* cxArrowWidth  */ 6 );
    m_pDiagObjMarkerVirtualMemory->setToolTipStyle(EDiagObjStateCount,pToolTipStyleMarkerVirtualMemory);
    pToolTipStyleMarkerVirtualMemory = nullptr;
    pValueFormatMarkerToolTipXVal = new SValueFormatProvider(
        /* pUnitVal                      */ Kinematics::Time()->Seconds(),
        /* unitFindVal                   */ EUnitFindNone,
        /* iValSubStrVisibility          */ EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
        /* iDigitsMantissa               */ -1,
        /* iDigitsExponent               */ -1,
        /* bUseEngineeringFormat         */ false,
        /* iDigitsPerDigitGroup          */ -1,
        /* pstrDigitsGroupDelimiter      */ nullptr,
        /* pstrDecimalPoint              */ nullptr,
        /* iDigitsAccuracy               */ 1,
        /* bDigitsAccuracyLimitsMantissa */ false );
    m_pDiagObjMarkerVirtualMemory->setToolTipValueFormat(EScaleDirX,pValueFormatMarkerToolTipXVal);
    pValueFormatMarkerToolTipXVal = nullptr;
    m_pDiagObjMarkerVirtualMemory->showElement(EDiagObjStateCount,CDiagObjMarker::EElementToolTip);

    CImageStyleHCursorFocused* pImageStyleHCursorFocusedVirtualMemory = new CImageStyleHCursorFocused();
    pImageStyleHCursorFocusedVirtualMemory->setColBg(Qt::black);
    m_pDiagObjMarkerVirtualMemory->setImageStyleCursor(EDiagObjStateFocused,pImageStyleHCursorFocusedVirtualMemory);
    pImageStyleHCursorFocusedVirtualMemory = nullptr; //lint !e423
    m_pDiagObjMarkerVirtualMemory->showElement(EDiagObjStateFocused,CDiagObjMarker::EElementCursor);

    CImageStyleHCursorEditing* pImageStyleHCursorEditingVirtualMemory = new CImageStyleHCursorEditing();
    pImageStyleHCursorEditingVirtualMemory->setColBg(Qt::black);
    m_pDiagObjMarkerVirtualMemory->setImageStyleCursor(EDiagObjStateEditing,pImageStyleHCursorEditingVirtualMemory);
    pImageStyleHCursorEditingVirtualMemory = nullptr; //lint !e423
    m_pDiagObjMarkerVirtualMemory->showElement(EDiagObjStateEditing,CDiagObjMarker::EElementCursor);

    m_pWdgtDiagram->addDiagObj(m_pDiagObjMarkerVirtualMemory);

    // Keeping indicated parameters up to date ..
    //===========================================

    onCltStdPCMemoryChanged();
    onCltStdPCMemoryValuesChanged();

    if( !connect(
        /* pObjSender   */ m_pPCResourceMonitor,
        /* szSignal     */ SIGNAL(memoryChanged()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCltStdPCMemoryChanged()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pPCResourceMonitor,
        /* szSignal     */ SIGNAL(memoryValuesChanged()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCltStdPCMemoryValuesChanged()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Restore geometry
    //=================

    // Restore position and size

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CWdgtPCMemory::~CWdgtPCMemory()
//------------------------------------------------------------------------------
{
    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    m_pPCResourceMonitor = nullptr;
    m_pLyt = nullptr;
    m_pGrpEquipment = nullptr;
    m_pLytGrpEquipment = nullptr;
    m_pLytLinePhysicalMemory = nullptr;
    m_pLblPhysicalMemorySize = nullptr;
    m_pEdtPhysicalMemorySize = nullptr;
    m_pLytLineVirtualMemory = nullptr;
    m_pLblVirtualMemorySize = nullptr;
    m_pEdtVirtualMemorySize = nullptr;
    m_pModelMemory = nullptr;
    m_pGrpLoadValues = nullptr;
    m_pLytGrpLoadValues = nullptr;
    m_pSplitterLoadValues = nullptr;
    m_pWdgtMemoryValues = nullptr;
    m_pLytMemoryValues = nullptr;
    m_pLytMemoryValuesShowAll = nullptr;
    m_pLblMemoryValuesShowAll = nullptr;
    m_pChkMemoryValuesShowAll = nullptr;
    m_pLblMemoryValuesUnit = nullptr;
    m_pCmbMemoryValuesUnit = nullptr;
    m_pModelMemoryValues = nullptr;
    m_pTableViewMemoryValues = nullptr;
    m_pWdgtDiagram = nullptr;
    m_pDiagScaleX = nullptr;
    m_pDiagScaleY = nullptr;
    m_pDiagObjGrid = nullptr;
    m_pDiagObjAxisLabelX = nullptr;
    m_pDiagObjAxisLabelY = nullptr;
    m_pDiagObjLinePhysicalMemorySize = nullptr;
    m_pDiagTracePhysicalMemory = nullptr;
    m_pDiagObjCurvePhysicalMemory = nullptr;
    m_pDiagObjMarkerPhysicalMemory = nullptr;
    m_pDiagObjLineVirtualMemorySize = nullptr;
    m_pDiagTraceVirtualMemory = nullptr;
    m_pDiagObjCurveVirtualMemory = nullptr;
    m_pDiagObjMarkerVirtualMemory = nullptr;

} // dtor

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPCMemory::onChkMemoryValuesShowAllToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pModelMemoryValues != nullptr )
    {
        m_pModelMemoryValues->showAllValues(i_bChecked);
    }

} // onChkMemoryValuesShowAllToggled

//------------------------------------------------------------------------------
void CWdgtPCMemory::onCmbMemoryValuesUnitCurrentIndexChanged( const QString& i_strSymbol )
//------------------------------------------------------------------------------
{
    m_strDataQuantityUnit = i_strSymbol;

    CUnitDataQuantity* pUnitDataQuantity = dynamic_cast<CUnitDataQuantity*>(DataQuantity()->findUnitBySymbol(m_strDataQuantityUnit));

    if( pUnitDataQuantity != nullptr )
    {
        m_pModelMemory->setUnit(pUnitDataQuantity);
        m_pModelMemoryValues->setUnit(pUnitDataQuantity);

        onCltStdPCMemoryChanged();
        onCltStdPCMemoryValuesChanged();
    }

} // onCmbMemoryValuesUnitCurrentIndexChanged

/*==============================================================================
protected slots: // connected to the signals of the standard PC client
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPCMemory::onCltStdPCMemoryChanged()
//------------------------------------------------------------------------------
{
    m_pEdtPhysicalMemorySize->setText(m_pModelMemory->getPhysicalMemorySize().toString());
    m_pEdtVirtualMemorySize->setText(m_pModelMemory->getVirtualMemorySize().toString());

} // onCltStdPCMemoryChanged

//------------------------------------------------------------------------------
void CWdgtPCMemory::onCltStdPCMemoryValuesChanged()
//------------------------------------------------------------------------------
{
    // Update tables
    //--------------

    m_pTableViewMemoryValues->resizeColumnsToContents();
    m_pTableViewMemoryValues->resizeRowsToContents();

    // Update diagram
    //---------------

    // X-Scale
    double fXScaleRange_ms = m_pModelMemoryValues->getLastPhysicalMemoryUpdSysTime().getVal() - m_pModelMemoryValues->getFirstPhysicalMemoryUpdSysTime().getVal();
    if( fXScaleRange_ms < m_pModelMemoryValues->getLastVirtualMemoryUpdSysTime().getVal() - m_pModelMemoryValues->getFirstVirtualMemoryUpdSysTime().getVal() )
    {
        fXScaleRange_ms = m_pModelMemoryValues->getLastVirtualMemoryUpdSysTime().getVal() - m_pModelMemoryValues->getFirstVirtualMemoryUpdSysTime().getVal();
    }
    if( m_pModelMemoryValues->getMaxValCount() * m_pPCResourceMonitor->getAutoUpdateIntervalMemoryValuesInMs() > fXScaleRange_ms )
    {
        fXScaleRange_ms = m_pModelMemoryValues->getMaxValCount() * m_pPCResourceMonitor->getAutoUpdateIntervalMemoryValuesInMs();
    }

    m_scaleX.m_fMin = -fXScaleRange_ms;
    m_scaleX.m_fMax = 0.0;
    m_pDiagScaleX->setScale(m_scaleX);

    // Y-Scale
    m_scaleY.m_fMax = m_pModelMemory->getVirtualMemorySize().getVal();
    if( m_scaleY.m_fMax < m_pModelMemory->getPhysicalMemorySize().getVal() )
    {
        m_scaleY.m_fMax = m_pModelMemory->getPhysicalMemorySize().getVal();
    }
    m_scaleY.m_pUnit = m_pModelMemory->getPhysicalMemorySize().getUnit();

    int iScaleYMaxSign;
    int iScaleYMaxMant;
    int iScaleYMaxExp;

    ZS::System::Math::normalize(m_scaleY.m_fMax,&iScaleYMaxSign,&iScaleYMaxMant,&iScaleYMaxExp);
    iScaleYMaxMant++;

    if( iScaleYMaxMant > 0 )
    {
        m_scaleY.m_fMax = iScaleYMaxMant * pow(10.0,static_cast<double>(iScaleYMaxExp));
    }
    m_pDiagScaleY->setScale(m_scaleY);

    // Limit lines
    m_pDiagObjLinePhysicalMemorySize->setX1(m_scaleX.m_fMin);
    m_pDiagObjLinePhysicalMemorySize->setY1(m_pModelMemory->getPhysicalMemorySize().getVal());

    m_pDiagObjLinePhysicalMemorySize->setX2(m_scaleX.m_fMax);
    m_pDiagObjLinePhysicalMemorySize->setY2(m_pModelMemory->getPhysicalMemorySize().getVal());

    m_pDiagObjLineVirtualMemorySize->setX1(m_scaleX.m_fMin);
    m_pDiagObjLineVirtualMemorySize->setY1(m_pModelMemory->getVirtualMemorySize().getVal());

    m_pDiagObjLineVirtualMemorySize->setX2(m_scaleX.m_fMax);
    m_pDiagObjLineVirtualMemorySize->setY2(m_pModelMemory->getVirtualMemorySize().getVal());

    // Traces
    double fXScaleOffs_ms = m_pModelMemoryValues->getLastPhysicalMemoryUpdSysTime().getVal();
    if( fXScaleOffs_ms < m_pModelMemoryValues->getLastVirtualMemoryUpdSysTime().getVal() )
    {
        fXScaleOffs_ms = m_pModelMemoryValues->getLastVirtualMemoryUpdSysTime().getVal();
    }

    QVector<double> arfUsedSizesInBytes;
    QVector<double> arfUpdSysTimesInMs;
    double          fUpdSysTimeInMs;
    int             idxVal;

    // Physical Memory
    arfUsedSizesInBytes = m_pModelMemoryValues->getPhysicalMemoryUsedSizesInBytes();
    arfUpdSysTimesInMs = m_pModelMemoryValues->getPhysicalMemoryUpdSysTimesInMs();
    for( idxVal = 0; idxVal < arfUpdSysTimesInMs.size(); idxVal++ )
    {
        fUpdSysTimeInMs = arfUpdSysTimesInMs[idxVal];
        fUpdSysTimeInMs -= fXScaleOffs_ms;
        arfUpdSysTimesInMs[idxVal] = arfUpdSysTimesInMs[idxVal] - fXScaleOffs_ms;
    }
    m_pDiagTracePhysicalMemory->setValues( EScaleDirX, arfUpdSysTimesInMs );
    m_pDiagTracePhysicalMemory->setValues( EScaleDirY, arfUsedSizesInBytes );

    // Virtual Memory
    arfUsedSizesInBytes = m_pModelMemoryValues->getVirtualMemoryUsedSizesInBytes();
    arfUpdSysTimesInMs = m_pModelMemoryValues->getVirtualMemoryUpdSysTimesInMs();
    for( idxVal = 0; idxVal < arfUpdSysTimesInMs.size(); idxVal++ )
    {
        arfUpdSysTimesInMs[idxVal] = arfUpdSysTimesInMs[idxVal] - fXScaleOffs_ms;
    }
    m_pDiagTraceVirtualMemory->setValues( EScaleDirX, arfUpdSysTimesInMs );
    m_pDiagTraceVirtualMemory->setValues( EScaleDirY, arfUsedSizesInBytes );

    // Update diagram
    m_pWdgtDiagram->update();

} // onCltStdPCMemoryValuesChanged
