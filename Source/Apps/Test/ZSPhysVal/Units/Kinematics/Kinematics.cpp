/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "Units/Kinematics/Kinematics.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysScienceFieldKinematics : public CUnitsTreeEntryGrpScienceField
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

*/
CPhysScienceFieldKinematics::CPhysScienceFieldKinematics(CIdxTreeEntry* i_pParentBranch) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpScienceField(i_pParentBranch, EPhysScienceField::Kinematics),
    BitRate(this),
    Frequency(this),
    Samples(this),
    Time(this),
    Velocity(this)
{
    // As this phyiscal science field is not added below the root node we
    // need to add a shortcut so that the units can be accessed without the
    // name of the parent branch (which is "Science Fields" for this application).
    i_pParentBranch->tree()->addShortcut(this, "Kinematics");
}

//------------------------------------------------------------------------------
CPhysScienceFieldKinematics::~CPhysScienceFieldKinematics()
//------------------------------------------------------------------------------
{
}
