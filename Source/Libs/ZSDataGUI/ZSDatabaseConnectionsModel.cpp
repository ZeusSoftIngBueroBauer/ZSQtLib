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

#if QT_VERSION < 0x050000
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif

#include "ZSDataGUI/ZSDatabaseConnectionsModel.h"
//#include "ZSDataGUI/ZSDatabaseRegisteredNodesDlg.h"
#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDatabaseClient.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Data;
using namespace ZS::Data::GUI;


/*******************************************************************************
class CDelegateDbConnectedNodes : public QItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateDbConnectedNodes::CDelegateDbConnectedNodes( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QItemDelegate(i_pObjParent),
    m_pDb(nullptr),
    m_pModel(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CDelegateDbConnectedNodes::~CDelegateDbConnectedNodes()
//------------------------------------------------------------------------------
{
    m_pDb = nullptr;
    m_pModel = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateDbConnectedNodes::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    if( m_pDb != i_pDb )
    {
        m_pDb = i_pDb;

        if( m_pDb != nullptr )
        {
            if( !QObject::connect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbDestroyed(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
    }
} // setDb

//------------------------------------------------------------------------------
void CDelegateDbConnectedNodes::setModel( QAbstractItemModel* i_pModel )
//------------------------------------------------------------------------------
{
    m_pModel = dynamic_cast<CModelDbConnections*>(i_pModel);

    if( m_pModel == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "dynamic_cast<CModelDbConnections*>(i_pModel)" );
    }

} // setModel

/*==============================================================================
public: // overridables of base class QItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
QWidget* CDelegateDbConnectedNodes::createEditor(
    QWidget*                    i_pWdgtParent,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QWidget* pWdgtEditor = nullptr;

    if( i_modelIdx.column() == CModelDbConnections::EColumnRegisteredNodes )
    {
        if( m_pDb != nullptr )
        {
            EProtocolType    protocolType = m_pModel->getProtocolType();
            ESocketType socketType   = m_pModel->getSocketType();

            QList<SCnctId> arCncts = m_pDb->getClientCnctIds(protocolType, socketType);

            //if( i_modelIdx.row() >= 0 && i_modelIdx.row() < arCncts.size() )
            //{
            //    SCnctId cnctId = arCncts[i_modelIdx.row()];

            //    SDbClientRegSpec regSpec = m_pDb->getClientRegistrationSpec(cnctId);

            //    CDlgDbRegisteredNodes* pDlg = new CDlgDbRegisteredNodes(
            //        /* strTitle    */ QApplication::applicationName() + ": " + cnctId.toString() + ": Registered Data Set Object Nodes",
            //        /* pDb         */ m_pDb,
            //        /* cnctSpec    */ regSpec,
            //        /* pWdgtParent */ nullptr );

            //    pDlg->exec();

            //    delete pDlg;
            //    pDlg = nullptr;
            //}
        } // if( m_pDb != nullptr && m_iSocketId >= 0 )
    } // if( i_modelIdx.column() == CModelDbConnections::EColumnRegisteredNodes )

    else // if( i_modelIdx.column() != CModelDbConnections::EColumnRegisteredNodes )
    {
        pWdgtEditor = QItemDelegate::createEditor(i_pWdgtParent,i_styleOption,i_modelIdx);
    }

    return pWdgtEditor;

} // createEditor

//------------------------------------------------------------------------------
void CDelegateDbConnectedNodes::updateEditorGeometry(
    QWidget*                    i_pWdgtEditor,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex&          /*i_modelIdx*/ ) const
//------------------------------------------------------------------------------
{
    i_pWdgtEditor->setGeometry(i_styleOption.rect);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateDbConnectedNodes::onDbDestroyed( QObject* i_pDb )
//------------------------------------------------------------------------------
{
    if( i_pDb != m_pDb )
    {
        QString strAddInfo = "onDbDestroyed( i_pDb != m_pDb )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    m_pDb = nullptr;

} // onDbDestroyed


/*******************************************************************************
class CModelDbConnections : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelDbConnections::CModelDbConnections( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pDb(nullptr),
    m_pDbClient(nullptr),
    m_protocolType(EProtocolTypeUndefined),
    m_socketType(ESocketTypeUndefined),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("DbConnectionsModel");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // "update" must be called after creating the model.
    // If desired the protocol and/or connection type must also be set
    // after creating the model but before calling update.

} // ctor

//------------------------------------------------------------------------------
CModelDbConnections::~CModelDbConnections()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDb = nullptr;
    m_pDbClient = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelDbConnections::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDb",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDbClient != nullptr )
    {
        setDbClient(nullptr);
    }

    if( m_pDb != i_pDb )
    {
        if( m_pDb != nullptr )
        {
            //QObject::disconnect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onConnected(QObject*,const ZS::Ipc::SSocketDscr&)) );
            //QObject::disconnect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) );
            //QObject::disconnect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(clientRegistered(QObject*,const ZS::Data::SDbClientRegSpec&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onClientRegistered(QObject*,const ZS::Data::SDbClientRegSpec&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbDestroyed(QObject*)) );

        } // if( m_pDb != nullptr )

        m_pDb = i_pDb;

        if( m_pDb != nullptr )
        {
            //if( !QObject::connect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onConnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
            //{
            //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            //}
            //if( !QObject::connect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
            //{
            //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            //}
            //if( !QObject::connect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(clientRegistered(QObject*,const ZS::Data::SDbClientRegSpec&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onClientRegistered(QObject*,const ZS::Data::SDbClientRegSpec&)) ) )
            //{
            //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            //}
            if( !QObject::connect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbDestroyed(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        } // if( m_pDb != nullptr )

        // "update" must be called after setting the database or database client

    } // if( m_pDb != i_pDb )

} // setDb

//------------------------------------------------------------------------------
void CModelDbConnections::setDbClient( CDbClient* i_pDbClient )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClient == nullptr ? "nullptr" : i_pDbClient->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDbClient",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDb != nullptr )
    {
        setDb(nullptr);
    }

    if( m_pDbClient != i_pDbClient )
    {
        if( m_pDbClient != nullptr )
        {
            //QObject::disconnect(
            //    /* pObjSender   */ m_pDbClient,
            //    /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onConnected(QObject*,const ZS::Ipc::SSocketDscr&)) );
            //QObject::disconnect(
            //    /* pObjSender   */ m_pDbClient,
            //    /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDestroyed(QObject*)) );

        } // if( m_pDbClient != nullptr )

        m_pDbClient = i_pDbClient;

        if( m_pDbClient != nullptr )
        {
            //if( !QObject::connect(
            //    /* pObjSender   */ m_pDbClient,
            //    /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onConnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
            //{
            //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            //}
            //if( !QObject::connect(
            //    /* pObjSender   */ m_pDbClient,
            //    /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
            //{
            //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            //}
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDestroyed(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        } // if( m_pDbClient != nullptr )

        // "update" must be called after setting the database or database client

    } // if( m_pDbClient != i_pDbClient )

} // setDbClient

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelDbConnections::update()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "update",
        /* strAddInfo   */ "" );

    beginResetModel();
    endResetModel();

} // update

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelDbConnections::setProtocolType( EProtocolType i_protocolType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = protocolType2Str(i_protocolType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setProtocolType",
        /* strAddInfo   */ strAddTrcInfo );

    m_protocolType = i_protocolType;

    // "update" must be called after setting the protocol and/or connection type.

} // setProtocolType

//------------------------------------------------------------------------------
void CModelDbConnections::setSocketType( ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setSocketType",
        /* strAddInfo   */ strAddTrcInfo );

    m_socketType = i_socketType;

    // "update" must be called after setting the protocol and/or connection type.

} // setSocketType

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelDbConnections::onConnected( QObject* i_pSrvClt, const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pSrvClt == nullptr )
        {
            strAddTrcInfo = "SrvClt: nullptr";
        }
        else if( i_pSrvClt == m_pDb )
        {
            strAddTrcInfo = "Db: " + m_pDb->objectName();
        }
        else if( i_pSrvClt == m_pDbClient )
        {
            strAddTrcInfo = "DbClient: " + m_pDbClient->objectName();
        }
        strAddTrcInfo += ", Socket: " + i_socketDscr.getConnectionString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onConnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pSrvClt != m_pDb && i_pSrvClt != m_pDbClient )
    {
        QString strAddInfo = "onConnected( i_pSrvClt != m_pDb && i_pSrvClt != m_pDbClient )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    beginResetModel();
    endResetModel();

} // onConnected

//------------------------------------------------------------------------------
void CModelDbConnections::onDisconnected( QObject* i_pSrvClt, const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pSrvClt == nullptr )
        {
            strAddTrcInfo = "SrvClt: nullptr";
        }
        else if( i_pSrvClt == m_pDb )
        {
            strAddTrcInfo = "Db: " + m_pDb->objectName();
        }
        else if( i_pSrvClt == m_pDbClient )
        {
            strAddTrcInfo = "DbClient: " + m_pDbClient->objectName();
        }
        strAddTrcInfo += ", Socket: " + i_socketDscr.getConnectionString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pSrvClt != m_pDb && i_pSrvClt != m_pDbClient )
    {
        QString strAddInfo = "onDisconnected( ";
        strAddInfo += "i_pSrvClt != m_pDb && i_pSrvClt != m_pDbClient )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    beginResetModel();
    endResetModel();

} // onDisconnected

//------------------------------------------------------------------------------
void CModelDbConnections::onClientRegistered( QObject* i_pDb, const SDbClientRegSpec& /*i_regSpec*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pDb == nullptr )
        {
            strAddTrcInfo = "SrvClt: nullptr";
        }
        else if( i_pDb == m_pDb )
        {
            strAddTrcInfo = "Db: " + m_pDb->objectName();
        }
        else
        {
            strAddTrcInfo = "Db: INVALID (" + i_pDb->objectName() + ")";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onClientRegistered",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pDb != m_pDb )
    {
        QString strAddInfo = "onClientRegistered( i_pDb != m_pDb )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    beginResetModel();
    endResetModel();

} // onClientRegistered

//------------------------------------------------------------------------------
void CModelDbConnections::onDbDestroyed( QObject* i_pDb )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pDb != m_pDb )
    {
        QString strAddInfo = "onDbDestroyed( i_pDb != m_pDb )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    m_pDb = nullptr;

    beginResetModel();
    endResetModel();

} // onDbDestroyed

//------------------------------------------------------------------------------
void CModelDbConnections::onDbClientDestroyed( QObject* i_pDbClient )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pDbClient != m_pDbClient )
    {
        QString strAddInfo = "onDbClientDestroyed( i_pDbClient != m_pDbClient )";
        throw CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    m_pDb = nullptr;

    beginResetModel();
    endResetModel();

} // onDbClientDestroyed

/*==============================================================================
public: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelDbConnections::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( m_pDb != nullptr )
    {
        iRowCount = m_pDb->getClientCnctsCount(m_protocolType, m_socketType);
    }
    else if( m_pDbClient != nullptr )
    {
        iRowCount = 1;
    }
    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelDbConnections::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelDbConnections::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;
    int      iRow = i_modelIdx.row();
    int      iClm = i_modelIdx.column();

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( iRow < 0 || iRow >= rowCount() )
    {
        return varData;
    }
    if( iClm < 0 || iClm >= columnCount() )
    {
        return varData;
    }

    SCnctId cnctId(m_protocolType,m_socketType);

    //SServerHostSettings hostSettingsServer;
    //SClientHostSettings hostSettingsClient;

    SSocketDscr      socketDscr;
    SDbClientRegSpec dbCltRegSpec;

    if( m_pDb != nullptr )
    {
        QList<SCnctId> arCncts = m_pDb->getClientCnctIds(m_protocolType, m_socketType);

        if( iRow < arCncts.size() )
        {
            cnctId = arCncts[iRow];

            if( cnctId.m_socketType == ESocketTypeTcp || cnctId.m_socketType == ESocketTypeShm )
            {
                socketDscr = m_pDb->getClientCnctDscr(iRow, m_protocolType, m_socketType);
            }
            //dbCltRegSpec = m_pDb->getClientRegistrationSpec(cnctId);
        }
    }
    else if( m_pDbClient != nullptr )
    {
        cnctId.m_protocolType = m_protocolType;
        cnctId.m_socketType   = m_socketType;

        if( cnctId.m_socketType == ESocketTypeTcp || cnctId.m_socketType == ESocketTypeShm )
        {
            socketDscr = m_pDbClient->getSocketDscr();
        }

        //dbCltRegSpec = m_pDbClient->getRegistrationSpec();
    }
    else
    {
        return varData;
    }

    //ESocketState socketState = socketDscr.m_socketState;

    switch( iClm )
    {
        case EColumnProtocolType:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = protocolType2Str(cnctId.m_protocolType);
            }
            break;
        }
        case EColumnSocketType:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = socketType2Str(cnctId.m_socketType);
            }
            break;
        }
        case EColumnClientName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = dbCltRegSpec.m_strClientName;
            }
            break;
        }
        case EColumnRegisteredNodes:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = dbCltRegSpec.toString(1);
            }
            break;
        }
        case EColumnSocketId:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = socketDscr.m_iSocketId;
            }
            break;
        }
        case EColumnSocketState:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = socketState2Str(socketDscr.m_socketState);
            }
            break;
        }
        case EColumnServerListenPort:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( socketDscr.m_iSocketId >= 0 )
                {
                    varData = socketDscr.m_uServerListenPort;
                }
            }
            break;
        }
        case EColumnLocalPort:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( socketDscr.m_iSocketId >= 0 )
                {
                    varData = socketDscr.m_uLocalPort;
                }
            }
            break;
        }
        case EColumnRemoteHostName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( socketDscr.m_iSocketId >= 0 )
                {
                    if( socketDscr.m_socketType != ESocketTypeInProcMsg )
                    {
                        varData = socketDscr.m_strRemoteHostName;
                    }
                }
            }
            break;
        }
        case EColumnRemotePort:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( socketDscr.m_iSocketId >= 0 )
                {
                    varData = socketDscr.m_uRemotePort;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return varData;

} // data

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
Qt::ItemFlags CModelDbConnections::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(i_modelIdx);

    if( !i_modelIdx.isValid() )
    {
        return flags;
    }
    if( m_pDb == nullptr )
    {
        return flags;
    }
    if( i_modelIdx.column() != EColumnRegisteredNodes )
    {
        return flags;
    }

    QList<SCnctId> arCncts = m_pDb->getClientCnctIds(m_protocolType, m_socketType);

    if( i_modelIdx.row() < 0 || i_modelIdx.row() > arCncts.size() )
    {
        return flags;
    }

    flags |= Qt::ItemIsEditable;

    return flags;

} // flags

//------------------------------------------------------------------------------
QVariant CModelDbConnections::headerData(
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
            case EColumnProtocolType:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Data Protocol";
                }
                break;
            }
            case EColumnSocketType:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Socket Type";
                }
                break;
            }
            case EColumnClientName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Client Name";
                }
                break;
            }
            case EColumnRegisteredNodes:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Registered Nodes";
                }
                break;
            }
            case EColumnSocketId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Socket-Id";
                }
                break;
            }
            case EColumnSocketState:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Socket State";
                }
                break;
            }
            case EColumnServerListenPort:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Server Listen Port";
                }
                break;
            }
            case EColumnLocalPort:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Local Port";
                }
                break;
            }
            case EColumnRemoteHostName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Remote Host Name";
                }
                break;
            }
            case EColumnRemotePort:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Remote Port";
                }
                break;
            }
            default:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "?";
                }
                break;
            }
        }
    }
    else if( i_orientation == Qt::Vertical )
    {
        varData = i_iSection;
    }
    return varData;

} // headerData
