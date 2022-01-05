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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawAux.h"
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
using namespace ZS::Trace;


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

int       CObjFactory::s_iInstCount = 0;
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

    if( s_pIdxTree != nullptr && i_idx >= 0 && i_idx < s_pIdxTree->treeEntriesVectorSize() )
    {
        pObjFactory = dynamic_cast<CObjFactory*>(s_pIdxTree->getEntry(i_idx));
    }
    return pObjFactory;

} // GetObjFactory

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
//
//    if( s_pIdxTree != nullptr )
//    {
//        QString strPath = s_pIdxTree->buildPathStr(i_strFactoryGroupName, graphObjType2Str(i_iGraphObjType));
//        CIdxTreeEntry* pTreeEntry = s_pIdxTree->findLeave(strPath);
//        pObjFactory = dynamic_cast<CObjFactory*>(pTreeEntry);
//    }
//    return pObjFactory;
//
//} // FindObjFactory

//------------------------------------------------------------------------------
CObjFactory* CObjFactory::FindObjFactory(
    const QString& i_strFactoryGroupName,
    const QString& i_strGraphObjType )
//------------------------------------------------------------------------------
{
    CObjFactory* pObjFactory = nullptr;

    if( s_pIdxTree != nullptr )
    {
        QString strPath = s_pIdxTree->buildPathStr(i_strFactoryGroupName, i_strGraphObjType);
        CIdxTreeEntry* pTreeEntry = s_pIdxTree->findLeave(strPath);
        pObjFactory = dynamic_cast<CObjFactory*>(pTreeEntry);
    }
    return pObjFactory;

} // FindObjFactory

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
    int            i_iGraphObjType,
    const QString& i_strGraphObjType,
    const QIcon&   i_toolIcon ) :
//------------------------------------------------------------------------------
    QObject(),
    CIdxTreeEntry(EIdxTreeEntryType::Leave, i_strGraphObjType),
    m_strGroupName(i_strGroupName),
    m_iGraphObjType(i_iGraphObjType),
    m_toolIcon(i_toolIcon),
    m_strFileName(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strGroupName + "::" + i_strGraphObjType);

    s_iInstCount++;

    if( s_pIdxTree == nullptr )
    {
        s_pIdxTree = new CIdxTree(
            /* strObjName       */ "Graphical Object Factories",
            /* pRootTreeEntry   */ nullptr,
            /* strNodeSeparator */ "::" );
    }

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace() + c_strGroupSeparater + i_strGroupName, ClassName(), m_strName);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Group: " + i_strGroupName;
        strMthInArgs += "ObjType: " + i_strGraphObjType + "(" + QString::number(i_iGraphObjType) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    if( FindObjFactory(m_strGroupName, m_strName) != nullptr )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyRegistered);
    }

    s_pIdxTree->add(this, m_strGroupName);

} // default ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactory::~CObjFactory()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    s_pIdxTree->remove(keyInTree());

    s_iInstCount--;

    if( s_iInstCount == 0 )
    {
        try
        {
            delete s_pIdxTree;
        }
        catch(...)
        {
        }
        s_pIdxTree = nullptr;
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_strGroupName;
    m_iGraphObjType = 0;
    //m_toolIcon;
    //m_strFileDir;
    //m_strFileName;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjFactory::setToolIcon( const QIcon& i_toolIcon )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setToolIcon",
        /* strAddInfo   */ strAddTrcInfo );

    m_toolIcon = i_toolIcon;

} // setToolIcon

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjFactory::setFileDir( const QString& i_strFileDir )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "FileDir:" + i_strFileDir;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFileDir",
        /* strAddInfo   */ strAddTrcInfo );

    m_strFileDir = i_strFileDir;

} // setFileDir

//------------------------------------------------------------------------------
void CObjFactory::setFileName( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "FileName:" + i_strFileName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFileName",
        /* strAddInfo   */ strAddTrcInfo );

    QFileInfo fileInfo(i_strFileName);

    if( fileInfo.fileName() == i_strFileName )
    {
        m_strFileName = i_strFileName;
    }
    else
    {
        m_strFileDir = fileInfo.path();
        m_strFileName = fileInfo.fileName();
    }

} // setFileName

//------------------------------------------------------------------------------
void CObjFactory::setFilePath( const QString& i_strFilePath )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "FilePath:" + i_strFilePath;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFilePath",
        /* strAddInfo   */ strAddTrcInfo );

    QFileInfo fileInfo(i_strFilePath);

    if( fileInfo.fileName() == i_strFilePath )
    {
        m_strFileDir = fileInfo.path();
        m_strFileName = fileInfo.fileName();
    }

} // setFilePath

//------------------------------------------------------------------------------
QString CObjFactory::getFilePath() const
//------------------------------------------------------------------------------
{
    QString strFileDir = m_strFileDir;

    if( strFileDir.isEmpty() )
    {
        strFileDir = QCoreApplication::applicationDirPath();
    }

    QFileInfo fileInfo( strFileDir + QDir::separator() + m_strFileName );

    QString strFilePath = fileInfo.filePath();

    return strFilePath;

} // getFilePath

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CObjFactory::saveGraphObjLabels(
    const QHash<QString,SGraphObjLabel*>& i_arpLabels,
    QXmlStreamWriter&                     i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "saveGraphObjLabels",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    QHashIterator<QString,SGraphObjLabel*> itLabels(i_arpLabels);
    SGraphObjLabel*                        pGraphObjLabel;

    while( itLabels.hasNext() )
    {
        itLabels.next();

        pGraphObjLabel = itLabels.value();

        i_xmlStreamWriter.writeStartElement("Label");

        // To keep the XML file as short as possible the properties of
        // the labels are stored as attributes and not as text elements.
        i_xmlStreamWriter.writeAttribute( "Key", pGraphObjLabel->m_strKey );
        i_xmlStreamWriter.writeAttribute( "Text", pGraphObjLabel->m_strText );
        i_xmlStreamWriter.writeAttribute( "SelectionPoint", pGraphObjLabel->m_selPt.toString() );
        i_xmlStreamWriter.writeAttribute( "Distance", size2Str(pGraphObjLabel->m_sizDist) );
        i_xmlStreamWriter.writeAttribute( "Visible", bool2Str(pGraphObjLabel->m_bVisible) );

        i_xmlStreamWriter.writeEndElement();
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // saveGraphObjLabels

//------------------------------------------------------------------------------
QHash<QString,SGraphObjLabel*> CObjFactory::loadGraphObjLabels(
    QXmlStreamReader& i_xmlStreamReader,
    SErrResultInfo&   io_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "loadGraphObjLabels",
        /* strAddInfo   */ strAddTrcInfo );

    QHash<QString,SGraphObjLabel*> arpLabels;

    QXmlStreamAttributes xmlStreamAttrs;

    QString         strElemName;
    QString         strAttr;
    bool            bConverted;
    SGraphObjLabel* pGraphObjLabel = nullptr;

    while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
    {
        //xmlStreamTokenType = i_xmlStreamReader.readNext();
        strElemName = i_xmlStreamReader.name().toString();

        if( i_xmlStreamReader.isStartElement() )
        {
            if( strElemName == "Label" )
            {
                QString         strKey;
                QString         strText;
                ESelectionPoint selPt = ESelectionPoint::Undefined;
                QSizeF          sizDist;
                bool            bDistValid = false;
                bool            bVisible = false;

                // To keep the XML file as short as possible the properties of
                // the labels are stored as attributes and not as text elements.

                xmlStreamAttrs = i_xmlStreamReader.attributes();

                if( xmlStreamAttrs.hasAttribute("Key") )
                {
                    strKey = xmlStreamAttrs.value("Key").toString();
                }

                if( xmlStreamAttrs.hasAttribute("Text") )
                {
                    strText = xmlStreamAttrs.value("Text").toString();
                }

                if( xmlStreamAttrs.hasAttribute("SelectionPoint") )
                {
                    strAttr = xmlStreamAttrs.value("SelectionPoint").toString();
                    CEnumSelectionPoint selPtTmp = CEnumSelectionPoint::fromString(strAttr, &bConverted);
                    if( bConverted ) selPt = selPtTmp.enumerator();
                }

                if( xmlStreamAttrs.hasAttribute("Distance") )
                {
                    strAttr = xmlStreamAttrs.value("Distance").toString();
                    QSizeF sizTmp = str2SizeF(strAttr,&bConverted);
                    if( bConverted )
                    {
                        sizDist = sizTmp;
                        bDistValid = true;
                    }
                }

                if( xmlStreamAttrs.hasAttribute("Visible") )
                {
                    strAttr = xmlStreamAttrs.value("Visible").toString();
                    bVisible = str2Bool(strAttr);
                }

                if( !strKey.isEmpty() && !strText.isEmpty() && !arpLabels.contains(strKey) && selPt != ESelectionPoint::Undefined && bDistValid )
                {
                    if( pGraphObjLabel != nullptr )
                    {
                        if( arpLabels.contains(pGraphObjLabel->m_strKey) )
                        {
                            arpLabels.remove(pGraphObjLabel->m_strKey);
                        }
                        delete pGraphObjLabel;
                        pGraphObjLabel = nullptr;
                    }

                    pGraphObjLabel = new SGraphObjLabel(strKey,strText,selPt);

                    pGraphObjLabel->m_sizDist = sizDist;
                    pGraphObjLabel->m_bDistValid = bDistValid;
                    pGraphObjLabel->m_bVisible = bVisible;
                }
            } // if( strElemName == "Label" )
        } // if( i_xmlStreamReader.isStartElement() )

        else if( i_xmlStreamReader.isEndElement() )
        {
            if( strElemName == "Label" )
            {
                if( pGraphObjLabel != nullptr && !pGraphObjLabel->m_strKey.isEmpty() && !arpLabels.contains(pGraphObjLabel->m_strKey) )
                {
                    arpLabels.insert( pGraphObjLabel->m_strKey, pGraphObjLabel );
                    pGraphObjLabel = nullptr;
                }
            }
            else if( strElemName == "Labels" )
            {
                break;
            }

        } // if( i_xmlStreamReader.isEndElement() )
    } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(io_errResultInfo);
    }

    return arpLabels;

} // loadGraphObjLabels
