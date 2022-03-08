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

#ifndef ZSIpc_BlockTypeL_h
#define ZSIpc_BlockTypeL_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcBlkType.h"


namespace ZS
{
namespace Ipc
{
//******************************************************************************
class ZSIPCDLL_API CBlkTypeL : public CBlkType
//******************************************************************************
{
    // +--------+----------+
    // | LENgth | Data ... |
    // +--------+----------+
    // The block is synchronized by defining the length of the data block.
    // By setting the InsertBlkLen and RemoveBlkLen flags the gateway may
    // insert the length before sending the data to the peer or may remove
    // the length from the data block received from the peer before providing
    // the data to the ipc client or server.
public: // ctors and dtor
    CBlkTypeL(
        EBlkLenDataType i_blkLenDataType,
        bool            i_bInsertBlkLen = true,
        bool            i_bRemoveBlkLen = true,
        int             i_iBlkLenDataTypeSize = -1 ); // -1 means use default (see enum EBlkLenDataType above)
    CBlkTypeL( const CBlkTypeL& i_blkTypeOther );
    virtual ~CBlkTypeL();
public: // must overridables of base class CBlkType
    virtual CBlkType* clone() const override;
public: // must overridables of base class CBlkType
    virtual QByteArray* createWatchDogBlock() const override;
public: // must overridables of base class CBlkType
    virtual QList<QByteArray> receiveDataBlocks(
        ESrvCltType            i_srvCltType,
        QObject*               i_pObjGtw,
        QObject*               i_pObjSrvClt,
        CIpcSocketWrapper*     i_pSocketWrapper,
        QByteArray*            i_pByteArrWatchDog = nullptr,
        Trace::CMethodTracer*  i_pMethodTracer = nullptr,
        const QList<QObject*>& i_arpTrcMsgLogObjects = QList<QObject*>() ) const override;
    virtual bool writeDataBlock(
        ESrvCltType            i_srvCltType,
        QObject*               i_pObjGtw,
        QObject*               i_pObjSrvClt,
        CIpcSocketWrapper*     i_pSocketWrapper,
        const QByteArray&      i_byteArr,
        bool                   i_bIsWatchDogBlock = false,
        Trace::CMethodTracer*  i_pMethodTracer = nullptr,
        const QList<QObject*>& i_arpTrcMsgLogObjects = QList<QObject*>() ) const override;
public: // instance methods
    EBlkLenDataType blkLenDataType() const { return m_blkLenDataType; }
    int blkLenDataTypeSize() const { return m_iBlkLenDataTypeSize; }
    bool insertBlkLen() const { return m_bInsertBlkLen; }
    bool removeBlkLen() const { return m_bRemoveBlkLen; }
private: // default ctor not allowed
    CBlkTypeL();
protected: // instance members
    EBlkLenDataType m_blkLenDataType;
    int             m_iBlkLenDataTypeSize;
    bool            m_bInsertBlkLen;
    bool            m_bRemoveBlkLen;

}; // class CBlkTypeL

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_BlockTypeL_h
