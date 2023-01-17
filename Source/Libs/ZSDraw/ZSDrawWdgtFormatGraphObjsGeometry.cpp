/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSDraw/ZSDrawWdgtFormatGraphObjsGeometry.h"
#include "ZSDraw/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjImage.h"
#include "ZSDraw/ZSDrawGraphObjLine.h"
#include "ZSDraw/ZSDrawGraphObjPoint.h"
#include "ZSDraw/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/ZSDrawGraphObjPolyline.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSDraw/ZSDrawGraphObjText.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSysGUI/ZSSysPushButton.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtFormatGraphObjsGeometry : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsGeometry::CWdgtFormatGraphObjsGeometry(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_ptPos(0.0,0.0),
    m_size(0.0,0.0),
    m_plg(),
    m_fRotAngle_deg(0.0),
    m_fZValue(0.0),
    m_bIsChangingValue(false),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pWdgtPosSize(nullptr),
    m_pLytWdgtPosSize(nullptr),
    m_pLytHeadLineWdgtPosSize(nullptr),
    m_pLblHeadLinePosSize(nullptr),
    m_pSepHeadLinePosSize(nullptr),
    m_pLytLinePos(nullptr),
    m_pLblPosX(nullptr),
    m_pEdtPosX(nullptr),
    m_pLblPosY(nullptr),
    m_pEdtPosY(nullptr),
    m_pLytLineSize(nullptr),
    m_pLblSize(nullptr),
    m_pLblSizeWidth(nullptr),
    m_pEdtSizeWidth(nullptr),
    m_pLblSizeHeight(nullptr),
    m_pEdtSizeHeight(nullptr),
    m_pWdgtShapePoints(nullptr),
    m_pLytWdgtShapePoints(nullptr),
    m_pLytHeadLineShapePoints(nullptr),
    m_pLblHeadLineShapePoints(nullptr),
    m_pSepHeadLineShapePoints(nullptr),
    m_pLytShapePoints(nullptr),
    m_pModelShapePoints(nullptr),
    m_pViewShapePoints(nullptr),
    m_pWdgtShapePointButtons(nullptr),
    m_pLytWdgtShapePointButtons(nullptr),
    m_pBtnShapePointAdd(nullptr),
    m_pBtnShapePointDelete(nullptr),
    m_pBtnShapePointUp(nullptr),
    m_pBtnShapePointDown(nullptr),
    m_bOnModelShapePointsItemChangedInProcess(false),
    m_pWdgtRotation(nullptr),
    m_pLytWdgtRotation(nullptr),
    m_pLytHeadLineRotation(nullptr),
    m_pLblHeadLineRotation(nullptr),
    m_pSepHeadLineRotation(nullptr),
    m_pLytLineRotationAngle(nullptr),
    m_pLblRotationAngle(nullptr),
    m_pEdtRotationAngle(nullptr),
    m_pWdgtStackingOrder(nullptr),
    m_pLytWdgtStackingOrder(nullptr),
    m_pLytHeadLineStackingOrder(nullptr),
    m_pLblHeadLineStackingOrder(nullptr),
    m_pSepHeadLineStackingOrder(nullptr),
    m_pLytLineZValue(nullptr),
    m_pLblZValue(nullptr),
    m_pEdtZValue(nullptr),
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

    m_pLblHeadLine = new QLabel("Geometry");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Widget> Position and Size
    //===========================

    m_ptPos = m_pGraphObj->getPos();
    m_size  = m_pGraphObj->getSize();

    if( graphObjType != EGraphObjTypeLine && graphObjType != EGraphObjTypeConnectionLine )
    {
        m_pWdgtPosSize = new QWidget();
        m_pLytWdgtPosSize = new QVBoxLayout();
        m_pLytWdgtPosSize->setContentsMargins(0,0,0,0);
        m_pWdgtPosSize->setLayout(m_pLytWdgtPosSize);
        m_pLyt->addWidget(m_pWdgtPosSize);

        // <Separator> Position and Size
        //------------------------------

        m_pLytWdgtPosSize->addSpacing(4);

        m_pLytHeadLineWdgtPosSize = new QHBoxLayout();
        m_pLytWdgtPosSize->addLayout(m_pLytHeadLineWdgtPosSize);

        m_pLblHeadLinePosSize = new QLabel("Position and Size:");
        m_pLytHeadLineWdgtPosSize->addWidget(m_pLblHeadLinePosSize);
        m_pSepHeadLinePosSize = new QFrame();
        m_pSepHeadLinePosSize->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytHeadLineWdgtPosSize->addWidget(m_pSepHeadLinePosSize, 1);

        m_pLytWdgtPosSize->addSpacing(4);

        // <Line> Position X/Y
        //--------------------

        m_pLytLinePos = new QHBoxLayout();
        m_pLytWdgtPosSize->addLayout(m_pLytLinePos);
        m_pLblPos = new QLabel("Position");
        m_pLblPos->setFixedWidth(cxLblWidthClm0);
        m_pLytLinePos->addWidget(m_pLblPos);

        // <Edit> Position X
        //------------------

        m_pLblPosX = new QLabel("X:");
        m_pLblPosX->setFixedWidth(cxLblWidthClm1);
        m_pLytLinePos->addWidget(m_pLblPosX);
        m_pEdtPosX = new QDoubleSpinBox();
        m_pEdtPosX->setFixedWidth(cxEdtWidthClm1);
        m_pEdtPosX->setRange(0.0, rctScene.right());
        m_pEdtPosX->setDecimals(1);
        m_pEdtPosX->setSingleStep(0.1);
        m_pEdtPosX->setSuffix(" px");
        m_pEdtPosX->setValue(m_ptPos.x());
        m_pLytLinePos->addWidget(m_pEdtPosX);
        m_pLytLinePos->addSpacing(cxSpacingClm1Cml2);

        if( !connect(
            /* pObjSender   */ m_pEdtPosX,
            /* szSignal     */ SIGNAL(valueChanged(double)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtPositionXValueChanged(double)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <Edit> Position Y
        //------------------

        m_pLblPosY = new QLabel("Y:");
        m_pLblPosY->setFixedWidth(cxLblWidthClm2);
        m_pLytLinePos->addWidget(m_pLblPosY);
        m_pEdtPosY = new QDoubleSpinBox();
        m_pEdtPosY->setFixedWidth(cxEdtWidthClm2);
        m_pEdtPosY->setRange(0.0,rctScene.right());
        m_pEdtPosY->setDecimals(1);
        m_pEdtPosY->setSingleStep(0.1);
        m_pEdtPosY->setSuffix(" px");
        m_pEdtPosY->setValue(m_ptPos.y());
        m_pLytLinePos->addWidget(m_pEdtPosY);
        m_pLytLinePos->addStretch();

        if( !connect(
            /* pObjSender   */ m_pEdtPosY,
            /* szSignal     */ SIGNAL(valueChanged(double)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtPositionYValueChanged(double)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        if( m_pGraphObj->hasBoundingRect() )
        {
            // <Line> Size Width/Height
            //------------ ------------

            m_pLytLineSize = new QHBoxLayout();
            m_pLytWdgtPosSize->addLayout(m_pLytLineSize);
            m_pLblSize = new QLabel("Size");
            m_pLblSize->setFixedWidth(cxLblWidthClm0);
            m_pLytLineSize->addWidget(m_pLblSize);

            // <Edit> Size Width
            //-------------------

            m_pLblSizeWidth = new QLabel("Width:");
            m_pLblSizeWidth->setFixedWidth(cxLblWidthClm1);
            m_pLytLineSize->addWidget(m_pLblSizeWidth);
            m_pEdtSizeWidth = new QDoubleSpinBox();
            m_pEdtSizeWidth->setFixedWidth(cxEdtWidthClm1);
            m_pEdtSizeWidth->setRange(0.0, rctScene.right());
            m_pEdtSizeWidth->setDecimals(1);
            m_pEdtSizeWidth->setSingleStep(0.1);
            m_pEdtSizeWidth->setSuffix(" px");
            m_pEdtSizeWidth->setValue(m_size.width());
            m_pLytLineSize->addWidget(m_pEdtSizeWidth);
            m_pLytLineSize->addSpacing(cxSpacingClm1Cml2);

            if( !connect(
                /* pObjSender   */ m_pEdtSizeWidth,
                /* szSignal     */ SIGNAL(valueChanged(double)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeWidthValueChanged(double)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
            if( !connect(
                /* pObjSender   */ m_pEdtSizeWidth,
                /* szSignal     */ SIGNAL(editingFinished()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeWidthEditingFinished()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            // <Edit> Size Height
            //-------------------

            m_pLblSizeHeight = new QLabel("Height:");
            m_pLblSizeHeight->setFixedWidth(cxLblWidthClm2);
            m_pLytLineSize->addWidget(m_pLblSizeHeight);
            m_pEdtSizeHeight = new QDoubleSpinBox();
            m_pEdtSizeHeight->setFixedWidth(cxEdtWidthClm2);
            m_pEdtSizeHeight->setRange(0.0, rctScene.right());
            m_pEdtSizeHeight->setDecimals(1);
            m_pEdtSizeHeight->setSingleStep(0.1);
            m_pEdtSizeHeight->setSuffix(" px");
            m_pEdtSizeHeight->setValue(m_size.height());
            m_pLytLineSize->addWidget(m_pEdtSizeHeight);
            m_pLytLineSize->addStretch();

            if( !connect(
                /* pObjSender   */ m_pEdtSizeHeight,
                /* szSignal     */ SIGNAL(valueChanged(double)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeHeightValueChanged(double)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
            if( !connect(
                /* pObjSender   */ m_pEdtSizeHeight,
                /* szSignal     */ SIGNAL(editingFinished()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeHeightEditingFinished()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
        } // if( m_pGraphObj->hasBoundingRect() )
    } // if( graphObjType != EGraphObjTypeLine && graphObjType != EGraphObjTypeConnectionLine )

    // <Widget> Shape Points
    //======================

    if( m_pGraphObj->hasLineShapePoints() )
    {
        m_pWdgtShapePoints = new QWidget();
        m_pLytWdgtShapePoints = new QVBoxLayout();
        m_pLytWdgtShapePoints->setContentsMargins(0,0,0,0);
        m_pWdgtShapePoints->setLayout(m_pLytWdgtShapePoints);
        m_pLyt->addWidget(m_pWdgtShapePoints);

        // <Separator> ShapePoints
        //------------------------

        m_pLytWdgtShapePoints->addSpacing(4);

        m_pLytHeadLineShapePoints = new QHBoxLayout();
        m_pLytWdgtShapePoints->addLayout(m_pLytHeadLineShapePoints);

        m_pLblHeadLineShapePoints = new QLabel("Shape Points:");
        m_pLytHeadLineShapePoints->addWidget(m_pLblHeadLineShapePoints);
        m_pSepHeadLineShapePoints = new QFrame();
        m_pSepHeadLineShapePoints->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytHeadLineShapePoints->addWidget(m_pSepHeadLineShapePoints,1);

        m_pLytWdgtShapePoints->addSpacing(4);

        // <Table> Shape Points
        //-----------------------

        m_pLytShapePoints = new QHBoxLayout();
        m_pLytWdgtShapePoints->addLayout(m_pLytShapePoints);
        m_pViewShapePoints = new QTableView();
        m_pLytShapePoints->addWidget(m_pViewShapePoints);
        m_pModelShapePoints = new QStandardItemModel;
        m_pModelShapePoints->setColumnCount(EClmCount);
        m_pViewShapePoints->setModel(m_pModelShapePoints);
        m_pViewShapePoints->horizontalHeader()->setStretchLastSection(true);

        if( !connect(
            /* pObjSender   */ m_pModelShapePoints,
            /* szSignal     */ SIGNAL(itemChanged(QStandardItem*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onModelShapePointsItemChanged(QStandardItem*)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // Called by onGraphObjChanged:
        //fillShapePointsModel();

        // <Widget> Shape Point Buttons
        //-----------------------------

        if( graphObjType != EGraphObjTypeLine )
        {
            m_pWdgtShapePointButtons = new QWidget();
            m_pLytWdgtShapePointButtons = new QVBoxLayout();
            m_pLytWdgtShapePointButtons->setContentsMargins(10,10,0,0);
            m_pWdgtShapePointButtons->setLayout(m_pLytWdgtShapePointButtons);
            m_pLytShapePoints->addWidget(m_pWdgtShapePointButtons);
            m_pLytShapePoints->addStretch();

            // <Button> Shape Point Add
            //-------------------------

            QPixmap pxmBtnAdd(":/ZS/Button/ButtonAdd16x16.bmp");
            pxmBtnAdd.setMask(pxmBtnAdd.createHeuristicMask());

            m_pBtnShapePointAdd = new CPushButton();
            m_pBtnShapePointAdd->setFixedWidth(24);
            m_pBtnShapePointAdd->setFixedHeight(24);
            m_pBtnShapePointAdd->setPixmap(pxmBtnAdd);
            m_pLytWdgtShapePointButtons->addWidget(m_pBtnShapePointAdd);

            if( !connect(
                /* pObjSender   */ m_pBtnShapePointAdd,
                /* szSignal     */ SIGNAL(clicked()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnShapePointAddClicked()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            // <Button> Shape Point Delete
            //----------------------------

            QPixmap pxmBtnDelete(":/ZS/Button/ButtonDelete16x16.bmp");
            pxmBtnDelete.setMask(pxmBtnDelete.createHeuristicMask());

            m_pBtnShapePointDelete = new CPushButton();
            m_pBtnShapePointDelete->setFixedWidth(24);
            m_pBtnShapePointDelete->setFixedHeight(24);
            m_pBtnShapePointDelete->setPixmap(pxmBtnDelete);
            m_pLytWdgtShapePointButtons->addWidget(m_pBtnShapePointDelete);

            if( !connect(
                /* pObjSender   */ m_pBtnShapePointDelete,
                /* szSignal     */ SIGNAL(clicked()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnShapePointDeleteClicked()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            // <Button> Shape Point Move Up
            //-----------------------------

            QPixmap pxmBtnUp(":/ZS/Button/ButtonMoveUp16x16.bmp");
            pxmBtnUp.setMask(pxmBtnUp.createHeuristicMask());

            m_pBtnShapePointUp = new CPushButton();
            m_pBtnShapePointUp->setFixedWidth(24);
            m_pBtnShapePointUp->setFixedHeight(24);
            m_pBtnShapePointUp->setPixmap(pxmBtnUp);
            m_pLytWdgtShapePointButtons->addWidget(m_pBtnShapePointUp);

            if( !connect(
                /* pObjSender   */ m_pBtnShapePointUp,
                /* szSignal     */ SIGNAL(clicked()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnShapePointUpClicked()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            // <Button> Shape Point Move Down
            //-------------------------------

            QPixmap pxmBtnDown(":/ZS/Button/ButtonMoveDown16x16.bmp");
            pxmBtnDown.setMask(pxmBtnDown.createHeuristicMask());

            m_pBtnShapePointDown = new CPushButton();
            m_pBtnShapePointDown->setFixedWidth(24);
            m_pBtnShapePointDown->setFixedHeight(24);
            m_pBtnShapePointDown->setPixmap(pxmBtnDown);
            m_pLytWdgtShapePointButtons->addWidget(m_pBtnShapePointDown);
            m_pLytWdgtShapePointButtons->addStretch();

            if( !connect(
                /* pObjSender   */ m_pBtnShapePointDown,
                /* szSignal     */ SIGNAL(clicked()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnShapePointDownClicked()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
        } // if( graphObjType != EGraphObjTypeLine )
    } // if( m_pGraphObj->hasLineShapePoints() )

    // <Widget> Rotation
    //==================

    m_fRotAngle_deg = m_pGraphObj->getRotationAngleInDegree();

    if( m_pGraphObj->hasRotationSelectionPoints() )
    {
        m_pWdgtRotation = new QWidget();
        m_pLytWdgtRotation = new QVBoxLayout();
        m_pLytWdgtRotation->setContentsMargins(0,0,0,0);
        m_pWdgtRotation->setLayout(m_pLytWdgtRotation);
        m_pLyt->addWidget(m_pWdgtRotation);

        // <Separator> Rotation
        //----------------------

        m_pLytWdgtRotation->addSpacing(4);

        m_pLytHeadLineRotation = new QHBoxLayout();
        m_pLytWdgtRotation->addLayout(m_pLytHeadLineRotation);

        m_pLblHeadLineRotation = new QLabel("Rotation:");
        m_pLytHeadLineRotation->addWidget(m_pLblHeadLineRotation);
        m_pSepHeadLineRotation = new QFrame();
        m_pSepHeadLineRotation->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytHeadLineRotation->addWidget(m_pSepHeadLineRotation, 1);

        m_pLytWdgtRotation->addSpacing(4);

        // <Line> Rotation Angle
        //----------------------

        m_pLytLineRotationAngle = new QHBoxLayout();
        m_pLytWdgtRotation->addLayout(m_pLytLineRotationAngle);
        m_pLblRotationAngle = new QLabel("Angle:");
        m_pLblRotationAngle->setFixedWidth(cxLblWidthClm0);
        m_pLytLineRotationAngle->addWidget(m_pLblRotationAngle);

        // <Edit> Rotation Angle
        //----------------------

        m_pLytLineRotationAngle->addSpacing(cxLblWidthClm1 + 4);
        m_pEdtRotationAngle = new QDoubleSpinBox();
        m_pEdtRotationAngle->setFixedWidth(cxEdtWidthClm1);
        m_pEdtRotationAngle->setRange(-360.0, 360.0);
        m_pEdtRotationAngle->setDecimals(1);
        m_pEdtRotationAngle->setSingleStep(0.1);
        m_pEdtRotationAngle->setSuffix(" " + ZS::PhysVal::c_strSymbolDegree);
        //m_pEdtRotationAngle->setValue(m_fRotAngle_deg);
        m_pLytLineRotationAngle->addWidget(m_pEdtRotationAngle);
        m_pLytLineRotationAngle->addStretch();

        if( !connect(
            /* pObjSender   */ m_pEdtRotationAngle,
            /* szSignal     */ SIGNAL(valueChanged(double)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRotationAngleValueChanged(double)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    } // if( m_pGraphObj->hasRotationSelectionPoints() )

    // <Widget> Stacking Order
    //========================

    m_fZValue = m_pGraphObj->getStackingOrderValue();

    m_pWdgtStackingOrder = new QWidget();
    m_pLytWdgtStackingOrder = new QVBoxLayout();
    m_pLytWdgtStackingOrder->setContentsMargins(0, 0, 0, 0);
    m_pWdgtStackingOrder->setLayout(m_pLytWdgtStackingOrder);
    m_pLyt->addWidget(m_pWdgtStackingOrder);

    // <Separator> Z Value
    //----------------------

    m_pLytWdgtStackingOrder->addSpacing(4);

    m_pLytHeadLineStackingOrder = new QHBoxLayout();
    m_pLytWdgtStackingOrder->addLayout(m_pLytHeadLineStackingOrder);

    m_pLblHeadLineStackingOrder = new QLabel("Stacking Order:");
    m_pLytHeadLineStackingOrder->addWidget(m_pLblHeadLineStackingOrder);
    m_pSepHeadLineStackingOrder = new QFrame();
    m_pSepHeadLineStackingOrder->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    m_pLytHeadLineStackingOrder->addWidget(m_pSepHeadLineStackingOrder,1);

    m_pLytWdgtStackingOrder->addSpacing(4);

    // <Line> ZValue Angle
    //----------------------

    m_pLytLineZValue = new QHBoxLayout();
    m_pLytWdgtStackingOrder->addLayout(m_pLytLineZValue);
    m_pLblZValue = new QLabel("ZValue:");
    m_pLblZValue->setFixedWidth(cxLblWidthClm0);
    m_pLytLineZValue->addWidget(m_pLblZValue);

    // <Edit> ZValue Angle
    //----------------------

    m_pLytLineZValue->addSpacing(cxLblWidthClm1 + 4);
    m_pEdtZValue = new QDoubleSpinBox();
    m_pEdtZValue->setFixedWidth(cxEdtWidthClm1);
    m_pEdtZValue->setRange(0.0, 360.0);
    m_pEdtZValue->setDecimals(1);
    m_pEdtZValue->setSingleStep(0.1);
    m_pEdtZValue->setValue(m_fZValue);
    m_pLytLineZValue->addWidget(m_pEdtZValue);
    m_pLytLineZValue->addStretch();

    if( !connect(
        /* pObjSender   */ m_pEdtZValue,
        /* szSignal     */ SIGNAL(valueChanged(double)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtZValueValueChanged(double)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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
CWdgtFormatGraphObjsGeometry::~CWdgtFormatGraphObjsGeometry()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pModelShapePoints;
    }
    catch(...)
    {
    }
    m_pModelShapePoints = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    //m_ptPos;
    //m_size;
    //m_plg.clear();
    m_fRotAngle_deg = 0.0;
    m_fZValue = 0.0;
    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pWdgtPosSize = nullptr;
    m_pLytWdgtPosSize = nullptr;
    m_pLytHeadLineWdgtPosSize = nullptr;
    m_pLblHeadLinePosSize = nullptr;
    m_pSepHeadLinePosSize = nullptr;
    m_pLytLinePos = nullptr;
    m_pLblPos = nullptr;
    m_pLblPosX = nullptr;
    m_pEdtPosX = nullptr;
    m_pLblPosY = nullptr;
    m_pEdtPosY = nullptr;
    m_pLytLineSize = nullptr;
    m_pLblSize = nullptr;
    m_pLblSizeWidth = nullptr;
    m_pEdtSizeWidth = nullptr;
    m_pLblSizeHeight = nullptr;
    m_pEdtSizeHeight = nullptr;
    m_pWdgtShapePoints = nullptr;
    m_pLytWdgtShapePoints = nullptr;
    m_pLytHeadLineShapePoints = nullptr;
    m_pLblHeadLineShapePoints = nullptr;
    m_pSepHeadLineShapePoints = nullptr;
    m_pLytShapePoints = nullptr;
    m_pModelShapePoints = nullptr;
    m_pViewShapePoints = nullptr;
    m_pWdgtShapePointButtons = nullptr;
    m_pLytWdgtShapePointButtons = nullptr;
    m_pBtnShapePointAdd = nullptr;
    m_pBtnShapePointDelete = nullptr;
    m_pBtnShapePointUp = nullptr;
    m_pBtnShapePointDown = nullptr;
    m_bOnModelShapePointsItemChangedInProcess = false;
    m_pWdgtRotation = nullptr;
    m_pLytWdgtRotation = nullptr;
    m_pLytHeadLineRotation = nullptr;
    m_pLblHeadLineRotation = nullptr;
    m_pSepHeadLineRotation = nullptr;
    m_pLytLineRotationAngle = nullptr;
    m_pLblRotationAngle = nullptr;
    m_pEdtRotationAngle = nullptr;
    m_pWdgtStackingOrder = nullptr;
    m_pLytWdgtStackingOrder = nullptr;
    m_pLytHeadLineStackingOrder = nullptr;
    m_pLblHeadLineStackingOrder = nullptr;
    m_pSepHeadLineStackingOrder = nullptr;
    m_pLytLineZValue = nullptr;
    m_pLblZValue = nullptr;
    m_pEdtZValue = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::applyChanges()
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

    // Position and Size
    //==================

    if( m_pWdgtPosSize != nullptr )
    {
        m_pGraphicsItem->setPos(m_ptPos);
        m_pGraphObj->setSize(m_size);
    }

    // Shape Points
    //=============

    if( m_pWdgtShapePoints != nullptr )
    {
        CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
        if( pGraphObjLine != nullptr && m_plg.size() == 2 )
        {
            QPointF pt1 = pGraphObjLine->mapFromParent(m_plg[0]);
            QPointF pt2 = pGraphObjLine->mapFromParent(m_plg[1]);
            QLineF  lin( pt1, pt2 );
            pGraphObjLine->setLine(lin);
        }
        else
        {
            CGraphObjPolyline* pGraphObjPolyline = dynamic_cast<CGraphObjPolyline*>(m_pGraphObj);
            if( pGraphObjPolyline != nullptr )
            {
                pGraphObjPolyline->setPolygon(m_plg);
            }
            else
            {
                CGraphObjConnectionLine* pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(m_pGraphObj);
                if( pGraphObjCnctLine != nullptr )
                {
                    QPolygonF plg = pGraphObjCnctLine->mapFromScene(m_plg);
                    pGraphObjCnctLine->setPolygon(plg);
                }
            }
        }
    }

    // Rotation
    //=========

    if( m_pWdgtRotation != nullptr )
    {
        m_pGraphObj->setRotationAngleInDegree(m_fRotAngle_deg);
    }

    // Stacking Order
    //===============

    if( m_pWdgtStackingOrder != nullptr )
    {
        m_pGraphObj->setStackingOrderValue(m_fZValue);
    }
} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::resetChanges()
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
bool CWdgtFormatGraphObjsGeometry::hasChanges() const
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

    bool bHasChanges = m_bIsChangingValue;

    if( !bHasChanges )
    {
        // Position and Size
        //==================

        if( m_pWdgtPosSize != nullptr )
        {
            QPointF ptPos = m_pGraphicsItem->pos();

            if( m_ptPos != ptPos )
            {
                bHasChanges = true;
            }

            QSizeF siz = m_pGraphObj->getSize();

            if( m_size != siz )
            {
                bHasChanges = true;
            }
        }

        // Shape Points
        //==================

        if( !bHasChanges && m_pWdgtShapePoints != nullptr )
        {
            QPolygonF plg;

            CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
            if( pGraphObjLine != nullptr && m_plg.size() == 2 )
            {
                QLineF  lin = pGraphObjLine->line();
                QPointF pt1 = pGraphObjLine->mapToParent(lin.p1());
                QPointF pt2 = pGraphObjLine->mapToParent(lin.p2());
                plg.append(pt1);
                plg.append(pt2);
            }
            else
            {
                CGraphObjPolyline* pGraphObjPolyline = dynamic_cast<CGraphObjPolyline*>(m_pGraphObj);
                if( pGraphObjPolyline != nullptr )
                {
                    plg = pGraphObjPolyline->polygon();
                }
                else
                {
                    CGraphObjConnectionLine* pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(m_pGraphObj);
                    if( pGraphObjCnctLine != nullptr )
                    {
                        QPolygonF plgTmp = pGraphObjCnctLine->polygon();
                        plg = pGraphObjCnctLine->mapToScene(plgTmp);
                    }
                }
            }

            if( m_plg != plg )
            {
                bHasChanges = true;
            }
        } // if( !bHasChanges && m_pWdgtShapePoints != nullptr )

        // Rotation
        //=========

        if( !bHasChanges && m_pWdgtRotation != nullptr )
        {
            double fRotAngle_deg = m_pGraphObj->getRotationAngleInDegree();

            if( m_fRotAngle_deg != fRotAngle_deg )
            {
                bHasChanges = true;
            }
        }

        // Stacking Order
        //===============

        if( !bHasChanges && m_pWdgtStackingOrder != nullptr )
        {
            double fZValue = m_pGraphObj->getStackingOrderValue();

            if( fZValue != m_fZValue )
            {
                bHasChanges = true;
            }
        }
    } // if( !bHasChanges )

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
void CWdgtFormatGraphObjsGeometry::onGraphObjChanged()
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

    m_ptPos = m_pGraphicsItem->pos();
    m_size = m_pGraphObj->getSize();
    m_fRotAngle_deg = m_pGraphObj->getRotationAngleInDegree();
    m_fZValue = m_pGraphObj->getStackingOrderValue();

    if( m_pEdtPosX != nullptr )
    {
        m_pEdtPosX->setValue(m_ptPos.x());
    }
    if( m_pEdtPosY != nullptr )
    {
        m_pEdtPosY->setValue(m_ptPos.y());
    }

    if( m_pEdtSizeWidth != nullptr )
    {
        m_pEdtSizeWidth->setValue(m_size.width());
    }
    if( m_pEdtSizeHeight != nullptr )
    {
        m_pEdtSizeWidth->setValue(m_size.height());
    }

    if( m_pEdtRotationAngle != nullptr )
    {
        m_pEdtRotationAngle->setValue(m_fRotAngle_deg);
    }

    if( m_pEdtZValue != nullptr )
    {
        m_pEdtZValue->setValue(m_fZValue);
    }

    if( m_pModelShapePoints != nullptr )
    {
        fillShapePointsModel();
    }

} // onGraphObjChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::fillShapePointsModel( )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "fillShapePointsModel",
        /* strAddInfo   */ strMthInArgs );

    if( m_pModelShapePoints != nullptr )
    {
        if( !disconnect(
            /* pObjSender   */ m_pModelShapePoints,
            /* szSignal     */ SIGNAL(itemChanged(QStandardItem*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onModelShapePointsItemChanged(QStandardItem*)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        m_plg.clear();

        QString strRowHeaderMax("Position");
        int     iRowCountMax = -1; // means that there is no maximum number of shape points
        int     idxRow, idxClm;

        QStandardItem* pItemClmX;
        QStandardItem* pItemClmY;
        QPointF        pt;

        CGraphObjLine*           pGraphObjLine     = nullptr;
        CGraphObjPolyline*       pGraphObjPolyline = nullptr;
        CGraphObjConnectionLine* pGraphObjCnctLine = nullptr;

        pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
        if( pGraphObjLine != nullptr )
        {
            iRowCountMax = 2;

            QLineF  lin = pGraphObjLine->line();
            QPointF pt1 = pGraphObjLine->mapToParent(lin.p1());
            QPointF pt2 = pGraphObjLine->mapToParent(lin.p2());

            m_plg.append(pt1);
            m_plg.append(pt2);

            m_pModelShapePoints->setHeaderData( 0, Qt::Vertical, "P1 (Start)", Qt::DisplayRole );
            m_pModelShapePoints->setHeaderData( 1, Qt::Vertical, "P2 (End)", Qt::DisplayRole );
        }
        else
        {
            pGraphObjPolyline = dynamic_cast<CGraphObjPolyline*>(m_pGraphObj);
            if( pGraphObjPolyline != nullptr )
            {
                m_plg = pGraphObjPolyline->polygon();
            }
            else
            {
                pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(m_pGraphObj);
                if( pGraphObjCnctLine != nullptr )
                {
                    QPolygonF plgTmp = pGraphObjCnctLine->polygon();
                    m_plg = pGraphObjCnctLine->mapToScene(plgTmp);
                }
            }
        }

        m_pModelShapePoints->setRowCount(m_plg.size());

        for( idxRow = 0; idxRow < m_plg.size(); idxRow++ )
        {
            pt = m_plg[idxRow];

            //m_pModelShapePoints->setHeaderData( 0, Qt::Vertical, "P" + QString::number(idxRow), Qt::DisplayRole );

            pItemClmX = new QStandardItem();
            pItemClmX->setData( pt.x(), Qt::DisplayRole );
            m_pModelShapePoints->setItem( idxRow, EClmX, pItemClmX );

            pItemClmY = new QStandardItem();
            pItemClmY->setData( pt.y(), Qt::DisplayRole );
            m_pModelShapePoints->setItem( idxRow, EClmY, pItemClmY );

            if( pGraphObjCnctLine != nullptr )
            {
                if( idxRow == 0 || (idxRow == m_plg.size()-1) )
                {
                    pItemClmX->setEditable(false);
                    pItemClmX->setEnabled(false);
                    pItemClmY->setEditable(false);
                    pItemClmY->setEnabled(false);
                }
            }
        }


        // In my opinion it's a bug in Qt that "setHeaderData" is without effect it no items have been added
        // to the model event if "setRowCount" and/or "setColumnCount" has been called before.
        m_pModelShapePoints->setHeaderData( EClmX, Qt::Horizontal, "X", Qt::DisplayRole );
        m_pModelShapePoints->setHeaderData( EClmY, Qt::Horizontal, "Y", Qt::DisplayRole );

        QFont        fnt = m_pViewShapePoints->font();
        QFontMetrics fntmtr(fnt);
        QString      strCoor("XXXX.XX");
        QRect        rctCoor = fntmtr.boundingRect(strCoor);
        QRect        rctRowHeader = fntmtr.boundingRect(strRowHeaderMax);

        int iViewHeight, iViewWidth;

        QMargins margins = m_pViewShapePoints->contentsMargins();

        iViewHeight = margins.top() + margins.bottom(); //+ 2*m_pViewShapePoints->frameWidth()
        iViewWidth = margins.left() + margins.right() + 2*m_pViewShapePoints->frameWidth();

        iViewHeight += m_pViewShapePoints->horizontalHeader()->height();
        iViewWidth  += rctRowHeader.width() + 20;

        for( idxRow = 0; idxRow < m_pModelShapePoints->rowCount(); idxRow++ )
        {
            iViewHeight += m_pViewShapePoints->rowHeight(idxRow);
        }
        for( idxClm = 0; idxClm < m_pModelShapePoints->columnCount(); idxClm++ )
        {
            iViewWidth += rctCoor.width()+20; // incl. Spin Boxes
        }

        // If there is no limit for the number of shape points ..
        if( iRowCountMax < 0 )
        {
            m_pViewShapePoints->setMinimumHeight(iViewHeight);
        }
        // If the number of shape points is limited (e.g. for lines) ..
        else
        {
            m_pViewShapePoints->setMaximumHeight(iViewHeight);
        }

        m_pViewShapePoints->setMaximumWidth(iViewWidth+2);

        for( idxClm = 0; idxClm < m_pModelShapePoints->columnCount(); idxClm++ )
        {
            m_pViewShapePoints->setColumnWidth( idxClm, rctCoor.width()+20 );
        }

        m_pViewShapePoints->show();

        if( !connect(
            /* pObjSender   */ m_pModelShapePoints,
            /* szSignal     */ SIGNAL(itemChanged(QStandardItem*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onModelShapePointsItemChanged(QStandardItem*)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    } // if( m_pModelShapePoints != nullptr )

} // fillShapePointsModel

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtPositionXValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPositionXValueChanged",
        /* strAddInfo   */ strMthInArgs );

    m_ptPos.setX(i_fVal);

} // onEdtPositionXValueChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtPositionYValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtPositionYValueChanged",
        /* strAddInfo   */ strMthInArgs );

    m_ptPos.setY(i_fVal);

} // onEdtPositionYValueChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtSizeWidthValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtSizeWidthValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if( i_fVal != m_size.width() )
    {
        m_bIsChangingValue = true;
    }
    else
    {
        m_bIsChangingValue = false;
    }

} // onEdtSizeWidthValueChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtSizeWidthEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtSizeWidthEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    double fVal = m_pEdtSizeWidth->value();

    if( fVal != m_size.width() )
    {
        double fFacX = 1.0;

        if( m_size.width() != 0.0 )
        {
            fFacX = fVal / m_size.width();
        }

        m_size.setWidth(fVal);

        if( m_pModelShapePoints != nullptr )
        {
            if( !disconnect(
                /* pObjSender   */ m_pModelShapePoints,
                /* szSignal     */ SIGNAL(itemChanged(QStandardItem*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onModelShapePointsItemChanged(QStandardItem*)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
        }

        QPointF     pt;
        QModelIndex modelIdx;
        int         idxPt;

        for( idxPt = 0; idxPt < m_plg.size(); idxPt++ )
        {
            pt = m_plg[idxPt];
            pt.setX( fFacX * pt.x() );
            m_plg[idxPt] = pt;

            if( m_pModelShapePoints != nullptr )
            {
                modelIdx = m_pModelShapePoints->index( idxPt, EClmX, QModelIndex() );
                m_pModelShapePoints->setData( modelIdx, pt.x() );
            }
        }

        if( m_pModelShapePoints != nullptr )
        {
            if( !connect(
                /* pObjSender   */ m_pModelShapePoints,
                /* szSignal     */ SIGNAL(itemChanged(QStandardItem*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onModelShapePointsItemChanged(QStandardItem*)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
        }

    } // if( i_fVal != m_size.width() )

    m_bIsChangingValue = false;

} // onEdtSizeWidthEditingFinished

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtSizeHeightValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtSizeHeightValueChanged",
        /* strAddInfo   */ strMthInArgs );

    if( i_fVal != m_size.height() )
    {
        m_bIsChangingValue = true;
    }
    else
    {
        m_bIsChangingValue = false;
    }

} // onEdtSizeHeightValueChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtSizeHeightEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtSizeHeightEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    double fVal = m_pEdtSizeHeight->value();

    if( fVal != m_size.height() )
    {
        double fFacY = 1.0;

        if( m_size.height() != 0.0 )
        {
            fFacY = fVal / m_size.height();
        }

        m_size.setHeight(fVal);

        if( m_pModelShapePoints != nullptr )
        {
            if( !disconnect(
                /* pObjSender   */ m_pModelShapePoints,
                /* szSignal     */ SIGNAL(itemChanged(QStandardItem*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onModelShapePointsItemChanged(QStandardItem*)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
        }

        QPointF     pt;
        QModelIndex modelIdx;
        int         idxPt;

        for( idxPt = 0; idxPt < m_plg.size(); idxPt++ )
        {
            pt = m_plg[idxPt];
            pt.setY( fFacY * pt.y() );
            m_plg[idxPt] = pt;

            if( m_pModelShapePoints != nullptr )
            {
                modelIdx = m_pModelShapePoints->index( idxPt, EClmY, QModelIndex() );
                m_pModelShapePoints->setData( modelIdx, pt.y() );
            }
        }

        if( m_pModelShapePoints != nullptr )
        {
            if( !connect(
                /* pObjSender   */ m_pModelShapePoints,
                /* szSignal     */ SIGNAL(itemChanged(QStandardItem*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onModelShapePointsItemChanged(QStandardItem*)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
        }

    } // if( i_fVal != m_size.height() )

    m_bIsChangingValue = false;

} // onEdtSizeHeightEditingFinished

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onModelShapePointsItemChanged( QStandardItem* i_pItem )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onModelShapePointsItemChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_bOnModelShapePointsItemChangedInProcess )
    {
        return;
    }
    m_bOnModelShapePointsItemChangedInProcess = true;

    //int iClm = i_pItem->column();
    int iRow = i_pItem->row();

    QVariant varData = i_pItem->data(Qt::EditRole);
    //double fVal    = varData.toDouble();

    if( m_plg.size() > 0 && iRow >= 0 && iRow <= m_plg.size() )
    {
        QModelIndex modelIdxXCoor = m_pModelShapePoints->index( iRow, EClmX, QModelIndex() );
        QModelIndex modelIdxYCoor = m_pModelShapePoints->index( iRow, EClmY, QModelIndex() );

        QPointF pt;

        pt.setX( m_pModelShapePoints->data(modelIdxXCoor).toDouble() );
        pt.setY( m_pModelShapePoints->data(modelIdxYCoor).toDouble() );

        m_plg[iRow] = pt;

        m_size = m_plg.boundingRect().size();

        if( m_pEdtSizeWidth != nullptr )
        {
            if( !disconnect(
                /* pObjSender   */ m_pEdtSizeWidth,
                /* szSignal     */ SIGNAL(valueChanged(double)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeWidthValueChanged(double)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
            if( !disconnect(
                /* pObjSender   */ m_pEdtSizeWidth,
                /* szSignal     */ SIGNAL(editingFinished()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeWidthEditingFinished()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            m_pEdtSizeWidth->setValue(m_size.width());

            if( !connect(
                /* pObjSender   */ m_pEdtSizeWidth,
                /* szSignal     */ SIGNAL(valueChanged(double)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeWidthValueChanged(double)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
            if( !connect(
                /* pObjSender   */ m_pEdtSizeWidth,
                /* szSignal     */ SIGNAL(editingFinished()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeWidthEditingFinished()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
        }

        if( m_pEdtSizeHeight != nullptr )
        {
            if( !disconnect(
                /* pObjSender   */ m_pEdtSizeHeight,
                /* szSignal     */ SIGNAL(valueChanged(double)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeHeightValueChanged(double)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
            if( !disconnect(
                /* pObjSender   */ m_pEdtSizeHeight,
                /* szSignal     */ SIGNAL(editingFinished()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeHeightEditingFinished()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            m_pEdtSizeHeight->setValue(m_size.height());

            if( !connect(
                /* pObjSender   */ m_pEdtSizeHeight,
                /* szSignal     */ SIGNAL(valueChanged(double)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeHeightValueChanged(double)) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
            if( !connect(
                /* pObjSender   */ m_pEdtSizeHeight,
                /* szSignal     */ SIGNAL(editingFinished()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onEdtSizeHeightEditingFinished()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }
        }

    } // if( m_plg.size() > 0 && iRow >= 0 && iRow <= m_plg.size() )

    m_bOnModelShapePointsItemChangedInProcess = false;

} // onModelShapePointsItemChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onBtnShapePointAddClicked()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnShapePointAddClicked",
        /* strAddInfo   */ strMthInArgs );

    QItemSelectionModel* pItemSelModel = m_pViewShapePoints->selectionModel();
    int                  idxRow = m_pModelShapePoints->rowCount();
    bool                 bRowInserted = false;

    QStandardItem* pItemClmX;
    QStandardItem* pItemClmY;

    if( pItemSelModel->hasSelection() )
    {
        QModelIndexList modelIdxList = pItemSelModel->selectedIndexes();

        if( modelIdxList.size() == 1 )
        {
            idxRow = modelIdxList[0].row()+1;
        }
    }

    if( idxRow == 0 )
    {
        pItemClmX = m_pModelShapePoints->item(idxRow,EClmX);
        if( pItemClmX != nullptr && !pItemClmX->isEditable() )
        {
            idxRow = 1;
        }
    }
    else if( idxRow == m_plg.size() )
    {
        if( m_plg.size() > 0 )
        {
            pItemClmX = m_pModelShapePoints->item(idxRow-1,EClmX);
            if( pItemClmX != nullptr && !pItemClmX->isEditable() )
            {
                idxRow = m_plg.size()-1;
            }
        }
    }

    if( idxRow >= 0 && idxRow < m_plg.size() )
    {
        m_plg.insert( idxRow, QPointF(0.0,0.0) );
        bRowInserted = true;
    }
    else if( idxRow == m_plg.size() )
    {
        m_plg.append( QPointF(0.0,0.0) );
        bRowInserted = true;
    }

    if( bRowInserted )
    {
        QPointF pt;

        m_pModelShapePoints->setRowCount(m_plg.size());

        for( ; idxRow < m_plg.size(); idxRow++ )
        {
            pt = m_plg[idxRow];

            pItemClmX = new QStandardItem();
            pItemClmX->setData( pt.x(), Qt::DisplayRole );
            m_pModelShapePoints->setItem( idxRow, EClmX, pItemClmX );

            pItemClmY = new QStandardItem();
            pItemClmY->setData( pt.y(), Qt::DisplayRole );
            m_pModelShapePoints->setItem( idxRow, EClmY, pItemClmY );
        }
    }

} // onBtnShapePointAddClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onBtnShapePointDeleteClicked()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnShapePointDeleteClicked",
        /* strAddInfo   */ strMthInArgs );

    QItemSelectionModel* pItemSelModel = m_pViewShapePoints->selectionModel();

    if( pItemSelModel->hasSelection() )
    {
        QModelIndexList modelIdxList = pItemSelModel->selectedIndexes();
        QModelIndex     modelIdx;
        QStandardItem*  pItemClmX;
        int             idxLst;
        int             idxRow;
        bool            bRowRemoved;

        if( modelIdxList.size() > 0 )
        {
            for( idxLst = modelIdxList.size()-1; idxLst >= 0; idxLst-- )
            {
                modelIdx = modelIdxList[idxLst];
                idxRow = modelIdx.row();
                bRowRemoved = false;

                if( idxRow == 0 )
                {
                    pItemClmX = m_pModelShapePoints->item(idxRow,EClmX);
                    if( pItemClmX != nullptr && pItemClmX->isEditable() )
                    {
                        m_pModelShapePoints->removeRow( idxRow, QModelIndex() );
                        bRowRemoved = true;
                    }
                }
                else if( idxRow > 0 && idxRow < m_plg.size() )
                {
                    m_pModelShapePoints->removeRow( idxRow, QModelIndex() );
                    bRowRemoved = true;
                }
                else if( idxRow == m_plg.size() )
                {
                    if( m_plg.size() > 0 )
                    {
                        pItemClmX = m_pModelShapePoints->item(idxRow-1,EClmX);
                        if( pItemClmX != nullptr && pItemClmX->isEditable() )
                        {
                            m_pModelShapePoints->removeRow( idxRow, QModelIndex() );
                            bRowRemoved = true;
                        }
                    }
                }
            }

            if( bRowRemoved )
            {
                if( idxRow >= 0 && idxRow < m_plg.size() )
                {
                    m_plg.remove(idxRow);
                }

                m_size = m_plg.boundingRect().size();

                if( m_pEdtSizeWidth != nullptr )
                {
                    if( !disconnect(
                        /* pObjSender   */ m_pEdtSizeWidth,
                        /* szSignal     */ SIGNAL(valueChanged(double)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onEdtSizeWidthValueChanged(double)) ) )
                    {
                        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }
                    if( !disconnect(
                        /* pObjSender   */ m_pEdtSizeWidth,
                        /* szSignal     */ SIGNAL(editingFinished()),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onEdtSizeWidthEditingFinished()) ) )
                    {
                        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }

                    m_pEdtSizeWidth->setValue(m_size.width());

                    if( !connect(
                        /* pObjSender   */ m_pEdtSizeWidth,
                        /* szSignal     */ SIGNAL(valueChanged(double)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onEdtSizeWidthValueChanged(double)) ) )
                    {
                        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }
                    if( !connect(
                        /* pObjSender   */ m_pEdtSizeWidth,
                        /* szSignal     */ SIGNAL(editingFinished()),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onEdtSizeWidthEditingFinished()) ) )
                    {
                        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }
                }

                if( m_pEdtSizeHeight != nullptr )
                {
                    if( !disconnect(
                        /* pObjSender   */ m_pEdtSizeHeight,
                        /* szSignal     */ SIGNAL(valueChanged(double)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onEdtSizeHeightValueChanged(double)) ) )
                    {
                        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }
                    if( !disconnect(
                        /* pObjSender   */ m_pEdtSizeHeight,
                        /* szSignal     */ SIGNAL(editingFinished()),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onEdtSizeHeightEditingFinished()) ) )
                    {
                        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }

                    m_pEdtSizeHeight->setValue(m_size.height());

                    if( !connect(
                        /* pObjSender   */ m_pEdtSizeHeight,
                        /* szSignal     */ SIGNAL(valueChanged(double)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onEdtSizeHeightValueChanged(double)) ) )
                    {
                        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }
                    if( !connect(
                        /* pObjSender   */ m_pEdtSizeHeight,
                        /* szSignal     */ SIGNAL(editingFinished()),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onEdtSizeHeightEditingFinished()) ) )
                    {
                        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                    }
                }

            } // if( bRowRemoved )
        } // if( modelIdxList.size() > 0 )
    } // if( pItemSelModel->hasSelection() )

} // onBtnShapePointDeleteClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onBtnShapePointUpClicked()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnShapePointUpClicked",
        /* strAddInfo   */ strMthInArgs );

    if( m_pModelShapePoints->rowCount() > 1 && m_plg.size() > 1 )
    {
        QItemSelectionModel* pItemSelModel = m_pViewShapePoints->selectionModel();

        QModelIndexList modelIdxList = pItemSelModel->selectedIndexes();

        if( modelIdxList.size() == 1 )
        {
            QModelIndex modelIdx = modelIdxList[0];
            int         idxRowSelected = modelIdx.row();

            if( idxRowSelected > 0 )
            {
                QStandardItem* pItemSelectedClmX;
                QPointF        ptSelected;
                int            idxRowOther = idxRowSelected-1;
                QStandardItem* pItemOtherClmX;
                QPointF        ptOther;

                pItemSelectedClmX = m_pModelShapePoints->item(idxRowSelected,EClmX);
                pItemOtherClmX = m_pModelShapePoints->item(idxRowOther,EClmX);

                if( pItemSelectedClmX->isEditable() && pItemOtherClmX->isEditable() )
                {
                    ptSelected = m_plg[idxRowSelected];
                    ptOther = m_plg[idxRowOther];

                    m_plg[idxRowOther] = ptSelected;
                    m_plg[idxRowSelected] = ptOther;

                    modelIdx = m_pModelShapePoints->index( idxRowOther, EClmX, QModelIndex() );
                    m_pModelShapePoints->setData( modelIdx, ptSelected.x() );
                    modelIdx = m_pModelShapePoints->index( idxRowOther, EClmY, QModelIndex() );
                    m_pModelShapePoints->setData( modelIdx, ptSelected.y() );

                    modelIdx = m_pModelShapePoints->index( idxRowSelected, EClmX, QModelIndex() );
                    m_pModelShapePoints->setData( modelIdx, ptOther.x() );
                    modelIdx = m_pModelShapePoints->index( idxRowSelected, EClmY, QModelIndex() );
                    m_pModelShapePoints->setData( modelIdx, ptOther.y() );

                } // if( pItemSelectedClmX->isEditable() && pItemOtherClmX->isEditable() )
            } // if( idxRowSelected > 0 )
        } // if( modelIdxList.size() == 1 )
    } // if( m_pModelShapePoints->rowCount() > 1 && m_plg.size() > 1 )

} // onBtnShapePointUpClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onBtnShapePointDownClicked()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnShapePointDownClicked",
        /* strAddInfo   */ strMthInArgs );

    if( m_pModelShapePoints->rowCount() > 1 && m_plg.size() > 1 )
    {
        QItemSelectionModel* pItemSelModel = m_pViewShapePoints->selectionModel();

        QModelIndexList modelIdxList = pItemSelModel->selectedIndexes();

        if( modelIdxList.size() == 1 )
        {
            QModelIndex modelIdx = modelIdxList[0];
            int         idxRowSelected = modelIdx.row();

            if( idxRowSelected < m_plg.size()-1 )
            {
                QStandardItem* pItemSelectedClmX;
                QPointF        ptSelected;
                int            idxRowOther = idxRowSelected+1;
                QStandardItem* pItemOtherClmX;
                QPointF        ptOther;

                pItemSelectedClmX = m_pModelShapePoints->item(idxRowSelected,EClmX);
                pItemOtherClmX = m_pModelShapePoints->item(idxRowOther,EClmX);

                if( pItemSelectedClmX->isEditable() && pItemOtherClmX->isEditable() )
                {
                    ptSelected = m_plg[idxRowSelected];
                    ptOther = m_plg[idxRowOther];

                    m_plg[idxRowOther] = ptSelected;
                    m_plg[idxRowSelected] = ptOther;

                    modelIdx = m_pModelShapePoints->index( idxRowOther, EClmX, QModelIndex() );
                    m_pModelShapePoints->setData( modelIdx, ptSelected.x() );
                    modelIdx = m_pModelShapePoints->index( idxRowOther, EClmY, QModelIndex() );
                    m_pModelShapePoints->setData( modelIdx, ptSelected.y() );

                    modelIdx = m_pModelShapePoints->index( idxRowSelected, EClmX, QModelIndex() );
                    m_pModelShapePoints->setData( modelIdx, ptOther.x() );
                    modelIdx = m_pModelShapePoints->index( idxRowSelected, EClmY, QModelIndex() );
                    m_pModelShapePoints->setData( modelIdx, ptOther.y() );

                } // if( pItemSelectedClmX->isEditable() && pItemOtherClmX->isEditable() )
            } // if( idxRowSelected < m_plg.size()-1 )
        } // if( modelIdxList.size() == 1 )
    } // if( m_pModelShapePoints->rowCount() > 1 && m_plg.size() > 1 )

} // onBtnShapePointDownClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtRotationAngleValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtRotationAngleValueChanged",
        /* strAddInfo   */ strMthInArgs );

    m_fRotAngle_deg = i_fVal;

} // onEdtRotationAngleValueChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtZValueValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtZValueValueChanged",
        /* strAddInfo   */ strMthInArgs );

    m_fZValue = i_fVal;

} // onEdtZValueValueChanged
