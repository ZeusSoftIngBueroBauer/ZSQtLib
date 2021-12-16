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
#include <QtGui/qevent.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qheaderview.h>
#include <QtGui/qmenu.h>
#else
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qmenu.h>
#endif

#include "ZSTestGUI/ZSTestStepIdxTreeView.h"
#include "ZSTestGUI/ZSTestStepIdxTreeDelegate.h"
#include "ZSTestGUI/ZSTestStepIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CTreeViewIdxTreeTestSteps : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTreeTestSteps::CTreeViewIdxTreeTestSteps(
    CModeldxTreeTestSteps* i_pModel,
    QWidget*               i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pDelegate(nullptr),
    m_pMenuGroupContext(nullptr),
    m_pActionTitle(nullptr),
    m_pActionEnableTestGroups(nullptr),
    m_pActionDisableTestGroups(nullptr),
    m_pTreeEntryGroupContextAction(nullptr)
{
    m_pDelegate = new CDelegateIdxTreeTestSteps(this);

    setModel(i_pModel);
    setItemDelegate(m_pDelegate);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAllColumnsShowFocus(true);

    setColumnHidden(CModeldxTreeTestSteps::EColumnName, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnBreakpoint, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnEnabled, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnResult, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnDuration, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnOperation, true);
    setColumnHidden(CModeldxTreeTestSteps::EColumnDescription, true);
    setColumnHidden(CModeldxTreeTestSteps::EColumnExpectedValues, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnResultValues, false);

    m_pMenuGroupContext = new QMenu("Test Group",this);

    m_pActionTitle = new QAction("Test Group:",this);

    QFont fntActionTitle = m_pActionTitle->font();
    fntActionTitle.setBold(true);
    m_pActionTitle->setFont(fntActionTitle);
    m_pMenuGroupContext->addAction(m_pActionTitle);

    m_pMenuGroupContext->addSeparator();

    m_pActionEnableTestGroups = new QAction("Recursively Enable Test Groups",this);
    m_pMenuGroupContext->addAction(m_pActionEnableTestGroups);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionEnableTestGroups,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionEnableTestGroupsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionDisableTestGroups = new QAction("Recursively Disable Test Groups",this);
    m_pMenuGroupContext->addAction(m_pActionDisableTestGroups);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionDisableTestGroups,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionDisableTestGroupsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    QHeaderView* pHdrView = header();

    #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    if( !pHdrView->isClickable() )
    {
        pHdrView->setClickable(true);
    }
    #else
    if( !pHdrView->sectionsClickable() )
    {
        pHdrView->setSectionsClickable(true);
    }
    #endif

    pHdrView->setContextMenuPolicy(Qt::CustomContextMenu);

    #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    if( pHdrView->isClickable() )
    #else
    if( pHdrView->sectionsClickable() )
    #endif
    {
        if( !QObject::connect(
            /* pObjSender   */ header(),
            /* szSignal     */ SIGNAL( customContextMenuRequested(const QPoint&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onHeaderCustomContextMenuRequested(const QPoint&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( pHdrView->sectionsClickable() )

} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTreeTestSteps::~CTreeViewIdxTreeTestSteps()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pDelegate;
    }
    catch(...)
    {
    }

    m_pDelegate = nullptr;
    m_pMenuGroupContext = nullptr;
    m_pActionTitle = nullptr;
    m_pActionEnableTestGroups = nullptr;
    m_pActionDisableTestGroups = nullptr;
    m_pTreeEntryGroupContextAction = nullptr;

} // dtor

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::keyPressEvent(QKeyEvent* i_pEv)
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pEv->key() == Qt::Key_F5 )
    {
        QAbstractItemModel* pModelAbstract = const_cast<QAbstractItemModel*>(model());

        if( pModelAbstract != nullptr )
        {
            CModeldxTreeTestSteps* pModel = dynamic_cast<CModeldxTreeTestSteps*>(pModelAbstract);

            if( pModel != nullptr )
            {
                //pModel->reset();
            }
        }
    }
    else if( i_pEv->key() == Qt::Key_Return || i_pEv->key() == Qt::Key_Enter || i_pEv->key() == Qt::Key_Space )
    {
        QModelIndex modelIdxSelected = selectionModel()->currentIndex();

        CModelIdxTreeEntry* pModelTreeEntry = nullptr;

        if( modelIdxSelected.isValid() )
        {
            pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());
        }

        CIdxTreeEntry* pIdxTreeEntry = nullptr;

        if( pModelTreeEntry != nullptr )
        {
            pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
        }

        if( pIdxTreeEntry != nullptr )
        {
            CAbstractTestStepIdxTreeEntry* pTestStepEntry = nullptr;
            CTestStepGroup*                pTestStepGroup = nullptr;
            CTestStep*                     pTestStep = nullptr;

            if( !pIdxTreeEntry->isRoot() )
            {
                pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

                if( pTestStepEntry->isGroup() )
                {
                    pTestStepGroup = dynamic_cast<CTestStepGroup*>(pTestStepEntry);
                }
                else
                {
                    pTestStep = dynamic_cast<CTestStep*>(pTestStepEntry);
                }
            }

            switch( modelIdxSelected.column() )
            {
                case CModeldxTreeTestSteps::EColumnName:
                {
                    break;
                }
                case CModeldxTreeTestSteps::EColumnBreakpoint:
                {
                    if( pTestStep != nullptr )
                    {
                        if( pTestStep->isBreakpointSet() )
                        {
                            pTestStep->removeBreakpoint();
                        }
                        else
                        {
                            pTestStep->setBreakpoint();
                        }
                        //pTestStep->update();
                        bEventHandled = true;
                    }
                    break;
                }
                case CModeldxTreeTestSteps::EColumnEnabled:
                {
                    if( pTestStepEntry->getEnabled() == EEnabled::Yes )
                    {
                        pTestStepEntry->setEnabled(EEnabled::No);
                    }
                    else
                    {
                        pTestStepEntry->setEnabled(EEnabled::Yes);
                    }
                    //pTestStepIdxTreeEntry->update();
                    bEventHandled = true;
                    break;
                }
                case CModeldxTreeTestSteps::EColumnResult:
                case CModeldxTreeTestSteps::EColumnDuration:
                case CModeldxTreeTestSteps::EColumnOperation:
                case CModeldxTreeTestSteps::EColumnDescription:
                case CModeldxTreeTestSteps::EColumnExpectedValues:
                case CModeldxTreeTestSteps::EColumnResultValues:
                default:
                {
                    break;
                }
            } // switch( modelIdxSelected.column() )
        } // if( pIdxTreeEntry != nullptr )
    } // if( i_pEv->key() == Qt::Key_Return || i_pEv->key() == Qt::Key_Enter || i_pEv->key() == Qt::Key_Space )

    if( !bEventHandled )
    {
        QTreeView::keyPressEvent(i_pEv);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pEv->buttons() & Qt::RightButton )
    {
        QModelIndex modelIdxSelected = indexAt(i_pEv->pos());

        CModelIdxTreeEntry* pModelTreeEntry = nullptr;

        if( modelIdxSelected.isValid() )
        {
            pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());
        }

        CIdxTreeEntry* pIdxTreeEntry = nullptr;

        if( pModelTreeEntry != nullptr )
        {
            pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
        }

        if( pIdxTreeEntry != nullptr )
        {
            CAbstractTestStepIdxTreeEntry* pTestStepEntry = nullptr;
            CTestStepGroup*                pTestStepGroup = nullptr;

            if( !pIdxTreeEntry->isRoot() )
            {
                pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

                if( pTestStepEntry->isGroup() )
                {
                    pTestStepGroup = dynamic_cast<CTestStepGroup*>(pTestStepEntry);
                }
            }

            if( pTestStepGroup != nullptr )
            {
                switch( modelIdxSelected.column() )
                {
                    case CModeldxTreeTestSteps::EColumnName:
                    {
                        if( m_pMenuGroupContext != nullptr )
                        {
                            m_pActionTitle->setText( "Name Space: " + pTestStepGroup->name() );
                            m_pTreeEntryGroupContextAction = pTestStepGroup;
                            m_pMenuGroupContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                        }
                        bEventHandled = true;
                        break;
                    }
                    case CModeldxTreeTestSteps::EColumnBreakpoint:
                    case CModeldxTreeTestSteps::EColumnEnabled:
                    case CModeldxTreeTestSteps::EColumnResult:
                    case CModeldxTreeTestSteps::EColumnDuration:
                    case CModeldxTreeTestSteps::EColumnOperation:
                    case CModeldxTreeTestSteps::EColumnDescription:
                    case CModeldxTreeTestSteps::EColumnExpectedValues:
                    case CModeldxTreeTestSteps::EColumnResultValues:
                    default:
                    {
                        break;
                    }
                } // switch( modelIdxSelected.column() )
            } // if( pTestStepGroup != nullptr )
        } // if( pIdxTreeEntry != nullptr )
    } // if( i_pEv->buttons() & Qt::RightButton )

    if( !bEventHandled )
    {
        QTreeView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    QModelIndex modelIdxSelected = indexAt(i_pEv->pos());

    CDelegateIdxTreeTestSteps* pDelegate = dynamic_cast<CDelegateIdxTreeTestSteps*>(itemDelegate());

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( modelIdxSelected.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());
    }

    CIdxTreeEntry* pIdxTreeEntry = nullptr;

    if( pModelTreeEntry != nullptr )
    {
        pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
    }

    if( pIdxTreeEntry != nullptr )
    {
        CAbstractTestStepIdxTreeEntry* pTestStepEntry = nullptr;
        CTestStepGroup*                pTestStepGroup = nullptr;
        CTestStep*                     pTestStep = nullptr;

        if( !pIdxTreeEntry->isRoot() )
        {
            pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

            if( pTestStepEntry->isGroup() )
            {
                pTestStepGroup = dynamic_cast<CTestStepGroup*>(pTestStepEntry);
            }
            else
            {
                pTestStep = dynamic_cast<CTestStep*>(pTestStepEntry);
            }
        }

        switch( modelIdxSelected.column() )
        {
            case CModeldxTreeTestSteps::EColumnName:
            {
                //if( pTestStepIdxTreeEntry->isSelected() )
                //{
                //    pTestStepIdxTreeEntry->resetStyleStateFlag(QStyle::State_Selected);
                //}
                break;
            }
            case CModeldxTreeTestSteps::EColumnBreakpoint:
            {
                QRect rectVisual = visualRect(modelIdxSelected);

                if( pDelegate != nullptr && pDelegate->isCheckBoxBreakpointHit(rectVisual,i_pEv->pos(),modelIdxSelected) )
                {
                    if( pTestStep != nullptr )
                    {
                        if( pTestStep->isBreakpointSet() )
                        {
                            pTestStep->removeBreakpoint();
                        }
                        else
                        {
                            pTestStep->setBreakpoint();
                        }
                        //pTestStepIdxTreeEntry->update();
                    }
                    bEventHandled = true;
                }
                break;
            }
            case CModeldxTreeTestSteps::EColumnEnabled:
            {
                QRect rectVisual = visualRect(modelIdxSelected);

                if( pDelegate != nullptr && pDelegate->isCheckBoxEnabledHit(rectVisual,i_pEv->pos(),modelIdxSelected) )
                {
                    if( pTestStepEntry->getEnabled() == EEnabled::Yes )
                    {
                        pTestStepEntry->setEnabled(EEnabled::No);
                    }
                    else
                    {
                        pTestStepEntry->setEnabled(EEnabled::Yes);
                    }
                    //pTestStepIdxTreeEntry->update();
                    bEventHandled = true;
                }
                break;
            }
            case CModeldxTreeTestSteps::EColumnOperation:
            case CModeldxTreeTestSteps::EColumnDescription:
            case CModeldxTreeTestSteps::EColumnExpectedValues:
            case CModeldxTreeTestSteps::EColumnResultValues:
            case CModeldxTreeTestSteps::EColumnDuration:
            case CModeldxTreeTestSteps::EColumnResult:
            default:
            {
                break;
            }
        } // switch( modelIdxSelected.column() )
    } // if( pIdxTreeEntry != nullptr )

    if( !bEventHandled )
    {
        QTreeView::mouseReleaseEvent(i_pEv);
    }

} // mouseReleaseEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::onHeaderCustomContextMenuRequested( const QPoint& i_pt )
//------------------------------------------------------------------------------
{
    QPoint ptGlobal = mapToGlobal(i_pt);

    CModeldxTreeTestSteps* pModelIdxTree = dynamic_cast<CModeldxTreeTestSteps*>(model());

    QMenu*   pMenuHeaderView = new QMenu("", this);
    QAction* pAction;
    QString  strClmName;
    bool     bClmVisible;
    int      idxClm;

    for( idxClm = 0; idxClm < CModeldxTreeTestSteps::EColumnCount; idxClm++ )
    {
        bClmVisible = !isColumnHidden(idxClm);

        if( idxClm != CModeldxTreeTestSteps::EColumnName )
        {
            strClmName = CModeldxTreeTestSteps::column2Str(idxClm);
            pAction = new QAction(strClmName, this);
            pAction->setCheckable(true);
            pAction->setChecked(bClmVisible);
            pAction->setData(idxClm);
            pMenuHeaderView->addAction(pAction);
        }
    } // for( idxClm = 0; idxClm < CModeldxTreeTestSteps::EColumnCount; idxClm++ )

    if( !isColumnHidden(CModeldxTreeTestSteps::EColumnExpectedValues)
     || !isColumnHidden(CModeldxTreeTestSteps::EColumnResultValues) )
    {
        pMenuHeaderView->addSeparator();

        QActionGroup* pActGrpShowValueClms = new QActionGroup(this);

        QAction* pActionShowAlways = new QAction("Show Always", this);
        pActionShowAlways->setCheckable(true);
        pActionShowAlways->setData(-1);
        pMenuHeaderView->addAction(pActionShowAlways);
        pActGrpShowValueClms->addAction(pActionShowAlways);

        QAction* pActionOnlyErrors = new QAction("Only Errors", this);
        pActionOnlyErrors->setCheckable(true);
        pActionOnlyErrors->setData(-2);
        pMenuHeaderView->addAction(pActionOnlyErrors);
        pActGrpShowValueClms->addAction(pActionOnlyErrors);

        if( pModelIdxTree->isSetShowExpectedAndResultValuesOnlyIfTestStepFailed() )
        {
            pActionOnlyErrors->setChecked(true);
        }
        else
        {
            pActionShowAlways->setChecked(true);
        }
    }

    QAction* pActSelected = pMenuHeaderView->exec(ptGlobal);

    if( pActSelected != nullptr )
    {
        idxClm = pActSelected->data().toInt();

        if( idxClm >= 0 && idxClm < CModeldxTreeTestSteps::EColumnCount )
        {
            setColumnHidden(idxClm, !pActSelected->isChecked());
        }
        else if( idxClm == -1 )
        {
            pModelIdxTree->setShowExpectedAndResultValuesOnlyIfTestStepFailed(false);
        }
        else if( idxClm == -2 )
        {
            pModelIdxTree->setShowExpectedAndResultValuesOnlyIfTestStepFailed(true);
        }
    }

} // onHeaderCustomContextMenuRequested

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::onActionEnableTestGroupsTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CModeldxTreeTestSteps* pModelIdxTree = dynamic_cast<CModeldxTreeTestSteps*>(model());

    if( m_pTreeEntryGroupContextAction != nullptr && pModelIdxTree != nullptr )
    {
        //m_pTestStepsModel->updateGroup(
        //    /* pTreeEntry */ m_pTreeEntryGroupContextAction,
        //    /* enabled    */ EEnabled::Yes );
    }

} // onActionEnableTestGroupsTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::onActionDisableTestGroupsTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CModeldxTreeTestSteps* pModelIdxTree = dynamic_cast<CModeldxTreeTestSteps*>(model());

    if( m_pTreeEntryGroupContextAction != nullptr && pModelIdxTree != nullptr )
    {
        //m_pTestStepsModel->updateGroup(
        //    /* pTreeEntry */ m_pTreeEntryGroupContextAction,
        //    /* enabled    */ EEnabled::No );
    }

} // onActionDisableTestGroupsTriggered
