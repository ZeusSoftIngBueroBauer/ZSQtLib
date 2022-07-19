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

#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
class CTrcAdminObj : public QObject, public CIdxTreeEntry
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor (trace admin objects may only be created by the trace server)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructs a trace admin object instance.

    Trace admin objects are leaves in the trace admin object index tree.

    Usually trace admin object instances are created by the trace method server
    on calling the static "getTraceAdminObj" methods.

    Trace admin objects will also be created if the trace server reads the trace
    admin object XML file containing the settings for all trace admin objects
    which are currently part of the index tree. Those are usually all trace admin
    objects which have been added to the index tree since program start.

    Recalling the settings of the trace admin objects is only possible if the
    trace servers "saveAdminObjs" method is called which should happen just before
    destroying the trace server by the application. Right after creating the trace
    method server the method "recallAdminObjs" should be called to restore the last
    recent settings of the trace admin objects.

    Trace admin objects live as long as the trace admin object index tree lives.
    Once created and added to the trace admin object index tree only the reference
    counter is incremented and decremented. If the reference counter becomes 0
    the trace admin object will not be deleted  - unless the trace admin object is
    renamed and the reference counter of the logger with the previous name becomes 0.
    Usually trace admin object will only be deleted by the destructor of the trace
    admin objects index tree.

    If a module, class or instance of a class using the trace admin object is
    destroyed the reference to the trace admin object must be releasing by invoking
    the trace method servers "releaseTraceAdminObj" method. After releasing the
    trace admin object the states and attributes of the admin object instance may
    still be changed. Meansing that e.g. the detail level may be changed and/or
    tracing may be enabled or disabled even if no module, class or instance of
    a class is referencing the admin object anymore.

    If the same instance is referenced by different threads on calling
    "TrcServer::getTraceAdminObj" the already created trace admin object is returned
    and the reference counter of the trace admin object is incremented.

    Access to the trace admin objects is protected by a mutex as the same instance
    (an instance with the same name space, the same class and object name) may be
    referenced from within different threads.

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
             ObjName = "ZS::IpcTrace-TestSteps"
             -> TreeEntryName = IpcTrace-TestSteps
        The caller must take this into account.
*/
CTrcAdminObj::CTrcAdminObj(
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
    m_eTrcDetailLevelMethodCalls(EMethodTraceDetailLevel::None),
    m_eTrcDetailLevelRuntimeInfo(ELogDetailLevel::None),
    m_strDataFilter()
{
    m_pMtx = new QMutex(QMutex::Recursive);

    QThread* pThread = QThread::currentThread();

    if( pThread != nullptr )
    {
        m_strObjThreadName = pThread->objectName();

        if( m_strObjThreadName.isEmpty() )
        {
            m_strObjThreadName = CTrcServer::GetCurrentThreadName();
        }
        if( m_strObjThreadName.isEmpty() )
        {
            m_strObjThreadName = QString("Thread") + threadId2Str(QThread::currentThreadId());
        }
    } // if( pThread != nullptr )

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the trace admin object instance removing itself from the index tree.

    Usually trace admin object instances will not be deleted directly but only
    by the index tree if the index tree itself is destroyed. Before invoking the
    destructor all references to the trace admin object should have been released
    (the reference counter should be 0).

    The dtor emits the "aboutToBeDestroyed" signal at the beginning and the
    QObjects "destroyed" signal at the end.
*/
CTrcAdminObj::~CTrcAdminObj()
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
    m_eTrcDetailLevelMethodCalls = static_cast<EMethodTraceDetailLevel>(0);
    m_eTrcDetailLevelRuntimeInfo = static_cast<ELogDetailLevel>(0);
    //m_strDataFilter;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns a reference to the index tree the trace admin object belongs to.

    The trace admin object is a leave entry in the index tree.

    @return Reference to the admin objects index tree.
*/
CIdxTreeTrcAdminObjs* CTrcAdminObj::getTraceAdminObjIdxTree()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CIdxTreeTrcAdminObjs*>(m_pTree);
}

//------------------------------------------------------------------------------
/*! @brief Returns a reference to the trace server the trace admin object belongs to.

    The trace server creates the trace admin object index tree and the
    admin object is a leave entry in the index tree.

    @return Reference to the trace server hosting the trace admin object index
            tree the trace admin object belongs to.
*/
CTrcServer* CTrcAdminObj::getTraceServer()
//------------------------------------------------------------------------------
{
    CTrcServer* pTrcServer = nullptr;

    CIdxTreeTrcAdminObjs* pTree = getTraceAdminObjIdxTree();

    if( pTree != nullptr )
    {
        QObject* pObjParent = pTree->parent();

        if( pObjParent != nullptr )
        {
            // May be nullptr if the parent object is not the trace server but
            // e.g. the trace client.
            pTrcServer = dynamic_cast<CTrcServer*>(pObjParent);
        }
    }
    return pTrcServer;

} // getTraceServer

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the name space of the module, class or instance referencing the
           trace admin object.

    Usually this is equal to the parent path of the parent branch of the leave entry.
    E.g. if the trace admin object is used to control the tracing state of
    instance "ZS::Diagram::CWdgtDiagram::Analyzer" the name space would
    be "ZS::Diagram".

    If the trace admin object is used to control the tracing state of
    a class or module like "ZS::Diagram::CWdgtDiagram" the name space
    equals the parent path of the leave entry.

    @return Name space of the module, class or instance referencing the
            trace admin object.
*/
QString CTrcAdminObj::getNameSpace() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strNameSpace;
}

//------------------------------------------------------------------------------
/*! @brief Returns the class name of the module, class or instance referencing the
    trace admin object.

    Usually this is the name of the parent branch of the leave entry.
    E.g. if the trace admin object is used to control the tracing state of
    instance "ZS::Diagram::CWdgtDiagram::Analyzer" the class name would
    be "CWdgtDiagram".

    If the trace admin object is used to control the tracing state of
    a class or module like "ZS::Diagram::CWdgtDiagram" the class name
    equals the name of leave entry.

    @return Class bame of the module, class or instance referencing the
            trace admin object.
*/
QString CTrcAdminObj::getClassName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strClassName;
}

//------------------------------------------------------------------------------
/*! @brief If the instance using the trace admin object is renamed and the
           trace admin object is used to control trace outputs of this instance
           the new name of the instance got to be forwarded to the trace admin
           object so that following trace outputs are using the new object name.

    @note Don't mangle this with the QObject::setObjectName as the trace admin
          object is derived from QObject but does not use the object name itself.

    @param i_strObjName [in] Object name (e.g. "ZSTrcClient")
        Object name of the instance creating this trace admin object.
        Please note that the object name may also contain node separators
        to define logically grouped objects.
*/
void CTrcAdminObj::setObjectName( const QString& i_strObjName )
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
           by the trace admin object.

    The object name might be the same as the name of the leave entry.
    But please note that the object name may also contain node separators
    to define logically grouped objects.

    E.g. if the trace admin object is used to control the tracing state of
    instance "RF::Analyzer" the object name would be this instance name and is
    different from the name of the leave which would be "Analyzer".

    If the trace admin object is used to control the tracing state of a class
    or module like "ZS::Diagram::CWdgtDiagram" the object name is empty.

    @note Don't mangle this with the QObject::objectName as the trace admin
          object is derived from QObject but does not use the object name itself.

    @return Object name of the module, class or instance referencing the
            trace admin object.
*/
QString CTrcAdminObj::getObjectName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the name of the thread in which context the trace admin object
           was created.

    It should normally not be necessary to use this method within the application
    to be traced. Only when sending the information about the trace admin object
    from the trace server to the method trace client the client needs to call
    this method.

    @param i_strThreadName [in]
        Name of the thread in which context the trace admin object was created.
*/
void CTrcAdminObj::setObjectThreadName( const QString& i_strThreadName )
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
/*! @brief Returns the name of the thread in which context the trace admin object
           was created.

    @return Name of the the thread in which context the trace admin object was created.
*/
QString CTrcAdminObj::getObjectThreadName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strObjThreadName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Locks the trace admin object by incrementing the lock count.

    The method tracer uses this method so that the trace admin object will not
    be deleted as long as the method tracer is alive.

    @return Current lock count.
*/
int CTrcAdminObj::lock()
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

    Only if the lock counter reaches 0 the admin object is really unlocked.

    @note When unlocking the admin object the "deleteOnUnlock" flag must be
          checked. If true and the reference counter is 0 the trace admin
          object should be deleted to reduce the number of no longer used trace
          admin objects (as the object referencing the object has been renamed).
          To delete the object release the instance by invoking
          CTrcServer::ReleaseTraceAdminObj.

    @return Current lock count.
*/
int CTrcAdminObj::unlock()
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
/*! Returns whether the admin object is locked.

    @return true if the object is locked, false otherwise.
*/
bool CTrcAdminObj::isLocked() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_iLockCount > 0);
}

//------------------------------------------------------------------------------
/*! Returns the current lock count.

    @return Current lock count.
*/
int CTrcAdminObj::getLockCount() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_iLockCount;
}

//------------------------------------------------------------------------------
/*! @brief Sets the flag that the admin object is no longer needed when unlocked.

    On renaming the object and if the reference counter reaches 0 the
    trace admin object should be deleted. But it cannot be deleted if the
    method tracer is active and still uses it. If unlocked the trace admin
    object will be marked as to be deleted as it is no longer used.

    @param i_bDelete [in]
        Flag to indicate whether the object can be deleted if unlocked
        (and the reference counter is still 0).
*/
void CTrcAdminObj::setDeleteOnUnlock( bool i_bDelete )
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
/*! @brief Returns the flag whether the admin object may be deleted if
           unlocked and no longer used.

    To delete the trace admin object call CTrcServer::ReleaseTraceAdminObj.

    @return true if the flag is set, false otherwise.
*/
bool CTrcAdminObj::deleteOnUnlock() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_iLockCount <= 0) && (m_iRefCount <= 0) && m_bDeleteOnUnlock;
}

//------------------------------------------------------------------------------
/*! @brief Increments the reference counter.

    Normally it should never be necessary to call this method directly.
    Only the trace server should use this method on each call of "getTraceAdminObj".

    Usually trace admin objects are only referenced by one specific module,
    class or instance of a class to control the detail level of method
    trace outputs. In certain circumstances or in case of a copy and paste
    error the same trace admin object may be referenced by several modules,
    classes or instances of classes. If so the reference counter may become
    greater than 1.

    @return Current reference counter.
*/
int CTrcAdminObj::incrementRefCount()
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
    Only the trace server should use this method on each call of "releaseTraceAdminObj".

    Usually trace admin objects are only referenced by one specific module,
    class or instance of a class to control the detail level of method
    trace outputs. In certain circumstances or in case of a copy and paste
    error the same trace admin object may be referenced by several modules,
    classes or instances of classes. If so the reference counter may become
    greater than 1.

    @return Current reference counter.
*/
int CTrcAdminObj::decrementRefCount()
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
    to be traced. Only when sending the information about the trace admin object
    from the trace server to the method trace client the client needs to call
    this method.

    @param i_iRefCount [in] Reference count to be set.
*/
void CTrcAdminObj::setRefCount( int i_iRefCount )
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
int CTrcAdminObj::getRefCount() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_iRefCount;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Enables trace output for this object.

    Tracing cannot only be controlled via the detail level but tracing can
    also be enabled or disabled by this flag. This is useful if a group of
    objects belonging to a namespace should be temporarily disabled and enabled
    later on restoring the previous detail level.

    @param i_enabled [in] Flag to enable or disable method trace output.
                          - On ... tracing is enabled
                          - Off .. tracing is disabled
*/
void CTrcAdminObj::setEnabled( EEnabled i_enabled )
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
/*! @brief Returns whether tracing is enabled or disabled.

    @return Flag indicating whether method trace output is enabled or disabled.
*/
EEnabled CTrcAdminObj::getEnabled() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_enabled;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether tracing is enabled.

    @return Flag indicating whether method trace output is enabled or disabled.
*/
bool CTrcAdminObj::isEnabled() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_enabled == EEnabled::Yes);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the detail level of trace output for this object.

    If set to None method trace outputs are disabled.
    Higher detail levels include lower detail levels.

    @param i_eDetailLevel [in] Detail level.
*/
void CTrcAdminObj::setMethodCallsTraceDetailLevel( EMethodTraceDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_eTrcDetailLevelMethodCalls != i_eDetailLevel )
    {
        m_eTrcDetailLevelMethodCalls = i_eDetailLevel;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the detail level of trace output for this object.

    @return Detail level.
*/
EMethodTraceDetailLevel CTrcAdminObj::getMethodCallsTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_eTrcDetailLevelMethodCalls;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether tracing is active.

    The method checks both the enabled flag and compares the detail level passed
    as argument with the current detail level set at the trace admin object.

    Tracing is active if
    - tracing is enabled and
    - if the passed detail level is equal or greater than the trace admin
      objects current detail level and
    - if the current trace detail level is not 0 (None).

    Example:

        CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CWdgtDiagram", "Analyzer");

        bool bTracingActive;

        pTrcAdminObj->setEnabled(EEnabledOn);
        pTrcAdminObj->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevel::EnterLeave);

        bTracingActive = pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave); .. returns true
        bTracingActive = pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::MethodArgs); .. returns false

    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return Flag indicating whether method trace output is active or not.
*/
bool CTrcAdminObj::areMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bActive = false;

    if( i_eFilterDetailLevel > EMethodTraceDetailLevel::None )
    {
        if( m_enabled == EEnabled::Yes && m_eTrcDetailLevelMethodCalls >= i_eFilterDetailLevel )
        {
            bActive = true;
        }
    }
    return bActive;
}

//------------------------------------------------------------------------------
/*! @brief Sets the detail level of trace output for this object.

    If set to None method trace outputs are disabled.
    Higher detail levels include lower detail levels.

    @param i_eDetailLevel [in] Detail level.
*/
void CTrcAdminObj::setRuntimeInfoTraceDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_eTrcDetailLevelRuntimeInfo != i_eDetailLevel )
    {
        m_eTrcDetailLevelRuntimeInfo = i_eDetailLevel;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the detail level of trace output for this object.

    @return Detail level.
*/
ELogDetailLevel CTrcAdminObj::getRuntimeInfoTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_eTrcDetailLevelRuntimeInfo;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether tracing is activated or disactived.

    The method checks both the enabled flag and compares the detail level passed
    as argument with the current detail level set at the trace admin object.

    Tracing is active if
    - tracing is enabled and
    - if the passed detail level is equal or greater than the trace admin
      objects current detail level and
    - if the current trace detail level is not 0 (None).

    Example:

        CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CWdgtDiagram", "Analyzer");

        bool bTracingActive;

        pTrcAdminObj->setEnabled(EEnabledOn);
        pTrcAdminObj->setDetailLevel(ELogDetailLevel::Debug);

        bTracingActive = pTrcAdminObj->isActive(ELogDetailLevel::InfoVerbose); .. returns true
        bTracingActive = pTrcAdminObj->isActive(ELogDetailLevel::DebugDetailed);  .. returns false

    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return Flag indicating whether method trace output is active or not.
*/
bool CTrcAdminObj::isRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bActive = false;

    if( i_eFilterDetailLevel > ELogDetailLevel::None )
    {
        if( m_enabled == EEnabled::Yes && m_eTrcDetailLevelRuntimeInfo >= i_eFilterDetailLevel )
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
/*! @brief Sets the trace data filter as a regular expression.

    The filter is a regular expression which allows to define a positive
    pattern where only the data will be traced which mets the expression or a
    negative pattern which suppresses trace outputs if the filter does not match.

    Please see the description of QRegExp class for more details on how
    to define regular expressions. Here are just a view (but often used)
    simple examples.

    Simple Examples (output trace if string contains sub string):

    - Check if the string "abc" is contained:

          m_pTrcAdminObj->setTraceDataFilter("abc");
          m_pTrcAdminObj->isTraceDataSuppressedByFilter("1 abc bca cab") // returns false
          m_pTrcAdminObj->isTraceDataSuppressedByFilter("2 xyz yzx zxy") // returns true

    - Check if the strings "def" or "uvw" are contained:

          m_pTrcAdminObj->setTraceDataFilter("def|uvw");
          m_pTrcAdminObj->isTraceDataSuppressedByFilter("3 def efd fde") // returns true
          m_pTrcAdminObj->isTraceDataSuppressedByFilter("4 uvw vwu wuv") // returns true

    More Complex Examples (output trace if string does not contain sub string):

    - Check if the string "ghi" is NOT contained:

      Suppressing a string if a string does not contain a substring is supposed
      to be seldom used. Its complicated to setup a regular expression for this.
      The following is an example on how this could be done.

          m_pTrcAdminObj->setTraceDataFilter("^((?!ghi).)*$");
          m_pTrcAdminObj->isTraceDataSuppressedByFilter("5 ghi hig igh") // returns true
          m_pTrcAdminObj->isTraceDataSuppressedByFilter("6 rst str trs") // returns false

    @param i_strFilter [in] Filter as regular expression.

    @note If a string should be suppressed if it does not contain a substring
          more simple to apply this would be an additional method or provide
          a flag like match or dontMatch.
*/
void CTrcAdminObj::setTraceDataFilter( const QString& i_strFilter )
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
/*! @brief Returns the trace data filter.

    @return Trace data filter (string containing a regular expression).
*/
QString CTrcAdminObj::getTraceDataFilter() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strDataFilter;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether given trace data should be suppressed by the data filter.

    @param i_strTraceData [in]
        Trace data to be checked against the filter string.

    @return true if the passed trace data should be suppressed, false otherwise.
*/
bool CTrcAdminObj::isTraceDataSuppressedByFilter( const QString& i_strTraceData ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bSuppressed = false;

    if( !m_strDataFilter.isEmpty() )
    {
        QRegExp rx(m_strDataFilter);

        if( rx.indexIn(i_strTraceData) < 0 )
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
bool CTrcAdminObj::blockTreeEntryChangedSignal( bool i_bBlock )
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

} // blockTreeEntryChangedSignal

//------------------------------------------------------------------------------
/*! @brief Returns true if signals are blocked; otherwise returns false.

    Signals are not blocked by default.

    @return true if signals are blocked, false otherwise.
*/
bool CTrcAdminObj::isTreeEntryChangedSignalBlocked() const
//------------------------------------------------------------------------------
{
    return (m_iBlockTreeEntryChangedSignalCounter > 0);
}


/*******************************************************************************
class CTrcAdminObjRefAnchor
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the class.

    @param i_strNameSpace [in] Namespace of the class to be traced.
    @param i_strClassName [in] Class name of the class to be traced.
*/
CTrcAdminObjRefAnchor::CTrcAdminObjRefAnchor(
    const QString& i_strNameSpace,
    const QString& i_strClassName ) :
//------------------------------------------------------------------------------
    m_mtx(),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_pTrcAdminObj(nullptr),
    m_idxInTree(-1),
    m_iRefCount(0)
{
}

//------------------------------------------------------------------------------
/*! @brief Destroys the instance.
*/
CTrcAdminObjRefAnchor::~CTrcAdminObjRefAnchor()
//------------------------------------------------------------------------------
{
    //m_mtx;
    //m_strNameSpace;
    //m_strClassName;
    m_pTrcAdminObj = nullptr;
    m_idxInTree = 0;
    m_iRefCount = 0;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Allocates the trace admin object.

    If the trace admin object does not yet belong to the trace server, the object
    will be added. Otherwise the reference to the already existing index tree entry
    is returned and the reference counter is incremented.

    The index tree entry is locally stored to speed up further access to the
    trace admin object.

    The pointer to the trace admin object is kept until the program exits and
    the reference anchor is destroyed or if the trace admin object is destroyed.
*/
void CTrcAdminObjRefAnchor::allocTrcAdminObj()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_iRefCount > 0 )
    {
        if( m_pTrcAdminObj == nullptr )
        {
            QString strExc = "RefCount is " + QString::number(m_iRefCount) + " but pointer to trace admin object has not been stored";
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
        // Also the reference counter of the trace admin object is incremented.
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(m_idxInTree);
    }
    else if( m_pTrcAdminObj == nullptr )
    {
        // Also the reference counter of the trace admin object is incremented.
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(m_strNameSpace, m_strClassName, "");

        if( m_pTrcAdminObj != nullptr )
        {
            // The pointer to the trace admin object is kept until the program is exited
            // and the reference anchor is destroyed or if the trace admin object is destroyed.
            if( !QObject::connect(
                /* pObjSender   */ m_pTrcAdminObj,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTrcAdminObjDestroyed(QObject*)),
                /* cnctType     */ Qt::DirectConnection ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
    }
    else // if( m_pTrcAdminObj != nullptr )
    {
        // Also the reference counter of the trace admin object is incremented.
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(m_strNameSpace, m_strClassName, "");
    }

    if( m_pTrcAdminObj != nullptr )
    {
        m_idxInTree = m_pTrcAdminObj->indexInTree();
    }

    m_iRefCount++;

} // allocTrcAdminObj

//------------------------------------------------------------------------------
/*! @brief Releases the trace admin object.

    The trace admin object is not deleted but just a reference counter is
    decremented. Even if the reference counter reaches 0 the trace admin object
    stays alive to allow modifying the properties like the detail level.

     @note The pointer to the trace admin object is kept until the program exits
           and the reference anchor is destroyed or if the trace admin object is
           destroyed.
*/
void CTrcAdminObjRefAnchor::releaseTrcAdminObj()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_iRefCount <= 0 )
    {
        QString strExc = "Cannot release the trace admin object if RefCount is " + QString::number(m_iRefCount);
        throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
    }
    if( m_pTrcAdminObj == nullptr )
    {
        QString strExc = "RefCount is " + QString::number(m_iRefCount) + " but pointer to trace admin object has not been stored";
        throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
    }
    if( m_pTrcAdminObj->getRefCount() < m_iRefCount )
    {
        QString strExc = "RefCount is " + QString::number(m_iRefCount) + " is greater than ref counter of trace admin object";
        throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, strExc);
    }

    // The pointer is kept and will only be reset to nullptr if the
    // trace admin object is going to be destroyed.
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_iRefCount--;
}

//------------------------------------------------------------------------------
/*! @brief Returns the reference to the trace admin object.

    @return Reference to trace admin object. Might be nullptr if the trace admin
            object has not yet been allocated.
*/
CTrcAdminObj* CTrcAdminObjRefAnchor::trcAdminObj()
//------------------------------------------------------------------------------
{
    return m_pTrcAdminObj;
}

//------------------------------------------------------------------------------
/*! @brief Sets the trace detail level.

    This method has no effect if the trace admin object has not yet been allocated.

    @param i_eTrcDetailLevel [in] Trace detail level.
*/
void CTrcAdminObjRefAnchor::setMethodCallsTraceDetailLevel( EMethodTraceDetailLevel i_eTrcDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObj->setMethodCallsTraceDetailLevel(i_eTrcDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active for the given filter detail level.

    @param i_eFilterDetailLevel [in]
        Trace detail level which should be checked. If the trace admin objects
        detail level is not 0 (None) and is greater or equal the filter level
        tracing is active.

    @return true if tracing is active, false otherwise.
*/
bool CTrcAdminObjRefAnchor::areMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    bool bActive = false;
    if( m_pTrcAdminObj != nullptr )
    {
        bActive = m_pTrcAdminObj->areMethodCallsActive(i_eFilterDetailLevel);
    }
    return bActive;
}

//------------------------------------------------------------------------------
/*! @brief Sets the trace detail level.

    This method has no effect if the trace admin object has not yet been allocated.

    @param i_eTrcDetailLevel [in] Trace detail level.
*/
void CTrcAdminObjRefAnchor::setRuntimeInfoTraceDetailLevel( ELogDetailLevel i_eTrcDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObj->setRuntimeInfoTraceDetailLevel(i_eTrcDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active for the given filter detail level.

    @param i_eFilterDetailLevel [in]
        Trace detail level which should be checked. If the trace admin objects
        detail level is not 0 (None) and is greater or equal the filter level
        tracing is active.

    @return true if tracing is active, false otherwise.
*/
bool CTrcAdminObjRefAnchor::isRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    bool bActive = false;
    if( m_pTrcAdminObj != nullptr )
    {
        bActive = m_pTrcAdminObj->isRuntimeInfoActive(i_eFilterDetailLevel);
    }
    return bActive;
}

/*==============================================================================
private slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Slot which is called if the trace admin object is going to be destroyed.

    @param i_pTrcAdminObj [in] Reference to trace admin object which will be destroyed.
*/
void CTrcAdminObjRefAnchor::onTrcAdminObjDestroyed( QObject* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&m_mtx);

    if( m_pTrcAdminObj == i_pTrcAdminObj )
    {
        m_pTrcAdminObj = nullptr;
        m_idxInTree = -1;
        m_iRefCount = 0;
    }
}


/*******************************************************************************
class CTrcAdminObjRefGuard
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the trace admin object reference guard.

    @param i_pRefAnchor [in] Pointer to trace admin object reference anchor which
        should be guarded.
*/
CTrcAdminObjRefGuard::CTrcAdminObjRefGuard(CTrcAdminObjRefAnchor* i_pRefAnchor) :
//------------------------------------------------------------------------------
    m_pRefAnchor(i_pRefAnchor)
{
    m_pRefAnchor->allocTrcAdminObj();
}

//------------------------------------------------------------------------------
/*! @brief Destroys the trace admin reference guard instance.
*/
CTrcAdminObjRefGuard::~CTrcAdminObjRefGuard()
//------------------------------------------------------------------------------
{
    m_pRefAnchor->releaseTrcAdminObj();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the pointer to the trace admin object.

    @return Pointer to trace admin object (which may also be nullptr if the
            trace admin object has not yet been allocated).
*/
CTrcAdminObj* CTrcAdminObjRefGuard::trcAdminObj()
//------------------------------------------------------------------------------
{
    CTrcAdminObj* pTrcAdminObj = nullptr;
    if( m_pRefAnchor != nullptr )
    {
        pTrcAdminObj = m_pRefAnchor->trcAdminObj();
    }
    return pTrcAdminObj;
}

//------------------------------------------------------------------------------
/*! @brief Sets the trace detail level of the trace admin object.

    This method has no effect if the trace admin object has not yet been allocated.

    @param i_iTrcDetailLevel [in] Trace detail level.
*/
void CTrcAdminObjRefGuard::setMethodCallsTraceDetailLevel(EMethodTraceDetailLevel i_eTrcDetailLevel)
//------------------------------------------------------------------------------
{
    if( m_pRefAnchor != nullptr )
    {
        m_pRefAnchor->setMethodCallsTraceDetailLevel(i_eTrcDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active for the given filter detail level.

    @param i_eFilterDetailLevel [in]
        Trace detail level which should be checked. If the trace admin objects
        detail level is not 0 (None) and is greater or equal the filter level
        tracing is active.

    @return true if tracing is active, false otherwise.
*/
bool CTrcAdminObjRefGuard::areMethodCallsActive(EMethodTraceDetailLevel i_eFilterDetailLevel) const
//------------------------------------------------------------------------------
{
    bool bActive = false;
    if( m_pRefAnchor != nullptr )
    {
        bActive = m_pRefAnchor->areMethodCallsActive(i_eFilterDetailLevel);
    }
    return bActive;
}

//------------------------------------------------------------------------------
/*! @brief Sets the trace detail level of the trace admin object.

    This method has no effect if the trace admin object has not yet been allocated.

    @param i_eTrcDetailLevel [in] Trace detail level.
*/
void CTrcAdminObjRefGuard::setRuntimeInfoTraceDetailLevel(ELogDetailLevel i_eTrcDetailLevel)
//------------------------------------------------------------------------------
{
    if( m_pRefAnchor != nullptr )
    {
        m_pRefAnchor->setRuntimeInfoTraceDetailLevel(i_eTrcDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active for the given filter detail level.

    @param i_eFilterDetailLevel [in]
        Trace detail level which should be checked. If the trace admin objects
        detail level is not 0 (None) and is greater or equal the filter level
        tracing is active.

    @return true if tracing is active, false otherwise.
*/
bool CTrcAdminObjRefGuard::isRuntimeInfoActive(ELogDetailLevel i_eFilterDetailLevel) const
//------------------------------------------------------------------------------
{
    bool bActive = false;
    if( m_pRefAnchor != nullptr )
    {
        bActive = m_pRefAnchor->isRuntimeInfoActive(i_eFilterDetailLevel);
    }
    return bActive;
}

//------------------------------------------------------------------------------
/*! Private method throwing an exception if called.

    The object name of a trace admin object is the object name of the instance
    creating the trace admin object. For the sake of clarification this method
    has been implemented as a private method throwing an exception if called).

    @return Nothing as method throws an exception
*/
QString CTrcAdminObj::objectName() const
//------------------------------------------------------------------------------
{
    throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    return getObjectName();
}
