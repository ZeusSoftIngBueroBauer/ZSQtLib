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

#ifdef _WINDOWS
#pragma message("..")
#pragma message("TODOS:")
#pragma message("------")
#pragma message("")
#pragma message("- DataSet model changed slots to update only the currently change node but not resetting the complete model")
#pragma message("- Introduce DSObjProperty with validity flag and changed by key")
#pragma message("- Introduce viewers for each object type indicating each version of the DSObjProperties")
#pragma message("- Introduce DSChangeSet as member of class DataSet")
#pragma message("- Introduce viewer for DSChangeSet")
#pragma message("..")
#endif

#include "ZSData/ZSDataSet.h"
#include "ZSData/ZSDataTable.h"
#include "ZSData/ZSDataSet.h"
#include "ZSData/ZSDataSetObjGroup.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Data;


/*******************************************************************************
class CDataSet : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QString CDataSet::s_strObjPathSeparator = "/";

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSet::SetObjPathSeparator( const QString& i_strSeparator )
//------------------------------------------------------------------------------
{
    s_strObjPathSeparator = i_strSeparator;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDataSet::CDataSet( const QString& i_strName ) :
//------------------------------------------------------------------------------
    CDataSetObjGroup(EDataSetObjTypeDataSet, i_strName),
    m_bConstraintsEnforced(false),
    // Data Set Objects
    m_hshpDSObjsByKey(),
    m_arpDSObjs(),
    // Groups
    m_mappGrpsByPath(),
    m_arpGrps(),
    // Tables
    m_mappDTsByPath(),
    m_arpDTs(),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Name: " + i_strName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_objState = EObjState::Created;

    m_pDS = this;

    // A data set can't be added as a child to a group.
    m_pGrpOrig = nullptr;
    m_rowStateGrpOrig = ERowState::Unchanged;
    m_pGrpCurr = nullptr;
    m_rowStateGrpCurr = ERowState::Undefined;

} // ctor

//------------------------------------------------------------------------------
CDataSet::~CDataSet()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

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

    //m_strNameSeparator;
    m_bConstraintsEnforced = false;
    // Data Set Objects
    //m_hshpDSObjsByKey.clear();
    //m_arpDSObjs.clear();
    // Groups
    //m_mappGrpsByPath.clear();
    //m_arpGrps.clear();
    // Tables
    //m_mappDTsByPath.clear();
    //m_arpDTs.clear();
    // Method Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSet::clear()
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

    bool bConstraintsEnforced = m_bConstraintsEnforced;

    enforceConstraints(false);

    ERowVersion versionSchema = getSchemaDefaultVersion();

    // Delete the data set objects
    //----------------------------

    // Delete parameters and tables first (those objects may have references to enumerations and may be child of groups).

    //if( m_arpDPs() > 0 )
    //{
    //    for( idxObj = m_arpDPs.size()-1; idxObj >= 0; idxObj-- )
    //    {
    //        pDP = m_arpDPs(idxObj);

    //        // The DSObjs dtor will remove itself from the data set.
    //        delete pDP;
    //        pDP = nullptr;
    //    }
    //}

    if( m_arpDTs.size() > 0 )
    {
        for( idxObj = m_arpDTs.size()-1; idxObj >= 0; idxObj-- )
        {
            pDT = m_arpDTs[idxObj];

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
    } // if( m_arpDTs.size() > 0 )

    // Delete enumerations second (those objects may be child of groups).
    //if( m_arpDEs() > 0 )
    //{
    //    for( idxObj = m_arpDEs.size()-1; idxObj >= 0; idxObj-- )
    //    {
    //        pDE = m_arpDEs(idxObj);

    //        // The DSObjs dtor will remove itself from the data set.
    //        delete pDE;
    //        pDE = nullptr;
    //    }
    //}

    // Delete groups at last.
    if( m_arpGrps.size() > 0 )
    {
        for( idxObj = m_arpGrps.size()-1; idxObj >= 0; idxObj-- )
        {
            pGrp = m_arpGrps[idxObj];

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
    } // if( m_arpGrps.size() > 0 )

    enforceConstraints(bConstraintsEnforced);

    // After "clearing" the data set the list of data set objects must be empty.
    if( m_hshpDSObjsByKey.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_hshpDSObjsByKey.size() > 0");
    }
    if( m_arpDSObjs.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_arpDSObjs.size() > 0");
    }
    // Groups
    if( m_mappGrpsByPath.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_mappGrpsByPath.size() > 0");
    }
    if( m_arpGrps.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_arpGrps.size() > 0");
    }
    // Tables
    if( m_mappDTsByPath.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_mappDTsByPath.size() > 0");
    }
    if( m_arpDTs.size() > 0 )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_arpDTs.size() > 0");
    }

} // clear

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSet::enforceConstraints( bool i_bEnforce )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bEnforce);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "enforceConstraints",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bConstraintsEnforced != i_bEnforce )
    {
        m_bConstraintsEnforced = i_bEnforce;
    }

} // enforceConstraints

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObject* CDataSet::findObjectByKey( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"Take row state and row version into account")
    #endif
    return m_hshpDSObjsByKey.value(i_strKey, nullptr);
}

//------------------------------------------------------------------------------
int CDataSet::getObjectsCount( ERowVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"Take row state and row version into account")
    #endif
    return m_arpDSObjs.count();
}

//------------------------------------------------------------------------------
CDataSetObject* CDataSet::getObject( int i_idxObj, ERowVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"Take row state and row version into account")
    #endif
    return m_arpDSObjs[i_idxObj];
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObject* CDataSet::addObj(
    EDataSetObjType   i_dsObjType,
    const QString&    i_strObjName,
    CDataSetObjGroup* i_pGrpParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ObjType: " + dataSetObjType2Str(i_dsObjType);
        strAddTrcInfo += ", ObjName: " + i_strObjName;
        strAddTrcInfo += ", GrpParent: " + QString(i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getDSObjKey());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addObj",
        /* strAddInfo   */ strAddTrcInfo );

    CDataSetObject* pDSObj = nullptr;

    return pDSObj;

} // addObj

//------------------------------------------------------------------------------
CDataSetObject* CDataSet::addObj(
    EDataSetObjType i_dsObjType,
    const QString&  i_strObjName,
    const QString&  i_strGrpPathParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ObjType: " + dataSetObjType2Str(i_dsObjType);
        strAddTrcInfo += ", ObjName: " + i_strObjName;
        strAddTrcInfo += ", GrpParent: " + i_strGrpPathParent;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addObj",
        /* strAddInfo   */ strAddTrcInfo );

    CDataSetObject* pDSObj = nullptr;

    return pDSObj;

} // addObj

//------------------------------------------------------------------------------
CDataSetObject* CDataSet::addObj(
    CDataSetObject*   i_pDSObj,
    CDataSetObjGroup* i_pGrpParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getGrpObjKey());
        strAddTrcInfo += ", GrpParent: " + QString(i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getDSObjKey());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addObj",
        /* strAddInfo   */ strAddTrcInfo );

    CDataSetObject* pDSObj = nullptr;

    return pDSObj;

} // addObj

//------------------------------------------------------------------------------
CDataSetObject* CDataSet::addObj(
    CDataSetObject* i_pDSObj,
    const QString&  i_strGrpPathParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getGrpObjKey());
        strAddTrcInfo += ", GrpParent: " + i_strGrpPathParent;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addObj",
        /* strAddInfo   */ strAddTrcInfo );

    CDataSetObject* pDSObj = nullptr;

    return pDSObj;

} // addObj

//------------------------------------------------------------------------------
void CDataSet::removeObj( CDataSetObject* i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeObj",
        /* strAddInfo   */ strAddTrcInfo );

} // removeObj

//------------------------------------------------------------------------------
void CDataSet::removeObj(
    EDataSetObjType i_dsObjType,
    const QString&  i_strObjPath )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ObjType: " + dataSetObjType2Str(i_dsObjType);
        strAddTrcInfo += ", ObjPath: " + i_strObjPath;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeObj",
        /* strAddInfo   */ strAddTrcInfo );

} // removeObj

//------------------------------------------------------------------------------
void CDataSet::removeObj(
    EDataSetObjType i_dsObjType,
    const QString&  i_strObjName,
    const QString&  i_strGrpPathParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ObjType: " + dataSetObjType2Str(i_dsObjType);
        strAddTrcInfo += ", ObjName: " + i_strObjName;
        strAddTrcInfo += ", GrpPathParent: " + i_strGrpPathParent;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeObj",
        /* strAddInfo   */ strAddTrcInfo );

} // removeObj

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObjGroup* CDataSet::addGroup(
    const QString&    i_strGrpName,
    CDataSetObjGroup* i_pGrpParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GrpName: " + i_strGrpName;
        strAddTrcInfo += ", GrpParent: " + QString(i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getPath());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addGroup",
        /* strAddInfo   */ strAddTrcInfo );

    CDataSetObjGroup* pGrpParent = i_pGrpParent;

    if( pGrpParent == nullptr )
    {
        pGrpParent = this;
    }

    return pGrpParent->addChildGroup(i_strGrpName);

} // addGroup

//------------------------------------------------------------------------------
CDataSetObjGroup* CDataSet::addGroup(
    const QString& i_strGrpName,
    const QString& i_strGrpPathParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GrpName: " + i_strGrpName;
        strAddTrcInfo += ", GrpPathParent: " + i_strGrpPathParent;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addGroup",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth  = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".addGroup";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CDataSetObjGroup* pGrpParent = findGroupByPath(i_strGrpPathParent);

    if( pGrpParent == nullptr )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "The parent group \"" + i_strGrpPathParent + "\" is not existing.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    return pGrpParent->addChildGroup(i_strGrpName);

} // addGroup

//------------------------------------------------------------------------------
CDataSetObjGroup* CDataSet::addGroup(
    CDataSetObjGroup* i_pGrp,
    CDataSetObjGroup* i_pGrpParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + QString(i_pGrp == nullptr ? "nullptr" : i_pGrp->getName());
        strAddTrcInfo += ", GrpParent: " + QString(i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getPath());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addGroup",
        /* strAddInfo   */ strAddTrcInfo );

    CDataSetObjGroup* pGrpParent = i_pGrpParent;

    if( pGrpParent == nullptr )
    {
        pGrpParent = this;
    }

    return pGrpParent->addChildGroup(i_pGrp);

} // addGroup

//------------------------------------------------------------------------------
CDataSetObjGroup* CDataSet::addGroup(
    CDataSetObjGroup* i_pGrp,
    const QString&    i_strGrpPathParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + QString(i_pGrp == nullptr ? "nullptr" : i_pGrp->getName());
        strAddTrcInfo += ", GrpPathParent: " + i_strGrpPathParent;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addGroup",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth  = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".addGroup";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CDataSetObjGroup* pGrpParent = findGroupByPath(i_strGrpPathParent);

    if( pGrpParent == nullptr )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "The parent group \"" + i_strGrpPathParent + "\" is not existing.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    return pGrpParent->addChildGroup(i_pGrp);

} // addGroup

//------------------------------------------------------------------------------
void CDataSet::removeGroup( CDataSetObjGroup* i_pGrp )
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
        /* strMethod    */ "removeGroup",
        /* strAddInfo   */ strAddTrcInfo );

    //QString strMth  = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".removeGroup";

    //SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

} // removeGroup

//------------------------------------------------------------------------------
void CDataSet::removeGroup( const QString& i_strGrpPath )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strGrpPath;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeGroup",
        /* strAddInfo   */ strAddTrcInfo );

    //QString strMth  = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".removeGroup";

    //SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

} // removeGroup

//------------------------------------------------------------------------------
void CDataSet::removeGroup(
    const QString& i_strGrpName,
    const QString& i_strGrpPathParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Grp: " + i_strGrpName;
        strAddTrcInfo += ", GrpPathParent: " + i_strGrpPathParent;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeGroup",
        /* strAddInfo   */ strAddTrcInfo );

    //QString strMth  = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".removeGroup";

    //SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

} // removeGroup

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetObjGroup* CDataSet::findGroupByPath( const QString& i_strPath ) const
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    #pragma message(__TODO__"Take row state and row version into account")
    #endif
    return m_mappGrpsByPath.value(i_strPath, nullptr);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSet::resolveObjectReferences()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resolveObjectReferences",
        /* strAddInfo   */ strAddTrcInfo );

    //CDataParameter* pDP;
    //CDataTable*     pDT;
    //int             idxObj;

    //for( idxObj = 0; idxObj < m_arpLstDPs.size(); idxObj++ )
    //{
    //    pDP = m_arpLstDPs[idxObj];

    //    if( pDP != nullptr )
    //    {
    //        pDP->resolveObjectReferences();
    //    }
    //} // for( idxObj = 0; idxObj < m_arpLstDPs.size(); idxObj++ )

    //for( idxObj = 0; idxObj < m_arpLstDTs.size(); idxObj++ )
    //{
    //    pDT = m_arpLstDTs[idxObj];

    //    if( pDT != nullptr )
    //    {
    //        pDT->resolveObjectReferences();
    //    }
    //} // for( idxObj < m_arpLstDTs.size() )

} // resolveObjectReferences

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSet::onDSObjAdded(
    CDataSetObject*   i_pDSObj,
    CDataSetObjGroup* i_pGrpParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey());
        strAddTrcInfo += ", GrpParent: " + QString(i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getDSObjKey());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSObjAdded",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth  = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".onDSObjAdded";
    QString strArgs;

    strArgs  = QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
    strArgs += ", " + QString( i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getPath() );

    strMth += "(" + strArgs + ")";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    EDataSetObjType dsObjType = i_pDSObj->getObjType();
    QString         strName   = i_pDSObj->getName();
    QString         strPath   = i_pDSObj->getPath();    // Can only be retrieved after the group has been set.
    QString         strKey    = i_pDSObj->getDSObjKey();

    if( m_hshpDSObjsByKey.contains(strKey) )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "An object with Key \"" + strKey + "\" already belongs to the data set.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjAlreadyInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    switch( dsObjType )
    {
        case EDataSetObjTypeGroup:
        {
            CDataSetObjGroup* pGrp = dynamic_cast<CDataSetObjGroup*>(i_pDSObj);
            m_mappGrpsByPath.insert(strPath, pGrp);                  // sorted by Path
            m_arpGrps = m_mappGrpsByPath.values().toVector();        // sorted by Path
            break;
        }
        //case EDataSetObjTypeEnumeration:
        //{
        //    CDataEnumeration* pDE = dynamic_cast<CDataEnumeration*>(i_pDSObj);
        //    m_mappDEsByPath.insert(strPath, pDE);                    // sorted by Path
        //    m_arpDEs = m_mappDEsByPath.values().toVector();          // sorted by Path
        //    break;
        //}
        case EDataSetObjTypeTable:
        {
            CDataTable* pDT = dynamic_cast<CDataTable*>(i_pDSObj);
            m_mappDTsByPath.insert(strPath, pDT);                    // sorted by Path
            m_arpDTs = m_mappDTsByPath.values().toVector();          // sorted by Path
            break;
        }
        //case EDataSetObjTypeParameter:
        //{
        //    CDataParameter* pDP = dynamic_cast<CDataParameter*>(i_pDSObj);
        //    m_mappDPsByPath.insert(strPath, pDP);                    // sorted by Path
        //    m_arpDPs = m_mappDPsByPath.values().toVector();          // sorted by Path
        //    break;
        //}
        default:
        {
            break;
        }
    } // switch( dsObjType )

    m_hshpDSObjsByKey.insert(strKey, i_pDSObj);

    // Sorted by Type and Key (Grps, DEs, DTs, DPs)
    m_arpDSObjs.resize( m_hshpDSObjsByKey.size() );

    CDataSetObject** ppDSObjTmp = m_arpDSObjs.data();

    if( m_arpGrps.size() > 0 )
    {
        memcpy( ppDSObjTmp, m_arpGrps.data(), m_arpGrps.size()*sizeof(CDataSetObject**) );
        ppDSObjTmp += m_arpGrps.size();
    }
    //if( m_arpDEs.size() > 0 )
    //{
    //    memcpy( ppDSObjTmp, m_arpDEs.data(), m_arpDEs.size()*sizeof(CDataSetObject**) );
    //    ppDSObjTmp += m_arpDEs.size();
    //}
    if( m_arpDTs.size() > 0 )
    {
        memcpy( ppDSObjTmp, m_arpDTs.data(), m_arpDTs.size()*sizeof(CDataSetObject**) );
        ppDSObjTmp += m_arpDTs.size();
    }
    //if( m_arpDPs.size() > 0 )
    //{
    //    memcpy( ppDSObjTmp, m_arpDPs.data(), m_arpDPs.size()*sizeof(CDataSetObject**) );
    //    ppDSObjTmp += m_arpDPs.size();
    //}

    // This method may be called after the object has been added to the group or
    // if this is a change data set (a data set that contains only change descriptors)
    // on executing the "onDSOrginalSchemaChanged" slot. In both case the row state
    // of the object has already been set by the caller - which is either the group's
    // "addChildObj" or the change data set's slot method.
    // In case "addChildObj" has been called by the group the row state is Added.
    // In case "onDSOrginalSchemaChanged" has been called the row state may be either
    // Added, Deleted or Modified.

    emit_dsObjAdded(i_pDSObj);

} // onDSObjAdded

//------------------------------------------------------------------------------
void CDataSet::onDSObjAboutToBeRemoved(
    CDataSetObject*   i_pDSObj,
    CDataSetObjGroup* i_pGrpParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey());
        strAddTrcInfo += ", GrpParent: " + QString(i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getDSObjKey());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSObjAboutToBeRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    // This method may be called before the object is being removed to the group or
    // if this is a change data set (a data set that contains only change descriptors)
    // on executing the "onDSOrginalSchemaChanged" slot. In both cases the row state
    // of the object has already been set by the caller - which is either the group's
    // "addChildObj" or the change data set's slot method.
    // In case "addChildObj" has been called by the group the row state is Added.

    emit_dsObjAboutToBeRemoved(i_pDSObj);

} // onDSObjAboutToBeRemoved

//------------------------------------------------------------------------------
void CDataSet::onDSObjRemoved(
    CDataSetObject*   i_pDSObj,
    CDataSetObjGroup* i_pGrpParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey());
        strAddTrcInfo += ", GrpParent: " + QString(i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getDSObjKey());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSObjRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    QString strMth  = nameSpace() + "::" + className() + "::" + getDSObjKey() + ".onDSObjRemoved";
    QString strArgs;

    strArgs  = QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
    strArgs += ", " + QString( i_pGrpParent == nullptr ? "nullptr" : i_pGrpParent->getPath() );

    strMth += "(" + strArgs + ")";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    EDataSetObjType dsObjType = i_pDSObj->getObjType();
    QString         strName   = i_pDSObj->getName();
    QString         strPath   = i_pDSObj->getPath();    // Can only be retrieved after the group has been set.
    QString         strKey    = i_pDSObj->getDSObjKey();

    if( !m_hshpDSObjsByKey.contains(strKey) )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "An object with Key \"" + strKey + "\" does not belong to the data set.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjAlreadyInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    switch( dsObjType )
    {
        case EDataSetObjTypeGroup:
        {
            //CDataSetObjGroup* pGrp = dynamic_cast<CDataSetObjGroup*>(i_pDSObj);
            m_mappGrpsByPath.remove(strPath);                        // sorted by Path
            m_arpGrps = m_mappGrpsByPath.values().toVector();        // sorted by Path
            break;
        }
        //case EDataSetObjTypeEnumeration:
        //{
        //    CDataEnumeration* pDE = dynamic_cast<CDataEnumeration*>(i_pDSObj);
        //    m_mappDEsByPath.remove(strPath);                         // sorted by Path
        //    m_arpDEs = m_mappDEsByPath.values().toVector();          // sorted by Path
        //    break;
        //}
        case EDataSetObjTypeTable:
        {
            //CDataTable* pDT = dynamic_cast<CDataTable*>(i_pDSObj);
            m_mappDTsByPath.remove(strPath);                         // sorted by Path
            m_arpDTs = m_mappDTsByPath.values().toVector();          // sorted by Path
            break;
        }
        //case EDataSetObjTypeParameter:
        //{
        //    CDataParameter* pDP = dynamic_cast<CDataParameter*>(i_pDSObj);
        //    m_mappDPsByPath.remove(strPath);                         // sorted by Path
        //    m_arpDPs = m_mappDPsByPath.values().toVector();          // sorted by Path
        //    break;
        //}
        default:
        {
            break;
        }
    } // switch( dsObjType )

    m_hshpDSObjsByKey.remove(strKey);

    // Sorted by Type and Key (Grps, DEs, DTs, DPs)
    m_arpDSObjs.resize( m_hshpDSObjsByKey.size() );

    CDataSetObject** ppDSObjTmp = m_arpDSObjs.data();

    if( m_arpGrps.size() > 0 )
    {
        memcpy( ppDSObjTmp, m_arpGrps.data(), m_arpGrps.size()*sizeof(CDataSetObject**) );
        ppDSObjTmp += m_arpGrps.size();
    }
    //if( m_arpDEs.size() > 0 )
    //{
    //    memcpy( ppDSObjTmp, m_arpDEs.data(), m_arpDEs.size()*sizeof(CDataSetObject**) );
    //    ppDSObjTmp += m_arpDEs.size();
    //}
    if( m_arpDTs.size() > 0 )
    {
        memcpy( ppDSObjTmp, m_arpDTs.data(), m_arpDTs.size()*sizeof(CDataSetObject**) );
        ppDSObjTmp += m_arpDTs.size();
    }
    //if( m_arpDPs.size() > 0 )
    //{
    //    memcpy( ppDSObjTmp, m_arpDPs.data(), m_arpDPs.size()*sizeof(CDataSetObject**) );
    //    ppDSObjTmp += m_arpDPs.size();
    //}

    // This method may be called after the object has been removed from the group or
    // if this is a change data set (a data set that contains only change descriptors)
    // on executing the "onDSOrginalSchemaChanged" slot. In both cases the row state
    // of the object has already been set by the caller - which is either the group's
    // "removeChildObj" or the change data set's slot method.
    // In case "removeChildObj" has been called by the group the row state is Removed.

    emit_dsObjRemoved(i_pDSObj);

} // onDSObjRemoved

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSet::onDSObjNameChanged(
    CDataSetObject* i_pDSObj,
    const QString&  i_strNameNew,
    const QString&  i_strNamePrev,
    ERowVersion     i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey());
        strAddTrcInfo += ", NameNew: " + i_strNameNew;
        strAddTrcInfo += ", NamePrev: " + i_strNamePrev;
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSObjNameChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDSObjNameChanged

//------------------------------------------------------------------------------
void CDataSet::onDSObjGroupChanged(
    CDataSetObject*   i_pDSObj,
    CDataSetObjGroup* i_pGrpNew,
    CDataSetObjGroup* i_pGrpPrev,
    ERowVersion       i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey());
        strAddTrcInfo += ", GrpNew: " + QString(i_pGrpNew == nullptr ? "nullptr" : i_pGrpNew->getDSObjKey());
        strAddTrcInfo += ", GrpPrev: " + QString(i_pGrpPrev == nullptr ? "nullptr" : i_pGrpPrev->getDSObjKey());
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSObjGroupChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDSObjGroupChanged

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSet::onDSObjHasErrorsChanged(
    CDataSetObject* i_pDSObj,
    bool            i_bHasErrorsNew,
    bool            i_bHasErrorsPrev,
    ERowVersion     i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString(i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey());
        strAddTrcInfo += ", HasErrorsNew: " + bool2Str(i_bHasErrorsNew);
        strAddTrcInfo += ", HasErrorsPrev: " + bool2Str(i_bHasErrorsPrev);
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSObjHasErrorsChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDSObjHasErrorsChanged

/*==============================================================================
protected: // instance methods (to trace emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSet::emit_dsObjAdded( CDataSetObject* i_pDSObj )
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
        /* strMethod    */ "emit_dsObjAdded",
        /* strAddInfo   */ strAddTrcInfo );

    emit dsObjAdded(i_pDSObj);

} // emit_dsObjAdded

//------------------------------------------------------------------------------
void CDataSet::emit_dsObjAboutToBeRemoved( CDataSetObject* i_pDSObj )
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
        /* strMethod    */ "emit_dsObjAboutToBeRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    emit dsObjAboutToBeRemoved(i_pDSObj);

} // emit_dsObjAboutToBeRemoved

//------------------------------------------------------------------------------
void CDataSet::emit_dsObjRemoved( CDataSetObject* i_pDSObj )
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
        /* strMethod    */ "emit_dsObjRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    emit dsObjRemoved(i_pDSObj);

} // emit_dsObjRemoved
