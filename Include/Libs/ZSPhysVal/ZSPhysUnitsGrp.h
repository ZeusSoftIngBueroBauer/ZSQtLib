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

namespace ZS
{
namespace PhysVal
{
class CUnit;

//******************************************************************************
class ZSPHYSVALDLL_API CUnitGrp
//******************************************************************************
{
public: // ctors and dtor
    CUnitGrp(
        CUnitsPool*    i_pUnitsPool,
        EUnitClassType i_classType,                 // [PhysScienceFields, Ratios, UserDefinedQuantities]
        const QString& i_strName,                   // e.g. "Time", "Power", "Voltage", "Current", ...
        const QString& i_strKey,                    // e.g. "PhysScienceFields", "Ratios", "Kinematics", "Kinematics::Time", ...
        bool           i_bIsNameSpaceNode,
        CUnitGrp*      i_pUnitGrpParent = nullptr );   // either nullptr, or root node PhysScienceFields, Ratios, UserDefinedQuantities, or physical science field node (e.g. "Electricity", ...) below root node PhysScienceFields
    virtual ~CUnitGrp();
public: // operators
    bool operator == ( const CUnitGrp& i_unitGrpOther ) const;
    bool operator != ( const CUnitGrp& i_unitGrpOther ) const;
public: // instance methods
    EUnitClassType classType() const { return m_classType; }
    QString classType2Str() const;
    QString getParentGroupName( bool i_bInsertParentNames = false ) const;
    QString getName( bool i_bInsertParentNames = false ) const;
    QString getKey() const { return m_strKey; }
    QChar getNameSeparator() const;
public: // instance methods
    bool isNameSpaceNode() const { return m_bIsNameSpaceNode; }
public: // instance methods
    int getChildUnitGrpCount() const { return m_vecpUnitGrpChilds.size(); }
    void addChildUnitGrp( CUnitGrp* i_pUnitGrp );
    void removeChildUnitGrp( int i_idx );
    void removeChildUnitGrp( CUnitGrp* i_pUnitGrp );
    void removeChildUnitGrp( const QString& i_strName );
    CUnitGrp* getChildUnitGrp( int i_idx ) const;
    CUnitGrp* findChildUnitGrp( const QString& i_strName ) const;
public: // instance methods
    int getUnitCount() const { return m_vecpUnits.size(); }
    void addUnit( CUnit* i_pUnit );
    void removeUnit( int i_idx );
    void removeUnit( CUnit* i_pUnit );
    void removeUnit( const QString& i_strSymbolOrName );
    CUnit* getUnit( int i_idx ) const;
    CUnit* findUnit( const QString& i_strSymbolOrName ) const;
    CUnit* findUnitBySymbol( const QString& i_strSymbol ) const;
    CUnit* findUnitByName( const QString& i_strName ) const;
private: // default and copy ctor not allowed
    CUnitGrp();
    CUnitGrp( const CUnitGrp& );
private: // assignment operator not allowed
    CUnitGrp& operator = ( const CUnitGrp& );
protected: // instance members
    CUnitsPool*              m_pUnitsPool;
    EUnitClassType           m_classType;
    QString                  m_strName;
    QString                  m_strKey;              // key used to insert the group in the units pool
    bool                     m_bIsNameSpaceNode;
    CUnitGrp*                m_pUnitGrpParent;
    QVector<CUnitGrp*>       m_vecpUnitGrpChilds;   // can't have both types of children (one list must be empty):
    QHash<QString,CUnitGrp*> m_hshpUnitGrpChilds;   // key is name of unit group
    QVector<CUnit*>          m_vecpUnits;           // either UnitGrps (if NameSpaceNode) or Units (if not NameSpaceNode).
    QHash<QString,CUnit*>    m_hshpUnits;           // key is symbol of unit

}; // class CUnitGrp

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnitsGroup_h
