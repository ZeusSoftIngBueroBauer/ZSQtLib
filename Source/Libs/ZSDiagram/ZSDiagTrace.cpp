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
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

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
    m_arpDiagScale(CEnumScaleDir::count(), nullptr),
    m_arphysValRes(CEnumScaleDir::count()),
    m_arphysValArr(CEnumScaleDir::count()),
    m_uUpdateFlags(EUpdateData),
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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

    m_arpDiagScale[static_cast<int>(EScaleDir::X)] = i_pDiagScaleX;
    m_arpDiagScale[static_cast<int>(EScaleDir::Y)] = i_pDiagScaleY;

    //m_arphysValRes[static_cast<int>(EScaleDir::X)].setValidity(EValueValidity::Invalid);
    //m_arphysValRes[static_cast<int>(EScaleDir::Y)].setValidity(EValueValidity::Invalid);

    //m_arphysValArr[static_cast<int>(EScaleDir::X)].setValidity(EValueValidity::Invalid);
    //m_arphysValArr[static_cast<int>(EScaleDir::Y)].setValidity(EValueValidity::Invalid);

    if( i_pDiagScaleX != nullptr )
    {
        m_arphysValRes[static_cast<int>(EScaleDir::X)].setUnit(i_pDiagScaleX->getScale().m_unit);
        m_arphysValArr[static_cast<int>(EScaleDir::X)].setUnit(i_pDiagScaleX->getScale().m_unit);
    }
    if( i_pDiagScaleY != nullptr )
    {
        m_arphysValRes[static_cast<int>(EScaleDir::Y)].setUnit(i_pDiagScaleY->getScale().m_unit);
        m_arphysValArr[static_cast<int>(EScaleDir::Y)].setUnit(i_pDiagScaleY->getScale().m_unit);
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
    //m_arpDiagScale.clear();
    //m_arphysValRes.clear();
    //m_arphysValArr.clear();
    m_uUpdateFlags = EUpdateNone;
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
CDiagScale* CDiagTrace::getDiagScale( const CEnumScaleDir& i_scaleDir )
//------------------------------------------------------------------------------
{
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()];
}
//lint +e1762

//------------------------------------------------------------------------------
ESpacing CDiagTrace::getSpacing( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getSpacing();
}

//------------------------------------------------------------------------------
bool CDiagTrace::isScaleValid( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    bool bScaleValid = true;
    int  idxScaleMin = 0;
    int  idxScaleMax = CEnumScaleDir::count()-1;

    if( i_scaleDir.isValid() )
    {
        idxScaleMin = i_scaleDir.enumeratorAsInt();
        idxScaleMax = i_scaleDir.enumeratorAsInt();
    }

    for (int idxScale = idxScaleMin; idxScale <= idxScaleMax; idxScale++)
    {
        const CDiagScale*  pDiagScale = m_arpDiagScale[idxScale];

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
    }
    return bScaleValid;
}

//------------------------------------------------------------------------------
SScale CDiagTrace::getScale( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getScale();
}

//------------------------------------------------------------------------------
int CDiagTrace::getScaleMinValPix( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getMinValPix();
}

//------------------------------------------------------------------------------
int CDiagTrace::getScaleMaxValPix( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getMaxValPix();
}

//------------------------------------------------------------------------------
int CDiagTrace::getScaleRangePix( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getRangePix();
}

//------------------------------------------------------------------------------
double CDiagTrace::getScaleRes( const CEnumScaleDir& i_scaleDir, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir.enumeratorAsInt()];
    return pDiagScale->getScaleRes(i_pUnit);
}

//------------------------------------------------------------------------------
double CDiagTrace::getScaleRes( const CEnumScaleDir& i_scaleDir, double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir.enumeratorAsInt()];
    return pDiagScale->getScaleRes(i_fVal, i_pUnit);
}

//------------------------------------------------------------------------------
double CDiagTrace::round2ScaleRes( const CEnumScaleDir& i_scaleDir, double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double fRes = getScaleRes(i_scaleDir,i_fVal,i_pUnit);
    double fVal = Math::round2Resolution(i_fVal,fRes);
    return fVal;
}

//------------------------------------------------------------------------------
double CDiagTrace::getValRes( const CEnumScaleDir& i_scaleDir, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir.enumeratorAsInt()];

    double fRes;

    if( m_arphysValArr[i_scaleDir.enumeratorAsInt()].hasRes() )
    {
        fRes = m_arphysValArr[i_scaleDir.enumeratorAsInt()].getRes().getVal(i_pUnit);
    }
    else if( m_arphysValRes[i_scaleDir.enumeratorAsInt()].isValid() )
    {
        fRes = m_arphysValRes[i_scaleDir.enumeratorAsInt()].getVal(i_pUnit);
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
}

//------------------------------------------------------------------------------
double CDiagTrace::getValRes( const CEnumScaleDir& i_scaleDir, double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir.enumeratorAsInt()];

    double fRes;

    if( m_arphysValArr[i_scaleDir.enumeratorAsInt()].hasRes() )
    {
        fRes = m_arphysValArr[i_scaleDir.enumeratorAsInt()].getRes().getVal(i_pUnit);
    }
    else if( m_arphysValRes[i_scaleDir.enumeratorAsInt()].isValid() )
    {
        fRes = m_arphysValRes[i_scaleDir.enumeratorAsInt()].getVal(i_pUnit);
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
double CDiagTrace::round2ValRes( const CEnumScaleDir& i_scaleDir, double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double fRes = getValRes(i_scaleDir, i_fVal, i_pUnit);
    double fVal = Math::round2Resolution(i_fVal, fRes);
    return fVal;
}

//------------------------------------------------------------------------------
void CDiagTrace::setValues( const CEnumScaleDir& i_scaleDir, const CPhysValArr& i_physValArr )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setValues",
        /* strAddInfo   */ "" );

    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir.enumeratorAsInt()];

    m_arphysValArr[i_scaleDir.enumeratorAsInt()] = i_physValArr;

    if( i_physValArr.unit().isValid() && pDiagScale->getScale().m_unit != i_physValArr.unit() )
    {
        m_arphysValArr[i_scaleDir.enumeratorAsInt()].convertValues(pDiagScale->getScale().m_unit);
    }

    invalidate(EUpdateData);

    if( m_pDiagram != nullptr )
    {
        m_pDiagram->traceChanged(this);
    }

} // setValues

//------------------------------------------------------------------------------
void CDiagTrace::setValues(
    const CEnumScaleDir&   i_scaleDir,
    const QVector<double>& i_arfValues,
    const CUnit*           i_pUnitVal,
    double                 i_fRes,
    CUnit*                 i_pUnitRes )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setValues",
        /* strAddInfo   */ "" );

    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[i_scaleDir.enumeratorAsInt()];
    CUnit  unitVal = i_pUnitVal == nullptr ? pDiagScale->getScale().m_unit : *i_pUnitVal;
    double fRes    = i_fRes;
    CUnit  unitRes = i_pUnitRes == nullptr ? unitVal : *i_pUnitRes;

    if( !areOfSameUnitGroup(m_arphysValArr[i_scaleDir.enumeratorAsInt()].unit(),unitVal) )
    {
        m_arphysValArr[i_scaleDir.enumeratorAsInt()].setUnit(unitVal);
    }

    if( fRes > 0.0 )
    {
        if( pDiagScale->getScale().m_unit != unitRes )
        {
            fRes = unitRes.convertValue(fRes, pDiagScale->getScale().m_unit);
        }
        m_arphysValArr[i_scaleDir.enumeratorAsInt()].setRes( CPhysValRes(fRes,pDiagScale->getScale().m_unit) );
    }

    m_arphysValArr[i_scaleDir.enumeratorAsInt()].setValues(0, i_arfValues, unitVal);

    if( pDiagScale->getScale().m_unit != unitVal )
    {
        m_arphysValArr[i_scaleDir.enumeratorAsInt()].convertValues(pDiagScale->getScale().m_unit);
    }
    m_arphysValArr[i_scaleDir.enumeratorAsInt()].setValidity(EValueValidity::Valid);

    invalidate(EUpdateData);

    if( m_pDiagram != nullptr )
    {
        m_pDiagram->traceChanged(this);
    }

} // setValues

//------------------------------------------------------------------------------
bool CDiagTrace::areValuesValid( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    bool bValuesValid = true;
    int  idxScaleMin;
    int  idxScaleMax;
    int  idxScale;

    if( !i_scaleDir.isValid() )
    {
        idxScaleMin = 0;
        idxScaleMax = CEnumScaleDir::count()-1;
    }
    else
    {
        idxScaleMin = i_scaleDir.enumeratorAsInt();
        idxScaleMax = i_scaleDir.enumeratorAsInt();
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
int CDiagTrace::getValCount( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    return m_arphysValArr[i_scaleDir.enumeratorAsInt()].size();
}

//------------------------------------------------------------------------------
QVector<double> CDiagTrace::getValues( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    return m_arphysValArr[i_scaleDir.enumeratorAsInt()].toDoubleVec(0,EArrayIndexCountAllElements);
}

//------------------------------------------------------------------------------
int CDiagTrace::getValPix( const CEnumScaleDir& i_scaleDir, double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getValPix(i_fVal,i_pUnit);
}

//------------------------------------------------------------------------------
QString CDiagTrace::getValString(
    const CEnumScaleDir& i_scaleDir,
    double i_fVal,
    const CUnit* i_pUnit,
    int i_iDigitsCountMax,
    bool i_bUseEngineeringFormat ) const
//------------------------------------------------------------------------------
{
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
    const CEnumScaleDir& i_scaleDirSrc,
    double               i_fValSrc,
    const CUnit*         i_pUnitSrc,
    const CEnumScaleDir& i_scaleDirDst,
    double*              o_pfValDst,
    CUnit*               i_pUnitDst,
    bool                 i_bRound2Res ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDirDst.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    if( m_arpDiagScale[i_scaleDirSrc.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    for( int idxScale = 0; idxScale < CEnumScaleDir::count(); idxScale++ )
    {
        if( !m_arphysValArr[idxScale].isValid() )
        {
            return EValueValidity::Invalid;
        }
    }

    QVector<double> arfValuesSrc = m_arphysValArr[i_scaleDirSrc.enumeratorAsInt()].toDoubleVec(0, EArrayIndexCountAllElements);
    QVector<double> arfValuesDst = m_arphysValArr[i_scaleDirDst.enumeratorAsInt()].toDoubleVec(0, EArrayIndexCountAllElements);

    if( arfValuesSrc.size() == 0 || arfValuesDst.size() == 0 )
    {
        return EValueValidity::Invalid;
    }

    double fValSrc = i_fValSrc;
    CUnit unitValuesSrc = m_arphysValArr[i_scaleDirSrc.enumeratorAsInt()].unit();
    if( i_pUnitSrc != nullptr && *i_pUnitSrc != unitValuesSrc )
    {
        fValSrc = i_pUnitSrc->convertValue(fValSrc, unitValuesSrc);
    }

    int iValCount = arfValuesSrc.size();
    if( arfValuesSrc.size() > arfValuesDst.size() )
    {
        iValCount = arfValuesDst.size();
    }

    EValueValidity validDst = EValueValidity::Invalid;
    double fValSrcPrev = 0.0;
    double fValSrcNext = 0.0;
    double fValDstPrev = 0.0;
    double fValDstNext = 0.0;
    double fResSrc = getValRes(i_scaleDirSrc, i_fValSrc, i_pUnitSrc);

    if( iValCount == 1 )
    {
        fValSrcPrev = arfValuesSrc[0];
        fValDstPrev = arfValuesDst[0];
        fValSrcNext = arfValuesSrc[0];
        fValDstNext = arfValuesDst[0];

        if ((fValSrc >= (arfValuesSrc[0]-fResSrc)) && (fValSrc <= (arfValuesSrc[0]+fResSrc)))
        {
            validDst = EValueValidity::Valid;
        }
    }
    else if( iValCount > 1 )
    {
        if( fValSrc >= (arfValuesSrc[0]-fResSrc/2.0) && fValSrc <= (arfValuesSrc[iValCount-1]+fResSrc/2.0) )
        {
            validDst = EValueValidity::Valid;

            const CDiagScale* pDiagScaleSrc = m_arpDiagScale[i_scaleDirSrc.enumeratorAsInt()];

            double fScaleMinSrc = arfValuesSrc[0];
            double fScaleMaxSrc = arfValuesSrc[iValCount-1];
            double fScaleRangeSrc = fScaleMaxSrc - fScaleMinSrc;
            double fValSrcTmp = fValSrc;

            if( pDiagScaleSrc->getSpacing() == ESpacing::Logarithmic )
            {
                if( fScaleMinSrc > 0.0 && fScaleMaxSrc > 0.0 && fValSrc > 0.0 )
                {
                    fValSrcTmp     = log10(fValSrc);
                    fScaleMinSrc   = log10(arfValuesSrc[0]);
                    fScaleMaxSrc   = log10(arfValuesSrc[iValCount-1]);
                    fScaleRangeSrc = fScaleMaxSrc - fScaleMinSrc;
                }
            }

            int idxVal = static_cast<int>( static_cast<double>(iValCount-1) * (fValSrcTmp - fScaleMinSrc) / fScaleRangeSrc );

            if( fValSrc > arfValuesSrc[idxVal] && idxVal >= 0 && idxVal < (iValCount-1) )
            {
                const double* pfValSrcPrev = &arfValuesSrc[idxVal];
                const double* pfValDstPrev = &arfValuesDst[idxVal];
                const double* pfValSrcNext = &arfValuesSrc[idxVal+1];
                const double* pfValDstNext = &arfValuesDst[idxVal+1];

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
                const double* pfValSrcPrev = &arfValuesSrc[idxVal-1];
                const double* pfValDstPrev = &arfValuesDst[idxVal-1];
                const double* pfValSrcNext = &arfValuesSrc[idxVal];
                const double* pfValDstNext = &arfValuesDst[idxVal];

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
        double fValDst = 0.0;

        if( fValSrcNext > fValSrcPrev )
        {
            double fDistSrc = fValSrcNext - fValSrcPrev;
            double fDistDst = fValDstNext - fValDstPrev;
            double fM = fDistDst / fDistSrc;
            double fT = fValDstPrev - fM * fValSrcPrev;
            fValDst = fM * fValSrc + fT;
        }
        else // if( fValSrcNext == fValSrcPrev )
        {
            fValDst = fValDstPrev;
        }

        double fResDst = getValRes(i_scaleDirDst, i_pUnitDst);
        if( fResDst != 0.0 && i_bRound2Res )
        {
            fValDst = Math::round2Resolution( fValDst, fabs(fResDst) );
        }

        const CDiagScale* pDiagScaleDst = m_arpDiagScale[i_scaleDirDst.enumeratorAsInt()];
        if( i_pUnitDst != nullptr && *i_pUnitDst != pDiagScaleDst->getScale().m_unit )
        {
            fValDst = pDiagScaleDst->getScale().m_unit.convertValue(fValDst, *i_pUnitDst);
        }
        *o_pfValDst = fValDst;
    }
    return EValueValidity::Valid;

} // getVal

//------------------------------------------------------------------------------
double CDiagTrace::getVal( const CEnumScaleDir& i_scaleDir, double i_fPix, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getVal(i_fPix,i_pUnit);
}

//------------------------------------------------------------------------------
void CDiagTrace::invalidate( unsigned int i_uUpdateFlags )
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

//------------------------------------------------------------------------------
void CDiagTrace::update( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = updateFlags2Str(i_uUpdateFlags);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strRuntimeInfo = "OldUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strRuntimeInfo);
    }

    // Layout processing is the task of the diagram:
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        validate(EUpdateLayout);
    }
    else if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        for( int idxScale = 0; idxScale < CEnumScaleDir::count(); idxScale++ )
        {
            CDiagScale* pDiagScale = m_arpDiagScale[idxScale];

            if( pDiagScale != nullptr )
            {
                bool bScaleValid = isScaleValid(static_cast<EScaleDir>(idxScale));

                if (pDiagScale->getScale().m_unit != m_arphysValArr[idxScale].unit())
                {
                    m_arphysValArr[idxScale].convertValues(pDiagScale->getScale().m_unit);
                }

                double fScaleRangePix = getScaleRangePix(static_cast<EScaleDir>(idxScale));
                double fScaleRangeVal = pDiagScale->getScale().m_fMax - pDiagScale->getScale().m_fMin;
                double fScaleRangeValLog = 0.0;

                if( bScaleValid && pDiagScale->getSpacing() == ESpacing::Logarithmic )
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
                    if( pDiagScale->getSpacing() == ESpacing::Linear )
                    {
                        if( fScaleRangePix > 1.0 )
                        {
                            fValRes = fScaleRangeVal / (fScaleRangePix-1.0);
                        }
                        else
                        {
                            fValRes = fScaleRangeVal / 1000.0;
                        }
                        if( idxScale == static_cast<int>(EScaleDir::X) )
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
                        if( idxScale == static_cast<int>(EScaleDir::X) )
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

                } // if( bScaleValid )

                m_arphysValRes[idxScale].setRes(fValRes);

            } // if( pDiagScale != nullptr )
        } // for( int idxScale = 0; idxScale < CEnumScaleDir::count(); idxScale++ )

        validate(EUpdateData);

        emit traceChanged(this);

    } // if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strRuntimeInfo = "NewUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strRuntimeInfo);
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

    const CDiagScale* pDiagScaleXSrc = m_arpDiagScale[static_cast<int>(EScaleDir::X)];
    const CDiagScale* pDiagScaleYSrc = m_arpDiagScale[static_cast<int>(EScaleDir::Y)];

    if( pDiagScaleXSrc == nullptr || pDiagScaleYSrc == nullptr )
    {
        return nullptr;
    }

    CDiagScale* pDiagScaleX = i_pDiagramTrg->findDiagScale(pDiagScaleXSrc->getObjName());
    CDiagScale* pDiagScaleY = i_pDiagramTrg->findDiagScale(pDiagScaleYSrc->getObjName());

    if( pDiagScaleX == nullptr || pDiagScaleY == nullptr )
    {
        return nullptr;
    }

    CDiagTrace* pDiagTrace = new CDiagTrace(
        /* strObjName  */ m_strObjName,
        /* pDiagScaleX */ pDiagScaleX,
        /* pDiagScaleY */ pDiagScaleY );

    int idxScale;

    for( idxScale = 0; idxScale < CEnumScaleDir::count(); idxScale++ )
    {
        pDiagTrace->m_arphysValArr[idxScale] = m_arphysValArr[idxScale];
    }
    i_pDiagramTrg->addDiagTrace(pDiagTrace);

    return pDiagTrace;

} // clone
