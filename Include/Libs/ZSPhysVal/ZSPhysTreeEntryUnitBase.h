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

#ifndef ZSPhysVal_PhysTreeEntryUnitBase_h
#define ZSPhysVal_PhysTreeEntryUnitBase_h

#include <QtCore/qstring.h>
#include <QtCore/qvector.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

namespace ZS
{
namespace PhysVal
{
class CUnitsTreeEntryGrpBase;

//******************************************************************************
class ZSPHYSVALDLL_API CUnitsTreeEntryUnitBase : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitsTreeEntryUnitBase"; }
public: // ctors and dtor
    CUnitsTreeEntryUnitBase(
        CUnitsTreeEntryGrpBase* i_pParentBranch,
        bool           i_bIsLogarithmic,
        double         i_fLogFactor,
        const QString& i_strName,
        const QString& i_strSymbol,
        const QString& i_strFactorPrefix );
    virtual ~CUnitsTreeEntryUnitBase();
public: // operators
    bool operator == ( const CUnitsTreeEntryUnitBase& i_other ) const;
    bool operator != ( const CUnitsTreeEntryUnitBase& i_other ) const;
public: // instance methods (configuration)
    EUnitClassType classType() const { return m_classType; }
    QString classType2Str() const;
    bool isRatio() const;
    bool isQuantity() const;
    bool isRatioOrQuantity() const;
    bool isPhysSize() const;
    CUnitsTreeEntryGrpBase* unitGroup() const;
    QString symbol() const { return m_strSymbol; }
    QString factorPrefix() const { return m_strFactorPrefix; }
public: // overridables (converting values)
    virtual bool isLogarithmic() const { return m_bIsLogarithmic; }
    virtual double logarithmicFactor() const { return m_fLogFactor; }
    virtual bool isConvertible( const CUnitsTreeEntryUnitBase* /*i_pUnitDst*/, double /*i_fVal = 1.0*/ ) const { return false; }
    virtual double convertValue( double i_fVal, const CUnitsTreeEntryUnitBase* /*i_pUnitDst*/ ) const { return i_fVal; }
public: // overridables (findBestUnit)
    virtual void setNextLowerHigherUnits( CUnitsTreeEntryUnitBase* i_pNextLower, CUnitsTreeEntryUnitBase* i_pNextHigher );
    virtual CUnitsTreeEntryUnitBase* nextLowerUnit() const { return m_pNextLower; }
    virtual CUnitsTreeEntryUnitBase* nextHigherUnit() const { return m_pNextHigher; }
    virtual CUnitsTreeEntryUnitBase* findBestUnit(
        double  i_fVal,
        double* o_pfValue = nullptr,
        int     i_iDigitsLeadingMax = 3 ) const;
protected: // instance members
    /*!< [PhysSize, Ratios, UserDefinedQuantities] */
    EUnitClassType m_classType;
    /*!< e.g. "s", "W", "V", "A", ... */
    QString m_strSymbol;
    /*!< e.g. M, k, m, ... */
    QString m_strFactorPrefix;
    /*!< e.g. "dBm" is logarithmic */
    bool m_bIsLogarithmic;
    /*!< e.g. 10.0 for power related quantities, 20.0 for Volt etc. */
    double m_fLogFactor;
    /*!< e.g. ns if this is µs */
    CUnitsTreeEntryUnitBase* m_pNextLower;
    /*!< e.g. ms if this is µs */
    CUnitsTreeEntryUnitBase* m_pNextHigher;

}; // class CUnitsTreeEntryUnitBase

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysTreeEntryUnitBase_h
