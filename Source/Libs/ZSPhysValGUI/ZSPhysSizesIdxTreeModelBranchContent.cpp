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

#include "ZSPhysValGUI/ZSPhysSizesIdxTreeModelBranchContent.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
//#include "ZSSys/ZSSysException.h"
//#include "ZSSys/ZSSysMutex.h"

#include <QtCore/qset.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qfontmetrics.h>
#include <QtQml/qqmlapplicationengine.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/******************************************************************************/
class CInitModulePhysValGUIIdxTreeModelBranchContent
/* Please note:
   The class name should be unique for the whole system. Otherwise the compiler
   may be confused and using a CInitModule class from other modules to create
   the static InitModule instance.
*******************************************************************************/
{
public: // ctor
    CInitModulePhysValGUIIdxTreeModelBranchContent()
    {
        qmlRegisterType<CModelIdxTreePhysSizesBranchContent>("ZSPhysValGUI", 1, 0, "ModelIdxTreePhysSizesBranchContent");
    }
};

static CInitModulePhysValGUIIdxTreeModelBranchContent s_initModule;


/*******************************************************************************
class CModelIdxTreePhysSizesBranchContent : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

QHash<int, QByteArray> CModelIdxTreePhysSizesBranchContent::s_roleNames;
QHash<QByteArray, int> CModelIdxTreePhysSizesBranchContent::s_roleValues;

QHash<int, QByteArray> CModelIdxTreePhysSizesBranchContent::s_clm2Name {
    /* 0 */ { CModelIdxTreePhysSizesBranchContent::EColumnTreeEntryName, "TreeEntryName"},
    /* 1 */ { CModelIdxTreePhysSizesBranchContent::EColumnTreeEntryType, "TreeEntryType"},
    /* 2 */ { CModelIdxTreePhysSizesBranchContent::EColumnInternalId, "InternalId"},
    /* 3 */ { CModelIdxTreePhysSizesBranchContent::EColumnIdxInTree, "IdxInTree"},
    /* 4 */ { CModelIdxTreePhysSizesBranchContent::EColumnIdxInParentBranch, "IdxInParentBranch"},
    /* 5 */ { CModelIdxTreePhysSizesBranchContent::EColumnKeyInTree, "KeyInTree"},
    /* 6 */ { CModelIdxTreePhysSizesBranchContent::EColumnKeyInParentBranch, "KeyInParentBranch"}
};

//------------------------------------------------------------------------------
QString CModelIdxTreePhysSizesBranchContent::column2Str(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return s_clm2Name.value(i_clm, "? (" + QByteArray::number(i_clm) + ")");
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreePhysSizesBranchContent::EColumn CModelIdxTreePhysSizesBranchContent::role2Column(int i_iRole)
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
QString CModelIdxTreePhysSizesBranchContent::role2Str(int i_iRole)
//------------------------------------------------------------------------------
{
    return s_roleNames.value(i_iRole, "? (" + QByteArray::number(i_iRole) + ")");
}

//------------------------------------------------------------------------------
int CModelIdxTreePhysSizesBranchContent::byteArr2Role(const QByteArray& i_byteArrRole)
//------------------------------------------------------------------------------
{
    return s_roleValues.value(i_byteArrRole, Qt::DisplayRole);
}

//------------------------------------------------------------------------------
int CModelIdxTreePhysSizesBranchContent::column2Role(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return static_cast<int>(ERole::FirstDataColumnRole) + i_clm;
}

//------------------------------------------------------------------------------
QString CModelIdxTreePhysSizesBranchContent::modelIdx2Str(
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
        if( i_iRole >= static_cast<int>(CModelIdxTreePhysSizesBranchContent::ERole::FirstDataColumnRole) ) {
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
CModelIdxTreePhysSizesBranchContent::CModelIdxTreePhysSizesBranchContent( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelIdxTreePhysSizesBranchContent(nullptr, i_pObjParent)
{
} // ctor

//------------------------------------------------------------------------------
CModelIdxTreePhysSizesBranchContent::CModelIdxTreePhysSizesBranchContent(
    CIdxTreePhysSizes* i_pIdxTree, QObject*  i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pIdxTree(nullptr),
    m_mappModelTreeEntries(),
    m_strKeyInTreeOfRootEntry(),
    m_pModelRootEntry(nullptr),
    m_ariClmWidths(QVector<int>(EColumnCount))
{
    fillRoleNames();

    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }

} // ctor

//------------------------------------------------------------------------------
CModelIdxTreePhysSizesBranchContent::~CModelIdxTreePhysSizesBranchContent()
//------------------------------------------------------------------------------
{
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
void CModelIdxTreePhysSizesBranchContent::setIdxTree( QObject* i_pIdxTree )
//------------------------------------------------------------------------------
{
    if( m_pIdxTree != i_pIdxTree )
    {
        if( m_pIdxTree != nullptr )
        {
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeAboutToBeDestroyed);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryAdded,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryAdded);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryChanged,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryChanged);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryAboutToBeRemoved);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryMoved,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryMoved);
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::treeEntryKeyInTreeChanged,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryKeyInTreeChanged);
        }

        m_pIdxTree = dynamic_cast<CIdxTreePhysSizes*>(i_pIdxTree);

        if( m_pIdxTree == nullptr )
        {
            setObjectName("IdxTree");
        }
        else
        {
            setObjectName(m_pIdxTree->objectName());

            QObject::connect(
                m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeAboutToBeDestroyed);
            QObject::connect(
                m_pIdxTree, &CIdxTree::treeEntryAdded,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryAdded);
            QObject::connect(
                m_pIdxTree, &CIdxTree::treeEntryChanged,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryChanged);
            QObject::connect(
                m_pIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryAboutToBeRemoved);
            QObject::connect(
                m_pIdxTree, &CIdxTree::treeEntryMoved,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryMoved);
            QObject::connect(
                m_pIdxTree, &CIdxTree::treeEntryKeyInTreeChanged,
                this, &CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryKeyInTreeChanged);
        }

        setKeyInTreeOfRootEntry("");

        emit idxTreeChanged(m_pIdxTree);

    } // if( m_pIdxTree != i_pIdxTree )
} // setIdxTree

//------------------------------------------------------------------------------
/*! Returns the pointer to the index tree (which might be null).

    @param Pointer to index tree. The type is of QObject so that it can also
           be accessed by QML.

    @note If you access the index tree and its entries and the index tree is
          modified by different threads you must lock and unlock the index tree.
*/
QObject* CModelIdxTreePhysSizesBranchContent::idxTree()
//------------------------------------------------------------------------------
{
    return m_pIdxTree;
}

//------------------------------------------------------------------------------
void CModelIdxTreePhysSizesBranchContent::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
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
QString CModelIdxTreePhysSizesBranchContent::getKeyInTreeOfRootEntry() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTreeOfRootEntry;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTreePhysSizesBranchContent::findEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    return m_mappModelTreeEntries.value(i_strKeyInTree, nullptr);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QModelIndex CModelIdxTreePhysSizesBranchContent::index( const QString& i_strKeyInTree, int i_iClm ) const
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
void CModelIdxTreePhysSizesBranchContent::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
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
void CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryAdded( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
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
void CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryChanged( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
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
void CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryAboutToBeRemoved(
    EIdxTreeEntryType i_entryType,
    const QString&    i_strKeyInTree,
    int               i_idxInTree )
//------------------------------------------------------------------------------
{
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
void CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryMoved(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree,
    const QString& i_strKeyInTreeOfTargetBranch )
//------------------------------------------------------------------------------
{
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
void CModelIdxTreePhysSizesBranchContent::onIdxTreeEntryKeyInTreeChanged(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree )
//------------------------------------------------------------------------------
{
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
void CModelIdxTreePhysSizesBranchContent::clear()
//------------------------------------------------------------------------------
{
    for( int idxEntry = m_pModelRootEntry->count()-1; idxEntry >= 0; --idxEntry )
    {
        CModelIdxTreeEntry* pModelTreeEntry = m_pModelRootEntry->at(idxEntry);
        remove(pModelTreeEntry);
        delete pModelTreeEntry;
        pModelTreeEntry = nullptr;
    }
}

//------------------------------------------------------------------------------
void CModelIdxTreePhysSizesBranchContent::remove( CModelIdxTreeEntry* i_pModelTreeEntry )
//------------------------------------------------------------------------------
{
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
int CModelIdxTreePhysSizesBranchContent::columnWidthByColumn(int i_iClm, int i_iFontPixelSize)
//------------------------------------------------------------------------------
{
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

    return iClmWidth;

} // columnWidthByColumn

//------------------------------------------------------------------------------
int CModelIdxTreePhysSizesBranchContent::columnWidthByRole(const QByteArray& i_byteArrRole, int i_iFontPixelSize)
//------------------------------------------------------------------------------
{
    int iClmWidth = 1;

    int iRole = byteArr2Role(i_byteArrRole);
    EColumn clm = role2Column(iRole);

    if(clm != EColumnUndefined)
    {
        iClmWidth = columnWidthByColumn(static_cast<int>(clm), i_iFontPixelSize);
    }
    return iClmWidth;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QHash<int, QByteArray> CModelIdxTreePhysSizesBranchContent::roleNames() const
//------------------------------------------------------------------------------
{
    return s_roleNames;
}

//------------------------------------------------------------------------------
int CModelIdxTreePhysSizesBranchContent::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( m_pModelRootEntry != nullptr )
    {
        iRowCount = m_pModelRootEntry->count();
    }
    return iRowCount;
}

//------------------------------------------------------------------------------
int CModelIdxTreePhysSizesBranchContent::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QModelIndex CModelIdxTreePhysSizesBranchContent::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    return QAbstractTableModel::index(i_iRow, i_iCol, i_modelIdxParent);
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelIdxTreePhysSizesBranchContent::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
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

    return varData;

} // headerData

//------------------------------------------------------------------------------
QVariant CModelIdxTreePhysSizesBranchContent::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    EColumn clm = static_cast<EColumn>(i_modelIdx.column());
    int iRole = i_iRole;

    if( i_iRole >= static_cast<int>(ERole::FirstDataColumnRole) )
    {
        clm = role2Column(i_iRole);
        iRole = Qt::DisplayRole;
    }

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
                    varData = CModelIdxTree::getIcon(pModelTreeEntry->entryType());
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

    return varData;

} // data

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelIdxTreePhysSizesBranchContent::fillRoleNames()
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
