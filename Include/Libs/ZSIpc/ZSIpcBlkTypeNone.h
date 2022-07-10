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

#ifndef ZSIpc_BlockTypeNone_h
#define ZSIpc_BlockTypeNone_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcBlkType.h"


namespace ZS
{
namespace Ipc
{
//******************************************************************************
class ZSIPCDLL_API CBlkTypeNone : public CBlkType
//******************************************************************************
{
public: // ctors and dtor
    CBlkTypeNone();
    CBlkTypeNone( const CBlkTypeNone& i_blkTypeOther );
    virtual ~CBlkTypeNone();
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

}; // class CBlkTypeNone

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_BlockTypeNone_h
