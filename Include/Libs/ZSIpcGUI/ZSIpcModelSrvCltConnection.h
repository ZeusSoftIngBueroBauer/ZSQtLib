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

#ifndef ZSIpcGUI_ModelSrvCltConnection_h
#define ZSIpcGUI_ModelSrvCltConnection_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSIpcGUI/ZSIpcGUIDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"

namespace ZS
{
namespace Ipc
{
class CServer;
class CClient;
struct SSocketDscr;

namespace GUI
{
//******************************************************************************
class ZSIPCGUIDLL_API CModelSrvCltConnection : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EColumn {
        EColumnSocketId         = 0,
        EColumnSocketState      = 1,
        EColumnServerListenPort = 2,
        EColumnLocalPort        = 3,
        EColumnRemoteHostName   = 4,
        EColumnRemotePort       = 5,
        EColumnSocketName       = 6,
        EColumnCount
    };
public: // class methods
    static QString NameSpace() { return "ZS::Ipc::GUI"; }
    static QString ClassName() { return "CModelSrvCltConnection"; }
public: // ctors and dtor
    CModelSrvCltConnection( ESrvCltType i_srvCltType, QObject* i_pObjParent = nullptr );
    virtual ~CModelSrvCltConnection();
public: // instance methods
    void setClient( CClient* i_pClient );
    void setServer( CServer* i_pServer );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
protected slots:
    void onClientConnected( QObject* i_pClient );
    void onClientDisconnected( QObject* i_pClient );
protected slots:
    void onServerConnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onServerDisconnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onServerSocketDscrChanged( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
private: // copy ctor not allowed
    CModelSrvCltConnection( const CModelSrvCltConnection& );
private: // assignment operator not allowed
    CModelSrvCltConnection& operator = ( const CModelSrvCltConnection& );
protected: // instance members
    ESrvCltType m_srvCltType;
    CServer*    m_pServer;
    CClient*    m_pClient;

}; // class CModelSrvCltConnection

} // namespace GUI

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpcGUI_ModelSrvCltConnection_h
