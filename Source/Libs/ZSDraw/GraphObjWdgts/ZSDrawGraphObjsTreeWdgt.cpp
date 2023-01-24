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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsTreeWdgt.h"
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsTreeViewWdgt.h"
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
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
class CWdgtIdxTreeGraphObjs : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTreeGraphObjs::CWdgtIdxTreeGraphObjs(
    CDrawingScene* i_pDrawingScene,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pDrawingScene(i_pDrawingScene),
    m_pLytMain(nullptr),
    m_pSplitter(nullptr),
    m_pWdgtTreeView(nullptr),
    m_pWdgtGraphObjs(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( i_pDrawingScene == nullptr ? "Drawing" : i_pDrawingScene->objectName() );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs =  QString(i_pDrawingScene == nullptr ? "nullptr" : i_pDrawingScene->objectName());
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

    m_pWdgtTreeView = new CWdgtIdxTreeViewGraphObjs(m_pDrawingScene);
    m_pWdgtTreeView->setMinimumWidth(50);
    m_pSplitter->addWidget(m_pWdgtTreeView);

    QObject::connect(
        m_pWdgtTreeView, &CWdgtIdxTreeViewGraphObjs::viewModeChanged,
        this, &CWdgtIdxTreeGraphObjs::onTreeViewViewModeChanged );
    QObject::connect(
        m_pWdgtTreeView, &CWdgtIdxTreeViewGraphObjs::currentRowChanged,
        this, &CWdgtIdxTreeGraphObjs::onTreeViewCurrentRowChanged );

    // <TableView>
    //------------

    m_pWdgtGraphObjs = new CWdgtGraphObjs(m_pDrawingScene);
    m_pSplitter->addWidget(m_pWdgtGraphObjs);

    CWdgtIdxTreeViewGraphObjs::EViewMode viewMode = m_pWdgtTreeView->viewMode();

    if( viewMode == CWdgtIdxTreeViewGraphObjs::EViewMode::NavPanelOnly ) {
        m_pWdgtGraphObjs->hide();
    }

    // <Geometry>
    //===================

    QSettings settings;

    restoreGeometry( settings.value(ClassName() + "/" + objectName() + "/Geometry").toByteArray() );

    QList<int> listSizes = m_pSplitter->sizes();
    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        listSizes[idx] = settings.value( ClassName() + "/" + objectName() + "/SplitterHeight" + QString::number(idx), 50 ).toInt();
    }
    m_pSplitter->setSizes(listSizes);

} // ctor

//------------------------------------------------------------------------------
CWdgtIdxTreeGraphObjs::~CWdgtIdxTreeGraphObjs()
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

    m_pDrawingScene = nullptr;
    m_pLytMain = nullptr;
    m_pSplitter = nullptr;
    m_pWdgtTreeView = nullptr;
    m_pWdgtGraphObjs = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeGraphObjs::onTreeViewViewModeChanged( const QString& i_strViewMode )
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
        /* strMethod    */ "onTreeViewViewModeChanged",
        /* strMthInArgs */ strMthInArgs );

    CWdgtIdxTreeViewGraphObjs::EViewMode viewMode =
        CWdgtIdxTreeViewGraphObjs::str2ViewMode(i_strViewMode);

    if( viewMode == CWdgtIdxTreeViewGraphObjs::EViewMode::NavPanelAndNodeContent ) {
        m_pWdgtGraphObjs->show();
    }
    else {
        m_pWdgtGraphObjs->hide();
    }
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeGraphObjs::onTreeViewCurrentRowChanged(
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
        /* strMethod    */ "onTreeViewCurrentRowChanged",
        /* strMthInArgs */ strMthInArgs );

    if( i_modelIdxCurr.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdxCurr.internalPointer());

        if( pModelTreeEntry != nullptr ) {
            m_pWdgtGraphObjs->setKeyInTree(pModelTreeEntry->keyInTree());
        }
        else {
            m_pWdgtGraphObjs->setKeyInTree("");
        }
    }

} // onTreeViewCurrentRowChanged
