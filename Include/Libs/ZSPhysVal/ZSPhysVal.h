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

#ifndef ZSPhysVal_PhysVal_h
#define ZSPhysVal_PhysVal_h

#include <QtCore/qstring.h>
#include <QtCore/qvector.h>

#include "ZSSys/ZSSysAux.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysValRes.h"

namespace ZS
{
namespace PhysVal
{
class CPhysVal;
class CIdxTreePhysSizes;

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

ZSPHYSVALDLL_API void insertDelimiter(
    int            i_iDigitsPerDigitGroup,
    const QString& i_strDelimiter,
    QString*       io_pstrValue,
    int            i_iDigitsLeading,
    int            i_iDigitsTrailing );

ZSPHYSVALDLL_API void removeTrailingZeros(
    QString* io_pstrValue,
    int      i_iDigitsTrailingMin = 1,
    QChar    i_charDecPoint = '.' );

ZSPHYSVALDLL_API void removeLeadingZerosFromExponent( QString* io_pstrValue );

ZSPHYSVALDLL_API EFormatResult formatString(
    const QString& i_strValue,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent );

ZSPHYSVALDLL_API EFormatResult formatValue(
    double       i_fVal,
    const CUnit* i_pUnitVal,
    double       i_fRes,
    const CUnit* i_pUnitRes,
    EResType     i_resType,
    int          i_iDigitsMantissa,
    int          i_iDigitsExponent,
    bool         i_bUseEngineeringFormat = false,
    double*      o_pfVal = nullptr,
    QString*     o_pstrVal = nullptr,
    CUnit**      o_ppUnitVal = nullptr,
    int*         o_piDigitsLeading = nullptr,
    int*         o_piDigitsTrailing = nullptr,
    int*         o_piDigitsExponent = nullptr );

ZSPHYSVALDLL_API EFormatResult formatValue(
    double         i_fVal,
    const CUnit*   i_pUnitVal,
    double         i_fRes,
    const CUnit*   i_pUnitRes,
    EResType       i_resType,
    int            i_iDigitsMantissa,
    int            i_iDigitsExponent,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint = nullptr,
    bool           i_bUseEngineeringFormat = false,
    double*        o_pfVal = nullptr,
    QString*       o_pstrVal = nullptr,
    CUnit**        o_ppUnitVal = nullptr,
    int*           o_piDigitsLeading = nullptr,
    int*           o_piDigitsTrailing = nullptr,
    int*           o_piDigitsExponent = nullptr );

ZSPHYSVALDLL_API EFormatResult formatValue(
    double         i_fVal,
    const CUnit*   i_pUnitVal,
    int            i_iDigitsMantissaMax,
    bool           i_bDigitsAccuracyLimitsMantissa,
    int            i_iDigitsAccuracy,
    int            i_iDigitsExponent,
    bool           i_bUseEngineeringFormat = false,
    double*        o_pfVal = nullptr,
    QString*       o_pstrVal = nullptr,
    CUnit**        o_ppUnitVal = nullptr,
    int*           o_piDigitsLeading = nullptr,
    int*           o_piDigitsTrailing = nullptr,
    int*           o_piDigitsExponent = nullptr );

ZSPHYSVALDLL_API EFormatResult formatValue(
    double         i_fVal,
    const CUnit*   i_pUnitVal,
    int            i_iDigitsMantissaMax,
    bool           i_bDigitsAccuracyLimitsMantissa,
    int            i_iDigitsAccuracy,
    int            i_iDigitsExponent,
    int            i_iDigitsPerDigitGroup,
    const QString* i_pstrDigitsGroupDelimiter,
    const QString* i_pstrDecimalPoint = nullptr,
    bool           i_bUseEngineeringFormat = false,
    double*        o_pfVal = nullptr,
    QString*       o_pstrVal = nullptr,
    CUnit**        o_ppUnitVal = nullptr,
    int*           o_piDigitsLeading = nullptr,
    int*           o_piDigitsTrailing = nullptr,
    int*           o_piDigitsExponent = nullptr );

ZSPHYSVALDLL_API EFormatResult parseValStr(
    const QString&     i_strVal,
    bool*              o_pbValOk,
    double*            o_pfVal,
    CUnitGrp**         io_ppUnitGrpVal,
    CUnit**            io_ppUnitVal,
    bool*              o_pbResOk,
    double*            o_pfResVal,
    CUnitGrp**         io_ppUnitGrpRes,
    CUnit**            io_ppUnitRes,
    CIdxTreePhysSizes* i_pIdxTree );

ZSPHYSVALDLL_API EFormatResult getSubStrings( const QString& i_strVal, QString* io_arSubStr/*[ESubStrCount]*/, CIdxTreePhysSizes* i_pIdxTree );

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
public: // ctors and dtor
    CPhysVal( EResType i_resType = EResTypeResolution );
    CPhysVal( CUnitGrp* i_pUnitGrp, EResType i_resType = EResTypeResolution );
    CPhysVal( CUnit* i_pUnit, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, double i_fResVal, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnitGrp* i_pUnitGrp, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnit* i_pUnit, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnit* i_pUnit, double i_fResVal, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnit* i_pUnitVal, double i_fResVal, CUnit* i_pUnitRes, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnit* i_pUnitVal, const CPhysValRes& i_physValRes );
    CPhysVal( double i_fVal, CUnitRatio* i_pUnitRatio, double i_fResVal = 0.0, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnitRatio* i_pUnitRatioVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnitRatio* i_pUnitRatioVal, const CPhysValRes& i_physValRes );
    CPhysVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal = 0.0, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity, const CPhysValRes& i_physValRes );
    CPhysVal( double i_fVal, CPhysUnit* i_pPhysUnit, double i_fResVal = 0.0, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CPhysUnit* i_pPhysUnitVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CPhysUnit* i_pPhysUnitVal, double i_fResVal, CPhysUnit* i_pPhysUnitRes, EResType i_resType = EResTypeResolution );
    CPhysVal( double i_fVal, CPhysUnit* i_pPhysUnitVal, const CPhysValRes& i_physValRes );
    CPhysVal( const QString& i_strVal, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, double i_fResVal, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnit* i_pUnit, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnit* i_pUnit, double i_fResVal, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnit* i_pUnitVal, double i_fResVal, CUnit* i_pUnitRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnit* i_pUnitVal, const CPhysValRes& i_physValRes );
    CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrp, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrp, double i_fResVal, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, CUnitGrp* i_pUnitGrpRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, double i_fResVal, CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, double i_fResVal, CPhysUnit* i_pPhysUnitRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitGrp* i_pUnitGrpVal, const CPhysValRes& i_physValRes );
    CPhysVal( const QString& i_strVal, CUnitRatio* i_pUnitRatio, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitRatio* i_pUnitRatio, double i_fResVal, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitRatio* i_pUnitRatioVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitRatio* i_pUnitRatioVal, const CPhysValRes& i_physValRes );
    CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, double i_fResVal, CUnitDataQuantity* i_pUnitDataQuantityRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, const CPhysValRes& i_physValRes );
    CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnit, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnit, double i_fResVal, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnitVal, double i_fResVal, CUnitRatio* i_pUnitRatioRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnitVal, double i_fResVal, CPhysUnit* i_pPhysUnitRes, EResType i_resType = EResTypeResolution );
    CPhysVal( const QString& i_strVal, CPhysUnit* i_pPhysUnitVal, const CPhysValRes& i_physValRes );
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
    void setUnitGroup( CUnitGrp* i_pUnitGrp );
    void setUnitGroupKey( const QString& i_strUnitGrpKey );
    CUnitGrp* unitGroup() const { return m_pUnitGrp; }
    QString getUnitGroupName( bool i_bInsertParentNames = false ) const;
    QString getUnitGroupKey() const { return m_strUnitGrpKey; }
    CPhysSize* getPhysSize() const;
public: // instance methods
    void setUnit( CUnit* i_pUnit );
    void setUnitKey( const QString& i_strUnitKey );
    void setUnitRatio( CUnitRatio* i_pUnitRatio );
    void setUnitDataQuantity( CUnitDataQuantity* i_pUnitDataQuantity );
    void setPhysUnit( CPhysUnit* i_pPhysUnit );
    CUnit* getUnit() const { return m_pUnit; }
    CUnitRatio* getUnitRatio() const;
    CUnitDataQuantity* getUnitDataQuantity() const;
    CPhysUnit* getPhysUnit() const;
    QString getUnitName( bool i_bInsertParentNames = false ) const;
    QString getUnitKey() const { return m_strUnitKey; }
public: // instance methods
    void invalidateObjectReferences();
public: // instance methods (to set the value)
    void setVal( double i_fVal );
    void setVal( double i_fVal, CUnit* i_pUnit );
    void setVal( double i_fVal, CUnitRatio* i_pUnitRatio );
    void setVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity );
    void setVal( double i_fVal, CPhysUnit* i_pPhysUnit );
public: // instance methods (to set the value)
    EFormatResult setVal( const QString& i_strVal );
    EFormatResult setVal( const QString& i_strVal, CUnit* i_pUnit );
    EFormatResult setVal( const QString& i_strVal, CUnitRatio* i_pUnitRatio );
    EFormatResult setVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity );
    EFormatResult setVal( const QString& i_strVal, CPhysUnit* i_pPhysUnit );
public: // instance methods (to get the double value)
    double getVal() const { return m_fVal; }
    double getVal( const CUnit* i_pUnit ) const;
    double getVal( const CUnitRatio* i_pUnitRatio ) const;
    double getVal( const CUnitDataQuantity* i_pUnitDataQuantity ) const;
    double getVal( const CPhysUnit* i_pPhysUnit ) const;
public: // instance methods (to convert the value into a string)
    QString toString(
        EUnitFind i_unitFindVal          = EUnitFindNone,
        int       i_iValSubStrVisibility = EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
        EUnitFind i_unitFindRes          = EUnitFindNone,
        int       i_iResSubStrVisibility = EPhysValSubStrNone ) const;
    QString toString(
        EUnitFind    i_unitFindVal,
        int          i_iValSubStrVisibility,
        const CUnit* i_pUnitRes,
        int          i_iResSubStrVisibility = EPhysValSubStrVal|EPhysValSubStrUnitSymbol ) const;
    QString toString(
        const CUnit* i_pUnitVal,
        int          i_iValSubStrVisibility = EPhysValSubStrVal|EPhysValSubStrUnitSymbol,
        EUnitFind    i_unitFindRes          = EUnitFindNone,
        int          i_iResSubStrVisibility = EPhysValSubStrVal|EPhysValSubStrUnitSymbol ) const;
    QString toString(
        const CUnit* i_pUnitVal,
        int          i_iValSubStrVisibility,
        const CUnit* i_pUnitRes,
        int          i_iResSubStrVisibility = EPhysValSubStrVal|EPhysValSubStrUnitSymbol ) const;
    QString toString( const SValueFormatProvider& i_valueFormat ) const;
public: // instance methods (to get and set the resolution of the value)
    bool hasRes() const { return m_physValRes.isValid(); }
    CPhysValRes getRes() const { return m_physValRes; }
    void setRes( double i_fRes );
    void setRes( double i_fRes, CUnit* i_pUnit );
    void setRes( const CPhysValRes& i_physValRes );
public: // instance methods (to convert the value into another unit)
    void convertValue( CUnit* i_pUnitDst );
protected: // instance members
    // Please note that object references are only valid if the referenced objects
    // are living in the same thread context (and therefore within the same process
    // and same machine). If the value has to be exchanged between threads, processes
    // or to remote machines unique keys are necessary.
    CUnitGrp*                  m_pUnitGrp;      // Could be either CUnitGrpRatio or CPhysSize
    CUnit*                     m_pUnit;         // Could be either CUnitRatio, CUnitDataQuantity or CPhysUnit
    QString                    m_strUnitGrpKey;
    QString                    m_strUnitKey;
    ZS::System::EValueValidity m_validity;
    double                     m_fVal;
    CPhysValRes                m_physValRes;

}; // class CPhysVal

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysVal_h
