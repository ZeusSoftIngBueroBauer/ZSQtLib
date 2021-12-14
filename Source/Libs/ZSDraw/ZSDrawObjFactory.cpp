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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSDraw/ZSDrawGraphObj.h"
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

/*==============================================================================
protected: // class members
==============================================================================*/

int           CObjFactory::s_iInstCount = 0;
CIdxTree*     CObjFactory::s_pIdxTreeAllFactories = nullptr;
CIdxTree*     CObjFactory::s_pIdxTreeToolBoxFactories = nullptr;
CTrcAdminObj* CObjFactory::s_pTrcAdminObj = nullptr;

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

    if( s_pIdxTreeAllFactories != nullptr && i_idx >= 0 && i_idx < s_pIdxTreeAllFactories->treeEntriesVectorSize() )
    {
        pObjFactory = dynamic_cast<CObjFactory*>(s_pIdxTreeAllFactories->getEntry(i_idx));
    }
    return pObjFactory;

} // GetObjFactory

//------------------------------------------------------------------------------
CObjFactory* CObjFactory::FindObjFactory( const QString& i_strGraphObjPath )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CObjFactory*>(s_pIdxTreeAllFactories->findLeave(i_strGraphObjPath));
}

//------------------------------------------------------------------------------
CObjFactory* CObjFactory::FindObjFactory(
    const QString& i_strGraphObjNameSpace,
    const QString& i_strGraphObjClassName,
    int            i_iGraphObjType )
//------------------------------------------------------------------------------
{
    CObjFactory* pObjFactory = nullptr;
    CObjFactory* pObjFactoryTmp;
    int          idx;

    if( s_pIdxTreeAllFactories != nullptr )
    {
        for( idx = 0; idx < s_pIdxTreeAllFactories->treeEntriesVectorSize(); idx++ )
        {
            CIdxTreeEntry* pTreeEntry = s_pIdxTreeAllFactories->getEntry(idx);

            if( pTreeEntry != nullptr )
            {
                pObjFactoryTmp = dynamic_cast<CObjFactory*>(pTreeEntry);

                if( pObjFactoryTmp != nullptr )
                {
                    if( i_iGraphObjType != EGraphObjTypeUndefined )
                    {
                        if( pObjFactoryTmp->getGraphObjType() == i_iGraphObjType )
                        {
                            if( !i_strGraphObjNameSpace.isEmpty() && !i_strGraphObjClassName.isEmpty() )
                            {
                                if( pObjFactoryTmp->getGraphObjNameSpace() == i_strGraphObjNameSpace
                                 && pObjFactoryTmp->getGraphObjClassName() == i_strGraphObjClassName )
                                {
                                    pObjFactory = pObjFactoryTmp;
                                    break;
                                }
                            }
                            else if( !i_strGraphObjNameSpace.isEmpty() )
                            {
                                if( pObjFactoryTmp->getGraphObjNameSpace() == i_strGraphObjNameSpace )
                                {
                                    pObjFactory = pObjFactoryTmp;
                                    break;
                                }
                            }
                            else if( !i_strGraphObjClassName.isEmpty() )
                            {
                                if( pObjFactoryTmp->getGraphObjClassName() == i_strGraphObjClassName )
                                {
                                    pObjFactory = pObjFactoryTmp;
                                    break;
                                }
                            }
                            else // if( i_strGraphObjNameSpace.isEmpty() && i_strGraphObjClassName.isEmpty() )
                            {
                                pObjFactory = pObjFactoryTmp;
                                break;
                            }
                        }
                    } // if( i_iGraphObjType != EGraphObjTypeUndefined )

                    else // if( i_iGraphObjType == EGraphObjTypeUndefined )
                    {
                        if( !i_strGraphObjNameSpace.isEmpty() && !i_strGraphObjClassName.isEmpty() )
                        {
                            if( pObjFactoryTmp->getGraphObjNameSpace() == i_strGraphObjNameSpace
                             && pObjFactoryTmp->getGraphObjClassName() == i_strGraphObjClassName )
                            {
                                pObjFactory = pObjFactoryTmp;
                                break;
                            }
                        }
                        else if( !i_strGraphObjNameSpace.isEmpty() )
                        {
                            if( pObjFactoryTmp->getGraphObjNameSpace() == i_strGraphObjNameSpace )
                            {
                                pObjFactory = pObjFactoryTmp;
                                break;
                            }
                        }
                        else if( !i_strGraphObjClassName.isEmpty() )
                        {
                            if( pObjFactoryTmp->getGraphObjClassName() == i_strGraphObjClassName )
                            {
                                pObjFactory = pObjFactoryTmp;
                                break;
                            }
                        }
                        else // if( i_strGraphObjNameSpace.isEmpty() && i_strGraphObjClassName.isEmpty() )
                        {
                            // No filter set. Returning the first registered object factory.
                            pObjFactory = pObjFactoryTmp;
                            break;
                        }
                    } // if( i_iGraphObjType == EGraphObjTypeUndefined )
                } // if( pObjFactoryTmp != nullptr )
            } // if( pTreeEntry != nullptr )
        } // for( idx = 0; idx < s_pIdxTreeAllFactories->treeEntriesVectorSize(); idx++ )
    } // if( s_pIdxTreeAllFactories != nullptr )

    return pObjFactory;

} // FindObjFactory

//------------------------------------------------------------------------------
CObjFactory* CObjFactory::FindObjFactory(
    const QString& i_strGraphObjNameSpace,
    const QString& i_strGraphObjClassName,
    const QString& i_strGraphObjType )
//------------------------------------------------------------------------------
{
    CObjFactory* pObjFactory = nullptr;
    CObjFactory* pObjFactoryTmp;
    int          idx;

    if( s_pIdxTreeAllFactories != nullptr )
    {
        for( idx = 0; idx < s_pIdxTreeAllFactories->treeEntriesVectorSize(); idx++ )
        {
            CIdxTreeEntry* pTreeEntry = s_pIdxTreeAllFactories->getEntry(idx);

            if( pTreeEntry != nullptr )
            {
                pObjFactoryTmp = dynamic_cast<CObjFactory*>(pTreeEntry);

                if( pObjFactoryTmp != nullptr )
                {
                    if( !i_strGraphObjType.isEmpty() )
                    {
                        if( pObjFactoryTmp->getGraphObjTypeAsString() == i_strGraphObjType )
                        {
                            if( !i_strGraphObjNameSpace.isEmpty() && !i_strGraphObjClassName.isEmpty() )
                            {
                                if( pObjFactoryTmp->getGraphObjNameSpace() == i_strGraphObjNameSpace
                                 && pObjFactoryTmp->getGraphObjClassName() == i_strGraphObjClassName )
                                {
                                    pObjFactory = pObjFactoryTmp;
                                    break;
                                }
                            }
                            else if( !i_strGraphObjNameSpace.isEmpty() )
                            {
                                if( pObjFactoryTmp->getGraphObjNameSpace() == i_strGraphObjNameSpace )
                                {
                                    pObjFactory = pObjFactoryTmp;
                                    break;
                                }
                            }
                            else if( !i_strGraphObjClassName.isEmpty() )
                            {
                                if( pObjFactoryTmp->getGraphObjClassName() == i_strGraphObjClassName )
                                {
                                    pObjFactory = pObjFactoryTmp;
                                    break;
                                }
                            }
                            else // if( i_strGraphObjNameSpace.isEmpty() && i_strGraphObjClassName.isEmpty() )
                            {
                                pObjFactory = pObjFactoryTmp;
                                break;
                            }
                        }
                    } // if( !i_strGraphObjType.isEmpty() )

                    else // if( i_strGraphObjType.isEmpty() )
                    {
                        if( !i_strGraphObjNameSpace.isEmpty() && !i_strGraphObjClassName.isEmpty() )
                        {
                            if( pObjFactoryTmp->getGraphObjNameSpace() == i_strGraphObjNameSpace
                             && pObjFactoryTmp->getGraphObjClassName() == i_strGraphObjClassName )
                            {
                                pObjFactory = pObjFactoryTmp;
                                break;
                            }
                        }
                        else if( !i_strGraphObjNameSpace.isEmpty() )
                        {
                            if( pObjFactoryTmp->getGraphObjNameSpace() == i_strGraphObjNameSpace )
                            {
                                pObjFactory = pObjFactoryTmp;
                                break;
                            }
                        }
                        else if( !i_strGraphObjClassName.isEmpty() )
                        {
                            if( pObjFactoryTmp->getGraphObjClassName() == i_strGraphObjClassName )
                            {
                                pObjFactory = pObjFactoryTmp;
                                break;
                            }
                        }
                        else // if( i_strGraphObjNameSpace.isEmpty() && i_strGraphObjClassName.isEmpty() )
                        {
                            // No filter set. Returning the first registered object factory.
                            pObjFactory = pObjFactoryTmp;
                            break;
                        }
                    } // if( i_strGraphObjType.isEmpty() )
                } // if( pObjFactoryTmp != nullptr )
            } // if( pTreeEntry != nullptr )
        } // for( idx = 0; idx < s_pIdxTreeAllFactories->treeEntriesVectorSize(); idx++ )
    } // if( s_pIdxTreeAllFactories != nullptr )

    return pObjFactory;

} // FindObjFactory

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjFactory::ResetAllCtorsDtorsCounters()
//------------------------------------------------------------------------------
{
    CObjFactory* pObjFactory;
    int          idx;

    for( idx = 0; idx < GetObjFactoriesCount(); idx++ )
    {
        pObjFactory = GetObjFactory(idx);

        if( pObjFactory != nullptr )
        {
            pObjFactory->ResetCtorsDtorsCounters();
        }
    }

} // ResetAllCtorsDtorsCounters

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjFactory::RegisterObjFactory( CObjFactory& i_objFactory, bool i_bAddToToolBoxIdxTree )
//------------------------------------------------------------------------------
{
    if( s_pTrcAdminObj == nullptr )
    {
        s_pTrcAdminObj = CTrcServer::GetTraceAdminObj(i_objFactory.getGraphObjNameSpace(), "CObjFactory", i_objFactory.getGraphObjTypeAsString());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ i_objFactory.getGraphObjClassName(),
        /* strMethod    */ "RegisterObjFactory",
        /* strAddInfo   */ "" );

    CObjFactory* pObjFactory = FindObjFactory(
        /* strNameSpace */ i_objFactory.getGraphObjNameSpace(),
        /* strClassName */ i_objFactory.getGraphObjClassName(),
        /* strObjType   */ i_objFactory.getGraphObjTypeAsString() );

    if( pObjFactory != nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyRegistered);
    }

    QString strNameSpace  = i_objFactory.getGroupName();
    QString strName       = i_objFactory.getGraphObjTypeAsString();
    QString strTargetPath = s_pIdxTreeAllFactories->buildPathStr(strNameSpace, strName);

    int iId = s_pIdxTreeAllFactories->add(&i_objFactory, strTargetPath);
    i_objFactory.setIdxTreeIdAllFactories(iId);

    if( i_bAddToToolBoxIdxTree )
    {
        iId = s_pIdxTreeToolBoxFactories->add(&i_objFactory, strTargetPath);
        i_objFactory.setIdxTreeIdToolBoxFactories(iId);
    }

    // The name got to be unique:
    if( strName != i_objFactory.getGraphObjTypeAsString() )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList);
    }

} // RegisterObjFactory

//------------------------------------------------------------------------------
void CObjFactory::UnregisterObjFactory( CObjFactory& i_objFactory )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ i_objFactory.getGraphObjClassName(),
        /* strMethod    */ "UnregisterObjFactory",
        /* strAddInfo   */ "" );

    CObjFactory* pObjFactory = FindObjFactory(
        /* strNameSpace */ i_objFactory.getGraphObjNameSpace(),
        /* strClassName */ i_objFactory.getGraphObjClassName(),
        /* strObjType   */ i_objFactory.getGraphObjTypeAsString() );

    if( pObjFactory == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotRegistered);
    }

    s_pIdxTreeAllFactories->remove(i_objFactory.keyInTree());

    if( i_objFactory.getIdxTreeIdToolBoxFactories() >= 0 )
    {
        s_pIdxTreeToolBoxFactories->remove(i_objFactory.keyInTree());
    }

} // UnregisterObjFactory

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactory::CObjFactory(
    const QString& i_strGroupName,
    const QString& i_strGraphObjNameSpace,
    const QString& i_strGraphObjClassName,
    int            i_iGraphObjType,
    const QString& i_strGraphObjType,
    bool           i_bAddToToolBoxIdxTree,
    const QIcon&   i_toolIcon ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Leave, i_strGroupName),
    m_strGroupName(i_strGroupName),
    m_strGraphObjNameSpace(i_strGraphObjNameSpace),
    m_strGraphObjClassName(i_strGraphObjClassName),
    m_iGraphObjType(i_iGraphObjType),
    m_strGraphObjType(i_strGraphObjType),
    m_toolIcon(i_toolIcon),
    m_iIdxTreeIdAllFactories(-1),
    m_iIdxTreeIdToolBoxFactories(-1),
    m_strFileName(),
    m_pTrcAdminObj(nullptr)
{
    s_iInstCount++;

    if( s_pIdxTreeAllFactories == nullptr )
    {
        s_pIdxTreeAllFactories = new CIdxTree(
            /* strObjName       */ "All Object Factories",
            /* pRootTreeEntry   */ nullptr,
            /* strNodeSeparator */ "::" );
    }

    if( s_pIdxTreeToolBoxFactories == nullptr )
    {
        s_pIdxTreeToolBoxFactories = new CIdxTree(
            /* strObjName       */ "Tool Box",
            /* pRootTreeEntry   */ nullptr,
            /* strNodeSeparator */ "::" );
    }

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(m_strGraphObjNameSpace, "CObjFactory", m_strGraphObjType);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    RegisterObjFactory(*this,i_bAddToToolBoxIdxTree);

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

    UnregisterObjFactory(*this);

    s_iInstCount--;

    if( s_iInstCount == 0 )
    {
        try
        {
            delete s_pIdxTreeToolBoxFactories;
        }
        catch(...)
        {
        }
        s_pIdxTreeToolBoxFactories = nullptr;

        try
        {
            delete s_pIdxTreeAllFactories;
        }
        catch(...)
        {
        }
        s_pIdxTreeAllFactories = nullptr;
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
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
        i_xmlStreamWriter.writeAttribute( "SelectionPoint", selectionPoint2Str(pGraphObjLabel->m_selPt) );
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
                ESelectionPoint selPt = ESelectionPointUndefined;
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
                    selPt = str2SelectionPoint(strAttr);
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

                if( !strKey.isEmpty() && !strText.isEmpty() && !arpLabels.contains(strKey) && selPt != ESelectionPointUndefined && bDistValid )
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
