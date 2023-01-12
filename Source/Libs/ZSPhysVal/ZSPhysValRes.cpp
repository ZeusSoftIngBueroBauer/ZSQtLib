/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsRatio.h"
#include "ZSPhysVal/ZSPhysVal.h"
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
    m_unitGrp(),
    m_unit()
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_unitGrp(),
    m_unit()
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_resType(i_physValRes.m_resType),
    m_fVal(i_physValRes.m_fVal),
    m_unitGrp(i_physValRes.m_unitGrp),
    m_unit(i_physValRes.m_unit)
{
} // copy ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CUnitGrp& i_unitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_unitGrp),
    m_unit()
{
    if( m_unitGrp.classType() == EUnitClassType::PhysSize )
    {
        m_unit = dynamic_cast<CPhysSize*>(&m_unitGrp)->getSIUnit();
    }
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CPhysSize& i_physSize, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_physSize),
    m_unit()
{
    if( m_unitGrp.classType() == EUnitClassType::PhysSize )
    {
        m_unit = dynamic_cast<CPhysSize*>(&m_unitGrp)->getSIUnit();
    }
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_unit.unitGroup()),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CUnitRatio& i_unitRatio, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_unitRatio.unitGroup()),
    m_unit(i_unitRatio)
{
}

////------------------------------------------------------------------------------
//CPhysValRes::CPhysValRes( const CUnitDataQuantity& i_unitDataQuantity, EResType i_resType ) :
////------------------------------------------------------------------------------
//    m_resType(i_resType),
//    m_fVal(0.0),
//    m_unitGrp(i_unitDataQuantity.unitGroup()),
//    m_unit(i_unitDataQuantity)
//{
//}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CPhysUnit& i_physUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_physUnit.physSize()),
    m_unit(i_physUnit)
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strUnitKey, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(),
    m_unit(i_strUnitKey)
{
    m_unitGrp = m_unit.unitGroup();
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, const CUnitGrp& i_unitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_unitGrp(i_unitGrp),
    m_unit()
{
    if( m_unitGrp.classType() == EUnitClassType::PhysSize )
    {
        m_unit = dynamic_cast<CPhysSize*>(&m_unitGrp)->getSIUnit();
    }
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_unitGrp(i_unit.unitGroup()),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, const CUnitRatio& i_unitRatio, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_unitGrp(i_unitRatio.unitGroup()),
    m_unit(i_unitRatio)
{
}

////------------------------------------------------------------------------------
//CPhysValRes::CPhysValRes( double i_fVal, const CUnitDataQuantity& i_unitDataQuantity, EResType i_resType ) :
////------------------------------------------------------------------------------
//    m_resType(i_resType),
//    m_fVal(i_fVal),
//    m_unitGrp(i_unitDataQuantity.unitGroup()),
//    m_unit(i_unitDataQuantity)
//{
//}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, const CPhysUnit& i_physUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_unitGrp(i_physUnit.physSize()),
    m_unit(i_physUnit)
{
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, const QString& i_strUnitKey, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_unitGrp(),
    m_unit(i_strUnitKey)
{
    m_unitGrp = m_unit.unitGroup();
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, const CUnitGrp& i_unitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_unitGrp),
    m_unit()
{
    if( m_unitGrp.classType() == EUnitClassType::PhysSize )
    {
        m_unit = dynamic_cast<CPhysSize*>(&m_unitGrp)->getSIUnit();
    }
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, const CUnit& i_unit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_unit.unitGroup()),
    m_unit(i_unit)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, const CUnitRatio& i_unitRatio, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_unitRatio.unitGroup()),
    m_unit(i_unitRatio)
{
    setVal(i_strVal);
}

////------------------------------------------------------------------------------
//CPhysValRes::CPhysValRes( const QString& i_strVal, const CUnitDataQuantity& i_unitDataQuantity, EResType i_resType ) :
////------------------------------------------------------------------------------
//    m_resType(i_resType),
//    m_fVal(0.0),
//    m_unitGrp(i_unitDataQuantity.unitGroup()),
//    m_unit(i_unitDataQuantity)
//{
//    setVal(i_strVal);
//}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, const CPhysUnit& i_physUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(i_physUnit.physSize()),
    m_unit(i_physUnit)
{
    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, const QString& i_strUnitKey, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_unitGrp(),
    m_unit(i_strUnitKey)
{
    m_unitGrp = m_unit.unitGroup();

    setVal(i_strVal);
}

//------------------------------------------------------------------------------
CPhysValRes::~CPhysValRes()
//------------------------------------------------------------------------------
{
} // dtor

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
void CPhysValRes::setUnitGroup( const CUnitGrp& i_unitGrp )
//------------------------------------------------------------------------------
{
    m_unitGrp = i_unitGrp;

    if( m_unitGrp.classType() == EUnitClassType::PhysSize )
    {
        m_unit = dynamic_cast<CPhysSize*>(&m_unitGrp)->getSIUnit();
    }
    else
    {
        m_unit = CUnit();
    }
}

//------------------------------------------------------------------------------
void CPhysValRes::setUnitGroupKey( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    if( m_unitGrp.keyInTree() != i_strUnitGrpKey )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CPhysVal::setUnitGroupKey";
        strArgs   = "UnitGrpKey: " + i_strUnitGrpKey;
        strErr    = "UnitGrpKey is different from key of already set unit group " + m_unitGrp.keyInTree();
        throw ZS::System::CException(
            __FILE__, __LINE__, EResultInvalidMethodCall,
            strMethod + "( " + strArgs + " ): " + strErr );
    }
    setUnitGroup(CUnitGrp(i_strUnitGrpKey));
}

//------------------------------------------------------------------------------
CUnitGrp CPhysValRes::unitGroup() const
//------------------------------------------------------------------------------
{
    return m_unitGrp;
}

////------------------------------------------------------------------------------
//QString CPhysValRes::unitGroupName() const
////------------------------------------------------------------------------------
//{
//    QString strName;
//
//    if( m_unitGrp == nullptr )
//    {
//        strName = "NoUnit";
//    }
//    else
//    {
//        strName = m_unitGrp->keyInTree();
//    }
//    return strName;
//
//} // getUnitGroupName

//------------------------------------------------------------------------------
CPhysSize CPhysValRes::physSize() const
//------------------------------------------------------------------------------
{
    CPhysSize physSize;

    if( m_unitGrp.classType() == EUnitClassType::PhysSize )
    {
        const CPhysSize* pPhysSize = dynamic_cast<const CPhysSize*>(&m_unitGrp);
        physSize = *pPhysSize;
    }
    return physSize;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
    m_unitGrp = m_unit.unitGroup();
}

//------------------------------------------------------------------------------
void CPhysValRes::setUnitRatio( const CUnitRatio& i_unitRatio )
//------------------------------------------------------------------------------
{
    m_unit = i_unitRatio;
    m_unitGrp = m_unit.unitGroup();
}

////------------------------------------------------------------------------------
//void CPhysValRes::setUnitDataQuantity( const CUnitDataQuantity& i_unitDataQuantity )
////------------------------------------------------------------------------------
//{
//    m_unit = unitDataQuantity;
//    m_unitGrp = m_unit.unitGroup();
//}

//------------------------------------------------------------------------------
void CPhysValRes::setPhysUnit( const CPhysUnit& i_physUnit )
//------------------------------------------------------------------------------
{
    m_unit = i_physUnit;
    m_unitGrp = i_physUnit.physSize();
}

//------------------------------------------------------------------------------
void CPhysValRes::setUnitKey( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( m_unit.keyInTree() != i_strUnitKey )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CPhysVal::setUnitKey";
        strArgs   = "UnitKey: " + i_strUnitKey;
        strErr    = "UnitKey is different from key of already set unit " + m_unit.keyInTree();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }
    setUnit(CUnit(i_strUnitKey));
}

//------------------------------------------------------------------------------
CUnit CPhysValRes::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

//------------------------------------------------------------------------------
CUnitRatio CPhysValRes::unitRatio() const
//------------------------------------------------------------------------------
{
    CUnitRatio unitRatio;
    const CUnitRatio* pUnitRatio = dynamic_cast<const CUnitRatio*>(&m_unit);
    if( pUnitRatio != nullptr )
    {
        unitRatio = *pUnitRatio;
    }
    return unitRatio;
}

////------------------------------------------------------------------------------
//CUnitDataQuantity* CPhysValRes::unitDataQuantity() const
////------------------------------------------------------------------------------
//{
//    return dynamic_cast<CUnitDataQuantity*>(m_unit);
//}

//------------------------------------------------------------------------------
CPhysUnit CPhysValRes::physUnit() const
//------------------------------------------------------------------------------
{
    CPhysUnit physUnit;
    const CPhysUnit* pPhysUnit = dynamic_cast<const CPhysUnit*>(&m_unit);
    if( pPhysUnit != nullptr )
    {
        physUnit = *pPhysUnit;
    }
    return physUnit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_unitGrp = CUnitGrp();
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
void CPhysValRes::setVal( double i_fVal, const CUnitRatio& i_unitRatio )
//------------------------------------------------------------------------------
{
    setUnitRatio(i_unitRatio);
    setVal(i_fVal);
}

////------------------------------------------------------------------------------
//void CPhysValRes::setVal( double i_fVal, const CUnitDataQuantity& i_unitDataQuantity )
////------------------------------------------------------------------------------
//{
//    setUnitDataQuantity(i_unitDataQuantity);
//    setVal(i_fVal);
//}

//------------------------------------------------------------------------------
void CPhysValRes::setVal( double i_fVal, const CPhysUnit& i_physUnit )
//------------------------------------------------------------------------------
{
    setPhysUnit(i_physUnit);
    setVal(i_fVal);
}

//------------------------------------------------------------------------------
TFormatResult CPhysValRes::setVal( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    bool bValOk = false;
    double fVal = 0.0;
    CUnitGrp unitGrpVal = m_unitGrp;
    CUnit unitVal = m_unit;

    TFormatResult formatResult = parseValStr(
        /* strVal      */ i_strVal,
        /* pbValOk     */ &bValOk,
        /* pfVal       */ &fVal,
        /* pUnitGrpVal */ &unitGrpVal,
        /* pUnitVal    */ &unitVal,
        /* pbResOk     */ nullptr,
        /* pfRes       */ nullptr,
        /* pUnitGrpRes */ nullptr,
        /* pUnitRes    */ nullptr,
        /* pIdxTree    */ nullptr );

    if( !(formatResult & FormatResult::Error) && bValOk )
    {
        m_fVal = fVal;
        m_unitGrp = unitGrpVal;
        m_unit = unitVal;
    }
    return formatResult;
}

//------------------------------------------------------------------------------
TFormatResult CPhysValRes::setVal( const QString& i_strVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    setUnit(i_unit);
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
TFormatResult CPhysValRes::setVal( const QString& i_strVal, const CUnitRatio& i_unitRatio )
//------------------------------------------------------------------------------
{
    setUnitRatio(i_unitRatio);
    return setVal(i_strVal);
}

////------------------------------------------------------------------------------
//TFormatResult CPhysValRes::setVal( const QString& i_strVal, const CUnitDataQuantity& i_unitDataQuantity )
////------------------------------------------------------------------------------
//{
//    setUnitDataQuantity(i_unitDataQuantity);
//    return setVal(i_strVal);
//}

//------------------------------------------------------------------------------
TFormatResult CPhysValRes::setVal( const QString& i_strVal, const CPhysUnit& i_physUnit )
//------------------------------------------------------------------------------
{
    setPhysUnit(i_physUnit);
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
        strUnitGrp = pUnit->unitGroup().keyInTree();
    }
    if( i_iSubStrVisibility & PhysValSubStr::UnitSymbol )
    {
        strUnit = pUnit->symbol();
    }
    else if( i_iSubStrVisibility & PhysValSubStr::UnitName )
    {
        strUnit = pUnit->name();
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
            strUnitGrp = i_unit.unitGroup().keyInTree();
        }
        if( i_iSubStrVisibility & PhysValSubStr::UnitSymbol )
        {
            strUnit = i_unit.symbol();
        }
        else if( i_iSubStrVisibility & PhysValSubStr::UnitName )
        {
            strUnit = i_unit.name();
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
    if( m_unitGrp != i_physValResOther.m_unitGrp )
    {
        return false;
    }
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
    m_unitGrp = i_physValResNew.m_unitGrp;
    m_unit    = i_physValResNew.m_unit;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValRes CPhysValRes::operator + ( const CPhysValRes& i_physValResOp ) const
//------------------------------------------------------------------------------
{
    if( m_unitGrp != i_physValResOp.m_unitGrp )
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
    if( m_unitGrp != i_physValResOp.m_unitGrp )
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
            QString strAddErrInfo = "Val:" + m_unit.name() + ", Op:" + unitOp.name();
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
