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
#include "ZSPhysVal/ZSPhysUnitsGrp.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;

//******************************************************************************
class ZSPHYSVALDLL_API CPhysScienceFieldTreeEntry : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysScienceFieldTreeEntry"; }
public: // ctors and dtor
    CPhysScienceFieldTreeEntry(CIdxTreePhysSizes* i_pIdxTree, EPhysScienceField i_scienceField);
    CPhysScienceFieldTreeEntry(CUnitGrpTreeEntry* i_pParentBranch, EPhysScienceField i_scienceField);
    CPhysScienceFieldTreeEntry(CPhysScienceFieldTreeEntry&& i_other) = delete;
    CPhysScienceFieldTreeEntry(CPhysScienceFieldTreeEntry& i_other) = delete;
    CPhysScienceFieldTreeEntry(const CPhysScienceFieldTreeEntry& i_other) = delete;
    virtual ~CPhysScienceFieldTreeEntry();
public: // operators
    CPhysScienceFieldTreeEntry& operator=(CPhysScienceFieldTreeEntry& i_other) = delete;
    CPhysScienceFieldTreeEntry& operator=(const CPhysScienceFieldTreeEntry& i_other) = delete;
    CPhysScienceFieldTreeEntry& operator=(CPhysScienceFieldTreeEntry&& i_other) = delete;

}; // class CPhysScienceFieldTreeEntry


//******************************************************************************
class ZSPHYSVALDLL_API CPhysScienceField
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysScienceField"; }
public: // ctors and dtor
    CPhysScienceField();
    CPhysScienceField(CPhysScienceField* i_pScienceField);
    CPhysScienceField(const CPhysScienceField* i_pScienceField);
    CPhysScienceField(CPhysScienceField& i_scienceField);
    CPhysScienceField(const CPhysScienceField& i_scienceField);
    CPhysScienceField(CPhysScienceFieldTreeEntry* i_pScienceField);
    CPhysScienceField(const CPhysScienceFieldTreeEntry* i_pScienceField);
    CPhysScienceField(CPhysScienceFieldTreeEntry& i_scienceField);
    CPhysScienceField(const CPhysScienceFieldTreeEntry& i_scienceField);
    CPhysScienceField(const QString& i_strUniqueName);
    virtual ~CPhysScienceField();
public: // operators
    bool operator == ( const CPhysScienceField& i_other ) const;
    bool operator != ( const CPhysScienceField& i_other ) const;
public: // overridables
    virtual bool isValid() const;
protected: // instance members
    CPhysScienceFieldTreeEntry* m_pTreeEntry;
    QString m_strUniqueName;

}; // class CPhysScienceFieldTreeEntry

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysScienceField_h
