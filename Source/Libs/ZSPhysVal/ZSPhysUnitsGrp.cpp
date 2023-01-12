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

#include "ZSPhysVal/ZSPhysUnitsGrp.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitGrpTreeEntry : CIdxTreeEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a unit group.

    The constructor will add itself as a child of the root node
    to the given index tree.

    @param i_pIdxTree
    @param i_classType
        [PhysSize, Ratios, UserDefinedQuantities]
    @param i_strName
        e.g. "Time", "Power", "Voltage", "Current", ...
*/
CUnitGrpTreeEntry::CUnitGrpTreeEntry(
    CIdxTreePhysSizes* i_pIdxTree,
    EUnitClassType     i_classType,
    const QString&     i_strName ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, i_strName),
    m_classType(EUnitClassType::Undefined),
    m_hshpUnitsBySymbol()
{
    i_pIdxTree->add(this);

} // ctor

//------------------------------------------------------------------------------
/*! @brief Creates a unit group.

    The constructor will add itself to an index tree of physical sizes
    as a child of the passed parent branch.

    @param i_pParentBranch
    @param i_classType
        [PhysSize, Ratios, UserDefinedQuantities]
    @param i_strName
        e.g. "Time", "Power", "Voltage", "Current", ...
*/
CUnitGrpTreeEntry::CUnitGrpTreeEntry(
    CIdxTreeEntry* i_pParentBranch,
    EUnitClassType i_classType,
    const QString& i_strName ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, i_strName),
    m_classType(i_classType),
    //m_pUnitGrpParent(nullptr),
    //m_vecpUnitGrpChilds(),
    //m_hshpUnitGrpChilds(),
    //m_vecpUnits(),
    m_hshpUnitsBySymbol()
{
    i_pParentBranch->tree()->add(this, i_pParentBranch);

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the unit group.

    The instance will be removed from the index tree.

    If the constructor created an index tree with the default name
    the index tree will be released by the destructor.
*/
CUnitGrpTreeEntry::~CUnitGrpTreeEntry()
//------------------------------------------------------------------------------
{
    //if( m_vecpUnits.count() > 0 )
    //{
    //    for( int idxUnit = m_vecpUnits.count()-1; idxUnit >= 0; idxUnit-- )
    //    {
    //        CUnit* pUnit = m_vecpUnits[idxUnit];
    //        removeUnit(idxUnit);
    //        try {
    //            delete pUnit;
    //        }
    //        catch(...) {
    //        }
    //        pUnit = nullptr;
    //    }
    //}

    // Recursively remove and delete the child groups ..
    //if( m_vecpUnitGrpChilds.count() > 0 )
    //{
    //    for( int idxChild = m_vecpUnitGrpChilds.count()-1; idxChild >= 0; idxChild-- )
    //    {
    //        CUnitGrpTreeEntry* pUnitGrp = m_vecpUnitGrpChilds[idxChild];

    //        try {
    //            // The dtor of the unit group also deletes child unit groups or units belonging to the unit group
    //            // and also calls "removeChildUnitGrp" as a reentry.
    //            delete pUnitGrp;
    //        }
    //        catch(...) {
    //        }
    //        pUnitGrp = nullptr;
    //    }
    //}

    // Attention: on removing this from the index tree the member m_pTree
    // will be set to nullptr. To release the tree later on we need to store
    // the pointer in a stack variable.

    //// Inform parent that the unit group has been destroyed.
    //if( m_pUnitGrpParent != nullptr )
    //{
    //    m_pUnitGrpParent->removeChildUnitGrp(m_strName);
    //}

    m_classType = static_cast<EUnitClassType>(0);
    //m_pUnitGrpParent = nullptr;
    //m_vecpUnitGrpChilds;
    //m_hshpUnitGrpChilds;
    //m_vecpUnits;
    //m_hshpUnitsBySymbol.clear();

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitGrpTreeEntry::operator == ( const CUnitGrpTreeEntry& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    if( this != &i_physsizeOther )
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
bool CUnitGrpTreeEntry::operator != ( const CUnitGrpTreeEntry& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physsizeOther );
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CUnitGrpTreeEntry::classType2Str() const
//------------------------------------------------------------------------------
{
    return CEnumUnitClassType(m_classType).toString();
}

////------------------------------------------------------------------------------
//QString CUnitGrpTreeEntry::getParentGroupName( bool i_bInsertParentNames ) const
////------------------------------------------------------------------------------
//{
//    QString strName;
//
//    if( m_pUnitGrpParent != nullptr )
//    {
//        strName = m_pUnitGrpParent->name();
//    }
//
//    if( i_bInsertParentNames && m_pUnitGrpParent != nullptr )
//    {
//        CUnitGrpTreeEntry* pUnitGrpParent = m_pUnitGrpParent;
//
//        while( pUnitGrpParent != nullptr )
//        {
//            strName.insert( 0, pUnitGrpParent->nodeSeparator() );
//            strName.insert( 0, pUnitGrpParent->name() );
//
//            pUnitGrpParent = pUnitGrpParent->m_pUnitGrpParent;
//        }
//    }
//
//    return strName;
//
//} // getParentGroupName

////------------------------------------------------------------------------------
//QString CUnitGrpTreeEntry::getName( bool i_bInsertParentNames ) const
////------------------------------------------------------------------------------
//{
//    QString strName = m_strName;
//
//    if( i_bInsertParentNames )
//    {
//        if( m_pUnitGrpParent != nullptr )
//        {
//            strName.insert( 0, m_pUnitGrpParent->nodeSeparator() );
//            strName.insert( 0, m_pUnitGrpParent->keyInTree() );
//        }
//    }
//
//    return strName;
//
//} // getName

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CUnitGrpTreeEntry::addChildUnitGrp( CUnitGrpTreeEntry* i_pUnitGrp )
////------------------------------------------------------------------------------
//{
//    if( i_pUnitGrp == nullptr )
//    {
//        QString strAddErrInfo;
//        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pUnitGroup == nullptr" );
//    }
//    if( i_pUnitGrp->m_pUnitGrpParent != nullptr )
//    {
//        QString strAddErrInfo = "Unit Group " + i_pUnitGrp->keyInTree() + " already belongs to unit group " + i_pUnitGrp->m_pUnitGrpParent->keyInTree();
//        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
//    }
//    if( !isBranch() || m_vecpUnits.size() > 0 )
//    {
//        QString strAddErrInfo = "Cannot add unit groups to groups which are not name space nodes.";
//        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
//    }
//
//    // The name got to be unique:
//    CUnitGrpTreeEntry* pUnitGrp = findChildUnitGrp(i_pUnitGrp->name());
//    if( pUnitGrp != nullptr )
//    {
//        QString strAddErrInfo = "A unit group with name " + i_pUnitGrp->name() + " already belongs to unit group " + keyInTree();
//        throw ZS::System::CException( __FILE__ ,__LINE__, EResultObjAlreadyInList, strAddErrInfo );
//    }
//
//    m_vecpUnitGrpChilds.append(i_pUnitGrp);
//    m_hshpUnitGrpChilds.insert( i_pUnitGrp->name(), i_pUnitGrp );
//
//    i_pUnitGrp->m_pUnitGrpParent = this;
//
//} // addChildUnitGrp
//
////------------------------------------------------------------------------------
//void CUnitGrpTreeEntry::removeChildUnitGrp( int i_idx )
////------------------------------------------------------------------------------
//{
//    if( m_vecpUnitGrpChilds.count() == 0 )
//    {
//        QString strAddErrInfo = keyInTree() + ".removeChildUnitGrp( " + QString::number(i_idx) + " ): List of child unit groups is empty";
//        throw CException( __FILE__, __LINE__, EResultListIsEmpty, strAddErrInfo );
//    }
//
//    if( i_idx < 0 || i_idx >= m_vecpUnitGrpChilds.count() )
//    {
//        QString strAddErrInfo = keyInTree() + ".removeChildUnitGrp( " + QString::number(i_idx) + " ): Index is out of range";
//        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
//    }
//
//    CUnitGrpTreeEntry* pUnitGrp = m_vecpUnitGrpChilds[i_idx];
//
//    m_vecpUnitGrpChilds.remove(i_idx);
//    m_hshpUnitGrpChilds.remove(pUnitGrp->name());
//
//} // removeChildUnitGrp
//
////------------------------------------------------------------------------------
//void CUnitGrpTreeEntry::removeChildUnitGrp( CUnitGrpTreeEntry* i_pUnitGrp )
////------------------------------------------------------------------------------
//{
//    if( i_pUnitGrp == nullptr )
//    {
//        QString strAddErrInfo;
//        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pUnitGroup == nullptr" );
//    }
//
//    int idxChild = -1;
//
//    if( m_vecpUnitGrpChilds.count() > 0 )
//    {
//        CUnitGrpTreeEntry* pUnitGrpTmp;
//        int       idxChildTmp;
//
//        for( idxChildTmp = m_vecpUnitGrpChilds.count()-1; idxChildTmp >= 0; idxChildTmp-- )
//        {
//            pUnitGrpTmp = m_vecpUnitGrpChilds[idxChildTmp];
//
//            if( i_pUnitGrp == pUnitGrpTmp )
//            {
//                idxChild = idxChildTmp;
//                break;
//            }
//        }
//    }
//
//    if( idxChild < 0 )
//    {
//        QString strAddErrInfo = "Unit Group " + i_pUnitGrp->keyInTree() + " does not belong to unit group " + keyInTree();
//        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
//    }
//
//    removeChildUnitGrp(idxChild);
//
//} // removeChildUnitGrp
//
////------------------------------------------------------------------------------
//void CUnitGrpTreeEntry::removeChildUnitGrp( const QString& i_strName )
////------------------------------------------------------------------------------
//{
//    CUnitGrpTreeEntry* pUnitGrp = findChildUnitGrp(i_strName);
//
//    if( pUnitGrp == nullptr )
//    {
//        QString strAddErrInfo = "Unit Group " + i_strName + " does not belong to unit group " + keyInTree();
//        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
//    }
//
//    removeChildUnitGrp(pUnitGrp);
//
//} // removeChildUnitGrp
//
////------------------------------------------------------------------------------
//CUnitGrpTreeEntry* CUnitGrpTreeEntry::getChildUnitGrp( int i_idx ) const
////------------------------------------------------------------------------------
//{
//    if( m_vecpUnitGrpChilds.count() == 0 )
//    {
//        QString strAddErrInfo = keyInTree() + ".getChildUnitGrp( " + QString::number(i_idx) + " ): List of child unit groups is empty";
//        throw CException( __FILE__, __LINE__, EResultListIsEmpty, strAddErrInfo );
//    }
//
//    if( i_idx < 0 || i_idx >= m_vecpUnitGrpChilds.count() )
//    {
//        QString strAddErrInfo = keyInTree() + ".getChildUnitGrp( " + QString::number(i_idx) + " ): Index is out of range";
//        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
//    }
//
//    return m_vecpUnitGrpChilds[i_idx];
//
//} // getChildUnitGrp
//
////------------------------------------------------------------------------------
//CUnitGrpTreeEntry* CUnitGrpTreeEntry::findChildUnitGrp( const QString& i_strName ) const
////------------------------------------------------------------------------------
//{
//    CUnitGrpTreeEntry* pUnitGrp = nullptr;
//
//    if( m_hshpUnitGrpChilds.contains(i_strName) )
//    {
//        pUnitGrp = m_hshpUnitGrpChilds[i_strName];
//    }
//    return pUnitGrp;
//
//} // findChildUnitGrp

/*=============================================================================
public: // overridables of base class CIdxTreeEntry
=============================================================================*/

//------------------------------------------------------------------------------
int CUnitGrpTreeEntry::addChild( CIdxTreeEntry* i_pChildTreeEntry )
//------------------------------------------------------------------------------
{
    int idxInParentBranch = CIdxTreeEntry::addChild(i_pChildTreeEntry);

    // The name and symbol got to be unique.
    // If the name would not have been unique the addChild method above
    // would have thrown an exception. Here we check whether the symbol is unique.
    if( i_pChildTreeEntry->isLeave() ) {
        CUnitTreeEntry* pUnit = dynamic_cast<CUnitTreeEntry*>(i_pChildTreeEntry);
        if( pUnit == nullptr ) {
            throw CException(
                __FILE__, __LINE__, EResultInternalProgramError,
                "Only units can be leaves in the index tree");
        }
        if( findUnitBySymbol(pUnit->symbol()) != nullptr )
        {
            throw CException(
                __FILE__ ,__LINE__, EResultObjAlreadyInList,
                "A unit with symbol " + pUnit->symbol() + " already belongs to unit group " + keyInTree());
        }
        m_hshpUnitsBySymbol[pUnit->symbol()] = pUnit;
    }
    return idxInParentBranch;
}

/*=============================================================================
public: // instance methods
=============================================================================*/

////------------------------------------------------------------------------------
//void CUnitGrpTreeEntry::addUnit( CUnit* i_pUnit )
////------------------------------------------------------------------------------
//{
//    if( i_pUnit == nullptr )
//    {
//        QString strAddErrInfo;
//        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pUnit == nullptr" );
//    }
//    if( i_pUnit->parentBranch() != nullptr )
//    {
//        QString strAddErrInfo = "Unit " + i_pUnit->keyInTree() + " already belongs to unit group " + i_pUnit->parentBranchKeyInTree();
//        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
//    }
//    if( isBranch() || m_vecpUnitGrpChilds.size() > 0 )
//    {
//        QString strAddErrInfo = "Cannot add units to name space nodes.";
//        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
//    }
//
//    // The name and symbol got to be unique:
//    CUnit* pUnit = findUnitByName(i_pUnit->name());
//    if( pUnit != nullptr )
//    {
//        QString strAddErrInfo = "A unit with name " + i_pUnit->name() + " already belongs to unit group " + keyInTree();
//        throw ZS::System::CException( __FILE__ ,__LINE__, EResultObjAlreadyInList, strAddErrInfo );
//    }
//    pUnit = findUnitBySymbol(i_pUnit->symbol());
//    if( pUnit != nullptr )
//    {
//        QString strAddErrInfo = "A unit with symbol " + i_pUnit->symbol() + " already belongs to unit group " + keyInTree();
//        throw ZS::System::CException( __FILE__ ,__LINE__, EResultObjAlreadyInList, strAddErrInfo );
//    }
//
//    m_vecpUnits.append(i_pUnit);
//    m_hshpUnits.insert( i_pUnit->symbol(), i_pUnit );
//
//    //i_pUnit->m_pUnitGrp = this;
//
//} // addUnit

////------------------------------------------------------------------------------
//void CUnitGrpTreeEntry::removeUnit( int i_idx )
////------------------------------------------------------------------------------
//{
//    if( m_vecpUnits.count() == 0 )
//    {
//        QString strAddErrInfo = keyInTree() + ".removeUnit( " + QString::number(i_idx) + " ): List of units is empty";
//        throw CException( __FILE__, __LINE__, EResultListIsEmpty, strAddErrInfo );
//    }
//
//    if( i_idx < 0 || i_idx >= m_vecpUnits.count() )
//    {
//        QString strAddErrInfo = keyInTree() + ".removeUnit( " + QString::number(i_idx) + " ): Index is out of range";
//        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
//    }
//
//    CUnit* pUnit = m_vecpUnits[i_idx];
//
//    m_vecpUnits.remove(i_idx);
//    m_hshpUnits.remove(pUnit->symbol());
//
//} // removeUnit
//
////------------------------------------------------------------------------------
//void CUnitGrpTreeEntry::removeUnit( CUnit* i_pUnit )
////------------------------------------------------------------------------------
//{
//    if( i_pUnit == nullptr )
//    {
//        QString strAddErrInfo;
//        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pUnit == nullptr" );
//    }
//
//    int idxUnit = -1;
//
//    if( m_vecpUnits.count() > 0 )
//    {
//        CUnit* pUnitTmp;
//        int    idxUnitTmp;
//
//        for( idxUnitTmp = m_vecpUnits.count()-1; idxUnitTmp >= 0; idxUnitTmp-- )
//        {
//            pUnitTmp = m_vecpUnits[idxUnitTmp];
//
//            if( i_pUnit == pUnitTmp )
//            {
//                idxUnit = idxUnitTmp;
//                break;
//            }
//        }
//    }
//
//    if( idxUnit < 0 )
//    {
//        QString strAddErrInfo = "Unit " + i_pUnit->keyInTree() + " does not belong to unit group " + keyInTree();
//        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
//    }
//
//    removeUnit(idxUnit);
//
//} // removeUnit
//
////------------------------------------------------------------------------------
//void CUnitGrpTreeEntry::removeUnit( const QString& i_strSymbolOrName )
////------------------------------------------------------------------------------
//{
//    CUnit* pUnit = findUnit(i_strSymbolOrName);
//
//    if( pUnit == nullptr )
//    {
//        QString strAddErrInfo = "Unit " + i_strSymbolOrName + " does not belong to unit group " + keyInTree();
//        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
//    }
//
//    removeUnit(pUnit);
//
//} // removeUnit
//
////------------------------------------------------------------------------------
//CUnit* CUnitGrpTreeEntry::getUnit( int i_idx ) const
////------------------------------------------------------------------------------
//{
//    if( m_vecpUnits.count() == 0 )
//    {
//        QString strAddErrInfo = keyInTree() + ".getUnit( " + QString::number(i_idx) + " ): List of units is empty";
//        throw CException( __FILE__, __LINE__, EResultListIsEmpty, strAddErrInfo );
//    }
//
//    if( i_idx < 0 || i_idx >= m_vecpUnits.count() )
//    {
//        QString strAddErrInfo = keyInTree() + ".getUnit( " + QString::number(i_idx) + " ): Index is out of range";
//        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
//    }
//
//    return m_vecpUnits[i_idx];
//
//} // getUnit

//------------------------------------------------------------------------------
CUnitTreeEntry* CUnitGrpTreeEntry::findUnit( const QString& i_strSymbolOrName ) const
//------------------------------------------------------------------------------
{
    CUnitTreeEntry* pUnit = findUnitBySymbol(i_strSymbolOrName);
    if( pUnit == nullptr )
    {
        pUnit = findUnitByName(i_strSymbolOrName);
    }
    return pUnit;
}

//------------------------------------------------------------------------------
CUnitTreeEntry* CUnitGrpTreeEntry::findUnitBySymbol( const QString& i_strSymbol ) const
//------------------------------------------------------------------------------
{
    return m_hshpUnitsBySymbol.value(i_strSymbol, nullptr);
}

//------------------------------------------------------------------------------
CUnitTreeEntry* CUnitGrpTreeEntry::findUnitByName( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitTreeEntry*>(find(EIdxTreeEntryType::Leave, i_strName));
}


/*******************************************************************************
class CUnitGrp
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp() :
//------------------------------------------------------------------------------
    m_pTreeEntry(nullptr),
    m_strUniqueName()
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(CUnitGrp* i_pUnitGrp) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnitGrp->m_pTreeEntry),
    m_strUniqueName(i_pUnitGrp->m_strUniqueName)
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(const CUnitGrp* i_pUnitGrp) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnitGrp->m_pTreeEntry),
    m_strUniqueName(i_pUnitGrp->m_strUniqueName)
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(CUnitGrp& i_unitGrp) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_unitGrp.m_pTreeEntry),
    m_strUniqueName(i_unitGrp.m_strUniqueName)
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(const CUnitGrp& i_unitGrp) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_unitGrp.m_pTreeEntry),
    m_strUniqueName(i_unitGrp.m_strUniqueName)
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(CUnitGrpTreeEntry* i_pUnitGrp) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnitGrp),
    m_strUniqueName(i_pUnitGrp->keyInTree())
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(const CUnitGrpTreeEntry* i_pUnitGrp) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitGrpTreeEntry*>(i_pUnitGrp)),
    m_strUniqueName(i_pUnitGrp->keyInTree())
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(CUnitGrpTreeEntry& i_unitGrp) :
//------------------------------------------------------------------------------
    m_pTreeEntry(&i_unitGrp),
    m_strUniqueName(i_unitGrp.keyInTree())
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(const CUnitGrpTreeEntry& i_unitGrp) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitGrpTreeEntry*>(&i_unitGrp)),
    m_strUniqueName(i_unitGrp.keyInTree())
{
}

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(const QString& i_strUniqueName) :
//------------------------------------------------------------------------------
    m_pTreeEntry(CIdxTreePhysSizes::GetInstance()->findUnitGrp(i_strUniqueName)),
    m_strUniqueName(i_strUniqueName)
{
}

//------------------------------------------------------------------------------
CUnitGrp::~CUnitGrp()
//------------------------------------------------------------------------------
{
    m_pTreeEntry = nullptr;
    //m_strUniqueName;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitGrp::operator == ( const CUnitGrp& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = (m_pTreeEntry == i_other.m_pTreeEntry);
    if( bEqual ) bEqual = (m_strUniqueName.compare(i_other.m_strUniqueName) == 0);
    return bEqual;
}

//------------------------------------------------------------------------------
bool CUnitGrp::operator != ( const CUnitGrp& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CUnitGrp::nodeSeparator() const
//------------------------------------------------------------------------------
{
    return QString(m_pTreeEntry == nullptr ? "" : m_pTreeEntry->nodeSeparator());
}

//------------------------------------------------------------------------------
QString CUnitGrp::keyInTree() const
//------------------------------------------------------------------------------
{
    return QString(m_pTreeEntry == nullptr ? "" : m_pTreeEntry->keyInTree());
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitGrp::isValid() const
//------------------------------------------------------------------------------
{
    return (m_pTreeEntry != nullptr);
}

//------------------------------------------------------------------------------
EUnitClassType CUnitGrp::classType() const
//------------------------------------------------------------------------------
{
    return m_pTreeEntry == nullptr ?
        EUnitClassType::Undefined : m_pTreeEntry->classType();
}

//------------------------------------------------------------------------------
QString CUnitGrp::classType2Str() const
//------------------------------------------------------------------------------
{
    return m_pTreeEntry == nullptr ?
        CEnumUnitClassType(EUnitClassType::Undefined).toString() :
        m_pTreeEntry->classType2Str();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CUnitGrp::count() const
//------------------------------------------------------------------------------
{
    return m_pTreeEntry == nullptr ? 0 : m_pTreeEntry->count();
}

//------------------------------------------------------------------------------
CUnit CUnitGrp::findUnit( const QString& i_strSymbolOrName ) const
//------------------------------------------------------------------------------
{
    return m_pTreeEntry == nullptr ?
        CUnit() : m_pTreeEntry->findUnit(i_strSymbolOrName);
}

//------------------------------------------------------------------------------
CUnit CUnitGrp::findUnitBySymbol( const QString& i_strSymbol ) const
//------------------------------------------------------------------------------
{
    return m_pTreeEntry == nullptr ?
        CUnit() : m_pTreeEntry->findUnitBySymbol(i_strSymbol);
}

//------------------------------------------------------------------------------
CUnit CUnitGrp::findUnitByName( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    return m_pTreeEntry == nullptr ?
        CUnit() : m_pTreeEntry->findUnitByName(i_strName);
}
