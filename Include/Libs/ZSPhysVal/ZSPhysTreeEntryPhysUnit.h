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

#ifndef ZSPhysVal_PhysTreeEntryPhysUnit_h
#define ZSPhysVal_PhysTreeEntryPhysUnit_h

#include "ZSPhysVal/ZSPhysTreeEntryUnitBase.h"
#include "ZSPhysVal/ZSPhysUnitsFctConvert.h"

namespace ZS
{
namespace PhysVal
{
//******************************************************************************
class ZSPHYSVALDLL_API CUnitsTreeEntryPhysUnit : public CUnitsTreeEntryUnitBase
//******************************************************************************
{
friend class CUnitsTreeEntryGrpPhysUnits;
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitsTreeEntryPhysUnit"; }
public: // ctors and dtor
    CUnitsTreeEntryPhysUnit(
        CUnitsTreeEntryGrpPhysUnits* i_pPhysSize,
        const QString& i_strFactorPrefix );
    CUnitsTreeEntryPhysUnit(
        CUnitsTreeEntryGrpPhysUnits* i_pPhysSize,
        bool i_bIsLogarithmic,
        const QString& i_strName,
        const QString& i_strSymbol,
        const double i_fMFromBaseOrRefVal );
    virtual ~CUnitsTreeEntryPhysUnit();
public: // operators
    bool operator == ( const CUnitsTreeEntryPhysUnit& i_other ) const;
    bool operator != ( const CUnitsTreeEntryPhysUnit& i_other ) const;
public: // instance methods
    CUnitsTreeEntryGrpPhysUnits* physSize() const;
    CUnitsTreeEntryPhysUnit* getSIUnit() const { return m_pPhysUnitSI; }
public: // instance methods
    double getFactorConvertFromSIUnit() const { return m_fctConvertFromSIUnit.m_fM; }
    void setFactorConvertFromSIUnit( double i_fFactor );
public: // overridables of base class CUnitsTreeEntryUnitBase (converting values)
    virtual bool isConvertible( const CUnitsTreeEntryUnitBase* i_pUnitDst, double i_fVal = 1.0 ) const;
    virtual double convertValue( double i_fVal, const CUnitsTreeEntryUnitBase* i_pUnitDst ) const;
public: // instance methods (converting values)
    double convertFromSIUnit( double i_fVal ) const;
    double convertIntoSIUnit( double i_fVal ) const;
    double convertValue(
        double i_fVal, const CUnitsTreeEntryPhysUnit* i_pUnitDst,
        double i_fValRef, const CUnitsTreeEntryPhysUnit* i_pUnitRef ) const;
public: // instance methods (conversion routine into SI unit)
    const CFctConvert& getFctConvertFromSIUnit() const { return m_fctConvertIntoSIUnit; }
    QString getFctConvertFromSIUnitName() const { return m_fctConvertFromSIUnit.m_strFctConvertName; }
    const CFctConvert& getFctConvertIntoSIUnit() const { return m_fctConvertIntoSIUnit; }
    QString getFctConvertIntoSIUnitName() const { return m_fctConvertIntoSIUnit.m_strFctConvertName; }
public: // instance methods (conversion routines to convert into units of same quantity)
    int getFctConvertsInternalCount() const { return m_arFctConvertsInternal.size(); }
    int findFctConvertInternalIdx( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const;
    CFctConvert* getFctConvertInternal( int i_idx ) const;
    CFctConvert* findFctConvertInternal( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const;
    QString findFctConvertInternalName( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const;
    QString getFctConvertInternalName( int i_idx ) const;
public: // instance methods (conversion routines to convert into units of other quantities)
    void addFctConvertExternal(
        CUnitsTreeEntryPhysUnit* i_pPhysUnitDst,
        CUnitsTreeEntryPhysUnit* i_pPhysUnitRef,
        EFctConvert         i_fctConvert );
    int getFctConvertsExternalCount() const { return m_arFctConvertsExternal.size(); }
    int findFctConvertExternalIdx( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const;
    CFctConvert* getFctConvertExternal( int i_idx ) const;
    CFctConvert* findFctConvertExternal( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const;
    QString findFctConvertExternalName( const CUnitsTreeEntryPhysUnit* i_pPhysUnitDst ) const;
    QString getFctConvertExternalName( int i_idx ) const;
protected: // instance members
    CUnitsTreeEntryPhysUnit* m_pPhysUnitSI;
    bool m_bInitialized;
    //int m_iPhysSizeRowIdx;
    CFctConvert m_fctConvertFromSIUnit;
    CFctConvert m_fctConvertIntoSIUnit;
    // Array of direct conversion routines into units of same quantity
    QVector<CFctConvert> m_arFctConvertsInternal;
    // Array of direct conversion routines into units of other quantities
    QVector<CFctConvert> m_arFctConvertsExternal;

}; // class CUnitsTreeEntryPhysUnit

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysTreeEntryPhysUnit_h
