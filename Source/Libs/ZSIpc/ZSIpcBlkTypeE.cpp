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

#include <QtCore/qglobal.h>

#include "ZSIpc/ZSIpcBlkTypeE.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Ipc;


/*******************************************************************************
class CBlkTypeE : public CBlkType
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CBlkTypeE::CBlkTypeE(
    QByteArray i_byteArrBlkEnd,
    bool       i_bInsertBlkEnd,
    bool       i_bRemoveBlkEnd ) :
//------------------------------------------------------------------------------
    CBlkType(EBlkTypeE),
    m_byteArrBlkEnd(i_byteArrBlkEnd),
    m_bInsertBlkEnd(i_bInsertBlkEnd),
    m_bRemoveBlkEnd(i_bRemoveBlkEnd)
{
} // ctor

//------------------------------------------------------------------------------
CBlkTypeE::CBlkTypeE( const CBlkTypeE& i_blkTypeOther ) :
//------------------------------------------------------------------------------
    CBlkType(EBlkTypeE),
    m_byteArrBlkEnd(i_blkTypeOther.m_byteArrBlkEnd),
    m_bInsertBlkEnd(i_blkTypeOther.m_bInsertBlkEnd),
    m_bRemoveBlkEnd(i_blkTypeOther.m_bRemoveBlkEnd)
{
} // copy ctor

//------------------------------------------------------------------------------
CBlkTypeE::~CBlkTypeE()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType* CBlkTypeE::clone() const
//------------------------------------------------------------------------------
{
    return new CBlkTypeE(*this);
}

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray* CBlkTypeE::createWatchDogBlock() const
//------------------------------------------------------------------------------
{
    QByteArray* pByteArrWatchDog = new QByteArray();
    char*       pcData;
    QByteArray  byteArrFormat;

    // A watch dog block uses the synchronization blocks as defined by the
    // block type to be transmitted (or received). The data block will be
    // filled with a GUID byte pattern.

    // +----------+-----+
    // | Data ... | END |
    // +----------+-----+

    pByteArrWatchDog->resize( c_uByteArrWatchDogDataBlockLen + m_byteArrBlkEnd.length() );
    pcData = pByteArrWatchDog->data();

    memcpy( pcData, c_byteArrWatchDogDataBlock.data(), c_uByteArrWatchDogDataBlockLen );
    pcData += c_uByteArrWatchDogDataBlockLen;

    memcpy( pcData, m_byteArrBlkEnd.data(), m_byteArrBlkEnd.length() );

    return pByteArrWatchDog;

} // createWatchDogBlock

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
QList<QByteArray> CBlkTypeE::receiveDataBlocks(
    ESrvCltType            i_srvCltType,
    QObject*               i_pObjGtw,
    QObject*               i_pObjSrvClt,
    CIpcSocketWrapper*     i_pSocketWrapper,
    QByteArray*            i_pByteArrWatchDog,
    CMethodTracer*         i_pMthTracer,
    const QList<QObject*>& i_arpTrcMsgLogObjects ) const
//------------------------------------------------------------------------------
{
    // +----------+-----+      +----------+-----+
    // | Data ... | END | .... | Data ... | END |
    // +----------+-----+      +----------+-----+

    QString strMthInArgs;
    QString strMthRet;
    QString strMthAddInfo;

    if( i_pMthTracer != nullptr )
    {
        if( i_pMthTracer->isActive(ETraceDetailLevelMethodArgs) )
        {
            SSocketDscr socketDscr = i_pSocketWrapper->getSocketDscr();
            strMthInArgs  = socketDscr.getConnectionString();
        }
        i_pMthTracer->trace("-> receiveDataBlocks(" + strMthInArgs + ")");
    }

    QList<QByteArray> arByteArrs;

    CMsgErr*       pMsgErr = nullptr;
    bool           bBlockReceived = false;
    bool           bIsWatchDogBlock = false;
    SErrResultInfo errResultInfo;
    int            idxBlkEnd;
    QByteArray     byteArrFromSocket;
    QByteArray     byteArrDataBlock;
    const int      iReadTimeout_ms = 5000;

    if( i_pObjGtw != nullptr )
    {
        errResultInfo.setErrSource("ZS::Ipc", "CSrvCltBaseGateway", i_pObjGtw->objectName(), "receiveDataBlocks");
    }

    // Wait until the end marker of the block has been received ..
    bBlockReceived = false;
    while( !bBlockReceived && i_pSocketWrapper->bytesAvailable() )
    {
        i_pSocketWrapper->setLastTimeDataBlockReceivedInMs( ZS::System::Time::getProcTimeInMilliSec() );

        // ... read those bytes.
        byteArrFromSocket += i_pSocketWrapper->readAll();

        idxBlkEnd = byteArrFromSocket.indexOf(m_byteArrBlkEnd);

        if( idxBlkEnd >= 0 )
        {
            bBlockReceived = true;
        }
        else // if( idxBlkEnd < 0 )
        {
            if( !i_pSocketWrapper->waitForReadyRead(iReadTimeout_ms) )
            {
                // Empty receive buffer
                i_pSocketWrapper->readAll();

                // Please note that the socket may have been closed in the meantime on
                // emitting the "error" or "disconnected" signals (see corresponding slots).

                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultUnfinishedSocketOperation);

                pMsgErr = new CMsgErr(
                    /* pObjSender    */ i_pObjGtw,
                    /* pObjReceiver  */ i_pObjSrvClt,
                    /* errResultInfo */ errResultInfo );
                POST_OR_DELETE_MESSAGE(pMsgErr, i_pMthTracer, ETraceDetailLevelRuntimeInfo);
                break;
            }
        } // if( idxBlkEnd < 0 )

        if( bBlockReceived )
        {
            int iBlkEndSize = m_byteArrBlkEnd.size();
            int iDataBlkSize = idxBlkEnd + 1 + (iBlkEndSize-1);

            byteArrDataBlock = byteArrFromSocket.left(iDataBlkSize);
            byteArrFromSocket.remove( 0, iDataBlkSize );

            if( i_pByteArrWatchDog != nullptr )
            {
                if( memcmp( i_pByteArrWatchDog->data(), byteArrDataBlock.data(), i_pByteArrWatchDog->length() ) == 0 )
                {
                    bIsWatchDogBlock = true;
                }
            }

            if( bIsWatchDogBlock )
            {
                if( i_pMthTracer != nullptr && i_pMthTracer->isActive(ETraceDetailLevelRuntimeInfo) )
                {
                    strMthAddInfo = "Received WatchDogBlock";
                    i_pMthTracer->trace(strMthAddInfo);
                }

                // Only servers acknowledge the receipt of watch dog data blocks.
                // Servers are the "passive" part of the watch dog communication.
                // Clients are the "active" part of the watch dog communication.
                if( i_srvCltType == ESrvCltTypeServer )
                {
                    CMsgReqSendData* pMsgReq = new CMsgReqSendData(
                        /* pObjSender       */ i_pObjGtw,
                        /* pObjReceiver     */ i_pObjGtw,
                        /* iSocketId        */ i_pSocketWrapper->getSocketId(),
                        /* byteArr          */ *i_pByteArrWatchDog,
                        /* bMustBeConfirmed */ false,
                        /* iReqId           */ -1 );
                    POST_OR_DELETE_MESSAGE(pMsgReq, i_pMthTracer, ETraceDetailLevelRuntimeInfo);
                    pMsgReq = nullptr;
                }

                // From the point of view of data transfer watch dog blocks got to be ignored.
                byteArrDataBlock.clear();

            } // if( bIsWatchDogBlock )

            else // if( !bIsWatchDogBlock )
            {
                if( i_pMthTracer != nullptr && i_pMthTracer->isActive(ETraceDetailLevelRuntimeInfo) )
                {
                    strMthAddInfo = "Received block data: BlockSize=" + QString::number(byteArrDataBlock.size()) + ", BlockEnd=" + m_byteArrBlkEnd;
                    i_pMthTracer->trace(strMthAddInfo);
                }

                if( m_bRemoveBlkEnd )
                {
                    byteArrDataBlock.remove(idxBlkEnd,m_byteArrBlkEnd.size());
                }
            }
        } // if( bBlockReceived )

        if( byteArrDataBlock.size() > 0 )
        {
            if( i_arpTrcMsgLogObjects.size() > 0 )
            {
                strMthAddInfo = "-- IpcSocket::RCV( " + QString::number(byteArrDataBlock.size()) + " Bytes: " + QString(byteArrDataBlock) + " )";

                for( int idxObj = 0; idxObj < i_arpTrcMsgLogObjects.size(); idxObj++ )
                {
                    CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                        /* pObjSender   */ i_pObjGtw,
                        /* pObjReceiver */ i_arpTrcMsgLogObjects[idxObj],
                        /* transmitDir  */ ETransmitDir::Receive,
                        /* bBold        */ false,
                        /* strMsg       */ strMthAddInfo );
                    POST_OR_DELETE_MESSAGE(pMsgLogItem, i_pMthTracer, ETraceDetailLevelRuntimeInfo);
                    pMsgLogItem = nullptr;
                }
            }
        }

        // Someone may sent a lot of data very quickly.
        // Let the GUI main thread process the received data.
        //m_pGatewayThread->msleep(50);

        arByteArrs += byteArrDataBlock;

        if( byteArrFromSocket.size() > 0 )
        {
            bBlockReceived = false;
        }

    } // while( !bBlockReceived && i_pSocketWrapper->bytesAvailable() || byteArrFromSocket.size() > 0 )

    // Please note that if still data is available in the read buffer the "onReadyReady" slot
    // (which invoked this "receiveDataBlock" method) sends the "ReqReceiveData" message to
    // the event method of the gateway. This way also data send requests may be executed even
    // if a lot of data is sent to the socket.

    if( i_pMthTracer != nullptr )
    {
        if( i_pMthTracer->isActive(ETraceDetailLevelMethodArgs) )
        {
            strMthRet = "Blocks [" + QString::number(arByteArrs.size()) + "]";
            if( arByteArrs.size() > 0 )
            {
                strMthRet += "(";
                for( int idxBlk = 0; idxBlk < arByteArrs.size(); idxBlk++ )
                {
                    strMthRet += "Block [Size=" + QString::number(arByteArrs[idxBlk].size()) + "]";

                    if( i_pMthTracer->isActive(ETraceDetailLevelInternalStates) )
                    {
                        strMthRet += "(";
                        if( arByteArrs[idxBlk].size() > 0 )
                        {
                            strMthRet += arByteArrs[idxBlk];
                        }
                        strMthRet += ")";
                    }
                }
                strMthRet += ")";
            }
        }
        i_pMthTracer->trace("<- receiveDataBlocks(): " + strMthRet, ETraceDetailLevelMethodCalls);
    }

    return arByteArrs;

} // receiveDataBlocks

//------------------------------------------------------------------------------
bool CBlkTypeE::writeDataBlock(
    ESrvCltType            /*i_srvCltType*/,
    QObject*               i_pObjGtw,
    QObject*               /*i_pObjSrvClt*/,
    CIpcSocketWrapper*     i_pSocketWrapper,
    const QByteArray&      i_byteArr,
    bool                   i_bIsWatchDogBlock,
    CMethodTracer*         i_pMthTracer,
    const QList<QObject*>& i_arpTrcMsgLogObjects ) const
//------------------------------------------------------------------------------
{
    // +----------+-----+
    // | Data ... | END |
    // +----------+-----+

    QString strMthInArgs;
    QString strMthRet;
    QString strMthAddInfo;

    if( i_pMthTracer != nullptr )
    {
        if( i_pMthTracer->isActive(ETraceDetailLevelMethodArgs) )
        {
            SSocketDscr socketDscr = i_pSocketWrapper->getSocketDscr();
            strMthInArgs  = socketDscr.getConnectionString();
            strMthInArgs += ", Bytes: " + QString::number(i_byteArr.size());
        }
        i_pMthTracer->trace("-> writeDataBlock(" + strMthInArgs + ")");
    }

    QByteArray* pByteArr = nullptr;

    if( m_bInsertBlkEnd )
    {
        quint16 uEndTagSize = m_byteArrBlkEnd.size();

        pByteArr = new QByteArray();
        pByteArr->resize(i_byteArr.size()+uEndTagSize);

        char* pcData = pByteArr->data();

        // Copy data and end tag to destination array.
        memcpy( pcData, i_byteArr, i_byteArr.size() );
        pcData += i_byteArr.size();
        memcpy( pcData, m_byteArrBlkEnd, uEndTagSize );
    }
    else
    {
        pByteArr = new QByteArray(i_byteArr);
    }

    if( !i_bIsWatchDogBlock )
    {
        if( i_pMthTracer != nullptr && i_pMthTracer->isActive(ETraceDetailLevelRuntimeInfo) )
        {
            strMthAddInfo = "Sending " + QString::number(pByteArr->size()) + " bytes of data";
            i_pMthTracer->trace(strMthAddInfo);
        }
    }

    if( i_arpTrcMsgLogObjects.size() > 0 )
    {
        strMthAddInfo = "-- IpcSocket::SND( " + QString::number(i_byteArr.size()) + " Bytes: " + QString(i_byteArr) + " )";

        for( int idxObj = 0; idxObj < i_arpTrcMsgLogObjects.size(); idxObj++ )
        {
            CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                /* pObjSender   */ i_pObjGtw,
                /* pObjReceiver */ i_arpTrcMsgLogObjects[idxObj],
                /* transmitDir  */ ETransmitDir::Send,
                /* bBold        */ false,
                /* strMsg       */ strMthAddInfo );
            POST_OR_DELETE_MESSAGE(pMsgLogItem, i_pMthTracer, ETraceDetailLevelRuntimeInfo);
            pMsgLogItem = nullptr;
        }
    }

    bool bOk = (i_pSocketWrapper->write(*pByteArr) == pByteArr->size());

    delete pByteArr;
    pByteArr = nullptr;

    if( i_pMthTracer != nullptr )
    {
        if( i_pMthTracer->isActive(ETraceDetailLevelMethodArgs) )
        {
            strMthRet = bool2Str(bOk);
        }
        i_pMthTracer->trace("<- writeDataBlock(): " + strMthRet);
    }

    return bOk;

} // writeDataBlock
