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
#include <QtCore/qmimedata.h>

#include "ZSDataGUI/ZSDataSetModel.h"
//#include "ZSDataGUI/ZSDataSetModelNode.h"
#include "ZSDataGUI/ZSDataGUICommon.h"
#include "ZSData/ZSDataSet.h"
//#include "ZSData/ZSDataSetSchemaChgDscr.h"
//#include "ZSData/ZSDataSetObjectDataChgDscr.h"
//#include "ZSData/ZSDataSetObjectSchemaChgDscr.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysLogFile.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
using namespace ZS::Data;
using namespace ZS::Data::GUI;


/*******************************************************************************
class CDataSetModel : public QAbstractItemModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDataSetModel::CDataSetModel( const QString& i_strObjName, CDataSet* i_pDS ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(),
    m_pDS(nullptr),
    m_viewMode(EViewModeTreeView),
    //m_pDSObjRoot(nullptr),
    //m_arpHshDSNodesByKey(),
    //m_arpHshDSNodesByUId(),
    m_pTrcAdminObj(nullptr)
{
    if( i_strObjName.isEmpty() )
    {
        setObjectName("DataSetModel");
    }
    else
    {
        setObjectName(i_strObjName);
    }

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( i_pDS == nullptr )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "DS == nullptr" );
    }

    setDataSet(i_pDS);

} // ctor

//------------------------------------------------------------------------------
CDataSetModel::~CDataSetModel()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //try
    //{
    //    clearNode(m_pDSObjRoot);
    //}
    //catch(...)
    //{
    //}

    // The root entry has not be deleted by the clear method and must be explicitly destroyed.
    //try
    //{
    //    delete m_pDSObjRoot;
    //}
    //catch(...)
    //{
    //}
    //m_pDSObjRoot = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDS = nullptr;
    m_viewMode = static_cast<EViewMode>(0);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetModel::setDataSet( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString(i_pDS == nullptr ? "nullptr" : i_pDS->getDSObjKey());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDataSet",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDS != i_pDS )
    {
        beginResetModel();

        if( m_pDS != nullptr )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSDestroyed(QObject*)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(dsObjAdded(ZS::Data::CDataSetObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSObjAdded(ZS::Data::CDataSetObject*)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(dsObjAdded(ZS::Data::CDataSetObject*,ZS::System::ERowVersion)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSObjAdded(ZS::Data::CDataSetObject*,ZS::System::ERowVersion)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(dsObjAboutToBeRemoved(ZS::Data::CDataSetObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSObjAboutToBeRemoved(ZS::Data::CDataSetObject*)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(dsObjAboutToBeRemoved(ZS::Data::CDataSetObject*,ZS::System::ERowVersion)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSObjAboutToBeRemoved(ZS::Data::CDataSetObject*,ZS::System::ERowVersion)) );

        } // if( m_pDS != nullptr )

        m_pDS = i_pDS;

        if( m_pDS != nullptr )
        {
            if( !QObject::connect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSDestroyed(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(dsObjAdded(ZS::Data::CDataSetObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSObjAdded(ZS::Data::CDataSetObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(dsObjAdded(ZS::Data::CDataSetObject*,ZS::System::ERowVersion)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSObjAdded(ZS::Data::CDataSetObject*,ZS::System::ERowVersion)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(dsObjAboutToBeRemoved(ZS::Data::CDataSetObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSObjAboutToBeRemoved(ZS::Data::CDataSetObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDS,
                /* szSignal     */ SIGNAL(dsObjAboutToBeRemoved(ZS::Data::CDataSetObject*,ZS::System::ERowVersion)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDSObjAboutToBeRemoved(ZS::Data::CDataSetObject*,ZS::System::ERowVersion)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        } // if( m_pDS != nullptr )

        endResetModel();

    } // if( m_pDS != i_pDS )

} // setDataSet

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetModel::setViewMode( EViewMode i_viewMode )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = viewMode2Str(i_viewMode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setViewMode",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_viewMode != i_viewMode )
    {
        beginResetModel();

        m_viewMode = i_viewMode;

        endResetModel();

    } // if( m_viewMode != i_viewMode )

} // setViewMode

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//CDataSetModelNode* CDataSetModel::findNodeByUId( const QString& i_strUId )
////------------------------------------------------------------------------------
//{
//    return m_arpHshDSNodesByUId.value(i_strUId, nullptr);
//}
//
////------------------------------------------------------------------------------
//CDataSetModelNode* CDataSetModel::findNodeByKey( const QString& i_strKey )
////------------------------------------------------------------------------------
//{
//    return m_arpHshDSNodesByKey.value(i_strKey, nullptr);
//}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetModel::onDSDestroyed( QObject* /*i_pDS*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSDestroyed",
        /* strAddInfo   */ "" );

    beginResetModel();

    m_pDS = nullptr;

    endResetModel();

    //clearNode(m_pDSObjRoot);

} // onDSDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDataSetModel::onDSObjAdded( CDataSetObject* i_pDSObj )
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
        /* strMethod    */ "onDSObjAdded",
        /* strAddInfo   */ strAddTrcInfo );

    beginResetModel();

    if( m_viewMode == EViewModeTableView )
    {
    } // if( m_viewMode == EViewModeTableView )

    else if( m_viewMode == EViewModeTreeView )
    {
    } // if( m_viewMode == EViewModeTreeView )

    endResetModel();

} // onDSObjAdded

//------------------------------------------------------------------------------
void CDataSetModel::onDSObjAdded( CDataSetObject* i_pDSObj, ERowVersion i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSObjAdded",
        /* strAddInfo   */ strAddTrcInfo );

    beginResetModel();

    if( m_viewMode == EViewModeTableView )
    {
    } // if( m_viewMode == EViewModeTableView )

    else if( m_viewMode == EViewModeTreeView )
    {
    } // if( m_viewMode == EViewModeTreeView )

    endResetModel();

} // onDSObjAdded

//------------------------------------------------------------------------------
void CDataSetModel::onDSObjAboutToBeRemoved( CDataSetObject* i_pDSObj )
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
        /* strMethod    */ "onDSObjAboutToBeRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    beginResetModel();

    if( m_viewMode == EViewModeTableView )
    {
    } // if( m_viewMode == EViewModeTableView )

    else if( m_viewMode == EViewModeTreeView )
    {
    } // if( m_viewMode == EViewModeTreeView )

    endResetModel();

} // onDSObjAboutToBeRemoved

//------------------------------------------------------------------------------
void CDataSetModel::onDSObjAboutToBeRemoved( CDataSetObject* i_pDSObj, ERowVersion i_version )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DSObj: " + QString( i_pDSObj == nullptr ? "nullptr" : i_pDSObj->getPath() );
        strAddTrcInfo += ", Version: " + CEnumRowVersion::toString(i_version);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSObjAboutToBeRemoved",
        /* strAddInfo   */ strAddTrcInfo );

    beginResetModel();

    if( m_viewMode == EViewModeTableView )
    {
    } // if( m_viewMode == EViewModeTableView )

    else if( m_viewMode == EViewModeTreeView )
    {
    } // if( m_viewMode == EViewModeTreeView )

    endResetModel();

} // onDSObjAboutToBeRemoved

/*==============================================================================
protected slots:
==============================================================================*/

////------------------------------------------------------------------------------
//void CDataSetModel::onDSStateChanged( ZS::Data::CDataSetObject* i_pDSObj )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = getDSObjKey(i_pDSObj);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDSStateChanged",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    // Update data set node:
//    QModelIndex modelIdxTopLeft     = index( m_pDSObjRoot->getModelIdx().row(), 0, QModelIndex() );
//    QModelIndex modelIdxBottomRight = index( m_pDSObjRoot->getModelIdx().row(), EColumnCount-1, QModelIndex() );
//
//    emit dataChanged(modelIdxTopLeft,modelIdxBottomRight);
//
//} // onDSStateChanged
//
////------------------------------------------------------------------------------
//void CDataSetModel::onDSSchemaChanged( ZS::Data::CDSObjSchemaChgDscr* i_pDSSchemaChangeEvArgs )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "EvArgs { " + QString( i_pDSSchemaChangeEvArgs == nullptr ? "nullptr" : i_pDSSchemaChangeEvArgs->getAddTrcInfo(iAddTrcInfoDetailLevel) ) + " }";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDSSchemaChanged",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    CDSSchemaChgDscr* pDSSchemaChangeEvArgs = dynamic_cast<CDSSchemaChgDscr*>(i_pDSSchemaChangeEvArgs);
//
//    QString strUId = pDSSchemaChangeEvArgs->getUId();
//
//    if( m_pDS->getUId() != strUId )
//    {
//        throw CException( __FILE__, __LINE__, EResultInternalProgramError );
//    }
//
//    CDataSetModelNode* pDSObjNode;
//    CDataSetModelNode* pDSObjNodeParent;
//    CDataSetObject*    pDSObj;
//    CDataSetObjGroup*  pGrp;
//    int                idxObj;
//    QString            strObjName;
//    QString            strObjUId;
//    ERowState          rowState;
//    bool               bAddNode;
//    bool               bDeleteNode;
//
//    QModelIndex modelIdxParent;
//    QModelIndex modelIdxTopLeft;
//    QModelIndex modelIdxBottomRight;
//
//    if( pDSSchemaChangeEvArgs->getObjectsCount() > 0 )
//    {
//        for( idxObj = 0; idxObj < pDSSchemaChangeEvArgs->getObjectsCount(); idxObj++ )
//        {
//            strObjUId = pDSSchemaChangeEvArgs->getObjectUId(idxObj);
//            rowState  = pDSSchemaChangeEvArgs->getObjectRowState(idxObj);
//
//            pDSObj = m_pDS->findObjectByUId(strObjUId);
//            pDSObjNode = findNodeByUId(strObjUId);
//
//            pGrp = nullptr;
//            pDSObjNodeParent = nullptr;
//
//            if( pDSObj != nullptr )
//            {
//                pGrp = pDSObj->getObjGroup();
//
//                if( pGrp != nullptr )
//                {
//                    pDSObjNodeParent = findNodeByUId(pGrp->getUId());
//                }
//            }
//            else if( pDSObjNode != nullptr )
//            {
//                pDSObjNodeParent = pDSObjNode->getParentNode();
//            }
//
//            bAddNode = false;
//            bDeleteNode = false;
//
//            if( rowState == ERowState::Added )
//            {
//                // If not yet added (and e.g. currently marked for deletion) ...
//                if( pDSObjNode == nullptr )
//                {
//                    bAddNode = true;
//                }
//            }
//            else if( rowState == ERowState::Removed )
//            {
//                // If really deleted from data set (not just marked for deletion) ...
//                if( pDSObj == nullptr )
//                {
//                    bDeleteNode = true;
//                }
//            }
//
//            if( bAddNode )
//            {
//                pDSObjNode = addNode(pDSObjNodeParent, pDSObj, false);
//            }
//            else if( bDeleteNode )
//            {
//                deleteNode(pDSObjNodeParent, pDSObjNode);
//            }
//            else // if( bChangeNode )
//            {
//                if( pDSObjNodeParent != nullptr )
//                {
//                    modelIdxTopLeft     = index( pDSObjNode->getModelIdx().row(), 0, pDSObjNodeParent->getModelIdx() );
//                    modelIdxBottomRight = index( pDSObjNode->getModelIdx().row(), EColumnCount-1, pDSObjNodeParent->getModelIdx() );
//                }
//                else
//                {
//                    modelIdxTopLeft     = index( pDSObjNode->getModelIdx().row(), 0, QModelIndex() );
//                    modelIdxBottomRight = index( pDSObjNode->getModelIdx().row(), EColumnCount-1, QModelIndex() );
//                }
//
//                emit dataChanged(modelIdxTopLeft,modelIdxBottomRight);
//
//            } // if( bChangeNode )
//        } // for( idxObj = 0; idxObj < pDSSchemaChangeEvArgs->getObjectsCount(); idxObj++ )
//    } // if( pDSSchemaChangeEvArgs->getObjectsCount() > 0 )
//
//    // Update data set node:
//    modelIdxTopLeft     = index( m_pDSObjRoot->getModelIdx().row(), 0, QModelIndex() );
//    modelIdxBottomRight = index( m_pDSObjRoot->getModelIdx().row(), EColumnCount-1, QModelIndex() );
//
//    emit dataChanged(modelIdxTopLeft,modelIdxBottomRight);
//
//} // onDSSchemaChanged

/*==============================================================================
protected slots:
==============================================================================*/

////------------------------------------------------------------------------------
//void CDataSetModel::onDSObjRowStateChanged( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowState i_rowState )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "DSObj: " + getDSObjKey(i_pDSObj);
//        strAddTrcInfo += ", RowState: " + CEnumRowState::toString(i_rowState);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDSObjRowStateChanged",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    QString strObjUId = i_pDSObj->getUId();
//
//    CDataSetModelNode* pDSObjNode = findNodeByUId(strObjUId);
//
//    if( pDSObjNode != nullptr )
//    {
//        CDataSetModelNode* pDSObjNodeParent = pDSObjNode->getParentNode();
//
//        // If the row state is set to detached ..
//        if( i_rowState == ERowState::Detached )
//        {
//            // .. the object has already been removed from the data set and is about to be destroyed.
//            // Please note that the row state of an object may be deleted. But this may just be
//            // a temporary change for the current and/or proposed version of an object. Only if the
//            // delection is accepted the object will be really removed from the data set.
//            deleteNode(pDSObjNodeParent, pDSObjNode);
//        }
//        else
//        {
//            QModelIndex modelIdxTopLeft;
//            QModelIndex modelIdxBottomRight;
//
//            if( pDSObjNodeParent != nullptr )
//            {
//                modelIdxTopLeft     = index( pDSObjNode->getModelIdx().row(), 0, pDSObjNodeParent->getModelIdx() );
//                modelIdxBottomRight = index( pDSObjNode->getModelIdx().row(), EColumnCount-1, pDSObjNodeParent->getModelIdx() );
//            }
//            else
//            {
//                modelIdxTopLeft     = index( pDSObjNode->getModelIdx().row(), 0, QModelIndex() );
//                modelIdxBottomRight = index( pDSObjNode->getModelIdx().row(), EColumnCount-1, QModelIndex() );
//            }
//
//            emit dataChanged(modelIdxTopLeft,modelIdxBottomRight);
//        }
//    } // if( pDSObjNode != nullptr )
//
//} // onDSObjRowStateChanged
//
////------------------------------------------------------------------------------
//void CDataSetModel::onDSObjSchemaChanged( ZS::Data::CDSObjSchemaChgDscr* i_pSchema )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "EvArgs { " + QString( i_pSchema == nullptr ? "nullptr" : i_pSchema->getAddTrcInfo(iAddTrcInfoDetailLevel) ) + " }";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDSObjSchemaChanged",
//        /* strAddInfo   */ strAddTrcInfo );
//
//
//} // onDSObjSchemaChanged
//
////------------------------------------------------------------------------------
//void CDataSetModel::onDSObjDataChanged( ZS::Data::CDSObjDataChgDscr* pDSObjDataChgDscr )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "EvArgs { " + QString( pDSObjDataChgDscr == nullptr ? "nullptr" : pDSObjDataChgDscr->getAddTrcInfo(iAddTrcInfoDetailLevel) ) + " }";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDSObjDataChanged",
//        /* strAddInfo   */ strAddTrcInfo );
//
//
//} // onDSChildObjDataChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//CDataSetModelNode* CDataSetModel::addNode(
//    CDataSetModelNode* i_pDSObjParent,
//    CDataSetObject*    i_pDSObj,
//    bool               i_bRecursive )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Parent: " + QString( i_pDSObjParent == nullptr ? "nullptr" : i_pDSObjParent->getKey() );
//        strAddTrcInfo += ", DSObj: " + getDSObjKey(i_pDSObj);
//        strAddTrcInfo += ", Recursive: " + bool2Str(i_bRecursive);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "addNode",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( i_pDSObj == nullptr )
//    {
//        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "DSObj == nullptr" );
//    }
//
//    CDataSetModelNode* pDSObjParent = i_pDSObjParent;
//
//    if( pDSObjParent == nullptr )
//    {
//        QList<CDataSetObjGroup*> arpGrps;
//
//        CDataSetObjGroup* pGrp = i_pDSObj->getObjGroup();
//
//        while( pDSObjParent == nullptr && pGrp != nullptr )
//        {
//            pDSObjParent = findNodeByUId(pGrp->getUId());
//
//            if( pDSObjParent == nullptr )
//            {
//                arpGrps.insert(0,pGrp);
//
//                pGrp = pGrp->getObjGroup();
//            }
//        }
//
//        for( int idxGrp = 0; idxGrp < arpGrps.size(); idxGrp++ )
//        {
//            pGrp = arpGrps[idxGrp];
//
//            if( pGrp->getObjGroup() != nullptr )
//            {
//                pDSObjParent = findNodeByUId( pGrp->getObjGroup()->getUId() );
//            }
//            else
//            {
//                pDSObjParent = nullptr;
//            }
//            pDSObjParent = addNode( pDSObjParent, pGrp, false );
//        }
//    }
//
//    if( pDSObjParent == nullptr )
//    {
//        throw CException( __FILE__, __LINE__, EResultInternalProgramError, "pDSObjParent == nullptr" );
//    }
//
//    QString strObjId = i_pDSObj->getUId();
//    QString strKey   = i_pDSObj->getKey();
//
//    CDataSetModelNode* pDSObj = new CDataSetModelNode( this, i_pDSObj );
//
//    QModelIndex modelIdxParent = pDSObjParent->getModelIdx();
//    int         iRowIdx        = pDSObjParent->addChildNode(pDSObj);
//
//    pDSObj->setModelIdx( createIndex(iRowIdx,0,pDSObj) );
//
//    beginInsertRows(modelIdxParent,iRowIdx,iRowIdx);
//
//    m_arpHshDSNodesByUId.insert(strObjId, pDSObj);
//    m_arpHshDSNodesByKey.insert(strKey, pDSObj);
//
//    endInsertRows();
//
//    // Recursively add groups, parameter and data tables
//    //--------------------------------------------------
//
//    if( i_bRecursive && i_pDSObj->getObjType() == EDataSetObjTypeGroup )
//    {
//        CDataSetObjGroup* pDSObjGrpChild = dynamic_cast<CDataSetObjGroup*>(i_pDSObj);
//
//        if( pDSObjGrpChild != nullptr )
//        {
//            CDataSetObject* pDSObjChild;
//            int             idxChild;
//
//            for( idxChild = 0; idxChild < pDSObjGrpChild->getChildObjsCount(); idxChild++ )
//            {
//                pDSObjChild = pDSObjGrpChild->getChildObj(idxChild);
//
//                if( pDSObjChild != nullptr )
//                {
//                    addNode(pDSObj,pDSObjChild,true);
//                }
//            }
//        }
//    }
//
//    return pDSObj;
//
//} // addNode( CDataSetObject )
//
////------------------------------------------------------------------------------
//void CDataSetModel::deleteNodeByUId(
//    CDataSetModelNode* i_pDSObjParent,
//    const QString&     i_strUId )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Parent: " + QString( i_pDSObjParent == nullptr ? "nullptr" : i_pDSObjParent->getKey() );
//        strAddTrcInfo += ", DSObjUId: " + i_strUId;
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "deleteNode",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    CDataSetModelNode* pDSObj = findNodeByUId(i_strUId);
//
//    if( pDSObj == nullptr )
//    {
//        throw CException( __FILE__, __LINE__, EResultObjNotInList, "Data Set Object with UId " + i_strUId + " not existing." );
//    }
//
//    deleteNode(i_pDSObjParent,pDSObj);
//
//} // deleteNodeByUId
//
////------------------------------------------------------------------------------
//void CDataSetModel::deleteNodeByKey(
//    CDataSetModelNode* i_pDSObjParent,
//    const QString&     i_strKey )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Parent: " + QString( i_pDSObjParent == nullptr ? "nullptr" : i_pDSObjParent->getKey() );
//        strAddTrcInfo += ", DSObjKey: " + i_strKey;
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "deleteNode",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    CDataSetModelNode* pDSObj = findNodeByKey(i_strKey);
//
//    if( pDSObj == nullptr )
//    {
//        throw CException( __FILE__, __LINE__, EResultObjNotInList, "Data Set Object with Key " + i_strKey + " not existing." );
//    }
//
//    deleteNode(i_pDSObjParent,pDSObj);
//
//} // deleteNodeByKey
//
////------------------------------------------------------------------------------
//void CDataSetModel::deleteNode(
//    CDataSetModelNode* i_pDSObjParent,
//    CDataSetModelNode* i_pDSObj )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        if( i_pDSObjParent == nullptr )
//        {
//            strAddTrcInfo = "Parent: nullptr";
//        }
//        else
//        {
//            strAddTrcInfo = "Parent: " + i_pDSObjParent->objectName();
//        }
//        if( i_pDSObj == nullptr )
//        {
//            strAddTrcInfo += ", DSNode: nullptr";
//        }
//        else
//        {
//            strAddTrcInfo += ", DSNode: " + i_pDSObj->objectName();
//        }
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "deleteNode",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( i_pDSObj == nullptr )
//    {
//        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, "DSNode == nullptr" );
//    }
//
//    // Delete all child nodes ..
//    if( i_pDSObj->getChildNodeCount() > 0 )
//    {
//        CDataSetModelNode* pDSObjChild;
//        int                iRowIdxChild;
//
//        for( iRowIdxChild = i_pDSObj->getChildNodeCount()-1; iRowIdxChild >= 0; iRowIdxChild-- )
//        {
//            pDSObjChild = i_pDSObj->getChildNode(iRowIdxChild);
//
//            if( pDSObjChild != nullptr )
//            {
//                deleteNode(i_pDSObj,pDSObjChild);
//            }
//        }
//    }
//
//    QModelIndex modelIdxParent;
//    QModelIndex modelIdx = i_pDSObj->getModelIdx();
//    int         iRowIdx  = i_pDSObj->getModelIdx().row();
//
//    if( i_pDSObjParent != nullptr )
//    {
//        modelIdxParent = i_pDSObjParent->getModelIdx();
//    }
//
//    beginRemoveRows(modelIdxParent,iRowIdx,iRowIdx);
//
//    // By deleting the node the model indices of the siblings
//    // "behind" the node to be deleted must be corrected.
//    if( i_pDSObjParent != nullptr )
//    {
//        // Remove the node from the parents child list.
//        i_pDSObjParent->removeChildNode(iRowIdx);
//
//        CDataSetModelNode* pDSObjSibling;
//        int                iRowIdxSibling;
//
//        // Need to correct the model index of the "moved" nodes. Unfortunately the
//        // "createIndex" method is protected and therefore the "removeChild" method
//        // of the node is not able to set new model indices.
//        for( iRowIdxSibling = iRowIdx; iRowIdxSibling < i_pDSObjParent->getChildNodeCount(); iRowIdxSibling++ )
//        {
//            pDSObjSibling = i_pDSObjParent->getChildNode(iRowIdxSibling);
//
//            if( pDSObjSibling != nullptr )
//            {
//                pDSObjSibling->setModelIdx( createIndex(iRowIdxSibling,0,pDSObjSibling) );
//            }
//        }
//    }
//
//    if( m_pDS != nullptr )
//    {
//        QString strObjId = i_pDSObj->getUId();
//        QString strKey   = i_pDSObj->getKey();
//
//        m_arpHshDSNodesByUId.remove(strObjId);
//        m_arpHshDSNodesByKey.remove(strKey);
//    }
//
//    delete i_pDSObj;
//    i_pDSObj = nullptr;
//
//    endRemoveRows();
//
//} // deleteNode( CDataSetModelNode* )

/*==============================================================================
protected: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDataSetModel::clearNode( CDataSetModelNode* i_pDSObj )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        if( i_pDSObj == nullptr )
//        {
//            strAddTrcInfo = "DSNode: nullptr";
//        }
//        else
//        {
//            strAddTrcInfo = "DSNode: " + i_pDSObj->objectName();
//        }
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "clearNode",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( m_pDSObjRoot == nullptr && i_pDSObj == nullptr )
//    {
//        return;
//    }
//
//    CDataSetModelNode* pDSObj = i_pDSObj;
//
//    if( pDSObj == nullptr )
//    {
//        pDSObj = m_pDSObjRoot;
//    }
//
//    // The node to be cleared will not be deleted.
//    if( pDSObj->getChildNodeCount() > 0 )
//    {
//        CDataSetModelNode* pDSObjChild;
//        int                iRowIdxChild;
//
//        for( iRowIdxChild = pDSObj->getChildNodeCount()-1; iRowIdxChild >= 0; iRowIdxChild-- )
//        {
//            pDSObjChild = pDSObj->getChildNode(iRowIdxChild);
//
//            clearNode(pDSObjChild);
//
//            // After the childs of the child have been removed the child may be removed and deleted.
//            beginRemoveRows( pDSObj->getModelIdx(), iRowIdxChild, iRowIdxChild );
//
//            if( m_pDS != nullptr )
//            {
//                m_arpHshDSNodesByUId.remove(pDSObjChild->getUId());
//                m_arpHshDSNodesByKey.remove(pDSObjChild->getKey());
//            }
//
//            pDSObj->removeChildNode(iRowIdxChild);
//
//            delete pDSObjChild;
//            pDSObjChild = nullptr;
//
//            endRemoveRows();
//        }
//    }
//
//    // The node has been cleared (all child nodes have been removed and deleted).
//    // The node to be cleared will not be deleted.
//
//} // clearNode

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CDataSetModel::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( m_pDS != nullptr )
    {
        if( m_viewMode == EViewModeTableView )
        {
            if( !i_modelIdxParent.isValid() )
            {
                iRowCount = 1;
            }
            else
            {
                iRowCount = m_pDS->getObjectsCount();
            }
        } // if( m_viewMode == EViewModeTableView )

        else // if( m_viewMode == EViewModeTreeView )
        {
            if( !i_modelIdxParent.isValid() )
            {
                iRowCount = 1;
            }
            else
            {
                iRowCount = m_pDS->getChildObjsCount();
            }
        } // if( m_viewMode == EViewModeTreeView )
    } // if( m_pDS != nullptr )

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CDataSetModel::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    int iClmCount = 0;

    if( m_pDS != nullptr )
    {
        iClmCount = EColumnCount;
    }
    return iClmCount;

} // columnCount

//------------------------------------------------------------------------------
QVariant CDataSetModel::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    CDataSetObject* pDSObj = static_cast<CDataSetObject*>(i_modelIdx.internalPointer());

    if( pDSObj == nullptr )
    {
        return varData;
    }

    switch( i_modelIdx.column() )
    {
        case EColumnName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                QString strName;

                if( pDSObj->hasErrors() )
                {
                    strName = "!";
                }
                if( pDSObj->isDetached() )
                {
                    strName += CEnumRowState::toString(ERowState::Detached,EEnumEntryAliasStrSymbol);
                }
                //if( pDSObj->isAdded() )
                //{
                //    strName += CEnumRowState::toString(ERowState::Added,EEnumEntryAliasStrSymbol);
                //}
                //if( pDSObj->isRemoved() )
                //{
                //    strName += CEnumRowState::toString(ERowState::Removed,EEnumEntryAliasStrSymbol);
                //}
                //if( pDSObj->isModified() )
                //{
                //    if( pDSObj->hasSchemaChanges() )
                //    {
                //        strName += "{" + CEnumRowState::toString(ERowState::Modified,EEnumEntryAliasStrSymbol) + "}";
                //    }
                //    if( pDSObj->hasDataChanges() )
                //    {
                //        strName += "[" + CEnumRowState::toString(ERowState::Modified,EEnumEntryAliasStrSymbol) + "]";
                //    }
                //}
                if( !strName.isEmpty() )
                {
                    strName += " ";
                }
                varData = strName + pDSObj->getName();
            }
            else if( i_iRole == Qt::ToolTipRole )
            {
                QStringList strlstToolTip;

                if( pDSObj->hasErrors() )
                {
                    strlstToolTip += "Err";
                }
                if( pDSObj->isDetached() )
                {
                    strlstToolTip += "Detached";
                }
                //if( pDSObj->isAdded() )
                //{
                //    strlstToolTip += "Added";
                //}
                //if( pDSObj->isRemoved() )
                //{
                //    strlstToolTip += "Removed";
                //}
                //if( pDSObj->isModified() )
                //{
                //    strlstToolTip += "Modified";
                //}
                //if( pDSObj->hasSchemaChanges() )
                //{
                //    strlstToolTip += "HasSchemaChanges";
                //}
                //if( pDSObj->hasDataChanges() )
                //{
                //    strlstToolTip += "HasDataChanges";
                //}
                varData = strlstToolTip.join("|");
            }
            else if( i_iRole == Qt::TextColorRole )
            {
                if( pDSObj->hasErrors() )
                {
                    varData = QColor(Qt::red);
                }
                else if( pDSObj->isDetached() )
                {
                    varData = QColor(Qt::lightGray);
                }
                //else if( pDSObj->isAdded() )
                //{
                //    varData = QColor(Qt::darkGreen);
                //}
                //else if( pDSObj->isRemoved() )
                //{
                //    varData = QColor(Qt::darkGray);
                //}
                //else if( pDSObj->isModified() )
                //{
                //    varData = QColor(Qt::darkCyan); //darkMagenta=violett/rosa
                //}
                //else if( pDSObj->hasSchemaChanges() )
                //{
                //    varData = QColor(Qt::darkCyan); //darkMagenta=violett/rosa
                //}
                //else if( pDSObj->hasDataChanges() )
                //{
                //    varData = QColor(Qt::blue);
                //}
            }
            else if( i_iRole == Qt::DecorationRole )
            {
                varData = dataSetObjType2Pixmap(pDSObj->getObjType());
            }
            break;
        } // case EColumnName

        default:
        {
            break;
        }
    } // switch( i_modelIdx.column() )

    return varData;

} // data

//------------------------------------------------------------------------------
QModelIndex CDataSetModel::index( int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    if( i_iRow >= 0 )
    {
        if( !i_modelIdxParent.isValid() )
        {
            if( i_iRow == 0 )
            {
                modelIdx = createIndex(i_iRow, i_iClm, m_pDS);
            }
        } // if( !i_modelIdxParent.isValid() )

        else // if( i_modelIdxParent.isValid() )
        {
            CDataSetObjGroup* pGrpParent = static_cast<CDataSetObjGroup*>(i_modelIdxParent.internalPointer());

            if( m_viewMode == EViewModeTableView )
            {
                CDataSet* pDS = dynamic_cast<CDataSet*>(pGrpParent);

                if( pDS->getObjectsCount() > 0 && i_iRow < pDS->getObjectsCount() )
                {
                    modelIdx = createIndex(i_iRow, i_iClm, pDS->getObject(i_iRow));
                }
            }
            else // if( m_viewMode == EViewModeTreeView )
            {
                if( pGrpParent->getChildObjsCount() > 0 && i_iRow < pGrpParent->getChildObjsCount() )
                {
                    modelIdx = createIndex(i_iRow, i_iClm, pGrpParent->getChildObj(i_iRow));
                }
            }
        }
    } // if( i_iRow >= 0 )

    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CDataSetModel::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    if( i_modelIdx.isValid() )
    {
        CDataSetObject* pDSObj = static_cast<CDataSetObject*>(i_modelIdx.internalPointer());

        int iRow = 0;

        CDataSetObjGroup* pGrpParent = nullptr;

        if( pDSObj != nullptr )
        {
            pGrpParent = pDSObj->getGroup();
        }

        if( pGrpParent != nullptr )
        {
            CDataSetObjGroup* pGrpGrandParent = pGrpParent->getGroup();

            if( pGrpGrandParent != nullptr )
            {
                iRow = pGrpGrandParent->getChildIndexOf(pGrpParent);
            }

            modelIdx = createIndex(iRow, 0, pGrpParent);

        } // if( pDSObj != nullptr )
    } // if( i_modelIdx.isValid() )

    return modelIdx;

} // parent

/*==============================================================================
public: // declare protected methods of base class QAbstractItemModel as public
==============================================================================*/

//------------------------------------------------------------------------------
QModelIndex CDataSetModel::createIndex( int i_iRow, int i_iClm, void* i_pvData ) const
//------------------------------------------------------------------------------
{
    return QAbstractItemModel::createIndex(i_iRow, i_iClm, i_pvData);
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CDataSetModel::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        switch( i_iSection )
        {
            case EColumnName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Name";
                }
                break;
            }
            default:
            {
                break;
            }
        }
    } // if( i_orientation == Qt::Horizontal )

    return varData;

} // headerData

//------------------------------------------------------------------------------
Qt::ItemFlags CDataSetModel::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelRuntimeInfo) )
    {
        //CDataSetModelNode* pDSObj = static_cast<CDataSetModelNode*>(i_modelIdx.internalPointer());
        //strAddTrcInfo  = "ModelIdx { ";
        //strAddTrcInfo += "Row:" + QString::number(i_modelIdx.row());
        //strAddTrcInfo += ", Clm:" + QString::number(i_modelIdx.column());
        //strAddTrcInfo += ", DSNode: " + QString( pDSObj == nullptr ? "nullptr" : pDSObj->getKey() ) + " }";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 3,
        /* strMethod    */ "flags",
        /* strAddInfo   */ strAddTrcInfo );

     Qt::ItemFlags itemFlags = QAbstractItemModel::flags(i_modelIdx);

     if( i_modelIdx.isValid() )
     {
        //CDataSetModelNode* pDSObj = static_cast<CDataSetModelNode*>(i_modelIdx.internalPointer());

        //if( pDSObj != nullptr )
        //{
        //    switch( pDSObj->getObjType() )
        //    {
        //        case EDataSetObjTypeDataSet:
        //        {
        //            itemFlags = Qt::ItemIsDropEnabled | itemFlags;
        //            break;
        //        }
        //        case EDataSetObjTypeGroup:
        //        {
        //            itemFlags = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | itemFlags;
        //            break;
        //        }
        //        case EDataSetObjTypeEnumeration:
        //        {
        //            itemFlags = Qt::ItemIsDragEnabled | itemFlags;
        //            break;
        //        }
        //        case EDataSetObjTypeParameter:
        //        {
        //            itemFlags = Qt::ItemIsDragEnabled | itemFlags;
        //            break;
        //        }
        //        case EDataSetObjTypeTable:
        //        {
        //            itemFlags = Qt::ItemIsDragEnabled |itemFlags;
        //            break;
        //        }
        //        default:
        //        {
        //            break;
        //        }
        //    }
        //} if( pDSObj != nullptr )
     } // if( i_modelIdx.isValid() )

    if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) )
    {
        strAddTrcInfo = "";
        if( itemFlags & Qt::ItemIsDragEnabled )
        {
            strAddTrcInfo += "DragEnabled";
        }
        if( itemFlags & Qt::ItemIsDragEnabled )
        {
            if( !strAddTrcInfo.isEmpty() )
            {
                strAddTrcInfo += "|";
            }
            strAddTrcInfo += "DropEnabled";
        }
        strAddTrcInfo += " (0x" + QString::number(itemFlags,16) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return itemFlags;

} // flags

//------------------------------------------------------------------------------
Qt::DropActions CDataSetModel::supportedDropActions() const
//------------------------------------------------------------------------------
{
    return Qt::CopyAction | Qt::MoveAction;
}

//------------------------------------------------------------------------------
QStringList CDataSetModel::mimeTypes() const
//------------------------------------------------------------------------------
{
    QStringList strlstTypes;
    strlstTypes << "application/vnd.text.list";
    return strlstTypes;

} // mimeTypes

//------------------------------------------------------------------------------
QMimeData* CDataSetModel::mimeData( const QModelIndexList& /*i_arModelIdxs*/ ) const
//------------------------------------------------------------------------------
{
    QMimeData*  pMimeData = new QMimeData();
    QByteArray  byteArrData;
    QDataStream stream(&byteArrData,QIODevice::WriteOnly);

    //CDataSetModelNode* pDSObj;
    //QModelIndex        modelIdx;
    //int                idxNode;

    //for( idxNode = 0; idxNode < i_arModelIdxs.size(); idxNode++ )
    //{
    //    modelIdx = i_arModelIdxs[idxNode];

    //    if( modelIdx.isValid() )
    //    {
    //        pDSObj = static_cast<CDataSetModelNode*>(modelIdx.internalPointer());

    //        if( pDSObj != nullptr )
    //        {
    //            QString strNodeKey = pDSObj->getKey();
    //            stream << strNodeKey;
    //        }
    //    }
    //}

    pMimeData->setData( "application/vnd.text.list", byteArrData );

    return pMimeData;

} // mimeData

//------------------------------------------------------------------------------
bool CDataSetModel::dropMimeData(
    const QMimeData*   i_pMimeData,
    Qt::DropAction     i_dropAction,
    int                i_iRow,
    int                i_iClm,
    const QModelIndex& /*i_modelIdxParent*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Action:" + qDropActions2Str(i_dropAction);
        strAddTrcInfo += ", Row:" + QString::number(i_iRow);
        strAddTrcInfo += ", Clm:" + QString::number(i_iClm);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dropMimeData",
        /* strAddInfo   */ strAddTrcInfo );

    bool bOk = false;

    if( m_pDS != nullptr && i_dropAction != Qt::IgnoreAction )
    {
        if( i_pMimeData->hasFormat("application/vnd.text.list") )
        {
            if( i_dropAction == Qt::MoveAction )
            {
                //QByteArray         byteArrData = i_pMimeData->data("application/vnd.text.list");
                //QDataStream        stream(&byteArrData,QIODevice::ReadOnly);
                //QString            strKey;
                //CDataSetModelNode* pDSObj = nullptr;

                //while( !stream.atEnd() )
                //{
                //    stream >> strKey;
                //    break;
                //}
                //pDSObj = findNodeByKey(strKey);

                //if( pDSObj != nullptr )
                //{
                //    CDataSetModelNode* pDSObjParentNew = static_cast<CDataSetModelNode*>(i_modelIdxParent.internalPointer());
                //    CDataSetModelNode* pDSObjParentOld = pDSObj->getParentNode();

                //    if( pDSObjParentOld != nullptr && pDSObjParentNew != nullptr )
                //    {
                //        // Changing the parent node ..
                //        if( pDSObjParentNew != pDSObjParentOld )
                //        {
                //            CDataSetObject*   pDSObj = pDSObj->getDataSetObject();
                //            CDataSetObjGroup* pDSGrp = pDSObjParentNew->toGroup();

                //            m_pDS->reparentObject(pDSObj,pDSGrp);
                //        }

                //        // Keeping the same parent node ..
                //        else // if( pDSObjParentNew == pDSObjParentOld )
                //        {
                //            // .. doesn't really make sense if there are several views on the model.
                //            // Each view may use a different sorting order and the elements may be
                //            // alphabetically sorted. And on changing the order in the model the
                //            // order of the objects within the data set is not affected.

                //            QModelIndex modelIdx = pDSObj->getModelIdx();
                //            int         iRowIdx  = modelIdx.row();
                //            //CDataSetModelNode* pDSObjSibling;
                //            //int                iRowIdxSibling;

                //            // Remove node from old position
                //            //----------------------------------

                //            beginRemoveRows( pDSObjParentOld->getModelIdx(), iRowIdx, iRowIdx );

                //            // Remove the node from the parents child list.
                //            // By removing a node the model indices of the siblings
                //            // "behind" the removed node must be corrected.
                //            // This is implicitly done by the nodes "removeChildNode" method.
                //            pDSObjParentOld->removeChildNode(pDSObj);

                //            endRemoveRows();

                //            // Insert node into new position
                //            //----------------------------------

                //            int iRowCount = rowCount(i_modelIdxParent);

                //            if( i_iRow != -1 )
                //            {
                //                iRowIdx = i_iRow;
                //            }
                //            else if( i_modelIdxParent.isValid() )
                //            {
                //                iRowIdx = i_modelIdxParent.row();
                //            }
                //            if( iRowIdx >= iRowCount )
                //            {
                //                iRowIdx = iRowCount-1;
                //            }

                //            // Insert the node into the parents child list.
                //            // By inserting a node the model indices of the siblings
                //            // "behind" the inserted node must be corrected.
                //            // This is implicitly done by the nodes "insertChildNode" method.
                //            iRowIdx = pDSObjParentNew->insertChildNode(pDSObj,iRowIdx);

                //            // Implicitly set by the node on calling "insertChildNode".
                //            //pDSObj->m_modelIdx = createIndex(iRowIdx,0,pDSObj);

                //            beginInsertRows( i_modelIdxParent, iRowIdx, iRowIdx );

                //            endInsertRows();

                //            bOk = true;

                //        } // if( pDSObjParentNew == pDSObjParentOld )
                //    } // if( pDSObjParentOld != nullptr && pDSObjParentNew != nullptr )
                //} // if( pDSObj != nullptr )
            } // if( i_dropAction == Qt::MoveAction )
        } // if( i_pMimeData->hasFormat("application/vnd.text.list") )
    } // if( i_dropAction != Qt::IgnoreAction )

    return bOk;

} // dropMimeData
