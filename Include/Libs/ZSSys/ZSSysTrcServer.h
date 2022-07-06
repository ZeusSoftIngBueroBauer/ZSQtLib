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

#ifndef ZSSys_TrcServer_h
#define ZSSys_TrcServer_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysErrResult.h"

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>


namespace ZS
{
namespace System
{
class CMsg;
class CMutex;
}

namespace Trace
{
class CTrcAdminObj;
class CIdxTreeTrcAdminObjs;
class CTrcMthFile;

//******************************************************************************
/*! struct STrcServerSettings
*/
struct ZSSYSDLL_API STrcServerSettings
//******************************************************************************
{
public: // ctor
    STrcServerSettings(
        bool i_bEnabled = true,
        bool i_bNewTrcAdminObjsEnabledAsDefault = true,
        ETraceDetailLevelMethodCalls i_eNewTrcAdminObjsDefaultMethodCallsDetailLevel = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelRuntimeInfo i_eNewTrcAdminObjsDefaultRuntimeInfoDetailLevel = ETraceDetailLevelRuntimeInfo::None,
        bool i_bUseIpcServer = true,
        bool i_bCacheDataIfNotConnected = false,
        int  i_iCacheDataMaxArrLen = 1000,
        bool i_bUseLocalTrcFile = true,
        int  i_iLocalTrcFileAutoSaveInterval_ms = 1000,
        int  i_iLocalTrcFileSubFileCountMax = 5,
        int  i_iLocalTrcFileSubFileLineCountMax = 2000,
        bool i_bLocalTrcFileCloseFileAfterEachWrite = false );
public: // operators
    bool operator == ( const STrcServerSettings& i_settingsOther ) const;
    bool operator != ( const STrcServerSettings& i_settingsOther ) const;
public: // struct methods
    QString toString() const;
public: // struct members
    /*!< Tracing may be enabled or disabled for both writing to local trace file and sending data to remote client. */
    bool m_bEnabled;
    /*!< Absolute file path the tree of trace admin objects and their settings will be saved and recalled.
         May be initialized by CTrcServer::GetAdminObjFileAbsoluteFilePath after the applications
         organization and application name have been set. */
    QString m_strAdminObjFileAbsFilePath;
    /*!< Defines whether newly created trace admin objects should be enabled as default. */
    bool m_bNewTrcAdminObjsEnabledAsDefault;
    /*!< Defines the trace detail level for method calls of newly created trace admin objects. */
    ETraceDetailLevelMethodCalls m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel;   
    /*!< Defines the trace detail level for runtime info of newly created trace admin objects. */
    ETraceDetailLevelRuntimeInfo m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;   
    /*!< Defines whether trace output should be send to remote client. */
    bool m_bUseIpcServer;
    /*!< If a trace client is not connect the flag defines whether trace data should be internally cached until a client connects. */
    bool m_bCacheDataIfNotConnected;
    /*!< If caching is enabled defines the maximum number of trace entries which should be locally cached. */
    int m_iCacheDataMaxArrLen;
    /*!< Defines whether trace output should be written to a local trace file. */
    bool m_bUseLocalTrcFile;
    /*!< If a local log file is used defines the absolute file path for the log file.
         May be initialized by CTrcServer::GetLocalTrcFileAbsoluteFilePath after the applications
         organization and application name have been set. */
    QString m_strLocalTrcFileAbsFilePath;
    /*!< Auto save interval for the local log file. */
    int m_iLocalTrcFileAutoSaveInterval_ms;
    /*!< Number of sub files to be created for round robin. The oldest log file will be overwritten. */
    int m_iLocalTrcFileSubFileCountMax;
    /*!< Number of lines which can be written to a log file before the file is closed and the next sub log file is created. */
    int m_iLocalTrcFileSubFileLineCountMax;
    /*!< For hard to find errors the log file may be immediately closed after an entry has been written.
         Use with special care as enabling this feature extremely slows down the program. */
    bool m_bLocalTrcFileCloseFileAfterEachWrite;

}; // struct STrcServerSettings


//******************************************************************************
/*! @brief All trace outputs should be made via the CTrcServer class.

The class manages a tree of trace objects, which can be used to activate and
deactivate the tracing for modules, classes and instances and to specify the
level of detail of the output.

These Trace Admin objects can be asked whether a log output should take place
and which log outputs should be created.

There is only one Trace Server instance per application, which is created when
the application is started by calling the "CreateInstance" class method.

During program execution, a reference to the instance can be obtained via
"GetInstance" and parameters can be changed or log outputs can be made in the
trace method file.

Before exiting the application, the Trace Server instance must be deleted again
with "ReleaseInstance".
*/
class ZSSYSDLL_API CTrcServer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace of the class.
        @note The static class functions name must be different from the instance method "nameSpace". */
    static QString NameSpace() { return "ZS::Trace"; }
    /*! Returns the class name.
        @note The static class functions name must be different from the instance method "className". */
    static QString ClassName() { return "CTrcServer"; }
public: // class methods
    static CTrcServer* GetInstance();
    static CTrcServer* CreateInstance(
        ETraceDetailLevelMethodCalls i_eTrcDetailLevel = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelMutex = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelAdminObjIdxTree = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelAdminObjIdxTreeMutex = ETraceDetailLevelMethodCalls::None );
    static void ReleaseInstance();
public: // class methods to register thread names
    static void RegisterCurrentThread(const QString& i_strThreadName);
    static void UnregisterCurrentThread();
    static QString GetCurrentThreadName();
public: // class methods to add, remove and modify admin objects
    static CIdxTreeTrcAdminObjs* GetTraceAdminObjIdxTree();
    static CTrcAdminObj* GetTraceAdminObj( int i_idxInTree );
    static CTrcAdminObj* GetTraceAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName = "" );
    static CTrcAdminObj* GetTraceAdminObj(
        const QString&               i_strNameSpace,
        const QString&               i_strClassName,
        const QString&               i_strObjName,
        ZS::System::EEnabled         i_bEnabledAsDefault,
        ETraceDetailLevelMethodCalls i_eMethodCallsDefaultDetailLevel,
        ETraceDetailLevelRuntimeInfo i_eRuntimeInfoDefaultDetailLevel );
    static void RenameTraceAdminObj( CTrcAdminObj** io_ppTrcAdminObj, const QString& i_strNewObjName );
    static void ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj );
public: // class methods to get default file paths
    static void SetAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    static QString GetAdminObjFileAbsoluteFilePath();
    static QString GetAdminObjFileCompleteBaseName();
    static QString GetAdminObjFileAbsolutePath();
    static void SetLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    static QString GetLocalTrcFileAbsoluteFilePath();
    static QString GetLocalTrcFileCompleteBaseName();
    static QString GetLocalTrcFileAbsolutePath();
protected: // ctors and dtor
    CTrcServer(
        ETraceDetailLevelMethodCalls i_eTrcDetailLevel = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelMutex = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelAdminObjIdxTree = ETraceDetailLevelMethodCalls::None,
        ETraceDetailLevelMethodCalls i_eTrcDetailLevelAdminObjIdxTreeMutex = ETraceDetailLevelMethodCalls::None );
    virtual ~CTrcServer();
signals:
    /*! Signal which is emitted if a trace setting has been changed.
        @param i_pTrcServer [in] Pointer to object emitting the signal. */
    void traceSettingsChanged( QObject* i_pTrcServer );
public: // instance methods
    /*! Returns the namespace of the class. May be overriden to return the namespace of the derived class. */
    QString nameSpace() const { return NameSpace(); }
    /*! Returns the class name. May be overriden to return the class name of the derived class. */
    QString className() const { return ClassName(); }
public: // instance methods
    CIdxTreeTrcAdminObjs* getTraceAdminObjIdxTree();
public: // instance methods to add, remove and modify admin objects
    virtual CTrcAdminObj* getTraceAdminObj( int i_idxInTree );
    virtual CTrcAdminObj* getTraceAdminObj(
        const QString&               i_strNameSpace,
        const QString&               i_strClassName,
        const QString&               i_strObjName,
        ZS::System::EEnabled         i_bEnabledAsDefault,
        ETraceDetailLevelMethodCalls i_eMethodCallsDefaultDetailLevel,
        ETraceDetailLevelRuntimeInfo i_eRuntimeInfoDefaultDetailLevel );
    virtual void renameTraceAdminObj( CTrcAdminObj** io_ppTrcAdminObj, const QString& i_strNewObjName );
    virtual void releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj );
public: // instance methods
    virtual void traceMethodEnter(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strMethod,
        const QString&      i_strMethodInArgs );
    virtual void traceMethodEnter(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strObjName,
        const QString&      i_strMethod,
        const QString&      i_strMethodInArgs );
    virtual void traceMethod(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strMethod,
        const QString&      i_strAddInfo );
    virtual void traceMethod(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strObjName,
        const QString&      i_strMethod,
        const QString&      i_strAddInfo );
    virtual void traceMethodLeave(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strMethod,
        const QString&      i_strMethodReturn,
        const QString&      i_strMethodOutArgs );
    virtual void traceMethodLeave(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strObjName,
        const QString&      i_strMethod,
        const QString&      i_strMethodReturn,
        const QString&      i_strMethodOutArgs );
public: // instance methods
    virtual void traceMethodEnter(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs );
    virtual void traceMethod(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strAddInfo );
    virtual void traceMethodLeave(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodReturn,
        const QString& i_strMethodOutArgs );
public: // instance methods
    virtual bool isActive() const;
    virtual bool areMethodCallsActive( const CTrcAdminObj* i_pTrcAdminObj ) const;
    virtual bool isRuntimeInfoActive( const CTrcAdminObj* i_pTrcAdminObj ) const;
public: // instance methods
    virtual void setEnabled( bool i_bEnabled );
    virtual bool isEnabled() const;
public: // instance methods
    virtual void setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled );
    virtual bool areNewTrcAdminObjsEnabledAsDefault() const;
    virtual void setNewTrcAdminObjsMethodCallsDefaultDetailLevel( ETraceDetailLevelMethodCalls i_eDetailLevel );
    virtual ETraceDetailLevelMethodCalls getNewTrcAdminObjsMethodCallsDefaultDetailLevel() const;
    virtual void setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( ETraceDetailLevelRuntimeInfo i_eDetailLevel );
    virtual ETraceDetailLevelRuntimeInfo getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel() const;
protected: // instance methods
    virtual void setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    virtual QString getAdminObjFileCompleteBaseName() const;
    virtual QString getAdminObjFileAbsolutePath() const;
public: // instance methods
    virtual ZS::System::SErrResultInfo recallAdminObjs( const QString& i_strAbsFilePath = QString() );
    virtual ZS::System::SErrResultInfo saveAdminObjs( const QString& i_strAbsFilePath = QString() );
protected: // instance methods
    virtual void setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    virtual QString getLocalTrcFileCompleteBaseName() const;
    virtual QString getLocalTrcFileAbsolutePath() const;
public: // instance methods
    virtual void setUseLocalTrcFile( bool i_bUse );
    virtual bool isLocalTrcFileUsed() const;
    virtual bool isLocalTrcFileActive() const;
    virtual CTrcMthFile* getLocalTrcFile();
    virtual void setLocalTrcFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms );
    virtual int getLocalTrcFileAutoSaveIntervalInMs() const;
    virtual void setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile );
    virtual bool getLocalTrcFileCloseFileAfterEachWrite() const;
public: // instance methods
    virtual void setLocalTrcFileSubFileCountMax( int i_iCountMax );
    virtual int getLocalTrcFileSubFileCountMax() const;
    virtual void setLocalTrcFileSubFileLineCountMax( int i_iCountMax );
    virtual int getLocalTrcFileSubFileLineCountMax() const;
public: // instance methods
    virtual void setUseIpcServer( bool i_bUse );
    virtual bool isIpcServerUsed() const;
    virtual void setCacheTrcDataIfNotConnected( bool i_bCacheData );
    virtual bool getCacheTrcDataIfNotConnected() const;
    virtual void setCacheTrcDataMaxArrLen( int i_iMaxArrLen );
    virtual int getCacheTrcDataMaxArrLen() const;
public: // instance methods
    virtual void setTraceSettings( const STrcServerSettings& i_settings );
    virtual STrcServerSettings getTraceSettings() const;
public: // instance methods
    virtual void clearLocalTrcFile();
protected: // instance methods
    virtual void addEntry(
        const QString&         i_strThreadName,
        const QDateTime&       i_dt,
        double                 i_fSysTimeInSec,
        ZS::System::EMethodDir i_mthDir,
        const QString&         i_strNameSpace,
        const QString&         i_strClassName,
        const QString&         i_strObjName,
        const QString&         i_strMethod,
        const QString&         i_strAddInfo = "",
        const QString&         i_strMethodOutArgs = "" );
protected: // auxiliary instance methods
    static QString currentThreadName();
protected: // reference counter
    int getRefCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected: // class members
    static QMutex      s_mtx;       /*!< Mutex to protect the class and instance members of the class for multithreaded access. */
    static CTrcServer* s_pTheInst;  /*!< Pointer to singleton instance. */
    static QHash<Qt::HANDLE, QString>  s_hshThreadNames; /*!< Hash with registered threads (key is thread id, value is name of thread). */
    static QHash<QString, Qt::HANDLE>  s_hshThreadIds;   /*!< Hash with registered threads (key name of thread, value is thread id). */
    static QString s_strAdminObjFileAbsFilePath;         /*!< Absolute file path the tree of trace admin objects and their settings will be saved and recalled. */
    static QString s_strLocalTrcFileAbsFilePath;         /*!< If a local log file is used defines the absolute file path for the log file. */
protected: // instance members
    CIdxTreeTrcAdminObjs*        m_pTrcAdminObjIdxTree; /*<! Index tree containg a hierarchically order tree of the trace admin objects. */
    STrcServerSettings           m_trcSettings;         /*<! Currently used trace settings. */
    CTrcMthFile*                 m_pTrcMthFile;         /*<! Reference to local trace method file. */
    ETraceDetailLevelMethodCalls m_eTrcDetailLevel;     /*<! If the trace server itself got to be traced. */
    int                          m_iRefCount;           /*<! Reference counter for createInstance and releaseInstance. */

}; // class CTrcServer

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSSys_TrcServer_h
