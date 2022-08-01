@page _PAGE_Libs_ZSIpcLog_DllInterface Using the ZSIpcLog Dll Interface

If

- you want to use method tracing by a none Qt application
- or if for any reason you cannot or don't want to link to the ZSQtLib libraries

you can integrate the method tracing into your application through the Dll interface.

Using the Dll interface the ZSQtLib libraries are loaded during runtime.

The Dll interface is a pure C interface, does not use the STL and also no C++ specific constructs.

If the classes of the ZSQtLib libraries "ZSSys", "ZSIpc" and "ZSIpcTrace" cannot be compiled and
linked into the application or Dll, ZSIpcTrace can still be used.

The application or dll that integrates the ZSIpcTrace Dlls via the ZSIpcTrace Dll interface
is referred to as the master application or master dll in the following.

The ZSIpcTrace Dll interface can be used both when the master application or master dll itself uses
Qt and when the master application does not use Qt. In the second case, a Qt version must be installed
beforehand to compile the ZSQtLib Dlls.

To do this, the ZSQtLib Dlls must be created separately. It is important to ensure that the same
compiler is used that was used to create the master application.

If the master application is an application that uses Qt itself, it must also be ensured that the
ZSQtLib Dlls are also created with this Qt version (at least the Qt major version should match).

There are also test applications in the repository, both for a Qt application and for a
native Windows application, which you can use as a copy and paste template.

Some master applications rename the Qt dlls by defining a "lib infix". This "Lib-Infix" must also
be used when creating the ZSQtLib Dlls, otherwise the Qt application will be instantiated twice,
which inevitably means that the master application will no longer work correctly and will probably
even crash.

Of course, debug and release versions as well as processor architecture with regard to 32 and 64 bit
applications must not be mixed.

In order to avoid possible version conflicts of the compiler, Qt, debug/release, Win32/x64 builds used,
the ZSQtLib Dlls are named accordingly.

The dlls compiled with Microsoft Visual Studio 2017 (version 15) in the debug version for 64 bits using
Qt 5.5 are given the following names:

    - ZSSysQt5_msvc2017_x64_d.dll
    - ZSIpcQt5_msvc2017_x64_d.dll
    - ZSIpcTraceQt5_msvc2017_x64_d.dll

If the Qt dlls were created with the lib infix "Swp", the dlls are given the following names.

    - ZSSysQt5Swp_msvc2017_x64_d.dll
    - ZSIpcQt5Swp_msvc2017_x64_d.dll
    - ZSIpcTraceQt5Swp_msvc2017_x64_d.dll

### Changes to your CMake files

If you are using the Dll interface there is no need to link to the ZQtLib libraries
but you need to include the header file `ZSIpcTrcDllIf.h` in all files you want to use
method tracing.

Please again refer to @ref _PAGE_Main_BuildInstructions on how to make the include path to this
header file available for your application without linking to the libraries.

As mentioned in @ref _PAGE_Main_BuildInstructions you need to add the include path to the
header files of the ZSQtLibs to the `target_include_directories` cmake command as
follows:

    target_include_directories(${MY_PROJECT_NAME}
        <PUBIC|PRIVATE|..>
        <OTHER_INCLUDES>
        ...
        ${ZSQTLIBS_INCLUDE_DIRS}
    )

You also need to add the source file `ZSIpcTrcDllIf.cpp` to the list of source files
of your target (either to `add_executable` as shown below or `add_library` respectively).

    add_executable(${MY_PROJECT_NAME}
        <OTHER_SOURCES>
        ...
        ${ZSQTLIBS_SOURCE_DIRS}/ZSIpcTrace/ZSIpcTrcDllIf.cpp
    )

In addition you need to set the compiler preprocesser directive USE_ZS_IPTRACE_DLL_IF.

    target_compile_definitions(${MY_PROJECT_NAME} <PUBIC|PRIVATE|..> USE_ZS_IPTRACE_DLL_IF)

### Integrate ZS IPC method tracing to your application

1.  Include Dll interface header file

    If you are using a Qt application, a Qt header file should be included before the include statement
    for "ZSIpcTrcDllIf.h" to make known the QT_VERSION used.

    @code
    #include <QtCore/qglobal.h>.
    #include "ZSIpcTrcDllIf.h"
    @endcode

    If you are not using a Qt application and are not using QT_VERSION 5, QT_VERSION must be set accordingly
    before including "ZSIpcTrcDllIf.h".

    @code
    #define QT_VERSION_MAJOR 5
    #include "ZSIpcTrcDllIf.h"
    @endcode

2.  Startup

    2.1 Load ZSQtLib-Dlls

        The first task is to load the required ZSQtLib Dlls into the master application. Ideally, this is done
        either in constructor of the class derived from QApplication (or QCoreApplication) or in the main module of
        the application. If you cannot load the Dlls from those main entry points but instead can only use method
        tracing in one or more plug-in dlls, select the appropriate main module of the plug-in dll.

        **When using the Dll Interface from within an Qt application and loading the Dlls dynamically you must
        first instantiate the QCoreApplication. When loading the Dlls via the Dll interface it will be checked
        whether a QCoreApplication is available. If not a QCoreApplication will be created. If later on you
        also create a QCoreApplication you would have two instance of a singleton class. Bad .... **

        The Remote Method Tracing Dlls are loaded via the following call:

        @code
        bool bZSQtLibDllsLoaded = ZS::Trace::DllIf::loadDll();
        @endcode

        or ..

        @code
        bool bZSQtLibDllsLoaded = ZS::Trace::DllIf::loadDll("msvc2013", "x64", EBuildConfigurationDebug, QT_VERSION_MAJOR);
        @endconde

        Possible reasons why the dlls could not be loaded are:

        - Dlls were not found
            - If necessary, expand PATH so that the operating system can find the Dlls.
        - File names are not correct (check all lib infixes).
        - Incompatible versions (x64/Win32, Qt version 4 instead of 5, etc.).
        - Qt dlls could not be loaded.
            - Were the Qt Dlls possibly compiled with a LibInfix?
            - Can the operating system find the Qt dlls?

    2.1 Instantiating the Trace Server, reading the Trace Admin Object XML file and setting some attributes.

        If the loading of the ZSQtLib Dlls was successful, the Trace Server can be instantiated. This is done
        via a CreateInstance call. If CreateInstance is called with the default parameters, it is checked whether
        a trace server instance with the name "ZSTrcServer" already exists. If so, a reference counter is incremented
        and a reference to that instance is returned. If no trace server with the name "ZSTrcServer" exists yet,
        a new instance is created and the reference counter is set to 1.

        > **If you want to use the Dll interface in a Qt Application instantiating the Trace Server MUST NOT BE DONE
          before the Qt Application is created. The interface does not know about whether later on a Qt Application
          will be created and therefore will create one if not yet existing. Having two QApplication instances
          will not work and Qt will throw a critical exception and the application exits. So if you create a
          trace server instance in a Qt application do it either after the QApplication has been created but before
          starting the event loop or (best solution) in the constructor of the class derived from either QApplication
          or QCoreApplication.**

        In order for the Trace Server to know where to put the log file and the settings file, the name of the
        application and the name of the organization should be specified, as shown in the following code section.

        @code
        if( bZSQtLibDllsLoaded )
        {
            // GetOrganizationName and GetApplicationName will allocate memory for the strings.
            // You must free them if no longer needed to avoid memory leaks.
            char* szOrgName = CIpcTrcServer::GetOrganizationName();
            char* szAppName = CIpcTrcServer::GetApplicationName();

            std::string stdstrOrgName(szOrgName);
            std::string stdstrAppName(szAppName);

            if( stdstrOrgName.empty() )
            {
                CIpcTrcServer::SetOrganizationName("Rohde-Schwarz");
            }
            if( stdstrAppName.empty() )
            {
                CIpcTrcServer::SetApplicationName("CMW");
            }

            m_pTrcServer = CIpcTrcServer::CreateInstance();

            delete szOrgName;
            szOrgName = nullptr;
            delete szAppName;
            szAppName = nullptr;
        }
        @endcode

        GetDefaultFilePaths can be used to check whether the Trace Server is using the desired paths.

        @code
        if( m_pTrcServer != nullptr )
        {
            // GetDefaultAdminObjFileAbsoluteFilePath and GetDefaultLocalTrcFileAbsoluteFilePath will
            // allocate memory for the strings. You must free them if no longer needed to avoid memory leaks.
            char* szAdminObjFileAbsFilePath = m_pTrcServer->GetDefaultAdminObjFileAbsoluteFilePath();
            char* szLocalTrcFileAbsFilePath = m_pTrcServer->GetDefaultLocalTrcFileAbsoluteFilePath();

            std::string stdstrAdminObjFileAbsFilePath(szAdminObjFileAbsFilePath);
            std::string stdstrLocalTrcFileAbsFilePath(szLocalTrcFileAbsFilePath);

            if( stdstrAdminObjFileAbsFilePath.empty() )
            {
            }
            if( stdstrLocalTrcFileAbsFilePath.empty() )
            {
            }

            delete szAdminObjFileAbsFilePath;
            szAdminObjFileAbsFilePath = nullptr;
            delete szLocalTrcFileAbsFilePath;
            szLocalTrcFileAbsFilePath = nullptr;
        }
        @endcode

        If the Trace Server was created successfully, the last saved status of the TraceAdmin object instances
        must be read into the index tree of the Trace Server (trace detail level, enabled).

        Furthermore, some properties of the trace server can be set. In the following code example, the caching
        of the method trace is activated so that the methods that were traced while the client was not yet
        connected also appear in the client (method trace when the application is started up).

        @code
        if( m_pTrcServer != nullptr )
        {
            m_pTrcServer->setCacheTrcDataIfNotConnected(true);
            m_pTrcServer->setCacheTrcDataMaxArrLen(1000);

            m_pTrcServer->recallAdminObjs();
        }
        @endcode

    2.3 Trace Server starten

        The Trace Server's Ipc Server is instantiated in its own gateway thread and waits there for incoming
        connection requests. Starting the Trace Server is an asynchronous process that is started with the
        "startup" method. If you call this method without defining argument, the method only returns when
        the gateway thread has been started and the ipc server is in listen mode. Or after the default
        timeout has elapsed.

        @code
        if( m_pTrcServer != nullptr )
        {
            m_pTrcServer->startup();
        }
        @endcode

3.  Shutdown

    3.1 Save Admin Object XML File and destroy Trace Server

        If the Trace Server is no longer needed, it must be released. This is done via a ReleaseInstance call,
        in which the reference received at ReleaseInstance is to be transferred to the trace server.
        The ReleaseInstance call will decrement the reference counter. If the reference counter reaches the
        value 0, the trace server is destroyed. The trace server's destructor also terminates the gateway thread
        and thus also destroys the ipc server. All active connections will be closed.

        Before the Trace Server is released, the current state of the Trace Admin objects should be saved in the
        XML file so that the states (trace detail level, enabled) can be restored when the application is restarted.

        @code
        if( m_pTrcServer != nullptr )
        {
            m_pTrcServer->saveAdminObjs();

            CIpcTrcServer::ReleaseInstance(m_pTrcServer);
        }
        @endcode

    3.1 Release ZSQtLib-Dlls

        The last task now is to unload the ZSQtLib Dlls. Ideally, this is done either in destructor of the class
        derived from QApplication (or QCoreApplication) or at the end of the main module of the application.
        Or - if the dlls were loaded in a plug-in dll, select the appropriate main module of the plug-in dll.

        @code
        ZS::Trace::DllIf::releaseDll();
        @endcode
