/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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
#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qstackedwidget.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qstackedwidget.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "UnitsWdgt.h"
#include "UnitModels.h"
#include "UnitViewWdgts.h"

#include "ZSPhysVal/ZSPhysTreeEntryGrpBase.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CWdgtUnits : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUnits::CWdgtUnits( const QString& i_strObjName, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pLyt(nullptr),
    m_pSplitter(nullptr),
    m_pUnitsModel(nullptr),
    m_pTreeView(nullptr),
    m_treeEntryTypeSelected(ETreeEntryTypeUndefined),
    m_pStackedWdgtUnitViews(nullptr)
    //m_arpWdgtUnitViews[ETreeEntryTypeCount]
{
    setObjectName(i_strObjName);

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pSplitter = new QSplitter();
    m_pSplitter->setChildrenCollapsible(false);
    m_pLyt->addWidget(m_pSplitter);

    // Tree view of physical units
    //----------------------------

    m_pUnitsModel = new CUnitsModel();

    m_pTreeView = new QTreeView();
    m_pSplitter->addWidget(m_pTreeView);
    m_pTreeView->setModel(m_pUnitsModel);

    m_pTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_pTreeView->setAllColumnsShowFocus(true);
    // Please note that the first column should not be hidden to allow
    // collapsing and expanding the tree view.
    m_pTreeView->showColumn(CModelIdxTree::EColumnTreeEntryName);
    m_pTreeView->hideColumn(CModelIdxTree::EColumnInternalId);
    m_pTreeView->hideColumn(CModelIdxTree::EColumnTreeEntryType);
    m_pTreeView->hideColumn(CModelIdxTree::EColumnIdxInTree);
    m_pTreeView->hideColumn(CModelIdxTree::EColumnIdxInParentBranch);
    m_pTreeView->hideColumn(CModelIdxTree::EColumnKeyInTree);
    m_pTreeView->hideColumn(CModelIdxTree::EColumnKeyInParentBranch);

    if( !QObject::connect(
        /* pObjSender   */ m_pTreeView,
        /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( expandedTreeView(const QModelIndex&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTreeView->selectionModel(),
        /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( selectedTreeEntryChanged(const QModelIndex&, const QModelIndex&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Content of selected physical unit
    //----------------------------------

    m_pStackedWdgtUnitViews = new QStackedWidget();
    m_pSplitter->addWidget(m_pStackedWdgtUnitViews);

    int iTreeEntryType;

    for( iTreeEntryType = 0; iTreeEntryType < ETreeEntryTypeCount; iTreeEntryType++ )
    {
        m_arpWdgtUnitViews[iTreeEntryType] = nullptr;
    }

    m_arpWdgtUnitViews[ETreeEntryTypeUndefined]                = new CWdgtUnitViewUndefined();
    //m_arpWdgtUnitViews[ETreeEntryTypeGrpRatios]                = new CWdgtUnitViewGrpRatios();
    //m_arpWdgtUnitViews[ETreeEntryTypeGrpDataQuantities]        = new CWdgtUnitViewGrpDataQuantities();
    m_arpWdgtUnitViews[ETreeEntryTypeGrpPhysScienceFields]     = new CWdgtUnitViewGrpPhysScienceFields();
    m_arpWdgtUnitViews[ETreeEntryTypeGrpUserDefinedQuantities] = new CWdgtUnitViewGrpUserDefinedQuantities();
    //m_arpWdgtUnitViews[ETreeEntryTypeUntRatio]                 = new CWdgtUnitViewUntRatio();
    //m_arpWdgtUnitViews[ETreeEntryTypeUntDataQuantity]          = new CWdgtUnitViewUntDataQuantity();
    m_arpWdgtUnitViews[ETreeEntryTypePhysUnit]                 = new CWdgtUnitViewPhysUnit();
    m_arpWdgtUnitViews[ETreeEntryTypeUntUserDefinedQuantity]   = new CWdgtUnitViewUntUserDefinedQuantity();

    for( iTreeEntryType = 0; iTreeEntryType < ETreeEntryTypeCount; iTreeEntryType++ )
    {
        m_pStackedWdgtUnitViews->addWidget(m_arpWdgtUnitViews[iTreeEntryType]);
    }
    m_pStackedWdgtUnitViews->setCurrentIndex(ETreeEntryTypeUndefined);

    // <Geometry>
    //--------------------------

    QSettings settings;

    QList<int> listSizes = m_pSplitter->sizes();

    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        listSizes[idx] = settings.value( objectName() + "/SplitterWidth" + QString::number(idx), 250 ).toInt();
    }
    m_pSplitter->setSizes(listSizes);

} // ctor

//------------------------------------------------------------------------------
CWdgtUnits::~CWdgtUnits()
//------------------------------------------------------------------------------
{
    // <Geometry>
    //-----------------------

    QSettings settings;

    QList<int> listSizes = m_pSplitter->sizes();

    for( int idx = 0; idx < listSizes.count(); idx++ )
    {
        settings.setValue( objectName() + "/SplitterWidth" + QString::number(idx), listSizes[idx] );
    }

    // <Clear>
    //-----------------------

    try
    {
        delete m_pUnitsModel;
    }
    catch(...)
    {
    }

    m_pLyt = nullptr;
    m_pSplitter = nullptr;
    m_pUnitsModel = nullptr;
    m_pTreeView = nullptr;
    m_pStackedWdgtUnitViews = nullptr;

    int iTreeEntryType;

    for( iTreeEntryType = 0; iTreeEntryType < ETreeEntryTypeCount; iTreeEntryType++ )
    {
        m_arpWdgtUnitViews[iTreeEntryType] = nullptr;
    }

} // dtor

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnits::showEvent( QShowEvent* i_pEv )
//------------------------------------------------------------------------------
{
    //int idxColumn;

    //for( idxColumn = 0; idxColumn < CModel::EColumnCount; idxColumn++ )
    //{
    //    if( m_pTreeView != nullptr )
    //    {
    //        m_pTreeView->resizeColumnToContents(idxColumn);
    //    }
    //}

    QWidget::showEvent(i_pEv);

} // showEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUnits::expandedTreeView( const QModelIndex& i_iModelIdx )
//------------------------------------------------------------------------------
{
    if( i_iModelIdx.isValid() )
    {
        if( m_pTreeView != nullptr )
        {
            m_pTreeView->resizeColumnToContents(i_iModelIdx.column());
        }
    }

} // expandedTreeView

//------------------------------------------------------------------------------
void CWdgtUnits::selectedTreeEntryChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    ETreeEntryType treeEntryType = ETreeEntryTypeUndefined;

    if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != nullptr )
    {
        SUnitsModelNode* pNode = static_cast<SUnitsModelNode*>(i_modelIdxCurr.internalPointer());

        if( pNode->m_pUnit != nullptr )
        {
            if( pNode->m_pUnit->isRatio() )
            {
                treeEntryType = ETreeEntryTypeUntRatio;
            }
            else if( pNode->m_pUnit->classType() == EUnitClassType::Quantity )
            {
                treeEntryType = ETreeEntryTypeUntDataQuantity;
            }
            else if( pNode->m_pUnit->classType() == EUnitClassType::PhysSize )
            {
                treeEntryType = ETreeEntryTypePhysUnit;
            }
        }

        else if( pNode->m_pUnitGrp != nullptr )
        {
            if( pNode->m_pUnitGrp->classType() == EUnitClassType::Ratio )
            {
                treeEntryType = ETreeEntryTypeGrpRatios;
            }
            else if( pNode->m_pUnitGrp->classType() == EUnitClassType::Quantity )
            {
                treeEntryType = ETreeEntryTypeGrpDataQuantities;
            }
            else if( pNode->m_pUnitGrp->classType() == EUnitClassType::PhysSize )
            {
                treeEntryType = ETreeEntryTypeGrpPhysScienceFields;
            }
        }

        CWdgtUnitView* pWdgtUnitView = m_arpWdgtUnitViews[treeEntryType];

        pWdgtUnitView->setUnitsModelNode(pNode);

    } // if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != nullptr )

    m_pStackedWdgtUnitViews->setCurrentIndex(treeEntryType);

} // selectedTreeEntryChanged
