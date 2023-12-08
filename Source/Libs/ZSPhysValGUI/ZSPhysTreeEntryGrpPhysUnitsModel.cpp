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

#include "ZSPhysValGUI/ZSPhysTreeEntryGrpPhysUnitsModel.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
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
        qmlRegisterType<CModelPhysUnitsGrp>("ZSPhysValGUI", 1, 0, "ModelPhysSize");
    }
};

static CInitModulePhysTreeEntryPhysSizeModel s_initModule;


/*******************************************************************************
class CModelPhysUnitsGrp : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum ViewMode
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumEntriesViewMode[CModelPhysUnitsGrp::EViewModeCount] =
//------------------------------------------------------------------------------
{
    SEnumEntry( CModelPhysUnitsGrp::EViewModeNormal,           "Normal"            ),
    SEnumEntry( CModelPhysUnitsGrp::EViewModeContingencyTable, "Contingency Table" )
};

//------------------------------------------------------------------------------
QString CModelPhysUnitsGrp::viewMode2Str( int i_iViewMode )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumEntriesViewMode,EViewModeCount,i_iViewMode);
}

//------------------------------------------------------------------------------
CModelPhysUnitsGrp::EViewMode CModelPhysUnitsGrp::str2ViewMode( const QString& i_str )
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

QHash<int, QByteArray> CModelPhysUnitsGrp::s_roleNames;
QHash<QByteArray, int> CModelPhysUnitsGrp::s_roleValues;

QHash<int, QByteArray> CModelPhysUnitsGrp::s_clm2Name {
    { CModelPhysUnitsGrp::EColumnSymbol, "Symbol"},
    { CModelPhysUnitsGrp::EColumnNextLower, "NextLower"},
    { CModelPhysUnitsGrp::EColumnNextHigher, "NextHigher"},
    { CModelPhysUnitsGrp::EColumnFctConvertIntoSIUnit, "FctConvertIntoSIUnit"},
    { CModelPhysUnitsGrp::EColumnFctConvertFromSIUnit, "FctConvertFromSIUnit"}
};

//------------------------------------------------------------------------------
QString CModelPhysUnitsGrp::column2Str(EColumn i_clm)
//------------------------------------------------------------------------------
{
    QString str;
    if( i_clm < static_cast<EColumn>(CModelIdxTreeBranchContent::EColumnCount) ) {
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
CModelPhysUnitsGrp::EColumn CModelPhysUnitsGrp::role2Column(int i_iRole)
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
QString CModelPhysUnitsGrp::role2Str(int i_iRole)
//------------------------------------------------------------------------------
{
    return s_roleNames.value(i_iRole, "? (" + QByteArray::number(i_iRole) + ")");
}

//------------------------------------------------------------------------------
int CModelPhysUnitsGrp::byteArr2Role(const QByteArray& i_byteArrRole)
//------------------------------------------------------------------------------
{
    return s_roleValues.value(i_byteArrRole, Qt::DisplayRole);
}

//------------------------------------------------------------------------------
int CModelPhysUnitsGrp::column2Role(EColumn i_clm)
//------------------------------------------------------------------------------
{
    return static_cast<int>(ERole::FirstDataColumnRole) + i_clm;
}

//------------------------------------------------------------------------------
QString CModelPhysUnitsGrp::modelIdx2Str(
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
        if( i_iRole >= static_cast<int>(CModelPhysUnitsGrp::ERole::FirstDataColumnRole) ) {
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
CModelPhysUnitsGrp::CModelPhysUnitsGrp(QObject* i_pObjParent) :
//------------------------------------------------------------------------------
    CModelIdxTreeBranchContent(CIdxTreeUnits::GetInstance(), i_pObjParent),
    m_viewMode(EViewModeNormal),
    m_pUnitsGrp(nullptr)
{
    fillRoleNames();

    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );
    #endif

} // ctor

//------------------------------------------------------------------------------
CModelPhysUnitsGrp::~CModelPhysUnitsGrp()
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
    m_pUnitsGrp = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelPhysUnitsGrp::setViewMode( EViewMode i_viewMode )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
}

//------------------------------------------------------------------------------
CModelPhysUnitsGrp::EViewMode CModelPhysUnitsGrp::getViewMode() const
//------------------------------------------------------------------------------
{
    return m_viewMode;
}

/*==============================================================================
public: // overridables of base class CModelIdxTreeBranchContent
==============================================================================*/

//------------------------------------------------------------------------------
void CModelPhysUnitsGrp::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
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
        m_pUnitsGrp = nullptr;

        if( m_pIdxTree != nullptr )
        {
            CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTreeOfRootEntry);

            if( pTreeEntry != nullptr && (pTreeEntry->isRoot() || pTreeEntry->isBranch()) )
            {
                m_pUnitsGrp = dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(pTreeEntry);

                if( m_pUnitsGrp != nullptr )
                {
                    m_pModelRootEntry = new CModelIdxTreeEntry(m_pUnitsGrp);
                    m_pModelRootEntry->setSortOrder(m_sortOrder);

                    for( int idxEntry = 0; idxEntry < m_pUnitsGrp->count(); ++idxEntry )
                    {
                        CIdxTreeEntry* pTreeEntryChild = m_pUnitsGrp->at(idxEntry);
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
QHash<int, QByteArray> CModelPhysUnitsGrp::roleNames() const
//------------------------------------------------------------------------------
{
    return s_roleNames;
}

//------------------------------------------------------------------------------
int CModelPhysUnitsGrp::columnCount( const QModelIndex& i_modelIdxParent ) const
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(iClmCount);
    }
    #endif

    return iClmCount;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelPhysUnitsGrp::headerData(
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
            if( i_iSection >= 0 && i_iSection < rowCount() )
            {
                if( m_pUnitsGrp != nullptr )
                {
                    CUnitsTreeEntryPhysUnit* pUnit =
                        dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_pUnitsGrp->at(i_iSection));
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // headerData

//------------------------------------------------------------------------------
QVariant CModelPhysUnitsGrp::data( const QModelIndex& i_modelIdx, int i_iRole ) const
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

    CUnitsTreeEntryPhysUnit* pUnit = nullptr;

    if( i_modelIdx.isValid() && m_pUnitsGrp != nullptr )
    {
        pUnit = dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_pUnitsGrp->at(i_modelIdx.row()));
    }

    if( pUnit != nullptr && s_rolesHandled.contains(iRole) )
    {
        CUnitsTreeEntryPhysUnit* pUnitNextLower  = dynamic_cast<CUnitsTreeEntryPhysUnit*>(pUnit->nextLowerUnit());
        CUnitsTreeEntryPhysUnit* pUnitNextHigher = dynamic_cast<CUnitsTreeEntryPhysUnit*>(pUnit->nextHigherUnit());

        if( m_viewMode == EViewModeNormal )
        {
            if( clm == static_cast<EColumn>(CModelIdxTreeBranchContent::EColumnTreeEntryName) )
            {
                if( iRole == Qt::DisplayRole || iRole == Qt::EditRole )
                {
                    varData = pUnit->name();
                }
            }
            else if( clm < static_cast<EColumn>(CModelIdxTreeBranchContent::EColumnCount) )
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
                CUnitsTreeEntryPhysUnit* pUnitClm =
                    dynamic_cast<CUnitsTreeEntryPhysUnit*>(m_pUnitsGrp->at(clm));

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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // data

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelPhysUnitsGrp::fillRoleNames()
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
