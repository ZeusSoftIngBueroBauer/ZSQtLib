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

#ifndef ZSIpc_BlockTypeTLV_h
#define ZSIpc_BlockTypeTLV_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcBlkType.h"


namespace ZS
{
namespace Ipc
{
//******************************************************************************
class ZSIPCDLL_API CBlkTypeTLV : public CBlkType
//******************************************************************************
{
    // +-----+--------+-----------+
    // | TAG | LENgth | Value ... |
    // +-----+--------+-----------+
    // The block is synchronized by a tag block with a predefined length and
    // by defining the length of the value data block.
public: // ctors and dtor
    CBlkTypeTLV(
        quint32         i_uTLVTagSize,
        EBlkLenDataType i_blkLenDataType,
        int             i_iBlkLenDataTypeSize = -1 ); // -1 means use default (see enum EBlkLenDataType above)
    CBlkTypeTLV( const CBlkTypeTLV& i_blkTypeOther );
    virtual ~CBlkTypeTLV();
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
public: // instance methods
    quint32 tagSize() const { return m_uTLVTagSize; }
    EBlkLenDataType blkLenDataType() const { return m_blkLenDataType; }
    int blkLenDataTypeSize() const { return m_iBlkLenDataTypeSize; }
private: // default ctor not allowed
    CBlkTypeTLV();
protected: // instance members
    quint32         m_uTLVTagSize;
    EBlkLenDataType m_blkLenDataType;
    int             m_iBlkLenDataTypeSize;

}; // class CBlkTypeTLV

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_BlockTypeTLV_h
