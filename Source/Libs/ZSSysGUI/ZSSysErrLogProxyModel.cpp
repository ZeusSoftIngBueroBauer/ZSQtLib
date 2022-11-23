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

#include "ZSSysGUI/ZSSysErrLogProxyModel.h"
#include "ZSSysGUI/ZSSysErrLogModel.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CModelErrLog : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CProxyModelErrLog::CProxyModelErrLog(CModelErrLog* i_pErrLogModel) :
//------------------------------------------------------------------------------
    QSortFilterProxyModel(),
    m_pErrLogModel(i_pErrLogModel),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pErrLogModel->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_pErrLogModel->objectName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

    QSortFilterProxyModel::setSourceModel(m_pErrLogModel);

} // ctor

//------------------------------------------------------------------------------
CProxyModelErrLog::~CProxyModelErrLog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pErrLogModel = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelErrLog* CProxyModelErrLog::errLogModel()
//------------------------------------------------------------------------------
{
    return m_pErrLogModel;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CProxyModelErrLog::columnWidth(int i_iClm, const QFont* i_pFont) const
//------------------------------------------------------------------------------
{
    return m_pErrLogModel->columnWidth(i_iClm, i_pFont);
}

/*==============================================================================
protected: // overridables of base class QSortFilterProxyModel
==============================================================================*/

//------------------------------------------------------------------------------
void CProxyModelErrLog::sort(int i_iClm, Qt::SortOrder i_sortOrder)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Clm: " + m_pErrLogModel->column2Str(static_cast<CModelErrLog::EColumn>(i_iClm));
        strMthInArgs += ", SortOrder: " + qSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rowCount",
        /* strMthInArgs */ strMthInArgs );

    QSortFilterProxyModel::sort(i_iClm, i_sortOrder);
}

//------------------------------------------------------------------------------
int CProxyModelErrLog::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Parent {" + m_pErrLogModel->modelIndex2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "columnCount",
        /* strMthInArgs */ strMthInArgs );

    int iClmCount = m_pErrLogModel->columnCount();

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(iClmCount);
    }
    return iClmCount;
}

//------------------------------------------------------------------------------
QModelIndex CProxyModelErrLog::mapFromSource( const QModelIndex& i_modelIdxSource ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + m_pErrLogModel->modelIndex2Str(i_modelIdxSource) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mapFromSource",
        /* strMthInArgs */ strMthInArgs );

    QModelIndex modelIdxProxy = QSortFilterProxyModel::mapFromSource(i_modelIdxSource);

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(m_pErrLogModel->modelIndex2Str(modelIdxProxy));
    }
    return modelIdxProxy;
}

//------------------------------------------------------------------------------
QModelIndex CProxyModelErrLog::mapToSource( const QModelIndex& i_modelIdxProxy ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + m_pErrLogModel->modelIndex2Str(i_modelIdxProxy) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mapToSource",
        /* strMthInArgs */ strMthInArgs );

    QModelIndex modelIdxSource = QSortFilterProxyModel::mapToSource(i_modelIdxProxy);;

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(m_pErrLogModel->modelIndex2Str(modelIdxSource));
    }
    return modelIdxSource;
}
