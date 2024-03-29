﻿/*******************************************************************************

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

#include "ZSSys/ZSSysWaitCondition.h"

#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
class CWaitCondition : public QWaitCondition
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructs a new wait condition object.

    For further information please refer to documentation of class QWaitCondition.

    @param i_strObjName [in] Descriptive name of the wait condition object.
*/
CWaitCondition::CWaitCondition(const QString& i_strObjName) :
//------------------------------------------------------------------------------
    QWaitCondition(),
    m_strObjName(i_strObjName),
    m_eTrcMthFileDetailLevel(EMethodTraceDetailLevel::None),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
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
}

//------------------------------------------------------------------------------
/*! @brief Constructs a new wait condition object.

    For further information please refer to documentation of class QWaitCondition.

    @param i_strObjName [in] Descriptive name of the wait condition object.
    @param i_eTrcMthFileDetailLevel [in]
        If trace outputs should not be forwarded to the trace server but directly
        to a trace method file allocated by the server, this detail level has to
        be to a value greater than None.
*/
CWaitCondition::CWaitCondition( const QString& i_strObjName, EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QWaitCondition(),
    m_strObjName(i_strObjName),
    m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    QString strLocalTrcFileAbsFilePath =
        CTrcServer::GetLocalTrcFileAbsoluteFilePath();
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
}

//------------------------------------------------------------------------------
/*! @brief Destroys the wait condition object.

    For further information please refer to documentation of class QMutex.
*/
CWaitCondition::~CWaitCondition()
//------------------------------------------------------------------------------
{
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

    //m_strObjName;
    m_eTrcMthFileDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Renames the object.

    @param i_strObjName [in] New object name.
*/
//------------------------------------------------------------------------------
void CWaitCondition::setObjectName(const QString& i_strObjName)
{
    QString strMthInArgs;

    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
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

    m_strObjName = i_strObjName;

    if( m_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObj = CTrcServer::RenameTraceAdminObj(m_pTrcAdminObj, objectName());
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the name of the wait condition object.

    @return Name of the object.
*/
//------------------------------------------------------------------------------
QString CWaitCondition::objectName() const
{
    return m_strObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CWaitCondition::notify_all()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "notify_all",
        /* strMthInArgs       */ "" );

    QWaitCondition::notify_all();
}

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CWaitCondition::notify_one()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "notify_one",
        /* strMthInArgs       */ "" );

    QWaitCondition::notify_one();
}

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
bool CWaitCondition::wait(CMutex* i_pMutexLocked, QDeadlineTimer i_deadline)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString(i_deadline.isForever() ? "Forever" : QString::number(i_deadline.deadline()));
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "wait",
        /* strMthInArgs       */ strMthInArgs );

    bool bOk = QWaitCondition::wait(i_pMutexLocked, i_deadline);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(bOk);
    }
    return bOk;
}
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
bool CWaitCondition::wait(QReadWriteLock* i_pReadWriteLock, QDeadlineTimer i_deadline)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString(i_deadline.isForever() ? "Forever" : QString::number(i_deadline.deadline()));
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "wait",
        /* strMthInArgs       */ strMthInArgs );

    bool bOk = QWaitCondition::wait(i_pReadWriteLock, i_deadline);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(bOk);
    }
    return bOk;
}
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CWaitCondition::wait(CMutex* i_pMutexLocked, unsigned long i_uTime_ms)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString::number(i_uTime_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "wait",
        /* strMthInArgs       */ strMthInArgs );

    bool bOk = QWaitCondition::wait(i_pMutexLocked, i_uTime_ms);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(bOk);
    }
    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
bool CWaitCondition::wait(QReadWriteLock* i_pReadWriteLock, unsigned long i_uTime_ms)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( isMethodTraceActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString::number(i_uTime_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "wait",
        /* strMthInArgs       */ strMthInArgs );

    bool bOk = QWaitCondition::wait(i_pReadWriteLock, i_uTime_ms);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(bOk);
    }
    return bOk;
}

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CWaitCondition::wakeAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "wakeAll",
        /* strMthInArgs       */ "" );

    QWaitCondition::wakeAll();
}

//------------------------------------------------------------------------------
/*! @brief Locks the mutex.

    For further information please refer to documentation of class QMutex.
*/
void CWaitCondition::wakeOne()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ m_strObjName,
        /* strMethod          */ "wakeOne",
        /* strMthInArgs       */ "" );

    QWaitCondition::wakeOne();
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
bool CWaitCondition::isMethodTraceActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(i_eFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_eTrcMthFileDetailLevel >= i_eFilterDetailLevel )
    {
        bIsActive = true;
    }
    return bIsActive;

} // isMethodTraceActive

//------------------------------------------------------------------------------
/*! @brief Returns the current active detail level.

    @return Active method trace detail level.
*/
EMethodTraceDetailLevel CWaitCondition::getMethodTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevel::None;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        eDetailLevel = m_eTrcMthFileDetailLevel;
    }
    return eDetailLevel;
}
