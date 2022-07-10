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

    Loggers live as long as loggers index tree is alive. Once created and added
    to the index tree only the reference counter is incremented and decremented.
    If the reference counter becomes 0 the logger not be deleted - unless the
    logger is renamed and the reference counter of the logger with the previous
    name becomes 0. Usually loggers will only be deleted by the destructor of
    loggers index tree.

    If a module, class or instance of a class using the logger is destroyed the
    reference to the logger must be releasing by invoking the log servers
    "releaseLogger" method. After releasing the logger the states and attributes
    of the logger instance may still be changed. Meansing that e.g. the detail
    level may be changed and/or logging may be enabled or disabled even if no
    module, class or instance of a class is referencing the logger anymore.

    If the same instance is referenced by different threads on calling
    "LogServer::getLogger" the already created logger is returned
    and the reference counter of the logger is incremented.

    Access to the loggers is protected by a mutex as the same instance
    (an instance with the same name space, the same class and object name)
    may be referenced from within different threads.

    @param i_strNameSpace [in] Namespace (e.g. "ZS::System").
    @param i_strClassName [in] Class name (e.g. "CClient")
        Please note that the class name may also contain node separators
        to define group of methods which should be separately controlled.
    @param i_strObjName [in] Object name (e.g. "ZSTrcClient")
        Please note that the object name may also contain node separators
        to define logically grouped objects.
    @param i_strTreeEntryName [in] Name of the leave within the index tree.
        This is usually equal to either the object name or the class name.
        If the class or object name contain node separators the name of the
        leave is different and corresponds to the last section within the
        concatenated names.
        E.g. NameSpace = "ZS::System"
             ClassName = "CModelIdxTree"
             ObjName = "ZS::IpcLog-TestSteps"
             -> TreeEntryName = IpcLog-TestSteps
        The caller must take this into account.
*/
CLogger::CLogger(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strTreeEntryName ) :
//------------------------------------------------------------------------------
    QObject(),
    CIdxTreeEntry(EIdxTreeEntryType::Leave, i_strTreeEntryName),
    m_iBlockTreeEntryChangedSignalCounter(0),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_strObjThreadName(),
    m_iLockCount(0),
    m_bDeleteOnUnlock(false),
    m_iRefCount(0),
    m_enabled(EEnabled::Yes),
    m_eDetailLevel(ELogDetailLevel::None),
    m_strDataFilter()
{
    m_pMtx = new QMutex(QMutex::Recursive);

    QThread* pThread = QThread::currentThread();

    if( pThread != nullptr )
    {
        m_strObjThreadName = pThread->objectName();

        if( m_strObjThreadName.isEmpty() )
        {
            m_strObjThreadName = CLogServer::GetCurrentThreadName();
        }
        if( m_strObjThreadName.isEmpty() )
        {
            m_strObjThreadName = QString("Thread") + threadId2Str(QThread::currentThreadId());
        }
    } // if( pThread != nullptr )

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the logger instance removing itself from the index tree.

    Usually logger instances will not be deleted directly but only by the index
    tree if the index tree itself is destroyed. Before invoking the destructor
    all references to the logger should have been released (the reference counter
    should be 0).

    The dtor emits the "aboutToBeDestroyed" signal at the beginning and the
    QObjects "destroyed" signal at the end.
*/
CLogger::~CLogger()
//------------------------------------------------------------------------------
{
    emit aboutToBeDestroyed(this);

    if( m_bDeleteOnUnlock )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), keyInTree(), "dtor",
            /* result        */ EResultObjRefCounterIsNotZero,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "The dtor is called even if the object should have already been removed by unlocking it");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    if( m_iLockCount != 0 )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), keyInTree(), "dtor",
            /* result        */ EResultObjRefCounterIsNotZero,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "The dtor is called even if the objects lock counter is not 0 but " + QString::number(m_iLockCount));

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    if( m_iRefCount != 0 )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), keyInTree(), "dtor",
            /* result        */ EResultObjRefCounterIsNotZero,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "The dtor is called even if the objects reference counter is not 0 but " + QString::number(m_iRefCount));

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

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
    //m_strNameSpace;
    //m_strClassName;
    //m_strObjName;
    //m_strObjThreadName;
    m_iLockCount = 0;
    m_bDeleteOnUnlock = false;
    m_iRefCount = 0;
    m_enabled = static_cast<EEnabled>(0);
    m_eDetailLevel = static_cast<ELogDetailLevel>(0);
    //m_strDataFilter;

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
/*! @brief Returns the name space of the module, class or instance referencing
           the logger.

    Usually this is equal to the parent path of the parent branch of the
    loggers leave entry.

    E.g. if the logger is used to control log outputs of the instance
    "ZS::Diagram::CWdgtDiagram::Analyzer" the name space would be "ZS::Diagram".

    If the logger is used to control the log outputs of a class or module like
    "ZS::Diagram::CWdgtDiagram" the name space equals the parent path of the
    leave entry.

    @return Name space of the module, class or instance referencing the logger.
*/
QString CLogger::getNameSpace() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strNameSpace;
}

//------------------------------------------------------------------------------
/*! @brief Returns the class name of the module, class or instance referencing
           the logger.

    Usually this is the name of the parent branch of the loggers leave entry.
    E.g. if the logger is used to control the log outputs of instance
    "ZS::Diagram::CWdgtDiagram::Analyzer" the class name would be "CWdgtDiagram".

    If the logger is used to control the log outputs of a class or module like
    "ZS::Diagram::CWdgtDiagram" the class name equals the name of leave entry.

    @return Class bame of the module, class or instance referencing the logger.
*/
QString CLogger::getClassName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strClassName;
}

//------------------------------------------------------------------------------
/*! @brief If the instance using the logger is renamed and the logger is used to
           control log outputs of this instance the new name of the instance got
           to be forwarded to the logger so that following log outputs are using
           the new object name.

    @note Don't mangle this with the QObject::setObjectName as the logger is
          derived from QObject but does not use the object name itself.

    @param i_strObjName [in] Object name (e.g. "ZSTrcClient")
        Object name of the instance creating this logger.
        Please note that the object name may also contain node separators
        to define logically grouped objects.
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
/*! @brief Returns the name of the object (instance) which should be controlled
           by the logger.

    The object name might be the same as the name of the leave entry.
    But please note that the object name may also contain node separators
    to define logically grouped objects.

    E.g. if the logger is used to control the log outputs of instance
    "RF::Analyzer" the object name would be this instance name and is different
    from the name of the leave which would be "Analyzer".

    If the logger is used to control the log outputs of a class or module like
    "ZS::Diagram::CWdgtDiagram" the object name is empty.

    @note Don't mangle this with the QObject::objectName as the logger is
          derived from QObject but does not use the object name itself.

    @return Object name of the module, class or instance referencing the logger.
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
/*! @brief Sets the name of the thread in which context the logger was created.

    It should normally not be necessary to use this method. Only when sending
    the information about the logger from the log server to the log client the
    client needs to call this method.

    @param i_strThreadName [in]
        Name of the thread in which context the loggerwas created.
*/
void CLogger::setObjectThreadName( const QString& i_strThreadName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_strObjThreadName != i_strThreadName )
    {
        m_strObjThreadName = i_strThreadName;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the name of the thread in which context the logger was created.

    @return Name of the the thread in which context the logger was created.
*/
QString CLogger::getObjectThreadName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strObjThreadName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Locks the logger by incrementing the lock count.

    @return Current lock count.
*/
int CLogger::lock()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    ++m_iLockCount;

    if( m_pTree != nullptr )
    {
        // Takes too much time as too often called.
        //if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
    }
    return m_iLockCount;
}

//------------------------------------------------------------------------------
/*! @brief Unlocks the object by decrementing the lock counter.

    Only if the lock counter reaches 0 the logger is really unlocked.

    @note When unlocking the logger the "deleteOnUnlock" flag must be
          checked. If true and the reference counter is 0 the logger
          should be deleted to reduce the number of no longer used loggers
          (as the object referencing the object has been renamed).
          To delete the object release the instance by invoking
          CLogServer::ReleaseTraceAdminObj.

    @return Current lock count.
*/
int CLogger::unlock()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    --m_iLockCount;

    if( m_pTree != nullptr )
    {
        // Takes too much time as too often called.
        //if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
    }
    return m_iLockCount;
}

//------------------------------------------------------------------------------
/*! Returns whether the logger is locked.

    @return true if the object is locked, false otherwise.
*/
bool CLogger::isLocked() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_iLockCount > 0);
}

//------------------------------------------------------------------------------
/*! Returns the current lock count.

    @return Current lock count.
*/
int CLogger::getLockCount() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_iLockCount;
}

//------------------------------------------------------------------------------
/*! @brief Sets the flag that the logger is no longer needed when unlocked.

    On renaming the object and if the reference counter reaches 0 the
    logger should be deleted. 

    @param i_bDelete [in]
        Flag to indicate whether the object can be deleted if unlocked
        (and the reference counter is still 0).
*/
void CLogger::setDeleteOnUnlock( bool i_bDelete )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_bDeleteOnUnlock != i_bDelete )
    {
        m_bDeleteOnUnlock = i_bDelete;

        if( m_pTree != nullptr )
        {
            //if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the flag whether the logger may be deleted if unlocked and
           no longer used.

    To delete the logger call CLogServer::ReleaseLogger.

    @return true if the flag is set, false otherwise.
*/
bool CLogger::deleteOnUnlock() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_iLockCount <= 0) && (m_iRefCount <= 0) && m_bDeleteOnUnlock;
}

//------------------------------------------------------------------------------
/*! @brief Increments the reference counter.

    Normally it should never be necessary to call this method directly.
    Only the log server should use this method on each call of "getLogger".

    Usually loggers are only referenced by one specific module, class or
    instance of a class to control the detail level of log outputs.
    In certain circumstances or in case of a copy and paste error the same
    logger may be referenced by several modules, classes or instances of
    classes. If so the reference counter may become greater than 1.

    @return Current reference counter.
*/
int CLogger::incrementRefCount()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    ++m_iRefCount;

    if( m_pTree != nullptr )
    {
        if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
    }
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! @brief Decrements the reference counter.

    Normally it should never be necessary to call this method directly.
    Only the log server should use this method on each call of "releaseLogger".

    Usually loggers are only referenced by one specific module, class or instance
    of a class to control the detail level of log outputs. In certain circumstances
    or in case of a copy and paste error the same logger may be referenced by
    several modules, classes or instances of classes. If so the reference counter
    may become greater than 1.

    @return Current reference counter.
*/
int CLogger::decrementRefCount()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    --m_iRefCount;

    if( m_pTree != nullptr )
    {
        if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
    }
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! @brief Sets the reference counter of the object.

    It should normally not be necessary to use this method within the application
    to be logged. Only when sending the information about the logger from the
    log server to the log client the client needs to call this method.

    @param i_iRefCount [in] Reference count to be set.
*/
void CLogger::setRefCount( int i_iRefCount )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_iRefCount != i_iRefCount )
    {
        m_iRefCount = i_iRefCount;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Returns the current reference counter.

    @return Current reference counter.
*/
int CLogger::getRefCount() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_iRefCount;
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
void CLogger::setDetailLevel( ELogDetailLevel i_eDetailLevel )
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
ELogDetailLevel CLogger::getDetailLevel() const
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
        pLogger->setDetailLevel(ELogDetailLevel::DebugNormal);

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
/*! @brief Sets the filter as a regular expression.

    The filter is a regular expression which allows to define a positive
    pattern where only the data will be logged which mets the expression or a
    negative pattern which suppresses log outputs if the filter does not match.

    Please see the description of QRegExp class for more details on how
    to define regular expressions. Here are just a view (but often used)
    simple examples.

    Simple Examples (output log if string contains sub string):

    - Check if the string "abc" is contained:

          m_pLogger->setDataFilter("abc");
          m_pLogger->isDataSuppressedByFilter("1 abc bca cab") // returns false
          m_pLogger->isDataSuppressedByFilter("2 xyz yzx zxy") // returns true

    - Check if the strings "def" or "uvw" are contained:

          m_pLogger->setDataFilter("def|uvw");
          m_pLogger->isDataSuppressedByFilter("3 def efd fde") // returns true
          m_pLogger->isDataSuppressedByFilter("4 uvw vwu wuv") // returns true

    More Complex Examples (output log if string does not contain sub string):

    - Check if the string "ghi" is NOT contained:

      Suppressing a string if a string does not contain a substring is supposed
      to be seldom used. Its complicated to setup a regular expression for this.
      The following is an example on how this could be done.

          m_pLogger->setDataFilter("^((?!ghi).)*$");
          m_pLogger->isDataSuppressedByFilter("5 ghi hig igh") // returns true
          m_pLogger->isDataSuppressedByFilter("6 rst str trs") // returns false

    @param i_strFilter [in] Filter as regular expression.

    @note If a string should be suppressed if it does not contain a substring
          more simple to apply this would be an additional method or provide
          a flag like match or dontMatch.
*/
void CLogger::setDataFilter( const QString& i_strFilter )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_strDataFilter != i_strFilter )
    {
        m_strDataFilter = i_strFilter;

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

    @param i_strData [in]
        String to be checked against the filter string.

    @return true if the passed string should be suppressed, false otherwise.
*/
bool CLogger::isDataSuppressedByFilter( const QString& i_strData ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bSuppressed = false;

    if( !m_strDataFilter.isEmpty() )
    {
        QRegExp rx(m_strDataFilter);

        if( rx.indexIn(i_strData) < 0 )
        {
            bSuppressed = true;
        }
    }
    return bSuppressed;
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
                /* errSource     */ nameSpace(), className(), keyInTree(), "blockTreeEntryChangedSignal",
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


/*******************************************************************************
class CLoggerRefAnchor
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the class.

    @param i_strNameSpace [in] Namespace of the class to be logged.
    @param i_strClassName [in] Class name of the class to be logged.
*/
CLoggerRefAnchor::CLoggerRefAnchor(
    const QString& i_strNameSpace,
    const QString& i_strClassName ) :
//------------------------------------------------------------------------------
    m_mtx(),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_pLogger(nullptr),
    m_idxInTree(-1),
    m_iRefCount(0)
{
}

//------------------------------------------------------------------------------
/*! @brief Destroys the instance.
*/
CLoggerRefAnchor::~CLoggerRefAnchor()
//------------------------------------------------------------------------------
{
    //m_mtx;
    //m_strNameSpace;
    //m_strClassName;
    m_pLogger = nullptr;
    m_idxInTree = 0;
    m_iRefCount = 0;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Allocates the logger object.

    If the logger object does not yet belong to the log server, the object will
    be added. Otherwise the reference to the already existing index tree entry
    is returned and the reference counter is incremented.

    The index tree entry is locally stored to speed up further access to the
    logger object.

    The pointer to the logger is kept until the program exits and the reference
    anchor is destroyed or if the logger is destroyed.
*/
void CLoggerRefAnchor::allocLogger()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_iRefCount > 0 )
    {
        if( m_pLogger == nullptr )
        {
            QString strExc = "RefCount is " + QString::number(m_iRefCount) + " but pointer to logger has not been stored";
            throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
        }
        if( m_idxInTree < 0 )
        {
            QString strExc = "RefCount is " + QString::number(m_iRefCount) + " but index in tree has not been stored";
            throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
        }
    }
    else if( m_iRefCount < 0 )
    {
        QString strExc = "RefCount " + QString::number(m_iRefCount) + " is out of range";
        throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
    }

    if( m_idxInTree >= 0 )
    {
        // Also the reference counter of the logger is incremented.
        m_pLogger = CLogServer::GetLogger(m_idxInTree);
    }
    else if( m_pLogger == nullptr )
    {
        // Also the reference counter of the logger is incremented.
        m_pLogger = CLogServer::GetLogger(m_strNameSpace, m_strClassName, "");

        if( m_pLogger != nullptr )
        {
            // The pointer to the logger is kept until the program is exited and the
            // reference anchor is destroyed or if the logger is destroyed.
            if( !QObject::connect(
                /* pObjSender   */ m_pLogger,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTrcAdminObjDestroyed(QObject*)),
                /* cnctType     */ Qt::DirectConnection ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
    }
    else // if( m_pLogger != nullptr )
    {
        // Also the reference counter of the logger is incremented.
        m_pLogger = CLogServer::GetLogger(m_strNameSpace, m_strClassName, "");
    }

    if( m_pLogger != nullptr )
    {
        m_idxInTree = m_pLogger->indexInTree();
    }

    m_iRefCount++;

} // allocTrcAdminObj

//------------------------------------------------------------------------------
/*! @brief Releases the logger object.

    The logger object is not deleted but just a reference counter is decremented.
    Even if the reference counter reaches 0 the logger stays alive to allow
    modifying the properties like the detail level.

     @note The pointer to the logger is kept until the program exits and the
           reference anchor is destroyed or if the logger is destroyed.
*/
void CLoggerRefAnchor::releaseLogger()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_iRefCount <= 0 )
    {
        QString strExc = "Cannot release the logger if RefCount is " + QString::number(m_iRefCount);
        throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
    }
    if( m_pLogger == nullptr )
    {
        QString strExc = "RefCount is " + QString::number(m_iRefCount) + " but pointer to logger has not been stored";
        throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
    }
    if( m_pLogger->getRefCount() < m_iRefCount )
    {
        QString strExc = "RefCount is " + QString::number(m_iRefCount) + " is greater than ref counter of logger";
        throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
    }

    // The pointer is kept and will only be reset to nullptr if the
    // logger is going to be destroyed.
    CLogServer::ReleaseLogger(m_pLogger);

    m_iRefCount--;
}

//------------------------------------------------------------------------------
/*! @brief Returns the reference to the logger.

    @return Reference to logger.
            Might be nullptr if the logger has not yet been allocated.
*/
CLogger* CLoggerRefAnchor::logger()
//------------------------------------------------------------------------------
{
    return m_pLogger;
}

//------------------------------------------------------------------------------
/*! @brief Sets the detail level.

    This method has no effect if the logger has not yet been allocated.

    @param i_eDetailLevel [in] Detail level.
*/
void CLoggerRefAnchor::setDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_pLogger != nullptr )
    {
        m_pLogger->setDetailLevel(i_eDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether log outputs are active for the given filter detail level.

    @param i_eFilterDetailLevel [in]
        Log detail level which should be checked. If the logger's detail level is
        not 0 (None) and is greater or equal the filter level log outputs are active.

    @return true if log outputs are active, false otherwise.
*/
bool CLoggerRefAnchor::isActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    bool bActive = false;
    if( m_pLogger != nullptr )
    {
        bActive = m_pLogger->isActive(i_eFilterDetailLevel);
    }
    return bActive;
}

/*==============================================================================
private slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Slot which is called if the logger is going to be destroyed.

    @param i_pLogger [in] Reference to logger which will be destroyed.
*/
void CLoggerRefAnchor::onLoggerDestroyed( QObject* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_pLogger == i_pLogger )
    {
        m_pLogger = nullptr;
        m_idxInTree = -1;
        m_iRefCount = 0;
    }
}


/*******************************************************************************
class CLoggerRefGuard
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the logger reference guard.

    @param i_pRefAnchor [in]
        Pointer to logger object reference anchor which should be guarded.
*/
CLoggerRefGuard::CLoggerRefGuard(CLoggerRefAnchor* i_pRefAnchor) :
//------------------------------------------------------------------------------
    m_pRefAnchor(i_pRefAnchor)
{
    m_pRefAnchor->allocLogger();
}

//------------------------------------------------------------------------------
/*! @brief Destroys the logger reference guard instance.
*/
CLoggerRefGuard::~CLoggerRefGuard()
//------------------------------------------------------------------------------
{
    m_pRefAnchor->releaseLogger();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the pointer to the logger.

    @return Pointer to logger (which may also be nullptr if the logger has not
            yet been allocated).
*/
CLogger* CLoggerRefGuard::logger()
//------------------------------------------------------------------------------
{
    CLogger* pLogger = nullptr;
    if( m_pRefAnchor != nullptr )
    {
        pLogger = m_pRefAnchor->logger();
    }
    return pLogger;
}

//------------------------------------------------------------------------------
/*! @brief Sets the logging detail level of the logger.

    This method has no effect if the logger has not yet been allocated.

    @param i_eDetailLevel [in] Detail level.
*/
void CLoggerRefGuard::setDetailLevel(ELogDetailLevel i_eDetailLevel)
//------------------------------------------------------------------------------
{
    if( m_pRefAnchor != nullptr )
    {
        m_pRefAnchor->setDetailLevel(i_eDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether log outputs are active for the given filter detail level.

    @param i_eFilterDetailLevel [in]
        Detail level which should be checked. If the logger's detail level is
        not 0 (None) and is greater or equal the filter level log outputs are active.

    @return true if logging is active, false otherwise.
*/
bool CLoggerRefGuard::isActive(ELogDetailLevel i_eFilterDetailLevel) const
//------------------------------------------------------------------------------
{
    bool bActive = false;
    if( m_pRefAnchor != nullptr )
    {
        bActive = m_pRefAnchor->isActive(i_eFilterDetailLevel);
    }
    return bActive;
}

//------------------------------------------------------------------------------
/*! Private method throwing an exception if called.

    The object name of the logger is the object name of the instance creating the
    logger. For the sake of clarification this method has been implemented as a
    private method throwing an exception if called.

    @return Nothing as method throws an exception
*/
QString CLogger::objectName() const
//------------------------------------------------------------------------------
{
    throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    return getObjectName();
}
