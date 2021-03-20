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

#ifndef ZSDataGUI_DatabaseConnectionsModel_h
#define ZSDataGUI_DatabaseConnectionsModel_h

#include <QtCore/qabstractitemmodel.h>
#if QT_VERSION < 0x050000
#include <QtGui/qitemdelegate.h>
#else
#include <QtWidgets/qitemdelegate.h>
#endif

#include "ZSDataGUI/ZSDataGUIDllMain.h"
#include "ZSData/ZSDataCommon.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDb;
class CDbClient;
struct SDbClientRegSpec;

namespace GUI
{
class CModelDbConnections;

//******************************************************************************
class ZSDATAGUIDLL_API CDelegateDbConnectedNodes : public QItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CDelegateDbConnectedNodes( QObject* i_pObjParent = nullptr );
    virtual ~CDelegateDbConnectedNodes();
public: // instance methods
    void setSocketId( int i_iSocketId );
public: // instance methods
    void setDb( CDb* i_pDb );
    CDb* getDb() const { return m_pDb; }
public: // instance methods
    void setModel( QAbstractItemModel* i_pModel );
public: // overridables of base class QItemDelegate
    virtual QWidget* createEditor(
        QWidget*                    i_pWdgtParent,
        const QStyleOptionViewItem& i_styleOption,
        const QModelIndex&          i_modelIdx ) const;
    virtual void updateEditorGeometry(
        QWidget*                    i_pWdgtEditor,
        const QStyleOptionViewItem& i_styleOption,
        const QModelIndex&          i_modelIdx ) const;
protected slots:
    void onDbDestroyed( QObject* i_pDb );
protected: // instance members
    CDb*                 m_pDb;
    CModelDbConnections* m_pModel;

}; // class CDelegateDbConnectedNodes


//******************************************************************************
class ZSDATAGUIDLL_API CModelDbConnections : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::DataGUI::Db"; }
    static QString ClassName() { return "CModelDbConnections"; }
public: // type definitions and constants
    typedef enum
    {
        EColumnProtocolType     = 0,
        EColumnSocketType       = 1,
        EColumnClientName       = 2,
        EColumnRegisteredNodes  = 3,
        EColumnSocketId         = 4,
        EColumnSocketState      = 5,
        EColumnServerListenPort = 6,
        EColumnLocalPort        = 7,
        EColumnRemoteHostName   = 8,
        EColumnRemotePort       = 9,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelDbConnections( QObject* i_pObjParent = nullptr );
    virtual ~CModelDbConnections();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setDb( CDb* i_pDb );
    CDb* getDb() const { return m_pDb; }
    void setDbClient( CDbClient* i_pDbClient );
    CDbClient* getDbClient() const { return m_pDbClient; }
public: // instance methods
    void update(); // must be called after creating the model and/or changing the protocol and/or connection type.
public: // instance methods
    void setProtocolType( EProtocolType i_protocolType ); // Undefined is allowed. "update" must be called afterwards.
    EProtocolType getProtocolType() const { return m_protocolType; }
public: // instance methods
    void setSocketType( Ipc::ESocketType i_socketType ); // Undefined is allowed. "update" must be called afterwards.
    Ipc::ESocketType getSocketType() const { return m_socketType; }
protected slots:
    void onConnected( QObject* i_pSrvClt, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onDisconnected( QObject* i_pSrvClt, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onClientRegistered( QObject* i_pSrvClt, const ZS::Data::SDbClientRegSpec& i_regSpec );
    void onDbDestroyed( QObject* i_pDb );
    void onDbClientDestroyed( QObject* i_pDbClient );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelDbConnections( const CModelDbConnections& );
private: // assignment operator not allowed
    CModelDbConnections& operator = ( const CModelDbConnections& );
protected: // instance members
    CDb*                 m_pDb;
    CDbClient*           m_pDbClient;
    EProtocolType        m_protocolType;
    Ipc::ESocketType     m_socketType;
    Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CModelDbConnections

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUI_DatabaseConnectionsModel_h
