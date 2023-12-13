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

#include "Units/Electricity/Electricity.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysScienceFieldElectricity : public CUnitsTreeEntryGrpScienceField
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

*/
CPhysScienceFieldElectricity::CPhysScienceFieldElectricity(CIdxTreeEntry* i_pParentBranch) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpScienceField(i_pParentBranch, EPhysScienceField::Electricity),
    Current(this),
    Power(this),
    Resistance(this),
    Voltage(this)
{
    // Add conversion routines to convert units between different physical sizes
    //==========================================================================

    //Current.addFctConvert( // P = (I*I)*R
    //    /* pPhysSizeDst   */ &Power,
    //    /* pPhysSizeRef   */ &Resistance,
    //    /* fctConvert     */ EFctConvert_SQRxDIVr );

    //Current.addFctConvert( // U = I*R
    //    /* pPhysSizeDst   */ &Voltage,
    //    /* pPhysSizeRef   */ &Resistance,
    //    /* fctConvert     */ EFctConvert_xMULr );

    //Power.addFctConvert( // U = sqrt(P*R)
    //    /* pPhysSizeDst   */ &Voltage,
    //    /* pPhysSizeRef   */ &Resistance,
    //    /* fctConvert     */ EFctConvert_SQRT_xMULr_ );

    //Power.addFctConvert( // I = sqrt(P/R)
    //    /* pPhysSizeDst   */ &Current,
    //    /* pPhysSizeRef   */ &Resistance,
    //    /* fctConvert     */ EFctConvert_SQRT_xDIVr_ );

    //Voltage.addFctConvert( // P = (U*U)/R
    //    /* pPhysSizeDst   */ &Power,
    //    /* pPhysSizeRef   */ &Resistance,
    //    /* fctConvert     */ EFctConvert_SQRxDIVr );

    //Voltage.addFctConvert( // I = U/R
    //    /* pPhysSizeDst   */ &Current,
    //    /* pPhysSizeRef   */ &Resistance,
    //    /* fctConvert     */ EFctConvert_xDIVr );

    // As this phyiscal science field is not added below the root node we
    // need to add a shortcut so that the units can be accessed without the
    // name of the parent branch (which is "Science Fields" for this application).
    i_pParentBranch->tree()->addShortcut(this, "Electricity");

} // ctor

//------------------------------------------------------------------------------
CPhysScienceFieldElectricity::~CPhysScienceFieldElectricity()
//------------------------------------------------------------------------------
{
} // dtor
