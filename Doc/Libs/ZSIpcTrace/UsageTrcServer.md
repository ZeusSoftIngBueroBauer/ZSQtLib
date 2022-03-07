@page _PAGE_Libs_ZSIpcTrace_Usage_TrcServer Instantiating the Trace Server

The following code examples are taken from the test application ZSIpcTrace.

In this application both an instance of class CIpcTrcServer and CIpcTrcClient is created.

The CIpcTrcClient is used in the trace method client application together with the
trace output window. The ZSIpcTrace test application should test both - the server and
the client site.

But here we consider only the server site as this is what you need to integrate remote
only tracing and logging into your application.

First of all a trace server has to be created. This must be done before starting the
event loop of the application. A good place for this is the constructor of the application.

After creating the trace server settings got to be applied if these differ from the default settings.

On the one hand the host settings (e.g. the local port the TCP server is listening for incoming
connections) but also settings affecting method tracing may be modified.

Please refer to ZS::Ipc::SServerHostSettings for futher details about the settings which can be
applied to the TCP server and refer to ZS::Trace::STrcServerSettings which settings can be applied
for method tracing through the trace server.

If you don't pass any arguments to CIpcTrcServer::CreateInstance a trace server named "ZSTrcServer"
is created. Internally some ZSQtLib modules and classes are trying to access a trace server with this name.
If you create one with this name those ZSQtLib modules and classes are using this trace server to trace
their method calls.

As default the trace server will listen at port 24763 for incoming connections. If this port is
already used or if you want to use more than just one trace server you need to change the port.

After the trace server has been created the previously used settings for the trace server got to be recalled.
You may use the QSettings class to store the hostSettings and the traceSettings. But you only need to save
those values which differ from the default values. How this can be accomplished is not shown in the code
lines below but this should be simple enough to implement your own "readSettings" and "saveSettings" method
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
method ZS::Trace::CTrcServer::ReleaseInstance. But before destroying the trace server the states of the
trace admin objects should be saved so that they can be recalled when restarting the application.

    CApplication::~CApplication()
    {
        ...

        if( m_pZSTrcServer != nullptr )
        {
            m_pZSTrcServer->saveAdminObjs();

            try
            {
                ZS::Trace::CTrcServer::ReleaseInstance(m_pZSTrcServer);
            }
            catch(...)
            {
            }
        }
        m_pZSTrcServer = nullptr;

        ...
    }

In the ZSIpcTrace test application an additional trace server is created in test step doTestStepTestTraceCreateInstance.
This test trace server must use a different port and the host settings will be changed as shown in the example below
for the test trace server.

    void CTest::doTestStepTraceServerStartup( ZS::Test::CTestStep* i_pTestStep )
    {
        ...

        SServerHostSettings hostSettings(24764);
        pTrcServer = ZS::Trace::CIpcTrcServer::GetInstance("TestTrcServer");
        pTrcServer->setHostSettings(hostSettings);
        pTrcServer->changeSettings();

        ...
    }


