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

#include "ZSPhysVal/ZSPhysValRes.h"
//#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CPhysValRes
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unit()
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_unit()
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_resType(i_physValRes.m_resType),
    m_fVal(i_physValRes.m_fVal),
    m_unit(i_physValRes.m_unit)
{
} // copy ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unit(i_unit)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysValRes::~CPhysValRes()
//------------------------------------------------------------------------------
{
    m_resType = static_cast<EResType>(0);
    m_fVal = 0.0;
    //m_unit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValRes::isValid() const
//------------------------------------------------------------------------------
{
    return (m_fVal != 0.0);
}

//------------------------------------------------------------------------------
EResType CPhysValRes::type() const
//------------------------------------------------------------------------------
{
    return m_resType;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
CUnit CPhysValRes::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_unit = CUnit();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::setVal( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fVal = i_fVal;
}

//------------------------------------------------------------------------------
void CPhysValRes::setVal( double i_fVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    setUnit(i_unit);
    setVal(i_fVal);
}

//------------------------------------------------------------------------------
TFormatResult CPhysValRes::setVal( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    bool bValOk = false;
    double fVal = 0.0;
    CUnit unitVal = m_unit;

    TFormatResult formatResult = parseValStr(
        /* strVal   */ i_strVal,
        /* pbValOk  */ &bValOk,
        /* pfVal    */ &fVal,
        /* pUnitVal */ &unitVal,
        /* pbResOk  */ nullptr,
        /* pfRes    */ nullptr,
        /* pUnitRes */ nullptr );

    if( !(formatResult & FormatResult::Error) && bValOk )
    {
        m_fVal = fVal;
        m_unit = unitVal;
    }
    return formatResult;

} // setVal

//------------------------------------------------------------------------------
TFormatResult CPhysValRes::setVal( const QString& i_strVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    setUnit(i_unit);
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
double CPhysValRes::getVal() const
//------------------------------------------------------------------------------
{
    return m_fVal;
}

//------------------------------------------------------------------------------
double CPhysValRes::getVal( const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    double fVal = m_fVal;

    if( !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException(
            __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( i_unit != m_unit )
    {
        fVal = m_unit.convertValue(fVal, i_unit);
    }
    return fVal;
}

/*==============================================================================
public: // instance methods (to convert the value into a string)
==============================================================================*/

//------------------------------------------------------------------------------
QString CPhysValRes::toString( EUnitFind i_unitFind, int i_iSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    TFormatResult formatResult;
    QString strVal;
    const CUnit* pUnit = &m_unit;

    if( i_unitFind == EUnitFind::Best )
    {
        formatResult = formatValue(
            /* fVal                       */ m_fVal,
            /* pUnitVal                   */ &m_unit,
            /* iDigitsMantissaMax         */ -1,
            /* bDigitsAccuracyLimitsMant. */ true,
            /* iDigitsAccuracy            */ 2,
            /* iDigitsExponent            */ 0,
            /* bUseEngineeringFormat      */ false,
            /* pfVal                      */ nullptr,
            /* pstr                       */ &strVal,
            /* ppUnitVal                  */ &pUnit );
        if( formatResult != FormatResult::Ok )
        {
            strVal = "";
        }
        removeTrailingZeros(&strVal);
    }
    else
    {
        formatResult = formatValue(
            /* fVal                       */ m_fVal,
            /* pUnitVal                   */ &m_unit,
            /* iDigitsMantissaMax         */ -1,
            /* bDigitsAccuracyLimitsMant. */ true,
            /* iDigitsAccuracy            */ 2,
            /* iDigitsExponent            */ 0,
            /* bUseEngineeringFormat      */ false,
            /* pfVal                      */ nullptr,
            /* pstrVal                    */ &strVal );
        if( formatResult != FormatResult::Ok )
        {
            strVal = "";
        }
        removeTrailingZeros(&strVal);
    }

    if( !(i_iSubStrVisibility & PhysValSubStr::Val) )
    {
        strVal = "";
    }

    QString strUnitGrp;
    QString strUnit;

    if( i_iSubStrVisibility & PhysValSubStr::UnitMask )
    {
        strVal += " ";
    }
    if( i_iSubStrVisibility & PhysValSubStr::UnitGrp )
    {
        strUnitGrp = pUnit->groupPath();
    }
    if( i_iSubStrVisibility & PhysValSubStr::UnitSymbol )
    {
        strUnit = pUnit->symbol();
    }
    else if( i_iSubStrVisibility & PhysValSubStr::UnitName )
    {
        strUnit = pUnit->unitName();
    }
    if( !strUnitGrp.isEmpty() )
    {
        strVal += strUnitGrp;
        if( !strUnit.isEmpty() )
        {
            strVal += pUnit->nodeSeparator();
        }
    }
    if( !strUnit.isEmpty() )
    {
        strVal += strUnit;
    }
    return strVal;

} // toString

//------------------------------------------------------------------------------
QString CPhysValRes::toString( const CUnit& i_unit, int i_iSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    QString strVal;

    if( i_unit == m_unit )
    {
        strVal = toString(EUnitFind::None, i_iSubStrVisibility);
    }
    else
    {
        double fVal = getVal(i_unit);

        TFormatResult formatResult = formatValue(
            /* fVal                       */ fVal,
            /* pUnitVal                   */ &i_unit,
            /* iDigitsMantissaMax         */ -1,
            /* bDigitsAccuracyLimitsMant. */ true,
            /* iDigitsAccuracy            */ 2,
            /* iDigitsExponent            */ 0,
            /* bUseEngineeringFormat      */ false,
            /* pfVal                      */ nullptr,
            /* pstrVal                    */ &strVal );
        if( formatResult != FormatResult::Ok )
        {
            strVal = "";
        }
        removeTrailingZeros(&strVal);

        if( !(i_iSubStrVisibility & PhysValSubStr::Val) )
        {
            strVal = "";
        }

        QString strUnitGrp;
        QString strUnit;

        if( i_iSubStrVisibility & PhysValSubStr::UnitMask )
        {
            strVal += " ";
        }
        if( i_iSubStrVisibility & PhysValSubStr::UnitGrp )
        {
            strUnitGrp = i_unit.groupPath();
        }
        if( i_iSubStrVisibility & PhysValSubStr::UnitSymbol )
        {
            strUnit = i_unit.symbol();
        }
        else if( i_iSubStrVisibility & PhysValSubStr::UnitName )
        {
            strUnit = i_unit.unitName();
        }
        if( !strUnitGrp.isEmpty() )
        {
            strVal += strUnitGrp;
            if( !strUnit.isEmpty() )
            {
                strVal += i_unit.nodeSeparator();
            }
        }
        if( !strUnit.isEmpty() )
        {
            strVal += strUnit;
        }
    }
    return strVal;

} // toString

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValRes::operator == ( const CPhysValRes& i_physValResOther ) const
//------------------------------------------------------------------------------
{
    if( m_resType != i_physValResOther.m_resType )
    {
        return false;
    }

    double fValOther;

    if( m_unit.isValid() )
    {
        fValOther = i_physValResOther.getVal(m_unit);
    }
    else
    {
        fValOther = i_physValResOther.getVal();
    }
    if( m_fVal == fValOther )
    {
        return true;
    }
    return false;

} // operator ==

//------------------------------------------------------------------------------
bool CPhysValRes::operator != ( const CPhysValRes& i_physValResOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValResOther);
}

//------------------------------------------------------------------------------
CPhysValRes& CPhysValRes::operator = ( const CPhysValRes& i_physValResNew )
//------------------------------------------------------------------------------
{
    m_resType = i_physValResNew.m_resType;
    m_fVal    = i_physValResNew.m_fVal;
    m_unit    = i_physValResNew.m_unit;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValRes CPhysValRes::operator + ( const CPhysValRes& i_physValResOp ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValResOp.unit()) )
    {
        CPhysValRes physValResThis(*this);
        CPhysValRes physValResOther(i_physValResOp);
        QString     strAddErrInfo = physValResThis.toString() + " + " + physValResOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    CPhysValRes physValResSum(*this);
    physValResSum += i_physValResOp;
    return physValResSum;
}

//------------------------------------------------------------------------------
CPhysValRes& CPhysValRes::operator += ( const CPhysValRes& i_physValResOp )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValResOp.unit()) )
    {
        CPhysValRes physValResThis(*this);
        CPhysValRes physValResOther(i_physValResOp);
        QString     strAddErrInfo = physValResThis.toString() + " += " + physValResOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    // Same unit group, so both units are either nullptr or not.
    if( !m_unit.isValid() )
    {
        m_fVal += i_physValResOp.getVal();
    }
    else
    {
        CUnit unitOp = i_physValResOp.unit();

        if( m_unit.isLogarithmic() && !unitOp.isLogarithmic() )
        {
            QString strAddErrInfo = "Val:" + m_unit.keyInTree() + ", Op:" + unitOp.keyInTree();
            throw CUnitConversionException(
                __FILE__, __LINE__, EResultMixedLinLogInMathOp, strAddErrInfo);
        }
        if( unitOp.isLogarithmic() )
        {
            double fFactor = pow( 10.0, i_physValResOp.getVal()/unitOp.logarithmicFactor() );
            m_fVal *= fFactor;
        }
        else
        {
            m_fVal += i_physValResOp.getVal(m_unit);
        }
    }
    return *this;

} // operator +=

//------------------------------------------------------------------------------
CPhysValRes CPhysValRes::operator * ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    CPhysValRes physValResMul(*this);
    physValResMul.m_fVal *= i_fOp;
    return physValResMul;
}

//------------------------------------------------------------------------------
CPhysValRes& CPhysValRes::operator *= ( double i_fOp )
//------------------------------------------------------------------------------
{
    m_fVal *= i_fOp;
    return *this;
}
