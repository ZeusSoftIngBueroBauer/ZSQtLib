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

#ifndef ZSData_DataSetObject_h
#define ZSData_DataSetObject_h

#include <QtCore/qobject.h>

#include "ZSData/ZSDataCommon.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"


class QXmlStreamReader;
class QXmlStreamWriter;


namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDataSet;
class CDataSetObjGroup;

//******************************************************************************
struct ZSDATADLL_API SDSObjSettings
//******************************************************************************
{
public: // ctors
    SDSObjSettings();  // ctor for invalid settings
    SDSObjSettings( const QString& i_strName );
public: // struct methods
    bool hasChanges() const;
public: // struct members
    QString m_strName;

}; // SDSObjSettings


//******************************************************************************
class ZSDATADLL_API CDataSetObject : public QObject
/*  Terminology about data set object names and keys:
    On adding objects to a data set they become part of lists and hashes within
    the data set and their parent group.
    To uniquely identify and clarify the usage of the objects the following names
    and key identifiers are used.
    1.  ObjectName (getName, setName)
        The ObjectName should clarify the usage of the object (e.g. "SampleRate",
        "Customers", etc.).
    2.  GrpObjKey (getGrpObjKey)
        As mentioned above objects within a data set must have a user readable
        name clarifying their usage. It may turn out that below the same group
        different objects have the same usage. E.g. if a parameter is named as
        "SampleRate" there might also be an enumeration object named "SampleRate"
        below the same group. To uniquely identify the object within the child list
        of the group the object type [Grp, Enum, DP, DT] is inserted at the
        beginning of the object name to build the key of of those objects within
        the parent group. E.g. the parameter "SampleRate" is added with the key
        "DP:SampleRate" into its parent group's child lists.
    3.  DSObjKey (getDSObjKey)
        The key of the object within a data set is comprised of the names of all
        parent group names followed by the name of the object. For this you may
        also refer to the DSObjKey as the object's path.
        A separator is appended at each group name. As different object types
        may have the same name below the same group (see "GrpObjectKey" above)
        also for the DSObjKey the object type is inserted at the beginning of
        the key. On using "/" as the group separator the parameter
        "SampleRate" below the group "SigGen" will get the ObjectKey
        "DP:SigGen/SampleRate" and the group "SigGen" will get the ObjectKey
        "Grp:SigGen". Please note that the name of the data set itself is not
        used to build the DSObjKey as the "same" data set may get different
        names depending on where they have been initially created (e.g. "Db"
        if created within the database or "DbCltSigGen" if created within a
        database client for signal generation.
*******************************************************************************/
{
friend class CDataSetObjGroup;
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::DS"; }
    static QString ClassName() { return "CDataSetObject"; }
protected: // ctors (Only classed derived from CDataSetObject with a "concrete" type can be "directly" created)
    CDataSetObject( EDataSetObjType i_objType, const QString& i_strName );
    CDataSetObject( const CDataSetObject& i_other );
public: // dtor
    virtual ~CDataSetObject();
private: // assignment operator not allowed
    CDataSetObject& operator = ( const CDataSetObject& i_other );
signals:
    void schemaStateChanged( ZS::Data::CDataSetObject* i_pDSObj );
    void dataStateChanged( ZS::Data::CDataSetObject* i_pDSObj );
signals:
    void rowStateChanged( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowState i_rowStateNew, ZS::System::ERowState i_rowStatePrev );
signals:
    void nameChanged( ZS::Data::CDataSetObject* i_pDSObj, const QString& i_strNameNew, const QString& i_strNamePrev );
    void nameChanged( ZS::Data::CDataSetObject* i_pDSObj, const QString& i_strNameNew, const QString& i_strNamePrev, ZS::System::ERowVersion i_version );
signals:
    void groupChanged( ZS::Data::CDataSetObject* i_pDSObj, ZS::Data::CDataSetObjGroup* i_pGrpNew, ZS::Data::CDataSetObjGroup* i_pGrpPrev );
    void groupChanged( ZS::Data::CDataSetObject* i_pDSObj, ZS::Data::CDataSetObjGroup* i_pGrpNew, ZS::Data::CDataSetObjGroup* i_pGrpPrev, ZS::System::ERowVersion i_version );
signals:
    void hasErrorsChanged( ZS::Data::CDataSetObject* i_pDSObj, bool i_bHasErrorsNew, bool i_bHasErrorsPrev );
    void hasErrorsChanged( ZS::Data::CDataSetObject* i_pDSObj, bool i_bHasErrorsNew, bool i_bHasErrorsPrev, ZS::System::ERowVersion i_version );
public: // overridables
    virtual QString nameSpace() const { return CDataSetObject::NameSpace(); }
    virtual QString className() const { return CDataSetObject::ClassName(); }
public: // instance methods
    EDataSetObjType getObjType() const { return m_objType; }
    QString getObjTypeStr( ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName ) const;
public: // instance methods
    ZS::System::EObjState getObjState() const { return m_objState; }
public: // instance methods
    CDataSet* getDataSet() { return m_pDS; }
    CDataSetObjGroup* getGroup( bool i_bTopMost = false, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const; // Does not return the DataSet if TopMost = true but the top most group below the DataSet. Returns nullptr if TopMost = true and the parent is the DataSet.
public: // overridables
    virtual void beginInitSchema();
    virtual void endInitSchema();
    virtual bool isInitializingSchema() const { return m_bInitializingSchema; }
public: // overridables
    virtual void beginEditSchema();
    virtual void endEditSchema();
    virtual void cancelEditSchema();
    virtual bool isEditingSchema() const { return m_bEditingSchema; }
public: // overridables
    virtual bool hasSchemaChanges() const;
public: // overridables
    virtual void beginInitData();
    virtual void endInitData();
    virtual bool isInitializingData() const { return m_bInitializingData; }
public: // overridables
    virtual void beginEditData();
    virtual void endEditData();
    virtual void cancelEditData();
    virtual bool isEditingData() const { return m_bEditingData; }
public: // overridables
    virtual bool hasDataChanges() const;
public: // overridables
    virtual void setName( const QString& i_strName );
    QString getName( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;    // Name of object (e.g. "Frequency")
    QString getPath( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;    // All parent names plus this object name (e.g. "SigGen/Frequency") but excluding name of DataSet.
public: // instance methods
    // Objects with the same name but with different object types may belong to the same parent group
    // (an enumeration called "Mode" and a parameter called "Mode"). To uniquely identify those objects
    // the key contains the object type.
    QString getGrpObjKey( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;  // The group key of the object is the name starting with object type symbol and ":" (e.g. "DP:Frequency")
    QString getDSObjKey( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;     // The key of the object is the path starting with object type symbol and ":" (e.g. "DP:SigGen/Frequency") but excluding name of DataSet.
public: // instance methods
    // Please note that the following methods don't return the name of the DataSet if TopMost = true
    // but the name of the top most group below the DataSet. If TopMost = true and the parent is the
    // DataSet the methods return an empty string.
    QString getNameOfParent( bool i_bTopMost = false, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    QString getPathOfParent( bool i_bTopMost = false, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    QString getGrpObjKeyOfParent( bool i_bTopMost = false, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    QString getDSObjKeyOfParent( bool i_bTopMost = false, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // instance methods
    bool isChildOf( const CDataSetObjGroup* i_pGrp, bool i_bRecursive = true, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const; // If not recursive only one level up is checked.
public: // instance methods
    virtual bool hasErrors( ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
protected: // overridables (reserved to be used by CDataSetObjGroup)
    virtual void setDataSet( CDataSet* i_pDS );
    virtual void setGroup( CDataSetObjGroup* i_pGrp );
protected: // overridables (reserved to be used by CDataSetObjGroup)
    virtual void setRowState( ZS::System::ERowState i_rowState, CDataSetObjGroup* i_pGrp = nullptr ); // Grp == nullptr is only allow if rowState = Removed
    ZS::System::ERowState getRowState( const CDataSetObjGroup* i_pGrp, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // instance methods (reserved to be used by CDataSetObjGroup)
    bool isUnchanged() const;
    bool isDetached() const;
    bool isModified() const;
    bool isAdded( const CDataSetObjGroup* i_pGrp, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
    bool isRemoved( const CDataSetObjGroup* i_pGrp, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Default ) const;
public: // overridables
    virtual ZS::System::ERowVersion getSchemaDefaultVersion() const;
protected: // instance methods (to trace emitting signals)
    void emit_schemaStateChanged( CDataSetObject* i_pDSObj );
    void emit_dataStateChanged( CDataSetObject* i_pDSObj );
    void emit_rowStateChanged( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowState i_rowStateNew, ZS::System::ERowState i_rowStatePrev );
    void emit_nameChanged( CDataSetObject* i_pDSObj, const QString& i_strNameNew, const QString& i_strNamePrev );
    void emit_nameChanged( CDataSetObject* i_pDSObj, const QString& i_strNameNew, const QString& i_strNamePrev, ZS::System::ERowVersion i_version );
    void emit_groupChanged( CDataSetObject* i_pDSObj, CDataSetObjGroup* i_pGrpNew, CDataSetObjGroup* i_pGrpPrev );
    void emit_groupChanged( CDataSetObject* i_pDSObj, CDataSetObjGroup* i_pGrpNew, CDataSetObjGroup* i_pGrpPrev, ZS::System::ERowVersion i_version );
    void emit_hasErrorsChanged( CDataSetObject* i_pDSObj, bool i_bHasErrorsNew, bool i_bHasErrorsPrev );
    void emit_hasErrorsChanged( CDataSetObject* i_pDSObj, bool i_bHasErrorsNew, bool i_bHasErrorsPrev, ZS::System::ERowVersion i_version );
protected: // instance members
    EDataSetObjType            m_objType;
    ZS::System::EObjState      m_objState;
    CDataSet*                  m_pDS;                   // May be equal to "this" if this is the data set itself.
    bool                       m_bInitializingSchema;   // Indicates whether the schema is being initialized (SchemaRowVersion == Original)
    bool                       m_bEditingSchema;        // Indicates whether the schema is being edited (SchemaRowVersion == Proposed)
    bool                       m_bInitializingData;     // Indicates whether the data is being initialized (DataRowVersion == Original)
    bool                       m_bEditingData;          // Indicates whether the data is being edited (SchemaRowVersion == Proposed)
    QVector<SDSObjSettings*>   m_arpDSObjSettings;      // Original always exists, Current and Proposed only depending on the edit mode.
    // On moving (reparenting) the object between groups the object may belong to two different groups:
    // The group where the object has been removed from (where the object originally belonged to)
    // and the group where the object has been moved to (currently belongs to).
    // If a view does not filter for versions the object will be indicated as a child of both groups.
    // In the original group with row state Removed and in the new group with row state Added.
    // After accepting the Added/Removed row states the current group becomes the only group the object belongs to.
    CDataSetObjGroup*          m_pGrpOrig;         // May be equal to DataSet if this is the "direct" child of the data set.
    ZS::System::ERowState      m_rowStateGrpOrig;  // Detached, if created but not added to a group. "Unchanged", if accepted. Removed if moved to Current Group.
    CDataSetObjGroup*          m_pGrpCurr;         // May be equal to DataSet if this is the "direct" child of the data set.
    ZS::System::ERowState      m_rowStateGrpCurr;  // Detached, if created but not added to a group. Undefined, if accepted. Added if moved from Original Group to Current Grop. Removed, if not moved but deleted and not yet accepted.
    Trace::CTrcAdminObj*       m_pTrcAdminObj;

}; // class CDataSetObject

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_DataSetObject_h
