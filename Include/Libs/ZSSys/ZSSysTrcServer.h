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
        int  i_iNewTrcAdminObjsDefaultDetailLevel = 0,
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
public: // struct members
    bool    m_bEnabled;                             /*!< Tracing may be enabled or disabled for the whole server. */
    bool    m_bNewTrcAdminObjsEnabledAsDefault;     /*!< Defines whether newly created trace admin objects should be enabled as default. */
    int     m_iNewTrcAdminObjsDefaultDetailLevel;   /*!< Defines the trace detail level for newly created trace admin objects. */
    bool    m_bCacheDataIfNotConnected;             /*!< If a trace client is not connect the flag defines whether trace data should be internally cached until a client connects. */
    int     m_iCacheDataMaxArrLen;                  /*!< If caching is enabled defines the maximum number of trace entries which should be locally cached. */
    QString m_strAdminObjFileAbsFilePath;           /*!< Absolute file path the tree of trace admin objects and their settings will be saved and recalled. */
    bool    m_bUseLocalTrcFile;                     /*!< Defines whether trace should also be output to a local log file. */
    QString m_strLocalTrcFileAbsFilePath;           /*!< If a local log file is used defines the absolute file path for the log file. */
    int     m_iLocalTrcFileAutoSaveInterval_ms;     /*!< Auto save interval for the local log file. */
    int     m_iLocalTrcFileSubFileCountMax;         /*!< Number of sub files to be created for round robin. The oldest log file will be overwritten. */
    int     m_iLocalTrcFileSubFileLineCountMax;     /*!< Number of lines which can be written to a log file before the file is closed and the next sub log file is created. */
    bool    m_bLocalTrcFileCloseFileAfterEachWrite; /*!< For hard to find errors the log file may be immediately closed after an entry has been written.
                                                         Use with special care as enabling this feature extremely slows down the program. */

}; // struct STrcServerSettings


//******************************************************************************
/*! @brief Über die Klasse CTrcServer sollten alle Trace Ausgaben erfolgen.

    Die Klasse verwaltet einen Baum von Trace Objekten, über die das Tracing für
    Module, Klassen und Instanzen aktiviert und deaktiviert als auch die Detail
    Tiefe der Ausgaben festgelegt werden kann.

    Diese Trace Admin Objekte können befragt werden, ob eine Log Ausgabe erfolgen
    soll und welche Log-Ausgaben zu erstellen sind.

    Normalerweise gibt es pro Applikation nur eine Trace Server Instanz die beim
    Start der Applikation durch Aufruf der Klassenmethode "CreateInstance" angelegt
    wird. Während der Programm-Ausführung kann über "GetInstance" eine Referenz auf
    die Instanz erhalten und Parameter ändern oder Log-Ausgaben in das Trace Method
    File vornehmen. Vor Beenden der Applikation ist die Trace Server Instanz mit
    "ReleaseInstance" wieder zu löschen.

    Für den Fall, dass mehrere Trace Server verwendet werden sollen (verschiedene
    Log Files), kann bei "CreateInstance" ein vom Default Wert "ZSTrcServer"
    abweichender Name übergeben werden. Dieser Name ist bei Aufruf von "GetInstance"
    und "ReleaseInstance" wieder zu verwenden.

    Hat man keinen Einfluss auf den Programmstart, programmiert PlugIn Dlls und
    weiss nicht, ob nicht auch an anderer Stelle der Trace Server verwendet wird,
    muss der Zugriff auf die Trace Server über Referenzzähler kontrolliert werden.
    Deshalb besteht die Möglichkeit bei Aufruf von "CreateInstance" das
    Flag "CreateOnlyIfNotYetExisting" zu übergeben.
*/
class ZSSYSDLL_API CTrcServer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Trace"; }  // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    static QString ClassName() { return "CTrcServer"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // class methods
    static CTrcServer* GetInstance( const QString& i_strName = "ZSTrcServer" );
    static CTrcServer* CreateInstance(
        const QString& i_strName = "ZSTrcServer",
        int i_iTrcDetailLevel = ETraceDetailLevelNone );
    static void ReleaseInstance( const QString& i_strName = "ZSTrcServer" );
    static void ReleaseInstance( CTrcServer* i_pTrcServer );
public: // class methods to register thread names
    static void RegisterCurrentThread(const QString& i_strThreadName);
    static void UnregisterCurrentThread();
    static QString GetCurrentThreadName();
public: // class methods to add, remove and modify admin objects
    static CIdxTreeTrcAdminObjs* GetTraceAdminObjIdxTree( const QString& i_strServerName = "ZSTrcServer" );
    static CTrcAdminObj* GetTraceAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strServerName = "ZSTrcServer" );
    static CTrcAdminObj* GetTraceAdminObj(
        const QString&       i_strNameSpace,
        const QString&       i_strClassName,
        const QString&       i_strObjName,
        ZS::System::EEnabled i_bEnabledAsDefault,
        int                  i_iDefaultDetailLevel,
        const QString&       i_strServerName = "ZSTrcServer" );
    static void ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj, const QString& i_strServerName = "ZSTrcServer" );
public: // class methods to get default file paths
    static QString GetDefaultAdminObjFileAbsoluteFilePath( const QString& i_strIniFileScope = "System" );
    static QString GetDefaultLocalTrcFileAbsoluteFilePath( const QString& i_strIniFileScope = "System" );
protected: // ctors and dtor
    CTrcServer( const QString& i_strName, int i_iTrcDetailLevel = ETraceDetailLevelNone );
    virtual ~CTrcServer();
signals:
    void traceSettingsChanged( QObject* i_pTrcServer );
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CIdxTreeTrcAdminObjs* getTraceAdminObjIdxTree();
public: // overridables to add, remove and modify admin objects
    virtual CTrcAdminObj* getTraceAdminObj(             // For each of the names an empty (or Null) string is allowed.
        const QString&       i_strNameSpace,            // Name space of the objects class (e.g. "ZS::Diagram")
        const QString&       i_strClassName,            // Class name of the object (e.g. "CWdgtDiagram")
        const QString&       i_strObjName,              // "Real" object name (e.g. "PvT" (Power versus Time))
        ZS::System::EEnabled i_bEnabledAsDefault,       // Undefined means use "trcServerSettings".
        int                  i_iDefaultDetailLevel );   // -1 means use "trcServerSettings".
    virtual void releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj );
public: // overridables
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
public: // overridables
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
public: // overridables
    virtual bool isActive() const;
public: // overridables
    virtual void setEnabled( bool i_bEnabled );
    virtual bool isEnabled() const;
public: // overridables
    virtual void setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled );
    virtual bool areNewTrcAdminObjsEnabledAsDefault() const;
    virtual void setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel );
    virtual int getNewTrcAdminObjsDefaultDetailLevel() const;
public: // overridables
    virtual void setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    virtual QString getAdminObjFileAbsoluteFilePath() const;
    virtual ZS::System::SErrResultInfo recallAdminObjs( const QString& i_strAbsFilePath = QString() );
    virtual ZS::System::SErrResultInfo saveAdminObjs( const QString& i_strAbsFilePath = QString() );
public: // overridables
    virtual void setUseLocalTrcFile( bool i_bUse );
    virtual bool isLocalTrcFileUsed() const;
    virtual void setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath );
    virtual QString getLocalTrcFileAbsoluteFilePath() const;
    virtual QString getLocalTrcFileCompleteBaseName() const;
    virtual QString getLocalTrcFileAbsolutePath() const;
    virtual bool isLocalTrcFileActive() const;
    virtual CTrcMthFile* getLocalTrcFile();
    virtual void setLocalTrcFileAutoSaveInterval( int i_iAutoSaveInterval_ms );
    virtual int getLocalTrcFileAutoSaveInterval() const;
    virtual void setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile );
    virtual bool getLocalTrcFileCloseFileAfterEachWrite() const;
public: // overridables
    virtual void setLocalTrcFileSubFileCountMax( int i_iCountMax );
    virtual int getLocalTrcFileSubFileCountMax() const;
    virtual void setLocalTrcFileSubFileLineCountMax( int i_iCountMax );
    virtual int getLocalTrcFileSubFileLineCountMax() const;
public: // overridables
    virtual void setCacheTrcDataIfNotConnected( bool i_bCacheData );
    virtual bool getCacheTrcDataIfNotConnected() const;
    virtual void setCacheTrcDataMaxArrLen( int i_iMaxArrLen );
    virtual int getCacheTrcDataMaxArrLen() const;
public: // overridables
    virtual void setTraceSettings( const STrcServerSettings& i_settings );
    virtual STrcServerSettings getTraceSettings() const;
public: // overridables
    virtual void clearLocalTrcFile();
protected: // overridables
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
    static QMutex                      s_mtx;            /*!< Mutex to protect the class and instance methods of the class for multithreaded access. */
    static QHash<QString, CTrcServer*> s_hshpInstances;  /*!< Hash with all created trace servers (key is name of instance). */
    static QHash<Qt::HANDLE, QString>  s_hshThreadNames; /*!< Hash with registered threads (key is thread id, value is name of thread). */
    static QHash<QString, Qt::HANDLE>  s_hshThreadIds;   /*!< Hash with registered threads (key name of thread, value is thread id). */
protected: // instance members
    CIdxTreeTrcAdminObjs* m_pTrcAdminObjIdxTree; /*<! Index tree containg a hierarchically order tree of the trace admin objects. */
    STrcServerSettings    m_trcSettings;         /*<! Currently used trace settings. */
    CTrcMthFile*          m_pTrcMthFile;         /*<! Reference to local trace method file. */
    int                   m_iTrcDetailLevel;     /*<! If the trace server itself got to be logged. */
    int                   m_iRefCount;           /*<! Reference counter for createInstance and releaseInstance. */

}; // class CTrcServer

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSSys_TrcServer_h
