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

#ifndef ZSPhysVal_PhysSize_h
#define ZSPhysVal_PhysSize_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSPhysVal/ZSPhysUnitsFctConvert.h"
#include "ZSPhysVal/ZSPhysUnitsGrp.h"

namespace ZS
{
namespace PhysVal
{
class CPhysScienceFieldTreeEntry;
class CPhysUnit;

//******************************************************************************
class ZSPHYSVALDLL_API CPhysSizeTreeEntry : public CUnitGrpTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysSizeTreeEntry"; }
public: // ctors and dtor
    CPhysSizeTreeEntry(
        CIdxTreePhysSizes* i_pIdxTree,
        const QString&     i_strName,
        const QString&     i_strSIUnitName,
        const QString&     i_strSIUnitSymbol,
        const QString&     i_strFormulaSymbol,
        bool               i_bIsPowerRelated );
    CPhysSizeTreeEntry(
        CPhysScienceFieldTreeEntry* i_pParentBranch,
        const QString& i_strName,
        const QString& i_strSIUnitName,
        const QString& i_strSIUnitSymbol,
        const QString& i_strFormulaSymbol,
        bool           i_bIsPowerRelated );
    CPhysSizeTreeEntry(CPhysSizeTreeEntry&& i_other) = delete;
    CPhysSizeTreeEntry(CPhysSizeTreeEntry& i_other) = delete;
    CPhysSizeTreeEntry(const CPhysSizeTreeEntry& i_other) = delete;
    virtual ~CPhysSizeTreeEntry();
public: // operators
    CPhysSizeTreeEntry& operator=(CPhysSizeTreeEntry& i_other) = delete;
    CPhysSizeTreeEntry& operator=(const CPhysSizeTreeEntry& i_other) = delete;
    CPhysSizeTreeEntry& operator=(CPhysSizeTreeEntry&& i_other) = delete;
public: // operators
    bool operator == ( const CPhysSizeTreeEntry& i_physsizeOther ) const;
    bool operator != ( const CPhysSizeTreeEntry& i_physsizeOther ) const;
public: // instance methods
    bool initialize( bool i_bCreateFindBestChainedList );
    void addFctConvert(
        CPhysSizeTreeEntry*  i_pPhysSizeDst,
        CPhysSizeTreeEntry*  i_pPhysSizeRef,
        EFctConvert i_fctConvert );
public: // instance methods
    QString getSIUnitName() const { return m_strSIUnitName; }
    QString getSIUnitSymbol() const { return m_strSIUnitSymbol; }
    CPhysUnitTreeEntry* getSIUnit() const { return m_pPhysUnitSI; }
    QString getFormulaSymbol() const { return m_strFormulaSymbol; }
    bool isPowerRelated() const { return m_bIsPowerRelated; }
public: // instance methods
    CPhysUnitTreeEntry* getPhysUnit( int i_idx ) const;
    CPhysUnitTreeEntry* findPhysUnit( const QString& i_strSymbolOrName ) const;
    CPhysUnitTreeEntry* findPhysUnitBySymbol( const QString& i_strSymbol ) const;
    CPhysUnitTreeEntry* findPhysUnitByName( const QString& i_strName ) const;
public: // overridables
    virtual double getRefVal( CPhysUnitTreeEntry* i_pPhysUnitRef = nullptr ) const;
protected: // instance members
    QString m_strSIUnitName;
    QString  m_strSIUnitSymbol;
    CPhysUnitTreeEntry* m_pPhysUnitSI;
    QString m_strFormulaSymbol;
    bool m_bIsPowerRelated;
    bool m_bInitialized;

}; // class CPhysSizeTreeEntry

//******************************************************************************
class ZSPHYSVALDLL_API CPhysSize : public CUnitGrp
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysSize"; }
public: // ctors and dtor
    CPhysSize();
    CPhysSize(CPhysSize* i_pPhysSize);
    CPhysSize(const CPhysSize* i_pPhysSize);
    CPhysSize(CPhysSize& i_physSize);
    CPhysSize(const CPhysSize& i_physSize);
    CPhysSize(CPhysSizeTreeEntry* i_pPhysSize);
    CPhysSize(const CPhysSizeTreeEntry* i_pPhysSize);
    CPhysSize(CPhysSizeTreeEntry& i_physSize);
    CPhysSize(const CPhysSizeTreeEntry& i_physSize);
    CPhysSize(const QString& i_strUniqueName);
    virtual ~CPhysSize();
public: // operators
    bool operator == ( const CPhysSize& i_other ) const;
    bool operator != ( const CPhysSize& i_other ) const;
public: // overridables of base class CUnitGrp
    virtual bool isValid() const override;
public: // instance methods
    QString getSIUnitName() const;
    QString getSIUnitSymbol() const;
    CPhysUnit getSIUnit() const;
    QString getFormulaSymbol() const;
    bool isPowerRelated() const;
public: // instance methods
    CPhysUnit getPhysUnit( int i_idx ) const;
    CPhysUnit findPhysUnit( const QString& i_strSymbolOrName ) const;
    CPhysUnit findPhysUnitBySymbol( const QString& i_strSymbol ) const;
    CPhysUnit findPhysUnitByName( const QString& i_strName ) const;
public: // overridables
    //virtual double getRefVal( CPhysUnitTreeEntry* i_pPhysUnitRef = nullptr ) const;

}; // class CPhysSize

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysSize_h
