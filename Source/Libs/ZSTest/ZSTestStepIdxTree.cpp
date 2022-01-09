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
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepRoot.h"
#include "ZSTest/ZSTest.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Test;


/*******************************************************************************
class CTestStepIdxTree : public ZS::System::CIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepIdxTree::CTestStepIdxTree( CTest* i_pTest ) :
//------------------------------------------------------------------------------
    CIdxTree(
        /* strIdxTreeName   */ i_pTest->objectName() + "-TestSteps",
        /* pRootTreeEntry   */ new CTestStepRoot(i_pTest, i_pTest->objectName()),
        /* strNodeSeparator */ "\\",
        /* bCreateMutex     */ true,
        /* pObjParent       */ nullptr),
    m_pTest(i_pTest),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pTest->objectName() + "-TestSteps");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Test", "CTestStepIdxTree", objectName());

    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strMthArgs   */ strAddTrcInfo);

} // ctor

//------------------------------------------------------------------------------
CTestStepIdxTree::~CTestStepIdxTree()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strMthArgs   */ strAddTrcInfo);

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pTest = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods find test step groups
==============================================================================*/

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepIdxTree::findTestStepGroup(
    const QString& i_strParentPath,
    const QString& i_strGroupName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CTestStepGroup*>(findBranch(i_strParentPath, i_strGroupName));
}

//------------------------------------------------------------------------------
CTestStepGroup* CTestStepIdxTree::findTestStepGroup(
    CTestStepGroup* i_pTestGroupParent,
    const QString&  i_strGroupName )
//------------------------------------------------------------------------------
{
    QString strParentPath;

    if( i_pTestGroupParent != nullptr )
    {
        strParentPath = i_pTestGroupParent->path();
    }
    return findTestStepGroup(strParentPath, i_strGroupName);

} // findTestStepGroup

/*==============================================================================
public: // instance methods to find test steps
==============================================================================*/

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::findTestStep(
    const QString& i_strName,
    const QString& i_strParentPath )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CTestStep*>(findLeave(i_strParentPath, i_strName));
}

//------------------------------------------------------------------------------
CTestStep* CTestStepIdxTree::findTestStep(
    CTestStepGroup* i_pTestGroupParent,
    const QString&  i_strName )
//------------------------------------------------------------------------------
{
    QString strParentPath;

    if( i_pTestGroupParent != nullptr )
    {
        strParentPath = i_pTestGroupParent->path();
    }
    return findTestStep(i_strName, strParentPath);

} // findTestStep

/*==============================================================================
public: // overridables of base class CModelIdxTree
==============================================================================*/

//------------------------------------------------------------------------------
/*! Recursively resets the test results of all test step groups and test steps.
*/
void CTestStepIdxTree::reset()
//------------------------------------------------------------------------------
{
    reset(m_pRoot);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Helper method to recursively reset the test results of all test step groups
    and test steps below the specified node.

    @param i_pTreeEntry Tree entry to be reset together with all its children.
*/
void CTestStepIdxTree::reset( CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    // But this is a protected method called by the public reset method
    // and the mutex has already been locked.

    CIdxTreeEntry* pTreeEntry;
    int            idxEntry;

    for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )
    {
        pTreeEntry = i_pTreeEntry->at(idxEntry);

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->isLeave() )
            {
                CTestStep* pTestStep = dynamic_cast<CTestStep*>(pTreeEntry);
                pTestStep->reset();
            }
            else // if( !pTreeEntry->isLeave() )
            {
                reset(pTreeEntry);
            }
        } // if(pTreeEntry != nullptr)
    } // for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )

} // reset

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Saves the settings of the test steps and test step groups in an XML file.

    The attributes (enabled, breakpoints) will be save so that they can be
    recalled on restarting the test.

    Please note that only the settings for those index tree entries may be recalled
    which again will be found on recalling the test tree.

    @param i_strAbsFilePath [in] Absolute path name of the test steps file.
*/
SErrResultInfo CTestStepIdxTree::save( const QString& i_strAbsFilePath ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "save",
        /* strMthInArgs */ strMthInArgs );

    QString strMth = "save";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QFile file;

    if( i_strAbsFilePath.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else
    {
        QFileInfo fileInfo(i_strAbsFilePath);
        QDir      dir = fileInfo.absoluteDir();

        if( !dir.exists() )
        {
            dir.mkpath(dir.absolutePath());
        }

        file.setFileName(i_strAbsFilePath);

        if( !file.open(QIODevice::WriteOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForWrite);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QByteArray byteArrSchema;

        QXmlStreamWriter xmlStreamWriter(&byteArrSchema);

        xmlStreamWriter.setAutoFormatting(true);

        xmlStreamWriter.writeStartDocument();
        xmlStreamWriter.writeStartElement("Test");
        xmlStreamWriter.writeAttribute( "Name", m_pRoot->name() );

        save(xmlStreamWriter, m_pRoot);

        xmlStreamWriter.writeEndElement();  // Method Trace Admin Objects
        xmlStreamWriter.writeEndDocument();

        if( !errResultInfo.isErrorResult() )
        {
            file.write(byteArrSchema);
        }
    } // if( !errResultInfo.isErrorResult() )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // save

//------------------------------------------------------------------------------
/*! Recalls the settings of the test steps and test step groups from an XML file.

    The attributes (enabled, breakpoints) will be recalled so that they can be
    reused on restarting the test.

    Please note that only the settings for those index tree entries may be recalled
    which will still be found in the test tree. Test steps and groups which will
    not be found will be ignored.

    @param i_strAbsFilePath [in] Absolute path name of the test steps file.
*/
SErrResultInfo CTestStepIdxTree::recall( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod       */ "recall",
        /* strMthInArgs    */ strMthInArgs );

    QString strMth = "recall";

    QString strAddErrInfo;

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QFile file;

    if( i_strAbsFilePath.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else
    {
        file.setFileName(i_strAbsFilePath);

        if( !file.open(QIODevice::ReadOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForRead);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QXmlStreamReader            xmlStreamReader(&file);
        QXmlStreamReader::TokenType xmlStreamTokenType;

        SErrResultInfo errResultInfoTmp = ErrResultInfoSuccess(strMth);

        CTestStepGroup* pTSGrp = nullptr;
        CTestStep*      pTestStep = nullptr;

        QString  strElemName;
        QString  strName;
        QString  strVal;
        EEnabled enabled = EEnabled::No;
        bool     bVal = false;
        bool     bConverted = false;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML document");
        }

        xmlStreamTokenType = xmlStreamReader.readNext();
        strElemName = xmlStreamReader.name().toString();

        if( xmlStreamTokenType != QXmlStreamReader::StartElement || strElemName != "Test" )
        {
            xmlStreamReader.raiseError("Invalid XML document (missing root node \"Test\")");
        }

        if( !xmlStreamReader.hasError() )
        {
            while( !xmlStreamReader.atEnd() )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();

                if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
                {
                    strElemName = xmlStreamReader.name().toString();

                    // For best performance start with the most frequently used element names ..

                    //---------------------------------------
                    if( strElemName == "TestStepGroup" )
                    //---------------------------------------
                    {
                        if( xmlStreamTokenType == QXmlStreamReader::StartElement )
                        {
                            strName = xmlStreamReader.attributes().value("Name").toString();

                            if( strName.isEmpty() )
                            {
                                strAddErrInfo = "Attribute \"Name\" for \"" + strElemName + "\" not defined";
                                xmlStreamReader.raiseError(strAddErrInfo);
                            }
                            else // if( !strName.isEmpty() )
                            {
                                CTestStepGroup* pTSGrpTmp = findTestStepGroup(pTSGrp, strName);

                                if( pTSGrpTmp != nullptr )
                                {
                                    pTSGrp = pTSGrpTmp;

                                    if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                                    {
                                        strVal = xmlStreamReader.attributes().value("Enabled").toString();
                                        enabled = CEnumEnabled::toEnumerator(strVal);
                                        if( enabled == EEnabled::Undefined )
                                        {
                                            strAddErrInfo = "Attribute \"Enabled\" for " + strElemName + " \"" + pTSGrp->path() + "\" is out of range.";
                                            errResultInfoTmp.setSeverity(EResultSeverityError);
                                            errResultInfoTmp.setResult(EResultValueOutOfRange);
                                            errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                            if( CErrLog::GetInstance() != nullptr )
                                            {
                                                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                            }
                                        }
                                        else
                                        {
                                            pTSGrp->setEnabled(enabled);
                                        }
                                    }
                                } // if( pTSGrpTmp != nullptr )
                            } // if( !strName.isEmpty() )
                        } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )

                        else if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                        {
                            if( pTSGrp != nullptr )
                            {
                                pTSGrp = pTSGrp->getParentGroup();
                            }
                        } // if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                    } // if( strElemName == "TestStepGroup" )

                    //---------------------------------------
                    else if( strElemName == "TestStep" )
                    //---------------------------------------
                    {
                        if( xmlStreamTokenType == QXmlStreamReader::StartElement )
                        {
                            strName = xmlStreamReader.attributes().value("Name").toString();

                            if( strName.isEmpty() )
                            {
                                strAddErrInfo = "Attribute \"Name\" for \"" + strElemName + "\" not defined";
                                xmlStreamReader.raiseError(strAddErrInfo);
                            }
                            else // if( !strName.isEmpty() )
                            {
                                pTestStep = findTestStep(pTSGrp, strName);

                                if( pTestStep != nullptr )
                                {
                                    if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                                    {
                                        strVal = xmlStreamReader.attributes().value("Enabled").toString();
                                        enabled = CEnumEnabled::toEnumerator(strVal);
                                        if( enabled == EEnabled::Undefined )
                                        {
                                            strAddErrInfo = "Attribute \"Enabled\" for " + strElemName + " \"" + pTestStep->path() + "\" is out of range.";
                                            errResultInfoTmp.setSeverity(EResultSeverityError);
                                            errResultInfoTmp.setResult(EResultValueOutOfRange);
                                            errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                            if( CErrLog::GetInstance() != nullptr )
                                            {
                                                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                            }
                                        }
                                        else
                                        {
                                            pTestStep->setEnabled(enabled);
                                        }
                                    }
                                    if( xmlStreamReader.attributes().hasAttribute("Breakpoint") )
                                    {
                                        strVal = xmlStreamReader.attributes().value("Breakpoint").toString();
                                        bVal = str2Bool(strVal, &bConverted);
                                        if( !bConverted )
                                        {
                                            strAddErrInfo = "Attribute \"Breakpoint\" for " + strElemName + " \"" + pTestStep->path() + "\" is out of range.";
                                            errResultInfoTmp.setSeverity(EResultSeverityError);
                                            errResultInfoTmp.setResult(EResultValueOutOfRange);
                                            errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                            if( CErrLog::GetInstance() != nullptr )
                                            {
                                                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                            }
                                        }
                                        else if( !bVal )
                                        {
                                            pTestStep->removeBreakpoint();
                                        }
                                        else // if( bVal )
                                        {
                                            pTestStep->setBreakpoint();

                                            if( xmlStreamReader.attributes().hasAttribute("BreakpointEnabled") )
                                            {
                                                strVal = xmlStreamReader.attributes().value("BreakpointEnabled").toString();
                                                bVal = str2Bool(strVal, &bConverted);
                                                if( !bConverted )
                                                {
                                                    strAddErrInfo = "Attribute \"BreakpointEnabled\" for " + strElemName + " \"" + pTestStep->path() + "\" is out of range.";
                                                    errResultInfoTmp.setSeverity(EResultSeverityError);
                                                    errResultInfoTmp.setResult(EResultValueOutOfRange);
                                                    errResultInfoTmp.setAddErrInfoDscr(strAddErrInfo);
                                                    if( CErrLog::GetInstance() != nullptr )
                                                    {
                                                        CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                                                    }
                                                }
                                                else if( bVal )
                                                {
                                                    pTestStep->disableBreakpoint();
                                                }
                                                else
                                                {
                                                    pTestStep->enableBreakpoint();
                                                }
                                            }
                                        } // if( bVal )
                                    } // if( xmlStreamReader.attributes().hasAttribute("Breakpoint") )
                                } // if( pTestStep != nullptr )
                            } // if( !strName.isEmpty() )
                        } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )

                        else if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                        {
                            pTestStep = nullptr;

                        } // if (xmlStreamTokenType == QXmlStreamReader::EndElement)
                    } // if( strElemName == "TestStep" )

                    //---------------------------------------
                    else if( strElemName == "Test" )
                    //---------------------------------------
                    {
                        if( xmlStreamTokenType != QXmlStreamReader::EndElement )
                        {
                            xmlStreamReader.raiseError("Invalid element name \"" + strElemName + "\"");
                        }
                    }

                    //---------------------------------------
                    else
                    //---------------------------------------
                    {
                        xmlStreamReader.raiseError("Invalid element name \"" + strElemName + "\"");
                    }
                } // if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
            } // while( !xmlStreamReader.atEnd() )
        } // if( !xmlStreamReader.hasError() )

        if( xmlStreamReader.hasError() )
        {
            strAddErrInfo  = xmlStreamReader.errorString() + " on reading " + i_strAbsFilePath + " ";
            strAddErrInfo += "Line:" + QString::number(xmlStreamReader.lineNumber()) + ", ";
            strAddErrInfo += "Column:" + QString::number(xmlStreamReader.columnNumber());

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( !errResultInfo.isErrorResult() )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // recall

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestStepIdxTree::save(
    QXmlStreamWriter& i_xmlStreamWriter,
    CIdxTreeEntry*    i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    CIdxTreeEntry* pTreeEntry;
    int            idxEntry;

    if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
    {
        CTestStep* pTestStep = dynamic_cast<CTestStep*>(i_pTreeEntry);

        i_xmlStreamWriter.writeStartElement("TestStep");
        i_xmlStreamWriter.writeAttribute( "Name", pTestStep->name() );
        i_xmlStreamWriter.writeAttribute( "Enabled", CEnumEnabled::toString(pTestStep->getEnabled()) );
        i_xmlStreamWriter.writeAttribute( "Breakpoint", bool2Str(pTestStep->isBreakpointSet()) );
        if( pTestStep->isBreakpointSet() )
        {
            i_xmlStreamWriter.writeAttribute( "BreakpointEnabled", bool2Str(pTestStep->isBreakpointEnabled()) );
        }
        i_xmlStreamWriter.writeEndElement(/*"TestStep"*/);
    }
    else if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    {
        CTestStepGroup* pTestStepGroup = dynamic_cast<CTestStepGroup*>(i_pTreeEntry);

        i_xmlStreamWriter.writeStartElement("TestStepGroup");
        i_xmlStreamWriter.writeAttribute( "Name", pTestStepGroup->name() );
        i_xmlStreamWriter.writeAttribute( "Enabled", CEnumEnabled::toString(pTestStepGroup->getEnabled()) );

        for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )
        {
            pTreeEntry = i_pTreeEntry->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                save(i_xmlStreamWriter, pTreeEntry);
            }
        }
        i_xmlStreamWriter.writeEndElement(/*"TestStepGroup"*/);
    }
    else // if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Root )
    {
        for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )
        {
            pTreeEntry = i_pTreeEntry->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                save(i_xmlStreamWriter, pTreeEntry);
            }
        }
    }

} // save
