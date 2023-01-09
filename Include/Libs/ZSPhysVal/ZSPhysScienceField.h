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

#ifndef ZSPhysVal_PhysScienceField_h
#define ZSPhysVal_PhysScienceField_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qhash.h>
#include <QtCore/qvector.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;

//******************************************************************************
class ZSPHYSVALDLL_API CPhysScienceField : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysScienceField"; }
public: // ctors and dtor
    CPhysScienceField(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree, EPhysScienceField i_scienceField);
    CPhysScienceField(ZS::System::CIdxTreeEntry* i_pParentBranch, EPhysScienceField i_scienceField);
    CPhysScienceField(CPhysScienceField&& i_other) = delete;
    CPhysScienceField(CPhysScienceField& i_other) = delete;
    CPhysScienceField(const CPhysScienceField& i_other) = delete;
    virtual ~CPhysScienceField();
public: // operators
    CPhysScienceField& operator=(CPhysScienceField& i_other) = delete;
    CPhysScienceField& operator=(const CPhysScienceField& i_other) = delete;
    CPhysScienceField& operator=(CPhysScienceField&& i_other) = delete;

}; // class CPhysScienceField

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysScienceField_h
