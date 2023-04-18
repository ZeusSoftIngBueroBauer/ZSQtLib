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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsWdgt.h"
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsTreeWdgt.h"
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsPropertiesWdgtStack.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjs : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjs::CWdgtGraphObjs(
    CDrawingView* i_pDrawingView,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pDrawingView(i_pDrawingView),
    m_pLytMain(nullptr),
    m_pSplitter(nullptr),
    m_pWdgtTreeView(nullptr),
    m_pWdgtStackGraphObjsProperties(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pDrawingView->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs =  QString(i_pDrawingView->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    // Split View with Tree View and Node Content
    //===========================================

    m_pSplitter = new QSplitter(Qt::Horizontal);
    m_pLytMain->addWidget(m_pSplitter, 1);

    // <TreeView>
    //-----------

    m_pWdgtTreeView = new CWdgtGraphObjsTree(m_pDrawingView);
    m_pWdgtTreeView->setMinimumWidth(180);
    m_pSplitter->addWidget(m_pWdgtTreeView);

    QObject::connect(
        m_pWdgtTreeView, &CWdgtGraphObjsTree::viewModeChanged,
        this, &CWdgtGraphObjs::onWdgtTreeViewModeChanged );
    QObject::connect(
        m_pWdgtTreeView, &CWdgtGraphObjsTree::currentRowChanged,
        this, &CWdgtGraphObjs::onWdgtTreeViewCurrentRowChanged );

    // <TableView>
    //------------

    m_pWdgtStackGraphObjsProperties = new CWdgtStackGraphObjsProperties(m_pDrawingView);
    m_pWdgtStackGraphObjsProperties->setMinimumWidth(180);
    m_pSplitter->addWidget(m_pWdgtStackGraphObjsProperties);

    CWdgtGraphObjsTree::EViewMode viewMode = m_pWdgtTreeView->viewMode();

    if( viewMode == CWdgtGraphObjsTree::EViewMode::NavPanelOnly ) {
        m_pWdgtStackGraphObjsProperties->hide();
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjs::~CWdgtGraphObjs()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDrawingView = nullptr;
    m_pLytMain = nullptr;
    m_pSplitter = nullptr;
    m_pWdgtTreeView = nullptr;
    m_pWdgtStackGraphObjsProperties = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjs::saveState(QSettings& i_settings) const
//------------------------------------------------------------------------------
{
    if( m_pSplitter != nullptr ) {
        QList<int> listSizes = m_pSplitter->sizes();
        for( int idx = 0; idx < listSizes.count(); idx++ ) {
            i_settings.setValue(
                ClassName() + "/" + objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx]);
        }
    }

    m_pWdgtTreeView->saveState(i_settings);
    m_pWdgtStackGraphObjsProperties->saveState(i_settings);
}

//------------------------------------------------------------------------------
void CWdgtGraphObjs::restoreState(const QSettings& i_settings)
//------------------------------------------------------------------------------
{
    m_pWdgtTreeView->restoreState(i_settings);
    m_pWdgtStackGraphObjsProperties->restoreState(i_settings);

    if( m_pSplitter != nullptr ) {
        QList<int> listSizes = m_pSplitter->sizes();
        for( int idx = 0; idx < listSizes.count(); idx++ ) {
            listSizes[idx] = i_settings.value(
                ClassName() + "/" + objectName() + "/SplitterHeight" + QString::number(idx), 50).toInt();
        }
        m_pSplitter->setSizes(listSizes);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjs::onWdgtTreeViewModeChanged( const QString& i_strViewMode )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strViewMode;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtTreeViewModeChanged",
        /* strMthInArgs */ strMthInArgs );

    CWdgtGraphObjsTree::EViewMode viewMode =
        CWdgtGraphObjsTree::str2ViewMode(i_strViewMode);

    if( viewMode == CWdgtGraphObjsTree::EViewMode::NavPanelAndNodeContent ) {
        m_pWdgtTreeView->setMinimumWidth(180);
        m_pWdgtStackGraphObjsProperties->show();
    }
    else {
        m_pWdgtTreeView->setMinimumWidth(180);
        m_pWdgtStackGraphObjsProperties->hide();
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjs::onWdgtTreeViewCurrentRowChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& i_modelIdxPrev )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Curr {" + CModelIdxTreeGraphObjs::modelIdx2Str(i_modelIdxCurr) + "}";
        strMthInArgs += ", Prev {" + CModelIdxTreeGraphObjs::modelIdx2Str(i_modelIdxPrev) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtTreeViewCurrentRowChanged",
        /* strMthInArgs */ strMthInArgs );

    if( i_modelIdxCurr.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdxCurr.internalPointer());

        if( pModelTreeEntry != nullptr ) {
            m_pWdgtStackGraphObjsProperties->setKeyInTree(pModelTreeEntry->keyInTree());
        }
        else {
            m_pWdgtStackGraphObjsProperties->setKeyInTree("");
        }
    }

} // onWdgtTreeViewCurrentRowChanged
