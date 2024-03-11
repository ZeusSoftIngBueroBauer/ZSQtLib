/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class ZSDRAWDLL_API CObjFactory
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

const QString CObjFactory::c_strGroupSeparater = "::";
const QString CObjFactory::c_strGroupNameStandardShapes = "Standard Shapes";
const QString CObjFactory::c_strGroupNameConnections = "Connections";
const QString CObjFactory::c_strGroupNameSelectionPoints = "Selection Points";


/*==============================================================================
protected: // class members
==============================================================================*/

int CObjFactory::s_iInstCount = 0;
CIdxTree* CObjFactory::s_pIdxTree = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
int CObjFactory::GetObjFactoriesCount()
//------------------------------------------------------------------------------
{
    return s_iInstCount;
}

//------------------------------------------------------------------------------
CObjFactory* CObjFactory::GetObjFactory( int i_idx )
//------------------------------------------------------------------------------
{
    CObjFactory* pObjFactory = nullptr;
    if (s_pIdxTree != nullptr && i_idx >= 0 && i_idx < s_pIdxTree->treeEntriesVectorSize()) {
        pObjFactory = dynamic_cast<CObjFactory*>(s_pIdxTree->getEntry(i_idx));
    }
    return pObjFactory;
}

//------------------------------------------------------------------------------
CObjFactory* CObjFactory::FindObjFactory( const QString& i_strGraphObjPath )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CObjFactory*>(s_pIdxTree->findLeave(i_strGraphObjPath));
}

////------------------------------------------------------------------------------
//CObjFactory* CObjFactory::FindObjFactory(
//    const QString& i_strFactoryGroupName,
//    int            i_iGraphObjType )
////------------------------------------------------------------------------------
//{
//    CObjFactory* pObjFactory = nullptr;
//    if( s_pIdxTree != nullptr )
//    {
//        QString strPath = s_pIdxTree->buildPathStr(i_strFactoryGroupName, graphObjType2Str(i_iGraphObjType));
//        CIdxTreeEntry* pTreeEntry = s_pIdxTree->findLeave(strPath);
//        pObjFactory = dynamic_cast<CObjFactory*>(pTreeEntry);
//    }
//    return pObjFactory;
//}

//------------------------------------------------------------------------------
CObjFactory* CObjFactory::FindObjFactory(
    const QString& i_strFactoryGroupName,
    const QString& i_strGraphObjType )
//------------------------------------------------------------------------------
{
    CObjFactory* pObjFactory = nullptr;
    if (s_pIdxTree != nullptr) {
        QString strPath = s_pIdxTree->buildPathStr(i_strFactoryGroupName, i_strGraphObjType);
        CIdxTreeEntry* pTreeEntry = s_pIdxTree->findLeave(strPath);
        pObjFactory = dynamic_cast<CObjFactory*>(pTreeEntry);
    }
    return pObjFactory;
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates an instance of the object factory.

    @param i_strGroupName [in]
        To group the factory in index tree
        (e.g. "Draw::Standard Shapes", "Draw::Widgets", "Draw::Electricity").
        Use "c_strGroupSeparator" ("::") to separate groups.
    @param i_strGraphObjNameSpace [in]
        C++ name space of the class
        (e.g. "ZS::Draw", "ZS::Draw::Widgets", "ZS::Draw::Electricity").
        Used for tracing.
    @param i_strGraphObjClassName [in]
        C++ class name (e.g. "CGraphObjRect", "CGraphObjWdgtCheckBox", "CGraphObjCapacitor").
        Used for tracing.
    @param i_iGraphObjType [in]
        Type as int of the graphic items created by this factory.
    @param i_strGraphObjType [in]
        Type as string of the graphic items created by this factory
        (e.g. "Line", "Rect", "CheckBox", "Resistor").
        The type as string becomes the name of the index tree leave entry.
    @param i_toolIcon [in]
        Icon to indicate the graphic items created by this factory in the index tree.
*/
CObjFactory::CObjFactory(
    const QString& i_strGroupName,
    const QString& i_strClassName,
    EGraphObjType  i_eGraphObjType,
    const QString& i_strGraphObjType,
    const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    QObject(),
    CIdxTreeEntry(EEntryType::Leave, i_strGraphObjType),
    m_strGroupName(i_strGroupName),
    m_eGraphObjType(i_eGraphObjType),
    m_pxmToolIcon(i_pxmToolIcon),
    m_strFileName(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strGroupName + "::" + i_strGraphObjType);

    s_iInstCount++;

    if (s_pIdxTree == nullptr) {
        s_pIdxTree = new CIdxTree(
            /* strObjName       */ "DrawObjectFactories",
            /* pRootTreeEntry   */ nullptr,
            /* strNodeSeparator */ "::" );
    }

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::ObjFactories::" + i_strGroupName, i_strClassName);

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Group: " + i_strGroupName + ", Class: " + i_strClassName + 
                       ", ObjType: " + graphObjType2Str(i_eGraphObjType) + "(" + i_strGraphObjType + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    if (FindObjFactory(m_strGroupName, m_strName) != nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyRegistered);
    }
    s_pIdxTree->add(this, m_strGroupName);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactory::~CObjFactory()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    s_pIdxTree->remove(keyInTree());

    s_iInstCount--;

    if (s_iInstCount == 0) {
        try {
            delete s_pIdxTree;
        }
        catch (...) {
        }
        s_pIdxTree = nullptr;
    }

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_strGroupName;
    m_eGraphObjType = static_cast<EGraphObjType>(0);
    //m_toolIcon;
    //m_strFileDir;
    //m_strFileName;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjFactory::setToolIconPixmap( const QPixmap& i_pxm )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setToolIconPixmap",
        /* strAddInfo   */ "" );

    m_pxmToolIcon = i_pxm;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjFactory::setFileDir( const QString& i_strFileDir )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strFileDir;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFileDir",
        /* strAddInfo   */ strMthInArgs );

    m_strFileDir = i_strFileDir;
}

//------------------------------------------------------------------------------
void CObjFactory::setFileName( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strFileName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFileName",
        /* strAddInfo   */ strMthInArgs );

    QFileInfo fileInfo(i_strFileName);
    if (fileInfo.fileName() == i_strFileName) {
        m_strFileName = i_strFileName;
    }
    else {
        m_strFileDir = fileInfo.path();
        m_strFileName = fileInfo.fileName();
    }
}

//------------------------------------------------------------------------------
void CObjFactory::setFilePath( const QString& i_strFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "FilePath:" + i_strFilePath;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setFilePath",
        /* strAddInfo   */ strMthInArgs );

    QFileInfo fileInfo(i_strFilePath);
    if (fileInfo.fileName() == i_strFilePath) {
        m_strFileDir = fileInfo.path();
        m_strFileName = fileInfo.fileName();
    }
}

//------------------------------------------------------------------------------
QString CObjFactory::getFilePath() const
//------------------------------------------------------------------------------
{
    QString strFileDir = m_strFileDir;
    if (strFileDir.isEmpty()) {
        strFileDir = QCoreApplication::applicationDirPath();
    }
    QFileInfo fileInfo(strFileDir + QDir::separator() + m_strFileName);
    return fileInfo.filePath();
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CObjFactory::saveGraphObjTextLabels(
    CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "null" : i_pGraphObj->keyInTree());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "saveGraphObjTextLabels",
        /* strAddInfo   */ strMthInArgs );

    // The predefined labels should be at the beginning. And sorted by their names so that
    // the content of the file is always the same (hashes have an unpredicted order).
    QStringList strlstLabelNames = i_pGraphObj->getLabelNames();
    strlstLabelNames.sort();
    QStringList strlstPredefinedLabelNames = i_pGraphObj->getPredefinedLabelNames();
    QSet<QString> strlstLabelNamesAdded;
    for (const QString& strName : strlstPredefinedLabelNames) {
        SLabelDscr labelDscr = i_pGraphObj->getLabelDescriptor(strName);
        i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameLabel);
        // To keep the XML file as short as possible the properties of
        // the labels are stored as attributes and not as text elements.
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrKey, labelDscr.m_strKey);
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrText, labelDscr.m_strText);
        SGraphObjSelectionPoint selPt = labelDscr.m_selPt1;
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrSelPt, selPt.toString(false));
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrDistance, labelDscr.m_polarCoorsToLinkedSelPt.toString());
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrVisible, bool2Str(i_pGraphObj->isLabelVisible(strName)));
        if (labelDscr.m_bShowAnchorLine) { // don't write default for this property
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrAnchorLineVisible, bool2Str(labelDscr.m_bShowAnchorLine));
        }
        i_xmlStreamWriter.writeEndElement();
        strlstLabelNamesAdded.insert(strName);
    }
    // The user defined labels should follow the predefined labels.
    // Add those after the predefined labels.
    for (const QString& strName : strlstLabelNames) {
        if (!strlstLabelNamesAdded.contains(strName)) {
            SLabelDscr labelDscr = i_pGraphObj->getLabelDescriptor(strName);
            i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameLabel);
            // To keep the XML file as short as possible the properties of
            // the labels are stored as attributes and not as text elements.
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrKey, labelDscr.m_strKey);
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrText, labelDscr.m_strText);
            SGraphObjSelectionPoint selPt = labelDscr.m_selPt1;
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrSelPt, selPt.toString(false));
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrDistance, labelDscr.m_polarCoorsToLinkedSelPt.toString());
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrVisible, bool2Str(i_pGraphObj->isLabelVisible(strName)));
            if (labelDscr.m_bShowAnchorLine) { // don't write default for this property
                i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrAnchorLineVisible, bool2Str(labelDscr.m_bShowAnchorLine));
            }
            i_xmlStreamWriter.writeEndElement();
            strlstLabelNamesAdded.insert(strName);
        }
    }
}

//------------------------------------------------------------------------------
QList<SLabelDscr> CObjFactory::loadGraphObjTextLabels(QXmlStreamReader& i_xmlStreamReader) const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "loadGraphObjTextLabels",
        /* strAddInfo   */ "" );

    QList<SLabelDscr> arLabelDscrs;

    while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd()) {
        QXmlStreamReader::TokenType xmlStreamTokenType = i_xmlStreamReader.readNext();
        if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement()) {
            QString strElemName = i_xmlStreamReader.name().toString();
            if (i_xmlStreamReader.isStartElement()) {
                if (strElemName == XmlStreamParser::c_strXmlElemNameLabel) {
                    SLabelDscr labelDscr;
                    QString strAttr;
                    bool bConverted;

                    // To keep the XML file as short as possible the properties of
                    // the labels are stored as attributes and not as text elements.
                    QXmlStreamAttributes xmlStreamAttrs = i_xmlStreamReader.attributes();

                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrKey)) {
                        labelDscr.m_strKey = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrKey).toString();
                    }
                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrText)) {
                        labelDscr.m_strText = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrText).toString();
                    }
                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrSelPt)) {
                        strAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrSelPt).toString();
                        SGraphObjSelectionPoint selPtTmp = SGraphObjSelectionPoint::fromString(strAttr, &bConverted);
                        if (bConverted) {
                            labelDscr.m_selPt1 = selPtTmp;
                        }
                    }
                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrDistance)) {
                        strAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrDistance).toString();
                        SPolarCoors polarCoorsTmp = SPolarCoors::fromString(strAttr, "/", &bConverted);
                        if (bConverted) {
                            labelDscr.m_polarCoorsToLinkedSelPt = polarCoorsTmp;
                        }
                    }
                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrVisible)) {
                        strAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrVisible).toString();
                        labelDscr.m_bLabelIsVisible = str2Bool(strAttr);
                    }
                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrAnchorLineVisible)) {
                        strAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrAnchorLineVisible).toString();
                        labelDscr.m_bShowAnchorLine = str2Bool(strAttr);
                    }

                    if (!labelDscr.m_strKey.isEmpty()) {
                        arLabelDscrs.append(labelDscr);
                    }
                }
            }
            else /* if (i_xmlStreamReader.isEndElement())*/ {
                if (strElemName == XmlStreamParser::c_strXmlElemNameTextLabels) {
                    break;
                }
            }
        } // if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement())
    } // while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd())
    
    return arLabelDscrs;

} // loadGraphObjTextLabels

//------------------------------------------------------------------------------
void CObjFactory::saveGraphObjGeometryLabels(
    CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "null" : i_pGraphObj->keyInTree());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "saveGraphObjGeometryLabels",
        /* strAddInfo   */ strMthInArgs );

    QStringList strlstLabelNames = i_pGraphObj->getGeometryLabelNames();
    strlstLabelNames.sort();
    for (const QString& strName : strlstLabelNames) {
        SLabelDscr labelDscr = i_pGraphObj->getGeometryLabelDescriptor(strName);
        i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameLabel);
        // To keep the XML file as short as possible the properties of
        // the labels are stored as attributes and not as text elements.
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrKey, labelDscr.m_strKey);
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrText, labelDscr.m_strText);
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrDistance, labelDscr.m_polarCoorsToLinkedSelPt.toString());
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrVisible, bool2Str(i_pGraphObj->isGeometryLabelVisible(strName)));
        if (labelDscr.m_bShowAnchorLine) { // don't write default for this property
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrAnchorLineVisible, bool2Str(labelDscr.m_bShowAnchorLine));
        }
        i_xmlStreamWriter.writeEndElement();
    }
}

//------------------------------------------------------------------------------
QList<SLabelDscr> CObjFactory::loadGraphObjGeometryLabels(QXmlStreamReader& i_xmlStreamReader) const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "loadGraphObjGeometryLabels",
        /* strAddInfo   */ "" );

    QList<SLabelDscr> arLabelDscrs;

    while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd()) {
        QXmlStreamReader::TokenType xmlStreamTokenType = i_xmlStreamReader.readNext();
        if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement()) {
            QString strElemName = i_xmlStreamReader.name().toString();
            if (i_xmlStreamReader.isStartElement()) {
                if (strElemName == XmlStreamParser::c_strXmlElemNameLabel) {
                    SLabelDscr labelDscr;
                    QString strAttr;
                    bool bConverted;

                    // To keep the XML file as short as possible the properties of
                    // the labels are stored as attributes and not as text elements.
                    QXmlStreamAttributes xmlStreamAttrs = i_xmlStreamReader.attributes();

                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrKey)) {
                        labelDscr.m_strKey = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrKey).toString();
                    }
                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrDistance)) {
                        strAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrDistance).toString();
                        SPolarCoors polarCoorsTmp = SPolarCoors::fromString(strAttr, "/", &bConverted);
                        if (bConverted) {
                            labelDscr.m_polarCoorsToLinkedSelPt = polarCoorsTmp;
                        }
                    }
                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrVisible)) {
                        strAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrVisible).toString();
                        labelDscr.m_bLabelIsVisible = str2Bool(strAttr);
                    }
                    if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlAttrAnchorLineVisible)) {
                        strAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlAttrAnchorLineVisible).toString();
                        labelDscr.m_bShowAnchorLine = str2Bool(strAttr);
                    }
                    if (!labelDscr.m_strKey.isEmpty()) {
                        arLabelDscrs.append(labelDscr);
                    }
                }
            }
            else /* if (i_xmlStreamReader.isEndElement())*/ {
                if (strElemName == XmlStreamParser::c_strXmlElemNameGeometryLabels) {
                    break;
                }
            }
        } // if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement())
    } // while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd())

    return arLabelDscrs;

} // loadGraphObjGeometryLabels
