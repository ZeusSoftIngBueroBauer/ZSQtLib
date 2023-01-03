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
//******************************************************************************
class ZSPHYSVALDLL_API CPhysSize : public CUnitGrp
//******************************************************************************
{
public: // ctors and dtor
    CPhysSize(
        CUnitsPool*       i_pUnitsPool,
        EPhysScienceField i_scienceField,
        const QString&    i_strName,           // e.g. "Time", "Length", "Power", "Voltage", ...
        const QString&    i_strSIUnitName,     // e.g. "Second" for Time, "Meter" for Length, "Watt" for el. Power, "Volt" for el. Voltage, ...
        const QString&    i_strSIUnitSymbol,   // e.g. "s" for Second, "m" for Meter, "W" for "Watt", "V" for el. Volt, ...
        const QString&    i_strFormulaSymbol,  // e.g. "t" for Time, "l" for Length, "P" for Power, "U" for Voltage, ...
        bool              i_bIsPowerRelated ); // true if X/dB = 10*log(X/X0), false if e.g. X/dB = 20*log(X/X0)
    virtual ~CPhysSize();
public: // operators
    bool operator == ( const CPhysSize& i_physsizeOther ) const;
    bool operator != ( const CPhysSize& i_physsizeOther ) const;
public: // instance methods
    bool initialize( bool i_bCreateFindBestChainedList );
    void addFctConvert(
        CPhysSize*  i_pPhysSizeDst,
        CPhysSize*  i_pPhysSizeRef,
        EFctConvert i_fctConvert );
public: // instance methods
    QString getSIUnitName() const { return m_strSIUnitName; }
    QString getSIUnitSymbol() const { return m_strSIUnitSymbol; }
    CPhysUnit* getSIUnit() const { return m_pPhysUnitSI; }
    QString getFormulaSymbol() const { return m_strFormulaSymbol; }
    bool isPowerRelated() const { return m_bIsPowerRelated; }
    int getPhysUnitCount() const { return m_vecpUnits.size(); }
    CPhysUnit* getPhysUnit( int i_idx );
    CPhysUnit* findPhysUnit( const QString& i_strSymbolOrName );
    CPhysUnit* findPhysUnitBySymbol( const QString& i_strSymbol );
    CPhysUnit* findPhysUnitByName( const QString& i_strName );
public: // overridables
    virtual double getRefVal( CPhysUnit* i_pPhysUnitRef = nullptr ) const; // nullptr to specify the SI unit
private: // default and copy ctor not allowed
    CPhysSize();
    CPhysSize( const CPhysSize& );
private: // assignment operator not allowed
    CPhysSize& operator = ( const CPhysSize& );
protected: // instance members
    QString    m_strSIUnitName;
    QString    m_strSIUnitSymbol;
    CPhysUnit* m_pPhysUnitSI;
    QString    m_strFormulaSymbol;
    bool       m_bIsPowerRelated;
    bool       m_bInitialized;

}; // class CPhysSize

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysSize_h
