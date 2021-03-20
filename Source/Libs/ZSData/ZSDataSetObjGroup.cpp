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
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSData/ZSDataSetObjGroup.h"
#include "ZSData/ZSDataExceptions.h"
#include "ZSData/ZSDataSet.h"
#include "ZSData/ZSDataTable.h"
#include "ZSPhysVal/ZSPhysValArr.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysLogFile.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Data;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDataSetObjectGroup : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObjGroup::CDataSetObjGroup( const QString& i_strGrpName ) :
//------------------------------------------------------------------------------
    CDataSetObject(EDataSetObjTypeGroup, i_strGrpName),
    m_hshpChildDSObjsByGrpKey(),
    m_arpChildDSObjs(),
    // Groups
    m_mappChildGrpsByName(),
    m_arpChildGrps(),
    // Enumerations
    //m_mappChildDEsByName(),
    //m_arpChildDEs(),
    // Tables
    m_mappChildDTsByName(),
    m_arpChildDTs(),
    // Parameters
    //m_mappChildDPsByName(),
    //m_arpChildDPs(),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_objState = EObjState::Created;

} // ctor

//------------------------------------------------------------------------------
CDataSetObjGroup::CDataSetObjGroup( const CDataSetObjGroup& i_other ) :
//------------------------------------------------------------------------------
    CDataSetObject(i_other),
    m_hshpChildDSObjsByGrpKey(),
    m_arpChildDSObjs(),
    // Groups
    m_mappChildGrpsByName(),
    m_arpChildGrps(),
    // Enumerations
    //m_mappChildDEsByName(),
    //m_arpChildDEs(),
    // Tables
    m_mappChildDTsByName(),
    m_arpChildDTs(),
    // Parameters
    //m_mappChildDPsByName(),
    //m_arpChildDPs(),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
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

    m_objState = EObjState::Created;

} // copy ctor

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObjGroup::CDataSetObjGroup( EDataSetObjType i_dsObjType, const QString& i_strGrpName ) :
//------------------------------------------------------------------------------
    CDataSetObject(i_dsObjType, i_strGrpName),
    m_hshpChildDSObjsByGrpKey(),
    m_arpChildDSObjs(),
    // Groups
    m_mappChildGrpsByName(),
    m_arpChildGrps(),
    // Enumerations
    //m_mappChildDEsByName(),
    //m_arpChildDEs(),
    // Tables
    m_mappChildDTsByName(),
    m_arpChildDTs(),
    // Parameters
    //m_mappChildDPsByName(),
    //m_arpChildDPs(),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // This is the protected ctor used by derived classes (DataSet).
    // The object state must be set to Created by the ctor of the derived class.
    //m_objState = EObjState::Created;

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObjGroup::~CDataSetObjGroup()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_objState = EObjState::Destroying;

    try
    {
        clear();
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_hshpChildDSObjsByGrpKey.clear();
    //m_arpChildDSObjs.clear();
    // Groups
    //m_mappChildGrpsByName.clear();
    //m_arpChildGrps.clear();
    // Enumerations
    //m_mappChildDEsByName.clear();
    //m_arpChildDEs.clear();
    // Tables
    //m_mappChildDTsByName.clear();
    //m_arpChildDTs.clear();
    // Parameters
    //m_mappChildDPsByName.clear();
    //m_arpChildDPs.clear();
    // Method Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObjGroup::clear()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "clear",
        /* strAddInfo   */ "" );

    CDataSetObjGroup* pGrp;
    //CDataEnumeration* pDE;
    CDataTable*       pDT;
    //CDataParameter*   pDP;
    int               idxObj;

    ERowVersion versionSchema = getSchemaDefaultVersion();

    // Delete the data set objects
    //----------------------------

    // Delete parameters and tables first (those objects may have references to enumerations and may be child of groups).

    //if( m_arpChildDPs.size() > 0 )
    //{
    //    for( idxObj = m_arpChildDPs.size()-1; idxObj >= 0; idxObj-- )
    //    {
    //        pDP = m_arpChildDPs[idxObj];

    //        // The DSObjs dtor will remove itself from the group.
    //        delete pDP;
    //        pDP = nullptr;
    //    }
    //}

    if( m_arpChildDTs.size() > 0 )
    {
        for( idxObj = m_arpChildDTs.size()-1; idxObj >= 0; idxObj-- )
        {
            pDT = m_arpChildDTs[idxObj];

            if( versionSchema == ERowVersion::Original )
            {
                // The DSObjs dtor will remove itself from the data set.
                delete pDT;
                pDT = nullptr;
            }
            else
            {
                pGrp = pDT->getGroup();
                pGrp->removeChildObj(pDT);
            }
        }
    } // if( m_arpChildDTs.size() > 0 )

    // Delete enumerations second (those objects may be child of groups).
    //if( m_arpChildDEs.size() > 0 )
    //{
    //    for( idxObj = m_arpChildDEs.size()-1; idxObj >= 0; idxObj-- )
    //    {
    //        pDE = m_arpChildDEs[idxObj];

    //        // The DSObjs dtor will remove itself from the group.
    //        delete pDE;
    //        pDE = nullptr;
    //    }
    //}

    // Delete groups at last.
    if( m_arpChildGrps.size() > 0 )
    {
        for( idxObj = m_arpChildGrps.size()-1; idxObj >= 0; idxObj-- )
        {
            pGrp = m_arpChildGrps[idxObj];

            if( versionSchema == ERowVersion::Original )
            {
                // The DSObjs dtor will remove itself from the data set.
                delete pGrp;
                pGrp = nullptr;
            }
            else
            {
                pGrp = pGrp->getGroup();
                pGrp->removeChildObj(pGrp);
            }
        }
    } // if( m_arpChildGrps.size() > 0 )

    // After "clearing" the group the child lists with data set objects must be empty.
    if( m_hshpChildDSObjsByGrpKey.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_hshpChildDSObjsByGrpKey.size() > 0");
    }
    if( m_arpChildDSObjs.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_arpChildDSObjs.size() > 0");
    }
    // Groups
    if( m_mappChildGrpsByName.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_mappChildGrpsByName.size() > 0");
    }
    if( m_arpChildGrps.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_arpChildGrps.size() > 0");
    }
    // Enumerations
    //if( m_mappChildDEsByName.size() > 0 )
    //{
    //    throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_mappChildDEsByName.size() > 0");
    //}
    //if( m_arpChildDEs.size() > 0 )
    //{
    //    throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_arpChildDEs.size() > 0");
    //}
    // Tables
    if( m_mappChildDTsByName.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_mappChildDTsByName.size() > 0");
    }
    if( m_arpChildDTs.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_arpChildDTs.size() > 0");
    }
    // Parameters
    //if( m_mappChildDPsByName.size() > 0 )
    //{
    //    throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_mappChildDPsByName.size() > 0");
    //}
    //if( m_arpChildDPs.size() > 0 )
    //{
    //    throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_arpChildDPs.size() > 0");
    //}

} // clear

/*==============================================================================
public: // overridables
ba==============================================================================*/

//------------------------------------------------------------------------------
bool CDataSetObjGroup::isParentOf(
    CDataSetObject* i_pDSObj,
    bool            i_bRecursive,
    ERowVersion     i_version ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"Take row state and row version into account")
    #endif

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".isParentOf";
    QString strArg = QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getGrpObjKey() );

    strMth += "(" + strArg + ")";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    if( i_pDSObj == nullptr )
    {
        strAddErrInfo = "i_pDSObj == nullptr";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw ZS::System::CException(__FILE__, __LINE__, errResultInfo);
    }

    bool bIs = false;

    CDataSetObjGroup* pGrp = i_pDSObj->getGroup(false, i_version);

    if( pGrp == this )
    {
        bIs = true;
    }

    if( !bIs && i_bRecursive )
    {
        // Please note that child objects may have been moved deeper into the
        // object tree. The object may exist in a higher level with a removed
        // row state but in a lower level with an added row state.

        CDataSetObjGroup* pGrp;
        int               idxGrp;

        for( idxGrp = 0; idxGrp < m_arpChildGrps.size(); idxGrp++ )
        {
            pGrp = m_arpChildGrps[idxGrp];

            bIs = pGrp->isParentOf(i_pDSObj, i_bRecursive, i_version);

            if( bIs )
            {
                break;
            }
        } // for( idxGrp = 0; idxGrp < m_arpChildGrps.size(); idxGrp++ )
    } // if( !bIs && i_bRecursive )

    return bIs;

} // isParentOf

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDataSetObjGroup::getUniqueChildObjName(
    EDataSetObjType i_dsObjType,
    const QString&  i_strPrefix,
    ERowVersion     /*i_version*/ )
//------------------------------------------------------------------------------
{
    QString strPrefix;
    QString strName;

    if( i_dsObjType != EDataSetObjTypeUndefined )
    {
        strPrefix = dataSetObjType2Str(i_dsObjType);
    }
    else if( i_strPrefix.isEmpty() )
    {
        strPrefix = "Data Set Object";
    }

    if( !i_strPrefix.isEmpty() )
    {
        if( strPrefix.isEmpty() )
        {
            strPrefix = i_strPrefix;
        }
        else
        {
            strPrefix += " " + i_strPrefix;
        }
    }

    int     iObjId   = getChildObjsCount();
    QString strObjId = QString::number(iObjId);

    if( strPrefix.isEmpty() )
    {
        strName = strObjId;
    }
    else
    {
        strName = strPrefix + " " + QString::number(iObjId);
    }

    while( findChildObjByName(i_dsObjType,strName) != nullptr )
    {
        ++iObjId;
        strObjId = QString::number(iObjId);

        if( strPrefix.isEmpty() )
        {
            strName = strObjId;
        }
        else
        {
            strName = strPrefix + " " + QString::number(iObjId);
        }
    }

    return strName;

} // getUniqueChildObjName

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObject* CDataSetObjGroup::addChildObj(
    EDataSetObjType i_dsObjType,
    const QString&  i_strObjName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObjType: " + dataSetObjType2Str(i_dsObjType);
        strAddTrcInfo += ", ObjName: " + i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addChildObj",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".addChildObj";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    QString strGrpObjKey = dataSetObjType2Str(i_dsObjType) + CDataSet::GetObjPathSeparator() + i_strObjName;

    if( findChildObjByGrpKey(strGrpObjKey) != nullptr )
    {
        strAddErrInfo = "A " + dataSetObjType2Str(i_dsObjType) + " with the name " + i_strObjName + " already belongs to group " + getPath();
        throw CDuplicateNameException(__FILE__, __LINE__, strAddErrInfo);
    }

    CDataSetObject* pDSObj = nullptr;

    switch( i_dsObjType )
    {
        case EDataSetObjTypeDataSet:
        {
            strAddErrInfo = "Cannot add data set objects.";
            errResultInfo.setSeverity(EResultSeverityCritical);
            errResultInfo.setResult(EResultArgOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }
        case EDataSetObjTypeGroup:
        {
            pDSObj = new CDataSetObjGroup(i_strObjName);
            break;
        }
        case EDataSetObjTypeEnumeration:
        {
            //pDSObj = new CDataEnumeration(i_strObjName);
            break;
        }
        case EDataSetObjTypeTable:
        {
            pDSObj = new CDataTable(i_strObjName);
            break;
        }
        case EDataSetObjTypeParameter:
        {
            //pDSObj = new CDataParameter(i_strObjName);
            break;
        }
        default:
        {
            strAddErrInfo = "Unknown data set object type " + dataSetObjType2Str(i_dsObjType);
            errResultInfo.setSeverity(EResultSeverityCritical);
            errResultInfo.setResult(EResultArgOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }
    } // switch( i_dsObjType )

    if( pDSObj == nullptr )
    {
        strAddErrInfo = "Unsopperted data set object type " + dataSetObjType2Str(i_dsObjType);
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultOptionNotYetSupported);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    return addChildObj(pDSObj);

} // addChildObj

//------------------------------------------------------------------------------
CDataSetObject* CDataSetObjGroup::addChildObj( CDataSetObject* i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getGrpObjKey() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addChildObj",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".addChildObj";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    if( i_pDSObj == nullptr )
    {
        strAddErrInfo = "i_pDSObj == nullptr";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw ZS::System::CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pDSObj->getDataSet() != nullptr && i_pDSObj->getDataSet() != m_pDS )
    {
        strAddErrInfo = "Object already belongs to another DataSet \"" + i_pDSObj->getDataSet()->getPath() + "\".";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    // Newly created object which does not currently belong to a group ...
    if( i_pDSObj->isDetached() )
    {
        EDataSetObjType dsObjType    = i_pDSObj->getObjType();
        QString         strGrpObjKey = i_pDSObj->getGrpObjKey(); // "<T>:Name"
        QString         strObjName   = i_pDSObj->getName();

        if( findChildObjByGrpKey(strGrpObjKey) != nullptr )
        {
            strAddErrInfo = "A " + dataSetObjType2Str(dsObjType) + " with the name " + strObjName + " already belongs to group " + getPath();
            throw CDuplicateNameException(__FILE__, __LINE__, strAddErrInfo);
        }

        if( i_pDSObj->getDataSet() == nullptr )
        {
            i_pDSObj->setDataSet(m_pDS);
        }

        switch( dsObjType )
        {
            case EDataSetObjTypeGroup:
            {
                CDataSetObjGroup* pGrp = dynamic_cast<CDataSetObjGroup*>(i_pDSObj);
                m_mappChildGrpsByName.insert(strObjName, pGrp);
                m_arpChildGrps = m_mappChildGrpsByName.values().toVector();
                break;
            }
            case EDataSetObjTypeEnumeration:
            {
                //CDataEnumeration* pDE = dynamic_cast<CDataEnumeration*>(i_pDSObj);
                //m_mappChildDEsByName.insert(strObjName, pDE);
                //m_arpChildDEs = m_mappChildDEsByName.values().toVector();
                break;
            }
            case EDataSetObjTypeTable:
            {
                CDataTable* pDT = dynamic_cast<CDataTable*>(i_pDSObj);
                m_mappChildDTsByName.insert(strObjName, pDT);
                m_arpChildDTs = m_mappChildDTsByName.values().toVector();
                break;
            }
            case EDataSetObjTypeParameter:
            {
                //CDataParameter* pDP = dynamic_cast<CDataParameter*>(i_pDSObj);
                //m_mappChildDPsByName.insert(strObjName, pDP);
                //m_arpChildDPs = m_mappChildDPsByName.values().toVector();
                break;
            }
            default:
            {
                break;
            }
        } // switch( dsObjType )

        m_hshpChildDSObjsByGrpKey.insert(strGrpObjKey, i_pDSObj);

        // Sorted by Type and Key (Grps, DEs, DTs, DPs).
        m_arpChildDSObjs.resize( m_hshpChildDSObjsByGrpKey.size() );

        CDataSetObject** ppDSObjTmp = m_arpChildDSObjs.data();

        if( m_arpChildGrps.size() > 0 )
        {
            memcpy( ppDSObjTmp, m_arpChildGrps.data(), m_arpChildGrps.size()*sizeof(CDataSetObject**) );
            ppDSObjTmp += m_arpChildGrps.size();
        }
        //if( m_arpChildDEs.size() > 0 )
        //{
        //    memcpy( ppDSObjTmp, m_arpChildDEs.data(), m_arpChildDEs.size()*sizeof(CDataSetObject**) );
        //    ppDSObjTmp += m_arpChildDEs.size();
        //}
        if( m_arpChildDTs.size() > 0 )
        {
            memcpy( ppDSObjTmp, m_arpChildDTs.data(), m_arpChildDTs.size()*sizeof(CDataSetObject**) );
            ppDSObjTmp += m_arpChildDTs.size();
        }
        //if( m_arpChildDPs.size() > 0 )
        //{
        //    memcpy( ppDSObjTmp, m_arpChildDPs.data(), m_arpChildDPs.size()*sizeof(CDataSetObject**) );
        //    ppDSObjTmp += m_arpChildDPs.size();
        //}

        // Inherit default schema version to child object.
        // If the schema of this group is currently being initialized,
        // also the childs schema will be initialized.
        if( isInitializingSchema() )
        {
            i_pDSObj->beginInitSchema();
        }

        i_pDSObj->setGroup(this);
        i_pDSObj->setRowState(ERowState::Added);

        m_pDS->onDSObjAdded(i_pDSObj, this);

        emit_childObjAdded(this, i_pDSObj);

    } // if( i_pDSObj->isDetached() )

    //// If current row state is removed from THIS group ...
    //if( i_pDSObj->isRemoved(this, ERowVersion::Current) )
    //{
    //    // .. and the original row state was unchanged ...
    //    if( i_pDSObj->isUnchanged(ERowVersion::Original) )
    //    {
    //    }

    //    i_pDSObj->setRowState(ERowState::Unchanged);

    //    m_pDS->onDSObjAdded(i_pDSObj, this);

    //    emit_childObjAdded(this, i_pDSObj);
    //}
    //else if( i_pDSObj->getRowState(this) != ERowState::Detached )
    //{
    //    strAddErrInfo  = "Cannot add object " + i_pDSObj->getGrpObjKey() + " to group " + getPath();
    //    strAddErrInfo += " as the row state is " + CEnumRowState::toString(i_pDSObj->getRowState(this));
    //    errResultInfo.setSeverity(EResultSeverityCritical);
    //    errResultInfo.setResult(EResultInternalProgramError);
    //    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    //    throw CException(__FILE__, __LINE__, errResultInfo);
    //}

    return i_pDSObj;

} // addChildObj

//------------------------------------------------------------------------------
void CDataSetObjGroup::removeChildObj(
    EDataSetObjType i_dsObjType,
    const QString&  i_strObjName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObjType: " + dataSetObjType2Str(i_dsObjType);
        strAddTrcInfo += ", ObjName: " + i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeChildObj",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".removeChildObj";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    QString strGrpObjKey = dataSetObjType2Str(i_dsObjType) + CDataSet::GetObjPathSeparator() + i_strObjName;

    CDataSetObject* pDSObj = findChildObjByGrpKey(strGrpObjKey);

    if( pDSObj == nullptr )
    {
        strAddErrInfo = "A " + dataSetObjType2Str(i_dsObjType) + " with the name " + i_strObjName + " does not belong to group " + getPath();
        throw CUnknownNameException(__FILE__, __LINE__, strAddErrInfo);
    }

    return removeChildObj(pDSObj);

} // removeChildObj

//------------------------------------------------------------------------------
void CDataSetObjGroup::removeChildObj( CDataSetObject* i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getGrpObjKey() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeChildObj",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".removeChildObj";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    if( i_pDSObj == nullptr )
    {
        strAddErrInfo = "i_pDSObj == nullptr";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw ZS::System::CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pDSObj->getDataSet() == nullptr )
    {
        strAddErrInfo = "Object does not belong to any DataSet.";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pDSObj->getDataSet() != m_pDS )
    {
        strAddErrInfo = "Object belongs to another DataSet \"" + i_pDSObj->getDataSet()->getPath() + "\".";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    EDataSetObjType dsObjType    = i_pDSObj->getObjType();
    QString         strGrpObjKey = i_pDSObj->getGrpObjKey();    // "<T>:Name"
    QString         strDSObjKey  = i_pDSObj->getDSObjKey();     // "<T>:Path/Name"
    QString         strObjName   = i_pDSObj->getName();

    if( findChildObjByGrpKey(strGrpObjKey) == nullptr )
    {
        strAddErrInfo = "Object " + strGrpObjKey + " does not belong to group " + getPath();
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( m_pDS->findObjectByKey(strDSObjKey) == nullptr )
    {
        strAddErrInfo = "Object " + strDSObjKey + " does not belong to the data set of group " + getPath();
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pDSObj->isUnchanged() )
    {
        switch( dsObjType )
        {
            case EDataSetObjTypeGroup:
            {
                m_mappChildGrpsByName.remove(strObjName);                        // sorted by Key
                m_arpChildGrps = m_mappChildGrpsByName.values().toVector();   // sorted by Key
                break;
            }
            case EDataSetObjTypeEnumeration:
            {
                //m_mappChildDEsByName.remove(strObjName);                         // sorted by Key
                //m_arpChildDEs = m_mappChildDEsByName.values().toVector();     // sorted by Key
                break;
            }
            case EDataSetObjTypeTable:
            {
                m_mappChildDTsByName.remove(strObjName);                         // sorted by Key
                m_arpChildDTs = m_mappChildDTsByName.values().toVector();     // sorted by Key
                break;
            }
            case EDataSetObjTypeParameter:
            {
                //m_mappChildDPsByName.remove(strObjName);                         // sorted by Key
                //m_arpChildDPs = m_mappChildDPsByName.values().toVector();     // sorted by Key
                break;
            }
            default:
            {
                break;
            }
        } // switch( dsObjType )

        m_hshpChildDSObjsByGrpKey.remove(strGrpObjKey);

        // Sorted by Type and Key (Grps, DEs, DTs, DPs).
        m_arpChildDSObjs.resize( m_hshpChildDSObjsByGrpKey.size() );

        CDataSetObject** ppDSObjTmp = m_arpChildDSObjs.data();

        if( m_arpChildGrps.size() > 0 )
        {
            memcpy( ppDSObjTmp, m_arpChildGrps.data(), m_arpChildGrps.size()*sizeof(CDataSetObject**) );
            ppDSObjTmp += m_arpChildGrps.size();
        }
        //if( m_arpChildDEs.size() > 0 )
        //{
        //    memcpy( ppDSObjTmp, m_arpChildDEs.data(), m_arpChildDEs.size()*sizeof(CDataSetObject**) );
        //    ppDSObjTmp += m_arpChildDEs.size();
        //}
        if( m_arpChildDTs.size() > 0 )
        {
            memcpy( ppDSObjTmp, m_arpChildDTs.data(), m_arpChildDTs.size()*sizeof(CDataSetObject**) );
            ppDSObjTmp += m_arpChildDTs.size();
        }
        //if( m_arpChildDPs.size() > 0 )
        //{
        //    memcpy( ppDSObjTmp, m_arpChildDPs.data(), m_arpChildDPs.size()*sizeof(CDataSetObject**) );
        //    ppDSObjTmp += m_arpChildDPs.size();
        //}

        // If the object was not removed because it is currently being destroyed ..
        if( i_pDSObj->getObjState() == EObjState::Created )
        {
            // .. accept the changes here.

            // If the object is being destroyed (see dtor of CDataSetObject) the
            // object checks whether the object still belongs to a group. If the
            // object belongs to a group this "removeChildObj" method is called.
            // Please also note that if not the original but the current row state
            // of the object was changed the object will be destroyed later on
            // accepting the current changes.

            // Invalidate object group of object.
            i_pDSObj->setGroup(nullptr);

        }// if( i_pDSObj->getObjState() == EObjState::Created )

        emit_childObjRemoved(this, i_pDSObj);

        // Inform data set and let data set emit schema changed signals.
        m_pDS->onDSObjRemoved(i_pDSObj, this);

    } // if( i_pDSObj->getRowState(this) == ERowState::Unchanged )

    //if( i_pDSObj->getRowState(this) == ERowState::Added )
    //{
    //    // Just change the row state (undoing removing object from group).
    //    i_pDSObj->setRowState(ERowState::Detached, this);

    //    emit_childObjAboutToBeRemoved(this, i_pDSObj);

    //    // Inform data set and let data set emit schema changed signals.
    //    m_pDS->onDSObjAboutToBeRemoved(i_pDSObj, this);
    //}
    //else if( i_pDSObj->getRowState(this) != ERowState::Unchanged )
    //{
    //    strAddErrInfo  = "Cannot remove object " + i_pDSObj->getGrpObjKey() + " from group " + getPath();
    //    strAddErrInfo += " as the row state is " + CEnumRowState::toString(i_pDSObj->getRowState());
    //    errResultInfo.setSeverity(EResultSeverityCritical);
    //    errResultInfo.setResult(EResultInternalProgramError);
    //    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    //    throw CException(__FILE__, __LINE__, errResultInfo);
    //}

} // removeChildObj

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDataSetObjGroup::getChildObjsCount( EDataSetObjType i_dsObjType, ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    int iCount = 0;

    switch( i_dsObjType )
    {
        case EDataSetObjTypeGroup:
        {
            //iCount = getChildGroupsCount(i_version);
            break;
        }
        case EDataSetObjTypeEnumeration:
        {
            //iCount = getChildEnumerationsCount(i_version);
            break;
        }
        case EDataSetObjTypeTable:
        {
            //iCount = getChildTablesCount(i_version);
            break;
        }
        case EDataSetObjTypeParameter:
        {
            //iCount = getChildParametersCount(i_version);
            break;
        }
        case EDataSetObjTypeUndefined:
        {
            CDataSetObject* pDSObjTmp;
            int             idxObjTmp;
            bool            bObjAdded;
            bool            bObjRemoved;

            for( idxObjTmp = 0; idxObjTmp < m_arpChildDSObjs.size(); idxObjTmp++ )
            {
                pDSObjTmp = m_arpChildDSObjs[idxObjTmp];

                if( static_cast<int>(i_version) >= 0 && i_version < ERowVersion::Undefined )
                {
                    bObjAdded = false;
                    bObjRemoved = false;

                    // If the row state indicates that the object has been newly added to this group ..
                    if( pDSObjTmp->isAdded(this,i_version) )
                    {
                        bObjAdded = true;
                    }

                    // If the row state indicates that the object has been newly removed from this group ..
                    if( pDSObjTmp->isRemoved(this,i_version) )
                    {
                        bObjRemoved = true;
                    }

                    if( bObjAdded && bObjRemoved )
                    {
                        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Objects row state indicates Added and Removed at the same time");
                    }

                    if( bObjAdded )
                    {
                        if( i_version == ERowVersion::Original )
                        {
                            // Added entries will be ignored for the original version
                            // as the entry will only be really added to the group on
                            // accepting this change.
                        }
                        else if( i_version == ERowVersion::Current || i_version == ERowVersion::Proposed || i_version == ERowVersion::Default )
                        {
                            // Added entries will be taken into account for the current version.
                            iCount++;
                        }
                        else // if( i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
                        {
                            // Added entries will also be taken into account for undefined versions.
                            iCount++;
                        }
                    } // if( bObjAdded )

                    else if( bObjRemoved )
                    {
                        if( i_version == ERowVersion::Original )
                        {
                            // Removed entries will be taken into account for the original version
                            // as the entry will only be really removed from the group on
                            // accepting this change.
                            iCount++;
                        }
                        else if( i_version == ERowVersion::Current || i_version == ERowVersion::Proposed || i_version == ERowVersion::Default )
                        {
                            // Removed entries will not be taken into account for the current version.
                        }
                        else // if( i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
                        {
                            // Removed entries will also be taken into account for undefined versions.
                            iCount++;
                        }
                    } // if( bObjRemoved )

                    else // if( !bObjAdded && !bObjRemoved )
                    {
                        // For all other row states of the object the object belongs to the group no matter which
                        // version has been specified (instead of Detached for which an exception could be thrown here).
                        iCount++;
                    }
                } // if( static_cast<int>(i_version) >= 0 && i_version < ERowVersion::Undefined )

                else // if( i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
                {
                    // All entries will be taken into account ignoring the row state.
                    iCount++;
                }
            } // for( idxObjTmp = 0; idxObjTmp < m_arpChildDSObjs.size(); idxObjTmp++ )
            break;
        } // case EDataSetObjTypeUndefined:

        default:
        {
            break;
        }
    } // switch( i_dsObjType )

    return iCount;

} // getChildObjsCount

//------------------------------------------------------------------------------
CDataSetObject* CDataSetObjGroup::getChildObj(
    int             i_idxObj,
    EDataSetObjType i_dsObjType,
    ERowVersion     i_version ) const
//------------------------------------------------------------------------------
{
    CDataSetObject* pDSObj = nullptr;

    switch( i_dsObjType )
    {
        case EDataSetObjTypeGroup:
        {
            //pDSObj = getChildGroup(i_idxObj, i_version);
            break;
        }
        case EDataSetObjTypeEnumeration:
        {
            //pDSObj = getChildEnumeration(i_idxObj, i_version);
            break;
        }
        case EDataSetObjTypeTable:
        {
            //pDSObj = getChildTable(i_idxObj, i_version);
            break;
        }
        case EDataSetObjTypeParameter:
        {
            //pDSObj = getChildParameter(i_idxObj, i_version);
            break;
        }
        case EDataSetObjTypeUndefined:
        {
            CDataSetObject* pDSObjTmp;
            int             idxObjTmp;
            bool            bObjAdded;
            bool            bObjRemoved;
            int             idxObj = -1;

            for( idxObjTmp = 0; idxObjTmp < m_arpChildDSObjs.size(); idxObjTmp++ )
            {
                pDSObjTmp = m_arpChildDSObjs[idxObjTmp];

                if( static_cast<int>(i_version) >= 0 && i_version < ERowVersion::Undefined )
                {
                    bObjAdded = false;
                    bObjRemoved = false;

                    // If the row state indicates that the object has been newly added to a group ..
                    if( pDSObjTmp->isAdded(this,i_version) )
                    {
                        bObjAdded = true;
                    }

                    // If the row state indicates that the object has been newly removed from a group ..
                    if( pDSObjTmp->isRemoved(this,i_version) )
                    {
                        bObjRemoved = true;
                    }

                    if( bObjAdded && bObjRemoved )
                    {
                        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "Objects row state indicates Added and Removed at the same time");
                    }

                    if( bObjAdded )
                    {
                        if( i_version == ERowVersion::Original )
                        {
                            // Added entries will be ignored for the original version
                            // as the entry will only be really added to the group on
                            // accepting this change.
                        }
                        else if( i_version == ERowVersion::Current || i_version == ERowVersion::Proposed || i_version == ERowVersion::Default )
                        {
                            // Added entries will be taken into account for the current version.
                            idxObj++;
                        }
                        else // if( i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
                        {
                            // Added entries will also be taken into account for undefined versions.
                            idxObj++;
                        }
                    } // if( bObjAdded )

                    else if( bObjRemoved )
                    {
                        if( i_version == ERowVersion::Original )
                        {
                            // Removed entries will be taken into account for the original version
                            // as the entry will only be really removed from the group on
                            // accepting this change.
                            idxObj++;
                        }
                        else if( i_version == ERowVersion::Current || i_version == ERowVersion::Proposed || i_version == ERowVersion::Default )
                        {
                            // Removed entries will not be taken into account for the current version.
                        }
                        else // if( i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
                        {
                            // Removed entries will also be taken into account for undefined versions.
                            idxObj++;
                        }
                    } // if( bObjRemoved )

                    else // if( !bObjAdded && !bObjRemoved )
                    {
                        // For all other row states of the object the object belongs to the group no matter which
                        // version has been specified (instead of Detached for which an exception could be thrown here).
                        idxObj++;
                    }
                } // if( static_cast<int>(i_version) >= 0 && i_version < ERowVersion::Undefined )

                else // if( i_version == ERowVersion::Undefined || i_version == ERowVersion::Undefined )
                {
                    // All entries will be taken into account ignoring the row state.
                    idxObj++;
                }

                if( i_idxObj == idxObj )
                {
                    pDSObj = pDSObjTmp;
                    break;
                }
            } // for( idxObjTmp = 0; idxObjTmp < m_arpChildDSObjs.size(); idxObjTmp++ )
            break;
        } // case EDataSetObjTypeUndefined:

        default:
        {
            break;
        }
    } // switch( i_dsObjType )

    return pDSObj;

} // getChildObj

//------------------------------------------------------------------------------
CDataSetObject* CDataSetObjGroup::findChildObjByName(
    EDataSetObjType i_dsObjType,
    const QString&  i_strName,
    ERowVersion     /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"Take row state and row version into account")
    #endif
    QString strGrpKey = dataSetObjType2Str(i_dsObjType, EEnumEntryAliasStrSymbol) + ":" + i_strName;
    return findChildObjByGrpKey(strGrpKey);
}

//------------------------------------------------------------------------------
CDataSetObject* CDataSetObjGroup::findChildObjByGrpKey(
    const QString& i_strGrpKey,
    ERowVersion    /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"Take row state and row version into account")
    #endif
    return m_hshpChildDSObjsByGrpKey.value(i_strGrpKey, nullptr);
}

//------------------------------------------------------------------------------
int CDataSetObjGroup::getChildIndexOf( CDataSetObject* i_pDSObj, ERowVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"Take row state and row version into account")
    #endif
    return m_arpChildDSObjs.indexOf(i_pDSObj);

} // getChildIndexOf

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObjGroup* CDataSetObjGroup::addChildGroup( const QString& i_strGrpName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strGrpName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addChildGroup",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".addChildGroup";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    EDataSetObjType dsObjType = EDataSetObjTypeGroup;
    QString         strGrpKey = dataSetObjType2Str(dsObjType, EEnumEntryAliasStrSymbol) + i_strGrpName; // "<T>:Name"

    if( findChildObjByGrpKey(strGrpKey) != nullptr )
    {
        strAddErrInfo = "A group with name " + i_strGrpName + " already belongs to the group.";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultObjAlreadyInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CDataSetObjGroup* pGrp = new CDataSetObjGroup(i_strGrpName);

    addChildObj(pGrp);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString(pGrp == nullptr ? "nullptr" : pGrp->getPath());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pGrp;

} // addChildGroup

//------------------------------------------------------------------------------
CDataSetObjGroup* CDataSetObjGroup::addChildGroup( CDataSetObjGroup* i_pGrp )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString(i_pGrp == nullptr ? "nullptr" : i_pGrp->getName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addChildGroup",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".addChildGroup";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    EDataSetObjType dsObjType = EDataSetObjTypeGroup;
    QString         strGrpKey = dataSetObjType2Str(dsObjType, EEnumEntryAliasStrSymbol) + i_pGrp->getName(); // "<T>:Name"

    if( findChildObjByGrpKey(strGrpKey) != nullptr )
    {
        strAddErrInfo = "A group with name " + strGrpKey + " already belongs to the group.";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultObjAlreadyInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    addChildObj(i_pGrp);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString(i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath());
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return i_pGrp;

} // addChildGroup

//------------------------------------------------------------------------------
void CDataSetObjGroup::removeChildGroup( const QString& i_strGrpName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strGrpName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeChildGroup",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".removeChildGroup";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    EDataSetObjType dsObjType = EDataSetObjTypeGroup;
    QString         strGrpKey = dataSetObjType2Str(dsObjType, EEnumEntryAliasStrSymbol) + i_strGrpName; // "<T>:Name"

    if( findChildObjByGrpKey(strGrpKey) == nullptr )
    {
        strAddErrInfo = "A group with name " + i_strGrpName + " does not belong to " + getDSObjKey();
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CDataSetObject* pGrp = findChildObjByGrpKey(strGrpKey);

    removeChildObj(pGrp);

} // removeChildGroup

//------------------------------------------------------------------------------
void CDataSetObjGroup::removeChildGroup( CDataSetObjGroup* i_pGrp )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString(i_pGrp == nullptr ? "nullptr" : i_pGrp->getName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeChildGroup",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".removeChildGroup";
    QString strArg = QString(i_pGrp == nullptr ? "nullptr" : i_pGrp->getName());

    strMth += "(" + strArg + ")";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    if( i_pGrp == nullptr )
    {
        strAddErrInfo = "i_pGrp == nullptr";
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw ZS::System::CException(__FILE__, __LINE__, errResultInfo);
    }

    removeChildObj(i_pGrp);

} // removeChildGroup

/*==============================================================================
protected: // instance methods (to trace emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetObjGroup::emit_childObjAdded(
    CDataSetObjGroup* i_pGrp,
    CDataSetObject*   i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + QString( i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath() );
        strAddTrcInfo += ", DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_childObjAdded",
        /* strAddInfo   */ strAddTrcInfo );

    emit childObjAdded(i_pGrp, i_pDSObj);

} // emit_childObjAdded

//------------------------------------------------------------------------------
void CDataSetObjGroup::emit_childObjAdded(
    CDataSetObjGroup* i_pGrp,
    CDataSetObject*   i_pDSObj,
    ERowVersion       i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + QString( i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath() );
        strAddTrcInfo += ", DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_childObjAdded",
        /* strAddInfo   */ strAddTrcInfo );

    emit childObjAdded(i_pGrp, i_pDSObj, i_version);

} // emit_childObjAdded

//------------------------------------------------------------------------------
void CDataSetObjGroup::emit_childObjAboutToBeRemoved(
    CDataSetObjGroup* i_pGrp,
    CDataSetObject*   i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + QString( i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath() );
        strAddTrcInfo += ", DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_childObjAboutToBeRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    emit childObjAboutToBeRemoved(i_pGrp, i_pDSObj);

} // emit_childObjAboutToBeRemoved

//------------------------------------------------------------------------------
void CDataSetObjGroup::emit_childObjAboutToBeRemoved(
    CDataSetObjGroup* i_pGrp,
    CDataSetObject*   i_pDSObj,
    ERowVersion       i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + QString( i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath() );
        strAddTrcInfo += ", DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_childObjAboutToBeRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    emit childObjAboutToBeRemoved(i_pGrp, i_pDSObj, i_version);

} // emit_childObjAboutToBeRemoved

//------------------------------------------------------------------------------
void CDataSetObjGroup::emit_childObjRemoved(
    CDataSetObjGroup* i_pGrp,
    CDataSetObject*   i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + QString( i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath() );
        strAddTrcInfo += ", DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_childObjRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    emit childObjRemoved(i_pGrp, i_pDSObj);

} // emit_childObjRemoved

//------------------------------------------------------------------------------
void CDataSetObjGroup::emit_childObjRemoved(
    CDataSetObjGroup* i_pGrp,
    CDataSetObject*   i_pDSObj,
    ERowVersion       i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + QString( i_pGrp == nullptr ? "nullptr" : i_pGrp->getPath() );
        strAddTrcInfo += ", DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "emit_childObjRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    emit childObjRemoved(i_pGrp, i_pDSObj, i_version);

} // emit_childObjRemoved
