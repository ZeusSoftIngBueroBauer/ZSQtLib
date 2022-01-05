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
#include "ZSSysGUI/ZSSysPushButton.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::Trace;


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
    m_pWdgtPosition(nullptr),
    m_pLytWdgtPosition(nullptr),
    m_pLytLinePosition(nullptr),
    m_pLblLinePosition(nullptr),
    m_pSepLinePosition(nullptr),
    m_pLytPositionX(nullptr),
    m_pLblPositionX(nullptr),
    m_pEdtPositionX(nullptr),
    m_pLytPositionY(nullptr),
    m_pLblPositionY(nullptr),
    m_pEdtPositionY(nullptr),
    m_pWdgtSize(nullptr),
    m_pLytWdgtSize(nullptr),
    m_pLytLineSize(nullptr),
    m_pLblLineSize(nullptr),
    m_pSepLineSize(nullptr),
    m_pLytSizeWidth(nullptr),
    m_pLblSizeWidth(nullptr),
    m_pEdtSizeWidth(nullptr),
    m_pLytSizeHeight(nullptr),
    m_pLblSizeHeight(nullptr),
    m_pEdtSizeHeight(nullptr),
    m_pWdgtShapePoints(nullptr),
    m_pLytWdgtShapePoints(nullptr),
    m_pLytLineShapePoints(nullptr),
    m_pLblLineShapePoints(nullptr),
    m_pSepLineShapePoints(nullptr),
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
    m_pLytLineRotation(nullptr),
    m_pLblLineRotation(nullptr),
    m_pSepLineRotation(nullptr),
    m_pLytRotationAngle(nullptr),
    m_pLblRotationAngle(nullptr),
    m_pEdtRotationAngle(nullptr),
    m_pWdgtZValue(nullptr),
    m_pLytWdgtZValue(nullptr),
    m_pLytLineZValue(nullptr),
    m_pLblLineZValue(nullptr),
    m_pSepLineZValue(nullptr),
    m_pLytZValue(nullptr),
    m_pLblZValue(nullptr),
    m_pEdtZValue(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsGeometry");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QRectF rctScene = m_pDrawingScene->sceneRect();

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 90;
    //int cxEdtWidth = 80;

    // Headline
    //=========

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Geometry");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Widget> Position
    //==================

    if( m_pGraphObj != nullptr )
    {
        m_ptPos = m_pGraphicsItem->pos();

        switch( m_pGraphObj->getType() )
        {
            case EGraphObjTypeLine:
            case EGraphObjTypeConnectionLine:
            {
                break;
            }
            case EGraphObjTypePoint:
            case EGraphObjTypeRect:
            case EGraphObjTypeEllipse:
            case EGraphObjTypePolygon:
            case EGraphObjTypePolyline:
            case EGraphObjTypeText:
            case EGraphObjTypeImage:
            case EGraphObjTypeConnectionPoint:
            case EGraphObjTypeGroup:
            case EGraphObjTypeUserDefined:
            default:
            {
                m_pWdgtPosition = new QWidget();
                m_pLytWdgtPosition = new QVBoxLayout();
                m_pLytWdgtPosition->setContentsMargins(0,0,0,0);
                m_pWdgtPosition->setLayout(m_pLytWdgtPosition);
                m_pLyt->addWidget(m_pWdgtPosition);
                break;
            }

        } // switch( m_pGraphObj->getType() )

    } // if( m_pGraphObj != nullptr )

    if( m_pWdgtPosition != nullptr )
    {
        // <Separator> Position
        //----------------------

        m_pLytWdgtPosition->addSpacing(4);

        m_pLytLinePosition = new QHBoxLayout();
        m_pLytWdgtPosition->addLayout(m_pLytLinePosition);

        m_pLblLinePosition = new QLabel("Position:");
        m_pLytLinePosition->addWidget(m_pLblLinePosition);
        m_pSepLinePosition = new QFrame();
        m_pSepLinePosition->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytLinePosition->addWidget(m_pSepLinePosition,1);

        m_pLytWdgtPosition->addSpacing(4);

        // <Edit> Position X
        //------------------

        m_pLytPositionX = new QHBoxLayout();
        m_pLytWdgtPosition->addLayout(m_pLytPositionX);
        m_pLblPositionX = new QLabel("X:");
        m_pLblPositionX->setFixedWidth(cxLblWidth);
        m_pLytPositionX->addWidget(m_pLblPositionX);
        m_pEdtPositionX = new QDoubleSpinBox();
        m_pEdtPositionX->setRange(0.0,rctScene.right());
        m_pEdtPositionX->setDecimals(1);
        m_pEdtPositionX->setSingleStep(0.1);
        m_pEdtPositionX->setSuffix(" px");
        m_pEdtPositionX->setValue(m_ptPos.x());
        m_pLytPositionX->addWidget(m_pEdtPositionX);
        m_pLytPositionX->addStretch();

        if( !connect(
            /* pObjSender   */ m_pEdtPositionX,
            /* szSignal     */ SIGNAL(valueChanged(double)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtPositionXValueChanged(double)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <Edit> Position Y
        //------------------

        m_pLytPositionY = new QHBoxLayout();
        m_pLytWdgtPosition->addLayout(m_pLytPositionY);
        m_pLblPositionY = new QLabel("Y:");
        m_pLblPositionY->setFixedWidth(cxLblWidth);
        m_pLytPositionY->addWidget(m_pLblPositionY);
        m_pEdtPositionY = new QDoubleSpinBox();
        m_pEdtPositionY->setRange(0.0,rctScene.right());
        m_pEdtPositionY->setDecimals(1);
        m_pEdtPositionY->setSingleStep(0.1);
        m_pEdtPositionY->setSuffix(" px");
        m_pEdtPositionY->setValue(m_ptPos.y());
        m_pLytPositionY->addWidget(m_pEdtPositionY);
        m_pLytPositionY->addStretch();

        if( !connect(
            /* pObjSender   */ m_pEdtPositionY,
            /* szSignal     */ SIGNAL(valueChanged(double)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtPositionYValueChanged(double)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

    } // if( m_pWdgtPosition != nullptr )

    // <Widget> Size
    //==================

    if( m_pGraphObj != nullptr )
    {
        m_size = m_pGraphObj->getSize();

        switch( m_pGraphObj->getType() )
        {
            case EGraphObjTypePoint:
            case EGraphObjTypeLine:
            case EGraphObjTypeText:
            case EGraphObjTypeConnectionPoint:
            case EGraphObjTypeConnectionLine:
            {
                break;
            }
            case EGraphObjTypeRect:
            case EGraphObjTypeEllipse:
            case EGraphObjTypeImage:
            case EGraphObjTypeGroup:
            case EGraphObjTypePolygon:
            case EGraphObjTypePolyline:
            case EGraphObjTypeUserDefined:
            default:
            {
                m_pWdgtSize = new QWidget();
                m_pLytWdgtSize = new QVBoxLayout();
                m_pLytWdgtSize->setContentsMargins(0,0,0,0);
                m_pWdgtSize->setLayout(m_pLytWdgtSize);
                m_pLyt->addWidget(m_pWdgtSize);
                break;
            }

        } // switch( m_pGraphObj->getType() )

    } // if( m_pGraphObj != nullptr )

    if( m_pWdgtSize != nullptr )
    {
        // <Separator> Size
        //-----------------

        m_pLytWdgtSize->addSpacing(4);

        m_pLytLineSize = new QHBoxLayout();
        m_pLytWdgtSize->addLayout(m_pLytLineSize);

        m_pLblLineSize = new QLabel("Size:");
        m_pLytLineSize->addWidget(m_pLblLineSize);
        m_pSepLineSize = new QFrame();
        m_pSepLineSize->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytLineSize->addWidget(m_pSepLineSize,1);

        m_pLytWdgtSize->addSpacing(4);

        // <Edit> Size Width
        //-------------------

        m_pLytSizeWidth = new QHBoxLayout();
        m_pLytWdgtSize->addLayout(m_pLytSizeWidth);
        m_pLblSizeWidth = new QLabel("Width:");
        m_pLblSizeWidth->setFixedWidth(cxLblWidth);
        m_pLytSizeWidth->addWidget(m_pLblSizeWidth);
        m_pEdtSizeWidth = new QDoubleSpinBox();
        m_pEdtSizeWidth->setRange(0.0,rctScene.right());
        m_pEdtSizeWidth->setDecimals(1);
        m_pEdtSizeWidth->setSingleStep(0.1);
        m_pEdtSizeWidth->setSuffix(" px");
        m_pEdtSizeWidth->setValue(m_size.width());
        m_pLytSizeWidth->addWidget(m_pEdtSizeWidth);
        m_pLytSizeWidth->addStretch();

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

        m_pLytSizeHeight = new QHBoxLayout();
        m_pLytWdgtSize->addLayout(m_pLytSizeHeight);
        m_pLblSizeHeight = new QLabel("Height:");
        m_pLblSizeHeight->setFixedWidth(cxLblWidth);
        m_pLytSizeHeight->addWidget(m_pLblSizeHeight);
        m_pEdtSizeHeight = new QDoubleSpinBox();
        m_pEdtSizeHeight->setRange(0.0,rctScene.right());
        m_pEdtSizeHeight->setDecimals(1);
        m_pEdtSizeHeight->setSingleStep(0.1);
        m_pEdtSizeHeight->setSuffix(" px");
        m_pEdtSizeHeight->setValue(m_size.height());
        m_pLytSizeHeight->addWidget(m_pEdtSizeHeight);
        m_pLytSizeHeight->addStretch();

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

    } // if( m_pWdgtSize != nullptr )

    // <Widget> Shape Points
    //======================

    if( m_pGraphObj != nullptr )
    {
        switch( m_pGraphObj->getType() )
        {
            case EGraphObjTypeLine:
            case EGraphObjTypePolygon:
            case EGraphObjTypePolyline:
            case EGraphObjTypeConnectionLine:
            {
                m_pWdgtShapePoints = new QWidget();
                m_pLytWdgtShapePoints = new QVBoxLayout();
                m_pLytWdgtShapePoints->setContentsMargins(0,0,0,0);
                m_pWdgtShapePoints->setLayout(m_pLytWdgtShapePoints);
                m_pLyt->addWidget(m_pWdgtShapePoints);
                break;
            }
            case EGraphObjTypePoint:
            case EGraphObjTypeRect:
            case EGraphObjTypeEllipse:
            case EGraphObjTypeText:
            case EGraphObjTypeImage:
            case EGraphObjTypeConnectionPoint:
            case EGraphObjTypeGroup:
            case EGraphObjTypeUserDefined:
            default:
            {
                break;
            }

        } // switch( m_pGraphObj->getType() )

    } // if( m_pGraphObj != nullptr )

    if( m_pWdgtShapePoints != nullptr )
    {
        // <Separator> ShapePoints
        //------------------------

        m_pLytWdgtShapePoints->addSpacing(4);

        m_pLytLineShapePoints = new QHBoxLayout();
        m_pLytWdgtShapePoints->addLayout(m_pLytLineShapePoints);

        m_pLblLineShapePoints = new QLabel("Shape Points:");
        m_pLytLineShapePoints->addWidget(m_pLblLineShapePoints);
        m_pSepLineShapePoints = new QFrame();
        m_pSepLineShapePoints->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytLineShapePoints->addWidget(m_pSepLineShapePoints,1);

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

        fillShapePointsModel();

        // <Widget> Shape Point Buttons
        //-----------------------------

        switch( m_pGraphObj->getType() )
        {
            case EGraphObjTypePolygon:
            case EGraphObjTypePolyline:
            case EGraphObjTypeConnectionLine:
            {
                m_pWdgtShapePointButtons = new QWidget();
                m_pLytWdgtShapePointButtons = new QVBoxLayout();
                m_pLytWdgtShapePointButtons->setContentsMargins(10,10,0,0);
                m_pWdgtShapePointButtons->setLayout(m_pLytWdgtShapePointButtons);
                m_pLytShapePoints->addWidget(m_pWdgtShapePointButtons);
                m_pLytShapePoints->addStretch();
                break;
            }
            case EGraphObjTypePoint:
            case EGraphObjTypeLine:
            case EGraphObjTypeRect:
            case EGraphObjTypeEllipse:
            case EGraphObjTypeText:
            case EGraphObjTypeImage:
            case EGraphObjTypeConnectionPoint:
            case EGraphObjTypeGroup:
            case EGraphObjTypeUserDefined:
            default:
            {
                break;
            }
        }

        if( m_pWdgtShapePointButtons != nullptr )
        {
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

        } // if( m_pWdgtShapePointButtons != nullptr )

    } // if( m_pWdgtShapePoints != nullptr )

    // <Widget> Rotation
    //======================

    if( m_pGraphObj != nullptr )
    {
        m_fRotAngle_deg = m_pGraphObj->getRotationAngleInDegree();

        switch( m_pGraphObj->getType() )
        {
            case EGraphObjTypePoint:
            case EGraphObjTypeLine:
            case EGraphObjTypeConnectionPoint:
            case EGraphObjTypeConnectionLine:
            {
                break;
            }
            case EGraphObjTypeRect:
            case EGraphObjTypeEllipse:
            case EGraphObjTypePolygon:
            case EGraphObjTypePolyline:
            case EGraphObjTypeText:
            case EGraphObjTypeImage:
            case EGraphObjTypeGroup:
            case EGraphObjTypeUserDefined:
            default:
            {
                m_pWdgtRotation = new QWidget();
                m_pLytWdgtRotation = new QVBoxLayout();
                m_pLytWdgtRotation->setContentsMargins(0,0,0,0);
                m_pWdgtRotation->setLayout(m_pLytWdgtRotation);
                m_pLyt->addWidget(m_pWdgtRotation);
                break;
            }

        } // switch( m_pGraphObj->getType() )

    } // if( m_pGraphObj != nullptr )

    if( m_pWdgtRotation != nullptr )
    {
        // <Separator> Rotation
        //----------------------

        m_pLytWdgtRotation->addSpacing(4);

        m_pLytLineRotation = new QHBoxLayout();
        m_pLytWdgtRotation->addLayout(m_pLytLineRotation);

        m_pLblLineRotation = new QLabel("Rotation:");
        m_pLytLineRotation->addWidget(m_pLblLineRotation);
        m_pSepLineRotation = new QFrame();
        m_pSepLineRotation->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytLineRotation->addWidget(m_pSepLineRotation,1);

        m_pLytWdgtRotation->addSpacing(4);

        // <Edit> Rotation Angle
        //----------------------

        m_pLytRotationAngle = new QHBoxLayout();
        m_pLytWdgtRotation->addLayout(m_pLytRotationAngle);
        m_pLblRotationAngle = new QLabel("Rotation Angle:");
        m_pLblRotationAngle->setFixedWidth(cxLblWidth);
        m_pLytRotationAngle->addWidget(m_pLblRotationAngle);
        m_pEdtRotationAngle = new QDoubleSpinBox();
        m_pEdtRotationAngle->setRange(0.0,360.0);
        m_pEdtRotationAngle->setDecimals(1);
        m_pEdtRotationAngle->setSingleStep(0.1);
        m_pEdtRotationAngle->setSuffix(" °");
        m_pEdtRotationAngle->setValue(m_fRotAngle_deg);
        m_pLytRotationAngle->addWidget(m_pEdtRotationAngle);
        m_pLytRotationAngle->addStretch();

        if( !connect(
            /* pObjSender   */ m_pEdtRotationAngle,
            /* szSignal     */ SIGNAL(valueChanged(double)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRotationAngleValueChanged(double)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

    } // if( m_pWdgtRotation != nullptr )

    // <Widget> Z Value
    //======================

    if( m_pGraphObj != nullptr )
    {
        m_fZValue = m_pGraphObj->getStackingOrderValue();

        m_pWdgtZValue = new QWidget();
        m_pLytWdgtZValue = new QVBoxLayout();
        m_pLytWdgtZValue->setContentsMargins(0,0,0,0);
        m_pWdgtZValue->setLayout(m_pLytWdgtZValue);
        m_pLyt->addWidget(m_pWdgtZValue);
    }

    if( m_pWdgtZValue != nullptr )
    {
        // <Separator> Z Value
        //----------------------

        m_pLytWdgtZValue->addSpacing(4);

        m_pLytLineZValue = new QHBoxLayout();
        m_pLytWdgtZValue->addLayout(m_pLytLineZValue);

        m_pLblLineZValue = new QLabel("Z-Value:");
        m_pLytLineZValue->addWidget(m_pLblLineZValue);
        m_pSepLineZValue = new QFrame();
        m_pSepLineZValue->setFrameStyle(QFrame::HLine|QFrame::Sunken);
        m_pLytLineZValue->addWidget(m_pSepLineZValue,1);

        m_pLytWdgtZValue->addSpacing(4);

        // <Edit> ZValue Angle
        //----------------------

        m_pLytZValue = new QHBoxLayout();
        m_pLytWdgtZValue->addLayout(m_pLytZValue);
        m_pLblZValue = new QLabel("ZValue :");
        m_pLblZValue->setFixedWidth(cxLblWidth);
        m_pLytZValue->addWidget(m_pLblZValue);
        m_pEdtZValue = new QDoubleSpinBox();
        m_pEdtZValue->setRange(0.0,360.0);
        m_pEdtZValue->setDecimals(1);
        m_pEdtZValue->setSingleStep(0.1);
        m_pEdtZValue->setValue(m_fZValue);
        m_pLytZValue->addWidget(m_pEdtZValue);
        m_pLytZValue->addStretch();

        if( !connect(
            /* pObjSender   */ m_pEdtZValue,
            /* szSignal     */ SIGNAL(valueChanged(double)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtZValueValueChanged(double)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

    } // if( m_pWdgtZValue != nullptr )

    // <Stretch> at bottom of Widget
    //------------------------------

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsGeometry::~CWdgtFormatGraphObjsGeometry()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pWdgtPosition = nullptr;
    m_pLytWdgtPosition = nullptr;
    m_pLytLinePosition = nullptr;
    m_pLblLinePosition = nullptr;
    m_pSepLinePosition = nullptr;
    m_pLytPositionX = nullptr;
    m_pLblPositionX = nullptr;
    m_pEdtPositionX = nullptr;
    m_pLytPositionY = nullptr;
    m_pLblPositionY = nullptr;
    m_pEdtPositionY = nullptr;
    m_pWdgtSize = nullptr;
    m_pLytWdgtSize = nullptr;
    m_pLytLineSize = nullptr;
    m_pLblLineSize = nullptr;
    m_pSepLineSize = nullptr;
    m_pLytSizeWidth = nullptr;
    m_pLblSizeWidth = nullptr;
    m_pEdtSizeWidth = nullptr;
    m_pLytSizeHeight = nullptr;
    m_pLblSizeHeight = nullptr;
    m_pEdtSizeHeight = nullptr;
    m_pWdgtShapePoints = nullptr;
    m_pLytWdgtShapePoints = nullptr;
    m_pLytLineShapePoints = nullptr;
    m_pLblLineShapePoints = nullptr;
    m_pSepLineShapePoints = nullptr;
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
    m_pLytLineRotation = nullptr;
    m_pLblLineRotation = nullptr;
    m_pSepLineRotation = nullptr;
    m_pLytRotationAngle = nullptr;
    m_pLblRotationAngle = nullptr;
    m_pEdtRotationAngle = nullptr;
    m_pWdgtZValue = nullptr;
    m_pLytZValue = nullptr;
    m_pLytLineZValue = nullptr;
    m_pLblLineZValue = nullptr;
    m_pSepLineZValue = nullptr;
    m_pLytZValue = nullptr;
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "applyChanges",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pGraphObj != nullptr )
    {
        // Position
        //==================

        if( m_pWdgtPosition != nullptr )
        {
            m_pGraphicsItem->setPos(m_ptPos);
        }

        // Size
        //==================

        if( m_pWdgtSize != nullptr )
        {
            m_pGraphObj->setSize(m_size);
        }

        // Shape Points
        //==================

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
        //==================

        if( m_pWdgtRotation != nullptr )
        {
            m_pGraphObj->setRotationAngleInDegree(m_fRotAngle_deg);
        }

        // Z Value
        //==================

        if( m_pWdgtZValue != nullptr )
        {
            m_pGraphObj->setStackingOrderValue(m_fZValue);
        }

    } // if( m_pGraphObj != nullptr )

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::resetChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resetChanges",
        /* strAddInfo   */ strAddTrcInfo );

    m_ptPos = QPointF(0.0,0.0);
    m_size = QSizeF(0.0,0.0);
    m_plg.clear();
    m_fRotAngle_deg = 0.0;
    m_fZValue = 0.0;

    if( m_pGraphObj != nullptr )
    {
        m_ptPos = m_pGraphicsItem->pos();
        m_size = m_pGraphObj->getSize();
        m_fRotAngle_deg = m_pGraphObj->getRotationAngleInDegree();
        m_fZValue = m_pGraphObj->getStackingOrderValue();

        if( m_pEdtPositionX != nullptr )
        {
            m_pEdtPositionX->setValue(m_ptPos.x());
        }
        if( m_pEdtPositionY != nullptr )
        {
            m_pEdtPositionY->setValue(m_ptPos.y());
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

    } // if( m_pGraphObj != nullptr )

    fillShapePointsModel();

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsGeometry::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = m_bIsChangingValue;

    if( !bHasChanges && m_pGraphObj != nullptr )
    {
        // Position
        //==================

        if( m_pWdgtPosition != nullptr )
        {
            QPointF ptPos = m_pGraphicsItem->pos();

            if( m_ptPos != ptPos )
            {
                bHasChanges = true;
            }
        }

        // Size
        //==================

        if( !bHasChanges && m_pWdgtSize != nullptr )
        {
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
        //==================

        if( !bHasChanges && m_pWdgtRotation != nullptr )
        {
            double fRotAngle_deg = m_pGraphObj->getRotationAngleInDegree();

            if( m_fRotAngle_deg != fRotAngle_deg )
            {
                bHasChanges = true;
            }
        }

        // Z Value
        //==================

        if( !bHasChanges && m_pWdgtZValue != nullptr )
        {
            double fZValue = m_pGraphObj->getStackingOrderValue();

            if( fZValue != m_fZValue )
            {
                bHasChanges = true;
            }
        }

    } // if( m_pGraphObj != nullptr )

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::fillShapePointsModel( )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillShapePointsModel",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pGraphObj != nullptr && m_pModelShapePoints != nullptr )
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

    } // if( m_pGraphObj != nullptr && m_pModelShapePoints != nullptr )

} // fillShapePointsModel

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtPositionXValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtPositionXValueChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_ptPos.setX(i_fVal);

} // onEdtPositionXValueChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtPositionYValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtPositionYValueChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_ptPos.setY(i_fVal);

} // onEdtPositionYValueChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtSizeWidthValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtSizeWidthValueChanged",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtSizeWidthEditingFinished",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtSizeHeightValueChanged",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtSizeHeightEditingFinished",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onModelShapePointsItemChanged",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnShapePointAddClicked",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnShapePointDeleteClicked",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnShapePointUpClicked",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnShapePointDownClicked",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtRotationAngleValueChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_fRotAngle_deg = i_fVal;

} // onEdtRotationAngleValueChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsGeometry::onEdtZValueValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtZValueValueChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_fZValue = i_fVal;

} // onEdtZValueValueChanged
