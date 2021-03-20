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

#include <QtNetwork/qhostinfo.h>

#include "ZSIpcGUI/ZSIpcModelSrvCltConnection.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSIpc/ZSIpcClient.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Ipc::GUI;


/*******************************************************************************
class CModelSrvCltConnection : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelSrvCltConnection::CModelSrvCltConnection( ESrvCltType i_srvCltType, QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_srvCltType(i_srvCltType),
    m_pServer(nullptr),
    m_pClient(nullptr)
{
    //if( m_srvCltType == ESrvCltTypeClient )
    //{
    //    beginInsertRows( QModelIndex(), 0, m_iRowCount-1 );
    //    beginInsertRows( QModelIndex(), 0, m_iRowCount-1 );
    //}

} // ctor

//------------------------------------------------------------------------------
CModelSrvCltConnection::~CModelSrvCltConnection()
//------------------------------------------------------------------------------
{
    m_srvCltType = static_cast<ESrvCltType>(0);
    m_pServer = nullptr;
    m_pClient = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelSrvCltConnection::setServer( CServer* i_pServer )
//------------------------------------------------------------------------------
{
    if( m_srvCltType != ESrvCltTypeServer && i_pServer != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    }

    if( m_pServer != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pServer,
            /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onServerConnected(QObject*,const ZS::Ipc::SSocketDscr&)) );
        QObject::disconnect(
            /* pObjSender   */ m_pServer,
            /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onServerDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) );
        QObject::disconnect(
            /* pObjSender   */ m_pServer,
            /* szSignal     */ SIGNAL(socketDscrChanged(QObject*,const ZS::Ipc::SSocketDscr&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onServerSocketDscrChanged(QObject*,const ZS::Ipc::SSocketDscr&)) );

        m_pServer = nullptr;

        #if QT_VERSION < 0x050000
        reset();
        #else
        beginResetModel();
        endResetModel();
        #endif

    } // if( m_pServer != nullptr )

    m_pServer = i_pServer;

    if( m_pServer != nullptr )
    {
        int iRowCount = m_pServer->getArrLenConnections();

        if( iRowCount > 0 )
        {
            beginInsertRows( QModelIndex(), 0, iRowCount-1 );
            endInsertRows();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pServer,
            /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onServerConnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pServer,
            /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onServerDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pServer,
            /* szSignal     */ SIGNAL(socketDscrChanged(QObject*,const ZS::Ipc::SSocketDscr&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onServerSocketDscrChanged(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pServer != nullptr )

} // setServer

//------------------------------------------------------------------------------
void CModelSrvCltConnection::setClient( CClient* i_pClient )
//------------------------------------------------------------------------------
{
    if( m_srvCltType != ESrvCltTypeClient && i_pClient != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    }

    if( m_pClient != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pClient,
            /* szSignal     */ SIGNAL(connected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClientConnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pClient,
            /* szSignal     */ SIGNAL(disconnected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClientDisconnected(QObject*)) );

        m_pClient = nullptr;

        #if QT_VERSION < 0x050000
        reset();
        #else
        beginResetModel();
        endResetModel();
        #endif

    } // if( m_pClient != nullptr )

    m_pClient = i_pClient;

    if( m_pClient != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pClient,
            /* szSignal     */ SIGNAL(connected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClientConnected(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pClient,
            /* szSignal     */ SIGNAL(disconnected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onClientDisconnected(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pClient != nullptr )

} // setClient

/*==============================================================================
public: // must overridables of base class QAbstractTableModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelSrvCltConnection::rowCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        if( m_pServer != nullptr )
        {
            iRowCount = m_pServer->getArrLenConnections();
        }
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        iRowCount = 1;
    }
    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelSrvCltConnection::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelSrvCltConnection::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;
    int      iRow = i_modelIdx.row();
    int      iCol = i_modelIdx.column();

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }
    if( iRow < 0 || iRow >= static_cast<int>(rowCount()) )
    {
        return varData;
    }
    if( iCol < 0 || iCol >= columnCount() )
    {
        return varData;
    }

    SSocketDscr socketDscr(ESrvCltTypeUndefined);

    if( m_pServer != nullptr )
    {
        socketDscr = m_pServer->getSocketDscr( static_cast<unsigned int>(iRow) );
    }
    else if( m_pClient != nullptr )
    {
        socketDscr = m_pClient->getSocketDscr();
    }
    else
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
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
                    if( socketDscr.m_socketType == Ipc::ESocketTypeInProcMsg )
                    {
                        varData = QHostInfo::localHostName();
                    }
                    else
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
        case EColumnSocketName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = socketDscr.m_strSocketName;
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

//------------------------------------------------------------------------------
QVariant CModelSrvCltConnection::headerData(
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
            case EColumnSocketName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Socket Name";
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

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelSrvCltConnection::onServerConnected( QObject* i_pServer, const SSocketDscr& /*i_socketDscr*/ )
//------------------------------------------------------------------------------
{
    if( i_pServer != m_pServer )
    {
        QString strAddInfo = "onServerConnected( i_pServer != m_pServer )";
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    int iRowCount = m_pServer->getArrLenConnections();

    beginInsertRows( QModelIndex(), iRowCount-1, iRowCount-1 );
    endInsertRows();

} // onServerConnected

//------------------------------------------------------------------------------
void CModelSrvCltConnection::onServerDisconnected( QObject* i_pServer, const SSocketDscr& /*i_socketDscr*/ )
//------------------------------------------------------------------------------
{
    if( i_pServer != m_pServer )
    {
        QString strAddInfo = "onServerConnected( i_pServer != m_pServer )";
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    #if QT_VERSION < 0x050000
    reset();
    #else
    beginResetModel();
    endResetModel();
    #endif

} // onServerDisconnected

//------------------------------------------------------------------------------
void CModelSrvCltConnection::onServerSocketDscrChanged( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    if( i_pServer != m_pServer )
    {
        QString strAddInfo = "onServerConnected( i_pServer != m_pServer )";
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    if( i_socketDscr.m_iSocketId >= 0 && i_socketDscr.m_iSocketId < rowCount() )
    {
        int iRow = i_socketDscr.m_iSocketId;

        emit dataChanged( index(iRow,0), index(iRow,EColumnCount-1) );
    }

} // onServerSocketDscrChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelSrvCltConnection::onClientConnected( QObject* i_pClient )
//------------------------------------------------------------------------------
{
    if( i_pClient != m_pClient )
    {
        QString strAddInfo = "onClientConnected( i_pClient != m_pClient )";
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    emit dataChanged( index(0,0), index(0,EColumnCount-1) );

} // onClientConnected

//------------------------------------------------------------------------------
void CModelSrvCltConnection::onClientDisconnected( QObject* i_pClient )
//------------------------------------------------------------------------------
{
    if( i_pClient != m_pClient )
    {
        QString strAddInfo = "onClientConnected( i_pClient != m_pClient )";
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError,strAddInfo);
    }

    emit dataChanged( index(0,0), index(0,EColumnCount-1) );

} // onClientDisconnected
