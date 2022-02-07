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
    static CUnitsPool* GetInstance( const QString& i_strName = "ZSPhysSizes" );
    static CUnitsPool* CreateInstance( const QString& i_strName = "ZSPhysSizes" );
    static void ReleaseInstance( const QString& i_strName = "ZSPhysSizes" );
    static void ReleaseInstance( CUnitsPool* i_pUnitsPool );
protected: // ctors and dtor
    CUnitsPool( const QString& i_strName );
    ~CUnitsPool();
public: // ctors and dtor
    QString name() const { return m_strName; }
public: // instance methods
    QChar getNameSeparator() const;
    void setNameSeparator( QChar i_cSeparator );
public: // instance methods
    CUnitGrp* getUnitClassTypeGroup( EUnitClassType i_classType );
    CUnitGrp* getPhysScienceFieldUnitGroup( EPhysScienceField i_scienceField );
public: // instance methods
    CUnitGrp* findUnitGrp( const QString& i_strUnitGrpKey );
    CPhysSize* findPhysSize( const QString& i_strUnitGrpKey );  // same as "findUnitGrp" but with implicit type cast
public: // instance methods
    CUnit* findUnit( const QString& i_strUnitKey );
    CUnit* findUnit( const QString& i_strUnitGrpKey, const QString& i_strUnitKey );
    CPhysUnit* findPhysUnit( const QString& i_strUnitKey );     // same as "findUnit" but with implicit type cast
    CPhysUnit* findPhysUnit( const QString& i_strUnitGrpKey, const QString& i_strUnitKey );
protected: // instance methods
    void onUnitGrpCreated( CUnitGrp* i_pUnitGrp );
    void onUnitGrpDestroyed( const QString& i_strUnitGrpKey );
protected: // instance methods
    void onUnitCreated( CUnit* i_pUnit );
    void onUnitDestroyed( const QString& i_strUnitKey );
protected: // reference counter
    int getRefCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected: // class members
    static QHash<QString, CUnitsPool*> s_hshpInstances; /*!< Hash with all created unit pools (key is name of instance). */
protected: // instance members
    QString                  m_strName;
    QChar                    m_cNameSeparator;
    QHash<QString,CUnitGrp*> m_hshpUnitGrps;
    QHash<QString,CUnit*>    m_hshpUnits;
    CUnitGrp*                m_arpUnitGrpClassTypes[EUnitClassTypeCount];
    CUnitGrp*                m_arpUnitGrpPhysScienceFields[EPhysScienceFieldCount];
    int                      m_iRefCount;

}; // class CUnitsPool

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnitsPool_h
