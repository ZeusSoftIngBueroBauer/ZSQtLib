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

#ifndef ZSPhysVal_PhysVal_h
#define ZSPhysVal_PhysVal_h

#include <QtCore/qvector.h>

#include "ZSPhysVal/ZSPhysValRes.h"
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace PhysVal
{
class CPhysVal;

/*******************************************************************************
Exported methods
*******************************************************************************/

ZSPHYSVALDLL_API CPhysVal minPhysVal( const QVector<CPhysVal>& i_arphysvals );
ZSPHYSVALDLL_API CPhysVal minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2 );
ZSPHYSVALDLL_API CPhysVal minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3 );
ZSPHYSVALDLL_API CPhysVal minPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3, const CPhysVal& i_physval4 );
ZSPHYSVALDLL_API CPhysVal maxPhysVal( const QVector<CPhysVal>& i_arphysvals );
ZSPHYSVALDLL_API CPhysVal maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2 );
ZSPHYSVALDLL_API CPhysVal maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3 );
ZSPHYSVALDLL_API CPhysVal maxPhysVal( const CPhysVal& i_physval1, const CPhysVal& i_physval2, const CPhysVal& i_physval3, const CPhysVal& i_physval4 );

//******************************************************************************
struct ZSPHYSVALDLL_API SValueFormatProvider
//******************************************************************************
{
public: // ctors
    // Default ctor.
    SValueFormatProvider();
    // Copy ctor
    SValueFormatProvider( const SValueFormatProvider& );
    // For values without an accuracy.
    SValueFormatProvider(
        CUnit*         i_pUnitVal,    // nullptr to indicate the value with the current or the "best unit".
        EUnitFind      i_unitFindVal, // Only evaluated if pUnit == nullptr
        int            i_iValSubStrVisibility,
        int            i_iDigitsMantissa,
        int            i_iDigitsExponent,
        bool           i_bUseEngineeringFormat = false,
        int            i_iDigitsPerDigitGroup  = 0,
        const QString* i_pstrDigitsGroupDelimiter = nullptr,
        const QString* i_pstrDecimalPoint = nullptr );
    // For values where the accuracy is defined by a value.
    SValueFormatProvider(
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
        int            i_iResSubStrVisibility );
    // For values where the accuracy should limit either the number of digits
    // for the whole mantissa or for the trailing string.
    SValueFormatProvider(
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
        bool           i_bDigitsAccuracyLimitsMantissa );
    ~SValueFormatProvider();
public: // operators
    SValueFormatProvider& operator = ( const SValueFormatProvider& );
public: // instance methods
    bool hasRes() const;
public: // instance members
    CUnit*     m_pUnitVal;
    EUnitFind  m_unitFindVal;
    int        m_iValSubStrVisibility;
    int        m_iDigitsMantissa;
    int        m_iDigitsExponent;
    bool       m_bUseEngineeringFormat;
    int        m_iDigitsPerDigitGroup;
    QString*   m_pstrDigitsGroupDelimiter;
    QString*   m_pstrDecimalPoint;
    // If the resolution is set to a value != 0.0 the resolution as defined
    // by the value provider will be used instead of the current resolution
    // of the physical value.
    double     m_fRes;
    CUnit*     m_pUnitRes;
    EResType   m_resType;
    EUnitFind  m_unitFindRes;
    int        m_iResSubStrVisibility;
    int        m_iDigitsAccuracy;
    bool       m_bDigitsAccuracyLimitsMantissa;

}; // struct SValueFormatProvider


//******************************************************************************
class ZSPHYSVALDLL_API CPhysVal
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysVal"; }
public: // ctors and dtor
    CPhysVal( EResType i_resType = EResType::Resolution );
    CPhysVal( const CUnit& i_unit, EResType i_resType = EResType::Resolution );
    CPhysVal( const CUnit& i_unitVal, const CUnit& i_unitRes, EResType i_resType = EResType::Resolution );
    CPhysVal( double i_fVal, EResType i_resType = EResType::Resolution );
    CPhysVal( double i_fVal, double i_fResVal, EResType i_resType = EResType::Resolution );
    CPhysVal( double i_fVal, const CUnit& i_unit, EResType i_resType = EResType::Resolution );
    CPhysVal(
        double i_fVal, const CUnit& i_unitVal,
        const CUnit& i_unitRes, EResType i_resType = EResType::Resolution );
    CPhysVal(
        double i_fVal, const CUnit& i_unit,
        double i_fResVal, EResType i_resType = EResType::Resolution );
    CPhysVal(
        double i_fVal, const CUnit& i_unitVal,
        double i_fResVal, const CUnit& i_unitRes, EResType i_resType = EResType::Resolution );
    CPhysVal( double i_fVal, const CUnit& i_unitVal, const CPhysValRes& i_physValRes );
    CPhysVal( const QString& i_strVal, EResType i_resType = EResType::Resolution );
    CPhysVal( const QString& i_strVal, const CUnit& i_unit, EResType i_resType = EResType::Resolution );
    CPhysVal(
        const QString& i_strVal,
        double i_fResVal, EResType i_resType = EResType::Resolution );
    CPhysVal(
        const QString& i_strVal, const CUnit& i_unitVal,
        const CUnit& i_unitRes, EResType i_resType = EResType::Resolution );
    CPhysVal(
        const QString& i_strVal,
        double i_fResVal, const CUnit& i_unitRes, EResType i_resType = EResType::Resolution );
    CPhysVal(
        const QString& i_strVal, const CUnit& i_unit,
        double i_fResVal, EResType i_resType = EResType::Resolution );
    CPhysVal(
        const QString& i_strVal, const CUnit& i_unitVal,
        double i_fResVal, const CUnit& i_unitRes, EResType i_resType = EResType::Resolution );
    CPhysVal( const QString& i_strVal, const CUnit& i_unitVal, const CPhysValRes& i_physValRes );
    CPhysVal( const CPhysVal& i_physVal );
    ~CPhysVal();
public: // operators
    bool operator == ( const CPhysVal& i_physValOther ) const;
    bool operator != ( const CPhysVal& i_physValOther ) const;
    bool operator < ( const CPhysVal& i_physValOther ) const;
    bool operator > ( const CPhysVal& i_physValOther ) const;
    bool operator <= ( const CPhysVal& i_physValOther ) const;
    bool operator >= ( const CPhysVal& i_physValOther ) const;
    CPhysVal& operator = ( const CPhysVal& i_physValNew );
    CPhysVal operator + ( const CPhysVal& i_physValOp ) const;
    CPhysVal& operator += ( const CPhysVal& i_physValOp );
    CPhysVal operator - ( const CPhysVal& i_physValOp ) const;
    CPhysVal& operator -= ( const CPhysVal& i_physValOp );
    bool operator == ( double i_fValOther ) const;
    bool operator != ( double i_fValOther ) const;
    bool operator < ( double i_fValOther ) const;
    bool operator > ( double i_fValOther ) const;
    bool operator <= ( double i_fValOther ) const;
    bool operator >= ( double i_fValOther ) const;
    CPhysVal& operator = ( const double i_fValNew );
    CPhysVal operator + ( double i_fOp ) const;
    CPhysVal& operator += ( double i_fOp );
    CPhysVal operator - ( double i_fOp ) const;
    CPhysVal& operator -= ( double i_fOp );
    CPhysVal operator * ( double i_fOp ) const;
    CPhysVal& operator *= ( double i_fOp );
    CPhysVal operator / ( double i_fOp ) const;
    CPhysVal& operator /= ( double i_fOp );
    bool operator == ( const QString& i_strValOther ) const;
    bool operator != ( const QString& i_strValOther ) const;
    bool operator < ( const QString& i_strValOther ) const;
    bool operator > ( const QString& i_strValOther ) const;
    bool operator <= ( const QString& i_strValOther ) const;
    bool operator >= ( const QString& i_strValOther ) const;
    CPhysVal& operator = ( const QString& i_strValNew );
    CPhysVal operator + ( const QString& i_strValOp ) const;
    CPhysVal& operator += ( const QString& i_strValOp );
    CPhysVal operator - ( const QString& i_strValOp ) const;
    CPhysVal& operator -= ( const QString& i_strValOp );
public: // instance methods
    bool isValid() const { return (m_validity != ZS::System::EValueValidity::Invalid); }
    bool isNull() const { return (m_validity == ZS::System::EValueValidity::Null); }
    void setValidity( ZS::System::EValueValidity i_validity );
    ZS::System::EValueValidity getValidity() const { return m_validity; }
public: // instance methods
    void setUnit( const CUnit& i_unit );
    CUnit unit() const { return m_unit; }
public: // instance methods
    void invalidateObjectReferences();
public: // instance methods (to set the value)
    void setVal( double i_fVal );
    void setVal( double i_fVal, const CUnit& i_unit );
public: // instance methods (to set the value)
    TFormatResult setVal( const QString& i_strVal );
    TFormatResult setVal( const QString& i_strVal, const CUnit& i_unit );
public: // instance methods (to get the double value)
    double getVal() const { return m_fVal; }
    double getVal( const CUnit& i_unit ) const;
public: // instance methods (to convert the value into a string)
    QString toString(
        EUnitFind i_unitFindVal          = EUnitFind::None,
        int       i_iValSubStrVisibility = PhysValSubStr::Val|PhysValSubStr::UnitSymbol,
        EUnitFind i_unitFindRes          = EUnitFind::None,
        int       i_iResSubStrVisibility = PhysValSubStr::None ) const;
    QString toString(
        EUnitFind    i_unitFindVal,
        int          i_iValSubStrVisibility,
        const CUnit& i_unitRes,
        int          i_iResSubStrVisibility = PhysValSubStr::Val|PhysValSubStr::UnitSymbol ) const;
    QString toString(
        const CUnit& i_unitVal,
        int          i_iValSubStrVisibility = PhysValSubStr::Val|PhysValSubStr::UnitSymbol,
        EUnitFind    i_unitFindRes          = EUnitFind::None,
        int          i_iResSubStrVisibility = PhysValSubStr::Val|PhysValSubStr::UnitSymbol ) const;
    QString toString(
        const CUnit& i_unitVal,
        int          i_iValSubStrVisibility,
        const CUnit& i_unitRes,
        int          i_iResSubStrVisibility = PhysValSubStr::Val|PhysValSubStr::UnitSymbol ) const;
    QString toString( const SValueFormatProvider& i_valueFormat ) const;
public: // instance methods (to get and set the resolution of the value)
    bool hasRes() const { return m_physValRes.isValid(); }
    CPhysValRes getRes() const { return m_physValRes; }
    void setRes( double i_fRes );
    void setRes( double i_fRes, const CUnit& i_unit );
    void setRes( const CPhysValRes& i_physValRes );
public: // instance methods (to convert the value into another unit)
    void convertValue( const CUnit& i_unitDst );
protected: // instance members
    CUnit m_unit;
    ZS::System::EValueValidity m_validity;
    double m_fVal;
    CPhysValRes m_physValRes;

}; // class CPhysVal

Q_DECLARE_METATYPE(CPhysVal);

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysVal_h
