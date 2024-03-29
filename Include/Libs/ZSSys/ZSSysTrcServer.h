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
        EMethodTraceDetailLevel i_eNewTrcAdminObjsDefaultMethodCallsDetailLevel = EMethodTraceDetailLevel::None,
        ELogDetailLevel i_eNewTrcAdminObjsDefaultRuntimeInfoDetailLevel = ELogDetailLevel::None,
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
    EMethodTraceDetailLevel m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel;   
    /*!< Defines the trace detail level for runtime info of newly created trace admin objects. */
    ELogDetailLevel m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;   
    /*!< Defines whether trace output should be send to remote client. */
    bool m_bUseIpcServer;
    /*!< If a trace client is not connected the flag defines whether trace data should be internally cached until a client connects. */
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
    Q_PROPERTY(QString nameSpace READ NameSpace CONSTANT)
    Q_PROPERTY(QString className READ ClassName CONSTANT)
    Q_PROPERTY(QString objectName READ objectName CONSTANT)
public: // class methods
    /*! Returns the namespace of the class. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CTrcServer"; }
public: // class methods
    static CTrcServer* GetInstance();
    static CTrcServer* CreateInstance(
        EMethodTraceDetailLevel i_eTrcDetailLevel = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelMutex = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelAdminObjIdxTree = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelAdminObjIdxTreeMutex = EMethodTraceDetailLevel::None );
    static void ReleaseInstance();
public: // class methods to register thread names
    static void RegisterThread( const QString& i_strThreadName, void* i_pvThreadHandle );
    static void UnregisterThread( void* i_pvThreadHandle );
    static QString GetThreadName( void* i_pvThreadHandle );
    static QString GetCurrentThreadName();
public: // class methods
    static CIdxTreeTrcAdminObjs* GetTraceAdminObjIdxTree();
public: // class methods to add, remove and modify admin objects
    static CTrcAdminObj* GetTraceAdminObj( int i_idxInTree );
    static CTrcAdminObj* GetTraceAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName = "" );
    static CTrcAdminObj* GetTraceAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        EEnabled i_bEnabledAsDefault,
        EMethodTraceDetailLevel i_eMethodCallsDefaultDetailLevel,
        ELogDetailLevel i_eRuntimeInfoDefaultDetailLevel );
    static CTrcAdminObj* RenameTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj, const QString& i_strNewObjName );
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
        EMethodTraceDetailLevel i_eTrcDetailLevel = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelMutex = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelAdminObjIdxTree = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelAdminObjIdxTreeMutex = EMethodTraceDetailLevel::None );
    virtual ~CTrcServer();
signals:
    /*! Signal which is emitted if a trace setting has been changed.
        @param i_pTrcServer [in] Pointer to object emitting the signal. */
    void traceSettingsChanged( QObject* i_pTrcServer );
public: // instance methods
    CIdxTreeTrcAdminObjs* getTraceAdminObjIdxTree();
public: // instance methods to add, remove and modify admin objects
    Q_INVOKABLE virtual CTrcAdminObj* getTraceAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strEnabledAsDefault = "Undefined",
        const QString& i_strMethodCallsDefaultDetailLevel = "Undefined",
        const QString& i_strRuntimeInfoDefaultDetailLevel = "Undefined" );
    Q_INVOKABLE void releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj );
    virtual CTrcAdminObj* getTraceAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        EEnabled i_eEnabledAsDefault,
        EMethodTraceDetailLevel i_eMethodCallsDefaultDetailLevel,
        ELogDetailLevel i_eRuntimeInfoDefaultDetailLevel);
    virtual CTrcAdminObj* getTraceAdminObj( int i_idxInTree );
    Q_INVOKABLE CTrcAdminObj* renameTraceAdminObj(
        CTrcAdminObj* i_pTrcAdminObj, const QString& i_strNewObjName );
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
    virtual void setNewTrcAdminObjsMethodCallsDefaultDetailLevel( EMethodTraceDetailLevel i_eDetailLevel );
    virtual EMethodTraceDetailLevel getNewTrcAdminObjsMethodCallsDefaultDetailLevel() const;
    virtual void setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( ELogDetailLevel i_eDetailLevel );
    virtual ELogDetailLevel getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel() const;
protected: // instance methods
    virtual void setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    virtual QString getAdminObjFileAbsoluteFilePath() const;
    virtual QString getAdminObjFileCompleteBaseName() const;
    virtual QString getAdminObjFileAbsolutePath() const;
public: // instance methods
    virtual SErrResultInfo recallAdminObjs( const QString& i_strAbsFilePath = QString() );
    virtual SErrResultInfo saveAdminObjs( const QString& i_strAbsFilePath = QString() );
protected: // instance methods
    virtual void setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    virtual QString getLocalTrcFileAbsoluteFilePath() const;
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
        const QString&   i_strThreadName,
        const QDateTime& i_dt,
        double           i_fSysTimeInSec,
        EMethodDir       i_mthDir,
        const QString&   i_strNameSpace,
        const QString&   i_strClassName,
        const QString&   i_strObjName,
        const QString&   i_strMethod,
        const QString&   i_strAddInfo = "",
        const QString&   i_strMethodOutArgs = "" );
protected: // auxiliary instance methods
    static QString currentThreadName();
protected: // reference counter
    int getRefCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected: // class members
    /*!< Mutex to protect the class and instance members of the class for multithreaded access. */
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    static QRecursiveMutex s_mtx;
    #else
    static QMutex s_mtx;
    #endif
    /*!< Pointer to singleton instance. */
    static CTrcServer* s_pTheInst;
    /*!< Hash with registered threads (key is thread id, value is name of thread). */
    static QHash<Qt::HANDLE, QString> s_hshThreadNames;
    /*!< Hash with registered threads (key name of thread, value is thread id). */
    static QHash<QString, Qt::HANDLE> s_hshThreadIds;
    /*!< Absolute file path the tree of trace admin objects and their settings will be saved and recalled. */
    static QString s_strAdminObjFileAbsFilePath;
    /*!< If a local log file is used defines the absolute file path for the log file. */
    static QString s_strLocalTrcFileAbsFilePath;
protected: // instance members
    /*<! Index tree containg a hierarchically ordered tree of the trace admin objects. */
    CIdxTreeTrcAdminObjs* m_pTrcAdminObjIdxTree;
    /*<! Currently used trace settings. */
    STrcServerSettings m_trcSettings;
    /*<! Reference to local trace method file. */
    CTrcMthFile* m_pTrcMthFile;
    /*<! If the trace server itself got to be traced. */
    EMethodTraceDetailLevel m_eTrcDetailLevel;
    /*<! Reference counter for createInstance and releaseInstance. */
    int m_iRefCount;

}; // class CTrcServer

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_TrcServer_h
