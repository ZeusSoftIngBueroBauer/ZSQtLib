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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtXml/qdom.h>

#include "ZSRemCmd/ZSRemCmdClient.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdSubSysNode.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdParser.h"
#include "ZSIpc/ZSIpcBlkTypeL.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
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
class CRemCmdClient : public ZS::Ipc::CClient
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdClient::CRemCmdClient( const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    CClient(i_strObjName, false),
    m_pRemCmdParser(nullptr),
    m_pRemCmdObjPool(nullptr),
    m_pRemCmdCommCat(nullptr),
    m_pTrcAdminObj(nullptr)
{
    QString strObjName = i_strObjName;

    if( strObjName.isEmpty() )
    {
        strObjName = "RemCmdClient";
    }
    setObjectName(strObjName);

    CBlkType* pBlkType = new CBlkTypeL(EBlkLenDataTypeBinUINT32);
    setBlkType(pBlkType);
    pBlkType = nullptr;

    // Create trace admin object
    //--------------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdClient", i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Create Remote Command Parser
    //-----------------------------

    m_pRemCmdParser = new CRemCmdParser(i_strObjName + "Parser");

    // Create Command Object Pool
    //---------------------------

    m_pRemCmdObjPool = new CRemCmdObjPool(i_strObjName + "ObjPool");

    createRemoteCommands();

    // Keeping indicated parameters up to date
    //----------------------------------------

    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL( connected(QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onIpcClientConnected(QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL( disconnected(QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onIpcClientDisconnected(QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CRemCmdClient::~CRemCmdClient()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // Destroy instance members
    //-------------------------

    try
    {
        delete m_pRemCmdObjPool;
    }
    catch(...)
    {
    }
    m_pRemCmdObjPool = nullptr;

    try
    {
        delete m_pRemCmdParser;
    }
    catch(...)
    {
    }
    m_pRemCmdParser = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    // Invalidate instance members
    //----------------------------

    m_pRemCmdObjPool = nullptr;
    m_pRemCmdCommCat = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected: // instance methods (belonging to ctor)
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdClient::createRemoteCommands()
//------------------------------------------------------------------------------
{
    //--------------------------------------------------------------------------
    m_pRemCmdCommCat = new CRemCmdObj( m_pRemCmdObjPool, "COMMands:CATalog" );
    //--------------------------------------------------------------------------

    m_pRemCmdCommCat->setSyntax("COMMands:CATalog?");
    m_pRemCmdCommCat->setIsQueryOnly(true);
    m_pRemCmdCommCat->setDescription("Returns a comma separated list of all available remote commands.");

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdCommCat,
        /* szSignal     */ SIGNAL( destroyed(QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onRemCmdCommCatDestroyed(QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // createRemoteCommands

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdSubSysNode* CRemCmdClient::getRemCmdSubSysNode( const QString& i_strNode )
//------------------------------------------------------------------------------
{
    CRemCmdSubSysNode* pRemCmdSubSysNode = nullptr;

    CObjPoolTreeEntry* pTreeEntry = m_pRemCmdObjPool->getTreeEntry(i_strNode,EObjPoolEntryTypeNameSpace);

    if( pTreeEntry != nullptr )
    {
        pRemCmdSubSysNode = reinterpret_cast<CRemCmdSubSysNode*>(pTreeEntry->getObj());
    }
    return pRemCmdSubSysNode;

} // getRemCmdSubSysNode

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdObj* CRemCmdClient::getRemCmdObj( const QString& i_strCmd )
//------------------------------------------------------------------------------
{
    return reinterpret_cast<CRemCmdObj*>(m_pRemCmdObjPool->getObj(i_strCmd));
}

//------------------------------------------------------------------------------
CRemCmdObj* CRemCmdClient::getRemCmdObj( int i_iObjId )
//------------------------------------------------------------------------------
{
    return reinterpret_cast<CRemCmdObj*>(m_pRemCmdObjPool->getObj(i_iObjId));
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CRemCmdClient::parseData(
    const QByteArray&  i_byteArrData,
    QString*           o_pstrCmd,
    bool*              o_pbIsQuery,
    QByteArray*        o_pByteArrArgs ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_byteArrData;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "parseData",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo( objectName(), EResultSuccess );

    if( m_pRemCmdParser != nullptr )
    {
        errResultInfo = m_pRemCmdParser->parse(
            /* byteArrData */ i_byteArrData,
            /* pstrCmd     */ o_pstrCmd,
            /* pbIsQuery   */ o_pbIsQuery,
            /* pstrlstArgs */ o_pByteArrArgs );
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        if( errResultInfo.isErrorResult() )
        {
            strAddTrcInfo = errResultInfo.getSeverityStr() + " " + errResultInfo.getResultStr() + " " + errResultInfo.getAddErrInfoDscr();
        }
        else
        {
            strAddTrcInfo = "Ok";
        }

        if( o_pstrCmd != nullptr )
        {
            strAddTrcInfo += ", Cmd: " + *o_pstrCmd;
        }
        if( o_pbIsQuery != nullptr )
        {
            strAddTrcInfo += ", IsQuery: " + bool2Str(*o_pbIsQuery);
        }
        if( o_pByteArrArgs != nullptr )
        {
            strAddTrcInfo += ", Args: " + *o_pByteArrArgs;
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return errResultInfo;

} // parseData

/*==============================================================================
public: // instance methods to send commands to the connected server
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CRemCmdClient::sendCommand( const QString& i_strCmd, const QByteArray& i_byteArrArgs )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Cmd: " + i_strCmd;
        strAddTrcInfo += ", Args: " + i_byteArrArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "sendCommand",
        /* strAddInfo   */ strAddTrcInfo );

    QByteArray strData;

    #if QT_VERSION < 0x050000
    strData = i_strCmd.toAscii();
    #else
    strData = i_strCmd.toLatin1();
    #endif

    if( i_byteArrArgs.size() > 0 )
    {
        strData += m_pRemCmdParser->getCmdSeparator();
        strData += i_byteArrArgs;
    }

    CRequest* pReq = sendData(strData);

    if( !isAsynchronousRequest(pReq) && !isErrorResult(pReq) )
    {
        emit commandSent( this, i_strCmd, i_byteArrArgs );
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Result: SUCCESS";
        }
        else
        {
            strAddTrcInfo = "Result: " + pReq->getResultStr();
        }
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    return pReq;

} // sendCommand

//------------------------------------------------------------------------------
CRequest* CRemCmdClient::sendCommand( CRemCmdObj* i_pRemCmdObj, const QByteArray& i_byteArrArgs )
//------------------------------------------------------------------------------
{
    CRequest* pReq;

    #if QT_VERSION < 0x050000
    pReq = sendCommand( i_pRemCmdObj->getCommandStr().toAscii(), i_byteArrArgs );
    #else
    pReq = sendCommand( i_pRemCmdObj->getCommandStr().toLatin1(), i_byteArrArgs );
    #endif

    return pReq;

} // sendCommand

/*==============================================================================
protected: // overridables of base class Ipc::CClient
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdClient::onReceivedData( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_byteArr;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedData",
        /* strAddInfo   */ strAddTrcInfo );

    QString strData = byteArr2Str(i_byteArr);

    if( strData.endsWith("\n") )
    {
        strData.remove(strData.length()-1,1);
    }

    if( strData.startsWith("<RemCmds>") && strData.endsWith("</RemCmds>") )
    {
        QDomDocument domdoc;

        domdoc.setContent(strData);

        QDomElement domelem = domdoc.documentElement();
        QString     strDomNodeName = domelem.nodeName();
        QDomNode    domnodeLev1;
        QDomElement domelemLev1;
        QString     strDomNodeNameLev1;
        QDomNode    domnodeLev2;
        QDomElement domelemLev2;
        QString     strDomNodeNameLev2;

        if( strDomNodeName == "RemCmds" )
        {
            CRemCmdSubSysNode* pRemCmdSubSysNode = nullptr;
            CRemCmdObj*        pRemCmdObj = nullptr;
            QString            strCmd;
            QString            strNode;
            QString            strSyntax;
            bool               bIsQueryOnly;
            bool               bIsEventOnly;
            QString            strDscr;
            int                iObjId;
            QString            strTmp;
            QString            strAddErrInfo;

            for( domnodeLev1 = domelem.firstChild(); !domnodeLev1.isNull(); domnodeLev1 = domnodeLev1.nextSibling() )
            {
                if( domnodeLev1.isElement() )
                {
                    domelemLev1 = domnodeLev1.toElement();
                    strDomNodeNameLev1 = domnodeLev1.nodeName();

                    if( strDomNodeNameLev1 == "RemCmdObj" )
                    {
                        strCmd       = "";
                        iObjId       = -1;
                        strSyntax    = "";
                        bIsQueryOnly = false;
                        bIsEventOnly = false;
                        strDscr      = "";

                        for( domnodeLev2 = domelemLev1.firstChild(); !domnodeLev2.isNull(); domnodeLev2 = domnodeLev2.nextSibling() )
                        {
                            if( domnodeLev2.isElement() )
                            {
                                domelemLev2 = domnodeLev2.toElement();
                                strDomNodeNameLev2 = domnodeLev2.nodeName();

                                if( strDomNodeNameLev2 == "Cmd" )
                                {
                                    strCmd = domelemLev2.text();
                                }
                                else if( strDomNodeNameLev2 == "ObjId" )
                                {
                                    strTmp = domelemLev2.text();
                                    iObjId = strTmp.toInt();
                                }
                                else if( strDomNodeNameLev2 == "Syntax" )
                                {
                                    strSyntax = domelemLev2.text();
                                }
                                else if( strDomNodeNameLev2 == "QueryOnly" )
                                {
                                    strTmp = domelemLev2.text();
                                    bIsQueryOnly = str2Bool(strTmp);
                                }
                                else if( strDomNodeNameLev2 == "EventOnly" )
                                {
                                    strTmp = domelemLev2.text();
                                    bIsEventOnly = str2Bool(strTmp);
                                }
                                else if( strDomNodeNameLev2 == "Dscr" )
                                {
                                    strDscr = domelemLev2.text();
                                }
                            } // if( domnodeLev2.isElement() )
                        } // for( !domnodeLev2.isNull() )

                        //if( strCmd.contains("&lt;") )
                        //{
                        //    strCmd.replace("&lt;","<");
                        //}
                        //if( strCmd.contains("&gt;") )
                        //{
                        //    strCmd.replace("&gt;",">");
                        //}

                        //if( strDscr.contains("&lt;") )
                        //{
                        //    strDscr.replace("&lt;","<");
                        //}
                        //if( strDscr.contains("&gt;") )
                        //{
                        //    strDscr.replace("&gt;",">");
                        //}

                        if( strCmd.isEmpty() )
                        {
                            if( m_pErrLog != nullptr )
                            {
                                SErrResultInfo errResultInfo = ErrResultInfoError("onReceivedData", EResultInvalidMessageContent, "Command string is empty");
                                m_pErrLog->addEntry(errResultInfo);
                            }
                        }
                        else // if( !strCmd.isEmpty() )
                        {
                            pRemCmdObj = getRemCmdObj(strCmd);

                            if( pRemCmdObj != nullptr )
                            {
                                if( pRemCmdObj->getObjId() != iObjId )
                                {
                                    if( m_pErrLog != nullptr )
                                    {
                                        QString strAddErrInfo  = "Cmd: " + pRemCmdObj->getCommandStr();
                                        strAddErrInfo += "; ObjId: " + QString::number(pRemCmdObj->getObjId());
                                        strAddErrInfo += "; Received Id: " + QString::number(iObjId);
                                        SErrResultInfo errResultInfo = ErrResultInfoError("onReceivedData", EResultInvalidObjId, strAddErrInfo);
                                        m_pErrLog->addEntry(errResultInfo);
                                    }
                                }
                            }
                            else // if( pRemCmdObj != nullptr )
                            {
                                pRemCmdObj = new CRemCmdObj(
                                    /* pModel  */ m_pRemCmdObjPool,
                                    /* strPath */ strCmd,
                                    /* iObjId  */ iObjId );
                            }

                            pRemCmdObj->setSyntax(strSyntax);
                            pRemCmdObj->setIsQueryOnly(bIsQueryOnly);
                            pRemCmdObj->setIsEventOnly(bIsEventOnly);
                            pRemCmdObj->setDescription(strDscr);

                        } // if( !strCmd.isEmpty() )
                    } // if( strDomNodeNameLev1 == "RemCmdObj" )

                    else if( strDomNodeNameLev1 == "SubSysNode" )
                    {
                        strNode = "";
                        strDscr = "";

                        for( domnodeLev2 = domelemLev1.firstChild(); !domnodeLev2.isNull(); domnodeLev2 = domnodeLev2.nextSibling() )
                        {
                            if( domnodeLev2.isElement() )
                            {
                                domelemLev2 = domnodeLev2.toElement();
                                strDomNodeNameLev2 = domnodeLev2.nodeName();

                                if( strDomNodeNameLev2 == "Node" )
                                {
                                    strNode = domelemLev2.text();
                                }
                                else if( strDomNodeNameLev2 == "Dscr" )
                                {
                                    strDscr = domelemLev2.text();
                                }
                            } // if( domnodeLev2.isElement() )
                        } // for( !domnodeLev2.isNull() )

                        //if( strNode.contains("&lt;") )
                        //{
                        //    strNode.replace("&lt;","<");
                        //}
                        //if( strNode.contains("&gt;") )
                        //{
                        //    strNode.replace("&gt;",">");
                        //}

                        //if( strDscr.contains("&lt;") )
                        //{
                        //    strDscr.replace("&lt;","<");
                        //}
                        //if( strDscr.contains("&gt;") )
                        //{
                        //    strDscr.replace("&gt;",">");
                        //}

                        if( strNode.isEmpty() )
                        {
                            if( m_pErrLog != nullptr )
                            {
                                SErrResultInfo errResultInfo = ErrResultInfoError("onReceivedData", EResultInvalidMessageContent, "Node string is empty");
                                m_pErrLog->addEntry(errResultInfo);
                            }
                        }
                        else // if( !strNode.isEmpty() )
                        {
                            pRemCmdSubSysNode = getRemCmdSubSysNode(strNode);

                            if( pRemCmdSubSysNode == nullptr )
                            {
                                pRemCmdSubSysNode = new CRemCmdSubSysNode(
                                    /* pModel  */ m_pRemCmdObjPool,
                                    /* strPath */ strNode );
                            }

                            pRemCmdSubSysNode->setDescription(strDscr);

                        } // if( !strNode.isEmpty() )
                    } // if( strDomNodeNameLev1 == "SubSysNode" )
                } // if( domnodeLev1.isElement() )
            } // for( !domnodeLev1.isNull() )
        } // if( strDomNodeName == "RemCmds" )
    } // if( strData.startsWith("<RemCmds>") && strData.endsWith("</RemCmds>") )

} // onReceivedData

/*==============================================================================
protected slots: // connected to the signals of the Ipc::Client (thats me)
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdClient::onIpcClientConnected( QObject* /*i_pIpcClient*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onIpcClientConnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pRemCmdCommCat == nullptr )
    {
        m_pRemCmdCommCat = new CRemCmdObj(
            /* pObjPool */ m_pRemCmdObjPool,
            /* strCmd   */ "COMMands:CATalog" );

        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdCommCat,
            /* szSignal     */ SIGNAL( destroyed(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdCommCatDestroyed(QObject*) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pRemCmdCommCat == nullptr )

} // onIpcClientConnected

//------------------------------------------------------------------------------
void CRemCmdClient::onIpcClientDisconnected( QObject* /*i_pIpcClient*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onIpcClientDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    m_pRemCmdObjPool->clear();

    if( m_pRemCmdCommCat == nullptr )
    {
        m_pRemCmdCommCat = new CRemCmdObj(
            /* pObjPool */ m_pRemCmdObjPool,
            /* strCmd   */ "COMMands:CATalog" );

        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdCommCat,
            /* szSignal     */ SIGNAL( destroyed(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdCommCatDestroyed(QObject*) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pRemCmdCommCat == nullptr )

} // onIpcClientDisconnected

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdClient::onRemCmdCommCatDestroyed( QObject* i_pRemCmdObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdCommCatDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pRemCmdObj == m_pRemCmdCommCat )
    {
        m_pRemCmdCommCat = nullptr;
    }

} // onRemCmdCommCatDestroyed
