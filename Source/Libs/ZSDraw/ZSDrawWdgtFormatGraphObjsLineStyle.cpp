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

#include <QtCore/qglobal.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcolordialog.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSDraw/ZSDrawWdgtFormatGraphObjsLineStyle.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
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
class CWdgtFormatGraphObjsLineStyle : public CWdgtFormatGraphObjs
*******************************************************************************/

static QRect s_rctPxmLineEndSize(0,0,24,24);

static QLine s_arlineLineEndSizes[] = {
    /* ELinePoint::Start */ QLine( 5, s_rctPxmLineEndSize.center().y(), s_rctPxmLineEndSize.right()-4, s_rctPxmLineEndSize.center().y() ),
    /* ELinePoint::End   */ QLine( 4, s_rctPxmLineEndSize.center().y(), s_rctPxmLineEndSize.right()-5, s_rctPxmLineEndSize.center().y() )
};


/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsLineStyle::CWdgtFormatGraphObjsLineStyle(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytPenColor(nullptr),
    m_pLblPenColor(nullptr),
    m_pPxmBtnPenColor(nullptr),
    m_rctBtnPenColor(0,12,16,4),
    m_lineBtnPenColor1( QPoint(0,2), QPoint(8,2) ),
    m_lineBtnPenColor2( QPoint(0,2), QPoint(0,10) ),
    m_lineBtnPenColor3( QPoint(0,10), QPoint(5,10) ),
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
    m_pModelLineEndSizesLineEnd(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsLineStyle");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 60;
    //int cxEdtWidth = 80;

    QFont fntHeadLine;

    if( m_pGraphObj != nullptr && m_pGraphObj->getType() == EGraphObjTypePoint )
    {
        m_pLblHeadLine = new QLabel("Pen Style");
    }
    else
    {
        m_pLblHeadLine = new QLabel("Line Style");
    }

    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Button> Line Color
    //--------------------

    if( m_drawSettings.isPenUsed() )
    {
        m_pLytPenColor = new QHBoxLayout();
        m_pLyt->addLayout(m_pLytPenColor);
        m_pLblPenColor = new QLabel("Color:");
        m_pLblPenColor->setFixedWidth(cxLblWidth);
        m_pLytPenColor->addWidget(m_pLblPenColor);
        m_pBtnPenColor = new QPushButton();
        m_pLytPenColor->addWidget(m_pBtnPenColor);
        m_pLytPenColor->addStretch();

        m_pPxmBtnPenColor = new QPixmap(":/ZS/Draw/DrawToolPen16x16.bmp");
        m_pPxmBtnPenColor->setMask(m_pPxmBtnPenColor->createHeuristicMask());

        if( !connect(
            /* pObjSender   */ m_pBtnPenColor,
            /* szSignal     */ SIGNAL(clicked()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnPenColorClicked()) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    // <EditField> Line Width
    //-----------------------

    if( m_drawSettings.isPenUsed() )
    {
        m_pLytPenWidth = new QHBoxLayout();
        m_pLyt->addLayout(m_pLytPenWidth);
        m_pLblPenWidth = new QLabel("Width/px:");
        m_pLblPenWidth->setFixedWidth(cxLblWidth);
        m_pLytPenWidth->addWidget(m_pLblPenWidth);
        m_pEdtPenWidth = new QLineEdit();
        m_pLytPenWidth->addWidget(m_pEdtPenWidth);
        m_pLytPenWidth->addStretch();

        if( !connect(
            /* pObjSender   */ m_pEdtPenWidth,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtPenWidthTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    // <Widget> Line Style
    //-----------------------

    if( m_drawSettings.isLineStyleUsed() )
    {
        m_pWdgtLineStyle = new QWidget();
        m_pLytWdgtLineStyle = new QVBoxLayout();
        m_pLytWdgtLineStyle->setContentsMargins(0,0,0,0);
        m_pWdgtLineStyle->setLayout(m_pLytWdgtLineStyle);
        m_pLyt->addWidget(m_pWdgtLineStyle);

        // <ComboBox> Line Style
        //-----------------------

        m_pLytLineStyle = new QHBoxLayout();
        m_pLytWdgtLineStyle->addLayout(m_pLytLineStyle);
        m_pLblLineStyle = new QLabel("Style:");
        m_pLblLineStyle->setFixedWidth(cxLblWidth);
        m_pLytLineStyle->addWidget(m_pLblLineStyle);
        m_pBtnLineStyle = new CPushButton();
        m_pBtnLineStyle->setFixedWidth(72);
        m_pBtnLineStyle->setFixedHeight(24);
        m_pLytLineStyle->addWidget(m_pBtnLineStyle);
        m_pLytLineStyle->addStretch();

        m_pModelLineStyles = new QStandardItemModel(CEnumLineStyle::count(),1);

        fillLineStylesModel(m_pModelLineStyles);

        if( !connect(
            /* pObjSender   */ m_pBtnLineStyle,
            /* szSignal     */ SIGNAL(clicked()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnLineStyleClicked()) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        // <Widget> Line End Settings
        //---------------------------

        if( m_drawSettings.isLineEndStyleUsed() )
        {
            m_pWdgtLineEndSettings = new QWidget();
            m_pLytWdgtLineEndSettings = new QVBoxLayout();
            m_pLytWdgtLineEndSettings->setContentsMargins(0,0,0,0);
            m_pWdgtLineEndSettings->setLayout(m_pLytWdgtLineEndSettings);
            m_pLyt->addWidget(m_pWdgtLineEndSettings);

            // <Separator> Line End Styles
            //----------------------------

            m_pLytWdgtLineEndSettings->addSpacing(4);

            m_pLytLineEndSettings = new QHBoxLayout();
            m_pLytWdgtLineEndSettings->addLayout(m_pLytLineEndSettings);

            m_pLblLineEndSettings = new QLabel("Line End Styles:");
            m_pLytLineEndSettings->addWidget(m_pLblLineEndSettings);
            m_pSepLineEndSettings = new QFrame();
            m_pSepLineEndSettings->setFrameStyle(QFrame::HLine|QFrame::Sunken);
            m_pLytLineEndSettings->addWidget(m_pSepLineEndSettings,1);

            m_pLytWdgtLineEndSettings->addSpacing(4);

            // <ComboBoxes> Line End Styles
            //-----------------------------

            m_pLytLineEndStyles = new QHBoxLayout();
            m_pLytWdgtLineEndSettings->addLayout(m_pLytLineEndStyles);

            // <ComboBoxes> Line End Styles - Line Start
            //------------------------------------------

            m_pLblLineEndStylesLineStart = new QLabel("Start Style:");
            m_pLblLineEndStylesLineStart->setFixedWidth(cxLblWidth);
            m_pLytLineEndStyles->addWidget(m_pLblLineEndStylesLineStart);
            m_pBtnLineEndStylesLineStart = new CPushButton();
            m_pBtnLineEndStylesLineStart->setFixedWidth(32);
            m_pBtnLineEndStylesLineStart->setFixedHeight(28);
            m_pLytLineEndStyles->addWidget(m_pBtnLineEndStylesLineStart);
            m_pLytLineEndStyles->addSpacing(10);

            m_pModelLineEndStylesLineStart = new QStandardItemModel();

            fillLineEndStylesModel( ELinePoint::Start, m_pModelLineEndStylesLineStart );

            if( !connect(
                /* pObjSender   */ m_pBtnLineEndStylesLineStart,
                /* szSignal     */ SIGNAL(clicked()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnLineEndStylesLineStartClicked()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            // <ComboBoxes> Line End Styles - Line End
            //----------------------------------------

            m_pLblLineEndStylesLineEnd = new QLabel("End Style:");
            m_pLblLineEndStylesLineEnd->setFixedWidth(cxLblWidth);
            m_pLytLineEndStyles->addWidget(m_pLblLineEndStylesLineEnd);
            m_pBtnLineEndStylesLineEnd = new CPushButton();
            m_pBtnLineEndStylesLineEnd->setFixedWidth(32);
            m_pBtnLineEndStylesLineEnd->setFixedHeight(28);
            m_pLytLineEndStyles->addWidget(m_pBtnLineEndStylesLineEnd);
            m_pLytLineEndStyles->addStretch();

            m_pModelLineEndStylesLineEnd = new QStandardItemModel();

            fillLineEndStylesModel( ELinePoint::End, m_pModelLineEndStylesLineEnd );

            if( !connect(
                /* pObjSender   */ m_pBtnLineEndStylesLineEnd,
                /* szSignal     */ SIGNAL(clicked()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnLineEndStylesLineEndClicked()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            // <ComboBoxes> Line End Sizes
            //----------------------------

            m_pLytLineEndSizes = new QHBoxLayout();
            m_pLytWdgtLineEndSettings->addLayout(m_pLytLineEndSizes);

            // <ComboBoxes> Line End Sizes - Line Start
            //-----------------------------------------

            m_pLblLineEndSizesLineStart = new QLabel("Start Size:");
            m_pLblLineEndSizesLineStart->setFixedWidth(cxLblWidth);
            m_pLytLineEndSizes->addWidget(m_pLblLineEndSizesLineStart);
            m_pBtnLineEndSizesLineStart = new CPushButton();
            m_pBtnLineEndSizesLineStart->setFixedWidth(32);
            m_pBtnLineEndSizesLineStart->setFixedHeight(28);
            m_pLytLineEndSizes->addWidget(m_pBtnLineEndSizesLineStart);
            m_pLytLineEndSizes->addSpacing(10);

            m_pModelLineEndSizesLineStart = new QStandardItemModel();

            fillLineEndSizesModel( ELinePoint::Start, m_pModelLineEndSizesLineStart );

            if( !connect(
                /* pObjSender   */ m_pBtnLineEndSizesLineStart,
                /* szSignal     */ SIGNAL(clicked()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnLineEndSizesLineStartClicked()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

            // <ComboBoxes> Line End Sizes - Line End
            //---------------------------------------

            m_pLblLineEndSizesLineEnd = new QLabel("End Size:");
            m_pLblLineEndSizesLineEnd->setFixedWidth(cxLblWidth);
            m_pLytLineEndSizes->addWidget(m_pLblLineEndSizesLineEnd);
            m_pBtnLineEndSizesLineEnd = new CPushButton();
            m_pBtnLineEndSizesLineEnd->setFixedWidth(32);
            m_pBtnLineEndSizesLineEnd->setFixedHeight(28);
            m_pLytLineEndSizes->addWidget(m_pBtnLineEndSizesLineEnd);
            m_pLytLineEndSizes->addStretch();

            m_pModelLineEndSizesLineEnd = new QStandardItemModel();

            fillLineEndSizesModel( ELinePoint::End, m_pModelLineEndSizesLineEnd );

            if( !connect(
                /* pObjSender   */ m_pBtnLineEndSizesLineEnd,
                /* szSignal     */ SIGNAL(clicked()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnLineEndSizesLineEndClicked()) ) )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
            }

        } // if( m_drawSettings.isLineEndStyleUsed() )

    } // if( m_drawSettings.isLineStyleUsed() )

    // <Stretch> at bottom of widget
    //------------------------------

    m_pLyt->addStretch();

    // Set settings at GUI controls
    //-----------------------------

    setPenColor(m_drawSettings);

    if( m_pEdtPenWidth != nullptr )
    {
        m_pEdtPenWidth->setText( QString::number(m_drawSettings.getPenWidth()) );
    }

    if( m_pWdgtLineStyle != nullptr )
    {
        if( m_drawSettings.isLineStyleUsed() )
        {
            m_pWdgtLineStyle->show();

            setLineStyle(m_drawSettings);
        }
        else
        {
            m_pWdgtLineStyle->hide();
        }
    }

    if( m_pWdgtLineEndSettings != nullptr )
    {
        if( m_drawSettings.isLineEndStyleUsed() )
        {
            m_pWdgtLineEndSettings->show();

            setLineEndStyle( ELinePoint::Start, m_drawSettings );
            setLineEndStyle( ELinePoint::End, m_drawSettings );

            setLineEndSize( ELinePoint::Start, m_drawSettings );
            setLineEndSize( ELinePoint::End, m_drawSettings );
        }
        else
        {
            m_pWdgtLineEndSettings->hide();
        }
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsLineStyle::~CWdgtFormatGraphObjsLineStyle()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pPxmBtnPenColor;
    }
    catch(...)
    {
    }
    m_pPxmBtnPenColor = nullptr;

    try
    {
        delete m_pViewLineStyles;
    }
    catch(...)
    {
    }
    m_pViewLineStyles = nullptr;

    try
    {
        delete m_pModelLineStyles;
    }
    catch(...)
    {
    }
    m_pModelLineStyles = nullptr;

    try
    {
        delete m_pViewLineEndStylesLineStart;
    }
    catch(...)
    {
    }
    m_pViewLineEndStylesLineStart = nullptr;

    try
    {
        delete m_pModelLineEndStylesLineStart;
    }
    catch(...)
    {
    }
    m_pModelLineEndStylesLineStart = nullptr;

    try
    {
        delete m_pViewLineEndStylesLineEnd;
    }
    catch(...)
    {
    }
    m_pViewLineEndStylesLineEnd = nullptr;

    try
    {
        delete m_pModelLineEndStylesLineEnd;
    }
    catch(...)
    {
    }
    m_pModelLineEndStylesLineEnd = nullptr;

    try
    {
        delete m_pViewLineEndSizesLineStart;
    }
    catch(...)
    {
    }
    m_pViewLineEndSizesLineStart = nullptr;

    try
    {
        delete m_pModelLineEndSizesLineStart;
    }
    catch(...)
    {
    }
    m_pModelLineEndSizesLineStart = nullptr;

    try
    {
        delete m_pViewLineEndSizesLineEnd;
    }
    catch(...)
    {
    }
    m_pViewLineEndSizesLineEnd = nullptr;

    try
    {
        delete m_pModelLineEndSizesLineEnd;
    }
    catch(...)
    {
    }
    m_pModelLineEndSizesLineEnd = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pLytPenColor = nullptr;
    m_pLblPenColor = nullptr;
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

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::applyChanges()
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
        m_pGraphObj->setPenColor( m_drawSettings.getPenColor(), false );
        m_pGraphObj->setPenWidth( m_drawSettings.getPenWidth(), false );

        if( m_drawSettings.isLineStyleUsed() )
        {
            m_pGraphObj->setLineStyle( m_drawSettings.getLineStyle(), false );
        }

        if( m_drawSettings.isLineEndStyleUsed() )
        {
            m_pGraphObj->setLineEndStyle( ELinePoint::Start, m_drawSettings.getLineEndStyle(ELinePoint::Start), false );
            m_pGraphObj->setLineEndBaseLineType( ELinePoint::Start, m_drawSettings.getLineEndBaseLineType(ELinePoint::Start), false );
            m_pGraphObj->setLineEndFillStyle( ELinePoint::Start, m_drawSettings.getLineEndFillStyle(ELinePoint::Start), false );
            m_pGraphObj->setLineEndWidth( ELinePoint::Start, m_drawSettings.getLineEndWidth(ELinePoint::Start), false );
            m_pGraphObj->setLineEndLength( ELinePoint::Start, m_drawSettings.getLineEndLength(ELinePoint::Start), false );
            m_pGraphObj->setLineEndStyle( ELinePoint::End, m_drawSettings.getLineEndStyle(ELinePoint::End), false );
            m_pGraphObj->setLineEndBaseLineType( ELinePoint::End, m_drawSettings.getLineEndBaseLineType(ELinePoint::End), false );
            m_pGraphObj->setLineEndFillStyle( ELinePoint::End, m_drawSettings.getLineEndFillStyle(ELinePoint::End), false );
            m_pGraphObj->setLineEndWidth( ELinePoint::End, m_drawSettings.getLineEndWidth(ELinePoint::End), false );
            m_pGraphObj->setLineEndLength( ELinePoint::End, m_drawSettings.getLineEndLength(ELinePoint::End), false );
        }

        m_pGraphObj->onDrawSettingsChanged();

    } // if( m_pGraphObj != nullptr )

    else if( /*m_pGraphObj == nullptr &&*/ m_pDrawingScene != nullptr )
    {
        QList<QGraphicsItem*> arpGraphicsItemsSelected = m_pDrawingScene->selectedItems();

        // If graphical objects are selected ...
        if( arpGraphicsItemsSelected.size() > 0 )
        {
            // .. all selected graphical objects will be changed according to the new settings.
            QGraphicsItem* pGraphicsItem;
            CGraphObj*     pGraphObj;
            int            idxGraphObj;

            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                if( pGraphObj != nullptr )
                {
                    pGraphObj->setPenColor( m_drawSettings.getPenColor(), false );
                    pGraphObj->setPenWidth( m_drawSettings.getPenWidth(), false );
                    pGraphObj->setLineStyle( m_drawSettings.getLineStyle(), false );
                    pGraphObj->setLineEndStyle( ELinePoint::Start, m_drawSettings.getLineEndStyle(ELinePoint::Start), false );
                    pGraphObj->setLineEndBaseLineType( ELinePoint::Start, m_drawSettings.getLineEndBaseLineType(ELinePoint::Start), false );
                    pGraphObj->setLineEndFillStyle( ELinePoint::Start, m_drawSettings.getLineEndFillStyle(ELinePoint::Start), false );
                    pGraphObj->setLineEndWidth( ELinePoint::Start, m_drawSettings.getLineEndWidth(ELinePoint::Start), false );
                    pGraphObj->setLineEndLength( ELinePoint::Start, m_drawSettings.getLineEndLength(ELinePoint::Start), false );
                    pGraphObj->setLineEndStyle( ELinePoint::End, m_drawSettings.getLineEndStyle(ELinePoint::End), false );
                    pGraphObj->setLineEndBaseLineType( ELinePoint::End, m_drawSettings.getLineEndBaseLineType(ELinePoint::End), false );
                    pGraphObj->setLineEndFillStyle( ELinePoint::End, m_drawSettings.getLineEndFillStyle(ELinePoint::End), false );
                    pGraphObj->setLineEndWidth( ELinePoint::End, m_drawSettings.getLineEndWidth(ELinePoint::End), false );
                    pGraphObj->setLineEndLength( ELinePoint::End, m_drawSettings.getLineEndLength(ELinePoint::End), false );
                    pGraphObj->onDrawSettingsChanged();
                }
            }
        } // if( arpGraphicsItemsSelected.size() > 0 )

        // If no graphical object is selected ...
        else // if( arpGraphicsItemsSelected.size() == 0 )
        {
            // .. set default attribute for following draw commands.
            m_pDrawingScene->setPenColor( m_drawSettings.getPenColor() );
            m_pDrawingScene->setPenWidth( m_drawSettings.getPenWidth() );
            m_pDrawingScene->setLineStyle( m_drawSettings.getLineStyle() );
            m_pDrawingScene->setLineEndStyle( ELinePoint::Start, m_drawSettings.getLineEndStyle(ELinePoint::Start) );
            m_pDrawingScene->setLineEndBaseLineType( ELinePoint::Start, m_drawSettings.getLineEndBaseLineType(ELinePoint::Start) );
            m_pDrawingScene->setLineEndFillStyle( ELinePoint::Start, m_drawSettings.getLineEndFillStyle(ELinePoint::Start) );
            m_pDrawingScene->setLineEndWidth( ELinePoint::Start, m_drawSettings.getLineEndWidth(ELinePoint::Start) );
            m_pDrawingScene->setLineEndLength( ELinePoint::Start, m_drawSettings.getLineEndLength(ELinePoint::Start) );
            m_pDrawingScene->setLineEndStyle( ELinePoint::End, m_drawSettings.getLineEndStyle(ELinePoint::End) );
            m_pDrawingScene->setLineEndBaseLineType( ELinePoint::End, m_drawSettings.getLineEndBaseLineType(ELinePoint::End) );
            m_pDrawingScene->setLineEndFillStyle( ELinePoint::End, m_drawSettings.getLineEndFillStyle(ELinePoint::End) );
            m_pDrawingScene->setLineEndWidth( ELinePoint::End, m_drawSettings.getLineEndWidth(ELinePoint::End) );
            m_pDrawingScene->setLineEndLength( ELinePoint::End, m_drawSettings.getLineEndLength(ELinePoint::End) );

        } // if( arpGraphicsItemsSelected.size() == 0 )

    } // if( m_pGraphObj == nullptr && m_pDrawingScene != nullptr )

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::resetChanges()
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

    if( m_pGraphObj != nullptr )
    {
        m_drawSettings = m_pGraphObj->getDrawSettings();
    }
    else if( m_pDrawingScene != nullptr )
    {
        m_drawSettings = m_pDrawingScene->getDrawSettings();
    }

    // Set settings at GUI controls
    //-----------------------------

    setPenColor(m_drawSettings);

    if( m_pEdtPenWidth != nullptr )
    {
        m_pEdtPenWidth->setText( QString::number(m_drawSettings.getPenWidth()) );
    }

    if( m_drawSettings.isLineStyleUsed() )
    {
        setLineStyle(m_drawSettings);
    }

    if( m_drawSettings.isLineEndStyleUsed() )
    {
        setLineEndStyle( ELinePoint::Start, m_drawSettings );
        setLineEndStyle( ELinePoint::End, m_drawSettings );

        setLineEndSize( ELinePoint::Start, m_drawSettings );
        setLineEndSize( ELinePoint::End, m_drawSettings );
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsLineStyle::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    if( m_pGraphObj != nullptr )
    {
        if( m_pGraphObj->getPenColor() != m_drawSettings.getPenColor() )
        {
            bHasChanges = true;
        }
        else if( m_pGraphObj->getPenWidth() != m_drawSettings.getPenWidth() )
        {
            bHasChanges = true;
        }
        if( !bHasChanges && m_drawSettings.isLineStyleUsed() )
        {
            if( m_pGraphObj->getLineStyle() != m_drawSettings.getLineStyle() )
            {
                bHasChanges = true;
            }
        }
        if( !bHasChanges && m_drawSettings.isLineEndStyleUsed() )
        {
            if( m_pGraphObj->getLineEndStyle( ELinePoint::Start) != m_drawSettings.getLineEndStyle(ELinePoint::Start) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndBaseLineType(ELinePoint::Start) != m_drawSettings.getLineEndBaseLineType(ELinePoint::Start) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndFillStyle(ELinePoint::Start) != m_drawSettings.getLineEndFillStyle(ELinePoint::Start) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndWidth(ELinePoint::Start) != m_drawSettings.getLineEndWidth(ELinePoint::Start) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndLength(ELinePoint::Start) != m_drawSettings.getLineEndLength(ELinePoint::Start) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndStyle(ELinePoint::End) != m_drawSettings.getLineEndStyle(ELinePoint::End) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndBaseLineType(ELinePoint::End) != m_drawSettings.getLineEndBaseLineType(ELinePoint::End) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndFillStyle(ELinePoint::End) != m_drawSettings.getLineEndFillStyle(ELinePoint::End) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndWidth(ELinePoint::End) != m_drawSettings.getLineEndWidth(ELinePoint::End) )
            {
                bHasChanges = true;
            }
            else if( m_pGraphObj->getLineEndLength(ELinePoint::End) != m_drawSettings.getLineEndLength(ELinePoint::End) )
            {
                bHasChanges = true;
            }
        }
    }

    else if( m_pDrawingScene != nullptr )
    {
        if( m_pDrawingScene->getPenColor() != m_drawSettings.getPenColor() )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getPenWidth() != m_drawSettings.getPenWidth() )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineStyle() != m_drawSettings.getLineStyle() )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndStyle( ELinePoint::Start) != m_drawSettings.getLineEndStyle(ELinePoint::Start) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndBaseLineType(ELinePoint::Start) != m_drawSettings.getLineEndBaseLineType(ELinePoint::Start) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndFillStyle(ELinePoint::Start) != m_drawSettings.getLineEndFillStyle(ELinePoint::Start) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndWidth(ELinePoint::Start) != m_drawSettings.getLineEndWidth(ELinePoint::Start) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndLength(ELinePoint::Start) != m_drawSettings.getLineEndLength(ELinePoint::Start) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndStyle(ELinePoint::End) != m_drawSettings.getLineEndStyle(ELinePoint::End) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndBaseLineType(ELinePoint::End) != m_drawSettings.getLineEndBaseLineType(ELinePoint::End) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndFillStyle(ELinePoint::End) != m_drawSettings.getLineEndFillStyle(ELinePoint::End) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndWidth(ELinePoint::End) != m_drawSettings.getLineEndWidth(ELinePoint::End) )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getLineEndLength(ELinePoint::End) != m_drawSettings.getLineEndLength(ELinePoint::End) )
        {
            bHasChanges = true;
        }
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::fillLineStylesModel( QStandardItemModel* i_pModel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillLineStylesModel",
        /* strAddInfo   */ strAddTrcInfo );

    QPixmap        pxmLineStyle(64,16);
    QStandardItem* pLineStyleItem;
    CEnumLineStyle lineStyle;

    QPainter   painter;
    QPen       pen;

    pen.setColor(Qt::black);
    pen.setWidth(3);

    for( lineStyle = 0; lineStyle < CEnumLineStyle::count(); lineStyle++ )
    {
        if( lineStyle != ELineStyle::NoLine )
        {
            pxmLineStyle.fill(Qt::white);

            painter.begin(&pxmLineStyle);
            pen.setStyle( lineStyle2QtPenStyle(lineStyle.enumerator()) );
            painter.setPen(pen);
            painter.drawLine( 4, pxmLineStyle.height()/2, pxmLineStyle.width()-4, pxmLineStyle.height()/2 );
            painter.end();

            pLineStyleItem = new QStandardItem();
            pLineStyleItem->setData( pxmLineStyle, Qt::DecorationRole );
        }
        else
        {
            pLineStyleItem = new QStandardItem(" No Line");
        }

        pLineStyleItem->setData( static_cast<int>(lineStyle.enumerator()), EItemDataRoleLineStyle );

        i_pModel->setItem( static_cast<int>(lineStyle.enumerator()), 0, pLineStyleItem );
        pLineStyleItem = nullptr;
    }

} // fillLineStylesModel

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::fillLineEndStylesModel( ELinePoint i_linePoint, QStandardItemModel* i_pModel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillLineEndStylesModel",
        /* strAddInfo   */ strAddTrcInfo );

    QPixmap        pxmLineEnd(24,24);
    QPainter       painter;
    QPen           pen;
    QBrush         brsh;
    QStandardItem* pLineEndItem;
    int            idxRow;
    int            idxClm;

    QRect  rctPxm = pxmLineEnd.rect();

    QLine  line;
    QPoint ptArrowHead1;
    QPoint ptArrowHead2;
    QPoint ptArrowHead3;
    QPoint ptArrowHead4;

    if( i_linePoint == ELinePoint::Start )
    {
        line = QLine( 5, rctPxm.center().y(), rctPxm.right()-4, rctPxm.center().y() );
        ptArrowHead1 = QPoint( line.p1().x()+7, line.p1().y()+6 );
        ptArrowHead2 = QPoint( line.p1().x()-2, line.p1().y()   );
        ptArrowHead3 = QPoint( line.p1().x()+7, line.p1().y()-6 );
        ptArrowHead4 = QPoint( line.p1().x()+3, line.p1().y()   );
    }
    else // if( i_linePoint == ELinePoint::End )
    {
        line = QLine( 4, rctPxm.center().y(), rctPxm.right()-5, rctPxm.center().y() );
        ptArrowHead1 = QPoint( line.p2().x()-7, line.p2().y()+6 );
        ptArrowHead2 = QPoint( line.p2().x()+2, line.p2().y()   );
        ptArrowHead3 = QPoint( line.p2().x()-7, line.p2().y()-6 );
        ptArrowHead4 = QPoint( line.p2().x()-3, line.p2().y()   );
    }

    QPoint arPts[4] = { ptArrowHead1, ptArrowHead2, ptArrowHead3, ptArrowHead4 };

    // ELineEndStyle::Normal
    //======================

    idxRow = 0;
    idxClm = 0;

    pxmLineEnd.fill(Qt::white);
    painter.begin(&pxmLineEnd);

    pen.setColor(Qt::black);
    pen.setWidth(3);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(line);

    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData( pxmLineEnd, Qt::DecorationRole );
    pLineEndItem->setData( static_cast<int>(ELineEndStyle::Normal), EItemDataRoleLineEndStyle );
    pLineEndItem->setData( static_cast<int>(ELineEndBaseLineType::NoLine), EItemDataRoleLineEndBaseLineType );
    pLineEndItem->setData( static_cast<int>(ELineEndFillStyle::NoFill), EItemDataRoleLineEndFillStyle );
    i_pModel->setItem(idxRow,idxClm,pLineEndItem);
    pLineEndItem = nullptr;

    // ELineEndStyle::ArrowHead
    //=======================

    // ELineEndBaseLineType::NoLine && ELineEndFillStyle::NoFill
    //----------------------------------------------------------

    idxRow = 0;
    idxClm = 1;

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
    painter.drawPolyline(arPts,3);

    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData( pxmLineEnd, Qt::DecorationRole );
    pLineEndItem->setData( static_cast<int>(ELineEndStyle::ArrowHead), EItemDataRoleLineEndStyle );
    pLineEndItem->setData( static_cast<int>(ELineEndBaseLineType::NoLine), EItemDataRoleLineEndBaseLineType );
    pLineEndItem->setData( static_cast<int>(ELineEndFillStyle::NoFill), EItemDataRoleLineEndFillStyle );
    i_pModel->setItem(idxRow,idxClm,pLineEndItem);
    pLineEndItem = nullptr;

    // ELineEndBaseLineType::Normal && ELineEndFillStyle::NoFill
    //------------------------------------------------------

    if( i_linePoint == ELinePoint::Start )
    {
        line.setP1( QPoint( ptArrowHead1.x(), rctPxm.center().y() ) );
    }
    else // if( i_linePoint == ELinePoint::End )
    {
        line.setP2( QPoint( ptArrowHead1.x(), rctPxm.center().y() ) );
    }

    idxRow = 1;
    idxClm = 0;

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
    painter.drawPolygon(arPts,3);

    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData( pxmLineEnd, Qt::DecorationRole );
    pLineEndItem->setData( static_cast<int>(ELineEndStyle::ArrowHead), EItemDataRoleLineEndStyle );
    pLineEndItem->setData( static_cast<int>(ELineEndBaseLineType::Normal), EItemDataRoleLineEndBaseLineType );
    pLineEndItem->setData( static_cast<int>(ELineEndFillStyle::NoFill), EItemDataRoleLineEndFillStyle );
    i_pModel->setItem(idxRow,idxClm,pLineEndItem);
    pLineEndItem = nullptr;

    // ELineEndBaseLineType::Normal && ELineEndFillStyle::SolidPattern
    //------------------------------------------------------------

    idxRow = 1;
    idxClm = 1;

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
    painter.drawPolygon(arPts,3);

    painter.end();

    pLineEndItem = new QStandardItem();
    pLineEndItem->setData( pxmLineEnd, Qt::DecorationRole );
    pLineEndItem->setData( static_cast<int>(ELineEndStyle::ArrowHead), EItemDataRoleLineEndStyle );
    pLineEndItem->setData( static_cast<int>(ELineEndBaseLineType::Normal), EItemDataRoleLineEndBaseLineType );
    pLineEndItem->setData( static_cast<int>(ELineEndFillStyle::SolidPattern), EItemDataRoleLineEndFillStyle );
    i_pModel->setItem(idxRow,idxClm,pLineEndItem);
    pLineEndItem = nullptr;

    // ELineEndBaseLineType::Indented && ELineEndFillStyle::NoFill
    //--------------------------------------------------------

    idxRow = 2;
    idxClm = 0;

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
    pLineEndItem->setData( pxmLineEnd, Qt::DecorationRole );
    pLineEndItem->setData( static_cast<int>(ELineEndStyle::ArrowHead), EItemDataRoleLineEndStyle );
    pLineEndItem->setData( static_cast<int>(ELineEndBaseLineType::Indented), EItemDataRoleLineEndBaseLineType );
    pLineEndItem->setData( static_cast<int>(ELineEndFillStyle::NoFill), EItemDataRoleLineEndFillStyle );
    i_pModel->setItem(idxRow,idxClm,pLineEndItem);
    pLineEndItem = nullptr;

    // ELineEndBaseLineType::Indented && ELineEndFillStyle::SolidPattern
    //--------------------------------------------------------------

    idxRow = 2;
    idxClm = 1;

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
    pLineEndItem->setData( pxmLineEnd, Qt::DecorationRole );
    pLineEndItem->setData( static_cast<int>(ELineEndStyle::ArrowHead), EItemDataRoleLineEndStyle );
    pLineEndItem->setData( static_cast<int>(ELineEndBaseLineType::Indented), EItemDataRoleLineEndBaseLineType );
    pLineEndItem->setData( static_cast<int>(ELineEndFillStyle::SolidPattern), EItemDataRoleLineEndFillStyle );
    i_pModel->setItem(idxRow,idxClm,pLineEndItem);
    pLineEndItem = nullptr;

} // fillLineEndStylesModel

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::fillLineEndSizesModel( ELinePoint i_linePoint, QStandardItemModel* i_pModel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillLineEndSizesModel",
        /* strAddInfo   */ strAddTrcInfo );

    QPixmap        pxmLineEnd( s_rctPxmLineEndSize.size() );
    QPainter       painter;
    QPen           pen;
    QBrush         brsh;
    QStandardItem* pLineEndItem;
    CDrawSettings  drawSettings;
    QPolygonF      plgArrowHead;

    CEnumLineEndWidth  lineEndWidth;
    CEnumLineEndLength lineEndLength;

    drawSettings.setLineEndStyle(i_linePoint,ELineEndStyle::ArrowHead);
    drawSettings.setLineEndBaseLineType(i_linePoint,ELineEndBaseLineType::Normal);
    drawSettings.setLineEndFillStyle(i_linePoint,ELineEndFillStyle::SolidPattern);

    for( lineEndWidth = 0; lineEndWidth < CEnumLineEndWidth::count(); lineEndWidth++ )
    {
        drawSettings.setLineEndWidth(i_linePoint,lineEndWidth.enumerator());

        for( lineEndLength = 0; lineEndLength < CEnumLineEndLength::count(); lineEndLength++ )
        {
            drawSettings.setLineEndLength(i_linePoint,lineEndLength.enumerator());

            getLineEndPolygons(
                /* line          */ s_arlineLineEndSizes[static_cast<int>(i_linePoint)],
                /* drawSetings   */ drawSettings,
                /* pplgLineStart */ i_linePoint == ELinePoint::Start ? &plgArrowHead : nullptr,
                /* pplgLineEnd   */ i_linePoint == ELinePoint::End ? &plgArrowHead : nullptr );

            pxmLineEnd.fill(Qt::white);
            painter.begin(&pxmLineEnd);

            pen.setColor(Qt::black);
            pen.setWidth(1);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(s_arlineLineEndSizes[static_cast<int>(i_linePoint)]);

            pen.setColor(Qt::black);
            pen.setWidth(1);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            brsh.setColor(Qt::black);
            brsh.setStyle(Qt::SolidPattern);
            painter.setBrush(brsh);
            painter.drawPolygon(plgArrowHead);

            painter.end();

            pLineEndItem = new QStandardItem();
            pLineEndItem->setData( pxmLineEnd, Qt::DecorationRole );
            pLineEndItem->setData( lineEndWidth.enumeratorAsInt(), EItemDataRoleLineEndWidth );
            pLineEndItem->setData( lineEndLength.enumeratorAsInt(), EItemDataRoleLineEndLength );
            i_pModel->setItem(lineEndWidth.enumeratorAsInt(), lineEndLength.enumeratorAsInt(), pLineEndItem);
            pLineEndItem = nullptr;
        }
    }

} // fillLineEndSizesModel

/*==============================================================================
protected slots: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    QWidget::mousePressEvent(i_pEv);

} // mousePressEvent

/*==============================================================================
protected slots: // instance methods (Line Style)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onBtnPenColorClicked()
//------------------------------------------------------------------------------
{
    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_drawSettings.getPenColor(),
        /* pWdgtParent */ m_pBtnPenColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    hidePopups();

    if( clr.isValid() )
    {
        m_drawSettings.setPenColor(clr);

        setPenColor(m_drawSettings);
    }

} // onBtnPenColorClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onEdtPenWidthTextChanged( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtPenWidthTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    m_drawSettings.setPenWidth(i_strText.toInt());

} // onEdtPenWidthTextChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onBtnLineStyleClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnLineStyleClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups(m_pViewLineStyles);

    if( m_pViewLineStyles == nullptr )
    {
        m_pViewLineStyles = new QListView(this);
        m_pViewLineStyles->setModel(m_pModelLineStyles);

        // List of popup widgets which have to be closed on activating other widgets
        m_arpWdgtsPopup.append(m_pViewLineStyles);

        if( !connect(
            /* pObjSender   */ m_pViewLineStyles,
            /* szSignal     */ SIGNAL(clicked(const QModelIndex&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onViewLineStylesClicked(const QModelIndex&)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    if( m_pViewLineStyles->isVisible() )
    {
        m_pViewLineStyles->hide();
    }
    else
    {
        int idxRow;
        int iViewHeight;

        QMargins margins = m_pViewLineStyles->contentsMargins();

        iViewHeight = margins.top() + margins.bottom() + 2*m_pViewLineStyles->frameWidth();

        for( idxRow = 0; idxRow < m_pModelLineStyles->rowCount(); idxRow++ )
        {
            iViewHeight += m_pViewLineStyles->sizeHintForRow(idxRow); // + m_pViewLineStyles->spacing();
        }

        QPoint ptBtnLB = m_pBtnLineStyle->geometry().bottomLeft();

        ptBtnLB = m_pBtnLineStyle->parentWidget()->mapTo(this,ptBtnLB);

        m_pViewLineStyles->setMinimumSize( QSize(m_pBtnLineStyle->width(),iViewHeight) );
        m_pViewLineStyles->move(ptBtnLB);
        m_pViewLineStyles->show();
    }

} // onBtnLineStyleClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onViewLineStylesClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onViewLineStylesClicked",
        /* strAddInfo   */ strAddTrcInfo );

    QStandardItem* pLineStyleItem = m_pModelLineStyles->itemFromIndex(i_modelIdx);

    if( pLineStyleItem != nullptr )
    {
        QVariant varData = pLineStyleItem->data(EItemDataRoleLineStyle);
        CEnumLineStyle lineStyle = static_cast<ELineStyle>(varData.toInt());

        if( lineStyle >= 0 && lineStyle < CEnumLineStyle::count() )
        {
            m_drawSettings.setLineStyle(lineStyle.enumerator());

            setLineStyle(m_drawSettings);
        }
    }

    if( m_pViewLineStyles->isVisible() )
    {
        m_pViewLineStyles->hide();
    }

} // onViewLineStylesClicked

/*==============================================================================
protected slots: // instance methods (Line End Styles)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onBtnLineEndStylesLineStartClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnLineEndStylesLineStartClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups(m_pViewLineEndStylesLineStart);

    if( m_pViewLineEndStylesLineStart == nullptr )
    {
        m_pViewLineEndStylesLineStart = new QTableView(this);
        m_pViewLineEndStylesLineStart->setModel(m_pModelLineEndStylesLineStart);
        m_pViewLineEndStylesLineStart->verticalHeader()->hide();
        m_pViewLineEndStylesLineStart->horizontalHeader()->hide();
        m_pViewLineEndStylesLineStart->resizeRowsToContents();
        m_pViewLineEndStylesLineStart->resizeColumnsToContents();

        // List of popup widgets which have to be closed on activating other widgets
        m_arpWdgtsPopup.append(m_pViewLineEndStylesLineStart);

        if( !connect(
            /* pObjSender   */ m_pViewLineEndStylesLineStart,
            /* szSignal     */ SIGNAL(clicked(const QModelIndex&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onViewLineEndStylesLineStartClicked(const QModelIndex&)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    if( m_pViewLineEndStylesLineStart->isVisible() )
    {
        m_pViewLineEndStylesLineStart->hide();
    }
    else
    {
        int idxRow;
        int idxClm;
        int iViewMinHeight;
        int iViewMinWidth;

        QMargins margins = m_pViewLineEndStylesLineStart->contentsMargins();

        iViewMinHeight = margins.top() + margins.bottom(); // + 2*m_pViewLineEndStylesLineStart->frameWidth();
        iViewMinWidth = margins.left() + margins.right();  // + 2*m_pViewLineEndStylesLineStart->frameWidth();

        for( idxRow = 0; idxRow < m_pModelLineEndStylesLineStart->rowCount(); idxRow++ )
        {
            iViewMinHeight += m_pViewLineEndStylesLineStart->rowHeight(idxRow);
        }
        for( idxClm = 0; idxClm < m_pModelLineEndStylesLineStart->columnCount(); idxClm++ )
        {
            iViewMinWidth += m_pViewLineEndStylesLineStart->columnWidth(idxClm);
        }

        QPoint ptBtnLB = m_pBtnLineEndStylesLineStart->geometry().bottomLeft();

        ptBtnLB = m_pBtnLineEndStylesLineStart->parentWidget()->mapTo(this,ptBtnLB);

        m_pViewLineEndStylesLineStart->setFixedSize( QSize(iViewMinWidth,iViewMinHeight) );
        m_pViewLineEndStylesLineStart->move(ptBtnLB);
        m_pViewLineEndStylesLineStart->show();
    }

} // onBtnLineEndStylesLineStartClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onViewLineEndStylesLineStartClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onViewLineEndStylesLineStartClicked",
        /* strAddInfo   */ strAddTrcInfo );

    QStandardItem* pLineEndStyleItem = m_pModelLineEndStylesLineStart->itemFromIndex(i_modelIdx);

    if( pLineEndStyleItem != nullptr )
    {
        CEnumLineEndStyle        lineEndStyle        = pLineEndStyleItem->data(EItemDataRoleLineEndStyle).toInt();
        CEnumLineEndBaseLineType lineEndBaseLineType = pLineEndStyleItem->data(EItemDataRoleLineEndBaseLineType).toInt();
        CEnumLineEndFillStyle    lineEndFillStyle    = pLineEndStyleItem->data(EItemDataRoleLineEndFillStyle).toInt();

        if( lineEndStyle >= 0 && lineEndStyle < CEnumLineEndStyle::count() )
        {
            m_drawSettings.setLineEndStyle(ELinePoint::Start,lineEndStyle.enumerator());
        }
        if( lineEndBaseLineType >= 0 && lineEndBaseLineType < CEnumLineEndBaseLineType::count() )
        {
            m_drawSettings.setLineEndBaseLineType(ELinePoint::Start,lineEndBaseLineType.enumerator());
        }
        if( lineEndFillStyle >= 0 && lineEndFillStyle < CEnumLineEndFillStyle::count() )
        {
            m_drawSettings.setLineEndFillStyle(ELinePoint::Start,lineEndFillStyle.enumerator());
        }

        setLineEndStyle( ELinePoint::Start, m_drawSettings );
    }

    if( m_pViewLineEndStylesLineStart->isVisible() )
    {
        m_pViewLineEndStylesLineStart->hide();
    }

} // onViewLineEndStylesLineStartClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onBtnLineEndStylesLineEndClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnLineEndStylesLineEndClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups(m_pViewLineEndStylesLineEnd);

    if( m_pViewLineEndStylesLineEnd == nullptr )
    {
        m_pViewLineEndStylesLineEnd = new QTableView(this);
        m_pViewLineEndStylesLineEnd->setModel(m_pModelLineEndStylesLineEnd);
        m_pViewLineEndStylesLineEnd->verticalHeader()->hide();
        m_pViewLineEndStylesLineEnd->horizontalHeader()->hide();
        m_pViewLineEndStylesLineEnd->resizeRowsToContents();
        m_pViewLineEndStylesLineEnd->resizeColumnsToContents();

        // List of popup widgets which have to be closed on activating other widgets
        m_arpWdgtsPopup.append(m_pViewLineEndStylesLineEnd);

        if( !connect(
            /* pObjSender   */ m_pViewLineEndStylesLineEnd,
            /* szSignal     */ SIGNAL(clicked(const QModelIndex&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onViewLineEndStylesLineEndClicked(const QModelIndex&)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    if( m_pViewLineEndStylesLineEnd->isVisible() )
    {
        m_pViewLineEndStylesLineEnd->hide();
    }
    else
    {
        int idxRow;
        int idxClm;
        int iViewMinHeight;
        int iViewMinWidth;

        QMargins margins = m_pViewLineEndStylesLineEnd->contentsMargins();

        iViewMinHeight = margins.top() + margins.bottom(); // + 2*m_pViewLineEndStylesLineEnd->frameWidth();
        iViewMinWidth = margins.left() + margins.right();  // + 2*m_pViewLineEndStylesLineEnd->frameWidth();

        for( idxRow = 0; idxRow < m_pModelLineEndStylesLineEnd->rowCount(); idxRow++ )
        {
            iViewMinHeight += m_pViewLineEndStylesLineEnd->rowHeight(idxRow);
        }
        for( idxClm = 0; idxClm < m_pModelLineEndStylesLineEnd->columnCount(); idxClm++ )
        {
            iViewMinWidth += m_pViewLineEndStylesLineEnd->columnWidth(idxClm);
        }

        QPoint ptBtnLB = m_pBtnLineEndStylesLineEnd->geometry().bottomLeft();

        ptBtnLB = m_pBtnLineEndStylesLineEnd->parentWidget()->mapTo(this,ptBtnLB);

        m_pViewLineEndStylesLineEnd->setFixedSize( QSize(iViewMinWidth,iViewMinHeight) );
        m_pViewLineEndStylesLineEnd->move(ptBtnLB);
        m_pViewLineEndStylesLineEnd->show();
    }

} // onBtnLineEndStylesLineEndClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onViewLineEndStylesLineEndClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onViewLineEndStylesLineEndClicked",
        /* strAddInfo   */ strAddTrcInfo );

    QStandardItem* pLineEndStyleItem = m_pModelLineEndStylesLineEnd->itemFromIndex(i_modelIdx);

    if( pLineEndStyleItem != nullptr )
    {
        CEnumLineEndStyle        lineEndStyle        = pLineEndStyleItem->data(EItemDataRoleLineEndStyle).toInt();
        CEnumLineEndBaseLineType lineEndBaseLineType = pLineEndStyleItem->data(EItemDataRoleLineEndBaseLineType).toInt();
        CEnumLineEndFillStyle    lineEndFillStyle    = pLineEndStyleItem->data(EItemDataRoleLineEndFillStyle).toInt();

        if( lineEndStyle >= 0 && lineEndStyle < CEnumLineEndStyle::count() )
        {
            m_drawSettings.setLineEndStyle(ELinePoint::End,lineEndStyle.enumerator());
        }
        if( lineEndBaseLineType >= 0 && lineEndBaseLineType < CEnumLineEndBaseLineType::count() )
        {
            m_drawSettings.setLineEndBaseLineType(ELinePoint::End,lineEndBaseLineType.enumerator());
        }
        if( lineEndFillStyle >= 0 && lineEndFillStyle < CEnumLineEndFillStyle::count() )
        {
            m_drawSettings.setLineEndFillStyle(ELinePoint::End,lineEndFillStyle.enumerator());
        }

        setLineEndStyle( ELinePoint::End, m_drawSettings );
    }

    if( m_pViewLineEndStylesLineEnd->isVisible() )
    {
        m_pViewLineEndStylesLineEnd->hide();
    }

} // onViewLineEndStylesLineEndClicked

/*==============================================================================
protected slots: // instance methods (Line End Sizes)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onBtnLineEndSizesLineStartClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnLineEndSizesLineStartClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups(m_pViewLineEndSizesLineStart);

    if( m_pViewLineEndSizesLineStart == nullptr )
    {
        m_pViewLineEndSizesLineStart = new QTableView(this);
        m_pViewLineEndSizesLineStart->setModel(m_pModelLineEndSizesLineStart);
        m_pViewLineEndSizesLineStart->verticalHeader()->hide();
        m_pViewLineEndSizesLineStart->horizontalHeader()->hide();
        m_pViewLineEndSizesLineStart->resizeRowsToContents();
        m_pViewLineEndSizesLineStart->resizeColumnsToContents();

        // List of popup widgets which have to be closed on activating other widgets
        m_arpWdgtsPopup.append(m_pViewLineEndSizesLineStart);

        if( !connect(
            /* pObjSender   */ m_pViewLineEndSizesLineStart,
            /* szSignal     */ SIGNAL(clicked(const QModelIndex&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onViewLineEndSizesLineStartClicked(const QModelIndex&)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    if( m_pViewLineEndSizesLineStart->isVisible() )
    {
        m_pViewLineEndSizesLineStart->hide();
    }
    else
    {
        int idxRow;
        int idxClm;
        int iViewMinHeight;
        int iViewMinWidth;

        QMargins margins = m_pViewLineEndSizesLineStart->contentsMargins();

        iViewMinHeight = margins.top() + margins.bottom(); // + 2*m_pViewLineEndSizesLineEnd->frameWidth();
        iViewMinWidth = margins.left() + margins.right();  // + 2*m_pViewLineEndSizesLineEnd->frameWidth();

        for( idxRow = 0; idxRow < m_pModelLineEndSizesLineStart->rowCount(); idxRow++ )
        {
            iViewMinHeight += m_pViewLineEndSizesLineStart->rowHeight(idxRow);
        }
        for( idxClm = 0; idxClm < m_pModelLineEndSizesLineStart->columnCount(); idxClm++ )
        {
            iViewMinWidth += m_pViewLineEndSizesLineStart->columnWidth(idxClm);
        }

        QPoint ptBtnLB = m_pBtnLineEndSizesLineStart->geometry().bottomLeft();

        ptBtnLB = m_pBtnLineEndSizesLineStart->parentWidget()->mapTo(this,ptBtnLB);

        m_pViewLineEndSizesLineStart->setFixedSize( QSize(iViewMinWidth,iViewMinHeight) );
        m_pViewLineEndSizesLineStart->move(ptBtnLB);
        m_pViewLineEndSizesLineStart->show();
    }

} // onBtnLineEndSizesLineStartClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onViewLineEndSizesLineStartClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onViewLineEndSizesLineStartClicked",
        /* strAddInfo   */ strAddTrcInfo );

    QStandardItem* pLineEndSizeItem = m_pModelLineEndSizesLineStart->itemFromIndex(i_modelIdx);

    if( pLineEndSizeItem != nullptr )
    {
        CEnumLineEndWidth  lineEndWidth  = pLineEndSizeItem->data(EItemDataRoleLineEndWidth).toInt();
        CEnumLineEndLength lineEndLength = pLineEndSizeItem->data(EItemDataRoleLineEndLength).toInt();

        if( lineEndWidth >= 0 && lineEndWidth < CEnumLineEndWidth::count() )
        {
            m_drawSettings.setLineEndWidth(ELinePoint::Start,lineEndWidth.enumerator());
        }
        if( lineEndLength >= 0 && lineEndLength < CEnumLineEndLength::count() )
        {
            m_drawSettings.setLineEndLength(ELinePoint::Start,lineEndLength.enumerator());
        }

        setLineEndSize( ELinePoint::Start, m_drawSettings );
    }

    if( m_pViewLineEndSizesLineStart->isVisible() )
    {
        m_pViewLineEndSizesLineStart->hide();
    }

} // onViewLineEndSizesLineStartClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onBtnLineEndSizesLineEndClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnLineEndSizesLineEndClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups(m_pViewLineEndSizesLineEnd);

    if( m_pViewLineEndSizesLineEnd == nullptr )
    {
        m_pViewLineEndSizesLineEnd = new QTableView(this);
        m_pViewLineEndSizesLineEnd->setModel(m_pModelLineEndSizesLineEnd);
        m_pViewLineEndSizesLineEnd->verticalHeader()->hide();
        m_pViewLineEndSizesLineEnd->horizontalHeader()->hide();
        m_pViewLineEndSizesLineEnd->resizeRowsToContents();
        m_pViewLineEndSizesLineEnd->resizeColumnsToContents();

        // List of popup widgets which have to be closed on activating other widgets
        m_arpWdgtsPopup.append(m_pViewLineEndSizesLineEnd);

        if( !connect(
            /* pObjSender   */ m_pViewLineEndSizesLineEnd,
            /* szSignal     */ SIGNAL(clicked(const QModelIndex&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onViewLineEndSizesLineEndClicked(const QModelIndex&)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    if( m_pViewLineEndSizesLineEnd->isVisible() )
    {
        m_pViewLineEndSizesLineEnd->hide();
    }
    else
    {
        int idxRow;
        int idxClm;
        int iViewMinHeight;
        int iViewMinWidth;

        QMargins margins = m_pViewLineEndSizesLineEnd->contentsMargins();

        iViewMinHeight = margins.top() + margins.bottom(); // + 2*m_pViewLineEndSizesLineEnd->frameWidth();
        iViewMinWidth = margins.left() + margins.right();  // + 2*m_pViewLineEndSizesLineEnd->frameWidth();

        for( idxRow = 0; idxRow < m_pModelLineEndSizesLineEnd->rowCount(); idxRow++ )
        {
            iViewMinHeight += m_pViewLineEndSizesLineEnd->rowHeight(idxRow);
        }
        for( idxClm = 0; idxClm < m_pModelLineEndSizesLineEnd->columnCount(); idxClm++ )
        {
            iViewMinWidth += m_pViewLineEndSizesLineEnd->columnWidth(idxClm);
        }

        QPoint ptBtnLB = m_pBtnLineEndSizesLineEnd->geometry().bottomLeft();

        ptBtnLB = m_pBtnLineEndSizesLineEnd->parentWidget()->mapTo(this,ptBtnLB);

        m_pViewLineEndSizesLineEnd->setFixedSize( QSize(iViewMinWidth,iViewMinHeight) );
        m_pViewLineEndSizesLineEnd->move(ptBtnLB);
        m_pViewLineEndSizesLineEnd->show();
    }

} // onBtnLineEndSizesLineEndClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::onViewLineEndSizesLineEndClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onViewLineEndSizesLineEndClicked",
        /* strAddInfo   */ strAddTrcInfo );

    QStandardItem* pLineEndSizeItem = m_pModelLineEndSizesLineEnd->itemFromIndex(i_modelIdx);

    if( pLineEndSizeItem != nullptr )
    {
        CEnumLineEndWidth  lineEndWidth  = pLineEndSizeItem->data(EItemDataRoleLineEndWidth).toInt();
        CEnumLineEndLength lineEndLength = pLineEndSizeItem->data(EItemDataRoleLineEndLength).toInt();

        if( lineEndWidth >= 0 && lineEndWidth < CEnumLineEndWidth::count() )
        {
            m_drawSettings.setLineEndWidth(ELinePoint::End,lineEndWidth.enumerator());
        }
        if( lineEndLength >= 0 && lineEndLength < CEnumLineEndLength::count() )
        {
            m_drawSettings.setLineEndLength(ELinePoint::End,lineEndLength.enumerator());
        }

        setLineEndSize( ELinePoint::End, m_drawSettings );
    }

    if( m_pViewLineEndSizesLineEnd->isVisible() )
    {
        m_pViewLineEndSizesLineEnd->hide();
    }

} // onViewLineEndSizesLineEndClicked

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::setPenColor( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setPenColor",
        /* strAddInfo   */ strAddTrcInfo );

    QPainter painter(m_pPxmBtnPenColor);

    painter.setPen(i_drawSettings.getPenColor());
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(m_lineBtnPenColor1);
    painter.drawLine(m_lineBtnPenColor2);
    painter.drawLine(m_lineBtnPenColor3);
    painter.setPen(Qt::NoPen);
    painter.setBrush(i_drawSettings.getPenColor());
    painter.drawRect(m_rctBtnPenColor);

    m_pBtnPenColor->setIcon(*m_pPxmBtnPenColor);

} // setPenColor

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::setLineStyle( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineStyle",
        /* strAddInfo   */ strAddTrcInfo );

    QPixmap  pxmLineStyle(64,24);
    QLine    linLineStyle( 4, pxmLineStyle.height()/2, pxmLineStyle.width()-18, pxmLineStyle.height()/2 );
    QPolygon plgTriangle;
    QPoint   ptTriangleTL( pxmLineStyle.width()-13, pxmLineStyle.height()/2-3 );
    QPoint   ptTriangleTR( pxmLineStyle.width()-4, pxmLineStyle.height()/2-3 );
    QPoint   ptTriangleBC( pxmLineStyle.width()-9, pxmLineStyle.height()/2+4 );

    QPainter     painter(&pxmLineStyle);
    QPen         pen;
    Qt::PenStyle penStyle;
    QBrush       brsh;

    plgTriangle.append(ptTriangleTL);
    plgTriangle.append(ptTriangleTR);
    plgTriangle.append(ptTriangleBC);

    pxmLineStyle.fill(Qt::white);

    penStyle = lineStyle2QtPenStyle(i_drawSettings.getLineStyle());
    pen.setStyle(penStyle);
    pen.setColor(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(linLineStyle);

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    brsh.setColor(Qt::black);
    brsh.setStyle(Qt::SolidPattern);
    painter.setBrush(brsh);
    painter.drawPolygon(plgTriangle);

    painter.end(); // must be called before setting the mask

    pxmLineStyle.setMask( pxmLineStyle.createMaskFromColor(Qt::white) );

    m_pBtnLineStyle->setPixmap(pxmLineStyle);

} // setLineStyle

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::setLineEndStyle( ELinePoint i_linePoint, const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndStyle",
        /* strAddInfo   */ strAddTrcInfo );

    ELineEndStyle        lineEndStyle        = i_drawSettings.getLineEndStyle(i_linePoint);
    ELineEndBaseLineType lineEndBaseLineType = i_drawSettings.getLineEndBaseLineType(i_linePoint);
    ELineEndFillStyle    lineEndFillStyle    = i_drawSettings.getLineEndFillStyle(i_linePoint);

    QPixmap  pxmLineEnd(24,24);
    QPainter painter;
    QPen     pen;
    QBrush   brsh;

    QRect  rctPxm = pxmLineEnd.rect();

    QLine  line;
    QPoint ptArrowHead1;
    QPoint ptArrowHead2;
    QPoint ptArrowHead3;
    QPoint ptArrowHead4;

    if( i_linePoint == ELinePoint::Start )
    {
        line = QLine( 5, rctPxm.center().y(), rctPxm.right()-4, rctPxm.center().y() );
        ptArrowHead1 = QPoint( line.p1().x()+7, line.p1().y()+6 );
        ptArrowHead2 = QPoint( line.p1().x()-2, line.p1().y()   );
        ptArrowHead3 = QPoint( line.p1().x()+7, line.p1().y()-6 );
        ptArrowHead4 = QPoint( line.p1().x()+3, line.p1().y()   );
    }
    else // if( i_linePoint == ELinePoint::End )
    {
        line = QLine( 4, rctPxm.center().y(), rctPxm.right()-5, rctPxm.center().y() );
        ptArrowHead1 = QPoint( line.p2().x()-7, line.p2().y()+6 );
        ptArrowHead2 = QPoint( line.p2().x()+2, line.p2().y()   );
        ptArrowHead3 = QPoint( line.p2().x()-7, line.p2().y()-6 );
        ptArrowHead4 = QPoint( line.p2().x()-3, line.p2().y()   );
    }

    QPoint arPts[4] = { ptArrowHead1, ptArrowHead2, ptArrowHead3, ptArrowHead4 };

    pxmLineEnd.fill(Qt::white);

    painter.begin(&pxmLineEnd);

    if( lineEndStyle == ELineEndStyle::Normal )
    {
        pen.setColor(Qt::black);
        pen.setWidth(3);
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);
        painter.drawLine(line);
    }
    else if( lineEndStyle == ELineEndStyle::ArrowHead )
    {
        if( lineEndBaseLineType == ELineEndBaseLineType::NoLine )
        {
            pen.setColor(Qt::black);
            pen.setWidth(3);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(line);
            painter.setBrush(Qt::NoBrush);
            painter.drawPolyline(arPts,3);
        }
        else if( lineEndBaseLineType == ELineEndBaseLineType::Normal )
        {
            if( i_linePoint == ELinePoint::Start )
            {
                line.setP1( QPoint( ptArrowHead1.x(), rctPxm.center().y() ) );
            }
            else // if( i_linePoint == ELinePoint::End )
            {
                line.setP2( QPoint( ptArrowHead1.x(), rctPxm.center().y() ) );
            }
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
            brsh.setStyle( lineEndFillStyle2QtBrushStyle(lineEndFillStyle) );
            painter.setBrush(brsh);
            painter.drawPolygon(arPts,3);
        }
        else if( lineEndBaseLineType == ELineEndBaseLineType::Indented )
        {
            if( i_linePoint == ELinePoint::Start )
            {
                line.setP1( QPoint( ptArrowHead1.x(), rctPxm.center().y() ) );
            }
            else // if( i_linePoint == ELinePoint::End )
            {
                line.setP2( QPoint( ptArrowHead1.x(), rctPxm.center().y() ) );
            }
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
            brsh.setStyle( lineEndFillStyle2QtBrushStyle(lineEndFillStyle) );
            painter.setBrush(brsh);
            painter.drawPolygon(arPts,4);
        }
    }

    painter.end();

    pxmLineEnd.setMask( pxmLineEnd.createMaskFromColor(Qt::white) );

    if( i_linePoint == ELinePoint::Start )
    {
        m_pBtnLineEndStylesLineStart->setPixmap(pxmLineEnd);
    }
    else if( i_linePoint == ELinePoint::End )
    {
        m_pBtnLineEndStylesLineEnd->setPixmap(pxmLineEnd);
    }

} // setLineEndStyle

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLineStyle::setLineEndSize( ELinePoint i_linePoint, const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndSize",
        /* strAddInfo   */ strAddTrcInfo );

    CEnumLineEndWidth  lineEndWidth  = i_drawSettings.getLineEndWidth(i_linePoint);
    CEnumLineEndLength lineEndLength = i_drawSettings.getLineEndLength(i_linePoint);

    QPolygonF plgArrowHead;

    if( lineEndWidth >= 0 && lineEndWidth < CEnumLineEndWidth::count()
     && lineEndLength >= 0 && lineEndLength < CEnumLineEndLength::count() )
    {
        QPixmap       pxmLineEnd( s_rctPxmLineEndSize.size() );
        QPainter      painter;
        CDrawSettings drawSettings = i_drawSettings;
        QPen          pen;
        QBrush        brsh;

        drawSettings.setLineEndStyle(i_linePoint,ELineEndStyle::ArrowHead);
        drawSettings.setLineEndBaseLineType(i_linePoint,ELineEndBaseLineType::Normal);
        drawSettings.setLineEndFillStyle(i_linePoint,ELineEndFillStyle::SolidPattern);

        getLineEndPolygons(
            /* line          */ s_arlineLineEndSizes[static_cast<int>(i_linePoint)],
            /* drawSetings   */ drawSettings,
            /* pplgLineStart */ i_linePoint == ELinePoint::Start ? &plgArrowHead : nullptr,
            /* pplgLineEnd   */ i_linePoint == ELinePoint::End ? &plgArrowHead : nullptr );

        pxmLineEnd.fill(Qt::white);
        painter.begin(&pxmLineEnd);

        pen.setColor(Qt::black);
        pen.setWidth(1);
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);
        painter.drawLine(s_arlineLineEndSizes[static_cast<int>(i_linePoint)]);

        pen.setColor(Qt::black);
        pen.setWidth(1);
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);
        brsh.setColor(Qt::black);
        brsh.setStyle(Qt::SolidPattern);
        painter.setBrush(brsh);
        painter.drawPolygon(plgArrowHead);

        painter.end();

        pxmLineEnd.setMask( pxmLineEnd.createMaskFromColor(Qt::white) );

        if( i_linePoint == ELinePoint::Start )
        {
            m_pBtnLineEndSizesLineStart->setPixmap(pxmLineEnd);
        }
        else if( i_linePoint == ELinePoint::End )
        {
            m_pBtnLineEndSizesLineEnd->setPixmap(pxmLineEnd);
        }
    }

} // setLineEndSize
