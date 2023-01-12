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

#include "ZSPhysVal/ZSPhysScienceField.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CPhysScienceFieldTreeEntry : public ZS::System::CIdxTreeEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of a physical science field.

    The constructor will add itself to an index tree of physical sizes.

    If no index tree is passed the constructor will look for an index tree
    with the default name "PhysSizes". If also this index tree is not yet
    existing the constructor will create an index tree with the default name.

    @param i_strName [in]
        e.g. "Kinematics", "Electricity", "Geometry", ...
    @param i_pIdxTree [in]
        Pointer to index tree the science field should be added to.
        If nullptr is passed the constructor first checks whether
        an index tree with the default name "PhysSizes" is existing.
        If not an index tree with the default name will be created.
        The destructor will again release the index tree in this case.
*/
CPhysScienceFieldTreeEntry::CPhysScienceFieldTreeEntry(
    CIdxTreePhysSizes* i_pIdxTree, EPhysScienceField i_scienceField ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, CEnumPhysScienceField(i_scienceField).toString())
{
    i_pIdxTree->add(this);

} // ctor

//------------------------------------------------------------------------------
/*! @brief Creates an instance of a physical science field.

    The constructor will add itself to an index tree of physical sizes.

    If no index tree is passed the constructor will look for an index tree
    with the default name "PhysSizes". If also this index tree is not yet
    existing the constructor will create an index tree with the default name.

    @param i_strName [in]
        e.g. "Kinematics", "Electricity", "Geometry", ...
    @param i_pIdxTree [in]
        Pointer to index tree the science field should be added to.
        If nullptr is passed the constructor first checks whether
        an index tree with the default name "PhysSizes" is existing.
        If not an index tree with the default name will be created.
        The destructor will again release the index tree in this case.
*/
CPhysScienceFieldTreeEntry::CPhysScienceFieldTreeEntry(
    CUnitGrpTreeEntry* i_pParentBranch, EPhysScienceField i_scienceField ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, CEnumPhysScienceField(i_scienceField).toString())
{
    i_pParentBranch->tree()->add(this, i_pParentBranch);

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the science field instance.

    The instance will be removed from the index tree of physical sizes.

    If the constructor created an index tree with the default name
    the index tree will be released by the destructor.
*/
CPhysScienceFieldTreeEntry::~CPhysScienceFieldTreeEntry()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CPhysScienceField
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField() :
//------------------------------------------------------------------------------
    m_pTreeEntry(nullptr),
    m_strUniqueName()
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(CPhysScienceField* i_pScienceField) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pScienceField->m_pTreeEntry),
    m_strUniqueName(i_pScienceField->m_strUniqueName)
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(const CPhysScienceField* i_pScienceField) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pScienceField->m_pTreeEntry),
    m_strUniqueName(i_pScienceField->m_strUniqueName)
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(CPhysScienceField& i_scienceField) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_scienceField.m_pTreeEntry),
    m_strUniqueName(i_scienceField.m_strUniqueName)
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(const CPhysScienceField& i_scienceField) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_scienceField.m_pTreeEntry),
    m_strUniqueName(i_scienceField.m_strUniqueName)
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(CPhysScienceFieldTreeEntry* i_pScienceField) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pScienceField),
    m_strUniqueName(i_pScienceField->keyInTree())
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(const CPhysScienceFieldTreeEntry* i_pScienceField) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CPhysScienceFieldTreeEntry*>(i_pScienceField)),
    m_strUniqueName(i_pScienceField->keyInTree())
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(CPhysScienceFieldTreeEntry& i_scienceField) :
//------------------------------------------------------------------------------
    m_pTreeEntry(&i_scienceField),
    m_strUniqueName(i_scienceField.keyInTree())
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(const CPhysScienceFieldTreeEntry& i_scienceField) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CPhysScienceFieldTreeEntry*>(&i_scienceField)),
    m_strUniqueName(i_scienceField.keyInTree())
{
}

//------------------------------------------------------------------------------
CPhysScienceField::CPhysScienceField(const QString& i_strUniqueName) :
//------------------------------------------------------------------------------
    m_pTreeEntry(CIdxTreePhysSizes::GetInstance()->findPhysScienceField(i_strUniqueName)),
    m_strUniqueName(i_strUniqueName)
{
}

//------------------------------------------------------------------------------
CPhysScienceField::~CPhysScienceField()
//------------------------------------------------------------------------------
{
    m_pTreeEntry = nullptr;
    //m_strUniqueName;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysScienceField::operator == ( const CPhysScienceField& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = (m_pTreeEntry == i_other.m_pTreeEntry);
    if( bEqual ) bEqual = (m_strUniqueName.compare(i_other.m_strUniqueName) == 0);
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysScienceField::operator != ( const CPhysScienceField& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // overridables of base class CUnitGrp
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysScienceField::isValid() const
//------------------------------------------------------------------------------
{
    return (m_pTreeEntry != nullptr);
}

