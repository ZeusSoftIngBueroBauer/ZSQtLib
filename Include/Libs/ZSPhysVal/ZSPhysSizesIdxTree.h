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

#ifndef ZSPhysVal_PhysSizesIdxTree_h
#define ZSPhysVal_PhysSizesIdxTree_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qhash.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

namespace ZS
{
namespace PhysVal
{
class CUnit;
class CUnitGrp;
class CPhysSize;
class CPhysUnit;

//******************************************************************************
class ZSPHYSVALDLL_API CIdxTreePhysSizes : public ZS::System::CIdxTree
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CIdxTreePhysSizes"; }
public: // class methods
    static CIdxTreePhysSizes* CreateInstance( const QString& i_strObjName = "PhysSizes" );
    static CIdxTreePhysSizes* GetInstance( const QString& i_strObjName = "PhysSizes" );
    static int ReleaseInstance( const QString& i_strObjName = "PhysSizes" );
    static int ReleaseInstance( CIdxTreePhysSizes* i_pIdxTree );
public: // ctors and dtor
    CIdxTreePhysSizes(
        const QString& i_strObjName = "PhysSizes",
        ZS::System::CIdxTreeEntry* i_pRootTreeEntry = nullptr );
    ~CIdxTreePhysSizes();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    //CUnitGrp* getUnitClassTypeGroup( EUnitClassType i_classType );
    //CUnitGrp* getPhysScienceFieldUnitGroup( EPhysScienceField i_scienceField );
public: // instance methods
    CUnitGrp* findUnitGrp( const QString& i_strKeyInTree );
    CPhysSize* findPhysSize( const QString& i_strKeyInTree );
public: // instance methods
    CUnit* findUnit( const QString& i_strKeyInTree );
    CUnit* findUnit( const QString& i_strGrpKey, const QString& i_strUnit );
    CPhysUnit* findPhysUnit( const QString& i_strKeyInTree );
    CPhysUnit* findPhysUnit( const QString& i_strGrpKey, const QString& i_strUnit );
protected: // reference counter
    int getRefCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected: // class members
    /*! < Key is name of the index tree. */
    static QHash<QString, CIdxTreePhysSizes*> s_hshpIdxTrees;
protected: // instance members
    /*<! Reference counter for createInstance and releaseInstance. */
    int m_iRefCount;

}; // class CIdxTreePhysSizes

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysSizesIdxTree_h
