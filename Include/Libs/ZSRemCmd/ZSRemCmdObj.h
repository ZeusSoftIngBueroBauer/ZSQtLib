/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSRemCmd_Obj_h
#define ZSRemCmd_Obj_h

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysRequest.h"

namespace ZS
{
namespace System
{
class CObjPoolListEntry;
class CRequest;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace RemCmd
{
class CRemCmdObjPool;
class CRemCmdServer;

//******************************************************************************
// Function prototypes
//******************************************************************************

typedef ZS::System::CRequest* (*TFctExecute)(
    QObject*                    i_pObj,
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    qint64                      i_iReqIdParent );


//******************************************************************************
class ZSREMCMDDLL_API CRemCmdObj : public QObject
//******************************************************************************
{
friend class CRemCmdObjPool;
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::RemCmd"; }
    static QString ClassName() { return "CRemCmdObj"; }
public: // ctors at servers side only
    CRemCmdObj(
        CRemCmdServer* i_pServer,
        const QString& i_strCmd,
        int            i_iObjId = -1 );
    CRemCmdObj(
        CRemCmdServer* i_pServer,
        const QString& i_strCmd,
        QObject*       i_pObjFctExecute,
        TFctExecute    i_fctExecute,
        int            i_iObjId = -1 );
public: // ctors at clients side
    CRemCmdObj(
        CRemCmdObjPool* i_pObjPool,
        const QString&  i_strCmd,
        int             i_iObjId = -1 );
public: // dtor
    virtual ~CRemCmdObj();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void changed( QObject* i_pRemCmdObj );
    void execute(
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        ZS::System::CRequest*       io_pReq );
public: // instance methods
    QString getCommandStr() const { return m_strCmd; }
public: // instance methods
    QString getSyntax() const { return m_strSyntax; }
    void setSyntax( const QString& i_strSyntax );
public: // instance methods
    bool isQueryOnly() const { return m_bIsQueryOnly; }
    void setIsQueryOnly( bool i_bIs );
public: // instance methods
    bool isEventOnly() const { return m_bIsEventOnly; }
    void setIsEventOnly( bool i_bIs );
public: // instance methods
    QString getDescription() const { return m_strDscr; }
    void setDescription( const QString& i_strDscr );
public: // instance methods
    int getObjId() const { return m_iObjId; }
public: // instance methods
    QString toXmlString() const;
public: // instance methods
    void setObjFctExecute( QObject* i_pObjFctExecute );
    QObject* getObjFctExecute() { return m_pObjFctExecute; }
    void setFctExecute( TFctExecute i_fctExecute );
    TFctExecute getFctExecute() const { return m_fctExecute; }
public: // overridables
    virtual ZS::System::CRequest* executeCommand(
        bool              i_bIsQuery,
        const QByteArray& i_byteArrArgs,
        QByteArray&       o_byteArrResults,
        int               i_iTimeout_ms  = 0,
        bool              i_bWait        = false,
        qint64            i_iReqIdParent = -1 );
    virtual ZS::System::CRequest* executeCommand(
        ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                  i_bIsQuery,
        const QByteArray&     i_byteArrArgs,
        QByteArray&           o_byteArrResults,
        int                   i_iTimeout_ms  = 0,
        bool                  i_bWait        = false,
        qint64                i_iReqIdParent = -1 );
public: // instance methods (experts use only)
    ZS::System::CObjPoolListEntry* getListEntry() { return m_pListEntry; }
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
protected slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected: // instance members
    CRemCmdServer*                      m_pServer;
    CRemCmdObjPool*                     m_pObjPool;
    // Command specification
    QString                             m_strCmd;
    QString                             m_strSyntax;
    bool                                m_bIsQueryOnly;   // IsQueryOnly and IsEventOnly are mutual exclusive.
    bool                                m_bIsEventOnly;   // Not both flags can be true at the same time.
    QString                             m_strDscr;
    QObject*                            m_pObjFctExecute;
    TFctExecute                         m_fctExecute;
    int                                 m_iObjId;
    ZS::System::CObjPoolListEntry*      m_pListEntry;
    // State Machine
    // Please note that the request queue cannot be used as the command
    // object may execute several commands simultaneously.
    QHash<qint64,ZS::System::CRequest*> m_hshpReqsInProgress;
    ZS::System::CRequest*               m_pReqSyncToBeDeletedLater;
    // Method Trace
    ZS::Trace::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CRemCmdObj

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmd_Obj_h

