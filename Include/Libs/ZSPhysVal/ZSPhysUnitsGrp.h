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

#ifndef ZSPhysVal_PhysUnitsGroup_h
#define ZSPhysVal_PhysUnitsGroup_h

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
class CUnit;

//******************************************************************************
class ZSPHYSVALDLL_API CUnitGrp : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitGrp"; }
public: // ctors and dtor
    //CUnitGrp();
    CUnitGrp(
        CIdxTreePhysSizes* i_pIdxTree,
        EUnitClassType     i_classType,
        const QString&     i_strName );
    CUnitGrp(
        ZS::System::CIdxTreeEntry* i_pParentBranch,
        EUnitClassType             i_classType,
        const QString&             i_strName );
    CUnitGrp(CUnitGrp&& i_other) = delete;
    CUnitGrp(CUnitGrp& i_other) = delete;
    CUnitGrp(const CUnitGrp& i_other) = delete;
    virtual ~CUnitGrp();
public: // operators
    CUnitGrp& operator=(CUnitGrp& i_other) = delete;
    CUnitGrp& operator=(const CUnitGrp& i_other) = delete;
    CUnitGrp& operator=(CUnitGrp&& i_other) = delete;
public: // operators
    bool operator == ( const CUnitGrp& i_other ) const;
    bool operator != ( const CUnitGrp& i_other ) const;
public: // instance methods
    EUnitClassType classType() const { return m_classType; }
    QString classType2Str() const;
    //QString getParentGroupName( bool i_bInsertParentNames = false ) const;
    //QString getName( bool i_bInsertParentNames = false ) const;
    //QString getKey() const { return m_strKey; }
public: // instance methods
    //bool isNameSpaceNode() const { return m_bIsNameSpaceNode; }
public: // instance methods
    //int getChildUnitGrpCount() const { return m_vecpUnitGrpChilds.size(); }
    //void addChildUnitGrp( CUnitGrp* i_pUnitGrp );
    //void removeChildUnitGrp( int i_idx );
    //void removeChildUnitGrp( CUnitGrp* i_pUnitGrp );
    //void removeChildUnitGrp( const QString& i_strName );
    //CUnitGrp* getChildUnitGrp( int i_idx ) const;
    //CUnitGrp* findChildUnitGrp( const QString& i_strName ) const;
public: // overridables of base class CIdxTreeEntry
    virtual int addChild( CIdxTreeEntry* i_pChildTreeEntry ) override;
public: // instance methods
    //int getUnitCount() const { return m_vecpUnits.size(); }
    //void addUnit( CUnit* i_pUnit );
    //void removeUnit( int i_idx );
    //void removeUnit( CUnit* i_pUnit );
    //void removeUnit( const QString& i_strSymbolOrName );
    //CUnit* unit( int i_idx ) const;
    CUnit* findUnit( const QString& i_strSymbolOrName ) const;
    CUnit* findUnitBySymbol( const QString& i_strSymbol ) const;
    CUnit* findUnitByName( const QString& i_strName ) const;
protected: // instance members
    EUnitClassType m_classType;
    //QString m_strName;
    /*!< Key used to insert the group in the units pool */
    //QString m_strKey;
    //bool m_bIsNameSpaceNode;
    //CUnitGrp* m_pUnitGrpParent;
    /*!< Can't have both types of children (one list must be empty): */
    //QVector<CUnitGrp*> m_vecpUnitGrpChilds;
    /*!< key is name of unit group */
    //QHash<QString,CUnitGrp*> m_hshpUnitGrpChilds;
    /*!< Either UnitGrps (if NameSpaceNode) or Units (if not NameSpaceNode). */
    //QVector<CUnit*> m_vecpUnits;
    /*!< Key is symbol of unit */
    QHash<QString, CUnit*> m_hshpUnitsBySymbol;

}; // class CUnitGrp

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnitsGroup_h
