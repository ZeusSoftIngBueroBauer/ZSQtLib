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
connections) got to be set but also settings affecting method tracing.

Please refer to ZS::Ipc::SServerHostSettings for futher details about the settings which can be
applied to the TCP server and refer to ZS::Trace::STrcServerSettings which settings can be applied
for method tracing throug the trace server.

    CApplication::CApplication() :
       ...
    {
        ZS::Ipc::SServerHostSettings trcServerHostSettings(
            /* strLocalHostName */ "127.0.0.1",
            /* uLocalPort       */ 24763,
            /* uMaxConnections  */ 30 );

        ...
        m_pTrcServer = CIpcTrcServer::CreateInstance("ZSTrcServer", m_iTrcDetailLevelTrcServer);
        ...
    }

