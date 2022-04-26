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
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;


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
    m_strObjName(i_strObjName),
    m_iTrcMthFileDetailLevel(ETraceDetailLevelNone),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), i_strObjName);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = qMutexRecursionMode2Str(i_mode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ strMthInArgs );
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_strObjName [in] Descriptive name of the mutex locker.
*/
CMutex::CMutex(const QString& i_strObjName) :
//------------------------------------------------------------------------------
    QMutex(),
    m_strObjName(i_strObjName),
    m_iTrcMthFileDetailLevel(ETraceDetailLevelNone),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_mode [in]
        If mode is QMutex::Recursive, a thread can lock the same mutex multiple times
        and the mutex won't be unlocked until a corresponding number of unlock() calls
        have been made. Otherwise a thread may only lock a mutex once.
    @param i_strObjName [in] Descriptive name of the mutex locker.
    @param i_iTrcMthFileDetailLevel [in]
        If trace outputs should not be forwarded to the trace server but directly
        to a trace method file allocated by the server, this detail level has to
        be to a value greater than None.
*/
CMutex::CMutex(
    QMutex::RecursionMode i_mode,
    const QString&        i_strObjName,
    int                   i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QMutex(i_mode),
    m_strObjName(i_strObjName),
    m_iTrcMthFileDetailLevel(i_iTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    QString strLocalTrcFileAbsFilePath = CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath("System");
    m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);

    QString strMthInArgs;

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = qMutexRecursionMode2Str(i_mode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ strMthInArgs );
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_strObjName [in] Descriptive name of the mutex locker.
    @param i_iTrcMthFileDetailLevel [in]
        If trace outputs should not be forwarded to the trace server but directly
        to a trace method file allocated by the server, this detail level has to
        be to a value greater than None.
*/
CMutex::CMutex( const QString& i_strObjName, int i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QMutex(),
    m_strObjName(i_strObjName),
    m_iTrcMthFileDetailLevel(i_iTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    QString strLocalTrcFileAbsFilePath = CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath("System");
    m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
}

//------------------------------------------------------------------------------
/*! @brief Destroys the mutex.

    For further information please refer to documentation of class QMutex.
*/
CMutex::~CMutex()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

    //m_strObjName;
    m_iTrcMthFileDetailLevel = 0;
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the name of the mutex.

    @return Name of the object.
*/
//------------------------------------------------------------------------------
QString CMutex::objectName() const
{
    return m_strObjName;
}

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

//------------------------------------------------------------------------------
/*! @brief Returns the detail level to be used if the mutex cannot use the
           trace server for trace output.

    If the mutex cannot use the trace server for method tracer the trace
    method file is directly used. Otherwise the mutex allocs a trace admin
    object and uses the trace server for method trace logging.

    @return Trace detail level. None if the trace server is used.
*/
int CMutex::traceMethodFileDetailLevel() const
//------------------------------------------------------------------------------
{
    return m_iTrcMthFileDetailLevel;
}

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
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "lock",
        /* strMthInArgs       */ "" );

    QMutex::lock();
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CMutex::tryLock(int i_timeout_ms)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_timeout_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "tryLock",
        /* strMthInArgs       */ strMthInArgs );

    bool bOk = QMutex::tryLock(i_timeout_ms);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CMutex::try_lock()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock",
        /* strAddInfo         */ "" );

    bool bOk = QMutex::try_lock();

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bOk);
    }
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
    QString strMthInArgs;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_duration.count());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock_for",
        /* strAddInfo         */ "" );

    bool bOk = QMutex::try_lock_for(i_duration);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bOk);
    }
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
    QString strMthInArgs;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_timePoint.time_since_epoch());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock_until",
        /* strAddInfo         */ strMthInArgs );

    bool bOk = QMutex::try_lock_until(i_timePoint);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Unlocks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CMutex::unlock()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "unlock",
        /* strAddInfo         */ "" );

    return QMutex::unlock();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing for the given filter detail level is active.

    If the given filter detail level is not None and greater or equal to the
    trace detail level set at the trace admin object trace output is active.

    @param i_iFilterDetailLevel [in] Filter detail level to be checked.
    @return true if tracing is active, false otherwise
*/
bool CMutex::isMethodTraceActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(i_iFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_iTrcMthFileDetailLevel >= i_iFilterDetailLevel )
    {
        bIsActive = true;
    }
    return bIsActive;

} // isMethodTraceActive


/*******************************************************************************
class CRecursiveMutex : public QRecursiveMutex
*******************************************************************************/

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
    m_strObjName(i_strObjName),
    m_iTrcMthFileDetailLevel(ETraceDetailLevelNone),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new mutex. The mutex is created in an unlocked state.

    For further information please refer to documentation of class QMutex.

    @param i_strObjName [in] Descriptive name of the mutex locker.
    @param i_iTrcMthFileDetailLevel [in]
        If trace outputs should not be forwarded to the trace server but directly
        to a trace method file allocated by the server, this detail level has to
        be to a value greater than None.
*/
CRecursiveMutex::CRecursiveMutex( const QString& i_strObjName, int i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QRecursiveMutex(),
    m_strObjName(i_strObjName),
    m_iTrcMthFileDetailLevel(i_iTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    QString strLocalTrcFileAbsFilePath = CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath("System");
    m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );
}

//------------------------------------------------------------------------------
/*! @brief Destroys the mutex.

    For further information please refer to documentation of class QMutex.
*/
CRecursiveMutex::~CRecursiveMutex()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

    //m_strObjName;
    //m_strTrcFileServerName;
    m_iTrcMthFileDetailLevel = 0;
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the name of the mutex.

    @return Name of the object.
*/
//------------------------------------------------------------------------------
QString CRecursiveMutex::objectName() const
{
    return m_strObjName;
}

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

//------------------------------------------------------------------------------
/*! @brief Returns the detail level to be used if the mutex cannot use the
           trace server for trace output.

    If the mutex cannot use the trace server for method tracer the trace
    method file is directly used. Otherwise the mutex allocs a trace admin
    object and uses the trace server for method trace logging.

    @return Trace detail level. None if the trace server is used.
*/
int CRecursiveMutex::traceMethodFileDetailLevel() const
//------------------------------------------------------------------------------
{
    return m_iTrcMthFileDetailLevel;
}

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
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "lock",
        /* strMthInArgs       */ "" );

    QRecursiveMutex::lock();
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CRecursiveMutex::tryLock(int i_timeout_ms)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_timeout_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "tryLock",
        /* strMthInArgs       */ strMthInArgs );

    bool bOk = QRecursiveMutex::tryLock(i_timeout_ms);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Attempts to locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CRecursiveMutex::try_lock()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock",
        /* strMthInArgs       */ "" );

    bool bOk = QRecursiveMutex::try_lock();

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bOk);
    }
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
    QString strMthInArgs;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_duration.count());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock_for",
        /* strMthInArgs       */ strMthInArgs );

    bool bOk = QRecursiveMutex::try_lock_for(i_duration);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bOk);
    }
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
    QString strMthInArgs;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_timePoint.time_since_epoch());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "try_lock_until",
        /* strMthInArgs       */ strMthInArgs );

    bool bOk = QRecursiveMutex::try_lock_until(i_timePoint);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bOk);
    }
    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Unlocks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CRecursiveMutex::unlock()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "unlock",
        /* strMthInArgs       */ "" );

    return QRecursiveMutex::unlock();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing for the given filter detail level is active.

    If the given filter detail level is not None and greater or equal to the
    trace detail level set at the trace admin object trace output is active.

    @param i_iFilterDetailLevel [in] Filter detail level to be checked.
    @return true if tracing is active, false otherwise
*/
bool CRecursiveMutex::isMethodTraceActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(i_iFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_iTrcMthFileDetailLevel >= i_iFilterDetailLevel )
    {
        bIsActive = true;
    }
    return bIsActive;

} // isMethodTraceActive


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
    m_pMtx(i_pMutex),
    m_pRecursiveMtx(nullptr),
    m_iTrcMthFileDetailLevel(ETraceDetailLevelNone),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( m_pMtx != nullptr )
    {
        m_iTrcMthFileDetailLevel = m_pMtx->traceMethodFileDetailLevel();
        m_pTrcMthFile = m_pMtx->traceMethodFile();

        if( m_pTrcMthFile == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
        }

        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ NameSpace(),
            /* strClassName       */ ClassName(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "ctor",
            /* strMthInArgs       */ "" );

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
CMutexLocker::CMutexLocker(CRecursiveMutex* i_pMutex) :
//------------------------------------------------------------------------------
    m_pMtx(nullptr),
    m_pRecursiveMtx(i_pMutex),
    m_iTrcMthFileDetailLevel(ETraceDetailLevelNone),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( m_pRecursiveMtx != nullptr )
    {
        m_iTrcMthFileDetailLevel = m_pRecursiveMtx->traceMethodFileDetailLevel();
        m_pTrcMthFile = m_pRecursiveMtx->traceMethodFile();

        if( m_pTrcMthFile == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
        }

        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ NameSpace(),
            /* strClassName       */ ClassName(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "ctor",
            /* strMthInArgs       */ "" );

        m_pRecursiveMtx->lock();
    }
}

//------------------------------------------------------------------------------
/*! @brief Destroys the mutex.

    For further information please refer to documentation of class QMutex.
*/
CMutexLocker::~CMutexLocker()
//------------------------------------------------------------------------------
{
    if( m_pMtx != nullptr || m_pRecursiveMtx != nullptr )
    {
        CMethodTracer mthTracer(
            /* pAdminObj          */ m_pTrcAdminObj,
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ NameSpace(),
            /* strClassName       */ ClassName(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "dtor",
            /* strMthInArgs       */ "" );

        if( m_pMtx != nullptr )
        {
            m_pMtx->unlock();
        }
        else if( m_pRecursiveMtx != nullptr )
        {
            m_pRecursiveMtx->unlock();
        }

        if( m_pTrcAdminObj != nullptr )
        {
            mthTracer.onAdminObjAboutToBeReleased();
            CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        }
    }

    m_pMtx = nullptr;
    m_pRecursiveMtx = nullptr;
    m_iTrcMthFileDetailLevel = 0;
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;
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
    else if( m_pRecursiveMtx != nullptr )
    {
        strObjName = m_pRecursiveMtx->objectName();
    }
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
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_pMtx->objectName(),
        /* strMethod          */ "relock",
        /* strMthInArgs       */ "" );

    if( m_pMtx != nullptr )
    {
        m_pMtx->lock();
    }
    else if( m_pRecursiveMtx != nullptr )
    {
        m_pRecursiveMtx->lock();
    }
}

//------------------------------------------------------------------------------
/*! @brief Unlocks this mutex locker.

    For further information please refer to documentation of class QMutex.
*/
void CMutexLocker::unlock()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_pMtx->objectName(),
        /* strMethod          */ "unlock",
        /* strMthInArgs       */ "" );

    if( m_pMtx != nullptr )
    {
        m_pMtx->unlock();
    }
    else if( m_pRecursiveMtx != nullptr )
    {
        m_pRecursiveMtx->unlock();
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing for the given filter detail level is active.

    If the given filter detail level is not None and greater or equal to the
    trace detail level set at the trace admin object trace output is active.

    @param i_iFilterDetailLevel [in] Filter detail level to be checked.
    @return true if tracing is active, false otherwise
*/
bool CMutexLocker::isMethodTraceActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(i_iFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_iTrcMthFileDetailLevel >= i_iFilterDetailLevel )
    {
        bIsActive = true;
    }
    return bIsActive;

} // isMethodTraceActive

