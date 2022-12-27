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

#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequest.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
class CMethodTracer : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Instance tracer (name space, class and object name set at trace admin object).

    @param i_pTrcAdminObj [in]
        Trace admin object used to control the output detail level of the method trace.
    @param i_eFilterDetailLevel [in]
        Entering and leaving the method is traced if the admin objects detail level
        is greater or equal than the filter setting than the detail level.
    @param i_strMethod [in]
        Name of the method to be traced.
    @param i_strMethodInArgs [in]
        String describing the input arguments passed to the method to be traced.
*/
CMethodTracer::CMethodTracer(
    CTrcAdminObj*           i_pTrcAdminObj,
    EMethodTraceDetailLevel i_eFilterDetailLevel,
    const QString&          i_strMethod,
    const QString&          i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_pTrcMthFile(nullptr),
    m_eMethodCallsTrcDetailLevel(EMethodTraceDetailLevel::Undefined),
    m_eEnterLeaveFilterDetailLevel(i_eFilterDetailLevel),
    m_bEnterTraced(false),
    m_strNameSpace(),
    m_strClassName(),
    m_strObjName(),
    m_strMethod(i_strMethod),
    m_strMethodReturn(),
    m_strMethodOutArgs()
{
    if( m_pTrcAdminObj != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pTrcAdminObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjAboutToBeDestroyed(QObject*)),
            /* cnctType     */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( m_pTrcAdminObj->areMethodCallsActive(m_eEnterLeaveFilterDetailLevel)
         && !m_pTrcAdminObj->isTraceDataSuppressedByFilter(i_strMethodInArgs) )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethodEnter(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* strMethod    */ m_strMethod,
                    /* strMthInArgs */ i_strMethodInArgs );
                m_bEnterTraced = true;
            }
        }
        m_pTrcAdminObj->lock();
    }
} // ctor

//------------------------------------------------------------------------------
/*! Class tracer (name space and class name (but not object name) set at trace admin object).

    @param i_pTrcAdminObj [in]
        Trace admin object used to control the output detail level of the method trace.
    @param i_eFilterDetailLevel [in]
        Entering and leaving the method is traced if the admin objects detail level
        is greater or equal than the filter setting than the detail level.
    @param i_strObjName [in]
        Name of the object (instance) the method to be traced is applied to.
    @param i_strMethod [in]
        Name of the method to be traced.
    @param i_strMethodInArgs [in]
        String describing the input arguments passed to the method to be traced.
*/
CMethodTracer::CMethodTracer(
    CTrcAdminObj*           i_pTrcAdminObj,
    EMethodTraceDetailLevel i_eFilterDetailLevel,
    const QString&          i_strObjName,
    const QString&          i_strMethod,
    const QString&          i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_pTrcMthFile(nullptr),
    m_eMethodCallsTrcDetailLevel(EMethodTraceDetailLevel::Undefined),
    m_eEnterLeaveFilterDetailLevel(i_eFilterDetailLevel),
    m_bEnterTraced(false),
    m_strNameSpace(),
    m_strClassName(),
    m_strObjName(i_strObjName),
    m_strMethod(i_strMethod),
    m_strMethodReturn(),
    m_strMethodOutArgs()
{
    if( m_pTrcAdminObj != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pTrcAdminObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjAboutToBeDestroyed(QObject*)),
            /* cnctType     */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( m_pTrcAdminObj->areMethodCallsActive(m_eEnterLeaveFilterDetailLevel)
         && !m_pTrcAdminObj->isTraceDataSuppressedByFilter(i_strMethodInArgs) )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethodEnter(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* strObjName   */ m_strObjName,
                    /* strMethod    */ m_strMethod,
                    /* strMthInArgs */ i_strMethodInArgs );
                m_bEnterTraced = true;
            }
        }
        m_pTrcAdminObj->lock();
    }
} // ctor

//------------------------------------------------------------------------------
/*! Method trace without trace admin object but with reference to trace method file.

    This constructor may be used in classes when trace admin objects cannot be used
    (e.g. when tracing the trace server itself).

    @param i_pTrcMthFile [in]
        Pointer to trace method file used to write the trace outputs.
    @param i_eTrcDetailLevel [in]
        Entering and leaving the method is traced if the method trace detail level is
        greater or equal than the filter setting than the detail level.
    @param i_eFilterDetailLevel [in]
        Entering and leaving the method is traced if the admin objects detail level
        is greater or equal than the filter setting than the detail level.
    @param i_strNameSpace [in]
        Name space of the class to be traced.
        Will be ignored if i_pTrcAdminObj != nullptr is passed.
    @param i_strClassName [in]
        Class name of the class to be traced.
        Will be ignored if i_pTrcAdminObj != nullptr is passed.
    @param i_strObjName [in]
        Name of the object (instance) the method to be traced is applied to.
    @param i_strMethod [in]
        Name of the method to be traced.
    @param i_strMethodInArgs [in]
        String describing the input arguments passed to the method to be traced.
*/
CMethodTracer::CMethodTracer(
    CTrcMthFile*            i_pTrcMthFile,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eFilterDetailLevel,
    const QString&          i_strNameSpace,
    const QString&          i_strClassName,
    const QString&          i_strObjName,
    const QString&          i_strMethod,
    const QString&          i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(nullptr),
    m_pTrcMthFile(i_pTrcMthFile),
    m_eMethodCallsTrcDetailLevel(i_eTrcDetailLevel),
    m_eEnterLeaveFilterDetailLevel(i_eFilterDetailLevel),
    m_bEnterTraced(false),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_strMethod(i_strMethod),
    m_strMethodReturn(),
    m_strMethodOutArgs()
{
    if( m_pTrcMthFile != nullptr && m_eMethodCallsTrcDetailLevel >= m_eEnterLeaveFilterDetailLevel )
    {
        QString strMth = buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);

        if( !strMth.isEmpty() && !m_strMethod.isEmpty() )
        {
            strMth += "." + m_strMethod;
        }
        m_pTrcMthFile->traceMethodEnter(strMth, i_strMethodInArgs);
        m_bEnterTraced = true;
    }
} // ctor

//------------------------------------------------------------------------------
/*! Method trace either with trace admin object or with trace method file.

    This constructor may be used in classes when trace admin objects cannot be
    used in some cirmumstances. E.g. tracing Ipc Server is not possible if the
    server is used by the trace server.

    @param i_pTrcAdminObj [in]
        Trace admin object used to control the output detail level of the method trace.
        If != nullptr arguments i_pTrcServer and TrcDetailLevel are i_eTrcDetailLevel.
    @param i_pTrcMthFile [in]
        If i_pTrcAdminObj == nullptr i_pTrcMthFile must not be nullptr and i_eTrcDetailLevel
        is used to control the detail level of the trace output.
    @param i_eTrcDetailLevel [in]
        Entering and leaving the method is traced if the method trace detail level is
        greater or equal than the filter setting than the detail level.
    @param i_eFilterDetailLevel [in]
        Entering and leaving the method is traced if the admin objects detail level
        is greater or equal than the filter setting than the detail level.
    @param i_strNameSpace [in]
        Name space of the class to be traced.
        Will be ignored if i_pTrcAdminObj != nullptr is passed.
    @param i_strClassName [in]
        Class name of the class to be traced.
        Will be ignored if i_pTrcAdminObj != nullptr is passed.
    @param i_strObjName [in]
        Name of the object (instance) the method to be traced is applied to.
    @param i_strMethod [in]
        Name of the method to be traced.
    @param i_strMethodInArgs [in]
        String describing the input arguments passed to the method to be traced.
*/
CMethodTracer::CMethodTracer(
    CTrcAdminObj*           i_pTrcAdminObj,
    CTrcMthFile*            i_pTrcMthFile,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eFilterDetailLevel,
    const QString&          i_strNameSpace,
    const QString&          i_strClassName,
    const QString&          i_strObjName,
    const QString&          i_strMethod,
    const QString&          i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_pTrcMthFile(i_pTrcMthFile),
    m_eMethodCallsTrcDetailLevel(i_eTrcDetailLevel),
    m_eEnterLeaveFilterDetailLevel(i_eFilterDetailLevel),
    m_bEnterTraced(false),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_strMethod(i_strMethod),
    m_strMethodReturn(),
    m_strMethodOutArgs()
{
    if( m_pTrcAdminObj != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pTrcAdminObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjAboutToBeDestroyed(QObject*)),
            /* cnctType     */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( m_pTrcAdminObj->areMethodCallsActive(m_eEnterLeaveFilterDetailLevel)
         && !m_pTrcAdminObj->isTraceDataSuppressedByFilter(i_strMethodInArgs) )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethodEnter(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* strObjName   */ m_strObjName.isEmpty() ? m_pTrcAdminObj->getObjectName() : m_strObjName,
                    /* strMethod    */ m_strMethod,
                    /* strMthInArgs */ i_strMethodInArgs );
                m_bEnterTraced = true;
            }
        }
        m_pTrcAdminObj->lock();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        if( m_eMethodCallsTrcDetailLevel >= m_eEnterLeaveFilterDetailLevel )
        {
            QString strMth = buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);

            if( !strMth.isEmpty() && !m_strMethod.isEmpty() )
            {
                strMth += "." + m_strMethod;
            }
            m_pTrcMthFile->traceMethodEnter(strMth, i_strMethodInArgs);
            m_bEnterTraced = true;
        }
    }
} // ctor

//------------------------------------------------------------------------------
/*! Destroys the CMethodTracer and traces leaving the method.

    @note If the CMethodTracer is used in the destructor of a class and a trace
          admin object is used the trace admin object is usually released in this
          destructor. If the trace admin object would be destroyed when releasing
          the instance leaving the destructor may not be traced and the program
          may crash as the method tracer would access a dead instance.
          This should usually not happen as the trace server together with the
          tree of admin objects should be destroyed at the end of the program.
          But to be on safe side you can call "onAdminObjAboutToBeReleased"
          before releasing the trace admin object.
*/
CMethodTracer::~CMethodTracer()
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        onAdminObjAboutToBeReleased(); // Sets m_pTrcAdminObj to nullptr
    }
    else if( m_pTrcMthFile != nullptr )
    {
        if( m_bEnterTraced )
        {
            QString strMth = buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);

            if( !strMth.isEmpty() && !m_strMethod.isEmpty() )
            {
                strMth += "." + m_strMethod;
            }
            m_pTrcMthFile->traceMethodLeave(strMth, m_strMethodReturn, m_strMethodOutArgs);
        }
    }

    m_pTrcAdminObj = nullptr;
    m_pTrcMthFile = nullptr;
    m_eMethodCallsTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_eEnterLeaveFilterDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_bEnterTraced = false;
    //m_strNameSpace;
    //m_strClassName;
    //m_strObjName;
    //m_strMethod;
    //m_strMethodReturn;
    //m_strMethodOutArgs;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! This method may be called if the CMethodTracer is used in the destructor
    of a class right before the trace admin object is released.

    @see ~CMethodTracer for more details.

    "onAdminObjAboutToBeReleased" will trace leaving the destructor.
*/
void CMethodTracer::onAdminObjAboutToBeReleased()
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcAdminObj,
            /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjAboutToBeDestroyed(QObject*)) );

        if( m_bEnterTraced )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethodLeave(
                    /* pAdminObj     */ m_pTrcAdminObj,
                    /* strObjName    */ m_strObjName.isEmpty() ? m_pTrcAdminObj->getObjectName() : m_strObjName,
                    /* strMethod     */ m_strMethod,
                    /* strMthReturn  */ m_strMethodReturn,
                    /* strMthOutArgs */ m_strMethodOutArgs );
            }
        }
        m_pTrcAdminObj->unlock();
        if( m_pTrcAdminObj->deleteOnUnlock() )
        {
            CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        }
        m_pTrcAdminObj = nullptr;
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the current detail level either set at the trace admin object or
    at the method tracer itself.

    @return Current trace detail level.
*/
EMethodTraceDetailLevel CMethodTracer::getMethodCallsTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = m_eMethodCallsTrcDetailLevel;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    else
    {
        eDetailLevel = m_eMethodCallsTrcDetailLevel;
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
/*! Returns whether tracing is active for the given filter detail level.

    E.g. if or the trace admin object the detail level MethodArgs is set and
    "areMethodCallsActive" will return false for detail levels greater than MethodArgs.

    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return true if tracing is active for the given filter detail level.
            false otherwise.
*/
bool CMethodTracer::areMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bActive = false;

    if( m_pTrcAdminObj != nullptr )
    {
        bActive = m_pTrcAdminObj->areMethodCallsActive(i_eFilterDetailLevel);
    }
    else
    {
        // Without trace admin object the caller must decide whether tracing is active.
        bActive = (m_eMethodCallsTrcDetailLevel >= i_eFilterDetailLevel);
    }
    return bActive;
}

//------------------------------------------------------------------------------
/*! Returns the detail level which was passed to the constructor.

    When leaving the method and the method tracer is destroyed the same detail
    level must be used as when entering the method.

    @return Detail level used for entering and leaving a method.
*/
EMethodTraceDetailLevel CMethodTracer::getEnterLeaveFilterDetailLevel() const
//------------------------------------------------------------------------------
{
    return m_eEnterLeaveFilterDetailLevel;
}

//------------------------------------------------------------------------------
/*! Returns the current detail level either set at the trace admin object or
    at the method tracer itself.

    @return Current trace detail level.
*/
ELogDetailLevel CMethodTracer::getRuntimeInfoTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevel::None;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel();
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
/*! Returns whether tracing is active for the given filter detail level.

    E.g. if or the trace admin object the detail level MethodArgs is set and
    "isActive" will return false for detail levels greater than MethodArgs.

    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return true if tracing is active for the given filter detail level.
            false otherwise.
*/
bool CMethodTracer::isRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bActive = false;

    if( m_pTrcAdminObj != nullptr )
    {
        bActive = m_pTrcAdminObj->isRuntimeInfoActive(i_eFilterDetailLevel);
    }
    return bActive;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether given trace data should be suppressed by the data filter.

    Example

        if( !m_pTrcAdminObj->isTraceDataSuppressedByFilter("bla bla bla") )
        {
            strTrcOutData = "bla bla bla";
        }

    @param i_strTraceData [in]
        Trace data to be checked against the filter string.

    @return true if the passed trace data should be suppressed, false otherwise.
*/
bool CMethodTracer::isTraceDataSuppressedByFilter( const QString& i_strTraceData ) const
//------------------------------------------------------------------------------
{
    bool bSuppressed = false;

    if( m_pTrcAdminObj != nullptr )
    {
        bSuppressed = m_pTrcAdminObj->isTraceDataSuppressedByFilter(i_strTraceData);
    }
    return bSuppressed;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the name space of the class to be traced as passed to the constructor
    of the CMethodTracer class.

    @return Name space of the class to be traced.
*/
QString CMethodTracer::getNameSpace() const
//------------------------------------------------------------------------------
{
    QString strNameSpace = m_strNameSpace;

    if( m_pTrcAdminObj != nullptr )
    {
        strNameSpace = m_pTrcAdminObj->getNameSpace();
    }
    return strNameSpace;
}

//------------------------------------------------------------------------------
/*! Returns the name of the class to be traced as passed to the constructor
    of the CMethodTracer class.

    @return Name space of the class to be traced.
*/
QString CMethodTracer::getClassName() const
//------------------------------------------------------------------------------
{
    QString strClassName = m_strClassName;

    if( m_pTrcAdminObj != nullptr )
    {
        strClassName = m_pTrcAdminObj->getClassName();
    }
    return strClassName;

} // getClassName

//------------------------------------------------------------------------------
/*! Returns the object name of the instance to be traced as passed to the constructor
    of the CMethodTracer class.

    @return Name of the object the method to be traced is applied.
*/
QString CMethodTracer::getObjectName() const
//------------------------------------------------------------------------------
{
    QString strObjName = m_strObjName;

    if( m_pTrcAdminObj != nullptr && m_strObjName.isEmpty() )
    {
        strObjName = m_pTrcAdminObj->getObjectName();
    }
    return strObjName;

} // getObjectName

//------------------------------------------------------------------------------
/*! Returns the method name to be traced as passed to the constructor.

    @return Method name.
*/
QString CMethodTracer::getMethod() const
//------------------------------------------------------------------------------
{
    return m_strMethod;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_bResult [in] Return value of the method to be traced.
*/
void CMethodTracer::setMethodReturn( bool i_bResult )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = bool2Str(i_bResult);
}

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_iResult [in] Return value of the method to be traced.
*/
void CMethodTracer::setMethodReturn( int i_iResult )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = QString::number(i_iResult);
}

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_result [in] Return value of the method to be traced.
*/
void CMethodTracer::setMethodReturn( EResult i_result )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = result2Str(i_result);
}

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_errResultInfo [in] Return value of the method to be traced.
*/
void CMethodTracer::setMethodReturn( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = i_errResultInfo.toString();
}

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_pReq [in] Return value of the method to be traced.
*/
//------------------------------------------------------------------------------
void CMethodTracer::setMethodReturn( ZS::System::CRequest* i_pReq )
{
    m_strMethodReturn = i_pReq == nullptr ? "SUCCESS" : i_pReq->getAddTrcInfoStr();
}

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns a value and this return value should appear
    in the trace output the return value may be set by this method.

    @param i_str [in] Return value of the method to be traced.
*/
void CMethodTracer::setMethodReturn( const QString& i_str )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = i_str;
}

//------------------------------------------------------------------------------
/*! Returns the string which should be used as the return value of the method
    to be traced.

    @return Return value string.
*/
QString CMethodTracer::getMethodReturn() const
//------------------------------------------------------------------------------
{
    return m_strMethodReturn;
}

//------------------------------------------------------------------------------
/*! If the destructor of the method tracer class is called leaving the method
    is traced. If the method returns values through arguments and those output
    arguments should appear in the trace output the output values may be set
    by invoking this method.

    @param i_str [in] String describing the output arguments of the method to be traced.
*/
void CMethodTracer::setMethodOutArgs( const QString& i_str )
//------------------------------------------------------------------------------
{
    m_strMethodOutArgs = i_str;
}

//------------------------------------------------------------------------------
/*! Returns the string which should be used to indicate output arguments of the
    method to be traced.

    @return String descibing the output arguments.
*/
QString CMethodTracer::getMethodOutArgs() const
//------------------------------------------------------------------------------
{
    return m_strMethodOutArgs;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Adds a trace output string.

    This method is used to add additional runtime information to the trace output.

    @param i_strAddInfo [in]
        Additional runtime information to be output.
    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.
*/
void CMethodTracer::trace(
    const QString&  i_strAddInfo,
    ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        if( m_pTrcAdminObj->isRuntimeInfoActive(i_eFilterDetailLevel)
         && !m_pTrcAdminObj->isTraceDataSuppressedByFilter(i_strAddInfo) )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethod(
                    /* pAdminObj  */ m_pTrcAdminObj,
                    /* strObjName */ m_strObjName.isEmpty() ? m_pTrcAdminObj->getObjectName() : m_strObjName,
                    /* strMethod  */ m_strMethod,
                    /* strAddInfo */ i_strAddInfo );
            }
        }
    } // if( m_pTrcAdminObj != nullptr )
} // trace

//------------------------------------------------------------------------------
/*! Adds a trace output string.

    This method is used to add additional runtime information to the trace output
    if the trace detail level is set at the class to be traced itself.

    @param i_strAddInfo [in]
        Additional runtime information to be output.
    @param i_eTrcDetailLevel [in]
        Current trace detail level to be compared against the filter detail level.
    @param i_eFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.
*/
void CMethodTracer::trace(
    const QString&  i_strAddInfo,
    ELogDetailLevel i_eTrcDetailLevel,
    ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        ELogDetailLevel eTrcDetailLevel = i_eTrcDetailLevel;

        if( i_eTrcDetailLevel == ELogDetailLevel::Undefined )
        {
            eTrcDetailLevel = m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel();
        }

        if( m_pTrcAdminObj->isEnabled() && eTrcDetailLevel >= i_eFilterDetailLevel )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethod(
                    /* pAdminObj  */ m_pTrcAdminObj,
                    /* strObjName */ m_strObjName.isEmpty() ? m_pTrcAdminObj->getObjectName() : m_strObjName,
                    /* strMethod  */ m_strMethod,
                    /* strAddInfo */ i_strAddInfo );
            }
        }
    } // if( m_pTrcAdminObj != nullptr )
} // trace

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! If trace admin objects are used to control the detail level of the trace outputs
    those admin objects may be destroyed while the method tracer accesssing those
    admin objects is still alive.

    To ensure that the method tracer does not acces the destroyed instance anymore
    this slot method is connected to the aboutToBeDestroyed signal of the trace admin
    object.

    When renaming trace admin objects the trace admin objects will not be really
    just renamed. What happens depends on the reference counter of the trace admin
    object and whether a trace admin object with the new name is already existing.

    The admin object may be moved, a new object may be created replacing the
    current trace admin object for the instance using the trace admin object
    and - if the "old" trace admin object is no longer reference - the trace
    admin object may be destroyed.

    The method tracer does not take care whether the trace admin object will be
    replaced with a new one as this may only happen when renaming the instance
    using the trace admin object (name spaces or classes cannot be renamed
    during runtime). But the method tracer needs to update the thread calling
    depth and will therefor log "traceMethodLeave".

    If the method tracer is used again the new trace admin object will be passed
    anyway to the ctor of the method tracer.

    @see also CMethodTracer::onAdminObjAboutToBeReleased

    @param i_pTrcAdminObj [in] Trace admin object which is going to be destroyed.
*/
void CMethodTracer::onAdminObjAboutToBeDestroyed( QObject* /*i_pTrcAdminObj*/ )
//------------------------------------------------------------------------------
{
    onAdminObjAboutToBeReleased();
}
