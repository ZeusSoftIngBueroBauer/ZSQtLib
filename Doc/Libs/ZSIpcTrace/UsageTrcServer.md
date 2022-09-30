@page _PAGE_Libs_ZSIpcTrace_Usage_TrcServer Instantiating and starting the Trace Server

The following code examples are taken from the test application ZSIpcTrace.

In this application both an instance of class CIpcTrcServer and CIpcTrcClient is created.

The CIpcTrcClient is used in the test application together with the trace output window.
The ZSIpcTrace test application should test both - the server and the client site.

But here we consider only the server site as this is what you need to integrate

- online remote tracing and
- tracing to a local log file

into your application.

First of all a trace server has to be created. This must be done before starting the
event loop of the application. A good place for this is the constructor of the application.

After creating the trace server settings got to be applied if these differ from the default settings.

On the one hand the host settings (e.g. the local port the TCP server is listening for incoming
connections) but also settings affecting method tracing may be modified.

Please refer to ZS::Ipc::SServerHostSettings for futher details about the settings which can be
applied to the TCP server and refer to ZS::System::STrcServerSettings which settings can be applied
for method tracing through the trace server.

The trace server is a singleton class which has to be created by the static method
CIpcTrcServer::CreateInstance which creates a trace server named 'ZSTrcServer'.

As default the trace server will listen at port 24763 for incoming connections. If this port is
already used you need to change the port.

After the trace server has been created the previously used settings for the trace server got to be recalled.
You may use the QSettings class to store the hostSettings and the traceSettings. But you only need to save
those values which differ from the default values. How this can be accomplished is not shown in the code
lines below but this should be simple enough to implement your own 'readSettings' and 'saveSettings' methods
and include the host and trace settings for the trace server.

During runtime the trace admin objects will be changed concerning their enabled and detail level states.
After creating the trace server the states of the trace admin objects should be recalled to start with
the previously used trace detail levels. That's been done by invoking recallAdminObjs as shown below after
the trace server has been created.

    CApplication::CApplication() :
       ...
    {
        ...
        m_pZSTrcServer = CIpcTrcServer::CreateInstance();
        m_pZSTrcServer->recallAdminObjs();
        ...
    }

On closing the application the trace server got to be destroyed. This has to be done by invoking
method ZS::System::CTrcServer::ReleaseInstance. But before destroying the trace server the states of the
trace admin objects should be saved so that they can be recalled when restarting the application.

    CApplication::~CApplication()
    {
        ...
        if( m_pZSTrcServer != nullptr )
        {
            m_pZSTrcServer->saveAdminObjs();

            try
            {
                ZS::Trace::CTrcServer::ReleaseInstance();
            }
            catch(...)
            {
            }
        }
        m_pZSTrcServer = nullptr;
        ...
    }

After creating the server it must be started using the 'startup' method.

The QTcpServer is instantiated in a gateway thread and listens there for incoming connections.
If data receives the data will be parsed and split into blocks depending on the defined block type.
The IpcServer of the trace server uses the block type "<Len>[Data]" with the block data length
encoded as binary data in 4 bytes (EBlkLenDataTypeBinUINT32).

As the QTcpServer lives in a separated thread preparsing and splitting the received data into blocks
will not block the main thread. If a complete block has been received the block will be wrapped into
a message and sent from the gateway thread to the Ipc Server instance (which very likely lives in
the main thread).

In the constructor of class CTrcIpcServer the trace server connects the slot `onIpcServerReceivedData`
to the signal `receivedData` of the Ipc server.

Data which may be sent to the TrcIpcServer are commands to modify the trace admin objects (enabled,
trace detail level) or properties (STrcSettings) of the TrcIpcServer itself.

Starting the server is an asynchronous request as the startup must be synchronized with the
gateway thread. A CRequest instance is returned when starting the server.

As arguments you may pass

- a timeout in milliseconds and
- a wait flag defining whether the method should immediately return or return only after
  the specified timeout or the gatweay thread has been started and QTcpServer has been
  created in the gateway thread.

    void CTest::doTestStepTraceServerStartup( ZS::Test::CTestStep* i_pTestStep )
    {
        ...
        CRequest* pReq = pTrcServer->startup(5000);

        if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "startupTrcServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            Trace Server state should be `Listening`.
        }
        ...
    }

If the server is no longer needed it should be shutdown before it is destroyed.

Shutting down the server is also an asynchronous request as the gateway thread has to be stopped
and destroyed afterwards.

    void CTest::doTestStepTraceServerShutdown( ZS::Test::CTestStep* i_pTestStep )
    {
        ...
        CRequest* pReq = pTrcServer->shutdown();

        if( isAsynchronousRequest(pReq) )
        {
            strResultValue = "shutdownTrcServer not expected to be asynchronous";
        }
        else // if( isAsynchronousRequest(pReq) )
        {
            Trace Server state should be `Idle`.
        }
        ...
    }

