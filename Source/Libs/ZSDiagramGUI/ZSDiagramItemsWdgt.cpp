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

#include "ZSDiagramGUI/ZSDiagramItemsWdgt.h"
#include "ZSDiagramGUI/ZSDiagramItemsTreeModel.h"
#include "ZSDiagramGUI/ZSDiagramItemsTreeWdgt.h"
#include "ZSDiagramGUI/ZSDiagramItemsPropertiesWdgtStack.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
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
using namespace ZS::Diagram;
using namespace ZS::Diagram::GUI;


/*******************************************************************************
class CWdgtDiagramItems : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDiagramItems::CWdgtDiagramItems(
    CWdgtDiagram* i_pDiagram,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pDiagram(i_pDiagram),
    m_pLyt(nullptr),
    m_pSplitter(nullptr),
    m_pWdgtTreeView(nullptr),
    m_pWdgtStackObjsProperties(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pDiagram->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs =  QString(i_pDiagram->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pLyt = new QVBoxLayout;
    setLayout(m_pLyt);

    // Split View with Tree View and Node Content
    //===========================================

    m_pSplitter = new QSplitter(Qt::Horizontal);
    m_pLyt->addWidget(m_pSplitter, 1);

    // <TreeView>
    //-----------

    m_pWdgtTreeView = new CWdgtDiagramItemsTree(m_pDiagram);
    m_pWdgtTreeView->setMinimumWidth(180);
    m_pSplitter->addWidget(m_pWdgtTreeView);
    m_pSplitter->setChildrenCollapsible(false);

    QObject::connect(
        m_pWdgtTreeView, &CWdgtDiagramItemsTree::currentRowChanged,
        this, &CWdgtDiagramItems::onWdgtTreeViewCurrentRowChanged );

    // <NodeContent>
    //--------------

    m_pWdgtStackObjsProperties = new CWdgtStackDiagramItemsProperties(m_pDiagram);
    m_pWdgtStackObjsProperties->setMinimumWidth(180);
    m_pSplitter->addWidget(m_pWdgtStackObjsProperties);

} // ctor

//------------------------------------------------------------------------------
CWdgtDiagramItems::~CWdgtDiagramItems()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDiagram = nullptr;
    m_pLyt = nullptr;
    m_pSplitter = nullptr;
    m_pWdgtTreeView = nullptr;
    m_pWdgtStackObjsProperties = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagramItems::saveState(QSettings& i_settings) const
//------------------------------------------------------------------------------
{
    if( m_pSplitter != nullptr ) {
        QList<int> listSizes = m_pSplitter->sizes();
        for( int idx = 0; idx < listSizes.count(); idx++ ) {
            i_settings.setValue(
                ClassName() + "/" + objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx]);
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagramItems::restoreState(const QSettings& i_settings)
//------------------------------------------------------------------------------
{
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
void CWdgtDiagramItems::onWdgtTreeViewCurrentRowChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& i_modelIdxPrev )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Curr {" + CModelDiagramItems::modelIdx2Str(i_modelIdxCurr) + "}";
        strMthInArgs += ", Prev {" + CModelDiagramItems::modelIdx2Str(i_modelIdxPrev) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onWdgtTreeViewCurrentRowChanged",
        /* strMthInArgs */ strMthInArgs );

    if( i_modelIdxCurr.isValid() )
    {
        CModelDiagramTreeItem* pItem =
            static_cast<CModelDiagramTreeItem*>(i_modelIdxCurr.internalPointer());

        if( pItem != nullptr ) {
            m_pWdgtStackObjsProperties->setCurrentDiagObj(pItem->className(), pItem->objectName());
        }
        else {
            m_pWdgtStackObjsProperties->setCurrentDiagObj("", "");
        }
    }
}
