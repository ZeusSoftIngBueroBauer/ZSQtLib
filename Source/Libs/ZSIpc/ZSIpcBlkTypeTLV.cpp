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

#include "ZSIpc/ZSIpcBlkTypeTLV.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Ipc;


/*******************************************************************************
class CBlkTypeTLV : public CBlkType
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CBlkTypeTLV::CBlkTypeTLV(
    quint32         i_uTLVTagSize,
    EBlkLenDataType i_blkLenDataType,
    int             i_iBlkLenDataTypeSize ) :
//------------------------------------------------------------------------------
    CBlkType(EBlkTypeTLV),
    m_uTLVTagSize(i_uTLVTagSize),
    m_blkLenDataType(i_blkLenDataType),
    m_iBlkLenDataTypeSize(i_iBlkLenDataTypeSize)
{
    if( m_iBlkLenDataTypeSize < 0 )
    {
        m_iBlkLenDataTypeSize = blockLenDataType2SizeInBytes(m_blkLenDataType);
    }
} // ctor

//------------------------------------------------------------------------------
CBlkTypeTLV::CBlkTypeTLV( const CBlkTypeTLV& i_blkTypeOther ) :
//------------------------------------------------------------------------------
    CBlkType(EBlkTypeTLV),
    m_uTLVTagSize(i_blkTypeOther.m_uTLVTagSize),
    m_blkLenDataType(i_blkTypeOther.m_blkLenDataType),
    m_iBlkLenDataTypeSize(i_blkTypeOther.m_iBlkLenDataTypeSize)
{
} // copy ctor

//------------------------------------------------------------------------------
CBlkTypeTLV::~CBlkTypeTLV()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType* CBlkTypeTLV::clone() const
//------------------------------------------------------------------------------
{
    return new CBlkTypeTLV(*this);
}

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray* CBlkTypeTLV::createWatchDogBlock() const
//------------------------------------------------------------------------------
{
    QByteArray* pByteArrWatchDog = new QByteArray();
    char*       pcData;
    QByteArray  byteArrFormat;

    // A watch dog block uses the synchronization blocks as defined by the
    // block type to be transmitted (or received). The data block will be
    // filled with a GUID byte pattern.

    // +-----+--------+----------+
    // | TAG | LENgth | Value... |
    // +-----+--------+----------+

    pByteArrWatchDog->resize( m_uTLVTagSize + m_iBlkLenDataTypeSize + c_uByteArrWatchDogDataBlockLen );
    pcData = pByteArrWatchDog->data();

    memset( pcData, 0x00, m_uTLVTagSize );
    pcData += m_uTLVTagSize;

    switch( m_blkLenDataType )
    {
        case EBlkLenDataTypeBinINT8:
        case EBlkLenDataTypeBinUINT8:
        {
            quint8* pi = reinterpret_cast<quint8*>(pcData);
            *pi = c_uByteArrWatchDogDataBlockLen;
            break;
        }
        case EBlkLenDataTypeBinINT16:
        case EBlkLenDataTypeBinUINT16:
        {
            quint16* pi = reinterpret_cast<quint16*>(pcData);
            *pi = c_uByteArrWatchDogDataBlockLen;
            break;
        }
        case EBlkLenDataTypeBinINT32:
        case EBlkLenDataTypeBinUINT32:
        {
            quint32* pi = reinterpret_cast<quint32*>(pcData);
            *pi = c_uByteArrWatchDogDataBlockLen;
            break;
        }
        case EBlkLenDataTypeBinINT64:
        case EBlkLenDataTypeBinUINT64:
        {
            quint64* pi = reinterpret_cast<quint64*>(pcData);
            *pi = c_uByteArrWatchDogDataBlockLen;
            break;
        }
        case EBlkLenDataTypeStrBase10INT8:
        case EBlkLenDataTypeStrBase10INT16:
        case EBlkLenDataTypeStrBase10INT32:
        {
            byteArrFormat = "%0" + QByteArray::number(m_iBlkLenDataTypeSize) + "i";
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            sprintf( pcData, byteArrFormat, c_uByteArrWatchDogDataBlockLen );
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            break;
        }
        case EBlkLenDataTypeStrBase10UINT8:
        case EBlkLenDataTypeStrBase10UINT16:
        case EBlkLenDataTypeStrBase10UINT32:
        {
            byteArrFormat = "%0" + QByteArray::number(m_iBlkLenDataTypeSize) + "u";
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            sprintf( pcData, byteArrFormat, c_uByteArrWatchDogDataBlockLen );
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            break;
        }
        case EBlkLenDataTypeStrBase10INT64:
        case EBlkLenDataTypeStrBase10UINT64:
        {
            byteArrFormat = "%0" + QByteArray::number(m_iBlkLenDataTypeSize) + "l";
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            sprintf( pcData, byteArrFormat, c_uByteArrWatchDogDataBlockLen );
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            break;
        }
        case EBlkLenDataTypeStrBase16INT8:
        case EBlkLenDataTypeStrBase16UINT8:
        case EBlkLenDataTypeStrBase16INT16:
        case EBlkLenDataTypeStrBase16UINT16:
        case EBlkLenDataTypeStrBase16INT32:
        case EBlkLenDataTypeStrBase16UINT32:
        {
            byteArrFormat = "%0" + QByteArray::number(m_iBlkLenDataTypeSize) + "x";
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            sprintf( pcData, byteArrFormat, c_uByteArrWatchDogDataBlockLen );
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            break;
        }
        case EBlkLenDataTypeStrBase16INT64:
        case EBlkLenDataTypeStrBase16UINT64:
        {
            byteArrFormat = "%0" + QByteArray::number(m_iBlkLenDataTypeSize) + "lx";
            #ifdef _WINDOWS
            #pragma warning(disable:4996)
            #endif
            sprintf( pcData, byteArrFormat, c_uByteArrWatchDogDataBlockLen );
            #ifdef _WINDOWS
            #pragma warning(default:4996)
            #endif
            break;
        }
        default:
        {
            quint32* pi = reinterpret_cast<quint32*>(pcData);
            *pi = c_uByteArrWatchDogDataBlockLen;
            break;
        }
    } // switch( m_blkLenDataType )

    pcData += m_iBlkLenDataTypeSize;

    memcpy( pcData, c_byteArrWatchDogDataBlock.data(), c_uByteArrWatchDogDataBlockLen );

    return pByteArrWatchDog;

} // createWatchDogBlock

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
QList<QByteArray> CBlkTypeTLV::receiveDataBlocks(
    ESrvCltType            i_srvCltType,
    QObject*               i_pObjGtw,
    QObject*               i_pObjSrvClt,
    CIpcSocketWrapper*     i_pSocketWrapper,
    QByteArray*            i_pByteArrWatchDog,
    CMethodTracer*         i_pMthTracer,
    const QList<QObject*>& i_arpTrcMsgLogObjects ) const
//------------------------------------------------------------------------------
{
    // +-----+--------+----------+      +-----+--------+----------+
    // | TAG | LENgth | Value... |      | TAG | LENgth | Value... |
    // +-----+--------+----------+      +-----+--------+----------+

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

    QByteArray     byteArr;
    CMsgErr*       pMsgErr = nullptr;
    bool           bBlockReceived = false;
    bool           bBlockSizeConverted;
    bool           bIsWatchDogBlock = false;
    SErrResultInfo errResultInfo;

    if( i_pObjGtw != nullptr )
    {
        errResultInfo.setErrSource("ZS::Ipc", "CSrvCltBaseGateway", i_pObjGtw->objectName(), "receiveDataBlocks");
    }

    while( !bBlockReceived && pMsgErr == nullptr && i_pSocketWrapper->bytesAvailable() )
    {
        quint8      uTagSize      = m_uTLVTagSize;
        quint8      uBlockLenSize = m_iBlkLenDataTypeSize;
        quint64     uBlockSize    = 0;
        QByteArray  byteArrBlockSize;
        bool        bReadyRead;
        const int   iReadTimeout_ms = 1000;
        double      fStartTime_ms;
        double      fCurrTime_ms;

        if( m_blkLenDataType >= EBlkLenDataTypeBinMin && m_blkLenDataType <= EBlkLenDataTypeBinMax )
        {
            byteArrBlockSize.resize( sizeof(uBlockSize) );
        }
        else
        {
            byteArrBlockSize.resize(42);
        }
        byteArrBlockSize.fill(0x00);

        // Wait until the number of bytes containing the tag (main header excluding length)
        // of the block have been received ..
        fStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
        while( i_pSocketWrapper->bytesAvailable() < uTagSize )
        {
            fCurrTime_ms = ZS::System::Time::getProcTimeInMilliSec();

            bReadyRead = i_pSocketWrapper->waitForReadyRead(iReadTimeout_ms);

            if( !bReadyRead || (fCurrTime_ms - fStartTime_ms) > static_cast<double>(iReadTimeout_ms) )
            {
                // Empty receive buffer
                i_pSocketWrapper->read(i_pSocketWrapper->bytesAvailable());

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
        }

        // If the tag has been received ..
        if( pMsgErr == nullptr )
        {
            // ... read the tag of the block.
            byteArr += i_pSocketWrapper->read(uTagSize);
        }

        // Wait until the number of bytes containing the length of the value block have been received ...
        fStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
        while( i_pSocketWrapper->bytesAvailable() < uBlockLenSize )
        {
            fCurrTime_ms = ZS::System::Time::getProcTimeInMilliSec();

            bReadyRead = i_pSocketWrapper->waitForReadyRead(iReadTimeout_ms);

            if( !bReadyRead || (fCurrTime_ms - fStartTime_ms) > static_cast<double>(iReadTimeout_ms) )
            {
                // Empty receive buffer
                i_pSocketWrapper->read(i_pSocketWrapper->bytesAvailable());

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
        }

        // If the length of the value block has been received ..
        if( pMsgErr == nullptr )
        {
            // ... read the length of the value block.
            byteArrBlockSize = i_pSocketWrapper->read(uBlockLenSize);

            if( m_blkLenDataType >= EBlkLenDataTypeBinMin && m_blkLenDataType <= EBlkLenDataTypeBinMax )
            {
                memcpy( &uBlockSize, byteArrBlockSize.data(), uBlockLenSize );
                bBlockSizeConverted = true;
            }
            else if( m_blkLenDataType >= EBlkLenDataTypeStrBase10Min && m_blkLenDataType <= EBlkLenDataTypeStrBase10Max )
            {
                uBlockSize = byteArrBlockSize.toULong(&bBlockSizeConverted,10);
            }
            else if( m_blkLenDataType >= EBlkLenDataTypeStrBase16Min && m_blkLenDataType <= EBlkLenDataTypeStrBase16Max )
            {
                uBlockSize = byteArrBlockSize.toULong(&bBlockSizeConverted,16);
            }
            else
            {
                bBlockSizeConverted = false;
            }

            if( !bBlockSizeConverted )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultStringParseError);
                errResultInfo.setAddErrInfoDscr( "Invalid block size " + byteArrBlockSize + " received" );

                pMsgErr = new CMsgErr(
                    /* pObjSender    */ i_pObjGtw,
                    /* pObjReceiver  */ i_pObjSrvClt,
                    /* errResultInfo */ errResultInfo );
                POST_OR_DELETE_MESSAGE(pMsgErr, i_pMthTracer, ETraceDetailLevelRuntimeInfo);
                break;
            }

            // Copy size info to destination array (LSB at the beginning).
            int iSize = byteArr.size();
            byteArr.resize( iSize + uBlockLenSize );
            char* pcData = &byteArr.data()[iSize];
            memcpy( pcData, &uBlockSize, uBlockLenSize );

            // Now wait until as many bytes as defined by the length of the value block have been received.
            fStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
            while( i_pSocketWrapper->bytesAvailable() < static_cast<int>(uBlockSize) )
            {
                fCurrTime_ms = ZS::System::Time::getProcTimeInMilliSec();

                bReadyRead = i_pSocketWrapper->waitForReadyRead(iReadTimeout_ms);

                if( !bReadyRead || (fCurrTime_ms - fStartTime_ms) > static_cast<double>(iReadTimeout_ms) )
                {
                    // Empty receive buffer
                    i_pSocketWrapper->read(i_pSocketWrapper->bytesAvailable());

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
            }
        } // if( pMsgErr == nullptr ) // If the length of the value block has been received ..

        // If the number of bytes of the value block have been received ..
        if( pMsgErr == nullptr )
        {
            if( uBlockSize > 0 )
            {
                // ... read those bytes.
                byteArr += i_pSocketWrapper->read(uBlockSize);
            }

            bBlockReceived = true;

            i_pSocketWrapper->setLastTimeDataBlockReceivedInMs( ZS::System::Time::getProcTimeInMilliSec() );

            if( i_pByteArrWatchDog != nullptr )
            {
                if( memcmp( i_pByteArrWatchDog->data(), byteArr.data(), i_pByteArrWatchDog->length() ) == 0 )
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
                byteArr.clear();

            } // if( bIsWatchDogBlock )

            else // if( !bIsWatchDogBlock )
            {
                if( i_pMthTracer != nullptr && i_pMthTracer->isActive(ETraceDetailLevelRuntimeInfo) )
                {
                    strMthAddInfo = "Received block data: TagSize=" + QString::number(uTagSize) + ", ValueLen=" + QString::number(uBlockSize);
                    i_pMthTracer->trace(strMthAddInfo);
                }

                if( i_arpTrcMsgLogObjects.size() > 0 )
                {
                    strMthAddInfo = "-- IpcSocket::RCV( " + QString::number(uTagSize+uBlockSize) + " Bytes: " + byteArr + " )";

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
            } // if( !bIsWatchDogBlock )
        } // if( pMsgErr == nullptr ) // If the number of bytes of the value block have been received ..

        // Someone may sent a lot of data very quickly.
        // Let the GUI main thread process the received data.
        //m_pGatewayThread->msleep(50);

        arByteArrs += byteArr;

    } // while( !bBlockReceived && pMsgErr == nullptr && i_pSocketWrapper->bytesAvailable() )

    // Please note that if still data is available in the read buffer the "onReadyReady" slot
    // (which invoked this "receiveDataBlock" method) the message "ReqReceiveData" will be sent
    // to the event method of the gateway. This way also data send requests may be executed even
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
bool CBlkTypeTLV::writeDataBlock(
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
    // +-----+--------+----------+
    // | TAG | LENgth | Value... |
    // +-----+--------+----------+

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

    if( !i_bIsWatchDogBlock )
    {
        if( i_pMthTracer != nullptr && i_pMthTracer->isActive(ETraceDetailLevelRuntimeInfo) )
        {
            strMthAddInfo = "Sending " + QString::number(i_byteArr.size()) + " bytes of data";
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

    bool bOk = ( i_pSocketWrapper->write(i_byteArr) == i_byteArr.size() );

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
