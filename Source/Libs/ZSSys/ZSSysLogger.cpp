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

#include <QtCore/qmutex.h>
#include <QtCore/qthread.h>

#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysLogServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
class CLogger : public QObject, public CIdxTreeEntry
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructs a logger instance.

    Loggers are leaves in the logger index tree.

    Usually logger instances are created by the log server
    on calling the static "getLogger" methods.

    Logger objects will also be created if the log server reads the logger
    XML file containing the settings for all loggers which are currently part
    of the loggers index tree. Those are usually all logger objects which have
    been added to the index tree since program start.

    Recalling the settings of the loggers is only possible if the log server's
    "saveLoggers" method is called which should happen just before destroying
    the log server by the application. Right after creating the log server the
    method "recallLoggers" should be called to restore the last recent settings
    of the logger objects.

    Loggers live as long as loggers index tree is alive.

    Access to the loggers is protected by a mutex as the same instance
    (an instance with the same name space, the same class and object name)
    may be referenced from within different threads.

    @param i_strName [in] Name of the logger.
        This is the name of the leave within the index tree.
*/
CLogger::CLogger( const QString& i_strLeaveName ) :
//------------------------------------------------------------------------------
    QObject(),
    CIdxTreeEntry(EEntryType::Leave, i_strLeaveName),
    m_iBlockTreeEntryChangedSignalCounter(0),
    m_bAddThreadName(false),
    m_bAddDateTime(false),
    m_bAddSystemTime(false),
    m_strNameSpace(),
    m_strClassName(),
    m_strObjName(),
    m_enabled(EEnabled::Yes),
    m_eDetailLevel(ELogDetailLevel::None),
    m_strDataFilter(),
    m_strlstDataFilterInclude(),
    m_strlstDataFilterExclude()
{
    m_pMtx = new QRecursiveMutex();
}

//------------------------------------------------------------------------------
/*! @brief Destroys the logger instance removing itself from the index tree.

    Usually logger instances will not be deleted directly but only by the index
    tree if the index tree itself is destroyed.

    The dtor emits the "aboutToBeDestroyed" signal at the beginning and the
    QObjects "destroyed" signal at the end.
*/
CLogger::~CLogger()
//------------------------------------------------------------------------------
{
    emit aboutToBeDestroyed(this);

    if( m_iBlockTreeEntryChangedSignalCounter != 0 )
    {
        SErrResultInfo errResultInfo(
            /* errSource         */ NameSpace(), ClassName(), keyInTree(), "dtor",
            /* result            */ EResultObjRefCounterIsNotZero,
            /* severity          */ EResultSeverityError,
            /* strAddErrInfoDscr */ "The dtor is called even if the treeEntryChangedBlocked counter is not 0 but " + QString::number(m_iBlockTreeEntryChangedSignalCounter));
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }
    m_pMtx = nullptr;

    m_iBlockTreeEntryChangedSignalCounter = 0;
    m_bAddThreadName = false;
    m_bAddDateTime = false;
    m_bAddSystemTime = false;
    //m_strNameSpace;
    //m_strClassName;
    //m_strObjName;
    m_enabled = static_cast<EEnabled>(0);
    m_eDetailLevel = static_cast<ELogDetailLevel>(0);
    //m_strDataFilter;
    //m_strlstDataFilterInclude;
    //m_strlstDataFilterExclude;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns a reference to the index tree the logger belongs to.

    The logger is a leave entry in the index tree.

    @return Reference to the loggers index tree.
*/
CIdxTreeLoggers* CLogger::getLoggersIdxTree()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CIdxTreeLoggers*>(m_pTree);
}

//------------------------------------------------------------------------------
/*! @brief Returns a reference to the log server the logger belongs to.

    The log server creates the loggers index tree and the logger is a leave entry
    in the index tree.

    @return Reference to the log server hosting the loggers index tree the
            logger belongs to.
*/
CLogServer* CLogger::getLogServer()
//------------------------------------------------------------------------------
{
    CLogServer* pLogServer = nullptr;

    CIdxTreeLoggers* pTree = getLoggersIdxTree();

    if( pTree != nullptr )
    {
        QObject* pObjParent = pTree->parent();

        if( pObjParent != nullptr )
        {
            // May be nullptr if the parent object is not the log
            // server but e.g. the log client.
            pLogServer = dynamic_cast<CLogServer*>(pObjParent);
        }
    }
    return pLogServer;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a log entry for the given log level.

    A log entry will only be created if the passed filter detail level is not
    None and is equal or greater than the current log level of the logger.

    @Examples

        // The logger should be used to just show normal info messages.
        CLogger* pLogger = CLogServer::GetLogger("MyLogger");
        pLogger->setLogLevel(ELogDetailLevel::Info);

        // The following call will not create a log entry:
        pLogger->log(ELogDetailLevel::Debug, "Debug Message");

        // The following calls will create log entries:
        pLogger->log(ELogDetailLevel::Info, "Info Message");
        pLogger->log(ELogDetailLevel::Notice, "Notification Message");

    In addition before writing a log entry you may check how detailed the string
    to be added to the log entry should be:

        // The log server should be used to output detailed debug messages.
        CLogger* pLogger = CLogServer::GetLogger("MyLogger");
        pLogger->setLogLevel(ELogDetailLevel::DebugDetailed);

        QString strLogEntry;
        if( pLogger->getLogLevel() >= ELogDetailLevel::DebugVerbose ) { // false
            strLogEntry = "Verbose debug message containing many details";
        }
        else if( pLogger->getLogLevel() >= ELogDetailLevel::DebugDetailed ) { // true
            strLogEntry = "Detailed debug message containing some details";
        }
        else if( pLogger->getLogLevel() >= ELogDetailLevel::Debug ) {
            strLogEntry = "Debug message";
        }
        // Output of Detailed debug message
        pLogger->log(ELogDetailLevel::Debug, strLogEntry);

    @param i_eFilterDetailLevel [in]
        If the given filter detail level is not None and is equal or greater
        than the current detail level of the log server the log entry will
        be added the log file.
    @param i_strLogEntry [in] String to be logged.
*/
void CLogger::log( ELogDetailLevel i_eFilterDetailLevel, const QString& i_strLogEntry )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    CLogServer* pLogServer = getLogServer();
    if( pLogServer != nullptr )
    {
        // The log server will access the logger to check whether
        // logging is active for the given log level.
        pLogServer->log(this, i_eFilterDetailLevel, i_strLogEntry);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Enables log outputs for this object.

    Log outputs cannot only be controlled via the detail level but log outputs
    can also be enabled or disabled by this flag. This is useful if a group of
    objects belonging to a namespace should be temporarily disabled and enabled
    later on restoring the previous detail level.

    @param i_enabled [in] Flag to enable or disable log outputs.
                          - On ... tracing is enabled
                          - Off .. tracing is disabled
*/
void CLogger::setEnabled( EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_enabled != i_enabled )
    {
        m_enabled = i_enabled;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns whether log outputs are enabled or disabled.

    @return Flag indicating whether log outputs are enabled or disabled.
*/
EEnabled CLogger::getEnabled() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_enabled;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether log outputs are enabled.

    @return Flag indicating whether log outputs are enabled or disabled.
*/
bool CLogger::isEnabled() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_enabled == EEnabled::Yes);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the detail level of log outputs for this object.

    If set to None log outputs are disabled.
    Higher detail levels include lower detail levels.

    @param i_eDetailLevel [in] Detail level.
*/
void CLogger::setLogLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_eDetailLevel != i_eDetailLevel )
    {
        m_eDetailLevel = i_eDetailLevel;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the detail level of log outputs for this object.

    @return Detail level.
*/
ELogDetailLevel CLogger::getLogLevel() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_eDetailLevel;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether logging is active.

    The method checks both the enabled flag and compares the detail level passed
    as argument with the current detail level set at the logger.

    Log outputs are active if
    - loggin is enabled and
    - if the passed detail level is equal or greater than the loggers current
      detail level and
    - if the current detail level is not 0 (None).

    Example:

        CLogger* pLogger = CLogServer::GetLogger("ZS::Diagram", "CWdgtDiagram", "Analyzer");

        bool bLoggingActive;

        pLogger->setEnabled(EEnabledOn);
        pLogger->setDetailLevel(ELogDetailLevel::Debug);

        bLoggingActive = pLogger->isActive(ELogDetailLevel::InfoVerbose); .. returns true
        bLoggingActive = pLogger->isActive(ELogDetailLevel::DebugDetailed);  .. returns false

    @param i_eFilterDetailLevel [in]
        Log outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the logger.

    @return Flag indicating whether log outputs are active.
*/
bool CLogger::isActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bActive = false;

    if( i_eFilterDetailLevel > ELogDetailLevel::None )
    {
        if( m_enabled == EEnabled::Yes && m_eDetailLevel >= i_eFilterDetailLevel )
        {
            bActive = true;
        }
    }
    return bActive;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets a filter for log entries.

    Filtering can be done in two ways:

    - Strings may be defined which must occur in the log entry.
    - Strings may be defined which may not occur in the log entry.

    To define a string which must be included encapsulate the string
    in the delimiter sequence "$I{" and "}I$" (the end delimiter is the
    start delimiter in reverse order).

        $ .. Command
        I .. Include
        { .. Start
        } .. End

    To define a string which must NOT be included encapsulate the string
    in the delimiter sequence "$!I{" and "}I!$"

        $ .. Command
        ! .. Not
        I .. Include
        { .. Start
        } .. End

    @Examples

    - Log only those strings which include the string "abc":

        m_pLogger->setDataFilter("$I{abc}I$");

        m_pLogger->isSuppressedByDataFilter("1 abc bca cab") // returns false
        m_pLogger->isSuppressedByDataFilter("2 xyz yzx zxy") // returns true

    - Don't log strings which include the string "abc":

        m_pLogger->setDataFilter("$!I{abc}I!$");

        m_pLogger->isSuppressedByDataFilter("1 abc bca cab") // returns true
        m_pLogger->isSuppressedByDataFilter("2 xyz yzx zxy") // returns false

    - Log only those strings which include "Hello" but don't log strings including "World":

          m_pLogger->setDataFilter("$I{Hello}I!$$!I{World}I!$");
          m_pLogger->isSuppressedByDataFilter("Hello World") // returns true
          m_pLogger->isSuppressedByDataFilter("Hello Welt")  // returns false
          m_pLogger->isSuppressedByDataFilter("Hallo World") // returns true
          m_pLogger->isSuppressedByDataFilter("Hallo Welt")  // returns true

    @param i_strFilter [in] Filter containing Include and Not Include expressions.
*/
void CLogger::setDataFilter( const QString& i_strFilter )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_strDataFilter != i_strFilter )
    {
        m_strDataFilter = i_strFilter;

        m_strlstDataFilterInclude.clear();
        m_strlstDataFilterExclude.clear();

        if( !m_strDataFilter.isEmpty() )
        {
            QString strDataFilter = m_strDataFilter;

            while( !strDataFilter.isEmpty() )
            {
                int idxStart = strDataFilter.indexOf("$I{");
                if( idxStart < 0 )
                {
                    break;
                }
                int idxEnd = strDataFilter.indexOf("}I$", idxStart);
                if( idxEnd < 0 )
                {
                    SErrResultInfo errResultInfo(
                        /* errSource         */ NameSpace(), ClassName(), keyInTree(), "setDataFilter",
                        /* result            */ EResultArgOutOfRange,
                        /* severity          */ EResultSeverityError,
                        /* strAddErrInfoDscr */ "Invalid data filter expression " + i_strFilter + " (missing closing \"}I$\")");
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    break;
                }
                idxStart += 3;
                int iLength = idxEnd - idxStart;
                m_strlstDataFilterInclude.append(strDataFilter.mid(idxStart, iLength));
                idxStart -= 3;
                iLength += 6;
                strDataFilter.remove(idxStart, iLength);
            }

            while( !strDataFilter.isEmpty() )
            {
                int idxStart = strDataFilter.indexOf("$!I{");
                if( idxStart < 0 )
                {
                    break;
                }
                int idxEnd = strDataFilter.indexOf("}I!$", idxStart);
                if( idxEnd < 0 )
                {
                    SErrResultInfo errResultInfo(
                        /* errSource         */ NameSpace(), ClassName(), keyInTree(), "setDataFilter",
                        /* result            */ EResultArgOutOfRange,
                        /* severity          */ EResultSeverityError,
                        /* strAddErrInfoDscr */ "Invalid data filter expression " + i_strFilter + " (missing closing \"}I!$\")");
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    break;
                }
                idxStart += 4;
                int iLength = idxEnd - idxStart;
                m_strlstDataFilterExclude.append(strDataFilter.mid(idxStart, iLength));
                idxStart -= 4;
                iLength += 8;
                strDataFilter.remove(idxStart, iLength);
            }

            if( m_strlstDataFilterInclude.isEmpty() && m_strlstDataFilterExclude.isEmpty() )
            {
                SErrResultInfo errResultInfo(
                    /* errSource         */ NameSpace(), ClassName(), keyInTree(), "setDataFilter",
                    /* result            */ EResultArgOutOfRange,
                    /* severity          */ EResultSeverityError,
                    /* strAddErrInfoDscr */ "Invalid data filter expression " + i_strFilter + " (missing any valid \"$I{\" expressions)");
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }
        }

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the data filter.

    @return Data filter (string containing a regular expression).
*/
QString CLogger::getDataFilter() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strDataFilter;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether given data should be suppressed by the data filter.

    @param i_strLogEntry [in]
        String to be checked against the filter string.

    @return true if the passed string should be suppressed, false otherwise.
*/
bool CLogger::isSuppressedByDataFilter( const QString& i_strLogEntry ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bSuppressed = false;

    if( !m_strlstDataFilterInclude.isEmpty() )
    {
        bSuppressed = true;

        for( const QString& strFilter : m_strlstDataFilterInclude )
        {
            if( i_strLogEntry.contains(strFilter) )
            {
                bSuppressed = false;
                break;
            }
        }
    }
    if( !bSuppressed && !m_strlstDataFilterExclude.isEmpty() )
    {
        for( const QString& strFilter : m_strlstDataFilterExclude )
        {
            if( i_strLogEntry.contains(strFilter) )
            {
                bSuppressed = true;
                break;
            }
        }
    }
    return bSuppressed;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the flag which indicates whether the current thread name
           should be added to the log entry.

    @param i_bAdd [in]
        true if the current thread name should be added, false otherwise.
*/
void CLogger::setAddThreadName( bool i_bAdd )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_bAddThreadName != i_bAdd )
    {
        m_bAddThreadName = i_bAdd;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the flag indicating whether the current thread name should be
           added to the log entry.

    @return true if the current thread name should be added, false otherwise.
*/
bool CLogger::addThreadName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_bAddThreadName;
}

//------------------------------------------------------------------------------
/*! @brief Sets the flag which indicates whether the current date time
           should be added to the log entry.

    @param i_bAdd [in]
        true if the current date time should be added, false otherwise.
*/
void CLogger::setAddDateTime( bool i_bAdd )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_bAddDateTime != i_bAdd )
    {
        m_bAddDateTime = i_bAdd;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the flag indicating whether the current date time should be
           added to the log entry.

    @return true if the current date time should be added, false otherwise.
*/
bool CLogger::addDateTime() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_bAddDateTime;
}

//------------------------------------------------------------------------------
/*! @brief Sets the flag which indicates whether the current system time
           (time in seconds the application is running) should be added
           to the log entry.

    @param i_bAdd [in]
        true if the system time should be added, false otherwise.
*/
void CLogger::setAddSystemTime( bool i_bAdd )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_bAddSystemTime != i_bAdd )
    {
        m_bAddSystemTime = i_bAdd;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the flag indicating whether the current system time
           (time in seconds the application is running) should be added
           to the log entry.

    @return true if the current syste time should be added, false otherwise.
*/
bool CLogger::addSystemTime() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_bAddSystemTime;
}

//------------------------------------------------------------------------------
/*! @brief Sets the name space of the logger.

    @param i_strNameSpace [in]
        Name space used by the logger to format the log entry string.
        Use empty string to remove the name space.
*/
void CLogger::setNameSpace( const QString& i_strNameSpace )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_strNameSpace != i_strNameSpace )
    {
        m_strNameSpace = i_strNameSpace;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the name space of the logger.

    @return Name space used by the logger to format the log entry string.
*/
QString CLogger::getNameSpace() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strNameSpace;
}

//------------------------------------------------------------------------------
/*! @brief Sets the class name of the logger.

    @param i_strClass [in]
        Class name used by the logger to format the log entry string.
        Use empty string to remove the class name.
*/
void CLogger::setClassName( const QString& i_strClassName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_strClassName != i_strClassName )
    {
        m_strClassName = i_strClassName;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the class name of the logger.

    @return Class name used by the logger to format the log entry string.
*/
QString CLogger::getClassName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strClassName;
}

//------------------------------------------------------------------------------
/*! @brief Sets the object name of the logger.

    @param i_strObjName [in]
        Object name used by the logger to format the log entry string.
        Use empty string to remove the object name.
*/
void CLogger::setObjectName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    if( m_strObjName != i_strObjName )
    {
        m_strObjName = i_strObjName;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the object name of the logger.

    @return Object name used by the logger to format the log entry string.
*/
QString CLogger::getObjectName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief If block is true, the treeEntryChanged signal emitted by this object will
           be blocked. If block is false, no such blocking will occur.

    When unblocking the treeEntryChanged signal is emitted.

    @param i_bBlock [in] If true the signal will be blocked, otherwise unblocked.

    @return The return value is the previous value of isTreeEntryChangedSignalBlocked().

    @note May throw an exception with EResultObjRefCounterIsNotZero if the signal
          has been unblocked more often than blocked.
*/
bool CLogger::blockTreeEntryChangedSignal( bool i_bBlock )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bPreviouslyLocked = (m_iBlockTreeEntryChangedSignalCounter > 0);

    if( i_bBlock )
    {
        ++m_iBlockTreeEntryChangedSignalCounter;
    }
    else // if( !i_bBlock )
    {
        if( m_iBlockTreeEntryChangedSignalCounter <= 0 )
        {
            SErrResultInfo errResultInfo(
                /* errSource     */ NameSpace(), ClassName(), keyInTree(), "blockTreeEntryChangedSignal",
                /* result        */ EResultObjRefCounterIsNotZero,
                /* severity      */ EResultSeverityError,
                /* strAddErrInfo */ "Unblock tree entry changed signal counter becomes less than 0" );
            throw CException(__FILE__, __LINE__, errResultInfo);
        }
        --m_iBlockTreeEntryChangedSignalCounter;

         if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
    }

    return bPreviouslyLocked;
}

//------------------------------------------------------------------------------
/*! @brief Returns true if signals are blocked; otherwise returns false.

    Signals are not blocked by default.

    @return true if signals are blocked, false otherwise.
*/
bool CLogger::isTreeEntryChangedSignalBlocked() const
//------------------------------------------------------------------------------
{
    return (m_iBlockTreeEntryChangedSignalCounter > 0);
}
