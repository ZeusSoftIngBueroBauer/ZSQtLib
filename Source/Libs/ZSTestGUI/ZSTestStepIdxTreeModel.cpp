/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSTest/ZSTestStepRoot.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysTime.h"
#ifdef ZS_TRACE_GUI_MODELS
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CModeldxTreeTestSteps : public CModelIdxTree
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

static const SEnumEntry s_arEnumStrTestStepColumns[] =
{                                                                   // IdxName,          Symbol, Text
    /*  0 */ SEnumEntry( CModeldxTreeTestSteps::EColumnName,           "Name",           "Name", "Name"         ),
    /*  1 */ SEnumEntry( CModeldxTreeTestSteps::EColumnBreakpoint,     "Breakpoint",     "Bp",   "Breakpoint"   ),
    /*  2 */ SEnumEntry( CModeldxTreeTestSteps::EColumnEnabled,        "Enabled",        "En",   "Enabled"      ),
    /*  3 */ SEnumEntry( CModeldxTreeTestSteps::EColumnResult,         "Result",         "Res",  "Result"       ),
    /*  4 */ SEnumEntry( CModeldxTreeTestSteps::EColumnDuration,       "Duration",       "Dur",  "Duration"     ),
    /*  5 */ SEnumEntry( CModeldxTreeTestSteps::EColumnOperation,      "Operation",      "Op",   "Operation"    ),
    /*  6 */ SEnumEntry( CModeldxTreeTestSteps::EColumnDescription,    "Description",    "Dscr", "Description"  ),
    /*  7 */ SEnumEntry( CModeldxTreeTestSteps::EColumnExpectedValues, "ExpectedValues", "Exp",  "ExpectedValues"),
    /*  8 */ SEnumEntry( CModeldxTreeTestSteps::EColumnResultValues,   "ResultValues",   "Act",  "ResultValues" )
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
    m_pTest(i_pTestStepIdxTree->getTest()),
    m_pTestStepCurr(nullptr),
    m_bShowExpectedAndResultValuesOnlyIfTestStepFailed(true)
    #ifdef ZS_TRACE_GUI_MODELS
    ,m_pTrcAdminObj(nullptr)
    #endif
{
    #ifdef ZS_TRACE_GUI_MODELS
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "IdxTree: " + QString(i_pTestStepIdxTree == nullptr ? "nullptr" : i_pTestStepIdxTree->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );
    #endif

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(currentTestStepChanged(ZS::Test::CTestStep*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTestCurrentTestStepChanged(ZS::Test::CTestStep*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CModeldxTreeTestSteps::~CModeldxTreeTestSteps()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );
    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    #endif

    m_pTest = nullptr;
    m_pTestStepCurr = nullptr;
    m_bShowExpectedAndResultValuesOnlyIfTestStepFailed = false;

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

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModeldxTreeTestSteps::setShowExpectedAndResultValuesOnlyIfTestStepFailed( bool i_bSet )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bSet);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setShowExpectedAndResultValuesOnlyIfTestStepFailed",
        /* strAddInfo   */ strMthInArgs );
    #endif

    if( m_bShowExpectedAndResultValuesOnlyIfTestStepFailed != i_bSet )
    {
        m_bShowExpectedAndResultValuesOnlyIfTestStepFailed = i_bSet;

        beginResetModel();
        endResetModel();
    }
}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CModeldxTreeTestSteps::onTestCurrentTestStepChanged( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "TestStep: " + QString(i_pTestStep == nullptr ? "nullptr" : i_pTestStep->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onTestCurrentTestStepChanged",
        /* strMthInArgs */ strMthInArgs );
    #endif

    if( m_pTestStepCurr != nullptr )
    {
        QModelIndex modelIdxTL = index(m_pTestStepCurr->keyInTree(), 0);
        QModelIndex modelIdxBR = index(m_pTestStepCurr->keyInTree(), columnCount()-1);

        emit_dataChanged(modelIdxTL, modelIdxBR);
    }

    m_pTestStepCurr = i_pTestStep;

    if( m_pTestStepCurr != nullptr )
    {
        QModelIndex modelIdxTL = index(m_pTestStepCurr->keyInTree(), 0);
        QModelIndex modelIdxBR = index(m_pTestStepCurr->keyInTree(), columnCount()-1);

        emit_dataChanged(modelIdxTL, modelIdxBR);
    }
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModeldxTreeTestSteps::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsVerbose))
    {
        strMthInArgs = "ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ EMethodTraceDetailLevel::ArgsVerbose,
        /* strMethod    */ "columnCount",
        /* strAddInfo   */ strMthInArgs );
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsVerbose) )
    {
        mthTracer.setMethodReturn(EColumnCount);
    }
    #endif

    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModeldxTreeTestSteps::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsVerbose))
    {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ EMethodTraceDetailLevel::ArgsVerbose,
        /* strMethod    */ "headerData",
        /* strAddInfo   */ strMthInArgs );
    #endif

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

    #ifdef ZS_TRACE_GUI_MODELS
    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsVerbose) )
    {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;
}

//------------------------------------------------------------------------------
QVariant CModeldxTreeTestSteps::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    QString strMthInArgs;
    if(m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsVerbose))
    {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ EMethodTraceDetailLevel::ArgsVerbose,
        /* strMethod    */ "data",
        /* strAddInfo   */ strMthInArgs );
    #endif

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
        CAbstractTestStepIdxTreeEntry* pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

        CTestStepGroup* pTestStepGroup = nullptr;
        CTestStep*      pTestStep = nullptr;

        if( pTestStepEntry->isGroup() )
        {
            pTestStepGroup = dynamic_cast<CTestStepGroup*>(pTestStepEntry);
        }
        else if( pTestStepEntry->isLeave() )
        {
            pTestStep = dynamic_cast<CTestStep*>(pTestStepEntry);
        }

        CEnumTestResult testResult = pTestStepEntry->getTestResult();

        switch( i_modelIdx.column() )
        {
            case EColumnName:
            {
                if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                {
                    varData = pModelTreeEntry->name();
                }
                else if(i_iRole == Qt::DecorationRole)
                {
                    varData = CModeldxTreeTestSteps::getIcon(pModelTreeEntry->entryType());
                }
                else if(i_iRole == Qt::FontRole)
                {
                    QFont fnt;

                    if( pTestStep != nullptr && pTestStep == m_pTestStepCurr )
                    {
                        fnt.setBold(true);
                    }
                    else if( pTestStepGroup != nullptr && m_pTestStepCurr != nullptr )
                    {
                        if( m_pTestStepCurr->isChildOf(pTestStepGroup) )
                        {
                            fnt.setBold(true);
                        }
                    }
                    varData = fnt;
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
                            if( pTestStep->isBreakpointEnabled() )
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
                if( pTestStepGroup != nullptr || pTestStep != nullptr )
                {
                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        varData = static_cast<int>(pTestStepEntry->getEnabled());
                    }
                }
                break;
            }
            case EColumnResult:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = testResult.toString();
                }
                else if( i_iRole == Qt::ForegroundRole )
                {
                    if( testResult == ETestResult::TestFailed )
                    {
                        varData = QColor(Qt::red);
                    }
                    else if( testResult == ETestResult::TestPassed )
                    {
                        varData = QColor(Qt::darkGreen);
                    }
                    else if( testResult == ETestResult::TestSkipped )
                    {
                        varData = QColor(Qt::darkGray);
                    }
                    else
                    {
                        varData = QColor(Qt::blue);
                    }
                }
                break;
            }
            case EColumnDuration:
            {
                if( pTestStepEntry != nullptr )
                {
                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        if( testResult == ETestResult::Undefined )
                        {
                            varData = "...";
                        }
                        else
                        {
                            varData = pTestStepEntry->testDuration2StrInBestUnit();
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
            case EColumnExpectedValues:
            {
                if( pTestStep != nullptr )
                {
                    if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                    {
                        bool bShow = true;

                        if(m_bShowExpectedAndResultValuesOnlyIfTestStepFailed)
                        {
                            if(testResult != ETestResult::TestFailed)
                            {
                                bShow = false;
                            }
                        }
                        if(bShow)
                        {
                            varData = pTestStep->getExpectedValues().join("\n");
                        }
                    }
                    else if(i_iRole == Qt::ToolTipRole)
                    {
                        varData = pTestStep->getToolTip();
                    }
                }
                break;
            }
            case EColumnResultValues:
            {
                if( pTestStep != nullptr )
                {
                    if(i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole)
                    {
                        QStringList strlstExpectedValues;
                        QStringList strlstResultValues;
                        QString     strExpectedVal;
                        QString     strResultVal;
                        int         idxVal;
                        bool        bShow = true;

                        if(m_bShowExpectedAndResultValuesOnlyIfTestStepFailed)
                        {
                            if(testResult != ETestResult::TestFailed)
                            {
                                bShow = false;
                            }
                        }
                        if(bShow)
                        {
                            strlstExpectedValues = pTestStep->getExpectedValues();
                            strlstResultValues = pTestStep->getResultValues();

                            for(idxVal = 0; idxVal < strlstResultValues.size(); idxVal++)
                            {
                                strResultVal = strlstResultValues[idxVal];

                                if(idxVal < strlstExpectedValues.size())
                                {
                                    strExpectedVal = strlstExpectedValues[idxVal];
                                }
                                else
                                {
                                    strExpectedVal = "";
                                }
                                if(strResultVal.compare(strExpectedVal, Qt::CaseSensitive) != 0)
                                {
                                    strlstResultValues[idxVal] = "!! " + strResultVal + " !!";
                                }
                            }
                            varData = strlstResultValues.join("\n");
                        }
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

    #ifdef ZS_TRACE_GUI_MODELS
    if(mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsVerbose))
    {
        mthTracer.setMethodReturn(varData.toString());
    }
    #endif

    return varData;

} // data
