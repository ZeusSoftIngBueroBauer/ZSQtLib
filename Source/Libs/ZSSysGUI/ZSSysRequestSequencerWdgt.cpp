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
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSSysGUI/ZSSysRequestSequencerWdgt.h"
#include "ZSSysGUI/ZSSysRequestSequencerModel.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CWdgtRequestSequencer : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtRequestSequencer::CWdgtRequestSequencer(
    const QString& i_strHeadline,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pModelRequests(nullptr),
    m_strHeadline(i_strHeadline),
    m_pLyt(nullptr),
    m_pLblHeadline(nullptr),
    m_pTreeView(nullptr),
    m_pItemSelectionModel(nullptr),
    m_pLytLineBtns(nullptr),
    m_pBtnClear(nullptr),
    m_pBtnDeleteRow(nullptr),
    m_pBtnResizeRowsAndColumnsToContents(nullptr),
    m_pBtnExpandAll(nullptr),
    m_pBtnCollapseAll(nullptr)
{
    m_pLyt = new QVBoxLayout;
    m_pLyt->setContentsMargins(0,0,0,0);

    setLayout(m_pLyt);

    if( !m_strHeadline.isEmpty() )
    {
        m_pLblHeadline = new QLabel(m_strHeadline);
        m_pLyt->addWidget(m_pLblHeadline);
    }

    m_pModelRequests = new CModelRequestSequencer();

    // <TreeView> Indicating the error model entries
    //===============================================

    m_pTreeView = new QTreeView();
    m_pLyt->addWidget(m_pTreeView);

    QFont fntTableView = m_pTreeView->font();
    fntTableView.setPointSize(8);
    m_pTreeView->setFont(fntTableView);

    m_pTreeView->setModel(m_pModelRequests);

    m_pTreeView->hideColumn(ZS::System::GUI::CModelRequestSequencer::EColumnRequest);
    m_pTreeView->hideColumn(ZS::System::GUI::CModelRequestSequencer::EColumnCreaterObjName); // Shown together with the request name as "ObjName.RequestName".
    m_pTreeView->hideColumn(ZS::System::GUI::CModelRequestSequencer::EColumnToolTip);

    m_pTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_pItemSelectionModel = m_pTreeView->selectionModel();

    // <Line> Buttons
    //===============

    int cxBtnWidth = 100;

    m_pLytLineBtns = new QHBoxLayout;
    m_pLyt->addLayout(m_pLytLineBtns);

    // <Button> Delete all entries
    //-----------------------------

    m_pBtnClear = new QPushButton("Clear");
    m_pBtnClear->setFixedWidth(cxBtnWidth);
    m_pLytLineBtns->addWidget(m_pBtnClear);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnClear,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnClearClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Delete selected entries
    //---------------------------------

    m_pBtnDeleteRow = new QPushButton("Delete Row");
    m_pBtnDeleteRow->setFixedWidth(cxBtnWidth);
    m_pLytLineBtns->addWidget(m_pBtnDeleteRow);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDeleteRow,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDeleteRowClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Resize Columns To Contents
    //------------------------------------

    m_pBtnResizeRowsAndColumnsToContents = new QPushButton("Resize Columns");
    m_pBtnResizeRowsAndColumnsToContents->setFixedWidth(cxBtnWidth);
    m_pLytLineBtns->addWidget(m_pBtnResizeRowsAndColumnsToContents);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnResizeRowsAndColumnsToContents,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnResizeRowsAndColumnsToContentsClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Expand All
    //--------------------

    m_pBtnExpandAll = new QPushButton("Expand All");
    m_pBtnExpandAll->setFixedWidth(cxBtnWidth);
    m_pLytLineBtns->addWidget(m_pBtnExpandAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnExpandAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnExpandAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Collapse All
    //----------------------

    m_pBtnCollapseAll = new QPushButton("Collapse All");
    m_pBtnCollapseAll->setFixedWidth(cxBtnWidth);
    m_pLytLineBtns->addWidget(m_pBtnCollapseAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnCollapseAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCollapseAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytLineBtns->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtRequestSequencer::~CWdgtRequestSequencer()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pModelRequests;
    }
    catch(...)
    {
    }

    m_pModelRequests = nullptr;
    m_pLyt = nullptr;
    m_pLblHeadline = nullptr;
    m_pTreeView = nullptr;
    m_pLytLineBtns = nullptr;
    m_pBtnClear = nullptr;
    m_pBtnDeleteRow = nullptr;
    m_pBtnResizeRowsAndColumnsToContents = nullptr;
    m_pBtnExpandAll = nullptr;
    m_pBtnCollapseAll = nullptr;

} // dtor

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtRequestSequencer::setSequencer( CRequestSequencer* i_pReqSeq )
//------------------------------------------------------------------------------
{
    m_pModelRequests->setSequencer(i_pReqSeq);
}

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtRequestSequencer::onBtnClearClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pModelRequests->clear();
}

//------------------------------------------------------------------------------
void CWdgtRequestSequencer::onBtnDeleteRowClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pItemSelectionModel != nullptr )
    {
        if( m_pItemSelectionModel->selectedRows().count() == 1 )
        {
            m_pModelRequests->removeEntry(m_pItemSelectionModel->selectedRows()[0]);
        }
    }

} // onBtnDeleteRowClicked

//------------------------------------------------------------------------------
void CWdgtRequestSequencer::onBtnResizeRowsAndColumnsToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeView != nullptr )
    {
        int idxClm;

        for( idxClm = 0; idxClm < CModelRequestSequencer::EColumnCount; idxClm++ )
        {
            m_pTreeView->resizeColumnToContents(idxClm);
        }
    }

} // onBtnResizeRowsAndColumnsToContentsClicked

//------------------------------------------------------------------------------
void CWdgtRequestSequencer::onBtnExpandAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeView != nullptr )
    {
        m_pTreeView->expandAll();
    }

} // onBtnExpandAllClicked

//------------------------------------------------------------------------------
void CWdgtRequestSequencer::onBtnCollapseAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeView != nullptr )
    {
        m_pTreeView->collapseAll();
    }

} // onBtnCollapseAllClicked
