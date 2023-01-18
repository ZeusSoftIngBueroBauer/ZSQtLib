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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSysGUI/ZSSysErrLogWdgt.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CWdgtErrLog : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtErrLog::CWdgtErrLog(
    const QString& i_strErrLogName,
    const QString& i_strHeadline,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pModelErrLog(nullptr),
    m_strHeadline(i_strHeadline),
    m_pLyt(nullptr),
    m_pLblHeadline(nullptr),
    m_pLytLineBtns(nullptr),
    m_pBtnClearTable(nullptr),
    m_pBtnDeleteRows(nullptr),
    m_pBtnResizeRowsAndColumnsToContents(nullptr),
    m_pLblFileName(nullptr),
    m_pEdtFileName(nullptr),
    m_pTableView(nullptr),
    m_pItemSelectionModel(nullptr)
{
    m_pLyt = new QVBoxLayout;
    m_pLyt->setContentsMargins(0,0,0,0);

    setLayout(m_pLyt);

    if( !m_strHeadline.isEmpty() )
    {
        m_pLblHeadline = new QLabel(m_strHeadline);
        m_pLyt->addWidget(m_pLblHeadline);
    }

    if( CErrLog::GetInstance(i_strErrLogName) != nullptr )
    {
        m_pModelErrLog = new CModelErrLog( CErrLog::GetInstance(i_strErrLogName) );

        // <Line> Push buttons to edit (delete) error model entries
        //=========================================================

        int cxPxmBtnWidth = 24;
        int cxBtnWidth = 100;

        m_pLytLineBtns = new QHBoxLayout;
        m_pLyt->addLayout(m_pLytLineBtns);

        // <Button> Delete all entries
        //-----------------------------

        m_pBtnClearTable = new QPushButton("Clear");
        m_pBtnClearTable->setFixedWidth(cxBtnWidth);
        m_pLytLineBtns->addWidget(m_pBtnClearTable);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnClearTable,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnClearTableClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Button> Delete selected entries
        //---------------------------------

        m_pBtnDeleteRows = new QPushButton("Delete Row(s)");
        m_pBtnDeleteRows->setFixedWidth(cxBtnWidth);
        m_pLytLineBtns->addWidget(m_pBtnDeleteRows);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnDeleteRows,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnDeleteRowsClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Button> Resize Columns To Contents
        //------------------------------------

        QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");

        m_pBtnResizeRowsAndColumnsToContents = new QPushButton();
        m_pBtnResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
        m_pBtnResizeRowsAndColumnsToContents->setFixedWidth(cxPxmBtnWidth);
        m_pLytLineBtns->addWidget(m_pBtnResizeRowsAndColumnsToContents);

        if( !QObject::connect(
            /* pObjSender   */ m_pBtnResizeRowsAndColumnsToContents,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnResizeRowsAndColumnsToContentsClicked(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // <Label> File Name
        //------------------

        m_pLytLineBtns->addSpacing(10);

        m_pLblFileName = new QLabel("File Name:");
        m_pLytLineBtns->addWidget(m_pLblFileName);
        m_pEdtFileName = new QLabel( CErrLog::GetInstance()->getAbsFilePath() );
        m_pLytLineBtns->addWidget(m_pEdtFileName, 1);

        // <Stretch> at end of line
        //-------------------------

        //m_pLytLineBtns->addStretch();

        // <TableView> Indicating the error model entries
        //===============================================

        m_pTableView = new QTableView();
        m_pLyt->addWidget(m_pTableView);

        QFont fntTableView = m_pTableView->font();
        fntTableView.setPointSize(8);
        m_pTableView->setFont(fntTableView);

        m_pTableView->setModel(m_pModelErrLog);
        m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_pTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        m_pTableView->setShowGrid(false);
        m_pTableView->setAlternatingRowColors(true);
        m_pTableView->hideColumn(CModelErrLog::EColumnSeverityImageUrl);
        m_pTableView->hideColumn(CModelErrLog::EColumnSeverity);
        m_pTableView->hideColumn(CModelErrLog::EColumnResultNumber);
        m_pTableView->hideColumn(CModelErrLog::EColumnProposal);
        m_pTableView->resizeColumnsToContents();
        m_pTableView->resizeRowsToContents();

        m_pItemSelectionModel = m_pTableView->selectionModel();

    } // if( CErrLog::GetInstance(i_strErrLogName) != nullptr )

} // ctor

//------------------------------------------------------------------------------
CWdgtErrLog::~CWdgtErrLog()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pModelErrLog;
    }
    catch(...)
    {
    }

    m_pModelErrLog = nullptr;
    m_pLyt = nullptr;
    m_pLblHeadline = nullptr;
    m_pLytLineBtns = nullptr;
    m_pBtnClearTable = nullptr;
    m_pBtnDeleteRows = nullptr;
    m_pBtnResizeRowsAndColumnsToContents = nullptr;
    m_pLblFileName = nullptr;
    m_pEdtFileName = nullptr;
    m_pTableView = nullptr;
    m_pItemSelectionModel = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtErrLog::showColumn( CModelErrLog::EColumn i_clm )
//------------------------------------------------------------------------------
{
    if( m_pTableView != nullptr )
    {
        m_pTableView->showColumn(i_clm);
    }

} // showColumn

//------------------------------------------------------------------------------
void CWdgtErrLog::hideColumn( CModelErrLog::EColumn i_clm )
//------------------------------------------------------------------------------
{
    if( m_pTableView != nullptr )
    {
        m_pTableView->hideColumn(i_clm);
    }

} // hideColumn

/*==============================================================================
protected: // slots
==============================================================================*/

////------------------------------------------------------------------------------
//void CWdgtErrLog::onEntryAddedInView( SErrLogEntry* )
////------------------------------------------------------------------------------
//{
//    m_pTableView->resizeColumnsToContents();
//    m_pTableView->resizeRowsToContents();
//
//} // onEntryAddedInView

//------------------------------------------------------------------------------
void CWdgtErrLog::onBtnClearTableClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pModelErrLog->clear();
}

//------------------------------------------------------------------------------
void CWdgtErrLog::onBtnDeleteRowsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pItemSelectionModel != nullptr )
    {
        if( m_pItemSelectionModel->selectedRows().count() > 0 )
        {
            m_pModelErrLog->removeEntries(m_pItemSelectionModel->selectedRows());
        }
    }

} // onBtnDeleteRowsClicked

//------------------------------------------------------------------------------
void CWdgtErrLog::onBtnResizeRowsAndColumnsToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTableView != nullptr )
    {
        m_pTableView->resizeColumnsToContents();
        m_pTableView->resizeRowsToContents();
    }

} // onBtnResizeRowsAndColumnsToContentsClicked
