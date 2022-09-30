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

#include <QtCore/qmutex.h>
#include <QtCore/qdir.h>
#include <QtCore/qthread.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
class CIdxTreeLoggers : public ZS::System::CIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreeLoggers::CIdxTreeLoggers( const QString& i_strObjName, QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    CIdxTree(
        /* strIdxTreeName   */ i_strObjName,
        /* pRootTreeEntry   */ nullptr,
        /* strNodeSeparator */ "::",
        /* bCreateMutex     */ true,
        /* pObjParent       */ i_pObjParent )
{
}

//------------------------------------------------------------------------------
CIdxTreeLoggers::~CIdxTreeLoggers()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // instance methods to get and release admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the logger with the given name space, class name and object name.

    If a logger with the given name space, class name and object name is not yet
    existing in the index tree a new object is created and added to the index tree.

    If a logger with the given name space, class name and object name is already
    existing a reference counter will be incremented and the already existing
    logger is returned.

    @param i_strName [in] Name of the logger.
        The name may contain node separators and will be used to build the path
        to the tree entry. The last section of the name becomes the name of the
        leave entry.
    @param i_bEnabledAsDefault [in] Range [Yes, No, Undefined]
        If not Undefined the enabled flag will be set as the logger.
    @param i_eDefaultDetailLevel [in]
        If not Undefined the detail level will be set at the logger.
    @param i_strDefaultDataFilter [in]
        If not null (QString()) the data filter will be set at the logger.

    @return Pointer to allocated logger or nullptr on error.
*/
CLogger* CIdxTreeLoggers::getLogger(
    const QString&  i_strName,
    EEnabled        i_bEnabledAsDefault,
    ELogDetailLevel i_eDefaultDetailLevel,
    const QString&  i_strDefaultDataFilter )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CLogger* pLogger = nullptr;

    if( i_strName.isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "getLogger",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "No name defined");
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else
    {
        QString strParentBranchPath;
        QString strLeaveName;

        splitPathStr(i_strName, &strParentBranchPath, &strLeaveName);

        CIdxTreeEntry* pLeave = findLeave(strParentBranchPath, strLeaveName);

        if( pLeave != nullptr )
        {
            pLogger = dynamic_cast<CLogger*>(pLeave);
        }
        else // if( pLeave == nullptr )
        {
            if( !strParentBranchPath.isEmpty() )
            {
                QStringList strlstBranchNames = strParentBranchPath.split(m_strNodeSeparator);
                QString strParentPathTmp;
                QString strPathTmp;

                CIdxTreeEntry* pBranch;

                for( auto& strBranchName : strlstBranchNames )
                {
                    strPathTmp = buildPathStr(strPathTmp, strBranchName);

                    pBranch = findBranch(strPathTmp);

                    if( pBranch == nullptr )
                    {
                        pBranch = createBranch(strBranchName);
                        add(pBranch, strParentPathTmp);
                    }
                    strParentPathTmp = buildPathStr(strParentPathTmp, strBranchName);
                }
            }

            pLogger = new CLogger(strLeaveName);

            EEnabled bEnabled = EEnabled::Yes;
            ELogDetailLevel eDetailLevel = ELogDetailLevel::None;
            QString strDataFilter = "";

            if( i_bEnabledAsDefault != EEnabled::Undefined )
            {
                bEnabled = i_bEnabledAsDefault;
            }
            if( i_eDefaultDetailLevel != ELogDetailLevel::Undefined )
            {
                eDetailLevel = i_eDefaultDetailLevel;
            }
            if( !i_strDefaultDataFilter.isNull() )
            {
                strDataFilter = i_strDefaultDataFilter;
            }

            pLogger->setEnabled(bEnabled);
            pLogger->setLogLevel(eDetailLevel);
            pLogger->setDataFilter(strDataFilter);

            add(pLogger, strParentBranchPath);

        } // if( pLeave == nullptr )
    } // if( !i_strObjName.isEmpty() || !i_strClassName.isEmpty() || !i_strNameSpace.isEmpty() )

    return pLogger;

} // getLogger

//------------------------------------------------------------------------------
/*! @brief Returns the logger at the given tree index.

    @param i_idxInTree [in] Index of the logger.

    @return Pointer to allocated logger or nullptr on error.
*/
CLogger* CIdxTreeLoggers::getLogger( int i_idxInTree )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return dynamic_cast<CLogger*>(getEntry(i_idxInTree));
}

/*==============================================================================
public: // instance methods to insert branch nodes and logger leaves
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method is used by log clients to insert a branch at the given
           index in the tree.

    The branch becomes a child of the branch given by parameter i_iParentBranchIdxInTree.

    @param i_iParentBranchIdxInTree [in] Tree entry index of the parent entry.
    @param i_strBranchName [in] Branch name to be created.
    @param i_idxInTree [in] Tree entry index at which the new branch will be inserted.

    @return Pointer to allocated logger or nullptr on error.
*/
CIdxTreeEntry* CIdxTreeLoggers::insertBranch(
    int            i_iParentBranchIdxInTree,
    const QString& i_strBranchName,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pBranch = nullptr;

    QString strMth = "insertBranch";
    QString strAddErrInfo;

    CIdxTreeEntry* pBranchParent = nullptr;

    if( i_iParentBranchIdxInTree >= 0 )
    {
        pBranchParent = getEntry(i_iParentBranchIdxInTree);

        if( pBranchParent == nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add branch " + i_strBranchName + " with tree index " + QString::number(i_idxInTree);
                strAddErrInfo += " as there is no parent branch at the specified parent branch tree index " + QString::number(i_iParentBranchIdxInTree);
                SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultObjNotInList, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    }
    else // if( i_iParentBranchIdxInTree < 0 )
    {
        pBranchParent = m_pRoot;
    }

    if( i_idxInTree >= 0 )
    {
        CIdxTreeEntry* pTreeEntry = getEntry(i_idxInTree);

        // If there is already a tree entry at the given index ..
        if( pTreeEntry != nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add branch " + i_strBranchName + " with tree index " + QString::number(i_idxInTree);
                strAddErrInfo += " as the tree index is already occupied by " + pTreeEntry->keyInTree();
                SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultObjAlreadyInList, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        // If there is no tree entry at the given index ..
        else // if( pTreeEntry == nullptr )
        {
             pBranch = createBranch(i_strBranchName);
             insert(pBranch, pBranchParent, -1, i_idxInTree);
        }
    } // if( i_idxInTree >= 0 )

    return pBranch;

} // insertBranch

//------------------------------------------------------------------------------
/*! @brief This method is used by log clients to insert a logger at the given
           index in the tree.

    If there is no logger at the given index a new object is created with the
    given name space, class name and object name and inserted in the index
    tree at the index given by the object id.

    If there is already an object at the given index with a different name space,
    class name or object name nullptr is returned and an err log entry is created.

    As this method is used by log clients the reference counter of the logger
    is !! NOT !! incremented.

    The logger becomes a child of the branch given by parameter
    i_iParentBranchIdxInTree.

    @param i_iParentBranchIdxInTree [in] Tree entry index of the parent entry.
    @param i_strLeaveName [in] Leave name of the logger.

    @return Pointer to allocated logger or nullptr on error.
*/
CLogger* CIdxTreeLoggers::insertLogger(
    int            i_iParentBranchIdxInTree,
    const QString& i_strLeaveName,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CLogger* pLogger = nullptr;

    QString strMth = "insertLogger";
    QString strAddErrInfo;

    CIdxTreeEntry* pBranchParent = nullptr;

    if( i_iParentBranchIdxInTree >= 0 )
    {
        pBranchParent = getEntry(i_iParentBranchIdxInTree);

        if( pBranchParent == nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add logger " + i_strLeaveName + " with tree index " + QString::number(i_idxInTree);
                strAddErrInfo += " as there is no parent branch at the specified parent branch tree index " + QString::number(i_iParentBranchIdxInTree);
                SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultObjNotInList, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    }
    else // if( i_iParentBranchIdxInTree < 0 )
    {
        pBranchParent = m_pRoot;
    }

    if( i_idxInTree >= 0 )
    {
        CIdxTreeEntry* pTreeEntry = getEntry(i_idxInTree);

        // If there is already a tree entry at the given index ..
        if( pTreeEntry != nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add logger " + i_strLeaveName + " with tree index " + QString::number(i_idxInTree);
                strAddErrInfo += " as the tree index is already occupied by " + pTreeEntry->keyInTree();
                SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultObjAlreadyInList, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        // If there is no tree entry at the given index ..
        else // if( pTreeEntry == nullptr )
        {
            pLogger = new CLogger(i_strLeaveName);
            insert(pLogger, pBranchParent, -1, i_idxInTree);
        }
    } // if( i_idxInTree >= 0 )

    return pLogger;

} // insertLogger

/*==============================================================================
public: // instance methods to recursively modify loggers via index of node entries
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setEnabled( int i_iIdxInTree, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iIdxInTree);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setEnabled(pTreeEntry, i_enabled);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

            if( pLogger != nullptr )
            {
                pLogger->setEnabled(i_enabled);
            }
        }
    }
}

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setDetailLevel(
    int i_iIdxInTree, ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iIdxInTree);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setDetailLevel(pTreeEntry, i_eDetailLevel);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

            if( pLogger != nullptr )
            {
                pLogger->setLogLevel(i_eDetailLevel);
            }
        }
    }
}

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setDataFilter(
    int i_iIdxInTree, const QString& i_strDataFilter )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iIdxInTree);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setDataFilter(pTreeEntry, i_strDataFilter);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

            if( pLogger != nullptr )
            {
                pLogger->setDataFilter(i_strDataFilter);
            }
        }
    }
}

/*==============================================================================
public: // instance methods to recursively modify loggers via namespace node entries
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setEnabled( CIdxTreeEntry* i_pBranch, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CLogger*       pLogger;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                    if( pLogger != nullptr )
                    {
                        if( pLogger->getEnabled() != i_enabled )
                        {
                            pLogger->setEnabled(i_enabled);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setEnabled(pTreeEntry, i_enabled );
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )
}

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setDetailLevel(
    CIdxTreeEntry* i_pBranch, ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CLogger*       pLogger;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                    if( pLogger != nullptr )
                    {
                        if( pLogger->getLogLevel() != i_eDetailLevel )
                        {
                            pLogger->setLogLevel(i_eDetailLevel);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setDetailLevel(pTreeEntry, i_eDetailLevel);
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )
}

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setDataFilter(
    CIdxTreeEntry* i_pBranch, const QString& i_strDataFilter )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CLogger*       pLogger;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                    if( pLogger != nullptr )
                    {
                        if( pLogger->getDataFilter() != i_strDataFilter )
                        {
                            pLogger->setDataFilter(i_strDataFilter);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setDataFilter(pTreeEntry, i_strDataFilter);
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )
} // setTraceDataFilter

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CIdxTreeLoggers::save( const QString& i_strAbsFilePath ) const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
        xmlStreamWriter.writeStartElement("Loggers");

        save(xmlStreamWriter, m_pRoot);

        xmlStreamWriter.writeEndElement();  // Loggers
        xmlStreamWriter.writeEndDocument();

        if( !errResultInfo.isErrorResult() )
        {
            file.write(byteArrSchema);
        }
    } // if( !errResultInfo.isErrorResult() )

    return ErrResultSuccess;
}

//------------------------------------------------------------------------------
SErrResultInfo CIdxTreeLoggers::recall( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
        QXmlStreamReader xmlStreamReader(&file);

        QXmlStreamReader::TokenType xmlStreamTokenType;

        QString  strElemName;
        QString  strAttr;
        bool     bVal;
        bool     bOk;
        QString  strName;
        EEnabled enabled;
        QString  strDataFilter;
        bool     bAddThreadName;
        bool     bAddDateTime;
        bool     bAddSystemTime;
        QString  strNameSpace;
        QString  strClassName;
        QString  strObjName;
        ELogDetailLevel eDetailLevel;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML document");
        }

        xmlStreamTokenType = xmlStreamReader.readNext();
        strElemName = xmlStreamReader.name().toString();

        if( xmlStreamTokenType != QXmlStreamReader::StartElement || strElemName != "Loggers" )
        {
            xmlStreamReader.raiseError("Invalid XML logger objects file (missing root node \"Loggers\")");
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

                    if( strElemName == "Logger" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strName = "";
                            enabled = EEnabled::Yes;
                            eDetailLevel = ELogDetailLevel::None;
                            strDataFilter = "";
                            bAddThreadName = false;
                            bAddDateTime = false;
                            bAddSystemTime = false;
                            strNameSpace = "";
                            strClassName = "";
                            strObjName = "";

                            if( xmlStreamReader.attributes().hasAttribute("Name") )
                            {
                                strName = xmlStreamReader.attributes().value("Name").toString();
                            }
                            if( strName.isEmpty() )
                            {
                                xmlStreamReader.raiseError("Name not defined");
                            }
                            else
                            {
                                if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                                    enabled = CEnumEnabled::toEnumerator(strAttr, &bOk);
                                    if( !bOk ) {
                                        enabled = EEnabled::Yes;
                                        xmlStreamReader.raiseError(
                                            "Attribute \"Enabled\" (" + strAttr + ") for \"" +
                                            strName + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("LogLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("LogLevel").toString();
                                    eDetailLevel = CEnumLogDetailLevel::toEnumerator(strAttr, &bOk);
                                    if( !bOk ) {
                                        eDetailLevel = ELogDetailLevel::None;
                                        xmlStreamReader.raiseError(
                                            "Attribute \"LogLevel\" (" + strAttr + ") for \"" +
                                            strName + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("DataFilter") )
                                {
                                    strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                                }
                                if( xmlStreamReader.attributes().hasAttribute("AddThreadName") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("AddThreadName").toString();
                                    bVal = str2Bool(strAttr, &bOk);
                                    if( bOk ) {
                                        bAddThreadName = bVal;
                                    }
                                    else {
                                        xmlStreamReader.raiseError(
                                            "Attribute \"AddThreadName\" (" + strAttr + ") for \""
                                            + strName + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("AddDateTime") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("AddDateTime").toString();
                                    bVal = str2Bool(strAttr, &bOk);
                                    if( bOk ) {
                                        bAddDateTime = bVal;
                                    }
                                    else {
                                        xmlStreamReader.raiseError(
                                            "Attribute \"AddDateTime\" (" + strAttr + ") for \"" +
                                            strName + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("AddSystemTime") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("AddSystemTime").toString();
                                    bVal = str2Bool(strAttr, &bOk);
                                    if( bOk ) {
                                        bAddSystemTime = bVal;
                                    }
                                    else {
                                        xmlStreamReader.raiseError(
                                            "Attribute \"AddSystemTime\" (" + strAttr + ") for \"" +
                                            strName + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("NameSpace") )
                                {
                                    strNameSpace = xmlStreamReader.attributes().value("NameSpace").toString();
                                }
                                if( xmlStreamReader.attributes().hasAttribute("ClassName") )
                                {
                                    strClassName = xmlStreamReader.attributes().value("ClassName").toString();
                                }
                                if( xmlStreamReader.attributes().hasAttribute("ObjName") )
                                {
                                    strObjName = xmlStreamReader.attributes().value("ObjName").toString();
                                }

                                CLogger* pLogger = getLogger(strName, enabled, eDetailLevel, strDataFilter);

                                if( pLogger != nullptr )
                                {
                                    pLogger->setAddThreadName(bAddThreadName);
                                    pLogger->setAddDateTime(bAddDateTime);
                                    pLogger->setAddSystemTime(bAddSystemTime);
                                    pLogger->setNameSpace(strNameSpace);
                                    pLogger->setClassName(strClassName);
                                    pLogger->setObjectName(strObjName);
                                }
                            }
                        } // if( xmlStreamReader.isStartElement() )
                    } // if( strElemName == "Logger" )
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

    return errResultInfo;

} // recall

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeLoggers::save(
    QXmlStreamWriter& i_xmlStreamWriter,
    CIdxTreeEntry*    i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    CIdxTreeEntry* pTreeEntry;
    CLogger*       pLogger;
    int            idxEntry;

    if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
    {
        pLogger = dynamic_cast<CLogger*>(i_pTreeEntry);

        i_xmlStreamWriter.writeStartElement("Logger");
        i_xmlStreamWriter.writeAttribute( "Name", pLogger->path() );
        i_xmlStreamWriter.writeAttribute( "Enabled", CEnumEnabled::toString(pLogger->getEnabled()) );
        i_xmlStreamWriter.writeAttribute( "LogLevel", CEnumLogDetailLevel::toString(pLogger->getLogLevel()) );
        i_xmlStreamWriter.writeAttribute( "DataFilter", pLogger->getDataFilter() );
        i_xmlStreamWriter.writeAttribute( "AddThreadName", bool2Str(pLogger->addThreadName()) );
        i_xmlStreamWriter.writeAttribute( "AddDateTime", bool2Str(pLogger->addDateTime()) );
        i_xmlStreamWriter.writeAttribute( "AddSystemTime", bool2Str(pLogger->addSystemTime()) );
        i_xmlStreamWriter.writeAttribute( "NameSpace", pLogger->getNameSpace() );
        i_xmlStreamWriter.writeAttribute( "ClassName", pLogger->getClassName() );
        i_xmlStreamWriter.writeAttribute( "ObjName", pLogger->getObjectName() );
        i_xmlStreamWriter.writeEndElement(/*"Logger"*/);
    }
    else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
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
