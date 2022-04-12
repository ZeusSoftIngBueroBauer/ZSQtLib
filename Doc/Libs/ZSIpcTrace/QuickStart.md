@page _PAGE_Libs_ZSIpcTrace_QuickStart Quick Start

In order to use method tracing, three main tasks have to be completed.
These three tasks are described in more details in the next chapters.

**Create and Start Trace Server**

Either at the beginning of 'main.cpp' or in the constructor in class derived from 'CApplication'
create and start the trace server.

    CIpcTrcServer* pTrcServer = CIpcTrcServer::CreateInstance();
    pTrcServer->recallAdminObjs();
    pTrcServer->startup();

Either at the end of 'main.cpp' or in the destructor in class derived from 'CApplication'
stop and destroy the trace server.

    CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();
    pTrcServer->saveAdminObjs();
    ZS::Trace::CTrcServer::ReleaseInstance();
    pTrcServer = nullptr;

**Create Trace Admin Objects**

In the constructor of your class allocate the trace admin object.

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

In the destructor of your class release the trace admin object.

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

**Method Tracing**

Instantiate the method tracer in the method to be traced passing the trace admin object.

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

**Method Trace Client Application**

Download the installer from <a href="https://www.zeussoft.de">ZeusSoft</a> or create the application following @ref _PAGE_Main_BuildInstructions.

