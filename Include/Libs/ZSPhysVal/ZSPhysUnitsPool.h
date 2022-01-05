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

#ifndef ZSPhysVal_PhysUnitsPool_h
#define ZSPhysVal_PhysUnitsPool_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qhash.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"

namespace ZS
{
namespace PhysVal
{
class CUnit;
class CUnitGrp;

//******************************************************************************
class ZSPHYSVALDLL_API CUnitsPool
//******************************************************************************
{
friend class CUnit;
friend class CUnitGrp;
public: // class methods
    static CUnitsPool* GetInstance() { return s_pInstance; }
public: // class methods
    static QChar GetNameSeparator();
    static void SetNameSeparator( QChar i_cSeparator );
public: // class methods
    static CUnitGrp* GetUnitClassTypeGroup( EUnitClassType i_classType );
    static CUnitGrp* GetPhysScienceFieldUnitGroup( EPhysScienceField i_scienceField );
public: // class methods
    static CUnitGrp* FindUnitGrp( const QString& i_strUnitGrpKey );
    static CPhysSize* FindPhysSize( const QString& i_strUnitGrpKey );  // same as "findUnitGrp" but with implicit type cast
public: // class methods
    static CUnit* FindUnit( const QString& i_strUnitKey );
    static CUnit* FindUnit( const QString& i_strUnitGrpKey, const QString& i_strUnitKey );
    static CPhysUnit* FindPhysUnit( const QString& i_strUnitKey );     // same as "findUnit" but with implicit type cast
    static CPhysUnit* FindPhysUnit( const QString& i_strUnitGrpKey, const QString& i_strUnitKey );
public: // ctors and dtor
    CUnitsPool();
    ~CUnitsPool();
protected: // instance methods
    void onUnitGrpCreated( CUnitGrp* i_pUnitGrp );
    void onUnitGrpDestroyed( const QString& i_strUnitGrpKey );
protected: // instance methods
    void onUnitCreated( CUnit* i_pUnit );
    void onUnitDestroyed( const QString& i_strUnitKey );
protected: // class members
    static CUnitsPool* s_pInstance; // singleton class
protected: // instance members
    QChar                    m_cNameSeparator;
    QHash<QString,CUnitGrp*> m_hshpUnitGrps;
    QHash<QString,CUnit*>    m_hshpUnits;
    CUnitGrp*                m_arpUnitGrpClassTypes[EUnitClassTypeCount];
    CUnitGrp*                m_arpUnitGrpPhysScienceFields[EPhysScienceFieldCount];

}; // class CUnitsPool

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnitsPool_h
