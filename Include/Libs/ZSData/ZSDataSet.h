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

#ifndef ZSData_DataSet_h
#define ZSData_DataSet_h

#include <QtCore/qobject.h>
#include <QtCore/qhash.h>
#include <QtCore/qvector.h>

#include "ZSData/ZSDataCommon.h"
//#include "ZSData/ZSDataParameter.h"
#include "ZSData/ZSDataSetObjGroup.h"
//#include "ZSData/ZSDataTable.h"
#include "ZSSys/ZSSysErrResult.h"


namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDataSetObject;
class CDataSetObjGroup;
class CDataTable;

//******************************************************************************
class ZSDATADLL_API CDataSet : public CDataSetObjGroup
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::DS"; }
    static QString ClassName() { return "CDataSet"; }
public: // class methods
    static void SetObjPathSeparator( const QString& i_strSeparator );
    static QString GetObjPathSeparator() { return s_strObjPathSeparator; }
public: // ctors and dtor
    CDataSet( const QString& i_strName );
    virtual ~CDataSet();
private: // copy ctor not allowed
    CDataSet( const CDataSet& i_other );
private: // assignment operator not allowed
    CDataSet& operator = ( const CDataSet& i_other );
signals:
    void dsObjAdded( ZS::Data::CDataSetObject* i_pDSObj );
    void dsObjAdded( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
    void dsObjAboutToBeRemoved( ZS::Data::CDataSetObject* i_pDSObj );
    void dsObjAboutToBeRemoved( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
    void dsObjRemoved( ZS::Data::CDataSetObject* i_pDSObj );
    void dsObjRemoved( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
public: // overridables
    virtual QString nameSpace() const { return CDataSet::NameSpace(); }
    virtual QString className() const { return CDataSet::ClassName(); }
public: // overridables of base class CDataSetObjGroup
    virtual void clear();
public: // overridables
    //virtual ZS::System::SErrResultInfo merge( const CDataSetObject* i_pDSOther ); // Merges the schema and data from DSOther into this data set object.
    //virtual ZS::System::SErrResultInfo merge( const CChangeDataSet* i_pChgDS );
public: // instance methods
    virtual void enforceConstraints( bool i_bEnforce );
    bool areConstraintsEnforced() const { return m_bConstraintsEnforced; }
public: // instance methods
    CDataSetObject* findObjectByKey( const QString& i_strKey ) const;    // The key must start with the object type specifier and ":" followed by the complete path to the object (e.g. "DP:SigGen/Frequency").
    //CDataSetObject* findObjectByPath( EDataSetObjType i_dsObjType, const QString& i_strPath ) const;
    int getObjectsCount( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    CDataSetObject* getObject( int i_idxObj, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // overridables
    virtual CDataSetObject* addObj( EDataSetObjType i_dsObjType, const QString& i_strObjName, CDataSetObjGroup* i_pGrpParent = nullptr );
    virtual CDataSetObject* addObj( EDataSetObjType i_dsObjType, const QString& i_strObjName, const QString& i_strGrpPathParent );
    virtual CDataSetObject* addObj( CDataSetObject* i_pDSObj, CDataSetObjGroup* i_pGrpParent = nullptr );
    virtual CDataSetObject* addObj( CDataSetObject* i_pDSObj, const QString& i_strGrpPathParent );
    virtual void removeObj( CDataSetObject* i_pDSObj );
    virtual void removeObj( EDataSetObjType i_dsObjType, const QString& i_strObjPath );
    virtual void removeObj( EDataSetObjType i_dsObjType, const QString& i_strObjName, const QString& i_strGrpPathParent );
public: // overridables
    virtual CDataSetObjGroup* addGroup( const QString& i_strGrpName, CDataSetObjGroup* i_pGrpParent = nullptr );
    virtual CDataSetObjGroup* addGroup( const QString& i_strGrpName, const QString& i_strGrpPathParent );
    virtual CDataSetObjGroup* addGroup( CDataSetObjGroup* i_pGrp, CDataSetObjGroup* i_pGrpParent = nullptr );
    virtual CDataSetObjGroup* addGroup( CDataSetObjGroup* i_pGrp, const QString& i_strGrpPathParent );
    virtual void removeGroup( CDataSetObjGroup* i_pGrp );
    virtual void removeGroup( const QString& i_strGrpPath );
    virtual void removeGroup( const QString& i_strGrpName, const QString& i_strGrpPathParent );
public: // instance methods
    CDataSetObjGroup* findGroupByPath( const QString& i_strPath ) const;
    //CDataSetObjGroup* findGroupByPath( const QString& i_strObjPathParent, const QString& i_strObjName ) const;
    //int getGroupsCount( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    //CDataSetObjGroup* getGroup( int i_idxObj, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // overridables
    //virtual CDataTable* addTable( const QString& i_strObjName, const QString& i_strObjUIdParent = "" );
    //virtual void removeTable( CDataTable* i_pDT );
    //virtual void removeTableByPath( const QString& i_strObjPath );
    //virtual void removeTableByPath( const QString& i_strObjPathParent, const QString& i_strObjName );
public: // instance methods
    //CDataSetObjGroup* findTableByPath( const QString& i_strObjPath ) const;
    //CDataSetObjGroup* findTableByPath( const QString& i_strObjPathParent, const QString& i_strObjName ) const;
    //int getTablesCount( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    //CDataTable* getTable( int i_idxObj, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // instance methods
    //ZS::System::SErrResultInfo set2Default();
public: // overridables
    virtual void resolveObjectReferences();
    //virtual void invalidateObjectReferences();
public: // instance methods
    //void reparentObject( CDataSetObject* i_pDSObj, CDataSetObjGroup* i_pGrpParentNew );
public: // overridables
    virtual void onDSObjAdded( ZS::Data::CDataSetObject* i_pDSObj, ZS::Data::CDataSetObjGroup* i_pGrpParent );
    virtual void onDSObjAboutToBeRemoved( ZS::Data::CDataSetObject* i_pDSObj, ZS::Data::CDataSetObjGroup* i_pGrpParent );
    virtual void onDSObjRemoved( ZS::Data::CDataSetObject* i_pDSObj, ZS::Data::CDataSetObjGroup* i_pGrpParent );
public: // overridables
    virtual void onDSObjNameChanged( CDataSetObject* i_pDSObj, const QString& i_strNameNew, const QString& i_strNamePrev, ZS::System::ERowVersion i_version );
    virtual void onDSObjGroupChanged( CDataSetObject* i_pDSObj, CDataSetObjGroup* i_pGrpNew, CDataSetObjGroup* i_pGrpPrev, ZS::System::ERowVersion i_version );
public: // overridables
    virtual void onDSObjHasErrorsChanged( CDataSetObject* i_pDSObj, bool i_bHasErrorsNew, bool i_bHasErrorsPrev, ZS::System::ERowVersion i_version );
protected: // instance methods (to trace emitting signals)
    void emit_dsObjAdded( CDataSetObject* i_pDSObj );
    void emit_dsObjAboutToBeRemoved( CDataSetObject* i_pDSObj );
    void emit_dsObjRemoved( CDataSetObject* i_pDSObj );
protected: // class members
    static QString                  s_strObjPathSeparator;
protected: // instance members
    bool                            m_bConstraintsEnforced;
    // Please note that the data set instance hosts references to the data set objects.
    // To allow calls like "getChild<T>(idxObj)" the reference lists are administered
    // for all object types but also for each object type explicitly.
    // The path of an object is the name of the object preceded by the names of its parent groups
    // (but without name of data set) separated by the separation character (default "/").
    QHash<QString,CDataSetObject*>  m_hshpDSObjsByKey;       // unsorted, key is object type symbol followed by colon and the path of the object (e.g. "DT:SigGen:Freqs")
    QVector<CDataSetObject*>        m_arpDSObjs;             // sorted by Type and Path (Grps, DEs, DTs, DPs), to allow for loops with "getDSObj( int i_idxObj )"
    // Groups
    QMap<QString,CDataSetObjGroup*> m_mappGrpsByPath;        // sorted by Path
    QVector<CDataSetObjGroup*>      m_arpGrps;               // sorted by Path, to allow for loops with "getGroup( int i_idxGrp )"
    // Tables
    QMap<QString,CDataTable*>       m_mappDTsByPath;         // sorted by Path
    QVector<CDataTable*>            m_arpDTs;                // sorted by Path, to allow for loops with "getTable( int i_idxTbl )"
    // Method Trace
    Trace::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CDataSet

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_DataSet_h
