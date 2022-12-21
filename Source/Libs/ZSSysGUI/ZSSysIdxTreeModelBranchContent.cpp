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

#include "ZSSysGUI/ZSSysIdxTreeModelBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;

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
    EMethodTraceDetailLevel i_eTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pIdxTree(i_pIdxTree),
    m_sortOrder(EIdxTreeSortOrder::Config),
    m_mappModelTreeEntries(),
    m_strKeyInTreeOfRootEntry(),
    m_pModelRootEntry(nullptr),
    m_eTrcDetailLevel(i_eTrcDetailLevel)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QObject::connect(
        m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
        this, &CModelIdxTreeBranchContent::onIdxTreeAboutToBeDestroyed);
    QObject::connect(
        m_pIdxTree, &CIdxTree::treeEntryAdded,
        this, &CModelIdxTreeBranchContent::onIdxTreeEntryAdded);
    QObject::connect(
        m_pIdxTree, &CIdxTree::treeEntryChanged,
        this, &CModelIdxTreeBranchContent::onIdxTreeEntryChanged);
    QObject::connect(
        m_pIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
        this, &CModelIdxTreeBranchContent::onIdxTreeEntryAboutToBeRemoved);
    QObject::connect(
        m_pIdxTree, &CIdxTree::treeEntryMoved,
        this, &CModelIdxTreeBranchContent::onIdxTreeEntryMoved);
    QObject::connect(
        m_pIdxTree, &CIdxTree::treeEntryKeyInTreeChanged,
        this, &CModelIdxTreeBranchContent::onIdxTreeEntryKeyInTreeChanged);

} // ctor

//------------------------------------------------------------------------------
CModelIdxTreeBranchContent::~CModelIdxTreeBranchContent()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );
    #endif

    if( m_pModelRootEntry != nullptr )
    {
        clear();

        try
        {
            delete m_pModelRootEntry;
        }
        catch(...)
        {
        }
    }

    m_pIdxTree = nullptr;
    m_sortOrder = static_cast<EIdxTreeSortOrder>(0);
    m_mappModelTreeEntries.clear();
    //m_strKeyInTreeOfRootEntry;
    m_pModelRootEntry = nullptr;
    m_eTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setKeyInTreeOfRootEntry",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_strKeyInTreeOfRootEntry.compare(i_strKeyInTree) != 0 )
    {
        if( m_pModelRootEntry != nullptr )
        {
            int iRowCountPrev = rowCount();

            if( iRowCountPrev > 0 )
            {
                beginRemoveRows(QModelIndex(), 0, iRowCountPrev-1);
            }

            delete m_pModelRootEntry;
            m_pModelRootEntry = nullptr;

            if( iRowCountPrev > 0 )
            {
                endRemoveRows();
            }
        }

        if( m_pIdxTree != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);

            CIdxTreeEntry* pTreeEntryBranch = m_pIdxTree->findEntry(i_strKeyInTree);

            if( pTreeEntryBranch != nullptr && (pTreeEntryBranch->isRoot() || pTreeEntryBranch->isBranch()) )
            {
                m_pModelRootEntry = new CModelIdxTreeEntry(pTreeEntryBranch);
                m_pModelRootEntry->setSortOrder(m_sortOrder);

                for( int idxEntry = 0; idxEntry < pTreeEntryBranch->count(); ++idxEntry )
                {
                    CIdxTreeEntry* pTreeEntryChild = pTreeEntryBranch->at(idxEntry);
                    onIdxTreeEntryAdded(pTreeEntryChild->keyInTree());
                }
            }
        }

        m_strKeyInTreeOfRootEntry = i_strKeyInTree;

        emit keyInTreeOfRootEntryChanged(m_strKeyInTreeOfRootEntry);
    }
} // setKeyInTreeOfRootEntry

//------------------------------------------------------------------------------
QString CModelIdxTreeBranchContent::getKeyInTreeOfRootEntry() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTreeOfRootEntry;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_sortOrder != i_sortOrder )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        m_sortOrder = i_sortOrder;

        if( m_pModelRootEntry != nullptr && m_pModelRootEntry->count() > 1 )
        {
            beginRemoveRows(QModelIndex(), 0, m_pModelRootEntry->count()-1);
            endRemoveRows();

            beginInsertRows(QModelIndex(), 0, m_pModelRootEntry->count()-1);
            m_pModelRootEntry->setSortOrder(i_sortOrder);
            endInsertRows();
        }

        m_sortOrder = i_sortOrder;

        emit sortOrderChanged(m_sortOrder);
        emit sortOrderChanged(idxTreeSortOrder2Str(m_sortOrder));
    }

} // setSortOrder

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::setSortOrder( const QString& i_strSortOrder )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = i_strSortOrder;
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    EIdxTreeSortOrder sortOrder = str2IdxTreeSortOrder(i_strSortOrder);
    if( sortOrder != EIdxTreeSortOrder::Undefined && m_sortOrder != sortOrder )
    {
        setSortOrder(sortOrder);
    }
}

//------------------------------------------------------------------------------
EIdxTreeSortOrder CModelIdxTreeBranchContent::sortOrder() const
//------------------------------------------------------------------------------
{
    return m_sortOrder;
}

//------------------------------------------------------------------------------
QString CModelIdxTreeBranchContent::sortOrderAsString() const
//------------------------------------------------------------------------------
{
    return idxTreeSortOrder2Str(m_sortOrder);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTreeBranchContent::findEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    return m_mappModelTreeEntries.value(i_strKeyInTree, nullptr);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QModelIndex CModelIdxTreeBranchContent::index( const QString& i_strKeyInTree, int i_iClm ) const
//------------------------------------------------------------------------------
{
    CModelIdxTreeEntry* pModelTreeEntry = m_mappModelTreeEntries.value(i_strKeyInTree, nullptr);

    int iRow = -1;
    int iClm = i_iClm >= 0 ? i_iClm : 0;

    if( pModelTreeEntry != nullptr )
    {
        iRow = pModelTreeEntry->indexInParentBranch();
    }

    QModelIndex modelIdx = createIndex(iRow, iClm, pModelTreeEntry);

    return modelIdx;

} // index

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeAboutToBeDestroyed",
        /* strMethodInArgs    */ "" );
    #endif

    m_pIdxTree = nullptr;

    clear();

    delete m_pModelRootEntry;
    m_pModelRootEntry = nullptr;

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
void CModelIdxTreeBranchContent::onIdxTreeEntryAdded( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryAdded",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_pModelRootEntry != nullptr )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(i_strKeyInTree);

        if( pTreeEntry != nullptr )
        {
            QString strKeyInTreeOfParentBranch = pTreeEntry->parentBranchKeyInTree();

            if( m_strKeyInTreeOfRootEntry.compare(strKeyInTreeOfParentBranch) == 0 )
            {
                CModelIdxTreeEntry* pModelTreeEntry = new CModelIdxTreeEntry(pTreeEntry);
                int idxInParentBranch = m_pModelRootEntry->add(pModelTreeEntry);
                m_mappModelTreeEntries.insert(i_strKeyInTree, pModelTreeEntry);
                beginInsertRows(QModelIndex(), idxInParentBranch, idxInParentBranch);
                endInsertRows();
            }
        }
    }
} // onIdxTreeEntryAdded

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryChanged( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_pModelRootEntry != nullptr )
    {
        CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strKeyInTree);

        if( pModelTreeEntry != nullptr )
        {
            QModelIndex modelIdxTL = index(i_strKeyInTree, 0);
            QModelIndex modelIdxBR = index(i_strKeyInTree, columnCount()-1);
            emit dataChanged(modelIdxTL, modelIdxBR);
        }
    }
} // onIdxTreeEntryChanged

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryAboutToBeRemoved(
    EIdxTreeEntryType i_entryType,
    const QString&    i_strKeyInTree,
    int               i_idxInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = "Type: " + idxTreeEntryType2Str(i_entryType);
        strMthInArgs += ", Key: " + i_strKeyInTree;
        strMthInArgs += ", Idx: " + QString::number(i_idxInTree);
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryAboutToBeRemoved",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_pModelRootEntry != nullptr )
    {
        QString strBranchPath;
        QString strName;

        m_pIdxTree->splitPathStr(i_strKeyInTree, &strBranchPath, &strName);

        QString strKeyInBranch = idxTreeEntryType2Str(EEnumEntryAliasStrSymbol) + ":" + strName;

        // If the root entry is going to be destroyed ...
        if( m_strKeyInTreeOfRootEntry.compare(strKeyInBranch) == 0 )
        {
            clear();

            delete m_pModelRootEntry;
            m_pModelRootEntry = nullptr;
        }
        // If it is not the root entry which is going to be destroyed ...
        else
        {
            CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strKeyInTree);
            if( pModelTreeEntry != nullptr )
            {
                remove(pModelTreeEntry);
                delete pModelTreeEntry;
                pModelTreeEntry = nullptr;
            }
        }
    }
} // onIdxTreeEntryAboutToBeRemoved

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryMoved(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree,
    const QString& i_strKeyInTreeOfTargetBranch )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = ", NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKey: " + i_strOrigKeyInTree;
        strMthInArgs += ", TargetBranch: " + i_strKeyInTreeOfTargetBranch;
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryMoved",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_pModelRootEntry != nullptr )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strOrigKeyInTree);

        // If the entry belonged to the branch but has been moved to another branch ..
        if( pModelTreeEntry != nullptr && m_strKeyInTreeOfRootEntry.compare(i_strKeyInTreeOfTargetBranch) != 0 )
        {
        }
        // If the entry did not belong to the branch but has been moved to this branch ..
        else if( pModelTreeEntry == nullptr && m_strKeyInTreeOfRootEntry.compare(i_strKeyInTreeOfTargetBranch) == 0 )
        {
        }
        // If the entry belonged to the branch and has been moved within this branch ..
        else if( pModelTreeEntry != nullptr && m_strKeyInTreeOfRootEntry.compare(i_strKeyInTreeOfTargetBranch) == 0 )
        {
        }

        #if 0
        // Please note that before the index tree emits the "treeEntryMoved" signal
        // and this slot is executed the "treeEntryKeyInTreeChanged" signal has been
        // emitted and the slot "onIdxTreeEntryKeyInTreeChanged" was processed.
        // Which means that the "KeyInTreePrev" has already been updated and is no
        // longer part of the models map. But the model node can be found using
        // the new key in tree.
        CModelIdxTreeEntry* pModelTreeEntry    = findEntry(i_pTreeEntry);
        CModelIdxTreeEntry* pModelTargetBranch = findEntry(i_pTargetBranch);

        if( pModelTreeEntry != nullptr && pModelTargetBranch != nullptr )
        {
            CModelIdxTreeEntry* pModelParentBranch = pModelTreeEntry->modelParentBranch();

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
        #endif
    } // if( m_pModelRootEntry != nullptr )
} // onIdxTreeEntryMoved

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::onIdxTreeEntryKeyInTreeChanged(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs  = "NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKey: " + i_strOrigKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryKeyInTreeChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    #if 0
    CModelIdxTreeEntry* pModelTreeEntry = m_mappModelTreeEntries.value(i_strKeyInTreePrev, nullptr);
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
void CModelIdxTreeBranchContent::clear()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "clear",
        /* strMethodInArgs    */ "" );
    #endif

    for( int idxEntry = m_pModelRootEntry->count()-1; idxEntry >= 0; --idxEntry )
    {
        CModelIdxTreeEntry* pModelTreeEntry = m_pModelRootEntry->at(idxEntry);
        remove(pModelTreeEntry);
        delete pModelTreeEntry;
        pModelTreeEntry = nullptr;
    }
} // clear

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::remove( CModelIdxTreeEntry* i_pModelTreeEntry )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = "TreeEntry: " + QString(i_pModelTreeEntry == nullptr ? "nullptr" : i_pModelTreeEntry->keyInTree());
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "remove",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( i_pModelTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModelTreeEntry == nullptr");
    }

    int idxInParentBranch = i_pModelTreeEntry->indexInParentBranch();

    if( m_pModelRootEntry != nullptr && idxInParentBranch >= 0 )
    {
        beginRemoveRows(QModelIndex(), idxInParentBranch, idxInParentBranch);
        m_mappModelTreeEntries.remove(i_pModelTreeEntry->keyInTree());
        m_pModelRootEntry->remove(i_pModelTreeEntry);
        endRemoveRows();
    }
} // remove

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = "ModelIdxParent {" + CModelIdxTree::modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rowCount",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    int iRowCount = 0;

    if( m_pModelRootEntry != nullptr )
    {
        iRowCount = m_pModelRootEntry->count();
    }

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        mthTracer.setMethodReturn(QString::number(iRowCount));
    }
    #endif

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = "ModelIdxParent {" + CModelIdxTree::modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        mthTracer.setMethodReturn(QString::number(EColumnCount));
    }
    #endif

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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "headerData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // headerData

//------------------------------------------------------------------------------
Qt::ItemFlags CModelIdxTreeBranchContent::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "flags",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    Qt::ItemFlags uFlags = QAbstractItemModel::flags(i_modelIdx);

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() && m_pModelRootEntry != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelRootEntry->count() )
        {
            pModelTreeEntry = m_pModelRootEntry->at(i_modelIdx.row());
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
                if( pModelTreeEntry != m_pModelRootEntry )
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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }
    #endif

    return uFlags;

} // flags

//------------------------------------------------------------------------------
Qt::DropActions CModelIdxTreeBranchContent::supportedDropActions() const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "supportedDropActions",
        /* strMethodInArgs    */ "" );
    #endif

    Qt::DropActions dropActions = Qt::CopyAction | Qt::MoveAction;

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        mthTracer.setMethodReturn(qDropActions2Str(dropActions));
    }
    #endif

    return dropActions;

} // supportedDropActions

//------------------------------------------------------------------------------
QVariant CModelIdxTreeBranchContent::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs  = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx, i_iRole) + "}";
        strMthInArgs += ", Role: " + CModelIdxTree::role2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "data",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QVariant varData;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() && m_pModelRootEntry != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelRootEntry->count() )
        {
            pModelTreeEntry = m_pModelRootEntry->at(i_modelIdx.row());
        }
    }

    if( pModelTreeEntry != nullptr )
    {
        CModelIdxTreeEntry* pModelBranch = nullptr;
        //CModelLeaveTreeEntry*  pModelLeave  = nullptr;

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            pModelBranch = pModelTreeEntry;
        }
        else if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            //pModelLeave = pModelTreeEntry;
        }

        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole || i_iRole == Qt::ToolTipRole || i_iRole == Qt::ForegroundRole)
                {
                    QString strKeyInTree = pModelTreeEntry->keyInTree();

                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        varData = pModelTreeEntry->name();
                    }
                    else if( i_iRole == Qt::ToolTipRole )
                    {
                        QString strData;
                        strData  = "KeyInTree: " + strKeyInTree;
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
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = CModelIdxTree::getIcon(pModelTreeEntry->entryType());
                }
                else if( i_iRole == Qt::CheckStateRole )
                {
                    bool bIsSelected = pModelTreeEntry->isSelected();
                    Qt::CheckState checkState = bIsSelected ? Qt::Checked : Qt::Unchecked;
                    varData = checkState;
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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // data

//------------------------------------------------------------------------------
bool CModelIdxTreeBranchContent::setData(
    const QModelIndex& i_modelIdx,
    const QVariant&    i_varData,
    int                i_iRole )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs  = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + CModelIdxTree::role2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    bool bOk = false;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() && m_pModelRootEntry != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelRootEntry->count() )
        {
            pModelTreeEntry = m_pModelRootEntry->at(i_modelIdx.row());
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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        mthTracer.setMethodReturn(bool2Str(bOk));
    }
    #endif

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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        strMthInArgs  = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + CModelIdxTree::role2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canSetData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QString strMth = "setData";

    SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultInvalidMethodCall, "Value cannot be changed");

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() && m_pModelRootEntry != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelRootEntry->count() )
        {
            pModelTreeEntry = m_pModelRootEntry->at(i_modelIdx.row());
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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        int iAddTrcInfoDetails = m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ? 1 : 0;
        mthTracer.setMethodReturn(errResultInfo.toString(iAddTrcInfoDetails));
    }
    #endif

    return errResultInfo;

} // canSetData
