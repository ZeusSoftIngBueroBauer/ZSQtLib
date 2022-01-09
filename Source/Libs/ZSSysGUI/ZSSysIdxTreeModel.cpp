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

#include <QtCore/qmimedata.h>

#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;


/*******************************************************************************
Type definitions and constants
*******************************************************************************/

/* enum class EIdxTreeSortOrder
==============================================================================*/

static const SEnumEntry s_arEnumStrIdxTreeSortOrders[] = {                    // IdxName,             Symbol, Text
    /*  0 */ SEnumEntry( static_cast<int>(EIdxTreeSortOrder::Config),            "Config",            "-",    "As Configured"        ),
    /*  1 */ SEnumEntry( static_cast<int>(EIdxTreeSortOrder::Ascending),         "Ascending",         "A"     "Ascending"            )
    //  2 ** SEnumEntry( static_cast<int>(EIdxTreeSortOrder::LogicalDescending), "LogicalDescending", "LD",   "Descending Logically" ),
    //  3 ** SEnumEntry( static_cast<int>(EIdxTreeSortOrder::ByNameAscending),   "ByNameAscending",   "NA",   "Ascending By Name"    ),
    //  4 ** SEnumEntry( static_cast<int>(EIdxTreeSortOrder::ByNameDescending),  "ByNameDescending",  "ND",   "Descending By Name"   )
};

//------------------------------------------------------------------------------
QString ZS::System::GUI::idxTreeSortOrder2Str( EIdxTreeSortOrder i_eVal, int i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrIdxTreeSortOrders, _ZSArrLen(s_arEnumStrIdxTreeSortOrders), static_cast<int>(i_eVal), i_alias);
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::idxTreeSortOrder2Str( int i_iVal, int i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrIdxTreeSortOrders, _ZSArrLen(s_arEnumStrIdxTreeSortOrders), i_iVal, i_alias);
}

//------------------------------------------------------------------------------
QPixmap ZS::System::GUI::idxTreeSortOrder2Pixmap( EIdxTreeSortOrder i_eVal, const QSize& i_sz )
//------------------------------------------------------------------------------
{
    return idxTreeSortOrder2Pixmap(static_cast<int>(i_eVal), i_sz);
}

//------------------------------------------------------------------------------
QPixmap ZS::System::GUI::idxTreeSortOrder2Pixmap( int i_iVal, const QSize& i_sz )
//------------------------------------------------------------------------------
{
    QString str = idxTreeSortOrder2Str(i_iVal);
    QPixmap pxm = QPixmap( ":/ZS/TreeView/TreeViewSortOrder" + str + ".bmp" );
    pxm = pxm.scaled(i_sz);
    return pxm;

} // idxTreeSortOrder2Pixmap

//------------------------------------------------------------------------------
EIdxTreeSortOrder ZS::System::GUI::str2IdxTreeSortOrder( const QString& i_strVal, int i_alias )
//------------------------------------------------------------------------------
{
    EIdxTreeSortOrder sortOrder = EIdxTreeSortOrder::Undefined;

    int iVal = SEnumEntry::str2Enumerator(s_arEnumStrIdxTreeSortOrders, _ZSArrLen(s_arEnumStrIdxTreeSortOrders), i_strVal, i_alias, Qt::CaseInsensitive);

    if( iVal >= 0 && iVal < static_cast<int>(EIdxTreeSortOrder::Count) )
    {
        sortOrder = static_cast<EIdxTreeSortOrder>(iVal);
    }
    return sortOrder;
}


/*******************************************************************************
class CModelIdxTree : public QAbstractItemModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* class iterator
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTree::iterator::operator * () const
//------------------------------------------------------------------------------
{
    return m_pModelTreeEntryCurr;
}

//------------------------------------------------------------------------------
bool CModelIdxTree::iterator::operator == ( const iterator& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_pModel != i_other.m_pModel )
    {
        bEqual = false;
    }
    else if( m_pModelTreeEntryCurr != i_other.m_pModelTreeEntryCurr )
    {
        bEqual = false;
    }
    else if( m_traversalOrder != ETraversalOrder::Undefined
         && i_other.m_traversalOrder != ETraversalOrder::Undefined
         && m_traversalOrder != i_other.m_traversalOrder )
    {
        bEqual = false;
    }
    return bEqual;

} // iterator::operator ==

//------------------------------------------------------------------------------
bool CModelIdxTree::iterator::operator == ( iterator& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_pModel != i_other.m_pModel )
    {
        bEqual = false;
    }
    else if( m_pModelTreeEntryCurr != i_other.m_pModelTreeEntryCurr )
    {
        bEqual = false;
    }
    else if( m_traversalOrder != ETraversalOrder::Undefined
         && i_other.m_traversalOrder != ETraversalOrder::Undefined
         && m_traversalOrder != i_other.m_traversalOrder )
    {
        bEqual = false;
    }
    return bEqual;

} // iterator::operator ==

//------------------------------------------------------------------------------
bool CModelIdxTree::iterator::operator != ( const iterator& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

//------------------------------------------------------------------------------
bool CModelIdxTree::iterator::operator != ( iterator& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

//------------------------------------------------------------------------------
CModelIdxTree::iterator& CModelIdxTree::iterator::operator ++ ()
//------------------------------------------------------------------------------
{
    if( m_pModelTreeEntryCurr == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_pModelTreeEntryCurr == nullptr");
    }

    CModelIdxTreeEntry* pModelTreeEntryNew = nullptr;

    if( m_traversalOrder == ETraversalOrder::Index )
    {
        CIdxTreeEntry* pTreeEntry = m_pModelTreeEntryCurr->treeEntry();
        int            idxInTree  = pTreeEntry->indexInTree();

        for( ++idxInTree; idxInTree < m_pModel->idxTree()->treeEntriesVec().size(); ++idxInTree )
        {
            pTreeEntry = m_pModel->idxTree()->treeEntriesVec()[idxInTree];
            if( pTreeEntry != nullptr )
            {
                pModelTreeEntryNew = m_pModel->findModelEntry(pTreeEntry);
                break;
            }
        }
    }
    else if( m_traversalOrder == ETraversalOrder::PreOrder )
    {
        if( m_pModelTreeEntryCurr == m_pModel->m_pModelRoot )
        {
            CModelIdxTreeEntry* pModelBranchCurr = m_pModel->m_pModelRoot;

            if( pModelBranchCurr->count() > 0 )
            {
                pModelTreeEntryNew = pModelBranchCurr->at(0);
            }
        }
        else // if( m_pModelTreeEntryCurr != m_pModel->m_pModelRoot )
        {
            if( m_pModelTreeEntryCurr->entryType() == EIdxTreeEntryType::Leave )
            {
                int idxInParentBranch = m_pModelTreeEntryCurr->modelIndexInParentBranch();
                CModelIdxTreeEntry* pModelBranchParent = m_pModelTreeEntryCurr->modelParentBranch();

                if( idxInParentBranch >= (pModelBranchParent->count()-1) )
                {
                    idxInParentBranch = pModelBranchParent->modelIndexInParentBranch();
                    pModelBranchParent = pModelBranchParent->modelParentBranch();

                    while( pModelBranchParent != nullptr && idxInParentBranch >= (pModelBranchParent->count()-1) )
                    {
                        idxInParentBranch = pModelBranchParent->modelIndexInParentBranch();
                        pModelBranchParent = pModelBranchParent->modelParentBranch();
                    }
                } // if( idxInParentBranch >= (pModelBranchParent->count()-1) )

                if( pModelBranchParent != nullptr && idxInParentBranch < (pModelBranchParent->count()-1) )
                {
                    pModelTreeEntryNew = pModelBranchParent->at(idxInParentBranch+1);
                }
            }
            else // if( m_pModelTreeEntryCurr->entryType() == EIdxTreeEntryType::Branch )
            {
                CModelIdxTreeEntry* pModelBranchCurr = m_pModelTreeEntryCurr;

                if( pModelBranchCurr->count() > 0 )
                {
                    pModelTreeEntryNew = pModelBranchCurr->at(0);
                }
                else
                {
                    int idxInParentBranch = pModelBranchCurr->modelIndexInParentBranch();
                    CModelIdxTreeEntry* pModelBranchParent = pModelBranchCurr->modelParentBranch();

                    while( pModelBranchParent != nullptr && idxInParentBranch >= (pModelBranchParent->count()-1) )
                    {
                        pModelBranchCurr = pModelBranchParent;
                        idxInParentBranch = pModelBranchCurr->modelIndexInParentBranch();
                        pModelBranchParent = pModelBranchParent->modelParentBranch();
                    }

                    if( pModelBranchParent != nullptr && idxInParentBranch < (pModelBranchParent->count()-1) )
                    {
                        pModelTreeEntryNew = pModelBranchParent->at(idxInParentBranch+1);
                    }
                }
            }
        } // if( m_pModelTreeEntryCurr != m_pModel->m_pModelRoot )
    } // if( m_traversalOrder == ETraversalOrder::PreOrder )

    m_pModelTreeEntryCurr = pModelTreeEntryNew;

    return *this;

} // operator ++

/*==============================================================================
protected: // class members
==============================================================================*/

int CModelIdxTree::s_iInstCount = 0;

bool CModelIdxTree::s_bIconsCreated = false;

QPixmap* CModelIdxTree::s_pPxmRoot    = nullptr;
QPixmap* CModelIdxTree::s_pPxmBranch  = nullptr;
QPixmap* CModelIdxTree::s_pPxmLeave   = nullptr;
QIcon*   CModelIdxTree::s_pIconRoot   = nullptr;
QIcon*   CModelIdxTree::s_pIconBranch = nullptr;
QIcon*   CModelIdxTree::s_pIconLeave  = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QIcon CModelIdxTree::GetIcon( EIdxTreeEntryType i_entryType )
//------------------------------------------------------------------------------
{
    QIcon icon;

    if( !s_bIconsCreated )
    {
        s_pPxmRoot = new QPixmap(":/ZS/TreeView/TreeViewRootEntry.bmp");
        s_pPxmRoot->setMask(s_pPxmRoot->createHeuristicMask());

        s_pPxmBranch = new QPixmap(":/ZS/TreeView/TreeViewBranchEntry.bmp");
        s_pPxmBranch->setMask(s_pPxmBranch->createHeuristicMask());

        s_pPxmLeave = new QPixmap(":/ZS/TreeView/TreeViewLeaveEntry.bmp");
        s_pPxmLeave->setMask(s_pPxmLeave->createHeuristicMask());

        s_pIconRoot = new QIcon();
        s_pIconBranch = new QIcon();
        s_pIconLeave = new QIcon();

        s_pIconRoot->addPixmap(*s_pPxmRoot);
        s_pIconBranch->addPixmap(*s_pPxmBranch);
        s_pIconLeave->addPixmap(*s_pPxmLeave);

        s_bIconsCreated = true;
    }

    switch( i_entryType )
    {
        case EIdxTreeEntryType::Root:
        {
            if( s_pIconRoot != nullptr )
            {
                icon = *s_pIconRoot;
            }
            break;
        }
        case EIdxTreeEntryType::Branch:
        {
            if( s_pIconBranch != nullptr )
            {
                icon = *s_pIconBranch;
            }
            break;
        }
        case EIdxTreeEntryType::Leave:
        {
            if( s_pIconLeave != nullptr )
            {
                icon = *s_pIconLeave;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return icon;

} // GetIcon

//------------------------------------------------------------------------------
QString CModelIdxTree::ModelIdx2Str( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString str;
    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    str  = "Row: " + QString::number(i_modelIdx.row());
    str += ", Clm: " + QString::number(i_modelIdx.column());
    str += ", Id: " + QString::number(i_modelIdx.internalId()) + " (" + pointer2Str(i_modelIdx.internalPointer()) + ")";
    str += ", TreeEntry: " + QString(pModelTreeEntry == nullptr ? "nullptr" : pModelTreeEntry->keyInTree());
    return str;

} // ModelIdx2Str

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CModelIdxTree::AreIconsCreated()
//------------------------------------------------------------------------------
{
    return s_bIconsCreated;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTree::CModelIdxTree( CIdxTree* i_pIdxTree, QObject* i_pObjParent, int i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pObjParent),
    m_pIdxTree(nullptr),
    m_entryTypeFilter(EIdxTreeEntryType::Undefined),
    m_sortOrder(EIdxTreeSortOrder::Config),
    m_mappModelTreeEntries(),
    m_pModelRoot(nullptr),
    m_iTrcDetailLevel(i_iTrcDetailLevel),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    // If the tree's parent is the trace server the detail level of trace outputs
    // may not be controlled by trace admin objects as the belong to the index tree
    // of the trace server.
    if( dynamic_cast<CTrcServer*>(i_pIdxTree->parent()) == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

        if( m_pTrcAdminObj != nullptr )
        {
            m_iTrcDetailLevel = m_pTrcAdminObj->getTraceDetailLevel();

            if( !QObject::connect(
                /* pObjSender   */ m_pTrcAdminObj,
                /* szSignal     */ SIGNAL(changed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTrcAdminObjChanged(QObject*)),
                /* cnctType     */ Qt::DirectConnection ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
    }

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    s_iInstCount++;

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }

} // ctor

//------------------------------------------------------------------------------
CModelIdxTree::~CModelIdxTree()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );

    s_iInstCount--;

    if( m_pModelRoot != nullptr )
    {
        clear(m_pModelRoot);

        try
        {
            delete m_pModelRoot;
        }
        catch(...)
        {
        }
    }

    if( s_iInstCount == 0 )
    {
        delete s_pPxmRoot;
        s_pPxmRoot = nullptr;
        delete s_pPxmBranch;
        s_pPxmBranch = nullptr;
        delete s_pPxmLeave;
        s_pPxmLeave = nullptr;

        delete s_pIconRoot;
        s_pIconRoot = nullptr;
        delete s_pIconBranch;
        s_pIconBranch = nullptr;
        delete s_pIconLeave;
        s_pIconLeave = nullptr;

        s_bIconsCreated = false;
    }

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    m_pIdxTree = nullptr;
    m_entryTypeFilter = static_cast<EIdxTreeEntryType>(0);
    m_sortOrder = static_cast<EIdxTreeSortOrder>(0);
    m_mappModelTreeEntries.clear();
    m_pModelRoot = nullptr;
    m_iTrcDetailLevel = 0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setIdxTree( CIdxTree* i_pIdxTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        if( m_pIdxTree == nullptr )
        {
            strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        }
        else
        {
            strMthInArgs  = "IdxTreeOrig: " + QString(m_pIdxTree == nullptr ? "nullptr" : m_pIdxTree->objectName());
            strMthInArgs += ", IdxTreeNew: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        }
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setIdxTree",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pIdxTree != nullptr )
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
            /* szSignal     */ SIGNAL(treeEntryAdded(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryAdded(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryChanged(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryChanged(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryAboutToBeRemoved(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryAboutToBeRemoved(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryMoved(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*, const QString&, ZS::System::CIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryMoved(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*, const QString&, ZS::System::CIdxTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryKeyInTreeChanged(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryKeyInTreeChanged(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*, const QString&)) );

        _beginRemoveRows(QModelIndex(), 0, 0);

        // The root entry will neither be added to the list nor to the map of tree entries.
        //m_pModelRoot->setModel(nullptr);
        //m_pModelRoot->setModelIndexInParentBranch(-1);

        delete m_pModelRoot;
        m_pModelRoot = nullptr;

        _endRemoveRows();

        m_pIdxTree = nullptr;

        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        reset();
        #else
        _beginResetModel();
        _endResetModel();
        #endif

    } // if( m_pIdxTree != nullptr )

    m_pIdxTree = i_pIdxTree;

    if( m_pIdxTree != nullptr )
    {
        setObjectName( QString(m_pIdxTree == nullptr ? "IdxTree" : m_pIdxTree->objectName()) );

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
            /* szSignal     */ SIGNAL(treeEntryAdded(ZS::System::CIdxTree*,ZS::System::CIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryAdded(ZS::System::CIdxTree*,ZS::System::CIdxTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryChanged(ZS::System::CIdxTree*,ZS::System::CIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryChanged(ZS::System::CIdxTree*,ZS::System::CIdxTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryAboutToBeRemoved(ZS::System::CIdxTree*,ZS::System::CIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryAboutToBeRemoved(ZS::System::CIdxTree*,ZS::System::CIdxTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryMoved(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*, const QString&, ZS::System::CIdxTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryMoved(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*, const QString&, ZS::System::CIdxTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pIdxTree,
            /* szSignal     */ SIGNAL(treeEntryKeyInTreeChanged(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*, const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIdxTreeEntryKeyInTreeChanged(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*, const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( m_pIdxTree->root() != nullptr )
        {
            setObjectName(m_pIdxTree->objectName());

            _beginInsertRows(QModelIndex(), 0, 0);

            m_pModelRoot = new CModelIdxTreeEntry(m_pIdxTree->root());

            // The root entry will neither be added to the list nor to the map of tree entries.
            m_pModelRoot->setModel(this);
            m_pModelRoot->setModelIndexInParentBranch(0);
            m_pModelRoot->setSortOrder(m_sortOrder);

            _endInsertRows();

            CIdxTreeEntry* pTreeEntry;
            int idxEntry;

            for( idxEntry = 0; idxEntry < m_pIdxTree->root()->size(); ++idxEntry )
            {
                pTreeEntry = m_pIdxTree->root()->at(idxEntry);
                onIdxTreeEntryAdded(m_pIdxTree, pTreeEntry);
            }
        } // if( m_pIdxTree->root() != nullptr )
    } // if( m_pIdxTree != nullptr )

} // setIdxTree

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelIdxTree::nodeSeparator() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());
    QString strNodeSeparator = m_pIdxTree != nullptr ? m_pIdxTree->nodeSeparator() : "/";
    return strNodeSeparator;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setFilter( EIdxTreeEntryType i_entryType )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = idxTreeEntryType2Str(i_entryType);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setFilter",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_entryTypeFilter != i_entryType && m_pModelRoot != nullptr )
    {
        setFilter(m_pModelRoot, i_entryType, true);
    }

    m_entryTypeFilter = i_entryType;

} // setFilter

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_sortOrder != i_sortOrder && m_pModelRoot != nullptr )
    {
        setSortOrder(m_pModelRoot, i_sortOrder, true);
    }

    m_sortOrder = i_sortOrder;

} // setSortOrder

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setFilter(
    CModelIdxTreeEntry* i_pModelBranch,
    EIdxTreeEntryType   i_entryType,
    bool                i_bRecursive )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Branch: " + QString(i_pModelBranch == nullptr ? "nullptr" : i_pModelBranch->keyInTree());
        strMthInArgs += ", EntryType: " + idxTreeEntryType2Str(i_entryType);
        strMthInArgs += ", Recursive: " + bool2Str(i_bRecursive);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setFilter",
        /* strMethodInArgs    */ strMthInArgs );

    CIdxTreeEntry* pBranch = i_pModelBranch->treeEntry();

    CModelIdxTreeEntry* pModelTreeEntryChild;
    CIdxTreeEntry*      pTreeEntryChild;
    int                 idxTreeEntryChild;

    if( i_bRecursive && i_pModelBranch->count() > 0 )
    {
        CModelIdxTreeEntry* pModelBranchChild;

        for( idxTreeEntryChild = i_pModelBranch->count()-1; idxTreeEntryChild >= 0; --idxTreeEntryChild )
        {
            pModelTreeEntryChild = i_pModelBranch->at(idxTreeEntryChild);

            if( pModelTreeEntryChild->entryType() == EIdxTreeEntryType::Branch )
            {
                pModelBranchChild = pModelTreeEntryChild;

                setFilter(pModelBranchChild, i_entryType, i_bRecursive);
            }
        }
    } // if( i_bRecursive && i_pModelBranch->count() > 0 )

    QModelIndex modelIdxParent = _createIndex(i_pModelBranch->modelIndexInParentBranch(), 0, i_pModelBranch);

    QString strKeyInParentBranch;
    QString strKeyInTree;

    // Map containing current model entries of the model branch.
    //----------------------------------------------------------

    QMap<QString, CModelIdxTreeEntry*> mappModelTreeEntriesCurr;

    for( idxTreeEntryChild = 0; idxTreeEntryChild < i_pModelBranch->count(); ++idxTreeEntryChild )
    {
        pModelTreeEntryChild = i_pModelBranch->at(idxTreeEntryChild);
        strKeyInParentBranch = pModelTreeEntryChild->keyInParentBranch();
        mappModelTreeEntriesCurr.insert(strKeyInParentBranch, pModelTreeEntryChild);
    }

    // Resulting list of branch childs as it should be after applying the filter.
    //---------------------------------------------------------------------------

    QMap<QString, CIdxTreeEntry*> mappTreeEntriesNew;
    QVector<CIdxTreeEntry*>       arpTreeEntriesNew;

    if( i_pModelBranch->sortOrder() == EIdxTreeSortOrder::Config )
    {
        for( idxTreeEntryChild = 0; idxTreeEntryChild < pBranch->count(); ++idxTreeEntryChild )
        {
            pTreeEntryChild = pBranch->at(idxTreeEntryChild);
            strKeyInParentBranch = pTreeEntryChild->keyInParentBranch();

            if( i_entryType == EIdxTreeEntryType::Undefined )
            {
                arpTreeEntriesNew << pTreeEntryChild;
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
            else if( pTreeEntryChild->entryType() == i_entryType )
            {
                arpTreeEntriesNew << pTreeEntryChild;
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
        }
    } // if( i_pModelBranch->sortOrder() == EIdxTreeSortOrder::Config )

    else if( i_pModelBranch->sortOrder() == EIdxTreeSortOrder::Ascending )
    {
        for( idxTreeEntryChild = 0; idxTreeEntryChild < pBranch->count(); ++idxTreeEntryChild )
        {
            pTreeEntryChild = pBranch->at(idxTreeEntryChild);
            strKeyInParentBranch = pTreeEntryChild->keyInParentBranch();

            if( i_entryType == EIdxTreeEntryType::Undefined )
            {
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
            else if( pTreeEntryChild->entryType() == i_entryType )
            {
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
        }
        for( auto strKeyInParentBranch : mappTreeEntriesNew.keys() )
        {
            pTreeEntryChild = mappTreeEntriesNew[strKeyInParentBranch];
            arpTreeEntriesNew << pTreeEntryChild;
        }
    } // if( i_pModelBranch->sortOrder() == EIdxTreeSortOrder::Ascending )

    // Remove all model entries which should no longer belong to the branch.
    //----------------------------------------------------------------------

    if( i_pModelBranch->count() > 0 )
    {
        for( idxTreeEntryChild = i_pModelBranch->count()-1; idxTreeEntryChild >= 0; --idxTreeEntryChild )
        {
            pModelTreeEntryChild = i_pModelBranch->at(idxTreeEntryChild);
            strKeyInParentBranch = pModelTreeEntryChild->keyInParentBranch();

            // If the tree entry does no longer belong to the model branch ..
            if( !mappTreeEntriesNew.contains(strKeyInParentBranch) )
            {
                int idxModelInParentBranch = pModelTreeEntryChild->modelIndexInParentBranch();

                _beginRemoveRows(modelIdxParent, idxModelInParentBranch, idxModelInParentBranch);

                i_pModelBranch->remove(pModelTreeEntryChild);
                mappModelTreeEntriesCurr.remove(strKeyInParentBranch);

                QString strKeyInTree = pModelTreeEntryChild->keyInTree();

                if( !m_mappModelTreeEntries.contains(strKeyInTree) )
                {
                    throw CException(__FILE__, __LINE__, EResultObjNotInList, strKeyInTree);
                }
                m_mappModelTreeEntries.remove(strKeyInTree);

                delete pModelTreeEntryChild;
                pModelTreeEntryChild = nullptr;

                _endRemoveRows();
            }
        } // for( idxTreeEntryChild = 0; idxTreeEntryChild < pBranch->count(); ++idxTreeEntryChild )
    } // if( i_pModelBranch->sortOrder() == EIdxTreeSortOrder::None )

    // Create and add model entries which should be newly added.
    //----------------------------------------------------------

    // Please note that "arpTreeEntriesNew" is already filtered and sorted as desired.
    // The model branches "add" method takes the sort order and entry type filter
    // into account and inserts the child at the desired index "idxTreeEntryChild".
    // But for this the used entry type filter got to be set before using "add".

    i_pModelBranch->setFilter(i_entryType);

    for( idxTreeEntryChild = 0; idxTreeEntryChild < arpTreeEntriesNew.size(); ++idxTreeEntryChild )
    {
        pTreeEntryChild = arpTreeEntriesNew[idxTreeEntryChild];
        strKeyInParentBranch = pTreeEntryChild->keyInParentBranch();

        // If the tree entry does not yet belong to the model branch ..
        if( !mappModelTreeEntriesCurr.contains(strKeyInParentBranch) )
        {
            pModelTreeEntryChild = nullptr;

            if( pTreeEntryChild->entryType() == EIdxTreeEntryType::Branch )
            {
                CModelIdxTreeEntry* pModelBranch = new CModelIdxTreeEntry(pTreeEntryChild);
                pModelBranch->setSortOrder(i_pModelBranch->sortOrder());
                pModelBranch->setFilter(i_entryType);
                pModelTreeEntryChild = pModelBranch;
            }
            else if( pTreeEntryChild->entryType() == EIdxTreeEntryType::Leave )
            {
                pModelTreeEntryChild = new CModelIdxTreeEntry(pTreeEntryChild);
            }

            if( pModelTreeEntryChild != nullptr )
            {
                _beginInsertRows(modelIdxParent, idxTreeEntryChild, idxTreeEntryChild);

                i_pModelBranch->add(pModelTreeEntryChild);

                pModelTreeEntryChild->setModel(this);
                strKeyInTree = pModelTreeEntryChild->keyInTree();
                m_mappModelTreeEntries.insert(strKeyInTree, pModelTreeEntryChild);

                _endInsertRows();
            }
        }
    } // for( idxTreeEntryChild = 0; idxTreeEntryChild < pBranch->count(); ++idxTreeEntryChild )

} // setFilter

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setSortOrder(
    CModelIdxTreeEntry* i_pModelBranch,
    EIdxTreeSortOrder   i_sortOrder,
    bool                i_bRecursive )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Branch: " + QString(i_pModelBranch == nullptr ? "nullptr" : i_pModelBranch->keyInTree());
        strMthInArgs += ", SortOrder: " + idxTreeSortOrder2Str(i_sortOrder);
        strMthInArgs += ", Recursive: " + bool2Str(i_bRecursive);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry;
    int                 idxModelTreeEntry;

    if( i_pModelBranch->count() > 0 )
    {
        CModelIdxTreeEntry* pModelBranch;

        if( i_bRecursive )
        {
            for( idxModelTreeEntry = i_pModelBranch->count()-1; idxModelTreeEntry >= 0; --idxModelTreeEntry )
            {
                pModelTreeEntry = i_pModelBranch->at(idxModelTreeEntry);

                if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
                {
                    pModelBranch = pModelTreeEntry;

                    setSortOrder(pModelBranch, i_sortOrder, i_bRecursive);
                }
            }
        } // if( i_bRecursive )

        if( i_pModelBranch->count() > 1 )
        {
            QMap<QString, CModelIdxTreeEntry*> mappTreeEntries;

            CModelIdxTreeEntry* pModelTreeEntryChild;
            int                 idxEntryChild;

            QModelIndex modelIdxParent = _createIndex(i_pModelBranch->modelIndexInParentBranch(), 0, i_pModelBranch);

            _beginRemoveRows(modelIdxParent, 0, i_pModelBranch->count()-1);

            for( idxEntryChild = i_pModelBranch->count()-1; idxEntryChild >= 0; --idxEntryChild )
            {
                pModelTreeEntryChild = i_pModelBranch->at(idxEntryChild);
                mappTreeEntries.insert(pModelTreeEntryChild->keyInParentBranch(), pModelTreeEntryChild);
                i_pModelBranch->remove(pModelTreeEntryChild);
            }

            _endRemoveRows();

            _beginInsertRows(modelIdxParent, 0, mappTreeEntries.size()-1);

            i_pModelBranch->setSortOrder(i_sortOrder);

            if( i_sortOrder == EIdxTreeSortOrder::Ascending )
            {
                for( auto strKeyInParentBranch : mappTreeEntries.keys() )
                {
                    pModelTreeEntryChild = mappTreeEntries.value(strKeyInParentBranch, nullptr);
                    i_pModelBranch->add(pModelTreeEntryChild);
                }
            }
            else // if( i_sortOrder == EIdxTreeSortOrder::None )
            {
                CIdxTreeEntry* pBranch = i_pModelBranch->treeEntry();
                CIdxTreeEntry* pTreeEntryChild;

                for( idxEntryChild = 0; idxEntryChild < pBranch->count(); ++idxEntryChild )
                {
                    pTreeEntryChild = pBranch->at(idxEntryChild);

                    // If a filter is set not all of the children may be part of the model.
                    if( mappTreeEntries.contains(pTreeEntryChild->keyInParentBranch()) )
                    {
                        pModelTreeEntryChild = mappTreeEntries.value(pTreeEntryChild->keyInParentBranch());
                        i_pModelBranch->add(pModelTreeEntryChild);
                    }
                }
            }  // if( i_sortOrder == EIdxTreeSortOrder::None )

            _endInsertRows();

        } // if( i_pModelBranch->count() > 1 )
    } // if( i_pModelBranch->count() > 0 )

} // setSortOrder

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setIsExpanded(
    CModelIdxTreeEntry* i_pModelBranch,
    bool                i_bIsExpanded )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Branch: " + QString(i_pModelBranch == nullptr ? "nullptr" : i_pModelBranch->keyInTree());
        strMthInArgs += ", IsExpanded: " + bool2Str(i_bIsExpanded);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setIsExpanded",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pModelBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModelBranch == nullptr");
    }

    if( i_pModelBranch->isExpanded() != i_bIsExpanded )
    {
        i_pModelBranch->setIsExpanded(i_bIsExpanded);

        QModelIndex modelIdxTL = _createIndex(i_pModelBranch->indexInParentBranch(), 0, i_pModelBranch);
        QModelIndex modelIdxBR = _createIndex(i_pModelBranch->indexInParentBranch(), columnCount()-1, i_pModelBranch);

        emit_dataChanged(modelIdxTL, modelIdxBR);
    }

} // setIsExpanded

//------------------------------------------------------------------------------
bool CModelIdxTree::areAllParentBranchesExpanded( CModelIdxTreeEntry* i_pModelBranch ) const
//------------------------------------------------------------------------------
{
    CModelIdxTreeEntry* pModelBranchParent = i_pModelBranch->modelParentBranch();

    bool bAre = pModelBranchParent == nullptr ? true : pModelBranchParent->isExpanded();

    if( bAre && pModelBranchParent != nullptr )
    {
        pModelBranchParent = pModelBranchParent->modelParentBranch();

        bAre = pModelBranchParent == nullptr ? true : pModelBranchParent->isExpanded();
    }
    return bAre;

} // areAllParentBranchesExpanded

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setIsSelected(
    CModelIdxTreeEntry* i_pModelTreeEntry,
    bool                i_bIsSelected,
    bool                i_bRecursive )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pModelTreeEntry == nullptr ? "nullptr" : i_pModelTreeEntry->keyInTree());
        strMthInArgs += ", IsSelected: " + bool2Str(i_bIsSelected);
        strMthInArgs += ", Recursive: " + bool2Str(i_bRecursive);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setIsSelected",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pModelTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModelTreeEntry == nullptr");
    }

    CModelIdxTreeEntry* pModelTreeEntry;
    int                 idxModelTreeEntry;

    if( i_bRecursive )
    {
        if( i_pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || i_pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            CModelIdxTreeEntry* pModelBranch = i_pModelTreeEntry;

            for( idxModelTreeEntry = 0; idxModelTreeEntry < pModelBranch->count(); ++idxModelTreeEntry )
            {
                pModelTreeEntry = pModelBranch->at(idxModelTreeEntry);

                setIsSelected(pModelTreeEntry, i_bIsSelected, i_bRecursive);
            }
        }
    } // if( i_bRecursive )

    if( i_pModelTreeEntry->isSelected() != i_bIsSelected )
    {
        i_pModelTreeEntry->setIsSelected(i_bIsSelected);

        QModelIndex modelIdxTL = _createIndex(i_pModelTreeEntry->indexInParentBranch(), 0, i_pModelTreeEntry);
        QModelIndex modelIdxBR = _createIndex(i_pModelTreeEntry->indexInParentBranch(), columnCount()-1, i_pModelTreeEntry);

        emit_dataChanged(modelIdxTL, modelIdxBR);
    }

} // setIsSelected

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTree::findModelEntry( CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());
    QString strKeyInTree = i_pTreeEntry->keyInTree();
    return m_mappModelTreeEntries.value(strKeyInTree, nullptr);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QModelIndex CModelIdxTree::index( const QString& i_strKeyInTree, int i_iClm ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "KeyInTree: " + i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "index",
        /* strMethodInArgs    */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = m_mappModelTreeEntries.value(i_strKeyInTree, nullptr);

    int iRow = -1;
    int iClm = i_iClm >= 0 ? i_iClm : 0;

    if( pModelTreeEntry != nullptr )
    {
        iRow = pModelTreeEntry->modelIndexInParentBranch();
    }

    QModelIndex modelIdx = _createIndex(iRow, iClm, pModelTreeEntry);

    return modelIdx;

} // index

/*==============================================================================
public: // iterator methods
==============================================================================*/

//------------------------------------------------------------------------------
/*typename*/ CModelIdxTree::iterator CModelIdxTree::begin( iterator::ETraversalOrder i_traversalOrder )
//------------------------------------------------------------------------------
{
    CModelIdxTree::iterator itModelIdxTree(this, i_traversalOrder);

    if( i_traversalOrder == iterator::ETraversalOrder::Index )
    {
        CIdxTreeEntry* pTreeEntry;
        int            idxInTree;

        for( idxInTree = 0; idxInTree < m_pIdxTree->treeEntriesVec().size(); ++idxInTree )
        {
            pTreeEntry = m_pIdxTree->treeEntriesVec()[idxInTree];
            if( pTreeEntry != nullptr )
            {
                itModelIdxTree.m_pModelTreeEntryCurr = findModelEntry(pTreeEntry);
                break;
            }
        }
    }
    else if( i_traversalOrder == iterator::ETraversalOrder::PreOrder )
    {
        itModelIdxTree.m_pModelTreeEntryCurr = m_pModelRoot;
    }

    return itModelIdxTree;

} // iterator::begin()

//------------------------------------------------------------------------------
/*typename*/ CModelIdxTree::iterator CModelIdxTree::end()
//------------------------------------------------------------------------------
{
    CModelIdxTree::iterator itModelIdxTree(this, iterator::ETraversalOrder::Undefined);
    itModelIdxTree.m_pModelTreeEntryCurr = nullptr;
    return itModelIdxTree;

} // iterator::end()

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::onIdxTreeAboutToBeDestroyed( QObject* i_pIdxTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeAboutToBeDestroyed",
        /* strMethodInArgs    */ strMthInArgs );

    m_pIdxTree = nullptr;

    //_beginRemoveRows(QModelIndex(), 0, 0);

    clear(m_pModelRoot);

    // The root entry will neither be added to the list nor to the map of tree entries.
    m_pModelRoot->setModel(nullptr);
    m_pModelRoot->setModelIndexInParentBranch(-1);

    delete m_pModelRoot;
    m_pModelRoot = nullptr;

    //_endRemoveRows();

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    reset();
    #else
    _beginResetModel();
    _endResetModel();
    #endif

} // onIdxTreeAboutToBeDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::onIdxTreeEntryAdded(
    CIdxTree*      i_pIdxTree,
    CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryAdded",
        /* strMethodInArgs    */ strMthInArgs );

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    CModelIdxTreeEntry* pModelTreeEntry = findModelEntry(i_pTreeEntry);

    if( pModelTreeEntry == nullptr )
    {
        CIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

        CModelIdxTreeEntry* pModelParentBranch = m_pModelRoot;

        QString     strKeyInTree;
        QModelIndex modelIdxParent;
        int         idxInParentBranch;

        if( pParentBranch != m_pIdxTree->root() )
        {
            pModelParentBranch = findModelEntry(pParentBranch);

            // Must not happen as on adding a node to the index tree all parent
            // branches are created and for each parent branch the treeEntryAdded
            // signal is emitted and this slot is called created the corresponding
            // model branch entry.
            if( pModelParentBranch == nullptr )
            {
                throw CException(__FILE__, __LINE__, EResultInternalProgramError, "ModelParentBranch == nullptr");
            }
        } // if( pParentBranch != i_pIdxTree->root() )

        CIdxTreeEntry* pBranch = nullptr;
        CIdxTreeEntry* pLeave  = nullptr;

        pModelTreeEntry = nullptr;

        if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            if( m_entryTypeFilter == EIdxTreeEntryType::Undefined || m_entryTypeFilter == EIdxTreeEntryType::Branch )
            {
                pBranch = i_pTreeEntry;
                CModelIdxTreeEntry* pModelBranch = new CModelIdxTreeEntry(pBranch);
                pModelBranch->setSortOrder(m_sortOrder);
                pModelBranch->setFilter(m_entryTypeFilter);
                pModelTreeEntry = pModelBranch;
            }
        }
        else if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            if( m_entryTypeFilter == EIdxTreeEntryType::Undefined || m_entryTypeFilter == EIdxTreeEntryType::Leave )
            {
                pLeave = i_pTreeEntry;
                pModelTreeEntry = new CModelIdxTreeEntry(pLeave);
            }
        }

        if( pModelTreeEntry != nullptr )
        {
            idxInParentBranch = pModelParentBranch->add(pModelTreeEntry);
            pModelTreeEntry->setModel(this);

            strKeyInTree = pModelTreeEntry->keyInTree();
            m_mappModelTreeEntries.insert(strKeyInTree, pModelTreeEntry);

            modelIdxParent = _createIndex(pModelParentBranch->modelIndexInParentBranch(), 0, pModelParentBranch);

            _beginInsertRows(modelIdxParent, idxInParentBranch, idxInParentBranch);
            _endInsertRows();

            if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch && pBranch != nullptr )
            {
                CIdxTreeEntry* pTreeEntry;
                int            idxEntry;

                for( idxEntry = 0; idxEntry < pBranch->size(); ++idxEntry )
                {
                    pTreeEntry = pBranch->at(idxEntry);
                    onIdxTreeEntryAdded(i_pIdxTree, pTreeEntry);
                }
            } // if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        } // if( pModelTreeEntry != nullptr )
    } // if( pModelTreeEntry == nullptr )

} // onIdxTreeEntryAdded

//------------------------------------------------------------------------------
void CModelIdxTree::onIdxTreeEntryChanged(
    CIdxTree*      i_pIdxTree,
    CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryChanged",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pIdxTree == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pIdxTree == nullptr");
    }
    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pTreeEntry == nullptr");
    }

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    QModelIndex modelIdxTL = index(i_pTreeEntry->keyInTree(), 0);
    QModelIndex modelIdxBR = index(i_pTreeEntry->keyInTree(), columnCount()-1);

    emit_dataChanged(modelIdxTL, modelIdxBR);

} // onIdxTreeEntryChanged

//------------------------------------------------------------------------------
void CModelIdxTree::onIdxTreeEntryAboutToBeRemoved(
    CIdxTree*      i_pIdxTree,
    CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
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

    CModelIdxTreeEntry* pModelTreeEntry = findModelEntry(i_pTreeEntry);

    // Please note that the model may not contain each index tree entry as a filter
    // may have been applied.
    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            clear(pModelTreeEntry);
        }

        remove(pModelTreeEntry);

        delete pModelTreeEntry;
        pModelTreeEntry = nullptr;
    }

} // onIdxTreeEntryAboutToBeRemoved

//------------------------------------------------------------------------------
void CModelIdxTree::onIdxTreeEntryMoved(
    CIdxTree*      i_pIdxTree,
    CIdxTreeEntry* i_pTreeEntry,
    const QString& i_strKeyInTreePrev,
    CIdxTreeEntry* i_pTargetBranch )
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
        /* pTrcAdminObj       */ m_pTrcAdminObj,
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

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Please note that before the index tree emits the "treeEntryMoved" signal
    // and this slot is executed the "treeEntryKeyInTreeChanged" signal has been
    // emitted and the slot "onIdxTreeEntryKeyInTreeChanged" was processed.
    // Which means that the "KeyInTreePrev" has already been updated and is no
    // longer part of the models map. But the model node can be found using
    // the new key in tree.
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    CModelIdxTreeEntry* pModelTreeEntry = findModelEntry(i_pTreeEntry);
    CModelIdxTreeEntry* pModelTargetBranch = findModelEntry(i_pTargetBranch);

    // Please note that the model may not contain each index tree entry as a filter
    // may have been applied.
    if( pModelTreeEntry != nullptr && pModelTargetBranch != nullptr )
    {
        CModelIdxTreeEntry* pModelParentBranchPrev = pModelTreeEntry->modelParentBranch();

        QModelIndex modelIdxParentPrev = _createIndex(pModelParentBranchPrev->modelIndexInParentBranch(), 0, pModelParentBranchPrev);

        int idxInParentBranchPrev = pModelTreeEntry->modelIndexInParentBranch();

        _beginRemoveRows(modelIdxParentPrev, idxInParentBranchPrev, idxInParentBranchPrev);

        pModelParentBranchPrev->remove(pModelTreeEntry);

        _endRemoveRows();

        QModelIndex modelIdxTargetBranch = _createIndex(pModelTargetBranch->modelIndexInParentBranch(), 0, pModelTargetBranch);

        int idxInParentBranch = pModelTargetBranch->add(pModelTreeEntry);

        _beginInsertRows(modelIdxTargetBranch, idxInParentBranch, idxInParentBranch);
        _endInsertRows();

    } // if( pModelTreeEntry != nullptr && pModelTargetBranch != nullptr )

} // onIdxTreeEntryMoved

//------------------------------------------------------------------------------
void CModelIdxTree::onIdxTreeEntryKeyInTreeChanged(
    CIdxTree*      i_pIdxTree,
    CIdxTreeEntry* i_pTreeEntry,
    const QString& i_strKeyInTreePrev )
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
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryKeyInTreeChanged",
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

    CModelIdxTreeEntry* pModelTreeEntry = m_mappModelTreeEntries.value(i_strKeyInTreePrev, nullptr);

    // Please note that the model may not contain each index tree entry as a filter
    // may have been applied.
    if( pModelTreeEntry != nullptr )
    {
        CModelIdxTreeEntry* pModelParentBranch = pModelTreeEntry->modelParentBranch();

        if( pModelParentBranch != nullptr )
        {
            QString strBranchPathPrev;
            QString strNamePrev;
            QString strBranchPath;
            QString strName;

            QString strKeyInTree = i_pTreeEntry->keyInTree();

            m_pIdxTree->splitPathStr(i_strKeyInTreePrev, &strBranchPathPrev, &strNamePrev );
            m_pIdxTree->splitPathStr(strKeyInTree, &strBranchPath, &strName );

            QString strKeyInBranchPrev = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol) + ":" + strNamePrev;
            QString strKeyInBranch     = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol) + ":" + strName;

            if( m_mappModelTreeEntries.contains(strKeyInTree) )
            {
                throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKeyInTree);
            }

            // If the key has been changed because the name has been changed ...
            if( strBranchPath == strBranchPathPrev )
            {
                // ... we got to correct the position of the tree entry in its parent and update the key in the model.
                QModelIndex modelIdxParent = _createIndex(pModelParentBranch->modelIndexInParentBranch(), 0, pModelParentBranch);

                int idxInParentBranch = pModelTreeEntry->modelIndexInParentBranch();

                _beginRemoveRows(modelIdxParent, idxInParentBranch, idxInParentBranch);

                pModelParentBranch->remove(strKeyInBranchPrev);

                m_mappModelTreeEntries.remove(i_strKeyInTreePrev);

                _endRemoveRows();

                idxInParentBranch = pModelParentBranch->add(pModelTreeEntry);

                _beginInsertRows(modelIdxParent, idxInParentBranch, idxInParentBranch);

                m_mappModelTreeEntries.insert(strKeyInTree, pModelTreeEntry);

                _endInsertRows();

            } // if( strKeyInBranchCurr == strKeyInBranchPrev )

            // If the key has been changed because the entry has been moved ...
            else // if( strBranchPath != strBranchPathPrev )
            {
                // ... the slot "onIdxTreeEntryMoved" will be called afterwards. But here we have to update the key in the model.
                m_mappModelTreeEntries.remove(i_strKeyInTreePrev);
                m_mappModelTreeEntries.insert(strKeyInTree, pModelTreeEntry);
            }
        } // if( pModelParentBranch != nullptr )
    } // if( pModelTreeEntry != nullptr )

} // onIdxTreeEntryKeyInTreeChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::clear( CModelIdxTreeEntry* i_pModelBranch, bool i_bDestroyTreeEntries )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pModelBranch == nullptr ? "nullptr" : i_pModelBranch->keyInTree());
        strMthInArgs += ", DestroyEntries: " + bool2Str(i_bDestroyTreeEntries);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "clear",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pModelBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModelBranch == nullptr");
    }

    CModelIdxTreeEntry* pModelTreeEntry;
    int                 idxEntry;

    for( idxEntry = i_pModelBranch->count()-1; idxEntry >= 0; --idxEntry )
    {
        pModelTreeEntry = i_pModelBranch->at(idxEntry);

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            clear(pModelTreeEntry, i_bDestroyTreeEntries);
        }

        remove(pModelTreeEntry);

        if( i_bDestroyTreeEntries )
        {
            delete pModelTreeEntry;
            pModelTreeEntry = nullptr;
        }
    }
} // clear

//------------------------------------------------------------------------------
void CModelIdxTree::remove( CModelIdxTreeEntry* i_pModelTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pModelTreeEntry == nullptr ? "nullptr" : i_pModelTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
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

    if( i_pModelTreeEntry != m_pModelRoot )
    {
        CModelIdxTreeEntry* pModelParentBranch = i_pModelTreeEntry->modelParentBranch();

        int idxInParentBranch = i_pModelTreeEntry->modelIndexInParentBranch();

        if( pModelParentBranch != nullptr && idxInParentBranch >= 0 )
        {
            QModelIndex modelIdxParent = _createIndex(pModelParentBranch->modelIndexInParentBranch(), 0, pModelParentBranch);

            _beginRemoveRows(modelIdxParent, idxInParentBranch, idxInParentBranch);

            QString strKeyInTree = i_pModelTreeEntry->keyInTree();

            if( !m_mappModelTreeEntries.contains(strKeyInTree) )
            {
                throw CException(__FILE__, __LINE__, EResultObjNotInList, strKeyInTree);
            }
            m_mappModelTreeEntries.remove(strKeyInTree);

            i_pModelTreeEntry->setModel(nullptr);

            pModelParentBranch->remove(i_pModelTreeEntry);

            _endRemoveRows();

        } // if( pModelParentBranch != nullptr && idxInParentBranch >= 0 )
    } // if( i_pModelTreeEntry != m_pModelRoot )

} // remove

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTree::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "ModelIdxParent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rowCount",
        /* strMethodInArgs    */ strMthInArgs );

    int iRowCount = 1;

    if( i_modelIdxParent.isValid() )
    {
        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

        CModelIdxTreeEntry* pModelParentTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdxParent.internalPointer());

        if( pModelParentTreeEntry == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalProgramError);
        }

        if( pModelParentTreeEntry->entryType() == EIdxTreeEntryType::Root
         || pModelParentTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            CModelIdxTreeEntry* pModelParentBranch = pModelParentTreeEntry;
            iRowCount = pModelParentBranch->size();
        }
        else // if( pModelParentTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            iRowCount = 0;
        }
    } // if( i_modelIdxParent.isValid() )

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(iRowCount);
    }

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelIdxTree::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "ModelIdxParent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(EColumnCount);
    }

    return EColumnCount;

} // columnCount

//------------------------------------------------------------------------------
QModelIndex CModelIdxTree::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + QString::number(i_iCol);
        strMthInArgs += ", ModelIdxParent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "index",
        /* strMethodInArgs    */ strMthInArgs );

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker( m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    QModelIndex modelIdx;

    if( hasIndex(i_iRow,i_iCol,i_modelIdxParent) )
    {
        CModelIdxTreeEntry* pModelTreeEntry = nullptr;

        if( !i_modelIdxParent.isValid() )
        {
            pModelTreeEntry = m_pModelRoot;
        }
        else // if( i_modelIdxParent.isValid() )
        {
            CModelIdxTreeEntry* pModelParentTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdxParent.internalPointer());

            if( pModelParentTreeEntry == nullptr )
            {
                throw CException(__FILE__, __LINE__, EResultInternalProgramError);
            }

            if( pModelParentTreeEntry->entryType() != EIdxTreeEntryType::Root
             && pModelParentTreeEntry->entryType() != EIdxTreeEntryType::Branch )
            {
                throw CException(__FILE__, __LINE__, EResultInternalProgramError);
            }

            CModelIdxTreeEntry* pModelParentBranch = pModelParentTreeEntry;

            if( i_iRow >= 0 && i_iRow < pModelParentBranch->size() )
            {
                pModelTreeEntry = pModelParentBranch->at(i_iRow);
            }
        } // if( i_modelIdxParent.isValid() )

        if( pModelTreeEntry != nullptr )
        {
            modelIdx = _createIndex(i_iRow, i_iCol, pModelTreeEntry);
        }
    } // if( hasIndex(i_iRow,i_iCol,i_modelIdxParent) )

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(ModelIdx2Str(modelIdx));
    }

    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelIdxTree::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "parent",
        /* strMethodInArgs    */ strMthInArgs );

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    QModelIndex modelIdxParent;

    if( i_modelIdx.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelTreeEntry == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalProgramError);
        }

        CModelIdxTreeEntry* pModelParentBranch = pModelTreeEntry->modelParentBranch();

        if( pModelParentBranch != nullptr )
        {
            modelIdxParent = _createIndex( pModelParentBranch->modelIndexInParentBranch(), 0, pModelParentBranch);
        }
    } // if( i_modelIdx.isValid() )

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(ModelIdx2Str(modelIdxParent));
    }

    return modelIdxParent;

} // parent

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelIdxTree::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(varData.toString());
    }

    return varData;

} // headerData

//------------------------------------------------------------------------------
Qt::ItemFlags CModelIdxTree::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "flags",
        /* strMethodInArgs    */ strMthInArgs );

    Qt::ItemFlags uFlags = QAbstractItemModel::flags(i_modelIdx);

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
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
                if( pModelTreeEntry != m_pModelRoot )
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }

    return uFlags;

} // flags

//------------------------------------------------------------------------------
Qt::DropActions CModelIdxTree::supportedDropActions() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "supportedDropActions",
        /* strMethodInArgs    */ strMthInArgs );

    Qt::DropActions dropActions = Qt::CopyAction | Qt::MoveAction;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(qDropActions2Str(dropActions));
    }

    return dropActions;

} // supportedDropActions

//------------------------------------------------------------------------------
QVariant CModelIdxTree::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "data",
        /* strMethodInArgs    */ strMthInArgs );

    QVariant varData;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr )
    {
        CModelIdxTreeEntry* pModelBranch = nullptr;

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            pModelBranch = pModelTreeEntry;
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
                        if( pModelTreeEntry == m_pModelRoot )
                        {
                            varData = pModelTreeEntry->name();
                        }
                        else if( strCalculatedKeyInModel != strCalculatedKeyInTree || strCalculatedKeyInModel != strKeyInTree )
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
                        if( pModelTreeEntry != m_pModelRoot )
                        {
                            if( strCalculatedKeyInModel != strCalculatedKeyInTree || strCalculatedKeyInModel != strKeyInTree )
                            {
                                varData = QColor(Qt::red);
                            }
                        }
                    }
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = GetIcon(pModelTreeEntry->entryType());
                }
                break;
            }
            case EColumnInternalId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = QString::number(i_modelIdx.internalId()) + " (" + pointer2Str(i_modelIdx.internalPointer()) + ")";
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

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(varData.toString());
    }

    return varData;

} // data

//------------------------------------------------------------------------------
bool CModelIdxTree::setData(
    const QModelIndex& i_modelIdx,
    const QVariant&    i_varData,
    int                i_iRole )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setData",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = false;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr )
    {
        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::EditRole )
                {
                    if( pModelTreeEntry != m_pModelRoot )
                    {
                        QString strKeyInTree = pModelTreeEntry->keyInTree();
                        QString strNameNew = i_varData.toString();

                        // "canSetData" should have been called before to verify that the name can be changed.
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
                    } // if( pModelTreeEntry != m_pModelRoot )
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
        emit_dataChanged(i_modelIdx, i_modelIdx);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(bool2Str(bOk));
    }

    return bOk;

} // setData

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QStringList CModelIdxTree::mimeTypes() const
//------------------------------------------------------------------------------
{
    QStringList strlstTypes;
    strlstTypes << "application/vnd.text.list";
    return strlstTypes;

} // mimeTypes

//------------------------------------------------------------------------------
QMimeData* CModelIdxTree::mimeData( const QModelIndexList& i_arModelIdxs ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ModelIdxs [" + QString::number(i_arModelIdxs.count()) + "]";
        if( i_arModelIdxs.count() > 0 )
        {
            strMthInArgs += "(";
            for( int idx = 0; idx < i_arModelIdxs.count(); ++idx )
            {
                if( idx > 0 ) strMthInArgs += ", ";
                strMthInArgs += "[" + QString::number(idx) + ": ";
                strMthInArgs += "{" + ModelIdx2Str(i_arModelIdxs[idx]) + "}]";
            }
            strMthInArgs += ")";
        }
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "mimeData",
        /* strMethodInArgs    */ strMthInArgs );

    QMimeData*  pMimeData = new QMimeData();
    QByteArray  byteArr;
    QDataStream stream(&byteArr, QIODevice::WriteOnly);

    const CModelIdxTreeEntry* pModelTreeEntry;

    for( const QModelIndex& modelIdx : i_arModelIdxs )
    {
        if( modelIdx.isValid() )
        {
            pModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(modelIdx.internalPointer());
            QString strItem = pModelTreeEntry->keyInTree();
            stream << strItem;
        }
    }

    pMimeData->setData("application/vnd.text.list", byteArr);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodCalls )
    {
        int iAddTrcInfoDetails = m_iTrcDetailLevel >= ETraceDetailLevelVerbose ? 1 : 0;
        mthTracer.setMethodReturn(qMimeData2Str(pMimeData, iAddTrcInfoDetails));
    }

    return pMimeData;

} // mimeData

//------------------------------------------------------------------------------
bool CModelIdxTree::canDropMimeData(
    const QMimeData*   i_pMimeData,
    Qt::DropAction     i_dropAction,
    int                i_iRow,
    int                i_iClm,
    const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "MimeData {" + qMimeData2Str(i_pMimeData) + "}";
        strMthInArgs += ", DropAction: " + qDropActions2Str(i_dropAction);
        strMthInArgs += ", Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + QString::number(i_iClm);
        strMthInArgs += ", ModelIdxParent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canDropMimeData",
        /* strMethodInArgs    */ strMthInArgs );

    bool bCanDrop = false;

    if( i_dropAction != Qt::IgnoreAction && i_pMimeData->hasFormat("application/vnd.text.list") && i_modelIdxParent.isValid() )
    {
        QByteArray byteArr = i_pMimeData->data("application/vnd.text.list");

        QDataStream stream(&byteArr, QIODevice::ReadOnly);
        QStringList strlstItems;

        while( !stream.atEnd() )
        {
            QString strItem;
            stream >> strItem;
            strlstItems << strItem;
        }

        // When row and column are -1 it means that the dropped data should be considered
        // as dropped directly on parent. Usually this will mean appending the data as
        // child items of parent.
        // If row and column are greater than or equal zero, it means that the drop occurred
        // just before the specified row and column in the specified parent.

        CIdxTreeEntry* pBranchParent = nullptr;

        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

        CModelIdxTreeEntry* pModelBranchParent = static_cast<CModelIdxTreeEntry*>(i_modelIdxParent.internalPointer());

        if( pModelBranchParent != nullptr )
        {
            pBranchParent = pModelBranchParent->treeEntry();
        }

        if( pBranchParent != nullptr )
        {
            SErrResultInfo errResultInfo;
            CIdxTreeEntry* pTreeEntryDropped;

            if( strlstItems.size() > 0 )
            {
                bCanDrop = true;
            }

            for( auto strKeyInTree : strlstItems )
            {
                pTreeEntryDropped = m_pIdxTree->findEntry(strKeyInTree);

                if( pTreeEntryDropped == nullptr )
                {
                    bCanDrop = false;
                }
                else // if( pTreeEntryDropped != nullptr )
                {
                    if( i_dropAction & Qt::CopyAction )
                    {
                        errResultInfo = m_pIdxTree->canCopy(pTreeEntryDropped, pBranchParent, i_iRow);
                        if( errResultInfo.getResult() == EResultObjAlreadyInList )
                        {
                            // If an object with the same name already belongs to the branch the
                            // entry can be copied but must be renamed before.
                            bCanDrop = true;
                        }
                        else
                        {
                            bCanDrop = !errResultInfo.isErrorResult();
                        }
                    }
                    else if( i_dropAction & Qt::MoveAction )
                    {
                        errResultInfo = m_pIdxTree->canMove(pTreeEntryDropped, pBranchParent, i_iRow);
                        bCanDrop = !errResultInfo.isErrorResult();
                    }
                } // if( pTreeEntryDropped != nullptr )

                if( !bCanDrop )
                {
                    break;
                }
            } // for( auto strKeyInTree : strlstItems )
        } // if( pBranchParent != nullptr )
    } // if( i_dropAction != Qt::IgnoreAction && i_pMimeData->hasFormat("application/vnd.text.list") && i_modelIdxParent.isValid() )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodCalls )
    {
        mthTracer.setMethodReturn(bool2Str(bCanDrop));
    }

    return bCanDrop;

} // canDropMimeData

//------------------------------------------------------------------------------
bool CModelIdxTree::dropMimeData(
    const QMimeData*   i_pMimeData,
    Qt::DropAction     i_dropAction,
    int                i_iRow,
    int                i_iClm,
    const QModelIndex& i_modelIdxParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "MimeData {" + qMimeData2Str(i_pMimeData) + "}";
        strMthInArgs += ", DropAction: " + qDropActions2Str(i_dropAction);
        strMthInArgs += ", Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + QString::number(i_iClm);
        strMthInArgs += ", ModelIdxParent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dropMimeData",
        /* strMethodInArgs    */ strMthInArgs );

    bool bDropped = false;

    bool bCanDrop = canDropMimeData(i_pMimeData, i_dropAction, i_iRow, i_iClm, i_modelIdxParent);

    if( bCanDrop && i_dropAction != Qt::IgnoreAction )
    {
        QByteArray byteArr = i_pMimeData->data("application/vnd.text.list");

        QDataStream stream(&byteArr, QIODevice::ReadOnly);
        QStringList strlstItems;

        while( !stream.atEnd() )
        {
            QString strItem;
            stream >> strItem;
            strlstItems << strItem;
        }

        // When row and column are -1 it means that the dropped data should be considered
        // as dropped directly on parent. Usually this will mean appending the data as
        // child items of parent.
        // If row and column are greater than or equal zero, it means that the drop occurred
        // just before the specified row and column in the specified parent.

        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

        CModelIdxTreeEntry* pModelBranchParent = static_cast<CModelIdxTreeEntry*>(i_modelIdxParent.internalPointer());

        CIdxTreeEntry* pBranchParent = pModelBranchParent->treeEntry();

        //CModelIdxTreeEntry* pModelTreeEntrySuccessor = nullptr;
        //CIdxTreeEntry*      pTreeEntrySuccessor = nullptr;

        //if( pModelBranchParent != nullptr )
        //{
        //    if( i_iRow >= 0 && i_iRow < pModelBranchParent->count() )
        //    {
        //        pModelTreeEntrySuccessor = pModelBranchParent->at(i_iRow);
        //
        //        if( pModelTreeEntrySuccessor != nullptr )
        //        {
        //            pTreeEntrySuccessor = pModelTreeEntrySuccessor->treeEntry();
        //        }
        //    }
        //}

        CIdxTreeEntry* pTreeEntryDropped;

        for( auto strKeyInTree : strlstItems )
        {
            pTreeEntryDropped = m_pIdxTree->findEntry(strKeyInTree);

            if( pTreeEntryDropped != nullptr )
            {
                if( i_dropAction & Qt::CopyAction )
                {
                    m_pIdxTree->copy(pTreeEntryDropped, pBranchParent, i_iRow);
                }
                else if( i_dropAction & Qt::MoveAction )
                {
                    m_pIdxTree->move(pTreeEntryDropped, pBranchParent, i_iRow);
                }
            } // if( pTreeEntryDropped != nullptr )
        } // for( auto strKeyInTree : strlstItems )

        bDropped = true;

    } // if( bCanDrop && i_dropAction != Qt::IgnoreAction )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(bool2Str(bDropped));
    }

    return bDropped;

} // dropMimeData

/*==============================================================================
public: // instance methods for editing data
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CModelIdxTree::canSetData(
    const QModelIndex& i_modelIdx,
    const QVariant&    i_varData,
    int                i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
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

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pIdxTree == nullptr ? nullptr : m_pIdxTree->mutex());

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr )
    {
        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::EditRole )
                {
                    if( pModelTreeEntry != m_pModelRoot )
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
        int iAddTrcInfoDetails = m_iTrcDetailLevel >= ETraceDetailLevelVerbose ? 1 : 0;
        mthTracer.setMethodReturn(errResultInfo.toString(iAddTrcInfoDetails));
    }

    return errResultInfo;

} // canSetData

/*==============================================================================
public: // overridables of base class QAbstractItemModel (just overwritten to trace the method calls for debugging purposes)
==============================================================================*/

//------------------------------------------------------------------------------
QModelIndex CModelIdxTree::sibling( int i_iRow, int i_iCol, const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Col: " + QString::number(i_iCol);
        strMthInArgs += ", ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sibling",
        /* strMethodInArgs    */ strMthInArgs );

    QModelIndex modelIdxSibling = QAbstractItemModel::sibling(i_iRow, i_iCol, i_modelIdx);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthRet = ModelIdx2Str(modelIdxSibling);
        mthTracer.setMethodReturn(strMthRet);
    }

    return modelIdxSibling;

} // sibling

//------------------------------------------------------------------------------
bool CModelIdxTree::hasChildren( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs = "Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "hasChildren",
        /* strMethodInArgs    */ strMthInArgs );

    bool bHas = QAbstractItemModel::hasChildren(i_modelIdxParent);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bHas);
    }

    return bHas;

} // hasChildren

//------------------------------------------------------------------------------
bool CModelIdxTree::insertRows( int i_iRow, int i_iRowCount, const QModelIndex& i_modelIdxParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Count: " + QString::number(i_iRowCount);
        strMthInArgs += ", Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insertRows",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = QAbstractItemModel::insertRows(i_iRow, i_iRowCount, i_modelIdxParent);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bOk);
    }

    return bOk;

} // insertRows

//------------------------------------------------------------------------------
bool CModelIdxTree::insertColumns( int i_iCol, int i_iColCount, const QModelIndex& i_modelIdxParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Col: " + QString::number(i_iCol);
        strMthInArgs += ", Count: " + QString::number(i_iColCount);
        strMthInArgs += ", Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insertColumns",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = QAbstractItemModel::insertColumns(i_iCol, i_iColCount, i_modelIdxParent);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bOk);
    }

    return bOk;

} // insertColumns

//------------------------------------------------------------------------------
bool CModelIdxTree::removeRows( int i_iRow, int i_iRowCount, const QModelIndex& i_modelIdxParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Count: " + QString::number(i_iRowCount);
        strMthInArgs += ", Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "removeRows",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = QAbstractItemModel::removeRows(i_iRow, i_iRowCount, i_modelIdxParent);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bOk);
    }

    return bOk;

} // removeRows

//------------------------------------------------------------------------------
bool CModelIdxTree::removeColumns( int i_iCol, int i_iColCount, const QModelIndex& i_modelIdxParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Col: " + QString::number(i_iCol);
        strMthInArgs += ", Count: " + QString::number(i_iColCount);
        strMthInArgs += ", Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "removeColumns",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = QAbstractItemModel::removeColumns(i_iCol, i_iColCount, i_modelIdxParent);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bOk);
    }

    return bOk;

} // removeColumns

//------------------------------------------------------------------------------
bool CModelIdxTree::moveRows(
    const QModelIndex& i_modelIdxSourceParent,
    int                i_iRowSource,
    int                i_iRowCount,
    const QModelIndex& i_modelIdxDestParent,
    int                i_iRowDestChild )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "SourceParent {" + ModelIdx2Str(i_modelIdxSourceParent) + "}";
        strMthInArgs += ", RowSource: " + QString::number(i_iRowSource);
        strMthInArgs += ", Count: " + QString::number(i_iRowCount);
        strMthInArgs += ", DestParent {" + ModelIdx2Str(i_modelIdxDestParent) + "}";
        strMthInArgs += ", DestChild: " + QString::number(i_iRowDestChild);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "moveRows",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = QAbstractItemModel::moveRows(i_modelIdxSourceParent, i_iRowSource, i_iRowCount, i_modelIdxDestParent, i_iRowDestChild);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bOk);
    }

    return bOk;

} // moveRows

//------------------------------------------------------------------------------
bool CModelIdxTree::moveColumns(
    const QModelIndex& i_modelIdxSourceParent,
    int                i_iColSource,
    int                i_iColCount,
    const QModelIndex& i_modelIdxDestParent,
    int                i_iColDestChild )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "SourceParent {" + ModelIdx2Str(i_modelIdxSourceParent) + "}";
        strMthInArgs += ", ColSource: " + QString::number(i_iColSource);
        strMthInArgs += ", Count: " + QString::number(i_iColCount);
        strMthInArgs += ", DestParent {" + ModelIdx2Str(i_modelIdxDestParent) + "}";
        strMthInArgs += ", DestChild: " + QString::number(i_iColDestChild);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "moveColumns",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = QAbstractItemModel::moveColumns(i_modelIdxSourceParent, i_iColSource, i_iColCount, i_modelIdxDestParent, i_iColDestChild);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bOk);
    }

    return bOk;

} // moveColumns

/*==============================================================================
protected: // to trace emitting signals for debugging purposes
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::emit_dataChanged( const QModelIndex& i_modelIdxTL, const QModelIndex& i_modelIdxBR, const QVector<int>& i_ariRoles )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "TL {" + ModelIdx2Str(i_modelIdxTL) + "}";
        strMthInArgs += ", BR {" + ModelIdx2Str(i_modelIdxBR) + "}";
        strMthInArgs += ", Roles [" + QString::number(i_ariRoles.size()) + "]";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_dataChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit dataChanged(i_modelIdxTL, i_modelIdxBR, i_ariRoles);

} // emit_dataChanged

//------------------------------------------------------------------------------
void CModelIdxTree::emit_headerDataChanged( Qt::Orientation i_orientation, int i_iFirstSection, int i_iLastSection )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", First: " + QString::number(i_iFirstSection);
        strMthInArgs += ", Last: " + QString::number(i_iLastSection);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_headerDataChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit headerDataChanged(i_orientation, i_iFirstSection, i_iLastSection);

} // emit_headerDataChanged

//------------------------------------------------------------------------------
void CModelIdxTree::emit_layoutChanged( const QList<QPersistentModelIndex>& i_arModelIdxsParents, QAbstractItemModel::LayoutChangeHint i_hint )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "ModelIdxsParents [" + QString::number(i_arModelIdxsParents.size()) + "]";
        strMthInArgs += ", Hint: " + QString::number(i_hint);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_layoutChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit layoutChanged(i_arModelIdxsParents, i_hint);

} // emit_layoutChanged

//------------------------------------------------------------------------------
void CModelIdxTree::emit_layoutAboutToBeChanged( const QList<QPersistentModelIndex>& i_arModelIdxsParents, QAbstractItemModel::LayoutChangeHint i_hint )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "ModelIdxsParents [" + QString::number(i_arModelIdxsParents.size()) + "]";
        strMthInArgs += ", Hint: " + QString::number(i_hint);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_layoutAboutToBeChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit layoutAboutToBeChanged(i_arModelIdxsParents, i_hint);

} // emit_layoutAboutToBeChanged

/*==============================================================================
protected: // reimplemented to trace emitting signals for debugging purposes
==============================================================================*/

//------------------------------------------------------------------------------
inline QModelIndex CModelIdxTree::_createIndex( int i_iRow, int i_iCol, void* i_pvData ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Col: " + QString::number(i_iCol);
        strMthInArgs += ", Data: " + pointer2Str(i_pvData);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createIndex",
        /* strMethodInArgs    */ strMthInArgs );

    QModelIndex modelIdx = QAbstractItemModel::createIndex(i_iRow, i_iCol, i_pvData);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthRet = ModelIdx2Str(modelIdx);
        mthTracer.setMethodReturn(strMthRet);
    }

    return modelIdx;

} // _createIndex

//------------------------------------------------------------------------------
inline QModelIndex CModelIdxTree::_createIndex( int i_iRow, int i_iCol, quintptr i_uId ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Col: " + QString::number(i_iCol);
        strMthInArgs += ", Id: " + QString::number(i_uId);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createIndex",
        /* strMethodInArgs    */ strMthInArgs );

    QModelIndex modelIdx = QAbstractItemModel::createIndex(i_iRow, i_iCol, i_uId);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthRet = ModelIdx2Str(modelIdx);
        mthTracer.setMethodReturn(strMthRet);
    }

    return modelIdx;

} // _createIndex

//------------------------------------------------------------------------------
void CModelIdxTree::_beginInsertRows( const QModelIndex& i_modelIdxParent, int i_iFirstRow, int i_iLastRow )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
        strMthInArgs += ", First: " + QString::number(i_iFirstRow);
        strMthInArgs += ", Last: " + QString::number(i_iLastRow);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginInsertRows",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::beginInsertRows(i_modelIdxParent, i_iFirstRow, i_iLastRow);

} // _beginInsertRows

//------------------------------------------------------------------------------
void CModelIdxTree::_endInsertRows()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endInsertRows",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::endInsertRows();

} // _endInsertRows

//------------------------------------------------------------------------------
void CModelIdxTree::_beginRemoveRows( const QModelIndex &i_modelIdxParent, int i_iFirstRow, int i_iLastRow )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
        strMthInArgs += ", First: " + QString::number(i_iFirstRow);
        strMthInArgs += ", Last: " + QString::number(i_iLastRow);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginRemoveRows",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::beginRemoveRows(i_modelIdxParent, i_iFirstRow, i_iLastRow);

} // _beginRemoveRows

//------------------------------------------------------------------------------
void CModelIdxTree::_endRemoveRows()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endRemoveRows",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::endRemoveRows();

} // _endRemoveRows

//------------------------------------------------------------------------------
bool CModelIdxTree::_beginMoveRows( const QModelIndex& i_modelIdxSourceParent, int i_iRowSourceStart, int i_iRowSourceEnd, const QModelIndex& i_modelIdxDestParent, int i_iRowDest )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "SourceParent {" + ModelIdx2Str(i_modelIdxSourceParent) + "}";
        strMthInArgs += ", SourceStart: " + QString::number(i_iRowSourceStart);
        strMthInArgs += ", SourceEnd: " + QString::number(i_iRowSourceEnd);
        strMthInArgs += ", DestParent {" + ModelIdx2Str(i_modelIdxDestParent) + "}";
        strMthInArgs += ", DestChild: " + QString::number(i_iRowDest);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginMoveRows",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = QAbstractItemModel::beginMoveRows(i_modelIdxSourceParent, i_iRowSourceStart, i_iRowSourceEnd, i_modelIdxDestParent, i_iRowDest);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bOk);
    }

    return bOk;

} // _beginMoveRows

//------------------------------------------------------------------------------
void CModelIdxTree::_endMoveRows()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endMoveRows",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::endMoveRows();

} // _endMoveRows

//------------------------------------------------------------------------------
void CModelIdxTree::_beginInsertColumns( const QModelIndex& i_modelIdxParent, int i_iFirstCol, int i_iLastCol )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
        strMthInArgs += ", First: " + QString::number(i_iFirstCol);
        strMthInArgs += ", Last: " + QString::number(i_iLastCol);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginInsertColumns",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::beginInsertColumns(i_modelIdxParent, i_iFirstCol, i_iLastCol);

} // beginInsertColumns

//------------------------------------------------------------------------------
void CModelIdxTree::_endInsertColumns()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endInsertColumns",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::endInsertColumns();

} // _endInsertColumns

//------------------------------------------------------------------------------
void CModelIdxTree::_beginRemoveColumns( const QModelIndex& i_modelIdxParent, int i_iFirstCol, int i_iLastCol )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "Parent {" + ModelIdx2Str(i_modelIdxParent) + "}";
        strMthInArgs += ", First: " + QString::number(i_iFirstCol);
        strMthInArgs += ", Last: " + QString::number(i_iLastCol);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginRemoveColumns",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::beginRemoveColumns(i_modelIdxParent, i_iFirstCol, i_iLastCol);

} // _beginRemoveColumns

//------------------------------------------------------------------------------
void CModelIdxTree::_endRemoveColumns()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endRemoveColumns",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::endRemoveColumns();

} // _endRemoveColumns

//------------------------------------------------------------------------------
bool CModelIdxTree::_beginMoveColumns( const QModelIndex& i_modelIdxSourceParent, int i_iColSourceStart, int i_iColSourceEnd, const QModelIndex& i_modelIdxDestParent, int i_iColDest )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        strMthInArgs  = "SourceParent {" + ModelIdx2Str(i_modelIdxSourceParent) + "}";
        strMthInArgs += ", SourceStart: " + QString::number(i_iColSourceStart);
        strMthInArgs += ", SourceEnd: " + QString::number(i_iColSourceEnd);
        strMthInArgs += ", DestParent {" + ModelIdx2Str(i_modelIdxDestParent) + "}";
        strMthInArgs += ", DestChild: " + QString::number(i_iColDest);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginMoveColumns",
        /* strMethodInArgs    */ strMthInArgs );

    bool bOk = QAbstractItemModel::beginMoveColumns(i_modelIdxSourceParent, i_iColSourceStart, i_iColSourceEnd, i_modelIdxDestParent, i_iColDest);

    if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose )
    {
        mthTracer.setMethodReturn(bOk);
    }

    return bOk;

} // _beginMoveColumns

//------------------------------------------------------------------------------
void CModelIdxTree::_endMoveColumns()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endMoveColumns",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::endMoveColumns();

} // _endMoveColumns

//------------------------------------------------------------------------------
void CModelIdxTree::_beginResetModel()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginResetModel",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::beginResetModel();

} // _beginResetModel

//------------------------------------------------------------------------------
void CModelIdxTree::_endResetModel()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelVerbose,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endResetModel",
        /* strMethodInArgs    */ strMthInArgs );

    QAbstractItemModel::endResetModel();

} // _endResetModel

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::onTrcAdminObjChanged( QObject* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(i_pTrcAdminObj);

    if( pTrcAdminObj != nullptr && m_pTrcAdminObj == pTrcAdminObj )
    {
        m_iTrcDetailLevel = pTrcAdminObj->getTraceDetailLevel();
    }
}
