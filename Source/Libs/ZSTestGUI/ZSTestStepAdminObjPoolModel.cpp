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

#if QT_VERSION < 0x050000
#include <QtGui/qstyle.h>
#else
#include <QtWidgets/qstyle.h>
#endif

#include "ZSTestGUI/ZSTestStepAdminObjPoolModel.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CTestStepAdminObjPoolModel : public CModelObjPool
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

static const SEnumEntry s_arEnumStrTestStepColumns[] =
{                                                                        // IdxName,        Symbol, Text
    /*  0 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnName,          "Name",          "Name", "Name"         ),
    /*  1 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnObjId,         "ObjId",         "Id",   "ObjId"        ),
    /*  2 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnBreakpoint,    "Breakpoint",    "Bp",   "Breakpoint"   ),
    /*  3 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnEnabled,       "Enabled",       "En",   "Enabled"      ),
    /*  4 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnResult,        "Result",        "Res",  "Result"       ),
    /*  5 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnDuration,      "Duration",      "Dur",  "Duration"     ),
    /*  6 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnOperation,     "Operation",     "Op",   "Operation"    ),
    /*  7 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnDescription,   "Description",   "Dscr", "Description"  ),
    /*  8 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnDesiredValues, "DesiredValues", "Des",  "DesiredValues"),
    /*  9 */ SEnumEntry( CTestStepAdminObjPoolModel::EColumnActualValues,  "ActualValues",  "Act",  "ActualValues" )
};

//------------------------------------------------------------------------------
QString CTestStepAdminObjPoolModel::column2Str( int i_iClm, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrTestStepColumns, _ZSArrLen(s_arEnumStrTestStepColumns), i_iClm, i_alias);
}

/*==============================================================================
protected: // class members
==============================================================================*/

int CTestStepAdminObjPoolModel::s_iInstCount = 0;

bool CTestStepAdminObjPoolModel::s_bIconsCreated = false;

QIcon* CTestStepAdminObjPoolModel::s_pIconTypeNameSpace = nullptr;
QIcon* CTestStepAdminObjPoolModel::s_pIconTypeObject = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QIcon CTestStepAdminObjPoolModel::GetIcon( CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QIcon icon;

    if( !CModelObjPool::AreIconsCreated() )
    {
        CModelObjPool::GetIcon(EObjPoolEntryTypeNameSpace);
        CModelObjPool::GetIcon(EObjPoolEntryTypeObject);
    }

    if( !s_bIconsCreated )
    {
        QSize sizePxm;

        s_pIconTypeNameSpace = new QIcon();
        s_pIconTypeObject = new QIcon();

        sizePxm = CModelObjPool::GetPxmTypeNameSpaceNormalOff()->size();
        sizePxm = CModelObjPool::GetPxmTypeNameSpaceSelectedOff()->size();
        sizePxm = CModelObjPool::GetPxmTypeObjectNormalOff()->size();
        sizePxm = CModelObjPool::GetPxmTypeObjectSelectedOff()->size();

        s_pIconTypeNameSpace->addPixmap( *CModelObjPool::GetPxmTypeNameSpaceNormalOff(), QIcon::Normal,   QIcon::Off );
        s_pIconTypeNameSpace->addPixmap( *CModelObjPool::GetPxmTypeNameSpaceSelectedOff(), QIcon::Selected, QIcon::Off );
        s_pIconTypeNameSpace->addPixmap( *CModelObjPool::GetPxmTypeNameSpaceNormalOff(), QIcon::Normal,   QIcon::On  );
        s_pIconTypeNameSpace->addPixmap( *CModelObjPool::GetPxmTypeNameSpaceSelectedOff(), QIcon::Selected, QIcon::On  );

        s_pIconTypeObject->addPixmap( *CModelObjPool::GetPxmTypeObjectNormalOff(), QIcon::Normal,   QIcon::Off );
        s_pIconTypeObject->addPixmap( *CModelObjPool::GetPxmTypeObjectSelectedOff(), QIcon::Selected, QIcon::Off );
        s_pIconTypeObject->addPixmap( *CModelObjPool::GetPxmTypeObjectNormalOff(), QIcon::Normal,   QIcon::On  );
        s_pIconTypeObject->addPixmap( *CModelObjPool::GetPxmTypeObjectSelectedOff(), QIcon::Selected, QIcon::On  );

        s_bIconsCreated = true;
    }

    if( i_pTreeEntry != nullptr )
    {
        if( i_pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
        {
            icon = *s_pIconTypeObject;
        }
        else
        {
            icon = *s_pIconTypeNameSpace;
        }
    }

    return icon;

} // getIcon

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepAdminObjPoolModel::CTestStepAdminObjPoolModel(
    CTestStepAdminObjPool* i_pAdminObjPool,
    QObject*               i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelObjPool(i_pAdminObjPool,i_pObjParent),
    m_bShowDesiredAndActualValuesOnlyIfTestStepFailed(true)
{
    s_iInstCount++;

    if( !QObject::connect(
        /* pObjSender   */ i_pAdminObjPool,
        /* szSignal     */ SIGNAL( testStepGroupInserted(QObject*,QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTestStepGroupInserted(QObject*,QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ i_pAdminObjPool,
        /* szSignal     */ SIGNAL( testStepGroupChanged(QObject*,QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTestStepGroupChanged(QObject*,QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ i_pAdminObjPool,
        /* szSignal     */ SIGNAL( testStepInserted(QObject*,QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTestStepInserted(QObject*,QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ i_pAdminObjPool,
        /* szSignal     */ SIGNAL( testStepChanged(QObject*,QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTestStepChanged(QObject*,QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ i_pAdminObjPool,
        /* szSignal     */ SIGNAL( nameSpaceNodeChanged(QObject*,ZS::System::CObjPoolTreeEntry*,int) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onNameSpaceNodeChanged(QObject*,ZS::System::CObjPoolTreeEntry*,int) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CTestStepAdminObjPoolModel::~CTestStepAdminObjPoolModel()
//------------------------------------------------------------------------------
{
    s_iInstCount--;

    if( s_iInstCount == 0 )
    {
        try
        {
            delete s_pIconTypeNameSpace;
        }
        catch(...)
        {
        }
        s_pIconTypeNameSpace = nullptr;

        try
        {
            delete s_pIconTypeObject;
        }
        catch(...)
        {
        }
        s_pIconTypeObject = nullptr;

        s_bIconsCreated = false;
    }

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepAdminObjPool* CTestStepAdminObjPoolModel::getAdminObjIdxTree()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CTestStepAdminObjPool*>(m_pObjPool);
}

//------------------------------------------------------------------------------
void CTestStepAdminObjPoolModel::setShowDesiredAndActualValuesOnlyIfTestStepFailed( bool i_bSet )
//------------------------------------------------------------------------------
{
    if( m_bShowDesiredAndActualValuesOnlyIfTestStepFailed != i_bSet )
    {
        m_bShowDesiredAndActualValuesOnlyIfTestStepFailed = i_bSet;

        reset();
    }

} // setShowDesiredAndActualValuesOnlyIfTestStepFailed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObjPoolModel::onTestStepGroupInserted( QObject* i_pAdminObjPool, QObject* i_pTSGrp )
//------------------------------------------------------------------------------
{
    if( i_pAdminObjPool == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pAdminObjPool == nullptr");
    }

    CTestStepGroup* pTSGrp = dynamic_cast<CTestStepGroup*>(i_pTSGrp);

    if( pTSGrp == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTSGrp == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntry = pTSGrp->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTSGrp->m_pTreeEntry == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntryParent = pTreeEntry->getParentEntry();

    QModelIndex modelIdxParent;

    if( pTreeEntryParent != nullptr )
    {
         modelIdxParent = createIndex( pTreeEntryParent->getRowId(), 0, pTreeEntryParent );
    }

    beginInsertRows( modelIdxParent, pTreeEntry->getRowId(), pTreeEntry->getRowId() );
    endInsertRows();

} // onTestStepGroupInserted

//------------------------------------------------------------------------------
void CTestStepAdminObjPoolModel::onTestStepGroupChanged( QObject* i_pAdminObjPool, QObject* i_pTSGrp )
//------------------------------------------------------------------------------
{
    if( i_pAdminObjPool == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pAdminObjPool == nullptr");
    }

    CTestStepGroup* pTSGrp = dynamic_cast<CTestStepGroup*>(i_pTSGrp);

    if( pTSGrp == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTSGrp == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntry = pTSGrp->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTSGrp->m_pTreeEntry == nullptr");
    }

    QModelIndex modelIdxTopLeft     = createIndex( pTreeEntry->getRowId(), 0, pTreeEntry );
    QModelIndex modelIdxBottomRight = createIndex( pTreeEntry->getRowId(), CTestStepAdminObjPoolModel::EColumnCount-1, pTreeEntry );

    emit dataChanged( modelIdxTopLeft, modelIdxBottomRight );

} // onTestStepGroupChanged

//------------------------------------------------------------------------------
void CTestStepAdminObjPoolModel::onTestStepInserted( QObject* i_pAdminObjPool, QObject* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( i_pAdminObjPool == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pAdminObjPool == nullptr");
    }

    CTestStep* pTestStep = dynamic_cast<CTestStep*>(i_pTestStep);

    if( pTestStep == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep == nullptr");
    }

    CObjPoolListEntry* pListEntry = pTestStep->getListEntry();

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntry = pListEntry->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry->m_pTreeEntry == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntryParent = pTreeEntry->getParentEntry();

    if( pTreeEntryParent == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry->m_pTreeEntry->pTreeEntryParent == nullptr");
    }

    int                iTestStepCount = -1;
    int                idxRow;
    CObjPoolTreeEntry* pTreeEntryChild;

    for( idxRow = 0; idxRow < static_cast<int>(pTreeEntryParent->getChildCount()); idxRow++ )
    {
        pTreeEntryChild = pTreeEntryParent->getChildEntry(idxRow);

        if( pTreeEntryChild != nullptr && pTreeEntryChild->getEntryType() == EObjPoolEntryTypeObject )
        {
            iTestStepCount++;
        }
        if( pTreeEntryChild == pTreeEntry )
        {
            break;
        }
    }

    if( iTestStepCount >= 0 && pTreeEntryChild == pTreeEntry )
    {
        QModelIndex modelIdxParent = createIndex( pTreeEntryParent->getRowId(), 0, pTreeEntryParent );

        beginInsertRows( modelIdxParent, iTestStepCount, iTestStepCount );
        endInsertRows();
    }

} // onTestStepInserted

//------------------------------------------------------------------------------
void CTestStepAdminObjPoolModel::onTestStepChanged( QObject* i_pAdminObjPool, QObject* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( i_pAdminObjPool == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObjPool == nullptr");
    }

    CTestStep* pTestStep = dynamic_cast<CTestStep*>(i_pTestStep);

    if( pTestStep == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep == nullptr");
    }

    CObjPoolListEntry* pListEntry = pTestStep->getListEntry();

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntry = pListEntry->getTreeEntry();

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry->m_pTreeEntry == nullptr");
    }

    CObjPoolTreeEntry* pTreeEntryParent = pTreeEntry->getParentEntry();

    if( pTreeEntryParent == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTestStep->m_pListEntry->m_pTreeEntry->pTreeEntryParent == nullptr");
    }

    int                iTestStepCount = -1;
    int                idxRow;
    CObjPoolTreeEntry* pTreeEntryChild;

    for( idxRow = 0; idxRow < static_cast<int>(pTreeEntryParent->getChildCount()); idxRow++ )
    {
        pTreeEntryChild = pTreeEntryParent->getChildEntry(idxRow);

        if( pTreeEntryChild != nullptr && pTreeEntryChild->getEntryType() == EObjPoolEntryTypeObject )
        {
            iTestStepCount++;
        }
        if( pTreeEntryChild == pTreeEntry )
        {
            break;
        }
    }

    if( iTestStepCount >= 0 && pTreeEntryChild == pTreeEntry )
    {
        QModelIndex modelIdxTopLeft     = createIndex( iTestStepCount, 0, pTreeEntry );
        QModelIndex modelIdxBottomRight = createIndex( iTestStepCount, CTestStepAdminObjPoolModel::EColumnCount-1, pTreeEntry );
        emit dataChanged( modelIdxTopLeft, modelIdxBottomRight );
    }

} // onTestStepChanged

//------------------------------------------------------------------------------
void CTestStepAdminObjPoolModel::onNameSpaceNodeChanged(
    QObject*           i_pAdminObjPool,
    CObjPoolTreeEntry* i_pTreeEntry,
    int                /*i_enabled*/ )
//------------------------------------------------------------------------------
{
    if( i_pAdminObjPool == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObjPool == nullptr");
    }

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pTreeEntry == nullptr");
    }

    QModelIndex modelIdxTopLeft     = createIndex( i_pTreeEntry->getRowId(), 0, i_pTreeEntry );
    QModelIndex modelIdxBottomRight = createIndex( i_pTreeEntry->getRowId(), CTestStepAdminObjPoolModel::EColumnCount-1, i_pTreeEntry );
    emit dataChanged( modelIdxTopLeft, modelIdxBottomRight );

} // onNameSpaceNodeChanged

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepAdminObjPoolModel::reset()
//------------------------------------------------------------------------------
{
    beginResetModel();
    endResetModel();
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CTestStepAdminObjPoolModel::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CTestStepAdminObjPoolModel::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    CObjPoolTreeEntry* pTreeEntry = static_cast<CObjPoolTreeEntry*>(i_modelIdx.internalPointer());

    if( pTreeEntry == nullptr )
    {
        return varData;
    }

    CTest*             pTest = dynamic_cast<CTestStepAdminObjPool*>(m_pObjPool)->getTest();
    CTestStep*         pTestStepCurr = pTest->getCurrentTestStep();
    CTestStepAdminObj* pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntry->getObj());
    CTestStepGroup*    pTSGrp = nullptr;
    CTestStep*         pTestStep = nullptr;
    ETestResult        testResult = ETestResultUndefined;

    // TestModelEntry == nullptr in case of the root node.

    if( pTSAdmObj != nullptr )
    {
        if( pTSAdmObj->isGroup() )
        {
            pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
        }
        else
        {
            pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
        }
        testResult = pTSAdmObj->getTestResult();
    }

    switch( i_modelIdx.column() )
    {
        case EColumnName:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                varData = pTreeEntry->getNodeName(false);
            }
            else if( i_iRole == Qt::DecorationRole )
            {
                varData = CTestStepAdminObjPoolModel::GetIcon(pTreeEntry);
            }
            else if( i_iRole == Qt::FontRole )
            {
                QFont fnt;

                if( pTestStep != nullptr && pTestStepCurr != nullptr )
                {
                    if( pTestStep == pTestStepCurr )
                    {
                        fnt.setBold(true);
                    }
                }
                else if( pTSGrp != nullptr && pTestStepCurr != nullptr )
                {
                    if( pTSGrp->isParentOf(pTestStepCurr) )
                    {
                        fnt.setBold(true);
                    }
                }
                varData = fnt;
            }
            break;
        }
        case EColumnObjId:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
                {
                    varData = pTreeEntry->getObjId();
                }
            }
            break;
        }
        case EColumnBreakpoint:
        {
            if( pTestStep != nullptr )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pTestStep->isBreakpointSet();
                }
                else if( i_iRole == Qt::ForegroundRole )
                {
                    if( pTestStep->isBreakpointSet() )
                    {
                        if( pTestStep->isBreakpointDisabled() )
                        {
                            varData = QColor(Qt::darkGray);
                        }
                        else
                        {
                            varData = QColor(Qt::red);
                        }
                    }
                }
            }
            break;
        }
        case EColumnEnabled:
        {
            if( pTSAdmObj != nullptr )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = static_cast<int>(pTSAdmObj->getEnabled());
                }
            }
            break;
        }
        case EColumnResult:
        {
            if( pTSAdmObj != nullptr )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = testResult2Str(testResult);
                }
                else if( i_iRole == Qt::ForegroundRole )
                {
                    if( testResult == ETestResultTestFailed )
                    {
                        varData = QColor(Qt::red);
                    }
                    else if( testResult == ETestResultTestPassed )
                    {
                        varData = QColor(Qt::darkGreen);
                    }
                    else if( testResult == ETestResultIgnore )
                    {
                        varData = QColor(Qt::darkGray);
                    }
                    else
                    {
                        varData = QColor(Qt::blue);
                    }
                }
            }
            break;
        }
        case EColumnDuration:
        {
            if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
            {
                if( pTSAdmObj != nullptr && testResult != ETestResultUndefined )
                {
                    if( pTSAdmObj->isTestRunning() )
                    {
                        varData = "...";
                    }
                    else
                    {
                        varData = pTSAdmObj->testDuration2StrInBestUnit();
                    }
                }
            }
            break;
        }
        case EColumnOperation:
        {
            if( pTestStep != nullptr )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pTestStep->getOperation();
                }
                else if( i_iRole == Qt::ToolTipRole )
                {
                    varData = pTestStep->getToolTip();
                }
            }
            break;
        }
        case EColumnDescription:
        {
            if( pTestStep != nullptr )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pTestStep->getDescription();
                }
                else if( i_iRole == Qt::ToolTipRole )
                {
                    varData = pTestStep->getToolTip();
                }
            }
            break;
        }
        case EColumnDesiredValues:
        {
            if( pTestStep != nullptr )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    bool bShow = true;

                    if( m_bShowDesiredAndActualValuesOnlyIfTestStepFailed )
                    {
                        if( testResult != ETestResultTestFailed )
                        {
                            bShow = false;
                        }
                    }
                    if( bShow )
                    {
                        varData = pTestStep->getDesiredValues().join("\n");
                    }
                }
                else if( i_iRole == Qt::ToolTipRole )
                {
                    varData = pTestStep->getToolTip();
                }
            }
            break;
        }
        case EColumnActualValues:
        {
            if( pTestStep != nullptr )
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    QStringList strlstDesiredValues;
                    QStringList strlstActualValues;
                    QString     strDesiredVal;
                    QString     strActualVal;
                    int         idxVal;
                    bool        bShow = true;

                    if( m_bShowDesiredAndActualValuesOnlyIfTestStepFailed )
                    {
                        if( testResult != ETestResultTestFailed )
                        {
                            bShow = false;
                        }
                    }
                    if( bShow )
                    {
                        strlstDesiredValues = pTestStep->getDesiredValues();
                        strlstActualValues = pTestStep->getActualValues();

                        for( idxVal = 0; idxVal < strlstActualValues.size(); idxVal++ )
                        {
                            strActualVal = strlstActualValues[idxVal];

                            if( idxVal < strlstDesiredValues.size() )
                            {
                                strDesiredVal = strlstDesiredValues[idxVal];
                            }
                            else
                            {
                                strDesiredVal = "";
                            }
                            if( strActualVal.compare(strDesiredVal,Qt::CaseSensitive) != 0 )
                            {
                                strlstActualValues[idxVal] = "!! " + strActualVal + " !!";
                            }
                        }
                        varData = strlstActualValues.join("\n");
                    }
                }
                else if( i_iRole == Qt::ToolTipRole )
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

    return varData;

} // data

//------------------------------------------------------------------------------
Qt::ItemFlags CTestStepAdminObjPoolModel::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(i_modelIdx);

    if( !i_modelIdx.isValid() )
    {
        return flags;
    }

    //CObjPoolTreeEntry* pTreeEntry = static_cast<CObjPoolTreeEntry*>(i_modelIdx.internalPointer());
    //CTestStepAdminObj* pTSAdmObj = nullptr;
    //CTestStepGroup*    pTSGrp = nullptr;
    //CTestStep*         pTestStep = nullptr;
    
    //if( pTreeEntry != nullptr )
    //{
    //    pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntry->getObj());
    
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

    return flags;

} // flags

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CTestStepAdminObjPoolModel::headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole ) const
//------------------------------------------------------------------------------
{
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

    return varData;

} // headerData
