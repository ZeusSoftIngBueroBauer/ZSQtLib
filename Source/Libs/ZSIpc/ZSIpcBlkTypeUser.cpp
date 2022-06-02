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

#include "ZSIpc/ZSIpcBlkTypeUser.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Ipc;


/*******************************************************************************
class CBlkTypeUser : public CBlkType
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CBlkTypeUser::CBlkTypeUser(
    TFctReceiveDataBlocks   /*i_pFctReceiveDataBlocks*/,
    TFctWriteDataBlock      /*i_pFctWriteDataBlock*/,
    TFctCreateWatchDogBlock /*i_pFctCreateWatchDogBlock*/ ) :
//------------------------------------------------------------------------------
    CBlkType(EBlkTypeUser),
    m_fctReceiveDataBlocks(nullptr),
    m_fctWriteDataBlock(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CBlkTypeUser::CBlkTypeUser( const CBlkTypeUser& i_blkTypeOther ) :
//------------------------------------------------------------------------------
    CBlkType(EBlkTypeUser),
    m_fctReceiveDataBlocks(i_blkTypeOther.m_fctReceiveDataBlocks),
    m_fctWriteDataBlock(i_blkTypeOther.m_fctWriteDataBlock)
{
} // copy ctor

//------------------------------------------------------------------------------
CBlkTypeUser::~CBlkTypeUser()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType* CBlkTypeUser::clone() const
//------------------------------------------------------------------------------
{
    return new CBlkTypeUser(*this);
}

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray* CBlkTypeUser::createWatchDogBlock() const
//------------------------------------------------------------------------------
{
    QByteArray* pByteArrWatchDog = nullptr;

    if( m_fctCreateWatchDogBlock != nullptr )
    {
        pByteArrWatchDog = m_fctCreateWatchDogBlock(this);
    }

    return pByteArrWatchDog;

} // createWatchDogBlock

/*==============================================================================
public: // must overridables of base class CBlkType
==============================================================================*/

//------------------------------------------------------------------------------
QList<QByteArray> CBlkTypeUser::receiveDataBlocks(
    ESrvCltType            i_srvCltType,
    QObject*               i_pObjGtw,
    QObject*               i_pObjSrvClt,
    CIpcSocketWrapper*     i_pSocketWrapper,
    QByteArray*            i_pByteArrWatchDog,
    CMethodTracer*         i_pMthTracer,
    const QList<QObject*>& i_arpTrcMsgLogObjects ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( i_pMthTracer != nullptr && i_pMthTracer->areMethodCallsActive(ETraceDetailLevelMethodCalls::EnterLeave) )
    {
        if( i_pMthTracer->areMethodCallsActive(ETraceDetailLevelMethodCalls::ArgsNormal) )
        {
            SSocketDscr socketDscr = i_pSocketWrapper->getSocketDscr();
            strMthInArgs = socketDscr.getConnectionString();
        }
        i_pMthTracer->trace("-> receiveDataBlocks(" + strMthInArgs + ")");
    }

    QList<QByteArray> arByteArrs;

    if( m_fctReceiveDataBlocks != nullptr )
    {
        arByteArrs = m_fctReceiveDataBlocks(
            /* pBlkType            */ this,
            /* srvCltType          */ i_srvCltType,
            /* pObjGtw             */ i_pObjGtw,
            /* pObjSrvClt          */ i_pObjSrvClt,
            /* pSocketWrapper      */ i_pSocketWrapper,
            /* pByteArrWatchDog    */ i_pByteArrWatchDog,
            /* pTrcAdminObj        */ i_pMthTracer,
            /* arpTrcMsgLogObjects */ i_arpTrcMsgLogObjects );
    }

    if( i_pMthTracer != nullptr && i_pMthTracer->areMethodCallsActive(ETraceDetailLevelMethodCalls::EnterLeave) )
    {
        if( i_pMthTracer->areMethodCallsActive(ETraceDetailLevelMethodCalls::ArgsNormal) )
        {
            strMthRet = "Blocks [" + QString::number(arByteArrs.size()) + "]";
            if( arByteArrs.size() > 0 )
            {
                strMthRet += "(";
                for( int idxBlk = 0; idxBlk < arByteArrs.size(); idxBlk++ )
                {
                    strMthRet += "Block [Size=" + QString::number(arByteArrs[idxBlk].size()) + "]";

                    if( i_pMthTracer->isRuntimeInfoActive(ETraceDetailLevelRuntimeInfo::DebugNormal) )
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
        i_pMthTracer->trace("<- receiveDataBlocks(): " + strMthRet);
    }

    return arByteArrs;

} // receiveDataBlocks

//------------------------------------------------------------------------------
bool CBlkTypeUser::writeDataBlock(
    ESrvCltType            i_srvCltType,
    QObject*               i_pObjGtw,
    QObject*               i_pObjSrvClt,
    CIpcSocketWrapper*     i_pSocketWrapper,
    const QByteArray&      i_byteArr,
    bool                   i_bIsWatchDogBlock,
    CMethodTracer*         i_pMthTracer,
    const QList<QObject*>& i_arpTrcMsgLogObjects ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( i_pMthTracer != nullptr && i_pMthTracer->areMethodCallsActive(ETraceDetailLevelMethodCalls::EnterLeave) )
    {
        if( i_pMthTracer->areMethodCallsActive(ETraceDetailLevelMethodCalls::ArgsNormal) )
        {
            SSocketDscr socketDscr = i_pSocketWrapper->getSocketDscr();
            strMthInArgs  = socketDscr.getConnectionString();
            strMthInArgs += ", Bytes: " + QString::number(i_byteArr.size());
        }
        i_pMthTracer->trace("-> writeDataBlock(" + strMthInArgs + ")");
    }

    bool bOk = false;

    if( m_fctWriteDataBlock != nullptr )
    {
        bOk = m_fctWriteDataBlock(
            /* pBlkType            */ this,
            /* srvCltType          */ i_srvCltType,
            /* pObjGtw             */ i_pObjGtw,
            /* pObjSrvClt          */ i_pObjSrvClt,
            /* pSocketWrapper      */ i_pSocketWrapper,
            /* pByteArrWatchDog    */ i_byteArr,
            /* bIsWatchDog         */ i_bIsWatchDogBlock,
            /* pTrcAdminObj        */ i_pMthTracer,
            /* arpTrcMsgLogObjects */ i_arpTrcMsgLogObjects );
    }

    if( i_pMthTracer != nullptr && i_pMthTracer->areMethodCallsActive(ETraceDetailLevelMethodCalls::EnterLeave) )
    {
        if( i_pMthTracer->areMethodCallsActive(ETraceDetailLevelMethodCalls::ArgsNormal) )
        {
            strMthRet = bool2Str(bOk);
        }
        i_pMthTracer->trace("<- writeDataBlock(): " + strMthRet);
    }

    return bOk;

} // writeDataBlock
