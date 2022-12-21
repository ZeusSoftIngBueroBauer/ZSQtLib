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

#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#ifdef ZS_TRACE_GUI_MODELS
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#endif

#include <QtCore/qmimedata.h>
#include <QtCore/qset.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qfontmetrics.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
Type definitions and constants
*******************************************************************************/

/* enum class EIdxTreeSortOrder
==============================================================================*/

static const SEnumEntry s_arEnumStrIdxTreeSortOrders[] = {                    // IdxName,             Symbol, Text
    /*  0 */ SEnumEntry( static_cast<int>(EIdxTreeSortOrder::Config),            "Config",            "-",    "As Configured"        ),
    /*  1 */ SEnumEntry( static_cast<int>(EIdxTreeSortOrder::Ascending),         "Ascending",         "A"     "Ascending"            ),
    /*  2 */ SEnumEntry( static_cast<int>(EIdxTreeSortOrder::Descending),        "Descending",        "D",    "Descending"           )
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
    QPixmap pxm = QPixmap( ":/ZS/TreeView/TreeViewSortOrder" + str + ".png" );
    pxm = pxm.scaled(i_sz);
    return pxm;
}

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
        int idxInTree = m_pModelTreeEntryCurr->indexInTree();

        for( ++idxInTree; idxInTree < m_pModel->idxTree()->treeEntriesVec().size(); ++idxInTree )
        {
            CIdxTreeEntry* pTreeEntry = m_pModel->idxTree()->treeEntriesVec()[idxInTree];
            if( pTreeEntry != nullptr )
            {
                pModelTreeEntryNew = m_pModel->findEntry(pTreeEntry->keyInTree());
                break;
            }
        }
    }
    else if( m_traversalOrder == ETraversalOrder::PreOrder )
    {
        if( m_pModelTreeEntryCurr == m_pModel->m_pModelRootEntry )
        {
            CModelIdxTreeEntry* pModelBranchCurr = m_pModel->m_pModelRootEntry;

            if( pModelBranchCurr->count() > 0 )
            {
                pModelTreeEntryNew = pModelBranchCurr->at(0);
            }
        }
        else // if( m_pModelTreeEntryCurr != m_pModel->m_pModelRootEntry )
        {
            if( m_pModelTreeEntryCurr->entryType() == EIdxTreeEntryType::Leave )
            {
                int idxInParentBranch = m_pModelTreeEntryCurr->indexInParentBranch();
                CModelIdxTreeEntry* pModelBranchParent = m_pModelTreeEntryCurr->parentBranch();

                if( idxInParentBranch >= (pModelBranchParent->count()-1) )
                {
                    idxInParentBranch = pModelBranchParent->indexInParentBranch();
                    pModelBranchParent = pModelBranchParent->parentBranch();

                    while( pModelBranchParent != nullptr && idxInParentBranch >= (pModelBranchParent->count()-1) )
                    {
                        idxInParentBranch = pModelBranchParent->indexInParentBranch();
                        pModelBranchParent = pModelBranchParent->parentBranch();
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
                    int idxInParentBranch = pModelBranchCurr->indexInParentBranch();
                    CModelIdxTreeEntry* pModelBranchParent = pModelBranchCurr->parentBranch();

                    while( pModelBranchParent != nullptr && idxInParentBranch >= (pModelBranchParent->count()-1) )
                    {
                        pModelBranchCurr = pModelBranchParent;
                        idxInParentBranch = pModelBranchCurr->indexInParentBranch();
                        pModelBranchParent = pModelBranchParent->parentBranch();
                    }

                    if( pModelBranchParent != nullptr && idxInParentBranch < (pModelBranchParent->count()-1) )
                    {
                        pModelTreeEntryNew = pModelBranchParent->at(idxInParentBranch+1);
                    }
                }
            }
        } // if( m_pModelTreeEntryCurr != m_pModel->m_pModelRootEntry )
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

QHash<int, QByteArray> CModelIdxTree::s_roleNames;
QHash<QByteArray, int> CModelIdxTree::s_roleValues;

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

QHash<int, QByteArray> CModelIdxTree::s_clm2Name {
    { CModelIdxTree::EColumnTreeEntryName, "TreeEntryName"},
    { CModelIdxTree::EColumnTreeEntryNameDecorated, "TreeEntryNameDecorated"},
    { CModelIdxTree::EColumnTreeEntryTypeImageUrl, "TreeEntryTypeImageUrl"},
    { CModelIdxTree::EColumnTreeEntryTypeIcon, "TreeEntryTypeIcon"},
    { CModelIdxTree::EColumnTreeEntryType, "TreeEntryType"},
    { CModelIdxTree::EColumnInternalId, "InternalId"},
    { CModelIdxTree::EColumnIdxInTree, "IdxInTree"},
    { CModelIdxTree::EColumnIdxInParentBranch, "IdxInParentBranch"},
    { CModelIdxTree::EColumnKeyInTree, "KeyInTree"},
    { CModelIdxTree::EColumnKeyInParentBranch, "KeyInParentBranch"}
};

//------------------------------------------------------------------------------
QString CModelIdxTree::column2Str(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return s_clm2Name.value(i_clm, "? (" + QByteArray::number(i_clm) + ")");
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTree::EColumn CModelIdxTree::role2Column(int i_iRole)
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
QString CModelIdxTree::role2Str(int i_iRole)
//------------------------------------------------------------------------------
{
    return s_roleNames.value(i_iRole, "? (" + QByteArray::number(i_iRole) + ")");
}

//------------------------------------------------------------------------------
int CModelIdxTree::byteArr2Role(const QByteArray& i_byteArrRole)
//------------------------------------------------------------------------------
{
    return s_roleValues.value(i_byteArrRole, Qt::DisplayRole);
}

//------------------------------------------------------------------------------
int CModelIdxTree::column2Role(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return static_cast<int>(ERole::FirstDataColumnRole) + i_clm;
}

//------------------------------------------------------------------------------
QString CModelIdxTree::modelIdx2Str( const QModelIndex& i_modelIdx, int i_iRole, bool i_bIncludeId )
//------------------------------------------------------------------------------
{
    QString str;
    if( !i_modelIdx.isValid() ) {
        str = "Invalid";
    } else {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        str  = "Row: " + QString::number(i_modelIdx.row());
        if( i_iRole >= static_cast<int>(CModelIdxTree::ERole::FirstDataColumnRole) ) {
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
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelIdxTree::getImageUrl( EIdxTreeEntryType i_entryType )
//------------------------------------------------------------------------------
{
    QString strType = ZS::System::idxTreeEntryType2Str(i_entryType);
    return ":/ZS/TreeView/TreeView" + strType + "Entry.png";
}

//------------------------------------------------------------------------------
QIcon CModelIdxTree::getIcon( EIdxTreeEntryType i_entryType )
//------------------------------------------------------------------------------
{
    QIcon icon;

    if( !s_bIconsCreated )
    {
        s_pPxmRoot = new QPixmap(":/ZS/TreeView/TreeViewRootEntry.png");
        s_pPxmBranch = new QPixmap(":/ZS/TreeView/TreeViewBranchEntry.png");
        s_pPxmLeave = new QPixmap(":/ZS/TreeView/TreeViewLeaveEntry.png");

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

} // getIcon

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CModelIdxTree::areIconsCreated()
//------------------------------------------------------------------------------
{
    return s_bIconsCreated;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTree::CModelIdxTree(
    CIdxTree* i_pIdxTree,
    QObject* i_pObjParent,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pObjParent),
    m_pIdxTree(nullptr),
    m_bExcludeLeaves(false),
    m_sortOrder(EIdxTreeSortOrder::Config),
    m_mappModelTreeEntries(),
    m_pModelRootEntry(nullptr),
    m_ariClmWidths(QVector<int>(EColumnCount))
    #ifdef ZS_TRACE_GUI_MODELS
    ,m_eTrcDetailLevel(i_eTrcDetailLevel),
    m_eTrcDetailLevelNoisyMethods(i_eTrcDetailLevelNoisyMethods),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr)
    #endif
{
    fillRoleNames();

    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    #ifdef ZS_TRACE_GUI_MODELS
    // If the tree's parent is the trace server the detail level of trace outputs
    // may not be controlled by trace admin objects as the belong to the index tree
    // of the trace server.
    if( i_pIdxTree != nullptr && dynamic_cast<CTrcServer*>(i_pIdxTree->parent()) == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
        if( m_pTrcAdminObj != nullptr ) {
            m_eTrcDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();
            QObject::connect(
                m_pTrcAdminObj, &CTrcAdminObj::changed,
                this, &CModelIdxTree::onTrcAdminObjChanged);
        }
        m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
            NameSpace(), ClassName() + "::NoisyMethods", objectName());
        if( m_pTrcAdminObjNoisyMethods != nullptr ) {
            m_eTrcDetailLevelNoisyMethods = m_pTrcAdminObjNoisyMethods->getMethodCallsTraceDetailLevel();
            QObject::connect(
                m_pTrcAdminObjNoisyMethods, &CTrcAdminObj::changed,
                this, &CModelIdxTree::onTrcAdminObjNoisyMethodsChanged);
        }
    }
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    s_iInstCount--;

    if( m_pModelRootEntry != nullptr )
    {
        clear(m_pModelRootEntry);

        try
        {
            delete m_pModelRootEntry;
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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_pTrcAdminObj != nullptr ) {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }
    if( m_pTrcAdminObjNoisyMethods != nullptr ) {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    }
    m_eTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_eTrcDetailLevelNoisyMethods = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;
    #endif

    m_pIdxTree = nullptr;
    m_bExcludeLeaves = false;
    m_sortOrder = static_cast<EIdxTreeSortOrder>(0);
    m_mappModelTreeEntries.clear();
    m_pModelRootEntry = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setIdxTree( CIdxTree* i_pIdxTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ) {
        if( m_pIdxTree == nullptr ) {
            strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        }
        else {
            strMthInArgs  = "IdxTreeOrig: " + QString(m_pIdxTree == nullptr ? "nullptr" : m_pIdxTree->objectName());
            strMthInArgs += ", IdxTreeNew: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        }
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setIdxTree",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_pIdxTree != nullptr )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        QObject::disconnect(
            m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
            this, &CModelIdxTree::onIdxTreeAboutToBeDestroyed);
        QObject::disconnect(
            m_pIdxTree, &CIdxTree::treeEntryAdded,
            this, &CModelIdxTree::onIdxTreeEntryAdded);
        QObject::disconnect(
            m_pIdxTree, &CIdxTree::treeEntryChanged,
            this, &CModelIdxTree::onIdxTreeEntryChanged);
        QObject::disconnect(
            m_pIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
            this, &CModelIdxTree::onIdxTreeEntryAboutToBeRemoved);
        QObject::disconnect(
            m_pIdxTree, &CIdxTree::treeEntryMoved,
            this, &CModelIdxTree::onIdxTreeEntryMoved);
        QObject::disconnect(
            m_pIdxTree, &CIdxTree::treeEntryKeyInTreeChanged,
            this, &CModelIdxTree::onIdxTreeEntryKeyInTreeChanged);

        _beginRemoveRows(QModelIndex(), 0, 0);

        // The root entry will neither be added to the list nor to the map of tree entries.
        //m_pModelRootEntry->setModel(nullptr);
        //m_pModelRootEntry->setModelIndexInParentBranch(-1);

        delete m_pModelRootEntry;
        m_pModelRootEntry = nullptr;

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
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        setObjectName( QString(m_pIdxTree == nullptr ? "IdxTree" : m_pIdxTree->objectName()) );

        QObject::connect(
            m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
            this, &CModelIdxTree::onIdxTreeAboutToBeDestroyed);
        QObject::connect(
            m_pIdxTree, &CIdxTree::treeEntryAdded,
            this, &CModelIdxTree::onIdxTreeEntryAdded);
        QObject::connect(
            m_pIdxTree, &CIdxTree::treeEntryChanged,
            this, &CModelIdxTree::onIdxTreeEntryChanged);
        QObject::connect(
            m_pIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
            this, &CModelIdxTree::onIdxTreeEntryAboutToBeRemoved);
        QObject::connect(
            m_pIdxTree, &CIdxTree::treeEntryMoved,
            this, &CModelIdxTree::onIdxTreeEntryMoved);
        QObject::connect(
            m_pIdxTree, &CIdxTree::treeEntryKeyInTreeChanged,
            this, &CModelIdxTree::onIdxTreeEntryKeyInTreeChanged);

        if( m_pIdxTree->root() != nullptr )
        {
            setObjectName(m_pIdxTree->objectName());

            _beginInsertRows(QModelIndex(), 0, 0);

            m_pModelRootEntry = new CModelIdxTreeEntry(m_pIdxTree->root());

            // The root entry will neither be added to the list nor to the map of tree entries.
            m_pModelRootEntry->setIndexInParentBranch(0);
            m_pModelRootEntry->setSortOrder(m_sortOrder);

            _endInsertRows();

            for( int idxEntry = 0; idxEntry < m_pIdxTree->root()->size(); ++idxEntry )
            {
                CIdxTreeEntry* pTreeEntry = m_pIdxTree->root()->at(idxEntry);
                onIdxTreeEntryAdded(pTreeEntry->keyInTree());
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
    CIdxTreeLocker idxTreeLocker(m_pIdxTree);
    QString strNodeSeparator = m_pIdxTree != nullptr ? m_pIdxTree->nodeSeparator() : "/";
    return strNodeSeparator;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setExcludeLeaves( bool i_bExcludeLeaves )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bExcludeLeaves);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setExcludeLeaves",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_bExcludeLeaves != i_bExcludeLeaves )
    {
        if( m_pModelRootEntry != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);
            setExcludeLeaves(m_pModelRootEntry, i_bExcludeLeaves, true);
        }
        m_bExcludeLeaves = i_bExcludeLeaves;
        emit_excludeLeavesChanged(m_bExcludeLeaves);
    }
}

//------------------------------------------------------------------------------
bool CModelIdxTree::areLeavesExcluded() const
//------------------------------------------------------------------------------
{
    return m_bExcludeLeaves;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
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
        if( m_pModelRootEntry != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);
            setSortOrder(m_pModelRootEntry, i_sortOrder, true);
        }
        m_sortOrder = i_sortOrder;
        emit_sortOrderChanged(m_sortOrder);
        emit_sortOrderChanged(idxTreeSortOrder2Str(m_sortOrder));
    }
}

//------------------------------------------------------------------------------
void CModelIdxTree::setSortOrder( const QString& i_strSortOrder )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strSortOrder;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
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
        if( m_pModelRootEntry != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);
            setSortOrder(m_pModelRootEntry, sortOrder, true);
        }
        m_sortOrder = sortOrder;
        emit_sortOrderChanged(m_sortOrder);
        emit_sortOrderChanged(idxTreeSortOrder2Str(m_sortOrder));
    }
}

//------------------------------------------------------------------------------
EIdxTreeSortOrder CModelIdxTree::sortOrder() const
//------------------------------------------------------------------------------
{
    return m_sortOrder;
}

//------------------------------------------------------------------------------
QString CModelIdxTree::sortOrderAsString() const
//------------------------------------------------------------------------------
{
    return idxTreeSortOrder2Str(m_sortOrder);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setExcludeLeaves(
    CModelIdxTreeEntry* i_pModelBranch,
    bool                i_bExcludeLeaves,
    bool                i_bRecursive )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Branch: " + QString(i_pModelBranch == nullptr ? "nullptr" : i_pModelBranch->keyInTree());
        strMthInArgs += ", ExludeLeaves: " + bool2Str(i_bExcludeLeaves);
        strMthInArgs += ", Recursive: " + bool2Str(i_bRecursive);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setExcludeLeaves",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    // Protected method which may only be invoked by public setExcludeLeaves method.
    // The index tree is already locked by the public method.
    //CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    if( i_bRecursive && i_pModelBranch->count() > 0 )
    {
        for( int idxTreeEntryChild = i_pModelBranch->count()-1; idxTreeEntryChild >= 0; --idxTreeEntryChild )
        {
            CModelIdxTreeEntry* pModelTreeEntryChild = i_pModelBranch->at(idxTreeEntryChild);
            if( pModelTreeEntryChild->entryType() == EIdxTreeEntryType::Branch )
            {
                setExcludeLeaves(pModelTreeEntryChild, i_bExcludeLeaves, i_bRecursive);
            }
        }
    }

    // Map containing current model entries of the model branch.
    //----------------------------------------------------------

    QMap<QString, CModelIdxTreeEntry*> mappModelTreeEntriesCurr;

    for( int idxTreeEntryChild = 0; idxTreeEntryChild < i_pModelBranch->count(); ++idxTreeEntryChild )
    {
        CModelIdxTreeEntry* pModelTreeEntryChild = i_pModelBranch->at(idxTreeEntryChild);
        QString strKeyInParentBranch = pModelTreeEntryChild->keyInParentBranch();
        mappModelTreeEntriesCurr.insert(strKeyInParentBranch, pModelTreeEntryChild);
    }

    // Resulting list of branch childs as it should be after applying the filter.
    //---------------------------------------------------------------------------

    CIdxTreeEntry* pBranch = i_pModelBranch->getIdxTreeEntry();

    QMap<QString, CIdxTreeEntry*> mappTreeEntriesNew;
    QVector<CIdxTreeEntry*>       arpTreeEntriesNew;

    if( i_pModelBranch->sortOrder() == EIdxTreeSortOrder::Ascending )
    {
        for( int idxTreeEntryChild = 0; idxTreeEntryChild < pBranch->count(); ++idxTreeEntryChild )
        {
            CIdxTreeEntry* pTreeEntryChild = pBranch->at(idxTreeEntryChild);
            QString strKeyInParentBranch = pTreeEntryChild->keyInParentBranch();

            if( !i_bExcludeLeaves )
            {
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
            else if( pTreeEntryChild->entryType() != EIdxTreeEntryType::Leave )
            {
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
        }
        for( auto strKeyInParentBranch : mappTreeEntriesNew.keys() )
        {
            CIdxTreeEntry* pTreeEntryChild = mappTreeEntriesNew[strKeyInParentBranch];
            arpTreeEntriesNew << pTreeEntryChild;
        }
    }
    else if( i_pModelBranch->sortOrder() == EIdxTreeSortOrder::Descending )
    {
        for( int idxTreeEntryChild = 0; idxTreeEntryChild < pBranch->count(); ++idxTreeEntryChild )
        {
            CIdxTreeEntry* pTreeEntryChild = pBranch->at(idxTreeEntryChild);
            QString strKeyInParentBranch = pTreeEntryChild->keyInParentBranch();

            if( !i_bExcludeLeaves )
            {
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
            else if( pTreeEntryChild->entryType() != EIdxTreeEntryType::Leave )
            {
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
        }
        QMapIterator<QString, CIdxTreeEntry*> it(mappTreeEntriesNew);
        it.toBack();
        while(it.hasPrevious())
        {
            it.previous();
            QString strKeyInParentBranch = it.key();
            CIdxTreeEntry* pTreeEntryChild = mappTreeEntriesNew[strKeyInParentBranch];
            arpTreeEntriesNew << pTreeEntryChild;
        }
    }
    else
    {
        for( int idxTreeEntryChild = 0; idxTreeEntryChild < pBranch->count(); ++idxTreeEntryChild )
        {
            CIdxTreeEntry* pTreeEntryChild = pBranch->at(idxTreeEntryChild);
            QString strKeyInParentBranch = pTreeEntryChild->keyInParentBranch();

            if( !i_bExcludeLeaves )
            {
                arpTreeEntriesNew << pTreeEntryChild;
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
            else if( pTreeEntryChild->entryType() != EIdxTreeEntryType::Leave )
            {
                arpTreeEntriesNew << pTreeEntryChild;
                mappTreeEntriesNew.insert(strKeyInParentBranch, pTreeEntryChild);
            }
        }
    } // if( i_pModelBranch->sortOrder() == EIdxTreeSortOrder::Config )

    // Remove all model entries which should no longer belong to the branch.
    //----------------------------------------------------------------------

    QModelIndex modelIdxParent = _createIndex(i_pModelBranch->indexInParentBranch(), 0, i_pModelBranch);

    if( i_pModelBranch->count() > 0 )
    {
        for( int idxTreeEntryChild = i_pModelBranch->count()-1; idxTreeEntryChild >= 0; --idxTreeEntryChild )
        {
            CModelIdxTreeEntry* pModelTreeEntryChild = i_pModelBranch->at(idxTreeEntryChild);
            QString strKeyInParentBranch = pModelTreeEntryChild->keyInParentBranch();

            // If the tree entry does no longer belong to the model branch ..
            if( !mappTreeEntriesNew.contains(strKeyInParentBranch) )
            {
                int idxModelInParentBranch = pModelTreeEntryChild->indexInParentBranch();

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

    i_pModelBranch->setExcludeLeaves(i_bExcludeLeaves);

    for( int idxTreeEntryChild = 0; idxTreeEntryChild < arpTreeEntriesNew.size(); ++idxTreeEntryChild )
    {
        CIdxTreeEntry* pTreeEntryChild = arpTreeEntriesNew[idxTreeEntryChild];
        QString strKeyInParentBranch = pTreeEntryChild->keyInParentBranch();

        // If the tree entry does not yet belong to the model branch ..
        if( !mappModelTreeEntriesCurr.contains(strKeyInParentBranch) )
        {
            CModelIdxTreeEntry* pModelTreeEntryChild = nullptr;

            if( pTreeEntryChild->entryType() == EIdxTreeEntryType::Branch )
            {
                CModelIdxTreeEntry* pModelBranch = new CModelIdxTreeEntry(pTreeEntryChild);
                pModelBranch->setSortOrder(i_pModelBranch->sortOrder());
                pModelBranch->setExcludeLeaves(i_bExcludeLeaves);
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

                QString strKeyInTree = pModelTreeEntryChild->keyInTree();
                m_mappModelTreeEntries.insert(strKeyInTree, pModelTreeEntryChild);

                _endInsertRows();
            }
        }
    } // for( idxTreeEntryChild = 0; idxTreeEntryChild < pBranch->count(); ++idxTreeEntryChild )

} // setExcludeLeaves

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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Branch: " + QString(i_pModelBranch == nullptr ? "nullptr" : i_pModelBranch->keyInTree());
        strMthInArgs += ", SortOrder: " + idxTreeSortOrder2Str(i_sortOrder);
        strMthInArgs += ", Recursive: " + bool2Str(i_bRecursive);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    // Protected method which may only be invoked by public setExcludeLeaves method.
    // The index tree is already locked by the public method.
    //CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    if( i_pModelBranch->count() == 0 )
    {
        i_pModelBranch->setSortOrder(i_sortOrder);
    }
    else
    {
        //CModelIdxTreeEntry* pModelTreeEntry;
        //int                 idxModelTreeEntry;
        //CModelIdxTreeEntry* pModelBranch;

        if( i_bRecursive )
        {
            for( int idxTreeEntryChild = i_pModelBranch->count()-1; idxTreeEntryChild >= 0; --idxTreeEntryChild )
            {
                CModelIdxTreeEntry* pModelTreeEntryChild = i_pModelBranch->at(idxTreeEntryChild);
                if( pModelTreeEntryChild->entryType() == EIdxTreeEntryType::Branch )
                {
                    setSortOrder(pModelTreeEntryChild, i_sortOrder, i_bRecursive);
                }
            }
        }

        if( i_pModelBranch->count() == 1 )
        {
            i_pModelBranch->setSortOrder(i_sortOrder);
        }
        else
        {
            QMap<QString, CModelIdxTreeEntry*> mappTreeEntries;

            CModelIdxTreeEntry* pModelTreeEntryChild;
            int                 idxEntryChild;

            QModelIndex modelIdxParent = _createIndex(i_pModelBranch->indexInParentBranch(), 0, i_pModelBranch);

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
                    pModelTreeEntryChild = mappTreeEntries.value(strKeyInParentBranch);
                    i_pModelBranch->add(pModelTreeEntryChild);
                }
            }
            else if( i_sortOrder == EIdxTreeSortOrder::Descending )
            {
                QMapIterator<QString, CModelIdxTreeEntry*> it(mappTreeEntries);
                it.toBack();
                while(it.hasPrevious())
                {
                    it.previous();
                    pModelTreeEntryChild = mappTreeEntries.value(it.key());
                    i_pModelBranch->add(pModelTreeEntryChild);
                }
            }
            else
            {
                CIdxTreeEntry* pBranch = i_pModelBranch->getIdxTreeEntry();
                CIdxTreeEntry* pTreeEntryChild;

                for( idxEntryChild = 0; idxEntryChild < pBranch->count(); ++idxEntryChild )
                {
                    pTreeEntryChild = pBranch->at(idxEntryChild);

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
QString CModelIdxTree::getTreeEntryTypeAsString( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strEntryType;
    CModelIdxTreeEntry* pModelTreeEntry = nullptr;
    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }
    if( pModelTreeEntry != nullptr )
    {
        strEntryType = pModelTreeEntry->entryType2Str();
    }
    else
    {
        strEntryType = idxTreeEntryType2Str(EIdxTreeEntryType::Undefined);
    }
    return strEntryType;
}

//------------------------------------------------------------------------------
QString CModelIdxTree::getTreeEntryName( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strEntryName;
    CModelIdxTreeEntry* pModelTreeEntry = nullptr;
    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }
    if( pModelTreeEntry != nullptr )
    {
        strEntryName = pModelTreeEntry->name();
    }
    return strEntryName;
}

//------------------------------------------------------------------------------
QString CModelIdxTree::getTreeEntryPath( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strEntryPath;
    CModelIdxTreeEntry* pModelTreeEntry = nullptr;
    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }
    if( pModelTreeEntry != nullptr )
    {
        strEntryPath = pModelTreeEntry->path();
    }
    return strEntryPath;
}

//------------------------------------------------------------------------------
QString CModelIdxTree::getTreeEntryAbsoluteNodePath( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strEntryPath;
    CModelIdxTreeEntry* pModelTreeEntry = nullptr;
    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }
    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root )
        {
            strEntryPath = pModelTreeEntry->name();
        }
        else
        {
            strEntryPath = m_pModelRootEntry->name();
            strEntryPath += nodeSeparator();
            if( pModelTreeEntry->parentBranch() != m_pModelRootEntry )
            {
                strEntryPath += pModelTreeEntry->path();
                strEntryPath += nodeSeparator();
            }
            strEntryPath += pModelTreeEntry->name();
        }
    }
    return strEntryPath;
}

//------------------------------------------------------------------------------
QString CModelIdxTree::getTreeEntryKeyInTree( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strEntryKey;
    CModelIdxTreeEntry* pModelTreeEntry = nullptr;
    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }
    if( pModelTreeEntry != nullptr )
    {
        strEntryKey = pModelTreeEntry->keyInTree();
    }
    return strEntryKey;
}

//------------------------------------------------------------------------------
int CModelIdxTree::getTreeEntryIndexInTree( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    int idx = -1;
    CModelIdxTreeEntry* pModelTreeEntry = nullptr;
    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }
    if( pModelTreeEntry != nullptr )
    {
        idx = pModelTreeEntry->indexInTree();
    }
    return idx;
}

//------------------------------------------------------------------------------
QString CModelIdxTree::getTreeEntryKeyInParentPath( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strEntryKey;
    CModelIdxTreeEntry* pModelTreeEntry = nullptr;
    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }
    if( pModelTreeEntry != nullptr )
    {
        strEntryKey = pModelTreeEntry->keyInParentBranch();
    }
    return strEntryKey;
}

//------------------------------------------------------------------------------
int CModelIdxTree::getTreeEntryKeyIndexInParentPath( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    int idx = -1;
    CModelIdxTreeEntry* pModelTreeEntry = nullptr;
    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }
    if( pModelTreeEntry != nullptr )
    {
        idx = pModelTreeEntry->indexInParentBranch();
    }
    return idx;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::setIsExpanded(
    CModelIdxTreeEntry* i_pModelBranch,
    bool                i_bIsExpanded )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Branch: " + QString(i_pModelBranch == nullptr ? "nullptr" : i_pModelBranch->keyInTree());
        strMthInArgs += ", IsExpanded: " + bool2Str(i_bIsExpanded);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setIsExpanded",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

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
    CModelIdxTreeEntry* pModelBranchParent = i_pModelBranch->parentBranch();

    bool bAre = pModelBranchParent == nullptr ? true : pModelBranchParent->isExpanded();

    if( bAre && pModelBranchParent != nullptr )
    {
        pModelBranchParent = pModelBranchParent->parentBranch();

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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pModelTreeEntry == nullptr ? "nullptr" : i_pModelTreeEntry->keyInTree());
        strMthInArgs += ", IsSelected: " + bool2Str(i_bIsSelected);
        strMthInArgs += ", Recursive: " + bool2Str(i_bRecursive);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setIsSelected",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

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
CModelIdxTreeEntry* CModelIdxTree::findEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    return m_mappModelTreeEntries.value(i_strKeyInTree, nullptr);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QModelIndex CModelIdxTree::index( const QString& i_strKeyInTree, int i_iClm ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "KeyInTree: " + i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "index",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CModelIdxTreeEntry* pModelTreeEntry = m_mappModelTreeEntries.value(i_strKeyInTree, nullptr);

    int iRow = -1;
    int iClm = i_iClm >= 0 ? i_iClm : 0;

    if( pModelTreeEntry != nullptr )
    {
        iRow = pModelTreeEntry->indexInParentBranch();
    }

    QModelIndex modelIdx = _createIndex(iRow, iClm, pModelTreeEntry);

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(modelIdx2Str(modelIdx));
    }
    #endif

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
                itModelIdxTree.m_pModelTreeEntryCurr = findEntry(pTreeEntry->keyInTree());
                break;
            }
        }
    }
    else if( i_traversalOrder == iterator::ETraversalOrder::PreOrder )
    {
        itModelIdxTree.m_pModelTreeEntryCurr = m_pModelRootEntry;
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
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::removeEntries( const QModelIndexList& i_modelIdxList )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdxList[" + QString::number(i_modelIdxList.count()) + "]";
        if( i_modelIdxList.count() > 0 ) {
            strMthInArgs += "(";
            for( const QModelIndex& modelIdx : i_modelIdxList ) {
                if( !strMthInArgs.endsWith("(")) {
                    strMthInArgs += ", ";
                }
                strMthInArgs += "{" + modelIdx2Str(modelIdx) + "}";
            }
            strMthInArgs += ")";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeEntries",
        /* strMthInArgs */ strMthInArgs );

    if( i_modelIdxList.count() > 0 )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);
        for( const QModelIndex& modelIdx : i_modelIdxList )
        {
            removeEntry(modelIdx);
        }
    }

} // removeEntries

//------------------------------------------------------------------------------
void CModelIdxTree::removeEntry( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = modelIdx2Str(i_modelIdx);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "removeEntry",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    if( pModelTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModelTreeEntry == nullptr");
    }

    if( pModelTreeEntry != m_pModelRootEntry )
    {
        CIdxTreeEntry* pTreeEntry = pModelTreeEntry->getIdxTreeEntry();
        if( pTreeEntry != nullptr )
        {
            // The index tree has been locked.
            // No other thread may change the index tree and its entries.
            // The dtor of the tree entry will remove itself from the index tree.
            // The index tree will emit the "treeEntryAboutToBeRemoved" and
            // "treeEntryRemoved" signals whereupon the slot method
            // "onIdxTreeEntryAboutToBeRemoved" of this model is called.
            // The slot removes and destroys the model entry.
            delete pTreeEntry;
            pTreeEntry = nullptr;
        }
    }
} // removeEntry

//------------------------------------------------------------------------------
SErrResultInfo CModelIdxTree::canCopy( const QModelIndex& i_modelIdxSource, const QModelIndex& i_modelIdxTarget )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Source {" + modelIdx2Str(i_modelIdxSource) + "}";
        strMthInArgs += ", Target {" + modelIdx2Str(i_modelIdxTarget) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    SErrResultInfo errResultInfo;

    //CModelIdxTreeEntry* pModelBranchTrg = nullptr;

    //if( pModelTreeEntrySelected->isLeave() )
    //{
    //    pModelBranchTrg = pModelTreeEntry->parentBranch();
    //}
    //else
    //{
    //    pModelBranchTrg = pModelTreeEntry;
    //}
    //if( pModelBranchTrg != nullptr )
    //{
    //    CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
    //    //CIdxTreeEntry*      pTreeEntrySrc = pModelTreeEntrySrc->treeEntry();
    //    //CIdxTreeEntry*      pBranchTrg    = pModelBranchTrg->treeEntry();

    //    int idxInTargetBranch = -1;

    //    if( pModelIdxTree->sortOrder() == EIdxTreeSortOrder::Config )
    //    {
    //        idxInTargetBranch = m_modelIdxSelectedForPaste.row();
    //    }
    //    else // if( pModelIdxTree->sortOrder() != EIdxTreeSortOrder::Config )
    //    {
    //        idxInTargetBranch = pTreeEntrySrc->indexInParentBranch();
    //    }

    //    if( m_pasteMode == EPasteMode::Copy )
    //    {
    //        SErrResultInfo errResultInfo = pIdxTree->canCopy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

    //        if( errResultInfo.isErrorResult() )
    //        {
    //            if( !m_bSilentlyIgnoreInvalidCopyRequests )
    //            {
    //                CDlgErr* pDlgErr = new CDlgErr("Error");
    //                pDlgErr->setErrResultInfo(errResultInfo);
    //                pDlgErr->show();
    //            }
    //        }
    //        else // if( !errResultInfo.isErrorResult() )
    //        {
    //            pIdxTree->copy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);
    //        }
    //    }
    //    else if( m_pasteMode == EPasteMode::Cut )
    //    {
    //        SErrResultInfo errResultInfo = pIdxTree->canMove(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

    //        if( errResultInfo.isErrorResult() )
    //        {
    //            if( !m_bSilentlyIgnoreInvalidCopyRequests )
    //            {
    //                CDlgErr* pDlgErr = new CDlgErr("Error");
    //                pDlgErr->setErrResultInfo(errResultInfo);
    //                pDlgErr->show();
    //             }
    //        }
    //        else // if( !errResultInfo.isErrorResult() )
    //        {
    //            pIdxTree->move(pTreeEntrySrc, pBranchTrg);
    //        }
    //    }
    //} // if( pModelBranchTrg != nullptr )

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(errResultInfo.toString());
    }
    return errResultInfo;
}

//------------------------------------------------------------------------------
int CModelIdxTree::copy( const QModelIndex& i_modelIdxSource, const QModelIndex& i_modelIdxTarget )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Source {" + modelIdx2Str(i_modelIdxSource) + "}";
        strMthInArgs += ", Target {" + modelIdx2Str(i_modelIdxTarget) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    int idxInTree = -1;

    //CModelIdxTreeEntry* pModelBranchTrg = nullptr;

    //if( pModelTreeEntrySelected->isLeave() )
    //{
    //    pModelBranchTrg = pModelTreeEntry->parentBranch();
    //}
    //else
    //{
    //    pModelBranchTrg = pModelTreeEntry;
    //}
    //if( pModelBranchTrg != nullptr )
    //{
    //    CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
    //    //CIdxTreeEntry*      pTreeEntrySrc = pModelTreeEntrySrc->treeEntry();
    //    //CIdxTreeEntry*      pBranchTrg    = pModelBranchTrg->treeEntry();

    //    int idxInTargetBranch = -1;

    //    if( pModelIdxTree->sortOrder() == EIdxTreeSortOrder::Config )
    //    {
    //        idxInTargetBranch = m_modelIdxSelectedForPaste.row();
    //    }
    //    else // if( pModelIdxTree->sortOrder() != EIdxTreeSortOrder::Config )
    //    {
    //        idxInTargetBranch = pTreeEntrySrc->indexInParentBranch();
    //    }

    //    if( m_pasteMode == EPasteMode::Copy )
    //    {
    //        SErrResultInfo errResultInfo = pIdxTree->canCopy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

    //        if( errResultInfo.isErrorResult() )
    //        {
    //            if( !m_bSilentlyIgnoreInvalidCopyRequests )
    //            {
    //                CDlgErr* pDlgErr = new CDlgErr("Error");
    //                pDlgErr->setErrResultInfo(errResultInfo);
    //                pDlgErr->show();
    //            }
    //        }
    //        else // if( !errResultInfo.isErrorResult() )
    //        {
    //            pIdxTree->copy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);
    //        }
    //    }
    //    else if( m_pasteMode == EPasteMode::Cut )
    //    {
    //        SErrResultInfo errResultInfo = pIdxTree->canMove(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

    //        if( errResultInfo.isErrorResult() )
    //        {
    //            if( !m_bSilentlyIgnoreInvalidCopyRequests )
    //            {
    //                CDlgErr* pDlgErr = new CDlgErr("Error");
    //                pDlgErr->setErrResultInfo(errResultInfo);
    //                pDlgErr->show();
    //             }
    //        }
    //        else // if( !errResultInfo.isErrorResult() )
    //        {
    //            pIdxTree->move(pTreeEntrySrc, pBranchTrg);
    //        }
    //    }
    //} // if( pModelBranchTrg != nullptr )

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(idxInTree);
    }
    return idxInTree;
}

//------------------------------------------------------------------------------
SErrResultInfo CModelIdxTree::canMove( const QModelIndex& i_modelIdxSource, const QModelIndex& i_modelIdxTarget )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Source {" + modelIdx2Str(i_modelIdxSource) + "}";
        strMthInArgs += ", Target {" + modelIdx2Str(i_modelIdxTarget) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    SErrResultInfo errResultInfo;

    //CModelIdxTreeEntry* pModelBranchTrg = nullptr;

    //if( pModelTreeEntrySelected->isLeave() )
    //{
    //    pModelBranchTrg = pModelTreeEntry->parentBranch();
    //}
    //else
    //{
    //    pModelBranchTrg = pModelTreeEntry;
    //}
    //if( pModelBranchTrg != nullptr )
    //{
    //    CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
    //    //CIdxTreeEntry*      pTreeEntrySrc = pModelTreeEntrySrc->treeEntry();
    //    //CIdxTreeEntry*      pBranchTrg    = pModelBranchTrg->treeEntry();

    //    int idxInTargetBranch = -1;

    //    if( pModelIdxTree->sortOrder() == EIdxTreeSortOrder::Config )
    //    {
    //        idxInTargetBranch = m_modelIdxSelectedForPaste.row();
    //    }
    //    else // if( pModelIdxTree->sortOrder() != EIdxTreeSortOrder::Config )
    //    {
    //        idxInTargetBranch = pTreeEntrySrc->indexInParentBranch();
    //    }

    //    if( m_pasteMode == EPasteMode::Copy )
    //    {
    //        SErrResultInfo errResultInfo = pIdxTree->canCopy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

    //        if( errResultInfo.isErrorResult() )
    //        {
    //            if( !m_bSilentlyIgnoreInvalidCopyRequests )
    //            {
    //                CDlgErr* pDlgErr = new CDlgErr("Error");
    //                pDlgErr->setErrResultInfo(errResultInfo);
    //                pDlgErr->show();
    //            }
    //        }
    //        else // if( !errResultInfo.isErrorResult() )
    //        {
    //            pIdxTree->copy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);
    //        }
    //    }
    //    else if( m_pasteMode == EPasteMode::Cut )
    //    {
    //        SErrResultInfo errResultInfo = pIdxTree->canMove(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

    //        if( errResultInfo.isErrorResult() )
    //        {
    //            if( !m_bSilentlyIgnoreInvalidCopyRequests )
    //            {
    //                CDlgErr* pDlgErr = new CDlgErr("Error");
    //                pDlgErr->setErrResultInfo(errResultInfo);
    //                pDlgErr->show();
    //             }
    //        }
    //        else // if( !errResultInfo.isErrorResult() )
    //        {
    //            pIdxTree->move(pTreeEntrySrc, pBranchTrg);
    //        }
    //    }
    //} // if( pModelBranchTrg != nullptr )

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(errResultInfo.toString());
    }
    return errResultInfo;
}

//------------------------------------------------------------------------------
void CModelIdxTree::move( const QModelIndex& i_modelIdxSource, const QModelIndex& i_modelIdxTarget )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Source {" + modelIdx2Str(i_modelIdxSource) + "}";
        strMthInArgs += ", Target {" + modelIdx2Str(i_modelIdxTarget) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    //CModelIdxTreeEntry* pModelBranchTrg = nullptr;

    //if( pModelTreeEntrySelected->isLeave() )
    //{
    //    pModelBranchTrg = pModelTreeEntry->parentBranch();
    //}
    //else
    //{
    //    pModelBranchTrg = pModelTreeEntry;
    //}
    //if( pModelBranchTrg != nullptr )
    //{
    //    CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
    //    //CIdxTreeEntry*      pTreeEntrySrc = pModelTreeEntrySrc->treeEntry();
    //    //CIdxTreeEntry*      pBranchTrg    = pModelBranchTrg->treeEntry();

    //    int idxInTargetBranch = -1;

    //    if( pModelIdxTree->sortOrder() == EIdxTreeSortOrder::Config )
    //    {
    //        idxInTargetBranch = m_modelIdxSelectedForPaste.row();
    //    }
    //    else // if( pModelIdxTree->sortOrder() != EIdxTreeSortOrder::Config )
    //    {
    //        idxInTargetBranch = pTreeEntrySrc->indexInParentBranch();
    //    }

    //    if( m_pasteMode == EPasteMode::Copy )
    //    {
    //        SErrResultInfo errResultInfo = pIdxTree->canCopy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

    //        if( errResultInfo.isErrorResult() )
    //        {
    //            if( !m_bSilentlyIgnoreInvalidCopyRequests )
    //            {
    //                CDlgErr* pDlgErr = new CDlgErr("Error");
    //                pDlgErr->setErrResultInfo(errResultInfo);
    //                pDlgErr->show();
    //            }
    //        }
    //        else // if( !errResultInfo.isErrorResult() )
    //        {
    //            pIdxTree->copy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);
    //        }
    //    }
    //    else if( m_pasteMode == EPasteMode::Cut )
    //    {
    //        SErrResultInfo errResultInfo = pIdxTree->canMove(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

    //        if( errResultInfo.isErrorResult() )
    //        {
    //            if( !m_bSilentlyIgnoreInvalidCopyRequests )
    //            {
    //                CDlgErr* pDlgErr = new CDlgErr("Error");
    //                pDlgErr->setErrResultInfo(errResultInfo);
    //                pDlgErr->show();
    //             }
    //        }
    //        else // if( !errResultInfo.isErrorResult() )
    //        {
    //            pIdxTree->move(pTreeEntrySrc, pBranchTrg);
    //        }
    //    }
    //} // if( pModelBranchTrg != nullptr )
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
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

    clear(m_pModelRootEntry);

    // The root entry will neither be added to the list nor to the map of tree entries.
    m_pModelRootEntry->setIndexInParentBranch(-1);

    delete m_pModelRootEntry;
    m_pModelRootEntry = nullptr;

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
/*! 
*/
void CModelIdxTree::onIdxTreeEntryAdded( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryAdded",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(i_strKeyInTree);
    CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strKeyInTree);

    if( pTreeEntry != nullptr && pModelTreeEntry == nullptr )
    {
        CIdxTreeEntry* pParentBranch = pTreeEntry->parentBranch();
        CModelIdxTreeEntry* pModelParentBranch = m_pModelRootEntry;

        if( pParentBranch != m_pIdxTree->root() )
        {
            pModelParentBranch = findEntry(pParentBranch->keyInTree());

            // Must not happen as on adding a node to the index tree all parent
            // branches are created and for each parent branch the treeEntryAdded
            // signal is emitted and this slot is called created the corresponding
            // model branch entry.
            if( pModelParentBranch == nullptr )
            {
                throw CException(__FILE__, __LINE__, EResultInternalProgramError, "ModelParentBranch == nullptr");
            }
        }

        if( pTreeEntry->isBranch() )
        {
            pModelTreeEntry = new CModelIdxTreeEntry(pTreeEntry);
            pModelTreeEntry->setSortOrder(m_sortOrder);
            pModelTreeEntry->setExcludeLeaves(m_bExcludeLeaves);
        }
        else if( pTreeEntry->isLeave() && !m_bExcludeLeaves )
        {
            pModelTreeEntry = new CModelIdxTreeEntry(pTreeEntry);
        }

        if( pModelTreeEntry != nullptr )
        {
            int idxInParentBranch = pModelParentBranch->add(pModelTreeEntry);

            m_mappModelTreeEntries.insert(i_strKeyInTree, pModelTreeEntry);

            QModelIndex modelIdxParent = _createIndex(idxInParentBranch, 0, pModelParentBranch);

            _beginInsertRows(modelIdxParent, idxInParentBranch, idxInParentBranch);
            _endInsertRows();

            if( pTreeEntry->isBranch() )
            {
                for( int idxEntry = 0; idxEntry < pTreeEntry->size(); ++idxEntry )
                {
                    CIdxTreeEntry* pTreeEntryChild = pTreeEntry->at(idxEntry);
                    onIdxTreeEntryAdded(pTreeEntryChild->keyInTree());
                }
            }
        }
    } // if( pTreeEntry != nullptr && pModelTreeEntry == nullptr )

} // onIdxTreeEntryAdded

//------------------------------------------------------------------------------
/*! 
*/
void CModelIdxTree::onIdxTreeEntryChanged( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strKeyInTree);

    if( pModelTreeEntry != nullptr )
    {
        QModelIndex modelIdxTL = index(i_strKeyInTree, 0);
        QModelIndex modelIdxBR = index(i_strKeyInTree, columnCount()-1);
        emit_dataChanged(modelIdxTL, modelIdxBR);
    }
}

//------------------------------------------------------------------------------
void CModelIdxTree::onIdxTreeEntryAboutToBeRemoved(
    EIdxTreeEntryType i_entryType,
    const QString&    i_strKeyInTree,
    int               i_idxInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Type: " + idxTreeEntryType2Str(i_entryType);
        strMthInArgs += ", Key: " + i_strKeyInTree;
        strMthInArgs += ", Idx: " + QString::number(i_idxInTree);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryAboutToBeRemoved",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strKeyInTree);

    // Please note that the model may not contain each index tree entry as a filter
    // may have been applied.
    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->isBranch() )
        {
            clear(pModelTreeEntry);
        }

        remove(pModelTreeEntry);

        delete pModelTreeEntry;
        pModelTreeEntry = nullptr;
    }

} // onIdxTreeEntryAboutToBeRemoved

//------------------------------------------------------------------------------
/*! 
*/
void CModelIdxTree::onIdxTreeEntryMoved(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree,
    const QString& i_strKeyInTreeOfTargetBranch )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = ", NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKey: " + i_strOrigKeyInTree;
        strMthInArgs += ", TargetBranch: " + i_strKeyInTreeOfTargetBranch;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryMoved",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Please note that before the index tree emits the "treeEntryMoved" signal
    // and this slot is executed the "treeEntryKeyInTreeChanged" signal has been
    // emitted and the slot "onIdxTreeEntryKeyInTreeChanged" was processed for
    // each tree entry for which the key has been changed. When moving nodes thats
    // the case for the moved as well as for all childrens of the moved node.
    // Which means that the "KeyInTreePrev" has already been updated and is no
    // longer part of the models map. But the model node can be found using
    // the new key in tree.
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strOrigKeyInTree);
    CModelIdxTreeEntry* pModelTargetBranch = findEntry(i_strKeyInTreeOfTargetBranch);

    // Please note that the model may not contain each index tree entry as a filter
    // may have been applied.
    if( pModelTreeEntry != nullptr && pModelTargetBranch != nullptr )
    {
        CModelIdxTreeEntry* pModelParentBranchPrev = pModelTreeEntry->parentBranch();

        QModelIndex modelIdxParentPrev = _createIndex(pModelParentBranchPrev->indexInParentBranch(), 0, pModelParentBranchPrev);
        int idxInParentBranchPrev = pModelTreeEntry->indexInParentBranch();
        _beginRemoveRows(modelIdxParentPrev, idxInParentBranchPrev, idxInParentBranchPrev);
        pModelParentBranchPrev->remove(pModelTreeEntry);
        _endRemoveRows();

        int idxInParentBranch = pModelTargetBranch->add(pModelTreeEntry);
        QModelIndex modelIdxTargetBranch = _createIndex(idxInParentBranch, 0, pModelTargetBranch);
        _beginInsertRows(modelIdxTargetBranch, idxInParentBranch, idxInParentBranch);
        _endInsertRows();
    }
} // onIdxTreeEntryMoved

//------------------------------------------------------------------------------
/*! 
*/
void CModelIdxTree::onIdxTreeEntryKeyInTreeChanged(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKey: " + i_strOrigKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIdxTreeEntryKeyInTreeChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strOrigKeyInTree);

    // Please note that the model may not contain each index tree entry as a filter
    // may have been applied.
    if( pModelTreeEntry != nullptr )
    {
        CModelIdxTreeEntry* pModelParentBranch = pModelTreeEntry->parentBranch();

        if( pModelParentBranch != nullptr )
        {
            if( m_mappModelTreeEntries.contains(i_strNewKeyInTree) )
            {
                throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, i_strNewKeyInTree);
            }

            QString strBranchPathPrev;
            QString strNamePrev;
            QString strBranchPath;
            QString strName;

            m_pIdxTree->splitPathStr(i_strOrigKeyInTree, &strBranchPathPrev, &strNamePrev);
            m_pIdxTree->splitPathStr(i_strNewKeyInTree, &strBranchPath, &strName);

            QString strKeyInBranchPrev = pModelTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol) + ":" + strNamePrev;
            QString strKeyInBranch     = pModelTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol) + ":" + strName;

            // The key of entries may be changed for several reasons.
            // - If the name of an entry will be changed.
            //   The node itself and all its children will get a new key.
            //   For each of those nodes this slot is called.
            //   If sorting is enabled (e.g. Ascending instead of Config) the model
            //   node may be repositioned below its parent node.
            //   The index tree will not emit the "treeEntryMoved" signal because
            //   the index tree itself is always in Config order.
            // - If an entry is moved.
            //   The node itself and all its children will get a new key.
            //   For each of those nodes this slot is called.
            //   No matter whether sorting is enabled or not the model
            //   node has to be moved to another parent. But thats been done
            //   later if the index tree emits the "treeEntryMoved" signal.
            //   In the index tree the entry has already been moved and
            //   "indexInParent" branch already returns the position below
            //   the new parent. This method does not move the model entry to
            //   another model node but keeps the node below its current
            //   parent taking the sort order into account when changing
            //   the name of the entry.

            QModelIndex modelIdxParent = _createIndex(pModelParentBranch->indexInParentBranch(), 0, pModelParentBranch);

            int idxInParentBranchPrev = pModelTreeEntry->indexInParentBranch();

            m_mappModelTreeEntries.remove(i_strOrigKeyInTree);

            pModelTreeEntry->setKeyInTree(i_strNewKeyInTree);

            int idxInParentBranchNew = pModelTreeEntry->indexInParentBranch();

            m_mappModelTreeEntries.insert(i_strNewKeyInTree, pModelTreeEntry);

            // Moved below its parent node ...
            if( idxInParentBranchPrev != idxInParentBranchNew )
            {
                _beginMoveRows(modelIdxParent, idxInParentBranchPrev, idxInParentBranchPrev, modelIdxParent, idxInParentBranchNew);
            }
            // Kept position below its parent node ...
            else
            {
                QModelIndex modelIdxTL = _createIndex(idxInParentBranchPrev, 0, pModelTreeEntry);
                QModelIndex modelIdxBR = _createIndex(idxInParentBranchPrev, columnCount()-1, pModelTreeEntry);
                emit_dataChanged(modelIdxTL, modelIdxBR);
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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Branch: " + QString(i_pModelBranch == nullptr ? "nullptr" : i_pModelBranch->keyInTree());
        strMthInArgs += ", DestroyEntries: " + bool2Str(i_bDestroyTreeEntries);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "clear",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( i_pModelBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModelBranch == nullptr");
    }

    for( int idxEntry = i_pModelBranch->count()-1; idxEntry >= 0; --idxEntry )
    {
        CModelIdxTreeEntry* pModelTreeEntry = i_pModelBranch->at(idxEntry);

        if( pModelTreeEntry->isBranch() )
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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pModelTreeEntry == nullptr ? "nullptr" : i_pModelTreeEntry->keyInTree());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
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

    if( i_pModelTreeEntry != m_pModelRootEntry )
    {
        CModelIdxTreeEntry* pModelParentBranch = i_pModelTreeEntry->parentBranch();

        int idxInParentBranch = i_pModelTreeEntry->indexInParentBranch();

        if( pModelParentBranch != nullptr && idxInParentBranch >= 0 )
        {
            QModelIndex modelIdxParent = _createIndex(pModelParentBranch->indexInParentBranch(), 0, pModelParentBranch);

            _beginRemoveRows(modelIdxParent, idxInParentBranch, idxInParentBranch);

            QString strKeyInTree = i_pModelTreeEntry->keyInTree();

            if( !m_mappModelTreeEntries.contains(strKeyInTree) )
            {
                throw CException(__FILE__, __LINE__, EResultObjNotInList, strKeyInTree);
            }
            m_mappModelTreeEntries.remove(strKeyInTree);

            pModelParentBranch->remove(i_pModelTreeEntry);

            _endRemoveRows();

        } // if( pModelParentBranch != nullptr && idxInParentBranch >= 0 )
    } // if( i_pModelTreeEntry != m_pModelRootEntry )

} // remove

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTree::columnWidthByColumn(int i_iClm, int i_iFontPixelSize, const QModelIndex& i_modelIdxParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Clm: " + column2Str(static_cast<EColumn>(i_iClm));
        strMthInArgs += ", FontPixelSize: " + QString::number(i_iFontPixelSize);
        strMthInArgs += ", ModelIdxParent: " + modelIdx2Str(i_modelIdxParent);
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

            if( !i_modelIdxParent.isValid() )
            {
                QString strClmHeader = headerData(i_iClm, Qt::Horizontal).toString();
                iClmWidth = fntMetrics.horizontalAdvance(strClmHeader);
            }

            for( int iRowIdx = 0; iRowIdx < rowCount(i_modelIdxParent); ++iRowIdx )
            {
                QModelIndex modelIdxParent = index(iRowIdx, i_iClm, i_modelIdxParent);
                if( rowCount(modelIdxParent) > 0 )
                {
                    iClmWidth = qMax(iClmWidth, columnWidthByColumn(i_iClm, i_iFontPixelSize, modelIdxParent));
                }

                QString strType = "string";
                QVariant varData = data(index(iRowIdx, i_iClm, i_modelIdxParent), static_cast<int>(ERole::Type));
                if( varData.canConvert(QVariant::String) )
                {
                    strType = varData.toString();
                }
                if( strType == "string" || strType == "int" )
                {
                    varData = data(index(iRowIdx, i_iClm, i_modelIdxParent), Qt::DisplayRole);
                    if( varData.canConvert(QVariant::String) )
                    {
                        QString strCellData = varData.toString();
                        iClmWidth = qMax(iClmWidth, fntMetrics.horizontalAdvance(strCellData));
                    }
                }
                else if( strType == "imageUrl" || strType == "icon" )
                {
                    varData = data(index(iRowIdx, i_iClm, i_modelIdxParent), Qt::DisplayRole);
                    if( varData.canConvert(QVariant::String) )
                    {
                        QString strCellData = varData.toString();
                        QPixmap pixmap(strCellData);
                        iClmWidth = qMax(iClmWidth, pixmap.width());
                    }
                }
            }

            if( !i_modelIdxParent.isValid() )
            {
                //m_ariClmWidths[i_iClm] = iClmWidth + 2*iClmSpacing;
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
int CModelIdxTree::columnWidthByRole(const QByteArray& i_byteArrRole, int i_iFontPixelSize, const QModelIndex& i_modelIdxParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Role: " + i_byteArrRole;
        strMthInArgs += ", FontPixelSize: " + QString::number(i_iFontPixelSize);
        strMthInArgs += ", ModelIdxParent: " + modelIdx2Str(i_modelIdxParent, byteArr2Role(i_byteArrRole));
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
        iClmWidth = columnWidthByColumn(static_cast<int>(clm), i_iFontPixelSize, i_modelIdxParent);
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
QHash<int, QByteArray> CModelIdxTree::roleNames() const
//------------------------------------------------------------------------------
{
    return s_roleNames;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTree::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdxParent {" + modelIdx2Str(i_modelIdxParent, 0) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rowCount",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    int iRowCount = 1;

    CModelIdxTreeEntry* pModelParentTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdxParent.internalPointer());

    if( pModelParentTreeEntry != nullptr )
    {
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
    }

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(iRowCount);
    }
    #endif

    return iRowCount;
}

//------------------------------------------------------------------------------
int CModelIdxTree::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(EColumnCount);
    }
    #endif

    return EColumnCount;
}

//------------------------------------------------------------------------------
QModelIndex CModelIdxTree::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + QString::number(i_iCol);
        strMthInArgs += ", ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "index",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QModelIndex modelIdx;

    if( hasIndex(i_iRow,i_iCol,i_modelIdxParent) )
    {
        CModelIdxTreeEntry* pModelTreeEntry = nullptr;

        if( !i_modelIdxParent.isValid() )
        {
            pModelTreeEntry = m_pModelRootEntry;
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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(modelIdx2Str(modelIdx));
    }
    #endif

    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelIdxTree::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "parent",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QModelIndex modelIdxParent;

    if( i_modelIdx.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelTreeEntry == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultInternalProgramError);
        }

        CModelIdxTreeEntry* pModelParentBranch = pModelTreeEntry->parentBranch();

        if( pModelParentBranch != nullptr )
        {
            modelIdxParent = _createIndex( pModelParentBranch->indexInParentBranch(), 0, pModelParentBranch);
        }
    } // if( i_modelIdx.isValid() )

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(modelIdx2Str(modelIdxParent));
    }
    #endif

    return modelIdxParent;
}

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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
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
            case EColumnTreeEntryNameDecorated:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Decorated Name";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnTreeEntryTypeImageUrl:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Type Image Url";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnTreeEntryTypeIcon:
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
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // headerData

//------------------------------------------------------------------------------
Qt::ItemFlags CModelIdxTree::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "flags",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

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
                if( pModelTreeEntry != m_pModelRootEntry )
                {
                    uFlags = uFlags | Qt::ItemIsEditable;
                }
                uFlags = uFlags | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
                break;
            }
            case EColumnTreeEntryNameDecorated:
            {
                break;
            }
            case EColumnTreeEntryTypeImageUrl:
            {
                break;
            }
            case EColumnTreeEntryTypeIcon:
            {
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
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }
    #endif

    return uFlags;
}

//------------------------------------------------------------------------------
Qt::DropActions CModelIdxTree::supportedDropActions() const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "supportedDropActions",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    Qt::DropActions dropActions = Qt::CopyAction | Qt::MoveAction;

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(qDropActions2Str(dropActions));
    }
    #endif

    return dropActions;
}

//------------------------------------------------------------------------------
QVariant CModelIdxTree::data( const QModelIndex& i_modelIdx, int i_iRole ) const
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
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "ModelIdx {" + modelIdx2Str(i_modelIdx, i_iRole) + "}";
        strMthInArgs += ", Role: " + QString::number(i_iRole) + " (" + role2Str(i_iRole) + ")";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
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

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr && s_rolesHandled.contains(iRole) )
    {
        CModelIdxTreeEntry* pModelBranch = nullptr;

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            pModelBranch = pModelTreeEntry;
        }

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
                        if( pModelBranch != nullptr )
                        {
                            strData += "\nExpanded: " + bool2Str(pModelBranch->isExpanded());
                        }
                        varData = strData;
                    }
                }
                else if( iRole == Qt::DecorationRole )
                {
                    varData = getIcon(pModelTreeEntry->entryType());
                }
                break;
            }
            case EColumnTreeEntryNameDecorated:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("imageUrl");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::DecorationRole )
                {
                    QString strUrl = getImageUrl(pModelTreeEntry->entryType());
                    if( strUrl.startsWith(":") ) {
                        strUrl.insert(0, "qrc");
                    }
                    else if( !strUrl.startsWith("qrc:") ) {
                        strUrl.insert(0, "qrc:");
                    }
                    varData = strUrl;
                }
                break;
            }
            case EColumnTreeEntryTypeImageUrl:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("imageUrl");
                }
                else if( iRole == Qt::DisplayRole || iRole == Qt::DecorationRole )
                {
                    QString strUrl = getImageUrl(pModelTreeEntry->entryType());
                    if( strUrl.startsWith(":") ) {
                        strUrl.insert(0, "qrc");
                    }
                    else if( !strUrl.startsWith("qrc:") ) {
                        strUrl.insert(0, "qrc:");
                    }
                    varData = strUrl;
                }
                break;
            }
            case EColumnTreeEntryTypeIcon:
            {
                if( iRole == static_cast<int>(ERole::Type) )
                {
                    varData = QLatin1String("icon");
                }
                else if( iRole == Qt::DisplayRole )
                {
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = getIcon(pModelTreeEntry->entryType());
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
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // data

//------------------------------------------------------------------------------
bool CModelIdxTree::setData(
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
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "ModelIdx {" + modelIdx2Str(i_modelIdx, i_iRole) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + QString::number(i_iRole) + " (" + role2Str(i_iRole) + ")";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    bool bOk = false;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr )
    {
        switch( clm )
        {
            case EColumnTreeEntryName:
            {
                if( iRole == Qt::EditRole )
                {
                    if( pModelTreeEntry != m_pModelRootEntry )
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
                    } // if( pModelTreeEntry != m_pModelRootEntry )
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
            case EColumnTreeEntryNameDecorated:
            {
                break;
            }
            case EColumnTreeEntryTypeImageUrl:
            {
                break;
            }
            case EColumnTreeEntryTypeIcon:
            {
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
        emit_dataChanged(i_modelIdx, i_modelIdx);
    }

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(bool2Str(bOk));
    }
    #endif

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
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ModelIdxs [" + QString::number(i_arModelIdxs.count()) + "]";
        if( i_arModelIdxs.count() > 0 )
        {
            strMthInArgs += "(";
            for( int idx = 0; idx < i_arModelIdxs.count(); ++idx )
            {
                if( idx > 0 ) strMthInArgs += ", ";
                strMthInArgs += "[" + QString::number(idx) + ": ";
                strMthInArgs += "{" + modelIdx2Str(i_arModelIdxs[idx]) + "}]";
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "mimeData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iAddTrcInfoDetails = m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ? 1 : 0;
        mthTracer.setMethodReturn(qMimeData2Str(pMimeData, iAddTrcInfoDetails));
    }
    #endif

    return pMimeData;
}

//------------------------------------------------------------------------------
bool CModelIdxTree::canDropMimeData(
    const QMimeData*   i_pMimeData,
    Qt::DropAction     i_dropAction,
    int                i_iRow,
    int                i_iClm,
    const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "MimeData {" + qMimeData2Str(i_pMimeData) + "}";
        strMthInArgs += ", DropAction: " + qDropActions2Str(i_dropAction);
        strMthInArgs += ", Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + QString::number(i_iClm);
        strMthInArgs += ", ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canDropMimeData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

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

        CModelIdxTreeEntry* pModelBranchParent = static_cast<CModelIdxTreeEntry*>(i_modelIdxParent.internalPointer());

        if( pModelBranchParent != nullptr )
        {
            pBranchParent = pModelBranchParent->getIdxTreeEntry();
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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(bool2Str(bCanDrop));
    }
    #endif

    return bCanDrop;
}

//------------------------------------------------------------------------------
bool CModelIdxTree::dropMimeData(
    const QMimeData*   i_pMimeData,
    Qt::DropAction     i_dropAction,
    int                i_iRow,
    int                i_iClm,
    const QModelIndex& i_modelIdxParent )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "MimeData {" + qMimeData2Str(i_pMimeData) + "}";
        strMthInArgs += ", DropAction: " + qDropActions2Str(i_dropAction);
        strMthInArgs += ", Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + QString::number(i_iClm);
        strMthInArgs += ", ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dropMimeData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

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

        CModelIdxTreeEntry* pModelBranchParent = static_cast<CModelIdxTreeEntry*>(i_modelIdxParent.internalPointer());

        CIdxTreeEntry* pBranchParent = pModelBranchParent->getIdxTreeEntry();

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

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(bool2Str(bDropped));
    }
    #endif

    return bDropped;
}

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
    EColumn clm = static_cast<EColumn>(i_modelIdx.column());
    int iRole = i_iRole;

    if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole) )
    {
        clm = role2Column(i_iRole);
        iRole = Qt::DisplayRole;
    }

    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "ModelIdx {" + modelIdx2Str(i_modelIdx, i_iRole) + "}";
        strMthInArgs += ", Data: " + i_varData.toString();
        strMthInArgs += ", Role: " + QString::number(i_iRole) + " (" + role2Str(i_iRole) + ")";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canSetData",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    CIdxTreeLocker idxTreeLocker(m_pIdxTree);

    QString strMth = "setData";

    SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultInvalidMethodCall, "Value cannot be changed");

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr )
    {
        switch( clm )
        {
            case EColumnTreeEntryName:
            {
                if( iRole == Qt::EditRole )
                {
                    if( pModelTreeEntry != m_pModelRootEntry )
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
            case EColumnTreeEntryNameDecorated:
            {
                break;
            }
            case EColumnTreeEntryTypeImageUrl:
            {
                break;
            }
            case EColumnTreeEntryTypeIcon:
            {
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
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iAddTrcInfoDetails = m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal ? 1 : 0;
        mthTracer.setMethodReturn(errResultInfo.toString(iAddTrcInfoDetails));
    }
    #endif

    return errResultInfo;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel (just overwritten to trace the method calls for debugging purposes)
==============================================================================*/

////------------------------------------------------------------------------------
//QModelIndex CModelIdxTree::sibling( int i_iRow, int i_iCol, const QModelIndex& i_modelIdx ) const
////------------------------------------------------------------------------------
//{
//    #ifdef ZS_TRACE_GUI_MODELS
//    QString strMthInArgs;
//    QString strMthRet;
//    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthInArgs  = "Row: " + QString::number(i_iRow);
//        strMthInArgs += ", Col: " + QString::number(i_iCol);
//        strMthInArgs += ", ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
//        /* pTrcServer         */ CTrcServer::GetInstance(),
//        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "sibling",
//        /* strMethodInArgs    */ strMthInArgs );
//    #endif
//
//    QModelIndex modelIdxSibling = QAbstractItemModel::sibling(i_iRow, i_iCol, i_modelIdx);
//
//    #ifdef ZS_TRACE_GUI_MODELS
//    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthRet = modelIdx2Str(modelIdxSibling);
//        mthTracer.setMethodReturn(strMthRet);
//    }
//    #endif
//
//    return modelIdxSibling;
//}
//
////------------------------------------------------------------------------------
//bool CModelIdxTree::hasChildren( const QModelIndex& i_modelIdxParent ) const
////------------------------------------------------------------------------------
//{
//    #ifdef ZS_TRACE_GUI_MODELS
//    QString strMthInArgs;
//    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthInArgs = "Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
//        /* pTrcServer         */ CTrcServer::GetInstance(),
//        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "hasChildren",
//        /* strMethodInArgs    */ strMthInArgs );
//    #endif
//
//    bool bHas = QAbstractItemModel::hasChildren(i_modelIdxParent);
//
//    #ifdef ZS_TRACE_GUI_MODELS
//    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        mthTracer.setMethodReturn(bHas);
//    }
//    #endif
//
//    return bHas;
//}
//
////------------------------------------------------------------------------------
//bool CModelIdxTree::insertRows( int i_iRow, int i_iRowCount, const QModelIndex& i_modelIdxParent )
////------------------------------------------------------------------------------
//{
//    #ifdef ZS_TRACE_GUI_MODELS
//    QString strMthInArgs;
//    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthInArgs  = "Row: " + QString::number(i_iRow);
//        strMthInArgs += ", Count: " + QString::number(i_iRowCount);
//        strMthInArgs += ", Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
//        /* pTrcServer         */ CTrcServer::GetInstance(),
//        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "insertRows",
//        /* strMethodInArgs    */ strMthInArgs );
//    #endif
//
//    bool bOk = QAbstractItemModel::insertRows(i_iRow, i_iRowCount, i_modelIdxParent);
//
//    #ifdef ZS_TRACE_GUI_MODELS
//    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        mthTracer.setMethodReturn(bOk);
//    }
//    #endif
//
//    return bOk;
//}
//
////------------------------------------------------------------------------------
//bool CModelIdxTree::insertColumns( int i_iCol, int i_iColCount, const QModelIndex& i_modelIdxParent )
////------------------------------------------------------------------------------
//{
//    #ifdef ZS_TRACE_GUI_MODELS
//    QString strMthInArgs;
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthInArgs  = "Col: " + QString::number(i_iCol);
//        strMthInArgs += ", Count: " + QString::number(i_iColCount);
//        strMthInArgs += ", Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObj,
//        /* pTrcServer         */ CTrcServer::GetInstance(),
//        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "insertColumns",
//        /* strMethodInArgs    */ strMthInArgs );
//    #endif
//
//    bool bOk = QAbstractItemModel::insertColumns(i_iCol, i_iColCount, i_modelIdxParent);
//
//    #ifdef ZS_TRACE_GUI_MODELS
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        mthTracer.setMethodReturn(bOk);
//    }
//    #endif
//
//    return bOk;
//}
//
////------------------------------------------------------------------------------
//bool CModelIdxTree::removeRows( int i_iRow, int i_iRowCount, const QModelIndex& i_modelIdxParent )
////------------------------------------------------------------------------------
//{
//    #ifdef ZS_TRACE_GUI_MODELS
//    QString strMthInArgs;
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthInArgs  = "Row: " + QString::number(i_iRow);
//        strMthInArgs += ", Count: " + QString::number(i_iRowCount);
//        strMthInArgs += ", Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObj,
//        /* pTrcServer         */ CTrcServer::GetInstance(),
//        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "removeRows",
//        /* strMethodInArgs    */ strMthInArgs );
//    #endif
//
//    bool bOk = QAbstractItemModel::removeRows(i_iRow, i_iRowCount, i_modelIdxParent);
//
//    #ifdef ZS_TRACE_GUI_MODELS
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        mthTracer.setMethodReturn(bOk);
//    }
//    #endif
//
//    return bOk;
//}
//
////------------------------------------------------------------------------------
//bool CModelIdxTree::removeColumns( int i_iCol, int i_iColCount, const QModelIndex& i_modelIdxParent )
////------------------------------------------------------------------------------
//{
//    #ifdef ZS_TRACE_GUI_MODELS
//    QString strMthInArgs;
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthInArgs  = "Col: " + QString::number(i_iCol);
//        strMthInArgs += ", Count: " + QString::number(i_iColCount);
//        strMthInArgs += ", Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObj,
//        /* pTrcServer         */ CTrcServer::GetInstance(),
//        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "removeColumns",
//        /* strMethodInArgs    */ strMthInArgs );
//    #endif
//
//    bool bOk = QAbstractItemModel::removeColumns(i_iCol, i_iColCount, i_modelIdxParent);
//
//    #ifdef ZS_TRACE_GUI_MODELS
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        mthTracer.setMethodReturn(bOk);
//    }
//    #endif
//
//    return bOk;
//}
//
////------------------------------------------------------------------------------
//bool CModelIdxTree::moveRows(
//    const QModelIndex& i_modelIdxSourceParent,
//    int                i_iRowSource,
//    int                i_iRowCount,
//    const QModelIndex& i_modelIdxDestParent,
//    int                i_iRowDestChild )
////------------------------------------------------------------------------------
//{
//    #ifdef ZS_TRACE_GUI_MODELS
//    QString strMthInArgs;
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthInArgs  = "SourceParent {" + modelIdx2Str(i_modelIdxSourceParent) + "}";
//        strMthInArgs += ", RowSource: " + QString::number(i_iRowSource);
//        strMthInArgs += ", Count: " + QString::number(i_iRowCount);
//        strMthInArgs += ", DestParent {" + modelIdx2Str(i_modelIdxDestParent) + "}";
//        strMthInArgs += ", DestChild: " + QString::number(i_iRowDestChild);
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObj,
//        /* pTrcServer         */ CTrcServer::GetInstance(),
//        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "moveRows",
//        /* strMethodInArgs    */ strMthInArgs );
//    #endif
//
//    bool bOk = QAbstractItemModel::moveRows(i_modelIdxSourceParent, i_iRowSource, i_iRowCount, i_modelIdxDestParent, i_iRowDestChild);
//
//    #ifdef ZS_TRACE_GUI_MODELS
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        mthTracer.setMethodReturn(bOk);
//    }
//    #endif
//
//    return bOk;
//}
//
////------------------------------------------------------------------------------
//bool CModelIdxTree::moveColumns(
//    const QModelIndex& i_modelIdxSourceParent,
//    int                i_iColSource,
//    int                i_iColCount,
//    const QModelIndex& i_modelIdxDestParent,
//    int                i_iColDestChild )
////------------------------------------------------------------------------------
//{
//    #ifdef ZS_TRACE_GUI_MODELS
//    QString strMthInArgs;
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        strMthInArgs  = "SourceParent {" + modelIdx2Str(i_modelIdxSourceParent) + "}";
//        strMthInArgs += ", ColSource: " + QString::number(i_iColSource);
//        strMthInArgs += ", Count: " + QString::number(i_iColCount);
//        strMthInArgs += ", DestParent {" + modelIdx2Str(i_modelIdxDestParent) + "}";
//        strMthInArgs += ", DestChild: " + QString::number(i_iColDestChild);
//    }
//    CMethodTracer mthTracer(
//        /* pTrcAdminObj       */ m_pTrcAdminObj,
//        /* pTrcServer         */ CTrcServer::GetInstance(),
//        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
//        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strNameSpace       */ NameSpace(),
//        /* strClassName       */ ClassName(),
//        /* strObjName         */ objectName(),
//        /* strMethod          */ "moveColumns",
//        /* strMethodInArgs    */ strMthInArgs );
//    #endif
//
//    bool bOk = QAbstractItemModel::moveColumns(i_modelIdxSourceParent, i_iColSource, i_iColCount, i_modelIdxDestParent, i_iColDestChild);
//
//    #ifdef ZS_TRACE_GUI_MODELS
//    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
//    {
//        mthTracer.setMethodReturn(bOk);
//    }
//    #endif
//
//    return bOk;
//}

/*==============================================================================
protected: // to trace emitting signals for debugging purposes
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::emit_dataChanged( const QModelIndex& i_modelIdxTL, const QModelIndex& i_modelIdxBR, const QVector<int>& i_ariRoles )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TL {" + modelIdx2Str(i_modelIdxTL) + "}";
        strMthInArgs += ", BR {" + modelIdx2Str(i_modelIdxBR) + "}";
        strMthInArgs += ", Roles [" + QString::number(i_ariRoles.size()) + "]";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_dataChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    emit dataChanged(i_modelIdxTL, i_modelIdxBR, i_ariRoles);
}

//------------------------------------------------------------------------------
void CModelIdxTree::emit_headerDataChanged( Qt::Orientation i_orientation, int i_iFirstSection, int i_iLastSection )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", First: " + QString::number(i_iFirstSection);
        strMthInArgs += ", Last: " + QString::number(i_iLastSection);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_headerDataChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    emit headerDataChanged(i_orientation, i_iFirstSection, i_iLastSection);
}

//------------------------------------------------------------------------------
void CModelIdxTree::emit_layoutChanged( const QList<QPersistentModelIndex>& i_arModelIdxsParents, QAbstractItemModel::LayoutChangeHint i_hint )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "ModelIdxsParents [" + QString::number(i_arModelIdxsParents.size()) + "]";
        strMthInArgs += ", Hint: " + QString::number(i_hint);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_layoutChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    emit layoutChanged(i_arModelIdxsParents, i_hint);
}

//------------------------------------------------------------------------------
void CModelIdxTree::emit_layoutAboutToBeChanged( const QList<QPersistentModelIndex>& i_arModelIdxsParents, QAbstractItemModel::LayoutChangeHint i_hint )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "ModelIdxsParents [" + QString::number(i_arModelIdxsParents.size()) + "]";
        strMthInArgs += ", Hint: " + QString::number(i_hint);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_layoutAboutToBeChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    emit layoutAboutToBeChanged(i_arModelIdxsParents, i_hint);
}

//------------------------------------------------------------------------------
void CModelIdxTree::emit_sortOrderChanged(const QString& i_strSortOrder)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strSortOrder;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_sortOrderChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    emit sortOrderChanged(i_strSortOrder);
}

//------------------------------------------------------------------------------
void CModelIdxTree::emit_sortOrderChanged(EIdxTreeSortOrder i_sortOrder)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_sortOrderChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    emit sortOrderChanged(i_sortOrder);
}

//------------------------------------------------------------------------------
void CModelIdxTree::emit_excludeLeavesChanged(bool i_bExcludeLeaves)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bExcludeLeaves);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_excludeLeavesChanged",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    emit excludeLeavesChanged(i_bExcludeLeaves);
}

/*==============================================================================
protected: // reimplemented to trace emitting signals for debugging purposes
==============================================================================*/

//------------------------------------------------------------------------------
inline QModelIndex CModelIdxTree::_createIndex( int i_iRow, int i_iCol, void* i_pvData ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    QString strMthRet;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Col: " + QString::number(i_iCol);
        strMthInArgs += ", Data: " + pointer2Str(i_pvData);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createIndex",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QModelIndex modelIdx = QAbstractItemModel::createIndex(i_iRow, i_iCol, i_pvData);

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthRet = modelIdx2Str(modelIdx);
        mthTracer.setMethodReturn(strMthRet);
    }
    #endif

    return modelIdx;
}

//------------------------------------------------------------------------------
inline QModelIndex CModelIdxTree::_createIndex( int i_iRow, int i_iCol, quintptr i_uId ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    QString strMthRet;
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Col: " + QString::number(i_iCol);
        strMthInArgs += ", Id: " + QString::number(i_uId);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevelNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createIndex",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QModelIndex modelIdx = QAbstractItemModel::createIndex(i_iRow, i_iCol, i_uId);

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevelNoisyMethods >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthRet = modelIdx2Str(modelIdx);
        mthTracer.setMethodReturn(strMthRet);
    }
    #endif

    return modelIdx;
}

//------------------------------------------------------------------------------
void CModelIdxTree::_beginInsertRows( const QModelIndex& i_modelIdxParent, int i_iFirstRow, int i_iLastRow )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
        strMthInArgs += ", First: " + QString::number(i_iFirstRow);
        strMthInArgs += ", Last: " + QString::number(i_iLastRow);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginInsertRows",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::beginInsertRows(i_modelIdxParent, i_iFirstRow, i_iLastRow);
}

//------------------------------------------------------------------------------
void CModelIdxTree::_endInsertRows()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endInsertRows",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::endInsertRows();
}

//------------------------------------------------------------------------------
void CModelIdxTree::_beginRemoveRows( const QModelIndex &i_modelIdxParent, int i_iFirstRow, int i_iLastRow )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
        strMthInArgs += ", First: " + QString::number(i_iFirstRow);
        strMthInArgs += ", Last: " + QString::number(i_iLastRow);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginRemoveRows",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::beginRemoveRows(i_modelIdxParent, i_iFirstRow, i_iLastRow);
}

//------------------------------------------------------------------------------
void CModelIdxTree::_endRemoveRows()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endRemoveRows",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::endRemoveRows();
}

//------------------------------------------------------------------------------
bool CModelIdxTree::_beginMoveRows( const QModelIndex& i_modelIdxSourceParent, int i_iRowSourceStart, int i_iRowSourceEnd, const QModelIndex& i_modelIdxDestParent, int i_iRowDest )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SourceParent {" + modelIdx2Str(i_modelIdxSourceParent) + "}";
        strMthInArgs += ", SourceStart: " + QString::number(i_iRowSourceStart);
        strMthInArgs += ", SourceEnd: " + QString::number(i_iRowSourceEnd);
        strMthInArgs += ", DestParent {" + modelIdx2Str(i_modelIdxDestParent) + "}";
        strMthInArgs += ", DestChild: " + QString::number(i_iRowDest);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginMoveRows",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    bool bOk = QAbstractItemModel::beginMoveRows(i_modelIdxSourceParent, i_iRowSourceStart, i_iRowSourceEnd, i_modelIdxDestParent, i_iRowDest);

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
void CModelIdxTree::_endMoveRows()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endMoveRows",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::endMoveRows();
}

//------------------------------------------------------------------------------
void CModelIdxTree::_beginInsertColumns( const QModelIndex& i_modelIdxParent, int i_iFirstCol, int i_iLastCol )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
        strMthInArgs += ", First: " + QString::number(i_iFirstCol);
        strMthInArgs += ", Last: " + QString::number(i_iLastCol);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginInsertColumns",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::beginInsertColumns(i_modelIdxParent, i_iFirstCol, i_iLastCol);
}

//------------------------------------------------------------------------------
void CModelIdxTree::_endInsertColumns()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endInsertColumns",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::endInsertColumns();
}

//------------------------------------------------------------------------------
void CModelIdxTree::_beginRemoveColumns( const QModelIndex& i_modelIdxParent, int i_iFirstCol, int i_iLastCol )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Parent {" + modelIdx2Str(i_modelIdxParent) + "}";
        strMthInArgs += ", First: " + QString::number(i_iFirstCol);
        strMthInArgs += ", Last: " + QString::number(i_iLastCol);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginRemoveColumns",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::beginRemoveColumns(i_modelIdxParent, i_iFirstCol, i_iLastCol);
}

//------------------------------------------------------------------------------
void CModelIdxTree::_endRemoveColumns()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endRemoveColumns",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::endRemoveColumns();
}

//------------------------------------------------------------------------------
bool CModelIdxTree::_beginMoveColumns( const QModelIndex& i_modelIdxSourceParent, int i_iColSourceStart, int i_iColSourceEnd, const QModelIndex& i_modelIdxDestParent, int i_iColDest )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SourceParent {" + modelIdx2Str(i_modelIdxSourceParent) + "}";
        strMthInArgs += ", SourceStart: " + QString::number(i_iColSourceStart);
        strMthInArgs += ", SourceEnd: " + QString::number(i_iColSourceEnd);
        strMthInArgs += ", DestParent {" + modelIdx2Str(i_modelIdxDestParent) + "}";
        strMthInArgs += ", DestChild: " + QString::number(i_iColDest);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginMoveColumns",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    bool bOk = QAbstractItemModel::beginMoveColumns(i_modelIdxSourceParent, i_iColSourceStart, i_iColSourceEnd, i_modelIdxDestParent, i_iColDest);

    #ifdef ZS_TRACE_GUI_MODELS
    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
void CModelIdxTree::_endMoveColumns()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endMoveColumns",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::endMoveColumns();
}

//------------------------------------------------------------------------------
void CModelIdxTree::_beginResetModel()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "beginResetModel",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::beginResetModel();
}

//------------------------------------------------------------------------------
void CModelIdxTree::_endResetModel()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "endResetModel",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    QAbstractItemModel::endResetModel();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::onTrcAdminObjChanged( QObject* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(i_pTrcAdminObj);

    if( pTrcAdminObj != nullptr && m_pTrcAdminObj == pTrcAdminObj )
    {
        m_eTrcDetailLevel = pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    #endif
}

//------------------------------------------------------------------------------
void CModelIdxTree::onTrcAdminObjNoisyMethodsChanged( QObject* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(i_pTrcAdminObj);

    if( pTrcAdminObj != nullptr && m_pTrcAdminObjNoisyMethods == pTrcAdminObj )
    {
        m_eTrcDetailLevelNoisyMethods = pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    #endif
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTree::fillRoleNames()
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
