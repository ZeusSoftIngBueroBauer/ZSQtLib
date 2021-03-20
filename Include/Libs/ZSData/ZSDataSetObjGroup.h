/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSData_DataSetObjGroup_h
#define ZSData_DataSetObjGroup_h

#include <QtCore/qobject.h>

#include "ZSData/ZSDataCommon.h"
#include "ZSData/ZSDataSetObject.h"
#include "ZSSys/ZSSysErrResult.h"

class QXmlStreamWriter;


namespace ZS
{
namespace System
{
struct SEnumEntry;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDataTable;

//******************************************************************************
class ZSDATADLL_API CDataSetObjGroup : public CDataSetObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::DS"; }
    static QString ClassName() { return "CDataSetObjGroup"; }
public: // ctors
    CDataSetObjGroup( const QString& i_strName );
protected: // ctor (for derived class (CDataSet))
    CDataSetObjGroup( EDataSetObjType i_dsObjType, const QString& i_strName );
public: // ctors and dtor
    virtual ~CDataSetObjGroup();
private: // copy ctor not allowed
    CDataSetObjGroup( const CDataSetObjGroup& i_other );
private: // assignment operator not allowed
    CDataSetObjGroup& operator = ( const CDataSetObjGroup& i_other );
signals:
    void childObjAdded( ZS::Data::CDataSetObjGroup* i_pGrp, ZS::Data::CDataSetObject* i_pDSObj );
    void childObjAdded( ZS::Data::CDataSetObjGroup* i_pGrp, ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
    void childObjAboutToBeRemoved( ZS::Data::CDataSetObjGroup* i_pGrp, ZS::Data::CDataSetObject* i_pDSObj );
    void childObjAboutToBeRemoved( ZS::Data::CDataSetObjGroup* i_pGrp, ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
    void childObjRemoved( ZS::Data::CDataSetObjGroup* i_pGrp, ZS::Data::CDataSetObject* i_pDSObj );
    void childObjRemoved( ZS::Data::CDataSetObjGroup* i_pGrp, ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
public: // overridables
    virtual QString nameSpace() const { return CDataSetObjGroup::NameSpace(); }
    virtual QString className() const { return CDataSetObjGroup::ClassName(); }
public: // overridables
    virtual void clear();
public: // instance methods
    bool isParentOf( CDataSetObject* i_pDSObj, bool i_bRecursive = false, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const; // If not recursive only one level down is checked.
public: // instance methods
    QString getUniqueChildObjName( EDataSetObjType i_dsObjType, const QString& i_strPrefix = "", ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default );
public: // overridables
    virtual CDataSetObject* addChildObj( EDataSetObjType i_dsObjType, const QString& i_strObjName );
    virtual CDataSetObject* addChildObj( CDataSetObject* i_pDSObj );
    virtual void removeChildObj( EDataSetObjType i_dsObjType, const QString& i_strObjName );
    virtual void removeChildObj( CDataSetObject* i_pDSObj );
public: // instance methods
    int getChildObjsCount( EDataSetObjType i_dsObjType = EDataSetObjTypeUndefined, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    CDataSetObject* getChildObj( int i_idxObj, EDataSetObjType i_dsObjType = EDataSetObjTypeUndefined, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    CDataSetObject* findChildObjByName( EDataSetObjType i_dsObjType, const QString& i_strName, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;    // If objType = Undefined the name must start with the object type specifier [Grp, Enum, DP, DT] and ":".
    CDataSetObject* findChildObjByGrpKey( const QString& i_strGrpKey, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;                           // The group key is the name of the object starting with object type symbol and ":" (e.g. "DP:Frequency").
    int getChildIndexOf( CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // overridables
    virtual CDataSetObjGroup* addChildGroup( const QString& i_strGrpName );
    virtual CDataSetObjGroup* addChildGroup( CDataSetObjGroup* i_pGrp );
    virtual void removeChildGroup( const QString& i_strGrpName );
    virtual void removeChildGroup( CDataSetObjGroup* i_pGrp );
public: // instance methods
    //int getChildGroupsCount( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    //CDataSetObjGroup* getChildGroup( int i_idxObj, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    //CDataSetObjGroup* findChildGroupByName( const QString& i_strName, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // overridables
    //virtual CDataEnumeration* addChildEnumeration( ZS::System::CEnumeration* i_pEnum );
    //virtual CDataEnumeration* addChildEnumeration( const QString& i_strName );
    //virtual CDataEnumeration* addChildEnumeration( const QString& i_strName, ZS::System::SEnumEntry* i_pEnumArr, int i_iArrLen );
    //void virtual removeChildEnumeration( CDataEnumeration* i_pDE );
public: // instance methods
    //int getChildEnumerationsCount() const;
    //CDataEnumeration* getChildEnumeration( int i_idxObj ) const;
    //CDataEnumeration* findChildEnumerationByName( const QString& i_strName ) const;
public: // overridables
    //virtual CDataTable* addChildTable( const QString& i_strName );
    //virtual void removeChildTable( CDataTable* i_pDT );
public: // instance methods
    //int getChildTablesCount( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    //CDataTable* getChildTable( int i_idxObj, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    //CDataTable* findChildTableByName( const QString& i_strName, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // overridables
    //virtual CDataParameter* addChildParameter( const QString& i_strName, ZS::System::EDimensionType i_dimType = ZS::System::EDimensionType::SingleValue );
    //virtual CDataParameter* addChildParameter( const QString& i_strName, EDataType i_dataType, ZS::System::EDimensionType i_dimType = ZS::System::EDimensionType::SingleValue );
    //virtual CDataParameter* addChildParameter( const QString& i_strName, CDataEnumeration* i_pDE, ZS::System::EDimensionType i_dimType = ZS::System::EDimensionType::SingleValue );
    //virtual CDataParameter* addChildParameter( const QString& i_strName, ZS::PhysVal::CUnitGrp* i_pUnitGrp, ZS::System::EDimensionType i_dimType = ZS::System::EDimensionType::SingleValue );
    //virtual CDataParameter* addChildParameter( const QString& i_strName, ZS::PhysVal::CUnit* i_pUnit, ZS::System::EDimensionType i_dimType = ZS::System::EDimensionType::SingleValue );
    //virtual void removeChildParameter( CDataParameter* i_pDP );
public: // instance methods
    //int getChildParametersCount() const;
    //CDataParameter* getChildParameter( int i_idxObj ) const;
    //CDataParameter* findChildParameterByName( const QString& i_strName ) const;
//protected: // overridables of base class CDataSetObject
//    virtual void createSchemaVersion( ZS::System::ERowVersion i_version );
//    virtual void createSchemaVersionChgEvArgs( ZS::System::ERowVersion i_version );
public: // overridables of base class CDataSetObject
//    virtual QString getAddTrcInfo( int i_iDetailLevel ) const;
protected: // instance methods (to trace emitting signals)
    void emit_childObjAdded( CDataSetObjGroup* i_pGrp, CDataSetObject* i_pDSObj );
    void emit_childObjAdded( CDataSetObjGroup* i_pGrp, CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
    void emit_childObjAboutToBeRemoved( CDataSetObjGroup* i_pGrp, CDataSetObject* i_pDSObj );
    void emit_childObjAboutToBeRemoved( CDataSetObjGroup* i_pGrp, CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
    void emit_childObjRemoved( CDataSetObjGroup* i_pGrp, CDataSetObject* i_pDSObj );
    void emit_childObjRemoved( CDataSetObjGroup* i_pGrp, CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
protected: // instance members
    // Please note that the group instance hosts references to its child objects.
    // To allow calls like "getChild<T>(idxObj)" the reference lists are administered
    // for all object types but also for each object type explicitly.
    // The schema information is part of messages which may be transferred to another
    // process or even to other computers or networks all over the world. Therefore
    // object references cannot be part of a schema descriptor.
    QHash<QString,CDataSetObject*>  m_hshpChildDSObjsByGrpKey;  // unsorted, key is object type symbol followed by colon and the name of the object (e.g. "DT:Freqs")
    QVector<CDataSetObject*>        m_arpChildDSObjs;           // sorted by Type and Name (Grps, DEs, DTs, DPs), to allow for loops with "getChild( int i_idxChild )"
    // Groups
    QMap<QString,CDataSetObjGroup*> m_mappChildGrpsByName;      // sorted by Name
    QVector<CDataSetObjGroup*>      m_arpChildGrps;             // sorted by Name, to allow for loops with "getChildGroup( int i_idxGrp )"
    // Enumerations
    //QMap<QString,CDataEnumeration*> m_mappChildDEsByName;     // sorted by Name
    //QVector<CDataEnumeration*>      m_arpChildDEs;            // sorted by Name, to allow for loops with "getChildEnum( int i_idxDE )"
    // Tables
    QMap<QString,CDataTable*>       m_mappChildDTsByName;       // sorted by Name
    QVector<CDataTable*>            m_arpChildDTs;              // sorted by Name, to allow for loops with "getChildTable( int i_idxDT )"
    // Parameters
    //QMap<QString,CDataParameter*>   m_mappChildDPsByName;     // sorted by Name
    //QVector<CDataParameter*>        m_arpChildDPs;            // sorted by Name, to allow for loops with "getChildParameter( int i_idxDP )"
    // Method Trace
    Trace::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CDataSetObjGroup

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_DataSetObjGroup_h
