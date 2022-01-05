/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license reszictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#ifndef ZSIpcTrace_DllIf_h
#define ZSIpcTrace_DllIf_h

//******************************************************************************
/*! Diese Header File stellt das DLL Interface für das Remote Method Tracing.
    der ZSQtLib dar. Um es verwenden zu koennen, muss auch das cpp File in die
    Anwendung oder die Dll compiliert werden, in der das Interface verwendet
    soll.

    Die Anwendung oder Dll, die die ZSIpcTrace Dlls ueber das ZSIpcTrace Dll Interface
    einbindet, wird im folgenden als Master-Anwendung bzw. Master-Dll bezeichnet.

    Dieses File als auch allen anderen fuer das Remote Method Tracing notwendigen
    Dateien koennen ueber das Git-Repository https://github.com/ZeusSoftIngBueroBauer/ZSQtLib
    bezogen werden.

    - Erstellen eines Verzeichnisses fuer das lokale Reposity.
      (z.B. "mkdir \Projects\ZeusSoft")
    - git clone https://github.com/ZeusSoftIngBueroBauer/ZSQtLib
    - cd ZSQtLib
    - git checkout origin/main

    Das Header File "ZSIpcTrcDllIf.h" ist im Unterverzeichnis "Include\Libs\ZSIpcTrace" zu finden.
    Das Source File "ZSIpcTrcDllIf.cpp" ist im Unterverzeichnis "Source\Libs\ZSIpcTrace" zu finden.

    Zunaechst ist es ratsam, sowohl das Header File "ZSIpcTrcDllIf.h" als auch
    das Source File "ZSIpcTrcDllIf.cpp" an eine Stelle zu kopieren, in der die
    Files in die Master-Anwendung includiert und mit compiliert werden koennen.

    Das Dll Interface ist eine reine C Schnittstelle, verwendet nicht die STL
    und auch keine C++ spezifischen Konstrukte.

    Falls die Klassen der ZSQtLib Bibliotheken "ZSSys", "ZSIpc" und "ZSIpcTrace"
    nicht in die Anwendung oder Dll compiliert und gelinkt werden koennen, kann
    trotzdem ZSIpcTrace verwendet werden.

    Wird das ZSIpcTrace Dll Interface verwendet, werden die ZSQtLib Dlls
    dynamisch zur Laufzeit geladen.

    Das ZSIpcTrace Dll Interface kann sowohl dann verwendet werden, wenn die
    Master-Anwendung oder Master-Dll selbst Qt verwendet aber auch, wenn die
    Master-Anwendung kein Qt verwendet. Im zweiten Fall muss zum Compilieren
    der ZSQtLib-Dlls vorher eine Qt Version installiert werden.

    Hierzu muessen die ZSQtLib-Dlls separat erstellt werden. Dabei ist darauf zu
    achten, dass derselbe Compiler verwendet wird, mit der auch die Master Anwendung
    erstellt wird.

    Falls es sich bei der Master-Anwendung um eine Anwendung handelt, die selbst
    Qt verwendet, ist ferner darauf zu achten, dass die ZSQtLib Dlls auch mit
    dieser Qt Version erstellt werden (zumindest sollte die Qt Major Version
    uebereinstimmen).

    Manche Master-Anwendungen benennen die Qt Dlls durch Definition eines
    "Lib-Infixes" um. Dieser "Lib-Infix" ist auch bei der Erstellung der ZSQtLib
    Dlls zu verwenden, da ansonsten die Qt-Applikation zweimal intstanziiert
    wird, was zwangsweise dazu fuehrt, dass die Master-Applikation nicht mehr
    korrekt funktioniert und wahrscheinlich sogar abstuerzt.

    Selbstverstaendlich duerfen auch Debug und Release Versionen wie auch
    Prozessor-Architektur hinsichtlich 32 und 64 Bit Anwendung nicht vermischt
    werden.

    Um etwaiige Versions-Konflikte verwendeter Compiler-, Qt-, Debug/Release,
    Win32/x64 Builds weitgehends vermeiden zu koennen, werden die ZSQtLib
    Dlls entsprechend benamst.

    So erhalten die mit dem Microsoft Visual Studio 2017 (Version 15) in der
    Debug Version für 64 Bit unter Vewendung von Qt 5.5 kompilierten Dlls
    folgende Namen:

    - ZSSysQt5_msvc2017_x64_d.dll
    - ZSIpcQt5_msvc2017_x64_d.dll
    - ZSIpcTraceQt5_msvc2017_x64_d.dll

    Sollten die Qt-Dlls mit dem Lib-Infix "Swp" erstellt worden sein,
    erhalten die Dlls folgende Namen.

    - ZSSysQt5Swp_msvc2017_x64_d.dll
    - ZSIpcQt5Swp_msvc2017_x64_d.dll
    - ZSIpcTraceQt5Swp_msvc2017_x64_d.dll

    Bitte lesen Sie die Dokumentation in "Make/CMakeLists.txt" um zu erfahren,
    wie die ZSQtLib Dlls zu erstellen sind.

    Hier wird beschrieben, wie das Remote Method Tracing über das Dll Interface
    anzusprechen und zu verwenden ist.

    Fuer weiter gehende Informationen ueber Features und Internas des Remote
    Method Tracings lesen Sie bitte die entsprechende Online Dokumentation,
    die ueber Doxygen erstellt werden kann.

    Ferner gibt es im Repository Test Anwendungen, sowohl fuer eine Qt Applikation
    als auch eine native Windows Anwendungen, die Sie als Copy and Paste
    Vorlage verwenden koennen.

    Haben Sie, wie anfangs beschrieben, sowohl das Header File "ZSIpcTrcDllIf.h"
    als auch das Source File "ZSIpcTrcDllIf.cpp" an eine Stelle kopiert, von dem
    aus die Files in die Master-Anwendung includiert und mit compiliert werden koennen,
    muss zunaechst ggf. das Include auf "ZSIpcTrcDllIf.h" im Source File "ZSIpcTrcDllIf.cpp"
    angepasst werden. Moeglicherweise muessen Sie "StdAfx.h" vor "ZSIpcTrcDllIf.h"
    includieren und den Include Pfad fuer "ZSIpcTrcDllIf.h" korrigieren.

    Falls sie eine Qt Anwendung verwenden, sollte vor der Include Anweisung fuer "ZSIpcTrcDllIf.h"
    eine Qt-Header Datei includiert werden, um die verwendete QT_VERSION bekannt zu machen:

        @code
        #include <QtCore/qglobal.h>.
        #include "ZSIpcTrcDllIf.h"
        @endcode

    Falls sie keine Qt Anwendung verwenden, und nicht die QT_VERSION 5 verwenden, muss QT_VERSION
    vor includieren von "ZSIpcTrcDllIf.h" entsprechend gesetzt werden:

        @code
        #include <QtCore/qglobal.h>.
        #include "ZSIpcTrcDllIf.h"
        @endcode

    Stellen Sie sicher, dass das Source File "ZSIpcTrcDllIf.cpp" compiliert werden
    kann.

    1. Laden der ZSQtLib-Dlls

        Die erste Aufgabe besteht nun darin, die erforderlichen ZSQtLib Dlls in die
        Master Anwendung zu laden. Idealerweise erfolgt dies im Main-Modul der Anwendung.
        Falls Sie keine Moeglichkeit haben, die Dlls im Main-Modul der Anwendung zu
        laden und stattdessen das Remote Method Tracing nur in einem oder mehreren
        PlugIn-Dlls verwenden wollen, koennen oder duerfen, waehlen Sie hierzu das
        geeignete Main-Module der PlugIn-Dll aus.

        Geladen werden die Remote Method Tracing Dlls ueber folgenden Aufruf:

        @code
        bool bZSQtLibDllsLoaded = ZS::Trace::DllIf::loadDll();
        @endcode

        or ..

        @code
        bool bZSQtLibDllsLoaded = ZS::Trace::DllIf::loadDll("msvc2013", "x64", EBuildConfigurationDebug, 5);
        @endconde

        Moegliche Ursachen dafuer, dass die Dlls nicht geladen werden konnten, sind:

        - Dlls wurden nicht gefunden
          - Ggf. PATH erweitern, so dass das Betriebssystem die Dlls finden koennen.
          - Dateinamen sind nicht korrekt (alle Lib-Infixes pruefen).
        - Incompatible Versionen (x64/Win32, Qt Version 4 statt 5, etc.).
        - Qt Dlls konnten nicht geladen werden
          - Wurden die Qt Dlls ggf. mit einem LibInfix compiliert?
          - Kann das Betriebssystem die Qt-Dlls finden?

    2. Instanziieren des Trace Servers, einlesen des Trace Admin Object XML Files und setzen einiger Attribute.

        Falls das Laden der ZSQtLib Dlls erfolgreich war, kann der Trace Server instanziiert werden.
        Dies erfolgt ueber einen CreateInstance Aufruf. Wird CreateInstance mit den Default-Parametern
        aufgeufen, wird geprueft, ob es eine Trace Server Instanz mit dem Namen "ZSTrcServer" bereits
        gibt. Wenn ja, wird ein Referenz-Counter incrementiert und eine Referenz auf diese Instanz
        zurückgegeben. Existiert noch kein Trace Server mit dem Namen "ZSTrcServer" wird eine neue
        Instanz angelegt und der Referenz-Counter auf 1 gesetzt.

        Damit der Trace Server weiss, wo er das Log File und das Settings File ablegen soll,
        sollte der Name der Applikation und der Name der Organisation mitgeteilt werden, wie im
        folgenden Code-Abschnitt gezeigt.

        @code
        if( bZSQtLibDllsLoaded )
        {
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

        Mit GetDefaultFilePaths kann gepueft werden, ob der Trace Server die gewuenschten Pfade verwendet.

        @code
        if( bZSQtLibDllsLoaded )
        {
            char* szAdminObjFileAbsFilePath = CIpcTrcServer::GetDefaultAdminObjFileAbsoluteFilePath();
            char* szLocalTrcFileAbsFilePath = CIpcTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath();

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

        Wurde der Trace Server erfolgreich erzeugt ist der zuletzt gesicherte Stand der TraceAdmin Objekt
        Instancen in den Index Tree des Trace Servers einzulesen (trace detail level, enabled).

        Des weiteren koennen einige Eigenschaften der Trace Server gesetzt werden. Im Folgenden Code
        Beispiel wird das Caching des Methoden Trace aktiviert, damit auch die Methoden im Client
        erscheinen, die getraced wurden, waehrend der Client sich noch nicht verbunden hatte
        (Methoden Trace beim Startup der Applikation).

        @code
        if( m_pTrcServer != nullptr )
        {
            m_pTrcServer->setCacheTrcDataIfNotConnected(true);
            m_pTrcServer->setCacheTrcDataMaxArrLen(1000);

            m_pTrcServer::recallAdminObjs();
        }
        @endcode

    3. Admin Object XML File sichern und Trace Server freigeben

        Eigentlich der letzte Schritt der Sequenz. Aber in jedem Fall notwendig und sollte deshalb
        gleich nach Schritt 2 implementiert werden.
        Wird der Trace Server nicht mehr gebraucht, muss dieser freigegeben werden.
        Dies erfolgt ueber einen ReleaseInstance Aufruf, bei dem die bei ReleaseInstance erhaltene
        Referenz auf den Trace Server zu uebergbeben ist.
        Beim ReleaseInstance wird der Referenz-Counter decrementiert. Erreicht der Referenz-Counter
        den Wert 0 wird der Trace Server zerstoert. Der Destructor des Trace Servers beendet auch
        den Gateway Thread und zerstoert damit auch den Ipc server. Alle aktiven Verbindungen
        werden dabei geschlossen.

        Bevor der Trace Server freigegeben wird sollte der aktuelle Zustand der Trace Admin Objekte
        im XML File gesichert werden, damit die Zustaende (trace detail level, enabled) beim Neustart
        der Applikation wieder hergestellt werden koennen.

        @code
        if( m_pTrcServer != nullptr )
        {
            m_pTrcServer->saveAdminObjs();

            CIpcTrcServer::ReleaseInstance(m_pTrcServer);
        }
        @endcode

    4. Trace Server starten

        Der Ipc Server des Trace Servers wird in einem eigenen Gateway Thread
        instanziiert und wartet dort auf ankommende Verbindungsanforderungen.
        Das starten des Trace Servers ist ein asynchroner Vorgang der mit
        der Methode "startup" in Gang gesetzt wird. Ruft man diese Methode
        mit den Uebergabeparametern auf, returniert die Methode erst, wenn
        der Gateway Thread gestartet wurde und sich der Ipc Server im Listen-Mode
        befindet. Oder nach Ablauf des mit 5000 ms bemessenen timeouts.

        @code
        if( m_pTrcServer != nullptr )
        {
            CIpcTrcServer::startup();
        }
        @endcode

*/
//******************************************************************************

/*******************************************************************************
public type definitions and constants
*******************************************************************************/

#ifndef QT_VERSION_MAJOR
#if QT_VERSION >= 0x050000
#define QT_VERSION_MAJOR 5
#elif QT_VERSION >= 0x040804
#define QT_VERSION_MAJOR 4
#endif
#endif

#ifndef QT_VERSION_MAJOR
#define QT_VERSION_MAJOR 5
#endif

/*
Visual Studio version            | _MSC_VER
---------------------------------+---------------
Visual Studio 6.0                | 1200
Visual Studio .NET 2002 (7.0)    | 1300
Visual Studio .NET 2003 (7.1)    | 1310
Visual Studio 2005 (8.0)         | 1400
Visual Studio 2008 (9.0)         | 1500
Visual Studio 2010 (10.0)        | 1600
Visual Studio 2012 (11.0)        | 1700
Visual Studio 2013 (12.0)        | 1800
Visual Studio 2015 (14.0)        | 1900
Visual Studio 2017 RTW (15.0)    | 1910
Visual Studio 2017 version 15.3  | 1911
Visual Studio 2017 version 15.5  | 1912
Visual Studio 2017 version 15.6  | 1913
Visual Studio 2017 version 15.7  | 1914
Visual Studio 2017 version 15.8  | 1915
Visual Studio 2017 version 15.9  | 1916
Visual Studio 2019 RTW (16.0)    | 1920
Visual Studio 2019 version 16.1  | 1921
Visual Studio 2019 version 16.2  | 1922
Visual Studio 2019 version 16.3  | 1923
Visual Studio 2019 version 16.4  | 1924
Visual Studio 2019 version 16.5  | 1925
Visual Studio 2019 version 16.6  | 1926
Visual Studio 2019 version 16.7  | 1927
Visual Studio 2019 version 16.8  | 1928
Visual Studio 2019 version 16.10 | 1929
*/
#ifdef _WINDOWS
#ifndef COMPILERLIBINFIX
#if _MSC_VER <= 1200
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2000"
#elif _MSC_VER >= 1300 && _MSC_VER <= 1300
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2002"
#elif _MSC_VER >= 1310 && _MSC_VER <= 1310
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2003"
#elif _MSC_VER >= 1400 && _MSC_VER <= 1400
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2005"
#elif _MSC_VER >= 1500 && _MSC_VER <= 1500
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2008"
#elif _MSC_VER >= 1600 && _MSC_VER <= 1600
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2010"
#elif _MSC_VER >= 1700 && _MSC_VER <= 1700
#define __CXX_STANDARD__ 1
#define COMPILERLIBINFIX "msvc2012"
#elif _MSC_VER >= 1800 && _MSC_VER <= 1800
#define __CXX_STANDARD__ 201402
#define COMPILERLIBINFIX "msvc2013"
#elif _MSC_VER >= 1900 && _MSC_VER <= 1900
#define __CXX_STANDARD__ 201703
#define COMPILERLIBINFIX "msvc2015"
#elif _MSC_VER >= 1910 && _MSC_VER <= 1916
#define __CXX_STANDARD__ 201703
#define COMPILERLIBINFIX "msvc2017"
#elif _MSC_VER >= 1920 && _MSC_VER <= 1929
#define __CXX_STANDARD__ 201703
#define COMPILERLIBINFIX "msvc2019"
#else
#define __CXX_STANDARD__ 1
#endif
#endif
#endif // #ifdef _WINDOWS

#ifndef COMPILERLIBINFIX
#define COMPILERLIBINFIX ""
#endif

#ifdef _WINDOWS
#if (__CXX_STANDARD__ == 201703)
#define CXX_STANDARD 17
#elif (__CXX_STANDARD__ == 201402)
#define CXX_STANDARD 14
#elif (__CXX_STANDARD__ == 201103)
#define CXX_STANDARD 11
#elif (__CXX_STANDARD__ == 19971)
#define CXX_STANDARD 98
#elif (__CXX_STANDARD__ == 1)
#define CXX_STANDARD 1
#ifndef nullptr
#define nullptr 0
#endif
#endif
#endif // #ifdef _WINDOWS

// Some customers are configuring Qt to rename the Qt libraries to Qt*<infix>.
#define QTLIBINFIX ""
//#define QTLIBINFIX "Isar"
//#define QTLIBINFIX "SWP"

#ifndef PLATFORMLIBINFIX
#ifdef _WIN64
#define PLATFORMLIBINFIX "x64"
#else
#define PLATFORMLIBINFIX "Win32"
#endif
#endif

#ifndef PLATFORMLIBINFIX
#define PLATFORMLIBINFIX ""
#endif

#ifndef CONFIGLIBINFIX
#ifdef _DEBUG
#define CONFIGLIBINFIX "d"
#else
#define CONFIGLIBINFIX ""
#endif
#endif

extern "C"
{
namespace ZS
{
namespace Trace
{
namespace DllIf
{
class CTrcServer;

//==============================================================================
/*! Indicates whether an optional value is enabled or disabled.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EEnabled
//==============================================================================
{
    EEnabledNo        = 0, /*! Indicates that the option is disabled (alias for false). */
    EEnabledYes       = 1, /*! Indicates that the option is enabled (alias for true). */
    EEnabledUndefined = 2  /*! The variable is not yet initialised and its value is unknown. */
};

//==============================================================================
/*! Predefined trace detail levels.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    Higher detail levels include lower detail levels.
    Please note that this enum is only a suggestion for trace detail levels
    which can be used. The detail level of the trace admin object is an integer
    type and user defined values can be used if necessary.
    To avoid type casts requested by the compiler this enum is not a class enum definition.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ETraceDetailLevel
//==============================================================================
{
    ETraceDetailLevelUndefined      = -1,    /*!< Used e.g. to indicate that the trace level should not be used but the predefined detail level should be used. */
    ETraceDetailLevelNone           =  0,    /*!< Trace output is disabled. */
    ETraceDetailLevelMethodCalls    =  1,    /*!< Tracing method entries and leaves without tracing input and output arguments and without method return value. */
    ETraceDetailLevelMethodArgs     =  2,    /*!< Tracing method entries and leaves together with input and output arguemnts and method return value. */
    ETraceDetailLevelInternalStates =  3,    /*!< Tracing everything above and internal states (e.g. current state, current request in progress etc.. */
    ETraceDetailLevelRuntimeInfo    =  4,    /*!< Tracing everything above and additional runtime info (e.g. what is going to be done, what has been executed and how). */
    ETraceDetailLevelVerbose        =  5,    /*!< Tracing everything above and even more. */
    ETraceDetailLevelCount
};

//******************************************************************************
class CTrcAdminObj
//******************************************************************************
{
public: // instance methods
    const char* serverName() const { return m_szServerName; }
    const char* keyInTree() const { return m_szKeyInTree; }
public: // instance methods
    char* getNameSpace() const;     // returned character string must be freed by caller
    char* getClassName() const;     // returned character string must be freed by caller
    char* getObjectName() const;    // returned character string must be freed by caller
public: // instance methods
    void setObjectThreadName( const char* i_szThreadName );
    char* getObjectThreadName() const;  // returned character string must be freed by caller
public: // instance methods
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
public: // instance methods
    void setTraceDetailLevel( int i_iTrcDetailLevel );
    int getTraceDetailLevel() const;
public: // instance methods
    bool isActive( int i_iDetailLevel ) const;
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
    CTrcAdminObj( const char* i_szServerName, const char* i_szKeyInTree );
    ~CTrcAdminObj();
protected: // instance members
    char* m_szServerName;
    char* m_szKeyInTree;

}; // class CTrcAdminObj


//******************************************************************************
class CMethodTracer
//******************************************************************************
{
public: // ctors and dtor
    CMethodTracer(  // instance tracer (name space, class and object name set at trace admin object)
        CTrcAdminObj* i_pTrcAdminObj,
        int           i_iFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const char*   i_szMethod,
        const char*   i_szMethodInArgs );
    CMethodTracer(  // class tracer (name space and class name (but not object name) set at trace admin object)
        CTrcAdminObj* i_pTrcAdminObj,
        int           i_iFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const char*   i_szObjName,
        const char*   i_szMethod,
        const char*   i_szMethodInArgs );
    ~CMethodTracer();
public: // instance methods
    void onAdminObjAboutToBeReleased( bool i_bTraceMethodLeave = true );
public: // instance methods
    int getTraceDetailLevel() const;
    int getEnterLeaveFilterDetailLevel() const { return m_iEnterLeaveFilterDetailLevel; }
public: // instance methods
    const char* getObjectName() const { return m_szObjName; } // This is NOT the object name of the trace admin object
    const char* getMethod() const { return m_szMethod; }
public: // instance methods
    void setMethodReturn( bool i_bResult );
    void setMethodReturn( int i_iResult );
    void setMethodReturn( const char* i_sz );
    char* getMethodReturn() const { return m_szMethodReturn; }
public: // instance methods
    void setMethodOutArgs( const char* i_sz );
    char* getMethodOutArgs() const { return m_szMethodOutArgs; }
public: // instance methods
    bool isActive( int i_iFilterDetailLevel ) const;
public: // instance methods
    void trace( const char* i_szAddInfo, int i_iFilterDetailLevel = ETraceDetailLevelInternalStates ) const;
protected: // instance members
    CTrcAdminObj* m_pTrcAdminObj;
    int           m_iEnterLeaveFilterDetailLevel;
    char*         m_szObjName;
    char*         m_szMethod;
    char*         m_szMethodReturn;
    char*         m_szMethodOutArgs;

}; // class CMethodTracer


//******************************************************************************
struct STrcServerSettings
//******************************************************************************
{
    bool  m_bEnabled;
    bool  m_bNewTrcAdminObjsEnabledAsDefault;
    int   m_iNewTrcAdminObjsDefaultDetailLevel;
    bool  m_bCacheDataIfNotConnected;
    int   m_iCacheDataMaxArrLen;
    char* m_szAdminObjFileAbsFilePath;
    bool  m_bUseLocalTrcFile;
    char* m_szLocalTrcFileAbsFilePath;
    int   m_iLocalTrcFileAutoSaveInterval_ms;
    int   m_iLocalTrcFileSubFileCountMax;
    int   m_iLocalTrcFileSubFileLineCountMax;
    bool  m_bLocalTrcFileCloseFileAfterEachWrite;

}; // struct STrcServerSettings

void STrcServerSettings_init( STrcServerSettings& i_trcSettings );
void STrcServerSettings_release( STrcServerSettings& i_trcSettings );


//******************************************************************************
class CTrcServer
//******************************************************************************
{
public: // class methods to add, remove and modify admin objects
    static CTrcAdminObj* GetTraceAdminObj(
        const char* i_szNameSpace,
        const char* i_szClassName,
        const char* i_szObjName,
        EEnabled    i_bEnabledAsDefault = EEnabledUndefined,
        int         i_iDefaultDetailLevel = ETraceDetailLevelUndefined,
        const char* i_szServerName = "ZSTrcServer" );
    static void ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj, const char* i_szServerName = "ZSTrcServer" );
public: // class method to save/recall admin objects file
    // Set organization and application if the trace server is used in a none Qt Application.
    // Only if the organization and application name is set the default file paths for the
    // trace admin objects and the trace log files may be correctly determined.
    static void SetOrganizationName( const char* i_szName );
    static char* GetOrganizationName(); // returned string must be freed by caller
    static void SetApplicationName( const char* i_szName );
    static char* GetApplicationName();  // returned string must be freed by caller
    static char* GetDefaultAdminObjFileAbsoluteFilePath( const char* i_szIniFileScope = "System" );
    static char* GetDefaultLocalTrcFileAbsoluteFilePath( const char* i_szIniFileScope = "System" );
public: // class methods
    static void RegisterCurrentThread( const char* i_szThreadName );
    static void UnregisterCurrentThread();
    static char* GetCurrentThreadName();    // returned string must be freed by caller
public: // instance methods
    const char* name() const { return m_szName; }
public: // instance methods
    bool isActive() const;
public: // instance methods
    void setEnabled( bool i_bEnabled );
    bool isEnabled() const;
public: // instance methods
    void setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled );
    bool areNewTrcAdminObjsEnabledAsDefault() const;
    void setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel );
    int getNewTrcAdminObjsDefaultDetailLevel() const;
public: // instance methods
    void setAdminObjFileAbsoluteFilePath( const char* i_szAbsFilePath );
    char* getAdminObjFileAbsoluteFilePath() const; // returned string must be freed by caller
public: // instance methods
    bool recallAdminObjs();
    bool saveAdminObjs();
public: // instance methods
    void setUseLocalTrcFile( bool i_bUse );
    bool isLocalTrcFileUsed() const;
    void setLocalTrcFileAbsoluteFilePath( const char* i_szAbsFilePath );
    char* getLocalTrcFileAbsoluteFilePath() const;  // returned string must be freed by caller
    char* getLocalTrcFileCompleteBaseName() const;  // returned string must be freed by caller
    char* getLocalTrcFileAbsolutePath() const;      // returned string must be freed by caller
    bool isLocalTrcFileActive() const;
    void setLocalTrcFileAutoSaveInterval( int i_iAutoSaveInterval_ms );
    int getLocalTrcFileAutoSaveInterval() const;
    void setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile );
    bool getLocalTrcFileCloseFileAfterEachWrite() const;
public: // instance methods
    void setLocalTrcFileSubFileCountMax( int i_iCountMax );
    int getLocalTrcFileSubFileCountMax() const;
    void setLocalTrcFileSubFileLineCountMax( int i_iCountMax );
    int getLocalTrcFileSubFileLineCountMax() const;
public: // instance methods
    void setCacheTrcDataIfNotConnected( bool i_bCacheData );
    bool getCacheTrcDataIfNotConnected() const;
    void setCacheTrcDataMaxArrLen( int i_iMaxArrLen );
    int getCacheTrcDataMaxArrLen() const;
public: // instance methods
    bool setTraceSettings( const STrcServerSettings& i_settings );
    STrcServerSettings getTraceSettings() const;  // !! char pointers returned in the struct must be freed by caller !!
public: // instance methods
    void clearLocalTrcFile();
public: // instance methods
    void traceMethodEnter(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szMethod,
        const char*         i_szMethodInArgs );
    void traceMethodEnter(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szObjName,
        const char*         i_szMethod,
        const char*         i_szMethodInArgs );
    void traceMethod(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szMethod,
        const char*         i_szAddInfo );
    void traceMethod(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szObjName,
        const char*         i_szMethod,
        const char*         i_szAddInfo );
    void traceMethodLeave(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szMethod,
        const char*         i_szMethodReturn,
        const char*         i_szMethodOutArgs );
    void traceMethodLeave(
        const CTrcAdminObj* i_pAdminObj,
        const char*         i_szObjName,
        const char*         i_szMethod,
        const char*         i_szMethodReturn,
        const char*         i_szMethodOutArgs );
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
    CTrcServer( const char* i_szName );
    ~CTrcServer();
protected: // instance members
    char* m_szName;

}; // class CTrcServer


//******************************************************************************
class CIpcTrcServer : public CTrcServer
//******************************************************************************
{
public: // class methods
    static CIpcTrcServer* GetInstance( const char* i_szName = "ZSTrcServer" );
    static CIpcTrcServer* CreateInstance(
        const char* i_szName = "ZSTrcServer",
        int i_iTrcDetailLevel = ETraceDetailLevelNone );
    static void ReleaseInstance( CIpcTrcServer* i_pTrcServer );
public: // instance methods
    bool startup( int i_iTimeout_ms = 5000, bool i_bWait = true );
    bool shutdown( int i_iTimeout_ms = 5000, bool i_bWait = true );
public: // instance methods
    bool isListening() const;
    bool isConnected() const;
public: // instance methods changing and reading the host settings
    bool setPort( unsigned short i_uPort, int i_iTimeout_ms = 5000, bool i_bWait = true);
    unsigned short getPort() const;
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
    CIpcTrcServer( const char* i_szName, int i_iTrcDetailLevel = ETraceDetailLevelNone );
    ~CIpcTrcServer();

}; // class CIpcTrcServer

//==============================================================================
enum EBuildConfiguration
//==============================================================================
{
    EBuildConfigurationRelease    =  0,
    EBuildConfigurationDebug      =  1,
    EBuildConfigurationAutoDetect =  2
};

bool loadDll(
    const char* i_szCompiler = nullptr,
    const char* i_szPlatform = nullptr,
    EBuildConfiguration i_configuration = EBuildConfigurationAutoDetect,
    int i_iQtVersionMajor = QT_VERSION_MAJOR );
const char* getDllFileName();
bool releaseDll();


} // namespace DllIf

} // namespace Trace

} // namespace ZS

} // extern "C"

#endif // #ifndef ZSIpcTrace_DllIf_h
