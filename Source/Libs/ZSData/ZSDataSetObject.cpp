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

#include <QtCore/qcoreapplication.h>
#include <QtCore/quuid.h>

#include "ZSData/ZSDataSetObject.h"
#include "ZSData/ZSDataSet.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysLogFile.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Data;


/*******************************************************************************
struct SDSObjSettings
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
SDSObjSettings::SDSObjSettings() :
//------------------------------------------------------------------------------
    m_strName()
{
} // ctor

//------------------------------------------------------------------------------
SDSObjSettings::SDSObjSettings( const QString& i_strName ) :
//------------------------------------------------------------------------------
    m_strName(i_strName)
{
} // ctor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SDSObjSettings::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( !m_strName.isNull() )
    {
        bHas = true;
    }

    return bHas;

} // hasChanges


/*******************************************************************************
class CDataSetObject : public QObject
*******************************************************************************/

/*==============================================================================
protected: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObject::CDataSetObject( EDataSetObjType i_objType, const QString& i_strName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_objType(i_objType),
    m_objState(EObjState::Creating),
    m_pDS(nullptr),
    m_bInitializingSchema(false),
    m_bEditingSchema(false),
    m_bInitializingData(false),
    m_bEditingData(false),
    m_arpDSObjSettings(),
    m_pGrpOrig(nullptr),
    m_rowStateGrpOrig(ERowState::Detached),
    m_pGrpCurr(nullptr),
    m_rowStateGrpCurr(ERowState::Detached),
    m_pTrcAdminObj(nullptr)
{
    // The name of the data set object may be changed during runtime.
    // The name of the Qt Object is used as the name of the trace admin object.
    // It should be possible to active/deactivate tracing of e.g. specific data tables.
    // Also renaming a data set object should be traced. But on the other hand to many
    // trace admin objects is not really good. This problem can not be solved
    // satisfactorily for everybody and for all circumstances. What I decided to do is:
    // - The admin object will be created with the original object name.
    // - On accepting the changed name the Qt Object name will also be changed together
    //   with the name of the trace admin object.
    // - This way the same object may get several trace admin object names during runtime.
    setObjectName(i_strName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // An original version always exists, Current and Proposed only
    // depending on the edit or init mode.
    m_arpDSObjSettings = QVector<SDSObjSettings*>(static_cast<int>(ERowVersion::Undefined), nullptr);
    m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)] = new SDSObjSettings(i_strName);

    // This is the protected ctor used by derived classes.
    // The object state must be set to Created by the ctor of the derived class.
    //m_objState = EObjState::Created;

} // ctor

//------------------------------------------------------------------------------
CDataSetObject::CDataSetObject( const CDataSetObject& i_other ) :
//------------------------------------------------------------------------------
    QObject(),
    m_objType(i_other.m_objType),
    m_objState(EObjState::Creating),
    m_pDS(nullptr),
    m_bInitializingSchema(false),
    m_bEditingSchema(false),
    m_bInitializingData(false),
    m_bEditingData(false),
    m_arpDSObjSettings(),
    m_pGrpOrig(nullptr),
    m_rowStateGrpOrig(ERowState::Detached),
    m_pGrpCurr(nullptr),
    m_rowStateGrpCurr(ERowState::Detached),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( i_other.objectName() );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Other: " + i_other.getDSObjKey();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "copyCtor",
        /* strAddInfo   */ strAddTrcInfo );

    // An original version always exists, Current and Proposed only
    // depending on the edit or init mode.
    // Please note that names must be unique for each object type.
    // After cloning the object the name must be changed afterwards.
    m_arpDSObjSettings = QVector<SDSObjSettings*>(static_cast<int>(ERowVersion::Undefined), nullptr);
    m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)] = new SDSObjSettings(*i_other.m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)]);

    // This is the protected ctor used by derived classes.
    // The object state must be set to Created by the ctor of the derived class.
    //m_objState = EObjState::Created;

} // copy ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObject::~CDataSetObject()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_objState = EObjState::Destroying;

    if( m_pGrpOrig != nullptr )
    {
        // The object may be destroyed because removing the object from its parent group
        // is currently being accepted. In this case "isParentOf" returns false.
        if( m_pGrpOrig->isParentOf(this) )
        {
            m_pGrpOrig->removeChildObj(this);
        }
        m_pGrpOrig = nullptr;
    }

    if( m_pGrpCurr != nullptr )
    {
        // The object may be destroyed because removing the object from its parent group
        // is currently being accepted. In this case "isParentOf" returns false.
        if( m_pGrpCurr->isParentOf(this) )
        {
            m_pGrpCurr->removeChildObj(this);
        }
        m_pGrpCurr = nullptr;
    }

    int idxVersion;

    for( idxVersion = 0; idxVersion < m_arpDSObjSettings.size(); idxVersion++ )
    {
        try
        {
            delete m_arpDSObjSettings[idxVersion];
        }
        catch(...)
        {
        }
        m_arpDSObjSettings[idxVersion] = nullptr;
    }

    m_objState = EObjState::Destroyed;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_objType = static_cast<EDataSetObjType>(0);
    m_objState = static_cast<EObjState>(0);
    m_pDS = nullptr;
    m_bInitializingSchema = false;
    m_bEditingSchema = false;
    m_bInitializingData = false;
    m_bEditingData = false;
    m_arpDSObjSettings.clear();
    m_pGrpOrig = nullptr;
    m_rowStateGrpOrig = static_cast<ERowState>(0);
    m_pGrpCurr = nullptr;
    m_rowStateGrpCurr = static_cast<ERowState>(0);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDataSetObject::getObjTypeStr( EEnumEntryAliasStr i_alias ) const
//------------------------------------------------------------------------------
{
    return dataSetObjType2Str(m_objType, i_alias);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObjGroup* CDataSetObject::getGroup( bool i_bTopMost, ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    CDataSetObjGroup* pGrp = nullptr;

    if( i_version == ERowVersion::Original )
    {
        pGrp = m_pGrpOrig;
    }
    else if( i_version == ERowVersion::Current || i_version == ERowVersion::Proposed )
    {
        pGrp = m_pGrpCurr;
    }
    else // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
    {
        if( m_rowStateGrpCurr == ERowState::Added )
        {
            pGrp = m_pGrpCurr;
        }
        else if( m_rowStateGrpCurr == ERowState::Removed )
        {
            pGrp = nullptr;
        }
        else // if( m_rowStateGrpCurr == ERowState::Detached )
        {
            pGrp = m_pGrpOrig;
        }
    } // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )

    if( i_bTopMost )
    {
        CDataSetObjGroup* pGrpTmp = pGrp;

        // Does not return the DataSet if TopMost = true but the top most group below the DataSet.
        // Returns nullptr if TopMost = true and the parent is the DataSet.
        pGrp = nullptr;

        // Please note that on inserting the name of data set the
        // object keys at client and server site are different.
        while( pGrpTmp->getGroup(i_bTopMost,i_version) != nullptr && pGrpTmp->getGroup(i_bTopMost,i_version) != m_pDS )
        {
            pGrp = pGrpTmp;
            pGrpTmp = pGrp->getGroup(i_bTopMost,i_version);
        }
    }

    return pGrp;

} // getGroup

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObject::beginInitSchema()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "beginInitSchema",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsInitializingSchema: " + bool2Str(m_bInitializingSchema);
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_bInitializingSchema )
    {
        m_bInitializingSchema = true;

        emit_schemaStateChanged(this);
    }

} // beginInitSchema

//------------------------------------------------------------------------------
void CDataSetObject::endInitSchema()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "endInitSchema",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsInitializingSchema: " + bool2Str(m_bInitializingSchema);
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_bInitializingSchema )
    {
        m_bInitializingSchema = false;

        emit_schemaStateChanged(this);
    }

} // endInitSchema

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObject::beginEditSchema()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "beginEditSchema",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsEditingSchema: " + bool2Str(m_bEditingSchema);
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_bEditingSchema )
    {
        m_bEditingSchema = true;

        emit_schemaStateChanged(this);
    }

} // beginEditSchema

//------------------------------------------------------------------------------
void CDataSetObject::endEditSchema()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "endEditSchema",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsEditingSchema: " + bool2Str(m_bEditingSchema);
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_bEditingSchema )
    {
        m_bEditingSchema = false;

        emit_schemaStateChanged(this);
    }

} // endEditSchema

//------------------------------------------------------------------------------
void CDataSetObject::cancelEditSchema()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "cancelEditSchema",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsEditingSchema: " + bool2Str(m_bEditingSchema);
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_bEditingSchema )
    {
        m_bEditingSchema = false;

        emit_schemaStateChanged(this);
    }

} // cancelEditSchema

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CDataSetObject::hasSchemaChanges() const
//------------------------------------------------------------------------------
{
    return false; // m_hshpDSObjSchemaChgDscrs.contains(static_cast<int>(ERowVersion::Current));
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObject::beginInitData()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "beginInitData",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsInitializingData: " + bool2Str(m_bInitializingData);
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_bInitializingData )
    {
        m_bInitializingData = true;

        emit_dataStateChanged(this);
    }

} // beginInitData

//------------------------------------------------------------------------------
void CDataSetObject::endInitData()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "endInitData",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsInitializingData: " + bool2Str(m_bInitializingData);
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_bInitializingData )
    {
        m_bInitializingData = false;

        emit_dataStateChanged(this);
    }

} // endInitData

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObject::beginEditData()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "beginEditData",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsEditingData: " + bool2Str(m_bEditingData);
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_bEditingData )
    {
        m_bEditingData = true;

        emit_dataStateChanged(this);
    }

} // beginEditData

//------------------------------------------------------------------------------
void CDataSetObject::endEditData()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "endEditData",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsEditingData: " + bool2Str(m_bEditingData);
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_bEditingData )
    {
        m_bEditingData = false;

        emit_dataStateChanged(this);
    }

} // endEditData

//------------------------------------------------------------------------------
void CDataSetObject::cancelEditData()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "cancelEditData",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "IsEditingData: " + bool2Str(m_bEditingData);
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_bEditingData )
    {
        m_bEditingData = false;

        emit_dataStateChanged(this);
    }

} // cancelEditData

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CDataSetObject::hasDataChanges() const
//------------------------------------------------------------------------------
{
    return false; // m_hshpDSObjDataChgDscrs.contains(static_cast<int>(ERowVersion::Current));
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObject::setName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setName",
        /* strAddInfo   */ strAddTrcInfo );

    // Get and create (if not yet created) selected version
    //-----------------------------------------------------

    ERowVersion versionSchema = getSchemaDefaultVersion();

    if( m_arpDSObjSettings[static_cast<int>(versionSchema)] == nullptr )
    {
        m_arpDSObjSettings[static_cast<int>(versionSchema)] = new SDSObjSettings(i_strName);
    }

    // Take over value according to selected version
    //----------------------------------------------

    int idxVersion;

    bool bChanged = false;

    QString strNamePrev = getName();

    QVector<QString> arstrNamesPrev(static_cast<int>(ERowVersion::Undefined), QString::null);

    for( idxVersion = 0; idxVersion < static_cast<int>(ERowVersion::Undefined); idxVersion++ )
    {
        arstrNamesPrev[static_cast<int>(idxVersion)] = getName(static_cast<ERowVersion>(idxVersion));
    }

    bool bHasErrorsPrev = hasErrors();

    QVector<bool> arbHasErrorsPrev(static_cast<int>(ERowVersion::Undefined), false);

    for( idxVersion = 0; idxVersion < static_cast<int>(ERowVersion::Undefined); idxVersion++ )
    {
        arbHasErrorsPrev[idxVersion] = hasErrors(static_cast<ERowVersion>(idxVersion));
    }

    if( versionSchema == ERowVersion::Original )
    {
        // Change Qt object name if changed name is Accepted.
        setObjectName(i_strName);

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;

        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

        // If an original version for the value is not yet available ..
        if( m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )
        {
            // ... take over new value as the new original version.
            m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName = i_strName;
            bChanged = true;
        }
        // If there is already an original version for the value available ..
        else // if( !m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )
        {
            // If the new value is different from the previous original version ...
            if( m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName != i_strName )
            {
                // ... take over new value as the new original version.
                m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName = i_strName;
                bChanged = true;
            }
        } // if( !m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )

        // Please note that on changing the original version the current
        // and/or the proposed version becomes superfluous if equal to the
        // original version. But this will be corrected a further lines below.

    } // if( versionSchema == ERowVersion::Original )

    else if( versionSchema == ERowVersion::Current )
    {
        // If a current version for the value is not yet available ..
        if( m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )
        {
            // If the new value is different from the original setting ...
            if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)]->m_strName != i_strName )
            {
                // ... take over new value as the current version to be accepted later on.
                m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName = i_strName;
                bChanged = true;
            }
        }
        // If there is already a current version for the value available ..
        else // if( !m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )
        {
            // If the new value is different from the previous current version ...
            if( m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName != i_strName )
            {
                // ... take over new value as the current version to be accepted later on.
                m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName = i_strName;
                bChanged = true;
            }
        } // if( !m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )

        // Please note that on changing the current version the proposed version
        // becomes superfluous if equal to the current version. But thats been
        // corrected a further lines below.

    } // if( versionSchema == ERowVersion::Current )

    else if( versionSchema == ERowVersion::Proposed )
    {
        // If a proposed version for the value is not yet available ..
        if( m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )
        {
            // If there is a no current version ..
            if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)] == nullptr )
            {
                // .. and if the new value is different from the original version ...
                if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)]->m_strName != i_strName )
                {
                    // .. take over new setting as the proposed version to be accepted later on.
                    m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName = i_strName;
                    bChanged = true;
                }
            }
            // If there is a current version ..
            else // if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)] != nullptr )
            {
                // .. and if the new value is different from the current version ...
                if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)]->m_strName != i_strName )
                {
                    // .. take over new value as the proposed version to be accepted later on.
                    m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName = i_strName;
                    bChanged = true;
                }
            }
        }
        // If there is already a proposed version for the value available ..
        else // if( !m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )
        {
            // If the new value is different from the previous proposed version ...
            if( m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName != i_strName )
            {
                // ... take over new setting as the current version to be accepted later on.
                m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName = i_strName;
                bChanged = true;
            }
        } // if( !m_arpDSObjSettings[static_cast<int>(versionSchema)]->m_strName.isNull() )
    } // if( versionSchema == ERowVersion::Proposed )

    // Get rid of unnecessary versions.
    //---------------------------------

    if( bChanged )
    {
        // If the proposed version is equal to the current version ...
        if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)] != nullptr && m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)] != nullptr )
        {
            if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)]->m_strName == m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)]->m_strName )
            {
                // .. the proposed version is superfluous.
                m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)]->m_strName = QString::null;
                bChanged = true;

                // If the proposed version has no changes anymore ..
                if( !m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)]->hasChanges() )
                {
                    delete m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)];
                    m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)] = nullptr;
                }
            }
        } // if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)] != nullptr && m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)] != nullptr )

        // If the current version is equal to the original version ...
        if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)] != nullptr && m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)] != nullptr )
        {
            if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)]->m_strName == m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)]->m_strName )
            {
                // .. the current version is superfluous.
                m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)]->m_strName = QString::null;
                bChanged = true;

                // If the current version has no changes anymore ..
                if( !m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)]->hasChanges() )
                {
                    delete m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)];
                    m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)] = nullptr;
                }
            }
        } // if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)] != nullptr && m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)] != nullptr )
    } // if( bChanged )

    // Emit changed signals
    //---------------------

    if( bChanged )
    {
        // Please note that the current version may have been invalidated.
        // We need to read the effective setting.
        QString strNameNew = getName();

        QVector<QString> arstrNamesNew(static_cast<int>(ERowVersion::Undefined), QString::null);

        for( idxVersion = 0; idxVersion < static_cast<int>(ERowVersion::Undefined); idxVersion++ )
        {
            arstrNamesNew[idxVersion] = getName(static_cast<ERowVersion>(idxVersion));
        }

        bool bHasErrorsNew = hasErrors();

        QVector<bool> arbHasErrorsNew(static_cast<int>(ERowVersion::Undefined), false);

        for( idxVersion = 0; idxVersion < static_cast<int>(ERowVersion::Undefined); idxVersion++ )
        {
            arbHasErrorsNew[idxVersion] = hasErrors(static_cast<ERowVersion>(idxVersion));
        }

        // Emit "ProposedVersionChanged" before "CurrentVersionChanged" and
        // "CurrentVersionChanged" before "OriginalVersionChanged".
        for( idxVersion = static_cast<int>(ERowVersion::Undefined)-1; idxVersion >= 0; idxVersion-- )
        {
            if( arstrNamesNew[idxVersion] != arstrNamesPrev[idxVersion] )
            {
                emit_nameChanged(this, arstrNamesNew[idxVersion], arstrNamesPrev[idxVersion], static_cast<ERowVersion>(idxVersion));
                m_pDS->onDSObjNameChanged(this, arstrNamesNew[idxVersion], arstrNamesPrev[idxVersion], static_cast<ERowVersion>(idxVersion));
            }
        }

        // Effective setting changed?
        if( strNameNew != strNamePrev )
        {
            emit_nameChanged(this, strNameNew, strNamePrev);
            m_pDS->onDSObjNameChanged(this, strNameNew, strNamePrev, ERowVersion::Undefined);
        }

        // Emit "ProposedVersionChanged" before "CurrentVersionChanged" and
        // "CurrentVersionChanged" before "OriginalVersionChanged".
        for( idxVersion = static_cast<int>(ERowVersion::Undefined)-1; idxVersion >= 0; idxVersion-- )
        {
            if( arbHasErrorsNew[idxVersion] != arbHasErrorsPrev[idxVersion] )
            {
                emit_hasErrorsChanged(this, arbHasErrorsNew[idxVersion], arbHasErrorsPrev[idxVersion], static_cast<ERowVersion>(idxVersion));
                m_pDS->onDSObjHasErrorsChanged(this, arbHasErrorsNew[idxVersion], arbHasErrorsPrev[idxVersion], static_cast<ERowVersion>(idxVersion));
            }
        }

        if( bHasErrorsNew != bHasErrorsPrev )
        {
            emit_hasErrorsChanged(this, bHasErrorsNew, bHasErrorsPrev);
            m_pDS->onDSObjHasErrorsChanged(this, bHasErrorsNew, bHasErrorsPrev, ERowVersion::Undefined);
        }
    } // if( bChanged )

} // setName

//------------------------------------------------------------------------------
QString CDataSetObject::getName( ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( static_cast<int>(i_version) >= 0 && static_cast<int>(i_version) < m_arpDSObjSettings.size() )
    {
        if( m_arpDSObjSettings[static_cast<int>(i_version)] != nullptr )
        {
            strName = m_arpDSObjSettings[static_cast<int>(i_version)]->m_strName;
        }
    } // if( static_cast<int>(i_version) >= 0 && i_version < m_arpDSObjSettings.size() )

    else // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
    {
        if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)] != nullptr && !m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)]->m_strName.isNull() )
        {
            strName = m_arpDSObjSettings[static_cast<int>(ERowVersion::Proposed)]->m_strName;
        }
        else if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)] != nullptr && !m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)]->m_strName.isNull() )
        {
            strName = m_arpDSObjSettings[static_cast<int>(ERowVersion::Current)]->m_strName;
        }
        else if( m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)] != nullptr )
        {
            strName = m_arpDSObjSettings[static_cast<int>(ERowVersion::Original)]->m_strName;
        }
    } // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )

    return strName;

} // getName

//------------------------------------------------------------------------------
QString CDataSetObject::getPath( ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QString strPath = getName(i_version);

    CDataSetObjGroup* pGrp = nullptr;

    if( i_version == ERowVersion::Original )
    {
        if( m_rowStateGrpOrig != ERowState::Removed )
        {
            pGrp = m_pGrpOrig;
        }
    }
    else if( i_version == ERowVersion::Current || i_version == ERowVersion::Proposed )
    {
        if( m_rowStateGrpCurr == ERowState::Added )
        {
            pGrp = m_pGrpOrig;
        }
    }
    else // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
    {
        if( m_rowStateGrpCurr == ERowState::Added )
        {
            pGrp = m_pGrpCurr;
        }
        else if( m_rowStateGrpOrig != ERowState::Removed )
        {
            pGrp = m_pGrpOrig;
        }
        else
        {
            pGrp = m_pGrpOrig;
        }
    } // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )

    while( pGrp != nullptr )
    {
        QString strNameSeparator = CDataSet::GetObjPathSeparator();

        // The name of the data set will not be inserted.
        if( pGrp != m_pDS )
        {
            strPath.insert( 0, strNameSeparator );
            strPath.insert( 0, pGrp->getName(i_version) );
        }
        pGrp = pGrp->getGroup(false, i_version);

    } // while( pGrp != nullptr )

    return strPath;

} // getPath

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDataSetObject::getDSObjKey( ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QString strKey = getPath(i_version);
    strKey = getObjTypeStr(EEnumEntryAliasStrSymbol) + ":" + strKey;
    return strKey;

} // getDSObjKey

//------------------------------------------------------------------------------
QString CDataSetObject::getGrpObjKey( ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QString strGrpKey = getObjTypeStr(EEnumEntryAliasStrSymbol);
    strGrpKey += ":" + getName(i_version);
    return strGrpKey;

} // getGrpObjKey

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDataSetObject::getNameOfParent( bool i_bTopMost, ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QString strName;

    CDataSetObjGroup* pGrp = getGroup(i_bTopMost, i_version);

    if( pGrp != nullptr && pGrp != m_pDS )
    {
        strName = pGrp->getName(i_version);
    }
    return strName;

} // getNameOfParent

//------------------------------------------------------------------------------
QString CDataSetObject::getPathOfParent( bool i_bTopMost, ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QString strPath;

    CDataSetObjGroup* pGrp = getGroup(i_bTopMost, i_version);

    if( pGrp != nullptr )
    {
        strPath = pGrp->getPath(i_version);
    }
    return strPath;

} // getPathOfParent

//------------------------------------------------------------------------------
QString CDataSetObject::getGrpObjKeyOfParent( bool i_bTopMost, ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QString strGrpKey;

    CDataSetObjGroup* pGrp = getGroup(i_bTopMost, i_version);

    if( pGrp != nullptr )
    {
        strGrpKey = pGrp->getGrpObjKey(i_version);
    }
    return strGrpKey;

} // getGrpObjKeyOfParent

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDataSetObject::isChildOf(
    const CDataSetObjGroup* i_pGrp,
    bool                    i_bRecursive,
    ERowVersion             i_version ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    CDataSetObjGroup* pGrp = nullptr;

    if( i_version == ERowVersion::Original )
    {
        if( m_rowStateGrpOrig != ERowState::Removed )
        {
            pGrp = m_pGrpOrig;
        }
    }
    else if( i_version == ERowVersion::Current || i_version == ERowVersion::Proposed )
    {
        if( m_rowStateGrpCurr == ERowState::Added )
        {
            pGrp = m_pGrpOrig;
        }
    }
    else // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
    {
        if( m_rowStateGrpCurr == ERowState::Added )
        {
            pGrp = m_pGrpCurr;
        }
        else if( m_rowStateGrpOrig != ERowState::Removed )
        {
            pGrp = m_pGrpOrig;
        }
        else
        {
            pGrp = m_pGrpOrig;
        }
    } // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )

    if( pGrp == i_pGrp )
    {
        bIs = true;
    }

    if( !bIs && i_bRecursive )
    {
        CDataSetObjGroup* pGrpParent = pGrp;

        while( pGrpParent != nullptr )
        {
            pGrpParent = pGrpParent->getGroup(false, i_version);

            if( pGrpParent == i_pGrp )
            {
                bIs = true;
                break;
            }
        }
    }

    return bIs;

} // isChildOf

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CDataSetObject::hasErrors( ERowVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    bool bHasErrors = false;

    return bHasErrors;

} // hasErrors

/*==============================================================================
protected: // overridables (reserved to be used by CDataSetObjGroup)
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObject::setDataSet( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pDS == nullptr )
        {
            strAddTrcInfo = "DS: nullptr";
        }
        else
        {
            strAddTrcInfo = "DS: " + i_pDS->getDSObjKey();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDataSet",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".setDataSet";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    if( i_pDS == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr( "i_pDS == nullptr" );
        throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
    }

    if( m_pDS != nullptr )
    {
        strAddErrInfo = "Object already belongs to DataSet \"" + m_pDS->getDSObjKey() + "\".";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    m_pDS = i_pDS;

} // setDataSet

//------------------------------------------------------------------------------
void CDataSetObject::setGroup( CDataSetObjGroup* i_pGrp )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setGroup",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".setGroup";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    if( i_pGrp == nullptr && m_pGrpOrig == nullptr )
    {
        strAddErrInfo = "i_pGrp == nullptr";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
    }

    if( i_pGrp != nullptr && m_pGrpOrig != nullptr )
    {
        strAddErrInfo = "Object already belongs to Group \"" + m_pGrpOrig->getDSObjKey() + "\".";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    ERowVersion versionSchema = getSchemaDefaultVersion();

    // Only during "InitSchema" the method "setGroup" has to be used.
    // In any other state the method "setRowState" must be used to add
    // and/or remove the object from a group.
    if( versionSchema != ERowVersion::Original )
    {
        strAddErrInfo = "Method call only allow to originally assign the object to a group.";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultInvalidMethodCall);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    m_pGrpOrig = i_pGrp;

    if( m_pGrpOrig == nullptr )
    {
        m_rowStateGrpOrig = ERowState::Detached; // see comment in header file
    }
    else
    {
        m_rowStateGrpOrig = ERowState::Unchanged; // see comment in header file
    }

} // setGroup

/*==============================================================================
protected: // overridables (reserved to be used by CDataSetObjGroup)
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObject::setRowState( ERowState i_rowState, CDataSetObjGroup* i_pGrp )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "RowState: " + CEnumRowState::toString(i_rowState);
        strAddTrcInfo += ", Grp: " + QString( i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setRowState",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth  = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".setRowState";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    if( i_rowState != ERowState::Added && i_rowState != ERowState::Removed )
    {
        strAddErrInfo = "i_rowState != ERowState::Added && i_rowState != ERowState::Removed";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CDataSetObjGroup* pGrp = i_pGrp;

    // Get selected version
    //---------------------

    ERowVersion versionSchema = getSchemaDefaultVersion();

    // During "InitSchema" the method "setGroup" has to be used.
    if( i_rowState == ERowState::Added && versionSchema == ERowVersion::Original )
    {
        strAddErrInfo = "Method call not allow to originally assign the object to a group.";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultInvalidMethodCall);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_rowState == ERowState::Removed && i_pGrp == nullptr )
    {
        if( versionSchema == ERowVersion::Original )
        {
            pGrp = m_pGrpOrig;
        }
        else
        {
            pGrp = m_pGrpCurr;
        }
    } // if( i_rowState == ERowState::Removed && i_pGrp == nullptr )

    if( pGrp == nullptr )
    {
        // Modifying row state to Added or Removed requires that the group is known
        // from which the object has to be removed or to which the object has to be added.
        strAddErrInfo = "pGrp == nullptr";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
    }

    // Take over value according to selected version
    //----------------------------------------------

    int idxVersion;

    bool bChanged = false;

    CDataSetObjGroup* pGrpPrev = getGroup();

    CDataSetObjGroup* pGrpPrevOrig = m_pGrpOrig;
    CDataSetObjGroup* pGrpPrevCurr = m_pGrpCurr;

    bool bHasErrorsPrev = hasErrors();

    QVector<bool> arbHasErrorsPrev(static_cast<int>(ERowVersion::Undefined), false);

    for( idxVersion = 0; idxVersion < static_cast<int>(ERowVersion::Undefined); idxVersion++ )
    {
        arbHasErrorsPrev[idxVersion] = hasErrors(static_cast<ERowVersion>(idxVersion));
    }

    if( i_rowState == ERowState::Added )
    {
        m_pGrpCurr = i_pGrp;

        if( m_pGrpCurr == m_pGrpOrig )
        {
            // ... take over new value as the new original version.
            m_pGrpCurr = nullptr;
            m_rowStateGrpCurr = ERowState::Detached; // see comment in header file
            bChanged = true;
        }
        else // if( m_pGrpCurr != m_pGrpOrig )
        {
            m_rowStateGrpCurr = ERowState::Added;     // see comment in header file
            bChanged = true;
        }
    } // if( i_rowState == ERowState::Added )

    else if( i_rowState == ERowState::Removed )
    {
        if( i_pGrp == m_pGrpOrig )
        {
            // ... take over new value as the new original version.
            m_pGrpCurr = nullptr;
            m_rowStateGrpCurr = ERowState::Detached;
            m_rowStateGrpOrig = ERowState::Removed;
            bChanged = true;
        }
        else if( i_pGrp == m_pGrpCurr )
        {
            m_rowStateGrpCurr = ERowState::Removed;
            bChanged = true;
        }
        else // if( m_pGrpOrig != i_pGrp && m_pGrpCurr != i_pGrp )
        {
            strAddErrInfo = " m_pGrpOrig != i_pGrp && m_pGrpCurr != i_pGrp";
            errResultInfo.setSeverity(EResultSeverityCritical);
            errResultInfo.setResult(EResultInvalidMethodCall);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
        }
    } // if( i_rowState == ERowState::Removed )

    // Emit changed signals
    //---------------------

    if( bChanged )
    {
        // Please note that the current version may have been invalidated.
        // We need to read the effective setting.
        CDataSetObjGroup* pGrpNew = getGroup();

        CDataSetObjGroup* pGrpNewOrig = m_pGrpOrig;
        CDataSetObjGroup* pGrpNewCurr = m_pGrpCurr;

        bool bHasErrorsNew = hasErrors();

        QVector<bool> arbHasErrorsNew(static_cast<int>(ERowVersion::Undefined), false);

        for( idxVersion = 0; idxVersion < static_cast<int>(ERowVersion::Undefined); idxVersion++ )
        {
            arbHasErrorsNew[idxVersion] = hasErrors(static_cast<ERowVersion>(idxVersion));
        }

        // Emit "ProposedVersionChanged" before "CurrentVersionChanged" and
        // "CurrentVersionChanged" before "OriginalVersionChanged".
        if( pGrpNewCurr != pGrpPrevCurr )
        {
            emit_groupChanged(this, pGrpNewCurr, pGrpPrevCurr, ERowVersion::Current);
            m_pDS->onDSObjGroupChanged(this, pGrpNewCurr, pGrpPrevCurr, ERowVersion::Current);
        }
        if( pGrpNewOrig != pGrpPrevOrig )
        {
            emit_groupChanged(this, pGrpNewOrig, pGrpPrevOrig, ERowVersion::Original);
            m_pDS->onDSObjGroupChanged(this, pGrpNewOrig, pGrpPrevOrig, ERowVersion::Original);
        }

        // Effective setting changed?
        if( pGrpNew != pGrpPrev )
        {
            emit_groupChanged(this, pGrpNew, pGrpPrev);
            m_pDS->onDSObjGroupChanged(this, pGrpNew, pGrpPrev, ERowVersion::Undefined);
        }

        // Emit "ProposedVersionChanged" before "CurrentVersionChanged" and
        // "CurrentVersionChanged" before "OriginalVersionChanged".
        for( idxVersion = static_cast<int>(ERowVersion::Undefined)-1; idxVersion >= 0; idxVersion-- )
        {
            if( arbHasErrorsNew[idxVersion] != arbHasErrorsPrev[idxVersion] )
            {
                emit_hasErrorsChanged(this, arbHasErrorsNew[idxVersion], arbHasErrorsPrev[idxVersion], static_cast<ERowVersion>(idxVersion));
                m_pDS->onDSObjHasErrorsChanged(this, arbHasErrorsNew[idxVersion], arbHasErrorsPrev[idxVersion], static_cast<ERowVersion>(idxVersion));
            }
        }

        if( bHasErrorsNew != bHasErrorsPrev )
        {
            emit_hasErrorsChanged(this, bHasErrorsNew, bHasErrorsPrev);
            m_pDS->onDSObjHasErrorsChanged(this, bHasErrorsNew, bHasErrorsPrev, ERowVersion::Undefined);
        }
    } // if( bChanged )

} // setRowState

/*==============================================================================
public: // instance methods (reserved to be used by CDataSetObjGroup)
==============================================================================*/

//------------------------------------------------------------------------------
bool CDataSetObject::isUnchanged() const
//------------------------------------------------------------------------------
{
    return (m_rowStateGrpOrig == ERowState::Unchanged && m_rowStateGrpCurr == ERowState::Detached);
}

//------------------------------------------------------------------------------
bool CDataSetObject::isDetached() const
//------------------------------------------------------------------------------
{
    return (m_rowStateGrpOrig == ERowState::Detached && m_rowStateGrpCurr == ERowState::Detached);
}

//------------------------------------------------------------------------------
bool CDataSetObject::isModified() const
//------------------------------------------------------------------------------
{
    return (m_rowStateGrpCurr == ERowState::Modified);
}

//------------------------------------------------------------------------------
bool CDataSetObject::isAdded( const CDataSetObjGroup* i_pGrp, ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    if( i_version == ERowVersion::Original )
    {
        if( m_rowStateGrpOrig == ERowState::Added && m_pGrpOrig == i_pGrp )
        {
            bIs = true;
        }
    }
    else if( i_version == ERowVersion::Current )
    {
        if( m_rowStateGrpCurr == ERowState::Added && m_pGrpCurr == i_pGrp )
        {
            bIs = true;
        }
    }
    else if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )
    {
        if( m_rowStateGrpCurr != ERowState::Undefined )
        {
            if( m_rowStateGrpCurr == ERowState::Added && m_pGrpCurr == i_pGrp )
            {
                bIs = true;
            }
        }
        else if( m_rowStateGrpOrig != ERowState::Undefined )
        {
            if( m_rowStateGrpOrig == ERowState::Added && m_pGrpOrig == i_pGrp )
            {
                bIs = true;
            }
        }
    } // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )

    return bIs;

} // isAdded

//------------------------------------------------------------------------------
bool CDataSetObject::isRemoved( const CDataSetObjGroup* i_pGrp, ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;

    if( i_version == ERowVersion::Original )
    {
        if( m_rowStateGrpOrig == ERowState::Removed && m_pGrpOrig == i_pGrp )
        {
            bIs = true;
        }
    }
    else if( i_version == ERowVersion::Current )
    {
        if( m_rowStateGrpCurr == ERowState::Removed && m_pGrpCurr == i_pGrp )
        {
            bIs = true;
        }
    }
    else if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )
    {
        if( m_rowStateGrpCurr != ERowState::Undefined )
        {
            if( m_rowStateGrpCurr == ERowState::Removed && m_pGrpCurr == i_pGrp )
            {
                bIs = true;
            }
        }
        else if( m_rowStateGrpOrig != ERowState::Undefined )
        {
            if( m_rowStateGrpOrig == ERowState::Removed && m_pGrpOrig == i_pGrp )
            {
                bIs = true;
            }
        }
    } // if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )

    return bIs;

} // isRemoved

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
ERowVersion CDataSetObject::getSchemaDefaultVersion() const
//------------------------------------------------------------------------------
{
    ERowVersion version = ERowVersion::Current;

    // Initializing schema must be set
    // - by the database on reading the schema from XML file or
    // - by clients on merging received data from the database.
    if( isInitializingSchema() )
    {
        version = ERowVersion::Original;
    }
    // If myself is going to be destroyed ..
    else if( m_objState == EObjState::Destroying )
    {
        // .. it will not be just the row states to be set to Removed
        // but the objects will be really destroyed.
        version = ERowVersion::Original;
    }
    // If the data set is going to be destroyed ..
    else if( m_pDS != nullptr && m_pDS->getObjState() == EObjState::Destroying )
    {
        // .. it will not be just the row states to be set to Removed
        // but the objects will be really destroyed.
        version = ERowVersion::Original;
    }

    return version;

} // getSchemaDefaultVersion

/*==============================================================================
protected: // instance methods (to trace emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObject::emit_schemaStateChanged( CDataSetObject* i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_schemaStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit schemaStateChanged(i_pDSObj);

} // emit_schemaStateChanged

//------------------------------------------------------------------------------
void CDataSetObject::emit_dataStateChanged( CDataSetObject* i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_dataStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit dataStateChanged(i_pDSObj);

} // emit_dataStateChanged

//------------------------------------------------------------------------------
void CDataSetObject::emit_rowStateChanged(
    CDataSetObject* i_pDSObj,
    ERowState       i_rowStateNew,
    ERowState       i_rowStatePrev )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", RowStateNew: " + CEnumRowState::toString(i_rowStateNew);
        strAddTrcInfo += ", RowStatePrev: " + CEnumRowState::toString(i_rowStatePrev);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_rowStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit rowStateChanged(i_pDSObj, i_rowStateNew, i_rowStatePrev);

} // emit_rowStateChanged

//------------------------------------------------------------------------------
void CDataSetObject::emit_nameChanged(
    CDataSetObject* i_pDSObj,
    const QString&  i_strNameNew,
    const QString&  i_strNamePrev )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", NameNew: " + i_strNameNew;
        strAddTrcInfo += ", NamePrev: " + i_strNamePrev;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_nameChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit nameChanged(i_pDSObj, i_strNameNew, i_strNamePrev);

} // emit_nameChanged

//------------------------------------------------------------------------------
void CDataSetObject::emit_nameChanged(
    CDataSetObject* i_pDSObj,
    const QString&  i_strNameNew,
    const QString&  i_strNamePrev,
    ERowVersion     i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", NameNew: " + i_strNameNew;
        strAddTrcInfo += ", NamePrev: " + i_strNamePrev;
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_nameChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit nameChanged(i_pDSObj, i_strNameNew, i_strNamePrev, i_version);

} // emit_nameChanged

//------------------------------------------------------------------------------
void CDataSetObject::emit_groupChanged(
    CDataSetObject*   i_pDSObj,
    CDataSetObjGroup* i_pGrpNew,
    CDataSetObjGroup* i_pGrpPrev )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", GrpNew: " + QString( i_pGrpNew == nullptr ? "nullptr" : i_pGrpNew->getPath() );
        strAddTrcInfo += ", GrpPrev: " + QString( i_pGrpPrev == nullptr ? "nullptr" : i_pGrpPrev->getPath() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_groupChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit groupChanged(i_pDSObj, i_pGrpNew, i_pGrpPrev);

} // emit_groupChanged

//------------------------------------------------------------------------------
void CDataSetObject::emit_groupChanged(
    CDataSetObject*   i_pDSObj,
    CDataSetObjGroup* i_pGrpNew,
    CDataSetObjGroup* i_pGrpPrev,
    ERowVersion       i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", GrpNew: " + QString( i_pGrpNew == nullptr ? "nullptr" : i_pGrpNew->getPath() );
        strAddTrcInfo += ", GrpPrev: " + QString( i_pGrpPrev == nullptr ? "nullptr" : i_pGrpPrev->getPath() );
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_groupChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit groupChanged(i_pDSObj, i_pGrpNew, i_pGrpPrev, i_version);

} // emit_groupChanged

//------------------------------------------------------------------------------
void CDataSetObject::emit_hasErrorsChanged(
    CDataSetObject* i_pDSObj,
    bool            i_bHasErrorsNew,
    bool            i_bHasErrorsPrev )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", HasErrorsNew: " + bool2Str(i_bHasErrorsNew);
        strAddTrcInfo += ", HasErrorsPrev: " + bool2Str(i_bHasErrorsPrev);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_hasErrorsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit hasErrorsChanged(i_pDSObj, i_bHasErrorsNew, i_bHasErrorsPrev);

} // emit_hasErrorsChanged

//------------------------------------------------------------------------------
void CDataSetObject::emit_hasErrorsChanged(
    CDataSetObject* i_pDSObj,
    bool            i_bHasErrorsNew,
    bool            i_bHasErrorsPrev,
    ERowVersion     i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", HasErrorsNew: " + bool2Str(i_bHasErrorsNew);
        strAddTrcInfo += ", HasErrorsPrev: " + bool2Str(i_bHasErrorsPrev);
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_hasErrorsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit hasErrorsChanged(i_pDSObj, i_bHasErrorsNew, i_bHasErrorsPrev, i_version);

} // hasErrorsChanged
