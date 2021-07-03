/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <stdlib.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qmutex.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>

#if QT_VERSION < 0x050000
#if QT_VERSION >= 0x040501
#include <QtXml/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>
#endif
#else
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#endif

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

// Don't use mem leak dump for this module as this module is used to report those errors.
// The text stream, the file and the log entries remain allocated while the memory leaks are dumped.
//#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
struct SErrLogEntry : public QEvent
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an error log entry.

    The date time and the process time will be initialised with the current values.
    The result is initialised with "no error".
*/
SErrLogEntry::SErrLogEntry() :
//------------------------------------------------------------------------------
    m_iRowIdx(-1),
    m_dateTime(QDateTime::currentDateTime()),
    m_fSysTime_us(ZS::System::Time::getProcTimeInMicroSec()),
    m_errResultInfo(),
    m_strProposal(),
    m_iOccurrences()
{
} // default ctor

//------------------------------------------------------------------------------
/*! Copy constructor for an error log entry.

    @param i_other [in] Error log entry to be copied.
*/
SErrLogEntry::SErrLogEntry( const SErrLogEntry& i_other ) :
//------------------------------------------------------------------------------
    m_iRowIdx(i_other.m_iRowIdx),
    m_dateTime(i_other.m_dateTime),
    m_fSysTime_us(i_other.m_fSysTime_us),
    m_errResultInfo(i_other.m_errResultInfo),
    m_strProposal(i_other.m_strProposal),
    m_iOccurrences(i_other.m_iOccurrences)
{
} // copy ctor

//------------------------------------------------------------------------------
/*! Copy constructor for an error log gentry.

    @param i_dateTime [in] Date and time the error occurred.
    @param i_fSysTime_us [in] Time in micro seconds which have been passed since
                           program start when the error occured.
    @param i_errResultInfo [in] Referenz to structure contain the error code
                           together with severity, the error source and
                           additional info about the error.
    @param i_strProposal [in] Optional text with a suggestion what can be done
                           to remove the error cause.
    @param i_iOccurrences [in] Number of occurences for the error.
*/
SErrLogEntry::SErrLogEntry(
    const QDateTime&      i_dateTime,
    double                i_fSysTime_us,
    const SErrResultInfo& i_errResultInfo,
    const QString&        i_strProposal,
    int                   i_iOccurrences ) :
//------------------------------------------------------------------------------
    m_iRowIdx(-1),
    m_dateTime(i_dateTime),
    m_fSysTime_us(i_fSysTime_us),
    m_errResultInfo(i_errResultInfo),
    m_strProposal(i_strProposal),
    m_iOccurrences(i_iOccurrences)
{
} // ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! Assignment operator.

    @param i_other [in] Error log entry to be copied.
*/
SErrLogEntry& SErrLogEntry::operator = ( const SErrLogEntry& i_other )
//------------------------------------------------------------------------------
{
    m_iRowIdx       = i_other.m_iRowIdx;
    m_dateTime      = i_other.m_dateTime;
    m_fSysTime_us   = i_other.m_fSysTime_us;
    m_errResultInfo = i_other.m_errResultInfo;
    m_strProposal   = i_other.m_strProposal;
    m_iOccurrences  = i_other.m_iOccurrences;

    return *this;

} // operator =


/*******************************************************************************
class CErrLog : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QMutex CErrLog::s_mtx(QMutex::Recursive);
QHash<QString, CErrLog*> CErrLog::s_hshpInstances;
int CErrLog::s_iMsgHandlerInstallCount = 0;

#if QT_VERSION < 0x050000
QtMsgHandler s_qtMsgHandlerSaved = nullptr;     /*<! The currently installed Qt message handler is saved here and will be restored if RemoveQtMsgHandler is called. */
#else
QtMessageHandler s_qtMsgHandlerSaved = nullptr; /*<! The currently installed Qt message handler is saved here and will be restored if RemoveQtMsgHandler is called. */
#endif

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns a reference to an instance of the class.

    The CErrLog::GetInstance method checks whether an instance with the given
    object name is already existing. If not the method returns nullptr.

    @param i_strName [in] Object name of the error log instance. If this parameter
                          is ommitted "ZSErrLog" is used as default.
    @return Pointer to error log instance.
*/
CErrLog* CErrLog::GetInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return s_hshpInstances.value(i_strName, nullptr);
}

//------------------------------------------------------------------------------
/*! Creates an error log instance and returns a reference to the instance.

    The object names of the error log instances must be unique. If an instance
    with the given object name is already existing the method throws an exception.
    After creating the instance it may be accessed throughout the application using
    the GetInstance class method.

    If existing the current content of the error logs xml file is read and added
    to the error logs internal list of error entries.

    As default the maximum number of entries per severity is set as follows:

    - EResultSeveritySuccess  = 0;
    - EResultSeverityInfo     = 10;
    - EResultSeverityWarning  = 10;
    - EResultSeverityError    = 100;
    - EResultSeverityCritical = 10;

    @param i_bInstallQtMsgHandler [in] If true a Qt message handler will be installed
                          and the error log instance will receive all Qt errors and
                          will add them to the error log.
    @param i_strAbsFilePath [in] Absolute path including the file name and suffix.
                          If this parameter is ommitted (empty string) a default file
                          path will be used following a standard depending on the
                          underlying operating system.
                          Under Windows the directory "ProgramData/<CompanyName>/<AppName>"
                          is used and the file name is set to "<AppName>-ErrLog.xml".
    @param i_strName [in] Object name of the error log instance. If this parameter
                          is ommitted "ZSErrLog" is used as default.
    @return Pointer to error log instance.

    @note Throws an Exception
          - with Result = ObjAlreadyInList if an error log instance with the given
                          name is already existing.
*/
CErrLog* CErrLog::CreateInstance(
    bool           i_bInstallQtMsgHandler,
    const QString& i_strAbsFilePath,
    const QString& i_strName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( s_hshpInstances.contains(i_strName) )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "CErrLog::" + i_strName);
    }

    QString strAbsFilePath = i_strAbsFilePath;

    if( strAbsFilePath.isEmpty() )
    {
        QString strAppNameNormalized = QCoreApplication::applicationName();

        // The application name may contain characters which are invalid in file names:
        strAppNameNormalized.remove(":");
        strAppNameNormalized.remove(" ");
        strAppNameNormalized.remove("\\");
        strAppNameNormalized.remove("/");
        strAppNameNormalized.remove("<");
        strAppNameNormalized.remove(">");

        // Range of IniFileScope: ["AppDir", "User", "System"]
        #ifdef __linux__
        // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
        // where the application has not write access rights. Stupid ...
        QString strIniFileScope = "User";
        #else
        QString strIniFileScope = "System"; // Default
        #endif

        QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

        QString strErrLogFileBaseName = strAppNameNormalized + "-ErrLog";
        QString strErrLogFileSuffix = "xml";

        strAbsFilePath = strAppLogDir + "/" + strErrLogFileBaseName + "." + strErrLogFileSuffix;

    } // if( strAbsFilePath.isEmpty() )

    CErrLog* pErrLog = new CErrLog(i_strName, strAbsFilePath, i_bInstallQtMsgHandler);

    s_hshpInstances[i_strName] = pErrLog;

    return pErrLog;

} // CreateInstance

//------------------------------------------------------------------------------
/*! Destroys the error log instance with the given name.

    The one who created the error log instance with CErrLog::GetInstance method
    should also delete the error log instance if no longer needed.
    Usually the error log instance is created once at the beginning of the
    applications startup and destroyed at the end of the shutdown.
    When the error log file is deleted the current content will be saved in
    the specified xml file.

    @param i_strName [in] Object name of the error log instance. If this parameter
                          is ommitted "ZSErrLog" is used as default.

    @note Throws an Exception
          - with Result = ObjNotInList if an error log instance with the given
                          name is not existing.
*/
void CErrLog::ReleaseInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CErrLog* pErrLog = s_hshpInstances.value(i_strName, nullptr);

    if( pErrLog == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CErrLog::" + i_strName);
    }

    s_hshpInstances.remove(i_strName);

    delete pErrLog;
    pErrLog = nullptr;

} // ReleaseInstance

//------------------------------------------------------------------------------
/*! Destroys the error log instance.

    The one who created the error log instance with CErrLog::GetInstance method
    should also delete the error log instance if no longer needed.
    Usually the error log instance is created once at the beginning of the
    applications startup and destroyed at the end of the shutdown.
    When the error log file is deleted the current content will be saved in
    the specified xml file.

    @param i_pErrLog [in] Reference to the error log instance to be deleted.

    @note Throws an Exception
          - with Result = ObjNotInList if the error log instance was not found
                          by its name.
*/
void CErrLog::ReleaseInstance( CErrLog* i_pErrLog )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    QString strName = i_pErrLog->objectName();

    if( !s_hshpInstances.contains(strName) )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CErrLog::" + strName);
    }

    s_hshpInstances.remove(strName);

    delete i_pErrLog;
    i_pErrLog = nullptr;

} // ReleaseInstance

//------------------------------------------------------------------------------
/*! Destroys all error log instances.

    This method is only provided for convenience but should never been used.
    Usually the one who created an error log instance with CErrLog::GetInstance
    should also delete the error log instance if no longer needed.
*/
void CErrLog::DestroyAllInstances()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CErrLog* pErrLog;
    QString  strName;

    QHash<QString, CErrLog*>::iterator itErrLog;

    for( itErrLog = s_hshpInstances.begin(); itErrLog != s_hshpInstances.end(); itErrLog++ )
    {
        strName = itErrLog.key();
        pErrLog = itErrLog.value();

        s_hshpInstances[strName] = nullptr;

        delete pErrLog;
        pErrLog = nullptr;
    }

    s_hshpInstances.clear();

} // DestroyAllInstances

/*==============================================================================
private: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Installs the Qt message handler of the errog log instance.

    The Qt library (debug mode) contains hundreds of warning messages that are
    printed when internal errors (usually invalid function arguments) occur.
    Those errors may be redericted to the error log instance by installing a
    message handler.

    @note See qInstallMessageHander for further details.
*/
void CErrLog::InstallQtMsgHandler()
//------------------------------------------------------------------------------
{
    if( s_iMsgHandlerInstallCount == 0 )
    {
        #if QT_VERSION >= 0x050100
        s_qtMsgHandlerSaved = qInstallMessageHandler(QtMsgHandler);
        #else
        s_qtMsgHandlerSaved = qInstallMsgHandler(QtMsgHandler);
        #endif
    }
    s_iMsgHandlerInstallCount++;

} // InstallQtMsgHandler

//------------------------------------------------------------------------------
/*! Removes the Qt message handler of the errog log instance.

    @note See qInstallMessageHander for further details.
*/
void CErrLog::RemoveQtMsgHandler()
//------------------------------------------------------------------------------
{
    if( s_iMsgHandlerInstallCount == 1 )
    {
        if( s_qtMsgHandlerSaved != nullptr )
        {
            #if QT_VERSION < 0x050000
            qInstallMsgHandler(s_qtMsgHandlerSaved);
            #else
            qInstallMessageHandler(s_qtMsgHandlerSaved);
            #endif
            s_qtMsgHandlerSaved = nullptr;
        }
        else
        {
            #if QT_VERSION < 0x050000
            qInstallMsgHandler(nullptr);
            #else
            qInstallMessageHandler(nullptr);
            #endif
        }
    }
    s_iMsgHandlerInstallCount--;

} // RemoveQtMsgHandler

//------------------------------------------------------------------------------
/*! This is the error logs own Qt message handler.

    Warnings, critical and fatal error messages received from the Qt library
    will be saved in the error log instance.

    @param i_msgType [in] This enum describes the messages that can be sent to this
                        message handler (QtMessageHandler) identifying and associating
                        the various message types with the appropriate actions.
    @param i_context [in] The QMessageLogContext class provides additional information
                        about a log message. The error log instance does not use it
                        but just forwards the context to the previously installed
                        message handler.
    @param i_strMsg [in] The content of this string will used as the additional error
                        info for the error result info added to the error log instance.

    @note See qInstallMessageHander for further details.
*/
#if QT_VERSION < 0x050000
void CErrLog::QtMsgHandler( QtMsgType i_msgType, const char* i_strMsg )
#else
void CErrLog::QtMsgHandler( QtMsgType i_msgType, const QMessageLogContext& i_context, const QString& i_strMsg )
#endif
//------------------------------------------------------------------------------
{
    CErrLog* pModelErrLog = CErrLog::GetInstance();

    if( pModelErrLog != nullptr )
    {
        switch( i_msgType )
        {
            case QtDebugMsg:
            {
                break;
            }
            case QtWarningMsg:
            {
                QString strAddInfo = i_strMsg;
                if( strAddInfo.contains("QFileSystemWatcher",Qt::CaseInsensitive) )
                {
                    strAddInfo = "Set breakpoint here if desired";
                    strAddInfo = i_strMsg;
                }
                else if( strAddInfo.startsWith("QObject::disconnect",Qt::CaseInsensitive) )
                {
                    strAddInfo = "Set breakpoint here if desired";
                    strAddInfo = i_strMsg;
                }
                else
                {
                    SErrResultInfo errResultInfo(
                        /* errSource  */ "Qt", "Framework", "", "",
                        /* result     */ EResultUndefined,
                        /* severity   */ EResultSeverityWarning,
                        /* strAddInfo */ i_strMsg );
                    pModelErrLog->addEntry(errResultInfo);
                }
                break;
            }
            case QtCriticalMsg:
            {
                SErrResultInfo errResultInfo(
                    /* errSource  */ "Qt", "Framework", "", "",
                    /* result     */ EResultUndefined,
                    /* severity   */ EResultSeverityError,
                    /* strAddInfo */ i_strMsg );
                pModelErrLog->addEntry(errResultInfo);
                break;
            }
            case QtFatalMsg:
            {
                SErrResultInfo errResultInfo(
                    /* errSource  */ "Qt", "Framework", "", "", __FILE__, __LINE__,
                    /* result     */ EResultUndefined,
                    /* severity   */ EResultSeverityCritical,
                    /* strAddInfo */ i_strMsg );
                throw ZS::System::CException(errResultInfo);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if( s_qtMsgHandlerSaved != nullptr )
    {
        #if QT_VERSION < 0x050000
        s_qtMsgHandlerSaved(i_msgType,i_strMsg);
        #else
        s_qtMsgHandlerSaved(i_msgType,i_context,i_strMsg);
        #endif
    }

} // QtMsgHandler

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an error log instance.

    Error log instances may only be constructed using the static CreateInstance method.

    If existing the ctor reads the current content of the error logs xml file.

    @param i_strName [in] Object name of the error log instance.
    @param i_strAbsFilePath [in] Absolute path including the file name and suffix.
    @param i_bInstallQtMsgHandler [in] If true a Qt message handler will be installed
                          and the error log instance will receive all Qt errors and
                          will add them to the error log.
*/
CErrLog::CErrLog(
    const QString& i_strName,
    const QString& i_strAbsFilePath,
    bool           i_bInstallQtMsgHandler) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMtx(nullptr),
    m_strAbsFilePath(i_strAbsFilePath),
    m_pFile(nullptr),
    m_bRecallingModel(false),
    m_bClearingModel(false),
    m_iAddEntryRecursionCounter(0),
    m_ariEntriesCountMax(EResultSeverityCount),
    m_ararpEntries(EResultSeverityCount),
    m_bQtMsgHandlerInstalledByCtor(false)
{
    setObjectName(i_strName);

    m_ariEntriesCountMax[EResultSeveritySuccess]  = 0;
    m_ariEntriesCountMax[EResultSeverityInfo]     = 10;
    m_ariEntriesCountMax[EResultSeverityWarning]  = 10;
    m_ariEntriesCountMax[EResultSeverityError]    = 100;
    m_ariEntriesCountMax[EResultSeverityCritical] = 10;

    if( i_bInstallQtMsgHandler )
    {
        InstallQtMsgHandler();
        m_bQtMsgHandlerInstalledByCtor = true;
    }

    m_pMtx = new QMutex(QMutex::Recursive);

    QMutexLocker mtxLockerInst(m_pMtx);

    // Create and/or recall error log file
    //------------------------------------

    QFileInfo fileInfoLogFile(m_strAbsFilePath);

    QDir dirLogFile = fileInfoLogFile.absoluteDir();

    if( !dirLogFile.exists() )
    {
        dirLogFile.mkpath( dirLogFile.absolutePath() );
    }

    m_pFile = new QFile(m_strAbsFilePath);

    recall();

} // ctor

//------------------------------------------------------------------------------
/*! Destroys the error log instance saving the current error log entries to
    the xml file and closing the file.

    Error log instances may only be destroyed using the static DestroyInstance method.
*/
CErrLog::~CErrLog()
//------------------------------------------------------------------------------
{
    if( m_pMtx != nullptr )
    {
        m_pMtx->lock();
    }

    if( m_bQtMsgHandlerInstalledByCtor )
    {
        RemoveQtMsgHandler();
    }

    try
    {
        save();
    }
    catch(...)
    {
    }

    int iRowIdx;

    for( int iSeverity = 0; iSeverity < EResultSeverityCount; iSeverity++ )
    {
        if( m_ararpEntries[iSeverity].count() > 0 )
        {
            for( iRowIdx = m_ararpEntries[iSeverity].count()-1; iRowIdx >= 0; iRowIdx-- )
            {
                delete m_ararpEntries[iSeverity][iRowIdx];
                m_ararpEntries[iSeverity][iRowIdx] = nullptr;
            }
        }
    }

    try
    {
        delete m_pFile;
    }
    catch(...)
    {
    }

    if( m_pMtx != nullptr )
    {
        m_pMtx->unlock();
    }

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

    m_pMtx = nullptr;
    //m_strAbsFilePath;
    m_pFile = nullptr;
    m_bRecallingModel = false;
    m_bClearingModel = false;
    m_iAddEntryRecursionCounter = 0;
    //m_ariEntriesCountMax;
    //m_ararpEntries;
    m_bQtMsgHandlerInstalledByCtor = false;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the maximum number of error log entries for the given severity.

    If the maximum number is reached the latest entry will be removed and replaced
    with a ListIsFull error and the error source of the error log instance itself.

    @param i_iCount [in] Maximum number of error log entries.
    @param i_severity [in] Error severity for which the maximum number should be set.
*/
void CErrLog::setEntriesCountMax( int i_iCount, EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    int iSeverityMin = i_severity;
    int iSeverityMax = i_severity;

    if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )
    {
        iSeverityMin = 0;
        iSeverityMax = EResultSeverityCount-1;
    }

    for( int iSeverity = iSeverityMin; iSeverity <= iSeverityMax; iSeverity++ )
    {
        m_ariEntriesCountMax[iSeverity] = i_iCount;
    }

} // setEntriesCountMax

//------------------------------------------------------------------------------
/*! Returns the maximum number of error log entries for the given severity.

    @param i_severity [in] Error severity for which the maximum number should be set.

    @return Maximum number of error log entries for the given severity.
*/
int CErrLog::getEntriesCountMax( EResultSeverity i_severity ) const
//------------------------------------------------------------------------------
{
    int iCountMax = 0;

    int iSeverityMin = i_severity;
    int iSeverityMax = i_severity;

    if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )
    {
        iSeverityMin = 0;
        iSeverityMax = EResultSeverityCount-1;
    }

    for( int iSeverity = iSeverityMin; iSeverity <= iSeverityMax; iSeverity++ )
    {
        iCountMax += m_ariEntriesCountMax[iSeverity];
    }

    return iCountMax;

} // getEntriesCountMax

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the absolute path of the error log file including the file name and suffix.

    @return Absolute path of the log file including the file name and suffix.
*/
QString CErrLog::getAbsFilePath() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLockerInst(m_pMtx);

    return m_strAbsFilePath;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Adds an entry to the error log.

    The error logs date time and system time will be automatically set to
    the current date time and system time.

    @param i_errResultInfo [in] Error to be added to the error log.
    @param i_strProposal [in] Suggestion on how to remove the error cause.

    @note If the maximum number of error log entries has already been reached when
          invoking this method the entry will not be added. Instead the last error
          entry will be removed and replaced with a ListIsFull error and the error
          source of the error log instance itself.
*/
void CErrLog::addEntry( SErrResultInfo& i_errResultInfo, const QString& i_strProposal )
//------------------------------------------------------------------------------
{
    addEntry(QDateTime::currentDateTime(), i_errResultInfo, i_strProposal);
}

//------------------------------------------------------------------------------
/*! Adds an entry to the error log.

    @param i_dateTime [in] Date time when the error occurred.
    @param i_errResultInfo [in] Error to be added to the error log.
    @param i_strProposal [in] Suggestion on how to remove the error cause.

    @note If the maximum number of error log entries has already been reached when
          invoking this method the entry will not be added. Instead the last error
          entry will be removed and replaced with a ListIsFull error and the error
          source of the error log instance itself.
*/
void CErrLog::addEntry(
    const QDateTime& i_dateTime,
    SErrResultInfo&  i_errResultInfo,
    const QString&   i_strProposal )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLockerInst(m_pMtx);

    EResultSeverity severity = i_errResultInfo.getSeverity();

    if( m_ararpEntries[severity].size() < m_ariEntriesCountMax[severity] )
    {
        addEntry_(i_dateTime, i_errResultInfo, i_strProposal);
        i_errResultInfo.setAddedToErrLogModel(true);
    }
    else
    {
        removeEntry(m_ariEntriesCountMax[severity]-1, severity);

        addEntry_(
            /* dateTime    */ QDateTime::currentDateTime(),
            /* errResultInfo  */ SErrResultInfo(
                /* errSource  */ nameSpace(), className(), objectName(), "addEntry",
                /* result     */ EResultListIsFull,
                /* severity   */ EResultSeverityError,
                /* strAddInfo */ "Maximum number of error log entries for severity " + resultSeverity2Str(severity) + " reached." ),
            /* strProposal */ "" );
    }

} // addEntry

//------------------------------------------------------------------------------
/*! Searches the error log entry for the given error result info.

    The search pattern includes the result code, the result severity,
    the error source and the additional error info.

    @param i_errResultInfo [in] Error result info.

    @return Pointer to error log entry or nullptr, if the corresponding entry
            could not be found.

    @note If you use this method to loop through the error log entries don't forget
          to lock the instance before and unlocking it again afterwards.
*/
SErrLogEntry* CErrLog::findEntry( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLockerInst(m_pMtx);

    EResultSeverity severity = i_errResultInfo.getSeverity();

    SErrLogEntry* pEntry = nullptr;
    SErrLogEntry* pEntryTmp;
    int           iRowIdx;

    for( iRowIdx = 0; iRowIdx < m_ararpEntries[severity].count(); iRowIdx++ )
    {
        pEntryTmp = m_ararpEntries[severity][iRowIdx];

        if( pEntryTmp != nullptr )
        {
            if( pEntryTmp->m_errResultInfo == i_errResultInfo )
            {
                pEntry = pEntryTmp;
                break;
            }
        }
    }
    return pEntry;

} // findEntry

//------------------------------------------------------------------------------
/*! Searches the error log entry for the given error result info and modifies
    the date time or the proposal on how to fix the error.

    The search pattern includes the result code, the result severity,
    the error source and the additional error info.

    @param i_errResultInfo [in] Error result info.
    @param i_bModifyDateTime [in] Flag to indicate whether the date time should be modified.
    @param i_dateTime [in] Date time to be set for the error log entry - if the modify flag is set.
    @param i_bModifyProposal [in] Flag to indicate whether the proposol to fix the error should be modified.
    @param i_strProposal [in] Proposal to be set for the error log entry - if the modify flag is set.
*/
void CErrLog::changeEntry(
    const SErrResultInfo& i_errResultInfo,
    bool                  i_bModifyDateTime,
    const QDateTime&      i_dateTime,
    bool                  i_bModifyProposal,
    const QString&        i_strProposal )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLockerInst(m_pMtx);

    SErrLogEntry* pEntry = findEntry(i_errResultInfo);

    if( pEntry != nullptr )
    {
        changeEntry_(
            /* pEntry             */ pEntry,
            /* bModifyOccurrecnes */ false,
            /* iOccurrences       */ 0,
            /* bModifyDateTime    */ i_bModifyDateTime,
            /* dateTime           */ i_dateTime,
            /* bModifyProposal    */ i_bModifyProposal,
            /* strProposal        */ i_strProposal );
    }

} // changeEntry

//------------------------------------------------------------------------------
/*! Searches the error log entry for the given error result and removes it.

    The search pattern includes the result code, the result severity,
    the error source and the additional error info.

    @param i_errResultInfo [in] Error result info to be removed.
*/
void CErrLog::removeEntry( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLockerInst(m_pMtx);

    SErrLogEntry* pEntry = findEntry(i_errResultInfo);

    if( pEntry != nullptr )
    {
        removeEntry_(pEntry->m_iRowIdx, i_errResultInfo.getSeverity());
    }

} // removeEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Removes all error log entries for the given severity.

    @param i_severity [in] Severity for which the error log entries should be removed.
                           By passing Undefined or Count for the severity all entries
                           will be removed.
*/
void CErrLog::clear( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    // Entries may be added, changed and removed from within different thread
    // contexts. Accessing the list must therefore be serialized using a mutex ..
    QMutexLocker mtxLockerInst(m_pMtx);

    m_bClearingModel = true;

    int iSeverityMin = i_severity;
    int iSeverityMax = i_severity;

    if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )
    {
        iSeverityMin = 0;
        iSeverityMax = EResultSeverityCount-1;
    }

    int iSeverity;
    int iRowIdx;

    for( iSeverity = iSeverityMin; iSeverity <= iSeverityMax; iSeverity++ )
    {
        EResultSeverity severity = static_cast<EResultSeverity>(iSeverity);

        if( m_ararpEntries[severity].count() > 0 )
        {
            for( iRowIdx = m_ararpEntries[severity].count()-1; iRowIdx >= 0; iRowIdx-- )
            {
                removeEntry(iRowIdx, severity);
            }
        }
    }

    m_bClearingModel = false;

    save();

} // clear

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Locks the error log instance for the current thread.

    The CErrLog class offers methods to loop through the entries. To avoid that
    error log entries are added, removed or changed while looping through the list
    of entries and accessing entries which are no longer valid the error log instance
    may be temporarily locked. But don't forget to unlock the error log instance
    if no longer needed. Otherwise the appliation may be deadlocked.
*/
void CErrLog::lock()
//------------------------------------------------------------------------------
{
    if( m_pMtx != nullptr )
    {
        m_pMtx->lock();
    }

} // lock

//------------------------------------------------------------------------------
/*! Unlocks the error log instance for the current thread.

    @see CErrLog::lock
*/
void CErrLog::unlock()
//------------------------------------------------------------------------------
{
    if( m_pMtx != nullptr )
    {
        m_pMtx->unlock();
    }
} // unlock

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the number of error log entries for the given severity.

    @param i_severity [in] Severity for which the number of log entries should be returned.
                           By passing Undefined or Count for the severity the overall
                           number of entries will be returned.

    @return Number of error log entries for the desired severtiy.

    @note If you use this method to loop through the error log entries don't forget
          to lock the instance before and unlocking it again afterwards.
*/
int CErrLog::getEntryCount( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    int iCount = 0;

    int iSeverityMin = i_severity;
    int iSeverityMax = i_severity;

    if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )
    {
        iSeverityMin = 0;
        iSeverityMax = EResultSeverityCount-1;
    }

    int iSeverity;

    for( iSeverity = iSeverityMin; iSeverity <= iSeverityMax; iSeverity++ )
    {
        iCount += m_ararpEntries[iSeverity].count();
    }

    return iCount;

} // getEntryCount

//------------------------------------------------------------------------------
/*! Returns the entry at the given index for the given severity.

    @param i_iRowIdx [in] Row index of the error log entry. The row indices are
                          counted for each severity separately.
    @param i_severity [in] Severity for which the error log entry should be returned.
                           By passing Undefined or Count the row numbers are counted
                           for all severities.

    @return Error log entry for the given row index and severity.

    @note If the row index was out of range this method returns nullptr.
    @note If you use this method to loop through the error log entries don't forget
          to lock the instance before and unlocking it again afterwards.
*/
SErrLogEntry* CErrLog::getEntry( int i_iRowIdx, EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    SErrLogEntry* pEntry = nullptr;

    if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )
    {
        int iCount = 0;

        // In this case counting rows starts at highest severity (Critical).
        for( int iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
        {
            if( i_iRowIdx < (iCount + m_ararpEntries[iSeverity].count()) )
            {
                pEntry = m_ararpEntries[iSeverity][i_iRowIdx-iCount];
                break;
            }
            iCount += m_ararpEntries[iSeverity].count();
        }
    }
    else // if( i_severity >= 0 && i_severity < EResultSeverityCount )
    {
        if( i_iRowIdx >= 0 && i_iRowIdx < m_ararpEntries[i_severity].count() )
        {
            pEntry = m_ararpEntries[i_severity][i_iRowIdx];
        }
    }

    return pEntry;

} // getEntry

//------------------------------------------------------------------------------
/*! Removes the entry at the given index for the given severity.

    @param i_iRowIdx [in] Row index of the error log entry. The row indices are
                          counted for each severity separately.
    @param i_severity [in] Severity for which the error log entry should be removed.
                           By passing Undefined or Count the row numbers are counted
                           for all severities.

    @note If the row index is out of range this method does nothing.
    @note If you use this method to loop through the error log entries don't forget
          to lock the instance before and unlocking it again afterwards.
*/
void CErrLog::removeEntry( int i_iRowIdx, EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    removeEntry_(i_iRowIdx, i_severity);
}

//------------------------------------------------------------------------------
/*! Returns the the entry at the given index for the given severity and removes
    the entry from the error log.

    @param i_iRowIdx [in] Row index of the error log entry. The row indices are
                          counted for each severity separately.
    @param i_severity [in] Severity for which the error log entry should be returned
                           and removed. By passing Undefined or Count the row numbers
                           are counted for all severities.

    @note If the row index is out of range this method does nothing.
    @note If you use this method to loop through the error log entries don't forget
          to lock the instance before and unlocking it again afterwards.
*/
SErrLogEntry CErrLog::takeEntry( int i_iRowIdx, EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    SErrLogEntry entry;

    if( i_iRowIdx >= 0 && i_iRowIdx < getEntryCount(i_severity) )
    {
        SErrLogEntry* pEntry = getEntry(i_iRowIdx, i_severity);

        if( pEntry != nullptr )
        {
            entry = *pEntry;

            removeEntry_(i_iRowIdx, i_severity);

        } // if( pEntry != nullptr )
    } // if( i_iRowIdx >= 0 && i_iRowIdx < getEntryCount(i_severity) )

    return entry;

} // takeEntry

//------------------------------------------------------------------------------
/*! Returns the first entry the given severity and removes the entry from the error log.

    @param i_severity [in] Severity for which the error log entry should be returned
                           and removed. By passing Undefined or Count the row numbers
                           are counted for all severities.

    @note If there is no entry in the error log this method does nothing.
    @note If you use this method to loop through the error log entries don't forget
          to lock the instance before and unlocking it again afterwards.
*/
SErrLogEntry CErrLog::takeFirstEntry( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    SErrLogEntry entry;

    if( getEntryCount(i_severity) > 0 )
    {
        SErrLogEntry* pEntry = getEntry(0, i_severity);

        if( pEntry != nullptr )
        {
            entry = *pEntry;

            removeEntry_(0, i_severity);

        } // if( pEntry != nullptr )
    } // if( getEntryCount(i_severity) > 0 )

    return entry;

} // takeFirstEntry

//------------------------------------------------------------------------------
/*! Returns the last entry the given severity and removes the entry from the error log.

    @param i_severity [in] Severity for which the error log entry should be returned
                           and removed. By passing Undefined or Count the row numbers
                           are counted for all severities.

    @note If there is no entry in the error log this method does nothing.
    @note If you use this method to loop through the error log entries don't forget
          to lock the instance before and unlocking it again afterwards.
*/
SErrLogEntry CErrLog::takeLastEntry( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    SErrLogEntry entry;

    int iCount = getEntryCount(i_severity);

    if( iCount > 0 )
    {
        SErrLogEntry* pEntry = getEntry(iCount-1);

        if( pEntry != nullptr )
        {
            entry = *pEntry;

            removeEntry_(iCount-1, i_severity);

        } // if( pEntry != nullptr )
    } // if( getEntryCount(i_severity) > 0 )

    return entry;

} // takeLastEntry

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Reads the current content from the xml file and fills the error log with
    those entries.
*/
void CErrLog::recall()
//------------------------------------------------------------------------------
{
    /* This protected method is called from public methods and the mutex is already locked
    QMutexLocker mtxLockerInst(m_pMtx); */

    m_bRecallingModel = true;

    if( !m_strAbsFilePath.isEmpty() && m_pFile != nullptr )
    {
        m_pFile->open(QIODevice::ReadOnly);

        if( m_pFile->isOpen() )
        {
            QXmlStreamReader            xmlStreamReader(m_pFile);
            QXmlStreamReader::TokenType xmlStreamTokenType;

            QString         strElemName;
            QString         strElemText;
            QDate           date;
            QTime           time;
            QDateTime       dateTime;
            double          fSysTime_us = 0.0;
            int             iOccurrences = 0;
            EResultSeverity severity = EResultSeverityUndefined;
            EResult         result = EResultSuccess;
            QString         strResult;
            QString         strNameSpace;
            QString         strClassName;
            QString         strObjName;
            QString         strMth;
            QString         strFilePath;
            int             iLineNr = 0;
            QString         strAddInfo;
            QString         strProposal;
            int             iVal;
            bool            bConverted;

            xmlStreamTokenType = xmlStreamReader.readNext();

            if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
            {
                xmlStreamReader.raiseError("Invalid XML document");
            }

            while( !xmlStreamReader.hasError() && !xmlStreamReader.atEnd() )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();
                strElemName = xmlStreamReader.name().toString();

                if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
                {
                    //--------------------------------
                    if( strElemName == "Entry" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            date = QDate::currentDate();
                            time = QTime::currentTime();
                            fSysTime_us = 0.0;
                            iOccurrences = 0;
                            severity = EResultSeveritySuccess;
                            result = EResultSuccess;
                            strResult = "";
                            strNameSpace = "";
                            strClassName = "";
                            strObjName = "";
                            strMth = "";
                            strFilePath = "";
                            iLineNr = -1;
                            strAddInfo = "";
                            strProposal = "";
                        }
                        else if( xmlStreamReader.isEndElement() )
                        {
                            dateTime.setDate(date);
                            dateTime.setTime(time);

                            SErrLogEntry* pEntry = addEntry_(
                                /* dateTime     */ dateTime,
                                /* errResultInfo  */ SErrResultInfo(
                                    /* errSource      */ strNameSpace, strClassName, strObjName, strMth, strFilePath, iLineNr,
                                    /* result         */ result,
                                    /* severity       */ severity,
                                    /* strAddInfo     */ strAddInfo ),
                                /* strProposal  */ strProposal );
                            pEntry->m_fSysTime_us = fSysTime_us;

                            changeEntry_(
                                /* pEntry             */ pEntry,
                                /* bModifyOccurrences */ true,
                                /* iOccurrences       */ iOccurrences,
                                /* bModifyDateTime    */ false,
                                /* dateTime           */ QDateTime(),
                                /* bModifyProposal    */ false,
                                /* strProposal        */ "" );

                        } // else if( xmlStreamReader.isEndElement() )

                    } // if( strElemName == "Entry" )

                    //--------------------------------
                    else if( strElemName == "Date" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strElemText = xmlStreamReader.readElementText();
                            date = QDate::fromString(strElemText,"yyyy-MM-dd");
                        }
                    } // if( strElemName == "Date" )

                    //--------------------------------
                    else if( strElemName == "Time" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strElemText = xmlStreamReader.readElementText();
                            time = QTime::fromString(strElemText,"hh:mm:ss");
                        }
                    } // if( strElemName == "Time" )

                    //--------------------------------
                    else if( strElemName == "SysTime" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strElemText = xmlStreamReader.readElementText();
                            fSysTime_us = strElemText.toDouble();
                        }
                    } // if( strElemName == "SysTime" )

                    //--------------------------------
                    else if( strElemName == "Occurrences" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strElemText = xmlStreamReader.readElementText();
                            iOccurrences = strElemText.toInt();
                        }
                    } // if( strElemName == "Occurrences" )

                    //--------------------------------
                    else if( strElemName == "Severity" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strElemText = xmlStreamReader.readElementText();
                            severity = str2ResultSeverity(strElemText);
                        }
                    } // if( strElemName == "Severity" )

                    //--------------------------------
                    else if( strElemName == "Result" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strElemText = xmlStreamReader.readElementText();
                            result = static_cast<EResult>(strElemText.toInt());
                        }
                    } // if( strElemName == "Result" )

                    //--------------------------------
                    else if( strElemName == "ResultStr" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strResult = xmlStreamReader.readElementText();
                        }
                    } // if( strElemName == "ResultStr" )

                    //--------------------------------
                    else if( strElemName == "NameSpace" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strNameSpace = xmlStreamReader.readElementText();
                        }
                    } // if( strElemName == "NameSpace" )

                    //--------------------------------
                    else if( strElemName == "ClassName" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strClassName = xmlStreamReader.readElementText();
                        }
                    } // if( strElemName == "ClassName" )

                    //--------------------------------
                    else if( strElemName == "ObjectName" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strObjName = xmlStreamReader.readElementText();
                        }
                    } // if( strElemName == "ObjectName" )

                    //--------------------------------
                    else if( strElemName == "MethodName" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strMth = xmlStreamReader.readElementText();
                        }
                    } // if( strElemName == "MethodName" )

                    //--------------------------------
                    else if( strElemName == "FilePath" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strFilePath = xmlStreamReader.readElementText();
                        }
                    } // if( strElemName == "FilePath" )

                    //--------------------------------
                    else if( strElemName == "LineNr" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strElemText = xmlStreamReader.readElementText();
                            iVal = static_cast<EResult>(strElemText.toInt(&bConverted));
                            if( bConverted ) iLineNr = iVal;
                        }
                    } // if( strElemName == "LineNr" )

                    //--------------------------------
                    else if( strElemName == "AddInfo" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strAddInfo = xmlStreamReader.readElementText();
                        }
                    } // if( strElemName == "AddInfo" )

                    //--------------------------------
                    else if( strElemName == "Proposal" )
                    //--------------------------------
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strProposal = xmlStreamReader.readElementText();
                        }
                    } // if( strElemName == "Proposal" )

                } // if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )

            } // while( !xmlStreamReader.hasError() && !xmlStreamReader.atEnd() )

            if( xmlStreamReader.hasError() )
            {
                // For debugging purposes:
                strAddInfo  = "Line:" + QString::number(xmlStreamReader.lineNumber());
                strAddInfo += ", Column:" + QString::number(xmlStreamReader.columnNumber());
                strAddInfo += ", " + xmlStreamReader.errorString();
            }

            m_pFile->close();

        } // if( m_pFile->isOpen() )

    } // if( !m_strAbsFilePath.isEmpty() && m_pFile != nullptr )

    m_bRecallingModel = false;

} // recall

//------------------------------------------------------------------------------
/*! Saves the current content of the error log in the xml file.
*/
void CErrLog::save()
//------------------------------------------------------------------------------
{
    /* This protected method is called from public methods and the mutex is already locked
    QMutexLocker mtxLockerInst(m_pMtx); */

    if( m_bRecallingModel ||  m_bClearingModel )
    {
        return;
    }

    if( !m_strAbsFilePath.isEmpty() && m_pFile != nullptr )
    {
        int           iSeverity;
        int           iRowIdx;
        SErrLogEntry* pEntry;

        if( m_pFile->isOpen() )
        {
            m_pFile->close();
        }

        if( m_pFile->open(QIODevice::WriteOnly) )
        {
            QXmlStreamWriter xmlStreamWriter(m_pFile);

            xmlStreamWriter.setAutoFormatting(true);

            xmlStreamWriter.writeStartDocument();
            xmlStreamWriter.writeStartElement("Errors");

            for( iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
            {
                for( iRowIdx = 0; iRowIdx < m_ararpEntries[iSeverity].count(); iRowIdx++ )
                {
                    pEntry = m_ararpEntries[iSeverity][iRowIdx];

                    saveEntry(pEntry,xmlStreamWriter);
                }
            }

            xmlStreamWriter.writeEndElement();
            xmlStreamWriter.writeEndDocument();

            if( m_pFile->isOpen() )
            {
                m_pFile->close();
            }
        } // if( m_pFile->open(QIODevice::WriteOnly) )
    } // if( !m_strAbsFilePath.isEmpty() && m_pFile != nullptr )

} // save

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Saves the entry in the the xml file.

    Internal method which will be recursively called by the save method.

    @param i_pEntry [in] Pointer to entry to be saved.
    @param i_xmlStreamWriter [in] Xml stream writer to be used. This writer
                                  has been created by the save method.
*/
void CErrLog::saveEntry( SErrLogEntry* i_pEntry, QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    /* This protected method is called from public methods and the mutex is already locked
    QMutexLocker mtxLockerInst(m_pMtx); */

    if( m_bRecallingModel || m_bClearingModel )
    {
        return;
    }

    if( i_pEntry != nullptr )
    {
        SErrSource errSource  = i_pEntry->m_errResultInfo.getErrSource();
        QString    strDate    = i_pEntry->m_dateTime.toString("yyyy-MM-dd");
        QString    strTime    = i_pEntry->m_dateTime.toString("hh:mm:ss");
        QString    strSysTime = QString::number(i_pEntry->m_fSysTime_us,'f',6);
        QString    strTmp;

        i_xmlStreamWriter.writeStartElement("Entry");

        i_xmlStreamWriter.writeTextElement( "Date", strDate );
        i_xmlStreamWriter.writeTextElement( "Time", strTime );
        i_xmlStreamWriter.writeTextElement( "SysTime", strSysTime );
        i_xmlStreamWriter.writeTextElement( "Occurrences", QString::number(i_pEntry->m_iOccurrences) );
        i_xmlStreamWriter.writeTextElement( "Severity", resultSeverity2Str(i_pEntry->m_errResultInfo.getSeverity()) );
        i_xmlStreamWriter.writeTextElement( "Result", QString::number(i_pEntry->m_errResultInfo.getResult()) );

        if( !i_pEntry->m_errResultInfo.getResultStr().isEmpty() )
        {
            i_xmlStreamWriter.writeTextElement( "ResultStr", i_pEntry->m_errResultInfo.getResultStr() );
        }

        if( !errSource.getNameSpace().isEmpty() )
        {
            strTmp = errSource.getNameSpace();
            //strTmp.replace("<","&lt;");
            //strTmp.replace(">","&gt;");
            i_xmlStreamWriter.writeTextElement( "NameSpace", strTmp );
        }
        if( !errSource.getClassName().isEmpty() )
        {
            strTmp = errSource.getClassName();
            //strTmp.replace("<","&lt;");
            //strTmp.replace(">","&gt;");
            i_xmlStreamWriter.writeTextElement( "ClassName", strTmp );
        }
        if( !errSource.getObjectName().isEmpty() )
        {
            strTmp = errSource.getObjectName();
            //strTmp.replace("<","&lt;");
            //strTmp.replace(">","&gt;");
            i_xmlStreamWriter.writeTextElement( "ObjectName", strTmp );
        }
        if( !errSource.getMethodName().isEmpty() )
        {
            strTmp = errSource.getMethodName();
            //strTmp.replace("<","&lt;");
            //strTmp.replace(">","&gt;");
            i_xmlStreamWriter.writeTextElement( "MethodName", strTmp );
        }
        if( !errSource.getFilePath().isEmpty() )
        {
            strTmp = errSource.getFilePath();
            //strTmp.replace("<","&lt;");
            //strTmp.replace(">","&gt;");
            i_xmlStreamWriter.writeTextElement( "FilePath", strTmp );
        }
        if( errSource.getLineNr() >= 0 )
        {
            strTmp = QString::number(errSource.getLineNr());
            //strTmp.replace("<","&lt;");
            //strTmp.replace(">","&gt;");
            i_xmlStreamWriter.writeTextElement( "LineNr", strTmp );
        }

        if( !i_pEntry->m_errResultInfo.getAddErrInfoDscr().isEmpty() )
        {
            strTmp = i_pEntry->m_errResultInfo.getAddErrInfoDscr();
            //strTmp.replace("<","&lt;");
            //strTmp.replace(">","&gt;");
            i_xmlStreamWriter.writeTextElement( "AddInfo", strTmp );
        }
        if( !i_pEntry->m_strProposal.isEmpty() )
        {
            strTmp = i_pEntry->m_strProposal;
            //strTmp.replace("<","&lt;");
            //strTmp.replace(">","&gt;");
            i_xmlStreamWriter.writeTextElement( "Proposal", strTmp );
        }

        i_xmlStreamWriter.writeEndElement();

    } // if( i_pEntry != nullptr )

} // saveEntry

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Internal method called by the public addEntry method to add an entry
    to the error log.

    @param i_dateTime [in] Date time when the error occurred.
    @param i_errResultInfo [in] Error to be added to the error log.
    @param i_strProposal [in] Suggestion on how to remove the error cause.

    @return Pointer to created error log entry.

    @note If the maximum number of error log entries has already been reached when
          invoking this method the entry will not be added. Instead the last error
          entry will be removed and replaced with a ListIsFull error and the error
          source of the error log instance itself.
*/
SErrLogEntry* CErrLog::addEntry_(
    const QDateTime&      i_dateTime,
    const SErrResultInfo& i_errResultInfo,
    const QString&        i_strProposal )
//------------------------------------------------------------------------------
{
    /* This protected method is called from public methods and the mutex is already locked
    QMutexLocker mtxLockerInst(m_pMtx); */

    QDateTime dateTime = i_dateTime;

    if( !dateTime.isValid() )
    {
        dateTime = QDateTime::currentDateTime();
    }

    int iOccurrences = 1;

    SErrLogEntry* pEntry = findEntry(i_errResultInfo);

    if( pEntry != nullptr )
    {
        pEntry->m_dateTime = dateTime;
        pEntry->m_strProposal = i_strProposal;
        pEntry->m_iOccurrences++;

        if( m_iAddEntryRecursionCounter == 0 )
        {
            ++m_iAddEntryRecursionCounter;

            if( !m_bRecallingModel && !m_bClearingModel )
            {
                // If there is a problem with the save call (saving file not possible)
                // the qt message handler is called invoking this "addEntry_" method again
                // whereupon again the save method would be called and so on. The recursion
                // counter avoids this endless recursion.
                save();
            }

            --m_iAddEntryRecursionCounter;

        } // if( m_iAddEntryRecursionCounter == 0 )

        emit entryChanged(pEntry->m_errResultInfo);
    }
    else // if( pEntry == nullptr )
    {
        pEntry = new SErrLogEntry(
            /* dateTime      */ dateTime,
            /* fSysTime_us   */ ZS::System::Time::getProcTimeInMicroSec(),
            /* errResultInfo */ i_errResultInfo,
            /* strProposal   */ i_strProposal,
            /* iOccurrences  */ iOccurrences );

        EResultSeverity severity = i_errResultInfo.getSeverity();

        pEntry->m_iRowIdx = m_ararpEntries[severity].count();

        m_ararpEntries[severity].append(pEntry);

        if( m_iAddEntryRecursionCounter == 0 )
        {
            ++m_iAddEntryRecursionCounter;

            if( !m_bRecallingModel && !m_bClearingModel )
            {
                // If there is a problem with the save call (saving file not possible)
                // the qt message handler is called invoking this "addEntry_" method again
                // whereupon again the save method would be called and so on. The recursion
                // counter avoids this endless recursion.
                save();
            }

            --m_iAddEntryRecursionCounter;

        } // if( m_iAddEntryRecursionCounter == 0 )

        emit entryAdded(pEntry->m_errResultInfo);

    } // if( pEntry == nullptr )

    return pEntry;

} // addEntry_

//------------------------------------------------------------------------------
/*! Internal method called by the public changeEntry method to change an existing
    error log entry.

    The search pattern includes the result code, the result severity,
    the error source and the additional error info.

    @param i_pEntry [in] Pointer to error log entry to be modified.
    @param i_bModifyOccurrences [in] Flag to indicate whether the number of occurrences should be modified.
    @param i_iOccurrences [in] Number of occurrences to be set for the error log entry - if the modify flag is set.
    @param i_bModifyDateTime [in] Flag to indicate whether the date time should be modified.
    @param i_dateTime [in] Date time to be set for the error log entry - if the modify flag is set.
    @param i_bModifyProposal [in] Flag to indicate whether the proposol to fix the error should be modified.
    @param i_strProposal [in] Proposal to be set for the error log entry - if the modify flag is set.
*/
void CErrLog::changeEntry_(
    SErrLogEntry*     i_pEntry,
    bool              i_bModifyOccurrences,
    int               i_iOccurrences,
    bool              i_bModifyDateTime,
    const QDateTime&  i_dateTime,
    bool              i_bModifyProposal,
    const QString&    i_strProposal )
//------------------------------------------------------------------------------
{
    /* This protected method is called from public methods and the mutex is already locked
    QMutexLocker mtxLockerInst(m_pMtx); */

    if( i_pEntry != nullptr )
    {
        if( i_bModifyOccurrences )
        {
            i_pEntry->m_iOccurrences = i_iOccurrences;
        }
        if( i_bModifyDateTime )
        {
            if( !i_dateTime.isValid() )
            {
                i_pEntry->m_dateTime = QDateTime::currentDateTime();
            }
            else
            {
                i_pEntry->m_dateTime = i_dateTime;
            }
        }
        if( i_bModifyProposal )
        {
            i_pEntry->m_strProposal = i_strProposal;
        }

        if( !m_bRecallingModel && !m_bClearingModel )
        {
            save();
        }

        emit entryChanged(i_pEntry->m_errResultInfo);

    } // if( i_pEntry != nullptr )

} // changeEntry_

//------------------------------------------------------------------------------
/*! Internal method called by the public remove entry method to remove an existing
    error log entry.

    @param i_iRowIdx [in] Row index of the error log entry. The row indices are
                          counted for each severity separately.
    @param i_severity [in] Severity for which the error log entry should be removed.
                           By passing Undefined or Count the row numbers are counted
                           for all severities.
*/
void CErrLog::removeEntry_( int i_iRowIdx, EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    SErrLogEntry* pEntry = nullptr;

    if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )
    {
        int iCount = 0;

        // In this case counting rows starts at highest severity (Critical).
        for( int iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
        {
            if( i_iRowIdx < (iCount + m_ararpEntries[iSeverity].count()) )
            {
                pEntry = m_ararpEntries[iSeverity][i_iRowIdx-iCount];

                if( pEntry != nullptr )
                {
                    SErrResultInfo errResultInfo = pEntry->m_errResultInfo;

                    int iRowIdx = pEntry->m_iRowIdx;

                    m_ararpEntries[iSeverity].removeAt(iRowIdx);

                    delete pEntry;
                    pEntry = nullptr;

                    emit entryRemoved(errResultInfo);

                    for( ; iRowIdx < m_ararpEntries[iSeverity].count(); iRowIdx++ )
                    {
                        pEntry = m_ararpEntries[iSeverity][iRowIdx];

                        pEntry->m_iRowIdx--;

                        emit entryChanged(pEntry->m_errResultInfo);
                    }

                    if( !m_bRecallingModel && !m_bClearingModel )
                    {
                        save();
                    }
                } // if( pEntry != nullptr )

                break;

            } // if( i_iRowIdx < (iCount + m_ararpEntries[iSeverity].count()) )

            iCount += m_ararpEntries[iSeverity].count();

        } // for( int iSeverity = EResultSeverityCount-1; iSeverity >= 0; iSeverity-- )
    } // if( i_severity == EResultSeverityUndefined || i_severity == EResultSeverityCount )

    else // if( i_severity >= 0 && i_severity < EResultSeverityCount )
    {
        if( i_iRowIdx >= 0 && i_iRowIdx < m_ararpEntries[i_severity].count() )
        {
            pEntry = m_ararpEntries[i_severity][i_iRowIdx];

            if( pEntry != nullptr )
            {
                SErrResultInfo errResultInfo = pEntry->m_errResultInfo;

                int iRowIdx = pEntry->m_iRowIdx;

                m_ararpEntries[i_severity].removeAt(iRowIdx);

                delete pEntry;
                pEntry = nullptr;

                emit entryRemoved(errResultInfo);

                for( ; iRowIdx < m_ararpEntries[i_severity].count(); iRowIdx++ )
                {
                    pEntry = m_ararpEntries[i_severity][iRowIdx];

                    pEntry->m_iRowIdx--;

                    emit entryChanged(pEntry->m_errResultInfo);
                }

                if( !m_bRecallingModel && !m_bClearingModel )
                {
                    save();
                }
            } // if( pEntry != nullptr )
        } // if( i_iRowIdx >= 0 && i_iRowIdx < m_ararpEntries[i_severity].count() )
    } // if( i_severity >= 0 && i_severity < EResultSeverityCount )

} // removeEntry_
