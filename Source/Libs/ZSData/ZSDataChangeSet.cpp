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

#error Module not used anymore

#include "ZSData/ZSDataChangeSet.h"
#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDataSet.h"
#include "ZSData/ZSDataSetObjGroup.h"
//#include "ZSData/ZSDataEnumeration.h"
//#include "ZSData/ZSDataParameter.h"
#include "ZSData/ZSDataTable.h"
//#include "ZSData/ZSDataEnumerationSchemaChgDscr.h"
//#include "ZSData/ZSDataTableSchemaChgDscr.h"
//#include "ZSData/ZSDataTableDataChgDscr.h"
//#include "ZSData/ZSDataParameterDataChgDscr.h"
//#include "ZSData/ZSDataParameterSchemaChgDscr.h"
//#include "ZSData/ZSDataSetSchemaChgDscr.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Data;
using namespace ZS::Trace;


/*******************************************************************************
class CChangeDataSet : public CDataSet
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CChangeDataSet::CChangeDataSet(
    CDataSet*               i_pDS,
    ERowVersion            i_version,
    const SDbClientRegSpec& i_dbCltRegSpec ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pDS(i_pDS),
    m_version(i_version),
    m_dbCltRegSpec(i_dbCltRegSpec),
    //m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeCount], // default ctor sufficient
    //m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeCount], // default ctor sufficient
    // Trace
    m_pTrcAdminObj(nullptr)
{
    if( m_dbCltRegSpec.m_strClientName.isEmpty() )
    {
        setObjectName(m_dbCltRegSpec.m_cnctId.toString().remove(":") );
    }
    else
    {
        setObjectName(m_dbCltRegSpec.m_strClientName);
    }

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        QString     strAddTrcInfo;
        QStringList strlstSpecs;
        QString     strSpec;
        SDSNodeSpec dsNodeSpec;
        int         idxSpec;

        strAddTrcInfo = "";

        if( m_dbCltRegSpec.m_arDSNodeSpecs.size() == 0 )
        {
            strSpec = "SELECT SCHEMA *";
            strlstSpecs += strSpec;
        }
        else
        {
            for( idxSpec = 0; idxSpec < m_dbCltRegSpec.m_arDSNodeSpecs.size(); idxSpec++ )
            {
                dsNodeSpec = m_dbCltRegSpec.m_arDSNodeSpecs[idxSpec];
                strSpec = "SELECT SCHEMA " + dsNodeSpec.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
                strlstSpecs += strSpec;
            }
        }
        if( strlstSpecs.size() > 0 )
        {
            strAddTrcInfo += strlstSpecs.join("; ");
        }
        m_pTrcAdminObj->traceMethod("ctor",strAddTrcInfo,1);
    }

    // Change data set for all data set objects ...
    if( !QObject::connect(
        /* pObjSender   */ m_pDS,
        /* szSignal     */ SIGNAL(orginalSchemaChanged(ZS::Data::CDSObjSchemaChgDscr*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDSOrginalSchemaChanged(ZS::Data::CDSObjSchemaChgDscr*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pDS,
        /* szSignal     */ SIGNAL(dsObjOrginalSchemaChanged(ZS::Data::CDSObjSchemaChgDscr*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDSObjOrginalSchemaChanged(ZS::Data::CDSObjSchemaChgDscr*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pDS,
        /* szSignal     */ SIGNAL(dsObjOrginalDataChanged(ZS::Data::CDSObjDataChgDscr*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDSObjOrginalDataChanged(ZS::Data::CDSObjDataChgDscr*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CChangeDataSet::CChangeDataSet( const QString& i_strObjName, ERowVersion i_version ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pDS(nullptr),
    m_version(i_version),
    m_dbCltRegSpec(),
    //m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeCount], // default ctor sufficient
    //m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeCount], // default ctor sufficient
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        QString     strAddTrcInfo;
        QStringList strlstSpecs;
        QString     strSpec;
        SDSNodeSpec dsNodeSpec;
        int         idxSpec;

        strAddTrcInfo = "";

        if( m_dbCltRegSpec.m_arDSNodeSpecs.size() == 0 )
        {
            strSpec = "SELECT SCHEMA *";
            strlstSpecs += strSpec;
        }
        else
        {
            for( idxSpec = 0; idxSpec < m_dbCltRegSpec.m_arDSNodeSpecs.size(); idxSpec++ )
            {
                dsNodeSpec = m_dbCltRegSpec.m_arDSNodeSpecs[idxSpec];
                strSpec = "SELECT SCHEMA " + dsNodeSpec.getAddTrcInfoStr(iAddTrcInfoDetailLevel );
                strlstSpecs += strSpec;
            }
        }
        if( strlstSpecs.size() > 0 )
        {
            strAddTrcInfo += strlstSpecs.join("; ");
        }
        m_pTrcAdminObj->traceMethod("ctor",strAddTrcInfo,1);
    }

} // ctor

//------------------------------------------------------------------------------
CChangeDataSet::~CChangeDataSet()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //int iObjType;

    //QHash<QString,CDSObjSchemaChgDscr*>::iterator itSchemaChgDscr;
    //QHash<QString,CDSObjDataChgDscr*>::iterator   itDataChgDscr;

    //CDSObjSchemaChgDscr* pSchemaChgDscr;
    //CDSObjDataChgDscr*   pDataChgDscr;

    //for( iObjType = 0; iObjType < EDataSetObjTypeCount; iObjType++ )
    //{
    //    for( itSchemaChgDscr = m_arhshpDSObjSchemaChgDscr[iObjType].begin();
    //         itSchemaChgDscr != m_arhshpDSObjSchemaChgDscr[iObjType].end();
    //         itSchemaChgDscr++ )
    //    {
    //        pSchemaChgDscr = itSchemaChgDscr.value();

    //        try
    //        {
    //            delete pSchemaChgDscr;
    //        }
    //        catch(...)
    //        {
    //        }
    //        pSchemaChgDscr = nullptr;
    //    }

    //    for( itDataChgDscr = m_arhshpDSObjDataChgDscr[iObjType].begin();
    //         itDataChgDscr != m_arhshpDSObjDataChgDscr[iObjType].end();
    //         itDataChgDscr++ )
    //    {
    //        pDataChgDscr = itDataChgDscr.value();

    //        try
    //        {
    //            delete pDataChgDscr;
    //        }
    //        catch(...)
    //        {
    //        }
    //        pDataChgDscr = nullptr;
    //    }
    //} // for( iObjType = 0; iObjType < EDataSetObjTypeCount; iObjType++ )

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDS = nullptr;
    m_version = static_cast<ERowVersion>(0);
    //m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeCount];
    //m_arhshpDSObjDataChgDscr[EDataSetObjTypeCount];
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CChangeDataSet* CChangeDataSet::clone( ZS::Data::ECloneCopyDepth i_copyDepth ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = cloneCopyDepth2Str(i_copyDepth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ strAddTrcInfo );

    CChangeDataSet* pChgDS = nullptr;

    return pChgDS;

} // clone

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CChangeDataSet::addObject( CDataSetObject* i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addObject",
        /* strAddInfo   */ strAddTrcInfo );

    //CDSObjSchemaChgDscr* pSchemaChgDscr = i_pDSObj->getSchemaChanges();

    //addSchemaChanges(i_pDSObj->getObjType(), pSchemaChgDscr);

} // addObject

//------------------------------------------------------------------------------
void CChangeDataSet::removeObject( CDataSetObject* i_pDSObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getDSObjKey() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeObject",
        /* strAddInfo   */ strAddTrcInfo );

} // removeObject

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//bool CChangeDataSet::hasSchemaChanges() const
////------------------------------------------------------------------------------
//{
//    bool bHasChanges = false;
//
//    for( int iObjType = 0; iObjType < EDataSetObjTypeCount; iObjType++ )
//    {
//        if( m_arhshpDSObjSchemaChgDscr[iObjType].size() > 0 )
//        {
//            bHasChanges = true;
//            break;
//        }
//    }
//    return bHasChanges;
//
//} // hasSchemaChanges
//
////------------------------------------------------------------------------------
//QHash<QString,CDSObjSchemaChgDscr*> CChangeDataSet::getSchemaChanges( EDataSetObjType i_dsObjType ) const
////------------------------------------------------------------------------------
//{
//    QHash<QString,CDSObjSchemaChgDscr*> hshpSchemaChgDscrs;
//
//    if( i_dsObjType >= 0 && i_dsObjType < EDataSetObjTypeCount )
//    {
//        hshpSchemaChgDscrs = m_arhshpDSObjSchemaChgDscr[i_dsObjType];
//    }
//    else
//    {
//        for( int iObjType = 0; iObjType < EDataSetObjTypeCount; iObjType++ )
//        {
//            QHash<QString,CDSObjSchemaChgDscr*>::const_iterator itSchemaChgDscr;
//
//            for( itSchemaChgDscr = m_arhshpDSObjSchemaChgDscr[iObjType].begin();
//                 itSchemaChgDscr != m_arhshpDSObjSchemaChgDscr[iObjType].end();
//                 itSchemaChgDscr++ )
//            {
//                hshpSchemaChgDscrs.insert(itSchemaChgDscr.key(), itSchemaChgDscr.value());
//            }
//        }
//    }
//    return hshpSchemaChgDscrs;
//
//} // getSchemaChanges
//
////------------------------------------------------------------------------------
//QHash<QString,CDSObjSchemaChgDscr*> CChangeDataSet::takeSchemaChanges( EDataSetObjType i_dsObjType )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "DSObjType: " + dataSetObjType2Str(i_dsObjType);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "takeSchemaChanges",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    QHash<QString,CDSObjSchemaChgDscr*> hshpSchemaChgDscrs;
//
//    if( i_dsObjType >= 0 && i_dsObjType < EDataSetObjTypeCount )
//    {
//        hshpSchemaChgDscrs = m_arhshpDSObjSchemaChgDscr[i_dsObjType];
//        m_arhshpDSObjSchemaChgDscr[i_dsObjType].clear();
//    }
//    else
//    {
//        for( int iObjType = 0; iObjType < EDataSetObjTypeCount; iObjType++ )
//        {
//            QHash<QString,CDSObjSchemaChgDscr*>::iterator itSchemaChgDscr;
//
//            for( itSchemaChgDscr = m_arhshpDSObjSchemaChgDscr[iObjType].begin();
//                 itSchemaChgDscr != m_arhshpDSObjSchemaChgDscr[iObjType].end();
//                 itSchemaChgDscr++ )
//            {
//                hshpSchemaChgDscrs.insert(itSchemaChgDscr.key(), itSchemaChgDscr.value());
//            }
//            m_arhshpDSObjSchemaChgDscr[iObjType].clear();
//        }
//    }
//    return hshpSchemaChgDscrs;
//
//} // takeSchemaChanges
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addSchemaChanges( EDataSetObjType i_dsObjType, CDSObjSchemaChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "DSObjType: " + dataSetObjType2Str(i_dsObjType);
//        strAddTrcInfo += ", ChgEvArgs {" + i_pChgEvArgs->getAddTrcInfo(iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "takeSchemaChanges",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( i_dsObjType >= 0 && i_dsObjType < EDataSetObjTypeCount )
//    {
//        m_arhshpDSObjSchemaChgDscr[i_dsObjType].insert(i_pChgEvArgs->getUId(), i_pChgEvArgs);
//    }
//
//} // addSchemaChanges
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addSchemaChanges( CDESchemaChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    addSchemaChanges(EDataSetObjTypeEnumeration, i_pChgEvArgs);
//}
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addSchemaChanges( CDTSchemaChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    addSchemaChanges(EDataSetObjTypeTable, i_pChgEvArgs);
//}
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addSchemaChanges( CDPSchemaChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    addSchemaChanges(EDataSetObjTypeParameter, i_pChgEvArgs);
//}
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addSchemaChanges( CGrpSchemaChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    addSchemaChanges(EDataSetObjTypeGroup, i_pChgEvArgs);
//}
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addSchemaChanges( CDSSchemaChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    addSchemaChanges(EDataSetObjTypeDataSet, i_pChgEvArgs);
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//bool CChangeDataSet::hasDataChanges() const
////------------------------------------------------------------------------------
//{
//    bool bHasChanges = false;
//
//    for( int iObjType = 0; iObjType < EDataSetObjTypeCount; iObjType++ )
//    {
//        if( m_arhshpDSObjDataChgDscr[iObjType].size() > 0 )
//        {
//            bHasChanges = true;
//            break;
//        }
//    }
//    return bHasChanges;
//
//} // hasDataChanges
//
////------------------------------------------------------------------------------
//QHash<QString,CDSObjDataChgDscr*> CChangeDataSet::getDataChanges( EDataSetObjType i_dsObjType ) const
////------------------------------------------------------------------------------
//{
//    QHash<QString,CDSObjDataChgDscr*> hshpDataChgEvs;
//
//    if( i_dsObjType >= 0 && i_dsObjType < EDataSetObjTypeCount )
//    {
//        hshpDataChgEvs = m_arhshpDSObjDataChgDscr[i_dsObjType];
//    }
//    else
//    {
//        for( int iObjType = 0; iObjType < EDataSetObjTypeCount; iObjType++ )
//        {
//            QHash<QString,CDSObjDataChgDscr*>::const_iterator itSchemaChgDscr;
//
//            for( itSchemaChgDscr = m_arhshpDSObjDataChgDscr[iObjType].begin();
//                 itSchemaChgDscr != m_arhshpDSObjDataChgDscr[iObjType].end();
//                 itSchemaChgDscr++ )
//            {
//                hshpDataChgEvs.insert(itSchemaChgDscr.key(), itSchemaChgDscr.value());
//            }
//        }
//    }
//    return hshpDataChgEvs;
//
//} // getDataChanges
//
////------------------------------------------------------------------------------
//QHash<QString,CDSObjDataChgDscr*> CChangeDataSet::takeDataChanges( EDataSetObjType i_dsObjType )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "DSObjType: " + dataSetObjType2Str(i_dsObjType);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "takeDataChanges",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    QHash<QString,CDSObjDataChgDscr*> hshpDataChgEvs;
//
//    if( i_dsObjType >= 0 && i_dsObjType < EDataSetObjTypeCount )
//    {
//        hshpDataChgEvs = m_arhshpDSObjDataChgDscr[i_dsObjType];
//        m_arhshpDSObjDataChgDscr[i_dsObjType].clear();
//    }
//    else
//    {
//        for( int iObjType = 0; iObjType < EDataSetObjTypeCount; iObjType++ )
//        {
//            QHash<QString,CDSObjDataChgDscr*>::iterator itSchemaChgDscr;
//
//            for( itSchemaChgDscr = m_arhshpDSObjDataChgDscr[iObjType].begin();
//                 itSchemaChgDscr != m_arhshpDSObjDataChgDscr[iObjType].end();
//                 itSchemaChgDscr++ )
//            {
//                hshpDataChgEvs.insert(itSchemaChgDscr.key(), itSchemaChgDscr.value());
//            }
//            m_arhshpDSObjDataChgDscr[iObjType].clear();
//        }
//    }
//    return hshpDataChgEvs;
//
//} // takeDataChanges
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addDataChanges( EDataSetObjType i_dsObjType, CDSObjDataChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "DSObjType: " + dataSetObjType2Str(i_dsObjType);
//        strAddTrcInfo += ", ChgEvArgs {" + i_pChgEvArgs->getAddTrcInfo(iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "addDataChanges",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( i_dsObjType >= 0 && i_dsObjType < EDataSetObjTypeCount )
//    {
//        m_arhshpDSObjDataChgDscr[i_dsObjType].insert(i_pChgEvArgs->getUId(), i_pChgEvArgs);
//    }
//
//} // addDataChanges
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addDataChanges( CDTDataChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    addDataChanges(EDataSetObjTypeTable, i_pChgEvArgs);
//}
//
////------------------------------------------------------------------------------
//void CChangeDataSet::addDataChanges( CDPDataChgDscr* i_pChgEvArgs )
////------------------------------------------------------------------------------
//{
//    addDataChanges(EDataSetObjTypeParameter, i_pChgEvArgs);
//}
//
///*==============================================================================
//public: // instance methods
//==============================================================================*/
//
////------------------------------------------------------------------------------
//ZS::System::SErrResultInfo CChangeDataSet::serialize( QByteArray& io_byteArr ) const
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "serialize",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    QString strMethod = "CChangeDataSet::" + objectName() + ".serialize";
//
//    SErrResultInfo errResultInfo(strMethod);
//
//    return errResultInfo;
//
//} // serialize
//
////------------------------------------------------------------------------------
//ZS::System::SErrResultInfo CChangeDataSet::deserialize( const QByteArray& i_byteArr )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "deserialize",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    QString strMethod = "CChangeDataSet::" + objectName() + ".deserialize";
//
//    SErrResultInfo errResultInfo(strMethod);
//
//    return errResultInfo;
//
//} // deserialize

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CChangeDataSet::resolveObjectReferences( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DS: " + QString( i_pDS == nullptr ? "nullptr" : i_pDS->getDSObjKey() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resolveObjectReferences",
        /* strAddInfo   */ strAddTrcInfo );

} // resolveObjectReferences

//------------------------------------------------------------------------------
void CChangeDataSet::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "invalidateObjectReferences",
        /* strAddInfo   */ strAddTrcInfo );

} // invalidateObjectReferences

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CChangeDataSet::getAddTrcInfo( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    return strAddTrcInfo;

} // getAddTrcInfo

/*==============================================================================
protected slots:
==============================================================================*/

////------------------------------------------------------------------------------
//void CChangeDataSet::onDSOrginalSchemaChanged( ZS::Data::CDSObjSchemaChgDscr* i_pSchemaChgEvArgs )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "EvArgs { " + QString( i_pSchemaChgEvArgs == nullptr ? "nullptr" : i_pSchemaChgEvArgs->getAddTrcInfo(iAddTrcInfoDetailLevel) ) + " }";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDSOrginalSchemaChanged",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    QString strUId = i_pSchemaChgEvArgs->getUId();
//
//    CDSSchemaChgDscr* pChgEvArgsSrc  = dynamic_cast<CDSSchemaChgDscr*>(i_pSchemaChgEvArgs);
//    CDSSchemaChgDscr* pChgEvArgsThis = dynamic_cast<CDSSchemaChgDscr*>(m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeDataSet].value(strUId,nullptr));
//
//    if( pChgEvArgsThis == nullptr )
//    {
//        pChgEvArgsThis = dynamic_cast<CDSSchemaChgDscr*>(pChgEvArgsSrc->clone());
//        m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeDataSet].insert(strUId, pChgEvArgsThis);
//    }
//    else // if( m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeDataSet].contains(strUId) )
//    {
//        // CDataSetObject
//        //---------------
//
//        if( !pChgEvArgsSrc->getName().isNull() )
//        {
//            pChgEvArgsThis->setName( pChgEvArgsSrc->getName() );
//        }
//        if( !pChgEvArgsSrc->getPreferredNameInViews().isNull() )
//        {
//            pChgEvArgsThis->setPreferredNameInViews( pChgEvArgsSrc->getPreferredNameInViews() );
//        }
//        if( !pChgEvArgsSrc->getComment().isNull() )
//        {
//            pChgEvArgsThis->setComment( pChgEvArgsSrc->getComment() );
//        }
//        if( pChgEvArgsSrc->isDataSaveReadEnabled() != EYesNo::Undefined )
//        {
//            pChgEvArgsThis->setDataSaveReadEnabled( pChgEvArgsSrc->isDataSaveReadEnabled() );
//        }
//        if( pChgEvArgsSrc->getDataFileFormat() != EFileFormatUndefined )
//        {
//            pChgEvArgsThis->setDataFileFormat( pChgEvArgsSrc->getDataFileFormat() );
//        }
//        if( !pChgEvArgsSrc->getDataFileName().isNull() )
//        {
//            pChgEvArgsThis->setDataFileName( pChgEvArgsSrc->getDataFileName() );
//        }
//
//        // CDataSetObjGroup
//        //-----------------
//
//        if( !pChgEvArgsSrc->getNameSeparator().isNull() )
//        {
//            pChgEvArgsThis->setNameSeparator( pChgEvArgsSrc->getNameSeparator() );
//        }
//
//        // CDataSet
//        //---------
//
//        if( pChgEvArgsSrc->isSchemaSaveReadEnabled() != EYesNo::Undefined )
//        {
//            pChgEvArgsThis->setSchemaSaveReadEnabled( pChgEvArgsSrc->isSchemaSaveReadEnabled() );
//        }
//
//        if( pChgEvArgsSrc->getSchemaFileFormat() != EFileFormatUndefined )
//        {
//            pChgEvArgsThis->setSchemaFileFormat( pChgEvArgsSrc->getSchemaFileFormat() );
//        }
//
//        if( !pChgEvArgsSrc->getSchemaFileName().isNull() )
//        {
//            pChgEvArgsThis->setSchemaFileName( pChgEvArgsSrc->getSchemaFileName() );
//        }
//
//        // Data Set Objects
//        //=================
//
//        if( pChgEvArgsSrc->getObjectsCount() > 0 )
//        {
//            QString           strObjUId;
//            QString           strObjKey;
//            QString           strObjName;
//            EDataSetObjType   dsObjType;
//            CDataSetObject*   pDSObjDb;
//            CDataSetObject*   pDSObjThis;
//            QString           strNodeSpecObjUId;
//            QString           strNodeSpecObjKey;
//            CDataSetObject*   pDSObjDbByNodeSpec;
//            CDataSetObject*   pDSObjThisByNodeSpec;
//            CDataSetObjGroup* pGrpDbByNodeSpec;
//            bool              bClone;
//            ERowState         rowState;
//            int               idxObj;
//            SDSNodeSpec       dsNodeSpec;
//            int               idxSpec;
//
//            for( idxObj = 0; idxObj < pChgEvArgsSrc->getObjectsCount(); idxObj++ )
//            {
//                strObjUId = pChgEvArgsSrc->getObjectUId(idxObj);
//                strObjKey = pChgEvArgsSrc->getObjectKey(idxObj);
//                rowState  = pChgEvArgsSrc->getObjectRowState(idxObj);
//                dsObjType = pChgEvArgsSrc->getObjectType(idxObj);
//
//                pDSObjDb   = m_pDS->findObjectByUId(strObjUId);
//                pDSObjThis = nullptr; //findObjectByUId(strObjUId);
//
//                // If no filter is set ...
//                if( m_dbCltRegSpec.m_arDSNodeSpecs.size() == 0 )
//                {
//                    bClone = false;
//
//                    // Please note that Detached, Added or Deleted objects may be modified.
//                    // So the modified flag has to be checked at last.
//                    if( rowState == ERowState::Unchanged )
//                    {
//                    }
//                    else if(rowState == ERowState::Detached )
//                    {
//                    }
//                    else if( rowState == ERowState::Added )
//                    {
//                        bClone = (pDSObjThis == nullptr);
//                    }
//                    else if( rowState == ERowState::Removed )
//                    {
//                        bClone = (pDSObjThis == nullptr);
//                    }
//                    else if( rowState == ERowState::Modified )
//                    {
//                        bClone = (pDSObjThis == nullptr);
//                    }
//                } // if( m_arDSNodeSpecs.size() == 0 )
//
//                // If a filter is set ...
//                else // if( m_arDSNodeSpecs.size() > 0 )
//                {
//                    for( idxSpec = 0; idxSpec < m_dbCltRegSpec.m_arDSNodeSpecs.size(); idxSpec++ )
//                    {
//                        dsNodeSpec = m_dbCltRegSpec.m_arDSNodeSpecs[idxSpec];
//
//                        /*
//                        struct SDSNodeSpec {
//                            EDataSetObjType     m_dsObjType;            // Type of the data set object (table, parameter).
//                            QString             m_strObjKey;            // Key of the data set object (table, parameter).
//                            QStringList         m_strlstChildObjNames;  // Usually used to specify columns of a table.
//                            QVector<SRangeInfo> m_arRangeInfos;         // Usually used to specify part (sub array) of an array parameter.
//                            QString             m_strWhere;             // Filter.
//                            bool                m_bRecursive;           // Specifies whether children of a group should be included.
//                        }; */
//
//                        strNodeSpecObjKey = dataSetObjType2Str(dsNodeSpec.m_dsObjType,EEnumEntryAliasStrSymbol) + ":" + dsNodeSpec.m_strObjKey;
//
//                        bClone = false;
//
//                        // If the object specified by node spec is the changed object ...
//                        if( strNodeSpecObjKey == strObjKey )
//                        {
//                            pDSObjDbByNodeSpec   = m_pDS->findObjectByKey(strNodeSpecObjKey);
//                            pDSObjThisByNodeSpec = nullptr; // findObjectByKey(strNodeSpecObjKey);
//
//                            // Please note that Detached, Added or Deleted objects may be modified.
//                            // So the modified flag has to be checked at last.
//                            if( rowState == ERowState::Unchanged )
//                            {
//                            }
//                            else if(rowState == ERowState::Detached )
//                            {
//                            }
//                            else if( rowState == ERowState::Added )
//                            {
//                                bClone = (pDSObjThis == nullptr);
//                            }
//                            else if( rowState == ERowState::Removed )
//                            {
//                                bClone = (pDSObjThis == nullptr);
//                            }
//                            else if( rowState == ERowState::Modified )
//                            {
//                                bClone = (pDSObjThis == nullptr);
//                            }
//                        } // if( strNodeSpecObjKey == strObjKey )
//
//                        // If the object specified by node spec is not the changed object ...
//                        else // if( strNodeSpecObjKey != strObjKey )
//                        {
//                            // .. the changed object might be the child of the object specified by the node spec.
//                            if( dsNodeSpec.m_dsObjType == EDataSetObjTypeGroup && dsNodeSpec.m_bRecursive )
//                            {
//                                pGrpDbByNodeSpec = m_pDS->findGroupByKey(strNodeSpecObjKey);
//
//                                if( pDSObjDb->isChildOf(pGrpDbByNodeSpec) )
//                                {
//                                    if( rowState == ERowState::Unchanged )
//                                    {
//                                    }
//                                    else if(rowState == ERowState::Detached )
//                                    {
//                                    }
//                                    else if( rowState == ERowState::Added )
//                                    {
//                                        bClone = (pDSObjThis == nullptr);
//                                    }
//                                    else if( rowState == ERowState::Modified )
//                                    {
//                                        bClone = (pDSObjThis == nullptr);
//                                    }
//                                    else if( rowState == ERowState::Removed )
//                                    {
//                                        bClone = (pDSObjThis == nullptr);
//                                    }
//                                } // if( pDSObjDb->isChildOf(pGrpDbByNodeSpec) )
//                            } // if( dsNodeSpec.m_dsObjType == EDataSetObjTypeGroup && dsNodeSpec.m_bRecursive )
//                        } // if( strNodeSpecObjKey != strObjKey )
//
//                        if( bClone )
//                        {
//                            break;  // Loop of DSNodeSpecs
//                        }
//                    } // for( idxSpec = 0; idxSpec < m_arDSNodeSpecs.size(); idxSpec++ )
//                } // if( m_arDSNodeSpecs.size() > 0 )
//
//                if( bClone )
//                {
//                    // On accepting the deletion of a data set object the object is removed from
//                    // the data set's list of data set objects and the original object has already
//                    // been deleted. So it can't be cloned but a temporary object descriptor
//                    // marking the object as deleted has to be added to this change data set.
//                    if( pDSObjDb == nullptr )
//                    {
//                        strObjName = strObjKey.section( m_pDS->getNameSeparator(), -1 );
//
//                        switch( dsObjType )
//                        {
//                            case EDataSetObjTypeGroup:
//                            {
//                                pDSObjThis = new CDataSetObjGroup(strObjName);
//                                break;
//                            }
//                            case EDataSetObjTypeEnumeration:
//                            {
//                                pDSObjThis = new CDataEnumeration(strObjName);
//                                break;
//                            }
//                            case EDataSetObjTypeParameter:
//                            {
//                                pDSObjThis = new CDataParameter(strObjName);
//                                break;
//                            }
//                            case EDataSetObjTypeTable:
//                            {
//                                pDSObjThis = new CDataTable(strObjName);
//                                break;
//                            }
//                            default:
//                            {
//                                break;
//                            }
//                        }
//                    }
//                    else // if( pDSObjDb != nullptr )
//                    {
//                        pDSObjThis = pDSObjDb->clone(ECloneCopyDepthSchemaAll);
//                    }
//
//                    // The clone has got a unique object id as default. This must be corrected:
//                    pDSObjThis->setUId(strObjUId);
//
//                    //pDSObjThis->beginInitSchema();
//
//                    pDSObjThis->setRowState(rowState);
//
//                    //pChgEvArgsThis->onObjectAdded(pDSObjThis);
//
//                    //pDSObjThis->endInitSchema();
//
//                } // if( bClone )
//            } // for( idxObj = 0; idxObj < pChgEvArgsSrc->getObjectsCount(EDataSetObjTypeGroup); idxObj++ )
//        } // if( pChgEvArgsSrc->getObjectsCount() > 0 )
//    } // if( m_arhshpDSObjSchemaChgDscr[EDataSetObjTypeDataSet].contains(strObjUId) )
//
//} // onDSOrginalSchemaChanged

////------------------------------------------------------------------------------
//void CChangeDataSet::onDSObjOrginalSchemaChanged( ZS::Data::CDSObjSchemaChgDscr* i_pSchemaChgEvArgs )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "EvArgs { " + QString( i_pSchemaChgEvArgs == nullptr ? "nullptr" : i_pSchemaChgEvArgs->getAddTrcInfo(iAddTrcInfoDetailLevel) ) + " }";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDSObjOrginalSchemaChanged",
//        /* strAddInfo   */ strAddTrcInfo );
//
//} // onDSObjOrginalSchemaChanged
//
////------------------------------------------------------------------------------
//void CChangeDataSet::onDSObjOrginalDataChanged( ZS::Data::CDSObjDataChgDscr* i_pDataChgEvArgs )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "EvArgs { " + QString( i_pDataChgEvArgs == nullptr ? "nullptr" : i_pDataChgEvArgs->getAddTrcInfo(iAddTrcInfoDetailLevel) ) + " }";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDSObjOrginalDataChanged",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    //CDataSetObject* pDSObj = pDSDb->findObjectByUId(pDSObjDataChgDscr->getUId());
//
//    //if( pDSDb->getObjType() == EDataSetObjTypeParameter )
//    //{
//    //    CDataParameter* pDPDb = dynamic_cast<CDataParameter*>(pDSObj);
//
//    //    CDataSetObjGroup* pGrpDb = pDPDb->getObjGroup();
//
//    //    CDataSetObjGroup* pGrpClt = m_pDSDataChanges->findObjectByUId(pGrpDb->getUId());
//
//    //    if( pGrpClt == nullptr )
//    //    {
//    //        pGrpClt = dynamic_cast<CDataSetObjGroup*>(pGrpClt->clone(ECloneCopyDepthSchemaPrepareData));
//    //        m_pDSDataChanges->appendChildObj(pGrpClt);
//    //    }
//
//    //    CDataParameter* pDPClt = m_pDSDataChanges->findObjectByUId(pDSObjDataChgDscr->getUId());
//
//    //    if( pDPClt == nullptr )
//    //    {
//    //        pDPClt = dynamic_cast<CDataParameter*>(pDPDb->clone(ECloneCopyDepthSchemaPrepareData));
//    //        pGrpClt->appendChildObj(pDPClt);
//    //    }
//
//    //    // Send accepted values to the client.
//    //    pDPClt->beginInitData();
//
//    //    CDPDataChgDscr* pDPDataChangeEvArgs = dynamic_cast<CDPDataChgDscr*>(pDSObjDataChgDscr);
//
//    //    pDPClt->mergeData(pDPDataChangeEvArgs);
//    //    pDPClt->invalidateObjectReferences();
//
//    //    pDPClt->endInitData();
//
//    //} // if( pDSDb->getObjType() == EDataSetObjTypeParameter )
//
//} // onDSObjOrginalDataChanged
