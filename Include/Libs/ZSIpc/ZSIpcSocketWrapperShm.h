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

#ifndef ZSIpc_SocketWrapperShm_h
#define ZSIpc_SocketWrapperShm_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"

#if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
#include <QtCore/qsharedmemory.h>
#else
#include "SMIpc/QtSharedMemory/src/qsharedmemory.h"
#endif

class QTimer;

namespace ZS
{
namespace Ipc
{
static const int c_iTimerPeriodServerCtrlChanListen_ms = 100;
static const int c_iTimerPeriodServerCtrlChanReqConnect_ms = 100;
static const int c_iTimerPeriodServerCtrlChanConConnect_ms = 100;
static const int c_iTimerPeriodSocketCommChanListenForReq_ms = 100;
static const int c_iTimerPeriodSocketCommChanWaitForCon_ms = 100;
static const int c_iTimerPeriodSocketCommChanWatchDog_ms = 1000;
static const int c_iTimeoutServerCtrlChanConConnect_ms = 10*c_iTimerPeriodServerCtrlChanConConnect_ms;

struct SShmMsgHdr;

/*----------------------------------------------------------------------------!!
!! Please note that if a message will be deleted from a memory block          !!
!! that all messages with a higher block number will be moved "upwards".      !!
!! So after unlocking the memory block your current message pointer           !!
!! may immediately become invalid pointing to a completely different          !!
!! message block. So never access data within the memory block if the         !!
!! memory block has not been LOCKED by your process and never store           !!
!! pointers to messages after leaving a LOCK/UNLOCK code segment.             !!
!! If you need access to a message you have written into the memory block     !!
!! after unlocking the memory block (e.g. on waiting for a confirmation from  !!
!! the counterpart) you need to store the message Id and later on try to find !!
!! the message by using the "findMsg" method (after locking the memory block).!!
------------------------------------------------------------------------------*/

#pragma pack(push,1)
//******************************************************************************
struct ZSIPCDLL_API SShmMainHdr
//******************************************************************************
{
    // !! Please be sure to lock the memory block before allocating messages and   !!
    // !! reading or writing data into the memory block. Otherwise other processes !!
    // !! may modify the content of the memory block including deleting or moving  !!
    // !! the message you are currently reading from or writing to.                !!
public: // instance members
    qint32  m_iMsgId;        // Incremented each time a new message will be allocated.
    quint16 m_uMsgCount;     // Number of all messages (no matter what type they are) currently available in the memory block.
    quint16 m_uDataMsgCount; // Number of receive data request messages currently available in the memory block.
    quint32 m_uBlockLen;     // Number of bytes used by all blocks within the memory block (excluding this header).
public: // instance methods
    // Returns a pointer to the message at the specified block index (but does not remove the message).
    // This method is useful to loop through all messages within the memory block.
    SShmMsgHdr* getMsg( QSharedMemory* i_pSharedMemory, quint32 i_uBlock );
    // Searches the memory block for the message with the specified message Id and returns a pointer
    // to the message (but does not remove the message).
    SShmMsgHdr* findMsg( QSharedMemory* i_pSharedMemory, qint32 i_iMsgId );
    // Removes the message at the specified block index.
    void removeMsgAtIdx( QSharedMemory* i_pSharedMemory, quint32 i_uBlock );
    // Removes the message with the specified message id from the shared memory block.
    void removeMsgById( QSharedMemory* i_pSharedMemory, qint32 i_iMsgId );
    // Removes the specified message from the shared memory block.
    // !! Take care when using this method. This method may only be called if the address !!
    // !! of the message has been taken by the getMsg or findMsg methods within the same  !!
    // !! LOCK/UNLOCK block you are calling the removeMsg method. After unlocking the     !!
    // !! memory block other processes may modify the data within the memory block        !!
    // !! including deleting or moving the message blocks.                                !!
    void removeMsg( QSharedMemory* i_pSharedMemory, SShmMsgHdr* i_pMsgHdr );
    // Tries to allocate the specified bytes within the shared memory.
    SShmMsgHdr* allocMsg( QSharedMemory* i_pSharedMemory, EMsgType i_msgType, quint32 i_uMsgLen );
};
//#pragma pack(pop)

//#pragma pack(push,1)
//******************************************************************************
struct ZSIPCDLL_API SShmMsgHdr
//******************************************************************************
{
    ESrvCltType m_srvCltTypeReceiver; // Request may be sent from clients to servers and vice versa.
    EMsgType    m_msgType;            // Range [ReqConnect, ConConnect, ReqDisconnect, ConDisconnect, ReqReceiveData]
    qint32      m_iMsgId;             // Will be set to the message Id of the main header for request and indication messages. Will be set to the message id of the request or indication message for confirmations and acknwoldeged.
    quint32     m_uMsgLen;            // Number of bytes used by the message (excluding this header).
};
//#pragma pack(pop)

//#pragma pack(push,1)
//******************************************************************************
struct ZSIPCDLL_API SShmMsgHdrReqConnect : public SShmMsgHdr
//******************************************************************************
{
    bool    m_bAck;                    // true if the request must be confirmed.
    quint32 m_uBufferSize;             // Requested size in bytes of the shared memory block to be used as the communication channel.
    quint32 m_uPort;                   // The shared memory key name of the communication channel will be set to the key name of the control channel appended with ":<uPort>".
    quint32 m_uBlockLenHostNameClient; // Number of bytes used by the data block.
};
//#pragma pack(pop)

//#pragma pack(push,1)
//******************************************************************************
struct ZSIPCDLL_API SShmMsgReqConnect : public SShmMsgHdrReqConnect
//******************************************************************************
{
    char m_arcHostNameClient[1];    // Data block containing the client's host name.
};
//#pragma pack(pop)

//#pragma pack(push,1)
//******************************************************************************
struct ZSIPCDLL_API SShmMsgHdrReqDisconnect : public SShmMsgHdr
//******************************************************************************
{
    bool m_bAck;  // true if the request must be confirmed.
};
//#pragma pack(pop)

//#pragma pack(push,1)
//******************************************************************************
struct ZSIPCDLL_API SShmMsgReqDisconnect : public SShmMsgHdrReqDisconnect
//******************************************************************************
{
};
//#pragma pack(pop)

//#pragma pack(push,1)
//******************************************************************************
struct ZSIPCDLL_API SShmMsgHdrReqReceiveData : public SShmMsgHdr
//******************************************************************************
{
    quint32 m_uReadPos;      // Current read position.
    quint32 m_uBlockLenData; // Number of bytes used by the data block.
};
//#pragma pack(pop)

//#pragma pack(push,1)
//******************************************************************************
struct ZSIPCDLL_API SShmMsgReqReceiveData : public SShmMsgHdrReqReceiveData
//******************************************************************************
{
public: // instance members
    char m_arcData[1];    // Data block containing the data to be received by the counterpart.
public: // instance methods
    // Reads at most MaxSize bytes from the message starting from the current read
    // position and sets the read position to the end of the read block. If
    // "ReadPos+MaxSize" is equal or exceeds the number of available bytes in the
    // data message only the bytes still available will be read and the message will
    // be removed from shared memory.
    //quint32 read( QSharedMemory* i_pSharedMemory, char* i_pcData, quint32 i_uMaxSize );
    QByteArray read( QSharedMemory* i_pSharedMemory, quint32 i_uMaxSize );
    QByteArray readAll( QSharedMemory* i_pSharedMemory );
};
#pragma pack(pop)

//******************************************************************************
class ZSIPCDLL_API CShmSocketWrapper : public CIpcSocketWrapper
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CShmSocketWrapper"; }
public: // class methods
    static ZS::System::EResult SharedMemoryError2Result( QSharedMemory::SharedMemoryError i_shmError );
public: // ctors and dtor
    CShmSocketWrapper(
        const QString&      i_strObjName,
        ESrvCltType         i_srvCltType,
        int                 i_iSocketId = ZS::Ipc::ESocketIdUndefined,
        Trace::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        int                 i_iTrcMthFileDetailLevel = Trace::ETraceDetailLevelMethodArgs );
    CShmSocketWrapper(
        const QString&      i_strObjName,
        const SSocketDscr&  i_socketDscr,
        QSharedMemory*      i_pSharedMemory,
        Trace::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        int                 i_iTrcMthFileDetailLevel = Trace::ETraceDetailLevelMethodArgs );
    virtual ~CShmSocketWrapper();
public: // overridables of base class CIpcSocketWrapper
    virtual QString nameSpace() const override { return NameSpace(); }
    virtual QString className() const override { return ClassName(); }
public: // must overridables of base class CIpcSocketWrapper
    virtual void connectToHost( const QString& i_strRemoteHostName, unsigned int i_uRemotePort, unsigned int i_uBufferSize = 0 ) override;
    virtual bool waitForConnected( int i_iTimeout_ms = 30000 ) override;
    virtual void disconnectFromHost() override;
    virtual bool waitForDisconnected( int i_iTimeout_ms = 30000 ) override;
    virtual void abort() override;
    //virtual qint64 write( const char* i_pcData, qint64 i_iMaxSize );
    virtual qint64 write( const QByteArray& i_byteArr ) override;
    virtual bool waitForReadyRead( int i_iTimeout_ms = 30000 ) override;
    virtual qint64 bytesAvailable() const override;
    //virtual qint64 read( char* i_pcData, qint64 i_iMaxSize );
    virtual QByteArray read( qint64 i_iMaxSize ) override;
    virtual QByteArray readAll() override;
    virtual ZS::System::EResult error() const override;
    virtual QString errorString() const override;
    virtual ZS::System::SErrResultInfo errResultInfo() const override;
protected: // instance methods
    void createTimer();
protected slots:
    void onTimeoutServerCtrlChanReqConnect();
    void onTimeoutServerCtrlChanConConnect();
    void onTimeoutSocketCommChanListenForReq();
    void onTimeoutSocketCommChanWaitForCon();
    void onTimeoutSocketCommChanWatchDog();
protected: // instance members
    QSharedMemory*             m_pSharedMemoryServerCtrlChan;
    int                        m_iShmMsgReqConnectMsgId;
    mutable int                m_iSharedMemoryServerCtrlChanLockCount;
    QTimer*                    m_pTimerServerCtrlChanReqConnect;
    QTimer*                    m_pTimerServerCtrlChanConConnect;
    int                        m_iTimeElapsedServerCtrlChanConConnect_ms;
    QSharedMemory*             m_pSharedMemorySocketCommChan;
    mutable int                m_iSharedMemorySocketCommChanLockCount;
    QTimer*                    m_pTimerSocketCommChanListenForReq;
    QTimer*                    m_pTimerSocketCommChanWaitForCon;
    QSharedMemory*             m_pSharedMemorySocketCommChanWatchDog;
    QString                    m_strKeySocketCommChanWatchDog; // Key of the counterparts watch dog shared memory
    QTimer*                    m_pTimerSocketCommChanWatchDog; // Timer to check if the counterparts watch dog shared memory exists
    ZS::System::SErrResultInfo m_errResultInfo;

}; // class CShmSocketWrapper

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_SocketWrapperShm_h
