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

#ifndef ZSIpcTrace_DllIfServerThread_h
#define ZSIpcTrace_DllIfServerThread_h

#include <QtCore/qthread.h>

#include "ZSSys/ZSSysCommon.h"

class QCoreApplication;

namespace ZS
{
namespace System
{
class CTrcMthFile;
}
namespace Trace
{
namespace DllIf
{
/******************************************************************************/
class CIpcTrcServerThread : public QThread
/******************************************************************************/
{
// The trace server must to be created in the run method of the thread to create
// an event loop. The trace server will therefore be created (and is further on living)
// in a different thread context than the dll interface instance. Signals/slots
// between the server thread and the main thread are not working as the main event
// loop of the Qt application has not been started and is not running.
// The methods of the server interface thread class are very likely called from within the
// main thread of the application.
public: // class methods
    static QString NameSpace() { return "ZS::Trace::DllIf"; }
    static QString ClassName() { return "CIpcTrcServerThread"; }
public: // ctors and dtor
    CIpcTrcServerThread(
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelDllIf = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServer = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerMutex = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServer = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerMutex = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerGateway = ZS::System::EMethodTraceDetailLevel::None );
    virtual ~CIpcTrcServerThread();
public: // instance methods
    bool isServerCreated();
    bool isServerStarted();
    bool isServerShutdown();
public: // overridables of base class QThread
    virtual void run() override;
protected: // instance members
    ZS::System::EMethodTraceDetailLevel m_eTrcDetailLevelDllIf;
    ZS::System::EMethodTraceDetailLevel m_eTrcDetailLevelTrcServer;
    ZS::System::EMethodTraceDetailLevel m_eTrcDetailLevelTrcServerMutex;
    ZS::System::EMethodTraceDetailLevel m_eTrcDetailLevelTrcServerIpcServer;
    ZS::System::EMethodTraceDetailLevel m_eTrcDetailLevelTrcServerIpcServerMutex;
    ZS::System::EMethodTraceDetailLevel m_eTrcDetailLevelTrcServerIpcServerGateway;
    ZS::System::CTrcMthFile* m_pTrcMthFile;

}; // class CIpcTrcServerThread

} // namespace DllIf

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTrace_DllIfServerThread_h
