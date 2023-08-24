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

#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h" // Should not be necessary to be included
#include "ZSDiagram/ZSDiagObj.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;

//lint -e834

/*******************************************************************************
class CDataDiagram
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDataDiagram::CDataDiagram(
    const QString&     i_strObjName,
    EDiagramUpdateType i_updateType ) :
//------------------------------------------------------------------------------
    m_strObjName(i_strObjName),
    m_updateType(i_updateType),
    m_uUpdateFlags(EUpdateNone),
    m_measState(EMeasStateUndefined),
    m_measMode(EMeasModeUndefined),
    m_iMeasType(-1),
    m_arSpacing(CEnumScaleDir::count(), ESpacing::Linear),
    m_arpDiagScales(),
    m_arpDiagTraces(),
    m_arpDiagObjs(0),
    m_hshpDiagObjs(),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjEvents(nullptr),
    m_pTrcAdminObjLayout(nullptr),
    m_pTrcAdminObjValidate(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), m_strObjName);
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Update", m_strObjName);
    m_pTrcAdminObjEvents = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Events", m_strObjName);
    m_pTrcAdminObjLayout = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Layout", m_strObjName);
    m_pTrcAdminObjValidate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Validate", m_strObjName);

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", UpdateType: " + diagramUpdateType2Str(i_updateType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDataDiagram::~CDataDiagram()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // The derived classes are already destroyed and the virtual method table
    // does not exist anymore. No sense to call a virtual method here as the
    // overridden methods will not be called.
    //emit_aboutToBeDestroyed(m_strObjName);

    for (int idx = 0; idx < m_arpDiagTraces.size(); ++idx)
    {
        try
        {
            delete m_arpDiagTraces[idx];
        }
        catch(...)
        {
        }
        m_arpDiagTraces[idx] = nullptr;
    }
    m_arpDiagTraces.clear();

    for (int idx = 0; idx < m_arpDiagScales.size(); ++idx)
    {
        try
        {
            delete m_arpDiagScales[idx];
        }
        catch(...)
        {
        }
        m_arpDiagScales[idx] = nullptr;
    }
    m_arpDiagScales.clear();

    for (int idx = 0; idx < m_arpDiagObjs.size(); ++idx)
    {
        try
        {
            delete m_arpDiagObjs[idx];
        }
        catch(...)
        {
        }
        m_arpDiagObjs[idx] = nullptr;
    }
    m_arpDiagObjs.clear();
    m_hshpDiagObjs.clear();

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjUpdate);
    m_pTrcAdminObjUpdate = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjEvents);
    m_pTrcAdminObjEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjLayout);
    m_pTrcAdminObjLayout = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjValidate);
    m_pTrcAdminObjValidate = nullptr;

    m_updateType = static_cast<EDiagramUpdateType>(0);
    m_uUpdateFlags = 0;
    m_measState = static_cast<EMeasState>(0);
    m_measMode = static_cast<EMeasMode>(0);
    m_iMeasType = 0;
    //m_arSpacing.clear();

} // dtor

/*==============================================================================
public: // copy ctor not allowed but diagrams may be cloned
==============================================================================*/

//------------------------------------------------------------------------------
CDataDiagram* CDataDiagram::clone( EDiagramUpdateType /*i_diagramUpdateType*/ ) const
//------------------------------------------------------------------------------
{
    CDataDiagram* pDiagramCloned = new CDataDiagram(m_strObjName);

    pDiagramCloned->m_uUpdateFlags = m_uUpdateFlags;
    pDiagramCloned->m_measState = m_measState;
    pDiagramCloned->m_measMode = m_measMode;
    pDiagramCloned->m_iMeasType = m_iMeasType;

    for (int idx = 0; idx < CEnumScaleDir::count(); idx++)
    {
        pDiagramCloned->m_arSpacing[idx] = m_arSpacing[idx];
    }
    for (int idx = 0; idx < m_arpDiagScales.size(); ++idx)
    {
        m_arpDiagScales[idx]->clone(pDiagramCloned);
    }
    for (int idx = 0; idx < m_arpDiagTraces.size(); ++idx)
    {
        m_arpDiagTraces[idx]->clone(pDiagramCloned);
    }
    for (int idx = 0; idx < m_arpDiagObjs.size(); ++idx)
    {
        CDiagObj* pDiagObj = m_arpDiagObjs[idx]->clone(pDiagramCloned);
        QString strDiagObjKey = m_arpDiagObjs[idx]->className() + "::" + m_arpDiagObjs[idx]->getObjName();
        pDiagramCloned->m_hshpDiagObjs[strDiagObjKey] = pDiagObj;
    }
    return pDiagramCloned;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

//------------------------------------------------------------------------------
QString CDataDiagram::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
EDiagramUpdateType CDataDiagram::getUpdateType() const
//------------------------------------------------------------------------------
{
    return m_updateType;
}

/*==============================================================================
public: // overridables to update the content of the diagram
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::update( CDiagObj* i_pDiagObj, const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = QString(i_pDiagObj == nullptr ? "null" : i_pDiagObj->getObjName());
        strMthInArgs += ", " + qRect2Str(i_rect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "OldUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strAddTrcInfo);
    }

    // This method is called by objects controlling the diagram to update the
    // content of the diagram.

    if( m_uUpdateFlags == EUpdateNone )
    {
        return;
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    // As long as the maximum number of iterations is not exceeded ...
    for( int iIterations = 0; iIterations < 10; iIterations++ )
    {
        bool bUpdLayout = false;
        bool bUpdData = false;

        // If the layout need to be updated ..
        if( m_uUpdateFlags & EUpdateLayout )
        {
            // As long as the maximum number of iterations is not exceeded ...
            for( int iIterationsLayout = 0; iIterationsLayout < 10; iIterationsLayout++ )
            {
                updateLayout();
                bUpdLayout = true;

                // We break the layout processing iteration loop
                // if layout processing has been finished.
                if( m_uUpdateFlags & ~EUpdateLayout )
                {
                    break;
                }
            }
        }

        // If the internal data structures need to be updated ..
        if( m_uUpdateFlags & EUpdateData )
        {
            // As long as the maximum number of iterations is not exceeded ...
            for( int iIterationsData = 0; iIterationsData < 10; iIterationsData++ )
            {
                updateData();
                bUpdData = true;

                // We break the data processing iteration loop either if layout processing
                // is necessary again or if data processing has been finished.
                if( m_uUpdateFlags & EUpdateLayout || m_uUpdateFlags & ~EUpdateData )
                {
                    break;
                }
            }
        }

        if( !bUpdLayout && !bUpdData )
        {
            break;
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo = "NewUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strAddTrcInfo);
    }

} // update

//lint -esym(818,i_pDiagObj)
//------------------------------------------------------------------------------
void CDataDiagram::invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjValidate, EMethodTraceDetailLevel::ArgsNormal))
    {
        if( i_pDiagObj != nullptr )
        {
            strTrcMsg = i_pDiagObj->getObjName() + ",";
        }
        strTrcMsg += updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjValidate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "invalidate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        unsigned int uUpdateFlagsForObjects = EUpdateNone;

        // If a bit is set the corresponding update process need to be executed
        // (keeping the current update processes that still need to be executed).
        // This is the data diagram (without a pixmap and a widget that need to
        // be updated). So only data processing is needed ...

        // If method was called by the diagram itself ..
        if( i_pDiagObj == nullptr )
        {
            m_uUpdateFlags |= i_uUpdateFlags;

            // .. we pass on the update flags to the objects.
            uUpdateFlagsForObjects = i_uUpdateFlags;
        }
        // If the method was called by one of the diagram objects ..
        else
        {
            if( i_uUpdateFlags & EUpdateData )
            {
                m_uUpdateFlags |= EUpdateData;
            }

            // Other objects don't need to be informed. This is the data diagram.
            // And if data changes all other objects got new values and will
            // invalidate their data process depths themselves.
        }

        // If the method was called by the diagram itself to invalidate the
        // data process depth ...
        if( uUpdateFlagsForObjects != EUpdateNone )
        {
            for (CDiagObj* pDiagObj : m_arpDiagObjs)
            {
                // Don't invalidate the sender object. Its not necessary and
                // might lead to endless recursive calls.
                if( pDiagObj != i_pDiagObj )
                {
                    pDiagObj->invalidate(uUpdateFlagsForObjects, false);
                }
            }
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // invalidate
//lint +esym(818,i_pDiagObj)

//------------------------------------------------------------------------------
void CDataDiagram::validate( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjValidate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjValidate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "validate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        // If a bit is set the corresponding update process need to be executed.
        // To validate the process depth the bit need to be reset.
        m_uUpdateFlags &= ~i_uUpdateFlags;
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }
} // validate

/*==============================================================================
public: // overridables to rescale the diagram or output new X/Y values
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::setSpacing( const CEnumScaleDir& i_scaleDir, const CEnumSpacing& i_spacing )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = CEnumScaleDir(i_scaleDir).toString() + ", " + i_spacing.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpacing",
        /* strAddInfo   */ strTrcMsg );

    // Dispatch new spacing to scale objects
    //--------------------------------------

    // Please note that the scale objects just store the new scale values but do not
    // recalculate the scale division lines. This must be triggered later by explicitly
    // calling "update".

    // The spacing is valid for both the scale objects not linked to a specific trace and
    // also for the scale objects linked to a specific trace.
    m_arSpacing[i_scaleDir.enumeratorAsInt()] = i_spacing.enumerator();

    for (CDiagScale* pDiagScale : m_arpDiagScales)
    {
        // If the scale will be changed the method "scaleChanged" of the diagram will be
        // called invalidating the data of all objects linked to the changed scale.
        if( pDiagScale->getScaleDir() == i_scaleDir.enumerator() )
        {
            pDiagScale->setSpacing(m_arSpacing[i_scaleDir.enumeratorAsInt()]);
        }
    }
} // setSpacing

/*==============================================================================
public: // instance methods to add and remove diagram scales
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::addDiagScale( CDiagScale* i_pDiagScale )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = i_pDiagScale->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addDiagScale",
        /* strAddInfo   */ strTrcMsg );

    if (findDiagScale(i_pDiagScale->getObjName()) != nullptr)
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList);
    }

    m_arpDiagScales.append(i_pDiagScale);

    // Initialize some instance members of the scale object which can only be set if the
    // scale object has been added to the diagram.
    i_pDiagScale->m_pDiagram = this;

    emit_diagScaleAdded(i_pDiagScale->getObjName());
}

//------------------------------------------------------------------------------
void CDataDiagram::removeDiagScale( CDiagScale* i_pDiagScale )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = i_pDiagScale->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeDiagScale",
        /* strAddInfo   */ strTrcMsg );

    m_arpDiagScales.removeOne(i_pDiagScale);
    i_pDiagScale->m_pDiagram = nullptr;

    // Remove scale object from trace objects.
    for (CDiagTrace* pDiagTrace : m_arpDiagTraces)
    {
        if( pDiagTrace->m_arpDiagScale[static_cast<int>(i_pDiagScale->getScaleDir())] == i_pDiagScale )
        {
            pDiagTrace->m_arpDiagScale[static_cast<int>(i_pDiagScale->getScaleDir())] = nullptr;
        }
    }
    // Remove scale object from diagram objects.
    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        if( pDiagObj->m_arpDiagScale[static_cast<int>(i_pDiagScale->getScaleDir())] == i_pDiagScale )
        {
            pDiagObj->m_arpDiagScale[static_cast<int>(i_pDiagScale->getScaleDir())] = nullptr;
        }
    }
    emit_diagScaleRemoved(i_pDiagScale->getObjName());
}

//------------------------------------------------------------------------------
CDiagScale* CDataDiagram::removeDiagScale( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CDiagScale* pDiagScale = findDiagScale(i_strObjName);

    if( pDiagScale == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    removeDiagScale(pDiagScale);

    return pDiagScale;
}

//------------------------------------------------------------------------------
CDiagScale* CDataDiagram::findDiagScale( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    CDiagScale* pDiagScale = nullptr;

    for (CDiagScale* pDiagScaleTmp : m_arpDiagScales)
    {
        if( pDiagScaleTmp->getObjName() == i_strObjName )
        {
            pDiagScale = pDiagScaleTmp;
            break;
        }
    }
    return pDiagScale;
}

//------------------------------------------------------------------------------
int CDataDiagram::getDiagScalesCount() const
//------------------------------------------------------------------------------
{
    return m_arpDiagScales.size();
}

//------------------------------------------------------------------------------
CDiagScale* CDataDiagram::getDiagScale( int i_idx ) const
//------------------------------------------------------------------------------
{
    return m_arpDiagScales[i_idx];
}

/*==============================================================================
public: // instance methods to add and remove trace objects
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::addDiagTrace( CDiagTrace* i_pDiagTrace )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = i_pDiagTrace->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addDiagTrace",
        /* strAddInfo   */ strTrcMsg );

    if (findDiagTrace(i_pDiagTrace->getObjName()) != nullptr)
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList);
    }

    m_arpDiagTraces.append(i_pDiagTrace);

    // Initialize some instance members of the scale object which can only be set if the
    // scale object has been added to the diagram.
    i_pDiagTrace->m_pDiagram = this;

    emit_diagTraceAdded(i_pDiagTrace->getObjName());
}

//------------------------------------------------------------------------------
void CDataDiagram::removeDiagTrace( CDiagTrace* i_pDiagTrace )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = i_pDiagTrace->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeDiagTrace",
        /* strAddInfo   */ strTrcMsg );

    m_arpDiagTraces.removeOne(i_pDiagTrace);
    i_pDiagTrace->m_pDiagram = nullptr;

    // Remove trace object from diagram objects.
    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        if( pDiagObj->m_pDiagTrace == i_pDiagTrace )
        {
            pDiagObj->m_pDiagTrace = nullptr;
        }
    }
    emit_diagTraceRemoved(i_pDiagTrace->getObjName());
}

//------------------------------------------------------------------------------
CDiagTrace* CDataDiagram::removeDiagTrace( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CDiagTrace* pDiagTrace = findDiagTrace(i_strObjName);

    if( pDiagTrace == nullptr )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotDefined);
    }

    removeDiagTrace(pDiagTrace);

    return pDiagTrace;
}

//------------------------------------------------------------------------------
CDiagTrace* CDataDiagram::findDiagTrace( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    CDiagTrace* pDiagTrace = nullptr;

    for (CDiagTrace* pDiagTraceTmp : m_arpDiagTraces)
    {
        if( pDiagTraceTmp->getObjName() == i_strObjName )
        {
            pDiagTrace = pDiagTraceTmp;
            break;
        }
    }
    return pDiagTrace;
}

//------------------------------------------------------------------------------
int CDataDiagram::getDiagTracesCount() const
//------------------------------------------------------------------------------
{
    return m_arpDiagTraces.size();
}

//------------------------------------------------------------------------------
CDiagTrace* CDataDiagram::getDiagTrace( int i_idx ) const
//------------------------------------------------------------------------------
{
    return m_arpDiagTraces[i_idx];
}

/*==============================================================================
public: // instance methods to add and remove diagram objects
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::addDiagObj( CDiagObj* i_pDiagObj )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = i_pDiagObj->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addDiagObj",
        /* strAddInfo   */ strTrcMsg );

    if (findDiagObj(i_pDiagObj->className(), i_pDiagObj->getObjName()) != nullptr)
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList);
    }

    m_arpDiagObjs.append(i_pDiagObj);

    QString strDiagObjKey = i_pDiagObj->className() + "::" + i_pDiagObj->getObjName();
    m_hshpDiagObjs[strDiagObjKey] = i_pDiagObj;

    // Initialize some instance members of the diagram object which can only be
    // set if the object has been added to the diagram.
    i_pDiagObj->m_pDiagram = this;

    emit_diagObjAdded(i_pDiagObj->className(), i_pDiagObj->getObjName());

    invalidate(nullptr, EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
void CDataDiagram::removeDiagObj( CDiagObj* i_pDiagObj )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = i_pDiagObj->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeDiagObj",
        /* strAddInfo   */ strTrcMsg );

    m_arpDiagObjs.removeOne(i_pDiagObj);
    i_pDiagObj->m_pDiagram = nullptr;

    QString strDiagObjKey = i_pDiagObj->className() + "::" + i_pDiagObj->getObjName();
    m_hshpDiagObjs.remove(strDiagObjKey);

    emit_diagObjRemoved(i_pDiagObj->className(), i_pDiagObj->getObjName());

    invalidate(nullptr, EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
CDiagObj* CDataDiagram::removeDiagObj( const QString& i_strClassName, const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CDiagObj* pDiagObj = findDiagObj(i_strClassName, i_strObjName);
    removeDiagObj(pDiagObj);
    return pDiagObj;
}

//------------------------------------------------------------------------------
CDiagObj* CDataDiagram::findDiagObj( const QString& i_strClassName, const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    QString strDiagObjKey = i_strClassName + "::" + i_strObjName;
    return m_hshpDiagObjs.value(strDiagObjKey, nullptr);
}

//------------------------------------------------------------------------------
int CDataDiagram::getDiagObjsCount() const
//------------------------------------------------------------------------------
{
    return m_arpDiagObjs.size();
}

//------------------------------------------------------------------------------
CDiagObj* CDataDiagram::getDiagObj( int i_idx ) const
//------------------------------------------------------------------------------
{
    return m_arpDiagObjs[i_idx];
}

/*==============================================================================
public: // instance methods to change diagram objects
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::showDiagObj( const QString& i_strClassName, const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showDiagObj",
        /* strAddInfo   */ strMthInArgs );

    CDiagObj* pDiagObj = findDiagObj(i_strClassName, i_strObjName);

    if( pDiagObj != nullptr )
    {
        pDiagObj->show(true); // also informs other objects of necessary layout processing
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::showDiagObj( int i_idx ) const
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = QString::number(i_idx);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showDiagObj",
        /* strAddInfo   */ strTrcMsg );

    CDiagObj* pDiagObj = getDiagObj(i_idx);

    if( pDiagObj != nullptr )
    {
        pDiagObj->show(true); // also informs other objects of necessary layout processing
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::hideDiagObj( const QString& i_strClassName, const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideDiagObj",
        /* strAddInfo   */ i_strObjName );

    CDiagObj* pDiagObj = findDiagObj(i_strClassName, i_strObjName);

    if( pDiagObj != nullptr )
    {
        pDiagObj->hide(true); // also informs other objects of necessary layout processing
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::hideDiagObj( int i_idx ) const
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = QString::number(i_idx);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideDiagObj",
        /* strAddInfo   */ strTrcMsg );

    CDiagObj* pDiagObj = getDiagObj(i_idx);

    if( pDiagObj != nullptr )
    {
        pDiagObj->hide(true); // also informs other objects of necessary layout processing
    }
}

//------------------------------------------------------------------------------
bool CDataDiagram::isDiagObjVisible( const QString& i_strClassName, const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    const CDiagObj* pDiagObj = findDiagObj(i_strClassName, i_strObjName);

    if( pDiagObj != nullptr )
    {
        return pDiagObj->isVisible();
    }
    return false;
}

//------------------------------------------------------------------------------
bool CDataDiagram::isDiagObjVisible( int i_idx ) const
//------------------------------------------------------------------------------
{
    const CDiagObj* pDiagObj = getDiagObj(i_idx);

    if( pDiagObj != nullptr )
    {
        return pDiagObj->isVisible();
    }
    return false;
}

//------------------------------------------------------------------------------
void CDataDiagram::moveDiagObjInPaintList(
    const QString&   i_strClassName,
    const QString&   i_strObjName,
    EDiagObjMoveMode i_moveMode,
    int              i_idxCount )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName
                     + "," + moveMode2Str(i_moveMode) + ", " + QString::number(i_idxCount);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "moveDiagObjInUpdateList",
        /* strAddInfo   */ strMthInArgs );

    CDiagObj* pDiagObj = findDiagObj(i_strClassName, i_strObjName);

    if( pDiagObj != nullptr )
    {
        moveDiagObjInPaintList(pDiagObj, i_moveMode, i_idxCount);
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::moveDiagObjInPaintList(
    int              i_idx,
    EDiagObjMoveMode i_moveMode,
    int              i_idxCount )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx)
                     + "," + moveMode2Str(i_moveMode) + ", " + QString::number(i_idxCount);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "moveDiagObjInUpdateList",
        /* strAddInfo   */ strMthInArgs );

    CDiagObj* pDiagObj = getDiagObj(i_idx);

    if( pDiagObj != nullptr )
    {
        moveDiagObjInPaintList(pDiagObj, i_moveMode, i_idxCount);
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::moveDiagObjInPaintList(
    CDiagObj*        i_pDiagObj,
    EDiagObjMoveMode i_moveMode,
    int              i_idxCount )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pDiagObj->className() + "::" + i_pDiagObj->getObjName()
                     + "," + moveMode2Str(i_moveMode) + ", " + QString::number(i_idxCount);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "moveDiagObjInUpdateList",
        /* strAddInfo   */ strMthInArgs );

    switch( i_moveMode )
    {
        case EDiagObjMoveModeToTop:
        {
            m_arpDiagObjs.removeOne(i_pDiagObj);
            m_arpDiagObjs.insert(0, i_pDiagObj);
            break;
        }
        case EDiagObjMoveModeToBottom:
        {
            m_arpDiagObjs.removeOne(i_pDiagObj);
            m_arpDiagObjs.append(i_pDiagObj);
            break;
        }
        case EDiagObjMoveModeOneStepUp:
        {
            if (m_arpDiagObjs.first() != i_pDiagObj)
            {
                int idx = m_arpDiagObjs.indexOf(i_pDiagObj);
                m_arpDiagObjs.removeOne(i_pDiagObj);
                m_arpDiagObjs.insert(idx-1, i_pDiagObj);
            }
            break;
        }
        case EDiagObjMoveModeOneStepDown:
        {
            if (m_arpDiagObjs.last() != i_pDiagObj)
            {
                int idx = m_arpDiagObjs.indexOf(i_pDiagObj);
                m_arpDiagObjs.removeOne(i_pDiagObj);
                m_arpDiagObjs.insert(idx+1, i_pDiagObj);
            }
            break;
        }
        case EDiagObjMoveModeRelative:
        {
            // not yet supported
            break;
        }
        case EDiagObjMoveModeAbsolute:
        {
            // not yet supported
            break;
        }
        default:
        {
            break;
        }
    }

    invalidate(nullptr, EUpdateLayoutDataPixmapWidget);

} // moveDiagObjInPaintList

/*==============================================================================
public: // overridables (callbacks from CDiagScale and CDiagObj)
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::scaleChanged( ZS::Diagram::CDiagScale* i_pDiagScale )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = i_pDiagScale->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "scaleChanged",
        /* strAddInfo   */ strTrcMsg );

    unsigned int uUpdateFlagsDiagObj = EUpdateNone;
    unsigned int uUpdateFlagsDiagTrace = EUpdateNone;

    switch( m_updateType )
    {
        case EDiagramUpdateTypeData:
        {
            uUpdateFlagsDiagObj = EUpdateData;
            uUpdateFlagsDiagTrace = EUpdateData;
            break;
        }
        case EDiagramUpdateTypePixmap:
        {
            uUpdateFlagsDiagObj = EUpdateLayoutDataPixmap;
            uUpdateFlagsDiagTrace = EUpdateLayoutData;
            break;
        }
        default:
        case EDiagramUpdateTypeWidget:
        {
            uUpdateFlagsDiagObj = EUpdateLayoutDataPixmapWidget;
            uUpdateFlagsDiagTrace = EUpdateLayoutData;
            break;
        }
    }

    bool bDiagObjUpdData = false;

    // All objects linked to the changed scale will be invalidated.
    for (CDiagObj* pDiagObj : m_arpDiagObjs )
    {
        for( int idxScaleDir = 0; idxScaleDir < CEnumScaleDir::count(); idxScaleDir++ )
        {
            CDiagScale* pDiagScale = pDiagObj->m_arpDiagScale[idxScaleDir];
            if( pDiagScale == nullptr && pDiagObj->m_pDiagTrace != nullptr )
            {
                pDiagScale = pDiagObj->m_pDiagTrace->getDiagScale(static_cast<EScaleDir>(idxScaleDir));
            }

            // If the object is linked to the changed scale ..
            if( pDiagScale == i_pDiagScale )
            {
                pDiagObj->invalidate(uUpdateFlagsDiagObj,false);
                // If pixmap processing is necessary for one object
                // (and on changing the scale this is very likely the case)
                // all other objects also need to be redrawn.
                bDiagObjUpdData = true;
            }
        }
    }

    // All traces linked to the changed scale will be invalidated.
    for (CDiagTrace* pDiagTrace : m_arpDiagTraces)
    {
        for( int idxScaleDir = 0; idxScaleDir < CEnumScaleDir::count(); idxScaleDir++ )
        {
            // If the trace is linked to the changed scale ..
            if( pDiagTrace->m_arpDiagScale[idxScaleDir] == i_pDiagScale )
            {
                // Please note than on updating the diagram the diagram will call
                // the update method of the trace object whereupon the "traceChanged"
                // slot of the diagram is called invalidating all objects linked to
                // the trace.
                pDiagTrace->invalidate(uUpdateFlagsDiagTrace);
                if( uUpdateFlagsDiagTrace & EUpdateLayout || uUpdateFlagsDiagTrace & EUpdateData )
                {
                    bDiagObjUpdData = true;
                }

                // All objects linked to the changed traces will be invalidated.
                for (CDiagObj* pDiagObj : m_arpDiagObjs)
                {
                    // If the object is linked to the changed trace ..
                    if( pDiagObj->m_pDiagTrace == pDiagTrace )
                    {
                        pDiagObj->invalidate(uUpdateFlagsDiagObj,false);
                        // If pixmap processing is necessary for one object
                        // (and on changing the scale this is very likely the case)
                        // all other objects also need to be redrawn.
                        bDiagObjUpdData = true;
                    }
                }
            }
        }
    }

    // If pixmap processing is necessary for one object
    // (and on changing the scale this is very likely the case)
    // all other objects also need to be redrawn.
    if( bDiagObjUpdData )
    {
        for (CDiagObj* pDiagObj : m_arpDiagObjs)
        {
            pDiagObj->invalidate(EUpdatePixmapWidget,false);
        }
    }

    // We don't call "this->invalidate" here as this would lead to invalidate
    // calls of all objects. But we just want to invalidate the objects which
    // are linked to the changed scale. So we need to modify the update flags
    // as it would be done by the invalidate method.
    // If a bit is set the corresponding update process need to be executed
    // (keeping the current update processes that still need to be executed):
    switch( m_updateType )
    {
        case EDiagramUpdateTypeData:
        {
            m_uUpdateFlags |= EUpdateData;
            break;
        }
        case EDiagramUpdateTypePixmap:
        {
            m_uUpdateFlags |= EUpdateLayoutDataPixmap;
            break;
        }
        default:
        case EDiagramUpdateTypeWidget:
        {
            m_uUpdateFlags |= EUpdateLayoutDataPixmapWidget;
            break;
        }
    }

} // scaleChanged

//------------------------------------------------------------------------------
void CDataDiagram::traceChanged( ZS::Diagram::CDiagTrace* i_pDiagTrace )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = i_pDiagTrace->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "traceChanged",
        /* strAddInfo   */ strTrcMsg );

    unsigned int uUpdateFlagsDiagObj = EUpdateNone;

    switch( m_updateType )
    {
        case EDiagramUpdateTypeData:
        {
            uUpdateFlagsDiagObj = EUpdateData;
            break;
        }
        case EDiagramUpdateTypePixmap:
        {
            uUpdateFlagsDiagObj = EUpdateLayoutDataPixmap;
            break;
        }
        default:
        case EDiagramUpdateTypeWidget:
        {
            uUpdateFlagsDiagObj = EUpdateLayoutDataPixmapWidget;
            break;
        }
    }

    bool bDiagObjUpdData = false;

    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        // If the object is linked to the changed trace ..
        if( pDiagObj->m_pDiagTrace == i_pDiagTrace )
        {
            pDiagObj->invalidate(uUpdateFlagsDiagObj,false);
            // If pixmap processing is necessary for one object
            // (and on changing the trace this is very likely the case)
            // all other objects also need to be redrawn.
            bDiagObjUpdData = true;
        }
    }

    // If pixmap processing is necessary for one object
    // (and on changing the trace this is very likely the case)
    // all other objects also need to be redrawn.
    if( bDiagObjUpdData )
    {
        for (CDiagObj* pDiagObj : m_arpDiagObjs)
        {
            pDiagObj->invalidate(EUpdatePixmapWidget,false);
        }
    }

    // We don't call "this->invalidate" here as this would lead to invalidate
    // calls of all objects. But we just want to invalidate the objects which
    // are linked to the changed trace. So we need to modify the update flags
    // as it would be done by the invalidate method.
    // If a bit is set the corresponding update process need to be executed
    // (keeping the current update processes that still need to be executed):
    switch( m_updateType )
    {
        case EDiagramUpdateTypeData:
        {
            m_uUpdateFlags |= EUpdateData;
            break;
        }
        case EDiagramUpdateTypePixmap:
        {
            m_uUpdateFlags |= EUpdateLayoutDataPixmap;
            break;
        }
        default:
        case EDiagramUpdateTypeWidget:
        {
            m_uUpdateFlags |= EUpdateLayoutDataPixmapWidget;
            break;
        }
    }

} // traceChanged

/*==============================================================================
protected: // overridables to update the content of the diagram
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::updateLayout()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // This is the data diagram which does not contain a layout, no pixmap and no widget.

    // Mark current process depth as executed (reset bit):
    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        pDiagObj->validate(EUpdateLayout);
    }
    validate(EUpdateLayout);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }
} // updateLayout

//------------------------------------------------------------------------------
void CDataDiagram::updateData()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateData",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // This method is called by objects controlling the diagram to update the
    // content of the diagram.

    // Mark current process depth as executed (reset bit):
    // On updating an object this object may emit a "valueChanged" signal
    // whereupon other objects may need to be recalculated and will be
    // invalidated again.
    validate(EUpdateData);

    // Trigger the recalculation of the scale division lines for all scale objects.
    for (CDiagScale* pDiagScale : m_arpDiagScales)
    {
        // If the scale will be changed the method "scaleChanged" of the diagram will be
        // called invalidating the data of all objects linked to the changed scale.
        pDiagScale->update();
    }

    // Trigger the recalculation of internal constants within the traces. Those are used
    // to analyze the trace e.g. converting physical values into pixel coordinates and
    // vice versa, value resolutions and other things.
    for (CDiagTrace* pDiagTrace : m_arpDiagTraces)
    {
        // If the trace will be changed the method "traceChanged" of the diagram will be
        // called invalidating the data of all objects linked to the changed trace.
        pDiagTrace->update(EUpdateData);
    }

    // Trigger the recalculation of the output data for all diagram objects
    // (e.g. the diagram axis labels have to calculate the rectangles to output
    // the scale division lines, the curves have to transform the X/Y values
    // into point arrays, etc.).
    for (CDiagObj* pDiagObj : m_arpDiagObjs)
    {
        // Please note that also invisible objects need to mark their dirty rectangles
        // during widget processing. For invisible objects data processing is not
        // really necessary here. But maybe some objects need data processing to
        // calculate their dirty rectangles. For this data processing will also be
        // executed for invisible objects. But that should not slow down the
        // the system as only visible objects should really recalculate their data.
        pDiagObj->update(EUpdateData);
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // updateData
