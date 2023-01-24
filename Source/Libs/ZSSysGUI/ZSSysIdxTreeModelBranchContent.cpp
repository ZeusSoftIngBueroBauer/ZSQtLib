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

#include "ZSSysGUI/ZSSysIdxTreeModelBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"
#ifdef ZS_TRACE_GUI_MODELS
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#endif

#include <QtCore/qset.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qfontmetrics.h>
#include <QtQml/qqmlapplicationengine.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/******************************************************************************/
class CInitModuleSysIdxTreeModelBranchContent
/* Please note:
   The class name should be unique for the whole system. Otherwise the compiler
   may be confused and using a CInitModule class from other modules to create
   the static InitModule instance.
*******************************************************************************/
{
public: // ctor
    CInitModuleSysIdxTreeModelBranchContent()
    {
        qmlRegisterType<CModelIdxTreeBranchContent>("ZSSysGUI", 1, 0, "ModelIdxTreeBranchContent");
    }
};

static CInitModuleSysIdxTreeModelBranchContent s_initModule;


/*******************************************************************************
class CModelIdxTreeBranchContent : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

QHash<int, QByteArray> CModelIdxTreeBranchContent::s_roleNames;
QHash<QByteArray, int> CModelIdxTreeBranchContent::s_roleValues;

QHash<int, QByteArray> CModelIdxTreeBranchContent::s_clm2Name {
    /* 0 */ { CModelIdxTreeBranchContent::EColumnTreeEntryName, "TreeEntryName"},
    /* 1 */ { CModelIdxTreeBranchContent::EColumnTreeEntryType, "TreeEntryType"},
    /* 2 */ { CModelIdxTreeBranchContent::EColumnInternalId, "InternalId"},
    /* 3 */ { CModelIdxTreeBranchContent::EColumnIdxInTree, "IdxInTree"},
    /* 4 */ { CModelIdxTreeBranchContent::EColumnIdxInParentBranch, "IdxInParentBranch"},
    /* 5 */ { CModelIdxTreeBranchContent::EColumnKeyInTree, "KeyInTree"},
    /* 6 */ { CModelIdxTreeBranchContent::EColumnKeyInParentBranch, "KeyInParentBranch"}
};

//------------------------------------------------------------------------------
QString CModelIdxTreeBranchContent::column2Str(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return s_clm2Name.value(i_clm, "? (" + QByteArray::number(i_clm) + ")");
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeBranchContent::EColumn CModelIdxTreeBranchContent::role2Column(int i_iRole)
//------------------------------------------------------------------------------
{
    if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole)
        && i_iRole < (static_cast<int>(ERole::FirstDataColumnRole) + EColumnCount))
    {
        return static_cast<EColumn>(i_iRole - static_cast<int>(ERole::FirstDataColumnRole));
    }
    return EColumnUndefined;
}

//------------------------------------------------------------------------------
QString CModelIdxTreeBranchContent::role2Str(int i_iRole)
//------------------------------------------------------------------------------
{
    return s_roleNames.value(i_iRole, "? (" + QByteArray::number(i_iRole) + ")");
}

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::byteArr2Role(const QByteArray& i_byteArrRole)
//------------------------------------------------------------------------------
{
    return s_roleValues.value(i_byteArrRole, Qt::DisplayRole);
}

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::column2Role(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return static_cast<int>(ERole::FirstDataColumnRole) + i_clm;
}

//------------------------------------------------------------------------------
QString CModelIdxTreeBranchContent::modelIdx2Str(
    const QModelIndex& i_modelIdx,
    int i_iRole,
    bool i_bIncludeId )
//------------------------------------------------------------------------------
{
    QString str;
    if( !i_modelIdx.isValid() ) {
        str = "Invalid";
    } else {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        str  = "Row: " + QString::number(i_modelIdx.row());
        if( i_iRole >= static_cast<int>(CModelIdxTreeBranchContent::ERole::FirstDataColumnRole) ) {
            str += ", Clm: " + QString::number(i_modelIdx.column());
        } else if ((i_modelIdx.column() >= 0) && (i_modelIdx.column() < EColumnCount)) {
            str += ", Clm: " + column2Str(static_cast<EColumn>(i_modelIdx.column()));
        } else {
            str += ", Clm: " + QString::number(i_modelIdx.column());
        }
        str += ", Entry: " + QString(pModelTreeEntry == nullptr ? "nullptr" : pModelTreeEntry->keyInTree());
        if( i_bIncludeId ) {
            str += ", Id: " + QString::number(i_modelIdx.internalId()) + " (" + pointer2Str(i_modelIdx.internalPointer()) + ")";
        }
    }
    return str;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeBranchContent::CModelIdxTreeBranchContent( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelIdxTreeBranchContent(nullptr, i_pObjParent)
{
} // ctor

//------------------------------------------------------------------------------
CModelIdxTreeBranchContent::CModelIdxTreeBranchContent(
    CIdxTree* i_pIdxTree,
    QObject*  i_pObjParent,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pIdxTree(nullptr),
    m_sortOrder(EIdxTreeSortOrder::Config),
    m_mappModelTreeEntries(),
    m_strKeyInTreeOfRootEntry(),
    m_pModelRootEntry(nullptr),
    m_ariClmWidths(QVector<int>(EColumnCount))
    #ifdef ZS_TRACE_GUI_MODELS
    ,m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
    #endif
{
    fillRoleNames();

    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    #ifdef ZS_TRACE_GUI_MODELS
    // If the tree's parent is the trace server the detail level of trace outputs
    // may not be controlled by trace admin objects as the belong to the index tree
    // of the trace server.
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::NoisyMethods", objectName());
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }

} // ctor

//------------------------------------------------------------------------------
CModelIdxTreeBranchContent::~CModelIdxTreeBranchContent()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_pTrcAdminObj != nullptr ) {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }
    if( m_pTrcAdminObjNoisyMethods != nullptr ) {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    }
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;
    #endif

    m_pIdxTree = nullptr;
    m_sortOrder = static_cast<EIdxTreeSortOrder>(0);
    m_mappModelTreeEntries.clear();
    //m_strKeyInTreeOfRootEntry;
    m_pModelRootEntry = nullptr;
    m_ariClmWidths.clear();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the index tree whose content should be exposed by the model to views.

    @param i_pIdxTree
        Pointer to index tree to be used by model.
        The argument is of type QObject so that it can also be invoked by QML.
*/
void CModelIdxTreeBranchContent::setIdxTree( QObject* i_pIdxTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        if( m_pIdxTree == nullptr ) {
            strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        } else {
            strMthInArgs  = "IdxTreeOrig: " + QString(m_pIdxTree == nullptr ? "nullptr" : m_pIdxTree->objectName());
            strMthInArgs += ", IdxTreeNew: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        }
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setIdxTree",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QString strObjNameOrig = objectName();
    QString strObjNameNew = QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName());

    if( m_pIdxTree != i_pIdxTree )
    {
        if( m_pIdxTree != nullptr )
        {
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
                this, &CModelIdxTreeBranchContent::onIdxTreeAboutToBeDestroyed);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryAdded,
                this, &CModelIdxTreeBranchContent::onIdxTreeEntryAdded);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryChanged,
                this, &CModelIdxTreeBranchContent::onIdxTreeEntryChanged);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
                this, &CModelIdxTreeBranchContent::onIdxTreeEntryAboutToBeRemoved);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryMoved,
                this, &CModelIdxTreeBranchContent::onIdxTreeEntryMoved);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryKeyInTreeChanged,
                this, &CModelIdxTreeBranchContent::onIdxTreeEntryKeyInTreeChanged);
        }

        m_pIdxTree = dynamic_cast<CIdxTree*>(i_pIdxTree);

        if( m_pIdxTree == nullptr )
        {
            setObjectName("IdxTree");
        }
        else
        {
            setObjectName(m_pIdxTree->objectName());

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
        }

        setKeyInTreeOfRootEntry("");

        emit idxTreeChanged(m_pIdxTree);

    } // if( m_pIdxTree != i_pIdxTree )

    if( strObjNameOrig.compare(strObjNameNew) != 0 )
    {
        setObjectName(strObjNameNew);

        #ifdef ZS_TRACE_GUI_MODELS
        mthTracer.onAdminObjAboutToBeReleased();
        if( m_pTrcAdminObj != nullptr ) {
            m_pTrcAdminObj = CTrcServer::RenameTraceAdminObj(m_pTrcAdminObj, strObjNameNew);
        }
        if( m_pTrcAdminObjNoisyMethods != nullptr ) {
            m_pTrcAdminObjNoisyMethods = CTrcServer::RenameTraceAdminObj(m_pTrcAdminObjNoisyMethods, strObjNameNew);
        }
        #endif
    }
} // setIdxTree

//------------------------------------------------------------------------------
/*! Returns the pointer to the index tree (which might be null).

    @param Pointer to index tree. The type is of QObject so that it can also
           be accessed by QML.

    @note If you access the index tree and its entries and the index tree is
          modified by different threads you must lock and unlock the index tree.
*/
QObject* CModelIdxTreeBranchContent::idxTree()
//------------------------------------------------------------------------------
{
    return m_pIdxTree;
}

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

            clear();
            delete m_pModelRootEntry;
            m_pModelRootEntry = nullptr;

            if( iRowCountPrev > 0 )
            {
                endRemoveRows();
            }
        }

        // The key of the root entry must be set before trying to add entries
        // from the index tree to the model as only those entries will be added
        // whose parent node have this key.
        m_strKeyInTreeOfRootEntry = i_strKeyInTree;

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
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_sortOrder != i_sortOrder )
    {
        m_sortOrder = i_sortOrder;

        if( m_pModelRootEntry != nullptr && m_pModelRootEntry->count() > 1 )
        {
            QMap<QString, CModelIdxTreeEntry*> mappTreeEntries;

            beginRemoveRows(QModelIndex(), 0, m_pModelRootEntry->count()-1);

            for( int idxEntryChild = m_pModelRootEntry->count()-1; idxEntryChild >= 0; --idxEntryChild )
            {
                CModelIdxTreeEntry* pModelTreeEntryChild = m_pModelRootEntry->at(idxEntryChild);
                mappTreeEntries.insert(pModelTreeEntryChild->keyInParentBranch(), pModelTreeEntryChild);
                m_pModelRootEntry->remove(pModelTreeEntryChild);
            }
            endRemoveRows();

            beginInsertRows(QModelIndex(), 0, mappTreeEntries.size()-1);

            m_pModelRootEntry->setSortOrder(i_sortOrder);

            if( i_sortOrder == EIdxTreeSortOrder::Ascending )
            {
                for( auto strKeyInParentBranch : mappTreeEntries.keys() )
                {
                    CModelIdxTreeEntry* pModelTreeEntryChild = mappTreeEntries.value(strKeyInParentBranch);
                    m_pModelRootEntry->add(pModelTreeEntryChild);
                }
            }
            else if( i_sortOrder == EIdxTreeSortOrder::Descending )
            {
                QMapIterator<QString, CModelIdxTreeEntry*> it(mappTreeEntries);
                it.toBack();
                while(it.hasPrevious())
                {
                    it.previous();
                    CModelIdxTreeEntry* pModelTreeEntryChild = mappTreeEntries.value(it.key());
                    m_pModelRootEntry->add(pModelTreeEntryChild);
                }
            }
            else
            {
                CIdxTreeEntry* pBranch = m_pModelRootEntry->getIdxTreeEntry();
                for( int idxEntryChild = 0; idxEntryChild < pBranch->count(); ++idxEntryChild )
                {
                    CIdxTreeEntry* pTreeEntryChild = pBranch->at(idxEntryChild);
                    if( mappTreeEntries.contains(pTreeEntryChild->keyInParentBranch()) )
                    {
                        CModelIdxTreeEntry* pModelTreeEntryChild = mappTreeEntries.value(pTreeEntryChild->keyInParentBranch());
                        m_pModelRootEntry->add(pModelTreeEntryChild);
                    }
                }
            }
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
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strSortOrder;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onIdxTreeEntryAdded",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_pModelRootEntry != nullptr )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(i_strKeyInTree);
        CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strKeyInTree);

        if( pTreeEntry != nullptr && pModelTreeEntry == nullptr )
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
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
    const QString& i_strKeyInTree, int i_idxInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Key: " + i_strKeyInTree;
        strMthInArgs += ", Idx: " + QString::number(i_idxInTree);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onIdxTreeEntryAboutToBeRemoved",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_pModelRootEntry != nullptr )
    {
        QString strBranchPath;
        QString strName;

        QString strEntryTypeSymbol =
            m_pIdxTree->splitPathStr(i_strKeyInTree, &strBranchPath, &strName);

        QString strKeyInBranch = strEntryTypeSymbol + ":" + strName;

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
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = ", NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKey: " + i_strOrigKeyInTree;
        strMthInArgs += ", TargetBranch: " + i_strKeyInTreeOfTargetBranch;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs  = "NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKey: " + i_strOrigKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pModelTreeEntry == nullptr ? "nullptr" : i_pModelTreeEntry->keyInTree());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        QString strKeyInTree = i_pModelTreeEntry->keyInTree();
        if( !m_mappModelTreeEntries.contains(strKeyInTree) )
        {
            throw CException(__FILE__, __LINE__, EResultObjNotInList, strKeyInTree);
        }
        m_mappModelTreeEntries.remove(strKeyInTree);
        m_pModelRootEntry->remove(i_pModelTreeEntry);
        endRemoveRows();
    }
} // remove

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::columnWidthByColumn(int i_iClm, int i_iFontPixelSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Clm: " + column2Str(static_cast<EColumn>(i_iClm));
        strMthInArgs += ", FontPixelSize: " + QString::number(i_iFontPixelSize);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "columnWidthByColumn",
        /* strMthInArgs */ strMthInArgs );

    int iClmWidth = 1;

    if( i_iClm >= 0 && i_iClm < m_ariClmWidths.count() )
    {
        //iClmWidth = m_ariClmWidths[i_iClm];

        //if( iClmWidth == 0)
        {
            QFont fnt = QGuiApplication::font();
            if( i_iFontPixelSize > 0) {
                fnt.setPixelSize(i_iFontPixelSize);
            }

            QFontMetrics fntMetrics = QFontMetrics(fnt);

            QString strClmHeader = headerData(i_iClm, Qt::Horizontal).toString();
            iClmWidth = fntMetrics.horizontalAdvance(strClmHeader);

            for( int iRowIdx = 0; iRowIdx < rowCount(); ++iRowIdx )
            {
                QString strType = "string";
                QVariant varData = data(index(iRowIdx, i_iClm), static_cast<int>(ERole::Type));
                if( varData.canConvert(QVariant::String) )
                {
                    strType = varData.toString();
                }
                if( strType == "string" || strType == "int" )
                {
                    varData = data(index(iRowIdx, i_iClm), Qt::DisplayRole);
                    if( varData.canConvert(QVariant::String) )
                    {
                        QString strCellData = varData.toString();
                        iClmWidth = qMax(iClmWidth, fntMetrics.horizontalAdvance(strCellData));
                    }
                }
                else if( strType == "imageUrl" || strType == "icon" )
                {
                    varData = data(index(iRowIdx, i_iClm), Qt::DisplayRole);
                    if( varData.canConvert(QVariant::String) )
                    {
                        QString strCellData = varData.toString();
                        QPixmap pixmap(strCellData);
                        iClmWidth = qMax(iClmWidth, pixmap.width());
                    }
                }
            }
        }
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(iClmWidth);
    }

    return iClmWidth;

} // columnWidthByColumn

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::columnWidthByRole(const QByteArray& i_byteArrRole, int i_iFontPixelSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Role: " + i_byteArrRole;
        strMthInArgs += ", FontPixelSize: " + QString::number(i_iFontPixelSize);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "columnWidthByRole",
        /* strMthInArgs */ strMthInArgs );

    int iClmWidth = 1;

    int iRole = byteArr2Role(i_byteArrRole);
    EColumn clm = role2Column(iRole);

    if(clm != EColumnUndefined)
    {
        iClmWidth = columnWidthByColumn(static_cast<int>(clm), i_iFontPixelSize);
    }
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(iClmWidth);
    }
    return iClmWidth;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QHash<int, QByteArray> CModelIdxTreeBranchContent::roleNames() const
//------------------------------------------------------------------------------
{
    return s_roleNames;
}

//------------------------------------------------------------------------------
int CModelIdxTreeBranchContent::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "rowCount",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    int iRowCount = 0;

    if( m_pModelRootEntry != nullptr )
    {
        iRowCount = m_pModelRootEntry->count();
    }

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
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
    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(QString::number(EColumnCount));
    }
    #endif

    return EColumnCount;

} // columnCount

//------------------------------------------------------------------------------
QModelIndex CModelIdxTreeBranchContent::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + QString::number(i_iCol);
        strMthInArgs += ", ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "index",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QModelIndex modelIdx = QAbstractTableModel::index(i_iRow, i_iCol, i_modelIdxParent);

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(modelIdx2Str(modelIdx));
    }
    #endif

    return modelIdx;

} // index

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
    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
            case EColumnInternalId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Internal-Id";
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
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
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
    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        if( pModelTreeEntry->isRoot() )
        {
            uFlags = uFlags | Qt::ItemIsDropEnabled;
        }
        else if( pModelTreeEntry->isBranch() )
        {
            uFlags = uFlags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
        }
        else if( pModelTreeEntry->isLeave() )
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
            case EColumnTreeEntryType:
            {
                break;
            }
            case EColumnInternalId:
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
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
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
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "supportedDropActions",
        /* strMethodInArgs    */ "" );
    #endif

    Qt::DropActions dropActions = Qt::CopyAction | Qt::MoveAction;

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(qDropActions2Str(dropActions));
    }
    #endif

    return dropActions;

} // supportedDropActions

//------------------------------------------------------------------------------
QVariant CModelIdxTreeBranchContent::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    EColumn clm = static_cast<EColumn>(i_modelIdx.column());
    int iRole = i_iRole;

    if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole) )
    {
        clm = role2Column(i_iRole);
        iRole = Qt::DisplayRole;
    }

    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs  = "ModelIdx {" + modelIdx2Str(i_modelIdx, i_iRole) + "}";
        strMthInArgs += ", Role: " + QString::number(i_iRole) + " (" + role2Str(i_iRole) + ")";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "data",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    static QSet<int> s_rolesHandled = {
        Qt::DisplayRole,
        Qt::DecorationRole,
        Qt::ToolTipRole,
        static_cast<int>(ERole::Sort),
        static_cast<int>(ERole::ImageUrl),
        static_cast<int>(ERole::Type)
    };

    QVariant varData;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() && m_pModelRootEntry != nullptr )
    {
        if( i_modelIdx.row() >= 0 && i_modelIdx.row() < m_pModelRootEntry->count() )
        {
            pModelTreeEntry = m_pModelRootEntry->at(i_modelIdx.row());
        }
    }

    if( pModelTreeEntry != nullptr && s_rolesHandled.contains(iRole) )
    {
        switch( clm )
        {
            case EColumnTreeEntryName:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::EditRole || iRole == Qt::ToolTipRole || iRole == Qt::ForegroundRole)
                {
                    QString strKeyInTree = pModelTreeEntry->keyInTree();

                    if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                    {
                        varData = pModelTreeEntry->name();
                    }
                    else if( iRole == Qt::ToolTipRole )
                    {
                        QString strData;
                        strData  = "KeyInTree: " + strKeyInTree;
                        strData += "\nKeyInParentBranch: " + pModelTreeEntry->keyInParentBranch();
                        strData += "\nIdxInTree: " + QString::number(pModelTreeEntry->indexInTree());
                        strData += "\nIdxInParentBranch: " + QString::number(pModelTreeEntry->indexInParentBranch());
                        strData += "\nSelected: " + bool2Str(pModelTreeEntry->isSelected()) ;
                        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() ) {
                            strData += "\nExpanded: " + bool2Str(pModelTreeEntry->isExpanded());
                        }
                        varData = strData;
                    }
                }
                else if( iRole == Qt::DecorationRole )
                {
                    varData = CModelIdxTree::getIcon(pModelTreeEntry->entryTypeSymbol());
                }
                else if( iRole == Qt::CheckStateRole )
                {
                    bool bIsSelected = pModelTreeEntry->isSelected();
                    Qt::CheckState checkState = bIsSelected ? Qt::Checked : Qt::Unchecked;
                    varData = checkState;
                }
                break;
            }
            case EColumnTreeEntryType:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->entryType2Str();
                }
                break;
            }
            case EColumnInternalId:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole )
                {
                    varData = QString::number(i_modelIdx.internalId()) + " (" + pointer2Str(i_modelIdx.internalPointer()) + ")";
                }
                break;
            }
            case EColumnIdxInTree:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("int");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->indexInTree();
                }
                break;
            }
            case EColumnIdxInParentBranch:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("int");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->indexInParentBranch();
                }
                break;
            }
            case EColumnKeyInTree:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->keyInTree();
                }
                break;
            }
            case EColumnKeyInParentBranch:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("string");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->keyInParentBranch();
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( clm )
    } // if( pModelTreeEntry != nullptr && s_rolesHandled.contains(iRole) )

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
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
    EColumn clm = static_cast<EColumn>(i_modelIdx.column());
    int iRole = i_iRole;

    if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole) )
    {
        clm = role2Column(i_iRole);
        iRole = Qt::DisplayRole;
    }

    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs  = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + role2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

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
        switch( clm )
        {
            case EColumnTreeEntryName:
            {
                if( iRole == Qt::EditRole )
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
                }
                else if( iRole == Qt::CheckStateRole )
                {
                    //Qt::CheckState checkState = static_cast<Qt::CheckState>(i_varData.toInt());
                    //bool bIsSelected = (checkState == Qt::Checked);
                    //setIsSelected(pModelTreeEntry, bIsSelected, true);
                    //bOk = true;
                }
                break;
            }
            case EColumnTreeEntryType:
            {
                break;
            }
            case EColumnInternalId:
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
        } // switch( clm )
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
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
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
    EColumn clm = static_cast<EColumn>(i_modelIdx.column());
    int iRole = i_iRole;

    if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole) )
    {
        clm = role2Column(i_iRole);
        iRole = Qt::DisplayRole;
    }

    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs  = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + role2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "canSetData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

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
        switch( clm )
        {
            case EColumnTreeEntryName:
            {
                if( iRole == Qt::EditRole )
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
            case EColumnTreeEntryType:
            {
                break;
            }
            case EColumnInternalId:
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
        } // switch( clm )
    } // if( pModelTreeEntry != nullptr )

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(errResultInfo.toString());
    }
    #endif

    return errResultInfo;

} // canSetData

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreeBranchContent::fillRoleNames()
//------------------------------------------------------------------------------
{
    if( s_roleNames.isEmpty() )
    {
        s_roleNames = QAbstractItemModel::roleNames();

        s_roleNames[static_cast<int>(ERole::Sort)] = "sort";
        s_roleNames[static_cast<int>(ERole::ImageUrl)] = "imageUrl";
        s_roleNames[static_cast<int>(ERole::Type)] = "type";

        for( int clm = 0; clm < EColumnCount; ++clm)
        {
            int role = column2Role(static_cast<EColumn>(clm));
            s_roleNames[role] = s_clm2Name[clm];
        }

        for( int iRole : s_roleNames.keys() )
        {
            const QByteArray& byteArrRole = s_roleNames.value(iRole);
            s_roleValues[byteArrRole] = iRole;
        }
    }
}
