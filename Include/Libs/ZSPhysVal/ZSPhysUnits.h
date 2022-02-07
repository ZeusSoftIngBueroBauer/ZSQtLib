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

ZSPHYSVALDLL_API bool    areOfSameUnitGroup( const CUnitGrp* i_pUnitGrp1, const CUnit* i_pUnit2 );
ZSPHYSVALDLL_API bool    areOfSameUnitGroup( const CUnit* i_pUnit1, const CUnit* i_pUnit2 );
ZSPHYSVALDLL_API QString getSymbol( CUnit* i_pUnit );
ZSPHYSVALDLL_API QString getName( CUnit* i_pUnit, bool i_bInsertParentNames = false );


//******************************************************************************
class ZSPHYSVALDLL_API CUnit
//******************************************************************************
{
friend class CUnitGrp;
public: // ctors and dtor
    CUnit(
        CUnitGrp*      i_pUnitGrp,
        bool           i_bIsLogarithmic,
        double         i_fLogFactor,
        const QString& i_strName,
        const QString& i_strSymbol,
        const QString& i_strKey ); // e.g. "Kinematics::Time::ms", ...
    virtual ~CUnit();
public: // instance methods (configuration)
    EUnitClassType classType() const { return m_classType; }
    QString classType2Str() const;
    CUnitGrp* getUnitGroup() const { return m_pUnitGrp; };
    QString getGroupName( bool i_bInsertParentNames = false ) const;
    QString getName( bool i_bInsertParentNames = false ) const;
    QString getSymbol() const { return m_strSymbol; }
    QString getKey() const { return m_strKey; }
    QChar getNameSeparator() const;
public: // operators
    bool operator == ( const CUnit& i_unitOther ) const;
    bool operator != ( const CUnit& i_unitOther ) const;
public: // overridables (converting values)
    virtual bool isLogarithmic() const { return m_bIsLogarithmic; }
    virtual double getLogarithmicFactor() const { return m_fLogFactor; }
    virtual bool isConvertible( const CUnit* /*i_pUnitDst*/, double /*i_fVal = 1.0*/ ) const { return false; }
    virtual double convertValue( double i_fVal, const CUnit* /*i_pUnitDst*/ ) const { return i_fVal; }
public: // overridables (findBestUnit)
    virtual void setNextLowerHigherUnits( CUnit* i_pNextLower, CUnit* i_pNextHigher );
    virtual CUnit* getNextLowerUnit() const { return m_pNextLower; }
    virtual CUnit* getNextHigherUnit() const { return m_pNextHigher; }
    virtual CUnit* findBestUnit(
        double  i_fVal,
        double* o_pfValue = nullptr,
        int     i_iDigitsLeadingMax = 3 ) const;
private: // default and copy ctor not allowed
    CUnit();
    CUnit( const CUnit& );
private: // assignment operator not allowed
    CUnit& operator = ( const CUnit& );
protected: // instance members
    EUnitClassType m_classType;         // [PhysScienceFields, Ratios, UserDefinedQuantities]
    CUnitGrp*      m_pUnitGrp;
    QString        m_strName;           // e.g. "Seconds", "Watt", "Volt", "Ampere", ...
    QString        m_strSymbol;         // e.g. "s", "W", "V", "A", ...
    QString        m_strKey;            // e.g. "Kinematics::Time::ms", ...
    bool           m_bIsLogarithmic;    // e.g. "dBm" is logarithmic
    double         m_fLogFactor;        // e.g. 10.0 for power related quantities, 20.0 for Volt etc.
    CUnit*         m_pNextLower;        // e.g. ns if this is µs
    CUnit*         m_pNextHigher;       // e.g. ms if this is µs

}; // class CUnit

//******************************************************************************
class ZSPHYSVALDLL_API CPhysUnit : public CUnit
//******************************************************************************
{
friend class CPhysSize;
public: // ctors and dtor
    CPhysUnit( // ctor for linear units
        CPhysSize*     i_pPhysSize,
        const QString& i_strPrefix );            // e.g. "" for SI-Base, "m", "k", "µ", "M", ...
    CPhysUnit( // ctor for linear units
        CPhysSize*     i_pPhysSize,
        bool           i_bIsLogarithmic,         // false for linear units (e.g. Degree), true for logarithmic units (e.g. dBm)
        const QString& i_strName,                // e.g. "Degree", "Inch", ... or "dBWatt", "dBMilliWatt", "dBu(0.775V)", ...
        const QString& i_strSymbol,              // e.g. "°", "in", ... or "dBW", "dBm", "dBu", ...
        const double   i_fMFromBaseOrRefVal );   // e.g. "180/PI", 0.0254, ... or 1.0 for dBW, 1.0e-3 for dBm, 0.775 for 0.775 V, ...
    virtual ~CPhysUnit();
public: // operators
    bool operator == ( const CPhysUnit& i_physUnitOther ) const;
    bool operator != ( const CPhysUnit& i_physUnitOther ) const;
public: // instance methods
    CPhysSize* getPhysSize() const { return m_pPhysSize; }
    CPhysUnit* getSIUnit() const { return m_pPhysUnitSI; }
    QString getPrefixStr() const { return m_strPrefix; }
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
private: // default and copy ctor not allowed
    CPhysUnit();
    CPhysUnit( const CPhysUnit& );
private: // assignment operator not allowed
    CPhysUnit& operator = ( const CPhysUnit& );
protected: // instance members
    CPhysSize*           m_pPhysSize;
    CPhysUnit*           m_pPhysUnitSI;
    QString              m_strPrefix;                   // e.g. M, k, m, etc.
    bool                 m_bInitialized;
    int                  m_iPhysSizeRowIdx;
    CFctConvert          m_fctConvertFromSIUnit;
    CFctConvert          m_fctConvertIntoSIUnit;
    QVector<CFctConvert> m_arFctConvertsInternal;       // Array of direct conversion routines into units of same quantity
    QVector<CFctConvert> m_arFctConvertsExternal;       // Array of direct conversion routines into units of other quantities

}; // class CPhysUnit

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnits_h
