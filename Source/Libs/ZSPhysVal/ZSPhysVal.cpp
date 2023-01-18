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

#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSPhysVal/ZSPhysUnit.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
Exported methods
*******************************************************************************/

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::minPhysVal( const QVector<CPhysVal>& i_arphysval )
//------------------------------------------------------------------------------
{
    if( i_arphysval.size() < 2 )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_arphysval.size() == " + QString::number(i_arphysval.size()) );
    }

    CPhysVal physvalMin = i_arphysval[0];

    for( int iValIdx = 1; iValIdx < i_arphysval.size(); iValIdx++ )
    {
        CPhysVal physval = i_arphysval[iValIdx];
        if( physval < physvalMin )
        {
            physvalMin = physval;
        }
    }
    return physvalMin;
}

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;
    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    return minPhysVal(arphysVal);
}

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;
    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    arphysVal.append(i_physval3);
    return minPhysVal(arphysVal);
}

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3, const CPhysVal& i_physval4 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;
    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    arphysVal.append(i_physval3);
    arphysVal.append(i_physval4);
    return minPhysVal(arphysVal);
}

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::maxPhysVal( const QVector<CPhysVal>& i_arphysval )
//------------------------------------------------------------------------------
{
    if( i_arphysval.size() < 2 )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_arphysval.size() == " + QString::number(i_arphysval.size()) );
    }

    CPhysVal physvalMax = i_arphysval[0];
    for( int iValIdx = 1; iValIdx < i_arphysval.size(); iValIdx++ )
    {
        CPhysVal physval = i_arphysval[iValIdx];
        if( physval > physvalMax )
        {
            physvalMax = physval;
        }
    }
    return physvalMax;
}

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;
    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    return maxPhysVal(arphysVal);
}

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;
    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    arphysVal.append(i_physval3);
    return maxPhysVal(arphysVal);
}

//------------------------------------------------------------------------------
CPhysVal ZS::PhysVal::maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3, const CPhysVal& i_physval4 )
//------------------------------------------------------------------------------
{
    QVector<CPhysVal> arphysVal;
    arphysVal.append(i_physval1);
    arphysVal.append(i_physval2);
    arphysVal.append(i_physval3);
    arphysVal.append(i_physval4);
    return maxPhysVal(arphysVal);
}


/*******************************************************************************
struct SValueFormatProvider
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider() :
//------------------------------------------------------------------------------
    m_pUnitVal(nullptr),
    m_unitFindVal(EUnitFind::None),
    m_iValSubStrVisibility(PhysValSubStr::UnitSymbol),
    m_iDigitsMantissa(0),
    m_iDigitsExponent(0),
    m_bUseEngineeringFormat(false),
    m_iDigitsPerDigitGroup(0),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(0.0),
    m_pUnitRes(nullptr),
    m_resType(EResType::Undefined),
    m_unitFindRes(EUnitFind::None),
    m_iResSubStrVisibility(PhysValSubStr::None),
    m_iDigitsAccuracy(0),
    m_bDigitsAccuracyLimitsMantissa(false)
{
} // default ctor

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider( const SValueFormatProvider& i_other ) :
//------------------------------------------------------------------------------
    m_pUnitVal(i_other.m_pUnitVal),
    m_unitFindVal(i_other.m_unitFindVal),
    m_iValSubStrVisibility(i_other.m_iValSubStrVisibility),
    m_iDigitsMantissa(i_other.m_iDigitsMantissa),
    m_iDigitsExponent(i_other.m_iDigitsExponent),
    m_bUseEngineeringFormat(i_other.m_bUseEngineeringFormat),
    m_iDigitsPerDigitGroup(i_other.m_iDigitsPerDigitGroup),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(i_other.m_fRes),
    m_pUnitRes(i_other.m_pUnitRes),
    m_resType(i_other.m_resType),
    m_unitFindRes(i_other.m_unitFindRes),
    m_iResSubStrVisibility(i_other.m_iResSubStrVisibility),
    m_iDigitsAccuracy(i_other.m_iDigitsAccuracy),
    m_bDigitsAccuracyLimitsMantissa(i_other.m_bDigitsAccuracyLimitsMantissa)
{
    if( i_other.m_pstrDigitsGroupDelimiter != nullptr )
    {
        m_pstrDigitsGroupDelimiter = new QString(*i_other.m_pstrDigitsGroupDelimiter);
    }
    if( i_other.m_pstrDecimalPoint != nullptr )
    {
        m_pstrDecimalPoint = new QString(*i_other.m_pstrDecimalPoint);
    }
} // copy ctor

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider(
    CUnit*         i_pUnitVal,    // nullptr to indicate the value with the current or the "best unit".
    EUnitFind      i_unitFindVal, // Only evaluated if pUnit == nullptr
    int            i_iValSubStrVisibility,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    bool           i_bUseEngineeringFormat,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint ) :
//------------------------------------------------------------------------------
    m_pUnitVal(i_pUnitVal),
    m_unitFindVal(i_unitFindVal),
    m_iValSubStrVisibility(i_iValSubStrVisibility),
    m_iDigitsMantissa(i_iDigitsMantissa),
    m_iDigitsExponent(i_iDigitsExponent),
    m_bUseEngineeringFormat(i_bUseEngineeringFormat),
    m_iDigitsPerDigitGroup(i_iDigitsPerDigitGroup),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(0.0),
    m_pUnitRes(nullptr),
    m_resType(EResType::Undefined),
    m_unitFindRes(EUnitFind::None),
    m_iResSubStrVisibility(PhysValSubStr::None),
    m_iDigitsAccuracy(0),
    m_bDigitsAccuracyLimitsMantissa(false)
{
    if( i_pstrDigitsGroupDelimiter != nullptr )
    {
        m_pstrDigitsGroupDelimiter = new QString(*i_pstrDigitsGroupDelimiter);
    }
    if( i_pstrDecimalPoint != nullptr )
    {
        m_pstrDecimalPoint = new QString(*i_pstrDecimalPoint);
    }
}

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider(
    CUnit*         i_pUnitVal,    // nullptr to indicate the value with the current or the "best unit".
    EUnitFind      i_unitFindVal, // Only evaluated if pUnit == nullptr
    int            i_iValSubStrVisibility,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    bool           i_bUseEngineeringFormat,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint,
    double         i_fRes,        // If set to != 0.0 this resolution is used instead of the physical values current resolution
    CUnit*         i_pUnitRes,    // nullptr to indicate the value with the current or the "best unit".
    EResType       i_resType,
    EUnitFind      i_unitFindRes, // Only evaluated if pUnit == nullptr
    int            i_iResSubStrVisibility ) :
//------------------------------------------------------------------------------
    m_pUnitVal(i_pUnitVal),
    m_unitFindVal(i_unitFindVal),
    m_iValSubStrVisibility(i_iValSubStrVisibility),
    m_iDigitsMantissa(i_iDigitsMantissa),
    m_iDigitsExponent(i_iDigitsExponent),
    m_bUseEngineeringFormat(i_bUseEngineeringFormat),
    m_iDigitsPerDigitGroup(i_iDigitsPerDigitGroup),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(i_fRes),
    m_pUnitRes(i_pUnitRes),
    m_resType(i_resType),
    m_unitFindRes(i_unitFindRes),
    m_iResSubStrVisibility(i_iResSubStrVisibility),
    m_iDigitsAccuracy(0),
    m_bDigitsAccuracyLimitsMantissa(false)
{
    if( i_pstrDigitsGroupDelimiter != nullptr )
    {
        m_pstrDigitsGroupDelimiter = new QString(*i_pstrDigitsGroupDelimiter);
    }
    if( i_pstrDecimalPoint != nullptr )
    {
        m_pstrDecimalPoint = new QString(*i_pstrDecimalPoint);
    }
}

//------------------------------------------------------------------------------
SValueFormatProvider::SValueFormatProvider(
    CUnit*         i_pUnitVal,    // nullptr to indicate the value with the current or the "best unit".
    EUnitFind      i_unitFindVal, // Only evaluated if pUnit == nullptr
    int            i_iValSubStrVisibility,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    bool           i_bUseEngineeringFormat,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint,
    int            i_iDigitsAccuracy,
    bool           i_bDigitsAccuracyLimitsMantissa ) :
//------------------------------------------------------------------------------
    m_pUnitVal(i_pUnitVal),
    m_unitFindVal(i_unitFindVal),
    m_iValSubStrVisibility(i_iValSubStrVisibility),
    m_iDigitsMantissa(i_iDigitsMantissa),
    m_iDigitsExponent(i_iDigitsExponent),
    m_bUseEngineeringFormat(i_bUseEngineeringFormat),
    m_iDigitsPerDigitGroup(i_iDigitsPerDigitGroup),
    m_pstrDigitsGroupDelimiter(nullptr),
    m_pstrDecimalPoint(nullptr),
    m_fRes(0.0),
    m_pUnitRes(nullptr),
    m_resType(EResType::Undefined),
    m_unitFindRes(EUnitFind::None),
    m_iResSubStrVisibility(PhysValSubStr::None),
    m_iDigitsAccuracy(i_iDigitsAccuracy),
    m_bDigitsAccuracyLimitsMantissa(i_bDigitsAccuracyLimitsMantissa)
{
    if( i_pstrDigitsGroupDelimiter != nullptr )
    {
        m_pstrDigitsGroupDelimiter = new QString(*i_pstrDigitsGroupDelimiter);
    }
    if( i_pstrDecimalPoint != nullptr )
    {
        m_pstrDecimalPoint = new QString(*i_pstrDecimalPoint);
    }
}

//------------------------------------------------------------------------------
SValueFormatProvider::~SValueFormatProvider()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pstrDigitsGroupDelimiter;
    }
    catch(...)
    {
    }
    m_pstrDigitsGroupDelimiter = nullptr;

    try
    {
        delete m_pstrDecimalPoint;
    }
    catch(...)
    {
    }
    m_pstrDecimalPoint = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SValueFormatProvider& SValueFormatProvider::operator = ( const SValueFormatProvider& i_other )
//------------------------------------------------------------------------------
{
    if( &i_other != this )
    {
        delete m_pstrDigitsGroupDelimiter;
        m_pstrDigitsGroupDelimiter = nullptr;
        delete m_pstrDecimalPoint;
        m_pstrDecimalPoint = nullptr;

        m_pUnitVal = i_other.m_pUnitVal;
        m_unitFindVal = i_other.m_unitFindVal;
        m_iValSubStrVisibility = i_other.m_iValSubStrVisibility;
        m_iDigitsMantissa = i_other.m_iDigitsMantissa;
        m_iDigitsExponent = i_other.m_iDigitsExponent;
        m_bUseEngineeringFormat = i_other.m_bUseEngineeringFormat;
        m_iDigitsPerDigitGroup = i_other.m_iDigitsPerDigitGroup;
        m_fRes = i_other.m_fRes;
        m_pUnitRes = i_other.m_pUnitRes;
        m_resType = i_other.m_resType;
        m_unitFindRes = i_other.m_unitFindRes;
        m_iResSubStrVisibility = i_other.m_iResSubStrVisibility;
        m_iDigitsAccuracy = i_other.m_iDigitsAccuracy;
        m_bDigitsAccuracyLimitsMantissa = i_other.m_bDigitsAccuracyLimitsMantissa;

        if( i_other.m_pstrDigitsGroupDelimiter != nullptr )
        {
            m_pstrDigitsGroupDelimiter = new QString(*i_other.m_pstrDigitsGroupDelimiter);
        }
        if( i_other.m_pstrDecimalPoint != nullptr )
        {
            m_pstrDecimalPoint = new QString(*i_other.m_pstrDecimalPoint);
        }
    }

    return *this;

} // operator =

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SValueFormatProvider::hasRes() const
//------------------------------------------------------------------------------
{
    if( m_fRes > 0.0 || m_iDigitsAccuracy > 0 )
    {
        return true;
    }
    return false;
}


/*******************************************************************************
class CPhysVal
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_unit, i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const CUnit& i_unitVal, const CUnit& i_unitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unitVal),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_unitRes, i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal, i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( double i_fVal, const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_unit, i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    double i_fVal, const CUnit& i_unitVal,
    const CUnit& i_unitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unitVal),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_unitRes, i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    double i_fVal, const CUnit& i_unit,
    double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal, i_unit, i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    double i_fVal, const CUnit& i_unitVal,
    double i_fResVal, const CUnit& i_unitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unitVal),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_fResVal, i_unitRes, i_resType)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    double i_fVal, const CUnit& i_unitVal,
    const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_unit(i_unitVal),
    m_validity(EValueValidity::Valid),
    m_fVal(i_fVal),
    m_physValRes(i_physValRes)
{
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_resType)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_unit, i_resType)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const QString& i_strVal, double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal, i_resType)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    const QString& i_strVal, const CUnit& i_unitVal,
    const CUnit& i_unitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unitVal),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_unitRes, i_resType)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    const QString& i_strVal,
    double i_fResVal, const CUnit& i_unitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal, i_unitRes, i_resType)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    const QString& i_strVal, const CUnit& i_unit,
    double i_fResVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal, i_unit, i_resType)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    const QString& i_strVal, const CUnit& i_unitVal,
    double i_fResVal, const CUnit& i_unitRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unitVal),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_fResVal, i_unitRes, i_resType)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal(
    const QString& i_strVal, const CUnit& i_unitVal,
    const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_validity(EValueValidity::Invalid),
    m_fVal(0.0),
    m_physValRes(i_physValRes)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysVal::CPhysVal( const CPhysVal& i_physVal ) :
//------------------------------------------------------------------------------
    m_unit(i_physVal.m_unit),
    m_validity(i_physVal.m_validity),
    m_fVal(i_physVal.m_fVal),
    m_physValRes(i_physVal.m_physValRes)
{
} // copy ctor

//------------------------------------------------------------------------------
CPhysVal::~CPhysVal()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysVal::operator == ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_validity != i_physValOther.m_validity )
    {
        bEqual = false;
    }
    else
    {
        double fValOther = i_physValOther.getVal();
        double fValThis  = getVal(i_physValOther.m_unit);

        //if( i_physValOther.m_unit == nullptr )
        //{
        //    fValThis = getVal();
        //}
        //else
        //{
        //    fValThis = getVal(i_physValOther.m_unit);
        //}

        if( fValThis != fValOther )
        {
            bEqual = false;
        }
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CPhysVal::operator != ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator < ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValOther.m_unit) )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOther);
        QString strAddErrInfo = physValThis.toString() + " < " + physValOther.toString();
        throw CUnitConversionException(
            __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo);
    }

    double fValOther = i_physValOther.getVal();
    double fValThis  = getVal(i_physValOther.m_unit);

    if( fValThis < fValOther )
    {
        return true;
    }
    return false;

} // operator <

//------------------------------------------------------------------------------
bool CPhysVal::operator > ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValOther.m_unit) )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOther);
        QString strAddErrInfo = physValThis.toString() + " > " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    double fValOther = i_physValOther.getVal();
    double fValThis  = getVal(i_physValOther.m_unit);

    if( fValThis > fValOther )
    {
        return true;
    }
    return false;

} // operator >

//------------------------------------------------------------------------------
bool CPhysVal::operator <= ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValOther.m_unit) )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOther);
        QString strAddErrInfo = physValThis.toString() + " <= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    double fValOther = i_physValOther.getVal();
    double fValThis  = getVal(i_physValOther.m_unit);

    if( fValThis <= fValOther )
    {
        return true;
    }
    return false;

} // operator <=

//------------------------------------------------------------------------------
bool CPhysVal::operator >= ( const CPhysVal& i_physValOther ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValOther.m_unit) )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOther);
        QString strAddErrInfo = physValThis.toString() + " >= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    double fValOther = i_physValOther.getVal();
    double fValThis  = getVal(i_physValOther.m_unit);

    if( fValThis >= fValOther )
    {
        return true;
    }
    return false;

} // operator >=

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator = ( const CPhysVal& i_physValNew )
//------------------------------------------------------------------------------
{
    m_unit       = i_physValNew.m_unit;
    m_validity   = i_physValNew.m_validity;
    m_fVal       = i_physValNew.m_fVal;
    m_physValRes = i_physValNew.m_physValRes;
    return *this;
}

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator + ( const CPhysVal& i_physValOp ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValOp.m_unit) )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOp);
        QString strAddErrInfo = physValThis.toString() + " + " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysVal physValSum(*this);

    physValSum += i_physValOp;

    return physValSum;

} // operator +

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator += ( const CPhysVal& i_physValOp )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValOp.m_unit) )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOp);
        QString strAddErrInfo = physValThis.toString() + " += " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    // Same unit group, so both units are either nullptr or not.
    if( !m_unit.isValid() )
    {
        m_fVal += i_physValOp.getVal();

        // TODO: operator for resolution missing ...
        if( m_physValRes.isValid() )
        {
            m_physValRes += i_physValOp.m_physValRes;
        }
    }
    else
    {
        CUnit unitOp = i_physValOp.unit();

        if( m_unit.isLogarithmic() && !unitOp.isLogarithmic() )
        {
            QString strAddErrInfo = "Val:" + m_unit.keyInTree() + ", Res:" + unitOp.keyInTree();
            throw CUnitConversionException( __FILE__, __LINE__, EResultMixedLinLogInMathOp, strAddErrInfo );
        }
        if( unitOp.isLogarithmic() )
        {
            double fFactor = pow( 10.0, i_physValOp.getVal()/unitOp.logarithmicFactor() );

            m_fVal *= fFactor;

            if( m_physValRes.isValid() )
            {
                m_physValRes = m_physValRes * fFactor;
            }
        }
        else
        {
            m_fVal += i_physValOp.getVal(m_unit);

            if( m_physValRes.isValid() )
            {
                m_physValRes += m_physValRes;
            }
        }
    }
    return *this;

} // operator +=

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator - ( const CPhysVal& i_physValOp ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValOp.m_unit) )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOp);
        QString  strAddErrInfo = physValThis.toString() + " - " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysVal physValSum(*this);

    physValSum -= i_physValOp;

    return physValSum;

} // operator -

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator -= ( const CPhysVal& i_physValOp )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValOp.m_unit) )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_physValOp);
        QString  strAddErrInfo = physValThis.toString() + " - " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    // Same unit group, so both units are either nullptr or not.
    if( !m_unit.isValid() )
    {
        m_fVal -= i_physValOp.getVal();

        if( m_physValRes.isValid() )
        {
            m_physValRes += i_physValOp.m_physValRes;
        }
    }
    else
    {
        CUnit unitOp = i_physValOp.unit();

        if( m_unit.isLogarithmic() && !unitOp.isLogarithmic() )
        {
            QString strAddErrInfo = "Val:" + m_unit.keyInTree() + ", Res:" + unitOp.keyInTree();
            throw CUnitConversionException( __FILE__, __LINE__, EResultMixedLinLogInMathOp, strAddErrInfo );
        }
        if( unitOp.isLogarithmic() )
        {
            double fFactor = pow( 10.0, i_physValOp.getVal()/unitOp.logarithmicFactor() );

            m_fVal /= fFactor;

            if( m_physValRes.isValid() )
            {
                m_physValRes = m_physValRes * fFactor;
            }
        }
        else
        {
            m_fVal -= i_physValOp.getVal(m_unit);

            if( m_physValRes.isValid() )
            {
                m_physValRes += m_physValRes;
            }
        }
    }
    return *this;

} // operator -=

//------------------------------------------------------------------------------
bool CPhysVal::operator == ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( !m_unit.isValid() && m_fVal == i_fValOther )
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool CPhysVal::operator != ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_fValOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator < ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:NoUnit";
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_fVal < i_fValOther )
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool CPhysVal::operator > ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:NoUnit";
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_fVal > i_fValOther )
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool CPhysVal::operator <= ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fValOther);
        QString  strAddErrInfo = physValThis.toString() + " <= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_fVal <= i_fValOther )
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool CPhysVal::operator >= ( double i_fValOther ) const
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fValOther);
        QString  strAddErrInfo = physValThis.toString() + " >= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_fVal >= i_fValOther )
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator = ( const double i_fValNew )
//------------------------------------------------------------------------------
{
    setVal(i_fValNew);
    return *this;
}

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator + ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fOp);
        QString  strAddErrInfo = physValThis.toString() + " + " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    CPhysVal physValSum(*this);
    physValSum.m_fVal += i_fOp;
    return physValSum;
}

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator += ( double i_fOp )
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fOp);
        QString  strAddErrInfo = physValThis.toString() + " += " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    m_fVal += i_fOp;
    return *this;
}

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator - ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fOp);
        QString  strAddErrInfo = physValThis.toString() + " - " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    CPhysVal physValSum(*this);
    physValSum.m_fVal -= i_fOp;
    return physValSum;
}

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator -= ( double i_fOp )
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() )
    {
        CPhysVal physValThis(*this);
        CPhysVal physValOther(i_fOp);
        QString  strAddErrInfo = physValThis.toString() + " -= " + physValOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    m_fVal -= i_fOp;
    return *this;
}

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator * ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValMul(*this);
    physValMul.m_fVal *= i_fOp;
    return physValMul;
}

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator *= ( double i_fOp )
//------------------------------------------------------------------------------
{
    m_fVal *= i_fOp;
    return *this;
}

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator / ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValDif(*this);
    physValDif.m_fVal /= i_fOp;
    return physValDif;
}

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator /= ( double i_fOp )
//------------------------------------------------------------------------------
{
    m_fVal /= i_fOp;
    return *this;
}

//------------------------------------------------------------------------------
bool CPhysVal::operator == ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this == physValOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator != ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_valOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator < ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this < physValOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator > ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this > physValOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator <= ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this <= physValOther);
}

//------------------------------------------------------------------------------
bool CPhysVal::operator >= ( const QString& i_valOther ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValOther(*this);
    physValOther.setVal(i_valOther);
    return (*this >= physValOther);
}

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator = ( const QString& i_valNew )
//------------------------------------------------------------------------------
{
    setVal(i_valNew);
    return *this;
}

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator + ( const QString& i_valOp ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValSum(*this);
    CPhysVal physValOp(i_valOp, m_unit);
    physValSum += physValOp;
    return physValSum;
}

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator += ( const QString& i_valOp )
//------------------------------------------------------------------------------
{
    CPhysVal physValOp(i_valOp, m_unit);
    *this += physValOp;
    return *this;
}

//------------------------------------------------------------------------------
CPhysVal CPhysVal::operator - ( const QString& i_valOp ) const
//------------------------------------------------------------------------------
{
    CPhysVal physValSum(*this);
    CPhysVal physValOp(i_valOp, m_unit);
    physValSum -= physValOp;
    return physValSum;
}

//------------------------------------------------------------------------------
CPhysVal& CPhysVal::operator -= ( const QString& i_valOp )
//------------------------------------------------------------------------------
{
    CPhysVal physValOp(i_valOp, m_unit);
    *this -= physValOp;
    return *this;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::setValidity( EValueValidity i_validity )
//------------------------------------------------------------------------------
{
    m_validity = i_validity;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
    m_physValRes.setUnit(i_unit);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_unit = CUnit();
    m_physValRes.invalidateObjectReferences();
}

/*==============================================================================
public: // instance methods (to set the value)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::setVal( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fVal = i_fVal;
    m_validity = EValueValidity::Valid;
}

//------------------------------------------------------------------------------
void CPhysVal::setVal( double i_fVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( i_unit.isValid() )
    {
        setUnit(i_unit);
    }
    setVal(i_fVal);
}

/*==============================================================================
public: // instance methods (to set the value)
==============================================================================*/

//------------------------------------------------------------------------------
TFormatResult CPhysVal::setVal( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    bool   bValOk = false;
    double fVal = 0.0;
    CUnit  unitVal = m_unit;
    bool   bResOk = false;
    double fResVal = 0.0;
    CUnit  unitRes = m_physValRes.unit();

    TFormatResult formatResult = parseValStr(
        /* strVal   */ i_strVal,
        /* pbValOk  */ &bValOk,
        /* pfVal    */ &fVal,
        /* pUnitVal */ &unitVal,
        /* pbResOk  */ &bResOk,
        /* pfRes    */ &fResVal,
        /* pUnitRes */ &unitRes );

    if( !(formatResult & FormatResult::Error) && bValOk )
    {
        m_validity = EValueValidity::Valid;
        m_fVal = fVal;
        m_unit = unitVal;

        if( bResOk && fResVal != 0.0 )
        {
            if( !unitRes.isValid() )
            {
                m_physValRes.setVal(fResVal);
            }
            else if( unitVal.isValid() && (unitVal.isLogarithmic() != unitRes.isLogarithmic()) )
            {
                QString strAddErrInfo = "Val:" + unitVal.keyInTree() + ", Res:" + unitRes.keyInTree();
                throw CUnitConversionException(
                    __FILE__, __LINE__, EResultMixedLinLogInValAndRes, strAddErrInfo );
            }
            else
            {
                m_physValRes.setVal(fResVal, unitRes);
            }
        }
    }

    return formatResult;

} // setVal(QString)

//------------------------------------------------------------------------------
TFormatResult CPhysVal::setVal( const QString& i_strVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( i_unit.isValid() )
    {
        setUnit(i_unit);
    }
    return setVal(i_strVal);
}

/*==============================================================================
public: // instance methods (to get the value as double)
==============================================================================*/

//------------------------------------------------------------------------------
double CPhysVal::getVal( const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    double fVal = m_fVal;
    if( i_unit.isValid() )
    {
        if( !areOfSameUnitGroup(m_unit,i_unit) )
        {
            QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
            throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
        }
    }
    if( i_unit.isValid() && m_unit.isValid() && i_unit != m_unit )
    {
        fVal = m_unit.convertValue(fVal, i_unit);
    }
    return fVal;
}

/*==============================================================================
public: // instance methods (to convert the value into a string)
==============================================================================*/

//------------------------------------------------------------------------------
QString CPhysVal::toString(
    EUnitFind i_unitFindVal,
    int       i_iValSubStrVisibility,
    EUnitFind i_unitFindRes,
    int       i_iResSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat;
    valueFormat.m_unitFindVal = i_unitFindVal;
    valueFormat.m_iValSubStrVisibility = i_iValSubStrVisibility;
    valueFormat.m_unitFindRes = i_unitFindRes;
    valueFormat.m_iResSubStrVisibility = i_iResSubStrVisibility;
    return toString(valueFormat);
}

//------------------------------------------------------------------------------
QString CPhysVal::toString(
    EUnitFind    i_unitFindVal,
    int          i_iValSubStrVisibility,
    const CUnit& i_unitRes,
    int          i_iResSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat;
    valueFormat.m_unitFindVal = i_unitFindVal;
    valueFormat.m_iValSubStrVisibility = i_iValSubStrVisibility;
    valueFormat.m_pUnitRes = i_unitRes.isValid() ? const_cast<CUnit*>(&i_unitRes) : nullptr;
    valueFormat.m_iResSubStrVisibility = i_iResSubStrVisibility;
    return toString(valueFormat);
}

//------------------------------------------------------------------------------
QString CPhysVal::toString(
    const CUnit& i_unitVal,
    int          i_iValSubStrVisibility,
    EUnitFind    i_unitFindRes,
    int          i_iResSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat;
    valueFormat.m_pUnitVal = i_unitVal.isValid() ? const_cast<CUnit*>(&i_unitVal) : nullptr;
    valueFormat.m_iValSubStrVisibility = i_iValSubStrVisibility;
    valueFormat.m_unitFindRes = i_unitFindRes;
    valueFormat.m_iResSubStrVisibility = i_iResSubStrVisibility;
    return toString(valueFormat);
}

//------------------------------------------------------------------------------
QString CPhysVal::toString(
    const CUnit& i_unitVal,
    int          i_iValSubStrVisibility,
    const CUnit& i_unitRes,
    int          i_iResSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    SValueFormatProvider valueFormat;
    valueFormat.m_pUnitVal = i_unitVal.isValid() ? const_cast<CUnit*>(&i_unitVal) : nullptr;
    valueFormat.m_iValSubStrVisibility = i_iValSubStrVisibility;
    valueFormat.m_pUnitRes = i_unitRes.isValid() ? const_cast<CUnit*>(&i_unitRes) : nullptr;
    valueFormat.m_iResSubStrVisibility = i_iResSubStrVisibility;
    return toString(valueFormat);
}

//------------------------------------------------------------------------------
QString CPhysVal::toString( const SValueFormatProvider& i_valueFormat ) const
//------------------------------------------------------------------------------
{
    QString strPhysVal;

    CPhysValRes physValRes = m_physValRes;
    if( i_valueFormat.hasRes() )
    {
        physValRes = CPhysValRes(i_valueFormat.m_fRes, *i_valueFormat.m_pUnitRes, i_valueFormat.m_resType);
    }

    QString arSubStr[ESubStrCount];
    arSubStr[ESubStrVal] = invalidValueString();

    if( !isValid() )
    {
        strPhysVal = invalidValueString();
    }
    else
    {
        TFormatResult formatResult;
        QString strValTmp;
        double fVal = m_fVal;
        CUnit unitVal = m_unit;
        EUnitFind unitFindVal = i_valueFormat.m_unitFindVal;

        if( i_valueFormat.m_pUnitVal != nullptr )
        {
            if( *i_valueFormat.m_pUnitVal != unitVal && unitVal.isValid() && i_valueFormat.m_pUnitVal != nullptr )
            {
                fVal = unitVal.convertValue(fVal, *i_valueFormat.m_pUnitVal);
            }
            unitVal = *i_valueFormat.m_pUnitVal;
            unitFindVal = EUnitFind::None;
        }
        if( unitFindVal == EUnitFind::Best )
        {
            if( physValRes.isValid() )
            {
                formatResult = formatValue(
                    /* fVal                  */ fVal,
                    /* unitVal               */ unitVal,
                    /* fRes                  */ physValRes.getVal(),
                    /* pUnitRes              */ &physValRes.unit(),
                    /* resType               */ physValRes.type(),
                    /* iDigitsMantissa       */ i_valueFormat.m_iDigitsMantissa,
                    /* iDigitsExponent       */ i_valueFormat.m_iDigitsExponent,
                    /* iDigitsPerDigitGroup  */ i_valueFormat.m_iDigitsPerDigitGroup,
                    /* pstrDigitsGroupDelim. */ i_valueFormat.m_pstrDigitsGroupDelimiter,
                    /* pstrDecimalPoint      */ i_valueFormat.m_pstrDecimalPoint,
                    /* bUseEngineeringFormat */ i_valueFormat.m_bUseEngineeringFormat,
                    /* pfVal                 */ &fVal,
                    /* pstr                  */ &strValTmp,
                    /* pUnitVal              */ &unitVal );
            }
            else
            {
                formatResult = formatValue(
                    /* fVal                  */ fVal,
                    /* unitVal               */ unitVal,
                    /* iDigitsMantissaMax    */ i_valueFormat.m_iDigitsMantissa,
                    /* bResolutionLimitsMant.*/ i_valueFormat.m_bDigitsAccuracyLimitsMantissa,
                    /* iDigitsResolution     */ i_valueFormat.m_iDigitsAccuracy,
                    /* iDigitsExponent       */ i_valueFormat.m_iDigitsExponent,
                    /* iDigitsPerDigitGroup  */ i_valueFormat.m_iDigitsPerDigitGroup,
                    /* pstrDigitsGroupDelim. */ i_valueFormat.m_pstrDigitsGroupDelimiter,
                    /* pstrDecimalPoint      */ i_valueFormat.m_pstrDecimalPoint,
                    /* bUseEngineeringFormat */ i_valueFormat.m_bUseEngineeringFormat,
                    /* pfVal                 */ &fVal,
                    /* pstr                  */ &strValTmp,
                    /* pUnitVal              */ &unitVal );
            }
        }
        else
        {
            if( physValRes.isValid() )
            {
                formatResult = formatValue(
                    /* fVal                  */ fVal,
                    /* unitVal               */ unitVal,
                    /* fRes                  */ physValRes.getVal(),
                    /* pUnitRes              */ &physValRes.unit(),
                    /* resType               */ physValRes.type(),
                    /* iDigitsMantissa       */ i_valueFormat.m_iDigitsMantissa,
                    /* iDigitsExponent       */ i_valueFormat.m_iDigitsExponent,
                    /* iDigitsPerDigitGroup  */ i_valueFormat.m_iDigitsPerDigitGroup,
                    /* pstrDigitsGroupDelim. */ i_valueFormat.m_pstrDigitsGroupDelimiter,
                    /* pstrDecimalPoint      */ i_valueFormat.m_pstrDecimalPoint,
                    /* bUseEngineeringFormat */ i_valueFormat.m_bUseEngineeringFormat,
                    /* pfVal                 */ nullptr,
                    /* pstr                  */ &strValTmp );
            }
            else
            {
                formatResult = formatValue(
                    /* fVal                   */ fVal,
                    /* unitVal                */ unitVal,
                    /* iDigitsMantissaMax     */ i_valueFormat.m_iDigitsMantissa,
                    /* bResolutionLimitsMant. */ i_valueFormat.m_bDigitsAccuracyLimitsMantissa,
                    /* iDigitsResolution      */ i_valueFormat.m_iDigitsAccuracy,
                    /* iDigitsExponent        */ i_valueFormat.m_iDigitsExponent,
                    /* iDigitsPerDigitGroup   */ i_valueFormat.m_iDigitsPerDigitGroup,
                    /* pstrDigitsGroupDelim.  */ i_valueFormat.m_pstrDigitsGroupDelimiter,
                    /* pstrDecimalPoint       */ i_valueFormat.m_pstrDecimalPoint,
                    /* bUseEngineeringFormat  */ i_valueFormat.m_bUseEngineeringFormat,
                    /* pfVal                  */ nullptr,
                    /* pstr                   */ &strValTmp );
            }
        }

        switch( formatResult )
        {
            case FormatResult::Ok:
            case FormatResult::AccuracyOverflow:
            case FormatResult::AccuracyUnderflow:
            {
                if( i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::Val )
                {
                    arSubStr[ESubStrVal] = strValTmp;
                }
                if( unitVal.isValid() )
                {
                    if( i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitGrp )
                    {
                        // Use "key" instead of "name(IncludingParentNames)" as the path contains the
                        // root nodes (UnitClassType) whereas the key starts with the name of the science field.
                        arSubStr[ESubStrValUnitGrp] = unitVal.groupPath();
                    }
                    if( i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitSymbol )
                    {
                        arSubStr[ESubStrValUnit] = unitVal.symbol();
                    }
                    else if( i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitName )
                    {
                        arSubStr[ESubStrValUnit] = unitVal.unitName();
                    }
                }
                if( physValRes.isValid() && !(i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::None) )
                {
                    double fRes = physValRes.getVal();
                    CUnit unitRes = physValRes.unit();
                    EUnitFind unitFindRes = i_valueFormat.m_unitFindRes;

                    // If the resolution is defined as a ratio value but the value is not ...
                    if( unitVal.isValid() && !unitVal.isRatioOrQuantity()
                     && unitRes.isValid() && unitRes.isRatioOrQuantity() )
                    {
                        // If the unit of the resolution has been explicitly specified not as ratio
                        // or if the resolution should be indicated in the unit of the value ..
                        if( (i_valueFormat.m_pUnitRes != nullptr && !i_valueFormat.m_pUnitRes->isRatioOrQuantity())
                         || (!(i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitSymbol)
                          && !(i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitName)) )
                        {
                            // .. calculate the absolute value of the resolution in the unit of the value.
                            fRes = fRes * fVal * unitRes.factor();
                            unitRes = unitVal;

                            // If the unit of the resolution has been explicitly specified ..
                            if( i_valueFormat.m_pUnitRes != nullptr )
                            {
                                fRes = unitRes.convertValue(fRes, *i_valueFormat.m_pUnitRes);
                                unitRes = *i_valueFormat.m_pUnitRes;
                            }
                            else if( i_valueFormat.m_pUnitVal != nullptr )
                            {
                                fRes = unitVal.convertValue(fRes, *i_valueFormat.m_pUnitVal);
                                unitRes = *i_valueFormat.m_pUnitVal;
                            }
                        }
                    }
                    // If the unit of the resolution should not be indicated separately ..
                    if( !(i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitSymbol)
                     && !(i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitName) )
                    {
                        // ... the resolution must be indicated in the same unit as the value.
                        if( unitRes.isValid() && unitRes != unitVal )
                        {
                            fRes = unitRes.convertValue(fRes, unitVal);
                            unitRes = unitVal;
                        }
                        unitFindRes = EUnitFind::None;
                    }
                    if( unitFindRes == EUnitFind::Best )
                    {
                        formatResult = formatValue(
                            /* fVal                       */ fRes,
                            /* unitVal                   */  unitRes,
                            /* iDigitsMantissaMax         */ -1,
                            /* bDigitsAccuracyLimitsMant. */ true,
                            /* iDigitsAccuracy            */ 2,
                            /* iDigitsExponent            */ 0,
                            /* bUseEngineeringFormat      */ false,
                            /* pfVal                      */ &fRes,
                            /* pstr                       */ &strValTmp,
                            /* pUnitVal                   */ &unitRes );
                    }
                    else
                    {
                        formatResult = formatValue(
                            /* fVal                       */ fRes,
                            /* unitVal                    */ unitRes,
                            /* iDigitsMantissaMax         */ -1,
                            /* bDigitsAccuracyLimitsMant. */ true,
                            /* iDigitsAccuracy            */ 2,
                            /* iDigitsExponent            */ 0,
                            /* bUseEngineeringFormat      */ false,
                            /* pfVal                      */ nullptr,
                            /* pstr                       */ &strValTmp );
                    }
                    if( formatResult != FormatResult::Ok )
                    {
                        strValTmp = "";
                    }
                    removeTrailingZeros(&strValTmp);

                    if( i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::Val )
                    {
                        arSubStr[ESubStrRes] = strValTmp;
                    }
                    if( unitRes.isValid() )
                    {
                        if( i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitGrp )
                        {
                            // Use "key" instead of "name(IncludingParentNames)" as the path contains the
                            // root nodes (UnitClassType) whereas the key starts with the name of the science field.
                            arSubStr[ESubStrResUnitGrp] = unitRes.groupPath();
                        }
                        if( i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitSymbol )
                        {
                            arSubStr[ESubStrResUnit] = unitRes.symbol();
                        }
                        else if( i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitName )
                        {
                            arSubStr[ESubStrResUnit] = unitRes.unitName();
                        }
                    }
                } // if( physValRes.isValid() && !(i_iResSubStrVisibility & PhysValSubStr::None) )

                // If the value should be indicated without the resolution or if the value has no resolution ..
                if( i_valueFormat.m_iValSubStrVisibility != PhysValSubStr::None && (i_valueFormat.m_iResSubStrVisibility == PhysValSubStr::None || !physValRes.isValid()) )
                {
                    bool bUnitSpaceInserted = false;

                    if( (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::Val) && !arSubStr[ESubStrVal].isEmpty() )
                    {
                        strPhysVal += arSubStr[ESubStrVal];
                    }
                    if( (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitGrp) && !arSubStr[ESubStrValUnitGrp].isEmpty() )
                    {
                        strPhysVal += " ";
                        bUnitSpaceInserted = true;
                        strPhysVal += arSubStr[ESubStrValUnitGrp];
                    }
                    if( ((i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitSymbol) || (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitName)) && !arSubStr[ESubStrValUnit].isEmpty() )
                    {
                        if( !bUnitSpaceInserted )
                        {
                            strPhysVal += " ";
                            bUnitSpaceInserted = true;
                        }
                        strPhysVal += arSubStr[ESubStrValUnit];
                    }
                }

                // If the value should be indicated together with the resolution ..
                else if( i_valueFormat.m_iValSubStrVisibility != PhysValSubStr::None && i_valueFormat.m_iResSubStrVisibility != PhysValSubStr::None )
                {
                    // If the unit of the value and the unit of the resolution are the same and should 
                    // be indicated for both values together ...
                    if( !arSubStr[ESubStrValUnit].isEmpty() && arSubStr[ESubStrResUnit].isEmpty() )
                    {
                        bool bUnitSpaceInserted = false;

                        strPhysVal += "(";

                        if( (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::Val) && !arSubStr[ESubStrVal].isEmpty() )
                        {
                            strPhysVal += arSubStr[ESubStrVal];
                        }
                        if( (i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::Val) && !arSubStr[ESubStrRes].isEmpty() )
                        {
                            strPhysVal += " " + c_strSymbolPlusMinus + " " + arSubStr[ESubStrRes];
                        }
                        strPhysVal += ")";

                        if( (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitGrp) && !arSubStr[ESubStrValUnitGrp].isEmpty() )
                        {
                            strPhysVal += " ";
                            bUnitSpaceInserted = true;
                            strPhysVal += arSubStr[ESubStrValUnitGrp];
                        }
                        if( ((i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitSymbol) || (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitName)) && !arSubStr[ESubStrValUnit].isEmpty() )
                        {
                            if( !bUnitSpaceInserted )
                            {
                                strPhysVal += " ";
                                bUnitSpaceInserted = true;
                            }
                            strPhysVal += arSubStr[ESubStrValUnit];
                        }
                    }

                    // If the unit of the value and the unit of the resolution are different or should 
                    // be indicated for both values separately ...
                    else
                    {
                        bool bUnitSpaceInserted = false;

                        if( (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::Val) && !arSubStr[ESubStrVal].isEmpty() )
                        {
                            strPhysVal += arSubStr[ESubStrVal];
                        }
                        if( (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitGrp) && !arSubStr[ESubStrValUnitGrp].isEmpty() )
                        {
                            strPhysVal += " ";
                            bUnitSpaceInserted = true;
                            strPhysVal += arSubStr[ESubStrValUnitGrp];
                        }
                        if( ((i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitSymbol) || (i_valueFormat.m_iValSubStrVisibility & PhysValSubStr::UnitName)) && !arSubStr[ESubStrValUnit].isEmpty() )
                        {
                            if( !bUnitSpaceInserted )
                            {
                                strPhysVal += " ";
                                bUnitSpaceInserted = true;
                            }
                            strPhysVal += arSubStr[ESubStrValUnit];
                        }

                        // Append the resolution with its unit
                        bUnitSpaceInserted = false;
                        if( (i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::Val) && !arSubStr[ESubStrRes].isEmpty() )
                        {
                            strPhysVal += " " + c_strSymbolPlusMinus + " " + arSubStr[ESubStrRes];
                        }
                        if( (i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitGrp) && !arSubStr[ESubStrResUnitGrp].isEmpty() )
                        {
                            strPhysVal += " ";
                            bUnitSpaceInserted = true;
                            strPhysVal += arSubStr[ESubStrResUnitGrp];
                        }
                        if( ((i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitSymbol) || (i_valueFormat.m_iResSubStrVisibility & PhysValSubStr::UnitName)) && !arSubStr[ESubStrResUnit].isEmpty() )
                        {
                            if( !bUnitSpaceInserted )
                            {
                                strPhysVal += " ";
                                bUnitSpaceInserted = true;
                            }
                            strPhysVal += arSubStr[ESubStrResUnit];
                        }
                    }
                }
                break;
            }
            case FormatResult::ValueOverflow:
            {
                strPhysVal = "Overflow";
                break;
            }
            case FormatResult::ValueUnderflow:
            {
                strPhysVal = "Underflow";
                break;
            }
            default:
            {
                strPhysVal = "Format Error";
                break;
            }
        }
    }
    return strPhysVal;

} // toString

/*==============================================================================
public: // instance methods (to get and set the resolution of the value)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::setRes( double i_fVal )
//------------------------------------------------------------------------------
{
    m_physValRes.setVal(i_fVal);
}

//------------------------------------------------------------------------------
void CPhysVal::setRes( double i_fRes, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_physValRes.setVal(i_fRes, i_unit);
}

//------------------------------------------------------------------------------
void CPhysVal::setRes( const CPhysValRes& i_physValRes )
//------------------------------------------------------------------------------
{
    m_physValRes = i_physValRes;
}

/*==============================================================================
public: // instance methods (to convert the unit)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysVal::convertValue( const CUnit& i_unitDst )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_unitDst) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unitDst.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( isValid() && m_unit.isValid() && i_unitDst.isValid() && m_unit != i_unitDst )
    {
        m_fVal = m_unit.convertValue(m_fVal, i_unitDst);
        m_unit = i_unitDst;
    }
}
