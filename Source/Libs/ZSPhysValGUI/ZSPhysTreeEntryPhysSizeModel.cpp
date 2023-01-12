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

#include "ZSPhysValGUI/ZSPhysTreeEntryPhysSizeModel.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
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
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/******************************************************************************/
class CInitModulePhysTreeEntryPhysSizeModel
/* Please note:
   The class name should be unique for the whole system. Otherwise the compiler
   may be confused and using a CInitModule class from other modules to create
   the static InitModule instance.
*******************************************************************************/
{
public: // ctor
    CInitModulePhysTreeEntryPhysSizeModel()
    {
        qmlRegisterType<CModelPhysSize>("ZSPhysValGUI", 1, 0, "ModelPhysSize");
    }
};

static CInitModulePhysTreeEntryPhysSizeModel s_initModule;


/*******************************************************************************
class CModelPhysSize : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum ViewMode
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumEntriesViewMode[CModelPhysSize::EViewModeCount] =
//------------------------------------------------------------------------------
{
    SEnumEntry( CModelPhysSize::EViewModeNormal,           "Normal"            ),
    SEnumEntry( CModelPhysSize::EViewModeContingencyTable, "Contingency Table" )
};

//------------------------------------------------------------------------------
QString CModelPhysSize::viewMode2Str( int i_iViewMode )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumEntriesViewMode,EViewModeCount,i_iViewMode);
}

//------------------------------------------------------------------------------
CModelPhysSize::EViewMode CModelPhysSize::str2ViewMode( const QString& i_str )
//------------------------------------------------------------------------------
{
    EViewMode viewMode = EViewModeUndefined;
    int iViewMode = SEnumEntry::str2Enumerator(s_arEnumEntriesViewMode,EViewModeCount,i_str);
    if( iViewMode >= 0 && iViewMode < EViewModeCount )
    {
        viewMode = static_cast<EViewMode>(iViewMode);
    }
    return viewMode;
}

/* enum EColumn
==============================================================================*/

QHash<int, QByteArray> CModelPhysSize::s_roleNames;
QHash<QByteArray, int> CModelPhysSize::s_roleValues;

QHash<int, QByteArray> CModelPhysSize::s_clm2Name {
    { CModelPhysSize::EColumnSymbol, "Symbol"},
    { CModelPhysSize::EColumnNextLower, "NextLower"},
    { CModelPhysSize::EColumnNextHigher, "NextHigher"},
    { CModelPhysSize::EColumnFctConvertIntoSIUnit, "FctConvertIntoSIUnit"},
    { CModelPhysSize::EColumnFctConvertFromSIUnit, "FctConvertFromSIUnit"}
};

//------------------------------------------------------------------------------
QString CModelPhysSize::column2Str(EColumn i_clm)
//------------------------------------------------------------------------------
{
    QString str;
    if( i_clm < CModelIdxTreeBranchContent::EColumnCount ) {
        str = CModelIdxTreeBranchContent::column2Str(static_cast<CModelIdxTreeBranchContent::EColumn>(i_clm));
    }
    else {
        str = s_clm2Name.value(i_clm, "? (" + QByteArray::number(i_clm) + ")");
    }
    return str;
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelPhysSize::EColumn CModelPhysSize::role2Column(int i_iRole)
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
QString CModelPhysSize::role2Str(int i_iRole)
//------------------------------------------------------------------------------
{
    return s_roleNames.value(i_iRole, "? (" + QByteArray::number(i_iRole) + ")");
}

//------------------------------------------------------------------------------
int CModelPhysSize::byteArr2Role(const QByteArray& i_byteArrRole)
//------------------------------------------------------------------------------
{
    return s_roleValues.value(i_byteArrRole, Qt::DisplayRole);
}

//------------------------------------------------------------------------------
int CModelPhysSize::column2Role(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return static_cast<int>(ERole::FirstDataColumnRole) + i_clm;
}

//------------------------------------------------------------------------------
QString CModelPhysSize::modelIdx2Str(
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
        if( i_iRole >= static_cast<int>(CModelPhysSize::ERole::FirstDataColumnRole) ) {
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
CModelPhysSize::CModelPhysSize( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelPhysSize(nullptr, i_pObjParent)
{
} // ctor

//------------------------------------------------------------------------------
CModelPhysSize::CModelPhysSize(
    CIdxTreePhysSizes* i_pIdxTree, QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelIdxTreeBranchContent(i_pIdxTree, i_pObjParent),
    m_viewMode(EViewModeNormal),
    m_pPhysSize(nullptr)
{
    fillRoleNames();

    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

} // ctor

//------------------------------------------------------------------------------
CModelPhysSize::~CModelPhysSize()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );
    #endif

    m_viewMode = static_cast<EViewMode>(0);
    m_pPhysSize = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CModelPhysSize::setViewMode( EViewMode i_viewMode )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = viewMode2Str(i_viewMode);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setViewMode",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    if( m_viewMode != i_viewMode )
    {
        beginResetModel();
        m_viewMode = i_viewMode;
        endResetModel();
    }
    return true;
}

//------------------------------------------------------------------------------
CModelPhysSize::EViewMode CModelPhysSize::getViewMode() const
//------------------------------------------------------------------------------
{
    return m_viewMode;
}

/*==============================================================================
public: // overridables of base class CModelIdxTreeBranchContent
==============================================================================*/

//------------------------------------------------------------------------------
void CModelPhysSize::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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
        m_pPhysSize = nullptr;

        if( m_pIdxTree != nullptr )
        {
            CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(i_strKeyInTree);

            if( pTreeEntry != nullptr && (pTreeEntry->isRoot() || pTreeEntry->isBranch()) )
            {
                m_pPhysSize = dynamic_cast<CPhysSizeTreeEntry*>(pTreeEntry);

                if( m_pPhysSize != nullptr )
                {
                    m_pModelRootEntry = new CModelIdxTreeEntry(m_pPhysSize);
                    m_pModelRootEntry->setSortOrder(m_sortOrder);

                    for( int idxEntry = 0; idxEntry < m_pPhysSize->count(); ++idxEntry )
                    {
                        CIdxTreeEntry* pTreeEntryChild = m_pPhysSize->at(idxEntry);
                        onIdxTreeEntryAdded(pTreeEntryChild->keyInTree());
                    }
                }
            }
        }

        emit keyInTreeOfRootEntryChanged(m_strKeyInTreeOfRootEntry);

        if( m_viewMode == EViewModeContingencyTable )
        {
            beginResetModel();
            endResetModel();
        }
    } // if( m_strKeyInTreeOfRootEntry.compare(i_strKeyInTree) != 0 )
} // setKeyInTreeOfRootEntry

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QHash<int, QByteArray> CModelPhysSize::roleNames() const
//------------------------------------------------------------------------------
{
    return s_roleNames;
}

//------------------------------------------------------------------------------
int CModelPhysSize::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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

    if( m_pPhysSize != nullptr )
    {
        iRowCount = m_pPhysSize->count();
    }

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(iRowCount);
    }
    #endif

    return iRowCount;
}

//------------------------------------------------------------------------------
int CModelPhysSize::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

    int iClmCount = 0;

    switch( m_viewMode )
    {
        case EViewModeNormal:
        {
            iClmCount = EColumnCount;
            break;
        }
        case EViewModeContingencyTable:
        {
            iClmCount = rowCount();
            break;
        }
        default:
        {
            break;
        }
    }

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(iClmCount);
    }
    #endif

    return iClmCount;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelPhysSize::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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

    if( m_viewMode == EViewModeNormal )
    {
        if( i_iSection < CModelIdxTreeBranchContent::EColumnCount )
        {
            varData = CModelIdxTreeBranchContent::headerData(i_iSection, i_orientation, i_iRole);
        }
        else if( i_iSection == EColumnSymbol )
        {
            if( i_iRole == Qt::DisplayRole && i_orientation == Qt::Horizontal )
            {
                varData = "Symbol";
            }
        }
        else if( i_iSection == EColumnNextLower )
        {
            if( i_iRole == Qt::DisplayRole && i_orientation == Qt::Horizontal )
            {
                varData = "Next Lower";
            }
        }
        else if( i_iSection == EColumnNextHigher )
        {
            if( i_iRole == Qt::DisplayRole && i_orientation == Qt::Horizontal )
            {
                varData = "Next Higher";
            }
        }
        else if( i_iSection == EColumnFctConvertIntoSIUnit )
        {
            if( i_iRole == Qt::DisplayRole && i_orientation == Qt::Horizontal )
            {
                varData = "Conversion Into SI Unit";
            }
        }
        else if( i_iSection == EColumnFctConvertFromSIUnit )
        {
            if( i_iRole == Qt::DisplayRole && i_orientation == Qt::Horizontal )
            {
                varData = "Conversion From SI Unit";
            }
        }
        else
        {
            if( i_iRole == Qt::DisplayRole && i_orientation == Qt::Horizontal )
            {
                varData = "?";
            }
        }
    }
    else if( m_viewMode == EViewModeContingencyTable )
    {
        if( i_iRole == Qt::DisplayRole )
        {
            CPhysUnit* pUnit = nullptr;

            if( i_iSection >= 0 && i_iSection < rowCount() )
            {
                if( m_pPhysSize != nullptr )
                {
                    pUnit = dynamic_cast<CPhysUnit*>(m_pPhysSize->at(i_iSection));
                    if( pUnit == nullptr )
                    {
                        return varData;
                    }
                    varData = pUnit->symbol();
                }
            }
        }
    } // if( m_viewMode == EViewModeContingencyTable )

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // headerData

//------------------------------------------------------------------------------
QVariant CModelPhysSize::data( const QModelIndex& i_modelIdx, int i_iRole ) const
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
    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
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

    CPhysUnitTreeEntry* pUnit = nullptr;

    if( i_modelIdx.isValid() && m_pPhysSize != nullptr )
    {
        pUnit = dynamic_cast<CPhysUnitTreeEntry*>(m_pPhysSize->at(i_modelIdx.row()));
    }

    if( pUnit != nullptr && s_rolesHandled.contains(iRole) )
    {
        CPhysUnitTreeEntry* pUnitNextLower  = dynamic_cast<CPhysUnitTreeEntry*>(pUnit->nextLowerUnit());
        CPhysUnitTreeEntry* pUnitNextHigher = dynamic_cast<CPhysUnitTreeEntry*>(pUnit->nextHigherUnit());

        if( m_viewMode == EViewModeNormal )
        {
            if( clm == CModelIdxTreeBranchContent::EColumnTreeEntryName )
            {
                if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pUnit->name();
                }
            }
            else if( clm < CModelIdxTreeBranchContent::EColumnCount )
            {
                varData = CModelIdxTreeBranchContent::data(i_modelIdx, i_iRole);
            }
            else if( clm == EColumnSymbol )
            {
                if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pUnit->symbol();
                }
            }
            else if( clm == EColumnNextLower )
            {
                if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    if( pUnitNextLower != nullptr )
                    {
                        varData = pUnitNextLower->name();
                    }
                }
            }
            else if( clm == EColumnNextHigher )
            {
                if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    if( pUnitNextHigher != nullptr )
                    {
                        varData = pUnitNextHigher->name();
                    }
                }
            }
            else if( clm == EColumnFctConvertIntoSIUnit )
            {
                if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pUnit->getFctConvertIntoSIUnitName();
                }
            }
            else if( clm == EColumnFctConvertFromSIUnit )
            {
                if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pUnit->getFctConvertFromSIUnitName();
                }
            }
        } // if( m_viewMode == EViewModeNormal )

        else if( m_viewMode == EViewModeContingencyTable )
        {
            if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
            {
                CPhysUnitTreeEntry* pUnitClm = dynamic_cast<CPhysUnitTreeEntry*>(m_pPhysSize->at(clm));

                if( pUnitClm != nullptr )
                {
                    if( clm == i_modelIdx.row() )
                    {
                        varData = QString("-");
                    }
                    else
                    {
                        varData = pUnit->findFctConvertInternalName(pUnitClm);
                    }
                }
            }
        } // case EViewModeContingencyTable:
    } // if( pUnit != nullptr && s_rolesHandled.contains(iRole) )

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) ) {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // data

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelPhysSize::fillRoleNames()
//------------------------------------------------------------------------------
{
    if( s_roleNames.isEmpty() )
    {
        s_roleNames = CModelIdxTreeBranchContent::roleNames();

        for( int clm = CModelIdxTreeBranchContent::EColumnCount; clm < EColumnCount; ++clm)
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
