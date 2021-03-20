/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

#include <math.h>

#include <QtCore/qtimer.h>
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>
#include <QtGui/qpixmap.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qmenu.h>
#include <QtGui/qprinter.h>
#include <QtGui/qprintdialog.h>
#include <QtGui/qtextedit.h>
#else
#include <QtPrintSupport/qprinter.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSDiagram/ZSDiagram.h"
#include "ZSDiagram/ZSDiagObj.h"
#include "ZSDiagram/ZSDiagramFrameStyles.h"
#include "ZSDiagram/ZSDiagramMsg.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;
using namespace ZS::Trace;

//lint -e834

/*******************************************************************************
class CDataDiagram
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDataDiagram::CDataDiagram( EDiagramUpdateType i_updateType ) :
//------------------------------------------------------------------------------
    m_strObjName(),
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
    m_pTrcAdminObj(nullptr)
{
    if( i_updateType == EDiagramUpdateTypeData )
    {
        if( m_pTrcAdminObj == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CDataDiagram", m_strObjName);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int idxScaleDir;

    for( idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
    {
        m_arSpacing[idxScaleDir] = ESpacingLinear;
    }

} // ctor

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
    m_pTrcAdminObj(nullptr)
{
    if( i_updateType == EDiagramUpdateTypeData )
    {
        if( m_pTrcAdminObj == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CDataDiagram", m_strObjName);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int idxScaleDir;

    for( idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
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
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CDiagScale*  pDiagScale;
    CDiagScale*  pDiagScaleNext;
    CDiagTrace*  pDiagTrace;
    CDiagTrace*  pDiagTraceNext;
    CDiagObj*    pDiagObj;
    CDiagObj*    pDiagObjNext;

    pDiagScale = m_pDiagScaleFirst;
    while( pDiagScale != nullptr )
    {
        pDiagScaleNext = pDiagScale->m_pDiagScaleNext;

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

    pDiagTrace = m_pDiagTraceFirst;
    while( pDiagTrace != nullptr )
    {
        pDiagTraceNext = pDiagTrace->m_pDiagTraceNext;

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

    pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        pDiagObjNext = pDiagObj->m_pDiagObjNext;

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

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelUpdate) )
    {
        if( i_pDiagObj != nullptr )
        {
            strTrcMsg = i_pDiagObj->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "updateDiagram",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
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

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

} // updateDiagram

//lint -esym(818,i_pDiagObj)
//------------------------------------------------------------------------------
void CDataDiagram::invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelValidate) )
    {
        if( i_pDiagObj != nullptr )
        {
            strTrcMsg = i_pDiagObj->getObjName() + ",";
        }
        strTrcMsg += updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelValidate,
        /* strMethod    */ "invalidate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
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

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
    }

} // invalidate
//lint +esym(818,i_pDiagObj)

//------------------------------------------------------------------------------
void CDataDiagram::validate( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelValidate,
        /* strMethod    */ "validate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        // If a bit is set the corresponding update process need to be executed.
        // To validate the process depth the bit need to be reset.
        m_uUpdateFlags &= ~i_uUpdateFlags;
    }

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg  = "ScaleDir=";
        strTrcMsg += scaleDir2Str(i_scaleDir) + ", ";
        strTrcMsg += "Spacing=";
        strTrcMsg += spacing2Str(i_spacing);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagScale->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagScale->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagTrace->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagTrace->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagObj->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagObj->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

} // removeDiagObj

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

} // removeDiagObj

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

} // getDiagObj

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

} // getDiagObj

//------------------------------------------------------------------------------
void CDataDiagram::showDiagObj( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "showDiagObj",
        /* strAddInfo   */ i_strObjName );

    CDiagObj* pDiagObj = getDiagObj(i_strObjName);

    if( pDiagObj != nullptr )
    {
        pDiagObj->show(true); // also inform other objects of necessary layout processing
    }

} // showDiagObj

//------------------------------------------------------------------------------
void CDataDiagram::showDiagObj( int i_iObjId ) const
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = QString::number(i_iObjId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "showDiagObj",
        /* strAddInfo   */ strTrcMsg );

    CDiagObj* pDiagObj = getDiagObj(i_iObjId);

    if( pDiagObj != nullptr )
    {
        pDiagObj->show(true); // also inform other objects of necessary layout processing
    }

} // showDiagObj

//------------------------------------------------------------------------------
void CDataDiagram::hideDiagObj( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "hideDiagObj",
        /* strAddInfo   */ i_strObjName );

    CDiagObj* pDiagObj = getDiagObj(i_strObjName);

    if( pDiagObj != nullptr )
    {
        pDiagObj->hide(true); // also inform other objects of necessary layout processing
    }

} // hideDiagObj

//------------------------------------------------------------------------------
void CDataDiagram::hideDiagObj( int i_iObjId ) const
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = QString::number(i_iObjId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "hideDiagObj",
        /* strAddInfo   */ strTrcMsg );

    CDiagObj* pDiagObj = getDiagObj(i_iObjId);

    if( pDiagObj != nullptr )
    {
        pDiagObj->hide(true); // also inform other objects of necessary layout processing
    }

} // hideDiagObj

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

} // isDiagObjVisible

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

} // isDiagObjVisible

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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagObj->getObjName() + "," + moveMode2Str(i_moveMode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagScale->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevel0) )
    {
        strTrcMsg = i_pDiagTrace->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
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
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
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

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

} // updateLayout

//------------------------------------------------------------------------------
void CDataDiagram::updateData()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "updateData",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
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

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

} // updateData


/*******************************************************************************
class CPixmapDiagram : public CDataDiagram
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

const char* CPixmapDiagram::g_arszImageFormatFileFilter[EImageFormatCount+1] = {
    "*.bmp",
    "*.jpg",
    "*.xpm",
    "*.png",
    nullptr
};
const char* CPixmapDiagram::g_arszImageFormatFileExt[EImageFormatCount+1] = {
    "bmp",
    "jpg",
    "xpm",
    "png",
    nullptr
};
const int CPixmapDiagram::g_ariImageFormat[EImageFormatCount+1] = {
    EImageFormatBMP,
    EImageFormatJPG,
    EImageFormatXPM,
    EImageFormatPNG,
    -1
};
const char* CPixmapDiagram::g_arszQtImageFormat[EImageFormatCount+1] = {
    "BMP",
    "JPEG",
    "XPM",
    "PNG",
    nullptr
};

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
EImageFormat CPixmapDiagram::GetImageFormat( const QString& i_strImageFormat )
//------------------------------------------------------------------------------
{
    EImageFormat imageFormat = EImageFormatUndefined;
    int          idxFileExt;

    for( idxFileExt = 0; idxFileExt < EImageFormatCount; idxFileExt++ )
    {
        if( g_arszImageFormatFileExt[idxFileExt] != nullptr )
        {
            #if QT_VERSION >= 0x040100
            if( QString(g_arszImageFormatFileExt[idxFileExt]).contains(i_strImageFormat,Qt::CaseInsensitive) )
            #else
            if( QString(g_arszImageFormatFileExt[idxFileExt]).contains(i_strImageFormat,false) )
            #endif
            {
                imageFormat = static_cast<EImageFormat>(g_ariImageFormat[idxFileExt]);
                break;
            }
        }
    }
    if( imageFormat == EImageFormatUndefined )
    {
        for( idxFileExt = 0; idxFileExt < EImageFormatCount; idxFileExt++ )
        {
            if( g_arszImageFormatFileFilter[idxFileExt] != nullptr )
            {
                #if QT_VERSION >= 0x040100
                if( QString(g_arszImageFormatFileFilter[idxFileExt]).contains(i_strImageFormat,Qt::CaseInsensitive) )
                #else
                if( QString(g_arszImageFormatFileFilter[idxFileExt]).contains(i_strImageFormat,false) )
                #endif
                {
                    imageFormat = static_cast<EImageFormat>(g_ariImageFormat[idxFileExt]);
                    break;
                }
            }
        }
    }
    if( imageFormat == EImageFormatUndefined )
    {
        for( idxFileExt = 0; idxFileExt < EImageFormatCount; idxFileExt++ )
        {
            if( g_arszQtImageFormat[idxFileExt] != nullptr )
            {
                #if QT_VERSION >= 0x040100
                if( QString(g_arszQtImageFormat[idxFileExt]).contains(i_strImageFormat,Qt::CaseInsensitive) )
                #else
                if( QString(g_arszQtImageFormat[idxFileExt]).contains(i_strImageFormat,false) )
                #endif
                {
                    imageFormat = static_cast<EImageFormat>(g_ariImageFormat[idxFileExt]);
                    break;
                }
            }
        }
    }
    return imageFormat;

} // GetImageFormat

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPixmapDiagram::CPixmapDiagram( EDiagramUpdateType i_updateType ) :
//------------------------------------------------------------------------------
    CDataDiagram(i_updateType),
    m_colErase(Qt::gray),
    m_colBgPartCenter(Qt::black),
    m_pPixmap(nullptr),
    m_pPrinter(nullptr),
    m_size(),
    m_cyMinimumHeightPartTop(0),
    m_cyMinimumHeightPartBottom(0),
    m_cxMinimumWidthPartLeft(0),
    m_cxMinimumWidthPartRight(0),
    m_iMarginTop(4),
    m_iMarginBottom(4),
    m_iMarginLeft(4),
    m_iMarginRight(4),
    m_rectPartTop(),
    m_rectPartBottom(),
    m_rectPartLeft(),
    m_rectPartRight(),
    m_rectPartCenter(),
    m_pFrameStyle(nullptr),
    m_pFrameStylePartCenter(nullptr),
    m_rectFramePartCenterTopLineInv(),
    m_rectFramePartCenterBottomLineInv(),
    m_rectFramePartCenterLeftLineInv(),
    m_rectFramePartCenterRightLineInv()
{
    if( i_updateType == EDiagramUpdateTypePixmap )
    {
        if( m_pTrcAdminObj == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CPixmapDiagram", m_strObjName);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CPixmapDiagram::CPixmapDiagram(
    const QString&     i_strObjName,
    EDiagramUpdateType i_updateType ) :
//------------------------------------------------------------------------------
    CDataDiagram(i_strObjName,i_updateType),
    m_colErase(Qt::gray),
    m_colBgPartCenter(Qt::black),
    m_pPixmap(nullptr),
    m_pPrinter(nullptr),
    m_size(),
    m_cyMinimumHeightPartTop(0),
    m_cyMinimumHeightPartBottom(0),
    m_cxMinimumWidthPartLeft(0),
    m_cxMinimumWidthPartRight(0),
    m_iMarginTop(4),
    m_iMarginBottom(4),
    m_iMarginLeft(4),
    m_iMarginRight(4),
    m_rectPartTop(),
    m_rectPartBottom(),
    m_rectPartLeft(),
    m_rectPartRight(),
    m_rectPartCenter(),
    m_pFrameStyle(nullptr),
    m_pFrameStylePartCenter(nullptr),
    m_rectFramePartCenterTopLineInv(),
    m_rectFramePartCenterBottomLineInv(),
    m_rectFramePartCenterLeftLineInv(),
    m_rectFramePartCenterRightLineInv()
{
    if( i_updateType == EDiagramUpdateTypePixmap )
    {
        if( m_pTrcAdminObj == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CPixmapDiagram", m_strObjName);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CPixmapDiagram::~CPixmapDiagram()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pPixmap;
    }
    catch(...)
    {
    }
    m_pPixmap = nullptr;

    try
    {
        delete m_pPrinter;
    }
    catch(...)
    {
    }
    m_pPrinter = nullptr;

    try
    {
        delete m_pFrameStyle;
    }
    catch(...)
    {
    }
    m_pFrameStyle = nullptr;

    try
    {
        delete m_pFrameStylePartCenter;
    }
    catch(...)
    {
    }
    m_pFrameStylePartCenter = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // copy ctor not allowed but diagrams may be cloned
==============================================================================*/

//------------------------------------------------------------------------------
CDataDiagram* CPixmapDiagram::clone( EDiagramUpdateType i_diagramUpdateType ) const
//------------------------------------------------------------------------------
{
    CDataDiagram* pDiagram = nullptr;

    if( i_diagramUpdateType < EDiagramUpdateTypePixmap )
    {
        pDiagram = CDataDiagram::clone(i_diagramUpdateType);
    }
    else
    {
        CPixmapDiagram*   pPixmapDiagram = new CPixmapDiagram(m_strObjName);
        int               idxScaleDir;
        const CDiagScale* pDiagScale;
        const CDiagTrace* pDiagTrace;
        const CDiagObj*   pDiagObj;
        CDiagObj*         pDiagObjCloned;

        // Data Diagram
        //-------------

        pPixmapDiagram->m_uUpdateFlags = m_uUpdateFlags;
        pPixmapDiagram->m_measState = m_measState;
        pPixmapDiagram->m_measMode = m_measMode;
        pPixmapDiagram->m_iMeasType = m_iMeasType;

        for( idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
        {
            pPixmapDiagram->m_arSpacing[idxScaleDir] = m_arSpacing[idxScaleDir];
        }

        pDiagScale = m_pDiagScaleFirst;
        while( pDiagScale != nullptr )
        {
            pDiagScale->clone(pPixmapDiagram);
            pDiagScale = pDiagScale->m_pDiagScaleNext;
        }

        pDiagTrace = m_pDiagTraceFirst;
        while( pDiagTrace != nullptr )
        {
            pDiagTrace->clone(pPixmapDiagram);
            pDiagTrace = pDiagTrace->m_pDiagTraceNext;
        }

        pDiagObj = m_pDiagObjFirst;
        while( pDiagObj != nullptr )
        {
            pDiagObj->clone(pPixmapDiagram);
            pDiagObj = pDiagObj->m_pDiagObjNext;
        }

        if( m_pDiagObjPaintFirst != nullptr && m_pDiagObjPaintLast != nullptr )
        {
            pDiagObj = m_pDiagObjPaintFirst;
            while( pDiagObj != nullptr )
            {
                pDiagObjCloned = pPixmapDiagram->getDiagObj( pDiagObj->getObjId() );

                if( pDiagObjCloned == nullptr )
                {
                    throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
                }

                if( pDiagObj->m_pDiagObjPaintPrev == nullptr )
                {
                    pDiagObjCloned->m_pDiagObjPaintPrev = nullptr;
                }
                else
                {
                    pDiagObjCloned->m_pDiagObjPaintPrev = pPixmapDiagram->getDiagObj( pDiagObj->m_pDiagObjPaintPrev->getObjId() ); //lint !e613
                }
                if( pDiagObj->m_pDiagObjPaintNext == nullptr )
                {
                    pDiagObjCloned->m_pDiagObjPaintNext = nullptr;
                }
                else
                {
                    pDiagObjCloned->m_pDiagObjPaintNext = pPixmapDiagram->getDiagObj( pDiagObj->m_pDiagObjPaintNext->getObjId() ); //lint !e613
                }
                pDiagObj = pDiagObj->m_pDiagObjPaintNext;
            }
            pPixmapDiagram->m_pDiagObjPaintFirst = pPixmapDiagram->getDiagObj( m_pDiagObjPaintFirst->getObjId() );
            pPixmapDiagram->m_pDiagObjPaintLast  = pPixmapDiagram->getDiagObj( m_pDiagObjPaintLast->getObjId() );
        }

        // Pixmap Diagram
        //---------------

        pPixmapDiagram->m_colErase = m_colErase;
        pPixmapDiagram->m_colBgPartCenter = m_colBgPartCenter;

        pPixmapDiagram->m_size = m_size;
        pPixmapDiagram->m_cyMinimumHeightPartTop = m_cyMinimumHeightPartTop;
        pPixmapDiagram->m_cyMinimumHeightPartBottom = m_cyMinimumHeightPartBottom;
        pPixmapDiagram->m_cxMinimumWidthPartLeft = m_cxMinimumWidthPartLeft;
        pPixmapDiagram->m_cxMinimumWidthPartRight = m_cxMinimumWidthPartRight;
        pPixmapDiagram->m_iMarginTop = m_iMarginTop;
        pPixmapDiagram->m_iMarginBottom = m_iMarginBottom;
        pPixmapDiagram->m_iMarginLeft = m_iMarginLeft;
        pPixmapDiagram->m_iMarginRight = m_iMarginRight;
        pPixmapDiagram->m_rectPartTop = m_rectPartTop;
        pPixmapDiagram->m_rectPartBottom = m_rectPartBottom;
        pPixmapDiagram->m_rectPartLeft = m_rectPartLeft;
        pPixmapDiagram->m_rectPartRight = m_rectPartRight;
        pPixmapDiagram->m_rectPartCenter = m_rectPartCenter;

        if( m_pFrameStyle != nullptr )
        {
            pPixmapDiagram->m_pFrameStyle = new SFrameStyle(*m_pFrameStyle);
        }
        if( m_pFrameStylePartCenter != nullptr )
        {
            pPixmapDiagram->m_pFrameStylePartCenter = new SFrameStyle(*m_pFrameStylePartCenter);
        }
        pDiagram = pPixmapDiagram;
    }
    return pDiagram;

} // clone

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CPixmapDiagram::save( const QString& i_strFileName, EImageFormat i_imageFormat )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "save",
        /* strAddInfo   */ "" );

    bool bSuccess = false;

    if( m_uUpdateFlags != EUpdateNone )
    {
        // Update pixmap (but not the widget)
        updateData();
        updatePixmap();
    }

    if( m_pPixmap != nullptr && i_strFileName.length() > 0
     && i_imageFormat >= EImageFormatMin && i_imageFormat <= EImageFormatMax )
    {
        QString strFileName = i_strFileName;
        bool    bFileNameHasValidExtension = false;
        int     idxImageFormat;

        for( idxImageFormat = 0; idxImageFormat < EImageFormatCount; idxImageFormat++ )
        {
            #if QT_VERSION >= 0x040100
            if( strFileName.endsWith( "." + QString(g_arszImageFormatFileExt[idxImageFormat]), Qt::CaseInsensitive ) )
            #else
            if( strFileName.endsWith( "." + QString(g_arszImageFormatFileExt[idxImageFormat]), false ) )
            #endif
            {
                bFileNameHasValidExtension = true;
                break;
            }
        }
        if( !bFileNameHasValidExtension )
        {
            strFileName.append( "." + QString(g_arszImageFormatFileExt[i_imageFormat]) );
        }
        bSuccess = m_pPixmap->save( strFileName, g_arszQtImageFormat[i_imageFormat] );
    }
    return bSuccess;

} // save

//------------------------------------------------------------------------------
bool CPixmapDiagram::save( const QString& i_strFileName, const QString& i_strQtImageFormat )
//------------------------------------------------------------------------------
{
    return save( i_strFileName, GetImageFormat(i_strQtImageFormat) );
}

//------------------------------------------------------------------------------
bool CPixmapDiagram::save( QIODevice* i_pDevice, EImageFormat i_imageFormat )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "save",
        /* strAddInfo   */ "" );

    bool bSuccess = false;

    if( m_uUpdateFlags != EUpdateNone )
    {
        // Update pixmap (but not the widget)
        updateData();
        updatePixmap();
    }

    if( m_pPixmap != nullptr && i_pDevice != nullptr
     && i_imageFormat >= EImageFormatMin && i_imageFormat <= EImageFormatMax )
    {
        bSuccess = m_pPixmap->save( i_pDevice, g_arszQtImageFormat[i_imageFormat] );
    }
    return bSuccess;

} // save

//------------------------------------------------------------------------------
bool CPixmapDiagram::save( QIODevice* i_pDevice, const QString& i_strQtImageFormat )
//------------------------------------------------------------------------------
{
    return save( i_pDevice, GetImageFormat(i_strQtImageFormat) );
}

//------------------------------------------------------------------------------
void CPixmapDiagram::print( const QPrinter* i_pPrinter ) const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "print",
        /* strAddInfo   */ "" );

    if( i_pPrinter != nullptr )
    {
        //QPainter painter;

        //i_pPrinter->setFullPage(true);

        //if( !painter.begin(i_pPrinter) )
        //{
        //    return;
        //}

        //painter.drawText(20,20,"hallo");
        //paintDiagObjects(&painter);

        //QPaintDeviceMetrics metrics( painter.device() );

        //int dpiy = metrics.logicalDpiY();
        //int iMargin = static_cast<int>( (2.0/2.54)*dpiy ); // 2 cm margins

        //QRect rctView( iMargin, iMargin, metrics.width() - 2*iMargin, metrics.height() - 2*iMargin );

        //QSimpleRichText richText( QStyleSheet::convertFromPlainText(medit->text()),
        //                          QFont(),
        //                          medit->context(),
        //                          medit->styleSheet(),
        //                          medit->mimeSourceFactory(),
        //                          rctView.height() );
        //richText.setWidth( &painter, rctView.width() );

        //int iPage = 1;

        //do {

            //richText.draw( &painter, iMargin, iMargin, rctView, colorGroup() );

            //rctView.moveBy( 0, rctView.height() );

            //painter.translate( 0 , -rctView.height() );

            //painter.drawText(
            //    rctView.right() - painter.fontMetrics().width( QString::number(iPage) ),
            //    rctView.bottom() + painter.fontMetrics().ascent() + 5,
            //    QString::number( iPage ) );

            //if ( rctView.top() - iMargin >= richText.height() )
            //{
            //    break;
            //}

            //QString msg( "Printing (page " );
            //msg += QString::number( ++pageNo );
            //msg += ")...";

            //i_pPrinter->newPage();

            //iPage++;

        //} while( true );

    } // if( i_pPrinter != nullptr )

} // print

/*==============================================================================
public: // instance methods to set and get the attributes of the diagram
==============================================================================*/

//------------------------------------------------------------------------------
void CPixmapDiagram::setWidth( int i_cxWidth )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_cxWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strTrcMsg );

    QSize sizeNew = m_size;
    sizeNew.setWidth(i_cxWidth);
    setSize(sizeNew);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getWidth() const
//------------------------------------------------------------------------------
{
    return m_size.width();
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setHeight( int i_cyHeight )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_cyHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strTrcMsg );

    QSize sizeNew = m_size;
    sizeNew.setHeight(i_cyHeight);
    setSize(sizeNew);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getHeight() const
//------------------------------------------------------------------------------
{
    return m_size.height();
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setSize( const QSize& i_size )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(m_size.width());
        strTrcMsg +=  "," + QString::number(m_size.height());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strTrcMsg );

    QSize sizePrev     = m_size;
    bool  bSizeChanged = false;

    if( m_size.width()  != i_size.width()
     || m_size.height() != i_size.height() )
    {
        bSizeChanged = true;
        m_size = i_size;
    }

    if( bSizeChanged )
    {
        int cxWidthDist  = m_size.width() - sizePrev.width();
        int cyHeightDist = m_size.height() - sizePrev.height();

        // Before updating the layout the scale objects will be provided with their
        // (probable) new extents. Those extent may change later on if the real
        // size of the diagram areas will be calculated as some objects may change
        // their size according to the number and position of the scale division lines.
        // But setting the (probable) scale geometry in advance may save time ...
        m_rectPartCenter.setWidth( m_rectPartCenter.width() + cxWidthDist );
        m_rectPartCenter.setHeight( m_rectPartCenter.height() + cyHeightDist );

        // It is NOT sufficient to mark layout processing as necessary.
        // Even if on executing the layout process the geometry of the objects
        // might be changed (if necessary) and the necessary update flags would
        // be set anyway. To recalculate the geometry of the objects some of those
        // objects need to recalculate their internal data in advance to provide
        // the correct sizeHint for the diagram. E.g. on resizing the diagram the
        // number of division lines might change and therefore an axis label object
        // need to change the number of digits to scale the diagram.
        invalidate(nullptr,EUpdateLayoutDataPixmapWidget);

        // Layout processing is sufficient as a paint event will follow anyway and there
        // are more resive events than paint events. So it looks like that its more
        // efficient to let the objects recalculate their data and graphical output
        // later within the paint event method.
        //CDataDiagram::update(EUpdateLayout);
    }

} // setSize

//------------------------------------------------------------------------------
QSize CPixmapDiagram::getSize() const
//------------------------------------------------------------------------------
{
    return m_size;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMarginTop( int i_iMargin )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_iMargin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setMarginTop",
        /* strAddInfo   */ strTrcMsg );

    m_iMarginTop = i_iMargin;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMarginTop() const
//------------------------------------------------------------------------------
{
    return m_iMarginTop;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMarginBottom( int i_iMargin )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_iMargin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setMarginBottom",
        /* strAddInfo   */ strTrcMsg );

    m_iMarginBottom = i_iMargin;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMarginBottom() const
//------------------------------------------------------------------------------
{
    return m_iMarginBottom;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMarginLeft( int i_iMargin )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_iMargin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setMarginLeft",
        /* strAddInfo   */ strTrcMsg );

    m_iMarginLeft = i_iMargin;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMarginLeft() const
//------------------------------------------------------------------------------
{
    return m_iMarginLeft;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMarginRight( int i_iMargin )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_iMargin);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setMarginRight",
        /* strAddInfo   */ strTrcMsg );

    m_iMarginRight = i_iMargin;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMarginRight() const
//------------------------------------------------------------------------------
{
    return m_iMarginRight;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartTop() const
//------------------------------------------------------------------------------
{
    return m_rectPartTop;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartBottom() const
//------------------------------------------------------------------------------
{
    return m_rectPartBottom;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartLeft() const
//------------------------------------------------------------------------------
{
    return m_rectPartLeft;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartRight() const
//------------------------------------------------------------------------------
{
    return m_rectPartRight;
}

//------------------------------------------------------------------------------
QRect CPixmapDiagram::getRectPartCenter() const
//------------------------------------------------------------------------------
{
    return m_rectPartCenter;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setColBgPartCenter( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setColBgPartCenter",
        /* strAddInfo   */ "" );

    m_colBgPartCenter = i_col;
    invalidate(nullptr,EUpdatePixmapWidget);
}

//------------------------------------------------------------------------------
QColor CPixmapDiagram::getColBgPartCenter() const
//------------------------------------------------------------------------------
{
    return m_colBgPartCenter;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMinimumHeightPartTop( int i_cyHeight )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_cyHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setMinimumHeightPartTop",
        /* strAddInfo   */ strTrcMsg );

    m_cyMinimumHeightPartTop = i_cyHeight;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMinimumHeightPartTop() const
//------------------------------------------------------------------------------
{
    return m_cyMinimumHeightPartTop;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMinimumHeightPartBottom( int i_cyHeight )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_cyHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setMinimumHeightPartBottom",
        /* strAddInfo   */ strTrcMsg );

    m_cyMinimumHeightPartBottom = i_cyHeight;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMinimumHeightPartBottom() const
//------------------------------------------------------------------------------
{
    return m_cyMinimumHeightPartBottom;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMinimumWidthPartLeft( int i_cxWidth )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_cxWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setMinimumWidthPartLeft",
        /* strAddInfo   */ strTrcMsg );

    m_cxMinimumWidthPartLeft = i_cxWidth;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMinimumWidthPartLeft() const
//------------------------------------------------------------------------------
{
    return m_cxMinimumWidthPartLeft;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setMinimumWidthPartRight( int i_cxWidth )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg = QString::number(i_cxWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setMinimumWidthPartRight",
        /* strAddInfo   */ strTrcMsg );

    m_cxMinimumWidthPartRight = i_cxWidth;
    invalidate(nullptr,EUpdateLayoutDataPixmapWidget);
}

//------------------------------------------------------------------------------
int CPixmapDiagram::getMinimumWidthPartRight() const
//------------------------------------------------------------------------------
{
    return m_cxMinimumWidthPartRight;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setColErase( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setColErase",
        /* strAddInfo   */ "" );

    m_colErase = i_col;
    invalidate(nullptr,EUpdatePixmapWidget);
}

//------------------------------------------------------------------------------
QColor CPixmapDiagram::getColErase() const
//------------------------------------------------------------------------------
{
    return m_colErase;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setFrameStyle( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setFrameStyle",
        /* strAddInfo   */ "" );

    delete m_pFrameStyle;
    m_pFrameStyle = i_pFrameStyle;

} // setFrameStyle

//------------------------------------------------------------------------------
SFrameStyle* CPixmapDiagram::getFrameStyle()
//------------------------------------------------------------------------------
{
    return m_pFrameStyle;
}

//------------------------------------------------------------------------------
void CPixmapDiagram::setFrameStylePartCenter( SFrameStyle* i_pFrameStyle )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setFrameStylePartCenter",
        /* strAddInfo   */ "" );

    delete m_pFrameStylePartCenter;
    m_pFrameStylePartCenter = i_pFrameStyle;

} // setFrameStylePartCenter

//------------------------------------------------------------------------------
SFrameStyle* CPixmapDiagram::getFrameStylePartCenter()
//------------------------------------------------------------------------------
{
    return m_pFrameStylePartCenter;
}

//------------------------------------------------------------------------------
QPixmap* CPixmapDiagram::getPixmap()
//------------------------------------------------------------------------------
{
    return m_pPixmap;
}

/*==============================================================================
public: // overridables ob base class CDataDiagram
==============================================================================*/

//lint -esym(818,i_pDiagObj)
//------------------------------------------------------------------------------
void CPixmapDiagram::invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelValidate) )
    {
        if( i_pDiagObj != nullptr )
        {
            strTrcMsg = i_pDiagObj->getObjName();
        }
        strTrcMsg += updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelValidate,
        /* strMethod    */ "invalidate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        unsigned int uUpdateFlagsForObjects = EUpdateNone;

        // If a bit is set the corresponding update process need to be executed
        // (keeping the current update processes that still need to be executed):
        m_uUpdateFlags |= i_uUpdateFlags;

        // If method was called by the diagram itself ..
        if( i_pDiagObj == nullptr )
        {
            // .. we pass on the update flags to the objects.
            uUpdateFlagsForObjects = i_uUpdateFlags;
        }
        // If the method was called by one of the diagram objects ..
        else
        {
            // If pixmap processing is necessary for one object
            // all other objects also need to be redrawn.
            if( i_uUpdateFlags & EUpdatePixmap )
            {
                uUpdateFlagsForObjects = EUpdatePixmap;
            }
        }

        // If layout processing was announced to the diagram because of e.g. changing
        // the visibility of a diagram object the other objects also need to be invalidated.
        // It is not sufficient to have the layout update flag set at the diagram even if
        // this leads to "setGeometry" calls - if necessary. On changing the geometry of an
        // object the object will be invalidated anyway. But not if the object does not change
        // its geometry ...

        // If pixmap processing is necessary for one object
        // all other objects also need to be redrawn.
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
                pDiagObj = pDiagObj->m_pDiagObjNext; //lint !e613 .. da irrt sich blint
            }
        }
    }

    if( mthTracer.isActive(c_iTrcDetailLevelValidate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelValidate);
    }

} // invalidate
//lint +esym(818,i_pDiagObj)


/*==============================================================================
protected: // overridables ob base class CDataDiagram
==============================================================================*/

//------------------------------------------------------------------------------
void CPixmapDiagram::updateLayout()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "updateLayout",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    QSize sizePixmap;

    #if QT_VERSION >= 0x040100

    delete m_pPixmap;
    m_pPixmap = nullptr;
    m_pPixmap = new QPixmap(m_size);

    #else

    if( m_size.isEmpty() )
    {
        delete m_pPixmap;
        m_pPixmap = nullptr;
    }
    else
    {
        if( m_pPixmap == nullptr )
        {
            m_pPixmap = new QPixmap(m_size);
        }
        else if( m_pPixmap->size() != m_size )
        {
            m_pPixmap->resize(m_size);
        }
    }
    #endif

    // Please note that the center area of the diagram is used by e.g. grid or
    // curve objects and is considered to be expandable.
    // All other (used) areas (left, top, right, bottom) are usually used
    // by axis labels and are considered to have a minimum size which will be
    // queried by the "sizeHint" methods.

    // As the size of the center area is expandable its width and height may
    // need to be changed. The scale objects extent correspond to the extent
    // of the center area and therefore the number and position of the division
    // lines depend on the size of the center area. The sizeHint of some objects
    // (e.g. axis labels) again depend on the number and position of the division
    // lines. And again the center area may need to be resized.
    // So the whole layout stuff is an iterative process and will be repeated until
    // the size of center area is no longer changing ...

    CDiagObj*    pDiagObj;
    CDiagScale*  pDiagScale;
    CDiagTrace*  pDiagTrace;
    QRect        rectPartCenterPrev;
    int          iIterations;
    int          cyPartTopHeight;
    int          cyPartBottomHeight;
    int          cxPartLeftWidth;
    int          cxPartRightWidth;
    int          cxPartCenterWidth;
    int          cyPartCenterHeight;
    int          iFrameWidth = 0;
    int          iFrameWidthPartCenter = 0;
    int          iDiagObjExtent;
    int*         ariDiagObjOffset = new int[m_iDiagObjCount];
    int*         ariDiagObjExtent = new int[m_iDiagObjCount];
    ELayoutPos*  arLytPosDiagObj = new ELayoutPos[m_iDiagObjCount];
    int*         piDiagObjOffset;
    int*         piDiagObjExtent;
    ELayoutPos*  pLytPosDiagObj;
    QRect        rectDiagObjContent;

    if( m_pFrameStyle != nullptr )
    {
        iFrameWidth = m_pFrameStyle->getFrameWidth();
    }

    if( m_pFrameStylePartCenter != nullptr )
    {
        QRect rectFramePartCenterTopLineInv;
        QRect rectFramePartCenterBottomLineInv;
        QRect rectFramePartCenterLeftLineInv;
        QRect rectFramePartCenterRightLineInv;

        iFrameWidthPartCenter = m_pFrameStylePartCenter->getFrameWidth();

        rectFramePartCenterTopLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setWidth( m_rectPartCenter.width() + 2*iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setHeight( iFrameWidthPartCenter );

        rectFramePartCenterBottomLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterBottomLineInv.setTop( m_rectPartCenter.bottom() );
        rectFramePartCenterBottomLineInv.setWidth( m_rectPartCenter.width() + 2*iFrameWidthPartCenter );
        rectFramePartCenterBottomLineInv.setHeight( iFrameWidthPartCenter );

        rectFramePartCenterLeftLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setWidth( iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setHeight( m_rectPartCenter.height() + 2*iFrameWidthPartCenter );

        rectFramePartCenterRightLineInv.setLeft( m_rectPartCenter.right() );
        rectFramePartCenterRightLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterRightLineInv.setWidth( iFrameWidthPartCenter );
        rectFramePartCenterRightLineInv.setHeight( m_rectPartCenter.height() + 2*iFrameWidthPartCenter );

        m_rectFramePartCenterTopLineInv    |= rectFramePartCenterTopLineInv;
        m_rectFramePartCenterBottomLineInv |= rectFramePartCenterBottomLineInv;
        m_rectFramePartCenterLeftLineInv   |= rectFramePartCenterLeftLineInv;
        m_rectFramePartCenterRightLineInv  |= rectFramePartCenterRightLineInv;
    }

    // Before updating the layout the scale objects will be provided with their
    // (probable) new extents. Those extent may change later on if the real
    // size of the diagram areas will be calculated as some objects may change
    // their size according to the number and position of the scale division lines.
    // But setting the (probable) scale geometry in advance may save time ...
    pDiagScale = m_pDiagScaleFirst;
    while( pDiagScale != nullptr )
    {
        // If the scale will be changed the method "scaleChanged" of the diagram will be
        // called invalidating the data of all objects linked to the changed scale.
        switch( pDiagScale->getScaleDir() )
        {
            case EScaleDirX:
            {
                if( pDiagScale->getScaleMinValPix() != m_rectPartCenter.left()
                 || pDiagScale->getScaleMaxValPix() != m_rectPartCenter.right() )
                {
                    pDiagScale->setGeometry(m_rectPartCenter.left(),m_rectPartCenter.right());
                }
                break;
            }
            case EScaleDirY:
            {
                if( pDiagScale->getScaleMinValPix() != m_rectPartCenter.bottom()
                 || pDiagScale->getScaleMaxValPix() != m_rectPartCenter.top() )
                {
                    pDiagScale->setGeometry(m_rectPartCenter.bottom(),m_rectPartCenter.top());
                }
                break;
            }
            default:
            {
                break;
            }
        }
        pDiagScale = pDiagScale->m_pDiagScaleNext;

    } // while( pDiagScale != nullptr )

    // As long as the maximum number of iterations is not exceeded ...
    for( iIterations = 0; iIterations < 10; iIterations++ )
    {
        cyPartTopHeight    = 0;
        cyPartBottomHeight = 0;
        cxPartLeftWidth    = 0;
        cxPartRightWidth   = 0;
        cxPartCenterWidth  = 0;
        cyPartCenterHeight = 0;
        rectPartCenterPrev = m_rectPartCenter;

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
            pDiagTrace->update(EUpdateLayout);
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
            // during widget processing. For invisible objects layout processing is not
            // really necessary here. But maybe some objects need layout processing to
            // calculate their dirty rectangles. For this layout processing will also be
            // executed for invisible objects. But that should not slow down the
            // the system as only visible objects should really recalculate their data.
            pDiagObj->update(EUpdateLayout);

            pDiagObj = pDiagObj->m_pDiagObjNext;
        }

        pDiagObj        = m_pDiagObjFirst;
        piDiagObjOffset = &ariDiagObjOffset[0];
        piDiagObjExtent = &ariDiagObjExtent[0];
        pLytPosDiagObj  = &arLytPosDiagObj[0];
        while( pDiagObj != nullptr )
        {
            if( pDiagObj->isVisible() )
            {
                switch( pDiagObj->getLayoutPos() )
                {
                    case ELayoutPosTop:
                    {
                        iDiagObjExtent   = pDiagObj->sizeHint().height();
                        *piDiagObjOffset = cyPartTopHeight;
                        *piDiagObjExtent = iDiagObjExtent;
                        cyPartTopHeight += iDiagObjExtent;
                        break;
                    }
                    case ELayoutPosBottom:
                    {
                        iDiagObjExtent      = pDiagObj->sizeHint().height();
                        *piDiagObjOffset    = cyPartBottomHeight;
                        *piDiagObjExtent    = iDiagObjExtent;
                        cyPartBottomHeight += iDiagObjExtent;
                        break;
                    }
                    case ELayoutPosLeft:
                    {
                        iDiagObjExtent   = pDiagObj->sizeHint().width();
                        *piDiagObjOffset = cxPartLeftWidth;
                        *piDiagObjExtent = iDiagObjExtent;
                        cxPartLeftWidth += iDiagObjExtent;
                        break;
                    }
                    case ELayoutPosRight:
                    {
                        iDiagObjExtent    = pDiagObj->sizeHint().width();
                        *piDiagObjOffset  = cxPartRightWidth;
                        *piDiagObjExtent  = iDiagObjExtent;
                        cxPartRightWidth += iDiagObjExtent;
                        break;
                    }
                    case ELayoutPosCenter:
                    {
                        *piDiagObjOffset = 0;
                        *piDiagObjExtent = 0;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            else // if( !pDiagObj->isVisible() )
            {
                *piDiagObjOffset = 0;
                *piDiagObjExtent = 0;
            }
            *pLytPosDiagObj = pDiagObj->getLayoutPos();

            pDiagObj = pDiagObj->m_pDiagObjNext;
            piDiagObjOffset++;
            piDiagObjExtent++;
            pLytPosDiagObj++;

        } // while( pDiagObj != nullptr )

        if( m_cyMinimumHeightPartTop > 0 && cyPartTopHeight < m_cyMinimumHeightPartTop )
        {
            cyPartTopHeight = m_cyMinimumHeightPartTop;
        }
        if( m_cyMinimumHeightPartBottom > 0 && cyPartBottomHeight < m_cyMinimumHeightPartBottom )
        {
            cyPartBottomHeight = m_cyMinimumHeightPartBottom;
        }
        if( m_cxMinimumWidthPartLeft > 0 && cxPartLeftWidth < m_cxMinimumWidthPartLeft )
        {
            cxPartLeftWidth = m_cxMinimumWidthPartLeft;
        }
        if( m_cxMinimumWidthPartRight > 0 && cxPartRightWidth < m_cxMinimumWidthPartRight )
        {
            cxPartRightWidth = m_cxMinimumWidthPartRight;
        }

        // If we got the minimum sizes for each border area we are able to calculate the
        // size of the (expandable) center area and to set the new rectangle extents
        // and positions of the diagram objects.
        cxPartCenterWidth  = m_size.width() - m_iMarginLeft - m_iMarginRight - 2*iFrameWidth - cxPartLeftWidth - cxPartRightWidth - 2*iFrameWidthPartCenter;     //lint !e834
        cyPartCenterHeight = m_size.height() - m_iMarginTop - m_iMarginBottom - 2*iFrameWidth - cyPartTopHeight - cyPartBottomHeight - 2*iFrameWidthPartCenter;  //lint !e834

        // Calculate the center area
        m_rectPartCenter.setLeft(m_iMarginLeft+iFrameWidth+cxPartLeftWidth+iFrameWidthPartCenter);
        m_rectPartCenter.setTop(m_iMarginTop+iFrameWidth+cyPartTopHeight+iFrameWidthPartCenter);
        m_rectPartCenter.setWidth(cxPartCenterWidth);
        m_rectPartCenter.setHeight(cyPartCenterHeight);

        // If the size of the center area did not change ...
        if( m_rectPartCenter == rectPartCenterPrev )
        {
            break;
        }

        // Set the scale extent of the scale objects. Please note that the scales just
        // store their new extent but do not recalculate the division lines.
        // This must be triggered later ...
        pDiagScale = m_pDiagScaleFirst;
        while( pDiagScale != nullptr )
        {
            // If the scale will be changed the method "scaleChanged" of the diagram will be
            // called invalidating the data of all objects linked to the changed scale.
            switch( pDiagScale->getScaleDir() )
            {
                case EScaleDirX:
                {
                    if( pDiagScale->getScaleMinValPix() != m_rectPartCenter.left()
                     || pDiagScale->getScaleMaxValPix() != m_rectPartCenter.right() )
                    {
                        pDiagScale->setGeometry(m_rectPartCenter.left(),m_rectPartCenter.right());
                    }
                    break;
                }
                case EScaleDirY:
                {
                    if( pDiagScale->getScaleMinValPix() != m_rectPartCenter.bottom()
                     || pDiagScale->getScaleMaxValPix() != m_rectPartCenter.top() )
                    {
                        pDiagScale->setGeometry(m_rectPartCenter.bottom(),m_rectPartCenter.top());
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            pDiagScale = pDiagScale->m_pDiagScaleNext;

        } // while( pDiagScale != nullptr )

    } // for( iIterations < 10 )

    if( m_pFrameStylePartCenter != nullptr )
    {
        QRect rectFramePartCenterTopLineInv;
        QRect rectFramePartCenterBottomLineInv;
        QRect rectFramePartCenterLeftLineInv;
        QRect rectFramePartCenterRightLineInv;

        iFrameWidthPartCenter = m_pFrameStylePartCenter->getFrameWidth();

        rectFramePartCenterTopLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setWidth( m_rectPartCenter.width() + 2*iFrameWidthPartCenter );
        rectFramePartCenterTopLineInv.setHeight( iFrameWidthPartCenter );

        rectFramePartCenterBottomLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterBottomLineInv.setTop( m_rectPartCenter.bottom() );
        rectFramePartCenterBottomLineInv.setWidth( m_rectPartCenter.width() + 2*iFrameWidthPartCenter );
        rectFramePartCenterBottomLineInv.setHeight( iFrameWidthPartCenter );

        rectFramePartCenterLeftLineInv.setLeft( m_rectPartCenter.left() - iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setWidth( iFrameWidthPartCenter );
        rectFramePartCenterLeftLineInv.setHeight( m_rectPartCenter.height() + 2*iFrameWidthPartCenter );

        rectFramePartCenterRightLineInv.setLeft( m_rectPartCenter.right() );
        rectFramePartCenterRightLineInv.setTop( m_rectPartCenter.top() - iFrameWidthPartCenter );
        rectFramePartCenterRightLineInv.setWidth( iFrameWidthPartCenter );
        rectFramePartCenterRightLineInv.setHeight( m_rectPartCenter.height() + 2*iFrameWidthPartCenter );

        m_rectFramePartCenterTopLineInv    |= rectFramePartCenterTopLineInv;
        m_rectFramePartCenterBottomLineInv |= rectFramePartCenterBottomLineInv;
        m_rectFramePartCenterLeftLineInv   |= rectFramePartCenterLeftLineInv;
        m_rectFramePartCenterRightLineInv  |= rectFramePartCenterRightLineInv;
    }

    // Calculate the border areas which overlap at the corners:
    // (could be optimised by just calculating the used areas)
    m_rectPartTop.setLeft(m_iMarginLeft+iFrameWidth);
    m_rectPartTop.setTop(m_iMarginTop+iFrameWidth);
    m_rectPartTop.setWidth(m_size.width()-m_iMarginLeft-m_iMarginRight-2*iFrameWidth);
    m_rectPartTop.setHeight(cyPartTopHeight);
    m_rectPartBottom.setLeft(m_iMarginLeft+iFrameWidth);
    m_rectPartBottom.setTop(m_iMarginTop+iFrameWidth+cyPartTopHeight+cyPartCenterHeight+2*iFrameWidthPartCenter);
    m_rectPartBottom.setWidth(m_size.width()-m_iMarginLeft-m_iMarginRight-2*iFrameWidth);
    m_rectPartBottom.setHeight(cyPartBottomHeight);
    m_rectPartLeft.setLeft(m_iMarginLeft+iFrameWidth);
    m_rectPartLeft.setTop(m_iMarginTop+iFrameWidth);
    m_rectPartLeft.setWidth(cxPartLeftWidth);
    m_rectPartLeft.setHeight(m_size.height()-m_iMarginTop-m_iMarginBottom-2*iFrameWidth);
    m_rectPartRight.setLeft(m_iMarginLeft+iFrameWidth+cxPartLeftWidth+cxPartCenterWidth+2*iFrameWidthPartCenter);
    m_rectPartRight.setTop(m_iMarginTop+iFrameWidth);
    m_rectPartRight.setWidth(cxPartRightWidth);
    m_rectPartRight.setHeight(m_size.height()-m_iMarginTop-m_iMarginBottom-2*iFrameWidth);

    if( mthTracer.isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg  = "Areas calculated after ";
        strTrcMsg += QString::number(iIterations) + " Iterations:";
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelLayout);

        strTrcMsg  = "PartTop[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartTop.x()) + ",";
        strTrcMsg += QString::number(m_rectPartTop.y()) + ",";
        strTrcMsg += QString::number(m_rectPartTop.width()) + ",";
        strTrcMsg += QString::number(m_rectPartTop.height());
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelLayout);

        strTrcMsg  = "PartBottom[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartBottom.x()) + ",";
        strTrcMsg += QString::number(m_rectPartBottom.y()) + ",";
        strTrcMsg += QString::number(m_rectPartBottom.width()) + ",";
        strTrcMsg += QString::number(m_rectPartBottom.height());
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelLayout);

        strTrcMsg  = "PartLeft[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartLeft.x()) + ",";
        strTrcMsg += QString::number(m_rectPartLeft.y()) + ",";
        strTrcMsg += QString::number(m_rectPartLeft.width()) + ",";
        strTrcMsg += QString::number(m_rectPartLeft.height());
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelLayout);

        strTrcMsg  = "PartRight[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartRight.x()) + ",";
        strTrcMsg += QString::number(m_rectPartRight.y()) + ",";
        strTrcMsg += QString::number(m_rectPartRight.width()) + ",";
        strTrcMsg += QString::number(m_rectPartRight.height());
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelLayout);

        strTrcMsg  = "PartCenter[x,y,w,h]=";
        strTrcMsg += QString::number(m_rectPartCenter.x()) + ",";
        strTrcMsg += QString::number(m_rectPartCenter.y()) + ",";
        strTrcMsg += QString::number(m_rectPartCenter.width()) + ",";
        strTrcMsg += QString::number(m_rectPartCenter.height());
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelLayout);
    }

    // Move and resize the diagram objects according to the newly calculated
    // rectangle extents of the border areas:
    pDiagObj        = m_pDiagObjFirst;
    piDiagObjOffset = &ariDiagObjOffset[0];
    piDiagObjExtent = &ariDiagObjExtent[0];
    pLytPosDiagObj  = &arLytPosDiagObj[0];
    while( pDiagObj != nullptr )
    {
        switch( pDiagObj->getLayoutPos() )
        {
            case ELayoutPosTop:
            {
                rectDiagObjContent.setWidth( m_rectPartTop.width() );
                rectDiagObjContent.setHeight( *piDiagObjExtent );
                rectDiagObjContent.moveLeft( m_rectPartTop.left() );
                rectDiagObjContent.moveBottom( m_rectPartTop.bottom() - *piDiagObjOffset );

                if( pDiagObj->geometry() != rectDiagObjContent )
                {
                    pDiagObj->setGeometry(rectDiagObjContent,false);
                }
                break;
            }
            case ELayoutPosBottom:
            {
                rectDiagObjContent.setWidth( m_rectPartBottom.width() );
                rectDiagObjContent.setHeight( *piDiagObjExtent );
                rectDiagObjContent.moveLeft( m_rectPartBottom.left() );
                rectDiagObjContent.moveTop( m_rectPartBottom.top() + *piDiagObjOffset );

                if( pDiagObj->geometry() != rectDiagObjContent )
                {
                    pDiagObj->setGeometry(rectDiagObjContent,false);
                }
                break;
            }
            case ELayoutPosLeft:
            {
                rectDiagObjContent.setWidth( *piDiagObjExtent );
                rectDiagObjContent.setHeight( m_rectPartLeft.height() );
                rectDiagObjContent.moveRight( m_rectPartLeft.right() - *piDiagObjOffset );
                rectDiagObjContent.moveTop( m_rectPartLeft.top() );

                if( pDiagObj->geometry() != rectDiagObjContent )
                {
                    pDiagObj->setGeometry(rectDiagObjContent,false);
                }
                break;
            }
            case ELayoutPosRight:
            {
                rectDiagObjContent.setWidth( *piDiagObjExtent );
                rectDiagObjContent.setHeight( m_rectPartRight.height() );
                rectDiagObjContent.moveLeft( m_rectPartRight.left() + *piDiagObjOffset );
                rectDiagObjContent.moveTop( m_rectPartRight.top() );

                if( pDiagObj->geometry() != rectDiagObjContent )
                {
                    pDiagObj->setGeometry(rectDiagObjContent,false);
                }
                break;
            }
            case ELayoutPosCenter:
            {
                if( pDiagObj->geometry() != m_rectPartCenter )
                {
                    pDiagObj->setGeometry(m_rectPartCenter,false);
                }
                break;
            }
            default:
            {
                break;
            }
        }

        pDiagObj = pDiagObj->m_pDiagObjNext;
        piDiagObjOffset++;
        piDiagObjExtent++;
        pLytPosDiagObj++;

    } // while( pDiagObj != nullptr )

    delete [] ariDiagObjOffset;
    ariDiagObjOffset = nullptr;
    delete [] ariDiagObjExtent;
    ariDiagObjExtent = nullptr;
    delete [] arLytPosDiagObj;
    arLytPosDiagObj = nullptr;

    // Mark current process depth as executed (reset bit):
    pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        pDiagObj->validate(EUpdateLayout);
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }
    validate(EUpdateLayout);

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

} // updateLayout

//------------------------------------------------------------------------------
void CPixmapDiagram::updatePixmap()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "updatePixmap",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    // The scale objects are "virtual" (not visible) objects and don't draw into a bitmap.

    if( m_pPixmap == nullptr )
    {
        return;
    }

    QBrush    brush;
    QPen      pen;
    QPainter  painterPixmap;
    QRect     rectFrame;
    CDiagObj* pDiagObj;

    painterPixmap.begin(m_pPixmap);

    // Clear bitmap with the widgets current background color.
    m_pPixmap->fill(m_colErase);

    //#define DEBUG_AREAS

    #ifdef DEBUG_AREAS
    painterPixmap.setPen(Qt::black);
    painterPixmap.drawRect(0,0,getWidth()-1,getHeight()-1);
    rectFrame.setLeft(m_iMarginLeft);
    rectFrame.setTop(m_iMarginTop);
    rectFrame.setWidth( getWidth() - m_iMarginLeft - m_iMarginRight );
    rectFrame.setHeight( getHeight() - m_iMarginTop - m_iMarginBottom );
    brush.setColor(Qt::blue);
    painterPixmap.fillRect(rectFrame,brush);
    #endif

    // Fill center area with background color.
    brush.setColor(m_colBgPartCenter);
    brush.setStyle(Qt::SolidPattern);
    if( m_rectPartCenter.width() > 0 && m_rectPartCenter.height() > 0 )
    {
        painterPixmap.fillRect(m_rectPartCenter,brush);
    }

    // Frame around the whole diagram.
    #ifndef DEBUG_AREAS
    if( m_pFrameStyle != nullptr )
    {
        // The frame will be drawn WITHIN the widget area.
        rectFrame.setLeft(m_iMarginLeft);
        rectFrame.setTop(m_iMarginTop);
        rectFrame.setWidth( getWidth() - m_iMarginLeft - m_iMarginRight );
        rectFrame.setHeight( getHeight() - m_iMarginTop - m_iMarginBottom );
        m_pFrameStyle->draw( &painterPixmap, rectFrame );
    }
    #endif

    // Frame around the center part (output region for X/Y values)
    #ifndef DEBUG_AREAS
    if( m_pFrameStylePartCenter != nullptr )
    {
        // The frame will be drawn AROUND the center area.
        rectFrame.setLeft( m_rectPartCenter.left() - m_pFrameStylePartCenter->getFrameWidth() );
        rectFrame.setTop( m_rectPartCenter.top() - m_pFrameStylePartCenter->getFrameWidth() );
        rectFrame.setWidth( m_rectPartCenter.width() + 2*m_pFrameStylePartCenter->getFrameWidth() );
        rectFrame.setHeight( m_rectPartCenter.height() + 2*m_pFrameStylePartCenter->getFrameWidth() );
        m_pFrameStylePartCenter->draw( &painterPixmap, rectFrame );
    }
    #endif

    #ifdef DEBUG_AREAS
    brush.setColor(Qt::green);
    painterPixmap.fillRect(getRectPartTop(),brush);
    painterPixmap.fillRect(getRectPartBottom(),brush);
    painterPixmap.fillRect(getRectPartLeft(),brush);
    painterPixmap.fillRect(getRectPartRight(),brush);
    #endif

    painterPixmap.end();

    // Let the diagram objects draw their data into the bitmap.
    pDiagObj = m_pDiagObjPaintFirst;
    while( pDiagObj != nullptr )
    {
        // Please note that the pixmap has been erased before and therefore each object
        // need to be redrawn. Thats why before calling "update" of the object the pixmap
        // and widget process depth will be invalidated.
        if( pDiagObj->isVisible() )
        {
            pDiagObj->invalidate(EUpdatePixmapWidget,false);
            pDiagObj->update(EUpdatePixmap,m_pPixmap);
        }
        pDiagObj = pDiagObj->m_pDiagObjPaintNext;
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdatePixmap);

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

} // updatePixmap


/*******************************************************************************
class CWdgtDiagram : public QWidget, public CPixmapDiagram
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDiagram::CWdgtDiagram( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    CPixmapDiagram(EDiagramUpdateTypeWidget),
    m_bZoomingXScaleEnabled(false),
    m_bZoomingYScaleEnabled(false),
    m_bIsZooming(false),
    m_ptZoomStart(),
    m_ptZoomEnd(),
    m_rectZoom(),
    m_iZoomCount(0),
    m_bContextPopupMenuEnabled(false),
    m_pMenuContext(nullptr),
    m_strTipOfTheDay(),
    m_iTipOfTheDayIndentTop(20),
    m_iTipOfTheDayIndentBottom(20),
    m_iTipOfTheDayIndentLeft(20),
    m_iTipOfTheDayIndentRight(20),
    m_pLblTipOfTheDay(nullptr),
    m_strCurrentFileDir(),
    m_bMoveKeyAccelerationEnabled(true),
    m_pMoveKeyAccelerationDiagScale(nullptr),
    m_iMoveKeyAccelerationTimeout_ms(1000),
    m_iMoveKeyAccelerationTimeoutSlowDown_ms(400),
    m_iMoveKeyAccelerationTimeoutSpeedUp_ms(200),
    m_iMoveKeyAccelarationSpeedUpDelayRepCount(5),
    m_fMoveKeyAccelerationFactor(2.0),
    m_fMoveKeyAccelerationMaxStepScaleFactor(0.02),
    m_fMoveKeyEventTime_ms(0.0),
    m_fMoveKeyRepCount(0.0),
    m_iMoveKeySpeedUpDelayRepCount(0),
    m_iMoveKeyPrev(-1),
    m_pDiagObjFocused(nullptr),
    m_pDiagObjFocusedByKeyEvent(nullptr),
    m_pDiagObjFocusedByMouseEvent(nullptr),
    m_pDiagObjFocusedRecently(nullptr),
    m_pDiagObjEditing(nullptr),
    m_pDiagObjEditingByKeyEvent(nullptr),
    m_pDiagObjEditingByMouseEvent(nullptr)
{
    if( m_pTrcAdminObj == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CWdgtDiagram", m_strObjName);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    #if QT_VERSION >= 0x040100
    #else
    setWFlags( getWFlags() | Qt::WNoAutoErase | Qt::WRepaintNoErase ); //lint !e1506
    #endif

    #if QT_VERSION >= 0x040100
    #else
    m_colErase = eraseColor();
    #endif

} // ctor

//------------------------------------------------------------------------------
CWdgtDiagram::CWdgtDiagram(
    QWidget*       i_pWdgtParent,
    const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    CPixmapDiagram(i_strObjName,EDiagramUpdateTypeWidget),
    m_bZoomingXScaleEnabled(false),
    m_bZoomingYScaleEnabled(false),
    m_bIsZooming(false),
    m_ptZoomStart(),
    m_ptZoomEnd(),
    m_rectZoom(),
    m_iZoomCount(0),
    m_bContextPopupMenuEnabled(false),
    m_pMenuContext(nullptr),
    m_strTipOfTheDay(),
    m_iTipOfTheDayIndentTop(20),
    m_iTipOfTheDayIndentBottom(20),
    m_iTipOfTheDayIndentLeft(20),
    m_iTipOfTheDayIndentRight(20),
    m_pLblTipOfTheDay(nullptr),
    m_strCurrentFileDir(),
    m_bMoveKeyAccelerationEnabled(true),
    m_pMoveKeyAccelerationDiagScale(nullptr),
    m_iMoveKeyAccelerationTimeout_ms(1000),
    m_iMoveKeyAccelerationTimeoutSlowDown_ms(400),
    m_iMoveKeyAccelerationTimeoutSpeedUp_ms(200),
    m_iMoveKeyAccelarationSpeedUpDelayRepCount(5),
    m_fMoveKeyAccelerationFactor(2.0),
    m_fMoveKeyAccelerationMaxStepScaleFactor(0.02),
    m_fMoveKeyEventTime_ms(0.0),
    m_fMoveKeyRepCount(0.0),
    m_iMoveKeySpeedUpDelayRepCount(0),
    m_iMoveKeyPrev(-1),
    m_pDiagObjFocused(nullptr),
    m_pDiagObjFocusedByKeyEvent(nullptr),
    m_pDiagObjFocusedByMouseEvent(nullptr),
    m_pDiagObjFocusedRecently(nullptr),
    m_pDiagObjEditing(nullptr),
    m_pDiagObjEditingByKeyEvent(nullptr),
    m_pDiagObjEditingByMouseEvent(nullptr)
{
    if( m_pTrcAdminObj == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Diagram", "CWdgtDiagram", m_strObjName);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    #if QT_VERSION >= 0x040100
    #else
    setWFlags( getWFlags() | Qt::WNoAutoErase | Qt::WRepaintNoErase ); //lint !e1506
    #endif

    #if QT_VERSION >= 0x040100
    #else
    m_colErase = eraseColor();
    #endif

} // ctor

//------------------------------------------------------------------------------
CWdgtDiagram::~CWdgtDiagram()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pMenuContext;
    }
    catch(...)
    {
    }
    m_pMenuContext = nullptr;

    try
    {
        delete m_pLblTipOfTheDay;
    }
    catch(...)
    {
    }
    m_pLblTipOfTheDay = nullptr;

    m_pMoveKeyAccelerationDiagScale = nullptr;

    m_pDiagObjFocused = nullptr;
    m_pDiagObjFocusedByKeyEvent = nullptr;
    m_pDiagObjFocusedByMouseEvent = nullptr;
    m_pDiagObjFocusedRecently = nullptr;
    m_pDiagObjEditing = nullptr;
    m_pDiagObjEditingByKeyEvent = nullptr;
    m_pDiagObjEditingByMouseEvent = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // copy ctor not allowed but diagrams may be cloned
==============================================================================*/

//------------------------------------------------------------------------------
CDataDiagram* CWdgtDiagram::clone( EDiagramUpdateType i_diagramUpdateType ) const
//------------------------------------------------------------------------------
{
    CDataDiagram* pDiagram = nullptr;

    if( i_diagramUpdateType < EDiagramUpdateTypeWidget )
    {
        pDiagram = CPixmapDiagram::clone(i_diagramUpdateType);
    }
    else
    {
        CWdgtDiagram*     pWdgtDiagram = new CWdgtDiagram(nullptr,m_strObjName);
        int               idxScaleDir;
        const CDiagScale* pDiagScale;
        const CDiagTrace* pDiagTrace;
        const CDiagObj*   pDiagObj;
        CDiagObj*         pDiagObjCloned;

        // Data Diagram
        //-------------

        pWdgtDiagram->m_uUpdateFlags = m_uUpdateFlags;
        pWdgtDiagram->m_measState = m_measState;
        pWdgtDiagram->m_measMode = m_measMode;
        pWdgtDiagram->m_iMeasType = m_iMeasType;

        for( idxScaleDir = 0; idxScaleDir < EScaleDirCount; idxScaleDir++ )
        {
            pWdgtDiagram->m_arSpacing[idxScaleDir] = m_arSpacing[idxScaleDir];
        }

        pDiagScale = m_pDiagScaleFirst;
        while( pDiagScale != nullptr )
        {
            pDiagScale->clone(pWdgtDiagram);
            pDiagScale = pDiagScale->m_pDiagScaleNext;
        }

        pDiagTrace = m_pDiagTraceFirst;
        while( pDiagTrace != nullptr )
        {
            pDiagTrace->clone(pWdgtDiagram);
            pDiagTrace = pDiagTrace->m_pDiagTraceNext;
        }

        pDiagObj = m_pDiagObjFirst;
        while( pDiagObj != nullptr )
        {
            pDiagObj->clone(pWdgtDiagram);
            pDiagObj = pDiagObj->m_pDiagObjNext;
        }

        if( m_pDiagObjPaintFirst != nullptr && m_pDiagObjPaintLast != nullptr )
        {
            pDiagObj = m_pDiagObjPaintFirst;
            while( pDiagObj != nullptr )
            {
                pDiagObjCloned = pWdgtDiagram->getDiagObj( pDiagObj->getObjId() );

                if( pDiagObjCloned == nullptr )
                {
                    throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
                }

                if( pDiagObj->m_pDiagObjPaintPrev != nullptr && pDiagObj->m_pDiagObjPaintNext != nullptr )
                {
                    pDiagObjCloned->m_pDiagObjPaintPrev = pWdgtDiagram->getDiagObj( pDiagObj->m_pDiagObjPaintPrev->getObjId() ); //lint !e613
                    pDiagObjCloned->m_pDiagObjPaintNext = pWdgtDiagram->getDiagObj( pDiagObj->m_pDiagObjPaintNext->getObjId() ); //lint !e613
                }
                pDiagObj = pDiagObj->m_pDiagObjPaintNext;
            }
            pWdgtDiagram->m_pDiagObjPaintFirst = pWdgtDiagram->getDiagObj( m_pDiagObjPaintFirst->getObjId() );
            pWdgtDiagram->m_pDiagObjPaintLast  = pWdgtDiagram->getDiagObj( m_pDiagObjPaintLast->getObjId() );
        }

        // Pixmap Diagram
        //---------------

        pWdgtDiagram->m_colErase = m_colErase;
        pWdgtDiagram->m_colBgPartCenter = m_colBgPartCenter;

        if( m_pPixmap != nullptr )
        {
            pWdgtDiagram->m_pPixmap = new QPixmap(*m_pPixmap);
        }
        pWdgtDiagram->m_size = m_size;
        pWdgtDiagram->m_cyMinimumHeightPartTop = m_cyMinimumHeightPartTop;
        pWdgtDiagram->m_cyMinimumHeightPartBottom = m_cyMinimumHeightPartBottom;
        pWdgtDiagram->m_cxMinimumWidthPartLeft = m_cxMinimumWidthPartLeft;
        pWdgtDiagram->m_cxMinimumWidthPartRight = m_cxMinimumWidthPartRight;
        pWdgtDiagram->m_iMarginTop = m_iMarginTop;
        pWdgtDiagram->m_iMarginBottom = m_iMarginBottom;
        pWdgtDiagram->m_iMarginLeft = m_iMarginLeft;
        pWdgtDiagram->m_iMarginRight = m_iMarginRight;
        pWdgtDiagram->m_rectPartTop = m_rectPartTop;
        pWdgtDiagram->m_rectPartBottom = m_rectPartBottom;
        pWdgtDiagram->m_rectPartLeft = m_rectPartLeft;
        pWdgtDiagram->m_rectPartRight = m_rectPartRight;
        pWdgtDiagram->m_rectPartCenter = m_rectPartCenter;

        if( m_pFrameStyle != nullptr )
        {
            pWdgtDiagram->m_pFrameStyle = new SFrameStyle(*m_pFrameStyle);
        }
        if( m_pFrameStylePartCenter != nullptr )
        {
            pWdgtDiagram->m_pFrameStylePartCenter = new SFrameStyle(*m_pFrameStylePartCenter);
        }

        // Widget Diagram
        //---------------

        pWdgtDiagram->m_bZoomingXScaleEnabled = m_bZoomingXScaleEnabled;
        pWdgtDiagram->m_bZoomingYScaleEnabled = m_bZoomingYScaleEnabled;
        //pWdgtDiagram->m_bIsZooming;
        //pWdgtDiagram->m_ptZoomStart;
        //pWdgtDiagram->m_ptZoomEnd;
        //pWdgtDiagram->m_rectZoom;
        //pWdgtDiagram->m_iZoomCount;
        pWdgtDiagram->m_bContextPopupMenuEnabled = m_bContextPopupMenuEnabled;
        //pWdgtDiagram->m_pMenuContext;
        //pWdgtDiagram->m_pTipOfTheDay;
        //pWdgtDiagram->m_strCurrentFileDir;
        pWdgtDiagram->m_bMoveKeyAccelerationEnabled = m_bMoveKeyAccelerationEnabled;
        pWdgtDiagram->m_pMoveKeyAccelerationDiagScale = m_pMoveKeyAccelerationDiagScale;
        pWdgtDiagram->m_iMoveKeyAccelerationTimeout_ms = m_iMoveKeyAccelerationTimeout_ms;
        pWdgtDiagram->m_iMoveKeyAccelerationTimeoutSlowDown_ms = m_iMoveKeyAccelerationTimeoutSlowDown_ms;
        pWdgtDiagram->m_iMoveKeyAccelerationTimeoutSpeedUp_ms = m_iMoveKeyAccelerationTimeoutSpeedUp_ms;
        pWdgtDiagram->m_iMoveKeyAccelarationSpeedUpDelayRepCount = m_iMoveKeyAccelarationSpeedUpDelayRepCount;
        pWdgtDiagram->m_fMoveKeyAccelerationFactor = m_fMoveKeyAccelerationFactor;
        pWdgtDiagram->m_fMoveKeyAccelerationMaxStepScaleFactor = m_fMoveKeyAccelerationMaxStepScaleFactor;
        //pWdgtDiagram->m_fMoveKeyEventTime_ms;
        //pWdgtDiagram->m_fMoveKeyRepCount;
        //pWdgtDiagram->m_iMoveKeySpeedUpDelayRepCount;
        //pWdgtDiagram->m_iMoveKeyPrev;
        //pWdgtDiagram->m_pDiagObjFocused;
        //pWdgtDiagram->m_pDiagObjFocusedByKeyEvent;
        //pWdgtDiagram->m_pDiagObjFocusedByMouseEvent;
        //pWdgtDiagram->m_pDiagObjFocusedRecently;
        //pWdgtDiagram->m_pDiagObjEditing;
        //pWdgtDiagram->m_pDiagObjEditingByKeyEvent;
        //pWdgtDiagram->m_pDiagObjEditingByMouseEvent;

        pDiagram = pWdgtDiagram;
    }
    return pDiagram;

} // clone

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CWdgtDiagram::getDiagObjFocused()
//------------------------------------------------------------------------------
{
    return m_pDiagObjFocused;
}

//------------------------------------------------------------------------------
CDiagObj* CWdgtDiagram::getDiagObjEditing()
//------------------------------------------------------------------------------
{
    return m_pDiagObjEditing;
}

/*==============================================================================
public: // instance methods to set optional attributes of the diagram
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::enableZooming( EScaleDir i_scaleDir )
//------------------------------------------------------------------------------
{
    switch( i_scaleDir )
    {
        case EScaleDirX:
        {
            m_bZoomingXScaleEnabled = true;
            break;
        }
        case EScaleDirY:
        {
            m_bZoomingYScaleEnabled = true;
            break;
        }
        case EScaleDirCount:
        {
            m_bZoomingXScaleEnabled = true;
            m_bZoomingYScaleEnabled = true;
            break;
        }
        default:
        {
            break;
        }
    }

} // enableZooming

//------------------------------------------------------------------------------
void CWdgtDiagram::disableZooming( EScaleDir i_scaleDir )
//------------------------------------------------------------------------------
{
    switch( i_scaleDir )
    {
        case EScaleDirX:
        {
            m_bZoomingXScaleEnabled = false;
            break;
        }
        case EScaleDirY:
        {
            m_bZoomingYScaleEnabled = false;
            break;
        }
        case EScaleDirCount:
        {
            m_bZoomingXScaleEnabled = false;
            m_bZoomingYScaleEnabled = false;
            break;
        }
        default:
        {
            break;
        }
    }

} // disableZooming

//------------------------------------------------------------------------------
bool CWdgtDiagram::isZoomingEnabled( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    bool bZoomingEnabled = false;

    switch( i_scaleDir )
    {
        case EScaleDirX:
        {
            bZoomingEnabled = m_bZoomingXScaleEnabled;
            break;
        }
        case EScaleDirY:
        {
            bZoomingEnabled = m_bZoomingYScaleEnabled;
            break;
        }
        case EScaleDirCount:
        {
            bZoomingEnabled = (m_bZoomingXScaleEnabled || m_bZoomingYScaleEnabled);
            break;
        }
        default:
        {
            break;
        }
    }
    return bZoomingEnabled;

} // isZoomingEnabled

//------------------------------------------------------------------------------
void CWdgtDiagram::enableContextPopupMenu()
//------------------------------------------------------------------------------
{
    m_bContextPopupMenuEnabled = true;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::disableContextPopupMenu()
//------------------------------------------------------------------------------
{
    m_bContextPopupMenuEnabled = false;
}

//------------------------------------------------------------------------------
bool CWdgtDiagram::isContextPopupMenuEnabled() const
//------------------------------------------------------------------------------
{
    return m_bContextPopupMenuEnabled;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::showTipOfTheDay()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "showTipOfTheDay",
        /* strAddInfo   */ "" );

    if( m_pLblTipOfTheDay == nullptr )
    {
        m_pLblTipOfTheDay = new QLabel(this);
    }
    m_pLblTipOfTheDay->clear();
    m_pLblTipOfTheDay->setText(m_strTipOfTheDay);
    m_pLblTipOfTheDay->setAlignment(Qt::AlignTop);
    m_pLblTipOfTheDay->setMargin(4);
    m_pLblTipOfTheDay->show();
    invalidate(nullptr,EUpdateWidget);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::hideTipOfTheDay()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "hideTipOfTheDay",
        /* strAddInfo   */ "" );

    if( m_pLblTipOfTheDay != nullptr )
    {
        delete m_pLblTipOfTheDay;
        m_pLblTipOfTheDay = nullptr;
        invalidate(nullptr,EUpdateWidget);
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDay( const QString& i_str )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevel0,
        /* strMethod    */ "setTipOfTheDay",
        /* strAddInfo   */ i_str );

    m_strTipOfTheDay = i_str;

    if( m_pLblTipOfTheDay != nullptr )
    {
        m_pLblTipOfTheDay->clear();
        m_pLblTipOfTheDay->setText(m_strTipOfTheDay);
        invalidate(nullptr,EUpdateWidget);
    }
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDayIndentTop( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iTipOfTheDayIndentTop = i_iIndent;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDayIndentBottom( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iTipOfTheDayIndentBottom = i_iIndent;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDayIndentLeft( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iTipOfTheDayIndentLeft = i_iIndent;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setTipOfTheDayIndentRight( int i_iIndent )
//------------------------------------------------------------------------------
{
    m_iTipOfTheDayIndentRight = i_iIndent;
}

//------------------------------------------------------------------------------
void CWdgtDiagram::enableMoveKeyAcceleration(
    CDiagScale* i_pDiagScale,
    int         i_iTimeout_ms,
    int         i_iTimeoutSlowDown_ms,
    int         i_iTimeoutSpeedUp_ms,
    int         i_iSpeedUpDelayRepCount,
    double      i_fFactor,
    double      i_fMaxStepScaleFactor )
//------------------------------------------------------------------------------
{
    m_bMoveKeyAccelerationEnabled = true;
    m_pMoveKeyAccelerationDiagScale = i_pDiagScale;
    m_iMoveKeyAccelerationTimeout_ms = i_iTimeout_ms;
    m_iMoveKeyAccelerationTimeoutSlowDown_ms = i_iTimeoutSlowDown_ms;
    m_iMoveKeyAccelerationTimeoutSpeedUp_ms = i_iTimeoutSpeedUp_ms;
    m_iMoveKeyAccelarationSpeedUpDelayRepCount = i_iSpeedUpDelayRepCount;
    m_fMoveKeyAccelerationFactor = i_fFactor;
    m_fMoveKeyAccelerationMaxStepScaleFactor = i_fMaxStepScaleFactor;

} // enableMoveKeyAcceleration

//------------------------------------------------------------------------------
void CWdgtDiagram::disableMoveKeyAcceleration()
//------------------------------------------------------------------------------
{
    m_bMoveKeyAccelerationEnabled = false;

} // disableMoveKeyAcceleration

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::update( CDiagObj* i_pDiagObj, const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        if( i_pDiagObj != nullptr )
        {
            strTrcMsg = i_pDiagObj->getObjName() + ", ";
        }
        strTrcMsg += "UpdRect[x,y,w,h]=";
        strTrcMsg += QString::number(i_rect.x()) + ",";
        strTrcMsg += QString::number(i_rect.y()) + ",";
        strTrcMsg += QString::number(i_rect.width()) + ",";
        strTrcMsg += QString::number(i_rect.height());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    // This method is called by the diagram objects after they have
    // recalculated their outputs within their update methods.
    if( i_rect.isValid() )
    {
        QWidget::update(i_rect);
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

} // update

//------------------------------------------------------------------------------
void CWdgtDiagram::update()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "update",
        /* strAddInfo   */ "" );

    // This method is called by the underlying window system but may also be called
    // by objects controlling the diagram to update the content of the diagram.

    // If the bitmap is not yet ready ...
    //if( m_uUpdateFlags != EUpdateNone )
    //{
    //    // Update the pixmap (but not yet the widget)
    //    CDataDiagram::update( m_uUpdateFlags & ~EUpdateWidget );
    //}

    // Trigger update (repainting) content of diagram
    QWidget::update();

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

} // update

//------------------------------------------------------------------------------
void CWdgtDiagram::update( const QRect& i_rect )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelLayout) )
    {
        strTrcMsg  = "UpdRect[x,y,w,h]=";
        strTrcMsg += QString::number(i_rect.x()) + ",";
        strTrcMsg += QString::number(i_rect.y()) + ",";
        strTrcMsg += QString::number(i_rect.width()) + ",";
        strTrcMsg += QString::number(i_rect.height());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    // This method is called by the underlying window system to force an update
    // of an invalidated region but also by the diagram objects after they have
    // recalculated their outputs within their update methods.
    if( i_rect.isValid() )
    {
        QWidget::update(i_rect);
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

} // update

//------------------------------------------------------------------------------
void CWdgtDiagram::setMouseTracking( bool i_bEnable )
//------------------------------------------------------------------------------
{
    QWidget::setMouseTracking(i_bEnable);
}

//------------------------------------------------------------------------------
void CWdgtDiagram::setEraseColor( const QColor& i_col )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelLayout,
        /* strMethod    */ "setEraseColor",
        /* strAddInfo   */ "" );

    m_colErase = i_col;

    #if QT_VERSION >= 0x040100
    #else
    QWidget::setEraseColor(i_col);
    #endif

    invalidate(nullptr,EUpdatePixmapWidget);
}

/*==============================================================================
public: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::setFocus()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "setFocus",
        /* strAddInfo   */ "" );

    QWidget::setFocus();
}

//------------------------------------------------------------------------------
void CWdgtDiagram::clearFocus()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "clearFocus",
        /* strAddInfo   */ "" );

    QWidget::clearFocus();

    // Unselect currently selected object (if any object selected) but keep the
    // focus on the selected object.
    if( m_pDiagObjEditing != nullptr )
    {
        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set).
        m_pDiagObjEditing->stopEditSession(true); // also inform other objects of necessary pixmap processing

        // No object is longer being edited:
        m_pDiagObjEditing = nullptr;
        m_pDiagObjEditingByKeyEvent = nullptr;
        m_pDiagObjEditingByMouseEvent = nullptr;
    }

    // Remove focus from currently focused object (if any object focused).
    if( m_pDiagObjFocused != nullptr )
    {
        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set).
        m_pDiagObjFocused->unsetFocus(true); // also inform other objects of necessary pixmap processing

        // No object is longer being focused:
        m_pDiagObjFocused = nullptr;
        m_pDiagObjFocusedByKeyEvent = nullptr;
        m_pDiagObjFocusedByMouseEvent = nullptr;
    }

    // Update graphical output of the diagram.
    // For the unfocused or no longer edited object at least data processing will be executed
    // and the object invalidates the "dirty" rectangle area of the widget:

    m_uUpdateFlags |= EUpdateAll;

    // Trigger paint event
    update();

} // clearFocus

//------------------------------------------------------------------------------
void CWdgtDiagram::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelEvents) )
    {
        strTrcMsg = qtKey2Str(i_pEv->key());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strTrcMsg );

    CDiagObj* pDiagObj;

    // As default we ignore the event ..
    i_pEv->ignore();

    switch( i_pEv->key() )
    {
        case Qt::Key_Escape:
        {
            // Unselect currently selected object (if any object selected) but keep the
            // focus on the selected object.
            if( m_pDiagObjEditing != nullptr )
            {
                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjEditing->stopEditSession(true); // also inform other objects of necessary pixmap processing

                // No object is longer being edited:
                m_pDiagObjEditing = nullptr;
                m_pDiagObjEditingByKeyEvent = nullptr;
                m_pDiagObjEditingByMouseEvent = nullptr;

                // Update graphical output of the diagram.
                // For the no longer edited object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();

                // The diagram just "eats" the escape key if an object was focused or selected.
                i_pEv->accept();
            }
            // Remove focus from currently focused object (if any object focused).
            else if( m_pDiagObjFocused != nullptr )
            {
                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjFocused->unsetFocus();

                // No object is longer being focused:
                m_pDiagObjFocused = nullptr;
                m_pDiagObjFocusedByKeyEvent = nullptr;
                m_pDiagObjFocusedByMouseEvent = nullptr;

                // Update graphical output of the diagram.
                // For the unfocused object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();

                // The diagram just "eats" the escape key if an object was focused or selected.
                i_pEv->accept();
            }
            break;
        }
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            // The diagram "eats" each enter key.
            i_pEv->accept();

            // If any object is already being edited ..
            if( m_pDiagObjEditing != nullptr )
            {
                // Deselect the object (but keep the focus on it).
                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjEditing->stopEditSession(true); // also inform other objects of necessary pixmap processing

                // No object is longer being edited:
                m_pDiagObjEditing = nullptr;
                m_pDiagObjEditingByKeyEvent = nullptr;
                m_pDiagObjEditingByMouseEvent = nullptr;

                // Update graphical output of the diagram.
                // For the no longer edited object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();
            }
            // If any object is already focused ..
            else if( m_pDiagObjFocused != nullptr )
            {
                // Select the object (and keep the focus on it).
                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjFocused->startEditSession(true); // also inform other objects of necessary pixmap processing

                // Store the edited object:
                m_pDiagObjEditing = m_pDiagObjFocused;
                m_pDiagObjEditingByKeyEvent = m_pDiagObjFocused;
                m_pDiagObjEditingByMouseEvent = nullptr;

                // Update graphical output of the diagram.
                // For the newly edited object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();
            }
            // If neither an object is focused nor selected ...
            else
            {
                // If an object has been recently focused ...
                if( m_pDiagObjFocusedRecently != nullptr )
                {
                    // ... again set the focus on this object.
                    m_pDiagObjFocused = m_pDiagObjFocusedRecently;
                    m_pDiagObjFocusedByKeyEvent = m_pDiagObjFocused;
                    m_pDiagObjEditingByMouseEvent = nullptr;
                }
                // If no object was recently focused ...
                else
                {
                    // Check if any object is focusable. If - set the focus on the first found object.
                    pDiagObj = m_pDiagObjFirst;
                    while( pDiagObj != nullptr )
                    {
                        if( pDiagObj->isFocusable() && pDiagObj->isEditable() )
                        {
                            m_pDiagObjFocused = pDiagObj;
                            break;
                        }
                        pDiagObj = pDiagObj->m_pDiagObjNext;
                    }
                }

                // If an object has been focused ..
                if( m_pDiagObjFocused != nullptr )
                {
                    // Store the focused object:
                    m_pDiagObjFocusedByKeyEvent = m_pDiagObjFocused;
                    m_pDiagObjFocusedByMouseEvent = nullptr;
                    m_pDiagObjFocusedRecently = m_pDiagObjFocused;

                    // The focused object need to be updated at last so that no other
                    // object overwrites the graphic output of the focused object:
                    moveDiagObjInPaintList(m_pDiagObjFocused,EDiagObjMoveModeToTop);

                    // If the graphical output of the object changes the corresponding
                    // update flags will be invalidated (the bits will be set).
                    m_pDiagObjFocused->setFocus(true); // also inform other objects of necessary pixmap processing

                    // Update graphical output of the diagram.
                    // For the newly focues object at least data processing will be executed
                    // and the object invalidates the "dirty" rectangle area of the widget:
                    m_uUpdateFlags |= EUpdateAll;

                    // Trigger paint event
                    update();
                }
            }
            break;
        }
        case Qt::Key_Print:
        {
            i_pEv->accept();
            break;
        }
        case Qt::Key_Left:
        case Qt::Key_Down:
        case Qt::Key_Right:
        case Qt::Key_Up:
        {
            double fMoveKeyEventTime_ms = 0.0;
            double fMoveKeyEventTimeDelay_ms = 0.0;
            int    iMoveKeyCurr = i_pEv->key();

            // TODO: Hr. Koehler will, dass mit Up/Down der "Mouse-Cursor" nach
            //       oben und unten verschoben werden kann. Vorläufig wandeln wir
            //       aber Up in Right und Down in Left um.

            if( i_pEv->key() == Qt::Key_Down )
            {
                iMoveKeyCurr = Qt::Key_Left;
            }
            else if( i_pEv->key() == Qt::Key_Up )
            {
                iMoveKeyCurr = Qt::Key_Right;
            }
            if( m_iMoveKeyPrev == -1 )
            {
                m_iMoveKeyPrev = iMoveKeyCurr;
            }
            if( m_bMoveKeyAccelerationEnabled )
            {
                fMoveKeyEventTime_ms = Time::getProcTimeInMilliSec();
                fMoveKeyEventTimeDelay_ms = fMoveKeyEventTime_ms - m_fMoveKeyEventTime_ms;
            }

            // If no object focused or selected ...
            if( m_pDiagObjFocused == nullptr && m_pDiagObjEditing == nullptr )
            {
                // .. the move key event will be ignored by the diagram.
                i_pEv->ignore();
            }

            // If any object focused or selected ...
            else // if( m_pDiagObjFocused != nullptr || m_pDiagObjEditing != nullptr )
            {
                bool    bTrcMoveKeyActive = mthTracer.isActive(c_iTrcDetailLevelEvents);
                QString strTrcMsgMoveKey;

                // .. the move key event will be handled by the diagram.
                i_pEv->accept();

                if( m_pDiagObjEditing != nullptr )
                {
                    if( m_bMoveKeyAccelerationEnabled )
                    {
                        if( bTrcMoveKeyActive )
                        {
                            strTrcMsgMoveKey += "CurrTime=" + QString::number(fMoveKeyEventTime_ms,'f',0) + "[ms], ";
                            strTrcMsgMoveKey += "DelayTime=" + QString::number(fMoveKeyEventTimeDelay_ms,'f',0) + "[ms], ";
                        }
                        if( m_fMoveKeyRepCount < 1.0 )
                        {
                            m_fMoveKeyRepCount = 1.0;
                        }
                        if( fMoveKeyEventTimeDelay_ms >= m_iMoveKeyAccelerationTimeout_ms )
                        {
                            // Reset ..
                            m_iMoveKeySpeedUpDelayRepCount = 0;
                            m_fMoveKeyRepCount = 1.0;
                            if( bTrcMoveKeyActive )
                            {
                                strTrcMsgMoveKey += "Delay > TimeOut: Reset speed to " + QString::number(m_fMoveKeyRepCount,'f',0);
                            }
                        }
                        else if( fMoveKeyEventTimeDelay_ms >= m_iMoveKeyAccelerationTimeoutSlowDown_ms )
                        {
                            // Slow down ..
                            m_iMoveKeySpeedUpDelayRepCount = 0;
                            m_fMoveKeyRepCount /= m_fMoveKeyAccelerationFactor;
                            if( m_fMoveKeyRepCount < 1.0 )
                            {
                                m_fMoveKeyRepCount = 1.0;
                            }
                            if( bTrcMoveKeyActive )
                            {
                                strTrcMsgMoveKey += "Delay > TimeoutSlowDown: Slow down speed to " + QString::number(m_fMoveKeyRepCount,'f',0);
                            }
                        }
                        else if( fMoveKeyEventTimeDelay_ms >= m_iMoveKeyAccelerationTimeoutSpeedUp_ms )
                        {
                            m_iMoveKeySpeedUpDelayRepCount = 0;

                            // If the direction of the move events changed ...
                            if( m_iMoveKeyPrev != iMoveKeyCurr )
                            {
                                // Slow down ..
                                m_fMoveKeyRepCount /= m_fMoveKeyAccelerationFactor;
                                if( m_fMoveKeyRepCount < 1.0 )
                                {
                                    m_fMoveKeyRepCount = 1.0;
                                }
                                if( bTrcMoveKeyActive )
                                {
                                    strTrcMsgMoveKey += "Delay > TimeoutSpeedUp AND DirectionChanged: Slow down speed to " + QString::number(m_fMoveKeyRepCount,'f',0);
                                }
                            }
                            // If the direction of the move events did not change ...
                            else
                            {
                                // Keep the speed ..
                                if( bTrcMoveKeyActive )
                                {
                                    strTrcMsgMoveKey += "Delay > TimeoutSpeedUp: Keep speed by " + QString::number(m_fMoveKeyRepCount,'f',0);
                                }
                            }
                        }
                        else if( fMoveKeyEventTimeDelay_ms < m_iMoveKeyAccelerationTimeoutSpeedUp_ms )
                        {
                            // If the direction of the move events changed ...
                            if( m_iMoveKeyPrev != iMoveKeyCurr )
                            {
                                m_iMoveKeySpeedUpDelayRepCount = 0;

                                // Slow down ..
                                m_fMoveKeyRepCount /= m_fMoveKeyAccelerationFactor;
                                if( m_fMoveKeyRepCount < 1.0 )
                                {
                                    m_fMoveKeyRepCount = 1.0;
                                }
                                if( bTrcMoveKeyActive )
                                {
                                    strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DirectionChanged: Slow down speed to " + QString::number(m_fMoveKeyRepCount,'f',0);
                                }
                            }
                            // If the direction of the move events did not change ...
                            else
                            {
                                if( m_iMoveKeySpeedUpDelayRepCount < 0 )
                                {
                                    m_iMoveKeySpeedUpDelayRepCount = 0;
                                }
                                m_iMoveKeySpeedUpDelayRepCount++;

                                // If the number of move events exceeded the configured number of events ...
                                if( m_iMoveKeySpeedUpDelayRepCount >= m_iMoveKeyAccelarationSpeedUpDelayRepCount )
                                {
                                    // Speed up ..
                                    m_iMoveKeySpeedUpDelayRepCount = 0;

                                    if( m_pMoveKeyAccelerationDiagScale != nullptr && m_pMoveKeyAccelerationDiagScale->isScaleValid() )
                                    {
                                        EScaleDir scaleDir;
                                        double    fScaleMin;
                                        double    fScaleMax;
                                        double    fScaleRange;
                                        bool      bValIsValid = false;
                                        double    fVal = 0.0;
                                        double    fRes = 0.0;

                                        scaleDir = m_pMoveKeyAccelerationDiagScale->getScaleDir();
                                        bValIsValid = m_pDiagObjEditing->getVal(scaleDir).isValid();

                                        if( bValIsValid )
                                        {
                                            fVal = m_pDiagObjEditing->getVal(scaleDir).getVal();
                                            fRes = m_pDiagObjEditing->getValRes(scaleDir,fVal);
                                        }
                                        else
                                        {
                                            fRes = m_pDiagObjEditing->getValRes(scaleDir);
                                        }
                                        fScaleMin = m_pMoveKeyAccelerationDiagScale->getScale().m_fMin;
                                        fScaleMax = m_pMoveKeyAccelerationDiagScale->getScale().m_fMax;
                                        fScaleRange = fabs(fScaleMax-fScaleMin);

                                        if( bValIsValid && m_pMoveKeyAccelerationDiagScale->getSpacing() == ESpacingLogarithmic )
                                        {
                                            fScaleMin = log10(fScaleMin);
                                            fScaleMax = log10(fScaleMax);
                                            fScaleRange = fabs(fScaleMax-fScaleMin);
                                            fRes = log10(fVal+fRes)-log10(fVal);
                                        }

                                        m_fMoveKeyRepCount *= m_fMoveKeyAccelerationFactor;
                                        if( fRes*m_fMoveKeyRepCount > m_fMoveKeyAccelerationMaxStepScaleFactor*fScaleRange )
                                        {
                                            m_fMoveKeyRepCount = (m_fMoveKeyAccelerationMaxStepScaleFactor*fScaleRange)/fRes;
                                            if( bTrcMoveKeyActive )
                                            {
                                                strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DelayRepCount >= SpeedUpRepCount ";
                                                strTrcMsgMoveKey += "(ScaleRange=" + QString::number(fScaleRange) + ", Res=" + QString::number(fRes) + "): Speed limit reached at " + QString::number(m_fMoveKeyRepCount,'f',0);
                                            }
                                        }
                                        else
                                        {
                                            if( bTrcMoveKeyActive )
                                            {
                                                strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DelayRepCount >= SpeedUpRepCount ";
                                                strTrcMsgMoveKey += "(ScaleRange=" + QString::number(fScaleRange) + ", Res=" + QString::number(fRes) + "): Speed up to " + QString::number(m_fMoveKeyRepCount,'f',0);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        m_fMoveKeyRepCount *= m_fMoveKeyAccelerationFactor;
                                        if( bTrcMoveKeyActive )
                                        {
                                            strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DelayRepCount >= SpeedUpRepCount: Speed up to " + QString::number(m_fMoveKeyRepCount,'f',0);
                                        }
                                    }
                                }
                                // If the number of move events did not exceed the configured number of events ...
                                else
                                {
                                    // Keep the speed ..
                                    if( bTrcMoveKeyActive )
                                    {
                                        strTrcMsgMoveKey += "Delay < TimeoutSpeedUp AND DelayRepCount < SpeedUpRepCount: Keep speed by " + QString::number(m_fMoveKeyRepCount,'f',0);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        m_fMoveKeyRepCount = 1.0;
                    }

                    if( bTrcMoveKeyActive )
                    {
                        mthTracer.trace(strTrcMsgMoveKey,c_iTrcDetailLevelEvents);
                    }

                    processMoveKeyEvent(iMoveKeyCurr);

                    // Key events seem to have a higher priority than paint events. As we want
                    // to trace the current position of the edited object the paintEvent will
                    // be explicitly triggered.
                    #if QT_VERSION >= 0x040100
                    QPaintEvent* pPaintEv = new QPaintEvent( QRect(QPoint(0,0),m_size) );
                    #else
                    QPaintEvent* pPaintEv = new QPaintEvent( QRect(QPoint(0,0),m_size), false );
                    #endif
                    QApplication::postEvent(this,pPaintEv);
                    pPaintEv = nullptr; //lint !e423

                } // if( m_pDiagObjEdited != nullptr )

                else // if( m_pDiagObjFocused != nullptr )
                {
                    if( bTrcMoveKeyActive )
                    {
                        mthTracer.trace(strTrcMsgMoveKey,c_iTrcDetailLevelEvents);
                    }

                    // Selecting other objects don't need to be accelerated.
                    m_fMoveKeyRepCount = 1.0;
                    processMoveKeyEvent(iMoveKeyCurr);
                    m_fMoveKeyRepCount = 0.0;
                }
            } // if( m_pDiagObjFocused != nullptr || m_pDiagObjEditing != nullptr )

            if( m_bMoveKeyAccelerationEnabled )
            {
                m_fMoveKeyEventTime_ms = fMoveKeyEventTime_ms;
            }
            m_iMoveKeyPrev = iMoveKeyCurr;
            break;
        }

        default:
        {
            break;
        }

    } // switch( i_pEv->key() )

} // keyPressEvent

//lint -e1961
//------------------------------------------------------------------------------
void CWdgtDiagram::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelEvents) )
    {
        strTrcMsg = qtKey2Str(i_pEv->key());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strTrcMsg );

    i_pEv->ignore();

} // keyReleaseEvent
//lint +e1961

//------------------------------------------------------------------------------
void CWdgtDiagram::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ "" );

    CDiagObj* pDiagObj;

    // Maybe also parents are interested in the mouse event ...
    i_pEv->ignore();

    delete m_pMenuContext;
    m_pMenuContext = nullptr;

    if( i_pEv->button() == Qt::LeftButton )
    {
        // Remove focus from currently focused object (if any object focused).
        if( m_pDiagObjFocused != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjFocused->unsetFocus();

            // No object is longer being focused:
            m_pDiagObjFocused = nullptr;
            m_pDiagObjFocusedByKeyEvent = nullptr;
            m_pDiagObjFocusedByMouseEvent = nullptr;
        }

        // Unselect currently selected object (if any object selected).
        if( m_pDiagObjEditing != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjEditing->stopEditSession();

            // No object is longer being focused:
            m_pDiagObjEditing = nullptr;
            m_pDiagObjEditingByKeyEvent = nullptr;
            m_pDiagObjEditingByMouseEvent = nullptr;
        }

        // Check if any object has been hit. If - start editing the object.
        pDiagObj = m_pDiagObjPaintLast;
        while( pDiagObj != nullptr )
        {
            if( pDiagObj->isFocusable() && pDiagObj->isEditable() && pDiagObj->isHit(i_pEv->pos()) )
            {
                // Store the focused object:
                m_pDiagObjFocused = pDiagObj;
                m_pDiagObjFocusedByMouseEvent = pDiagObj;
                m_pDiagObjFocusedRecently = pDiagObj;

                // Store the edited object:
                m_pDiagObjEditing = pDiagObj;
                m_pDiagObjEditingByMouseEvent = pDiagObj;

                // The focused object need to be updated at last so that no other
                // object overwrites the graphic output of the focused object:
                moveDiagObjInPaintList(m_pDiagObjFocused,EDiagObjMoveModeToTop);

                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjFocused->startEditSession(i_pEv->pos()); // implicitly sets the focus on this object
                break;
            }
            pDiagObj = pDiagObj->m_pDiagObjPaintPrev;
        }

        // If none of the diagram objects has been hit and if zooming is enabled ...
        if( m_pDiagObjFocused == nullptr && m_pDiagObjEditing == nullptr && isZoomingEnabled() )
        {
            if( m_rectPartCenter.contains(i_pEv->pos()) )
            {
                m_bIsZooming = true;
                m_ptZoomStart = i_pEv->pos();
                m_ptZoomEnd = i_pEv->pos();
                m_rectZoom = calcRect(m_ptZoomStart,m_ptZoomEnd);
            }
        }

        // Update graphical output of the diagram.
        // For the newly focused or edited object at least data processing will be executed
        // and the object invalidates the "dirty" rectangle area of the widget:
        m_uUpdateFlags |= EUpdateAll;

        // Trigger paint event
        update();
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ "" );

    // Maybe also parents are interested in the mouse event ...
    i_pEv->ignore();

    delete m_pMenuContext;
    m_pMenuContext = nullptr;

    if( i_pEv->button() == Qt::LeftButton )
    {
        // Unselect currently selected object.
        if( m_pDiagObjEditing != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjEditing->stopEditSession();

            // No object is longer being focused:
            m_pDiagObjEditing = nullptr;
            m_pDiagObjEditingByKeyEvent = nullptr;
            m_pDiagObjEditingByMouseEvent = nullptr;

            // Update graphical output of the diagram.
            // For the no longer edited object at least data processing will be executed
            // and the object invalidates the "dirty" rectangle area of the widget:
            m_uUpdateFlags |= EUpdateAll;

            // Trigger paint event
            update();
        }

        if( m_bIsZooming )
        {
            CDiagScale* pDiagScale;
            QPoint      ptZoomEnd = i_pEv->pos();

            if( ptZoomEnd.x() < m_rectPartCenter.left() )
            {
                ptZoomEnd.setX(m_rectPartCenter.left());
            }
            if( ptZoomEnd.x() > m_rectPartCenter.right() )
            {
                ptZoomEnd.setX(m_rectPartCenter.right());
            }
            if( ptZoomEnd.y() < m_rectPartCenter.top() )
            {
                ptZoomEnd.setY(m_rectPartCenter.top());
            }
            if( ptZoomEnd.y() > m_rectPartCenter.bottom() )
            {
                ptZoomEnd.setY(m_rectPartCenter.bottom());
            }
            m_ptZoomEnd = ptZoomEnd;

            m_rectZoom = calcRect(m_ptZoomStart,m_ptZoomEnd);

            if( !isZoomingEnabled(EScaleDirX) )
            {
                m_rectZoom.setLeft(m_rectPartCenter.left());
                m_rectZoom.setWidth(m_rectPartCenter.width());
            }
            else if( !isZoomingEnabled(EScaleDirY) )
            {
                m_rectZoom.setTop(m_rectPartCenter.top());
                m_rectZoom.setHeight(m_rectPartCenter.height());
            }

            if( m_rectZoom.width() >= 2 && m_rectZoom.height() >= 2 )
            {
                pDiagScale = m_pDiagScaleFirst;
                while( pDiagScale != nullptr )
                {
                    switch( pDiagScale->getScaleDir() )
                    {
                        case EScaleDirX:
                        {
                            pDiagScale->zoomIn(m_rectZoom.left(),m_rectZoom.right());
                            break;
                        }
                        case EScaleDirY:
                        {
                            pDiagScale->zoomIn(m_rectZoom.bottom(),m_rectZoom.top());
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    pDiagScale = pDiagScale->m_pDiagScaleNext;
                }
                m_iZoomCount++;
            }
            else
            {
                invalidate(nullptr,EUpdatePixmap);
            }
            m_bIsZooming = false;

            // Update graphical output of the diagram (here paint event by diagram itself).
            update();
        }
    }
    else if( i_pEv->button() == Qt::RightButton )
    {
        if( m_bContextPopupMenuEnabled && m_pPixmap != nullptr )
        {
            #if QT_VERSION >= 0x040100
            //m_pMenuContext = new QMenu(this,"PopupMenuContextDiagram");
            //m_pMenuContext->addAction( "Save", dynamic_cast<QWidget*>(this), SLOT(popupMenuContextItemSaveActivated()) );
            //m_pMenuContext->addAction( "Print", dynamic_cast<QWidget*>(this), SLOT(popupMenuContextItemPrintActivated()) );
            #else
            m_pMenuContext = new QPopupMenu(this,"PopupMenuContextDiagram");
            m_pMenuContext->insertItem( "Save", dynamic_cast<QWidget*>(this), SLOT(popupMenuContextItemSaveActivated()) );
            m_pMenuContext->insertItem( "Print", dynamic_cast<QWidget*>(this), SLOT(popupMenuContextItemPrintActivated()) );
            m_pMenuContext->popup(mapToGlobal(i_pEv->pos()));
            #endif
        }
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ "" );

    // Maybe also parents are interested in the mouse event ...
    i_pEv->ignore();

    if( m_iZoomCount > 0 )
    {
        CDiagScale* pDiagScale;
        SScale      scale;

        pDiagScale = m_pDiagScaleFirst;
        while( pDiagScale != nullptr )
        {
            pDiagScale->zoomOut();
            pDiagScale = pDiagScale->m_pDiagScaleNext;
        }
        m_iZoomCount--;

        // Update graphical output of the diagram.
        // For the changed objects at least data processing will be executed and the
        // objects are invalidating the "dirty" rectangle area of the widget:
        m_uUpdateFlags |= EUpdateAll;

        // Trigger paint event
        update();
    }

} // mouseDoubleClickEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::mouseMoveEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    // Maybe also parents are interested in the mouse event ...
    i_pEv->ignore();

    // If an object has been selected by mouse click ...
    if( m_pDiagObjEditingByMouseEvent != nullptr )
    {
        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelEvents) )
        {
            strTrcMsg  = "DiagObjEdited: ";
            strTrcMsg += m_pDiagObjEditingByMouseEvent->getObjName();
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ c_iTrcDetailLevelEvents,
            /* strMethod    */ "mouseMoveEvent",
            /* strAddInfo   */ strTrcMsg );

        // ... move this object to the new mouse position.
        CDiagObjMoveEvent* pEv = new CDiagObjMoveEvent( CDiagObjMoveEvent::ETypePointAbs, i_pEv->pos() );

        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set), data
        // processing will be executed by the object and the object invalidates
        // the "dirty" rectangle area of the widget:
        m_pDiagObjEditingByMouseEvent->moveEvent(pEv);
        delete pEv;
        pEv = nullptr;

        // Trigger paintEvent to redraw the diagram.
        update();
    }

    // If the diagram will be zoomed ..
    else if( m_bIsZooming )
    {
        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelEvents) )
        {
            strTrcMsg = "Zooming";
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ c_iTrcDetailLevelEvents,
            /* strMethod    */ "mouseMoveEvent",
            /* strAddInfo   */ strTrcMsg );

        QRect rectZoomPrev = m_rectZoom;

        // Calculate new output region of zooming rectangle.
        m_ptZoomEnd = i_pEv->pos();
        m_rectZoom = calcRect(m_ptZoomStart,m_ptZoomEnd);

        // The objects and the diagram need to redraw the pixmap. Widget processing
        // will be realized by invalidating the zooming rectangle border lines below.
        invalidate(nullptr,EUpdatePixmap);

        // Invalidate old and new output region of zooming rectangle
        // (corresponds to update process depth "Widget").

        // Old output region:
        update( QRect(rectZoomPrev.left(),rectZoomPrev.top(),rectZoomPrev.width(),1) );
        update( QRect(rectZoomPrev.left(),rectZoomPrev.bottom(),rectZoomPrev.width(),1) );
        update( QRect(rectZoomPrev.left(),rectZoomPrev.top(),1,rectZoomPrev.height()) );
        update( QRect(rectZoomPrev.right(),rectZoomPrev.top(),1,rectZoomPrev.height()) );

        // New output region:
        update( QRect(m_rectZoom.left(),m_rectZoom.top(),m_rectZoom.width(),1) );
        update( QRect(m_rectZoom.left(),m_rectZoom.bottom(),m_rectZoom.width(),1) );
        update( QRect(m_rectZoom.left(),m_rectZoom.top(),1,m_rectZoom.height()) );
        update( QRect(m_rectZoom.right(),m_rectZoom.top(),1,m_rectZoom.height()) );

        // Trigger paintEvent to redraw the diagram.
        update();
    }

    else if( m_pDiagObjFocusedByKeyEvent == nullptr )
    {
        // Check if any object has been hit. If - set focus on this object.
        if( m_pDiagObjFocusedByMouseEvent != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjFocusedByMouseEvent->unsetFocus();
            m_pDiagObjFocusedByMouseEvent = nullptr;
        }

        CDiagObj* pDiagObj = m_pDiagObjFirst;
        while( pDiagObj != nullptr )
        {
            if( pDiagObj->isFocusable() && pDiagObj->isEditable() && pDiagObj->isHit(i_pEv->pos()) )
            {
                m_pDiagObjFocusedByMouseEvent = pDiagObj;
                break;
            }
            pDiagObj = pDiagObj->m_pDiagObjNext;
        }
        if( m_pDiagObjFocusedByMouseEvent != nullptr )
        {
            // If the graphical output of the object changes the corresponding
            // update flags will be invalidated (the bits will be set).
            m_pDiagObjFocusedByMouseEvent->setFocus(i_pEv->pos());
            m_pDiagObjFocused = m_pDiagObjFocusedByMouseEvent;
        }
        else
        {
            m_pDiagObjFocused = nullptr;
        }

        // Trigger paintEvent to redraw the diagram.
        update();
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::paintEvent( QPaintEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "paintEvent",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(c_iTrcDetailLevelEvents) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelEvents);
    }

    // If the paint event occurred while the diagram is not up to date ...
    if( m_uUpdateFlags & EUpdateLayout )
    {
        updateLayout();
    }
    if( m_uUpdateFlags & EUpdateData )
    {
        updateData();
    }
    if( m_uUpdateFlags & EUpdatePixmap )
    {
        updatePixmap();
    }

    // We are going to clear the update widget flags of the objects as the paint
    // event might have been triggered on the resize event and for better
    // performance the resize event has just executed the layout processing.
    CDiagObj* pDiagObj = m_pDiagObjFirst;
    while( pDiagObj != nullptr )
    {
        pDiagObj->validate(EUpdateWidget);
        pDiagObj = pDiagObj->m_pDiagObjNext;
    }

    QPainter painter(this);

    // Widget processing:
    if( m_pPixmap != nullptr )
    {
        #if QT_VERSION >= 0x040100
        //QSize sizePixmap = m_pPixmap->size();
        painter.drawPixmap(
            /* rectTarget */ i_pEv->rect(),
            /* pixmap     */ *m_pPixmap,
            /* rectSource */ i_pEv->rect() );
        #else
        bitBlt(
            /* pPaintDeviceDst  */ this,
            /* xLeftDst         */ i_pEv->rect().left(),
            /* yTopDst          */ i_pEv->rect().top(),
            /* pPaintDeviceSrc  */ m_pPixmap,
            /* xLeftSrc         */ i_pEv->rect().left(),
            /* yTopSrc          */ i_pEv->rect().top(),
            /* cxWidthSrc       */ i_pEv->rect().width(),
            /* cyHeightSrc      */ i_pEv->rect().height() );
        #endif
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

    if( mthTracer.isActive(c_iTrcDetailLevelEvents) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelEvents);
    }

    emit painted(this);

} // paintEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::resizeEvent( QResizeEvent* /*i_pEv*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "resizeEvent",
        /* strAddInfo   */ "" );

    QSize sizePrev     = m_size;
    bool  bSizeChanged = false;

    if( m_size.width() != static_cast<QWidget*>(this)->width()
     || m_size.height() != static_cast<QWidget*>(this)->height() )
    {
        if( static_cast<QWidget*>(this)->width() >= minimumWidth()
         && static_cast<QWidget*>(this)->height() >= minimumHeight() )
        {
            bSizeChanged = true;
            m_size = static_cast<QWidget*>(this)->size();
        }
    }

    if( mthTracer.isActive(c_iTrcDetailLevelLayout) )
    {
        QString strTrcMsg;
        QRect   rectGeometry = geometry();
        strTrcMsg  = "SizeChanged=";
        strTrcMsg += QString::number(bSizeChanged) + ", ";
        strTrcMsg += "Geometry[x,y,w,h]=";
        strTrcMsg += QString::number(rectGeometry.x()) + ",";
        strTrcMsg += QString::number(rectGeometry.y()) + ",";
        strTrcMsg += QString::number(rectGeometry.width()) + ",";
        strTrcMsg += QString::number(rectGeometry.height());
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelLayout);
    }
    if( bSizeChanged )
    {
        int cxWidthDist  = m_size.width() - sizePrev.width();
        int cyHeightDist = m_size.height() - sizePrev.height();

        // Before updating the layout the scale objects will be provided with their
        // (probable) new extents. Those extent may change later on if the real
        // size of the diagram areas will be calculated as some objects may change
        // their size according to the number and position of the scale division lines.
        // But setting the (probable) scale geometry in advance may save time ...
        m_rectPartCenter.setWidth( m_rectPartCenter.width() + cxWidthDist );
        m_rectPartCenter.setHeight( m_rectPartCenter.height() + cyHeightDist );

        // It is NOT sufficient to mark layout processing as necessary.
        // Even if on executing the layout process the geometry of the objects
        // might be changed (if necessary) and the necessary update flags would
        // be set anyway. To recalculate the geometry of the objects some of those
        // objects need to recalculate their internal data in advance to provide
        // the correct sizeHint for the diagram. E.g. on resizing the diagram the
        // number of division lines might change and therefore an axis label object
        // need to change the number of digits to scale the diagram.
        invalidate(nullptr,EUpdateLayoutDataPixmapWidget);

        // There is no need to trigger the paint event here as the paint
        // event will follow anyway upon a resize event.
        //update();
    }

} // resizeEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::focusInEvent( QFocusEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "focusInEvent",
        /* strAddInfo   */ "" );

    QWidget::focusInEvent(i_pEv);

    emit gotFocus(this);

} // focusInEvent

//------------------------------------------------------------------------------
void CWdgtDiagram::focusOutEvent( QFocusEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "focusOutEvent",
        /* strAddInfo   */ "" );

    QWidget::focusOutEvent(i_pEv);

    // Unselect currently selected object (if any object selected) but keep the
    // focus on the selected object.
    if( m_pDiagObjEditing != nullptr )
    {
        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set).
        m_pDiagObjEditing->stopEditSession();

        // No object is longer being edited:
        m_pDiagObjEditing = nullptr;
        m_pDiagObjEditingByKeyEvent = nullptr;
        m_pDiagObjEditingByMouseEvent = nullptr;

        // Update graphical output of the diagram.
        // For the no longer edited object at least data processing will be executed
        // and the object invalidates the "dirty" rectangle area of the widget:
        m_uUpdateFlags |= EUpdateAll;

        // Trigger paint event
        update();
    }

    // Remove focus from currently focused object (if any object focused).
    if( m_pDiagObjFocused != nullptr )
    {
        // If the graphical output of the object changes the corresponding
        // update flags will be invalidated (the bits will be set).
        m_pDiagObjFocused->unsetFocus();

        // No object is focused anymore:
        m_pDiagObjFocused = nullptr;
        m_pDiagObjFocusedByKeyEvent = nullptr;
        m_pDiagObjFocusedByMouseEvent = nullptr;

        // Update graphical output of the diagram.
        // For the unfocused object at least data processing will be executed
        // and the object invalidates the "dirty" rectangle area of the widget:
        m_uUpdateFlags |= EUpdateAll;

        // Trigger paint event
        update();
    }

    emit lostFocus(this);

} // focusOutEvent

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtDiagram::processMoveKeyEvent( int i_iKey )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(c_iTrcDetailLevelEvents) )
    {
        strTrcMsg = qtKey2Str(i_iKey);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelEvents,
        /* strMethod    */ "processMoveKeyEvent",
        /* strAddInfo   */ strTrcMsg );

    bool    bHandled = false;
    bool    bTrcMoveKeyActive = mthTracer.isActive(c_iTrcDetailLevelEvents);
    QString strTrcMsgMoveKey;

    switch( i_iKey )
    {
        case Qt::Key_Left:
        {
            // If any object is selected ...
            if( m_pDiagObjEditing != nullptr )
            {
                if( bTrcMoveKeyActive )
                {
                    strTrcMsgMoveKey  = "Moving selected object by ";
                    strTrcMsgMoveKey += QString::number(m_fMoveKeyRepCount,'f',0) + " steps left";
                    mthTracer.trace(strTrcMsgMoveKey,c_iTrcDetailLevelEvents);
                }

                // ... move this object to the left.
                CDiagObjMoveEvent* pEv = new CDiagObjMoveEvent(CDiagObjMoveEvent::ETypeResRel,-m_fMoveKeyRepCount,0);

                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjEditing->moveEvent(pEv,true); // also inform other objects of necessary pixmap processing

                // Update graphical output of the diagram.
                // For the edited object at least data processing will be executed
                // and the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();

                delete pEv;
                pEv = nullptr;
                bHandled = true;
            }

            // If any object is focused ...
            else if( m_pDiagObjFocused != nullptr )
            {
                if( bTrcMoveKeyActive )
                {
                    strTrcMsgMoveKey = "Set focus on previous object";
                    mthTracer.trace(strTrcMsgMoveKey,c_iTrcDetailLevelEvents);
                }

                CDiagObj* pDiagObj;

                // ... set focus on previous object.
                pDiagObj = m_pDiagObjFocused->m_pDiagObjPrev;
                if( pDiagObj == nullptr )
                {
                    pDiagObj = m_pDiagObjLast;
                }
                while( pDiagObj != nullptr && pDiagObj != m_pDiagObjFocused )
                {
                    if( pDiagObj->isFocusable() )
                    {
                        // If the graphical output of the object changes the corresponding
                        // update flags will be invalidated (the bits will be set).
                        // Please note that on setting the focus to the new object the other
                        // objects will be informed by the new focused object of the necessary
                        // pixmap processing.
                        m_pDiagObjFocused->unsetFocus(false);

                        // Store the newly focused object
                        m_pDiagObjFocused = pDiagObj;
                        m_pDiagObjFocusedRecently = m_pDiagObjFocused;

                        // The focused object need to be updated at last so that no other
                        // object overwrites the graphic output of the focused object:
                        moveDiagObjInPaintList(m_pDiagObjFocused,EDiagObjMoveModeToTop);

                        // If the graphical output of the object changes the corresponding
                        // update flags will be invalidated (the bits will be set), data
                        // processing will be executed by the object and the object invalidates
                        // the "dirty" rectangle area of the widget:
                        m_pDiagObjFocused->setFocus(true); // also inform other objects of necessary pixmap processing

                        // Update graphical output of the diagram.
                        // For the unfocused and newly focused object at least data processing
                        // will be executed and the object invalidates the "dirty" rectangle
                        // area of the widget:
                        m_uUpdateFlags |= EUpdateAll;

                        // Trigger paint event
                        update();
                        break;
                    }
                    if( pDiagObj == m_pDiagObjFirst )
                    {
                        pDiagObj = m_pDiagObjLast;
                    }
                    else
                    {
                        pDiagObj = pDiagObj->m_pDiagObjPrev;
                    }
                }
                bHandled = true;
            }
            break;
        }

        case Qt::Key_Right:
        {
            // If any object is selected ...
            if( m_pDiagObjEditing != nullptr )
            {
                if( bTrcMoveKeyActive )
                {
                    strTrcMsgMoveKey  = "Moving selected object by ";
                    strTrcMsgMoveKey += QString::number(m_fMoveKeyRepCount,'f',0) + " steps right";
                    mthTracer.trace(strTrcMsgMoveKey,c_iTrcDetailLevelEvents);
                }

                // ... move this object to the right.
                CDiagObjMoveEvent* pEv = new CDiagObjMoveEvent(CDiagObjMoveEvent::ETypeResRel,m_fMoveKeyRepCount,0);

                // If the graphical output of the object changes the corresponding
                // update flags will be invalidated (the bits will be set).
                m_pDiagObjEditing->moveEvent(pEv,true); // also inform other objects of necessary pixmap processing

                // Update graphical output of the diagram.
                // For the edited object at least data processing will be executed and
                // the object invalidates the "dirty" rectangle area of the widget:
                m_uUpdateFlags |= EUpdateAll;

                // Trigger paint event
                update();

                delete pEv;
                pEv = nullptr;
                bHandled = true;
            }

            // If any object is focused ...
            else if( m_pDiagObjFocused != nullptr )
            {
                if( bTrcMoveKeyActive )
                {
                    strTrcMsgMoveKey = "Set focus on next object";
                    mthTracer.trace(strTrcMsgMoveKey,c_iTrcDetailLevelEvents);
                }

                CDiagObj* pDiagObj;

                // ... set focus on next object.
                pDiagObj = m_pDiagObjFocused->m_pDiagObjNext;
                if( pDiagObj == nullptr )
                {
                    pDiagObj = m_pDiagObjFirst;
                }
                while( pDiagObj != nullptr && pDiagObj != m_pDiagObjFocused )
                {
                    if( pDiagObj->isFocusable() )
                    {
                        // If the graphical output of the object changes the corresponding
                        // update flags will be invalidated (the bits will be set).
                        // Please note that on setting the focus to the new object the other
                        // objects will be informed by the new focused object of the necessary
                        // pixmap processing.
                        m_pDiagObjFocused->unsetFocus(false);

                        // Store the newly focused object:
                        m_pDiagObjFocused = pDiagObj;
                        m_pDiagObjFocusedRecently = m_pDiagObjFocused;

                        // The focused object need to be updated at last so that no other
                        // object overwrites the graphic output of the focused object:
                        moveDiagObjInPaintList(m_pDiagObjFocused,EDiagObjMoveModeToTop);

                        // If the graphical output of the object changes the corresponding
                        // update flags will be invalidated (the bits will be set).
                        m_pDiagObjFocused->setFocus(true); // also inform other objects of necessary pixmap processing

                        // Update graphical output of the diagram.
                        // For the unfocused and newly focused object at least data processing
                        // will be executed and the object invalidates the "dirty" rectangle
                        // area of the widget:
                        m_uUpdateFlags |= EUpdateAll;

                        // Trigger paint event
                        update();
                        break;
                    }
                    if( pDiagObj == m_pDiagObjLast )
                    {
                        pDiagObj = m_pDiagObjFirst;
                    }
                    else
                    {
                        pDiagObj = pDiagObj->m_pDiagObjNext;
                    }
                }
                bHandled = true;
            }
            break;
        }

        default:
        {
            break;
        }

    } // switch( Key )

    return bHandled;

} // processMoveKeyEvent

/*==============================================================================
protected: // overidables of base class CDataDiagram
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::updatePixmap()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "updatePixmap",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

    if( m_pPixmap == nullptr )
    {
        return;
    }

    // This method is called by objects controlling the diagram to update the
    // content of the diagram.

    // Let the diagram objects draw into the pixmap:
    CPixmapDiagram::updatePixmap();

    // If the diagram will be zoomed ..
    if( m_bIsZooming )
    {
        QPainter painterPixmap(m_pPixmap);
        QPen     pen;
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DashLine);
        painterPixmap.setPen(pen);
        painterPixmap.drawRect(QRect(m_ptZoomStart,m_ptZoomEnd));
    }

    // Mark current process depth as executed (reset bit):
    validate(EUpdatePixmap);

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

} // updatePixmap

//------------------------------------------------------------------------------
void CWdgtDiagram::updateWidget()
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ c_iTrcDetailLevelUpdate,
        /* strMethod    */ "updateWidget",
        /* strAddInfo   */ "" );

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

    // This method is called by objects controlled by the diagram to update the
    // content of the diagram.

    CDiagObj* pDiagObj;

    // Please note that only if by invalidating the flag (setting the bit) in the
    // internal update flags and on requesting the process depth by this update
    // method the corresponding update process will be executed.

    pDiagObj = m_pDiagObjPaintFirst;
    while( pDiagObj != nullptr )
    {
        // Please also note that also invisible objects need to mark their dirty
        // rectangles during widget processing.
        pDiagObj->update(EUpdateWidget);

        pDiagObj = pDiagObj->m_pDiagObjPaintNext;
    }

    if( m_pFrameStylePartCenter != nullptr )
    {
        if( m_rectFramePartCenterTopLineInv.width() > 0 && m_rectFramePartCenterTopLineInv.height() > 0 )
        {
            update(m_rectFramePartCenterTopLineInv);
        }
        if( m_rectFramePartCenterBottomLineInv.width() > 0 && m_rectFramePartCenterBottomLineInv.height() > 0 )
        {
            update(m_rectFramePartCenterBottomLineInv);
        }
        if( m_rectFramePartCenterLeftLineInv.width() > 0 && m_rectFramePartCenterLeftLineInv.height() > 0 )
        {
            update(m_rectFramePartCenterLeftLineInv);
        }
        if( m_rectFramePartCenterRightLineInv.width() > 0 && m_rectFramePartCenterRightLineInv.height() > 0 )
        {
            update(m_rectFramePartCenterRightLineInv);
        }
        m_rectFramePartCenterTopLineInv.setWidth(0);
        m_rectFramePartCenterTopLineInv.setHeight(0);
        m_rectFramePartCenterBottomLineInv.setWidth(0);
        m_rectFramePartCenterBottomLineInv.setHeight(0);
        m_rectFramePartCenterLeftLineInv.setWidth(0);
        m_rectFramePartCenterLeftLineInv.setHeight(0);
        m_rectFramePartCenterRightLineInv.setWidth(0);
        m_rectFramePartCenterRightLineInv.setHeight(0);
    }

    // The diagram itself draws frames (around the whole diagram widget or around
    // the center part). If the center part changes its size also the frame need
    // to be marked as a "dirty" rectangle.

    // Mark current process depth as executed (reset bit):
    validate(EUpdateWidget);

    if( m_pLblTipOfTheDay != nullptr && m_pLblTipOfTheDay->isVisible() )
    {
        QRect rctTipOfTheDay;
        rctTipOfTheDay.setLeft( m_rectPartCenter.left() + m_iTipOfTheDayIndentLeft );
        rctTipOfTheDay.setTop( m_rectPartCenter.top() + m_iTipOfTheDayIndentTop );
        rctTipOfTheDay.setRight( m_rectPartCenter.right() - m_iTipOfTheDayIndentRight );
        rctTipOfTheDay.setBottom( m_rectPartCenter.bottom() - m_iTipOfTheDayIndentBottom );
        m_pLblTipOfTheDay->setGeometry(rctTipOfTheDay);
    }

    if( mthTracer.isActive(c_iTrcDetailLevelUpdate) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg,c_iTrcDetailLevelUpdate);
    }

} // updateWidget

/*==============================================================================
protected slots: // overridable instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDiagram::popupMenuContextItemSaveActivated( void )
//------------------------------------------------------------------------------
{
    if( m_pPixmap != nullptr )
    {
        QString      strSaveFileName;
        QString      strSelectedFilter;
        EImageFormat imageFormat = EImageFormatUndefined;
        QFileDialog* pFileDialog = nullptr;

        if( m_strCurrentFileDir.length() == 0 )
        {
            #if QT_VERSION >= 0x040100
            m_strCurrentFileDir = QDir::current().absolutePath();
            #else
            m_strCurrentFileDir = QDir::current().absPath();
            #endif
        }
        else
        {
            QDir dirCurrent = QDir(m_strCurrentFileDir);
            if( !dirCurrent.exists() )
            {
                #if QT_VERSION >= 0x040100
                m_strCurrentFileDir = QDir::current().absolutePath();
                #else
                m_strCurrentFileDir = QDir::current().absPath();
                #endif
            }
        }

        #if QT_VERSION < 0x050000

        #if QT_VERSION >= 0x040100

        pFileDialog = new QFileDialog( this, "Choose a file name to save under" );
        pFileDialog->setDirectory(m_strCurrentFileDir);
        pFileDialog->setFileMode(QFileDialog::AnyFile);
        #if QT_VERSION >= 0x040400
        pFileDialog->setFilter(QDir::Files);
        pFileDialog->setNameFilter("*.xpm *.jpeg *.jpg *.bmp");
        #else
        pFileDialog->setFilter("*.xpm *.jpeg *.jpg *.bmp");
        #endif

        if( pFileDialog->exec() == QDialog::Accepted )
        {
            if( pFileDialog->selectedFiles().count() == 1 )
            {
                strSaveFileName = pFileDialog->selectedFiles().at(0);
                QFileInfo fileInfoSaveFile(strSaveFileName);
                strSaveFileName = fileInfoSaveFile.absolutePath() + "/" + fileInfoSaveFile.completeBaseName();
                m_strCurrentFileDir = fileInfoSaveFile.absolutePath();
                strSelectedFilter = pFileDialog->selectedFilter();
                imageFormat = getImageFormat(strSelectedFilter);
            }
        }

        #else

        pFileDialog = new QFileDialog( this, "PopupMenuContextSaveDiagramFileDialog", true );
        pFileDialog->setCaption("Choose a file name to save under");
        pFileDialog->setDir(m_strCurrentFileDir);
        pFileDialog->setMode(QFileDialog::AnyFile);
        pFileDialog->addFilter("*.XPM");
        pFileDialog->addFilter("*.JPEG *.JPG");
        pFileDialog->addFilter("*.BMP");

        if( pFileDialog->exec() == QDialog::Accepted )
        {
            strSaveFileName = pFileDialog->selectedFile();
            QFileInfo fileInfoSaveFile(strSaveFileName);
            strSaveFileName = fileInfoSaveFile.dirPath() + "/" + fileInfoSaveFile.baseName(true);
            m_strCurrentFileDir = fileInfoSaveFile.dirPath();
            strSelectedFilter = pFileDialog->selectedFilter();
            imageFormat = getImageFormat(strSelectedFilter);
        }
        #endif

        #endif // #if QT_VERSION < 0x050000

        delete pFileDialog;
        pFileDialog = nullptr;

        if( strSaveFileName.length() > 0 && imageFormat != EImageFormatUndefined )
        {
            save( strSaveFileName, imageFormat );
        }

    } // if( pixmap != nullptr )


} // popupMenuContextItemSaveActivated

//------------------------------------------------------------------------------
void CWdgtDiagram::popupMenuContextItemPrintActivated( void )
//------------------------------------------------------------------------------
{
    if( m_pPrinter != nullptr )
    {
        #if QT_VERSION < 0x050000
        #if QT_VERSION >= 0x040100
        QPrintDialog printDialog(m_pPrinter,this);
        if( printDialog.exec() == QDialog::Accepted)
        {
            print(m_pPrinter);
        }
        #else
        if( m_pPrinter->setup(this) )
        {
            print(m_pPrinter);
        }
        #endif
        #endif // #if QT_VERSION < 0x050000
    }

} // popupMenuContextItemPrintActivated
