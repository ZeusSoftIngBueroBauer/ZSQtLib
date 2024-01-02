# Integrate ZSQtLibs IpcMethodTrace

## Dll Interface

- Copy necessary Dll interface files
  ZSQtLib/Include/Libs/ZSIpcTrace/ZSIpcTrcDllIf.h  to display_application/ServiceBase/ServiceBase/pub/
  ZSQtLib/Source/Libs/ZSIpcTrace/ZSIpcTrcDllIf.cpp to display_application/ServiceBase/ServiceBase/src/

- Modify main CMakeLists.txt file in display_application
  - Add option USE_ZS_IPCTRACE_DLL_IF 
    option(USE_ZS_IPCTRACE_DLL_IF "Use TCP/IP Method Tracing" OFF)
  - Set compiler definition USE_ZS_IPCTRACE_DLL_IF
    if(USE_ZS_IPCTRACE_DLL_IF)
        add_definitions(-DUSE_ZS_IPCTRACE_DLL_IF)
    endif()

- Modify CMakeLists.txt file of project ServiceBase
  - Include necessary Dll interface files in project ServiceBase by adding the files to 
    display_application/ServiceBase/CMakeLists.txt
    if(USE_ZS_IPCTRACE_DLL_IF)
        set(SOURCE ${SOURCE} ${CMAKE_CURRENT_SOURCE_DIR}/src/ZSIpcTrcDllIf.cpp)
        set(HEADERS ${HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/pub/ZSIpcTrcDllIf.h)
    endif()
  - Link against dl library (for dlopen etc.)
    if(USE_ZS_IPCTRACE_DLL_IF)
        target_link_libraries(${LIBNAME} PRIVATE ${CMAKE_DL_LIBS})
    endif()

- Run CMake
  The two Dll interface files should have been added to project ServiceBase in QtCreator

- Adjust include path of "ZSIpcTraceDllIf.h" in "ServiceBase/src/ZSIpcTraceDllIf.cpp"

- Edit main.cpp files to load the ZSIpcTrc dlls and start the trace server.
  - Include header file
    #ifdef USE_ZS_IPCTRACE_DLL_IF
    #include "ZSIpcTrcDllIf.h"
    #endif
  - Load Dll, set paths to trace admin objects XML file and to log file,
    start trace server after QApplication has been created,
    start event loop of QApplication,
    stop trace server and unload dlls after QApplication event loop has returned.
    #ifdef USE_ZS_IPCTRACE_DLL_IF
    using namespace ZS::Trace::DllIf;
    bool bZSQtLibDllsLoaded = loadDll();
    CIpcTrcServer* pTrcServer = nullptr;
    if (bZSQtLibDllsLoaded) {
        CIpcTrcServer::SetAdminObjFileAbsoluteFilePath("tmp/displayQt/ZSTrcServer-TrcMthAdminObj.xml");
        CIpcTrcServer::SetLocalTrcFileAbsoluteFilePath("tmp/displayQt/ZSTrcServer-TrcMth.log");
        pTrcServer = CIpcTrcServer::CreateInstance();
        if (pTrcServer != nullptr) {
            pTrcServer->startup();
        }
    }
    #endif
    int iAppResult = app.exec();
    #ifdef USE_ZS_IPCTRACE_DLL_IF
        if (bZSQtLibDllsLoaded) {
            if (pTrcServer != nullptr) {
                pTrcServer->shutdown();
                CIpcTrcServer::ReleaseInstance(pTrcServer);
                pTrcServer = nullptr;
            }
            releaseDll();
        }
    #endif
    return iAppResult;

- Add module with helper functions ZSIpcTrcAux (CMakeLists)
  - Add methods to check whether trace output is active:
    bool areMethodCallsActive(CTrcAdminObj* i_pTrcAdminObj, EMethodTraceDetailLevel i_detailLevel)
    {
        return (i_pTrcAdminObj != nullptr) && (i_pTrcAdminObj->areMethodCallsActive(i_detailLevel));
    }
    bool isRuntimeInfoActive(CTrcAdminObj* i_pTrcAdminObj, ELogDetailLevel i_detailLevel)
    {
        return (i_pTrcAdminObj != nullptr) && (i_pTrcAdminObj->isRuntimeInfoActive(i_detailLevel));
    }
  - Add toString method for bool
    QString toString(bool i_bVal)
    {
        return QString(i_bVal ? "true" : "false");
    }
  - Add toString methods for enumerations

- To convert Q_ENUM to string (e.g. VisibilityState)
    Needed: Q_ENUM(VisibilityState)
    QMetaEnum metaEnum = QMetaEnum::fromType<VisibilityState>();
    strMthInArgs = metaEnum.valueToKey(visibility);

- Get trace admin object in constructor
    #ifdef USE_ZS_IPCTRACE_DLL_IF
    using namespace ZS::Trace::DllIf;
    m_pTrcAdminObj = CIpcTrcServer::GetTraceAdminObj(
        "me::UniversalTerminalUI", "Model_ECU_List", "theInst");
    #endif

- Release trace admin object in destructor
    #ifdef USE_ZS_IPCTRACE_DLL_IF
    using namespace ZS::Trace::DllIf;
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "dtor",
        /* szMthInArgs  */ "" );
    #endif
    ....
    #ifdef USE_ZS_IPCTRACE_DLL_IF
    mthTracer.onAdminObjAboutToBeReleased();
    CIpcTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    #endif

- Method tracing
    #ifdef USE_ZS_IPCTRACE_DLL_IF
    using namespace ZS::Trace::DllIf;
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevelArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "methodName",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );
    #endif

## Link to ZSQtLibs

- Modify CMake Settings in Qt Creator
  Project ZSQtLibs: set CMAKE_INSTALL_PREFIX to "../Bin/<Kit>/gnu94_x64"
  Project display_application: add "/home/devel/ZSQtLib/Bin/<Kit>/gnu94_x64" to CMAKE_PREFIX_PATH

- Modify main CMakeLists.txt file in display_application
  - Add option USE_ZS_QTLIBS
    option(USE_ZS_QTLIBS "USE_ZS_QTLIBS" OFF)
  - Set compiler definition USE_ZS_QTLIBS
    if(USE_ZS_QTLIBS)
        add_definitions(-DUSE_ZS_QTLIBS)
    endif()

- Modify CMakeLists.txt file of project ServiceBase
  - Link against dl library (for dlopen etc.)
    if(USE_ZS_IPCTRACE_DLL_IF)
        target_link_libraries(${LIBNAME} PRIVATE ${CMAKE_DL_LIBS})
    endif()
  - Link against ZSQtLibs libraries
    if(USE_ZS_QTLIBS)
        target_link_libraries(${LIBNAME} PUBLIC
            ZSQtLibs::ZSSys ZSQtLibs::ZSIpc ZSQtLibs::ZSIpcTrace)
    endif()

- Run CMake
  The two Dll interface files should have been added to project ServiceBase in QtCreator

- Edit main.cpp files to start the trace server.
  - Include header file
    #ifdef USE_ZS_QTLIBS
    #include "ZSIpcTrace/ZSIpcTrcServer.h"
    #endif
  - Set paths to trace admin objects XML file and to log file,
    start trace server after QApplication has been created,
    start event loop of QApplication,
    stop trace server and unload dlls after QApplication event loop has returned.
    #ifdef USE_ZS_QTLIBS
    using namespace ZS::System;
    CIpcTrcServer::SetAdminObjFileAbsoluteFilePath("tmp/displayQt/ZSTrcServer-TrcMthAdminObj.xml");
    CIpcTrcServer::SetLocalTrcFileAbsoluteFilePath("tmp/displayQt/ZSTrcServer-TrcMth.log");
    CIpcTrcServer* pTrcServer = CIpcTrcServer::CreateInstance();
    if (pTrcServer != nullptr) {
        pTrcServer->startup();
    }
    #endif
    int iAppResult = app.exec();
    #ifdef USE_ZS_QTLIBS
    if (pTrcServer != nullptr) {
        pTrcServer->shutdown();
        CIpcTrcServer::ReleaseInstance();
        pTrcServer = nullptr;
    }
    #endif
    return iAppResult;

- Add module with helper functions ZSIpcTrcAux (CMakeLists)
  - Add toString methods for enumerations

- To convert Q_ENUM to string (e.g. VisibilityState)
    Needed: Q_ENUM(VisibilityState)
    QMetaEnum metaEnum = QMetaEnum::fromType<VisibilityState>();
    strMthInArgs = metaEnum.valueToKey(visibility);

### C++ Module

- Get trace admin object in constructor
    #ifdef USE_ZS_QTLIBS
    using namespace ZS::System;
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        "me::UniversalTerminalUI", "Model_ECU_List", "theInst");
    #endif

- Release trace admin object in destructor
    #ifdef USE_ZS_QTLIBS
    using namespace ZS::System;
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ "" );
    #endif
    ....
    #ifdef USE_ZS_QTLIBS
    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    #endif

- Method tracing
    #ifdef USE_ZS_QTLIBS
    using namespace ZS::System;
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "methodName",
        /* strMthInArgs */ strMthInArgs );
    #endif

### Qml Module

- For each TerminalWidget created with
  `Claas::CareDisplay::SafeWindowManagement::TerminalWidget::createWidget`
  set the the context property int UniversalTerminalUI::startUserInterface as follows:
    #ifdef USE_ZS_QTLIBS
        if (ZS::System::CTrcServer::GetInstance() != nullptr) {
            QQmlContext* pQmlCtx = m_UTCV->getRootContext();
            if (pQmlCtx != nullptr) {
                pQmlCtx->setContextProperty("_ZSSys_trcServer", ZS::System::CTrcServer::GetInstance());
            }
        }
    #endif

- Get trace admin object in Qml module
    //#ifdef USE_ZS_QTLIBS
    property string nameSpace: "me::UniversalTerminalUI::Forms"
    readonly property string className: "Carousel"
    property string objectName : "Carousel"
    property var trcAdminObj: TrcAdminObj {
        nameSpace: carousel.nameSpace
        className: carousel.className
        objectName: carousel.objectName
    }
    Component.onCompleted: {
        console.info("-> Carousel.qml.onCompleted()")
        if( typeof(_ZSSys_trcServer) !== "undefined" ) {
            console.info("   Carousel.qml.onCompleted: -> _ZSSys_trcServer.getTraceAdminObj")
            trcAdminObj = _ZSSys_trcServer.getTraceAdminObj(nameSpace, className, objectName)
            console.info("   Carousel.qml.onCompleted: <- _ZSSys_trcServer.getTraceAdminObj")
        }
        trcAdminObj.traceMethodEnter("EnterLeave", "Component.onCompleted")
        trcAdminObj.traceMethodLeave("EnterLeave", "Component.onCompleted")
        console.info("<- Carousel.qml.onCompleted()")
    }
    Component.onDestruction: {
        trcAdminObj.traceMethodEnter("EnterLeave", "Component.onDestruction")
        trcAdminObj.traceMethodLeave("EnterLeave", "Component.onDestruction")
        if( typeof(_ZSSys_trcServer) !== "undefined" ) {
            _ZSSys_trcServer.releaseTraceAdminObj(trcAdminObj);
        }
    }
    onObjectNameChanged: {
        if( typeof(_ZSSys_trcServer) !== "undefined" ) {
            trcAdminObj = _ZSSys_trcServer.renameTraceAdminObj(trcAdminObj, objectName)
        }
    }
    //#endif

