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

#ifndef ZSPhysVal_UnitsIdxTree_h
#define ZSPhysVal_UnitsIdxTree_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qhash.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

namespace ZS
{
namespace PhysVal
{
class CUnitsTreeEntryGrpScienceField;
class CUnitsTreeEntryGrpBase;
class CUnitsTreeEntryGrpPhysUnits;
class CUnitsTreeEntryUnitBase;
class CUnitsTreeEntryPhysUnit;
class CUnitsTreeEntryUnitRatio;

//******************************************************************************
class ZSPHYSVALDLL_API CIdxTreeUnits : public ZS::System::CIdxTree
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CIdxTreeUnits"; }
public: // class methods
    static CIdxTreeUnits* CreateInstance();
    static CIdxTreeUnits* GetInstance();
    static int ReleaseInstance();
public: // ctors and dtor
    CIdxTreeUnits(ZS::System::CIdxTreeEntry* i_pRootTreeEntry = nullptr);
    ~CIdxTreeUnits();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    //CUnitGrp* getUnitClassTypeGroup( EUnitClassType i_classType );
    //CUnitGrp* getPhysScienceFieldUnitGroup( EPhysScienceField i_scienceField );
public: // instance methods
    CUnitsTreeEntryGrpScienceField* findPhysScienceField( const QString& i_strUniqueName );
    CUnitsTreeEntryGrpBase* findUnitGrp( const QString& i_strUniqueName );
    CUnitsTreeEntryGrpPhysUnits* findPhysUnitsGroup( const QString& i_strUniqueName );
    //CUnitsTreeEntryGrpRatios* findRatiosGroup( const QString& i_strUniqueName );
    //CUnitsTreeEntryGrpDataQuantities* findDataQuantitiesGroup( const QString& i_strUniqueName );
public: // instance methods
    CUnitsTreeEntryUnitBase* findUnit( const QString& i_strUniqueName );
    CUnitsTreeEntryUnitBase* findUnit( const QString& i_strGrpPath, const QString& i_strUnitName );
    CUnitsTreeEntryPhysUnit* findPhysUnit( const QString& i_strUniqueName );
    CUnitsTreeEntryPhysUnit* findPhysUnit( const QString& i_strGrpPath, const QString& i_strUnitName );
    CUnitsTreeEntryUnitRatio* findRatio( const QString& i_strUniqueName );
    CUnitsTreeEntryUnitRatio* findRatio( const QString& i_strGrpPath, const QString& i_strUnitName );
    //CUnitsTreeEntryDataQuantity* findDataQuantity( const QString& i_strUniqueName );
    //CUnitsTreeEntryDataQuantity* findDataQuantity( const QString& i_strGrpPath, const QString& i_strUnitName );
protected: // reference counter
    int getRefCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected: // class members
    /*!< Singleton class. */
    static CIdxTreeUnits* s_pTheInst;
protected: // instance members
    /*<! Reference counter for createInstance and releaseInstance. */
    int m_iRefCount;

}; // class CIdxTreeUnits

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_UnitsIdxTree_h
