@page _PAGE_Libs_ZSIpcLog_Usage_LogServer Instantiating and starting the Log Server

The following code examples are taken from the test application ZSIpcLog.

In this application both an instance of class CIpcLogServer and CIpcLogClient is created.

The CIpcLogClient is used in the test application together with the log output window.
The ZSIpcLog test application should test both - the server and the client site.

But here we consider only the server site as this is what you need to integrate

- online remote logging and
- logging to a local log file

into your application.

First of all a log server has to be created. This must be done before starting the
event loop of the application. A good place for this is the constructor of the application.

After creating the log server settings got to be applied if these differ from the default settings.

On the one hand the host settings (e.g. the local port the TCP server is listening for incoming
connections) but also settings affecting logging may be modified.

Please refer to ZS::Ipc::SServerHostSettings for futher details about the settings which can be
applied to the TCP server and refer to ZS::System::SLogServerSettings which settings can be applied
for logging through the log server.

The log server is a singleton class which has to be created by the static method
CIpcLogServer::CreateInstance which creates a log server named 'theInst'.

As default the log server will listen at port 24762 for incoming connections. If this port is
already used you need to change the port.

After the log server has been created the previously used settings for the log server got to be recalled.
You may use the QSettings class to store the hostSettings and the logSettings. But you only need to save
those values which differ from the default values. How this can be accomplished is not shown in the code
lines below but this should be simple enough to implement your own 'readSettings' and 'saveSettings' methods
and include the host and log settings for the log server.

During runtime the logger instances will be changed concerning their enabled and detail level states.
After creating the log server the states of the logger instances should be recalled to start with
the previously used log detail levels. That's been done by invoking recallLoggers as shown below after
the log server has been created.

    CApplication::CApplication() :
       ...
    {
        ...
        m_pLogServer = CIpcLogServer::CreateInstance();
        m_pLogServer->recallAdminObjs();
        ...
    }

On closing the application the log server got to be destroyed. This has to be done by invoking
method ZS::System::CLogServer::ReleaseInstance. But before destroying the log server the states of the
logger instances should be saved so that they can be recalled when restarting the application.

    CApplication::~CApplication()
    {
        ...
        if( m_pLogServer != nullptr )
        {
            m_pLogServer->saveAdminObjs();

            try
            {
                ZS::System::CLogServer::ReleaseInstance();
            }
            catch(...)
            {
            }
        }
        m_pLogServer = nullptr;
        ...
    }

After creating the server it must be started using the 'startup' method.

The QTcpServer is instantiated in a gateway thread and listens there for incoming connections.
If data receives the data will be parsed and split into blocks depending on the defined block type.
The IpcServer of the log server uses the block type "<Len>[Data]" with the block data length
encoded as binary data in 4 bytes (EBlkLenDataTypeBinUINT32).

As the QTcpServer lives in a separated thread preparsing and splitting the received data into blocks
will not block the main thread. If a complete block has been received the block will be wrapped into
a message and sent from the gateway thread to the Ipc Server instance (which very likely lives in
the main thread).

In the constructor of class CLogIpcServer the log server connects the slot `onIpcServerReceivedData`
to the signal `receivedData` of the Ipc server.

Data which may be sent to the LogIpcServer are commands to modify the logger instances (enabled,
log detail level) or properties (SLogSettings) of the LogIpcServer itself.

Starting the server is an asynchronous request as the startup must be synchronized with the
gateway thread. A CRequest instance is returned when starting the server.

As arguments you may pass

- a timeout in milliseconds and
- a wait flag defining whether the method should immediately return or return only after
  the specified timeout or the gatweay thread has been started and QTcpServer has been
  created in the gateway thread.

    void CTest::doTestStepLogServerStartup( ZS::Test::CTestStep* i_pTestStep )
    {
        ...
        CRequest* pReq = pLogServer->startup(5000);

        if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "startupLogServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            Log Server state should be `Listening`.
        }
        ...
    }

If the server is no longer needed it should be shutdown before it is destroyed.

Shutting down the server is also an asynchronous request as the gateway thread has to be stopped
and destroyed afterwards.

    void CTest::doTestStepLogServerShutdown( ZS::Test::CTestStep* i_pTestStep )
    {
        ...
        CRequest* pReq = pLogServer->shutdown();

        if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "shutdownLogServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            Log Server state should be `Idle`.
        }
        ...
    }

