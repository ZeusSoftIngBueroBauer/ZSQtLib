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

#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagram.h"
//#include "ZSPhysVal/ZSPhysVal.h"
//#include "ZSSys/ZSSysErrResult.h"
//#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

//#include <float.h>
//#include <math.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagTrace
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

bool CDiagTrace::s_bClassInitialised = false;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDiagTrace::CDiagTrace(
    const QString& i_strObjName,
    CDiagScale*    i_pDiagScaleX,
    CDiagScale*    i_pDiagScaleY ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_pDiagram(nullptr),
    //m_arpDiagScale[EScaleDirCount]
    //m_arphysValRes[EScaleDirCount]
    //m_arphysValArr[EScaleDirCount]
    m_uUpdateFlags(EUpdateData),
    m_pDiagTraceNext(nullptr),
    m_pDiagTracePrev(nullptr),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjValidate(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Update", objectName());
    m_pTrcAdminObjValidate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Validate", objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", ScaleX: " + QString(i_pDiagScaleX == nullptr ? "null" : i_pDiagScaleX->objectName());
        strMthInArgs += ", ScaleY: " + QString(i_pDiagScaleY == nullptr ? "null" : i_pDiagScaleY->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( !s_bClassInitialised )
    {
        s_bClassInitialised = true;
    }

    m_arpDiagScale[EScaleDirX] = i_pDiagScaleX;
    m_arpDiagScale[EScaleDirY] = i_pDiagScaleY;

    //m_arphysValRes[EScaleDirX].setValidity(EValueValidity::Invalid);
    //m_arphysValRes[EScaleDirY].setValidity(EValueValidity::Invalid);

    //m_arphysValArr[EScaleDirX].setValidity(EValueValidity::Invalid);
    //m_arphysValArr[EScaleDirY].setValidity(EValueValidity::Invalid);

    if( i_pDiagScaleX != nullptr )
    {
        m_arphysValRes[EScaleDirX].setUnit(i_pDiagScaleX->getScale().m_unit);
        m_arphysValArr[EScaleDirX].setUnit(i_pDiagScaleX->getScale().m_unit);
    }
    if( i_pDiagScaleY != nullptr )
    {
        m_arphysValRes[EScaleDirY].setUnit(i_pDiagScaleY->getScale().m_unit);
        m_arphysValArr[EScaleDirY].setUnit(i_pDiagScaleY->getScale().m_unit);
    }

} // ctor

//------------------------------------------------------------------------------
CDiagTrace::~CDiagTrace()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjUpdate);
    m_pTrcAdminObjUpdate = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjValidate);
    m_pTrcAdminObjValidate = nullptr;

    m_pDiagram = nullptr;
    memset(m_arpDiagScale, 0x00, EScaleDirCount*sizeof(m_arpDiagScale[0]));
    //m_arphysValRes;
    //m_arphysValArr;
    m_uUpdateFlags = EUpdateNone;
    m_pDiagTraceNext = nullptr;
    m_pDiagTracePrev = nullptr;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjValidate = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDiagTrace::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
CDataDiagram* CDiagTrace::getDiagram()
//------------------------------------------------------------------------------
{
    return m_pDiagram;
}

//lint -e1762
//------------------------------------------------------------------------------
CDiagScale* CDiagTrace::getDiagScale( EScaleDir i_scaleDir )
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arpDiagScale[i_scaleDir];
}
//lint +e1762

//------------------------------------------------------------------------------
ESpacing CDiagTrace::getSpacing( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir]->getSpacing();
}

//------------------------------------------------------------------------------
bool CDiagTrace::isScaleValid( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir != EScaleDirCount && (i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    bool bScaleValid = true;
    int  idxScaleMin;
    int  idxScaleMax;
    int  idxScale;

    if( i_scaleDir == EScaleDirCount )
    {
        idxScaleMin = EScaleDirMin;
        idxScaleMax = EScaleDirMax;
    }
    else
    {
        idxScaleMin = i_scaleDir;
        idxScaleMax = i_scaleDir;
    }

    const CDiagScale* pDiagScale;

    for( idxScale = idxScaleMin; idxScale <= idxScaleMax; idxScale++ )
    {
        pDiagScale = m_arpDiagScale[idxScale];

        if( pDiagScale == nullptr )
        {
            bScaleValid = false;
        }
        else
        {
            bScaleValid = pDiagScale->isScaleValid();
        }
        if( !bScaleValid )
        {
            break;
        }

    } // for( idxScale <= idxScaleMax )

    return bScaleValid;

} // isScaleValid

//------------------------------------------------------------------------------
SScale CDiagTrace::getScale( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir]->getScale();

} // getScale

//------------------------------------------------------------------------------
int CDiagTrace::getScaleMinValPix( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir]->getScaleMinValPix();

} // getScaleMinValPix

//------------------------------------------------------------------------------
int CDiagTrace::getScaleMaxValPix( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir]->getScaleMaxValPix();

} // getScaleMaxValPix

//------------------------------------------------------------------------------
int CDiagTrace::getScaleRangePix( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir]->getScaleRangePix();

} // getScaleRangePix

//------------------------------------------------------------------------------
double CDiagTrace::getScaleRes( EScaleDir i_scaleDir, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir];
    return pDiagScale->getScaleRes(i_pUnit);

} // getScaleRes

//------------------------------------------------------------------------------
double CDiagTrace::getScaleRes( EScaleDir i_scaleDir, double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir];
    return pDiagScale->getScaleRes(i_fVal,i_pUnit);

} // getScaleRes

//------------------------------------------------------------------------------
double CDiagTrace::round2ScaleRes( EScaleDir i_scaleDir, double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    double fRes = getScaleRes(i_scaleDir,i_fVal,i_pUnit);
    double fVal = Math::round2Res(i_fVal,fRes);
    return fVal;

} // round2ScaleRes

//------------------------------------------------------------------------------
double CDiagTrace::getValRes( EScaleDir i_scaleDir, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir];

    double fRes;

    if( m_arphysValArr[i_scaleDir].hasRes() )
    {
        fRes = m_arphysValArr[i_scaleDir].getRes().getVal(i_pUnit);
    }
    else if( m_arphysValRes[i_scaleDir].isValid() )
    {
        fRes = m_arphysValRes[i_scaleDir].getVal(i_pUnit);
    }
    else
    {
        fRes = pDiagScale->getScaleRes(i_pUnit);
    }
    if( i_pUnit != nullptr && *i_pUnit != pDiagScale->getScale().m_unit )
    {
        fRes = pDiagScale->getScale().m_unit.convertValue(fRes, *i_pUnit);
    }
    return fRes;

} // getValRes

//------------------------------------------------------------------------------
double CDiagTrace::getValRes( EScaleDir i_scaleDir, double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir];

    double fRes;

    if( m_arphysValArr[i_scaleDir].hasRes() )
    {
        fRes = m_arphysValArr[i_scaleDir].getRes().getVal(i_pUnit);
    }
    else if( m_arphysValRes[i_scaleDir].isValid() )
    {
        fRes = m_arphysValRes[i_scaleDir].getVal(i_pUnit);
    }
    else
    {
        fRes = pDiagScale->getScaleRes(i_fVal,i_pUnit);
    }
    if( i_pUnit != nullptr && *i_pUnit != pDiagScale->getScale().m_unit )
    {
        fRes = pDiagScale->getScale().m_unit.convertValue(fRes, *i_pUnit);
    }
    return fRes;

} // getValRes

//------------------------------------------------------------------------------
double CDiagTrace::round2ValRes( EScaleDir i_scaleDir, double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    double fRes = getValRes(i_scaleDir, i_fVal, i_pUnit);
    double fVal = Math::round2Res(i_fVal, fRes);
    return fVal;
}

//------------------------------------------------------------------------------
void CDiagTrace::setValues( EScaleDir i_scaleDir, const CPhysValArr& i_physValArr )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "copyValues",
        /* strAddInfo   */ "" );

    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir];

    m_arphysValArr[i_scaleDir] = i_physValArr;

    if( i_physValArr.unit().isValid() && pDiagScale->getScale().m_unit != i_physValArr.unit() )
    {
        m_arphysValArr[i_scaleDir].convertValues(pDiagScale->getScale().m_unit);
    }

    invalidate(EUpdateData);

    if( m_pDiagram != nullptr )
    {
        m_pDiagram->traceChanged(this);
    }

} // setValues

//------------------------------------------------------------------------------
void CDiagTrace::setValues(
    EScaleDir              i_scaleDir,
    const QVector<double>& i_arfValues,
    CUnit*                 i_pUnitVal,
    double                 i_fRes,
    CUnit*                 i_pUnitRes )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "copyValues",
        /* strAddInfo   */ "" );

    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir];
    CUnit  unitVal = i_pUnitVal == nullptr ? pDiagScale->getScale().m_unit : *i_pUnitVal;
    double fRes    = i_fRes;
    CUnit  unitRes = i_pUnitRes == nullptr ? unitVal : *i_pUnitRes;

    if( !areOfSameUnitGroup(m_arphysValArr[i_scaleDir].unit(),unitVal) )
    {
        m_arphysValArr->setUnit(unitVal);
    }

    if( fRes > 0.0 )
    {
        if( pDiagScale->getScale().m_unit != unitRes )
        {
            fRes = unitRes.convertValue(fRes, pDiagScale->getScale().m_unit);
        }
        m_arphysValArr[i_scaleDir].setRes( CPhysValRes(fRes,pDiagScale->getScale().m_unit) );
    }

    m_arphysValArr[i_scaleDir].setValues(0, i_arfValues, unitVal);

    if( pDiagScale->getScale().m_unit != unitVal )
    {
        m_arphysValArr[i_scaleDir].convertValues(pDiagScale->getScale().m_unit);
    }
    m_arphysValArr[i_scaleDir].setValidity(EValueValidity::Valid);

    invalidate(EUpdateData);

    if( m_pDiagram != nullptr )
    {
        m_pDiagram->traceChanged(this);
    }

} // setValues

//------------------------------------------------------------------------------
bool CDiagTrace::areValuesValid( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir != EScaleDirCount && (i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    bool bValuesValid = true;
    int  idxScaleMin;
    int  idxScaleMax;
    int  idxScale;

    if( i_scaleDir == EScaleDirCount )
    {
        idxScaleMin = EScaleDirMin;
        idxScaleMax = EScaleDirMax;
    }
    else
    {
        idxScaleMin = i_scaleDir;
        idxScaleMax = i_scaleDir;
    }

    for( idxScale = idxScaleMin; idxScale <= idxScaleMax; idxScale++ )
    {
        if( !m_arphysValArr[idxScale].isValid() )
        {
            bValuesValid = false;
            break;
        }
    }
    return bValuesValid;

} // areValuesValid

//------------------------------------------------------------------------------
int CDiagTrace::getValCount( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arphysValArr[i_scaleDir].size();
}

//------------------------------------------------------------------------------
QVector<double> CDiagTrace::getValues( EScaleDir i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arphysValArr[i_scaleDir].toDoubleVec(0,EArrayIndexCountAllElements);

} // getValues

//------------------------------------------------------------------------------
int CDiagTrace::getValPix( EScaleDir i_scaleDir, double i_fVal, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir]->getValPix(i_fVal,i_pUnit);

} // getValPix

//------------------------------------------------------------------------------
QString CDiagTrace::getValString(
    EScaleDir i_scaleDir,
    double    i_fVal,
    CUnit*    i_pUnit,
    int       i_iDigitsCountMax,
    bool      i_bUseEngineeringFormat ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    QString strVal = "---";

    if( isScaleValid(i_scaleDir) )
    {
        double fVal = i_fVal;
        int    iLeadingDigits = 1;
        int    iTrailingDigits = 1;
        int    iExponentDigits = 0;
        double fRes = getValRes(i_scaleDir,i_fVal,i_pUnit);

        if( fVal >= 10.0 )
        {
            iLeadingDigits = static_cast<int>(log10(fabs(fVal)))+1;
        }
        if( fRes < 0.1 )
        {
            double fResLog = log10(fabs(fRes));
            double fResLogInt;
            double fResLogFrac = modf(fResLog,&fResLogInt);
            iTrailingDigits = static_cast<int>(-fResLog);
            if( fResLogFrac > 0.0 )
            {
                iTrailingDigits += 1;
            }
        }
        if( i_iDigitsCountMax > 0 || i_bUseEngineeringFormat )
        {
            if( ( (iLeadingDigits + iTrailingDigits) > i_iDigitsCountMax )
             || ( i_bUseEngineeringFormat && (iLeadingDigits > 1) ) )
            {
                if( fVal > 1.0 )
                {
                    iExponentDigits = static_cast<int>(log10(static_cast<double>(iLeadingDigits)))+1;
                }
                else if( fVal < 1.0 )
                {
                    iExponentDigits = static_cast<int>(log10(static_cast<double>(iTrailingDigits)))+1;
                }
                iTrailingDigits += iLeadingDigits-1;
                iLeadingDigits = 1;
                if( (iLeadingDigits + iTrailingDigits) > i_iDigitsCountMax )
                {
                    iTrailingDigits = i_iDigitsCountMax - iLeadingDigits;
                }
            }
        }
        if( iExponentDigits > 0 )
        {
            strVal = QString::number(
                /* fVal       */ fVal,
                /* chFormat   */ 'e',
                /* iPrecision */ iTrailingDigits );
        }
        else
        {
            strVal = QString::number(
                /* fVal       */ fVal,
                /* chFormat   */ 'f',
                /* iPrecision */ iTrailingDigits );
        }
    }
    return strVal;

} // getValString

//------------------------------------------------------------------------------
EValueValidity CDiagTrace::getVal(
    EScaleDir i_scaleDirSrc,
    double    i_fValSrc,
    CUnit*    i_pUnitSrc,
    EScaleDir i_scaleDirDst,
    double*   o_pfValDst,
    CUnit*    i_pUnitDst,
    bool      i_bRound2Res ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDirSrc < EScaleDirMin || i_scaleDirSrc > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDirDst] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    if( i_scaleDirDst < EScaleDirMin || i_scaleDirDst > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDirSrc] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    int idxScale;

    for( idxScale = 0; idxScale < EScaleDirCount; idxScale++ )
    {
        if( !m_arphysValArr[idxScale].isValid() )
        {
            return EValueValidity::Invalid;
        }
    }

    const CDiagScale* pDiagScaleSrc  = m_arpDiagScale[i_scaleDirSrc];
    const CDiagScale* pDiagScaleDst  = m_arpDiagScale[i_scaleDirDst];
    double            fScaleMinSrc   = pDiagScaleSrc->getScale().m_fMin;
    double            fScaleMaxSrc   = pDiagScaleSrc->getScale().m_fMax;
    double            fScaleRangeSrc = fScaleMaxSrc - fScaleMinSrc;
    QVector<double>   arfValuesSrc   = m_arphysValArr[i_scaleDirSrc].toDoubleVec(0,EArrayIndexCountAllElements);
    QVector<double>   arfValuesDst   = m_arphysValArr[i_scaleDirDst].toDoubleVec(0,EArrayIndexCountAllElements);
    int               iValCount      = 0;
    double            fValSrc        = i_fValSrc;
    double            fResSrc        = 0.0;
    double            fValDst        = 0.0;
    double            fResDst        = 0.0;
    double            fValSrcPrev    = 0.0;
    double            fValSrcNext    = 0.0;
    const double*     pfValSrcPrev   = nullptr;
    const double*     pfValDstPrev   = nullptr;
    double            fValDstPrev    = 0.0;
    double            fValDstNext    = 0.0;
    const double*     pfValSrcNext   = nullptr;
    const double*     pfValDstNext   = nullptr;
    EValueValidity    validDst       = EValueValidity::Invalid;
    double            fDistSrc       = 0.0;
    double            fDistDst       = 0.0;
    double            fM             = 0.0;
    double            fT             = 0.0;
    int               idxVal;

    if( arfValuesSrc.size() == 0 || arfValuesDst.size() == 0 )
    {
        return EValueValidity::Invalid;
    }

    if( i_pUnitSrc != nullptr && *i_pUnitSrc != pDiagScaleSrc->getScale().m_unit )
    {
        fValSrc = i_pUnitSrc->convertValue(fValSrc, pDiagScaleSrc->getScale().m_unit);
    }

    if( arfValuesSrc.size() > arfValuesDst.size() )
    {
        iValCount = arfValuesDst.size();
    }
    else
    {
        iValCount = arfValuesSrc.size();
    }

    fResSrc = getValRes(i_scaleDirSrc, i_fValSrc, i_pUnitSrc);
    fResDst = getValRes(i_scaleDirDst, i_pUnitDst);

    if( iValCount == 1 )
    {
        fValSrcPrev = arfValuesSrc[0];
        fValDstPrev = arfValuesDst[0];
        fValSrcNext = arfValuesSrc[0];
        fValDstNext = arfValuesDst[0];

        if( ( fValSrc >= (arfValuesSrc[0]-fResSrc) ) && ( fValSrc <= (arfValuesSrc[0]+fResSrc) ) )
        {
            validDst = EValueValidity::Valid;
        }
    }
    else if( iValCount > 1 )
    {
        if( fValSrc >= (arfValuesSrc[0]-fResSrc/2.0) && fValSrc <= (arfValuesSrc[iValCount-1]+fResSrc/2.0) )
        {
            double fValSrcTmp = fValSrc;

            validDst = EValueValidity::Valid;

            fScaleMinSrc   = arfValuesSrc[0];
            fScaleMaxSrc   = arfValuesSrc[iValCount-1];
            fScaleRangeSrc = fScaleMaxSrc - fScaleMinSrc;

            if( pDiagScaleSrc->getSpacing() == ESpacingLogarithmic )
            {
                if( fScaleMinSrc > 0.0 && fScaleMaxSrc > 0.0 && fValSrc > 0.0 )
                {
                    fValSrcTmp     = log10(fValSrc);
                    fScaleMinSrc   = log10(arfValuesSrc[0]);
                    fScaleMaxSrc   = log10(arfValuesSrc[iValCount-1]);
                    fScaleRangeSrc = fScaleMaxSrc - fScaleMinSrc;
                }
            }
            idxVal = static_cast<int>( static_cast<double>(iValCount-1) * (fValSrcTmp - fScaleMinSrc) / fScaleRangeSrc );

            if( fValSrc > arfValuesSrc[idxVal] && idxVal >= 0 && idxVal < (iValCount-1) )
            {
                pfValSrcPrev = &arfValuesSrc[idxVal];
                pfValDstPrev = &arfValuesDst[idxVal];
                pfValSrcNext = &arfValuesSrc[idxVal+1];
                pfValDstNext = &arfValuesDst[idxVal+1];

                for( ; idxVal < iValCount-1; idxVal++, pfValSrcPrev++, pfValSrcNext++, pfValDstPrev++, pfValDstNext++ )
                {
                    fValSrcPrev = *pfValSrcPrev;
                    fValSrcNext = *pfValSrcNext;
                    fValDstPrev = *pfValDstPrev;
                    fValDstNext = *pfValDstNext;

                    if( fValSrc >= fValSrcPrev && fValSrc <= fValSrcNext )
                    {
                        break;
                    }
                }
            }
            else if( fValSrc < arfValuesSrc[idxVal] && idxVal > 0 && idxVal < iValCount )
            {
                pfValSrcPrev = &arfValuesSrc[idxVal-1];
                pfValDstPrev = &arfValuesDst[idxVal-1];
                pfValSrcNext = &arfValuesSrc[idxVal];
                pfValDstNext = &arfValuesDst[idxVal];

                for( ; idxVal > 0; idxVal--, pfValSrcPrev--, pfValSrcNext--, pfValDstPrev--, pfValDstNext-- )
                {
                    fValSrcPrev = *pfValSrcPrev;
                    fValSrcNext = *pfValSrcNext;
                    fValDstPrev = *pfValDstPrev;
                    fValDstNext = *pfValDstNext;

                    if( fValSrc >= fValSrcPrev && fValSrc <= fValSrcNext )
                    {
                        break;
                    }
                }
            }
            else if( fValSrc >= (arfValuesSrc[idxVal]-fResSrc/2.0) && fValSrc <= (arfValuesSrc[idxVal]+fResSrc/2.0) && idxVal >= 0 && idxVal < iValCount )
            {
                fValSrcPrev = arfValuesSrc[idxVal];
                fValDstPrev = arfValuesDst[idxVal];
                fValSrcNext = arfValuesSrc[idxVal];
                fValDstNext = arfValuesDst[idxVal];
            }
        }
    }
    if( fValSrcNext < fValSrcPrev || validDst != EValueValidity::Valid )
    {
        return EValueValidity::Invalid;
    }
    if( o_pfValDst != nullptr )
    {
        if( fValSrcNext > fValSrcPrev )
        {
            fDistSrc = fValSrcNext - fValSrcPrev;
            fDistDst = fValDstNext - fValDstPrev;
            fM = fDistDst / fDistSrc;
            fT = fValDstPrev - fM * fValSrcPrev;
            fValDst = fM * fValSrc + fT;
        }
        else // if( fValSrcNext == fValSrcPrev )
        {
            fValDst = fValDstPrev;
        }
        if( fResDst != 0.0 && i_bRound2Res )
        {
            fValDst = Math::round2Res( fValDst, fabs(fResDst) );
        }
        if( i_pUnitDst != nullptr && *i_pUnitDst != pDiagScaleDst->getScale().m_unit )
        {
            fValDst = pDiagScaleDst->getScale().m_unit.convertValue(fValDst, *i_pUnitDst);
        }
        *o_pfValDst = fValDst;
    }
    return EValueValidity::Valid;

} // getVal

//------------------------------------------------------------------------------
double CDiagTrace::getVal( EScaleDir i_scaleDir, double i_fPix, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( i_scaleDir < EScaleDirMin || i_scaleDir > EScaleDirMax )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    if( m_arpDiagScale[i_scaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir]->getVal(i_fPix,i_pUnit);

} // getVal

//------------------------------------------------------------------------------
void CDiagTrace::invalidate( unsigned int i_uUpdateFlags )
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
        // A trace object does not draw themselves into a pixmap. Only data
        // processing is needed. Pixmap and widget processing is not needed.
        m_uUpdateFlags |= (i_uUpdateFlags & ~(EUpdatePixmap|EUpdateWidget));
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // invalidate

//------------------------------------------------------------------------------
void CDiagTrace::validate( unsigned int i_uUpdateFlags )
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

//------------------------------------------------------------------------------
void CDiagTrace::update( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if( m_pTrcAdminObjUpdate != nullptr && m_pTrcAdminObjUpdate->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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

    int idxScale;

    const CDiagScale* pDiagScale = m_arpDiagScale[0];
    double            fScaleRangePix;
    double            fScaleRangeVal;
    double            fScaleRangeValLog;
    bool              bScaleValid;

    for( idxScale = 0; idxScale < EScaleDirCount; idxScale++ )
    {
        pDiagScale = m_arpDiagScale[idxScale];

        if( pDiagScale != nullptr )
        {
            bScaleValid = isScaleValid(static_cast<EScaleDir>(idxScale));

            fScaleRangePix = getScaleRangePix(static_cast<EScaleDir>(idxScale));
            fScaleRangeVal = pDiagScale->getScale().m_fMax - pDiagScale->getScale().m_fMin;
            fScaleRangeValLog = 0.0;

            if( bScaleValid && pDiagScale->getSpacing() == ESpacingLogarithmic )
            {
                fScaleRangeValLog = log10(pDiagScale->getScale().m_fMax) - log10(pDiagScale->getScale().m_fMin);
            }

            // Internal resolution of the values
            //----------------------------------

            // The resulting resolution may be either the resolution as defined by the value
            // array or the "internal" resolution as calculated in the following depending on
            // the scale range, the number of pixels used for the scale or the number of values.

            double fValRes = fScaleRangeVal;
            double fValResTmp = fValRes;

            if( bScaleValid )
            {
                if( pDiagScale->getSpacing() == ESpacingLinear )
                {
                    if( fScaleRangePix > 1.0 )
                    {
                        fValRes = fScaleRangeVal / (fScaleRangePix-1.0);
                    }
                    else
                    {
                        fValRes = fScaleRangeVal / 1000.0;
                    }
                    if( idxScale == EScaleDirX )
                    {
                        fValResTmp = fValRes;

                        if( m_arphysValArr[idxScale].size() > 1 )
                        {
                            fValRes = fScaleRangeVal / (m_arphysValArr[idxScale].size()-1);
                        }
                        if( fValRes > fValResTmp )
                        {
                            fValRes = fValResTmp;
                        }
                    }
                }
                else
                {
                    if( fScaleRangePix > 1.0 )
                    {
                        fValRes = fScaleRangeValLog / (fScaleRangePix-1.0);
                    }
                    else
                    {
                        fValRes = fScaleRangeValLog / 1000.0;
                    }
                    if( idxScale == EScaleDirX )
                    {
                        fValResTmp = fValRes;

                        if( m_arphysValArr[idxScale].size() > 1 )
                        {
                            fValResTmp = fScaleRangeValLog / (m_arphysValArr[idxScale].size()-1);
                        }
                        if( fValRes > fValResTmp )
                        {
                            fValRes = fValResTmp;
                        }
                    }
                }
                fValRes = Math::round2LowerDecade(fValRes);
            }
            m_arphysValRes[idxScale].setRes(fValRes);

        } // if( pDiagScale != nullptr )

    } // for( idxScale < EScaleDirCount )

    validate(EUpdateData);

    emit traceChanged(this);

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

} // update

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CDiagTrace* CDiagTrace::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    const CDiagScale* pDiagScaleXSrc = m_arpDiagScale[EScaleDirX];
    const CDiagScale* pDiagScaleYSrc = m_arpDiagScale[EScaleDirY];

    if( pDiagScaleXSrc == nullptr || pDiagScaleYSrc == nullptr )
    {
        return nullptr;
    }

    CDiagScale* pDiagScaleX = i_pDiagramTrg->getDiagScale( pDiagScaleXSrc->getObjName() );
    CDiagScale* pDiagScaleY = i_pDiagramTrg->getDiagScale( pDiagScaleYSrc->getObjName() );

    if( pDiagScaleX == nullptr || pDiagScaleY == nullptr )
    {
        return nullptr;
    }

    CDiagTrace* pDiagTrace = new CDiagTrace(
        /* strObjName  */ m_strObjName,
        /* pDiagScaleX */ pDiagScaleX,
        /* pDiagScaleY */ pDiagScaleY );

    int idxScale;

    for( idxScale = 0; idxScale < EScaleDirCount; idxScale++ )
    {
        pDiagTrace->m_arphysValArr[idxScale] = m_arphysValArr[idxScale];
    }
    i_pDiagramTrg->addDiagTrace(pDiagTrace);

    return pDiagTrace;

} // clone
