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
//class CIdxTreePhysSizes;

//******************************************************************************
class ZSPHYSVALDLL_API CPhysScienceField : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysScienceField(const QString& i_strName, ZS::System::CIdxTree* i_pIdxTree = nullptr);
    CPhysScienceField(const QString& i_strName, ZS::System::CIdxTreeEntry* i_pParentBranch);
    virtual ~CPhysScienceField();
protected: // instance members
    /*!< If this instance needed to create the physical sizes index tree
         the index tree must be released if the physical science field
         instance is destroyed. The reference counter is set to one
         if the index tree was created by this science field. */
    int m_iIdxTreeRefCount;

}; // class CPhysScienceField

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysScienceField_h
