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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgroupbox.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSRemCmdGUI/ZSRemCmdObjPoolWdgt.h"
#include "ZSRemCmdGUI/ZSRemCmdObjPoolModel.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdSubSysNode.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSIpcGUI/ZSIpcServerWdgt.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::RemCmd;
using namespace ZS::RemCmd::GUI;


/*******************************************************************************
class CDelegateRemCmdObjPool : public QAbstractItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateRemCmdObjPool::CDelegateRemCmdObjPool( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QItemDelegate(i_pObjParent),
    m_rectChkBoxIsQuery(0,0,10,10),
    m_rectChkBoxIsEvent(0,0,10,10)
{
} // ctor

//------------------------------------------------------------------------------
CDelegateRemCmdObjPool::~CDelegateRemCmdObjPool()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDelegateRemCmdObjPool::isCheckBoxIsQueryHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& /*i_modelIdx*/ )
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxIsQuery.width()
     && i_rectVisual.height() <= m_rectChkBoxIsQuery.height() )
    {
        bIsHit = true;
    }

    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxIsQuery;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxIsQuery.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxIsQuery.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxIsQuery.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxIsQuery.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }
    return bIsHit;

} // isCheckBoxIsQueryHit

//------------------------------------------------------------------------------
bool CDelegateRemCmdObjPool::isCheckBoxIsEventHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& /*i_modelIdx*/ )
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxIsEvent.width()
     && i_rectVisual.height() <= m_rectChkBoxIsEvent.height() )
    {
        bIsHit = true;
    }

    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxIsEvent;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxIsEvent.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxIsEvent.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxIsEvent.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxIsEvent.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }
    return bIsHit;

} // isCheckBoxIsEventHit

/*==============================================================================
public: // overridables of base class QItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateRemCmdObjPool::paint(
    QPainter*                   i_pPainter,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    bool bPainted = false;

    const CObjPoolTreeEntry* pCTreeEntry   = static_cast<const CObjPoolTreeEntry*>(i_modelIdx.internalPointer());
    CObjPoolTreeEntry*       pVTreeEntry   = const_cast<CObjPoolTreeEntry*>(pCTreeEntry);
    QStyleOptionViewItem     styleOption  = i_styleOption;

    CRemCmdObj* pRemCmdObj = nullptr;

    if( pVTreeEntry != nullptr )
    {
        if( pVTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
        {
            pRemCmdObj = reinterpret_cast<CRemCmdObj*>(pVTreeEntry->getObj());
        }
    }

    switch( i_modelIdx.column() )
    {
        case CRemCmdObjPoolModel::EColumnIsQuery:
        {
            if( pRemCmdObj != nullptr )
            {
                QRect          rectCheckBox = m_rectChkBoxIsQuery;
                Qt::CheckState checkState   = Qt::Unchecked;

                rectCheckBox.moveLeft(styleOption.rect.left());
                rectCheckBox.moveTop(styleOption.rect.top());

                if( pVTreeEntry->getStyleState() & QStyle::State_Selected )
                {
                    styleOption.state |= QStyle::State_Selected;
                }
                if( styleOption.rect.width() > m_rectChkBoxIsQuery.width() )
                {
                    rectCheckBox.moveLeft(styleOption.rect.left()+(styleOption.rect.width()-m_rectChkBoxIsQuery.width())/2);
                }
                if( styleOption.rect.height() > m_rectChkBoxIsQuery.height() )
                {
                    rectCheckBox.moveTop(styleOption.rect.top()+(styleOption.rect.height()-m_rectChkBoxIsQuery.height())/2);
                }
                if( !pRemCmdObj->isEventOnly() )
                {
                    checkState = Qt::Checked;
                }
                else
                {
                    checkState = Qt::Unchecked;
                }

                QItemDelegate::drawBackground(i_pPainter,styleOption,i_modelIdx);
                QItemDelegate::drawCheck(i_pPainter,styleOption,rectCheckBox,checkState);
                if( styleOption.state & QStyle::State_HasFocus )
                {
                    QItemDelegate::drawFocus(i_pPainter,styleOption,styleOption.rect);
                }
                bPainted = true;
            }
            break;
        } // case CRemCmdObjPoolModel::EColumnIsQueryOnly

        case CRemCmdObjPoolModel::EColumnIsEvent:
        {
            if( pRemCmdObj != nullptr )
            {
                QRect          rectCheckBox = m_rectChkBoxIsEvent;
                Qt::CheckState checkState   = Qt::Unchecked;

                rectCheckBox.moveLeft(styleOption.rect.left());
                rectCheckBox.moveTop(styleOption.rect.top());

                if( pVTreeEntry->getStyleState() & QStyle::State_Selected )
                {
                    styleOption.state |= QStyle::State_Selected;
                }
                if( styleOption.rect.width() > m_rectChkBoxIsEvent.width() )
                {
                    rectCheckBox.moveLeft(styleOption.rect.left()+(styleOption.rect.width()-m_rectChkBoxIsEvent.width())/2);
                }
                if( styleOption.rect.height() > m_rectChkBoxIsEvent.height() )
                {
                    rectCheckBox.moveTop(styleOption.rect.top()+(styleOption.rect.height()-m_rectChkBoxIsEvent.height())/2);
                }
                if( !pRemCmdObj->isQueryOnly() )
                {
                    checkState = Qt::Checked;
                }
                else
                {
                    checkState = Qt::Unchecked;
                }

                QItemDelegate::drawBackground(i_pPainter,styleOption,i_modelIdx);
                QItemDelegate::drawCheck(i_pPainter,styleOption,rectCheckBox,checkState);
                if( styleOption.state & QStyle::State_HasFocus )
                {
                    QItemDelegate::drawFocus(i_pPainter,styleOption,styleOption.rect);
                }
                bPainted = true;
            }
            break;
        } // case CRemCmdObjPoolModel::EColumnIsEventOnly

        default:
        {
            break;
        }
    }

    if( !bPainted )
    {
        QItemDelegate::paint(i_pPainter,styleOption,i_modelIdx);
    }

} // paint


/*******************************************************************************
class CWdgtRemCmdObjPool : public QDialog
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtRemCmdObjPool::CWdgtRemCmdObjPool( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pObjPool(nullptr),
    m_pObjPoolModel(nullptr),
    m_pLyt(nullptr),
    m_pDelegate(nullptr),
    m_pTreeViewObjPool(nullptr),
    m_pLytLineObjPoolBtns(nullptr),
    m_pBtnObjPoolResizeRowsAndColumnsToContents(nullptr),
    m_pBtnObjPoolExpandAll(nullptr),
    m_pBtnObjPoolCollapseAll(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtRemCmdObjPool");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmdGUI", "CWdgtRemCmdObjPool", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int cxBtnWidth = 120;

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pObjPoolModel = new CRemCmdObjPoolModel();

    m_pDelegate = new CDelegateRemCmdObjPool(this);

    m_pTreeViewObjPool = new QTreeView();
    m_pTreeViewObjPool->setModel(m_pObjPoolModel);
    m_pTreeViewObjPool->setItemDelegate(m_pDelegate);

    m_pTreeViewObjPool->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTreeViewObjPool->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_pTreeViewObjPool->setAlternatingRowColors(true);

    m_pTreeViewObjPool->hideColumn(CRemCmdObjPoolModel::EColumnType);
    m_pTreeViewObjPool->hideColumn(CRemCmdObjPoolModel::EColumnObjId);

    m_pLyt->addWidget(m_pTreeViewObjPool);

    if( !QObject::connect(
        /* pObjSender   */ m_pTreeViewObjPool->selectionModel(),
        /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&,const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewObjPoolCurrentChanged(const QModelIndex&,const QModelIndex&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Line> Buttons
    //---------------

    m_pLytLineObjPoolBtns = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytLineObjPoolBtns);

    // <Button> Resize Columns To Contents
    //------------------------------------

    m_pBtnObjPoolResizeRowsAndColumnsToContents = new QPushButton("Resize Columns");
    m_pBtnObjPoolResizeRowsAndColumnsToContents->setFixedWidth(cxBtnWidth);
    m_pLytLineObjPoolBtns->addWidget(m_pBtnObjPoolResizeRowsAndColumnsToContents);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnObjPoolResizeRowsAndColumnsToContents,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnObjPoolResizeRowsAndColumnsToContentsClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Expand All
    //--------------------

    m_pBtnObjPoolExpandAll = new QPushButton("Expand All");
    m_pBtnObjPoolExpandAll->setFixedWidth(cxBtnWidth);
    m_pLytLineObjPoolBtns->addWidget(m_pBtnObjPoolExpandAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnObjPoolExpandAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnObjPoolExpandAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Collapse All
    //----------------------

    m_pBtnObjPoolCollapseAll = new QPushButton("Collapse All");
    m_pBtnObjPoolCollapseAll->setFixedWidth(cxBtnWidth);
    m_pLytLineObjPoolBtns->addWidget(m_pBtnObjPoolCollapseAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnObjPoolCollapseAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnObjPoolCollapseAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytLineObjPoolBtns->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtRemCmdObjPool::~CWdgtRemCmdObjPool()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pDelegate;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pObjPoolModel;
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pObjPool = nullptr;
    m_pObjPoolModel = nullptr;
    m_pLyt = nullptr;
    m_pDelegate = nullptr;
    m_pTreeViewObjPool = nullptr;
    m_pLytLineObjPoolBtns = nullptr;
    m_pBtnObjPoolResizeRowsAndColumnsToContents = nullptr;
    m_pBtnObjPoolExpandAll = nullptr;
    m_pBtnObjPoolCollapseAll = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtRemCmdObjPool::setObjPool( CRemCmdObjPool* i_pObjPool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "ObjPool: " + QString( i_pObjPool == nullptr ? "nullptr" : i_pObjPool->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setObjPool",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pObjPool != i_pObjPool )
    {
        m_pObjPoolModel->setRemCmdObjPool(i_pObjPool);
    }

} // setObjPool

/*==============================================================================
protected slots: // connected to the signals of the remote command client
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtRemCmdObjPool::onTreeViewObjPoolCurrentChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != nullptr )
    {
        CObjPoolTreeEntry* pTreeEntry = static_cast<CObjPoolTreeEntry*>(i_modelIdxCurr.internalPointer());

        if( pTreeEntry != nullptr )
        {
            CRemCmdSubSysNode* pRemCmdSubSysNode = nullptr;
            CRemCmdObj*        pRemCmdObj = nullptr;
            QString            strCmd;
            QString            strSyntax;
            QString            strDscr;

            if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
            {
                pRemCmdObj = reinterpret_cast<CRemCmdObj*>(pTreeEntry->getObj());

                if( pRemCmdObj != nullptr )
                {
                    strCmd    = pRemCmdObj->getCommandStr();
                    strSyntax = pRemCmdObj->getSyntax();
                    strDscr   = pRemCmdObj->getDescription();
                }
            }
            else // if( pTreeEntry->getEntryType() != EObjPoolEntryTypeObject )
            {
                pRemCmdSubSysNode = reinterpret_cast<CRemCmdSubSysNode*>(pTreeEntry->getObj());

                if( pRemCmdSubSysNode != nullptr )
                {
                    strCmd  = pRemCmdSubSysNode->getNodeStr();
                    strDscr = pRemCmdSubSysNode->getDescription();
                }
            } // if( pTreeEntry->getEntryType() != EObjPoolEntryTypeObject )

            emit remCmdObjSelected(strCmd,strSyntax,strDscr);

        } // if( pTreeEntry != nullptr )
    } // if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != nullptr )

} // onTreeViewRemCmdObjPoolCurrentChanged

//------------------------------------------------------------------------------
void CWdgtRemCmdObjPool::onBtnObjPoolResizeRowsAndColumnsToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewObjPool != nullptr )
    {
        int idxClm;

        for( idxClm = 0; idxClm < CRemCmdObjPoolModel::EColumnCount; idxClm++ )
        {
            m_pTreeViewObjPool->resizeColumnToContents(idxClm);
        }
    }

} // onBtnResizeRowsAndColumnsToContentsClicked

//------------------------------------------------------------------------------
void CWdgtRemCmdObjPool::onBtnObjPoolExpandAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewObjPool != nullptr )
    {
        m_pTreeViewObjPool->expandAll();
    }

} // onBtnExpandAllClicked

//------------------------------------------------------------------------------
void CWdgtRemCmdObjPool::onBtnObjPoolCollapseAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewObjPool != nullptr )
    {
        m_pTreeViewObjPool->collapseAll();
    }

} // onBtnCollapseAllClicked

