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

#ifndef ZSIpc_BlockTypeE_h
#define ZSIpc_BlockTypeE_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcBlkType.h"


namespace ZS
{
namespace Ipc
{
//******************************************************************************
class ZSIPCDLL_API CBlkTypeE : public CBlkType
//******************************************************************************
{
    // +----------+-----+
    // | Data ... | END |
    // +----------+-----+
    // The block is synchronized using an end block. The end block must
    // be a pattern which will never occur within the data block.
    // By setting the InsertBlkEnd and RemoveBlkEnd flags the gateway may
    // insert the end block before sending the data to the peer or may remove
    // the end block from the data block received from the peer before providing
    // the data to the ipc client or server.
public: // ctors and dtor
    CBlkTypeE(
        QByteArray i_byteArrBlkEnd,
        bool       i_bInsertBlkEnd = true,
        bool       i_bRemoveBlkEnd = true );
    CBlkTypeE( const CBlkTypeE& i_blkTypeOther );
    virtual ~CBlkTypeE();
public: // must overridables of base class CBlkType
    virtual CBlkType* clone() const;
public: // must overridables of base class CBlkType
    virtual QByteArray* createWatchDogBlock() const;
public: // must overridables of base class CBlkType
    virtual QList<QByteArray> receiveDataBlocks(
        ESrvCltType            i_srvCltType,
        QObject*               i_pObjGtw,
        QObject*               i_pObjSrvClt,
        CIpcSocketWrapper*     i_pSocketWrapper,
        QByteArray*            i_pByteArrWatchDog = nullptr,
        Trace::CMethodTracer*  i_pMethodTracer = nullptr,
        const QList<QObject*>& i_arpTrcMsgLogObjects = QList<QObject*>() ) const;
    virtual bool writeDataBlock(
        ESrvCltType            i_srvCltType,
        QObject*               i_pObjGtw,
        QObject*               i_pObjSrvClt,
        CIpcSocketWrapper*     i_pSocketWrapper,
        const QByteArray&      i_byteArr,
        bool                   i_bIsWatchDogBlock = false,
        Trace::CMethodTracer*  i_pMethodTracer = nullptr,
        const QList<QObject*>& i_arpTrcMsgLogObjects = QList<QObject*>() ) const;
public: // instance methods
    QByteArray blkEnd() const { return m_byteArrBlkEnd; }
    bool insertBlkEnd() const { return m_bInsertBlkEnd; }
    bool removeBlkEnd() const { return m_bRemoveBlkEnd; }
private: // default ctor not allowed
    CBlkTypeE();
protected: // instance members
    QByteArray m_byteArrBlkEnd;
    bool       m_bInsertBlkEnd;
    bool       m_bRemoveBlkEnd;

}; // class CBlkTypeE

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_BlockTypeE_h
