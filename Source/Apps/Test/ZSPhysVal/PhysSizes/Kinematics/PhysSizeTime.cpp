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

#include "PhysSizes/Kinematics/PhysSizeTime.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeTime : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeTime::CPhysSizeTime( CIdxTreePhysSizes* i_pIdxTree ) :
//------------------------------------------------------------------------------
    CPhysSizeTreeEntry(
        /* pIdxTree         */ i_pIdxTree,
        /* strName          */ "Time",
        /* strSIUnitName    */ "Seconds",
        /* strSIUnitSymbol  */ "s",
        /* strFormulaSymbol */ "t",
        /* bIsPowerRelated  */ false ),
    m_treeEntryNanoSeconds(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_treeEntryMicroSeconds(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_treeEntryMilliSeconds(
        /* pPhysSize */ this,
        /* strSymbol */ c_strPrefixMilli ),
    m_treeEntrySeconds(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntryMinutes(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Minutes",
        /* strSymbol      */ "m",
        /* fMFromSI       */ 1.0/60.0 ),
    m_treeEntryHours(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Hours",
        /* strSymbol      */ "h",
        /* fMFromSI       */ 1.0/3600.0 ),
    m_treeEntryDays(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Days",
        /* strSymbol      */ "d",
        /* fMFromSI       */ 1.0/(24.0*3600.0) ),
    NanoSeconds(m_treeEntryNanoSeconds),
    MicroSeconds(m_treeEntryMicroSeconds),
    MilliSeconds(m_treeEntryMilliSeconds),
    Seconds(m_treeEntrySeconds),
    Minutes(m_treeEntryMinutes),
    Hours(m_treeEntryHours),
    Days(m_treeEntryDays),
    ns(m_treeEntryNanoSeconds),
    us(m_treeEntryMicroSeconds),
    ms(m_treeEntryMilliSeconds),
    s(m_treeEntrySeconds),
    m(m_treeEntryMinutes),
    h(m_treeEntryHours),
    d(m_treeEntryDays)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeTime::CPhysSizeTime( CPhysScienceFieldTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CPhysSizeTreeEntry(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Time",
        /* strSIUnitName    */ "Seconds",
        /* strSIUnitSymbol  */ "s",
        /* strFormulaSymbol */ "t",
        /* bIsPowerRelated  */ false ),
    m_treeEntryNanoSeconds(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_treeEntryMicroSeconds(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_treeEntryMilliSeconds(
        /* pPhysSize */ this,
        /* strSymbol */ c_strPrefixMilli ),
    m_treeEntrySeconds(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntryMinutes(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Minutes",
        /* strSymbol      */ "m",
        /* fMFromSI       */ 1.0/60.0 ),
    m_treeEntryHours(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Hours",
        /* strSymbol      */ "h",
        /* fMFromSI       */ 1.0/3600.0 ),
    m_treeEntryDays(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Days",
        /* strSymbol      */ "d",
        /* fMFromSI       */ 1.0/(24.0*3600.0) ),
    NanoSeconds(m_treeEntryNanoSeconds),
    MicroSeconds(m_treeEntryMicroSeconds),
    MilliSeconds(m_treeEntryMilliSeconds),
    Seconds(m_treeEntrySeconds),
    Minutes(m_treeEntryMinutes),
    Hours(m_treeEntryHours),
    Days(m_treeEntryDays),
    ns(m_treeEntryNanoSeconds),
    us(m_treeEntryMicroSeconds),
    ms(m_treeEntryMilliSeconds),
    s(m_treeEntrySeconds),
    m(m_treeEntryMinutes),
    h(m_treeEntryHours),
    d(m_treeEntryDays)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeTime::~CPhysSizeTime()
//------------------------------------------------------------------------------
{
} // dtor
