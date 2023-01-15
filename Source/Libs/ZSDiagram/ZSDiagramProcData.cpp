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
    //m_arSpacing[EScaleDirCount],
    m_iDiagScaleCount(0),
    m_pDiagScaleFirst(nullptr),
    m_pDiagScaleLast(nullptr),
    m_iDiagTraceCount(0),
    m_pDiagTraceFirst(nullptr),
    m_pDiagTraceLast(nullptr),
    m_iDiagObjCount(0),
    m_pDiagObjFirst(nullptr),
    m_pDiagObjLast(nullptr),
    m_pDiagObjPaintFirst(nullptr),
    m_pDiagObjPaintLast(nullptr),
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", UpdateType: " + diagramUpdateType2Str(i_updateType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    for( int idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
    {
        m_arSpacing[idxScaleDir] = ESpacingLinear;
    }
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

    CDiagScale* pDiagScale = m_pDiagScaleFirst;
    while( pDiagScale != nullptr )
    {
        CDiagScale* pDiagScaleNext = pDiagScale->m_pDiagScaleNext;

        try
        {
            delete pDiagScale;
        }
        catch(...)
        {
        }
        pDiagScale = pDiagScaleNext;
    }

    m_iDiagScaleCount = 0;
    m_pDiagScaleFirst = nullptr;
    m_pDiagScaleLast  = nullptr;

    CDiagTrace* pDiagTrace = m_pDiagTraceFirst;
    while( pDiagTrace != nullptr )
    {
        CDiagTrace* pDiagTraceNext = pDiagTrace->m_pDiagTraceNext;

        try
        {
            delete pDiagTrace;
        }
        catch(...)
        {
        }
        pDiagTrace = pDiagTraceNext;
    }

    m_iDiagTraceCount = 0;
    m_pDiagTraceFirst = nullptr;
    m_pDiagTraceLast  = nullptr;

    CDiagObj* pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        CDiagObj* pDiagObjNext = pDiagObj->m_pDiagObjNext;

        try
        {
            delete pDiagObj;
        }
        catch(...)
        {
        }
        pDiagObj = pDiagObjNext;
    }

    m_iDiagObjCount = 0;
    m_pDiagObjFirst = nullptr;
    m_pDiagObjLast = nullptr;
    m_pDiagObjPaintFirst = nullptr;
    m_pDiagObjPaintLast = nullptr;

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
    memset(m_arSpacing, 0x00, EScaleDirCount*sizeof(m_arSpacing[0]));

} // dtor

/*==============================================================================
public: // copy ctor not allowed but diagrams may be cloned
==============================================================================*/

//------------------------------------------------------------------------------
CDataDiagram* CDataDiagram::clone( EDiagramUpdateType /*i_diagramUpdateType*/ ) const
//------------------------------------------------------------------------------
{
    CDataDiagram*     pDiagram = new CDataDiagram(m_strObjName);
    int               idxScaleDir;
    const CDiagScale* pDiagScale;
    const CDiagTrace* pDiagTrace;
    const CDiagObj*   pDiagObj;
    CDiagObj*         pDiagObjCloned;

    pDiagram->m_uUpdateFlags = m_uUpdateFlags;
    pDiagram->m_measState = m_measState;
    pDiagram->m_measMode = m_measMode;
    pDiagram->m_iMeasType = m_iMeasType;

    for( idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
    {
        pDiagram->m_arSpacing[idxScaleDir] = m_arSpacing[idxScaleDir];
    }

    pDiagScale = m_pDiagScaleFirst;
    while( pDiagScale != nullptr )
    {
        pDiagScale->clone(pDiagram);
        pDiagScale = pDiagScale->m_pDiagScaleNext;
    }

    pDiagTrace = m_pDiagTraceFirst;
    while( pDiagTrace != nullptr )
    {
        pDiagTrace->clone(pDiagram);
        pDiagTrace = pDiagTrace->m_pDiagTraceNext;
    }

    pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        pDiagObj->clone(pDiagram);
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }

    if( m_pDiagObjPaintFirst != nullptr && m_pDiagObjPaintLast != nullptr )
    {
        pDiagObj = m_pDiagObjPaintFirst;
        while( pDiagObj != nullptr )
        {
            pDiagObjCloned = pDiagram->getDiagObj( pDiagObj->getObjId() );

            if( pDiagObjCloned == nullptr )
            {
                throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
            }

            if( pDiagObj->m_pDiagObjPaintPrev != nullptr && pDiagObj->m_pDiagObjPaintNext != nullptr )
            {
                pDiagObjCloned->m_pDiagObjPaintPrev = pDiagram->getDiagObj( pDiagObj->m_pDiagObjPaintPrev->getObjId() ); //lint !e613
                pDiagObjCloned->m_pDiagObjPaintNext = pDiagram->getDiagObj( pDiagObj->m_pDiagObjPaintNext->getObjId() ); //lint !e613
            }
            pDiagObj = pDiagObj->m_pDiagObjPaintNext;
        }
        pDiagram->m_pDiagObjPaintFirst = pDiagram->getDiagObj( m_pDiagObjPaintFirst->getObjId() );
        pDiagram->m_pDiagObjPaintLast  = pDiagram->getDiagObj( m_pDiagObjPaintLast->getObjId() );
    }

    return pDiagram;

} // clone

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
void CDataDiagram::updateDiagram( CDiagObj* i_pDiagObj )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObjUpdate != nullptr && m_pTrcAdminObjUpdate->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        if( i_pDiagObj != nullptr )
        {
            strTrcMsg = i_pDiagObj->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateDiagram",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
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

    int  iIterations;
    int  iIterationsLayout;
    int  iIterationsData;
    bool bUpdLayout;
    bool bUpdData;

    // As long as the maximum number of iterations is not exceeded ...
    for( iIterations = 0; iIterations < 10; iIterations++ )
    {
        bUpdLayout = false;
        bUpdData = false;

        // If the layout need to be updated ..
        if( m_uUpdateFlags & EUpdateLayout )
        {
            // As long as the maximum number of iterations is not exceeded ...
            for( iIterationsLayout = 0; iIterationsLayout < 10; iIterationsLayout++ )
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
            for( iIterationsData = 0; iIterationsData < 10; iIterationsData++ )
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
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // updateDiagram

//lint -esym(818,i_pDiagObj)
//------------------------------------------------------------------------------
void CDataDiagram::invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObjValidate != nullptr && m_pTrcAdminObjValidate->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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
            CDiagObj* pDiagObj = m_pDiagObjFirst;
            while( pDiagObj != nullptr )
            {
                // Don't invalidate the sender object. Its not necessary and
                // might lead to endless recursive calls.
                if( pDiagObj != i_pDiagObj )
                {
                    pDiagObj->invalidate(uUpdateFlagsForObjects,false);
                }
                pDiagObj = pDiagObj->m_pDiagObjNext;
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

    if( m_pTrcAdminObjValidate != nullptr && m_pTrcAdminObjValidate->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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
void CDataDiagram::setSpacing( EScaleDir i_scaleDir, ESpacing i_spacing )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg  = "ScaleDir=";
        strTrcMsg += scaleDir2Str(i_scaleDir) + ", ";
        strTrcMsg += "Spacing=";
        strTrcMsg += spacing2Str(i_spacing);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpacing",
        /* strAddInfo   */ strTrcMsg );

    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    // Dispatch new spacing to scale objects
    //--------------------------------------

    CDiagScale* pDiagScale;

    // Please note that the scale objects just store the new scale values but do not
    // recalculate the scale division lines. This must be triggered later by explicitly
    // calling "update".

    // The spacing is valid for both the scale objects not linked to a specific trace and
    // also for the scale objects linked to a specific trace.
    m_arSpacing[i_scaleDir] = i_spacing;

    pDiagScale = m_pDiagScaleFirst;
    while( pDiagScale != nullptr )
    {
        // If the scale will be changed the method "scaleChanged" of the diagram will be
        // called invalidating the data of all objects linked to the changed scale.
        if( pDiagScale->getScaleDir() == i_scaleDir )
        {
            pDiagScale->setSpacing(m_arSpacing[i_scaleDir]);
        }
        pDiagScale = pDiagScale->m_pDiagScaleNext;
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = i_pDiagScale->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addDiagScale",
        /* strAddInfo   */ strTrcMsg );

    if( i_pDiagScale == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    // First scale ..
    if( m_iDiagScaleCount == 0 )
    {
        i_pDiagScale->m_pDiagScalePrev = nullptr;
        i_pDiagScale->m_pDiagScaleNext = nullptr;

        m_pDiagScaleFirst = i_pDiagScale;
        m_pDiagScaleLast  = i_pDiagScale;
    }
    // Not the first scale ...
    else
    {
        if( m_pDiagScaleLast == nullptr )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
        }

        m_pDiagScaleLast->m_pDiagScaleNext = i_pDiagScale;

        i_pDiagScale->m_pDiagScalePrev = m_pDiagScaleLast;
        i_pDiagScale->m_pDiagScaleNext = nullptr;

        m_pDiagScaleLast = i_pDiagScale;
    }
    m_iDiagScaleCount++;

    // Initialize some instance members of the scale object which can only be set if the
    // scale object has been added to the diagram.
    i_pDiagScale->m_pDiagram = this;

} // addDiagScale

//------------------------------------------------------------------------------
void CDataDiagram::removeDiagScale( CDiagScale* i_pDiagScale )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = i_pDiagScale->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeDiagScale",
        /* strAddInfo   */ strTrcMsg );

    if( i_pDiagScale == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
    if( m_iDiagScaleCount == 0 )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
    const CDiagScale* pDiagScale = m_pDiagScaleFirst;
    while( pDiagScale != nullptr && pDiagScale != i_pDiagScale )
    {
        pDiagScale = pDiagScale->m_pDiagScaleNext;
    }
    if( pDiagScale == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    CDiagTrace* pDiagTrace;
    CDiagObj*   pDiagObj;

    // First scale ...
    if( i_pDiagScale == m_pDiagScaleFirst )
    {
        m_pDiagScaleFirst = i_pDiagScale->m_pDiagScaleNext;
    }
    // Last scale ...
    if( i_pDiagScale == m_pDiagScaleLast )
    {
        m_pDiagScaleLast = i_pDiagScale->m_pDiagScalePrev;
    }
    if( i_pDiagScale->m_pDiagScalePrev != nullptr )
    {
        i_pDiagScale->m_pDiagScalePrev->m_pDiagScaleNext = i_pDiagScale->m_pDiagScaleNext;
    }
    if( i_pDiagScale->m_pDiagScaleNext != nullptr )
    {
        i_pDiagScale->m_pDiagScaleNext->m_pDiagScalePrev = i_pDiagScale->m_pDiagScalePrev;
    }
    m_iDiagScaleCount--;

    i_pDiagScale->m_pDiagram = nullptr;

    // Remove scale object from trace objects.
    pDiagTrace = m_pDiagTraceFirst;
    while( pDiagTrace != nullptr )
    {
        if( pDiagTrace->m_arpDiagScale[i_pDiagScale->getScaleDir()] == i_pDiagScale )
        {
            pDiagTrace->m_arpDiagScale[i_pDiagScale->getScaleDir()] = nullptr;
        }
        pDiagTrace = pDiagTrace->m_pDiagTraceNext;
    }

    // Remove scale object from diagram objects.
    pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        if( pDiagObj->m_arpDiagScale[i_pDiagScale->getScaleDir()] == i_pDiagScale )
        {
            pDiagObj->m_arpDiagScale[i_pDiagScale->getScaleDir()] = nullptr;
        }
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }

} // removeDiagScale

//------------------------------------------------------------------------------
CDiagScale* CDataDiagram::removeDiagScale( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CDiagScale* pDiagScale = getDiagScale(i_strObjName);

    if( pDiagScale == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    removeDiagScale(pDiagScale);

    return pDiagScale;

} // removeDiagScale

//------------------------------------------------------------------------------
CDiagScale* CDataDiagram::getDiagScale( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CDiagScale* pDiagScale = m_pDiagScaleFirst;
    while( pDiagScale != nullptr )
    {
        if( pDiagScale->getObjName() == i_strObjName )
        {
            break;
        }
        pDiagScale = pDiagScale->m_pDiagScaleNext;
    }
    return pDiagScale;

} // getDiagScale

/*==============================================================================
public: // instance methods to add and remove trace objects
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::addDiagTrace( CDiagTrace* i_pDiagTrace )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = i_pDiagTrace->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addDiagTrace",
        /* strAddInfo   */ strTrcMsg );

    if( i_pDiagTrace == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    // First trace ..
    if( m_iDiagTraceCount == 0 )
    {
        i_pDiagTrace->m_pDiagTracePrev = nullptr;
        i_pDiagTrace->m_pDiagTraceNext = nullptr;

        m_pDiagTraceFirst = i_pDiagTrace;
        m_pDiagTraceLast  = i_pDiagTrace;
    }
    // Not the first trace ...
    else
    {
        if( m_pDiagTraceLast == nullptr )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
        }

        m_pDiagTraceLast->m_pDiagTraceNext = i_pDiagTrace;

        i_pDiagTrace->m_pDiagTracePrev = m_pDiagTraceLast;
        i_pDiagTrace->m_pDiagTraceNext = nullptr;

        m_pDiagTraceLast = i_pDiagTrace;
    }
    m_iDiagTraceCount++;

    // Initialize some instance members of the scale object which can only be set if the
    // scale object has been added to the diagram.
    i_pDiagTrace->m_pDiagram = this;

} // addDiagTrace

//------------------------------------------------------------------------------
void CDataDiagram::removeDiagTrace( CDiagTrace* i_pDiagTrace )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = i_pDiagTrace->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeDiagTrace",
        /* strAddInfo   */ strTrcMsg );

    if( i_pDiagTrace == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
    if( m_iDiagTraceCount == 0 )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
    const CDiagTrace* pDiagTrace = m_pDiagTraceFirst;
    while( pDiagTrace != nullptr && pDiagTrace != i_pDiagTrace )
    {
        pDiagTrace = pDiagTrace->m_pDiagTraceNext;
    }
    if( pDiagTrace == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    CDiagObj* pDiagObj;

    // First trace ...
    if( i_pDiagTrace == m_pDiagTraceFirst )
    {
        m_pDiagTraceFirst = i_pDiagTrace->m_pDiagTraceNext;
    }
    // Last trace ...
    if( i_pDiagTrace == m_pDiagTraceLast )
    {
        m_pDiagTraceLast = i_pDiagTrace->m_pDiagTracePrev;
    }
    if( i_pDiagTrace->m_pDiagTracePrev != nullptr )
    {
        i_pDiagTrace->m_pDiagTracePrev->m_pDiagTraceNext = i_pDiagTrace->m_pDiagTraceNext;
    }
    if( i_pDiagTrace->m_pDiagTraceNext != nullptr )
    {
        i_pDiagTrace->m_pDiagTraceNext->m_pDiagTracePrev = i_pDiagTrace->m_pDiagTracePrev;
    }
    m_iDiagTraceCount--;

    i_pDiagTrace->m_pDiagram = nullptr;

    // Remove trace object from diagram objects.
    pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        if( pDiagObj->m_pDiagTrace == i_pDiagTrace )
        {
            pDiagObj->m_pDiagTrace = nullptr;
        }
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }

} // removeDiagTrace

//------------------------------------------------------------------------------
CDiagTrace* CDataDiagram::removeDiagTrace( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CDiagTrace* pDiagTrace = getDiagTrace(i_strObjName);

    if( pDiagTrace == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    removeDiagTrace(pDiagTrace);

    return pDiagTrace;

} // removeDiagTrace

//------------------------------------------------------------------------------
CDiagTrace* CDataDiagram::getDiagTrace( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CDiagTrace* pDiagTrace = m_pDiagTraceFirst;

    while( pDiagTrace != nullptr )
    {
        if( pDiagTrace->getObjName() == i_strObjName )
        {
            break;
        }
        pDiagTrace = pDiagTrace->m_pDiagTraceNext;
    }
    return pDiagTrace;

} // getDiagTrace

/*==============================================================================
public: // instance methods to add, remove, and change diagram objects
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::addDiagObj( CDiagObj* i_pDiagObj )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = i_pDiagObj->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addDiagObj",
        /* strAddInfo   */ strTrcMsg );

    if( i_pDiagObj == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    // First object ..
    if( m_iDiagObjCount == 0 )
    {
        i_pDiagObj->m_pDiagObjPrev      = nullptr;
        i_pDiagObj->m_pDiagObjNext      = nullptr;
        i_pDiagObj->m_pDiagObjPaintPrev = nullptr;
        i_pDiagObj->m_pDiagObjPaintNext = nullptr;

        m_pDiagObjFirst      = i_pDiagObj;
        m_pDiagObjLast       = i_pDiagObj;
        m_pDiagObjPaintFirst = i_pDiagObj;
        m_pDiagObjPaintLast  = i_pDiagObj;
    }
    // Not the first object ...
    else
    {
        if( m_pDiagObjLast == nullptr )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
        }
        if( m_pDiagObjPaintLast == nullptr )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
        }

        m_pDiagObjLast->m_pDiagObjNext           = i_pDiagObj;
        m_pDiagObjPaintLast->m_pDiagObjPaintNext = i_pDiagObj;

        i_pDiagObj->m_pDiagObjPrev      = m_pDiagObjLast;
        i_pDiagObj->m_pDiagObjNext      = nullptr;
        i_pDiagObj->m_pDiagObjPaintPrev = m_pDiagObjPaintLast;
        i_pDiagObj->m_pDiagObjPaintNext = nullptr;

        m_pDiagObjLast      = i_pDiagObj;
        m_pDiagObjPaintLast = i_pDiagObj;
    }
    i_pDiagObj->m_iObjId = m_iDiagObjCount;
    m_iDiagObjCount++;

    // Initialize some instance members of the diagram object which can only be
    // set if the object has been added to the diagram.
    i_pDiagObj->m_pDataDiagram = this;

    #ifdef _WINDOWS
    #pragma message(__TODO__"DiagObj should not know the type of the diagram.")
    #endif

    if( getUpdateType() >= EDiagramUpdateTypePixmap )
    {
        i_pDiagObj->m_pPixmapDiagram = dynamic_cast<CPixmapDiagram*>(this);
    }
    if( getUpdateType() >= EDiagramUpdateTypeWidget )
    {
        i_pDiagObj->m_pWdgtDiagram = dynamic_cast<CWdgtDiagram*>(this);
    }

    invalidate(nullptr, EUpdateLayoutDataPixmapWidget);

} // addDiagObj

//------------------------------------------------------------------------------
void CDataDiagram::removeDiagObj( CDiagObj* i_pDiagObj )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = i_pDiagObj->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeDiagObj",
        /* strAddInfo   */ strTrcMsg );

    if( i_pDiagObj == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
    if( m_iDiagObjCount == 0 )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
    const CDiagObj* pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr && pDiagObj != i_pDiagObj )
    {
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }
    if( pDiagObj == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    // Diagram object list
    if( i_pDiagObj == m_pDiagObjFirst )
    {
        m_pDiagObjFirst = i_pDiagObj->m_pDiagObjNext;
    }
    if( i_pDiagObj == m_pDiagObjLast )
    {
        m_pDiagObjLast = i_pDiagObj->m_pDiagObjPrev;
    }
    if( i_pDiagObj->m_pDiagObjPrev != nullptr )
    {
        i_pDiagObj->m_pDiagObjPrev->m_pDiagObjNext = i_pDiagObj->m_pDiagObjNext;
    }
    if( i_pDiagObj->m_pDiagObjNext != nullptr )
    {
        i_pDiagObj->m_pDiagObjNext->m_pDiagObjPrev = i_pDiagObj->m_pDiagObjPrev;
    }
    m_iDiagObjCount--;

    // Diagram object paint list
    if( i_pDiagObj == m_pDiagObjPaintFirst )
    {
        m_pDiagObjPaintFirst = i_pDiagObj->m_pDiagObjPaintNext;
    }
    if( i_pDiagObj == m_pDiagObjPaintLast )
    {
        m_pDiagObjPaintLast = i_pDiagObj->m_pDiagObjPaintPrev;
    }
    if( i_pDiagObj->m_pDiagObjPaintPrev != nullptr )
    {
        i_pDiagObj->m_pDiagObjPaintPrev->m_pDiagObjPaintNext = i_pDiagObj->m_pDiagObjPaintNext;
    }
    if( i_pDiagObj->m_pDiagObjPaintNext != nullptr )
    {
        i_pDiagObj->m_pDiagObjPaintNext->m_pDiagObjPaintPrev = i_pDiagObj->m_pDiagObjPaintPrev;
    }

    i_pDiagObj->m_pDataDiagram   = nullptr;
    i_pDiagObj->m_pPixmapDiagram = nullptr;
    i_pDiagObj->m_pWdgtDiagram   = nullptr;

    invalidate(nullptr, EUpdateLayoutDataPixmapWidget);

} // removeDiagObj

//------------------------------------------------------------------------------
CDiagObj* CDataDiagram::removeDiagObj( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CDiagObj* pDiagObj = getDiagObj(i_strObjName);

    if( pDiagObj == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    removeDiagObj(pDiagObj);

    return pDiagObj;
}

//------------------------------------------------------------------------------
CDiagObj* CDataDiagram::removeDiagObj( int i_iObjId )
//------------------------------------------------------------------------------
{
    CDiagObj* pDiagObj = getDiagObj(i_iObjId);

    if( pDiagObj == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }

    removeDiagObj(pDiagObj);

    return pDiagObj;
}

//------------------------------------------------------------------------------
CDiagObj* CDataDiagram::getDiagObj( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    CDiagObj* pDiagObj = m_pDiagObjFirst;

    while( pDiagObj != nullptr )
    {
        if( pDiagObj->getObjName() == i_strObjName )
        {
            break;
        }
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }
    return pDiagObj;
}

//------------------------------------------------------------------------------
CDiagObj* CDataDiagram::getDiagObj( int i_iObjId ) const
//------------------------------------------------------------------------------
{
    CDiagObj* pDiagObj = m_pDiagObjFirst;

    while( pDiagObj != nullptr )
    {
        if( pDiagObj->getObjId() == i_iObjId )
        {
            break;
        }
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }
    return pDiagObj;
}

//------------------------------------------------------------------------------
void CDataDiagram::showDiagObj( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showDiagObj",
        /* strAddInfo   */ i_strObjName );

    CDiagObj* pDiagObj = getDiagObj(i_strObjName);

    if( pDiagObj != nullptr )
    {
        pDiagObj->show(true); // also inform other objects of necessary layout processing
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::showDiagObj( int i_iObjId ) const
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = QString::number(i_iObjId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showDiagObj",
        /* strAddInfo   */ strTrcMsg );

    CDiagObj* pDiagObj = getDiagObj(i_iObjId);

    if( pDiagObj != nullptr )
    {
        pDiagObj->show(true); // also inform other objects of necessary layout processing
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::hideDiagObj( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideDiagObj",
        /* strAddInfo   */ i_strObjName );

    CDiagObj* pDiagObj = getDiagObj(i_strObjName);

    if( pDiagObj != nullptr )
    {
        pDiagObj->hide(true); // also inform other objects of necessary layout processing
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::hideDiagObj( int i_iObjId ) const
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = QString::number(i_iObjId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hideDiagObj",
        /* strAddInfo   */ strTrcMsg );

    CDiagObj* pDiagObj = getDiagObj(i_iObjId);

    if( pDiagObj != nullptr )
    {
        pDiagObj->hide(true); // also inform other objects of necessary layout processing
    }
}

//------------------------------------------------------------------------------
bool CDataDiagram::isDiagObjVisible( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    const CDiagObj* pDiagObj = getDiagObj(i_strObjName);

    if( pDiagObj != nullptr )
    {
        return pDiagObj->isVisible();
    }
    return false;
}

//------------------------------------------------------------------------------
bool CDataDiagram::isDiagObjVisible( int i_iObjId ) const
//------------------------------------------------------------------------------
{
    const CDiagObj* pDiagObj = getDiagObj(i_iObjId);

    if( pDiagObj != nullptr )
    {
        return pDiagObj->isVisible();
    }
    return false;
}

//------------------------------------------------------------------------------
void CDataDiagram::moveDiagObjInPaintList(
    const QString&   i_strObjName,
    EDiagObjMoveMode i_moveMode,
    int              /*i_idxCount*/ )
//------------------------------------------------------------------------------
{
    CDiagObj* pDiagObj = getDiagObj(i_strObjName);

    if( pDiagObj != nullptr )
    {
        moveDiagObjInPaintList(pDiagObj,i_moveMode);
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::moveDiagObjInPaintList(
    int              i_iObjId,
    EDiagObjMoveMode i_moveMode,
    int              /*i_idxCount*/ )
//------------------------------------------------------------------------------
{
    CDiagObj* pDiagObj = getDiagObj(i_iObjId);

    if( pDiagObj != nullptr )
    {
        moveDiagObjInPaintList(pDiagObj,i_moveMode);
    }
}

//------------------------------------------------------------------------------
void CDataDiagram::moveDiagObjInPaintList(
    CDiagObj*        i_pDiagObj,
    EDiagObjMoveMode i_moveMode,
    int              /*i_idxCount*/ )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        strTrcMsg = i_pDiagObj->getObjName() + "," + moveMode2Str(i_moveMode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "moveDiagObjInUpdateList",
        /* strAddInfo   */ strTrcMsg );

    if( i_pDiagObj == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
    if( m_iDiagObjCount == 0 )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined);
    }
    if( m_pDiagObjFirst == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
    }
    if( m_pDiagObjLast == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
    }
    if( m_pDiagObjPaintFirst == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
    }
    if( m_pDiagObjPaintLast == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
    }

    switch( i_moveMode )
    {
        case EDiagObjMoveModeToTop:
        {
            // Not yet the last object in the list ...
            if( i_pDiagObj->m_pDiagObjPaintNext != nullptr )
            {
                CDiagObj* pDiagObjA = i_pDiagObj->m_pDiagObjPaintPrev;
                CDiagObj* pDiagObjB = i_pDiagObj->m_pDiagObjPaintNext;

                if( pDiagObjA != nullptr )
                {
                    pDiagObjA->m_pDiagObjPaintNext = pDiagObjB;
                }
                else
                {
                    m_pDiagObjPaintFirst = pDiagObjB;
                }
                i_pDiagObj->m_pDiagObjPaintNext = nullptr;
                i_pDiagObj->m_pDiagObjPaintPrev = m_pDiagObjPaintLast;
                if( pDiagObjB->m_pDiagObjPaintNext == nullptr )
                {
                    pDiagObjB->m_pDiagObjPaintNext = i_pDiagObj;
                }
                else
                {
                    m_pDiagObjPaintLast->m_pDiagObjPaintNext = i_pDiagObj;
                }
                pDiagObjB->m_pDiagObjPaintPrev = pDiagObjA;
                m_pDiagObjPaintLast = i_pDiagObj;
            }
            break;
        }
        case EDiagObjMoveModeToBottom:
        {
            // not yet supported
            break;
        }
        case EDiagObjMoveModeOneStepUp:
        {
            // not yet supported
            break;
        }
        case EDiagObjMoveModeOneStepDown:
        {
            // not yet supported
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

    // TODO: Pixmap processing for all objects

} // moveDiagObjInPaintList

/*==============================================================================
public: // overridables (callbacks from CDiagScale and CDiagObj)
==============================================================================*/

//------------------------------------------------------------------------------
void CDataDiagram::scaleChanged( ZS::Diagram::CDiagScale* i_pDiagScale )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
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

    bool              bDiagObjUpdData = false;
    int               idxScaleDir;
    const CDiagScale* pDiagScale;
    CDiagTrace*       pDiagTrace;
    CDiagObj*         pDiagObj;

    // All objects linked to the changed scale will be invalidated.
    pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        for( idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
        {
            pDiagScale = pDiagObj->m_arpDiagScale[idxScaleDir];
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
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }

    // All traces linked to the changed scale will be invalidated.
    pDiagTrace = m_pDiagTraceFirst;
    while( pDiagTrace != nullptr )
    {
        for( idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
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
                pDiagObj = m_pDiagObjFirst;
                while( pDiagObj != nullptr )
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
                    pDiagObj = pDiagObj->m_pDiagObjNext;
                }
            }
        }
        pDiagTrace = pDiagTrace->m_pDiagTraceNext;
    }

    // If pixmap processing is necessary for one object
    // (and on changing the scale this is very likely the case)
    // all other objects also need to be redrawn.
    if( bDiagObjUpdData )
    {
        pDiagObj = m_pDiagObjFirst;
        while( pDiagObj != nullptr )
        {
            pDiagObj->invalidate(EUpdatePixmapWidget,false);
            pDiagObj = pDiagObj->m_pDiagObjNext;
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
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

    bool      bDiagObjUpdData = false;
    CDiagObj* pDiagObj;

    pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
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
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }

    // If pixmap processing is necessary for one object
    // (and on changing the trace this is very likely the case)
    // all other objects also need to be redrawn.
    if( bDiagObjUpdData )
    {
        pDiagObj = m_pDiagObjFirst;
        while( pDiagObj != nullptr )
        {
            pDiagObj->invalidate(EUpdatePixmapWidget,false);
            pDiagObj = pDiagObj->m_pDiagObjNext;
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
    CDiagObj* pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        pDiagObj->validate(EUpdateLayout);
        pDiagObj = pDiagObj->m_pDiagObjNext;
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

    CDiagScale*  pDiagScale;
    CDiagTrace*  pDiagTrace;
    CDiagObj*    pDiagObj;

    // Mark current process depth as executed (reset bit):
    // On updating an object this object may emit a "valueChanged" signal
    // whereupon other objects may need to be recalculated and will be
    // invalidated again.
    validate(EUpdateData);

    // Trigger the recalculation of the scale division lines for all scale objects.
    pDiagScale = m_pDiagScaleFirst;
    while( pDiagScale != nullptr )
    {
        // If the scale will be changed the method "scaleChanged" of the diagram will be
        // called invalidating the data of all objects linked to the changed scale.
        pDiagScale->update();
        pDiagScale = pDiagScale->m_pDiagScaleNext;
    }

    // Trigger the recalculation of internal constants within the traces. Those are used
    // to analyze the trace e.g. converting physical values into pixel coordinates and
    // vice versa, value resolutions and other things.
    pDiagTrace = m_pDiagTraceFirst;
    while( pDiagTrace != nullptr )
    {
        // If the trace will be changed the method "traceChanged" of the diagram will be
        // called invalidating the data of all objects linked to the changed trace.
        pDiagTrace->update(EUpdateData);
        pDiagTrace = pDiagTrace->m_pDiagTraceNext;
    }

    // Trigger the recalculation of the output data for all diagram objects
    // (e.g. the diagram axis labels have to calculate the rectangles to output
    // the scale division lines, the curves have to transform the X/Y values
    // into point arrays, etc.).
    pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        // Please note that also invisible objects need to mark their dirty rectangles
        // during widget processing. For invisible objects data processing is not
        // really necessary here. But maybe some objects need data processing to
        // calculate their dirty rectangles. For this data processing will also be
        // executed for invisible objects. But that should not slow down the
        // the system as only visible objects should really recalculate their data.
        pDiagObj->update(EUpdateData);

        pDiagObj = pDiagObj->m_pDiagObjNext;
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // updateData
