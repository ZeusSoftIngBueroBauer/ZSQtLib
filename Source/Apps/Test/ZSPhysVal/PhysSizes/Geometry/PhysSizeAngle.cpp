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

#include "PhysSizes/Geometry/PhysSizeAngle.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::System::Math;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeAngle : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // class members

==============================================================================*/

const QString CPhysSizeAngle::c_strSymbolPhi(QChar(0x03C6));

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeAngle::CPhysSizeAngle( CIdxTreePhysSizes* i_pIdxTree ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* pIdxTree         */ i_pIdxTree,
        /* strName          */ "Angle",
        /* strSIUnitName    */ "Radiant",
        /* strSIUnitSymbol  */ "rad",
        /* strFormulaSymbol */ QString(QChar(0x03C6)),
        /* bIsPowerRelated  */ false ),
    Rad(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Radiant",
        /* strSymbol      */ "rad",
        /* fMFromSI       */ 1.0 ),
    Degree(
      /* pPhysSize      */ this,
      /* bIsLogarithmic */ false,
      /* strName        */ "Degree",
      /* strSymbol      */ QString::fromLatin1("°"),
      /* fMFromSI       */ 180.0/c_fPI )
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(false);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeAngle::CPhysSizeAngle( CIdxTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Angle",
        /* strSIUnitName    */ "Radiant",
        /* strSIUnitSymbol  */ "rad",
        /* strFormulaSymbol */ QString(QChar(0x03C6)),
        /* bIsPowerRelated  */ false ),
    Rad(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Radiant",
        /* strSymbol      */ "rad",
        /* fMFromSI       */ 1.0 ),
    Degree(
      /* pPhysSize      */ this,
      /* bIsLogarithmic */ false,
      /* strName        */ "Degree",
      /* strSymbol      */ QString::fromLatin1("°"),
      /* fMFromSI       */ 180.0/c_fPI )
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(false);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeAngle::~CPhysSizeAngle()
//------------------------------------------------------------------------------
{
} // dtor
