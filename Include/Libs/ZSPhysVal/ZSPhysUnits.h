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

#ifndef ZSPhysVal_PhysUnits_h
#define ZSPhysVal_PhysUnits_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSPhysVal/ZSPhysUnitsFctConvert.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

namespace ZS
{
namespace PhysVal
{
class CUnit;
class CUnitGrp;
class CPhysSize;
class CPhysUnit;

/*******************************************************************************
Exported methods
*******************************************************************************/

ZSPHYSVALDLL_API bool areOfSameUnitGroup( const CUnitGrp* i_pUnitGrp1, const CUnit* i_pUnit2 );
ZSPHYSVALDLL_API bool areOfSameUnitGroup( const CUnit* i_pUnit1, const CUnit* i_pUnit2 );


//******************************************************************************
class ZSPHYSVALDLL_API CUnit : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnit"; }
public: // ctors and dtor
    CUnit();
    CUnit(
        CUnitGrp*      i_pParentBranch,
        bool           i_bIsLogarithmic,
        double         i_fLogFactor,
        const QString& i_strName,
        const QString& i_strSymbol );
    CUnit(CUnit&& i_other);
    CUnit(CUnit& i_other) = delete;
    CUnit(const CUnit& i_other) = delete;
    virtual ~CUnit();
public: // operators
    CUnit& operator=(CUnit& i_other) = delete;
    CUnit& operator=(const CUnit& i_other) = delete;
    CUnit& operator=(CUnit&& i_other);
public: // operators
    bool operator == ( const CUnit& i_other ) const;
    bool operator != ( const CUnit& i_other ) const;
public: // instance methods (configuration)
    EUnitClassType classType() const { return m_classType; }
    QString classType2Str() const;
    CUnitGrp* unitGroup() const;
    //QString groupName( bool i_bInsertParentNames = false ) const;
    QString symbol() const { return m_strSymbol; }
public: // overridables (converting values)
    virtual bool isLogarithmic() const { return m_bIsLogarithmic; }
    virtual double logarithmicFactor() const { return m_fLogFactor; }
    virtual bool isConvertible( const CUnit* /*i_pUnitDst*/, double /*i_fVal = 1.0*/ ) const { return false; }
    virtual double convertValue( double i_fVal, const CUnit* /*i_pUnitDst*/ ) const { return i_fVal; }
public: // overridables (findBestUnit)
    virtual void setNextLowerHigherUnits( CUnit* i_pNextLower, CUnit* i_pNextHigher );
    virtual CUnit* nextLowerUnit() const { return m_pNextLower; }
    virtual CUnit* nextHigherUnit() const { return m_pNextHigher; }
    virtual CUnit* findBestUnit(
        double  i_fVal,
        double* o_pfValue = nullptr,
        int     i_iDigitsLeadingMax = 3 ) const;
protected: // instance members
    /*!< [PhysScienceFields, Ratios, UserDefinedQuantities] */
    EUnitClassType m_classType;
    /*!< e.g. "s", "W", "V", "A", ... */
    QString m_strSymbol;
    /*!< e.g. "dBm" is logarithmic */
    bool m_bIsLogarithmic;
    /*!< e.g. 10.0 for power related quantities, 20.0 for Volt etc. */
    double m_fLogFactor;
    /*!< e.g. ns if this is µs */
    CUnit* m_pNextLower;
    /*!< e.g. ms if this is µs */
    CUnit* m_pNextHigher;

}; // class CUnit

//******************************************************************************
class ZSPHYSVALDLL_API CPhysUnit : public CUnit
//******************************************************************************
{
friend class CPhysSize;
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysUnit"; }
public: // ctors and dtor
    CPhysUnit();
    CPhysUnit(
        CPhysSize*     i_pPhysSize,
        const QString& i_strPrefix );
    CPhysUnit(
        CPhysSize*     i_pPhysSize,
        bool           i_bIsLogarithmic,
        const QString& i_strName,
        const QString& i_strSymbol,
        const double   i_fMFromBaseOrRefVal );
    CPhysUnit(CPhysUnit&& i_unit) = delete;
    CPhysUnit(CPhysUnit& i_unit) = delete;
    CPhysUnit(const CPhysUnit& i_unit) = delete;
    virtual ~CPhysUnit();
public: // operators
    CPhysUnit& operator=(CPhysUnit& i_other) = delete;
    CPhysUnit& operator=(const CPhysUnit& i_other) = delete;
    CPhysUnit& operator=(CPhysUnit&& i_other) = delete;
public: // operators
    bool operator == ( const CPhysUnit& i_other ) const;
    bool operator != ( const CPhysUnit& i_other ) const;
public: // instance methods
    CPhysSize* physSize() const;
    CPhysUnit* getSIUnit() const { return m_pPhysUnitSI; }
    QString prefixStr() const { return m_strPrefix; }
public: // instance methods
    double getFactorConvertFromSIUnit() const { return m_fctConvertFromSIUnit.m_fM; }
    void setFactorConvertFromSIUnit( double i_fFactor );
public: // overridables of base class CUnit (converting values)
    virtual bool isConvertible( const CUnit* i_pUnitDst, double i_fVal = 1.0 ) const;
    virtual double convertValue( double i_fVal, const CUnit* i_pUnitDst ) const;
public: // instance methods (converting values)
    double convertFromSIUnit( double i_fVal ) const;
    double convertIntoSIUnit( double i_fVal ) const;
    double convertValue(
        double           i_fVal,
        const CPhysUnit* i_pUnitDst,
        double           i_fValRef,
        const CPhysUnit* i_pUnitRef ) const;
public: // instance methods (conversion routine into SI unit)
    const CFctConvert& getFctConvertFromSIUnit() const { return m_fctConvertIntoSIUnit; }
    QString getFctConvertFromSIUnitName() const { return m_fctConvertFromSIUnit.m_strFctConvertName; }
    const CFctConvert& getFctConvertIntoSIUnit() const { return m_fctConvertIntoSIUnit; }
    QString getFctConvertIntoSIUnitName() const { return m_fctConvertIntoSIUnit.m_strFctConvertName; }
public: // instance methods (conversion routines to convert into units of same quantity)
    int getFctConvertsInternalCount() const { return m_arFctConvertsInternal.size(); }
    int findFctConvertInternalIdx( const CPhysUnit* i_pPhysUnitDst ) const;
    CFctConvert* getFctConvertInternal( int i_idx ) const;
    CFctConvert* findFctConvertInternal( const CPhysUnit* i_pPhysUnitDst ) const;
    QString findFctConvertInternalName( const CPhysUnit* i_pPhysUnitDst ) const;
    QString getFctConvertInternalName( int i_idx ) const;
public: // instance methods (conversion routines to convert into units of other quantities)
    void addFctConvertExternal(
        CPhysUnit*  i_pPhysUnitDst,
        CPhysUnit*  i_pPhysUnitRef,
        EFctConvert i_fctConvert );
    int getFctConvertsExternalCount() const { return m_arFctConvertsExternal.size(); }
    int findFctConvertExternalIdx( const CPhysUnit* i_pPhysUnitDst ) const;
    CFctConvert* getFctConvertExternal( int i_idx ) const;
    CFctConvert* findFctConvertExternal( const CPhysUnit* i_pPhysUnitDst ) const;
    QString findFctConvertExternalName( const CPhysUnit* i_pPhysUnitDst ) const;
    QString getFctConvertExternalName( int i_idx ) const;
protected: // instance members
    //CPhysSize* m_pPhysSize;
    CPhysUnit* m_pPhysUnitSI;
    // e.g. M, k, m, etc.
    QString m_strPrefix;
    bool m_bInitialized;
    //int m_iPhysSizeRowIdx;
    CFctConvert m_fctConvertFromSIUnit;
    CFctConvert m_fctConvertIntoSIUnit;
    // Array of direct conversion routines into units of same quantity
    QVector<CFctConvert> m_arFctConvertsInternal;
    // Array of direct conversion routines into units of other quantities
    QVector<CFctConvert> m_arFctConvertsExternal;

}; // class CPhysUnit

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnits_h
