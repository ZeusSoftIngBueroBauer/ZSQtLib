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

#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysException.h"

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
            CTrcServer* pTrcServer = CTrcServer::GetInstance();

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

    } // if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= m_iEnterLeaveFilterDetailLevel )

} // ctor

//------------------------------------------------------------------------------
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
int CMethodTracer::getTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    int iDetailLevel = m_iTrcDetailLevel;

    if( m_pTrcAdminObj != nullptr )
    {
        iDetailLevel = m_pTrcAdminObj->getTraceDetailLevel();
    }
    return iDetailLevel;

} // getTraceDetailLevel

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CMethodTracer::getNameSpace() const
//------------------------------------------------------------------------------
{
    QString strNameSpace = m_strNameSpace;

    if( m_pTrcAdminObj != nullptr )
    {
        strNameSpace = m_pTrcAdminObj->getNameSpace();
    }
    return strNameSpace;

} // getNameSpace

//------------------------------------------------------------------------------
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

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMethodTracer::setMethodReturn( bool i_bResult )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = bool2Str(i_bResult);
}

//------------------------------------------------------------------------------
void CMethodTracer::setMethodReturn( int i_iResult )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = QString::number(i_iResult);
}

//------------------------------------------------------------------------------
void CMethodTracer::setMethodReturn( EResult i_result )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = result2Str(i_result);
}

//------------------------------------------------------------------------------
void CMethodTracer::setMethodReturn( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = i_errResultInfo.toString();
}

//------------------------------------------------------------------------------
void CMethodTracer::setMethodReturn( const QString& i_str )
//------------------------------------------------------------------------------
{
    m_strMethodReturn = i_str;
}

//------------------------------------------------------------------------------
void CMethodTracer::setMethodOutArgs( const QString& i_str )
//------------------------------------------------------------------------------
{
    m_strMethodOutArgs = i_str;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
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

        } // if( m_iTrcDetailLevel >= m_iEnterLeaveFilterDetailLevel )
    } // if( m_pTrcMthFile != nullptr )

} // trace

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMethodTracer::onAdminObjDestroyed( QObject* /*i_pObj*/ )
//------------------------------------------------------------------------------
{
     m_pTrcAdminObj = nullptr;
}
