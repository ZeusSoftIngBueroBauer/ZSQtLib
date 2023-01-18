/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSIpc_BlockTypeUser_h
#define ZSIpc_BlockTypeUser_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcBlkType.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Ipc
{
typedef QList<QByteArray> (*TFctReceiveDataBlocks)(
    const CBlkType*            i_pBlkType,
    ESrvCltType                i_srvCltType,
    QObject*                   i_pObjGtw,
    QObject*                   i_pObjSrvClt,
    CIpcSocketWrapper*         i_pSocketWrapper,
    QByteArray*                i_pByteArrWatchDog,
    ZS::System::CMethodTracer* i_pMethodTracer,
    const QList<QObject*>&     i_arpTrcMsgLogObjects );

typedef bool (*TFctWriteDataBlock)(
    const CBlkType*            i_pBlkType,
    ESrvCltType                i_srvCltType,
    QObject*                   i_pObjGtw,
    QObject*                   i_pObjSrvClt,
    CIpcSocketWrapper*         i_pSocketWrapper,
    const QByteArray&          i_byteArr,
    bool                       i_bIsWatchDogBlock,
    ZS::System::CMethodTracer* i_pMethodTracer,
    const QList<QObject*>&     i_arpTrcMsgLogObjects );

typedef QByteArray* (*TFctCreateWatchDogBlock)( const CBlkType* i_pBlkType );

//******************************************************************************
class ZSIPCDLL_API CBlkTypeUser : public CBlkType
//******************************************************************************
{
public: // ctors and dtor
    CBlkTypeUser(
        TFctReceiveDataBlocks   i_pFctReceiveDataBlocks,
        TFctWriteDataBlock      i_pFctWriteDataBlock,
        TFctCreateWatchDogBlock i_pFctCreateWatchDogBlock );
    CBlkTypeUser( const CBlkTypeUser& i_blkTypeOther );
    virtual ~CBlkTypeUser();
public: // must overridables of base class CBlkType
    virtual CBlkType* clone() const override;
public: // must overridables of base class CBlkType
    virtual QByteArray* createWatchDogBlock() const override;
public: // must overridables of base class CBlkType
    virtual QList<QByteArray> receiveDataBlocks(
        ESrvCltType                i_srvCltType,
        QObject*                   i_pObjGtw,
        QObject*                   i_pObjSrvClt,
        CIpcSocketWrapper*         i_pSocketWrapper,
        QByteArray*                i_pByteArrWatchDog = nullptr,
        ZS::System::CMethodTracer* i_pMethodTracer = nullptr,
        const QList<QObject*>&     i_arpTrcMsgLogObjects = QList<QObject*>() ) const override;
    virtual bool writeDataBlock(
        ESrvCltType                i_srvCltType,
        QObject*                   i_pObjGtw,
        QObject*                   i_pObjSrvClt,
        CIpcSocketWrapper*         i_pSocketWrapper,
        const QByteArray&          i_byteArr,
        bool                       i_bIsWatchDogBlock = false,
        ZS::System::CMethodTracer* i_pMethodTracer = nullptr,
        const QList<QObject*>&     i_arpTrcMsgLogObjects = QList<QObject*>() ) const override;
protected: // instance members
    TFctReceiveDataBlocks   m_fctReceiveDataBlocks;
    TFctWriteDataBlock      m_fctWriteDataBlock;
    TFctCreateWatchDogBlock m_fctCreateWatchDogBlock;

}; // class CBlkTypeUser

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_BlockTypeUser_h
