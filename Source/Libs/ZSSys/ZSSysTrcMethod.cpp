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
using namespace ZS::Trace;


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
    @param i_iFilterDetailLevel [in]
        Entering and leaving the method is traced if the admin objects detail level
        is greater or equal than the filter setting than the detail level.
    @param i_strMethod [in]
        Name of the method to be traced.
    @param i_strMethodInArgs [in]
        String describing the input arguments passed to the method to be traced.
*/
CMethodTracer::CMethodTracer(
    CTrcAdminObj*  i_pTrcAdminObj,
    int            i_iFilterDetailLevel,
    const QString& i_strMethod,
    const QString& i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_pTrcServer(nullptr),
    m_pTrcMthFile(nullptr),
    m_iTrcDetailLevel(ETraceDetailLevelUndefined),
    m_iEnterLeaveFilterDetailLevel(i_iFilterDetailLevel),
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
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjDestroyed(QObject*)),
            /* cnctType     */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( m_pTrcAdminObj->isActive(m_iEnterLeaveFilterDetailLevel) )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethodEnter(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* strMethod    */ m_strMethod,
                    /* strMthInArgs */ i_strMethodInArgs );
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
/*! Class tracer (name space and class name (but not object name) set at trace admin object).

    @param i_pTrcAdminObj [in]
        Trace admin object used to control the output detail level of the method trace.
    @param i_iFilterDetailLevel [in]
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
    CTrcAdminObj*  i_pTrcAdminObj,
    int            i_iFilterDetailLevel,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_pTrcServer(nullptr),
    m_pTrcMthFile(nullptr),
    m_iTrcDetailLevel(ETraceDetailLevelUndefined),
    m_iEnterLeaveFilterDetailLevel(i_iFilterDetailLevel),
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
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjDestroyed(QObject*)),
            /* cnctType     */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( m_pTrcAdminObj->isActive(m_iEnterLeaveFilterDetailLevel) )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethodEnter(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* strObjName   */ m_strObjName,
                    /* strMethod    */ m_strMethod,
                    /* strMthInArgs */ i_strMethodInArgs );
            }
        }
    } // if( m_pTrcAdminObj != nullptr )

} // ctor

//------------------------------------------------------------------------------
/*! Method tracer either with or without trace admin object.

    This constructor may be used in classes when trace admin objects cannot be
    used in any cirmumstances. E.g. tracing the index tree is not possible if
    the index tree is used to hold trace admin objects.

    @param i_pTrcAdminObj [in]
        Trace admin object used to control the output detail level of the method trace.
        If != nullptr arguments i_pTrcServer and TrcDetailLevel are i_iTrcDetailLevel.
    @param i_pTrcServer [in]
        If i_pTrcAdminObj == nullptr i_pTrcServer must not be nullptr and i_iTrcDetailLevel
        is used to control the detail level of the trace output.
    @param i_iTrcDetailLevel [in]
        Entering and leaving the method is traced if the method trace detail level is
        greater or equal than the filter setting than the detail level.
    @param i_iFilterDetailLevel [in]
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
    CTrcAdminObj*  i_pTrcAdminObj,
    CTrcServer*    i_pTrcServer,
    int            i_iTrcDetailLevel,
    int            i_iFilterDetailLevel,
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_pTrcServer(i_pTrcServer),
    m_pTrcMthFile(nullptr),
    m_iTrcDetailLevel(i_iTrcDetailLevel),
    m_iEnterLeaveFilterDetailLevel(i_iFilterDetailLevel),
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
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjDestroyed(QObject*)),
            /* cnctType     */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( m_pTrcAdminObj->isActive(m_iEnterLeaveFilterDetailLevel) )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethodEnter(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* strObjName   */ m_strObjName,
                    /* strMethod    */ m_strMethod,
                    /* strMthInArgs */ i_strMethodInArgs );
            }
        }
    } // if( m_pTrcAdminObj != nullptr )

    else if( m_pTrcServer != nullptr && m_iTrcDetailLevel >= m_iEnterLeaveFilterDetailLevel )
    {
        m_pTrcServer->traceMethodEnter(
            /* strNameSpace */ m_strNameSpace,
            /* strClassName */ m_strClassName,
            /* strObjName   */ m_strObjName,
            /* strMethod    */ m_strMethod,
            /* strMthInArgs */ i_strMethodInArgs );
    }

} // ctor

//------------------------------------------------------------------------------
/*! Method trace without trace admin object but with trace server.

    This constructor may be used in classes when trace admin objects cannot be
    used at all but there is a trace server hosting a trace method file.

    @param i_pTrcServer [in]
        Pointer to trace server hosting the trace method file.
    @param i_iTrcDetailLevel [in]
        Entering and leaving the method is traced if the method trace detail level is
        greater or equal than the filter setting than the detail level.
    @param i_iFilterDetailLevel [in]
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
    CTrcServer*    i_pTrcServer,
    int            i_iTrcDetailLevel,
    int            i_iFilterDetailLevel,
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(nullptr),
    m_pTrcServer(i_pTrcServer),
    m_pTrcMthFile(nullptr),
    m_iTrcDetailLevel(i_iTrcDetailLevel),
    m_iEnterLeaveFilterDetailLevel(i_iFilterDetailLevel),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_strMethod(i_strMethod),
    m_strMethodReturn(),
    m_strMethodOutArgs()
{
    if( m_pTrcServer != nullptr && m_iTrcDetailLevel >= m_iEnterLeaveFilterDetailLevel )
    {
        m_pTrcServer->traceMethodEnter(
            /* strNameSpace */ m_strNameSpace,
            /* strClassName */ m_strClassName,
            /* strObjName   */ m_strObjName,
            /* strMethod    */ m_strMethod,
            /* strMthInArgs */ i_strMethodInArgs );
    }

} // ctor

//------------------------------------------------------------------------------
/*! Method trace without trace admin object and without trace server but with
    reference to trace method file.

    This constructor may be used in classes when neither trace admin objects nor
    a trace server can be used (e.g. when tracing the trace server itself).

    @param i_pTrcMthFile [in]
        Pointer to trace method file used to write the trace outputs.
    @param i_iTrcDetailLevel [in]
        Entering and leaving the method is traced if the method trace detail level is
        greater or equal than the filter setting than the detail level.
    @param i_iFilterDetailLevel [in]
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
    CTrcMthFile*   i_pTrcMthFile,
    int            i_iTrcDetailLevel,
    int            i_iFilterDetailLevel,
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(nullptr),
    m_pTrcServer(nullptr),
    m_pTrcMthFile(i_pTrcMthFile),
    m_iTrcDetailLevel(i_iTrcDetailLevel),
    m_iEnterLeaveFilterDetailLevel(i_iFilterDetailLevel),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_strMethod(i_strMethod),
    m_strMethodReturn(),
    m_strMethodOutArgs()
{
    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= m_iEnterLeaveFilterDetailLevel )
    {
        QString strMth = buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);

        if( !strMth.isEmpty() && !m_strMethod.isEmpty() )
        {
            strMth += "." + m_strMethod;
        }
        m_pTrcMthFile->traceMethodEnter(strMth, i_strMethodInArgs);
    }

} // ctor

//------------------------------------------------------------------------------
/*! Method trace either with trace admin object or with trace method file.

    This constructor may be used in classes when trace admin objects cannot be
    used in any cirmumstances. E.g. tracing Ipc Server is not possible if the
    server is used by the trace server.

    @param i_pTrcAdminObj [in]
        Trace admin object used to control the output detail level of the method trace.
        If != nullptr arguments i_pTrcServer and TrcDetailLevel are i_iTrcDetailLevel.
    @param i_pTrcMthFile [in]
        If i_pTrcAdminObj == nullptr i_pTrcMthFile must not be nullptr and i_iTrcDetailLevel
        is used to control the detail level of the trace output.
    @param i_iTrcDetailLevel [in]
        Entering and leaving the method is traced if the method trace detail level is
        greater or equal than the filter setting than the detail level.
    @param i_iFilterDetailLevel [in]
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
    CTrcAdminObj*  i_pTrcAdminObj,
    CTrcMthFile*   i_pTrcMthFile,
    int            i_iTrcDetailLevel,
    int            i_iFilterDetailLevel,
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strMethodInArgs ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObj(i_pTrcAdminObj),
    m_pTrcServer(nullptr),
    m_pTrcMthFile(i_pTrcMthFile),
    m_iTrcDetailLevel(i_iTrcDetailLevel),
    m_iEnterLeaveFilterDetailLevel(i_iFilterDetailLevel),
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
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjDestroyed(QObject*)),
            /* cnctType     */ Qt::DirectConnection ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( m_pTrcAdminObj->isActive(m_iEnterLeaveFilterDetailLevel) )
        {
            CTrcServer* pTrcServer = m_pTrcAdminObj->getTraceServer();

            if( pTrcServer != nullptr )
            {
                pTrcServer->traceMethodEnter(
                    /* pAdminObj    */ m_pTrcAdminObj,
                    /* strObjName   */ m_strObjName.isEmpty() ? m_pTrcAdminObj->getObjectName() : m_strObjName,
                    /* strMethod    */ m_strMethod,
                    /* strMthInArgs */ i_strMethodInArgs );
            }
        }
    } // if( m_pTrcAdminObj != nullptr )

    else if( m_pTrcMthFile != nullptr )
    {
        if( m_iTrcDetailLevel >= m_iEnterLeaveFilterDetailLevel )
        {
            QString strMth = buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);

            if( !strMth.isEmpty() && !m_strMethod.isEmpty() )
            {
                strMth += "." + m_strMethod;
            }
            m_pTrcMthFile->traceMethodEnter(strMth, i_strMethodInArgs);
        }
    } // if( m_pTrcMthFile != nullptr )

} // ctor

//------------------------------------------------------------------------------
/*! Destroys the CMethodTracer and traces leaving the method.

    @note If the CMethodTracer is used in the destructor of a class and a trace
          admin object is used the trace admin object is usually released in this
          destructor. In this case leaving the destructor cannot be traced if the
          CMethodTracer class as the trace admin object is no longer accessible
          if the destructor of the CMethodTracer class is called.
          So when using CMethodTracer in destructors the CMethodTracer got to be
          informed that the trace adming object will be released to trace leaving
          the method before the trace admin object is released. For this the method
          "onAdminObjAboutToBeReleased" has to be applied at the method tracer instance
          as this method will trace leaving the current method.
*/
CMethodTracer::~CMethodTracer()
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        onAdminObjAboutToBeReleased();
    }
    else if( m_pTrcServer != nullptr )
    {
        if( m_iTrcDetailLevel >= m_iEnterLeaveFilterDetailLevel )
        {
            m_pTrcServer->traceMethodLeave(
                /* strNameSpace  */ m_strNameSpace,
                /* strClassName  */ m_strClassName,
                /* strObjName    */ m_strObjName,
                /* strMethod     */ m_strMethod,
                /* strMthReturn  */ m_strMethodReturn,
                /* strMthOutArgs */ m_strMethodOutArgs );
        }
    }
    else if( m_pTrcMthFile != nullptr )
    {
        if( m_iTrcDetailLevel >= m_iEnterLeaveFilterDetailLevel )
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
    m_pTrcServer = nullptr;
    m_pTrcMthFile = nullptr;
    m_iTrcDetailLevel = 0;
    m_iEnterLeaveFilterDetailLevel = 0;
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
/*! This method has to be called if the CMethodTracer is used in the destructor
    of a class right before the trace admin object is released.

    "onAdminObjAboutToBeReleased" will trace leaving the destructor.
*/
void CMethodTracer::onAdminObjAboutToBeReleased()
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcAdminObj,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAdminObjDestroyed(QObject*)) );

        if( m_pTrcAdminObj->isActive(m_iEnterLeaveFilterDetailLevel) )
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

        m_pTrcAdminObj = nullptr;

    } // if( m_pTrcAdminObj != nullptr )

} // onAdminObjAboutToBeReleased

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the current detail level either set at the trace admin object or
    at the method tracer itself.

    @return Current trace detail level.
*/
int CMethodTracer::getTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    int iDetailLevel = m_iTrcDetailLevel;

    if( m_pTrcAdminObj != nullptr )
    {
        iDetailLevel = m_pTrcAdminObj->getTraceDetailLevel();
    }
    return iDetailLevel;
}

//------------------------------------------------------------------------------
/*! Returns the detail level which was passed to the constructor.

    When leaving the method and the method tracer is destroyed the same detail
    level must be used as when entering the method.

    @return Detail level used for entering and leaving a method.
*/
int CMethodTracer::getEnterLeaveFilterDetailLevel() const
//------------------------------------------------------------------------------
{
    return m_iEnterLeaveFilterDetailLevel;
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
/*! Returns whether tracing is active for the given filter detail level.

    E.g. if or the trace admin object the detail level MethodArgs is set and
    "isActive" will return false for detail levels greater than MethodArgs.

    @param i_iFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.

    @return true if tracing is active for the given filter detail level.
            false otherwise.
*/
bool CMethodTracer::isActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bActive = false;

    if( m_pTrcAdminObj != nullptr )
    {
        bActive = m_pTrcAdminObj->isActive(i_iFilterDetailLevel);
    }
    else
    {
        // Without trace admin object the caller must decide whether tracing is active.
        bActive = (m_iTrcDetailLevel >= i_iFilterDetailLevel);
    }
    return bActive;

} // isActive

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Adds a trace output string.

    This method is used to add additional runtime information to the trace output.

    @param i_strAddInfo [in]
        Additional runtime information to be output.
    @param i_iFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.
*/
void CMethodTracer::trace(
    const QString& i_strAddInfo,
    int            i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        if( m_pTrcAdminObj->isActive(i_iFilterDetailLevel) )
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

    else if( m_pTrcServer != nullptr )
    {
        if( m_iTrcDetailLevel >= i_iFilterDetailLevel )
        {
            m_pTrcServer->traceMethod(
                /* strNameSpace */ m_strNameSpace,
                /* strClassName */ m_strClassName,
                /* strObjName   */ m_strObjName,
                /* strMethod    */ m_strMethod,
                /* strAddInfo   */ i_strAddInfo );
        }
    } // if( m_pTrcMthFile != nullptr )

    else if( m_pTrcMthFile != nullptr )
    {
        if( m_iTrcDetailLevel >= i_iFilterDetailLevel )
        {
            QString strMth = buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);

            if( !strMth.isEmpty() && !m_strMethod.isEmpty() )
            {
                strMth += "." + m_strMethod;
            }
            m_pTrcMthFile->traceMethod(strMth, i_strAddInfo);
        }
    } // if( m_pTrcMthFile != nullptr )

} // trace

//------------------------------------------------------------------------------
/*! Adds a trace output string.

    This method is used to add additional runtime information to the trace output
    if the trace detail level is set at the class to be traced itself.

    @param i_strAddInfo [in]
        Additional runtime information to be output.
    @param i_iTrcDetailLevel [in]
        Current trace detail level to be compared against the filter detail level.
    @param i_iFilterDetailLevel [in]
        Trace outputs should be generated if the given filter detail level
        is greater or equal than the current detail level set at the trace
        admin object or at the method tracer itself.
*/
void CMethodTracer::trace(
    const QString& i_strAddInfo,
    int            i_iTrcDetailLevel,
    int            i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    if( m_pTrcAdminObj != nullptr )
    {
        int iTrcDetailLevel = i_iTrcDetailLevel;

        if( iTrcDetailLevel == ETraceDetailLevelUndefined )
        {
            iTrcDetailLevel = m_pTrcAdminObj->getTraceDetailLevel();
        }

        if( m_pTrcAdminObj->isEnabled() && iTrcDetailLevel >= i_iFilterDetailLevel )
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

    else if( m_pTrcServer != nullptr )
    {
        int iTrcDetailLevel = i_iTrcDetailLevel;

        if( iTrcDetailLevel == ETraceDetailLevelUndefined )
        {
            iTrcDetailLevel = m_iTrcDetailLevel;
        }

        if( iTrcDetailLevel >= i_iFilterDetailLevel )
        {
            m_pTrcServer->traceMethod(
                /* strNameSpace */ m_strNameSpace,
                /* strClassName */ m_strClassName,
                /* strObjName   */ m_strObjName,
                /* strMethod    */ m_strMethod,
                /* strAddInfo   */ i_strAddInfo );
        }
    } // if( m_pTrcMthFile != nullptr )

    else if( m_pTrcMthFile != nullptr )
    {
        int iTrcDetailLevel = i_iTrcDetailLevel;

        if( iTrcDetailLevel == ETraceDetailLevelUndefined )
        {
            iTrcDetailLevel = m_iTrcDetailLevel;
        }

        if( iTrcDetailLevel >= i_iFilterDetailLevel )
        {
            QString strMth = buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);

            if( !strMth.isEmpty() && !m_strMethod.isEmpty() )
            {
                strMth += "." + m_strMethod;
            }
            m_pTrcMthFile->traceMethod(strMth, i_strAddInfo);
        }
    } // if( m_pTrcMthFile != nullptr )

} // trace

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! If trace admin objects are used to control the detail level of the trace outputs
    those admin objects may be destroyed while the method tracer accesssing those
    admin objects is still alive.

    To ensure that the method tracer does not acces the destroyed instance anymore
    this slot method is connected to the destroyed signal of the trace admin object.
*/
void CMethodTracer::onAdminObjDestroyed( QObject* /*i_pObj*/ )
//------------------------------------------------------------------------------
{
     m_pTrcAdminObj = nullptr;
}
