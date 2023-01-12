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

#include <QtCore/qstring.h>
#include <QtCore/qvector.h>

#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnitsFctConvert.h"

namespace ZS
{
namespace PhysVal
{
class CUnitGrpTreeEntry;
class CUnitTreeEntry;
class CUnitGrp;
class CUnit;

/*******************************************************************************
Exported methods
*******************************************************************************/

ZSPHYSVALDLL_API bool areOfSameUnitGroup( const CUnitGrp& i_unitGrp1, const CUnit& i_unit2 );
ZSPHYSVALDLL_API bool areOfSameUnitGroup( const CUnit& i_unit1, const CUnit& i_unit2 );


//******************************************************************************
class ZSPHYSVALDLL_API CUnitTreeEntry : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitTreeEntry"; }
public: // ctors and dtor
    CUnitTreeEntry(
        CUnitGrpTreeEntry* i_pParentBranch,
        bool               i_bIsLogarithmic,
        double             i_fLogFactor,
        const QString&     i_strName,
        const QString&     i_strSymbol );
    CUnitTreeEntry(CUnitTreeEntry&& i_other);
    CUnitTreeEntry(CUnitTreeEntry& i_other) = delete;
    CUnitTreeEntry(const CUnitTreeEntry& i_other) = delete;
    virtual ~CUnitTreeEntry();
public: // operators
    CUnitTreeEntry& operator=(CUnitTreeEntry& i_other) = delete;
    CUnitTreeEntry& operator=(const CUnitTreeEntry& i_other) = delete;
    CUnitTreeEntry& operator=(CUnitTreeEntry&& i_other);
public: // operators
    bool operator == ( const CUnitTreeEntry& i_other ) const;
    bool operator != ( const CUnitTreeEntry& i_other ) const;
public: // instance methods (configuration)
    EUnitClassType classType() const { return m_classType; }
    QString classType2Str() const;
    CUnitGrpTreeEntry* unitGroup() const;
    QString symbol() const { return m_strSymbol; }
public: // overridables (converting values)
    virtual bool isLogarithmic() const { return m_bIsLogarithmic; }
    virtual double logarithmicFactor() const { return m_fLogFactor; }
    virtual bool isConvertible( const CUnitTreeEntry* /*i_pUnitDst*/, double /*i_fVal = 1.0*/ ) const { return false; }
    virtual double convertValue( double i_fVal, const CUnitTreeEntry* /*i_pUnitDst*/ ) const { return i_fVal; }
public: // overridables (findBestUnit)
    virtual void setNextLowerHigherUnits( CUnitTreeEntry* i_pNextLower, CUnitTreeEntry* i_pNextHigher );
    virtual CUnitTreeEntry* nextLowerUnit() const { return m_pNextLower; }
    virtual CUnitTreeEntry* nextHigherUnit() const { return m_pNextHigher; }
    virtual CUnitTreeEntry* findBestUnit(
        double  i_fVal,
        double* o_pfValue = nullptr,
        int     i_iDigitsLeadingMax = 3 ) const;
protected: // instance members
    /*!< [PhysSize, Ratios, UserDefinedQuantities] */
    EUnitClassType m_classType;
    /*!< e.g. "s", "W", "V", "A", ... */
    QString m_strSymbol;
    /*!< e.g. "dBm" is logarithmic */
    bool m_bIsLogarithmic;
    /*!< e.g. 10.0 for power related quantities, 20.0 for Volt etc. */
    double m_fLogFactor;
    /*!< e.g. ns if this is µs */
    CUnitTreeEntry* m_pNextLower;
    /*!< e.g. ms if this is µs */
    CUnitTreeEntry* m_pNextHigher;

}; // class CUnitTreeEntry

//******************************************************************************
class ZSPHYSVALDLL_API CUnit
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnit"; }
public: // ctors and dtor
    CUnit();
    CUnit(CUnit* i_pUnit);
    CUnit(const CUnit* i_pUnit);
    CUnit(CUnit& i_unit);
    CUnit(const CUnit& i_unit);
    CUnit(CUnitTreeEntry* i_pUnit);
    CUnit(const CUnitTreeEntry* i_pUnit);
    CUnit(CUnitTreeEntry& i_unit);
    CUnit(const CUnitTreeEntry& i_unit);
    CUnit(const QString& i_strUniqueName);
    virtual ~CUnit();
public: // operators
    bool operator == ( const CUnit& i_other ) const;
    bool operator != ( const CUnit& i_other ) const;
public: // instance methods
    QString nodeSeparator() const;
    QString keyInTree() const;
    CUnitGrp unitGroup() const;
    QString name() const;
    QString symbol() const;
public: // overridables
    virtual bool isValid() const;
    virtual EUnitClassType classType() const;
    virtual QString classType2Str() const;
public: // overridables (converting values)
    virtual bool isLogarithmic() const;
    virtual double logarithmicFactor() const;
    virtual bool isConvertible( const CUnit& i_unitDst, double i_fVal = 1.0 ) const;
    virtual double convertValue( double i_fVal, const CUnit& i_unitDst ) const;
public: // overridables (findBestUnit)
    virtual CUnit nextLowerUnit() const;
    virtual CUnit nextHigherUnit() const;
    virtual CUnit findBestUnit(
        double  i_fVal,
        double* o_pfValue = nullptr,
        int     i_iDigitsLeadingMax = 3 ) const;
protected: // instance members
    CUnitTreeEntry* m_pTreeEntry;
    QString m_strUniqueName;

}; // class CUnit

//******************************************************************************
class ZSPHYSVALDLL_API CPhysUnitTreeEntry : public CUnitTreeEntry
//******************************************************************************
{
friend class CPhysSizeTreeEntry;
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysUnitTreeEntry"; }
public: // ctors and dtor
    CPhysUnitTreeEntry(
        CPhysSizeTreeEntry* i_pPhysSize,
        const QString&      i_strPrefix );
    CPhysUnitTreeEntry(
        CPhysSizeTreeEntry* i_pPhysSize,
        bool                i_bIsLogarithmic,
        const QString&      i_strName,
        const QString&      i_strSymbol,
        const double        i_fMFromBaseOrRefVal );
    CPhysUnitTreeEntry(CPhysUnitTreeEntry&& i_unit) = delete;
    CPhysUnitTreeEntry(CPhysUnitTreeEntry& i_unit) = delete;
    CPhysUnitTreeEntry(const CPhysUnitTreeEntry& i_unit) = delete;
    virtual ~CPhysUnitTreeEntry();
public: // operators
    CPhysUnitTreeEntry& operator=(CPhysUnitTreeEntry& i_other) = delete;
    CPhysUnitTreeEntry& operator=(const CPhysUnitTreeEntry& i_other) = delete;
    CPhysUnitTreeEntry& operator=(CPhysUnitTreeEntry&& i_other) = delete;
public: // operators
    bool operator == ( const CPhysUnitTreeEntry& i_other ) const;
    bool operator != ( const CPhysUnitTreeEntry& i_other ) const;
public: // instance methods
    CPhysSizeTreeEntry* physSize() const;
    CPhysUnitTreeEntry* getSIUnit() const { return m_pPhysUnitSI; }
    QString prefixStr() const { return m_strPrefix; }
public: // instance methods
    double getFactorConvertFromSIUnit() const { return m_fctConvertFromSIUnit.m_fM; }
    void setFactorConvertFromSIUnit( double i_fFactor );
public: // overridables of base class CUnitTreeEntry (converting values)
    virtual bool isConvertible( const CUnitTreeEntry* i_pUnitDst, double i_fVal = 1.0 ) const;
    virtual double convertValue( double i_fVal, const CUnitTreeEntry* i_pUnitDst ) const;
public: // instance methods (converting values)
    double convertFromSIUnit( double i_fVal ) const;
    double convertIntoSIUnit( double i_fVal ) const;
    double convertValue(
        double i_fVal, const CPhysUnitTreeEntry* i_pUnitDst,
        double i_fValRef, const CPhysUnitTreeEntry* i_pUnitRef ) const;
public: // instance methods (conversion routine into SI unit)
    const CFctConvert& getFctConvertFromSIUnit() const { return m_fctConvertIntoSIUnit; }
    QString getFctConvertFromSIUnitName() const { return m_fctConvertFromSIUnit.m_strFctConvertName; }
    const CFctConvert& getFctConvertIntoSIUnit() const { return m_fctConvertIntoSIUnit; }
    QString getFctConvertIntoSIUnitName() const { return m_fctConvertIntoSIUnit.m_strFctConvertName; }
public: // instance methods (conversion routines to convert into units of same quantity)
    int getFctConvertsInternalCount() const { return m_arFctConvertsInternal.size(); }
    int findFctConvertInternalIdx( const CPhysUnitTreeEntry* i_pPhysUnitDst ) const;
    CFctConvert* getFctConvertInternal( int i_idx ) const;
    CFctConvert* findFctConvertInternal( const CPhysUnitTreeEntry* i_pPhysUnitDst ) const;
    QString findFctConvertInternalName( const CPhysUnitTreeEntry* i_pPhysUnitDst ) const;
    QString getFctConvertInternalName( int i_idx ) const;
public: // instance methods (conversion routines to convert into units of other quantities)
    void addFctConvertExternal(
        CPhysUnitTreeEntry* i_pPhysUnitDst,
        CPhysUnitTreeEntry* i_pPhysUnitRef,
        EFctConvert         i_fctConvert );
    int getFctConvertsExternalCount() const { return m_arFctConvertsExternal.size(); }
    int findFctConvertExternalIdx( const CPhysUnitTreeEntry* i_pPhysUnitDst ) const;
    CFctConvert* getFctConvertExternal( int i_idx ) const;
    CFctConvert* findFctConvertExternal( const CPhysUnitTreeEntry* i_pPhysUnitDst ) const;
    QString findFctConvertExternalName( const CPhysUnitTreeEntry* i_pPhysUnitDst ) const;
    QString getFctConvertExternalName( int i_idx ) const;
protected: // instance members
    CPhysUnitTreeEntry* m_pPhysUnitSI;
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

}; // class CPhysUnitTreeEntry

//******************************************************************************
class ZSPHYSVALDLL_API CPhysUnit : public CUnit
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysUnit"; }
public: // ctors and dtor
    CPhysUnit();
    CPhysUnit(CPhysUnit* i_pPhysUnit);
    CPhysUnit(const CPhysUnit* i_pPhysUnit);
    CPhysUnit(CPhysUnit& i_physUnit);
    CPhysUnit(const CPhysUnit& i_physUnit);
    CPhysUnit(CPhysUnitTreeEntry* i_pPhysUnit);
    CPhysUnit(const CPhysUnitTreeEntry* i_pPhysUnit);
    CPhysUnit(CPhysUnitTreeEntry& i_physUnit);
    CPhysUnit(const CPhysUnitTreeEntry& i_physUnit);
    CPhysUnit(const QString& i_strUniqueName);
    virtual ~CPhysUnit();
public: // operators
    bool operator == ( const CPhysUnit& i_other ) const;
    bool operator != ( const CPhysUnit& i_other ) const;
public: // instance methods
    CPhysSize physSize() const;
};

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnits_h
