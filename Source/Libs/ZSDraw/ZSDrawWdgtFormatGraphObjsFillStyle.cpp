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
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
#include <QtGui/qstandarditemmodel.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcolordialog.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSDraw/ZSDrawWdgtFormatGraphObjsFillStyle.h"
#include "ZSDraw/ZSDrawGraphObj.h"
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
class CWdgtFormatGraphObjsFillStyle : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsFillStyle::CWdgtFormatGraphObjsFillStyle(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytFillColor(nullptr),
    m_pLblFillColor(nullptr),
    m_pPxmBtnFillColor(nullptr),
    m_rctBtnFillColor(0,11,16,5),
    m_lineBtnFillColor1( QPoint(13,5), QPoint(13,10) ),
    m_lineBtnFillColor2( QPoint(14,6), QPoint(14,9) ),
    m_lineBtnFillColor3( QPoint(15,7), QPoint(15,8) ),
    m_pBtnFillColor(nullptr),
    m_pLytFillStyle(nullptr),
    m_pLblFillStyle(nullptr),
    m_pBtnFillStyle(nullptr),
    m_pViewFillStyles(nullptr),
    m_pModelFillStyles(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsFillStyle");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    int cxLblWidth = 60;
    //int cxEdtWidth = 80;

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Fill Style");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Button> Fill Color
    //---------------------

    m_pLytFillColor = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytFillColor);
    m_pLblFillColor = new QLabel("Color:");
    m_pLblFillColor->setFixedWidth(cxLblWidth);
    m_pLytFillColor->addWidget(m_pLblFillColor);
    m_pBtnFillColor = new QPushButton();
    m_pLytFillColor->addWidget(m_pBtnFillColor);
    m_pLytFillColor->addStretch();

    m_pPxmBtnFillColor = new QPixmap(":/ZS/Draw/DrawToolBrush16x16.bmp");
    m_pPxmBtnFillColor->setMask(m_pPxmBtnFillColor->createHeuristicMask());

    if( !connect(
        /* pObjSender   */ m_pBtnFillColor,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnFillColorClicked()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <ComboBox> Fill Style
    //-----------------------

    m_pLytFillStyle = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytFillStyle);
    m_pLblFillStyle = new QLabel("Style:");
    m_pLblFillStyle->setFixedWidth(cxLblWidth);
    m_pLytFillStyle->addWidget(m_pLblFillStyle);
    m_pBtnFillStyle = new CPushButton();
    m_pBtnFillStyle->setFixedWidth(72);
    m_pBtnFillStyle->setFixedHeight(24);
    m_pLytFillStyle->addWidget(m_pBtnFillStyle);
    m_pLytFillStyle->addStretch();

    m_pModelFillStyles = new QStandardItemModel();

    fillFillStylesModel(m_pModelFillStyles);

    if( !connect(
        /* pObjSender   */ m_pBtnFillStyle,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnFillStyleClicked()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Stretch> at bottom of widget
    //------------------------------

    m_pLyt->addStretch();

    // Set settings at GUI controls
    //-----------------------------

    setFillColor();
    setFillStyle();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsFillStyle::~CWdgtFormatGraphObjsFillStyle()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pPxmBtnFillColor;
    }
    catch(...)
    {
    }
    m_pPxmBtnFillColor = nullptr;

    try
    {
        delete m_pModelFillStyles;
    }
    catch(...)
    {
    }
    m_pModelFillStyles = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pLytFillColor = nullptr;
    m_pLblFillColor = nullptr;
    m_pBtnFillColor = nullptr;
    m_pLytFillStyle = nullptr;
    m_pLblFillStyle = nullptr;
    m_pBtnFillStyle = nullptr;
    m_pViewFillStyles = nullptr;
    m_pModelFillStyles = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::applyChanges()
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
        m_pGraphObj->setFillColor(m_drawSettings.getFillColor(),false);
        m_pGraphObj->setFillStyle(m_drawSettings.getFillStyle(),false);
        m_pGraphObj->onDrawSettingsChanged();
    }
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
                    pGraphObj->setFillColor(m_drawSettings.getFillColor(),false);
                    pGraphObj->setFillStyle(m_drawSettings.getFillStyle(),false);
                    pGraphObj->onDrawSettingsChanged();
                }
            }
        } // if( arpGraphicsItemsSelected.size() > 0 )

        // If no graphical object is selected ...
        else // if( arpGraphicsItemsSelected.size() == 0 )
        {
            // .. set default attribute for following draw commands.
            m_pDrawingScene->setFillColor(m_drawSettings.getFillColor());
            m_pDrawingScene->setFillStyle(m_drawSettings.getFillStyle());

        } // if( arpGraphicsItemsSelected.size() == 0 )

    } // if( m_pGraphObj == nullptr && m_pDrawingScene != nullptr )

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::resetChanges()
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

    setFillColor();
    setFillStyle();

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsFillStyle::hasChanges() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ strMthInArgs );

    bool bHasChanges = false;

    if( m_pGraphObj != nullptr )
    {
        if( m_pGraphObj->getFillColor() != m_drawSettings.getFillColor() )
        {
            bHasChanges = true;
        }
        else if( m_pGraphObj->getFillStyle() != m_drawSettings.getFillStyle() )
        {
            bHasChanges = true;
        }
    }
    else if( m_pDrawingScene != nullptr )
    {
        if( m_pDrawingScene->getFillColor() != m_drawSettings.getFillColor() )
        {
            bHasChanges = true;
        }
        else if( m_pDrawingScene->getFillStyle() != m_drawSettings.getFillStyle() )
        {
            bHasChanges = true;
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bHasChanges);
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjChanged",
        /* strAddInfo   */ strMthInArgs );

} // onGraphObjChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::fillFillStylesModel( QStandardItemModel* i_pModel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillFillStylesModel",
        /* strAddInfo   */ strAddTrcInfo );

    QPixmap        pxmFillStyle(64,16);
    QStandardItem* pFillStyleItem;
    CEnumFillStyle fillStyle;

    QPainter painter;
    QPen     pen;
    QBrush   brsh;

    pen.setColor(Qt::black);
    pen.setWidth(1);

    for( fillStyle = 0; fillStyle < CEnumFillStyle::count(); fillStyle++ )
    {
        if( !isFillStyleGradientPattern(fillStyle.enumerator()) )
        {
            if( fillStyle != EFillStyle::NoFill )
            {
                pxmFillStyle.fill(Qt::white);

                painter.begin(&pxmFillStyle);

                if( fillStyle == EFillStyle::TexturePattern )
                {
                    brsh.setTexture(pxmFillStyle);
                }
                brsh.setStyle( fillStyle2QtBrushStyle(fillStyle.enumerator()) );
                painter.setBrush(brsh);
                painter.drawRect( 2, 2, pxmFillStyle.width()-4, pxmFillStyle.height()-4 );
                painter.end();

                pFillStyleItem = new QStandardItem();
                pFillStyleItem->setData( pxmFillStyle, Qt::DecorationRole );
            }
            else
            {
                pFillStyleItem = new QStandardItem(" No Fill");
            }

            pFillStyleItem->setData( fillStyle.enumeratorAsInt(), EItemDataRoleFillStyle );

            i_pModel->setItem( fillStyle.enumeratorAsInt(), 0, pFillStyleItem );
            pFillStyleItem = nullptr;
        }
    }

} // fillFillStylesModel

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::mousePressEvent( QMouseEvent* i_pEv )
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
protected slots: // instance methods (Fill STyle)
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::onBtnFillColorClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnFillColorClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups();

    QColor clr = QColorDialog::getColor(
        /* clrInitial  */ m_drawSettings.getFillColor(),
        /* pWdgtParent */ m_pBtnFillColor,
        /* strTitle    */ "Colors",
        /* options     */ QColorDialog::ShowAlphaChannel );

    if( clr.isValid() )
    {
        m_drawSettings.setFillColor(clr);

        setFillColor();
    }

} // onBtnFillColorClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::onBtnFillStyleClicked()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnFillStyleClicked",
        /* strAddInfo   */ strAddTrcInfo );

    hidePopups(m_pViewFillStyles);

    if( m_pViewFillStyles == nullptr )
    {
        m_pViewFillStyles = new QTableView(m_pBtnFillStyle->parentWidget());
        m_pViewFillStyles->setModel(m_pModelFillStyles);
        m_pViewFillStyles->verticalHeader()->hide();
        m_pViewFillStyles->horizontalHeader()->hide();
        m_pViewFillStyles->resizeRowsToContents();
        m_pViewFillStyles->resizeColumnsToContents();

        // List of popup widgets which have to be closed on activating other widgets
        m_arpWdgtsPopup.append(m_pViewFillStyles);

        if( !connect(
            /* pObjSender   */ m_pViewFillStyles,
            /* szSignal     */ SIGNAL(clicked(const QModelIndex&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onViewFillStylesClicked(const QModelIndex&)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    if( m_pViewFillStyles->isVisible() )
    {
        m_pViewFillStyles->hide();
    }
    else
    {
        int idxRow;
        int idxClm;
        int iViewMinHeight;
        int iViewMinWidth;

        QMargins margins = m_pViewFillStyles->contentsMargins();

        iViewMinHeight = margins.top() + margins.bottom(); // + 2*m_pViewFillStyle->frameWidth();
        iViewMinWidth = margins.left() + margins.right();  // + 2*m_pViewFillStyle->frameWidth();

        for( idxRow = 0; idxRow < m_pModelFillStyles->rowCount(); idxRow++ )
        {
            iViewMinHeight += m_pViewFillStyles->rowHeight(idxRow);
        }
        for( idxClm = 0; idxClm < m_pModelFillStyles->columnCount(); idxClm++ )
        {
            iViewMinWidth += m_pViewFillStyles->columnWidth(idxClm);
        }

        m_pViewFillStyles->setFixedSize( QSize(iViewMinWidth,iViewMinHeight) );
        m_pViewFillStyles->move( m_pBtnFillStyle->geometry().bottomLeft() );
        m_pViewFillStyles->show();
    }

} // onBtnFillStyleClicked

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::onViewFillStylesClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onViewFillStylesClicked",
        /* strAddInfo   */ strAddTrcInfo );

    QStandardItem* pFillStyleItem = m_pModelFillStyles->itemFromIndex(i_modelIdx);

    if( pFillStyleItem != nullptr )
    {
        QVariant       varData = pFillStyleItem->data(EItemDataRoleFillStyle);
        CEnumFillStyle fillStyle = varData.toInt();

        if( fillStyle >= 0 && fillStyle < CEnumFillStyle::count() )
        {
            m_drawSettings.setFillStyle(fillStyle.enumerator());

            setFillStyle();
        }
    }

    if( m_pViewFillStyles->isVisible() )
    {
        m_pViewFillStyles->hide();
    }

} // onViewFillStylesClicked

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::setFillColor()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFillColor",
        /* strAddInfo   */ strAddTrcInfo );

    QPainter painter(m_pPxmBtnFillColor);

    painter.setPen(m_drawSettings.getFillColor());
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(m_lineBtnFillColor1);
    painter.drawLine(m_lineBtnFillColor2);
    painter.drawLine(m_lineBtnFillColor3);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_drawSettings.getFillColor());
    painter.drawRect(m_rctBtnFillColor);

    m_pBtnFillColor->setIcon(*m_pPxmBtnFillColor);

} // setFillColor

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsFillStyle::setFillStyle()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFillStyle",
        /* strAddInfo   */ strAddTrcInfo );

    QPixmap  pxmFillStyle(64,24);
    QRect    rctFillStyle( 3, 3, pxmFillStyle.width()-20, pxmFillStyle.height()-6 );
    QPolygon plgTriangle;
    QPoint   ptTriangleTL( pxmFillStyle.width()-13, pxmFillStyle.height()/2-3 );
    QPoint   ptTriangleTR( pxmFillStyle.width()-4, pxmFillStyle.height()/2-3 );
    QPoint   ptTriangleBC( pxmFillStyle.width()-9, pxmFillStyle.height()/2+4 );

    QPainter       painter(&pxmFillStyle);
    QPen           pen;
    Qt::BrushStyle brshStyle;
    QBrush         brsh;

    plgTriangle.append(ptTriangleTL);
    plgTriangle.append(ptTriangleTR);
    plgTriangle.append(ptTriangleBC);

    pxmFillStyle.fill(Qt::white);

    brshStyle = fillStyle2QtBrushStyle(m_drawSettings.getFillStyle());
    //brsh.setColor(i_drawSettings.getFillColor());
    brsh.setColor(Qt::black);
    brsh.setStyle(brshStyle);
    painter.setBrush(brsh);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rctFillStyle);

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    brsh.setColor(Qt::black);
    brsh.setStyle(Qt::SolidPattern);
    painter.setBrush(brsh);
    painter.drawPolygon(plgTriangle);

    painter.end(); // must be called before setting the mask

    pxmFillStyle.setMask( pxmFillStyle.createMaskFromColor(Qt::white) );

    m_pBtnFillStyle->setPixmap(pxmFillStyle);

} // setFillStyle
