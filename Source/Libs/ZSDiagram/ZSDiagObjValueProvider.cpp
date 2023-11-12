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

#include "ZSDiagram/ZSDiagObjValueProvider.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagObjValueProvider : public CDiagObj
*******************************************************************************/

//lint -esym(550,bConnect)

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObjValueProvider::CDiagObjValueProvider(
    const QString&       i_strObjName,
    EMathOp              i_mathOp,
    CDiagObj*            i_pDiagObjOp1,
    const CEnumScaleDir& i_scaleDirOp1,
    CDiagObj*            i_pDiagObjOp2,
    const CEnumScaleDir& i_scaleDirOp2 ) :
//------------------------------------------------------------------------------
    CDiagObj(
        /* strClassName */ CDiagObjValueProvider::ClassName(),
        /* strObjName   */ i_strObjName,
        /* pDiagTrace   */ nullptr,
        /* layoutPos    */ ELayoutPosUndefined ),
    m_mathOp(i_mathOp),
    m_pDiagObjOp1(i_pDiagObjOp1),
    m_scaleDirOp1(i_scaleDirOp1.enumerator()),
    m_pDiagObjOp2(i_pDiagObjOp2),
    m_scaleDirOp2(i_scaleDirOp2.enumerator()),
    m_valueFormat(),
    m_physVal(),
    m_strVal("---")
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( m_pDiagObjOp1 != nullptr )
    {
        if( m_scaleDirOp1 == EScaleDir::X )
        {
            QObject::connect(
                m_pDiagObjOp1, &CDiagObj::valueXChanged,
                this, &CDiagObjValueProvider::op1ValueXChanged);
        }
        else if( m_scaleDirOp1 == EScaleDir::Y )
        {
            QObject::connect(
                m_pDiagObjOp1, &CDiagObj::valueYChanged,
                this, &CDiagObjValueProvider::op1ValueYChanged);
        }
        QObject::connect(
            m_pDiagObjOp1, &CDiagObj::visibilityChanged,
            this, &CDiagObjValueProvider::op1VisibilityChanged);
    }
    if( m_pDiagObjOp2 != nullptr )
    {
        if( m_scaleDirOp2 == EScaleDir::X )
        {
            QObject::connect(
                m_pDiagObjOp2, &CDiagObj::valueXChanged,
                this, &CDiagObjValueProvider::op2ValueXChanged);
        }
        else if( m_scaleDirOp2 == EScaleDir::Y )
        {
            QObject::connect(
                m_pDiagObjOp2, &CDiagObj::valueYChanged,
                this, &CDiagObjValueProvider::op2ValueYChanged);
        }
        QObject::connect(
            m_pDiagObjOp2, &CDiagObj::visibilityChanged,
            this, &CDiagObjValueProvider::op2VisibilityChanged);
    }

} // ctor

//------------------------------------------------------------------------------
CDiagObjValueProvider::~CDiagObjValueProvider()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_mathOp = EMathOpNone;
    m_pDiagObjOp1 = nullptr;
    m_scaleDirOp1 = static_cast<EScaleDir>(0);
    m_pDiagObjOp2 = nullptr;
    m_scaleDirOp2 = static_cast<EScaleDir>(0);
    //m_valueFormat;
    //m_physVal;
    //m_strVal;

} // dtor


/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDiagObjValueProvider::setMathOp( EMathOp i_mathOp )
//------------------------------------------------------------------------------
{
    m_mathOp = i_mathOp;
    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);
}

//------------------------------------------------------------------------------
CDiagObjValueProvider::EMathOp CDiagObjValueProvider::getMathOp() const
//------------------------------------------------------------------------------
{
    return m_mathOp;
}

//------------------------------------------------------------------------------
void CDiagObjValueProvider::setOp1( CDiagObj* i_pDiagObj, const CEnumScaleDir& i_scaleDir )
//------------------------------------------------------------------------------
{
    if( m_pDiagObjOp1 != nullptr )
    {
        if( m_scaleDirOp1 == EScaleDir::X )
        {
            QObject::disconnect(
                m_pDiagObjOp1, &CDiagObj::valueXChanged,
                this, &CDiagObjValueProvider::op1ValueXChanged);
        }
        else if( m_scaleDirOp1 == EScaleDir::Y )
        {
            QObject::disconnect(
                m_pDiagObjOp1, &CDiagObj::valueYChanged,
                this, &CDiagObjValueProvider::op1ValueYChanged);
        }
        QObject::disconnect(
            m_pDiagObjOp1, &CDiagObj::visibilityChanged,
            this, &CDiagObjValueProvider::op1VisibilityChanged);
    }
    m_pDiagObjOp1 = i_pDiagObj;
    m_scaleDirOp1 = i_scaleDir.enumerator();

    if( m_pDiagObjOp1 != nullptr )
    {
        if( m_scaleDirOp1 == EScaleDir::X )
        {
            QObject::connect(
                m_pDiagObjOp1, &CDiagObj::valueXChanged,
                this, &CDiagObjValueProvider::op1ValueXChanged);
        }
        else if( m_scaleDirOp1 == EScaleDir::Y )
        {
            QObject::connect(
                m_pDiagObjOp1, &CDiagObj::valueYChanged,
                this, &CDiagObjValueProvider::op1ValueYChanged);
        }
        QObject::connect(
            m_pDiagObjOp1, &CDiagObj::visibilityChanged,
            this, &CDiagObjValueProvider::op1VisibilityChanged);
    }
    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);

} // setOp1

//------------------------------------------------------------------------------
CDiagObj* CDiagObjValueProvider::getDiagObjOp1()
//------------------------------------------------------------------------------
{
    return m_pDiagObjOp1;
}

//------------------------------------------------------------------------------
EScaleDir CDiagObjValueProvider::getScaleDirOp1() const
//------------------------------------------------------------------------------
{
    return m_scaleDirOp1;
}

//------------------------------------------------------------------------------
void CDiagObjValueProvider::setOp2( CDiagObj* i_pDiagObj, const CEnumScaleDir& i_scaleDir )
//------------------------------------------------------------------------------
{
    if( m_pDiagObjOp2 != nullptr )
    {
        if( m_scaleDirOp2 == EScaleDir::X )
        {
            QObject::disconnect(
                m_pDiagObjOp2, &CDiagObj::valueXChanged,
                this, &CDiagObjValueProvider::op2ValueXChanged);
        }
        else if( m_scaleDirOp1 == EScaleDir::Y )
        {
            QObject::disconnect(
                m_pDiagObjOp2, &CDiagObj::valueYChanged,
                this, &CDiagObjValueProvider::op2ValueYChanged);
        }
        QObject::disconnect(
            m_pDiagObjOp2, &CDiagObj::visibilityChanged,
            this, &CDiagObjValueProvider::op2VisibilityChanged);
    }
    m_pDiagObjOp2 = i_pDiagObj;
    m_scaleDirOp2 = i_scaleDir.enumerator();

    if( m_pDiagObjOp2 != nullptr )
    {
        if( m_scaleDirOp2 == EScaleDir::X )
        {
            QObject::connect(
                m_pDiagObjOp2, &CDiagObj::valueXChanged,
                this, &CDiagObjValueProvider::op2ValueXChanged);
        }
        else if( m_scaleDirOp1 == EScaleDir::Y )
        {
            QObject::connect(
                m_pDiagObjOp2, &CDiagObj::valueYChanged,
                this, &CDiagObjValueProvider::op2ValueYChanged);
        }
        QObject::connect(
            m_pDiagObjOp2, &CDiagObj::visibilityChanged,
            this, &CDiagObjValueProvider::op2VisibilityChanged);
    }
    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);

} // setOp2

//------------------------------------------------------------------------------
CDiagObj* CDiagObjValueProvider::getDiagObjOp2()
//------------------------------------------------------------------------------
{
    return m_pDiagObjOp2;
}

//------------------------------------------------------------------------------
EScaleDir CDiagObjValueProvider::getScaleDirOp2() const
//------------------------------------------------------------------------------
{
    return m_scaleDirOp2;
}

//------------------------------------------------------------------------------
void CDiagObjValueProvider::setValueFormatProvider( const SValueFormatProvider& i_valueFormat )
//------------------------------------------------------------------------------
{
    m_valueFormat = i_valueFormat;
    invalidate(EUpdateData);
}

//------------------------------------------------------------------------------
SValueFormatProvider CDiagObjValueProvider::getValueFormatProvider() const
//------------------------------------------------------------------------------
{
    return m_valueFormat;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal CDiagObjValueProvider::getVal() const
//------------------------------------------------------------------------------
{
    return m_physVal;
}

//------------------------------------------------------------------------------
QString CDiagObjValueProvider::getValString() const
//------------------------------------------------------------------------------
{
    return m_strVal;
}

/*==============================================================================
public: // must overridables of base class CDiagObj
==============================================================================*/

//------------------------------------------------------------------------------
CDiagObj* CDiagObjValueProvider::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    CDiagObj* pDiagObjOp1 = nullptr;
    CDiagObj* pDiagObjOp2 = nullptr;

    if( m_pDiagObjOp1 != nullptr )
    {
        pDiagObjOp1 = i_pDiagramTrg->findDiagObj(m_pDiagObjOp1->ClassName(), m_pDiagObjOp1->getObjName());
    }
    if( m_pDiagObjOp2 != nullptr )
    {
        pDiagObjOp2 = i_pDiagramTrg->findDiagObj(m_pDiagObjOp2->ClassName(), m_pDiagObjOp2->getObjName());
    }

    CDiagObjValueProvider* pDiagObjCloned = new CDiagObjValueProvider(
        /* strObjName  */ m_strObjName,
        /* mathOp      */ m_mathOp,
        /* pDiagObjOp1 */ pDiagObjOp1,
        /* scaleDirOp1 */ m_scaleDirOp1,
        /* pDiagObjOp2 */ pDiagObjOp2,
        /* scaleDirOp2 */ m_scaleDirOp2 );

    // Members from base class CDiagObj:
    pDiagObjCloned->m_layoutPos = m_layoutPos;
    pDiagObjCloned->m_rectContent = m_rectContent;
    pDiagObjCloned->m_bAdjustContentRect2DiagPartCenter = m_bAdjustContentRect2DiagPartCenter;
    pDiagObjCloned->m_bVisible = m_bVisible;
    pDiagObjCloned->m_state = m_state;
    pDiagObjCloned->m_bIsFocusable = m_bIsFocusable;
    pDiagObjCloned->m_bIsEditable = m_bIsEditable;

    // Members from this class:
    pDiagObjCloned->m_valueFormat = m_valueFormat;
    pDiagObjCloned->m_physVal = m_physVal;
    pDiagObjCloned->m_strVal = m_strVal;

    i_pDiagramTrg->addDiagObj(pDiagObjCloned);

    return pDiagObjCloned;

} // clone

//------------------------------------------------------------------------------
void CDiagObjValueProvider::update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal))
    {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strTrcMsg );

    if( i_uUpdateFlags == EUpdateNone )
    {
        return;
    }
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    // Layout processing is the task of the diagram:
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        validate(EUpdateLayout);
    }

    // If the internal data structures need to be updated ..
    if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        mthTracer.trace("Processing Data", ELogDetailLevel::Debug);

        // Read the operand values
        //------------------------

        if( m_pDiagObjOp1 == nullptr )
        {
            throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
        }
        if( m_mathOp != EMathOpNone )
        {
            if( m_pDiagObjOp2 == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
            }
        }

        CPhysVal physValOp1;
        CPhysVal physValOp2;
        CPhysVal physValPrev = m_physVal;
        QString  strValPrev = m_strVal;

        m_physVal.setValidity(EValueValidity::Invalid);
        m_strVal = "---";

        // Get the value from operand 1:
        physValOp1 = m_pDiagObjOp1->getVal(m_scaleDirOp1);

        // Get the value from operand 2:
        if( m_pDiagObjOp2 != nullptr )
        {
            physValOp2 = m_pDiagObjOp2->getVal(m_scaleDirOp2);
        }

        // Calculate the resulting value
        //------------------------------

        // Please note that the resulting accuracy depends on the mathematic
        // operation applied on the operands. But as long as physical values

        // with accuracies are not really supported within the current R&S system
        // we also don't take this into account here ...

        // Please note: on supporting mathematic operations like multiplications
        // and divisions either one of the operands must be with IdEUnitNone or
        // you need to know what physical units will be multiplied or divided
        // and what unit the result will be. But in the current R&S system this
        // is not really possible ...

        switch( m_mathOp )
        {
            case EMathOpNone:
            {
                m_physVal = physValOp1;

                if( !m_pDiagObjOp1->isVisible() )
                {
                    m_physVal.setValidity(EValueValidity::Invalid);
                }
                break;
            }
            case EMathOpAddition:
            case EMathOpSubtraction:
            {
                if( m_pDiagObjOp2 != nullptr )
                {
                    if( physValOp1.isValid() && physValOp2.isValid() )
                    {
                        if( m_mathOp == EMathOpAddition )
                        {
                            m_physVal = physValOp1 + physValOp2;
                        }
                        else
                        {
                            m_physVal = physValOp1 - physValOp2;
                        }
                    }
                    if( !m_pDiagObjOp1->isVisible() || !m_pDiagObjOp2->isVisible() )
                    {
                        m_physVal.setValidity(EValueValidity::Invalid);
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }

        // Convert the resulting value into its string representation:
        //------------------------------------------------------------

        m_strVal = m_physVal.toString(m_valueFormat);

        if( m_physVal != physValPrev || strValPrev != m_strVal )
        {
            emit valueChanged(this);
        }

        // Mark current process depth as executed (reset bit):
        validate(EUpdateData);

    } // if( EUpdateData )

    // If recalculation of bitmap is needed ..
    if( i_uUpdateFlags & EUpdatePixmap && m_uUpdateFlags & EUpdatePixmap && i_pPaintDevice != nullptr )
    {
        mthTracer.trace("Processing Pixmap", ELogDetailLevel::Debug);

        // Mark current process depth as executed (reset bit):
        validate(EUpdatePixmap);

    } // if( EUpdatePixmap )

    // If the widget need to be updated ..
    if( i_uUpdateFlags & EUpdateWidget && m_uUpdateFlags & EUpdateWidget )
    {
        mthTracer.trace("Processing Widget", ELogDetailLevel::Debug);

        // Mark current process depth as executed (reset bit):
        validate(EUpdateWidget);

    } // if( EUpdateWidget )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update

/*==============================================================================
protected slots:
==============================================================================*/

//lint -esym(818,i_pDiagObjOp1)
//------------------------------------------------------------------------------
void CDiagObjValueProvider::op1ValueXChanged( ZS::Diagram::CDiagObj* i_pDiagObjOp1 )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        if( i_pDiagObjOp1 != nullptr )
        {
            strTrcMsg = i_pDiagObjOp1->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "op1ValueXChanged",
        /* strAddInfo   */ strTrcMsg );

    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);
}
//lint +esym(818,i_pDiagObjOp1)

//lint -esym(818,i_pDiagObjOp1)
//------------------------------------------------------------------------------
void CDiagObjValueProvider::op1ValueYChanged( ZS::Diagram::CDiagObj* i_pDiagObjOp1 )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        if( i_pDiagObjOp1 != nullptr )
        {
            strTrcMsg = i_pDiagObjOp1->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "op1ValueYChanged",
        /* strAddInfo   */ strTrcMsg );

    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);
}
//lint +esym(818,i_pDiagObjOp1)

//lint -esym(818,i_pDiagObjOp2)
//------------------------------------------------------------------------------
void CDiagObjValueProvider::op2ValueXChanged( ZS::Diagram::CDiagObj* i_pDiagObjOp2 )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        if( i_pDiagObjOp2 != nullptr )
        {
            strTrcMsg = i_pDiagObjOp2->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "op2ValueXChanged",
        /* strAddInfo   */ strTrcMsg );

    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);
}
//lint +esym(818,i_pDiagObjOp2)

//lint -esym(818,i_pDiagObjOp2)
//------------------------------------------------------------------------------
void CDiagObjValueProvider::op2ValueYChanged( ZS::Diagram::CDiagObj* i_pDiagObjOp2 )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        if( i_pDiagObjOp2 != nullptr )
        {
            strTrcMsg = i_pDiagObjOp2->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "op2ValueYChanged",
        /* strAddInfo   */ strTrcMsg );

    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);
}
//lint +esym(818,i_pDiagObjOp2)

//lint -esym(818,i_pDiagObjOp1)
//------------------------------------------------------------------------------
void CDiagObjValueProvider::op1VisibilityChanged( ZS::Diagram::CDiagObj* i_pDiagObjOp1 )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        if( i_pDiagObjOp1 != nullptr )
        {
            strTrcMsg = i_pDiagObjOp1->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "op1VisibilityChanged",
        /* strAddInfo   */ strTrcMsg );

    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);
}
//lint +esym(818,i_pDiagObjOp1)

//lint -esym(818,i_pDiagObjOp2)
//------------------------------------------------------------------------------
void CDiagObjValueProvider::op2VisibilityChanged( ZS::Diagram::CDiagObj* i_pDiagObjOp2 )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        if( i_pDiagObjOp2 != nullptr )
        {
            strTrcMsg = i_pDiagObjOp2->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "op2VisibilityChanged",
        /* strAddInfo   */ strTrcMsg );

    m_physVal.setValidity(EValueValidity::Invalid);
    invalidate(EUpdateData);
}
//lint +esym(818,i_pDiagObjOp2)

