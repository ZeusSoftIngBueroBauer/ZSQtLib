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

#include "Units/Geometry/PhysSizeGraphDevice.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeGraphDevice : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeGraphDevice::CPhysSizeGraphDevice( CUnitsTreeEntryGrpScienceField* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "GraphDevice",
        /* strSIUnitName    */ "Pixel",
        /* strSIUnitSymbol  */ "px",
        /* strFormulaSymbol */ "px",
        /* bIsPowerRelated  */ false ),
    m_treeEntryPixel(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    Pixel(m_treeEntryPixel),
    px(m_treeEntryPixel)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    initialize(true);

    // Link the units to a chained list for the "findBestUnit" functionality:
    //======================================================================

    // Just one unit and therefore nothing to chain.

} // ctor

//------------------------------------------------------------------------------
CPhysSizeGraphDevice::~CPhysSizeGraphDevice()
//------------------------------------------------------------------------------
{
} // dtor
