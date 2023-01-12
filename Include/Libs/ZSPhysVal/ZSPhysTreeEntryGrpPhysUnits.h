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

#ifndef ZSPhysVal_PhysTreeEntryGrpPhysUnits_h
#define ZSPhysVal_PhysTreeEntryGrpPhysUnits_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpBase.h"
#include "ZSPhysVal/ZSPhysUnitsFctConvert.h"

namespace ZS
{
namespace PhysVal
{
class CUnitsTreeEntryGrpScienceField;

//******************************************************************************
class ZSPHYSVALDLL_API CUnitsTreeEntryGrpPhysUnits : public CUnitsTreeEntryGrpBase
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitsTreeEntryGrpPhysUnits"; }
public: // ctors and dtor
    CUnitsTreeEntryGrpPhysUnits(
        CIdxTreeUnits* i_pIdxTree,
        const QString& i_strName,
        const QString& i_strSIUnitName,
        const QString& i_strSIUnitSymbol,
        const QString& i_strFormulaSymbol,
        bool           i_bIsPowerRelated );
    CUnitsTreeEntryGrpPhysUnits(
        CUnitsTreeEntryGrpScienceField* i_pParentBranch,
        const QString& i_strName,
        const QString& i_strSIUnitName,
        const QString& i_strSIUnitSymbol,
        const QString& i_strFormulaSymbol,
        bool           i_bIsPowerRelated );
    CUnitsTreeEntryGrpPhysUnits(CUnitsTreeEntryGrpPhysUnits&& i_other) = delete;
    CUnitsTreeEntryGrpPhysUnits(CUnitsTreeEntryGrpPhysUnits& i_other) = delete;
    CUnitsTreeEntryGrpPhysUnits(const CUnitsTreeEntryGrpPhysUnits& i_other) = delete;
    virtual ~CUnitsTreeEntryGrpPhysUnits();
public: // operators
    CUnitsTreeEntryGrpPhysUnits& operator=(CUnitsTreeEntryGrpPhysUnits& i_other) = delete;
    CUnitsTreeEntryGrpPhysUnits& operator=(const CUnitsTreeEntryGrpPhysUnits& i_other) = delete;
    CUnitsTreeEntryGrpPhysUnits& operator=(CUnitsTreeEntryGrpPhysUnits&& i_other) = delete;
public: // operators
    bool operator == ( const CUnitsTreeEntryGrpPhysUnits& i_physsizeOther ) const;
    bool operator != ( const CUnitsTreeEntryGrpPhysUnits& i_physsizeOther ) const;
public: // instance methods
    bool initialize( bool i_bCreateFindBestChainedList );
    void addFctConvert(
        CUnitsTreeEntryGrpPhysUnits* i_pPhysSizeDst,
        CUnitsTreeEntryGrpPhysUnits* i_pPhysSizeRef,
        EFctConvert i_fctConvert );
public: // instance methods
    QString getSIUnitName() const { return m_strSIUnitName; }
    QString getSIUnitSymbol() const { return m_strSIUnitSymbol; }
    CUnitsTreeEntryPhysUnit* getSIUnit() const { return m_pPhysUnitSI; }
    QString getFormulaSymbol() const { return m_strFormulaSymbol; }
    bool isPowerRelated() const { return m_bIsPowerRelated; }
public: // instance methods
    CUnitsTreeEntryPhysUnit* getPhysUnit( int i_idx ) const;
    CUnitsTreeEntryPhysUnit* findPhysUnit( const QString& i_strSymbolOrName ) const;
    CUnitsTreeEntryPhysUnit* findPhysUnitBySymbol( const QString& i_strSymbol ) const;
    CUnitsTreeEntryPhysUnit* findPhysUnitByName( const QString& i_strName ) const;
public: // overridables
    virtual double getRefVal( CUnitsTreeEntryPhysUnit* i_pPhysUnitRef = nullptr ) const;
protected: // instance members
    QString m_strSIUnitName;
    QString  m_strSIUnitSymbol;
    CUnitsTreeEntryPhysUnit* m_pPhysUnitSI;
    QString m_strFormulaSymbol;
    bool m_bIsPowerRelated;
    bool m_bInitialized;

}; // class CUnitsTreeEntryGrpPhysUnits

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysTreeEntryGrpPhysUnits_h
