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
using namespace ZS::Trace;


/*******************************************************************************
class CTrcAdminObj : public QObject, public CIdxTreeEntry
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor (trace admin objects may only be created by the trace server)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs a trace admin object instance.

    Trace admin objects are leaves in the trace admin object index tree.

    Usually trace admin object instances are created by the trace method server
    on calling the static "getTraceAdminObj" methods.

    Trace admin objects will also be created if the trace server reads an trace
    admin object XML file containing the settings for all trace admin objects
    which are currently part of the trace admin object index tree. This are
    usually all trace admin object which have been added to the index tree
    since program start. Recalling the settings of the trace admin objects is
    only possible if the trace servers "saveAdminObjs" method is called which
    should happen just before destroying the trace server by the application.
    Right after creating the trace method server the method "recallAdminObjs"
    should be called to restore the last recent settings of the trace admin objects.

    Trace admin objects live as long as the trace admin object index tree lives.
    Once created and added to the trace admin object index tree only the reference
    counter is incremented and decremented. If the reference counter becomes 0
    the trace admin object will not be deleted. Trace admin object will only be
    deleted by the destructor of the trace admin object index tree.

    If a module, class or instance of a class using the trace admin object is
    destroyed the reference to the trace admin object must be releasing by invoking
    the trace method servers "releaseTraceAdminObj" method. After releasing the
    trace admin object the states and attributes of the admin object instance may
    still be changed. Meansing that e.g. the detail level may be changed and/or
    tracing may be enabled or disabled even if no module, class or instance of
    a class is referencing the admin object anymore.

    If the same instance is referenced by different threads on calling
    "TrcServerBase::getTraceAdminObj" the already created trace admin object is returned
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
    m_iRefCount(0),
    m_enabled(EEnabled::Yes),
    m_iTrcDetailLevel(ETraceDetailLevelNone)
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
/*! Destroys the trace admin object instance removing itself from the index tree.

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

    if( m_iRefCount != 0 )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), keyInTree(), "dtor",
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
            /* errSource         */ nameSpace(), className(), keyInTree(), "dtor",
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
    //m_strObjThreadName;
    m_iRefCount = 0;
    m_enabled = static_cast<EEnabled>(0);
    m_iTrcDetailLevel = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns a reference to the index tree the trace admin object belongs to.

    The trace admin object is a leave entry in the index tree.

    /return Reference to the admin objects index tree.
*/
CIdxTreeTrcAdminObjs* CTrcAdminObj::getTraceAdminObjIdxTree()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CIdxTreeTrcAdminObjs*>(m_pTree);
}

//------------------------------------------------------------------------------
/*! Returns a reference to the trace server the trace admin object belongs to.

    The trace server creates the trace admin object index tree and the
    admin object is a leave entry in the index tree.

    /return Reference to the trace server hosting the trace admin object index
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
            // May be nullptr if the parent object is not a trace server but
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
/*! Returns the name space of the module, class or instance referencing the
    trace admin object.

    Usually this is equal to the parent path of the parent branch of the leave entry.
    E.g. if the trace admin object is used to control the tracing state of
    instance "ZS::Diagram::CWdgtDiagram::Analyzer" the name space would
    be "ZS::Diagram".

    If the trace admin object is used to control the tracing state of
    a class or module like "ZS::Diagram::CWdgtDiagram" the name space
    equals the parent path of the leave entry.

    \return Name space of the module, class or instance referencing the
    trace admin object.
*/
QString CTrcAdminObj::getNameSpace() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strNameSpace;
}

//------------------------------------------------------------------------------
/*! Returns the class name of the module, class or instance referencing the
    trace admin object.

    Usually this is the name of the parent branch of the leave entry.
    E.g. if the trace admin object is used to control the tracing state of
    instance "ZS::Diagram::CWdgtDiagram::Analyzer" the class name would
    be "CWdgtDiagram".

    If the trace admin object is used to control the tracing state of
    a class or module like "ZS::Diagram::CWdgtDiagram" the class name
    equals the name of leave entry.

    \return Class bame of the module, class or instance referencing the
    trace admin object.
*/
QString CTrcAdminObj::getClassName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_strClassName;
}

//------------------------------------------------------------------------------
/*! Returns the name of the object (instance) which should be controlled
    by the trace admin object.

    Usually this is the name of the leave entry.
    E.g. if the trace admin object is used to control the tracing state of
    instance "ZS::Diagram::CWdgtDiagram::Analyzer" the object name would
    be "Analyzer".

    If the trace admin object is used to control the tracing state of a class
    or module like "ZS::Diagram::CWdgtDiagram" the object name is empty.

    \return Object name of the module, class or instance referencing the
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
/*! Sets the name of the thread in which context the trace admin object is created.

    It should normally not be necessary to use this method within the application
    to be traced. Only when sending the information about the trace admin object
    from the trace server to the method trace client the client needs to call
    this method.
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
} // setObjectThreadName

//------------------------------------------------------------------------------
/*! Returns the name of the thread in which context the trace admin object is created.

    \return Name of the the thread in which context the trace admin object is created.
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
/*! Increments the reference counter.

    Normally it should never be necessary to call this method directly.
    Only the trace server should use this method on each call of "getTraceAdminObj".

    Usually trace admin objects are only referenced by one specific module,
    class or instance of a class to control the detail level of method
    trace outputs. In certain circumstances or in case of a copy and paste
    error the same trace admin object may be referenced by several modules,
    classes or instances of classes. If so the reference counter may become
    greater than 1.

    /return Current reference counter.
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

} // incrementRefCount

//------------------------------------------------------------------------------
/*! Decrements the reference counter.

    Normally it should never be necessary to call this method directly.
    Only the trace server should use this method on each call of "releaseTraceAdminObj".

    Usually trace admin objects are only referenced by one specific module,
    class or instance of a class to control the detail level of method
    trace outputs. In certain circumstances or in case of a copy and paste
    error the same trace admin object may be referenced by several modules,
    classes or instances of classes. If so the reference counter may become
    greater than 1.

    /return Current reference counter.
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

} // decrementRefCount

//------------------------------------------------------------------------------
/*! Sets the reference counter of the object.

    It should normally not be necessary to use this method within the application
    to be traced. Only when sending the information about the trace admin object
    from the trace server to the method trace client the client needs to call
    this method.
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

} // setRefCount

//------------------------------------------------------------------------------
/*! Returns the current reference counter.

    /return Current reference counter.
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
/*! Enables trace output for this object.

    Tracing cannot only be controlled via the detail level but tracing can
    also be enabled or disabled by this flag. This is useful if a group of
    objects belonging to a namespace should be temporarily disabled and enabled
    later on restoring the previous detail level.

    /param i_enabled [in] Flag to enable or disable method trace output.
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

} // setEnabled

//------------------------------------------------------------------------------
/*! Returns whether tracing is enabled or disabled.

    /return Flag indicating whether method trace output is enabled or disabled.
*/
EEnabled CTrcAdminObj::getEnabled() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_enabled;
}

//------------------------------------------------------------------------------
/*! Returns whether tracing is enabled.

    /return Flag indicating whether method trace output is enabled or disabled.
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
/*! Sets the detail level of trace output for this object.

    If set to None method trace outputs are disabled.
    Higher detail levels include lower detail levels.

    There are predefined trace detail levels (see ETraceDetailLevel).

    Please note that this enum is only a suggestion for trace detail levels
    which can be used. The detail level of the trace admin object is an integer
    type and user defined values can be used if necessary. To avoid type casts
    requested by the compiler this enum is not a class enum definition.

    /param i_iDetailLevel [in] Integer value specifying the detail level.
*/
void CTrcAdminObj::setTraceDetailLevel( int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_iTrcDetailLevel != i_iDetailLevel )
    {
        m_iTrcDetailLevel = i_iDetailLevel;

        if( m_pTree != nullptr )
        {
            if( !isTreeEntryChangedSignalBlocked() ) m_pTree->onTreeEntryChanged(this);
        }
    }

} // setDetailLevel

//------------------------------------------------------------------------------
/*! Returns the detail level of trace output for this object.

    /return Integer value specifying the detail level.
*/
int CTrcAdminObj::getTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_iTrcDetailLevel;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns whether tracing is activated or disactived.

    The method checks both the enabled flag and compares the detail level passed
    as argument to the method with the current detail level set at the trace
    admin object.

    Tracing is active if
    - tracing is enabled and
    - if the passed detail level is equal or greater than the trace admin
      objects current detail level and
    - if the current trace detail level is not 0 (None).

    Example:

        CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CWdgtDiagram", "Analyzer");

        bool bTracingActive;

        pTrcAdminObj->setEnabled(EEnabledOn);
        pTrcAdminObj->setDetailLevel(ETraceDetailLevelMethodCalls);

        bTracingActive = pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls); .. returns true
        bTracingActive = pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs);  .. returns false

    /param i_iFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    /return Flag indicating whether method trace output is active or not.
*/
bool CTrcAdminObj::isActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bActive = false;

    if( i_iFilterDetailLevel > ETraceDetailLevelNone )
    {
        if( m_enabled == EEnabled::Yes && m_iTrcDetailLevel >= i_iFilterDetailLevel )
        {
            bActive = true;
        }
    } // if( i_iDetailLevel > ETraceDetailLevelNone )

    return bActive;

} // isActive

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! If block is true, the treeEntryChanged signal emitted by this object will
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
/*! Returns true if signals are blocked; otherwise returns false.

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
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_idxInTree(-1),
    m_pTrcAdminObj(nullptr)
{
}

//------------------------------------------------------------------------------
/*! @brief Destroys the instance.
*/
CTrcAdminObjRefAnchor::~CTrcAdminObjRefAnchor()
//------------------------------------------------------------------------------
{
    //m_strNameSpace;
    //m_strClassName;
    m_idxInTree = 0;
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Allocates the trace admin object.

    If the trace admin object does not yet belong to the trace server, it will
    be added. Otherwise the reference to the already existing index tree entry
    is return and the ref counter is incremented.

    The index tree entry is locally stored to speed up further access to the
    trace admin object.
*/
void CTrcAdminObjRefAnchor::allocTrcAdminObj()
//------------------------------------------------------------------------------
{
    if( m_idxInTree >= 0 )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(m_idxInTree);
    }

    if( m_pTrcAdminObj == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(m_strNameSpace, m_strClassName, "");
    }

    if( m_pTrcAdminObj != nullptr )
    {
        m_idxInTree = m_pTrcAdminObj->indexInTree();

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

//------------------------------------------------------------------------------
/*! @brief Release the trace admin object.

    The trace admin object is not deleted but just a reference counter is
    decremented. Even if the reference counter reaches 0 the trace admin object
    stays alive to allow modifying the properties like the detail level.
*/
void CTrcAdminObjRefAnchor::releaseTrcAdminObj()
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcAdminObj,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTrcAdminObjDestroyed(QObject*)));
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }
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

    @param i_iTrcDetailLevel [in] Trace detail level.
*/
void CTrcAdminObjRefAnchor::setTraceDetailLevel( int i_iTrcDetailLevel )
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObj->setTraceDetailLevel(i_iTrcDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active for the given filter detail level.

    @param i_iFilterDetailLevel [in]
        Trace detail level which should be checked. If the trace admin objects
        detail level is not 0 (None) and is greater or equal the filter level
        tracing is active.

    @return true if tracing is active, false otherwise.
*/
bool CTrcAdminObjRefAnchor::isActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bActive = false;
    if( m_pTrcAdminObj != nullptr )
    {
        bActive = m_pTrcAdminObj->isActive(i_iFilterDetailLevel);
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
    if( m_pTrcAdminObj == i_pTrcAdminObj )
    {
        m_pTrcAdminObj = nullptr;
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
void CTrcAdminObjRefGuard::setTraceDetailLevel(int i_iTrcDetailLevel)
//------------------------------------------------------------------------------
{
    if( m_pRefAnchor != nullptr )
    {
        m_pRefAnchor->setTraceDetailLevel(i_iTrcDetailLevel);
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active for the given filter detail level.

    @param i_iFilterDetailLevel [in]
        Trace detail level which should be checked. If the trace admin objects
        detail level is not 0 (None) and is greater or equal the filter level
        tracing is active.

    @return true if tracing is active, false otherwise.
*/
bool CTrcAdminObjRefGuard::isActive(int i_iFilterDetailLevel) const
//------------------------------------------------------------------------------
{
    bool bActive = false;
    if( m_pRefAnchor != nullptr )
    {
        bActive = m_pRefAnchor->isActive(i_iFilterDetailLevel);
    }
    return bActive;
}
