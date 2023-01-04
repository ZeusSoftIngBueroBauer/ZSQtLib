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
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitGrp
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    @param i_pIdxTree,
    @param i_classType
        [PhysScienceFields, Ratios, UserDefinedQuantities]
    @param i_strName
        e.g. "Time", "Power", "Voltage", "Current", ...
    @param i_pParentBranch
*/
CUnitGrp::CUnitGrp(
    CIdxTree*      i_pIdxTree,
    EUnitClassType i_classType,
    const QString& i_strName,
    CIdxTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, i_strName),
    m_classType(i_classType),
    //m_pUnitGrpParent(nullptr),
    //m_vecpUnitGrpChilds(),
    //m_hshpUnitGrpChilds(),
    //m_vecpUnits(),
    m_hshpUnitsBySymbol(),
    m_iIdxTreeRefCount(0)
{
    m_pTree = i_pIdxTree;
    if( m_pTree == nullptr ) {
        m_pTree = CIdxTreePhysSizes::GetInstance();
        if( m_pTree == nullptr ) {
            m_pTree = CIdxTreePhysSizes::CreateInstance();
            m_iIdxTreeRefCount = 1;
        }
    }
    m_pTree->add(this, i_pParentBranch);

} // ctor

//------------------------------------------------------------------------------
/*! @brief

    @param i_pParentBranch
    @param i_classType
        [PhysScienceFields, Ratios, UserDefinedQuantities]
    @param i_strName
        e.g. "Time", "Power", "Voltage", "Current", ...
*/
CUnitGrp::CUnitGrp(
    CIdxTreeEntry*  i_pParentBranch,
    EUnitClassType  i_classType,
    const QString&  i_strName ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, i_strName),
    m_classType(i_classType),
    //m_pUnitGrpParent(nullptr),
    //m_vecpUnitGrpChilds(),
    //m_hshpUnitGrpChilds(),
    //m_vecpUnits(),
    m_hshpUnitsBySymbol(),
    m_iIdxTreeRefCount(0)
{
    m_pTree = i_pParentBranch->tree();
    if( m_pTree == nullptr ) {
        m_pTree = CIdxTreePhysSizes::GetInstance();
        if( m_pTree == nullptr ) {
            m_pTree = CIdxTreePhysSizes::CreateInstance();
            m_iIdxTreeRefCount = 1;
        }
    }
    m_pTree->add(this, i_pParentBranch);

} // ctor

//------------------------------------------------------------------------------
CUnitGrp::~CUnitGrp()
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
    //        CUnitGrp* pUnitGrp = m_vecpUnitGrpChilds[idxChild];

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
    CIdxTree* pIdxTree = m_pTree;

    m_pTree->remove(this);

    if( m_iIdxTreeRefCount > 0 ) {
        CIdxTreePhysSizes::ReleaseInstance(pIdxTree->objectName());
    }

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
    m_hshpUnitsBySymbol.clear();
    m_iIdxTreeRefCount = 0;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitGrp::operator == ( const CUnitGrp& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    if( this != &i_physsizeOther )
    {
        return false;
    }
    return true;

} // operator ==

//------------------------------------------------------------------------------
bool CUnitGrp::operator != ( const CUnitGrp& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physsizeOther );

} // operator ==

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CUnitGrp::classType2Str() const
//------------------------------------------------------------------------------
{
    return unitClassType2Str(m_classType);
}

////------------------------------------------------------------------------------
//QString CUnitGrp::getParentGroupName( bool i_bInsertParentNames ) const
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
//        CUnitGrp* pUnitGrpParent = m_pUnitGrpParent;
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
//QString CUnitGrp::getName( bool i_bInsertParentNames ) const
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
//void CUnitGrp::addChildUnitGrp( CUnitGrp* i_pUnitGrp )
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
//    CUnitGrp* pUnitGrp = findChildUnitGrp(i_pUnitGrp->name());
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
//void CUnitGrp::removeChildUnitGrp( int i_idx )
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
//    CUnitGrp* pUnitGrp = m_vecpUnitGrpChilds[i_idx];
//
//    m_vecpUnitGrpChilds.remove(i_idx);
//    m_hshpUnitGrpChilds.remove(pUnitGrp->name());
//
//} // removeChildUnitGrp
//
////------------------------------------------------------------------------------
//void CUnitGrp::removeChildUnitGrp( CUnitGrp* i_pUnitGrp )
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
//        CUnitGrp* pUnitGrpTmp;
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
//void CUnitGrp::removeChildUnitGrp( const QString& i_strName )
////------------------------------------------------------------------------------
//{
//    CUnitGrp* pUnitGrp = findChildUnitGrp(i_strName);
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
//CUnitGrp* CUnitGrp::getChildUnitGrp( int i_idx ) const
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
//CUnitGrp* CUnitGrp::findChildUnitGrp( const QString& i_strName ) const
////------------------------------------------------------------------------------
//{
//    CUnitGrp* pUnitGrp = nullptr;
//
//    if( m_hshpUnitGrpChilds.contains(i_strName) )
//    {
//        pUnitGrp = m_hshpUnitGrpChilds[i_strName];
//    }
//    return pUnitGrp;
//
//} // findChildUnitGrp

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CUnitGrp::addChild( CIdxTreeEntry* i_pChildTreeEntry )
//------------------------------------------------------------------------------
{
    int idxInParentBranch = CIdxTreeEntry::addChild(i_pChildTreeEntry);

    // The name and symbol got to be unique.
    // If the name would not have been unique the addChild method above
    // would have thrown an exception. Here we check whether the symbol is unique.
    if( i_pChildTreeEntry->isLeave() ) {
        CUnit* pUnit = dynamic_cast<CUnit*>(i_pChildTreeEntry);
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
}

////------------------------------------------------------------------------------
//void CUnitGrp::addUnit( CUnit* i_pUnit )
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
//void CUnitGrp::removeUnit( int i_idx )
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
//void CUnitGrp::removeUnit( CUnit* i_pUnit )
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
//void CUnitGrp::removeUnit( const QString& i_strSymbolOrName )
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
//CUnit* CUnitGrp::getUnit( int i_idx ) const
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
CUnit* CUnitGrp::findUnit( const QString& i_strSymbolOrName ) const
//------------------------------------------------------------------------------
{
    CUnit* pUnit = findUnitBySymbol(i_strSymbolOrName);
    if( pUnit == nullptr )
    {
        pUnit = findUnitByName(i_strSymbolOrName);
    }
    return pUnit;
}

//------------------------------------------------------------------------------
CUnit* CUnitGrp::findUnitBySymbol( const QString& i_strSymbol ) const
//------------------------------------------------------------------------------
{
    return m_hshpUnitsBySymbol.value(i_strSymbol, nullptr);
}

//------------------------------------------------------------------------------
CUnit* CUnitGrp::findUnitByName( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnit*>(find(EIdxTreeEntryType::Leave, i_strName));
}
