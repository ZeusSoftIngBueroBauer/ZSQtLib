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

#include "ZSIpc/ZSIpcBlkTypeNone.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Ipc;


/*******************************************************************************
class CBlkTypeNone : public CBlkType
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CBlkTypeNone::CBlkTypeNone() :
//------------------------------------------------------------------------------
    CBlkType(EBlkTypeNone)
{
} // ctor

//------------------------------------------------------------------------------
CBlkTypeNone::CBlkTypeNone( const CBlkTypeNone& /*i_blkTypeOther*/ ) :
//------------------------------------------------------------------------------
    CBlkType(EBlkTypeNone)
{
} // copy ctor

//------------------------------------------------------------------------------
CBlkTypeNone::~CBlkTypeNone()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType* CBlkTypeNone::clone() const
//------------------------------------------------------------------------------
{
    return new CBlkTypeNone(*this);
}

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray* CBlkTypeNone::createWatchDogBlock() const
//------------------------------------------------------------------------------
{
    return new QByteArray(c_byteArrWatchDogDataBlock);
}

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
QList<QByteArray> CBlkTypeNone::receiveDataBlocks(
    ESrvCltType            /*i_srvCltType*/,
    QObject*               i_pObjGtw,
    QObject*               /*i_pObjSrvClt*/,
    CIpcSocketWrapper*     i_pSocketWrapper,
    QByteArray*            /*i_pByteArrWatchDog*/,
    CMethodTracer*         i_pMthTracer,
    const QList<QObject*>& i_arpTrcMsgLogObjects ) const
//------------------------------------------------------------------------------
{
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

    QByteArray byteArr;
    qint64     iBlkSize;

    // Here we read all bytes at once which are currently available within the socket.
    // Those might be more than just one TCP/IP block sent by the peer as the socket
    // accumulates the received blocks in his "IO device". The server class needs to
    // be able to separate (parse) the block potentially compound by single blocks.
    while( (iBlkSize = i_pSocketWrapper->bytesAvailable()) > 0 )
    {
        i_pSocketWrapper->setLastTimeDataBlockReceivedInMs( ZS::System::Time::getProcTimeInMilliSec() );

        byteArr = i_pSocketWrapper->read(iBlkSize);

        if( i_arpTrcMsgLogObjects.size() > 0 )
        {
            strMthAddInfo = "-- IpcSocket::RCV( " + QString::number(iBlkSize) + " Bytes: " + byteArr + " )";

            for( int idxObj = 0; idxObj < i_arpTrcMsgLogObjects.size(); idxObj++ )
            {
                CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                    /* pObjSender   */ i_pObjGtw,
                    /* pObjReceiver */ i_arpTrcMsgLogObjects[idxObj],
                    /* transmitDir  */ ETransmitDirReceive,
                    /* bBold        */ false,
                    /* strMsg       */ strMthAddInfo );
                POST_OR_DELETE_MESSAGE(pMsgLogItem, i_pMthTracer, ETraceDetailLevelRuntimeInfo);
                pMsgLogItem = nullptr;
            }
        }

        // Someone may sent a lot of data very quickly.
        // Let the GUI main thread process the received data.
        //m_pGatewayThread->msleep(50);

        arByteArrs += byteArr;

    } // while( (iBlkSize = i_pSocketWrapper->bytesAvailable()) > 0 )

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
bool CBlkTypeNone::writeDataBlock(
    ESrvCltType            /*i_srvCltType*/,
    QObject*               i_pObjGtw,
    QObject*               /*i_pObjSrvClt*/,
    CIpcSocketWrapper*     i_pSocketWrapper,
    const QByteArray&      i_byteArr,
    bool                   /*i_bIsWatchDogBlock*/,
    CMethodTracer*         i_pMthTracer,
    const QList<QObject*>& i_arpTrcMsgLogObjects ) const
//------------------------------------------------------------------------------
{
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

    bool bOk = true;

    if( i_arpTrcMsgLogObjects.size() > 0 )
    {
        strMthAddInfo = "-- IpcSocket::SND( " + QString::number(i_byteArr.size()) + " Bytes: " + QString(i_byteArr) + " )";

        for( int idxObj = 0; idxObj < i_arpTrcMsgLogObjects.size(); idxObj++ )
        {
            CMsgReqAddLogItem* pMsgLogItem = new CMsgReqAddLogItem(
                /* pObjSender   */ i_pObjGtw,
                /* pObjReceiver */ i_arpTrcMsgLogObjects[idxObj],
                /* transmitDir  */ ETransmitDirSend,
                /* bBold        */ false,
                /* strMsg       */ strMthAddInfo );
            POST_OR_DELETE_MESSAGE(pMsgLogItem, i_pMthTracer, ETraceDetailLevelRuntimeInfo);
            pMsgLogItem = nullptr;
        }
    }

    bOk = ( i_pSocketWrapper->write(i_byteArr) == i_byteArr.size() );

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
