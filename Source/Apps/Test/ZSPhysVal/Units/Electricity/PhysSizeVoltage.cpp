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

#include "Units/Electricity/PhysSizeVoltage.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeVoltage : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeVoltage::CPhysSizeVoltage( CUnitsTreeEntryGrpScienceField* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Voltage",
        /* strSIUnitName    */ "Volt",
        /* strSIUnitSymbol  */ "V",
        /* strFormulaSymbol */ "U",
        /* bIsPowerRelated  */ false ),
    m_treeEntryPicoVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    m_treeEntryNanoVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_treeEntryMicroVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_treeEntryMilliVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_treeEntryVolt(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntryKiloVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    m_treeEntryMegaVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    m_treeEntrydBVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBVolt",
        /* strSymbol      */ "dBV",
        /* fRefVal        */ 1.0 ),
    m_treeEntrydBu(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB(0,775V)",
        /* strSymbol      */ "dBu",
        /* fRefVal        */ c_fRefValdBu_0775V ),
    m_treeEntrydBMilliVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strMilli) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMilli) + "V",
        /* fRefVal        */ c_fFactorMilli ),
    m_treeEntrydBMicroVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strMicro) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMicro) + "V",
        /* fRefVal        */ c_fFactorMicro ),
    m_treeEntrydBNanoVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strNano) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixNano) + "V",
        /* fRefVal        */ c_fFactorNano ),
    m_treeEntrydBPicoVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strPico) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixPico) + "V",
        /* fRefVal        */ c_fFactorPico ),
    PicoVolt(m_treeEntryPicoVolt),
    NanoVolt(m_treeEntryNanoVolt),
    MicroVolt(m_treeEntryMicroVolt),
    MilliVolt(m_treeEntryMilliVolt),
    Volt(m_treeEntryVolt),
    KiloVolt(m_treeEntryKiloVolt),
    MegaVolt(m_treeEntryMegaVolt),
    dBVolt(m_treeEntrydBVolt),
    dBu(m_treeEntrydBu),
    dBMilliVolt(m_treeEntrydBMilliVolt),
    dBMicroVolt(m_treeEntrydBMicroVolt),
    dBNanoVolt(m_treeEntrydBNanoVolt),
    dBPicoVolt(m_treeEntrydBPicoVolt),
    pV(m_treeEntryPicoVolt),
    nV(m_treeEntryNanoVolt),
    uV(m_treeEntryMicroVolt),
    mV(m_treeEntryMilliVolt),
    V(m_treeEntryVolt),
    kV(m_treeEntryKiloVolt),
    MV(m_treeEntryMegaVolt),
    dBV(m_treeEntrydBVolt),
    dBmV(m_treeEntrydBMilliVolt),
    dBuV(m_treeEntrydBMicroVolt),
    dBnV(m_treeEntrydBNanoVolt),
    dBpV(m_treeEntrydBPicoVolt)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

    // To allow "short" unit strings like "Voltage.mV" we add a shortcut to this phyiscal size.
    i_pParentBranch->tree()->addShortcut(this, "Voltage");

    // To allow "short" unit strings like "mA" we add shortcuts to each unit.
    i_pParentBranch->tree()->addShortcut(&m_treeEntryMilliVolt, "mV");
    i_pParentBranch->tree()->addShortcut(&m_treeEntryVolt, "V");
    i_pParentBranch->tree()->addShortcut(&m_treeEntryKiloVolt, "kV");

} // ctor

//------------------------------------------------------------------------------
CPhysSizeVoltage::~CPhysSizeVoltage()
//------------------------------------------------------------------------------
{
} // dtor
