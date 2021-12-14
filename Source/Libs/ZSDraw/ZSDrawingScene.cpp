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

#include <QtCore/qfile.h>
#include <QtCore/qglobal.h>
#include <QtCore/qmimedata.h>
#include <QtCore/qsettings.h>
#include <QtCore/qurl.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include <QtGui/qbitmap.h>
#include <QtGui/qevent.h>
#include <QtGui/qpaintdevice.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qgraphicsitem.h>
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/qgraphicsview.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/qgraphicsview.h>
#endif

#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactoryGroup.h"
#include "ZSDraw/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjImage.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CDrawingScene : public QGraphicsScene
*******************************************************************************/

static const QString c_strGraphObjIdName4ListByIndex   = "arpGraphObjs";
static const QString c_strGraphObjIdName4PoolByParents = "graphIdxTreeByParents";
static const QString c_strGraphObjIdName4PoolByTypes   = "graphIdxTreeByTypes";

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawingScene::FindUniqueGraphObjId(
    const QMap<QString,CGraphObj*>& i_dctpGraphObjs,
    const QString&                  i_strObjIdCurr )
//------------------------------------------------------------------------------
{
    QString strObjId = i_strObjIdCurr;
    int     iObjNr   = 1;
    QString strObjNr;

    // Remove trailing number (if any).
    if( strObjId.length() > 0 && isDigitChar(strObjId[strObjId.length()-1]) )
    {
        while( isDigitChar(strObjId[strObjId.length()-1]) )
        {
            strObjNr.insert( 0, strObjId[strObjId.length()-1] );
            strObjId.remove( strObjId.length()-1, 1 );
        }
        iObjNr = strObjNr.toInt();
        iObjNr++;
    }
    else
    {
        strObjNr = QString::number(iObjNr);
    }

    QString strObjBaseName = strObjId;

    strObjId = strObjBaseName + strObjNr;

    while( i_dctpGraphObjs.contains(strObjId) )
    {
        iObjNr++;
        strObjNr = QString::number(iObjNr);
        strObjId = strObjBaseName + strObjNr;
    }

    return strObjId;

} // FindUniqueGraphObjId

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawingScene::CDrawingScene( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QGraphicsScene(i_pObjParent),
    m_drawSettings(),
    m_fXResolution_dpmm(1.0),
    m_fYResolution_dpmm(1.0),
    m_mode(EMode::Undefined),
    m_editTool(EEditToolUndefined),
    m_editMode(EEditModeUndefined),
    m_editResizeMode(EEditResizeModeUndefined),
    m_pGraphicsItemSelectionArea(nullptr),
    m_pObjFactory(nullptr),
    m_pGraphicsItemCreating(nullptr),
    m_pGraphObjCreating(nullptr),
    m_pGraphicsItemAddingShapePoints(nullptr),
    m_pGraphObjAddingShapePoints(nullptr),
    m_strGraphObjNameSeparator("/"),
    m_bGraphObjIdChangedByMyself(false),
    m_dctpGraphObjs(),
    m_dctpGraphObjsClipboard(),
    m_arpGraphicsItemsAcceptingHoverEvents(),
    m_fRotAngleRes_degree(1.0),
    m_fHitTolerance_px(2.0),
    m_bMouseDoubleClickEventInProcess(false),
    m_ptMouseEvScenePosOnMousePressEvent(),
    m_iEvKeyModifiers(0),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("Drawing");

    //if( m_strGraphObjNameSeparator.isEmpty() )
    //{
    //    m_strGraphObjNameSeparator = "/";
    //}

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    //setItemIndexMethod(NoIndex);

    // To update the mode labels:
    setMode( EMode::Edit, m_editTool, m_editMode, m_editResizeMode );

    QSettings settings;

    m_drawSettings.load( settings, objectName() + "/DrawSettings" );

} // ctor

//------------------------------------------------------------------------------
CDrawingScene::~CDrawingScene()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;

    m_drawSettings.save( settings, objectName() + "/DrawSettings" );

    CGraphObj* pGraphObj;

    // Clear internal clipboard.
    if( m_dctpGraphObjsClipboard.size() > 0 )
    {
        QMap<QString,CGraphObj*>::iterator itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

        while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
        {
            pGraphObj = itGraphObjsClipboard.value();

            if( pGraphObj != nullptr )
            {
                deleteItem(pGraphObj);
            }
            ++itGraphObjsClipboard;
        }
    }
    m_dctpGraphObjsClipboard.clear();

    // Clear (and destroy graphical object items) here as dtor of CGraphObj
    // accesses the drawing scene to remove the object name from the hash.
    clear();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pGraphicsItemSelectionArea = nullptr;
    m_pGraphicsItemCreating = nullptr;
    m_pGraphObjCreating = nullptr;
    m_pGraphicsItemAddingShapePoints = nullptr;
    m_pGraphObjAddingShapePoints = nullptr;
    m_pObjFactory = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::clear()
//------------------------------------------------------------------------------
{
   CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "clear",
        /* strAddInfo   */ "" );

    m_pGraphicsItemSelectionArea = nullptr;
    m_pGraphicsItemCreating = nullptr;
    m_pGraphObjCreating = nullptr;
    m_pGraphicsItemAddingShapePoints = nullptr;
    m_pGraphObjAddingShapePoints = nullptr;

    QGraphicsScene::clear();

    m_arpGraphicsItemsAcceptingHoverEvents.clear();

    CObjFactory::ResetAllCtorsDtorsCounters();

} // clear

//------------------------------------------------------------------------------
void CDrawingScene::removeItem( QGraphicsItem* i_pGraphicsItem )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + pGraphObj->getObjName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeItem",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsScene::removeItem(i_pGraphicsItem);

} // removeItem

//------------------------------------------------------------------------------
void CDrawingScene::removeItem( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + i_pGraphObj->getObjName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeItem",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);

    if( pGraphicsItem != nullptr )
    {
        QGraphicsScene::removeItem(pGraphicsItem);
    }

} // removeItem

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::deleteItem( QGraphicsItem* i_pGraphicsItem )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + pGraphObj->getObjName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "deleteItem",
        /* strAddInfo   */ strAddTrcInfo );

    if( pGraphObj != nullptr )
    {
        deleteItem(pGraphObj);
        pGraphObj = nullptr;
        i_pGraphicsItem = nullptr;
    }

} // deleteItem

//------------------------------------------------------------------------------
void CDrawingScene::deleteItem( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + i_pGraphObj->getObjName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "deleteItem",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pGraphObj != nullptr )
    {
        QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);

        if( pGraphicsItem != nullptr )
        {
            QList<QGraphicsItem*> arpGraphicsItemsChilds = pGraphicsItem->childItems();
            QStringList           strlstGraphObjIdsChilds;
            QGraphicsItem*        pGraphicsItemChild;
            CGraphObj*            pGraphObjChild;
            QString               strGraphObjIdChild;
            int                   idxGraphObjChild;

            // On deleting a graphical object the object may destroy other graphical objects
            // (e.g. connection points may delete connection lines) and the list of selected
            // items may change while the dtor is in process. So we don't store a list with object
            // references but a list with object ids and check whether the object still belong
            // to the drawing scene before deleting the object.

            for( idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++ )
            {
                pGraphicsItemChild = arpGraphicsItemsChilds[idxGraphObjChild];

                pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);

                if( pGraphObjChild != nullptr )
                {
                    strGraphObjIdChild = pGraphObjChild->getObjId();
                    strlstGraphObjIdsChilds.append(strGraphObjIdChild);
                }
            }

            for( idxGraphObjChild = 0; idxGraphObjChild < strlstGraphObjIdsChilds.size(); idxGraphObjChild++ )
            {
                strGraphObjIdChild = strlstGraphObjIdsChilds[idxGraphObjChild];

                pGraphObjChild = findGraphObj(strGraphObjIdChild);

                if( pGraphObjChild != nullptr )
                {
                    deleteItem(pGraphObjChild);
                    pGraphObjChild = nullptr;
                }
            }

            delete i_pGraphObj;
            i_pGraphObj = nullptr;
        }

    } // if( i_pGraphObj != nullptr )

} // deleteItem

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::addChildItems( QGraphicsItem* i_pGraphicsItem )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + pGraphObj->getObjName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addChildItems",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pGraphicsItem != nullptr )
    {
        QList<QGraphicsItem*> arpGraphicsItemsChilds = i_pGraphicsItem->childItems();
        QGraphicsItem*        pGraphicsItemChild;
        CGraphObj*            pGraphObjChild;
        int                   idxGraphObjChild;

        for( idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++ )
        {
            pGraphicsItemChild = arpGraphicsItemsChilds[idxGraphObjChild];
            pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);

            if( pGraphicsItemChild != nullptr && pGraphObjChild != nullptr )
            {
                addItem(pGraphicsItemChild);

                onGraphObjCreated(pGraphObjChild);

                if( pGraphicsItemChild->childItems().size() > 0 )
                {
                    addChildItems(pGraphicsItemChild);
                }
            }
        }

    } // if( i_pGraphicsItem != nullptr )

} // addChildItems

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CDrawingScene::load( const QString& i_strFileName )
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
        /* strMethod    */ "load",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    QFile fileXML;

    if( i_strFileName.isEmpty() )
    {
        errResultInfo = ErrResultInfoError("load", EResultInvalidFileName, "File name is empty");
    }
    else
    {
        fileXML.setFileName(i_strFileName);

        if( !fileXML.open(QIODevice::ReadOnly) )
        {
            errResultInfo = ErrResultInfoError("load", EResultFileOpenForRead, i_strFileName);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QXmlStreamReader            xmlStreamReader(&fileXML);
        QXmlStreamAttributes        xmlStreamAttrs;
        QXmlStreamReader::TokenType xmlStreamTokenType;
        QString                     strElemName;
        QString                     strAttr;
        bool                        bConverted;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML document");
        }

        while( !xmlStreamReader.hasError() && !xmlStreamReader.atEnd() )
        {
            xmlStreamTokenType = xmlStreamReader.readNext();
            strElemName = xmlStreamReader.name().toString();

            if( xmlStreamReader.isStartElement() )
            {
                //--------------------------------
                if( strElemName == "Drawing" )
                //--------------------------------
                {
                    xmlStreamAttrs = xmlStreamReader.attributes();

                    if( xmlStreamAttrs.hasAttribute("Rect") )
                    {
                        strAttr = xmlStreamAttrs.value("Rect").toString();

                        QRectF rctDrawing = str2RectF(strAttr,&bConverted);

                        if( bConverted )
                        {
                            setSceneRect(rctDrawing);
                        }
                    }

                } // if( (strElemName == "Drawing") )

                //--------------------------------
                else if( strElemName == "GraphObj" )
                //--------------------------------
                {
                    QString strNameSpace;
                    QString strClassName;
                    QString strGraphObjType;
                    QString strObjName;
                    QString strObjId;

                    xmlStreamAttrs = xmlStreamReader.attributes();

                    if( xmlStreamAttrs.hasAttribute("NameSpace") )
                    {
                        strNameSpace = xmlStreamAttrs.value("NameSpace").toString();;
                    }
                    if( xmlStreamAttrs.hasAttribute("ClassName") )
                    {
                        strClassName = xmlStreamAttrs.value("ClassName").toString();;
                    }
                    if( xmlStreamAttrs.hasAttribute("ObjectType") )
                    {
                        strGraphObjType = xmlStreamAttrs.value("ObjectType").toString();
                    }
                    if( xmlStreamAttrs.hasAttribute("ObjectName") )
                    {
                        strObjName = xmlStreamAttrs.value("ObjectName").toString();;
                    }
                    if( xmlStreamAttrs.hasAttribute("ObjectId") )
                    {
                        strObjId = xmlStreamAttrs.value("ObjectId").toString();;
                    }

                    CObjFactory* pObjFactory = CObjFactory::FindObjFactory( strNameSpace, strClassName, strGraphObjType );

                    if( pObjFactory != nullptr )
                    {
                        pObjFactory->loadGraphObj(
                            /* pDrawingScene   */ this,
                            /* pGraphObjGroup  */ nullptr,
                            /* strObjName      */ strObjName,
                            /* strObjId        */ strObjId,
                            /* xmlStreamReader */ xmlStreamReader,
                            /* errResultInfo   */ errResultInfo );
                    }

                } // if( strElemName == "GraphObj" )

            } // if( xmlStreamReader.isStartElement() )

        } // while( !xmlStreamReader.hasError() && !xmlStreamReader.atEnd() )

        if( xmlStreamReader.hasError() )
        {
            QString strAddErrInfo;
            strAddErrInfo += xmlStreamReader.errorString() + " on reading \"" + i_strFileName + "\" ";
            strAddErrInfo += "(Line:" + QString::number(xmlStreamReader.lineNumber());
            strAddErrInfo += ", Column:" + QString::number(xmlStreamReader.columnNumber()) + ")";
            errResultInfo = ErrResultInfoError("load", EResultFileReadContent, strAddErrInfo);
        }

    } // if( !errResultInfo.isErrorResult() )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // load

//------------------------------------------------------------------------------
SErrResultInfo CDrawingScene::save( const QString& i_strFileName )
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
        /* strMethod    */ "save",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    QFile fileXML;

    if( i_strFileName.isEmpty() )
    {
        errResultInfo = ErrResultInfoError("save", EResultInvalidFileName, "File name is empty");
    }
    else
    {
        fileXML.setFileName(i_strFileName);

        if( !fileXML.open(QIODevice::WriteOnly) )
        {
            errResultInfo = ErrResultInfoError("save", EResultFileOpenForWrite, i_strFileName);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QXmlStreamWriter xmlStreamWriter(&fileXML);

        xmlStreamWriter.setAutoFormatting(true);

        xmlStreamWriter.writeStartDocument();

        xmlStreamWriter.writeStartElement("Drawing");
        xmlStreamWriter.writeAttribute( "Rect", rect2Str(sceneRect()) );

        QGraphicsItem* pGraphicsItem;
        CGraphObj*     pGraphObj;
        int            idxGraphObj;

        // Connection points need to be recalled before the connection lines as on
        // creating the connection lines their connection points must already exist.
        // For this the connection lines will be saved at the end of the XML file.
        for( idxGraphObj = 0; idxGraphObj < items().size(); idxGraphObj++ )
        {
            pGraphicsItem = items()[idxGraphObj];
            pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

            if( pGraphicsItem->type() != EGraphObjTypeSelectionPoint
             && pGraphicsItem->type() != EGraphObjTypeLabel
             && pGraphicsItem->type() != EGraphObjTypeConnectionLine )
            {
                // Group members will be saved as child items of the groups.
                if( pGraphicsItem->parentItem() == nullptr )
                {
                    errResultInfo = save(pGraphObj,xmlStreamWriter);

                    if( errResultInfo.isErrorResult() )
                    {
                        break;
                    }
                }
            }
        }

        if( !errResultInfo.isErrorResult() )
        {
            for( idxGraphObj = 0; idxGraphObj < items().size(); idxGraphObj++ )
            {
                pGraphicsItem = items()[idxGraphObj];

                if( pGraphicsItem->type() != EGraphObjTypeSelectionPoint
                 && pGraphicsItem->type() != EGraphObjTypeLabel
                 && pGraphicsItem->type() == EGraphObjTypeConnectionLine )
                {
                    // Group members will be saved as child items of the groups.
                    if( pGraphicsItem->parentItem() == nullptr )
                    {
                        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                        errResultInfo = save(pGraphObj,xmlStreamWriter);

                        if( errResultInfo.isErrorResult() )
                        {
                            break;
                        }
                    }
                }
            }
        }

        xmlStreamWriter.writeEndElement();  // Drawing

        xmlStreamWriter.writeEndDocument();

    } // if( !errResultInfo.isErrorResult() )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;

} // save

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CDrawingScene::save( CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObj:" + i_pGraphObj->getNameSpace();
        strAddTrcInfo += "::" + i_pGraphObj->getClassName();
        strAddTrcInfo += "::" + i_pGraphObj->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "save",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    QString strNameSpace    = i_pGraphObj->getNameSpace();
    QString strClassName    = i_pGraphObj->getClassName();
    QString strGraphObjType = i_pGraphObj->getTypeAsString();
    QString strObjName      = i_pGraphObj->getObjName();
    QString strObjId        = i_pGraphObj->getObjId();

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory( strNameSpace, strClassName, strGraphObjType );

    if( pObjFactory == nullptr )
    {
        QString strAddErrInfo = strNameSpace + "::" + strClassName + " (" + strGraphObjType + ")";
        errResultInfo = ErrResultInfoError("save", EResultObjFactoryNotFound, strAddErrInfo);
    }
    else
    {
        i_xmlStreamWriter.writeStartElement("GraphObj");

        i_xmlStreamWriter.writeAttribute( "NameSpace", strNameSpace );
        i_xmlStreamWriter.writeAttribute( "ClassName", strClassName );
        i_xmlStreamWriter.writeAttribute( "ObjectType", strGraphObjType );
        i_xmlStreamWriter.writeAttribute( "ObjectName", strObjName );
        i_xmlStreamWriter.writeAttribute( "ObjectId", strObjId );

        errResultInfo = pObjFactory->saveGraphObj(i_pGraphObj,i_xmlStreamWriter);

        i_xmlStreamWriter.writeEndElement();
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // save

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setMode(
    EMode           i_mode,
    EEditTool       i_editTool,
    EEditMode       i_editMode,
    EEditResizeMode i_editResizeMode,
    bool            i_bObjFactoryTypeChanged )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Mode:" + CEnumMode::toString(i_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(i_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(i_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(i_editResizeMode);
        strAddTrcInfo += ", ObjFactoryChanged:" + bool2Str(i_bObjFactoryTypeChanged);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setMode",
        /* strAddInfo   */ strAddTrcInfo );

    bool bModeChanged = false;

    if( i_mode != EMode::Ignore && m_mode != i_mode )
    {
        bModeChanged = true;

        m_mode = i_mode;

        if( m_mode == EMode::Simulation )
        {
            if( i_editTool == EEditToolIgnore )
            {
                m_editTool = EEditToolUndefined;
            }
            if( i_editMode == EEditModeIgnore )
            {
                m_editMode = EEditModeUndefined;
            }
            if( i_editResizeMode == EEditResizeModeIgnore )
            {
                m_editResizeMode = EEditResizeModeUndefined;
            }
        }

        clearSelection();

    } // if( i_mode != EMode::Ignore && m_mode != i_mode )

    if( i_editTool != EEditToolIgnore && m_editTool != i_editTool )
    {
        bModeChanged = true;

        m_editTool = i_editTool;

        clearSelection();
    }

    QString strObjFactory;

    if( m_pObjFactory != nullptr )
    {
        strObjFactory = m_pObjFactory->getGraphObjTypeAsString();
    }
    else
    {
        m_pGraphicsItemCreating = nullptr;
        m_pGraphObjCreating = nullptr;
    }

    if( m_editTool != EEditToolCreateObjects )
    {
        m_pGraphicsItemCreating = nullptr;
        m_pGraphObjCreating = nullptr;
    }

    if( i_editMode != EEditModeIgnore && m_editMode != i_editMode )
    {
        bModeChanged = true;

        m_editMode = i_editMode;
    }

    if( i_editResizeMode != EEditResizeModeIgnore && m_editResizeMode != i_editResizeMode )
    {
        bModeChanged = true;

        m_editResizeMode = i_editResizeMode;
    }

    if( bModeChanged || i_bObjFactoryTypeChanged )
    {
        update();

        emit modeChanged();
    }

} // setMode

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setCurrentDrawingTool( CObjFactory* i_pObjFactory )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pObjFactory == nullptr )
        {
            strAddTrcInfo = "ObjFactory: nullptr";
        }
        else
        {
            strAddTrcInfo  = "ObjNameSpace:" + i_pObjFactory->getGraphObjNameSpace();
            strAddTrcInfo += ", ObjClassName:" + i_pObjFactory->getGraphObjClassName();
            strAddTrcInfo += ", ObjType:" + i_pObjFactory->getGraphObjTypeAsString();
        }
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentDrawingTool",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_mode == EMode::Edit )
    {
        int iGraphObjTypePrev = EGraphObjTypeUndefined;

        if( m_pObjFactory != nullptr )
        {
            iGraphObjTypePrev = m_pObjFactory->getGraphObjType();
        }

        int iGraphObjTypeCurr = iGraphObjTypePrev;

        m_pObjFactory = i_pObjFactory;

        if( m_pObjFactory != nullptr )
        {
            iGraphObjTypeCurr = m_pObjFactory->getGraphObjType();
        }
        else
        {
            iGraphObjTypeCurr = EGraphObjTypeUndefined;
        }

        if( iGraphObjTypePrev != iGraphObjTypeCurr )
        {
            if( iGraphObjTypeCurr == EGraphObjTypeUndefined )
            {
                EEditTool editTool = m_editTool;

                if( m_editTool != EEditToolSelect )
                {
                    editTool = EEditToolUndefined;
                }
                setMode( EMode::Ignore, editTool, EEditModeUndefined, EEditResizeModeUndefined, true );
            }
            else
            {
                setMode( EMode::Ignore, EEditToolCreateObjects, EEditModeUndefined, EEditResizeModeUndefined, true );
            }
        }

    } // if( m_mode == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo = "nullptr";
        }
        else
        {
            strAddTrcInfo  = "ObjNameSpace:" + m_pObjFactory->getGraphObjNameSpace();
            strAddTrcInfo += ", ObjClassName:" + m_pObjFactory->getGraphObjClassName();
            strAddTrcInfo += ", ObjType:" + m_pObjFactory->getGraphObjTypeAsString();
        }
        mthTracer.trace(strAddTrcInfo);
    }

} // setCurrentDrawingTool

//------------------------------------------------------------------------------
void CDrawingScene::setCurrentDrawingTool(
    const QString& i_strGraphObjNameSpace,
    const QString& i_strGraphObjClassName,
    const QString& i_strGraphObjType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ObjNameSpace:" + i_strGraphObjNameSpace;
        strAddTrcInfo += ", ObjClassName:" + i_strGraphObjClassName;
        strAddTrcInfo += ", ObjType:" + i_strGraphObjType;
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentDrawingTool",
        /* strAddInfo   */ strAddTrcInfo );

    QString strGraphObjTypePrev;

    if( m_pObjFactory != nullptr )
    {
        strGraphObjTypePrev = m_pObjFactory->getGraphObjTypeAsString();
    }

    QString strGraphObjTypeCurr;

    if( i_strGraphObjType.isEmpty() )
    {
        m_pObjFactory = nullptr;
    }
    else if( m_mode == EMode::Edit )
    {
        CObjFactory* pObjFactory = CObjFactory::FindObjFactory(
            /* strNameSpace */ i_strGraphObjNameSpace,
            /* strClassName */ i_strGraphObjClassName,
            /* objType      */ i_strGraphObjType );

        if( pObjFactory == nullptr )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotRegistered);
        }
        m_pObjFactory = pObjFactory;
        strGraphObjTypeCurr = i_strGraphObjType;
    }

    if( strGraphObjTypePrev != strGraphObjTypeCurr )
    {
        if( strGraphObjTypeCurr.isEmpty() )
        {
            EEditTool editTool = m_editTool;

            if( m_editTool != EEditToolSelect )
            {
                editTool = EEditToolUndefined;
            }
            setMode( EMode::Ignore, editTool, EEditModeUndefined, EEditResizeModeUndefined );
        }
        else
        {
            setMode( EMode::Ignore, EEditToolCreateObjects, EEditModeUndefined, EEditResizeModeUndefined );
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo = "nullptr";
        }
        else
        {
            strAddTrcInfo  = "ObjNameSpace:" + m_pObjFactory->getGraphObjNameSpace();
            strAddTrcInfo += ", ObjClassName:" + m_pObjFactory->getGraphObjClassName();
            strAddTrcInfo += ", ObjType:" + m_pObjFactory->getGraphObjTypeAsString();
        }
        mthTracer.trace(strAddTrcInfo);
    }

} // setCurrentDrawingTool

//------------------------------------------------------------------------------
int CDrawingScene::getCurrentDrawingToolGraphObjType() const
//------------------------------------------------------------------------------
{
    int iObjFactoryType = EGraphObjTypeUndefined;

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }
    return iObjFactoryType;

} // getCurrentDrawingToolGraphObjType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjConnectionPoint* CDrawingScene::getConnectionPoint( const QPointF& i_ptScenePos )
//------------------------------------------------------------------------------
{
    CGraphObjConnectionPoint* pGraphObjCnctPt = nullptr;

    QList<QGraphicsItem*> arpGraphicsItems;
    QGraphicsItem*        pGraphicsItem;
    int                   idxGraphObj;

    arpGraphicsItems = items(i_ptScenePos);

    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
    {
        pGraphicsItem = arpGraphicsItems[idxGraphObj];

        if( pGraphicsItem->type() == EGraphObjTypeConnectionPoint )
        {
            pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(pGraphicsItem);
            break;
        }
    }

    return pGraphObjCnctPt;

} // getConnectionPoint

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QCursor CDrawingScene::getProposedCursor( const QPointF& i_ptPos ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Pos:" + point2Str(i_ptPos);
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", ObjFactory:" + graphObjType2Str(getCurrentDrawingToolGraphObjType());

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getObjName();
        }
        if( m_pGraphObjAddingShapePoints == nullptr )
        {
            strAddTrcInfo += ", GraphObjAddingShapePoints:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjAddingShapePoints:" + m_pGraphObjAddingShapePoints->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "getProposedCursor",
        /* strAddInfo   */ strAddTrcInfo );

    int iObjFactoryType = getCurrentDrawingToolGraphObjType();

    QCursor cursor = Qt::ArrowCursor;

    if( m_mode == EMode::Simulation )
    {
        cursor = Qt::ArrowCursor;

    } // if( i_iMode == EMode::Simulation )

    else if( m_mode == EMode::Edit )
    {
        QList<QGraphicsItem*> arpGraphicsItemsUnderCursor = items(i_ptPos);
        QGraphicsItem*        pGraphicsItem;
        int                   idxGraphObj;

        switch( m_editTool )
        {
            case EEditToolSelect:
            {
                switch( m_editMode )
                {
                    case EEditModeMove:
                    {
                        cursor = Qt::ArrowCursor;
                        break;
                    }
                    case EEditModeResize:
                    {
                        switch( m_editResizeMode )
                        {
                            case EEditResizeModeResizeAll:
                            {
                                break;
                            }
                            case EEditResizeModeResizeHor:
                            {
                                break;
                            }
                            case EEditResizeModeResizeVer:
                            {
                                break;
                            }
                            case EEditResizeModeUndefined:
                            default:
                            {
                                break;
                            }
                        }
                        break;
                    }
                    case EEditModeRotate:
                    {
                        break;
                    }
                    case EEditModeMoveShapePoint:
                    {
                        break;
                    }
                    case EEditModeEditText:
                    {
                        cursor = Qt::IBeamCursor;
                        break;
                    }
                    case EEditModeCreating: // this mode is only used by graphical object but not by the scene
                    case EEditModeUndefined:
                    default:
                    {
                        cursor = Qt::ArrowCursor;
                        break;
                    }
                } // switch( m_editMode )
                break;
            } // case EEditToolSelect:

            case EEditToolCreateObjects:
            {
                switch( m_editMode )
                {
                    case EEditModeMove:
                    {
                        break;
                    }
                    case EEditModeResize:
                    {
                        switch( m_editResizeMode )
                        {
                            case EEditResizeModeResizeAll:
                            {
                                break;
                            }
                            case EEditResizeModeResizeHor:
                            {
                                break;
                            }
                            case EEditResizeModeResizeVer:
                            {
                                break;
                            }
                            case EEditResizeModeUndefined:
                            default:
                            {
                                break;
                            }
                        }
                        break;
                    }
                    case EEditModeRotate:
                    {
                        break;
                    }
                    case EEditModeMoveShapePoint:
                    {
                        if( iObjFactoryType != EGraphObjTypeUndefined )
                        {
                            cursor = Qt::CrossCursor;
                        }
                        break;
                    }
                    case EEditModeEditText:
                    {
                        cursor = Qt::IBeamCursor;
                        break;
                    }
                    case EEditModeCreating: // this mode is only used by graphical object but not by the scene
                    case EEditModeUndefined:
                    default:
                    {
                        if( iObjFactoryType != EGraphObjTypeUndefined )
                        {
                            // Special case for connection lines which may only be created on connection points.
                            if( iObjFactoryType == EGraphObjTypeConnectionLine )
                            {
                                cursor = Qt::ArrowCursor;

                                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsUnderCursor.size(); idxGraphObj++ )
                                {
                                    pGraphicsItem = arpGraphicsItemsUnderCursor[idxGraphObj];

                                    if( pGraphicsItem->type() == EGraphObjTypeConnectionPoint )
                                    {
                                        QPixmap pxmCursor(":/ZS/Draw/CursorPin16x16.bmp");
                                        QBitmap bmpCursorMask = pxmCursor.createHeuristicMask();
                                        pxmCursor.setMask(bmpCursorMask);
                                        cursor = QCursor(pxmCursor,0,pxmCursor.height()-1);
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                cursor = Qt::CrossCursor;
                            }
                        }
                        break;
                    }
                } // switch( m_editMode )
                break;
            } // case EEditToolCreateObjects

            case EEditToolUndefined:
            default:
            {
                cursor = Qt::ArrowCursor;
                break;
            }

        } // switch( m_editTool )

    } // if( m_mode == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Cursor:" + qCursorShape2Str(cursor.shape());
        //strAddTrcInfo  = ", CursorPrev:" + qCursorShape2Str(cursorShapePrev);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return cursor;

} // getProposedCursor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CDrawingScene::bringToFront( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObjName:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObjName:" + pGraphObj->getObjName();
        }
        strAddTrcInfo += ", ScenePos:" + point2Str(i_ptScenePos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "bringToFront",
        /* strAddInfo   */ strAddTrcInfo );

    QList<QGraphicsItem*> arpGraphicsItems = items(i_ptScenePos);

    return bringToFront( i_pGraphicsItem, arpGraphicsItems );

} // bringToFront

//------------------------------------------------------------------------------
double CDrawingScene::sendToBack( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObjName:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObjName:" + pGraphObj->getObjName();
        }
        strAddTrcInfo += ", ScenePos:" + point2Str(i_ptScenePos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "sendToBack",
        /* strAddInfo   */ strAddTrcInfo );

    QList<QGraphicsItem*> arpGraphicsItems = items(i_ptScenePos);

    return sendToBack( i_pGraphicsItem, arpGraphicsItems );

} // sendToBack

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
double CDrawingScene::bringToFront( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObjName:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObjName:" + pGraphObj->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "bringToFront",
        /* strAddInfo   */ strAddTrcInfo );

    QList<QGraphicsItem*> arpGraphicsItems;
    QGraphicsItem*        pGraphicsItem;
    int                   idxGraphObj;

    qreal fZValue = 0.0;

    if( i_arpGraphicsItems.size() == 0 )
    {
        arpGraphicsItems = items();
    }
    else
    {
        arpGraphicsItems = i_arpGraphicsItems;
    }

    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
    {
        pGraphicsItem = arpGraphicsItems[idxGraphObj];

        if( pGraphicsItem->type() != EGraphObjTypeSelectionPoint && pGraphicsItem->type() != EGraphObjTypeLabel )
        {
            if( pGraphicsItem->zValue() >= fZValue )
            {
                fZValue = pGraphicsItem->zValue() + 0.1;
            }
        }
    }

    i_pGraphicsItem->setZValue(fZValue);

    return fZValue;

} // bringToFront

//------------------------------------------------------------------------------
double CDrawingScene::sendToBack( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObjName:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObjName:" + pGraphObj->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "sendToBack",
        /* strAddInfo   */ strAddTrcInfo );

    QList<QGraphicsItem*> arpGraphicsItems;
    QGraphicsItem*        pGraphicsItem;
    int                   idxGraphObj;

    qreal fZValue = 0.0;

    if( i_arpGraphicsItems.size() == 0 )
    {
        arpGraphicsItems = items();
    }
    else
    {
        arpGraphicsItems = i_arpGraphicsItems;
    }

    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
    {
        pGraphicsItem = arpGraphicsItems[idxGraphObj];

        if( pGraphicsItem->type() != EGraphObjTypeSelectionPoint && pGraphicsItem->type() != EGraphObjTypeLabel )
        {
            if( pGraphicsItem->zValue() <= fZValue )
            {
                fZValue = pGraphicsItem->zValue() - 0.1;
            }
        }
    }

    i_pGraphicsItem->setZValue(fZValue);

    return fZValue;

} // sendToBack

/*==============================================================================
public: // to be called by graphical objects (as graphical objects are not derived from QObject and cannot emit signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjAddingShapePointsStarted( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strGraphObjName = i_pGraphObj->getObjName(true,m_strGraphObjNameSeparator);

    // This method has to be called by graphical objects (polylines and connection lines)
    // if new shape points will be inserted at the beginning or appended at the end
    // of the polylines so that the drawing scene will further on dispatch mouse events
    // "directly" to the object "under construction". On extended polylines the object
    // is not hit and the graphics scene does not dispatch the mouse event to the objects
    // or their selection points (similar to newly creating objects).

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObjName:" + strGraphObjName;
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }
        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getObjName();
        }
        if( m_pGraphObjAddingShapePoints == nullptr )
        {
            strAddTrcInfo += ", GraphObjAddingShapePoints:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjAddingShapePoints:" + m_pGraphObjAddingShapePoints->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjAddingShapePointsStarted",
        /* strAddInfo   */ strAddTrcInfo );

    //if( m_pGraphObjAddingShapePoints != nullptr )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "pGraphObjAddingShapePoints != nullptr" );
    //}

    m_pGraphObjAddingShapePoints = i_pGraphObj;
    m_pGraphicsItemAddingShapePoints = dynamic_cast<QGraphicsItem*>(i_pGraphObj);

    if( m_pGraphicsItemAddingShapePoints == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItemAddingShapePoints == nullptr" );
    }

} // onGraphObjAddingShapePointsStarted

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjAddingShapePointsFinished( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strGraphObjName = i_pGraphObj->getObjName(true,m_strGraphObjNameSeparator);

    // This method has to be called by graphical objects (polylines and connection lines)
    // if new shape points will be inserted at the beginning or appended at the end
    // of the polylines so that the drawing scene will further on dispatch mouse events
    // "directly" to the object "under construction". On extended polylines the object
    // is not hit and the graphics scene does not dispatch the mouse event to the objects
    // or their selection points (similar to newly creating objects).

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObjName:" + strGraphObjName;
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }
        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getObjName();
        }
        if( m_pGraphObjAddingShapePoints == nullptr )
        {
            strAddTrcInfo += ", GraphObjAddingShapePoints:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjAddingShapePoints:" + m_pGraphObjAddingShapePoints->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjAddingShapePointsFinished",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pGraphObjAddingShapePoints != nullptr && m_pGraphObjAddingShapePoints != i_pGraphObj )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "pGraphObjAddingShapePoints != i_pGraphObj" );
    }

    m_pGraphObjAddingShapePoints = nullptr;
    m_pGraphicsItemAddingShapePoints = nullptr;

} // onGraphObjAddingShapePointsFinished

/*==============================================================================
public: // to be called by graphical objects (as graphical objects are not derived from QObject and cannot emit signals)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjCreated( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strGraphObjNameSpace = i_pGraphObj->getNameSpace();
    QString strGraphObjClassName = i_pGraphObj->getClassName();
    QString strGraphObjName      = i_pGraphObj->getObjName(true,m_strGraphObjNameSeparator);
    QString strGraphObjId        = i_pGraphObj->getObjId();

    // This method has to be called by graphical objects AFTER they have been
    // initially created (after they have been completely created e.g. after
    // a rectangle has been resized and the mouse button has been releases).
    // After creating the object via mouse events the edit mode of the drawing
    // has to be reset. In addition the name of the object has to be inserted
    // in the hash of item names.

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObjName:" + strGraphObjName;
        strAddTrcInfo += ", GraphObjId:" + strGraphObjId;
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }
        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getObjName();
        }
        if( m_pGraphObjAddingShapePoints == nullptr )
        {
            strAddTrcInfo += ", GraphObjAddingShapePoints:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjAddingShapePoints:" + m_pGraphObjAddingShapePoints->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjCreated",
        /* strAddInfo   */ strAddTrcInfo );

    QString strGraphObjTypeName;

    if( strGraphObjNameSpace.isEmpty() )
    {
        strGraphObjTypeName = strGraphObjClassName;
    }
    else
    {
        strGraphObjTypeName = strGraphObjNameSpace + m_strGraphObjNameSeparator + strGraphObjClassName;
    }

    // Ensure that the object name is unique below its parent:
    //--------------------------------------------------------

    //strGraphObjName = findUniqueGraphObjName(i_pGraphObj);
    //i_pGraphObj->setObjName(strGraphObjName);

    // Ensure that the object id is unique and insert into map with objects sorted by id:
    //-----------------------------------------------------------------------------------

    if( m_dctpGraphObjs.contains(strGraphObjId) )
    {
        strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjs, strGraphObjId );

        // The old object id is inserted in the dictionary. On changing the
        // object id the graph object calls "onGraphObjIdChanged" as a reentry.
        // This method would search for the object with the old id - which is not
        // the object which has just been created but is the object with the already
        // existing id. So we need to block the "onGraphObjIdChanged" method:
        m_bGraphObjIdChangedByMyself = true;
        i_pGraphObj->setObjId(strGraphObjId);
        m_bGraphObjIdChangedByMyself = false;
    }

    m_dctpGraphObjs.insert( strGraphObjId, i_pGraphObj );

    // Reset drawings scenes edit modes:
    //----------------------------------

    if( m_mode == EMode::Edit )
    {
        if( m_editTool == EEditToolCreateObjects )
        {
            if( m_pGraphObjCreating == i_pGraphObj )
            {
                m_pGraphicsItemCreating->setSelected(false);
                m_pGraphicsItemCreating->setAcceptHoverEvents(false);
                m_pGraphicsItemCreating->setAcceptedMouseButtons(Qt::NoButton);

                m_pGraphicsItemCreating = nullptr;
                m_pGraphObjCreating = nullptr;
                m_pGraphicsItemAddingShapePoints = nullptr;
                m_pGraphObjAddingShapePoints = nullptr;

                setMode( EMode::Ignore, EEditToolIgnore, EEditModeUndefined, EEditResizeModeUndefined, false );
            }
        }
    }

    emit graphObjCreated(i_pGraphObj);

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "GraphObjId:" + i_pGraphObj->getObjId();
        mthTracer.trace(strAddTrcInfo);
    }

} // onGraphObjCreated

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjDestroying( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObjId:" + i_strObjId;
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjDestroying",
        /* strAddInfo   */ strAddTrcInfo );

    // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
    // dtor of QGraphicsItem has already been executed. The order the dtors
    // of inherited classes are called depends on the order the classes
    // appear in the list of the inherited classes on defining the
    // class implementation. So we can't call "removeItem" here but must
    // remove the graphics item from the drawing scene's item list before
    // the dtor of class QGraphicsItem is called. And this is only always
    // the case in the dtor of the class derived from QGraphicsItem.

    //QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    //removeItem(pGraphicsItem);

    if( m_dctpGraphObjs.contains(i_strObjId) )
    {
        emit graphObjDestroying(i_strObjId);

        m_dctpGraphObjs.remove(i_strObjId);

        if( m_dctpGraphObjsClipboard.contains(i_strObjId) )
        {
            m_dctpGraphObjs.remove(i_strObjId);
        }
    }

    if( m_pGraphObjCreating != nullptr && m_pGraphObjCreating->getObjId() == i_strObjId )
    {
        m_pGraphicsItemCreating = nullptr;
        m_pGraphObjCreating = nullptr;
    }

    if( m_pGraphObjAddingShapePoints != nullptr && m_pGraphObjAddingShapePoints->getObjId() == i_strObjId )
    {
        m_pGraphicsItemAddingShapePoints = nullptr;
        m_pGraphObjAddingShapePoints = nullptr;
    }

    if( m_arpGraphicsItemsAcceptingHoverEvents.size() > 0 )
    {
        QGraphicsItem* pGraphicsItem;
        CGraphObj*     pGraphObj;
        int            idxGraphObj;

        for( idxGraphObj = m_arpGraphicsItemsAcceptingHoverEvents.size()-1; idxGraphObj >= 0; idxGraphObj-- )
        {
            pGraphicsItem = m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj];
            pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

            if( pGraphObj != nullptr && pGraphObj->getObjId() == i_strObjId )
            {
                m_arpGraphicsItemsAcceptingHoverEvents.removeAt(idxGraphObj);
            }
        }

    } // if( m_arpGraphicsItemsAcceptingHoverEvents.size() > 0 )

} // onGraphObjDestroying

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjDestroyed( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObjId:" + i_strObjId;
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
    // dtor of QGraphicsItem has already been executed. The order the dtors
    // of inherited classes are called depends on the order the classes
    // appear in the list of the inherited classes on defining the
    // class implementation. So we can't call "removeItem" here but must
    // remove the graphics item from the drawing scene's item list before
    // the dtor of class QGraphicsItem is called. And this is only always
    // the case in the dtor of the class derived from QGraphicsItem.

    //QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    //removeItem(pGraphicsItem);

    emit graphObjDestroyed(i_strObjId);

} // onGraphObjDestroyed

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjIdChanged( const QString& i_strObjIdOld, const QString& i_strObjIdNew )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObjIdOld:" + i_strObjIdOld;
        strAddTrcInfo += ", GraphObjIdNew:" + i_strObjIdNew;
        strAddTrcInfo += ", ChangedByMyself:" + bool2Str(m_bGraphObjIdChangedByMyself);
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjIdChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( !m_bGraphObjIdChangedByMyself )
    {
        CGraphObj* pGraphObj = nullptr;

        if( m_dctpGraphObjs.contains(i_strObjIdOld) )
        {
            pGraphObj = m_dctpGraphObjs.value(i_strObjIdOld);
        }

        // Please note that this method may also be called as a reentry on creating a new
        // object with an ambiguous name whereupon the drawing scene corrects the name
        // of the object.

        // If the graphical object has a valid object id ..
        if( pGraphObj != nullptr )
        {
            // Before this slot is called the object must have gotten its new object id.
            if( pGraphObj->getObjId() != i_strObjIdNew )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidObjId, pGraphObj->getObjId() + " != " + i_strObjIdNew );
            }

            QString strObjIdNew = i_strObjIdNew;

            if( m_dctpGraphObjs.contains(strObjIdNew) )
            {
                strObjIdNew = FindUniqueGraphObjId( m_dctpGraphObjs, strObjIdNew );

                // As the new object id is not yet inserted in the dictionary
                // the "onGraphObjIdChanged" method (called as a reentry)
                // returns without doing anything.
                pGraphObj->setObjId(strObjIdNew);
            }

            // Update sorted map:
            //-------------------

            m_dctpGraphObjs.remove(i_strObjIdOld);
            m_dctpGraphObjs.insert(strObjIdNew,pGraphObj);

            emit graphObjIdChanged(i_strObjIdOld,strObjIdNew);

        } // if( pGraphObj != nullptr )

    } // if( !m_bGraphObjIdChangedByMyself )

} // onGraphObjIdChanged

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjNameChanged(
    const QString& i_strObjId,          // !!! Contains the OLD object id   !!!
    const QString& i_strObjNameOld,     // !!! NOT including name of parents !!!
    const QString& i_strObjNameNew )    // !!! NOT including name of parents !!!
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObjId:" + i_strObjId;
        strAddTrcInfo += ", GraphObjNameOld:" + i_strObjNameOld;
        strAddTrcInfo += ", GraphObjNameNew:" + i_strObjNameNew;
        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjNameChanged",
        /* strAddInfo   */ strAddTrcInfo );

    emit graphObjNameChanged(i_strObjId,i_strObjNameOld,i_strObjNameNew);

} // onGraphObjNameChanged

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QGraphicsItem* CDrawingScene::findGraphicsItem( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    QGraphicsItem* pGraphicsItem = nullptr;

    if( m_dctpGraphObjs.contains(i_strObjId) )
    {
        CGraphObj* pGraphObj = m_dctpGraphObjs.value(i_strObjId);
        pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
    }
    return pGraphicsItem;

} // findGraphicsItem

//------------------------------------------------------------------------------
CGraphObj* CDrawingScene::findGraphObj( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = nullptr;

    if( m_dctpGraphObjs.contains(i_strObjId) )
    {
        pGraphObj = m_dctpGraphObjs.value(i_strObjId);
    }
    return pGraphObj;

} // findGraphObj

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawingScene::findUniqueGraphObjName( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strObjName = i_pGraphObj->getObjName();
    int     iObjNr = 1;
    QString strObjNr;

    // Remove trailing number (if any).
    if( strObjName.length() > 0 && isDigitChar(strObjName[strObjName.length()-1]) )
    {
        while( isDigitChar(strObjName[strObjName.length()-1]) )
        {
            strObjNr.insert( 0, strObjName[strObjName.length()-1] );
            strObjName.remove( strObjName.length()-1, 1 );
        }
        iObjNr = strObjNr.toInt();
    }
    else
    {
        strObjNr = QString::number(iObjNr);
    }

    QString strObjNameParent;

    if( i_pGraphObj->getParent() != nullptr )
    {
        strObjNameParent = i_pGraphObj->getParent()->getObjName( true, m_strGraphObjNameSeparator );
    }

    QString strObjBaseName = strObjName;

    if( strObjNameParent.isEmpty() )
    {
        strObjName = strObjBaseName + strObjNr;
    }
    else
    {
        strObjName = strObjNameParent + m_strGraphObjNameSeparator + strObjBaseName + strObjNr;
    }

    QList<QGraphicsItem*> arpGraphicsItems = items();
    QGraphicsItem*        pGraphicsItem;
    CGraphObj*            pGraphObj;
    int                   idxGraphObj;
    bool                  bUniqueName = false;

    while( !bUniqueName )
    {
        bUniqueName = true;

        for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
        {
            pGraphicsItem = arpGraphicsItems[idxGraphObj];
            pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

            if( pGraphObj != nullptr && pGraphObj != i_pGraphObj )
            {
                if( strObjName.compare(pGraphObj->getObjName(true,m_strGraphObjNameSeparator),Qt::CaseInsensitive) == 0 )
                {
                    bUniqueName = false;
                    break;
                }
            }

        } // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )

        if( !bUniqueName )
        {
            iObjNr++;
            strObjNr = QString::number(iObjNr);

            if( strObjNameParent.isEmpty() )
            {
                strObjName = strObjBaseName + strObjNr;
            }
            else
            {
                strObjName = strObjNameParent + m_strGraphObjNameSeparator + strObjBaseName + strObjNr;
            }
        }

    } // while( !bUniqueName )

    return strObjName;

} // findUniqueGraphObjName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDrawingScene::groupGraphObjsSelected()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "groupGraphObjsSelected",
        /* strAddInfo   */ strAddTrcInfo );

    int iObjsGroupedCount = 0;

    QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();

    if( arpGraphicsItemsSelected.size() > 1 )
    {
        CObjFactory* pObjFactoryTmp = CObjFactory::FindObjFactory(
            /* strNameSpace */ "ZS::Draw",
            /* strClassName */ "CGraphObjGroup",
            /* strObjType   */ graphObjType2Str(EGraphObjTypeGroup) );

        CObjFactoryGroup* pObjFactoryGroup = dynamic_cast<CObjFactoryGroup*>(pObjFactoryTmp);
        CGraphObjGroup*   pGraphObjGroup = nullptr;

        if( pObjFactoryGroup != nullptr )
        {
            EEditTool editToolPrev = m_editTool;

            m_editTool = EEditToolCreateObjects;

            m_pGraphObjCreating = pObjFactoryGroup->createGraphObj(
                /* pDrawingScene */ this,
                /* ptItemPos     */ QPointF(0.0,0.0),
                /* drawSettings  */ m_drawSettings );

            m_pGraphicsItemCreating = dynamic_cast<QGraphicsItem*>(m_pGraphObjCreating);

            if( m_pGraphicsItemCreating == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
            }

            pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pGraphicsItemCreating);

            if( pGraphObjGroup == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjGroup == nullptr" );
            }

            // Add new (empty) group to graphics scene.
            addItem(pGraphObjGroup);

            // Start creation of group.
            pGraphObjGroup->setEditMode(EEditModeCreating);

            // Child objects are positioned relative to the top left corner of the
            // bounding rectangle of the parent object. On newly creating an object
            // they will be positioned relative to the top left corner of the drawing.
            // On adding the object to a parent group object they will be positioned
            // relative to the top left corner of the group's bounding rectangle.

            QGraphicsItem* pGraphicsItem;
            CGraphObj*     pGraphObj;
            int            idxGraphObj;
            QRectF         rctGroupSceneCoors;
            QPointF        posItem;
            QSizeF         sizItem;
            QRectF         rctItem;
            double         fXLeftMin   = INT_MAX;
            double         fYTopMin    = INT_MAX;
            double         fXRightMax  = INT_MIN;
            double         fYBottomMax = INT_MIN;

            //SGraphObjAlignment alignmentWidth(
            //    /* refChild  */ EAlignmentRefWidth,
            //    /* refParent */ EAlignmentRefWidth,
            //    /* bAbsolute */ false );
            //SGraphObjAlignment alignmentHeight(
            //    /* refChild  */ EAlignmentRefHeight,
            //    /* refParent */ EAlignmentRefHeight,
            //    /* bAbsolute */ false );
            //SGraphObjAlignment alignmentLeft(
            //    /* refChild  */ EAlignmentRefLeft,
            //    /* refParent */ EAlignmentRefLeft,
            //    /* bAbsolute */ false );
            //SGraphObjAlignment alignmentTop(
            //    /* refChild  */ EAlignmentRefTop,
            //    /* refParent */ EAlignmentRefTop,
            //    /* bAbsolute */ false );

            //// First deselect all child items so that the boundingRect does not
            //// include the selection points of the child items.
            //for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            //{
            //    pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
            //    pGraphicsItem->setSelected(false);
            //}

            // ConnectionLines are treated special. If one of the line's connection points
            // don't belong to the group the connection line also does not belong to the group.
            // If both connection points of a connection line are selected and will be grouped
            // the connection line may not have been selected.
            // Should connection lines belong to groups at all?
            // No, not directly. They are linked to connection points. If a connection point
            // is part of a group also the connection line is part of the group. And if the
            // line's connection points belong to different groups the connection line
            // indirectly belongs to two different groups.

            // Calculate resulting bounding rectangle of group (without selection rectangle and selection points).
            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];

                if( pGraphicsItem->type() != EGraphObjTypeConnectionLine )
                {
                    pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    posItem = pGraphObj->getPos();
                    sizItem = pGraphObj->getSize();
                    rctItem = QRectF(posItem,sizItem);

                    if( rctItem.width() >= 0.0 )
                    {
                        if( rctItem.left() < fXLeftMin )
                        {
                            fXLeftMin = rctItem.left();
                        }
                        if( rctItem.right() > fXRightMax )
                        {
                            fXRightMax = rctItem.right();
                        }
                    }
                    else
                    {
                        if( rctItem.right() < fXLeftMin )
                        {
                            fXLeftMin = rctItem.right();
                        }
                        if( rctItem.left() > fXRightMax )
                        {
                            fXRightMax = rctItem.left();
                        }
                    }

                    if( rctItem.height() >= 0.0 )
                    {
                        if( rctItem.top() < fYTopMin )
                        {
                            fYTopMin = rctItem.top();
                        }
                        if( rctItem.bottom() > fYBottomMax )
                        {
                            fYBottomMax = rctItem.bottom();
                        }
                    }
                    else
                    {
                        if( rctItem.bottom() < fYTopMin )
                        {
                            fYTopMin = rctItem.bottom();
                        }
                        if( rctItem.top() > fYBottomMax )
                        {
                            fYBottomMax = rctItem.top();
                        }
                    }
                }
            }

            rctGroupSceneCoors.setLeft(fXLeftMin);
            rctGroupSceneCoors.setTop(fYTopMin);
            rctGroupSceneCoors.setRight(fXRightMax);
            rctGroupSceneCoors.setBottom(fYBottomMax);

            pGraphObjGroup->setPos( rctGroupSceneCoors.topLeft() );

            // Add child items to group.
            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];

                if( pGraphicsItem->type() != EGraphObjTypeConnectionLine )
                {
                    pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObj != nullptr )
                    {
                        //posItem = pGraphObj->getPos(); // for debugging purposes also called here before adding the item to the group
                        //sizItem = pGraphObj->getSize();

                        pGraphObjGroup->addGraphObj(pGraphObj);

                        //posItem = pGraphObj->getPos();
                        //sizItem = pGraphObj->getSize();

                        //alignmentLeft.m_fVal = 0.0;
                        //alignmentTop.m_fVal = 0.0;
                        //alignmentWidth.m_fVal = 0.0;
                        //alignmentHeight.m_fVal = 0.0;

                        //if( rctGroupSceneCoors.width() != 0.0 )
                        //{
                        //    alignmentLeft.m_fVal = posItem.x() / rctGroupSceneCoors.width();
                        //}
                        //if( rctGroupSceneCoors.height() != 0.0 )
                        //{
                        //    alignmentTop.m_fVal = posItem.y() / rctGroupSceneCoors.height();
                        //}
                        //if( rctGroupSceneCoors.width() != 0.0 )
                        //{
                        //    alignmentWidth.m_fVal = sizItem.width() / rctGroupSceneCoors.width();
                        //}
                        //if( rctGroupSceneCoors.height() != 0.0 )
                        //{
                        //    alignmentHeight.m_fVal = sizItem.height() / rctGroupSceneCoors.height();
                        //}

                        //// The alignments will be adjusted in the order they are added. The order
                        //// takes effect on the result. Usually the size should be adjusted before
                        //// the positions to get relative adjustments working as expected.
                        //pGraphObj->addAlignment(alignmentWidth);
                        //pGraphObj->addAlignment(alignmentHeight);
                        //pGraphObj->addAlignment(alignmentLeft);
                        //pGraphObj->addAlignment(alignmentTop);

                        iObjsGroupedCount++;

                    } // if( pGraphObj != nullptr )
                } // if( pGraphicsItem->type() != EGraphObjTypeConnectionLine )
            } // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )

            // Finish creation of group.

            pGraphObjGroup->setSize( rctGroupSceneCoors.size() );
            pGraphObjGroup->acceptCurrentAsOriginalCoors();
            pGraphObjGroup->setEditMode(EEditModeUndefined);
            pGraphObjGroup->setEditResizeMode(EEditResizeModeUndefined);

            onGraphObjCreated(pGraphObjGroup);

            pGraphObjGroup->setSelected(true);

            m_editTool = editToolPrev;

        } // if( pObjFactoryGroup != nullptr )

    } // if( arpGraphicsItemsSelected.size() > 1 )

    return iObjsGroupedCount;

} // groupGraphObjsSelected

//------------------------------------------------------------------------------
int CDrawingScene::ungroupGraphObjsSelected()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "CurrentMode=" + CEnumMode::toString(m_mode);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 1,
        /* strMethod    */ "ungroupGraphObjsSelected",
        /* strAddInfo   */ strAddTrcInfo );

    int iObjsUngroupedCount = 0;

    QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();

    if( arpGraphicsItemsSelected.size() > 0 )
    {
        QGraphicsItem*        pGraphicsItem;
        CGraphObjGroup*       pGraphObjGroup;
        int                   idxGraphObj;
        QList<QGraphicsItem*> arpGraphicItemsChilds;
        QGraphicsItem*        pGraphicsItemChild;
        CGraphObj*            pGraphObjChild;
        QString               strObjIdOld;
        QString               strObjIdNew;
        int                   idxGraphObjChild;

        for( idxGraphObj = arpGraphicsItemsSelected.size()-1; idxGraphObj >= 0; idxGraphObj-- )
        {
            pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];

            pGraphObjGroup = nullptr;

            if( pGraphicsItem->type() == EGraphObjTypeGroup )
            {
                pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItem);

                if( pGraphObjGroup != nullptr )
                {
                    arpGraphicItemsChilds = pGraphicsItem->childItems();

                    if( arpGraphicItemsChilds.size() > 0 )
                    {
                        for( idxGraphObjChild = arpGraphicItemsChilds.size()-1; idxGraphObjChild >= 0; idxGraphObjChild-- )
                        {
                            pGraphicsItemChild = arpGraphicItemsChilds[idxGraphObjChild];

                            pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);

                            pGraphObjGroup->removeGraphObj(pGraphObjChild);

                            if( pGraphObjChild != nullptr )
                            {
                                strObjIdOld = pGraphObjChild->getObjId();
                                strObjIdNew = pGraphObjChild->getObjName(true);

                                m_dctpGraphObjs.remove(strObjIdOld);
                                m_dctpGraphObjs.insert(strObjIdNew,pGraphObjChild);
                                pGraphObjChild->setObjId(strObjIdNew);

                                emit graphObjIdChanged(strObjIdOld,strObjIdNew);
                            }
                        }
                    }

                    delete pGraphObjGroup;
                    pGraphObjGroup = nullptr;

                    iObjsUngroupedCount++;

                } // if( pGraphObjGroup != nullptr )

            } // if( pGraphicsItem->type() == EGraphObjTypeGroup )

        } // for( idxGraphObj = arpGraphicsItemsSelected.size()-1; idxGraphObj >= 0; idxGraphObj-- )

    } // if( arpGraphicsItemsSelected.size() > 1 )

    return iObjsUngroupedCount;

} // ungroupGraphObjsSelected

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setDrawSettings( const CDrawSettings& i_settings )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDrawSettings",
        /* strAddInfo   */ "" );

    if( i_settings != m_drawSettings )
    {
        m_drawSettings = i_settings;

        emit drawSettingsChanged(m_drawSettings);
    }

} // setDrawSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setPenColor( const QColor& i_clr )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setPenColor",
        /* strAddInfo   */ i_clr.name() );

    if( i_clr != m_drawSettings.getPenColor() )
    {
        m_drawSettings.setPenColor(i_clr);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setPenColor

//------------------------------------------------------------------------------
void CDrawingScene::setPenWidth( int i_iPenWidth )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setPenWidth",
        /* strAddInfo   */ QString::number(i_iPenWidth) );

    if( i_iPenWidth != m_drawSettings.getPenWidth() )
    {
        m_drawSettings.setPenWidth(i_iPenWidth);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setPenWidth

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setLineStyle( ELineStyle i_lineStyle )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineStyle",
        /* strAddInfo   */ lineStyle2Str(i_lineStyle) );

    if( i_lineStyle != m_drawSettings.getLineStyle() )
    {
        m_drawSettings.setLineStyle(i_lineStyle);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setLineStyle

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setFillColor( const QColor& i_clr )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFillColor",
        /* strAddInfo   */ i_clr.name() );

    if( i_clr != m_drawSettings.getFillColor() )
    {
        m_drawSettings.setFillColor(i_clr);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setFillColor

//------------------------------------------------------------------------------
void CDrawingScene::setFillStyle( EFillStyle i_fillStyle )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFillStyle",
        /* strAddInfo   */ fillStyle2Str(i_fillStyle) );

    if( i_fillStyle != m_drawSettings.getFillStyle() )
    {
        m_drawSettings.setFillStyle(i_fillStyle);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setFillStyle

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setLineRecordType( ELineRecordType i_lineRecordType )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineRecordType",
        /* strAddInfo   */ lineRecordType2Str(i_lineRecordType) );

    if( i_lineRecordType != m_drawSettings.getLineRecordType() )
    {
        m_drawSettings.setLineRecordType(i_lineRecordType);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setLineRecordType

//------------------------------------------------------------------------------
void CDrawingScene::setLineExtent( int i_iLineExtent )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineExtent",
        /* strAddInfo   */ QString::number(i_iLineExtent) );

    if( i_iLineExtent != m_drawSettings.getLineExtent() )
    {
        m_drawSettings.setLineExtent(i_iLineExtent);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setLineExtent

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setLineEndStyle( ELinePoint i_linePoint, ELineEndStyle i_endStyle )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndStyle",
        /* strAddInfo   */ linePoint2Str(i_linePoint) + ", " + lineEndStyle2Str(i_endStyle) );

    if( i_endStyle != m_drawSettings.getLineEndStyle(i_linePoint) )
    {
        m_drawSettings.setLineEndStyle(i_linePoint,i_endStyle);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setLineEndStyle

//------------------------------------------------------------------------------
void CDrawingScene::setLineEndBaseLineType( ELinePoint i_linePoint, ELineEndBaseLineType i_baseLineType )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndBaseLineType",
        /* strAddInfo   */ linePoint2Str(i_linePoint) + ", " + lineEndBaseLineType2Str(i_baseLineType) );

    if( i_baseLineType != m_drawSettings.getLineEndBaseLineType(i_linePoint) )
    {
        m_drawSettings.setLineEndBaseLineType(i_linePoint,i_baseLineType);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setLineEndBaseLineType

//------------------------------------------------------------------------------
void CDrawingScene::setLineEndFillStyle( ELinePoint i_linePoint, ELineEndFillStyle i_fillStyle )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndFillStyle",
        /* strAddInfo   */ linePoint2Str(i_linePoint) + ", " + lineEndFillStyle2Str(i_fillStyle) );

    if( i_fillStyle != m_drawSettings.getLineEndFillStyle(i_linePoint) )
    {
        m_drawSettings.setLineEndFillStyle(i_linePoint,i_fillStyle);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setLineEndFillStyle

//------------------------------------------------------------------------------
void CDrawingScene::setLineEndWidth( ELinePoint i_linePoint, ELineEndWidth i_width )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndWidth",
        /* strAddInfo   */ linePoint2Str(i_linePoint) + ", " + lineEndWidth2Str(i_width) );

    if( i_width != m_drawSettings.getLineEndWidth(i_linePoint) )
    {
        m_drawSettings.setLineEndWidth(i_linePoint,i_width);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setLineEndWidth

//------------------------------------------------------------------------------
void CDrawingScene::setLineEndLength( ELinePoint i_linePoint, ELineEndLength i_length )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndLength",
        /* strAddInfo   */ linePoint2Str(i_linePoint) + ", " + lineEndLength2Str(i_length) );

    if( i_length != m_drawSettings.getLineEndLength(i_linePoint) )
    {
        m_drawSettings.setLineEndLength(i_linePoint,i_length);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setLineEndLength

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setTextColor( const QColor& i_clr )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextColor",
        /* strAddInfo   */ i_clr.name() );

    if( i_clr != m_drawSettings.getTextColor() )
    {
        m_drawSettings.setTextColor(i_clr);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setTextColor

//------------------------------------------------------------------------------
void CDrawingScene::setTextFont( const QFont& i_fnt )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextFont",
        /* strAddInfo   */ i_fnt.family() );

    if( i_fnt != m_drawSettings.getTextFont() )
    {
        m_drawSettings.setTextFont(i_fnt);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setTextFont

//------------------------------------------------------------------------------
void CDrawingScene::setTextSize( ETextSize i_size )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextSize",
        /* strAddInfo   */ textSize2Str(i_size) );

    if( i_size != m_drawSettings.getTextSize() )
    {
        m_drawSettings.setTextSize(i_size);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setTextSize

//------------------------------------------------------------------------------
void CDrawingScene::setTextStyle( ETextStyle i_style )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextStyle",
        /* strAddInfo   */ textStyle2Str(i_style) );

    if( i_style != m_drawSettings.getTextStyle() )
    {
        m_drawSettings.setTextStyle(i_style);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setTextStyle

//------------------------------------------------------------------------------
void CDrawingScene::setTextEffect( ETextEffect i_effect )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "",
        /* strAddInfo   */ textEffect2Str(i_effect) );

    if( i_effect != m_drawSettings.getTextEffect() )
    {
        m_drawSettings.setTextEffect(i_effect);

        emit drawSettingsChanged(m_drawSettings);
    }

} // setTextEffect

/*==============================================================================
public: // overridables of base class QGraphicsScene
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::dragEnterEvent( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    const QMimeData* pMimeData = i_pEv->mimeData();
    QString strMimeData = pMimeData->text();

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", EvPossibleActions:" + qDropActions2Str(i_pEv->possibleActions());
        strAddTrcInfo += ", EvProposedAction:" + qDropActions2Str(i_pEv->proposedAction());
        strAddTrcInfo += ", EvDropAction:" + qDropActions2Str(i_pEv->dropAction());
        strAddTrcInfo += ", MimeDataText:" + strMimeData;

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragEnterEvent",
        /* strAddInfo   */ strAddTrcInfo );

    if( pMimeData->hasText() )
    {
        i_pEv->accept();
    }
    else if( pMimeData->hasImage() )
    {
        i_pEv->accept();
    }
    else if( pMimeData->hasUrls() )
    {
        i_pEv->accept();
    }
    else
    {
        i_pEv->ignore();
    }

    //QGraphicsScene::dragEnterEvent(i_pEv);

} // dragEnterEvent

//------------------------------------------------------------------------------
void CDrawingScene::dragMoveEvent( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    const QMimeData* pMimeData = i_pEv->mimeData();
    QString strMimeData = pMimeData->text();

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", EvPossibleActions:" + qDropActions2Str(i_pEv->possibleActions());
        strAddTrcInfo += ", EvProposedAction:" + qDropActions2Str(i_pEv->proposedAction());
        strAddTrcInfo += ", EvDropAction:" + qDropActions2Str(i_pEv->dropAction());
        strAddTrcInfo += ", MimeDataText:" + strMimeData;

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    if( pMimeData->hasText() )
    {
        i_pEv->accept();
    }
    else if( pMimeData->hasImage() )
    {
        i_pEv->accept();
    }
    else if( pMimeData->hasUrls() )
    {
        i_pEv->accept();
    }
    else
    {
        i_pEv->ignore();
    }

    //QGraphicsScene::dragMoveEvent(i_pEv);

} // dragMoveEvent

//------------------------------------------------------------------------------
void CDrawingScene::dragLeaveEvent( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    const QMimeData* pMimeData = i_pEv->mimeData();
    QString strMimeData = pMimeData->text();

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", EvPossibleActions:" + qDropActions2Str(i_pEv->possibleActions());
        strAddTrcInfo += ", EvProposedAction:" + qDropActions2Str(i_pEv->proposedAction());
        strAddTrcInfo += ", EvDropAction:" + qDropActions2Str(i_pEv->dropAction());
        strAddTrcInfo += ", MimeDataText:" + strMimeData;

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragLeaveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    if( sceneRect().contains(i_pEv->scenePos()) )
    {
        if( pMimeData->hasText() )
        {
            i_pEv->accept();
        }
        else if( pMimeData->hasImage() )
        {
            i_pEv->accept();
        }
        else if( pMimeData->hasUrls() )
        {
            i_pEv->accept();
        }
        else
        {
            i_pEv->ignore();
        }
    } // if( sceneRect().contains(i_pEv->scenePos()) )

    else // if( !sceneRect().contains(i_pEv->scenePos()) )
    {
        i_pEv->ignore();
    }

    //QGraphicsScene::dragLeaveEvent(i_pEv);

} // dragLeaveEvent

//------------------------------------------------------------------------------
void CDrawingScene::dropEvent( QGraphicsSceneDragDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    const QMimeData* pMimeData = i_pEv->mimeData();
    QString strMimeData;

    if( pMimeData != nullptr )
    {
        strMimeData = pMimeData->text();
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", EvPossibleActions:" + qDropActions2Str(i_pEv->possibleActions());
        strAddTrcInfo += ", EvProposedAction:" + qDropActions2Str(i_pEv->proposedAction());
        strAddTrcInfo += ", EvDropAction:" + qDropActions2Str(i_pEv->dropAction());
        strAddTrcInfo += ", MimeDataText:" + strMimeData;

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dropEvent",
        /* strAddInfo   */ strAddTrcInfo );

    bool bAccepted = false;

    if( pMimeData != nullptr && sceneRect().contains(i_pEv->scenePos()) )
    {
        if( pMimeData->hasText() )
        {
            bAccepted = true;

            QStringList strlstMimeData = strMimeData.split("; ",Qt::SkipEmptyParts);

            if( strlstMimeData.size() > 0 )
            {
                CObjFactory*   pObjFactory;
                QGraphicsItem* pGraphicsItem;
                CGraphObj*     pGraphObj;
                QString        strObj;
                QStringList    strlstObjPath;
                QString        strType;
                QString        strObjPath;
                int            idxObj;

                clearSelection();

                for( idxObj = 0; idxObj < strlstMimeData.size(); idxObj++ )
                {
                    strObj = strlstMimeData[idxObj];

                    strlstObjPath = strObj.split("::");

                    if( strlstObjPath.size() > 1 ) // must contain type and object path
                    {
                        strType = strlstObjPath[0];

                        if( strType.compare("ObjFactory",Qt::CaseInsensitive) == 0 )
                        {
                            strlstObjPath.removeFirst();
                            strObjPath = strlstObjPath.join("::");
                            pObjFactory = CObjFactory::FindObjFactory(strObjPath);

                            if( pObjFactory != nullptr )
                            {
                                pGraphObj = pObjFactory->createGraphObj(
                                    /* pDrawingScene */ this,
                                    /* ptItemPos     */ QPointF(0.0,0.0),
                                    /* drawSettings  */ m_drawSettings );

                                pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

                                if( pGraphicsItem == nullptr )
                                {
                                    throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                                }

                                addItem(pGraphicsItem);

                                pGraphicsItem->setPos( i_pEv->scenePos() );
                                pGraphicsItem->setSelected(true);

                                onGraphObjCreated(pGraphObj);

                                setMode( EMode::Ignore, EEditToolSelect, EEditModeMove, EEditResizeModeUndefined, false );
                            }
                        } // if( strType.compare("ObjFactory",Qt::CaseInsensitive) == 0 )
                    } // if( strlstObjPath.size() > 1 ) // must contain type and object path
                } // for( idxObj = 0; idxObj < strlstMimeData.size(); idxObj++ )
            } // if( strlstMimeData.size() > 0 )
        } // if( pMimeData->hasText() )

        else if( pMimeData->hasImage() )
        {
            bAccepted = true;

        } // if( pMimeData->hasImage() )

        else if( pMimeData->hasUrls() )
        {
            bAccepted = true;

            QList<QUrl>     arurls = pMimeData->urls();
            QUrl            url;
            int             idxUrl;
            QString         strFilePath;
            CGraphObjImage* pGraphObjImage;

            for( idxUrl = 0; idxUrl < arurls.size(); idxUrl++ )
            {
                url = arurls[idxUrl];

                strFilePath = url.toLocalFile();

                pGraphObjImage = new CGraphObjImage(this,m_drawSettings);

                pGraphObjImage->setImageFilePath(strFilePath);

                addItem(pGraphObjImage);

                pGraphObjImage->setPos( i_pEv->scenePos() );

                onGraphObjCreated(pGraphObjImage);

                setMode( EMode::Ignore, EEditToolSelect, EEditModeMove, EEditResizeModeUndefined, false );
            }

        } // if( pMimeData->hasUrls() )

    } // if( sceneRect().contains(i_pEv->scenePos()) )

    if( bAccepted )
    {
        i_pEv->accept();
    }
    else
    {
        i_pEv->ignore();
    }

    //QGraphicsScene::dropEvent(i_pEv);

} // dropEvent

/*==============================================================================
public: // overridables of base class QGraphicsScene
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    bool bEventHandled = false;

    QPointF ptMouseScenePos = i_pEv->scenePos();

    m_ptMouseEvScenePosOnMousePressEvent = ptMouseScenePos;

    emit mousePosChanged(m_ptMouseEvScenePosOnMousePressEvent);

    int iObjFactoryType = EGraphObjTypeUndefined;

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }

    if( m_mode != EMode::Edit )
    {
        if( m_pGraphicsItemCreating != nullptr || iObjFactoryType != EGraphObjTypeUndefined )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInvalidMethodCall);
        }
    }

    if( m_mode == EMode::Edit )
    {
        // If currently an object is "under construction" ...
        if( m_pGraphicsItemCreating != nullptr )
        {
            // ... dispatch mouse event to object "under construction".
            QGraphicsSceneMouseEvent evMouse(QEvent::GraphicsSceneMousePress);

            QPointF ptMouseItemPos = m_pGraphicsItemCreating->mapFromScene(i_pEv->scenePos());
            QPointF ptMouseItemPosLast = m_pGraphicsItemCreating->mapFromScene(i_pEv->lastScenePos());

            evMouse.setWidget(i_pEv->widget());

            evMouse.setPos(ptMouseItemPos);
            evMouse.setLastPos(ptMouseItemPosLast);

            evMouse.setScenePos(i_pEv->scenePos());
            evMouse.setLastScenePos(i_pEv->lastScenePos());

            evMouse.setScreenPos(i_pEv->screenPos());
            evMouse.setLastScreenPos(i_pEv->lastScreenPos());

            evMouse.setButton(i_pEv->button());
            evMouse.setButtons(i_pEv->buttons());
            evMouse.setModifiers(i_pEv->modifiers());

            sendEvent( m_pGraphicsItemCreating, &evMouse );

            bEventHandled = true;

        } // if( m_pGraphicsItemCreating != nullptr )

        // If currently an object is "under construction" ...
        else if( m_pGraphicsItemAddingShapePoints != nullptr )
        {
            // ... dispatch mouse event to object "under construction".
            QGraphicsSceneMouseEvent evMouse(QEvent::GraphicsSceneMousePress);

            QPointF ptMouseItemPos = m_pGraphicsItemAddingShapePoints->mapFromScene(i_pEv->scenePos());
            QPointF ptMouseItemPosLast = m_pGraphicsItemAddingShapePoints->mapFromScene(i_pEv->lastScenePos());

            evMouse.setWidget(i_pEv->widget());

            evMouse.setPos(ptMouseItemPos);
            evMouse.setLastPos(ptMouseItemPosLast);

            evMouse.setScenePos(i_pEv->scenePos());
            evMouse.setLastScenePos(i_pEv->lastScenePos());

            evMouse.setScreenPos(i_pEv->screenPos());
            evMouse.setLastScreenPos(i_pEv->lastScreenPos());

            evMouse.setButton(i_pEv->button());
            evMouse.setButtons(i_pEv->buttons());
            evMouse.setModifiers(i_pEv->modifiers());

            sendEvent( m_pGraphicsItemAddingShapePoints, &evMouse );

            bEventHandled = true;

        } // if( m_pGraphicsItemAddingShapePoints != nullptr )

        // If currently no object is "under construction" ...
        else // if( m_pGraphicsItemCreating == nullptr && m_pGraphicsItemAddingShapePoints == nullptr )
        {
            if( m_editTool == EEditToolCreateObjects )
            {
                // Any drawing tool selected?
                if( m_pObjFactory != nullptr )
                {
                    bool bCreateObj = true;

                    CGraphObjConnectionLine*  pGraphObjCnctLineCreating = nullptr;
                    CGraphObjConnectionPoint* pGraphObjCnctPtCreating = nullptr;
                    CGraphObjConnectionPoint* pGraphObjCnctPtHit = nullptr;

                    // Connection lines may only be created on connection points. So a
                    // graphical object (a connection point) must have been hit to create
                    // connection lines.
                    if( iObjFactoryType == EGraphObjTypeConnectionLine )
                    {
                        // Check whether a connection point has been hit.
                        pGraphObjCnctPtHit = getConnectionPoint(m_ptMouseEvScenePosOnMousePressEvent);

                        if( pGraphObjCnctPtHit == nullptr )
                        {
                            bCreateObj = false;
                        }
                    }

                    if( bCreateObj )
                    {
                        clearSelection();

                        // Graphical objects must always created at their transformation origin points.
                        // Otherwise mapping coordinates to group coordinates does not work correctly.
                        // In special this applies to connection points and circles.
                        m_pGraphObjCreating = m_pObjFactory->createGraphObj(
                            /* pDrawingScene */ this,
                            /* ptItemPos     */ QPointF(0.0,0.0),
                            /* drawSettings  */ m_drawSettings );

                        if( m_pGraphObjCreating != nullptr )
                        {
                            m_pGraphicsItemCreating = dynamic_cast<QGraphicsItem*>(m_pGraphObjCreating);

                            if( m_pGraphicsItemCreating == nullptr )
                            {
                                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                            }

                            addItem(m_pGraphicsItemCreating);

                            m_pGraphObjCreating->setEditMode(EEditModeCreating);

                            if( iObjFactoryType == EGraphObjTypeConnectionLine && m_pGraphicsItemCreating->type() == EGraphObjTypeConnectionLine )
                            {
                                pGraphObjCnctLineCreating = dynamic_cast<CGraphObjConnectionLine*>(m_pGraphObjCreating);
                            }
                            else if( iObjFactoryType == EGraphObjTypeConnectionPoint && m_pGraphicsItemCreating->type() == EGraphObjTypeConnectionPoint )
                            {
                                pGraphObjCnctPtCreating = dynamic_cast<CGraphObjConnectionPoint*>(m_pGraphObjCreating);
                            }

                            if( pGraphObjCnctPtCreating != nullptr )
                            {
                                // Need to set left top position of connection points as transformation origin point.
                                // Otherwise mapping coordinates to group coordinates does not work correctly.
                                QRectF  rctGraphObjCnctPt = pGraphObjCnctPtCreating->rect();
                                QPointF ptPosGraphObjCnctPt = QPointF(
                                    m_ptMouseEvScenePosOnMousePressEvent.x() - rctGraphObjCnctPt.width()/2.0,
                                    m_ptMouseEvScenePosOnMousePressEvent.y() - rctGraphObjCnctPt.height()/2.0 );
                                pGraphObjCnctPtCreating->setPos(ptPosGraphObjCnctPt);
                            }
                            else
                            {
                                m_pGraphicsItemCreating->setPos(m_ptMouseEvScenePosOnMousePressEvent);
                            }

                            m_pGraphicsItemCreating->setSelected(true);
                            m_pGraphicsItemCreating->setAcceptHoverEvents(true);

                            if( pGraphObjCnctLineCreating != nullptr && pGraphObjCnctPtHit != nullptr )
                            {
                                pGraphObjCnctLineCreating->setConnectionPoint(ELinePointStart,pGraphObjCnctPtHit);
                            }

                            emit modeChanged();

                        } // if( m_pGraphicsItemCreating != nullptr )
                    } // if( bCreateObj )
                } // if( m_pObjFactory != nullptr )

                // If an object has been newly created and is still under construction ...
                if( m_pGraphicsItemCreating != nullptr )
                {
                    // ... dispatch mouse event to object "under construction".
                    QGraphicsSceneMouseEvent evMouse(QEvent::GraphicsSceneMousePress);

                    QPointF ptMouseItemPos = m_pGraphicsItemCreating->mapFromScene(i_pEv->scenePos());
                    QPointF ptMouseItemPosLast = m_pGraphicsItemCreating->mapFromScene(i_pEv->lastScenePos());

                    evMouse.setWidget(i_pEv->widget());

                    evMouse.setPos(ptMouseItemPos);
                    evMouse.setLastPos(ptMouseItemPosLast);

                    evMouse.setScenePos(i_pEv->scenePos());
                    evMouse.setLastScenePos(i_pEv->lastScenePos());

                    evMouse.setScreenPos(i_pEv->screenPos());
                    evMouse.setLastScreenPos(i_pEv->lastScreenPos());

                    evMouse.setButton(i_pEv->button());
                    evMouse.setButtons(i_pEv->buttons());
                    evMouse.setModifiers(i_pEv->modifiers());

                    sendEvent( m_pGraphicsItemCreating, &evMouse );

                    bEventHandled = true;

                } // if( m_pGraphicsItemCreating != nullptr )

            } // if( m_editTool == EEditToolCreateObjects )

        } // if( m_pGraphicsItemCreating == nullptr && m_pGraphicsItemAddingShapePoints == nullptr )

    } // if( m_mode == EMode::Edit )

    if( !bEventHandled )
    {
        // Dispatch mouse move events to objects under mouse cursor.
        // Some items may completely overlap (encircle) other objects
        // (a big rectangle may completely enclose a smaller rectangle
        // or the bounding rectangle of a polyline may enclose other
        // objects). In this case Qt's graphic scene does not dispatch
        // the mouse events as hover events to the enclosed objects if
        // they are not "on top" (according to the scene's item list
        // Z order) of the enclosing items. If the outer object does not
        // hide the inner object (fill style solid pattern)this behavior
        // is not what we want and what the user expects. E.g. if a rectangle
        // with "FillStyle = NoFill" would enclose another object the user
        // would expect that the inner object can be selected by mouse
        // clicks. And if you consider polylines which are never "filled"
        // objects the inner objects should always be selectable by mouse
        // clicks. The polyline as the outer object should only be selected
        // if one of its line segments would be hit.

        QList<QGraphicsItem*> arpGraphicsItemsIntersected = items( i_pEv->scenePos(), Qt::IntersectsItemShape, Qt::AscendingOrder );
        QList<QGraphicsItem*> arpGraphicsItemsHit;
        QList<QGraphicsItem*> arpGraphicsItemsBroughtToFront;
        QPointF               ptMouseItemPos;
        SGraphObjHitInfo      graphObjHitInfo;
        QGraphicsItem*        pGraphicsItem;
        CGraphObj*            pGraphObj;
        int                   idxGraphObj;
        bool                  bGraphObjHit;

        if( arpGraphicsItemsIntersected.size() > 0 )
        {
            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsIntersected.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItemsIntersected[idxGraphObj];
                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                if( pGraphicsItem != nullptr && pGraphObj != nullptr )
                {
                    ptMouseItemPos = pGraphicsItem->mapFromScene(ptMouseScenePos);

                    // Selected graphic items are considered as being hit if it has been clicked
                    // inside their bounding rectangle. But if selected items overlap other items
                    // whose shape edge points or bounding rectangles have been clicked the mouse
                    // events should be passed through to those objects.
                    bGraphObjHit = pGraphObj->isHit(ptMouseItemPos,&graphObjHitInfo);

                    if( bGraphObjHit )
                    {
                        arpGraphicsItemsHit.append(pGraphicsItem);
                    }
                }
            }
        } // if( arpGraphicsItemsIntersected.size() > 0 )

        // Temporarily bring all objects to front which have been hit so that
        // Qt dispatches the mouse press events to those objects. We use several
        // loops to achieve the following order (items at the end of the list are
        // "above" the objects listed before and will receive the mouse events).
        // Please note that the second outer loop for selected objects will be
        // executed also in simulation mode even if unnecessary as in simulation
        // mode there will be no selected objects.
        //
        //   1. Not selected objects
        //     1.1. Graphics Items
        //     1.2. Selection Points of Graphics Items
        //     1.3. Labels of Graphics Items
        //     1.4. Connection Points
        //     1.5. Groups (only in simulation mode)
        //   2. Selected objects
        //     2.1. Graphics Items
        //     2.2. Selection Points of Graphics Items
        //     2.3. Labels of Graphics Items
        //     2.4. Connection Points
        //     2.5. Groups (only in simulation mode)

        const int c_iNotSelectedObjects = 0;
        const int c_iSelectedObjects    = 1;
        const int c_iOuterLoopCount     = 2;

        const int c_iGraphicsItems      = 0;
        const int c_iSelectionPoints    = 1;
        const int c_iLabels             = 2;
        const int c_iConnectionPoints   = 3;
        const int c_iGroups             = 4;
        const int c_iInnerLoopCount     = 5;

        if( arpGraphicsItemsHit.size() > 0 )
        {
            int  idxObjType;
            int  idxSel;
            bool bBringToFront;

            for( idxSel = 0; idxSel < c_iOuterLoopCount; idxSel++ )
            {
                for( idxObjType = 0; idxObjType < c_iInnerLoopCount; idxObjType++ )
                {
                    for( idxGraphObj = arpGraphicsItemsHit.size()-1; idxGraphObj >= 0; idxGraphObj-- )
                    {
                        bBringToFront = false;

                        pGraphicsItem = arpGraphicsItemsHit[idxGraphObj];
                        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                        if( pGraphicsItem != nullptr && pGraphObj != nullptr )
                        {
                            if( idxSel == c_iNotSelectedObjects )
                            {
                                if( !pGraphicsItem->isSelected() )
                                {
                                    if( idxObjType == c_iGraphicsItems )
                                    {
                                        if( pGraphicsItem->type() != EGraphObjTypeSelectionPoint
                                         && pGraphicsItem->type() != EGraphObjTypeLabel
                                         && pGraphicsItem->type() != EGraphObjTypeConnectionPoint
                                         && pGraphicsItem->type() != EGraphObjTypeGroup )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iSelectionPoints )
                                    {
                                        if( pGraphicsItem->type() == EGraphObjTypeSelectionPoint )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iLabels )
                                    {
                                        if( pGraphicsItem->type() == EGraphObjTypeLabel )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iConnectionPoints )
                                    {
                                        if( pGraphicsItem->type() == EGraphObjTypeConnectionPoint )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iGroups )
                                    {
                                        if( pGraphicsItem->type() == EGraphObjTypeGroup )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                }
                            }
                            else if( idxSel == c_iSelectedObjects )
                            {
                                // As mentioned above: the second outer loop for selected objects will be
                                // executed also in simulation mode even if unnecessary as in simulation
                                // mode there will be no selected objects.
                                if( pGraphicsItem->isSelected() )
                                {
                                    if( idxObjType == c_iGraphicsItems )
                                    {
                                        if( pGraphicsItem->type() != EGraphObjTypeSelectionPoint
                                         && pGraphicsItem->type() != EGraphObjTypeLabel
                                         && pGraphicsItem->type() != EGraphObjTypeConnectionPoint
                                         && pGraphicsItem->type() != EGraphObjTypeGroup )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iSelectionPoints )
                                    {
                                        if( pGraphicsItem->type() == EGraphObjTypeSelectionPoint )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iLabels )
                                    {
                                        if( pGraphicsItem->type() == EGraphObjTypeLabel )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iConnectionPoints )
                                    {
                                        if( pGraphicsItem->type() == EGraphObjTypeConnectionPoint )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iGroups )
                                    {
                                        if( pGraphicsItem->type() == EGraphObjTypeGroup )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                }
                            }

                            if( bBringToFront )
                            {
                                bringToFront( pGraphicsItem, arpGraphicsItemsIntersected );
                                pGraphicsItem->setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
                                arpGraphicsItemsBroughtToFront.append(pGraphicsItem);
                            }
                        }
                    }
                }
            }

        } // if( arpGraphicsItemsHit.size() > 0 )

        // Dispatch mouse press event to objects which have been hit.
        QGraphicsScene::mousePressEvent(i_pEv);

        // After the mouse event has been dispatched the Z-order of the objects
        // temporarily brought to front must be restored.
        for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsBroughtToFront.size(); idxGraphObj++ )
        {
            pGraphicsItem = arpGraphicsItemsBroughtToFront[idxGraphObj];
            pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

            if( pGraphicsItem != nullptr && pGraphObj != nullptr )
            {
                pGraphicsItem->setZValue( pGraphObj->getStackingOrderValue() );
            }
        }

        // In edit mode ..
        if( m_mode == EMode::Edit )
        {
            // If no item has been hit ...
            if( selectedItems().size() == 0 )
            {
                setMode( EMode::Ignore, EEditToolIgnore, EEditModeUndefined, EEditResizeModeUndefined, false );

                QRectF rctSelectionArea(
                    /* x      */ m_ptMouseEvScenePosOnMousePressEvent.x(),
                    /* y      */ m_ptMouseEvScenePosOnMousePressEvent.y(),
                    /* width  */ 1,
                    /* height */ 1 );

                if( m_pGraphicsItemSelectionArea == nullptr )
                {
                    QPen penSelectionArea(Qt::DotLine);
                    penSelectionArea.setColor(Qt::black);

                    QBrush brsSelectionArea(Qt::SolidPattern);
                    int iHue   = 240; // 0..359
                    int iSat   =  63; // 0..255
                    int iVal   = 127; // 0..255
                    int iAlpha =  63; // 0..255
                    brsSelectionArea.setColor( QColor::fromHsv(iHue,iSat,iVal,iAlpha) );

                    m_pGraphicsItemSelectionArea = addRect(
                        /* rect   */ rctSelectionArea,
                        /* pen    */ penSelectionArea,
                        /* brush  */ brsSelectionArea );
                }
            }

        } // if( m_mode == EMode::Edit )

    } // if( !bEventHandled )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";

        mthTracer.trace(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CDrawingScene::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().x()) + ")";
        strAddTrcInfo += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().x()) + ")";
        strAddTrcInfo += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().x()) + ")";

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 2,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    int iMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;

    QPointF ptMouseScenePos = i_pEv->scenePos();

    emit mousePosChanged(ptMouseScenePos);

    int iObjFactoryType = EGraphObjTypeUndefined;

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }

    if( m_mode != EMode::Edit )
    {
        if( m_pGraphicsItemCreating != nullptr || iObjFactoryType != EGraphObjTypeUndefined )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInvalidMethodCall);
        }
    }

    // What has to be done on moving the mouse while the left button is pressed depends
    // on the activated drawing tool and whether an object is currently under construction
    // or selected. If an object is selected the edit mode must have been set
    // correspondingly on receiving the prior mouse press event.
    // If the drawing tool Point is activated a new object will be created at the current
    // mouse position no matter whether an object is under construction or not.
    // If a polyline or polygon is under construction new line segments will be appended to
    // the previously created line segment (free hand painting).
    // If no drawing tool is activated and no object is under construction the selected
    // objects will be changed depending on the edit mode set by the prior mouse press event.
    // If no drawing tool is activated and no object is selected the mouse move
    // event is used to select a rectangle area within the drawing selecting all objects
    // within this area later on receiving the mouse release event.

    if( m_mode == EMode::Edit )
    {
        // If currently an object is "under construction" ...
        if( m_pGraphicsItemCreating != nullptr )
        {
            // ... dispatch mouse event to object "under construction".
            QGraphicsSceneMouseEvent evMouse(QEvent::GraphicsSceneMouseMove);

            QPointF ptMouseItemPos = m_pGraphicsItemCreating->mapFromScene(i_pEv->scenePos());
            QPointF ptMouseItemPosLast = m_pGraphicsItemCreating->mapFromScene(i_pEv->lastScenePos());

            evMouse.setWidget(i_pEv->widget());

            evMouse.setPos(ptMouseItemPos);
            evMouse.setLastPos(ptMouseItemPosLast);

            evMouse.setScenePos(i_pEv->scenePos());
            evMouse.setLastScenePos(i_pEv->lastScenePos());

            evMouse.setScreenPos(i_pEv->screenPos());
            evMouse.setLastScreenPos(i_pEv->lastScreenPos());

            evMouse.setButtons(i_pEv->buttons());
            evMouse.setModifiers(i_pEv->modifiers());
            evMouse.setAccepted(i_pEv->isAccepted());

            sendEvent( m_pGraphicsItemCreating, &evMouse );

        } // if( m_pGraphicsItemCreating != nullptr )

        // If currently an object is "under construction" ...
        else if( m_pGraphicsItemAddingShapePoints != nullptr )
        {
            // ... dispatch mouse event to object "under construction".
            QGraphicsSceneMouseEvent evMouse(QEvent::GraphicsSceneMouseMove);

            QPointF ptMouseItemPos = m_pGraphicsItemAddingShapePoints->mapFromScene(i_pEv->scenePos());
            QPointF ptMouseItemPosLast = m_pGraphicsItemAddingShapePoints->mapFromScene(i_pEv->lastScenePos());

            evMouse.setWidget(i_pEv->widget());

            evMouse.setPos(ptMouseItemPos);
            evMouse.setLastPos(ptMouseItemPosLast);

            evMouse.setScenePos(i_pEv->scenePos());
            evMouse.setLastScenePos(i_pEv->lastScenePos());

            evMouse.setScreenPos(i_pEv->screenPos());
            evMouse.setLastScreenPos(i_pEv->lastScreenPos());

            evMouse.setButtons(i_pEv->buttons());
            evMouse.setModifiers(i_pEv->modifiers());
            evMouse.setAccepted(i_pEv->isAccepted());

            sendEvent( m_pGraphicsItemAddingShapePoints, &evMouse );

        } // if( m_pGraphicsItemAddingShapePoints != nullptr )

        // If currently no object is "under construction" ...
        else // if( m_pGraphicsItemCreating == nullptr && m_pGraphicsItemAddingShapePoints == nullptr )
        {
            bool bDispatchMouseEvents2ObjectsUnderMouseCursor = false;
            bool bDispatchMouseEvents2ConnectionPointsUnderMouseCursor = false;

            if( m_editTool == EEditToolCreateObjects )
            {
                // If the left mouse button is pressed ...
                if( iMouseButtonState & Qt::LeftButton )
                {
                    // If the drawing tool "Point" is selected ...
                    if( m_pObjFactory != nullptr && iObjFactoryType == EGraphObjTypePoint )
                    {
                        // .. we are going to create further points at the current mouse position.
                        m_pGraphObjCreating = m_pObjFactory->createGraphObj(
                            /* pDrawingScene */ this,
                            /* ptItemPos     */ QPointF(0.0,0.0),
                            /* drawSettings  */ m_drawSettings );

                        m_pGraphicsItemCreating = dynamic_cast<QGraphicsItem*>(m_pGraphObjCreating);

                        if( m_pGraphicsItemCreating == nullptr )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                        }

                        addItem(m_pGraphicsItemCreating);

                        m_pGraphObjCreating->setEditMode(EEditModeCreating);

                        m_pGraphicsItemCreating->setPos(ptMouseScenePos);
                        m_pGraphicsItemCreating->setSelected(true);

                        // The current "mouseMove" event is not dispatched to the newly created object but to the
                        // object previously created by the "mousePress" event (even if the previously created
                        // object was explicitly unselected by the "onGraphObjCreated" method). So we call
                        // "onGraphObjCreated" on our own hoping that the object states set by the ctor are
                        // sufficient for the graphical object.
                        onGraphObjCreated(m_pGraphObjCreating);

                    } // if( m_pObjFactory != nullptr && iObjFactoryType == EGraphObjTypePoint )
                } // if( iMouseButtonState & Qt::LeftButton )

                // If no mouse button is pressed ...
                else if( iMouseButtonState == Qt::NoButton )
                {
                    // If the drawing tool "ConnectionLine" is selected ...
                    if( m_pObjFactory != nullptr && iObjFactoryType == EGraphObjTypeConnectionLine )
                    {
                        bDispatchMouseEvents2ConnectionPointsUnderMouseCursor = true;
                    }

                } // if( iMouseButtonState == Qt::NoButton )

            } // if( m_editTool == EEditToolCreateObjects )

            else if( m_editTool == EEditToolSelect )
            {
                // If the left mouse button is pressed ...
                if( iMouseButtonState & Qt::LeftButton )
                {
                    if( m_pGraphicsItemSelectionArea != nullptr )
                    {
                        QRectF rctSelectionArea(
                            /* x      */ m_ptMouseEvScenePosOnMousePressEvent.x(),
                            /* y      */ m_ptMouseEvScenePosOnMousePressEvent.y(),
                            /* width  */ ptMouseScenePos.x() - m_ptMouseEvScenePosOnMousePressEvent.x() + 1,
                            /* height */ ptMouseScenePos.y() - m_ptMouseEvScenePosOnMousePressEvent.y() + 1 );

                        m_pGraphicsItemSelectionArea->setRect(rctSelectionArea);
                    }
                    else
                    {
                        QGraphicsScene::mouseMoveEvent(i_pEv);
                    }

                } // if( iMouseButtonState & Qt::LeftButton )

                // If no mouse button is pressed ...
                else if( iMouseButtonState == Qt::NoButton )
                {
                    bDispatchMouseEvents2ObjectsUnderMouseCursor = true;
                }
            } // if( m_editTool == EEditToolSelect )

            if( bDispatchMouseEvents2ObjectsUnderMouseCursor || bDispatchMouseEvents2ConnectionPointsUnderMouseCursor )
            {
                // Some items may completely overlap (encircle) other objects
                // (a big rectangle may completely enclose a smaller rectangle
                // or the bounding rectangle of a polyline may enclose other
                // objects). In this case Qt's graphic scene does not dispatch
                // the mouse events as hover events to the enclosed objects if
                // they are not "on top" (according to the scene's item list
                // Z order) of the enclosing items. If the outer object does not
                // hide the inner object (fill style solid pattern)this behavior
                // is not what we want and what the user expects. E.g. if a rectangle
                // with "FillStyle = NoFill" would enclose another object the user
                // would expect that the inner object can be selected by mouse
                // clicks. And if you consider polylines which are never "filled"
                // objects the inner objects should always be selectable by mouse
                // clicks. The polyline as the outer object should only be selected
                // if one of its line segments would be hit.

                QList<QGraphicsItem*> arpGraphicsItemsIntersected = items( i_pEv->scenePos(), Qt::IntersectsItemShape, Qt::AscendingOrder );
                QList<QGraphicsItem*> arpGraphicsItemsHit;
                QPointF               ptMouseItemPos;
                SGraphObjHitInfo      graphObjHitInfo;
                QGraphicsItem*        pGraphicsItem;
                QGraphicsItem*        pGraphicsItemHit;
                CGraphObj*            pGraphObj;
                CGraphObj*            pGraphObjHit;
                int                   idxGraphObj;
                int                   idxGraphObjHit;
                bool                  bGraphObjHit;
                bool                  bGraphObjFound;

                // Create array with objects which have been hit by mouse cursor.
                if( arpGraphicsItemsIntersected.size() > 0 )
                {
                    bool bCheckIsHit;

                    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsIntersected.size(); idxGraphObj++ )
                    {
                        pGraphicsItem = arpGraphicsItemsIntersected[idxGraphObj];
                        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                        if( pGraphicsItem != nullptr && pGraphObj != nullptr )
                        {
                            bCheckIsHit = false;

                            switch( pGraphObj->getType() )
                            {
                                case EGraphObjTypeSelectionPoint:
                                {
                                    break;
                                }
                                case EGraphObjTypeConnectionPoint:
                                {
                                    bCheckIsHit = bDispatchMouseEvents2ConnectionPointsUnderMouseCursor || bDispatchMouseEvents2ObjectsUnderMouseCursor;
                                    break;
                                }
                                default:
                                {
                                    if( pGraphicsItem->parentItem() == nullptr )
                                    {
                                        bCheckIsHit = bDispatchMouseEvents2ObjectsUnderMouseCursor;
                                    }
                                    break;
                                }
                            }

                            if( bCheckIsHit )
                            {
                                ptMouseItemPos = pGraphicsItem->mapFromScene(ptMouseScenePos);

                                bGraphObjHit = pGraphObj->isHit(ptMouseItemPos,&graphObjHitInfo);

                                if( bGraphObjHit )
                                {
                                    arpGraphicsItemsHit.append(pGraphicsItem);
                                }
                            }
                        }
                    }
                } // if( arpGraphicsItemsIntersected.size() > 0 )

                // Objects which have been hit by mouse cursor temporarily accept hover events.
                for( idxGraphObjHit = 0; idxGraphObjHit < arpGraphicsItemsHit.size(); idxGraphObjHit++ )
                {
                    pGraphicsItemHit = arpGraphicsItemsHit[idxGraphObjHit];
                    pGraphObjHit = dynamic_cast<CGraphObj*>(pGraphicsItemHit);

                    if( pGraphicsItemHit != nullptr && pGraphObjHit != nullptr )
                    {
                        #if QT_VERSION < 0x050000
                        if( !pGraphicsItemHit->acceptsHoverEvents() )
                        #else
                        if( !pGraphicsItemHit->acceptHoverEvents() )
                        #endif
                        {
                            bGraphObjFound = false;

                            pGraphicsItemHit->setAcceptHoverEvents(true);
                            pGraphicsItemHit->setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
                            pGraphObjHit->setIsHit(true);

                            // Append object hit by mouse cursor to list of objects temporarily accepting hover events (if not yet already part of that list).
                            for( idxGraphObj = 0; idxGraphObj < m_arpGraphicsItemsAcceptingHoverEvents.size(); idxGraphObj++ )
                            {
                                pGraphicsItem = m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj];
                                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                                // If already part of list ...
                                if( pGraphObjHit == pGraphObj )
                                {
                                    bGraphObjFound = true;
                                    break;
                                }
                            }

                            // If not already part of list ...
                            if( !bGraphObjFound )
                            {
                                m_arpGraphicsItemsAcceptingHoverEvents.append(pGraphicsItemHit);
                            }
                        }
                    }
                }

                // Objects which have not been hit but have been part of the list temporarily accepting hover events
                // must be removed from the list and are no longer accepting hover events.
                if( m_arpGraphicsItemsAcceptingHoverEvents.size() > 0 )
                {
                    for( idxGraphObj = 0; idxGraphObj < m_arpGraphicsItemsAcceptingHoverEvents.size(); idxGraphObj++ )
                    {
                        bGraphObjFound = false;

                        pGraphicsItem = m_arpGraphicsItemsAcceptingHoverEvents[idxGraphObj];
                        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                        for( idxGraphObjHit = 0; idxGraphObjHit < arpGraphicsItemsHit.size(); idxGraphObjHit++ )
                        {
                            pGraphicsItemHit = arpGraphicsItemsHit[idxGraphObjHit];
                            pGraphObjHit = dynamic_cast<CGraphObj*>(pGraphicsItemHit);

                            if( pGraphObjHit == pGraphObj )
                            {
                                bGraphObjFound = true;
                                break;
                            }
                        }

                        if( !bGraphObjFound )
                        {
                            #if QT_VERSION < 0x050000
                            if( pGraphicsItem->acceptsHoverEvents() )
                            #else
                            if( pGraphicsItem->acceptHoverEvents() )
                            #endif
                            {
                                pGraphicsItem->setAcceptHoverEvents(false);
                                pGraphicsItem->setAcceptedMouseButtons(Qt::NoButton);
                                pGraphicsItem->unsetCursor();
                            }
                            if( /*!pGraphicsItem->isSelected() &&*/ pGraphObj->isHit() )
                            {
                                pGraphObj->setIsHit(false);
                            }
                        }
                    }
                } // if( m_arpGraphicsItemsAcceptingHoverEvents.size() > 0 )

                QGraphicsScene::mouseMoveEvent(i_pEv);

            } // if( bDispatchMouseEvents2ObjectsUnderMouseCursor || bDispatchMouseEvents2ConnectionPointsUnderMouseCursor )

        } // if( m_pGraphicsItemCreating == nullptr && m_pGraphicsItemAddingShapePoints == nullptr )

    } // if( m_mode == EMode::Edit )

    else if( m_mode == EMode::Simulation )
    {
        // ... dispatch mouse event to objects "under cursor".
        QGraphicsScene::mouseMoveEvent(i_pEv);
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";

        mthTracer.trace(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CDrawingScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().x()) + ")";
        strAddTrcInfo += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().x()) + ")";
        strAddTrcInfo += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().x()) + ")";

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    int iObjFactoryType = EGraphObjTypeUndefined;

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }

    if( m_mode != EMode::Edit )
    {
        if( m_pGraphicsItemCreating != nullptr || iObjFactoryType != EGraphObjTypeUndefined )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInvalidMethodCall);
        }
    }

    QPointF ptScenePosMouseEvent = i_pEv->scenePos();

    emit mousePosChanged(ptScenePosMouseEvent);

    if( m_mode == EMode::Edit )
    {
        // If currently an object is "under construction" ...
        if( m_pGraphicsItemCreating != nullptr )
        {
            // ... dispatch mouse event to object "under construction".
            QGraphicsSceneMouseEvent* pEv = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);
            QPointF ptItemPos = i_pEv->pos();
            QPointF ptScenePos = ptScenePosMouseEvent;
            QPoint  ptScreenPos = i_pEv->screenPos();
            ptItemPos = m_pGraphicsItemCreating->mapFromScene(ptScenePos);
            pEv->setButtons(i_pEv->buttons());
            pEv->setModifiers(i_pEv->modifiers());
            pEv->setPos(ptItemPos);
            pEv->setScenePos(ptScenePos);
            pEv->setScreenPos(ptScreenPos);
            sendEvent( m_pGraphicsItemCreating, pEv );
            delete pEv;
            pEv = nullptr;

        } // if( m_pGraphicsItemCreating != nullptr )

        // If currently an object is "under construction" ...
        else if( m_pGraphicsItemAddingShapePoints != nullptr )
        {
            // ... dispatch mouse event to object "under construction".
            QGraphicsSceneMouseEvent* pEv = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease);
            QPointF ptItemPos = i_pEv->pos();
            QPointF ptScenePos = ptScenePosMouseEvent;
            QPoint  ptScreenPos = i_pEv->screenPos();
            ptItemPos = m_pGraphicsItemAddingShapePoints->mapFromScene(ptScenePos);
            pEv->setButtons(i_pEv->buttons());
            pEv->setModifiers(i_pEv->modifiers());
            pEv->setPos(ptItemPos);
            pEv->setScenePos(ptScenePos);
            pEv->setScreenPos(ptScreenPos);
            sendEvent( m_pGraphicsItemAddingShapePoints, pEv );
            delete pEv;
            pEv = nullptr;

        } // if( m_pGraphicsItemAddingShapePoints != nullptr )

        // If currently no object is "under construction" ...
        else // if( m_pGraphicsItemCreating == nullptr && m_pGraphicsItemAddingShapePoints == nullptr )
        {
            // ... dispatch mouse press event to objects "under cursor".
            QGraphicsScene::mouseReleaseEvent(i_pEv);

            if( m_pGraphicsItemSelectionArea != nullptr )
            {
                QGraphicsScene::removeItem(m_pGraphicsItemSelectionArea);

                delete m_pGraphicsItemSelectionArea;
                m_pGraphicsItemSelectionArea = nullptr;

                QRectF rctSelectionArea(
                    /* x      */ m_ptMouseEvScenePosOnMousePressEvent.x(),
                    /* y      */ m_ptMouseEvScenePosOnMousePressEvent.y(),
                    /* width  */ ptScenePosMouseEvent.x() - m_ptMouseEvScenePosOnMousePressEvent.x() + 1,
                    /* height */ ptScenePosMouseEvent.y() - m_ptMouseEvScenePosOnMousePressEvent.y() + 1 );

                QPainterPath path;
                QTransform   trs;

                path.addRect(rctSelectionArea);

                setSelectionArea(path,Qt::ContainsItemShape);

            } // if( m_pGraphicsItemSelectionArea != nullptr )

            //else // if( m_pGraphicsItemSelectionArea == nullptr )
            //{
            //    // ... dispatch mouse press event to objects "under cursor".
            //    QGraphicsScene::mouseReleaseEvent(i_pEv);
            //}

        } // if( m_pGraphicsItemCreating == nullptr && m_pGraphicsItemAddingShapePoints == nullptr )

    } // if( m_mode == EMode::Edit )

    else if( m_mode == EMode::Simulation )
    {
        // ... dispatch mouse event to objects "under cursor".
        QGraphicsScene::mouseReleaseEvent(i_pEv);
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";

        mthTracer.trace(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CDrawingScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().x()) + ")";
        strAddTrcInfo += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().x()) + ")";
        strAddTrcInfo += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().x()) + ")";

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strAddTrcInfo );

    // Not a nice hack: mouse double click events will not be dispatched to the graphical objects
    // but will be converted to mouse press and mouse release events which are just dispatched
    // to the selection points. The "sceneEventFilter" functions of the graphical objects may
    // check the "DoubleClickInProcessFlag" to correct again the mouse press event into a mouse
    // double click event.
    m_bMouseDoubleClickEventInProcess = true;

    int iObjFactoryType = EGraphObjTypeUndefined;

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }

    if( m_mode != EMode::Edit )
    {
        if( m_pGraphicsItemCreating != nullptr || iObjFactoryType != EGraphObjTypeUndefined )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInvalidMethodCall);
        }
    }

    QPointF ptScenePosMouseEvent = i_pEv->scenePos();

    emit mousePosChanged(ptScenePosMouseEvent);

    if( m_mode == EMode::Edit )
    {
        // If currently an object is "under construction" ...
        if( m_pGraphicsItemCreating != nullptr )
        {
            // If currently a connection line is "under construction" ...
            if( m_pGraphicsItemCreating->type() == EGraphObjTypeConnectionLine )
            {
                bool bIsValidEndPoint = true;

                CGraphObjConnectionPoint* pGraphObjCnctPt   = nullptr;
                CGraphObjConnectionLine*  pGraphObjCnctLine = nullptr;

                pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(m_pGraphObjCreating);

                if( pGraphObjCnctLine != nullptr )
                {
                    pGraphObjCnctPt = getConnectionPoint(i_pEv->scenePos());

                    if( pGraphObjCnctPt != nullptr )
                    {
                        bIsValidEndPoint = true;
                    }
                }

                if( bIsValidEndPoint )
                {
                    bIsValidEndPoint = pGraphObjCnctLine->setConnectionPoint(ELinePointEnd,pGraphObjCnctPt);
                }

                if( !bIsValidEndPoint )
                {
                    delete m_pGraphicsItemCreating;
                    m_pGraphicsItemCreating = nullptr;
                    m_pGraphObjCreating = nullptr;
                    m_pGraphicsItemAddingShapePoints = nullptr;
                    m_pGraphObjAddingShapePoints = nullptr;
                    pGraphObjCnctLine = nullptr;
                }

            } // if( m_pGraphicsItemCreating->type() == EGraphObjTypeConnectionLine 

            // ... dispatch mouse event to object "under construction".
            if( m_pGraphicsItemCreating != nullptr )
            {
                QGraphicsSceneMouseEvent* pEv = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseDoubleClick);
                QPointF ptItemPos = i_pEv->pos();
                QPointF ptScenePos = i_pEv->scenePos();
                QPoint  ptScreenPos = i_pEv->screenPos();
                ptItemPos = m_pGraphicsItemCreating->mapFromScene(ptScenePos);
                pEv->setButtons(i_pEv->buttons());
                pEv->setModifiers(i_pEv->modifiers());
                pEv->setPos(ptItemPos);
                pEv->setScenePos(ptScenePos);
                pEv->setScreenPos(ptScreenPos);
                sendEvent( m_pGraphicsItemCreating, pEv );
                delete pEv;
                pEv = nullptr;
            }

        } // if( m_pGraphicsItemCreating != nullptr )

        // If currently an object is "under construction" ...
        else if( m_pGraphicsItemAddingShapePoints != nullptr )
        {
            // ... dispatch mouse event to object "under construction".
            QGraphicsSceneMouseEvent* pEv = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseDoubleClick);
            QPointF ptItemPos = i_pEv->pos();
            QPointF ptScenePos = i_pEv->scenePos();
            QPoint  ptScreenPos = i_pEv->screenPos();
            ptItemPos = m_pGraphicsItemAddingShapePoints->mapFromScene(ptScenePos);
            pEv->setButtons(i_pEv->buttons());
            pEv->setModifiers(i_pEv->modifiers());
            pEv->setPos(ptItemPos);
            pEv->setScenePos(ptScenePos);
            pEv->setScreenPos(ptScreenPos);
            sendEvent( m_pGraphicsItemAddingShapePoints, pEv );
            delete pEv;
            pEv = nullptr;

        } // if( m_pGraphicsItemAddingShapePoints != nullptr )

        // If currently no object is "under construction" ...
        else // if( m_pGraphicsItemCreating == nullptr && m_pGraphicsItemAddingShapePoints == nullptr )
        {
            // ... dispatch mouse event to objects "under cursor".
            QGraphicsScene::mouseDoubleClickEvent(i_pEv);
        }

    } // if( m_mode == EMode::Edit )

    else if( m_mode == EMode::Simulation )
    {
        // ... dispatch mouse event to objects "under cursor".
        QGraphicsScene::mouseDoubleClickEvent(i_pEv);
    }

    m_bMouseDoubleClickEventInProcess = false;

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";

        mthTracer.trace(strAddTrcInfo);
    }
} // mouseDoubleClickEvent

/*==============================================================================
public: // overridables of base class QGraphicsScene
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Key:" + qKeyCode2Str(i_pEv->key()) + " (" + i_pEv->text() + ")";
        strAddTrcInfo += ", Count:" + QString::number(i_pEv->count());
        strAddTrcInfo += ", IsAutoRepeat:" + QString::number(i_pEv->count());
        strAddTrcInfo += ", Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsScene::keyPressEvent(i_pEv);

    if( !i_pEv->isAccepted() )
    {
        QGraphicsItem* pGraphicsItem;
        CGraphObj*     pGraphObj;
        int            idxGraphObj;

        m_iEvKeyModifiers = i_pEv->modifiers();

        // As default the key is not handled by the manual test.
        i_pEv->ignore();

        // Pressed any key for moving or changing the shape of the objects ...
        if( i_pEv->key() == Qt::Key_Left || i_pEv->key() == Qt::Key_Right || i_pEv->key() == Qt::Key_Up || i_pEv->key() == Qt::Key_Down )
        {
            if( m_editTool == EEditToolSelect )
            {
                QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();

                // If at least one object has been selected which can be moved, resized or rotated ...
                if( arpGraphicsItemsSelected.size() > 0 )
                {
                    // We are going to change the position or rotate the objects.
                    QPointF ptPos;
                    double  fXOffs = 0;
                    double  fYOffs = 0;

                    switch( i_pEv->key() )
                    {
                        case Qt::Key_Left:
                        {
                            fXOffs = -1.0;
                            break;
                        }
                        case Qt::Key_Right:
                        {
                            fXOffs = 1.0;
                            break;
                        }
                        case Qt::Key_Up:
                        {
                            fYOffs = -1.0;
                            break;
                        }
                        case Qt::Key_Down:
                        {
                            fYOffs = 1.0;
                            break;
                        }
                    }

                    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                    {
                        pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];

                        // Children will be moved together with their parents ...
                        if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
                        {
                            ptPos = pGraphicsItem->pos();

                            ptPos.setX( ptPos.x() + fXOffs );
                            ptPos.setY( ptPos.y() + fYOffs );

                            pGraphicsItem->setPos(ptPos);
                        }
                    } // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                } // if( arpGraphicsItemsSelected.size() > 0 )
            } // if( m_editTool == EEditToolSelect )
        } // if( i_pEv->key() == Qt::Key_Left || i_pEv->key() == Qt::Key_Right || i_pEv->key() == Qt::Key_Up || i_pEv->key() == Qt::Key_Down )

        // Pressed any key for moving or changing the shape of the objects ...
        else if( i_pEv->key() == Qt::Key_Plus || i_pEv->key() == Qt::Key_Minus )
        {
            if( m_editTool == EEditToolSelect )
            {
                QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();

                // If at least one object has been selected which can be rotated ...
                if( arpGraphicsItemsSelected.size() > 0 )
                {
                    double fRotAngleOffs_deg = 0.0;
                    double fRotAngle_deg = 0.0;

                    if( i_pEv->key() == Qt::Key_Plus )
                    {
                        fRotAngleOffs_deg = 1.0; // by one degree
                    }
                    else // if( i_pEv->key() == Qt::Key_Minus )
                    {
                        fRotAngleOffs_deg = -1.0; // be one degree
                    }

                    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                    {
                        pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
                        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                        // Children will be moved together with their parents ...
                        if( pGraphObj != nullptr && pGraphicsItem->parentItem() == nullptr )
                        {
                            fRotAngle_deg = pGraphObj->getRotationAngleInDegree();

                            fRotAngle_deg += fRotAngleOffs_deg;

                            pGraphObj->setRotationAngleInDegree(fRotAngle_deg);
                        }
                    } // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                } // if( arpGraphicsItemsSelected.size() > 0 )
            } // if( m_editTool == EEditToolSelect )
        } // if( i_pEv->key() == Qt::Key_Plus || i_pEv->key() == Qt::Key_Minus )

        // Pressed key to delete selected objects ...
        else if( i_pEv->key() == Qt::Key_Delete )
        {
            QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();
            QStringList           strlstGraphObjIdsSelected;
            QString               strGraphObjId;

            // On deleting a graphical object the object may destroy other graphical objects
            // (e.g. connection points may delete connection lines) and the list of selected
            // items may change while the dtor is in process. So we don't store a list with object
            // references but a list with object ids and check whether the object still belong
            // to the drawing scene before deleting the object.

            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];

                if( pGraphicsItem != m_pGraphicsItemSelectionArea )
                {
                    pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObj != nullptr )
                    {
                        strlstGraphObjIdsSelected.append( pGraphObj->getObjId() );
                    }
                }
            }

            for( idxGraphObj = 0; idxGraphObj < strlstGraphObjIdsSelected.size(); idxGraphObj++ )
            {
                strGraphObjId = strlstGraphObjIdsSelected[idxGraphObj];

                pGraphObj = findGraphObj(strGraphObjId);

                if( pGraphObj != nullptr )
                {
                    deleteItem(pGraphObj);
                }
            }

        } // if( i_pEv->key() == Qt::Key_Delete )

        // Pressed key to copy selected objects to the (internal) clipboard ...
        else if( i_pEv->key() == Qt::Key_C )
        {
            if( m_iEvKeyModifiers == Qt::ControlModifier )
            {
                // Clear internal clipboard.
                if( m_dctpGraphObjsClipboard.size() > 0 )
                {
                    QMap<QString,CGraphObj*>::iterator itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

                    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                    {
                        pGraphObj = itGraphObjsClipboard.value();

                        if( pGraphObj != nullptr )
                        {
                            deleteItem(pGraphObj);
                        }
                        ++itGraphObjsClipboard;
                    }
                }
                m_dctpGraphObjsClipboard.clear();

                // Add selected objects to the internal clipboard. Not only the references
                // are copied but the objects will be newly created (cloned). By simply storing
                // the references changing attributes on currently selected objects would take
                // effect on objects in the clipboard.
                // Only those objects will be added to the clipboard which don't belong to a
                // group (which don't have parent objects) as the parent object must clone its
                // children.
                // Cloning connection points and connection lines is not that simply as cloning
                // a group with children. Connection lines can only be cloned if both of its
                // connection points will be cloned and the cloned connection line must be
                // connected with the clones of its original connection points.

                QList<QGraphicsItem*>     arpGraphicsItemsSelected = selectedItems();
                CGraphObj*                pGraphObjClone;
                CGraphObjConnectionLine*  pGraphObjCnctLine;
                CGraphObjConnectionLine*  pGraphObjCnctLineClone;
                CGraphObjConnectionPoint* pGraphObjCnctPt1;
                CGraphObjConnectionPoint* pGraphObjCnctPt2;
                CGraphObjConnectionPoint* pGraphObjCnctPt1Clone;
                CGraphObjConnectionPoint* pGraphObjCnctPt2Clone;
                QString                   strGraphObjName;
                QString                   strGraphObjId;
                QList<CGraphObj*>         arpGraphObjsOrigin;
                QList<CGraphObj*>         arpGraphObjsClones;
                int                       idxGraphObjTmp;

                // During the first loop connection lines will not be cloned. Instead we are going to keep
                // a list with original connection points and their clones. This way we are able in the
                // second loop on cloning connection lines to detect the clones of the connection points.
                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                {
                    pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];

                    // Only copy top level parent objects ..
                    if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
                    {
                        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                        if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                        {
                            pGraphObjClone = pGraphObj->clone();

                            if( pGraphObjClone != nullptr )
                            {
                                // The name of the clone should end with " Copy <Nr>". If not ..
                                strGraphObjName = pGraphObjClone->getObjName();
                                if( !strGraphObjName.endsWith(" Copy 1",Qt::CaseInsensitive) )
                                {
                                    strGraphObjName += " Copy 1";
                                    pGraphObjClone->setObjName(strGraphObjName);
                                }
                                strGraphObjName = findUniqueGraphObjName(pGraphObjClone);
                                pGraphObjClone->setObjName(strGraphObjName);

                                strGraphObjId = pGraphObjClone->getObjId();
                                strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                                pGraphObjClone->setObjId(strGraphObjId);
                                m_dctpGraphObjsClipboard[strGraphObjId] = pGraphObjClone;

                                if( pGraphObjClone->getType() == EGraphObjTypeConnectionPoint )
                                {
                                    arpGraphObjsOrigin.append(pGraphObj);
                                    arpGraphObjsClones.append(pGraphObjClone);
                                }

                            } // if( pGraphObjClone != nullptr )
                        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                    } // if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
                } // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )

                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                {
                    pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
                    pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                    if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                    {
                        pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

                        if( pGraphObjCnctLine != nullptr )
                        {
                            pGraphObjCnctPt1 = pGraphObjCnctLine->getConnectionPoint(ELinePointStart);
                            pGraphObjCnctPt2 = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);
                            pGraphObjCnctPt1Clone = nullptr;
                            pGraphObjCnctPt2Clone = nullptr;

                            // Look for the original connection points.
                            for( idxGraphObjTmp = 0; idxGraphObjTmp < arpGraphObjsOrigin.size(); idxGraphObjTmp++ )
                            {
                                if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt1 )
                                {
                                    pGraphObjCnctPt1Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                                }
                                if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt2 )
                                {
                                    pGraphObjCnctPt2Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                                }
                                if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                                {
                                    break;
                                }
                            }

                            // The references to the connection points cannot be copied with the
                            // connection lines as the connection points have also been cloned.
                            // Instead the cloned connection line got to be connected to the clones
                            // of the connection points 
                            if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                            {
                                pGraphObjCnctLineClone = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj->clone());

                                if( pGraphObjCnctLineClone != nullptr )
                                {
                                    pGraphObjCnctLineClone->setConnectionPoint(ELinePointStart,pGraphObjCnctPt1Clone);
                                    pGraphObjCnctLineClone->setConnectionPoint(ELinePointEnd,pGraphObjCnctPt2Clone);

                                    // The name of the clone should end with " Copy <Nr>". If not ..
                                    strGraphObjName = pGraphObjCnctLineClone->getObjName();
                                    if( !strGraphObjName.endsWith(" Copy 1",Qt::CaseInsensitive) )
                                    {
                                        strGraphObjName += " Copy 1";
                                        pGraphObjCnctLineClone->setObjName(strGraphObjName);
                                    }
                                    strGraphObjName = findUniqueGraphObjName(pGraphObjCnctLineClone);
                                    pGraphObjCnctLineClone->setObjName(strGraphObjName);

                                    strGraphObjId = pGraphObjCnctLineClone->getObjId();
                                    strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                                    pGraphObjCnctLineClone->setObjId(strGraphObjId);
                                    m_dctpGraphObjsClipboard[strGraphObjId] = pGraphObjCnctLineClone;
                                }
                            } // if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                        } // if( pGraphObjCnctLine != nullptr )
                    } // if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                } // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )

            } // if( m_iEvKeyModifiers == Qt::ControlModifier )
        } // if( i_pEv->key() == Qt::Key_C )

        // Pressed key to insert the objects from the (internal) clipboard ...
        else if( i_pEv->key() == Qt::Key_V )
        {
            if( m_iEvKeyModifiers == Qt::ControlModifier )
            {
                CGraphObj*                pGraphObjClone;
                QGraphicsItem*            pGraphicsItemClone;
                CGraphObjConnectionLine*  pGraphObjCnctLine;
                CGraphObjConnectionLine*  pGraphObjCnctLineClone;
                CGraphObjConnectionPoint* pGraphObjCnctPt1;
                CGraphObjConnectionPoint* pGraphObjCnctPt2;
                CGraphObjConnectionPoint* pGraphObjCnctPt1Clone;
                CGraphObjConnectionPoint* pGraphObjCnctPt2Clone;
                QString                   strGraphObjName;
                QString                   strGraphObjId;
                QList<CGraphObj*>         arpGraphObjsOrigin;
                QList<CGraphObj*>         arpGraphObjsClones;
                int                       idxGraphObjTmp;
                QPointF                   ptPos;

                // Add objects from the internal clipboard to the drawing scene. Not only the references
                // have been copied but the objects have already been newly created (cloned).
                // We leave the clones in the clipboard as the objects may be copied several times.
                // But for this we have to clone the clone.
                // Concerning cloning group objects, connection points and their connection lines the
                // same applies as on cloning the objects to the clipboard (see "Key_C" above).

                if( m_dctpGraphObjsClipboard.size() > 0 )
                {
                    clearSelection();

                    QMap<QString,CGraphObj*>::iterator itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

                    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                    {
                        pGraphObj = itGraphObjsClipboard.value();

                        if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                        {
                            // We leave the clones in the clipboard as the objects may be copied several times.
                            // But for this we have to clone the clone.
                            pGraphObjClone = pGraphObj->clone();

                            if( pGraphObjClone != nullptr )
                            {
                                strGraphObjName = pGraphObjClone->getObjName();
                                strGraphObjName = findUniqueGraphObjName(pGraphObjClone);
                                pGraphObjClone->setObjName(strGraphObjName);

                                strGraphObjId = pGraphObjClone->getObjId();
                                strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjs, strGraphObjId );
                                pGraphObjClone->setObjId(strGraphObjId);

                                pGraphicsItemClone = dynamic_cast<QGraphicsItem*>(pGraphObjClone);

                                if( pGraphicsItemClone == nullptr )
                                {
                                    throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                                }

                                addItem(pGraphicsItemClone);

                                onGraphObjCreated(pGraphObjClone);

                                ptPos = pGraphicsItemClone->pos();

                                ptPos.setX( ptPos.x() + 10 );
                                ptPos.setY( ptPos.y() + 10 );

                                pGraphicsItemClone->setPos(ptPos);
                                pGraphicsItemClone->setSelected(true);

                                if( pGraphicsItemClone->childItems().size() > 0 )
                                {
                                    // Rekursively add child items to the drawings scene's list of graphics items.
                                    addChildItems(pGraphicsItemClone);
                                }

                                if( pGraphObjClone->getType() == EGraphObjTypeConnectionPoint )
                                {
                                    arpGraphObjsOrigin.append(pGraphObj);
                                    arpGraphObjsClones.append(pGraphObjClone);
                                }

                            } // if( pGraphObjClone != nullptr )
                        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )

                        ++itGraphObjsClipboard;

                    } // while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )

                    itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

                    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                    {
                        pGraphObj = itGraphObjsClipboard.value();

                        if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                        {
                            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

                            if( pGraphObjCnctLine != nullptr )
                            {
                                pGraphObjCnctPt1 = pGraphObjCnctLine->getConnectionPoint(ELinePointStart);
                                pGraphObjCnctPt2 = pGraphObjCnctLine->getConnectionPoint(ELinePointEnd);
                                pGraphObjCnctPt1Clone = nullptr;
                                pGraphObjCnctPt2Clone = nullptr;

                                // Look for the "original" connection points in the clipboard.
                                for( idxGraphObjTmp = 0; idxGraphObjTmp < arpGraphObjsOrigin.size(); idxGraphObjTmp++ )
                                {
                                    if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt1 )
                                    {
                                        pGraphObjCnctPt1Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                                    }
                                    if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt2 )
                                    {
                                        pGraphObjCnctPt2Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                                    }
                                    if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                                    {
                                        break;
                                    }
                                }

                                // The references to the connection points cannot be copied with the
                                // connection lines as the connection points have also been cloned.
                                // Instead the cloned connection line got to be connected to the clones
                                // of the connection points 
                                if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                                {
                                    // We leave the clones in the clipboard as the objects may be copied several times.
                                    // But for this we have to clone the clone.
                                    pGraphObjClone = pGraphObj->clone();
                                    pGraphObjCnctLineClone = dynamic_cast<CGraphObjConnectionLine*>(pGraphObjClone);

                                    if( pGraphObjCnctLineClone != nullptr )
                                    {
                                        pGraphObjCnctLineClone->setConnectionPoint(ELinePointStart,pGraphObjCnctPt1Clone);
                                        pGraphObjCnctLineClone->setConnectionPoint(ELinePointEnd,pGraphObjCnctPt2Clone);

                                        strGraphObjName = pGraphObjCnctLineClone->getObjName();
                                        strGraphObjName = findUniqueGraphObjName(pGraphObjCnctLineClone);
                                        pGraphObjCnctLineClone->setObjName(strGraphObjName);

                                        strGraphObjId = pGraphObjCnctLineClone->getObjId();
                                        strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                                        pGraphObjCnctLineClone->setObjId(strGraphObjId);

                                        pGraphicsItemClone = dynamic_cast<QGraphicsItem*>(pGraphObjCnctLineClone);

                                        if( pGraphicsItemClone == nullptr )
                                        {
                                            throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                                        }

                                        addItem(pGraphicsItemClone);

                                        onGraphObjCreated(pGraphObjCnctLineClone);

                                        // Please note that the position for connection lines is defined by the connection points
                                        // and that connection lins never have children (calling "addChildItems" is not necessary).
                                        pGraphicsItemClone->setSelected(true);

                                    } // if( pGraphObjCnctLineClone != nullptr )
                                } // if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                            } // if( pGraphObjClone != nullptr )
                        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )

                        ++itGraphObjsClipboard;

                    } // while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )

                } // if( m_dctpGraphObjsClipboard.size() > 0 )

            } // if( m_iEvKeyModifiers == Qt::ControlModifier )

        } // if( i_pEv->key() == Qt::Key_V )

    } // if( !i_pEv->isAccepted() )

} // keyPressEvent

//------------------------------------------------------------------------------
void CDrawingScene::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Key:" + qKeyCode2Str(i_pEv->key()) + " (" + i_pEv->text() + ")";
        strAddTrcInfo += ", Count:" + QString::number(i_pEv->count());
        strAddTrcInfo += ", IsAutoRepeat:" + QString::number(i_pEv->count());
        strAddTrcInfo += ", Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());

        strAddTrcInfo += ", Mode:" + CEnumMode::toString(m_mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(m_editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", EditResizeMode:" + editResizeMode2Str(m_editResizeMode);

        if( m_pObjFactory == nullptr )
        {
            strAddTrcInfo += ", ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFactory:" + m_pObjFactory->getGraphObjTypeAsString();
        }

        if( m_pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + m_pGraphObjCreating->getNameSpace();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getClassName();
            strAddTrcInfo += "::" + m_pGraphObjCreating->getObjName();
        }

        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsScene::keyReleaseEvent(i_pEv);

    m_iEvKeyModifiers = 0;

} // keyReleaseEvent

/*==============================================================================
protected: // overridables of base class QGraphicsScene
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::drawBackground( QPainter* i_pPainter, const QRectF& i_rect )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Rect(x,y,w,h):(" + QString::number(i_rect.x()) + "," + QString::number(i_rect.y());
        strAddTrcInfo += "," + QString::number(i_rect.width()) + "," + QString::number(i_rect.height()) + ")";
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 2,
        /* strMethod    */ "drawBackground",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsScene::drawBackground(i_pPainter,i_rect);

    QRectF rctScene = sceneRect();

    rctScene.setLeft( rctScene.left()-1 );
    rctScene.setTop( rctScene.top()-1 );
    rctScene.setWidth( rctScene.width()+2 );
    rctScene.setHeight( rctScene.height()+2 );

    i_pPainter->setPen(Qt::darkGray);
    i_pPainter->drawRect(rctScene);

} // drawBackground

//------------------------------------------------------------------------------
void CDrawingScene::drawForeground( QPainter* i_pPainter, const QRectF& i_rect )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Rect(x,y,w,h):(" + QString::number(i_rect.x()) + "," + QString::number(i_rect.y());
        strAddTrcInfo += "," + QString::number(i_rect.width()) + "," + QString::number(i_rect.height()) + ")";
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 2,
        /* strMethod    */ "drawForeground",
        /* strAddInfo   */ strAddTrcInfo );

    i_pPainter->setClipRect(i_rect);

    QGraphicsScene::drawForeground(i_pPainter,i_rect);

} // drawForeground
