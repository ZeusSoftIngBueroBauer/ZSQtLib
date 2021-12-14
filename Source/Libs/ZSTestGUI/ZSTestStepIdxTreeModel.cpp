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
#include <QtCore/qmutex.h>
#include <QtCore/qfile.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtextstream.h>
#include <QtXml/qdom.h>
#include <QtGui/qicon.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qstyle.h>
#else
#include <QtWidgets/qstyle.h>
#endif

#include "ZSTestGUI/ZSTestStepIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;
using namespace ZS::Trace;


/*******************************************************************************
class CModeldxTreeTestSteps : public CModelIdxTree
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

static const SEnumEntry s_arEnumStrTestStepColumns[] =
{                                                                  // IdxName,         Symbol, Text
    /*  0 */ SEnumEntry( CModeldxTreeTestSteps::EColumnName,          "Name",          "Name", "Name"         ),
    /*  1 */ SEnumEntry( CModeldxTreeTestSteps::EColumnBreakpoint,    "Breakpoint",    "Bp",   "Breakpoint"   ),
    /*  2 */ SEnumEntry( CModeldxTreeTestSteps::EColumnEnabled,       "Enabled",       "En",   "Enabled"      ),
    /*  3 */ SEnumEntry( CModeldxTreeTestSteps::EColumnResult,        "Result",        "Res",  "Result"       ),
    /*  4 */ SEnumEntry( CModeldxTreeTestSteps::EColumnDuration,      "Duration",      "Dur",  "Duration"     ),
    /*  5 */ SEnumEntry( CModeldxTreeTestSteps::EColumnOperation,     "Operation",     "Op",   "Operation"    ),
    /*  6 */ SEnumEntry( CModeldxTreeTestSteps::EColumnDescription,   "Description",   "Dscr", "Description"  ),
    /*  7 */ SEnumEntry( CModeldxTreeTestSteps::EColumnDesiredValues, "DesiredValues", "Des",  "DesiredValues"),
    /*  8 */ SEnumEntry( CModeldxTreeTestSteps::EColumnActualValues,  "ActualValues",  "Act",  "ActualValues" )
};

//------------------------------------------------------------------------------
QString CModeldxTreeTestSteps::column2Str( int i_iClm, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrTestStepColumns, _ZSArrLen(s_arEnumStrTestStepColumns), i_iClm, i_alias);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModeldxTreeTestSteps::CModeldxTreeTestSteps(
    CTestStepIdxTree* i_pTestStepIdxTree,
    QObject*          i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelIdxTree(i_pTestStepIdxTree, i_pObjParent),
    m_bShowDesiredAndActualValuesOnlyIfTestStepFailed(true),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs))
    {
        strMthInArgs = "IdxTree: " + QString(i_pTestStepIdxTree == nullptr ? "nullptr" : i_pTestStepIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    //s_iInstCount++;

    //if( !QObject::connect(
    //    /* pObjSender   */ i_pTestStepIdxTree,
    //    /* szSignal     */ SIGNAL( testStepGroupInserted(QObject*,QObject*) ),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT( onTestStepGroupInserted(QObject*,QObject*) ) ) )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

    //if( !QObject::connect(
    //    /* pObjSender   */ i_pTestStepIdxTree,
    //    /* szSignal     */ SIGNAL( testStepGroupChanged(QObject*,QObject*) ),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT( onTestStepGroupChanged(QObject*,QObject*) ) ) )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

    //if( !QObject::connect(
    //    /* pObjSender   */ i_pTestStepIdxTree,
    //    /* szSignal     */ SIGNAL( testStepInserted(QObject*,QObject*) ),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT( onTestStepInserted(QObject*,QObject*) ) ) )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

    //if( !QObject::connect(
    //    /* pObjSender   */ i_pTestStepIdxTree,
    //    /* szSignal     */ SIGNAL( testStepChanged(QObject*,QObject*) ),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT( onTestStepChanged(QObject*,QObject*) ) ) )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

    //if( !QObject::connect(
    //    /* pObjSender   */ i_pTestStepIdxTree,
    //    /* szSignal     */ SIGNAL( nameSpaceNodeChanged(QObject*,ZS::System::#error CAbstractIdxTreeEntry*,int) ),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT( onNameSpaceNodeChanged(QObject*,ZS::System::#error CAbstractIdxTreeEntry*,int) ) ) )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

} // ctor

//------------------------------------------------------------------------------
CModeldxTreeTestSteps::~CModeldxTreeTestSteps()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_bShowDesiredAndActualValuesOnlyIfTestStepFailed = false;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepIdxTree* CModeldxTreeTestSteps::idxTree()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CTestStepIdxTree*>(m_pIdxTree);
}

//------------------------------------------------------------------------------
void CModeldxTreeTestSteps::setShowDesiredAndActualValuesOnlyIfTestStepFailed( bool i_bSet )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs))
    {
        strMthInArgs = bool2Str(i_bSet);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setShowDesiredAndActualValuesOnlyIfTestStepFailed",
        /* strAddInfo   */ strMthInArgs );

    if( m_bShowDesiredAndActualValuesOnlyIfTestStepFailed != i_bSet )
    {
        m_bShowDesiredAndActualValuesOnlyIfTestStepFailed = i_bSet;

        //reset();
    }

} // setShowDesiredAndActualValuesOnlyIfTestStepFailed

/*==============================================================================
protected slots:
==============================================================================*/

////------------------------------------------------------------------------------
//void CModeldxTreeTestSteps::onTestStepGroupInserted( QObject* i_pTestStepIdxTree, QObject* i_pTSGrp )
////------------------------------------------------------------------------------
//{
//    if( i_pTestStepIdxTree == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStepIdxTree == nullptr");
//    }
//
//    CTestStepGroup* pTSGrp = dynamic_cast<CTestStepGroup*>(i_pTSGrp);
//
//    if( pTSGrp == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTSGrp == nullptr");
//    }
//
//    #error CAbstractIdxTreeEntry* pTreeEntry = pTSGrp->getTreeEntry();
//
//    if( pTreeEntry == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTSGrp->m_pTreeEntry == nullptr");
//    }
//
//    #error CAbstractIdxTreeEntry* pTreeEntryParent = pTreeEntry->getParentEntry();
//
//    QModelIndex modelIdxParent;
//
//    if( pTreeEntryParent != nullptr )
//    {
//         modelIdxParent = createIndex( pTreeEntryParent->getRowId(), 0, pTreeEntryParent );
//    }
//
//    beginInsertRows( modelIdxParent, pTreeEntry->getRowId(), pTreeEntry->getRowId() );
//    endInsertRows();
//
//} // onTestStepGroupInserted
//
////------------------------------------------------------------------------------
//void CModeldxTreeTestSteps::onTestStepGroupChanged( QObject* i_pTestStepIdxTree, QObject* i_pTSGrp )
////------------------------------------------------------------------------------
//{
//    if( i_pTestStepIdxTree == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStepIdxTree == nullptr");
//    }
//
//    CTestStepGroup* pTSGrp = dynamic_cast<CTestStepGroup*>(i_pTSGrp);
//
//    if( pTSGrp == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTSGrp == nullptr");
//    }
//
//    #error CAbstractIdxTreeEntry* pTreeEntry = pTSGrp->getTreeEntry();
//
//    if( pTreeEntry == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTSGrp->m_pTreeEntry == nullptr");
//    }
//
//    QModelIndex modelIdxTopLeft     = createIndex( pTreeEntry->getRowId(), 0, pTreeEntry );
//    QModelIndex modelIdxBottomRight = createIndex( pTreeEntry->getRowId(), CModeldxTreeTestSteps::EColumnCount-1, pTreeEntry );
//
//    emit dataChanged( modelIdxTopLeft, modelIdxBottomRight );
//
//} // onTestStepGroupChanged
//
////------------------------------------------------------------------------------
//void CModeldxTreeTestSteps::onTestStepInserted( QObject* i_pTestStepIdxTree, QObject* i_pTestStep )
////------------------------------------------------------------------------------
//{
//    if( i_pTestStepIdxTree == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStepIdxTree == nullptr");
//    }
//
//    CTestStep* pTestStep = dynamic_cast<CTestStep*>(i_pTestStep);
//
//    if( pTestStep == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep == nullptr");
//    }
//
//    CIdxTreeListEntry* pListEntry = pTestStep->getListEntry();
//
//    if( pListEntry == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry == nullptr");
//    }
//
//    #error CAbstractIdxTreeEntry* pTreeEntry = pListEntry->getTreeEntry();
//
//    if( pTreeEntry == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry->m_pTreeEntry == nullptr");
//    }
//
//    #error CAbstractIdxTreeEntry* pTreeEntryParent = pTreeEntry->getParentEntry();
//
//    if( pTreeEntryParent == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry->m_pTreeEntry->pTreeEntryParent == nullptr");
//    }
//
//    int                iTestStepCount = -1;
//    int                idxRow;
//    #error CAbstractIdxTreeEntry* pTreeEntryChild;
//
//    for( idxRow = 0; idxRow < static_cast<int>(pTreeEntryParent->getChildCount()); idxRow++ )
//    {
//        pTreeEntryChild = pTreeEntryParent->getChildEntry(idxRow);
//
//        if( pTreeEntryChild != nullptr && pTreeEntryChild->getEntryType() == EIdxTreeEntryTypeObject )
//        {
//            iTestStepCount++;
//        }
//        if( pTreeEntryChild == pTreeEntry )
//        {
//            break;
//        }
//    }
//
//    if( iTestStepCount >= 0 && pTreeEntryChild == pTreeEntry )
//    {
//        QModelIndex modelIdxParent = createIndex( pTreeEntryParent->getRowId(), 0, pTreeEntryParent );
//
//        beginInsertRows( modelIdxParent, iTestStepCount, iTestStepCount );
//        endInsertRows();
//    }
//
//} // onTestStepInserted
//
////------------------------------------------------------------------------------
//void CModeldxTreeTestSteps::onTestStepChanged( QObject* i_pTestStepIdxTree, QObject* i_pTestStep )
////------------------------------------------------------------------------------
//{
//    if( i_pTestStepIdxTree == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pIdxTree == nullptr");
//    }
//
//    CTestStep* pTestStep = dynamic_cast<CTestStep*>(i_pTestStep);
//
//    if( pTestStep == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep == nullptr");
//    }
//
//    CIdxTreeListEntry* pListEntry = pTestStep->getListEntry();
//
//    if( pListEntry == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry == nullptr");
//    }
//
//    #error CAbstractIdxTreeEntry* pTreeEntry = pListEntry->getTreeEntry();
//
//    if( pTreeEntry == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry->m_pTreeEntry == nullptr");
//    }
//
//    #error CAbstractIdxTreeEntry* pTreeEntryParent = pTreeEntry->getParentEntry();
//
//    if( pTreeEntryParent == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry->m_pTreeEntry->pTreeEntryParent == nullptr");
//    }
//
//    int                iTestStepCount = -1;
//    int                idxRow;
//    #error CAbstractIdxTreeEntry* pTreeEntryChild;
//
//    for( idxRow = 0; idxRow < static_cast<int>(pTreeEntryParent->getChildCount()); idxRow++ )
//    {
//        pTreeEntryChild = pTreeEntryParent->getChildEntry(idxRow);
//
//        if( pTreeEntryChild != nullptr && pTreeEntryChild->getEntryType() == EIdxTreeEntryTypeObject )
//        {
//            iTestStepCount++;
//        }
//        if( pTreeEntryChild == pTreeEntry )
//        {
//            break;
//        }
//    }
//
//    if( iTestStepCount >= 0 && pTreeEntryChild == pTreeEntry )
//    {
//        QModelIndex modelIdxTopLeft     = createIndex( iTestStepCount, 0, pTreeEntry );
//        QModelIndex modelIdxBottomRight = createIndex( iTestStepCount, CModeldxTreeTestSteps::EColumnCount-1, pTreeEntry );
//        emit dataChanged( modelIdxTopLeft, modelIdxBottomRight );
//    }
//
//} // onTestStepChanged
//
////------------------------------------------------------------------------------
//void CModeldxTreeTestSteps::onNameSpaceNodeChanged(
//    QObject*           i_pTestStepIdxTree,
//    #error CAbstractIdxTreeEntry* i_pTreeEntry,
//    int                /*i_enabled*/ )
////------------------------------------------------------------------------------
//{
//    if( i_pTestStepIdxTree == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pIdxTree == nullptr");
//    }
//
//    if( i_pTreeEntry == nullptr )
//    {
//        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTreeEntry == nullptr");
//    }
//
//    QModelIndex modelIdxTopLeft     = createIndex( i_pTreeEntry->getRowId(), 0, i_pTreeEntry );
//    QModelIndex modelIdxBottomRight = createIndex( i_pTreeEntry->getRowId(), CModeldxTreeTestSteps::EColumnCount-1, i_pTreeEntry );
//    emit dataChanged( modelIdxTopLeft, modelIdxBottomRight );
//
//} // onNameSpaceNodeChanged

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CModeldxTreeTestSteps::reset()
////------------------------------------------------------------------------------
//{
//    beginResetModel();
//    endResetModel();
//}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModeldxTreeTestSteps::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelVerbose))
    {
        strMthInArgs = "ModelIdxParent {" + ModelIdx2Str(i_modelIdxParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelVerbose,
        /* strMethod    */ "columnCount",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelVerbose) )
    {
        mthTracer.setMethodReturn(EColumnCount);
    }

    return EColumnCount;

} // columnCount

//------------------------------------------------------------------------------
QVariant CModeldxTreeTestSteps::headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelVerbose))
    {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelVerbose,
        /* strMethod    */ "headerData",
        /* strAddInfo   */ strMthInArgs );

    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        if( i_iRole == Qt::DisplayRole )
        {
            varData = column2Str(i_iSection, EEnumEntryAliasStrText);
        }
    }
    else // if( i_orientation == Qt::Vertical )
    {
        if( i_iRole == Qt::DisplayRole )
        {
            varData = QString::number(i_iSection+1);
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelVerbose) )
    {
        mthTracer.setMethodReturn(varData.toString());
    }

    return varData;

} // headerData

//------------------------------------------------------------------------------
Qt::ItemFlags CModeldxTreeTestSteps::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelVerbose))
    {
        strMthInArgs = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelVerbose,
        /* strMethod    */ "flags",
        /* strAddInfo   */ strMthInArgs );

    Qt::ItemFlags uFlags = QAbstractItemModel::flags(i_modelIdx);

    if(i_modelIdx.isValid())
    {
        //#error CAbstractIdxTreeEntry* pTreeEntry = static_cast<#error CAbstractIdxTreeEntry*>(i_modelIdx.internalPointer());
        //CAbstractTestStepIdxTreeEntry* pTSAdmObj = nullptr;
        //CTestStepGroup*    pTSGrp = nullptr;
        //CTestStep*         pTestStep = nullptr;

        //if( pTreeEntry != nullptr )
        //{
        //    pTSAdmObj = reinterpret_cast<CAbstractTestStepIdxTreeEntry*>(pTreeEntry->getObj());

        //    if( pTSAdmObj != nullptr )
        //    {
        //        if( pTSAdmObj->isGroup() )
        //        {
        //            pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
        //        }
        //        else
        //        {
        //            pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
        //        }
        //    }
        //}

        //if( i_modelIdx.column() == EColumnActualValues )
        //{
        //    if( pTSGrp != nullptr )
        //    {
        //        flags |= Qt::ItemIsEditable;
        //    }
        //    else if( pTestStep != nullptr )
        //    {
        //        flags |= Qt::ItemIsEditable;
        //    }
        //}
    }

    if( mthTracer.isActive(ETraceDetailLevelVerbose) )
    {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }

    return uFlags;

} // flags

//------------------------------------------------------------------------------
QVariant CModeldxTreeTestSteps::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelVerbose))
    {
        strMthInArgs = "ModelIdx {" + ModelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelVerbose,
        /* strMethod    */ "data",
        /* strAddInfo   */ strMthInArgs );

    QVariant varData;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    CIdxTreeEntry* pIdxTreeEntry = nullptr;

    if( pModelTreeEntry != nullptr )
    {
        pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
    }

    if( pIdxTreeEntry != nullptr )
    {
        CAbstractTestStepIdxTreeEntry* pTestStepEntry = nullptr;
        CTestStepGroup*                pTestStepGroup = nullptr;
        CTestStep*                     pTestStep = nullptr;

        if( !pIdxTreeEntry->isRoot() )
        {
            pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

            if( pTestStepEntry->isGroup() )
            {
                pTestStepGroup = dynamic_cast<CTestStepGroup*>(pTestStepEntry);
            }
            else
            {
                pTestStep = dynamic_cast<CTestStep*>(pTestStepEntry);
            }
        }

        switch(i_modelIdx.column())
        {
            case EColumnName:
            {
                if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                {
                    varData = pModelTreeEntry->name();
                }
                else if(i_iRole == Qt::DecorationRole)
                {
                    varData = CModeldxTreeTestSteps::GetIcon(pModelTreeEntry->entryType());
                }
                else if(i_iRole == Qt::FontRole)
                {
                    //QFont fnt;

                    //if(pTestStep != nullptr && pTestStepCurr != nullptr)
                    //{
                    //    if(pTestStep == pTestStepCurr)
                    //    {
                    //        fnt.setBold(true);
                    //    }
                    //}
                    //else if(pTSGrp != nullptr && pTestStepCurr != nullptr)
                    //{
                    //    if(pTSGrp->isParentOf(pTestStepCurr))
                    //    {
                    //        fnt.setBold(true);
                    //    }
                    //}
                    //varData = fnt;
                }
                break;
            }
            case EColumnBreakpoint:
            {
                if(pTestStep != nullptr)
                {
                    if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                    {
                        varData = pTestStep->isBreakpointSet();
                    }
                    else if(i_iRole == Qt::ForegroundRole)
                    {
                        if(pTestStep->isBreakpointSet())
                        {
                            if(pTestStep->isBreakpointEnabled())
                            {
                                varData = QColor(Qt::red);
                            }
                            else
                            {
                                varData = QColor(Qt::darkGray);
                            }
                        }
                    }
                }
                break;
            }
            case EColumnEnabled:
            {
                if(pTestStepEntry != nullptr)
                {
                    if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                    {
                        varData = static_cast<int>(pTestStepEntry->getEnabled());
                    }
                }
                break;
            }
            case EColumnResult:
            {
                //if(pTreeEntry != nullptr)
                //{
                //    if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                //    {
                //        varData = testResult2Str(testResult);
                //    }
                //    else if(i_iRole == Qt::ForegroundRole)
                //    {
                //        if(testResult == ETestResultTestFailed)
                //        {
                //            varData = QColor(Qt::red);
                //        }
                //        else if(testResult == ETestResultTestPassed)
                //        {
                //            varData = QColor(Qt::darkGreen);
                //        }
                //        else if(testResult == ETestResultIgnore)
                //        {
                //            varData = QColor(Qt::darkGray);
                //        }
                //        else
                //        {
                //            varData = QColor(Qt::blue);
                //        }
                //    }
                //}
                break;
            }
            case EColumnDuration:
            {
                //if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                //{
                //    if(pTSAdmObj != nullptr && testResult != ETestResultUndefined)
                //    {
                //        if(pTSAdmObj->isTestRunning())
                //        {
                //            varData = "...";
                //        }
                //        else
                //        {
                //            varData = pTSAdmObj->testDuration2StrInBestUnit();
                //        }
                //    }
                //}
                break;
            }
            case EColumnOperation:
            {
                if(pTestStep != nullptr)
                {
                    if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                    {
                        varData = pTestStep->getOperation();
                    }
                    else if(i_iRole == Qt::ToolTipRole)
                    {
                        varData = pTestStep->getToolTip();
                    }
                }
                break;
            }
            case EColumnDescription:
            {
                if(pTestStep != nullptr)
                {
                    if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                    {
                        varData = pTestStep->getDescription();
                    }
                    else if(i_iRole == Qt::ToolTipRole)
                    {
                        varData = pTestStep->getToolTip();
                    }
                }
                break;
            }
            case EColumnDesiredValues:
            {
                if(pTestStep != nullptr)
                {
                    //if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                    //{
                    //    bool bShow = true;

                    //    if(m_bShowDesiredAndActualValuesOnlyIfTestStepFailed)
                    //    {
                    //        if(testResult != ETestResultTestFailed)
                    //        {
                    //            bShow = false;
                    //        }
                    //    }
                    //    if(bShow)
                    //    {
                    //        varData = pTestStep->getDesiredValues().join("\n");
                    //    }
                    //}
                    //else if(i_iRole == Qt::ToolTipRole)
                    //{
                    //    varData = pTestStep->getToolTip();
                    //}
                }
                break;
            }
            case EColumnActualValues:
            {
                if(pTestStep != nullptr)
                {
                    if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                    {
                        //QStringList strlstDesiredValues;
                        //QStringList strlstActualValues;
                        //QString     strDesiredVal;
                        //QString     strActualVal;
                        //int         idxVal;
                        //bool        bShow = true;

                        //if(m_bShowDesiredAndActualValuesOnlyIfTestStepFailed)
                        //{
                        //    if(testResult != ETestResultTestFailed)
                        //    {
                        //        bShow = false;
                        //    }
                        //}
                        //if(bShow)
                        //{
                        //    strlstDesiredValues = pTestStep->getDesiredValues();
                        //    strlstActualValues = pTestStep->getActualValues();

                        //    for(idxVal = 0; idxVal < strlstActualValues.size(); idxVal++)
                        //    {
                        //        strActualVal = strlstActualValues[idxVal];

                        //        if(idxVal < strlstDesiredValues.size())
                        //        {
                        //            strDesiredVal = strlstDesiredValues[idxVal];
                        //        }
                        //        else
                        //        {
                        //            strDesiredVal = "";
                        //        }
                        //        if(strActualVal.compare(strDesiredVal, Qt::CaseSensitive) != 0)
                        //        {
                        //            strlstActualValues[idxVal] = "!! " + strActualVal + " !!";
                        //        }
                        //    }
                        //    varData = strlstActualValues.join("\n");
                        //}
                    }
                    else if(i_iRole == Qt::ToolTipRole)
                    {
                        varData = pTestStep->getToolTip();
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pIdxTreeEntry != nullptr )

    if(mthTracer.isActive(ETraceDetailLevelVerbose))
    {
        mthTracer.setMethodReturn(varData.toString());
    }

    return varData;

} // data
