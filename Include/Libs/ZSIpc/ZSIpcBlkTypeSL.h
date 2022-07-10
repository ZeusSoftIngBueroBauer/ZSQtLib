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

#ifndef ZSIpc_BlockTypeSL_h
#define ZSIpc_BlockTypeSL_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcBlkType.h"


namespace ZS
{
namespace Ipc
{
//******************************************************************************
class ZSIPCDLL_API CBlkTypeSL : public CBlkType
//******************************************************************************
{
    // +-------+--------+----------+
    // | START | LENgth | Data ... |
    // +-------+--------+----------+
    // The block is synchronized by using a start block and by defining the length
    // of the data block. The start block must be a pattern which will never occur
    // within the data block. Usually a GUID should be used for the start block to
    // ensure that only packets between two known message recipients will be
    // accepted.
    // By setting the InsertBlkStart and RemoveBlkStart flags the gateway may
    // insert the start block before sending the data to the peer or may remove
    // the start block from the data block received from the peer before providing
    // the data to the ipc client or server.
    // By setting the InsertBlkLen and RemoveBlkLen flags the gateway may
    // insert the length before sending the data to the peer or may remove
    // the length from the data block received from the peer before providing
    // the data to the ipc client or server.
public: // ctors and dtor
    CBlkTypeSL(
        QByteArray      i_byteArrBlkStart,
        EBlkLenDataType i_blkLenDataType,
        bool            i_bInsertBlkStart = true,
        bool            i_bRemoveBlkStart = true,
        bool            i_bInsertBlkLen = true,
        bool            i_bRemoveBlkLen = true,
        int             i_iBlkLenDataTypeSize = -1 ); // -1 means use default (see enum EBlkLenDataType above)
    CBlkTypeSL( const CBlkTypeSL& i_blkTypeOther );
    virtual ~CBlkTypeSL();
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
    QByteArray blkStart() const { return m_byteArrBlkStart; }
    bool insertBlkStart() const { return m_bInsertBlkStart; }
    bool removeBlkStart() const { return m_bRemoveBlkStart; }
    EBlkLenDataType blkLenDataType() const { return m_blkLenDataType; }
    int blkLenDataTypeSize() const { return m_iBlkLenDataTypeSize; }
    bool insertBlkLen() const { return m_bInsertBlkLen; }
    bool removeBlkLen() const { return m_bRemoveBlkLen; }
private: // default ctor not allowed
    CBlkTypeSL();
protected: // instance members
    QByteArray      m_byteArrBlkStart;
    bool            m_bInsertBlkStart;
    bool            m_bRemoveBlkStart;
    EBlkLenDataType m_blkLenDataType;
    int             m_iBlkLenDataTypeSize;
    bool            m_bInsertBlkLen;
    bool            m_bRemoveBlkLen;

}; // class CBlkTypeSL

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_BlockTypeSL_h
