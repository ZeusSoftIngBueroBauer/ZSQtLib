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

#ifndef ZSPhysVal_PhysUnitsRatio_h
#define ZSPhysVal_PhysUnitsRatio_h

#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsGrp.h"

namespace ZS
{
namespace PhysVal
{
//******************************************************************************
class ZSPHYSVALDLL_API CUnitRatioTreeEntry : public CUnitTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitRatioTreeEntry"; }
public: // ctors and dtor
    CUnitRatioTreeEntry(
        CUnitGrpTreeEntry* i_pUnitGrp,
        const QString&     i_strName,
        const QString&     i_strSymbol,
        double             i_fFactor );
    virtual ~CUnitRatioTreeEntry();
public: // instance methods
    double factor() const;
public: // overridables of base class CUnitTreeEntry (converting values)
    virtual bool isConvertible( const CUnitTreeEntry* i_pUnitDst, double i_fVal = 1.0 ) const override;
    virtual double convertValue( double i_fVal, const CUnitTreeEntry* i_pUnitDst ) const override;
protected: // instance members
    double m_fFactor;

}; // class CUnitRatioTreeEntry

//******************************************************************************
class ZSPHYSVALDLL_API CUnitRatio : public CUnit
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitRatio"; }
public: // ctors and dtor
    CUnitRatio();
    CUnitRatio(CUnitRatio* i_pUnitRatio);
    CUnitRatio(const CUnitRatio* i_unitRatio);
    CUnitRatio(CUnitRatio& i_unitRatio);
    CUnitRatio(const CUnitRatio& i_unitRatio);
    CUnitRatio(CUnitRatioTreeEntry* i_pUnitRatio);
    CUnitRatio(const CUnitRatioTreeEntry* i_pUnitRatio);
    CUnitRatio(CUnitRatioTreeEntry& i_unitRatio);
    CUnitRatio(const CUnitRatioTreeEntry& i_unitRatio);
    CUnitRatio(const QString& i_strUniqueName);
    virtual ~CUnitRatio();
public: // operators
    bool operator == ( const CUnitRatio& i_other ) const;
    bool operator != ( const CUnitRatio& i_other ) const;
public: // instance methods
    double factor() const;
};

#if 0

//******************************************************************************
class ZSPHYSVALDLL_API CUnitGrpRatioTreeEntry : public CUnitGrpTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitGrpRatioTreeEntry"; }
public: // ctors and dtor
    CUnitGrpRatioTreeEntry(ZS::System::CIdxTree* i_pIdxTree);
    virtual ~CUnitGrpRatioTreeEntry();
public: // instance methods to access the ratio units
    CUnitRatioTreeEntry* PerOne() { return &m_unitRatioPerOne; }
    CUnitRatioTreeEntry* PerCent() { return &m_unitRatioPerCent; }
    CUnitRatioTreeEntry* PerMille() { return &m_unitRatioPerMille; }
    CUnitRatioTreeEntry* dB() { return &m_unitRatioDezibel; }
    CUnitRatioTreeEntry* Dezibel() { return &m_unitRatioDezibel; }
public: // instance methods
    CUnitRatioTreeEntry* unit( int i_idx );
    CUnitRatioTreeEntry* findUnitBySymbol( const QString& i_strSymbol );
    CUnitRatioTreeEntry* findUnitByName( const QString& i_strName );
private: // copy ctor not allowed
    CUnitGrpRatioTreeEntry( const CUnitGrpRatio& );
private: // assignment operator not allowed
    CUnitGrpRatioTreeEntry& operator = ( const CUnitGrpRatio& );
protected: // instance members
    CUnitRatioTreeEntry  m_unitRatioPerOne;
    CUnitRatioTreeEntry  m_unitRatioPerCent;
    CUnitRatioTreeEntry  m_unitRatioPerMille;
    CUnitRatioTreeEntry  m_unitRatioDezibel;

}; // class CUnitGrpRatioTreeEntry

#endif

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnitsRatio_h
