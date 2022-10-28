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

#include "ZSSys/ZSSysMutex.h"

#include "ZSSys/ZSSysAux.h"
#ifdef ZS_TRACE_MUTEXES
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
class CMutex : public QMutex
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_mode [in]
        If mode is QMutex::Recursive, a thread can lock the same mutex multiple times
        and the mutex won't be unlocked until a corresponding number of unlock() calls
        have been made. Otherwise a thread may only lock a mutex once.
    @param i_strObjName [in] Descriptive name of the mutex locker.
*/
CMutex::CMutex(QMutex::RecursionMode i_mode, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    QMutex(i_mode),
    m_strObjName(i_strObjName)
    #ifdef ZS_TRACE_MUTEXES
    ,m_eTrcMthFileDetailLevel(EMethodTraceDetailLevel::None),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
    #endif
{
    #ifdef ZS_TRACE_MUTEXES
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), i_strObjName);
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = qMutexRecursionMode2Str(i_mode);
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ strMthInArgs );
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_strObjName [in] Descriptive name of the mutex locker.
*/
CMutex::CMutex(const QString& i_strObjName) :
//------------------------------------------------------------------------------
    QMutex(),
    m_strObjName(i_strObjName)
    #ifdef ZS_TRACE_MUTEXES
    ,m_eTrcMthFileDetailLevel(EMethodTraceDetailLevel::None),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
    #endif
{
    #ifdef ZS_TRACE_MUTEXES
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), i_strObjName);
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_mode [in]
        If mode is QMutex::Recursive, a thread can lock the same mutex multiple times
        and the mutex won't be unlocked until a corresponding number of unlock() calls
        have been made. Otherwise a thread may only lock a mutex once.
    @param i_strObjName [in] Descriptive name of the mutex locker.
    @param i_eTrcMthFileDetailLevel [in]
        If trace outputs should not be forwarded to the trace server but directly
        to a trace method file allocated by the server, this detail level has to
        be to a value greater than None.
*/
CMutex::CMutex(
    QMutex::RecursionMode        i_mode,
    const QString&               i_strObjName,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QMutex(i_mode),
    m_strObjName(i_strObjName)
    #ifdef ZS_TRACE_MUTEXES
    ,m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
    #endif
{
    #ifdef ZS_TRACE_MUTEXES
    QString strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();
    m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
    QString strMthInArgs;
    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = qMutexRecursionMode2Str(i_mode);
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ strMthInArgs );
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_strObjName [in] Descriptive name of the mutex locker.
    @param i_eTrcMthFileDetailLevel [in]
        If trace outputs should not be forwarded to the trace server but directly
        to a trace method file allocated by the server, this detail level has to
        be to a value greater than None.
*/
CMutex::CMutex( const QString& i_strObjName, EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QMutex(),
    m_strObjName(i_strObjName)
    #ifdef ZS_TRACE_MUTEXES
    ,m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
    #endif
{
    #ifdef ZS_TRACE_MUTEXES
    QString strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();
    m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Destroys the mutex.

    For further information please refer to documentation of class QMutex.
*/
CMutex::~CMutex()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );
    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }
    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
        CTrcMthFile::Free(m_pTrcMthFile);
    }
    #endif
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Renames the object.

    @param i_strObjName [in] New object name.
*/
//------------------------------------------------------------------------------
void CMutex::setObjectName(const QString& i_strObjName)
{
    #ifdef ZS_TRACE_MUTEXES
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "setObjectName",
        /* strMthInArgs       */ strMthInArgs );
    #endif

    m_strObjName = i_strObjName;

    #ifdef ZS_TRACE_MUTEXES
    if( m_pTrcAdminObj != nullptr )
    {
        CTrcServer::RenameTraceAdminObj(&m_pTrcAdminObj, objectName());
    }
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Returns the name of the mutex.

    @return Name of the object.
*/
//------------------------------------------------------------------------------
QString CMutex::objectName() const
{
    return m_strObjName;
}

#ifdef ZS_TRACE_MUTEXES
//------------------------------------------------------------------------------
/*! @brief Returns the pointer to the trace method file used by the mutex.

    If the mutex cannot use the trace server for method tracer the trace
    method file is directly used. Otherwise the mutex allocs a trace admin
    object and uses the trace server for method trace logging.

    @return Pointer to trace method file. nullptr if the trace server is used.
*/
CTrcMthFile* CMutex::traceMethodFile()
//------------------------------------------------------------------------------
{
    return m_pTrcMthFile;
}
#endif

#ifdef ZS_TRACE_MUTEXES
//------------------------------------------------------------------------------
/*! @brief Returns the detail level to be used if the mutex cannot use the
           trace server for trace output.

    If the mutex cannot use the trace server for method tracer the trace
    method file is directly used. Otherwise the mutex allocs a trace admin
    object and uses the trace server for method trace logging.

    @return Trace detail level. None if the trace server is used.
*/
EMethodTraceDetailLevel CMutex::traceMethodFileDetailLevel() const
//------------------------------------------------------------------------------
{
    return m_eTrcMthFileDetailLevel;
}
#endif

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CMutex::lock()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "lock",
        /* strMthInArgs       */ "" );
    #endif

    QMutex::lock();
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CMutex::tryLock(int i_timeout_ms)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    QString strMthInArgs;
    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString::number(i_timeout_ms) + " ms";
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "tryLock",
        /* strMthInArgs       */ strMthInArgs );
    #endif

    bool bOk = QMutex::tryLock(i_timeout_ms);

    #ifdef ZS_TRACE_MUTEXES
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CMutex::try_lock()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock",
        /* strAddInfo         */ "" );
    #endif

    bool bOk = QMutex::try_lock();

    #ifdef ZS_TRACE_MUTEXES
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
template <typename Rep, typename Period>
bool CMutex::try_lock_for(std::chrono::duration<Rep, Period> i_duration)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    QString strMthInArgs;
    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString::number(i_duration.count());
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock_for",
        /* strAddInfo         */ "" );
    #endif

    bool bOk = QMutex::try_lock_for(i_duration);

    #ifdef ZS_TRACE_MUTEXES
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
template <typename Clock, typename Duration>
bool CMutex::try_lock_until(std::chrono::time_point<Clock, Duration> i_timePoint)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    QString strMthInArgs;
    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString::number(i_timePoint.time_since_epoch());
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock_until",
        /* strAddInfo         */ strMthInArgs );
    #endif

    bool bOk = QMutex::try_lock_until(i_timePoint);

    #ifdef ZS_TRACE_MUTEXES
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Unlocks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CMutex::unlock()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "unlock",
        /* strAddInfo         */ "" );
    #endif

    return QMutex::unlock();
}

/*==============================================================================
public: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing for the given filter detail level is active.

    If the given filter detail level is not None and greater or equal to the
    trace detail level set at the trace admin object trace output is active.

    @param i_eFilterDetailLevel [in] Filter detail level to be checked.
    @return true if tracing is active, false otherwise
*/
bool CMutex::isMethodTraceActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    #ifdef ZS_TRACE_MUTEXES
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(i_eFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_eTrcMthFileDetailLevel >= i_eFilterDetailLevel )
    {
        bIsActive = true;
    }
    #endif

    return bIsActive;
}

//------------------------------------------------------------------------------
/*! @brief Returns the current active detail level.

    @return Active method trace detail level.
*/
EMethodTraceDetailLevel CMutex::getMethodTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevel::None;

    #ifdef ZS_TRACE_MUTEXES
    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        eDetailLevel = m_eTrcMthFileDetailLevel;
    }
    #endif

    return eDetailLevel;
}


/*******************************************************************************
class CRecursiveMutex : public QRecursiveMutex
*******************************************************************************/

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_strObjName [in] Descriptive name of the mutex locker.
*/
CRecursiveMutex::CRecursiveMutex(const QString& i_strObjName) :
//------------------------------------------------------------------------------
    QRecursiveMutex(),
    m_strObjName(i_strObjName)
    #ifdef ZS_TRACE_MUTEXES
    ,m_eTrcMthFileDetailLevel(EMethodTraceDetailLevel::None),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
    #endif
{
    #ifdef ZS_TRACE_MUTEXES
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), i_strObjName);
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_strObjName [in] Descriptive name of the mutex locker.
    @param i_eTrcMthFileDetailLevel [in]
        If trace outputs should not be forwarded to the trace server but directly
        to a trace method file allocated by the server, this detail level has to
        be to a value greater than None.
*/
CRecursiveMutex::CRecursiveMutex( const QString& i_strObjName, EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QRecursiveMutex(),
    m_strObjName(i_strObjName)
    #ifdef ZS_TRACE_MUTEXES
    ,m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
    #endif
{
    #ifdef ZS_TRACE_MUTEXES
    QString strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();
    m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Destroys the mutex.

    For further information please refer to documentation of class QMutex.
*/
CRecursiveMutex::~CRecursiveMutex()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }
    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
        CTrcMthFile::Free(m_pTrcMthFile);
    }
    m_eTrcMthFileDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;
    #endif
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Renames the object.

    @param i_strObjName [in] New object name.
*/
//------------------------------------------------------------------------------
void CRecursiveMutex::setObjectName(const QString& i_strObjName)
{
    #ifdef ZS_TRACE_MUTEXES
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "setObjectName",
        /* strMthInArgs       */ strMthInArgs );
    #endif

    m_strObjName = i_strObjName;

    #ifdef ZS_TRACE_MUTEXES
    if( m_pTrcAdminObj != nullptr )
    {
        CTrcServer::RenameTraceAdminObj(&m_pTrcAdminObj, objectName());
    }
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Returns the name of the mutex.

    @return Name of the object.
*/
//------------------------------------------------------------------------------
QString CRecursiveMutex::objectName() const
{
    return m_strObjName;
}

#ifdef ZS_TRACE_MUTEXES
//------------------------------------------------------------------------------
/*! @brief Returns the pointer to the trace method file used by the mutex.

    If the mutex cannot use the trace server for method tracer the trace
    method file is directly used. Otherwise the mutex allocs a trace admin
    object and uses the trace server for method trace logging.

    @return Pointer to trace method file. nullptr if the trace server is used.
*/
CTrcMthFile* CRecursiveMutex::traceMethodFile()
//------------------------------------------------------------------------------
{
    return m_pTrcMthFile;
}
#endif

#ifdef ZS_TRACE_MUTEXES
//------------------------------------------------------------------------------
/*! @brief Returns the detail level to be used if the mutex cannot use the
           trace server for trace output.

    If the mutex cannot use the trace server for method tracer the trace
    method file is directly used. Otherwise the mutex allocs a trace admin
    object and uses the trace server for method trace logging.

    @return Trace detail level. None if the trace server is used.
*/
EMethodTraceDetailLevel CRecursiveMutex::traceMethodFileDetailLevel() const
//------------------------------------------------------------------------------
{
    return m_eTrcMthFileDetailLevel;
}
#endif

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CRecursiveMutex::lock()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "lock",
        /* strMthInArgs       */ "" );
    #endif

    QRecursiveMutex::lock();
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CRecursiveMutex::tryLock(int i_timeout_ms)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    QString strMthInArgs;
    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString::number(i_timeout_ms) + " ms";
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "tryLock",
        /* strMthInArgs       */ strMthInArgs );
    #endif

    bool bOk = QRecursiveMutex::tryLock(i_timeout_ms);

    #ifdef ZS_TRACE_MUTEXES
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CRecursiveMutex::try_lock()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock",
        /* strMthInArgs       */ "" );
    #endif

    bool bOk = QRecursiveMutex::try_lock();

    #ifdef ZS_TRACE_MUTEXES
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
template <typename Rep, typename Period>
bool CRecursiveMutex::try_lock_for(std::chrono::duration<Rep, Period> i_duration)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    QString strMthInArgs;
    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString::number(i_duration.count());
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock_for",
        /* strMthInArgs       */ strMthInArgs );
    #endif

    bool bOk = QRecursiveMutex::try_lock_for(i_duration);

    #ifdef ZS_TRACE_MUTEXES
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
template <typename Clock, typename Duration>
bool CRecursiveMutex::try_lock_until(std::chrono::time_point<Clock, Duration> i_timePoint)
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    QString strMthInArgs;
    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString::number(i_timePoint.time_since_epoch());
    }
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock_until",
        /* strMthInArgs       */ strMthInArgs );
    #endif

    bool bOk = QRecursiveMutex::try_lock_until(i_timePoint);

    #ifdef ZS_TRACE_MUTEXES
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    #endif

    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Unlocks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CRecursiveMutex::unlock()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "unlock",
        /* strMthInArgs       */ "" );
    #endif

    return QRecursiveMutex::unlock();
}

/*==============================================================================
public: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing for the given filter detail level is active.

    If the given filter detail level is not None and greater or equal to the
    trace detail level set at the trace admin object trace output is active.

    @param i_eFilterDetailLevel [in] Filter detail level to be checked.
    @return true if tracing is active, false otherwise
*/
bool CRecursiveMutex::isMethodTraceActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    #ifdef ZS_TRACE_MUTEXES
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(i_eFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_eTrcMthFileDetailLevel >= i_eFilterDetailLevel )
    {
        bIsActive = true;
    }
    #endif

    return bIsActive;
}

//------------------------------------------------------------------------------
/*! @brief Returns the current active detail level.

    @return Active method trace detail level.
*/
EMethodTraceDetailLevel CRecursiveMutex::getMethodTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevel::None;

    #ifdef ZS_TRACE_MUTEXES
    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        eDetailLevel = m_eTrcMthFileDetailLevel;
    }
    #endif

    return eDetailLevel;
}

#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)


/*******************************************************************************
class CMutexLocker
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructs a CMutexLocker and locks mutex.

    For further information please refer to documentation of class QMutexLocker.

    The passed mutex will be queried whether method tracing can be done through
    the trace server. If the mutex traces directly to a trace method file the
    locker will also trace to this file using the same detail level as the mutex.

    @param i_pMutex [in]
        Mutex to be locked and unlocked if the dtor of the locker is called.
        nullptr may be passed. In that case the locker does nothing.
*/
CMutexLocker::CMutexLocker(CMutex* i_pMutex) :
//------------------------------------------------------------------------------
    m_pMtx(i_pMutex)
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    ,m_pRecursiveMtx(nullptr)
    #endif
    #ifdef ZS_TRACE_MUTEXES
    ,m_eTrcMthFileDetailLevel(EMethodTraceDetailLevel::None),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
    #endif
{
    if( m_pMtx != nullptr )
    {
        #ifdef ZS_TRACE_MUTEXES
        m_eTrcMthFileDetailLevel = m_pMtx->traceMethodFileDetailLevel();
        m_pTrcMthFile = m_pMtx->traceMethodFile();
        if( m_pTrcMthFile == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
        }
        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ NameSpace(),
            /* strClassName       */ ClassName(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "ctor",
            /* strMthInArgs       */ "" );
        #endif

        m_pMtx->lock();
    }
}

//------------------------------------------------------------------------------
/*! @brief Constructs a CMutexLocker and locks mutex.

    For further information please refer to documentation of class QMutexLocker.

    The passed mutex will be queried whether method tracing can be done through
    the trace server. If the mutex traces directly to a trace method file the
    locker will also trace to this file using the same detail level as the mutex.

    @param i_pMutex [in]
        Mutex to be locked and unlocked if the dtor of the locker is called.
        nullptr may be passed. In that case the locker does nothing.
*/
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
CMutexLocker::CMutexLocker(CRecursiveMutex* i_pMutex) :
//------------------------------------------------------------------------------
    m_pMtx(nullptr),
    m_pRecursiveMtx(i_pMutex)
    #ifdef ZS_TRACE_MUTEXES
    ,m_eTrcMthFileDetailLevel(EMethodTraceDetailLevel::None),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
    #endif
{
    if( m_pRecursiveMtx != nullptr )
    {
        #ifdef ZS_TRACE_MUTEXES
        m_eTrcMthFileDetailLevel = m_pRecursiveMtx->traceMethodFileDetailLevel();
        m_pTrcMthFile = m_pRecursiveMtx->traceMethodFile();
        if( m_pTrcMthFile == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
        }
        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ NameSpace(),
            /* strClassName       */ ClassName(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "ctor",
            /* strMthInArgs       */ "" );
        #endif

        m_pRecursiveMtx->lock();
    }
}
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)

//------------------------------------------------------------------------------
/*! @brief Destroys the mutex.

    For further information please refer to documentation of class QMutex.
*/
CMutexLocker::~CMutexLocker()
//------------------------------------------------------------------------------
{
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    if( m_pMtx != nullptr || m_pRecursiveMtx != nullptr )
    #else
    if (m_pMtx != nullptr)
    #endif
    {
        #ifdef ZS_TRACE_MUTEXES
        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ NameSpace(),
            /* strClassName       */ ClassName(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "dtor",
            /* strMthInArgs       */ "" );
        #endif

        if( m_pMtx != nullptr )
        {
            m_pMtx->unlock();
        }
        #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        else if( m_pRecursiveMtx != nullptr )
        {
            m_pRecursiveMtx->unlock();
        }
        #endif

        #ifdef ZS_TRACE_MUTEXES
        if( m_pTrcAdminObj != nullptr )
        {
            mthTracer.onAdminObjAboutToBeReleased();
            CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        }
        m_eTrcMthFileDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
        m_pTrcMthFile = nullptr;
        m_pTrcAdminObj = nullptr;
        #endif
    }

    m_pMtx = nullptr;
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    m_pRecursiveMtx = nullptr;
    #endif
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the name of the mutex.

    @return Name of the object.
*/
//------------------------------------------------------------------------------
QString CMutexLocker::objectName() const
{
    QString strObjName;

    if( m_pMtx != nullptr )
    {
        strObjName = m_pMtx->objectName();
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    else if( m_pRecursiveMtx != nullptr )
    {
        strObjName = m_pRecursiveMtx->objectName();
    }
    #endif
    return strObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Relocks an unlocked mutex locker.

    For further information please refer to documentation of class QMutex.
*/
void CMutexLocker::relock()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_pMtx->objectName(),
        /* strMethod          */ "relock",
        /* strMthInArgs       */ "" );
    #endif

    if( m_pMtx != nullptr )
    {
        m_pMtx->lock();
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    else if( m_pRecursiveMtx != nullptr )
    {
        m_pRecursiveMtx->lock();
    }
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Unlocks this mutex locker.

    For further information please refer to documentation of class QMutex.
*/
void CMutexLocker::unlock()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_MUTEXES
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_pMtx->objectName(),
        /* strMethod          */ "unlock",
        /* strMthInArgs       */ "" );
    #endif

    if( m_pMtx != nullptr )
    {
        m_pMtx->unlock();
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    else if( m_pRecursiveMtx != nullptr )
    {
        m_pRecursiveMtx->unlock();
    }
    #endif
}

/*==============================================================================
public: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing for the given filter detail level is active.

    If the given filter detail level is not None and greater or equal to the
    trace detail level set at the trace admin object trace output is active.

    @param i_iFilterDetailLevel [in] Filter detail level to be checked.
    @return true if tracing is active, false otherwise
*/
bool CMutexLocker::isMethodTraceActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    #ifdef ZS_TRACE_MUTEXES
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(i_eFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_eTrcMthFileDetailLevel >= i_eFilterDetailLevel )
    {
        bIsActive = true;
    }
    #endif

    return bIsActive;
}

//------------------------------------------------------------------------------
/*! @brief Returns the current active detail level.

    @return Active method trace detail level.
*/
EMethodTraceDetailLevel CMutexLocker::getMethodTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevel::None;

    #ifdef ZS_TRACE_MUTEXES
    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        eDetailLevel = m_eTrcMthFileDetailLevel;
    }
    #endif

    return eDetailLevel;
}

