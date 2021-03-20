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

#include "ZSRemCmd/ZSRemCmdsSyst.h"
#include "ZSRemCmd/ZSRemCmdErrNumbers.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdSubSysNode.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdParser.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::RemCmd;


/*******************************************************************************
class CRemCmdsSyst : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdsSyst::CRemCmdsSyst(
    CRemCmdObjPool* i_pRemCmdObjPool,
    CRemCmdParser*  i_pRemCmdParser ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pRemCmdObjPool(i_pRemCmdObjPool),
    m_pRemCmdParser(i_pRemCmdParser),
    m_pRemCmdCommCat(nullptr),
    m_pRemCmdCls(nullptr),
    // Subsystem SYSTem
    m_pRemCmdSubSysNodeSyst(nullptr),
    // Subsystem SYSTem:ERRor
    m_pRemCmdSystErr(nullptr),
    m_pRemCmdSystErrAll(nullptr),
    m_pRemCmdSystErrNext(nullptr),
    m_pRemCmdSystErrCoun(nullptr),
    m_pRemCmdSystErrCodeAll(nullptr),
    m_pRemCmdSystErrCodeNext(nullptr),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("SYSTem");

    // Create trace admin object
    //====================================

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdsSyst", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Subsystem: COMMands (object pool)
    //===========================================================================

    //---------------------------------------------------------------------------
    m_pRemCmdCommCat = new CRemCmdObj( m_pRemCmdObjPool, "COMMands:CATalog" );
    //---------------------------------------------------------------------------

    m_pRemCmdCommCat->setSyntax("COMMands:CATalog?");
    //m_pRemCmdCommCat->setIsQueryOnly(true);
    m_pRemCmdCommCat->setDescription("Returns a comma separated list of all available remote commands.");

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdCommCat,
        /* szSignal     */ SIGNAL( execute(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExecuteRemCmdCommCat(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Subsystem: 
    //===========================================================================

    //---------------------------------------------------------------------------
    m_pRemCmdCls = new CRemCmdObj( m_pRemCmdObjPool, "*CLS" );
    //---------------------------------------------------------------------------

    m_pRemCmdCls->setSyntax("*CLS");
    m_pRemCmdCls->setIsEventOnly(true);
    m_pRemCmdCls->setDescription("Clears the instrument. This includes clearing the error/event queue.");

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdCls,
        /* szSignal     */ SIGNAL( execute(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExecuteRemCmdCls(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Subsystem: SYSTem
    //===========================================================================

    //---------------------------------------------------------------------------
    m_pRemCmdSubSysNodeSyst = new CRemCmdSubSysNode( m_pRemCmdObjPool, "SYST" );
    //---------------------------------------------------------------------------

    m_pRemCmdSubSysNodeSyst->setDescription("");

    // Subsystem SYSTem:ERRor
    //===========================================================================

    //---------------------------------------------------------------------------
    m_pRemCmdSystErr = new CRemCmdObj( m_pRemCmdObjPool, "SYST:ERR" );
    //---------------------------------------------------------------------------

    m_pRemCmdSystErr->setSyntax("SYSTem:ERRor?");
    //m_pRemCmdSystErr->setIsQueryOnly(true);
    m_pRemCmdSystErr->setDescription(
        "Queries the error/event queue for the next item and removes it "
        "from the queue. The response returns the full queue item consisting of an integer and a string "
        "as described in the introduction to the SYSTem:ERRor subsystem." );

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdSystErr,
        /* szSignal     */ SIGNAL( execute(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExecuteRemCmdSystErr(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    //---------------------------------------------------------------------------
    m_pRemCmdSystErrAll = new CRemCmdObj( m_pRemCmdObjPool, "SYST:ERR:ALL" );
    //---------------------------------------------------------------------------

    m_pRemCmdSystErrAll->setSyntax("SYSTem:ERRor:ALL?");
    //m_pRemCmdSystErrAll->setIsQueryOnly(true);
    m_pRemCmdSystErrAll->setDescription(
        "Queries the error/event queue for all the unread items and removes "
        "them from the queue. The response is a comma separated list of number, "
        "string pairs in FIFO order. If the queue is empty, the response is 0 (No error)."
        "<br>Note: If the queue is not empty, the 0 (No error) is not sent." );

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdSystErrAll,
        /* szSignal     */ SIGNAL( execute(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExecuteRemCmdSystErrAll(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    //---------------------------------------------------------------------------
    m_pRemCmdSystErrNext = new CRemCmdObj( m_pRemCmdObjPool, "SYST:ERR:NEXT" );
    //---------------------------------------------------------------------------

    m_pRemCmdSystErrNext->setSyntax("SYSTem:ERRor:NEXT?");
    //m_pRemCmdSystErrNext->setIsQueryOnly(true);
    m_pRemCmdSystErrNext->setDescription(
        "Queries the error/event queue for the next item and removes it "
        "from the queue. The response returns the full queue item consisting of an integer and a string "
        "as described in the introduction to the SYSTem:ERRor subsystem." );

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdSystErrNext,
        /* szSignal     */ SIGNAL( execute(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExecuteRemCmdSystErr(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    //---------------------------------------------------------------------------
    m_pRemCmdSystErrCoun = new CRemCmdObj( m_pRemCmdObjPool, "SYST:ERR:COUN" );
    //---------------------------------------------------------------------------

    m_pRemCmdSystErrCoun->setSyntax("SYSTem:ERRor:COUNt?");
    //m_pRemCmdSystErrCoun->setIsQueryOnly(true);
    m_pRemCmdSystErrCoun->setDescription(
        "Queries the error/event queue for the number of unread items. As "
        "errors and events may occur at any time, more items may be present in the queue at the time "
        "it is actually read."
        "<br>Note: If the queue is empty, the response is 0." );

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdSystErrCoun,
        /* szSignal     */ SIGNAL( execute(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExecuteRemCmdSystErrCoun(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    //---------------------------------------------------------------------------
    m_pRemCmdSystErrCodeAll = new CRemCmdObj( m_pRemCmdObjPool, "SYST:ERR:CODE:ALL" );
    //---------------------------------------------------------------------------

    m_pRemCmdSystErrCodeAll->setSyntax("SYSTem:ERRor:CODE:ALL?");
    //m_pRemCmdSystErrCodeAll->setIsQueryOnly(true);
    m_pRemCmdSystErrCodeAll->setDescription(
        "Queries the error/event queue for all the unread items and "
        "removes them from the queue. The response returns a comma separated list of only the "
        "error/event code numbers in FIFO order. If the queue is empty, the response is 0."
        "<br>Note: If the queue is not empty, the 0 is not sent." );

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdSystErrCodeAll,
        /* szSignal     */ SIGNAL( execute(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExecuteRemCmdSystErrCodeAll(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    //---------------------------------------------------------------------------
    m_pRemCmdSystErrCodeNext = new CRemCmdObj( m_pRemCmdObjPool, "SYST:ERR:CODE:NEXT" );
    //---------------------------------------------------------------------------

    m_pRemCmdSystErrCodeNext->setSyntax("SYSTem:ERRor:CODE:NEXT?");
    //m_pRemCmdSystErrCodeNext->setIsQueryOnly(true);
    m_pRemCmdSystErrCodeNext->setDescription(
        "Queries the error/event queue for the next item and "
        "removes it from the queue. The response returns only the error/event code number omitting "
        "the string. Except for the shortened response, the query operates identically to "
        "SYSTem:ERRor[:NEXT]?" );

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdSystErrCodeNext,
        /* szSignal     */ SIGNAL( execute(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExecuteRemCmdSystErrCodeNext(const ZS::Ipc::SSocketDscr&,bool,const QByteArray&,QByteArray&,ZS::System::CRequest*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CRemCmdsSyst::~CRemCmdsSyst()
//------------------------------------------------------------------------------
{
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    // Invalidate instance members
    //----------------------------

    m_pRemCmdObjPool = nullptr;
    m_pRemCmdParser = nullptr;
    // Subsystem: COMMands (object pool)
    m_pRemCmdCommCat = nullptr;
    // Subsystem:
    m_pRemCmdCls = nullptr;
    // Subsystem SYSTem
    m_pRemCmdSubSysNodeSyst = nullptr;
    // Subsystem SYSTem:ERRor
    //m_iSystErrResultInfoQueueMaxEntries;
    //m_arSystErrResultInfoQueue;
    m_pRemCmdSystErr = nullptr;
    m_pRemCmdSystErrAll = nullptr;
    m_pRemCmdSystErrNext = nullptr;
    m_pRemCmdSystErrCoun = nullptr;
    m_pRemCmdSystErrCodeAll = nullptr;
    m_pRemCmdSystErrCodeNext = nullptr;
    // Method Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected slots: // Subsystem: COMMands (object pool)
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdsSyst::onExecuteRemCmdCommCat(
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    CRequest*                   /*io_pReq*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);

        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else if( i_byteArrArgs.size() <= 50 )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs.left(5) + ".." + i_byteArrArgs.right(5);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onExecuteRemCmdCommCat",
        /* szAddInfo    */ strAddTrcInfo );

    QString    strCmdCat = m_pRemCmdObjPool->toXmlString();
    QByteArray byteArrCmdCat = str2ByteArr(strCmdCat);

    o_byteArrResults = byteArrCmdCat;

} // onExecuteRemCmdCommCat

/*==============================================================================
protected slots: // Subsystem: 
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdsSyst::onExecuteRemCmdCls(
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 /*o_byteArrResults*/,
    CRequest*                   /*io_pReq*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);

        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else if( i_byteArrArgs.size() <= 50 )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs.left(5) + ".." + i_byteArrArgs.right(5);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onExecuteRemCmdCls",
        /* szAddInfo    */ strAddTrcInfo );

    CErrLog::GetInstance()->clear(EResultSeverityCritical);
    CErrLog::GetInstance()->clear(EResultSeverityError);

} // onExecuteRemCmdCls

/*==============================================================================
protected slots: // Subsystem: SYSTem:ERRor
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdsSyst::onExecuteRemCmdSystErr(
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    CRequest*                   /*io_pReq*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);

        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else if( i_byteArrArgs.size() <= 50 )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs.left(5) + ".." + i_byteArrArgs.right(5);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onExecuteRemCmdSystErr",
        /* szAddInfo    */ strAddTrcInfo );

    // When all errors/events have been read from the queue, further error/event
    // queries shall return 0, “No error”.

    SErrLogEntry errLogEntry;
    QByteArray   byteArrErrResultInfo;

    CErrLog::GetInstance()->lock();

    if( (CErrLog::GetInstance()->getEntryCount(EResultSeverityCritical)) > 0 || (CErrLog::GetInstance()->getEntryCount(EResultSeverityError) > 0) )
    {
        errLogEntry = CErrLog::GetInstance()->takeFirstEntry();

        byteArrErrResultInfo = errResultInfo2ByteArr(errLogEntry.m_errResultInfo);
    }
    else // if( (CErrLog::GetInstance()->getEntryCount(EResultSeverityCritical)) == 0 && (CErrLog::GetInstance()->getEntryCount(EResultSeverityError) == 0) )
    {
        byteArrErrResultInfo = QByteArray::number(EErrNrNoError) + ", \"" + errNr2ByteArr(EErrNrNoError) + "\"";
    }

    CErrLog::GetInstance()->unlock();

    o_byteArrResults = byteArrErrResultInfo;

} // onExecuteRemCmdSystErr

//------------------------------------------------------------------------------
void CRemCmdsSyst::onExecuteRemCmdSystErrAll(
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    CRequest*                   /*io_pReq*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);

        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else if( i_byteArrArgs.size() <= 50 )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs.left(5) + ".." + i_byteArrArgs.right(5);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onExecuteRemCmdSystErrAll",
        /* szAddInfo    */ strAddTrcInfo );

    // The response is a comma separated list of number, string pairs in FIFO order.
    // If the queue is empty, the response is 0 (No error).
    // Note: If the queue is not empty, the 0 (No error) is not sent.
    // Where Date and Time are desired to be returned with the error message, they should be
    // appended to the <device_dependent_information> separated by a semi-colon using the
    // following format: <date><space><time>
    // - date - yyyy/mm/dd - year/month/day
    // - time - hh:mm:ss.sss - 24 hour time
    // Example:
    // -200, "Execution error;Torque Safety Limit Exceeded;1996/08/15 13:22:51.01"

    CErrLog::GetInstance()->lock();

    QByteArray byteArrErrResultInfo;

    int iErrCount = CErrLog::GetInstance()->getEntryCount(EResultSeverityCritical)
                  + CErrLog::GetInstance()->getEntryCount(EResultSeverityError);

    if( iErrCount > 0 )
    {
        SErrLogEntry errLogEntry;
        int          iUserErrCode;
        QByteArray   byteArrUserErrCode;
        int          idxErr = 0;

        while( iErrCount > 0 )
        {
            errLogEntry = CErrLog::GetInstance()->takeFirstEntry();

            if( idxErr > 0 )
            {
                o_byteArrResults += m_pRemCmdParser->getArgsSeparator();
                o_byteArrResults += " ";
            }

            iUserErrCode = sysErrResultInfo2ErrNr(errLogEntry.m_errResultInfo);

            byteArrUserErrCode = errNr2ByteArr(iUserErrCode);

            byteArrErrResultInfo  = QByteArray::number(iUserErrCode);
            byteArrErrResultInfo += ", \"";
            byteArrErrResultInfo += byteArrUserErrCode;

            if( !errLogEntry.m_errResultInfo.getAddErrInfoDscr().isEmpty() )
            {
                byteArrErrResultInfo += "; " + errLogEntry.m_errResultInfo.getAddErrInfoDscr().remove("\"");
            }
            byteArrErrResultInfo += "\"";

            o_byteArrResults += byteArrErrResultInfo;

            iErrCount = CErrLog::GetInstance()->getEntryCount(EResultSeverityCritical)
                      + CErrLog::GetInstance()->getEntryCount(EResultSeverityError);

            idxErr++;

        } // while( iErrCount > 0 )
    }
    else // if( iErrCount == 0) )
    {
        byteArrErrResultInfo = QByteArray::number(EErrNrNoError) + ", \"" + errNr2ByteArr(EErrNrNoError) + "\"";

        o_byteArrResults = byteArrErrResultInfo;
    }

    CErrLog::GetInstance()->unlock();

} // onExecuteRemCmdSystErrAll

//------------------------------------------------------------------------------
void CRemCmdsSyst::onExecuteRemCmdSystErrCoun(
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    CRequest*                   /*io_pReq*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);

        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else if( i_byteArrArgs.size() <= 50 )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs.left(5) + ".." + i_byteArrArgs.right(5);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onExecuteRemCmdSystErrCoun",
        /* szAddInfo    */ strAddTrcInfo );

    CErrLog::GetInstance()->lock();

    int iErrCount = CErrLog::GetInstance()->getEntryCount(EResultSeverityCritical)
                  + CErrLog::GetInstance()->getEntryCount(EResultSeverityError);

    o_byteArrResults = QByteArray::number(iErrCount);

    CErrLog::GetInstance()->unlock();

} // onExecuteRemCmdSystErrCoun

//------------------------------------------------------------------------------
void CRemCmdsSyst::onExecuteRemCmdSystErrCodeAll(
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    CRequest*                   /*io_pReq*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);

        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else if( i_byteArrArgs.size() <= 50 )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs.left(5) + ".." + i_byteArrArgs.right(5);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onExecuteRemCmdSystErrCodeAll",
        /* szAddInfo    */ strAddTrcInfo );

    CErrLog::GetInstance()->lock();

    QByteArray   byteArrErrResultInfo;

    int iErrCount = CErrLog::GetInstance()->getEntryCount(EResultSeverityCritical)
                  + CErrLog::GetInstance()->getEntryCount(EResultSeverityError);

    if( iErrCount > 0 )
    {
        SErrLogEntry errLogEntry;
        int          iUserErrCode;
        int          idxErr = 0;

        while( iErrCount > 0 )
        {
            errLogEntry = CErrLog::GetInstance()->takeFirstEntry();

            if( idxErr > 0 )
            {
                o_byteArrResults += m_pRemCmdParser->getArgsSeparator();
                o_byteArrResults += " ";
            }

            iUserErrCode = sysErrResultInfo2ErrNr(errLogEntry.m_errResultInfo);

            byteArrErrResultInfo = QByteArray::number(iUserErrCode);

            o_byteArrResults += byteArrErrResultInfo;

            iErrCount = CErrLog::GetInstance()->getEntryCount(EResultSeverityCritical)
                      + CErrLog::GetInstance()->getEntryCount(EResultSeverityError);

            idxErr++;

        } // while( iErrCount > 0 )
    }
    else // if( iErrCount == 0) )
    {
        byteArrErrResultInfo = QByteArray::number(EErrNrNoError);

        o_byteArrResults = byteArrErrResultInfo;
    }

    CErrLog::GetInstance()->unlock();

} // onExecuteRemCmdSystErrCodeAll

//------------------------------------------------------------------------------
void CRemCmdsSyst::onExecuteRemCmdSystErrCodeNext(
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    CRequest*                   /*io_pReq*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);

        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else if( i_byteArrArgs.size() <= 50 )
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs;
        }
        else
        {
            strAddTrcInfo += ", Args: " + i_byteArrArgs.left(5) + ".." + i_byteArrArgs.right(5);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onExecuteRemCmdSystErrCodeNext",
        /* szAddInfo    */ strAddTrcInfo );

    CErrLog::GetInstance()->lock();

    QByteArray byteArrErrResultInfo;

    int iErrCount = CErrLog::GetInstance()->getEntryCount(EResultSeverityCritical)
                  + CErrLog::GetInstance()->getEntryCount(EResultSeverityError);

    if( iErrCount > 0 )
    {
        SErrLogEntry errLogEntry = CErrLog::GetInstance()->takeFirstEntry();

        int iUserErrCode = sysErrResultInfo2ErrNr(errLogEntry.m_errResultInfo);

        byteArrErrResultInfo  = QByteArray::number(iUserErrCode);
    }
    else // if( iErrCount == 0 )
    {
        byteArrErrResultInfo = QByteArray::number(EErrNrNoError);
    }

    CErrLog::GetInstance()->unlock();

    o_byteArrResults = byteArrErrResultInfo;

} // onExecuteRemCmdSystErrCodeNext
