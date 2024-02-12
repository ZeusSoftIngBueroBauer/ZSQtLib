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

#ifndef ZSPhysVal_PhysUnits_h
#define ZSPhysVal_PhysUnits_h

#include "ZSPhysVal/ZSPhysValDllMain.h"

namespace ZS
{
namespace System
{
class CIdxTreeEntry;
}

namespace PhysVal
{
class CUnit;
class CUnitsTreeEntryGrpBase;
class CUnitsTreeEntryGrpPhysUnits;
class CUnitsTreeEntryUnitBase;
class CUnitsTreeEntryUnitRatio;
class CUnitsTreeEntryUnitQuantity;
class CUnitsTreeEntryPhysUnit;

/*******************************************************************************
Exported methods
*******************************************************************************/

ZSPHYSVALDLL_API bool areOfSameUnitGroup( const CUnit& i_unit1, const CUnit& i_unit2 );


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
    CUnit(const CUnit& i_unit);
    CUnit(CUnitsTreeEntryGrpBase* i_pGrpEntry);
    CUnit(const CUnitsTreeEntryGrpBase* i_pGrpEntry);
    CUnit(CUnitsTreeEntryGrpBase& i_grpEntry);
    CUnit(const CUnitsTreeEntryGrpBase& i_grpEntry);
    CUnit(CUnitsTreeEntryGrpPhysUnits* i_pGrpEntry);
    CUnit(const CUnitsTreeEntryGrpPhysUnits* i_pGrpEntry);
    CUnit(CUnitsTreeEntryGrpPhysUnits& i_grpEntry);
    CUnit(const CUnitsTreeEntryGrpPhysUnits& i_grpEntry);
    CUnit(CUnitsTreeEntryUnitBase* i_pUnitEntry);
    CUnit(CUnitsTreeEntryUnitRatio* i_pUnitEntry);
    CUnit(CUnitsTreeEntryUnitQuantity* i_pUnitEntry);
    CUnit(CUnitsTreeEntryPhysUnit* i_pUnitEntry);
    CUnit(const CUnitsTreeEntryUnitRatio* i_pUnitEntry);
    CUnit(const CUnitsTreeEntryUnitQuantity* i_pUnitEntry);
    CUnit(const CUnitsTreeEntryPhysUnit* i_pUnitEntry);
    CUnit(CUnitsTreeEntryUnitRatio& i_unitEntry);
    CUnit(CUnitsTreeEntryUnitQuantity& i_unitEntry);
    CUnit(CUnitsTreeEntryPhysUnit& i_unitEntry);
    CUnit(const CUnitsTreeEntryUnitRatio& i_unitEntry);
    CUnit(const CUnitsTreeEntryUnitQuantity& i_unitEntry);
    CUnit(const CUnitsTreeEntryPhysUnit& i_unitEntry);
    CUnit(const QString& i_strUniqueName);
    CUnit(const QString& i_strPath, const QString& i_strNameOrSymbol);
    virtual ~CUnit();
public: // operators
    bool operator == ( const CUnit& i_other ) const;
    bool operator != ( const CUnit& i_other ) const;
public: // instance methods
    QString groupPath() const;
    void setGroupPath(const QString& i_strPath);
    QString unitName() const;
    void setUnitName(const QString& i_strName);
    QString symbol() const;
    void setSymbol(const QString& i_strSymbol);
public: // instance methods
    QString nodeSeparator() const;
    QString keyInTree() const;
public: // overridables
    bool isValidGroup() const;
    bool isValid() const;
    EUnitClassType classType() const;
    QString classType2Str() const;
    bool isRatio() const;
    bool isQuantity() const;
    bool isRatioOrQuantity() const;
    bool isPhysSize() const;
public: // overridables (converting values)
    double factor() const;
    bool isLogarithmic() const;
    double logarithmicFactor() const;
    bool isConvertible( const CUnit& i_unitDst, double i_fVal = 1.0 ) const;
    double convertValue( double i_fVal, const CUnit& i_unitDst ) const;
public: // overridables (findBestUnit)
    virtual QVector<CUnit> getAllUnitsOfGroup() const;
    virtual CUnit getLowestUnit() const;
    virtual CUnit getHighestUnit() const;
    virtual CUnit nextLowerUnit() const;
    virtual CUnit nextHigherUnit() const;
    virtual CUnit findBestUnit(
        double  i_fVal,
        double* o_pfValue = nullptr,
        int     i_iDigitsLeadingMax = 3 ) const;
protected: // instance members
    ZS::System::CIdxTreeEntry* m_pTreeEntry;
    EUnitClassType m_classType;
    QString m_strKeyInTree;
    QString m_strUnitName;
    QString m_strGrpPath;
    QString m_strSymbol;

}; // class CUnit

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnits_h
