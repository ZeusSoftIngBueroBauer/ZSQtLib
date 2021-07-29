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

#include "ZSSysGUI/ZSSysIdxTreeModelBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntries.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;


/*******************************************************************************
class CModelIdxTreeBranchContent : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeBranchContent::CModelIdxTreeBranchContent(
    CIdxTree* i_pIdxTree,
    QObject*  i_pObjParent,
    int       i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pIdxTree(i_pIdxTree),
    m_sortOrder(EIdxTreeSortOrder::Config),
    m_pBranch(nullptr),
    m_pModelBranch(nullptr),
    m_iTrcDetailLevel(i_iTrcDetailLevel)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CModelIdxTreeBranchContent::~CModelIdxTreeBranchContent()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pModelBranch != nullptr )
    {
        clear(m_pModelBranch);

        try
        {
            delete m_pModelBranch;
        }
        catch(...)
        {
        }
    }

    m_pIdxTree = nullptr;
    m_sortOrder = static_cast<EIdxTreeSortOrder>(0);
    m_pBranch = nullptr;
    m_pModelBranch = nullptr;
    m_iTrcDetailLevel = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::setBranch( CBranchIdxTreeEntry* i_pBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        if( m_pBranch == nullptr )
        {
            strMthInArgs = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        }
        else
        {
            strMthInArgs  = "BranchOrig: " + QString(m_pBranch == nullptr ? "nullptr" : m_pBranch->keyInTree());
            strMthInArgs += ", BranchNew: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        }
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setBranch",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pBranch != nullptr )
    {
        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pIdxTree->mutex());

        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeAboutToBeDestroyed(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryAdded(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryAdded(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryChanged(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryChanged(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryAboutToBeRemoved(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryAboutToBeRemoved(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryMoved(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*, const QString&, ZS::System::CBranchIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryMoved(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*, const QString&, ZS::System::CBranchIdxTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryKeyInTreeChanged(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryKeyInTreeChanged(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*, const QString&)) );

        int iRowCountPrev = rowCount();

        if( iRowCountPrev > 0 )
        {
            beginRemoveRows(QModelIndex(), 0, iRowCountPrev-1);
        }

        // The root entry will neither be added to the list nor to the map of tree entries.
        delete m_pModelBranch;
        m_pModelBranch = nullptr;

        m_pIdxTree = nullptr;

        if( iRowCountPrev > 0 )
        {
            endRemoveRows();
        }

        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        //reset();
        #else
        //beginResetModel();
        //endResetModel();
        #endif

    } // if( m_pBranch != nullptr )

    m_pBranch = i_pBranch;

    if( m_pBranch != nullptr )
    {
        m_pIdxTree = m_pBranch->tree();

        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pIdxTree->mutex());

        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeAboutToBeDestroyed(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryAdded(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryAdded(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryChanged(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryChanged(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryAboutToBeRemoved(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryAboutToBeRemoved(ZS::System::CIdxTree*,ZS::System::CAbstractIdxTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryMoved(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*, const QString&, ZS::System::CBranchIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryMoved(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*, const QString&, ZS::System::CBranchIdxTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryKeyInTreeChanged(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryKeyInTreeChanged(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*, const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_pModelBranch = new CModelBranchTreeEntry(m_pBranch);

        m_pModelBranch->setSortOrder(m_sortOrder);

        CAbstractIdxTreeEntry* pTreeEntry;
        int idxEntry;

        for( idxEntry = 0; idxEntry < m_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = m_pBranch->at(idxEntry);
            onIdxTreeEntryAdded(m_pIdxTree, pTreeEntry);
        }
    } // if( m_pBranch != nullptr )

} // setBranch

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_sortOrder != i_sortOrder )
    {

        if( m_pModelBranch != nullptr && m_pModelBranch->count() > 1 )
        {
            QMap<QString, CModelAbstractTreeEntry*> mappTreeEntries;

            CModelAbstractTreeEntry* pModelTreeEntryChild;
            int                      idxEntryChild;

            beginRemoveRows(QModelIndex(), 0, m_pModelBranch->count()-1);

            for( idxEntryChild = m_pModelBranch->count()-1; idxEntryChild >= 0; --idxEntryChild )
            {
                pModelTreeEntryChild = m_pModelBranch->at(idxEntryChild);
                mappTreeEntries.insert(pModelTreeEntryChild->keyInParentBranch(), pModelTreeEntryChild);
                m_pModelBranch->remove(pModelTreeEntryChild);
            }

            endRemoveRows();

            beginInsertRows(QModelIndex(), 0, mappTreeEntries.size()-1);

            m_pModelBranch->setSortOrder(i_sortOrder);

            if( i_sortOrder == EIdxTreeSortOrder::Ascending )
            {
                for( auto strKeyInParentBranch : mappTreeEntries.keys() )
                {
                    pModelTreeEntryChild = mappTreeEntries.value(strKeyInParentBranch, nullptr);
                    m_pModelBranch->add(pModelTreeEntryChild);
                }
            }
            else // if( i_sortOrder == EIdxTreeSortOrder::None )
            {
                CBranchIdxTreeEntry*   pBranch = dynamic_cast<CBranchIdxTreeEntry*>(m_pModelBranch->treeEntry());
                CAbstractIdxTreeEntry* pTreeEntryChild;

                for( idxEntryChild = 0; idxEntryChild < pBranch->count(); ++idxEntryChild )
                {
                    pTreeEntryChild = pBranch->at(idxEntryChild);
                    pModelTreeEntryChild = mappTreeEntries.value(pTreeEntryChild->keyInParentBranch());
                    m_pModelBranch->add(pModelTreeEntryChild);
                }
            }  // if( i_sortOrder == EIdxTreeSortOrder::None )

            endInsertRows();

        } // if( m_pModelBranch != nullptr && m_pModelBranch->count() > 1 )

        m_sortOrder = i_sortOrder;

    } // if( m_sortOrder != i_sortOrder )

} // setSortOrder

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelBranchTreeEntry* CModelIdxTreeBranchContent::findModelBranch( CBranchIdxTreeEntry* i_pBranch )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());
    CModelBranchTreeEntry* pModelBranch = nullptr;
    QString strKeyInParentBranch = i_pBranch->keyInParentBranch();
    if( m_pModelBranch != nullptr )
    {
        pModelBranch = m_pModelBranch->findModelBranch(strKeyInParentBranch);
    }
    return pModelBranch;

} // findModelBranch

//------------------------------------------------------------------------------
CModelLeaveTreeEntry* CModelIdxTreeBranchContent::findModelLeave( CLeaveIdxTreeEntry* i_pLeave )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());
    CModelLeaveTreeEntry* pModelLeave = nullptr;
    QString strKeyInParentBranch = i_pLeave->keyInParentBranch();
    if( m_pModelBranch != nullptr )
    {
        pModelLeave = m_pModelBranch->findModelLeave(strKeyInParentBranch);
    }
    return pModelLeave;

} // findModelLeave

//------------------------------------------------------------------------------
CModelAbstractTreeEntry* CModelIdxTreeBranchContent::findModelEntry( CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());
    QString strName = i_pTreeEntry->name();
    EIdxTreeEntryType entryType = i_pTreeEntry->entryType();
    CModelAbstractTreeEntry* pModelEntry = nullptr;
    if( m_pModelBranch != nullptr )
    {
        pModelEntry = m_pModelBranch->findModelEntry(entryType, strName);
    }
    return pModelEntry;

} // findModelEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//QModelIndex CModelIdxTreeBranchContent::index( const QString& i_strKeyInParentBranch ) const
////------------------------------------------------------------------------------
//{
//    QModelIndex modelIdx;
//
//    if( m_pModelBranch != nullptr )
//    {
//        int iRow = m_pModelBranch->indexOf(i_strKeyInParentBranch);
//        int iClm = 0;
//
//        modelIdx = createIndex(iRow, iClm);
//    }
//
//    return modelIdx;
//
//} // index

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeAboutToBeDestroyed( QObject* i_pIdxTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeAboutToBeDestroyed",
        /* strMethodInArgs    */ strMthInArgs );

    m_pIdxTree = nullptr;

    //beginRemoveRows(QModelIndex(), 0, 0);

    clear();

    // The root entry will neither be added to the list nor to the map of tree entries.
    delete m_pModelBranch;
    m_pModelBranch = nullptr;

    //endRemoveRows();

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    reset();
    #else
    beginResetModel();
    endResetModel();
    #endif

} // onIdxTreeAboutToBeDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryAdded(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryAdded",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pIdxTree == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pIdxTree == nullptr");
    }
    if( i_pIdxTree != m_pIdxTree )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pIdxTree != m_pIdxTree");
    }
    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTreeEntry == nullptr");
    }

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    CModelAbstractTreeEntry* pModelTreeEntry = findModelEntry(i_pTreeEntry);

    if( pModelTreeEntry == nullptr )
    {
        CBranchIdxTreeEntry* pBranch = nullptr;
        CLeaveIdxTreeEntry*  pLeave  = nullptr;

        if( i_pTreeEntry->isChildOf(m_pBranch) )
        {
            if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
            {
                pBranch = dynamic_cast<CBranchIdxTreeEntry*>(i_pTreeEntry);
                pModelTreeEntry = new CModelBranchTreeEntry(pBranch);
            }
            else if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
            {
                pLeave = dynamic_cast<CLeaveIdxTreeEntry*>(i_pTreeEntry);
                pModelTreeEntry = new CModelLeaveTreeEntry(pLeave);
            }
        }

        if( pModelTreeEntry != nullptr )
        {
            int idxInParentBranch = m_pModelBranch->add(pModelTreeEntry);

            beginInsertRows(QModelIndex(), idxInParentBranch, idxInParentBranch);
            endInsertRows();

        } // if( pModelTreeEntry != nullptr )
    } // if( pModelTreeEntry == nullptr )

} // onIdxTreeEntryAdded

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryChanged(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    //QModelIndex modelIdxTopLeft = createIndex(i_pTreeEntry->index(), 0, i_pTreeEntry);
    //QModelIndex modelIdxBottomRight = createIndex(i_pTreeEntry->index(), EColumnCount, i_pTreeEntry);

    //emit QAbstractItemModel::dataChanged(modelIdxTopLeft, modelIdxBottomRight);

} // onIdxTreeEntryChanged

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryAboutToBeRemoved(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryAboutToBeRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    CModelAbstractTreeEntry* pModelTreeEntry = findModelEntry(i_pTreeEntry);

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            clear(dynamic_cast<CModelBranchTreeEntry*>(pModelTreeEntry));
        }

        remove(pModelTreeEntry);

        delete pModelTreeEntry;
        pModelTreeEntry = nullptr;
    }

} // onIdxTreeEntryAboutToBeRemoved

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryMoved(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strKeyInTreePrev,
    CBranchIdxTreeEntry*   i_pTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", KeyInTreePrev: " + i_strKeyInTreePrev;
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryMoved",
        /* strMethodInArgs    */ strMthInArgs );

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    // Please note that before the index tree emits the "treeEntryMoved" signal
    // and this slot is executed the "treeEntryKeyInTreeChanged" signal has been
    // emitted and the slot "onIdxTreeEntryKeyInTreeChanged" was processed.
    // Which means that the "KeyInTreePrev" has already been updated and is no
    // longer part of the models map. But the model node can be found using
    // the new key in tree.
    CModelAbstractTreeEntry* pModelTreeEntry    = findModelEntry(i_pTreeEntry);
    CModelBranchTreeEntry*   pModelTargetBranch = findModelBranch(i_pTargetBranch);

    if( pModelTreeEntry != nullptr && pModelTargetBranch != nullptr )
    {
        CModelBranchTreeEntry* pModelParentBranch = pModelTreeEntry->modelParentBranch();

        pModelParentBranch->remove(pModelTreeEntry);

        // I am surprised. It is not necessary to call beginRemoveRows and endRemoveRows.
        // Calling beginInsertRows and endInsertRows is sufficient to update the tree view.
        // On the contrary - if we would call beginRemoveRows and endRemoveRows the target
        // tree branch would be collapsed - which is not nice.
        // The model does not crash - neither with or without begin/endRemoveRows.

        //CModelBranchTreeEntry* pModelGrandParentBranch = pModelParentBranch->modelParentBranch();

        //int idxOfParentBranchInGrandParentBranch = pModelParentBranch->modelIndexInParentBranch();

        //QModelIndex modelIdxGrandParent = createIndex(pModelParentBranch->modelIndexInParentBranch(), 0, pModelGrandParentBranch);

        //beginRemoveRows(modelIdxGrandParent, idxOfParentBranchInGrandParentBranch, idxOfParentBranchInGrandParentBranch);
        //endRemoveRows();

        pModelTargetBranch->add(pModelTreeEntry);

        QModelIndex modelIdxTargetBranch = createIndex(pModelTargetBranch->modelIndexInParentBranch(), 0, pModelTargetBranch);

        beginInsertRows(modelIdxTargetBranch, pModelTreeEntry->modelIndexInParentBranch(), pModelTreeEntry->modelIndexInParentBranch());
        endInsertRows();

    } // if( pModelTreeEntry != nullptr && pModelTargetBranch != nullptr )

} // onIdxTreeEntryMoved

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryKeyInTreeChanged(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strKeyInTreePrev )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", KeyInTreePrev: " + i_strKeyInTreePrev;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryKeyInTreeChanged",
        /* strMethodInArgs    */ strMthInArgs );

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

#if 0
    CModelAbstractTreeEntry* pModelTreeEntry = m_mappModelTreeEntries.value(i_strKeyInTreePrev, nullptr);
    CModelBranchTreeEntry*   pModelParentBranch = pModelTreeEntry->modelParentBranch();

    if( pModelParentBranch != nullptr )
    {
        QString strBranchPath;
        QString strNamePrev;

        m_pIdxTree->splitPathStr(i_strKeyInTreePrev, &strBranchPath, &strNamePrev );
        QString strKeyInBranchPrev = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol) + ":" + strNamePrev;

        QModelIndex modelIdxParent = createIndex(pModelParentBranch->modelIndexInParentBranch(), 0, pModelParentBranch);

        int idxInParentBranch = pModelTreeEntry->modelIndexInParentBranch();

        beginRemoveRows(modelIdxParent, idxInParentBranch, idxInParentBranch);

        pModelParentBranch->remove(strKeyInBranchPrev);

        m_mappModelTreeEntries.remove(i_strKeyInTreePrev);

        QString strKeyInTree = i_pTreeEntry->keyInTree();

        if( m_mappModelTreeEntries.contains(strKeyInTree) )
        {
            throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKeyInTree);
        }

        endRemoveRows();

        idxInParentBranch = pModelParentBranch->add(pModelTreeEntry);

        beginInsertRows(modelIdxParent, idxInParentBranch, idxInParentBranch);

        m_mappModelTreeEntries.insert(strKeyInTree, pModelTreeEntry);

        endInsertRows();

        //QModelIndex modelIdxParent = createIndex(pModelParentBranch->modelIndexInParentBranch(), 0, pModelParentBranch);

        //QModelIndex modelIdxTL = index(pModelTreeEntry->modelIndexInParentBranch(), 0, modelIdxParent);
        //QModelIndex modelIdxBR = index(pModelTreeEntry->modelIndexInParentBranch(), columnCount()-1, modelIdxParent);

        //emit dataChanged(modelIdxTL, modelIdxBR);

    } // if( pModelParentBranch != nullptr )
#endif

} // onIdxTreeEntryKeyInTreeChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::clear( bool i_bDestroyTreeEntries )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "DestroyEntries: " + bool2Str(i_bDestroyTreeEntries);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "clear",
        /* strMethodInArgs    */ strMthInArgs );

    CModelAbstractTreeEntry* pModelTreeEntry;
    int                      idxEntry;

    for( idxEntry = m_pModelBranch->count()-1; idxEntry >= 0; --idxEntry )
    {
        pModelTreeEntry = m_pModelBranch->at(idxEntry);

        remove(pModelTreeEntry);

        if( i_bDestroyTreeEntries )
        {
            delete pModelTreeEntry;
            pModelTreeEntry = nullptr;
        }
    }
} // clear

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::remove( CModelAbstractTreeEntry* i_pModelTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pModelTreeEntry == nullptr ? "nullptr" : i_pModelTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "remove",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pModelTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModelTreeEntry == nullptr");
    }

    int idxInParentBranch = i_pModelTreeEntry->modelIndexInParentBranch();

    if( m_pModelBranch != nullptr && idxInParentBranch >= 0 )
    {
        beginRemoveRows(QModelIndex(), idxInParentBranch, idxInParentBranch);

        m_pModelBranch->remove(i_pModelTreeEntry);

        endRemoveRows();

    } // if( m_pModelBranch != nullptr && idxInParentBranch >= 0 )

} // remove

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdxParent {" + CModelIdxTree::ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rowCount",
        /* strMethodInArgs    */ strMthInArgs );

    int iRowCount = 0;

    if( m_pModelBranch != nullptr )
    {
        iRowCount = m_pModelBranch->count();
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(iRowCount));
    }

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdxParent {" + CModelIdxTree::ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(EColumnCount));
    }

    return EColumnCount;

} // columnCount

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelIdxTreeBranchContent::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "headerData",
        /* strMethodInArgs    */ strMthInArgs );

    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        switch( i_iSection )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Name";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnInternalId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Internal-Id";
                }
                break;
            }
            case EColumnTreeEntryType:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Type";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnIdxInTree:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "IdxInTree";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnIdxInParentBranch:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "IdxInParentBranch";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnKeyInTree:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "KeyInTree";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnKeyInParentBranch:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "KeyInParentBranch";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            default:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "?";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
        }
    }
    else // if( i_orientation == Qt::Vertical )
    {
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(varData.toString());
    }

    return varData;

} // headerData

//------------------------------------------------------------------------------
Qt::ItemFlags CModelIdxTreeBranchContent::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "flags",
        /* strMethodInArgs    */ strMthInArgs );

    Qt::ItemFlags uFlags = QAbstractItemModel::flags(i_modelIdx);

    CModelAbstractTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() && m_pModelBranch != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelBranch->count() )
        {
            pModelTreeEntry = m_pModelBranch->at(i_modelIdx.row());
        }
    }

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root )
        {
            uFlags = uFlags | Qt::ItemIsDropEnabled;
        }
        else if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            uFlags = uFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
        }
        else if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            uFlags = uFlags | Qt::ItemIsDragEnabled;
        }

        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( pModelTreeEntry != m_pModelBranch )
                {
                    uFlags = uFlags | Qt::ItemIsEditable;
                }
                uFlags = uFlags | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
                break;
            }
            case EColumnInternalId:
            {
                break;
            }
            case EColumnTreeEntryType:
            {
                break;
            }
            case EColumnIdxInTree:
            {
                break;
            }
            case EColumnIdxInParentBranch:
            {
                break;
            }
            case EColumnKeyInTree:
            {
                break;
            }
            case EColumnKeyInParentBranch:
            {
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pModelTreeEntry != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }

    return uFlags;

} // flags

//------------------------------------------------------------------------------
Qt::DropActions CModelIdxTreeBranchContent::supportedDropActions() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "supportedDropActions",
        /* strMethodInArgs    */ strMthInArgs );

    Qt::DropActions dropActions = Qt::CopyAction | Qt::MoveAction;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(qDropActions2Str(dropActions));
    }

    return dropActions;

} // supportedDropActions

//------------------------------------------------------------------------------
QVariant CModelIdxTreeBranchContent::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "data",
        /* strMethodInArgs    */ strMthInArgs );

    QVariant varData;

    CModelAbstractTreeEntry* pModelTreeEntry = nullptr;

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    if( i_modelIdx.isValid() && m_pModelBranch != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelBranch->count() )
        {
            pModelTreeEntry = m_pModelBranch->at(i_modelIdx.row());
        }
    }

    if( pModelTreeEntry != nullptr )
    {
        CModelBranchTreeEntry* pModelBranch = nullptr;
        //CModelLeaveTreeEntry*  pModelLeave  = nullptr;

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            pModelBranch = dynamic_cast<CModelBranchTreeEntry*>(pModelTreeEntry);
        }
        else if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            //pModelLeave = dynamic_cast<CModelLeaveTreeEntry*>(pModelTreeEntry);
        }

        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole || i_iRole == Qt::ToolTipRole || i_iRole == Qt::ForegroundRole)
                {
                    QString strKeyInTree = pModelTreeEntry->keyInTree();
                    QString strCalculatedKeyInTree  = pModelTreeEntry->getCalculatedKeyInTree();
                    QString strCalculatedKeyInModel = pModelTreeEntry->getCalculatedKeyInModel();

                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        if( strCalculatedKeyInModel != strCalculatedKeyInTree || strCalculatedKeyInModel != strKeyInTree )
                        {
                            varData = "!" + pModelTreeEntry->name();
                        }
                        else
                        {
                            varData = pModelTreeEntry->name();
                        }
                    }
                    else if( i_iRole == Qt::ToolTipRole )
                    {
                        QString strData;
                        strData  = "KeyInTree: " + strKeyInTree;
                        strData += "\nCalculatedKeyInTree: " + strCalculatedKeyInTree;
                        strData += "\nCalculatedKeyInModel: " + strCalculatedKeyInModel;
                        strData += "\nKeyInParentBranch: " + pModelTreeEntry->keyInParentBranch();
                        strData += "\nIdxInTree: " + QString::number(pModelTreeEntry->indexInTree());
                        strData += "\nIdxInParentBranch: " + QString::number(pModelTreeEntry->indexInParentBranch());
                        strData += "\nSelected: " + bool2Str(pModelTreeEntry->isSelected()) ;
                        if( pModelBranch != nullptr )
                        {
                            strData += "\nExpanded: " + bool2Str(pModelBranch->isExpanded());
                        }
                        varData = strData;
                    }
                    else if( i_iRole == Qt::ForegroundRole)
                    {
                        if( strCalculatedKeyInModel != strCalculatedKeyInTree || strCalculatedKeyInModel != strKeyInTree )
                        {
                            varData = QColor(Qt::red);
                        }
                    }
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = CModelIdxTree::GetIcon(pModelTreeEntry->entryType());
                }
                else if( i_iRole == Qt::CheckStateRole )
                {
                    //bool bIsSelected = pModelTreeEntry->isSelected();
                    //Qt::CheckState checkState = bIsSelected ? Qt::Checked : Qt::Unchecked;
                    //varData = checkState;
                }
                break;
            }
            case EColumnInternalId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = i_modelIdx.internalId();
                }
                break;
            }
            case EColumnTreeEntryType:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->entryType2Str();
                }
                break;
            }
            case EColumnIdxInTree:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->indexInTree();
                }
                break;
            }
            case EColumnIdxInParentBranch:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->indexInParentBranch();
                }
                break;
            }
            case EColumnKeyInTree:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->keyInTree();
                }
                break;
            }
            case EColumnKeyInParentBranch:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->keyInParentBranch();
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pModelTreeEntry != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(varData.toString());
    }

    return varData;

} // data

//------------------------------------------------------------------------------
bool CModelIdxTreeBranchContent::setData(
    const QModelIndex& i_modelIdx,
    const QVariant&    i_varData,
    int                i_iRole )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setData",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = false;

    CModelAbstractTreeEntry* pModelTreeEntry = nullptr;

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    if( i_modelIdx.isValid() && m_pModelBranch != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelBranch->count() )
        {
            pModelTreeEntry = m_pModelBranch->at(i_modelIdx.row());
        }
    }

    if( pModelTreeEntry != nullptr )
    {
        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::EditRole )
                {
                    QString strKeyInTree = pModelTreeEntry->keyInTree();
                    QString strNameNew = i_varData.toString();

                    // "canSetData" should have been called before to verify the the
                    // name can be changed.
                    if( pModelTreeEntry->name().compare(strNameNew, Qt::CaseInsensitive) != 0 )
                    {
                        try
                        {
                            m_pIdxTree->rename(strKeyInTree, strNameNew);
                            bOk = true;
                        }
                        catch( CException* )
                        {
                        }
                    } // if( pModelTreeEntry->name().compare(strNameNew, Qt::CaseInsensitive) != 0 )
                } // if( i_iRole == Qt::EditRole )
                else if( i_iRole == Qt::CheckStateRole )
                {
                    //Qt::CheckState checkState = static_cast<Qt::CheckState>(i_varData.toInt());
                    //bool bIsSelected = (checkState == Qt::Checked);
                    //setIsSelected(pModelTreeEntry, bIsSelected, true);
                    //bOk = true;
                }
                break;
            }
            case EColumnInternalId:
            {
                break;
            }
            case EColumnTreeEntryType:
            {
                break;
            }
            case EColumnIdxInTree:
            {
                break;
            }
            case EColumnIdxInParentBranch:
            {
                break;
            }
            case EColumnKeyInTree:
            {
                break;
            }
            case EColumnKeyInParentBranch:
            {
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pModelTreeEntry != nullptr )

    if( bOk )
    {
        // Please note that if the name has been changed there is no need to emit the
        // dataChanged signal here. The index tree will change the name of the node and
        // will emit the signals in the following order:
        // - treeEntryAboutToBeRenamed
        // - treeEntryKeyInTreeChanged
        // - treeEntryRenamed
        // This model is connected to the "treeEntryKeyInTreeChanged" signal
        // and emits the dataChanged signal for the changed tree node.
        // Please note that the index tree may be changed by anyone else
        // and not just be this model so this is the way it has to be.
        // But emitting the signal in case any other attribute has been changed
        // (e.g. the selection state) the data changed signal must be emitted.
        // So we do it here for all attributes.
        emit dataChanged(i_modelIdx, i_modelIdx);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(bool2Str(bOk));
    }

    return bOk;

} // setData

/*==============================================================================
public: // instance methods for editing data
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CModelIdxTreeBranchContent::canSetData(
    const QModelIndex& i_modelIdx,
    const QVariant&    i_varData,
    int                i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canSetData",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "setData";

    SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultInvalidMethodCall, "Value cannot be changed");

    CModelAbstractTreeEntry* pModelTreeEntry = nullptr;

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    if( i_modelIdx.isValid() && m_pModelBranch != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelBranch->count() )
        {
            pModelTreeEntry = m_pModelBranch->at(i_modelIdx.row());
        }
    }

    if( pModelTreeEntry != nullptr )
    {
        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::EditRole )
                {
                    QString strKeyInTree = pModelTreeEntry->keyInTree();
                    QString strNameNew = i_varData.toString();
                    if( pModelTreeEntry->name().compare(strNameNew, Qt::CaseInsensitive) == 0 )
                    {
                        // No problem to set data if name has not been changed.
                        errResultInfo = ErrResultSuccess;
                    }
                    else // if( pModelTreeEntry->name().compare(strNameNew, Qt::CaseInsensitive) != 0 )
                    {
                        errResultInfo = m_pIdxTree->canRename(strKeyInTree, strNameNew);
                    }
                }
                break;
            }
            case EColumnInternalId:
            {
                break;
            }
            case EColumnTreeEntryType:
            {
                break;
            }
            case EColumnIdxInTree:
            {
                break;
            }
            case EColumnIdxInParentBranch:
            {
                break;
            }
            case EColumnKeyInTree:
            {
                break;
            }
            case EColumnKeyInParentBranch:
            {
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pModelTreeEntry != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iAddTrcInfoDetails = m_iTrcDetailLevel >= ETraceDetailLevelInternalStates ? 1 : 0;
        mthTracer.setMethodReturn(errResultInfo.toString(iAddTrcInfoDetails));
    }

    return errResultInfo;

} // canSetData
