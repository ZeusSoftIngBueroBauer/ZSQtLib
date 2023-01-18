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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qframe.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmessagebox.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qframe.h>
//#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmessagebox.h>
#endif

#include "ZSDraw/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabelVisibilities.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtFormatGraphObjsLabelVisibilities : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsLabelVisibilities::CWdgtFormatGraphObjsLabelVisibilities(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pWdgtNameLabelVisibilities(nullptr),
    m_pLytWdgtNameLabelVisibilities(nullptr),
    m_pLytHeadLineWdgtNameLabelVisibilities(nullptr),
    m_pLblHeadLineNameLabelVisibilities(nullptr),
    m_pSepHeadLineNameLabelVisibilities(nullptr),
    m_pLytLineNameLabelVisibilities(nullptr),
    m_pLblNameLabel(nullptr),
    m_pCmbNameLabelAnchorSelPt(nullptr),
    m_pChkNameLabelVisible(nullptr),
    m_pLblNameLabelAnchorLineVisible(nullptr),
    m_pChkNameLabelAnchorLineVisible(nullptr),
    m_pLytLinePathLabelVisibilities(nullptr),
    m_pLblPathLabel(nullptr),
    m_pCmbPathLabelAnchorSelPt(nullptr),
    m_pChkPathLabelVisible(nullptr),
    m_pLblPathLabelAnchorLineVisible(nullptr),
    m_pChkPathLabelAnchorLineVisible(nullptr),
    m_pLytLineDescriptionLabelVisibilities(nullptr),
    m_pLblDescriptionLabel(nullptr),
    m_pCmbDescriptionLabelAnchorSelPt(nullptr),
    m_pChkDescriptionLabelVisible(nullptr),
    m_pLblDescriptionLabelAnchorLineVisible(nullptr),
    m_pChkDescriptionLabelAnchorLineVisible(nullptr),
    m_pWdgtDimensionLabelVisibilities(nullptr),
    m_pLytWdgtDimensionLabelVisibilities(nullptr),
    m_pLytHeadLineWdgtDimensionLabelVisibilities(nullptr),
    m_pLblHeadLineDimensionLabelVisibilities(nullptr),
    m_pSepHeadLineDimensionLabelVisibilities(nullptr),
    m_pLytLineDimensionPosLabelVisibilities(nullptr),
    m_pLblDimensionPosLabel(nullptr),
    m_pCmbDimensionPosLabelAnchorSelPt(nullptr),
    m_pChkDimensionPosLabelVisible(nullptr),
    m_pLblDimensionPosLabelAnchorLineVisible(nullptr),
    m_pChkDimensionPosLabelAnchorLineVisible(nullptr),
    m_pLytLineDimensionWidthLabelVisibilities(nullptr),
    m_pLblDimensionWidthLabel(nullptr),
    m_pCmbDimensionWidthLabelAnchorSelPt(nullptr),
    m_pChkDimensionWidthLabelVisible(nullptr),
    m_pLblDimensionWidthLabelAnchorLineVisible(nullptr),
    m_pChkDimensionWidthLabelAnchorLineVisible(nullptr),
    m_pLytLineDimensionHeightLabelVisibilities(nullptr),
    m_pLblDimensionHeightLabel(nullptr),
    m_pCmbDimensionHeightLabelAnchorSelPt(nullptr),
    m_pChkDimensionHeightLabelVisible(nullptr),
    m_pLblDimensionHeightLabelAnchorLineVisible(nullptr),
    m_pChkDimensionHeightLabelAnchorLineVisible(nullptr),
    m_pLytLineDimensionRotationAngleLabelVisible(nullptr),
    m_pLblDimensionRotationAngleLabel(nullptr),
    m_pCmbDimensionRotationAngleLabelAnchorSelPt(nullptr),
    m_pChkDimensionRotationAngleLabelVisible(nullptr),
    m_pLblDimensionRotationAngleLabelAnchorLineVisible(nullptr),
    m_pChkDimensionRotationAngleLabelAnchorLineVisible(nullptr),
    m_pWdgtDimensionLinesVisibilities(nullptr),
    m_pLytWdgtDimensionLinesVisibilities(nullptr),
    m_pLytHeadLineWdgtDimensionLinesVisibilities(nullptr),
    m_pLblHeadLineDimensionLinesVisibilities(nullptr),
    m_pSepHeadLineDimensionLinesVisibilities(nullptr),
    m_pLytLineDimensionLinesBoundingRectVisibile(nullptr),
    m_pLblDimensionLinesBoundingRectVisible(nullptr),
    m_pChkDimensionLinesBoundingRectVisible(nullptr),
    m_pLytLineDimensionLinesBoundingRectDiagonalsVisible(nullptr),
    m_pLblDimensionLinesBoundingRectDiagonalsVisible(nullptr),
    m_pChkDimensionLinesBoundingRectDiagonalsVisible(nullptr),
    m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible(nullptr),
    m_pLblDimensionLinesBoundingRectHorizontalSymmetryAxisVisible(nullptr),
    m_pChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisible(nullptr),
    m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible(nullptr),
    m_pLblDimensionLinesBoundingRectVerticalSymmetryAxisVisible(nullptr),
    m_pChkDimensionLinesBoundingRectVerticalSymmetryAxisVisible(nullptr),
   // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsGeometry");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr");
    }

    QRectF rctScene = m_pDrawingScene->sceneRect();

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidthClm0 = 60;
    int cxLblWidthClm1 = 40;
    int cxLblWidthClm2 = 40;
    int cxLblWidthClm3 = 20;
    int cxEdtWidthClm1 = 80;
    int cxEdtWidthClm2 = 80;
    int cxSpacingClm1Cml2 = 10;
    int cxSpacingClm2Cml3 = 10;

    EGraphObjType graphObjType = m_pGraphObj->type();

    // Headline
    //=========

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Labels");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Widget> Name
    //==============

    m_pWdgtNameLabelVisibilities = new QWidget();
    m_pLytWdgtNameLabelVisibilities = new QVBoxLayout();
    m_pLytWdgtNameLabelVisibilities->setContentsMargins(0,0,0,0);
    m_pWdgtNameLabelVisibilities->setLayout(m_pLytWdgtNameLabelVisibilities);
    m_pLyt->addWidget(m_pWdgtNameLabelVisibilities);

    // <Separator> Name Labels
    //------------------------

    m_pLytWdgtNameLabelVisibilities->addSpacing(4);

    m_pLytHeadLineWdgtNameLabelVisibilities = new QHBoxLayout();
    m_pLytWdgtNameLabelVisibilities->addLayout(m_pLytHeadLineWdgtNameLabelVisibilities);

    m_pLblHeadLineNameLabelVisibilities = new QLabel("Name Labels:");
    m_pLytHeadLineWdgtNameLabelVisibilities->addWidget(m_pLblHeadLineNameLabelVisibilities);
    m_pSepHeadLineNameLabelVisibilities = new QFrame();
    m_pSepHeadLineNameLabelVisibilities->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytHeadLineWdgtNameLabelVisibilities->addWidget(m_pSepHeadLineNameLabelVisibilities, 1);

    m_pLytWdgtNameLabelVisibilities->addSpacing(4);

    // <Line> Name Visiblities
    //------------------------

    m_pLytLineNameLabelVisibilities = new QHBoxLayout();
    m_pLytWdgtNameLabelVisibilities->addLayout(m_pLytLineNameLabelVisibilities);
    m_pLblNameLabel = new QLabel("Name:");
    m_pLblNameLabel->setFixedWidth(cxLblWidthClm0);
    m_pLytLineNameLabelVisibilities->addWidget(m_pLblNameLabel);

    // <ComboBox> Anchor Selection Point
    //----------------------------------

    m_pCmbNameLabelAnchorSelPt = new QComboBox();
    m_pCmbNameLabelAnchorSelPt->setObjectName("Name"); // needed in "updateVisibilityCheckStates"
    fillComboAnchorSelPt(m_pCmbNameLabelAnchorSelPt);
    m_pCmbNameLabelAnchorSelPt->setFixedWidth(cxEdtWidthClm1);
    m_pLytLineNameLabelVisibilities->addWidget(m_pCmbNameLabelAnchorSelPt);
    m_pLytLineNameLabelVisibilities->addSpacing(cxSpacingClm1Cml2);

    if( !connect(
        /* pObjSender   */ m_pCmbNameLabelAnchorSelPt,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbNameLabelAnchorSelPtCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Name
    //---------------------

    m_pChkNameLabelVisible = new QCheckBox();
    m_pLytLineNameLabelVisibilities->addWidget(m_pChkNameLabelVisible);
    m_pLytLineNameLabelVisibilities->addSpacing(cxSpacingClm2Cml3);

    if( !connect(
        /* pObjSender   */ m_pChkNameLabelVisible,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkNameLabelVisibleStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Anchor Line
    //----------------------------

    m_pLblNameLabelAnchorLineVisible = new QLabel("Line:");
    m_pLblNameLabelAnchorLineVisible->setFixedWidth(cxLblWidthClm3);
    m_pLytLineNameLabelVisibilities->addWidget(m_pLblNameLabelAnchorLineVisible);
    m_pChkNameLabelAnchorLineVisible = new QCheckBox();
    m_pChkNameLabelAnchorLineVisible->setTristate(true);
    m_pLytLineNameLabelVisibilities->addWidget(m_pChkNameLabelAnchorLineVisible);
    m_pLytLineNameLabelVisibilities->addStretch();

    if( !connect(
        /* pObjSender   */ m_pChkNameLabelAnchorLineVisible,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkNameLabelAnchorLineVisibleStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Line> Path Visiblities
    //------------------------

    m_pLytLinePathLabelVisibilities = new QHBoxLayout();
    m_pLytWdgtNameLabelVisibilities->addLayout(m_pLytLinePathLabelVisibilities);
    m_pLblPathLabel = new QLabel("Path:");
    m_pLblPathLabel->setFixedWidth(cxLblWidthClm0);
    m_pLytLinePathLabelVisibilities->addWidget(m_pLblPathLabel);

    // <ComboBox> Anchor Selection Point
    //----------------------------------

    m_pCmbPathLabelAnchorSelPt = new QComboBox();
    m_pCmbPathLabelAnchorSelPt->setObjectName("Path"); // needed in "updateVisibilityCheckStates"
    fillComboAnchorSelPt(m_pCmbPathLabelAnchorSelPt);
    m_pCmbPathLabelAnchorSelPt->setFixedWidth(cxEdtWidthClm1);
    m_pLytLinePathLabelVisibilities->addWidget(m_pCmbPathLabelAnchorSelPt);
    m_pLytLinePathLabelVisibilities->addSpacing(cxSpacingClm1Cml2);

    if( !connect(
        /* pObjSender   */ m_pCmbPathLabelAnchorSelPt,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbPathLabelAnchorSelPtCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Path
    //---------------------

    m_pChkPathLabelVisible = new QCheckBox();
    m_pLytLinePathLabelVisibilities->addWidget(m_pChkPathLabelVisible);
    m_pLytLinePathLabelVisibilities->addSpacing(cxSpacingClm2Cml3);

    if( !connect(
        /* pObjSender   */ m_pChkPathLabelVisible,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkPathLabelVisibleStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Anchor Line
    //----------------------------

    m_pLblPathLabelAnchorLineVisible = new QLabel("Line:");
    m_pLblPathLabelAnchorLineVisible->setFixedWidth(cxLblWidthClm3);
    m_pLytLinePathLabelVisibilities->addWidget(m_pLblPathLabelAnchorLineVisible);
    m_pChkPathLabelAnchorLineVisible = new QCheckBox();
    m_pChkPathLabelAnchorLineVisible->setTristate(true);
    m_pLytLinePathLabelVisibilities->addWidget(m_pChkPathLabelAnchorLineVisible);
    m_pLytLinePathLabelVisibilities->addStretch();

    if( !connect(
        /* pObjSender   */ m_pChkPathLabelAnchorLineVisible,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkPathLabelAnchorLineVisibleStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Line> User Defined Visiblities
    //--------------------------------

    m_pLytLineDescriptionLabelVisibilities = new QHBoxLayout();
    m_pLytWdgtNameLabelVisibilities->addLayout(m_pLytLineDescriptionLabelVisibilities);
    m_pLblDescriptionLabel = new QLabel("Description:");
    m_pLblDescriptionLabel->setFixedWidth(cxLblWidthClm0);
    m_pLytLineDescriptionLabelVisibilities->addWidget(m_pLblDescriptionLabel);

    // <ComboBox> Anchor Selection Point
    //----------------------------------

    m_pCmbDescriptionLabelAnchorSelPt = new QComboBox();
    m_pCmbDescriptionLabelAnchorSelPt->setObjectName("Description"); // needed in "updateVisibilityCheckStates"
    fillComboAnchorSelPt(m_pCmbDescriptionLabelAnchorSelPt);
    m_pCmbDescriptionLabelAnchorSelPt->setFixedWidth(cxEdtWidthClm1);
    m_pLytLineDescriptionLabelVisibilities->addWidget(m_pCmbDescriptionLabelAnchorSelPt);
    m_pLytLineDescriptionLabelVisibilities->addSpacing(cxSpacingClm1Cml2);

    if( !connect(
        /* pObjSender   */ m_pCmbDescriptionLabelAnchorSelPt,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDescriptionLabelAnchorSelPtCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show User Defined
    //-----------------------------

    m_pChkDescriptionLabelVisible = new QCheckBox();
    m_pLytLineDescriptionLabelVisibilities->addWidget(m_pChkDescriptionLabelVisible);
    m_pLytLineDescriptionLabelVisibilities->addSpacing(cxSpacingClm2Cml3);

    if( !connect(
        /* pObjSender   */ m_pChkDescriptionLabelVisible,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDescriptionLabelVisibleStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Anchor Line
    //----------------------------

    m_pLblDescriptionLabelAnchorLineVisible = new QLabel("Line:");
    m_pLblDescriptionLabelAnchorLineVisible->setFixedWidth(cxLblWidthClm3);
    m_pLytLineDescriptionLabelVisibilities->addWidget(m_pLblDescriptionLabelAnchorLineVisible);
    m_pChkDescriptionLabelAnchorLineVisible = new QCheckBox();
    m_pChkDescriptionLabelAnchorLineVisible->setTristate(true);
    m_pLytLineDescriptionLabelVisibilities->addWidget(m_pChkDescriptionLabelAnchorLineVisible);
    m_pLytLineDescriptionLabelVisibilities->addStretch();

    if( !connect(
        /* pObjSender   */ m_pChkDescriptionLabelAnchorLineVisible,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDescriptionLabelAnchorLineVisibleStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Widget> Dimension Labels
    //==========================

    //cxLblWidthClm0 = 104;
    //cxLblWidthClm1 = 0;
    //cxLblWidthClm2 = 0;
    //cxEdtWidthClm2 = 20;
    //cxSpacingClm1Cml2 = 20;

    m_pWdgtDimensionLabelVisibilities = new QWidget();
    m_pLytWdgtDimensionLabelVisibilities = new QVBoxLayout();
    m_pLytWdgtDimensionLabelVisibilities->setContentsMargins(0,0,0,0);
    m_pWdgtDimensionLabelVisibilities->setLayout(m_pLytWdgtDimensionLabelVisibilities);
    m_pLyt->addWidget(m_pWdgtDimensionLabelVisibilities);

    // <Separator> Dimension Labels
    //-----------------------------

    m_pLytWdgtDimensionLabelVisibilities->addSpacing(4);

    m_pLytHeadLineWdgtDimensionLabelVisibilities = new QHBoxLayout();
    m_pLytWdgtDimensionLabelVisibilities->addLayout(m_pLytHeadLineWdgtDimensionLabelVisibilities);

    m_pLblHeadLineDimensionLabelVisibilities = new QLabel("Dimension Labels:");
    m_pLytHeadLineWdgtDimensionLabelVisibilities->addWidget(m_pLblHeadLineDimensionLabelVisibilities);
    m_pSepHeadLineDimensionLabelVisibilities = new QFrame();
    m_pSepHeadLineDimensionLabelVisibilities->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytHeadLineWdgtDimensionLabelVisibilities->addWidget(m_pSepHeadLineDimensionLabelVisibilities, 1);

    m_pLytWdgtDimensionLabelVisibilities->addSpacing(4);

    // <Line> Position Visiblities
    //----------------------------

    m_pLytLineDimensionPosLabelVisibilities = new QHBoxLayout();
    m_pLytWdgtDimensionLabelVisibilities->addLayout(m_pLytLineDimensionPosLabelVisibilities);
    m_pLblDimensionPosLabel = new QLabel("Position:");
    m_pLblDimensionPosLabel->setFixedWidth(cxLblWidthClm0);
    m_pLytLineDimensionPosLabelVisibilities->addWidget(m_pLblDimensionPosLabel);

    // <ComboBox> Anchor Selection Point
    //----------------------------------

    m_pCmbDimensionPosLabelAnchorSelPt = new QComboBox();
    m_pCmbDimensionPosLabelAnchorSelPt->setObjectName("DimensionPos"); // needed in "updateVisibilityCheckStates"
    fillComboAnchorSelPt(m_pCmbDimensionPosLabelAnchorSelPt);
    m_pCmbDimensionPosLabelAnchorSelPt->setFixedWidth(cxEdtWidthClm1);
    m_pLytLineDimensionPosLabelVisibilities->addWidget(m_pCmbDimensionPosLabelAnchorSelPt);
    m_pLytLineDimensionPosLabelVisibilities->addSpacing(cxSpacingClm1Cml2);

    if( !connect(
        /* pObjSender   */ m_pCmbDimensionPosLabelAnchorSelPt,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDimensionPosLabelAnchorSelPtCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Coors
    //----------------------

    m_pChkDimensionPosLabelVisible = new QCheckBox();
    m_pLytLineDimensionPosLabelVisibilities->addWidget(m_pChkDimensionPosLabelVisible);
    m_pLytLineDimensionPosLabelVisibilities->addSpacing(cxSpacingClm2Cml3);

    if( !connect(
        /* pObjSender   */ m_pChkDimensionPosLabelVisible,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDimensionPosLabelVisibleStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Anchor Line
    //----------------------------

    m_pLblDimensionPosLabelAnchorLineVisible = new QLabel("Line:");
    m_pLblDimensionPosLabelAnchorLineVisible->setFixedWidth(cxLblWidthClm3);
    m_pLytLineDimensionPosLabelVisibilities->addWidget(m_pLblDimensionPosLabelAnchorLineVisible);
    m_pChkDimensionPosLabelAnchorLineVisible = new QCheckBox();
    m_pChkDimensionPosLabelAnchorLineVisible->setTristate(true);
    m_pLytLineDimensionPosLabelVisibilities->addWidget(m_pChkDimensionPosLabelAnchorLineVisible);
    m_pLytLineDimensionPosLabelVisibilities->addStretch();

    if( !connect(
        /* pObjSender   */ m_pChkDimensionPosLabelAnchorLineVisible,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDimensionPosLabelAnchorLineVisibleStateChanged(int)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Line> Width Visibilities
    //--------------------------

    if( graphObjType != EGraphObjTypePoint )
    {
        m_pLytLineDimensionWidthLabelVisibilities = new QHBoxLayout();
        m_pLytWdgtDimensionLabelVisibilities->addLayout(m_pLytLineDimensionWidthLabelVisibilities);
        m_pLblDimensionWidthLabel = new QLabel("Width:");
        m_pLblDimensionWidthLabel->setFixedWidth(cxLblWidthClm0);
        m_pLytLineDimensionWidthLabelVisibilities->addWidget(m_pLblDimensionWidthLabel);

        // <ComboBox> Anchor Selection Point
        //----------------------------------

        m_pCmbDimensionWidthLabelAnchorSelPt = new QComboBox();
        m_pCmbDimensionWidthLabelAnchorSelPt->setObjectName("DimensionWidth"); // needed in "updateVisibilityCheckStates"
        fillComboAnchorSelPt(m_pCmbDimensionWidthLabelAnchorSelPt);
        m_pCmbDimensionWidthLabelAnchorSelPt->setFixedWidth(cxEdtWidthClm1);
        m_pLytLineDimensionWidthLabelVisibilities->addWidget(m_pCmbDimensionWidthLabelAnchorSelPt);
        m_pLytLineDimensionWidthLabelVisibilities->addSpacing(cxSpacingClm1Cml2);

        if( !connect(
            /* pObjSender   */ m_pCmbDimensionWidthLabelAnchorSelPt,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbDimensionWidthLabelAnchorSelPtCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <CheckBox> Show Width
        //----------------------

        m_pChkDimensionWidthLabelVisible = new QCheckBox("");
        m_pLytLineDimensionWidthLabelVisibilities->addWidget(m_pChkDimensionWidthLabelVisible);
        m_pLytLineDimensionWidthLabelVisibilities->addSpacing(cxSpacingClm2Cml3);

        if( !connect(
            /* pObjSender   */ m_pChkDimensionWidthLabelVisible,
            /* szSignal     */ SIGNAL(stateChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDimensionWidthLabelVisibleStateChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <CheckBox> Show Anchor Line
        //----------------------------

        m_pLblDimensionWidthLabelAnchorLineVisible = new QLabel("Line:");
        m_pLblDimensionWidthLabelAnchorLineVisible->setFixedWidth(cxLblWidthClm3);
        m_pLytLineDimensionWidthLabelVisibilities->addWidget(m_pLblDimensionWidthLabelAnchorLineVisible);
        m_pChkDimensionWidthLabelAnchorLineVisible = new QCheckBox();
        m_pLytLineDimensionWidthLabelVisibilities->addWidget(m_pChkDimensionWidthLabelAnchorLineVisible);
        m_pLytLineDimensionWidthLabelVisibilities->addStretch();

    } // if( graphObjType != EGraphObjTypePoint )

    // <Line> Height Visiblilities
    //----------------------------

    if( graphObjType != EGraphObjTypePoint )
    {
        m_pLytLineDimensionHeightLabelVisibilities = new QHBoxLayout();
        m_pLytWdgtDimensionLabelVisibilities->addLayout(m_pLytLineDimensionHeightLabelVisibilities);
        m_pLblDimensionHeightLabel = new QLabel("Height:");
        m_pLblDimensionHeightLabel->setFixedWidth(cxLblWidthClm0);
        m_pLytLineDimensionHeightLabelVisibilities->addWidget(m_pLblDimensionHeightLabel);

        // <ComboBox> Anchor Selection Point
        //----------------------------------

        m_pCmbDimensionHeightLabelAnchorSelPt = new QComboBox();
        m_pCmbDimensionHeightLabelAnchorSelPt->setObjectName("DimensionHeight"); // needed in "updateVisibilityCheckStates"
        fillComboAnchorSelPt(m_pCmbDimensionHeightLabelAnchorSelPt);
        m_pCmbDimensionHeightLabelAnchorSelPt->setFixedWidth(cxEdtWidthClm1);
        m_pLytLineDimensionHeightLabelVisibilities->addWidget(m_pCmbDimensionHeightLabelAnchorSelPt);
        m_pLytLineDimensionHeightLabelVisibilities->addSpacing(cxSpacingClm1Cml2);

        if( !connect(
            /* pObjSender   */ m_pCmbDimensionHeightLabelAnchorSelPt,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbDimensionHeightLabelAnchorSelPtCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <CheckBox> Show Height
        //-------------------------

        m_pChkDimensionHeightLabelVisible = new QCheckBox();
        m_pLytLineDimensionHeightLabelVisibilities->addWidget(m_pChkDimensionHeightLabelVisible);
        m_pLytLineDimensionHeightLabelVisibilities->addSpacing(cxSpacingClm2Cml3);

        if( !connect(
            /* pObjSender   */ m_pChkDimensionHeightLabelVisible,
            /* szSignal     */ SIGNAL(stateChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDimensionHeightLabelVisibleStateChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <CheckBox> Show Anchor Line
        //----------------------------

        m_pLblDimensionHeightLabelAnchorLineVisible = new QLabel("Line:");
        m_pLblDimensionHeightLabelAnchorLineVisible->setFixedWidth(cxLblWidthClm3);
        m_pLytLineDimensionHeightLabelVisibilities->addWidget(m_pLblDimensionHeightLabelAnchorLineVisible);
        m_pChkDimensionHeightLabelAnchorLineVisible = new QCheckBox();
        m_pLytLineDimensionHeightLabelVisibilities->addWidget(m_pChkDimensionHeightLabelAnchorLineVisible);
        m_pLytLineDimensionHeightLabelVisibilities->addStretch();

    } // if( graphObjType != EGraphObjTypePoint )

    // <Line> Rotation Angle Visibilities
    //-----------------------------------

    if( graphObjType != EGraphObjTypePoint )
    {
        m_pLytLineDimensionRotationAngleLabelVisible = new QHBoxLayout();
        m_pLytWdgtDimensionLabelVisibilities->addLayout(m_pLytLineDimensionRotationAngleLabelVisible);
        m_pLblDimensionRotationAngleLabel = new QLabel("Rotation Angle:");
        m_pLblDimensionRotationAngleLabel->setFixedWidth(cxLblWidthClm0);
        m_pLytLineDimensionRotationAngleLabelVisible->addWidget(m_pLblDimensionRotationAngleLabel);

        // <ComboBox> Anchor Selection Point
        //----------------------------------

        m_pCmbDimensionRotationAngleLabelAnchorSelPt = new QComboBox();
        m_pCmbDimensionRotationAngleLabelAnchorSelPt->setObjectName("DimensionRotationAngle"); // needed in "updateVisibilityCheckStates"
        fillComboAnchorSelPt(m_pCmbDimensionRotationAngleLabelAnchorSelPt);
        m_pCmbDimensionRotationAngleLabelAnchorSelPt->setFixedWidth(cxEdtWidthClm1);
        m_pLytLineDimensionRotationAngleLabelVisible->addWidget(m_pCmbDimensionRotationAngleLabelAnchorSelPt);
        m_pLytLineDimensionRotationAngleLabelVisible->addSpacing(cxSpacingClm1Cml2);

        if( !connect(
            /* pObjSender   */ m_pCmbDimensionRotationAngleLabelAnchorSelPt,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbDimensionRotationAngleLabelAnchorSelPtCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <CheckBox> Show Rotation Angle
        //-------------------------------

        m_pChkDimensionRotationAngleLabelVisible = new QCheckBox();
        m_pLytLineDimensionRotationAngleLabelVisible->addWidget(m_pChkDimensionRotationAngleLabelVisible);
        m_pLytLineDimensionRotationAngleLabelVisible->addSpacing(cxSpacingClm2Cml3);

        if( !connect(
            /* pObjSender   */ m_pChkDimensionRotationAngleLabelVisible,
            /* szSignal     */ SIGNAL(stateChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDimensionRotationAngleLabelVisibleStateChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <CheckBox> Show Anchor Line
        //----------------------------

        m_pLblDimensionRotationAngleLabelAnchorLineVisible = new QLabel("Line:");
        m_pLblDimensionRotationAngleLabelAnchorLineVisible->setFixedWidth(cxLblWidthClm3);
        m_pLytLineDimensionRotationAngleLabelVisible->addWidget(m_pLblDimensionRotationAngleLabelAnchorLineVisible);
        m_pChkDimensionRotationAngleLabelAnchorLineVisible = new QCheckBox();
        m_pLytLineDimensionRotationAngleLabelVisible->addWidget(m_pChkDimensionRotationAngleLabelAnchorLineVisible);
        m_pLytLineDimensionRotationAngleLabelVisible->addStretch();

    } // if( graphObjType != EGraphObjTypePoint )

    // <Widget> Dimension Lines
    //=========================

    if( m_pGraphObj->hasBoundingRect() )
    {
        //cxLblWidthClm2 = 120;

        m_pWdgtDimensionLinesVisibilities = new QWidget();
        m_pLytWdgtDimensionLinesVisibilities = new QVBoxLayout();
        m_pLytWdgtDimensionLinesVisibilities->setContentsMargins(0,0,0,0);
        m_pWdgtDimensionLinesVisibilities->setLayout(m_pLytWdgtDimensionLinesVisibilities);
        m_pLyt->addWidget(m_pWdgtDimensionLinesVisibilities);

        // <Separator> Dimension Lines
        //----------------------------

        m_pLytWdgtDimensionLinesVisibilities->addSpacing(4);

        m_pLytHeadLineWdgtDimensionLinesVisibilities = new QHBoxLayout();
        m_pLytWdgtDimensionLinesVisibilities->addLayout(m_pLytHeadLineWdgtDimensionLinesVisibilities);

        m_pLblHeadLineDimensionLinesVisibilities = new QLabel("Dimension Lines:");
        m_pLytHeadLineWdgtDimensionLinesVisibilities->addWidget(m_pLblHeadLineDimensionLinesVisibilities);
        m_pSepHeadLineDimensionLinesVisibilities = new QFrame();
        m_pSepHeadLineDimensionLinesVisibilities->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytHeadLineWdgtDimensionLinesVisibilities->addWidget(m_pSepHeadLineDimensionLinesVisibilities, 1);

        m_pLytWdgtDimensionLinesVisibilities->addSpacing(4);

        // <Line> Bounding Rectangle
        //--------------------------

        m_pLytLineDimensionLinesBoundingRectVisibile = new QHBoxLayout();
        m_pLytWdgtDimensionLinesVisibilities->addLayout(m_pLytLineDimensionLinesBoundingRectVisibile);
        m_pLblDimensionLinesBoundingRectVisible = new QLabel("Bounding Rectangle:");
        m_pLblDimensionLinesBoundingRectVisible->setFixedWidth(cxLblWidthClm0);
        m_pLytLineDimensionLinesBoundingRectVisibile->addWidget(m_pLblDimensionLinesBoundingRectVisible);

        // <CheckBox> Visibile
        //--------------------

        m_pChkDimensionLinesBoundingRectVisible = new QCheckBox();
        m_pLytLineDimensionLinesBoundingRectVisibile->addWidget(m_pChkDimensionLinesBoundingRectVisible);
        m_pLytLineDimensionLinesBoundingRectVisibile->addStretch();

        if( !connect(
            /* pObjSender   */ m_pChkDimensionLinesBoundingRectVisible,
            /* szSignal     */ SIGNAL(stateChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDimensionLinesBoundingRectVisibleStateChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <Line> Bounding Rectangle Diagonals
        //------------------------------------

        m_pLytLineDimensionLinesBoundingRectDiagonalsVisible = new QHBoxLayout();
        m_pLytWdgtDimensionLinesVisibilities->addLayout(m_pLytLineDimensionLinesBoundingRectDiagonalsVisible);
        m_pLblDimensionLinesBoundingRectDiagonalsVisible = new QLabel("Rectangle Diagonals:");
        m_pLblDimensionLinesBoundingRectDiagonalsVisible->setFixedWidth(cxLblWidthClm0);
        m_pLytLineDimensionLinesBoundingRectDiagonalsVisible->addWidget(m_pLblDimensionLinesBoundingRectDiagonalsVisible);

        // <CheckBox> Visibile
        //--------------------

        m_pChkDimensionLinesBoundingRectDiagonalsVisible = new QCheckBox();
        m_pLytLineDimensionLinesBoundingRectDiagonalsVisible->addWidget(m_pChkDimensionLinesBoundingRectDiagonalsVisible);
        m_pLytLineDimensionLinesBoundingRectDiagonalsVisible->addStretch();

        if( !connect(
            /* pObjSender   */ m_pChkDimensionLinesBoundingRectDiagonalsVisible,
            /* szSignal     */ SIGNAL(stateChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDimensionLinesBoundingRectDiagonalsVisibleStateChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <Line> Bounding Rectangle Horizontal Symmetry Axis
        //---------------------------------------------------

        m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible = new QHBoxLayout();
        m_pLytWdgtDimensionLinesVisibilities->addLayout(m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible);
        m_pLblDimensionLinesBoundingRectHorizontalSymmetryAxisVisible = new QLabel("Horizontal Axis:");
        m_pLblDimensionLinesBoundingRectHorizontalSymmetryAxisVisible->setFixedWidth(cxLblWidthClm0);
        m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible->addWidget(m_pLblDimensionLinesBoundingRectHorizontalSymmetryAxisVisible);

        // <CheckBox> Visibile
        //--------------------

        m_pChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisible = new QCheckBox();
        m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible->addWidget(m_pChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisible);
        m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible->addStretch();

        if( !connect(
            /* pObjSender   */ m_pChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisible,
            /* szSignal     */ SIGNAL(stateChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisibleStateChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <Line> Bounding Rectangle Vertical Symmetry Axis
        //---------------------------------------------------

        m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible = new QHBoxLayout();
        m_pLytWdgtDimensionLinesVisibilities->addLayout(m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible);
        m_pLblDimensionLinesBoundingRectVerticalSymmetryAxisVisible = new QLabel("Vertical Axis:");
        m_pLblDimensionLinesBoundingRectVerticalSymmetryAxisVisible->setFixedWidth(cxLblWidthClm0);
        m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible->addWidget(m_pLblDimensionLinesBoundingRectVerticalSymmetryAxisVisible);

        // <CheckBox> Visibile
        //--------------------

        m_pChkDimensionLinesBoundingRectVerticalSymmetryAxisVisible = new QCheckBox();
        m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible->addWidget(m_pChkDimensionLinesBoundingRectVerticalSymmetryAxisVisible);
        m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible->addStretch();

        if( !connect(
            /* pObjSender   */ m_pChkDimensionLinesBoundingRectVerticalSymmetryAxisVisible,
            /* szSignal     */ SIGNAL(stateChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDimensionLinesBoundingRectVerticalSymmetryAxisVisibleStateChanged(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    } // if( m_pGraphObj->hasBoundingRect() )

    // Set settings at GUI controls
    //=============================

    if( m_pGraphObj != nullptr )
    {
        onGraphObjChanged();
    }

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsLabelVisibilities::~CWdgtFormatGraphObjsLabelVisibilities()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pWdgtNameLabelVisibilities = nullptr;
    m_pLytWdgtNameLabelVisibilities = nullptr;
    m_pLytHeadLineWdgtNameLabelVisibilities = nullptr;
    m_pLblHeadLineNameLabelVisibilities = nullptr;
    m_pSepHeadLineNameLabelVisibilities = nullptr;
    m_pLytLineNameLabelVisibilities = nullptr;
    m_pLblNameLabel = nullptr;
    m_pCmbNameLabelAnchorSelPt = nullptr;
    m_pChkNameLabelVisible = nullptr;
    m_pLblNameLabelAnchorLineVisible = nullptr;
    m_pChkNameLabelAnchorLineVisible = nullptr;
    m_pLytLinePathLabelVisibilities = nullptr;
    m_pLblPathLabel = nullptr;
    m_pCmbPathLabelAnchorSelPt = nullptr;
    m_pChkPathLabelVisible = nullptr;
    m_pLblPathLabelAnchorLineVisible = nullptr;
    m_pChkPathLabelAnchorLineVisible = nullptr;
    m_pLytLineDescriptionLabelVisibilities = nullptr;
    m_pLblDescriptionLabel = nullptr;
    m_pCmbDescriptionLabelAnchorSelPt = nullptr;
    m_pChkDescriptionLabelVisible = nullptr;
    m_pLblDescriptionLabelAnchorLineVisible = nullptr;
    m_pChkDescriptionLabelAnchorLineVisible = nullptr;
    m_pWdgtDimensionLabelVisibilities = nullptr;
    m_pLytWdgtDimensionLabelVisibilities = nullptr;
    m_pLytHeadLineWdgtDimensionLabelVisibilities = nullptr;
    m_pLblHeadLineDimensionLabelVisibilities = nullptr;
    m_pSepHeadLineDimensionLabelVisibilities = nullptr;
    m_pLytLineDimensionPosLabelVisibilities = nullptr;
    m_pLblDimensionPosLabel = nullptr;
    m_pCmbDimensionPosLabelAnchorSelPt = nullptr;
    m_pLblDimensionPosLabel = nullptr;
    m_pChkDimensionPosLabelVisible = nullptr;
    m_pLblDimensionPosLabelAnchorLineVisible = nullptr;
    m_pChkDimensionPosLabelAnchorLineVisible = nullptr;
    m_pLytLineDimensionWidthLabelVisibilities = nullptr;
    m_pLblDimensionWidthLabel = nullptr;
    m_pCmbDimensionWidthLabelAnchorSelPt = nullptr;
    m_pLblDimensionWidthLabel = nullptr;
    m_pChkDimensionWidthLabelVisible = nullptr;
    m_pLblDimensionWidthLabelAnchorLineVisible = nullptr;
    m_pChkDimensionWidthLabelAnchorLineVisible = nullptr;
    m_pLytLineDimensionHeightLabelVisibilities = nullptr;
    m_pLblDimensionHeightLabel = nullptr;
    m_pCmbDimensionHeightLabelAnchorSelPt = nullptr;
    m_pChkDimensionHeightLabelVisible = nullptr;
    m_pLblDimensionHeightLabelAnchorLineVisible = nullptr;
    m_pChkDimensionHeightLabelAnchorLineVisible = nullptr;
    m_pLytLineDimensionRotationAngleLabelVisible = nullptr;
    m_pLblDimensionRotationAngleLabel = nullptr;
    m_pCmbDimensionRotationAngleLabelAnchorSelPt = nullptr;
    m_pChkDimensionRotationAngleLabelVisible = nullptr;
    m_pLblDimensionRotationAngleLabelAnchorLineVisible = nullptr;
    m_pChkDimensionRotationAngleLabelAnchorLineVisible = nullptr;
    m_pWdgtDimensionLinesVisibilities = nullptr;
    m_pLytWdgtDimensionLinesVisibilities = nullptr;
    m_pLytHeadLineWdgtDimensionLinesVisibilities = nullptr;
    m_pLblHeadLineDimensionLinesVisibilities = nullptr;
    m_pSepHeadLineDimensionLinesVisibilities = nullptr;
    m_pLytLineDimensionLinesBoundingRectVisibile = nullptr;
    m_pLblDimensionLinesBoundingRectVisible = nullptr;
    m_pChkDimensionLinesBoundingRectVisible = nullptr;
    m_pLytLineDimensionLinesBoundingRectDiagonalsVisible = nullptr;
    m_pLblDimensionLinesBoundingRectDiagonalsVisible = nullptr;
    m_pChkDimensionLinesBoundingRectDiagonalsVisible = nullptr;
    m_pLytLineDimensionLinesBoundingRectHorizontalSymmetryAxisVisible = nullptr;
    m_pLblDimensionLinesBoundingRectHorizontalSymmetryAxisVisible = nullptr;
    m_pChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisible = nullptr;
    m_pLytLineDimensionLinesBoundingRectVerticalSymmetryAxisVisible = nullptr;
    m_pLblDimensionLinesBoundingRectVerticalSymmetryAxisVisible = nullptr;
    m_pChkDimensionLinesBoundingRectVerticalSymmetryAxisVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::applyChanges()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "applyChanges",
        /* strAddInfo   */ strMthInArgs );

    // On changing a setting of the graphical object "onGraphObjChanged" will be called as reentry.
    // "onGraphObjChanged" will modify the states of the GUI controls. E.g. if the "NameLabelVisible" and
    // "NameLabelAnchorLineVisible" CheckBoxes are checked and you call "showLabel" "onGraphObjChanged"
    // is immediately called resetting the check state of "NameLabelAnchorLineVisible".
    // To avoid this undesired correction the "onGraphObjChanged" method must be disabled while applying
    // changes but will be called after all changes have been applied. For this the flag "m_bApplyingChanges"
    // of the base class "CWdgtFormatGraphObjs" is set at the beginning, reset at the end of "applyChanges"
    // and afterwards "onGraphObjChanged" is explicitly called.

    m_bApplyingChanges = true;

    // Label Visibilities
    //===================

    if( m_pCmbNameLabelAnchorSelPt != nullptr && m_pChkNameLabelVisible != nullptr && m_pChkNameLabelAnchorLineVisible != nullptr )
    {
        QString strAnchorSelPt = m_pCmbNameLabelAnchorSelPt->currentText();
        bool bConverted = false;
        CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

        if( bConverted )
        {
            if( m_pChkNameLabelVisible->isChecked() )
            {
                if( !m_pGraphObj->isNameLabelVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->showNameLabel(eSelPt.enumerator());
                }
            }
            else
            {
                if( m_pGraphObj->isNameLabelVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->hideNameLabel();
                }
            }
            if( m_pChkNameLabelAnchorLineVisible->isChecked() )
            {
                if( !m_pGraphObj->isNameLabelAnchorLineVisible() )
                {
                    m_pGraphObj->showNameLabelAnchorLine();
                }
            }
            else
            {
                if( m_pGraphObj->isNameLabelAnchorLineVisible() )
                {
                    m_pGraphObj->hideNameLabelAnchorLine();
                }
            }
        }
    }

    if( m_pCmbPathLabelAnchorSelPt != nullptr && m_pChkPathLabelVisible != nullptr && m_pChkPathLabelAnchorLineVisible != nullptr )
    {
        QString strAnchorSelPt = m_pCmbPathLabelAnchorSelPt->currentText();
        bool bConverted = false;
        CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

        if( bConverted )
        {
            if( m_pChkPathLabelVisible->isChecked() )
            {
                if( !m_pGraphObj->isPathLabelVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->showPathLabel(eSelPt.enumerator());
                }
            }
            else
            {
                if( m_pGraphObj->isPathLabelVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->hidePathLabel();
                }
            }
            if( m_pChkPathLabelAnchorLineVisible->isChecked() )
            {
                if( !m_pGraphObj->isPathLabelAnchorLineVisible() )
                {
                    m_pGraphObj->showPathLabelAnchorLine();
                }
            }
            else
            {
                if( m_pGraphObj->isPathLabelAnchorLineVisible() )
                {
                    m_pGraphObj->hidePathLabelAnchorLine();
                }
            }
        }
    }

    if( m_pCmbDescriptionLabelAnchorSelPt != nullptr && m_pChkDescriptionLabelVisible != nullptr && m_pChkDescriptionLabelAnchorLineVisible != nullptr )
    {
        QString strAnchorSelPt = m_pCmbDescriptionLabelAnchorSelPt->currentText();
        bool bConverted = false;
        CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

        if( bConverted )
        {
            if( m_pChkDescriptionLabelVisible->isChecked() )
            {
                if( !m_pGraphObj->isDescriptionLabelVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->showDescriptionLabel(eSelPt.enumerator());
                }
            }
            else
            {
                if( m_pGraphObj->isDescriptionLabelVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->hideDescriptionLabel();
                }
            }
            if( m_pChkDescriptionLabelAnchorLineVisible->isChecked() )
            {
                if( !m_pGraphObj->isDescriptionLabelAnchorLineVisible() )
                {
                    m_pGraphObj->showDescriptionLabelAnchorLine();
                }
            }
            else
            {
                if( m_pGraphObj->isDescriptionLabelAnchorLineVisible() )
                {
                    m_pGraphObj->hideDescriptionLabelAnchorLine();
                }
            }
        }
    }

    // Dimension Label Visibilities
    //=============================

    if( m_pCmbDimensionPosLabelAnchorSelPt != nullptr && m_pChkDimensionPosLabelVisible != nullptr && m_pChkDimensionPosLabelAnchorLineVisible != nullptr )
    {
        QString strAnchorSelPt = m_pCmbDimensionPosLabelAnchorSelPt->currentText();
        bool bConverted = false;
        CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

        if( bConverted )
        {
            Qt::CheckState checkStatePosLabelVisible = m_pChkDimensionPosLabelVisible->checkState();
            Qt::CheckState checkStatePosLabelAnchorLineVisible = m_pChkDimensionPosLabelAnchorLineVisible->checkState();

            if( checkStatePosLabelVisible == Qt::Checked )
            {
                if( !m_pGraphObj->isPositionLabelVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->showPositionLabel(eSelPt.enumerator());
                }
            }
            else if( checkStatePosLabelVisible == Qt::Unchecked )
            {
                if( m_pGraphObj->isPositionLabelVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->hidePositionLabel(eSelPt.enumerator());
                }
            }
            if( checkStatePosLabelAnchorLineVisible == Qt::Checked )
            {
                if( !m_pGraphObj->isPositionLabelAnchorLineVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->showPositionLabelAnchorLine(eSelPt.enumerator());
                }
            }
            else if( checkStatePosLabelAnchorLineVisible == Qt::Unchecked )
            {
                if( m_pGraphObj->isPositionLabelAnchorLineVisible(eSelPt.enumerator()) )
                {
                    m_pGraphObj->hidePositionLabelAnchorLine(eSelPt.enumerator());
                }
            }
        }
    }

    // Dimension Line Visibilities
    //============================


    // At the end update the user controls with the last changes from the graphical object.
    //=====================================================================================

    // This might be necessary if on setting one property the graphical object itself
    // internally corrects other settings.

    m_bApplyingChanges = false;

    onGraphObjChanged();

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::resetChanges()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "resetChanges",
        /* strAddInfo   */ strMthInArgs );

    onGraphObjChanged();

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsLabelVisibilities::hasChanges() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ strMthInArgs );

    bool bHasChanges = false;

    // Label Visibilities
    //===================

    if( !bHasChanges && m_pCmbNameLabelAnchorSelPt != nullptr && m_pChkNameLabelVisible != nullptr && m_pChkNameLabelAnchorLineVisible != nullptr )
    {
        QString strAnchorSelPt = m_pCmbNameLabelAnchorSelPt->currentText();
        bool bConverted = false;
        CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

        bHasChanges = (m_pGraphObj->isNameLabelVisible(eSelPt.enumerator()) != m_pChkNameLabelVisible->isChecked());

        if( !bHasChanges )
        {
            bHasChanges = (m_pGraphObj->isNameLabelAnchorLineVisible() != m_pChkNameLabelAnchorLineVisible->isChecked());
        }
    }

    if( !bHasChanges && m_pCmbPathLabelAnchorSelPt != nullptr && m_pChkPathLabelVisible != nullptr && m_pChkPathLabelAnchorLineVisible != nullptr )
    {
        QString strAnchorSelPt = m_pCmbPathLabelAnchorSelPt->currentText();
        bool bConverted = false;
        CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

        bHasChanges = (m_pGraphObj->isPathLabelVisible(eSelPt.enumerator()) != m_pChkPathLabelVisible->isChecked());

        if( !bHasChanges )
        {
            bHasChanges = (m_pGraphObj->isPathLabelAnchorLineVisible() != m_pChkPathLabelAnchorLineVisible->isChecked());
        }
    }

    if( !bHasChanges && m_pCmbDescriptionLabelAnchorSelPt != nullptr && m_pChkDescriptionLabelVisible != nullptr && m_pChkDescriptionLabelAnchorLineVisible != nullptr )
    {
        QString strAnchorSelPt = m_pCmbDescriptionLabelAnchorSelPt->currentText();
        bool bConverted = false;
        CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

        bHasChanges = (m_pGraphObj->isDescriptionLabelVisible() != m_pChkDescriptionLabelVisible->isChecked());

        if( !bHasChanges )
        {
            bHasChanges = (m_pGraphObj->isDescriptionLabelAnchorLineVisible() != m_pChkDescriptionLabelAnchorLineVisible->isChecked());
        }
    }

    // Dimension Label Visibilities
    //=============================

    if( !bHasChanges && m_pCmbDimensionPosLabelAnchorSelPt != nullptr && m_pChkDimensionPosLabelVisible != nullptr && m_pChkDimensionPosLabelAnchorLineVisible != nullptr )
    {
        QString strAnchorSelPt = m_pCmbDimensionPosLabelAnchorSelPt->currentText();
        bool bConverted = false;
        CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

        if( bConverted )
        {
            Qt::CheckState checkStatePosLabelVisible = m_pChkDimensionPosLabelVisible->checkState();
            Qt::CheckState checkStatePosLabelAnchorLineVisible = m_pChkDimensionPosLabelAnchorLineVisible->checkState();

            if( checkStatePosLabelVisible == Qt::Checked )
            {
                bHasChanges = !m_pGraphObj->isPositionLabelVisible(eSelPt.enumerator());
            }
            else if( checkStatePosLabelVisible == Qt::Unchecked )
            {
                bHasChanges = m_pGraphObj->isPositionLabelVisible(eSelPt.enumerator());
            }
            if( !bHasChanges )
            {
                if( checkStatePosLabelAnchorLineVisible == Qt::Checked )
                {
                    bHasChanges = !m_pGraphObj->isPositionLabelAnchorLineVisible(eSelPt.enumerator());
                }
                else if( checkStatePosLabelAnchorLineVisible == Qt::Unchecked )
                {
                    bHasChanges = m_pGraphObj->isPositionLabelAnchorLineVisible(eSelPt.enumerator());
                }
            }
        }
    }

    // Dimension Line Visibilities
    //=============================


    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bHasChanges);
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_pCmbNameLabelAnchorSelPt != nullptr && m_pChkNameLabelVisible != nullptr && m_pChkNameLabelAnchorLineVisible != nullptr )
    {
        updateVisibilityCheckStates(m_pCmbNameLabelAnchorSelPt, m_pChkNameLabelVisible, m_pChkNameLabelAnchorLineVisible);
    }
    if( m_pCmbPathLabelAnchorSelPt != nullptr && m_pChkPathLabelVisible != nullptr && m_pChkPathLabelAnchorLineVisible != nullptr )
    {
        updateVisibilityCheckStates(m_pCmbPathLabelAnchorSelPt, m_pChkPathLabelVisible, m_pChkPathLabelAnchorLineVisible);
    }
    if( m_pCmbDescriptionLabelAnchorSelPt != nullptr && m_pChkDescriptionLabelVisible != nullptr && m_pChkDescriptionLabelAnchorLineVisible != nullptr )
    {
        updateVisibilityCheckStates(m_pCmbDescriptionLabelAnchorSelPt, m_pChkDescriptionLabelVisible, m_pChkDescriptionLabelAnchorLineVisible);
    }
    if( m_pCmbDimensionPosLabelAnchorSelPt != nullptr && m_pChkDimensionPosLabelVisible != nullptr && m_pChkDimensionPosLabelAnchorLineVisible != nullptr )
    {
        updateVisibilityCheckStates(m_pCmbDimensionPosLabelAnchorSelPt, m_pChkDimensionPosLabelVisible, m_pChkDimensionPosLabelAnchorLineVisible);
    }

} // onGraphObjChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onCmbNameLabelAnchorSelPtCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = CEnumSelectionPoint::toString(static_cast<ESelectionPoint>(i_iIdx));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbNameLabelAnchorSelPtCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

} // onCmbNameLabelAnchorSelPtCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkNameLabelVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkNameLabelVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkNameLabelVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkNameLabelAnchorLineVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkNameLabelAnchorLineVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkNameLabelAnchorLineVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onCmbPathLabelAnchorSelPtCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = CEnumSelectionPoint::toString(static_cast<ESelectionPoint>(i_iIdx));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbPathLabelAnchorSelPtCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

} // onCmbPathLabelAnchorSelPtCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkPathLabelVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkPathLabelVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkPathLabelVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkPathLabelAnchorLineVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkPathLabelAnchorLineVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkPathLabelAnchorLineVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onCmbDescriptionLabelAnchorSelPtCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = CEnumSelectionPoint::toString(static_cast<ESelectionPoint>(i_iIdx));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbDescriptionLabelAnchorSelPtCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

} // onCmbDescriptionLabelAnchorSelPtCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDescriptionLabelVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDescriptionLabelVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDescriptionLabelVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDescriptionLabelAnchorLineVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDescriptionLabelAnchorLineVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDescriptionLabelAnchorLineVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onCmbDimensionPosLabelAnchorSelPtCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = CEnumSelectionPoint::toString(static_cast<ESelectionPoint>(i_iIdx));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbDimensionPosLabelAnchorSelPtCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

    if( hasChanges() )
    {
        QString strMsgText;

        strMsgText  = "You have unsaved changes?\n";
        strMsgText += "Do you want to apply your changes?";

        QMessageBox::StandardButton msgBoxBtn = QMessageBox::question(
            /* pWdgtParent */ this,
            /* strTitle    */ QApplication::applicationName(),
            /* strText     */ strMsgText,
            /* buttons     */ QMessageBox::Yes | QMessageBox::No );

        if( msgBoxBtn == QMessageBox::Yes )
        {
            applyChanges();
        }
    }

    updateVisibilityCheckStates(m_pCmbDimensionPosLabelAnchorSelPt, m_pChkDimensionPosLabelVisible, m_pChkDimensionPosLabelAnchorLineVisible);

} // onCmbDimensionPosLabelAnchorSelPtCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionPosLabelVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionPosLabelVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

    QString strAnchorSelPt = m_pCmbDimensionPosLabelAnchorSelPt->currentText();
    bool bConverted = false;
    CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

    if( bConverted )
    {
        if( eSelPt == ESelectionPoint::All )
        {
            bool bAllPosLabelsAnchorLinesAreVisible = m_pGraphObj->isPositionLabelAnchorLineVisible(ESelectionPoint::All);
            bool bAnyPosLabelsAnchorLineIsVisible = m_pGraphObj->isPositionLabelAnchorLineVisible(ESelectionPoint::Any);

            Qt::CheckState checkStatePosLabelAnchorLineVisible = bAllPosLabelsAnchorLinesAreVisible ? Qt::Checked : bAnyPosLabelsAnchorLineIsVisible ? Qt::PartiallyChecked : Qt::Unchecked;

            m_pChkDimensionPosLabelAnchorLineVisible->setCheckState(checkStatePosLabelAnchorLineVisible);
        }
        else
        {
            if( i_iCheckState == Qt::Checked )
            {
                bool bIsPosLabelAnchorLineVisible = m_pGraphObj->isPositionLabelAnchorLineVisible(eSelPt.enumerator());
                m_pChkDimensionPosLabelAnchorLineVisible->setCheckState(bIsPosLabelAnchorLineVisible ? Qt::Checked : Qt::Unchecked);
            }
            else if( i_iCheckState == Qt::Unchecked )
            {
                m_pChkDimensionPosLabelAnchorLineVisible->setCheckState(Qt::Unchecked);
            }
        }
    }
} // onChkDimensionPosLabelVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionPosLabelAnchorLineVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionPosLabelAnchorLineVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionPosLabelAnchorLineVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onCmbDimensionWidthLabelAnchorSelPtCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = CEnumSelectionPoint::toString(static_cast<ESelectionPoint>(i_iIdx));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbDimensionWidthLabelAnchorSelPtCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

} // onCmbDimensionWidthLabelAnchorSelPtCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionWidthLabelVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionWidthLabelVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionWidthLabelVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionWidthLabelAnchorLineVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionWidthLabelAnchorLineVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionWidthLabelAnchorLineVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onCmbDimensionHeightLabelAnchorSelPtCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = CEnumSelectionPoint::toString(static_cast<ESelectionPoint>(i_iIdx));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbDimensionHeightLabelAnchorSelPtCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

} // onCmbDimensionHeightLabelAnchorSelPtCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionHeightLabelVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionHeightLabelVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionHeightLabelVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionHeightLabelAnchorLineVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionHeightLabelAnchorLineVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionHeightLabelAnchorLineVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onCmbDimensionRotationAngleLabelAnchorSelPtCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = CEnumSelectionPoint::toString(static_cast<ESelectionPoint>(i_iIdx));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCmbDimensionRotationAngleLabelAnchorSelPtCurrentIndexChanged",
        /* strAddInfo   */ strMthInArgs );

} // onCmbDimensionRotationAngleLabelAnchorSelPtCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionRotationAngleLabelVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionRotationAngleLabelVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionRotationAngleLabelVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionRotationAngleLabelAnchorLineVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionRotationAngleLabelAnchorLineVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionRotationAngleLabelAnchorLineVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionLinesBoundingRectVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionLinesBoundingRectVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionLinesBoundingRectVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionLinesBoundingRectDiagonalsVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionLinesBoundingRectDiagonalsVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionLinesBoundingRectDiagonalsVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionLinesBoundingRectHorizontalSymmetryAxisVisibleStateChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::onChkDimensionLinesBoundingRectVerticalSymmetryAxisVisibleStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qCheckState2Str(i_iCheckState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onChkDimensionLinesBoundingRectVerticalSymmetryAxisVisibleStateChanged",
        /* strAddInfo   */ strMthInArgs );

} // onChkDimensionLinesBoundingRectVerticalSymmetryAxisVisibleStateChanged

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::fillComboAnchorSelPt( QComboBox* i_pCmb )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillComboAnchorSelPt",
        /* strAddInfo   */ strMthInArgs );

    EGraphObjType graphObjType = m_pGraphObj->type();

    if( graphObjType == EGraphObjTypePoint )
    {
        CEnumSelectionPoint eSelPt = ESelectionPoint::Center;
        i_pCmb->addItem(eSelPt.toString(EEnumEntryAliasStrText), eSelPt.enumeratorAsInt());
        i_pCmb->setCurrentText( eSelPt.toString(EEnumEntryAliasStrText) );
        i_pCmb->setEnabled(false);
    }
    else if( graphObjType == EGraphObjTypeGroup )
    {
        CEnumSelectionPoint eSelPt;
        for( eSelPt = 0; eSelPt < CEnumSelectionPoint::count(); ++eSelPt )
        {
            if( eSelPt != ESelectionPoint::None && eSelPt != ESelectionPoint::Any
             && eSelPt != ESelectionPoint::RotateTop && eSelPt != ESelectionPoint::RotateBottom )
            {
                i_pCmb->addItem(eSelPt.toString(EEnumEntryAliasStrText), eSelPt.enumeratorAsInt());
            }
        }
        i_pCmb->setCurrentText( CEnumSelectionPoint(ESelectionPoint::TopLeft).toString(EEnumEntryAliasStrText) );
        i_pCmb->setEnabled(true);
    }

} // fillComboAnchorSelPt

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabelVisibilities::updateVisibilityCheckStates(
    QComboBox* i_pCmbSelPt,
    QCheckBox* i_pChkLabelVisible,
    QCheckBox* i_pChkLabelAnchorLineVisible )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_pCmbSelPt->objectName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateVisibilityCheckStates",
        /* strAddInfo   */ strMthInArgs );

    EGraphObjType graphObjType = m_pGraphObj->type();

    QString strAnchorSelPt = i_pCmbSelPt->currentText();
    bool bConverted = false;
    CEnumSelectionPoint eSelPt = CEnumSelectionPoint::fromString(strAnchorSelPt, &bConverted);

    if( eSelPt == ESelectionPoint::All )
    {
        bool bAllLabelsAreVisible = false;
        bool bAnyLabelsIsVisible = false;
        bool bAllLabelsAnchorLinesAreVisible = false;
        bool bAnyLabelsAnchorLineIsVisible = false;

        if( i_pCmbSelPt->objectName() == "Name" )
        {
            bAllLabelsAreVisible = m_pGraphObj->isNameLabelVisible(ESelectionPoint::All);
            bAnyLabelsIsVisible = m_pGraphObj->isNameLabelVisible(ESelectionPoint::Any);
            bAllLabelsAnchorLinesAreVisible = m_pGraphObj->isNameLabelAnchorLineVisible();
            bAnyLabelsAnchorLineIsVisible = m_pGraphObj->isNameLabelAnchorLineVisible();
        }
        else if( i_pCmbSelPt->objectName() == "Path" )
        {
            bAllLabelsAreVisible = m_pGraphObj->isPathLabelVisible(ESelectionPoint::All);
            bAnyLabelsIsVisible = m_pGraphObj->isPathLabelVisible(ESelectionPoint::Any);
            bAllLabelsAnchorLinesAreVisible = m_pGraphObj->isPathLabelAnchorLineVisible();
            bAnyLabelsAnchorLineIsVisible = m_pGraphObj->isPathLabelAnchorLineVisible();
        }
        else if( i_pCmbSelPt->objectName() == "Description" )
        {
            bAllLabelsAreVisible = m_pGraphObj->isDescriptionLabelVisible(ESelectionPoint::All);
            bAnyLabelsIsVisible = m_pGraphObj->isDescriptionLabelVisible(ESelectionPoint::Any);
            bAllLabelsAnchorLinesAreVisible = m_pGraphObj->isDescriptionLabelAnchorLineVisible();
            bAnyLabelsAnchorLineIsVisible = m_pGraphObj->isDescriptionLabelAnchorLineVisible();
        }
        else if( i_pCmbSelPt->objectName() == "DimensionPos" )
        {
            bAllLabelsAreVisible = m_pGraphObj->isPositionLabelVisible(ESelectionPoint::All);
            bAnyLabelsIsVisible = m_pGraphObj->isPositionLabelVisible(ESelectionPoint::Any);
            bAllLabelsAnchorLinesAreVisible = m_pGraphObj->isPositionLabelAnchorLineVisible(ESelectionPoint::All);
            bAnyLabelsAnchorLineIsVisible = m_pGraphObj->isPositionLabelAnchorLineVisible(ESelectionPoint::Any);
        }
        else if( i_pCmbSelPt->objectName() == "DimensionWidth" )
        {
        }
        else if( i_pCmbSelPt->objectName() == "DimensionHeight" )
        {
        }
        else if( i_pCmbSelPt->objectName() == "DimensionRotationAngle" )
        {
        }

        Qt::CheckState checkStateLabelVisible = bAllLabelsAreVisible ? Qt::Checked : bAnyLabelsIsVisible ? Qt::PartiallyChecked : Qt::Unchecked;
        Qt::CheckState checkStateLabelAnchorLineVisible = bAllLabelsAnchorLinesAreVisible ? Qt::Checked : bAnyLabelsAnchorLineIsVisible ? Qt::PartiallyChecked : Qt::Unchecked;

        if( checkStateLabelVisible == Qt::PartiallyChecked )
        {
            i_pChkLabelVisible->setTristate(true);
        }
        i_pChkLabelVisible->setCheckState(checkStateLabelVisible);

        if( checkStateLabelAnchorLineVisible == Qt::PartiallyChecked )
        {
            i_pChkLabelAnchorLineVisible->setTristate(true);
        }
        i_pChkLabelAnchorLineVisible->setCheckState(checkStateLabelAnchorLineVisible);
    }
    else
    {
        i_pChkLabelVisible->setTristate(false);
        i_pChkLabelAnchorLineVisible->setTristate(false);

        bool bIsLabelVisible = false;
        bool bIsLabelAnchorLineVisible = false;

        if( i_pCmbSelPt->objectName() == "Name" )
        {
            bIsLabelVisible = m_pGraphObj->isNameLabelVisible(eSelPt.enumerator());
            bIsLabelAnchorLineVisible = m_pGraphObj->isNameLabelAnchorLineVisible();
        }
        else if( i_pCmbSelPt->objectName() == "Path" )
        {
            bIsLabelVisible = m_pGraphObj->isPathLabelVisible(eSelPt.enumerator());
            bIsLabelAnchorLineVisible = m_pGraphObj->isPathLabelAnchorLineVisible();
        }
        else if( i_pCmbSelPt->objectName() == "Description" )
        {
            bIsLabelVisible = m_pGraphObj->isDescriptionLabelVisible(eSelPt.enumerator());
            bIsLabelAnchorLineVisible = m_pGraphObj->isDescriptionLabelAnchorLineVisible();
        }
        else if( i_pCmbSelPt->objectName() == "DimensionPos" )
        {
            bIsLabelVisible = m_pGraphObj->isPositionLabelVisible(eSelPt.enumerator());
            bIsLabelAnchorLineVisible = m_pGraphObj->isPositionLabelAnchorLineVisible(eSelPt.enumerator());
        }
        else if( i_pCmbSelPt->objectName() == "DimensionWidth" )
        {
        }
        else if( i_pCmbSelPt->objectName() == "DimensionHeight" )
        {
        }
        else if( i_pCmbSelPt->objectName() == "DimensionRotationAngle" )
        {
        }

        i_pChkLabelVisible->setCheckState(bIsLabelVisible ? Qt::Checked : Qt::Unchecked);

        if( bIsLabelVisible )
        {
            i_pChkLabelAnchorLineVisible->setCheckState(bIsLabelAnchorLineVisible ? Qt::Checked : Qt::Unchecked);
        }
        else
        {
            i_pChkLabelAnchorLineVisible->setCheckState(Qt::Unchecked);
        }
    }
} // updateVisibilityCheckStates