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
#include <QtCore/qwaitcondition.h>

#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/******************************************************************************/
class CInitModuleSysRequest
/* Please note:
   The class name should be unique for the whole system. Otherwise the compiler
   may be confused and using a CInitModule class from other modules to create
   the static InitModule instance.
*******************************************************************************/
{
public: // ctor
    CInitModuleSysRequest()
    {
        qRegisterMetaType<SRequestDscr>("SRequestDscr");
        qRegisterMetaType<ZS::System::SRequestDscr>("ZS::System::SRequestDscr");
    }
};

static CInitModuleSysRequest s_initModule;


/*******************************************************************************
struct SRequestDscr
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SRequestDscr::SRequestDscr() :
//------------------------------------------------------------------------------
    m_iId(-1),
    m_pObjCreator(nullptr),
    m_strCreatorNameSpace(),
    m_strCreatorClassName(),
    m_strCreatorObjName(),
    m_strCreatorThreadName(),
    m_pThread(nullptr),
    m_strThreadName(),
    m_request(-1),
    m_strRequest(),
    m_strDscr(),
    m_strToolTip(),
    m_iLockCount(0),
    m_iProgress_perCent(0),
    m_errResultInfo(ErrResultUndefined),
    m_valResult(),
    m_pMsgConResult(nullptr),
    m_pvResultData(nullptr),
    m_objStateAborting(EObjState::Detached),
    m_objState(EObjState::Detached),
    m_bIsBlocking(false),
    m_iTimeout_ms(0),
    m_fTimeStart_s(0.0),
    m_fTimeEnd_s(0.0),
    m_iParentId(-1),
    m_ariChildIds()
{
} // default ctor

//------------------------------------------------------------------------------
SRequestDscr::SRequestDscr(
    qint64         i_iId,
    const QString& i_strCreatorNameSpace,
    const QString& i_strCreatorClassName,
    QObject*       i_pObjCreator,
    int            i_request,
    const QString& i_strRequest,
    qint64         i_iParentId,
    int            i_iTimeout_ms,
    bool           i_bIsBlocking ) :
//------------------------------------------------------------------------------
    m_iId(i_iId),
    m_pObjCreator(i_pObjCreator),
    m_strCreatorNameSpace(i_strCreatorNameSpace),
    m_strCreatorClassName(i_strCreatorClassName),
    m_strCreatorObjName(),
    m_strCreatorThreadName(),
    m_pThread(nullptr),
    m_strThreadName(),
    m_request(i_request),
    m_strRequest(i_strRequest),
    m_strDscr(),
    m_strToolTip(),
    m_iLockCount(0),
    m_iProgress_perCent(0),
    m_errResultInfo(ErrResultUndefined),
    m_valResult(),
    m_pMsgConResult(nullptr),
    m_pvResultData(nullptr),
    m_objStateAborting(EObjState::Detached),
    m_objState(EObjState::Detached),
    m_bIsBlocking(i_bIsBlocking),
    m_iTimeout_ms(i_iTimeout_ms),
    m_fTimeStart_s(0.0),
    m_fTimeEnd_s(0.0),
    m_iParentId(i_iParentId),
    m_ariChildIds()
{
    if( m_pObjCreator != nullptr )
    {
        m_strCreatorObjName = m_pObjCreator->objectName();

        if( m_pObjCreator->thread() != nullptr )
        {
            m_strCreatorThreadName = m_pObjCreator->thread()->objectName();
        }
    }

    m_errResultInfo.setErrSource(m_strCreatorNameSpace, m_strCreatorClassName, m_strCreatorObjName, m_strRequest);

} // ctor

//------------------------------------------------------------------------------
SRequestDscr::SRequestDscr( const SRequestDscr& i_other ) :
//------------------------------------------------------------------------------
    m_iId(i_other.m_iId),
    m_pObjCreator(i_other.m_pObjCreator),
    m_strCreatorNameSpace(i_other.m_strCreatorNameSpace),
    m_strCreatorClassName(i_other.m_strCreatorClassName),
    m_strCreatorObjName(i_other.m_strCreatorObjName),
    m_strCreatorThreadName(i_other.m_strCreatorThreadName),
    m_pThread(i_other.m_pThread),
    m_strThreadName(i_other.m_strThreadName),
    m_request(i_other.m_request),
    m_strRequest(i_other.m_strRequest),
    m_strDscr(i_other.m_strDscr),
    m_strToolTip(i_other.m_strToolTip),
    m_iLockCount(i_other.m_iLockCount),
    m_iProgress_perCent(i_other.m_iProgress_perCent),
    m_errResultInfo(i_other.m_errResultInfo),
    m_valResult(i_other.m_valResult),
    m_pMsgConResult(nullptr),  // (i_other.m_pMsgConResult),
    m_pvResultData(nullptr),   // (i_other.m_pvResultData),
    m_objStateAborting(i_other.m_objStateAborting),
    m_objState(i_other.m_objState),
    m_bIsBlocking(i_other.m_bIsBlocking),
    m_iTimeout_ms(i_other.m_iTimeout_ms),
    m_fTimeStart_s(i_other.m_fTimeStart_s),
    m_fTimeEnd_s(i_other.m_fTimeEnd_s),
    m_iParentId(i_other.m_iParentId),
    m_ariChildIds(i_other.m_ariChildIds)
{
} // copy ctor

//------------------------------------------------------------------------------
SRequestDscr::~SRequestDscr()
//------------------------------------------------------------------------------
{
    m_iId = 0;
    m_pObjCreator = 0;
    //m_strCreatorNameSpace;
    //m_strCreatorClassName;
    //m_strCreatorObjName;
    //m_strCreatorThreadName;
    m_pThread = nullptr;
    //m_strThreadName;
    m_request = 0;
    //m_strRequest;
    //m_strDscr;
    //m_strToolTip;
    m_iLockCount = 0;
    m_iProgress_perCent = 0;
    //m_errResultInfo;
    //m_valResult;
    m_pMsgConResult = nullptr;
    m_pvResultData = nullptr;
    m_objStateAborting = static_cast<EObjState>(0);
    m_objState = static_cast<EObjState>(0);
    m_bIsBlocking = false;
    m_iTimeout_ms = 0;
    m_fTimeStart_s = 0.0;
    m_fTimeEnd_s = 0.0;
    m_iParentId = 0;
    //m_ariChildIds.clear();

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SRequestDscr& SRequestDscr::operator = ( const SRequestDscr& i_other )
//------------------------------------------------------------------------------
{
    m_iId = i_other.m_iId;
    m_pObjCreator = i_other.m_pObjCreator;
    m_strCreatorNameSpace = i_other.m_strCreatorNameSpace;
    m_strCreatorClassName = i_other.m_strCreatorClassName;
    m_strCreatorObjName = i_other.m_strCreatorObjName;
    m_strCreatorThreadName = i_other.m_strCreatorThreadName;
    m_pThread = i_other.m_pThread;
    m_strThreadName = i_other.m_strThreadName;
    m_request = i_other.m_request;
    m_strRequest = i_other.m_strRequest;
    m_strDscr = i_other.m_strDscr;
    m_strToolTip = i_other.m_strToolTip;
    m_iLockCount = i_other.m_iLockCount;
    m_iProgress_perCent = i_other.m_iProgress_perCent;
    m_errResultInfo = i_other.m_errResultInfo;
    m_valResult = i_other.m_valResult;
    //m_pMsgConResult = i_other.m_pMsgConResult;
    //m_pvResultData = i_other.m_pvResultData;
    m_objStateAborting = i_other.m_objStateAborting;
    m_objState = i_other.m_objState;
    m_bIsBlocking = i_other.m_bIsBlocking;
    m_iTimeout_ms = i_other.m_iTimeout_ms;
    m_fTimeStart_s = i_other.m_fTimeStart_s;
    m_fTimeEnd_s = i_other.m_fTimeEnd_s;
    m_iParentId = i_other.m_iParentId;
    m_ariChildIds = i_other.m_ariChildIds;

    return *this;

} // operator =

//------------------------------------------------------------------------------
bool SRequestDscr::operator == ( const SRequestDscr& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = false;

    // Comparing in an optimized order ...
    if( m_iId != i_other.m_iId )
    {
        bEqual = false;
    }
    else if( m_iLockCount != i_other.m_iLockCount )
    {
        bEqual = false;
    }
    else if( m_iProgress_perCent != i_other.m_iProgress_perCent )
    {
        bEqual = false;
    }
    else if( m_errResultInfo != i_other.m_errResultInfo )
    {
        bEqual = false;
    }
    else if( m_bIsBlocking != i_other.m_bIsBlocking )
    {
        bEqual = false;
    }
    else if( m_iTimeout_ms != i_other.m_iTimeout_ms )
    {
        bEqual = false;
    }
    else if( m_fTimeStart_s != i_other.m_fTimeStart_s )
    {
        bEqual = false;
    }
    else if( m_fTimeEnd_s != i_other.m_fTimeEnd_s )
    {
        bEqual = false;
    }
    else if( m_valResult != i_other.m_valResult )
    {
        bEqual = false;
    }
    else if( m_pMsgConResult != i_other.m_pMsgConResult )
    {
        bEqual = false;
    }
    else if( m_pvResultData != i_other.m_pvResultData )
    {
        bEqual = false;
    }
    else if( m_objStateAborting != i_other.m_objStateAborting )
    {
        bEqual = false;
    }
    else if( m_objState != i_other.m_objState )
    {
        bEqual = false;
    }
    else if( m_pObjCreator != i_other.m_pObjCreator )
    {
        bEqual = false;
    }
    else if( m_pThread != i_other.m_pThread )
    {
        bEqual = false;
    }
    else if( m_strDscr != i_other.m_strDscr )
    {
        bEqual = false;
    }
    else if( m_strToolTip != i_other.m_strToolTip )
    {
        bEqual = false;
    }
    else if( m_strCreatorNameSpace != i_other.m_strCreatorNameSpace )
    {
        bEqual = false;
    }
    else if( m_strCreatorClassName != i_other.m_strCreatorClassName )
    {
        bEqual = false;
    }
    else if( m_strCreatorObjName != i_other.m_strCreatorObjName )
    {
        bEqual = false;
    }
    else if( m_strCreatorThreadName != i_other.m_strCreatorThreadName )
    {
        bEqual = false;
    }
    else if( m_strThreadName != i_other.m_strThreadName )
    {
        bEqual = false;
    }
    else if( m_request != i_other.m_request )
    {
        bEqual = false;
    }
    else if( m_strRequest != i_other.m_strRequest )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SRequestDscr::operator != ( const SRequestDscr& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
void SRequestDscr::initFrom( const SRequestDscr& i_other )
//------------------------------------------------------------------------------
{
    m_iId = i_other.m_iId;
    m_pObjCreator = i_other.m_pObjCreator;
    m_strCreatorNameSpace = i_other.m_strCreatorNameSpace;
    m_strCreatorClassName = i_other.m_strCreatorClassName;
    m_strCreatorObjName = i_other.m_strCreatorObjName;
    m_strCreatorThreadName = i_other.m_strCreatorThreadName;
    m_pThread = i_other.m_pThread;
    m_strThreadName = i_other.m_strThreadName;
    m_request = i_other.m_request;
    m_strRequest = i_other.m_strRequest;
    m_strDscr = i_other.m_strDscr;
    m_strToolTip = i_other.m_strToolTip;
    m_iLockCount = i_other.m_iLockCount;
    m_iProgress_perCent = i_other.m_iProgress_perCent;
    m_errResultInfo = i_other.m_errResultInfo;
    m_valResult = i_other.m_valResult;
    //m_pMsgConResult = i_other.m_pMsgConResult;
    //m_pvResultData = i_other.m_pvResultData;
    m_objStateAborting = i_other.m_objStateAborting;
    m_objState = i_other.m_objState;
    m_bIsBlocking = i_other.m_bIsBlocking;
    m_iTimeout_ms = i_other.m_iTimeout_ms;
    m_fTimeStart_s = i_other.m_fTimeStart_s;
    m_fTimeEnd_s = i_other.m_fTimeEnd_s;
    //m_iParentId = i_other.m_iParentId;
    //m_ariChildIds = i_other.m_ariChildIds;

} // initFrom

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SRequestDscr::getAddTrcInfoStr( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( i_iDetailLevel >= 1 )
    {
        if( i_iDetailLevel >= 2 )
        {
            if( !m_strCreatorNameSpace.isEmpty() )
            {
                strAddTrcInfo += m_strCreatorNameSpace;
                if( !m_strCreatorClassName.isEmpty() )
                {
                    strAddTrcInfo += "::";
                }
            }
        }
        if( !m_strCreatorClassName.isEmpty() )
        {
            strAddTrcInfo += m_strCreatorClassName;
            if( !m_strCreatorObjName.isEmpty() )
            {
                strAddTrcInfo += "::";
            }
        }
        if( !m_strCreatorObjName.isEmpty() )
        {
            strAddTrcInfo += m_strCreatorObjName;
            strAddTrcInfo += ".";
        }
    } // if( i_iDetailLevel >= 1 )

    if( m_strRequest.isEmpty() )
    {
        if( m_request >= 0 )
        {
            strAddTrcInfo += "UnnamedReqNr" + QString::number(m_request);
        }
        else
        {
            strAddTrcInfo += "None";
        }
    }
    else
    {
        strAddTrcInfo += m_strRequest;
    }

    if( m_iId >= 0 )
    {
        strAddTrcInfo += " {";

        strAddTrcInfo += "Progress: " + QString::number(m_iProgress_perCent);
        strAddTrcInfo += ", Result: " + m_errResultInfo.getResultStr();

        if( i_iDetailLevel == 1 && m_objState != EObjState::Created )
        {
            strAddTrcInfo += ", ObjState: " + CEnumObjState::toString(m_objState);
        }

        if( i_iDetailLevel >= 2 )
        {
            // If the content of the request is added to the error log model the
            // same request may be added many times. The difference is only the Id.
            strAddTrcInfo += ", Id: " + QString::number(m_iId);
            strAddTrcInfo += ", Thread: " + m_strCreatorThreadName;
            strAddTrcInfo += ", LockCount: " + QString::number(m_iLockCount);

            switch( m_objStateAborting )
            {
                case EObjState::Destroying:
                {
                    strAddTrcInfo += ", Aborting";
                    break;
                }
                case EObjState::Destroyed:
                {
                    strAddTrcInfo += ", Aborted";
                    break;
                }
                case EObjState::Detached:
                default:
                {
                    break;
                }
            }

            strAddTrcInfo += ", ObjState: " + CEnumObjState::toString(m_objState);
            strAddTrcInfo += ", IsBlocking: " + bool2Str(m_bIsBlocking);
            strAddTrcInfo += ", Timeout: " + QString::number(m_iTimeout_ms) + " ms";
            strAddTrcInfo += ", ResultValue: " + m_valResult.toString();
            strAddTrcInfo += ", ResultConMsg: " + QString( m_pMsgConResult == nullptr ? "nullptr" : m_pMsgConResult->getAddTrcInfoStr(i_iDetailLevel) );
            strAddTrcInfo += ", ResultData: " + pointer2Str(m_pvResultData);
        }

        strAddTrcInfo += "}";

    } // if( m_iId >= 0 )

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CRequest
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

// Should be protected class members.
// But python bindings generated with shiboken will not be compilable then.

static qint64 s_iId = 0;
static QRecursiveMutex s_mtxId;

/*==============================================================================
public: // Macros
==============================================================================*/

//------------------------------------------------------------------------------
bool ZS::System::isAsynchronousRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    bool bIsAsynchronous = false;

    if( i_pReq != nullptr )
    {
        bIsAsynchronous = i_pReq->isAsynchronousRequest();
    }
    return bIsAsynchronous;

} // isAsynchronousRequest

//------------------------------------------------------------------------------
bool ZS::System::deleteResultConfirmationMessage( CRequest* i_pReq, QObject* i_pObjMsgReceiver )
//------------------------------------------------------------------------------
{
    bool bDeleted = false;

    if( i_pReq != nullptr )
    {
        CMsgCon* pMsgConResult = i_pReq->getResultConfirmationMessage();

        if( pMsgConResult != nullptr && pMsgConResult->getReceiver() == i_pObjMsgReceiver )
        {
            i_pReq->takeResultConfirmationMessage();
            delete pMsgConResult;
            pMsgConResult = nullptr;
        }
    }
    return bDeleted;

} // deleteResultConfirmationMessage

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
qint64 CRequest::GetUniqueRequestId()
//------------------------------------------------------------------------------
{
    s_mtxId.lock();
    ++s_iId;
    s_mtxId.unlock();
    return s_iId;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRequest::CRequest(
    const QString& i_strCreatorNameSpace,
    const QString& i_strCreatorClassName,
    QObject*       i_pObjCreator,
    int            i_request,
    const QString& i_strRequest,
    qint64         i_iParentId,
    bool           i_bKeepReqDscrInExecTree,
    int            i_iTimeout_ms,
    bool           i_bIsBlocking ) :
//------------------------------------------------------------------------------
    QObject(),
    m_dscr(
        /* iId                      */ GetUniqueRequestId(),
        /* strCreatorNameSpace */ i_strCreatorNameSpace,
        /* strCreatorClassName      */ i_strCreatorClassName,
        /* pObjCreator              */ i_pObjCreator,
        /* request                  */ i_request,
        /* strRequest               */ i_strRequest,
        /* iParentId                */ i_iParentId,
        /* iTimeout_ms              */ i_iTimeout_ms,
        /* bIsBlocking              */ i_bIsBlocking ),
    m_pMtxWaitCondition(nullptr),
    m_pWaitCondition(nullptr),
    m_bIsWaiting(false),
    m_bWokenUp(false),
    m_bWaitTimedOut(false),
    m_bChanged(false),
    m_bKeepReqDscrInExecTree(i_bKeepReqDscrInExecTree),
    m_hshvalsExec(),
    m_hshpvsExec(),
    m_pMsgReqExec(nullptr),
    m_pMsgConExec(nullptr),
    m_pMsgIndExec(nullptr),
    m_pMsgReqInitiator(nullptr),
    m_pMsgConInitiator(nullptr),
    m_pMsgIndInitiator(nullptr)
{
    // Only if the request execution tree instance has been created parent/child relationships of
    // requests are available. In addition the tree instance must be used to access requests from
    // within other threads than the request's creator thread.
    if( CRequestExecTree::GetInstance() == nullptr )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            // If the request execution tree instance has not yet been created it will be dynamically
            // created here. But very likely the singleton class will not be deleted and will therefore
            // be reported as a memory when shutting down the application.
            QString strAddErrInfo = "Request exection tree instance not existing.";
            SErrResultInfo errResultInfo = ErrResultInfoWarning("ctor", EResultSingletonClassNotInstantiated, strAddErrInfo);
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }

        CRequestExecTree::CreateInstance();
    }

    if( m_dscr.m_pObjCreator != nullptr )
    {
        QObject::connect(
            m_dscr.m_pObjCreator, &QObject::destroyed,
            this, &CRequest::onCreatorDestroyed,
            Qt::DirectConnection);
    }

    QString strObjName;

    if( m_dscr.m_strCreatorClassName.isEmpty() )
    {
        setObjectName(m_dscr.m_strRequest);
    }
    else
    {
        if( m_dscr.m_strCreatorNameSpace.isEmpty() )
        {
            setObjectName( m_dscr.m_strCreatorClassName + "::" + m_dscr.m_strRequest );
        }
        else
        {
            setObjectName( m_dscr.m_strCreatorNameSpace + "::" + m_dscr.m_strCreatorClassName + "::" + m_dscr.m_strRequest );
        }
    }

    m_dscr.m_pThread = QThread::currentThread();

    if( m_dscr.m_pObjCreator != nullptr )
    {
        m_dscr.m_strCreatorObjName = m_dscr.m_pObjCreator->objectName();

        if( m_dscr.m_pObjCreator->thread() == nullptr )
        {
            m_dscr.m_strCreatorThreadName = "nullptr";
        }
        else // if( m_dscr.m_pObjCreator->thread() != nullptr )
        {
            m_dscr.m_strCreatorThreadName = m_dscr.m_pObjCreator->thread()->objectName();

            if( m_dscr.m_strCreatorThreadName.isEmpty() && m_dscr.m_pObjCreator->thread() == m_dscr.m_pThread )
            {
                m_dscr.m_strCreatorThreadName = QString("Thread") + threadId2Str(QThread::currentThreadId());
            }
        } // if( m_dscr.m_pObjCreator->thread() != nullptr )
    } // if( m_dscr.m_pObjCreator != nullptr )

    if( m_dscr.m_pThread == nullptr )
    {
        m_dscr.m_strThreadName = "nullptr";
    }
    else // if( m_dscr.m_pThread != nullptr )
    {
        m_dscr.m_strThreadName = m_dscr.m_pThread->objectName();

        if( m_dscr.m_strThreadName.isEmpty() )
        {
            m_dscr.m_strThreadName = QString("Thread") + threadId2Str(QThread::currentThreadId());
        }

        QObject::connect(
            m_dscr.m_pThread, &QThread::finished,
            this, &CRequest::onThreadFinished,
            Qt::DirectConnection);
    }

    m_dscr.m_objState = EObjState::Created;

    m_dscr.m_fTimeStart_s = ZS::System::Time::getProcTimeInSec();

    if( m_dscr.m_bIsBlocking )
    {
        m_pMtxWaitCondition = new QMutex();
        m_pWaitCondition = new QWaitCondition();
    }

    CRequestExecTree::GetInstance()->addRequest(this);

} // ctor

//------------------------------------------------------------------------------
CRequest::CRequest( const SRequestDscr& i_reqDscr ) :
//------------------------------------------------------------------------------
    QObject(),
    m_dscr(),
    m_pMtxWaitCondition(nullptr),
    m_pWaitCondition(nullptr),
    m_bIsWaiting(false),
    m_bWokenUp(false),
    m_bWaitTimedOut(false),
    m_bChanged(false),
    m_bKeepReqDscrInExecTree(false),
    m_hshvalsExec(),
    m_hshpvsExec(),
    m_pMsgReqExec(nullptr),
    m_pMsgConExec(nullptr),
    m_pMsgIndExec(nullptr),
    m_pMsgReqInitiator(nullptr),
    m_pMsgConInitiator(nullptr),
    m_pMsgIndInitiator(nullptr)
{
    // Only if the request execution tree instance has been created parent/child relationships of
    // requests are available. In addition the tree instance must be used to access requests from
    // within other threads than the request's creator thread.
    if( CRequestExecTree::GetInstance() == nullptr )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            // If the request execution tree instance has not yet been created it will be dynamically
            // created here. But very likely the singleton class will not be deleted and will therefore
            // be reported as a memory when shutting down the application.
            QString strAddErrInfo = "Request exection tree instance not existing.";
            SErrResultInfo errResultInfo = ErrResultInfoWarning("ctor", EResultSingletonClassNotInstantiated, strAddErrInfo);
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    // The request descriptor (if provided by the request sequencer) may have a list of child requests.
    // The request must not taken over this list.
    m_dscr.initFrom(i_reqDscr);
    m_dscr.m_iParentId = i_reqDscr.m_iParentId;

    m_dscr.m_iId = ++s_iId;

    if( m_dscr.m_pObjCreator != nullptr )
    {
        QObject::connect(
            m_dscr.m_pObjCreator, &QObject::destroyed,
            this, &CRequest::onCreatorDestroyed,
            Qt::DirectConnection);
    }

    QString strObjName;

    if( m_dscr.m_strCreatorClassName.isEmpty() )
    {
        setObjectName(m_dscr.m_strRequest);
    }
    else
    {
        if( m_dscr.m_strCreatorNameSpace.isEmpty() )
        {
            setObjectName( m_dscr.m_strCreatorClassName + "::" + m_dscr.m_strRequest );
        }
        else
        {
            setObjectName( m_dscr.m_strCreatorNameSpace + "::" + m_dscr.m_strCreatorClassName + "::" + m_dscr.m_strRequest );
        }
    }

    m_dscr.m_pThread = QThread::currentThread();

    if( m_dscr.m_pObjCreator != nullptr )
    {
        m_dscr.m_strCreatorObjName = m_dscr.m_pObjCreator->objectName();

        if( m_dscr.m_pObjCreator->thread() == nullptr )
        {
            m_dscr.m_strCreatorThreadName = "nullptr";
        }
        else // if( m_dscr.m_pObjCreator->thread() != nullptr )
        {
            m_dscr.m_strCreatorThreadName = m_dscr.m_pObjCreator->thread()->objectName();

            if( m_dscr.m_strCreatorThreadName.isEmpty() && m_dscr.m_pObjCreator->thread() == m_dscr.m_pThread )
            {
                m_dscr.m_strCreatorThreadName = QString("Thread") + threadId2Str(QThread::currentThreadId());
            }
        } // if( m_dscr.m_pObjCreator->thread() != nullptr )
    } // if( m_dscr.m_pObjCreator != nullptr )

    if( m_dscr.m_pThread == nullptr )
    {
        m_dscr.m_strThreadName = "nullptr";
    }
    else // if( m_dscr.m_pThread != nullptr )
    {
        m_dscr.m_strThreadName = m_dscr.m_pThread->objectName();

        if( m_dscr.m_strThreadName.isEmpty() )
        {
            m_dscr.m_strThreadName = QString("Thread") + threadId2Str(QThread::currentThreadId());
        }
        QObject::connect(
            m_dscr.m_pThread, &QThread::finished,
            this, &CRequest::onThreadFinished,
            Qt::DirectConnection);
    }

    m_dscr.m_objState = EObjState::Created;

    m_dscr.m_fTimeStart_s = ZS::System::Time::getProcTimeInSec();

    if( m_dscr.m_bIsBlocking )
    {
        m_pMtxWaitCondition = new QMutex();
        m_pWaitCondition = new QWaitCondition();
    }

    CRequestExecTree::GetInstance()->addRequest(this);

} // ctor

//------------------------------------------------------------------------------
CRequest::~CRequest()
//------------------------------------------------------------------------------
{
    if( isLocked() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            QString strAddErrInfo = "Deleting the locked Request: " + getAddTrcInfoStr(1);
            SErrResultInfo errResultInfo(
                /* errSource  */ NameSpace(), ClassName(), objectName(), "dtor",
                /* result     */ EResultInternalProgramError,
                /* severtiy   */ EResultSeverityCritical,
                /* strAddInfo */ strAddErrInfo );
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    m_dscr.m_objState = EObjState::Destroyed;


    try
    {
        delete m_pMtxWaitCondition;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pWaitCondition;
    }
    catch(...)
    {
    }

    // The tree instance administers the parent/child relationship of the request items.
    // The tree node will not be deleted but the reference to the request instance is set
    // to invalid (the object state will be indicated as "destroyed").
    if( CRequestExecTree::GetInstance() != nullptr )
    {
        if( m_bKeepReqDscrInExecTree )
        {
            CRequestExecTree::GetInstance()->updateRequest(m_dscr);
        }
        else
        {
            CRequestExecTree::GetInstance()->removeRequest(m_dscr.m_iId);
        }
    }

    try
    {
        delete m_pMsgReqExec;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMsgConExec;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMsgIndExec;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMsgReqInitiator;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMsgConInitiator;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMsgIndInitiator;
    }
    catch(...)
    {
    }

    // Please note that emitting signals presupposes that casting to the
    // QMetaQObject of the signal receiver is possible. If a specialized class
    // (class derived from another class) creates a request instance and connects
    // to the changed signal of the request instance make sure that the instance
    // of the derived class is still alive if the request instance emits a signal.
    // E.g. you shouldn't destroy the request in the dtor of the inherited class
    // as if the dtor of the inherited class is invoked the dtor of the derived
    // class has already been invoked and a cast to the derived class will fail.
    emit changed(m_dscr);

    //m_dscr;
    m_pMtxWaitCondition = nullptr;
    m_pWaitCondition = nullptr;
    m_bIsWaiting = false;
    m_bWokenUp = false;
    m_bWaitTimedOut = false;
    m_bChanged = false;
    m_bKeepReqDscrInExecTree = false;
    //m_hshvalsExec.clear();
    //m_hshpvsExec.clear();
    m_pMsgReqExec = nullptr;
    m_pMsgConExec = nullptr;
    m_pMsgIndExec = nullptr;
    m_pMsgReqInitiator = nullptr;
    m_pMsgConInitiator = nullptr;
    m_pMsgIndInitiator = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::setKeepReqDscrInExecTree( bool i_bKeep )
//------------------------------------------------------------------------------
{
    m_bKeepReqDscrInExecTree = i_bKeep;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CRequest::lock()
//------------------------------------------------------------------------------
{
    return ++m_dscr.m_iLockCount;
}

//------------------------------------------------------------------------------
int CRequest::unlock()
//------------------------------------------------------------------------------
{
    if( m_dscr.m_iLockCount <= 0 )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "Lock Count is already 0" );
    }
    return --m_dscr.m_iLockCount;

} // unlock

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequest::wait()
//------------------------------------------------------------------------------
{
    if( m_pMtxWaitCondition != nullptr && m_pWaitCondition != nullptr )
    {
        m_pMtxWaitCondition->lock();

        if( !m_bWokenUp )
        {
            if( m_dscr.m_bIsBlocking && isAsynchronousRequest() )
            {
                m_bWaitTimedOut = false;

                //double fSysTimeBeforeWait_ms = 0.0;
                //double fSysTimeAfterWait_ms  = 0.0;

                if( isAsynchronousRequest() )
                {
                    // 0: Infinite (No Timeout), < 0 (-1): Use default from somewhere else (connection setting etc.)
                    unsigned long uTimeout_ms = ULONG_MAX;

                    if( m_dscr.m_iTimeout_ms > 0 )
                    {
                        uTimeout_ms = static_cast<unsigned long>(m_dscr.m_iTimeout_ms);
                    }

                    m_bIsWaiting = true;
                    //fSysTimeBeforeWait_ms = System::Time::getProcTimeInMilliSec();
                    m_bWaitTimedOut = !m_pWaitCondition->wait(m_pMtxWaitCondition, uTimeout_ms);
                    //fSysTimeAfterWait_ms = System::Time::getProcTimeInMilliSec();
                    m_bIsWaiting = false;
                }
            } // if( m_dscr.m_bIsBlocking && isAsynchronousRequest() )
        } // if( !m_bWokenUp )

        m_pMtxWaitCondition->unlock();

    } // if( m_pMtxWaitCondition != nullptr && m_pWaitCondition != nullptr )

    m_bWokenUp = false;

    return !m_bWaitTimedOut;

} // wait

//------------------------------------------------------------------------------
bool CRequest::wake()
//------------------------------------------------------------------------------
{
    bool m_bWokenUp = false;

    if( m_bIsWaiting && m_pWaitCondition != nullptr )
    {
        try
        {
            m_pWaitCondition->wakeOne();
        }
        catch(...)
        {
        }
    }

    m_bWokenUp = true;

    return m_bWokenUp;

} // wake

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::update()
//------------------------------------------------------------------------------
{
    if( m_dscr.m_pObjCreator != nullptr )
    {
        QThread* pThreadRequest = m_dscr.m_pThread;
        QThread* pThread = QThread::currentThread();

        if( pThreadRequest != pThread )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                QString strAddErrInfo;

                strAddErrInfo  = "Current thread: " + pThread->objectName();
                strAddErrInfo += ", Request creating thread: " + pThreadRequest->objectName();
                strAddErrInfo += ": Requests should only be changed from within the thread in which the request was created";

                SErrResultInfo errResultInfo(
                    /* errSource  */ NameSpace(), ClassName(), objectName(), "update",
                    /* result     */ EResultInvalidMethodCall,
                    /* severity   */ EResultSeverityWarning,
                    /* strAddInfo */ strAddErrInfo );
                CErrLog::GetInstance()->addEntry(errResultInfo);

            } // if( CErrLog::GetInstance() != nullptr )
        } // if( pThreadCreator != pThread )
    } // if( m_dscr.m_pObjCreator != nullptr )

    if( m_bChanged )
    {
        m_bChanged = false;

        emit changed(m_dscr);

        if( CRequestExecTree::GetInstance() != nullptr )
        {
            CRequestExecTree::GetInstance()->updateRequest(m_dscr);
        }
    } // if( m_bChanged )

} // update

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SRequestDscr CRequest::getDescriptor( bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    SRequestDscr reqDscr = m_dscr;

    if( i_bTopMost )
    {
        if( CRequestExecTree::GetInstance() != nullptr )
        {
            reqDscr = CRequestExecTree::GetInstance()->getDescriptor(m_dscr.m_iId,i_bTopMost);
        }
    }
    return m_dscr;

} // getDescriptor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QObject* CRequest::getCreator( bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QObject* pObjCreator = m_dscr.m_pObjCreator;

    if( i_bTopMost )
    {
        if( CRequestExecTree::GetInstance() != nullptr )
        {
            pObjCreator = CRequestExecTree::GetInstance()->getCreator(m_dscr.m_iId,i_bTopMost);
        }
    }
    return pObjCreator;

} // getCreator

//------------------------------------------------------------------------------
QString CRequest::getCreatorNameSpace( bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QString strName = m_dscr.m_strCreatorNameSpace;

    if( i_bTopMost )
    {
        if( CRequestExecTree::GetInstance() != nullptr )
        {
            strName = CRequestExecTree::GetInstance()->getCreatorNameSpace(m_dscr.m_iId,i_bTopMost);
        }
    }
    return strName;

} // getCreatorNameSpace

//------------------------------------------------------------------------------
QString CRequest::getCreatorClassName( bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QString strName = m_dscr.m_strCreatorClassName;

    if( i_bTopMost )
    {
        if( CRequestExecTree::GetInstance() != nullptr )
        {
            strName = CRequestExecTree::GetInstance()->getCreatorClassName(m_dscr.m_iId,i_bTopMost);
        }
    }
    return strName;

} // getCreatorClassName

//------------------------------------------------------------------------------
QString CRequest::getCreatorThreadName( bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QString strName = m_dscr.m_strCreatorThreadName;

    if( i_bTopMost )
    {
        if( CRequestExecTree::GetInstance() != nullptr )
        {
            strName = CRequestExecTree::GetInstance()->getCreatorThreadName(m_dscr.m_iId,i_bTopMost);
        }
    }
    return strName;

} // getCreatorThreadName

//------------------------------------------------------------------------------
QString CRequest::getCreatorObjectName( bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QString strName = m_dscr.m_strCreatorObjName;

    if( i_bTopMost )
    {
        if( CRequestExecTree::GetInstance() != nullptr )
        {
            strName = CRequestExecTree::GetInstance()->getCreatorObjectName(m_dscr.m_iId,i_bTopMost);
        }
    }
    return strName;

} // getCreatorObjectName

//------------------------------------------------------------------------------
QString CRequest::getRequestName( bool i_bRecursive ) const
//------------------------------------------------------------------------------
{
    QString strName = m_dscr.m_strRequest;

    if( i_bRecursive )
    {
        if( CRequestExecTree::GetInstance() != nullptr )
        {
            strName = CRequestExecTree::GetInstance()->getRequestName(m_dscr.m_iId,i_bRecursive);
        }
    }
    return strName;

} // getRequestName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void* CRequest::takeResultData()
//------------------------------------------------------------------------------
{
    void* pvData = nullptr;

    if( m_dscr.m_pvResultData != nullptr )
    {
        pvData = m_dscr.m_pvResultData;
        m_dscr.m_pvResultData = nullptr;
        m_bChanged = true;
    }
    return pvData;

} // takeResultData

//------------------------------------------------------------------------------
CMsgCon* CRequest::takeResultConfirmationMessage()
//------------------------------------------------------------------------------
{
    CMsgCon* pMsgCon = nullptr;

    if( m_dscr.m_pMsgConResult != nullptr )
    {
        pMsgCon = m_dscr.m_pMsgConResult;
        m_dscr.m_pMsgConResult = nullptr;
        m_bChanged = true;
    }
    return pMsgCon;

} // takeResultConfirmationMessage

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void  CRequest::setIsBlockingRequest( bool i_bIsBlockig )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_bIsBlocking != i_bIsBlockig )
    {
        m_dscr.m_bIsBlocking = i_bIsBlockig;
        m_bChanged = true;
    }

} // setIsBlockingRequest

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::setTimeoutInMs( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_iTimeout_ms != i_iTimeout_ms )
    {
        m_dscr.m_iTimeout_ms = i_iTimeout_ms;
        m_bChanged = true;
    }

} // setTimeoutInMs

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CRequest::getDurationInSec() const
//------------------------------------------------------------------------------
{
    double fDuration_s = 0.0;

    if( m_dscr.m_iProgress_perCent >= 100 )
    {
        fDuration_s = m_dscr.m_fTimeEnd_s - m_dscr.m_fTimeStart_s;
    }
    else
    {
        fDuration_s = ZS::System::Time::getProcTimeInSec() - m_dscr.m_fTimeStart_s;
    }
    return fDuration_s;

} // getDurationInSec

//------------------------------------------------------------------------------
QString CRequest::getDurationAsStrInBestUnit() const
//------------------------------------------------------------------------------
{
    QString strDuration;
    double  fDuration_s = getDurationInSec();

    if( fabs(fDuration_s) <= 1.0e-6 )
    {
        strDuration = QString::number(fDuration_s*1.0e9,'f',3) + " ns";
    }
    else if( fabs(fDuration_s) <= 1.0e-3 )
    {
        strDuration = QString::number(fDuration_s*1.0e6,'f',3) + " " + Math::c_chSymbolMicro + "s";
    }
    else if( fabs(fDuration_s) <= 1.0 )
    {
        strDuration = QString::number(fDuration_s*1.0e3,'f',3) + " ms";
    }
    else if( fabs(fDuration_s) <= 1.0e3 )
    {
        strDuration = QString::number(fDuration_s,'f',3) + " s";
    }
    else
    {
        strDuration = QString::number(fDuration_s,'f',3) + " s";
    }
    return strDuration;

} // getDurationAsStrInBestUnit

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void  CRequest::setParentId( qint64 i_iParentId )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_iParentId != i_iParentId )
    {
        m_dscr.m_iParentId = i_iParentId;
        m_bChanged = true;
    }

} // setParentId

//------------------------------------------------------------------------------
qint64 CRequest::getParentId( bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    qint64 iParentId = -1;

    if( CRequestExecTree::GetInstance() != nullptr )
    {
        iParentId = CRequestExecTree::GetInstance()->getParentId(m_dscr.m_iId,i_bTopMost);
    }
    return iParentId;

} // getParentId

//------------------------------------------------------------------------------
bool CRequest::isParentOf( qint64 i_iChildId, bool i_bRecursive ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    if( CRequestExecTree::GetInstance() != nullptr )
    {
        bIs = CRequestExecTree::GetInstance()->isParentOf(m_dscr.m_iId,i_iChildId,i_bRecursive);
    }
    return bIs;

} // isParentOf

//------------------------------------------------------------------------------
bool CRequest::isChildOf( qint64 i_iParentId, bool i_bRecursive ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    if( CRequestExecTree::GetInstance() != nullptr )
    {
        bIs = CRequestExecTree::GetInstance()->isChildOf(m_dscr.m_iId,i_iParentId,i_bRecursive);
    }
    return bIs;

} // isChildOf

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::setDescription( const QString& i_strDscr )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_strDscr != i_strDscr )
    {
        m_dscr.m_strDscr = i_strDscr;
        m_bChanged = true;
    }

} // setDescription

//------------------------------------------------------------------------------
void CRequest::setToolTip( const QString& i_strToolTip )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_strToolTip != i_strToolTip )
    {
        m_dscr.m_strToolTip = i_strToolTip;
        m_bChanged = true;
    }

} // setToolTip

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::setProgressInPerCent( int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_iProgress_perCent != i_iProgress_perCent )
    {
        m_dscr.m_iProgress_perCent = i_iProgress_perCent;
        m_bChanged = true;

        if( m_dscr.m_iProgress_perCent >= 100 )
        {
            m_dscr.m_fTimeEnd_s = ZS::System::Time::getProcTimeInSec();
        }
    }

} // setProgressInPerCent

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::setErrResultInfo( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_errResultInfo != i_errResultInfo )
    {
        m_dscr.m_errResultInfo = i_errResultInfo;
        m_bChanged = true;
    }

} // setErrResultInfo

//------------------------------------------------------------------------------
void CRequest::setErrResult( const SErrResult& i_errResult )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_errResultInfo.getErrResult() != i_errResult )
    {
        m_dscr.m_errResultInfo.setErrResult(i_errResult);
        m_bChanged = true;
    }

} // setErrResult

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::setSeverity( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_errResultInfo.getSeverity() != i_severity )
    {
        m_dscr.m_errResultInfo.setSeverity(i_severity);
        m_bChanged = true;
    }

} // setSeverity

//------------------------------------------------------------------------------
void CRequest::setResult( EResult i_result )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_errResultInfo.getResult() != i_result )
    {
        m_dscr.m_errResultInfo.setResult(i_result);
        m_bChanged = true;
    }

} // setResult

//------------------------------------------------------------------------------
void CRequest::setAddErrInfoDscr( const QString& i_strAddInfo )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_errResultInfo.getAddErrInfoDscr() != i_strAddInfo )
    {
        m_dscr.m_errResultInfo.setAddErrInfoDscr(i_strAddInfo);
        m_bChanged = true;
    }

} // setAddErrInfoDscr

//------------------------------------------------------------------------------
void CRequest::setErrSource( const SErrSource& i_errSource )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_errResultInfo.getErrSource() != i_errSource )
    {
        m_dscr.m_errResultInfo.setErrSource(i_errSource);
        m_bChanged = true;
    }

} // setErrSource

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::setResultValue( const QVariant& i_val )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_valResult != i_val )
    {
        m_dscr.m_valResult = i_val;
        m_bChanged = true;
    }

} // setResultValue

//------------------------------------------------------------------------------
void CRequest::setResultConfirmationMessage( CMsgCon* i_pMsgCon )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_pMsgConResult != i_pMsgCon )
    {
        m_dscr.m_pMsgConResult = i_pMsgCon;
        m_bChanged = true;
    }

} // setResultConfirmationMessage

//------------------------------------------------------------------------------
void CRequest::setResultData( void* i_pvData )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_pvResultData != i_pvData )
    {
        m_dscr.m_pvResultData = i_pvData;
        m_bChanged = true;
    }

} // setResultData

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::setObjState( EObjState i_objState )
//------------------------------------------------------------------------------
{
    if( m_dscr.m_objState != i_objState )
    {
        m_dscr.m_objState = i_objState;
        m_bChanged = true;
    }

} // setObjState

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::abort()
//------------------------------------------------------------------------------
{
    if( m_dscr.m_objStateAborting != EObjState::Destroying && m_dscr.m_objStateAborting != EObjState::Destroyed )
    {
        m_dscr.m_objStateAborting = EObjState::Destroying;
        m_bChanged = true;

        if( !m_dscr.m_errResultInfo.isErrorResult() )
        {
            m_dscr.m_errResultInfo.setSeverity(EResultSeverityError);
            m_dscr.m_errResultInfo.setResult(EResultRequestAborted);
        }

        // Immediately update the request.
        update();
    }

} // abort

//------------------------------------------------------------------------------
void CRequest::setHasBeenAborted()
//------------------------------------------------------------------------------
{
    if( m_dscr.m_objStateAborting != EObjState::Destroyed )
    {
        m_dscr.m_objStateAborting = EObjState::Destroyed;
        m_bChanged = true;
    }

} // setHasBeenAborted

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequest::hasExecutionValueOrData() const
//------------------------------------------------------------------------------
{
    return (m_hshvalsExec.size() > 0) || (m_hshpvsExec.size() > 0);
}

//------------------------------------------------------------------------------
bool CRequest::hasExecutionValueOrData( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    return m_hshvalsExec.contains(i_strKey) || m_hshpvsExec.contains(i_strKey);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequest::hasExecutionValue() const
//------------------------------------------------------------------------------
{
    return (m_hshvalsExec.size() > 0);
}

//------------------------------------------------------------------------------
bool CRequest::hasExecutionValue( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    return m_hshvalsExec.contains(i_strKey);
}

//------------------------------------------------------------------------------
QVariant CRequest::getExecutionValue( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    QVariant val;

    if( m_hshvalsExec.contains(i_strKey) )
    {
        val = m_hshvalsExec[i_strKey];
    }
    return val;

} // getExecutionValue

//------------------------------------------------------------------------------
void CRequest::setExecutionValue( const QString& i_strKey, const QVariant& i_val )
//------------------------------------------------------------------------------
{
    m_hshvalsExec[i_strKey] = i_val;
    m_bChanged = true;
}

//------------------------------------------------------------------------------
void CRequest::removeExecutionValue( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    if( m_hshvalsExec.contains(i_strKey) )
    {
        m_hshvalsExec.remove(i_strKey);
        m_bChanged = true;
    }

} // removeExecutionValue

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequest::hasExecutionData() const
//------------------------------------------------------------------------------
{
    return (m_hshpvsExec.size() > 0);
}

//------------------------------------------------------------------------------
bool CRequest::hasExecutionData( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    return m_hshpvsExec.contains(i_strKey);
}

//------------------------------------------------------------------------------
void* CRequest::getExecutionData( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    void* pvData = nullptr;

    if( m_hshpvsExec.contains(i_strKey) )
    {
        pvData = m_hshpvsExec[i_strKey];
    }
    return pvData;

} // getExecutionData

//------------------------------------------------------------------------------
void* CRequest::takeExecutionData( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    void* pvData = nullptr;

    if( m_hshpvsExec.contains(i_strKey) )
    {
        pvData = m_hshpvsExec.take(i_strKey);
        m_bChanged = true;
    }
    return pvData;

} // takeExecutionData

//------------------------------------------------------------------------------
void CRequest::setExecutionData( const QString& i_strKey, void* i_pvData )
//------------------------------------------------------------------------------
{
    m_hshpvsExec[i_strKey] = i_pvData;
    m_bChanged = true;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReq* CRequest::takeExecutionRequestMessage()
//------------------------------------------------------------------------------
{
    CMsgReq* pMsgReq = m_pMsgReqExec;
    m_pMsgReqExec = nullptr;
    m_bChanged = true;
    return pMsgReq;

} // takeExecutionRequestMessage

//------------------------------------------------------------------------------
void CRequest::setExecutionRequestMessage( CMsgReq* i_pMsgReq )
//------------------------------------------------------------------------------
{
    m_pMsgReqExec = i_pMsgReq;
    m_bChanged = true;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMsgCon* CRequest::takeExecutionConfirmationMessage()
//------------------------------------------------------------------------------
{
    CMsgCon* pMsgCon = m_pMsgConExec;
    m_pMsgConExec = nullptr;
    m_bChanged = true;
    return pMsgCon;

} // takeExecutionConfirmationMessage

//------------------------------------------------------------------------------
void CRequest::setExecutionConfirmationMessage( CMsgCon* i_pMsgCon )
//------------------------------------------------------------------------------
{
    m_pMsgConExec = i_pMsgCon;
    m_bChanged = true;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMsgInd* CRequest::takeExecutionIndicationMessage()
//------------------------------------------------------------------------------
{
    CMsgInd* pMsgInd = m_pMsgIndExec;
    m_pMsgIndExec = nullptr;
    m_bChanged = true;
    return pMsgInd;

} // takeExecutionIndicationMessage

//------------------------------------------------------------------------------
void CRequest::setExecutionIndicationMessage( CMsgInd* i_pMsgInd )
//------------------------------------------------------------------------------
{
    m_pMsgIndExec = i_pMsgInd;
    m_bChanged = true;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReq* CRequest::takeInitiatorRequestMessage()
//------------------------------------------------------------------------------
{
    CMsgReq* pMsgReq = nullptr;

    if( m_pMsgReqInitiator != nullptr )
    {
        pMsgReq = m_pMsgReqInitiator;
        m_pMsgReqInitiator = nullptr;
        m_bChanged = true;
    }
    return pMsgReq;

} // takeInitiatorRequestMessage

//------------------------------------------------------------------------------
void CRequest::setInitiatorRequestMessage( CMsgReq* i_pMsgReq )
//------------------------------------------------------------------------------
{
    delete m_pMsgReqInitiator;
    m_pMsgReqInitiator = i_pMsgReq;
    m_bChanged = true;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMsgInd* CRequest::takeInitiatorIndicationMessage()
//------------------------------------------------------------------------------
{
    CMsgInd* pMsgInd = nullptr;

    if( m_pMsgIndInitiator != nullptr )
    {
        pMsgInd = m_pMsgIndInitiator;
        m_pMsgIndInitiator = nullptr;
        m_bChanged = true;
    }
    return pMsgInd;

} // takeIndicationMessage

//------------------------------------------------------------------------------
void CRequest::setInitiatorIndicationMessage( CMsgInd* i_pMsgInd )
//------------------------------------------------------------------------------
{
    delete m_pMsgIndInitiator;
    m_pMsgIndInitiator = i_pMsgInd;
    m_bChanged = true;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMsgCon* CRequest::takeInitiatorConfirmationMessage()
//------------------------------------------------------------------------------
{
    CMsgCon* pMsgCon = nullptr;

    if( m_pMsgConInitiator != nullptr )
    {
        pMsgCon = m_pMsgConInitiator;
        m_pMsgConInitiator = nullptr;
        m_bChanged = true;
    }
    return pMsgCon;

} // takeInitiatorConfirmationMessage

//------------------------------------------------------------------------------
void CRequest::setInitiatorConfirmationMessage( CMsgCon* i_pMsgCon )
//------------------------------------------------------------------------------
{
    delete m_pMsgConInitiator;
    m_pMsgConInitiator = i_pMsgCon;
    m_bChanged = true;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CRequest::getAddTrcInfoStr( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    strAddTrcInfo = m_dscr.getAddTrcInfoStr(i_iDetailLevel);

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CRequest::onCreatorDestroyed( QObject* i_pObjCreator )
//------------------------------------------------------------------------------
{
    if( i_pObjCreator == nullptr || i_pObjCreator != m_dscr.m_pObjCreator )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "Invalid signal sender." );
    }

    m_dscr.m_pObjCreator = nullptr;

    if( CRequestExecTree::GetInstance() != nullptr )
    {
        CRequestExecTree::GetInstance()->updateRequest(m_dscr);
    }

} // onCreatorDestroyed

//------------------------------------------------------------------------------
void CRequest::onThreadFinished()
//------------------------------------------------------------------------------
{
    m_dscr.m_pThread = nullptr;

    if( CRequestExecTree::GetInstance() != nullptr )
    {
        CRequestExecTree::GetInstance()->updateRequest(m_dscr);
    }

} // onThreadFinished
