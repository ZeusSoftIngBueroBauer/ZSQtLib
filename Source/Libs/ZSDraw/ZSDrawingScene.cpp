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

#include <QtCore/qfile.h>
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
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSceneEvent>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#endif

#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawObjFactoryGroup.h"
#include "ZSDraw/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjImage.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
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

////------------------------------------------------------------------------------
//QString CDrawingScene::FindUniqueGraphObjId(
//    const QMap<QString,CGraphObj*>& i_dctpGraphObjs,
//    const QString&                  i_strObjIdCurr )
////------------------------------------------------------------------------------
//{
//    QString strObjId = i_strObjIdCurr;
//    int     iObjNr   = 1;
//    QString strObjNr;
//
//    // Remove trailing number (if any).
//    if( strObjId.length() > 0 && isDigitChar(strObjId[strObjId.length()-1]) )
//    {
//        while( isDigitChar(strObjId[strObjId.length()-1]) )
//        {
//            strObjNr.insert( 0, strObjId[strObjId.length()-1] );
//            strObjId.remove( strObjId.length()-1, 1 );
//        }
//        iObjNr = strObjNr.toInt();
//        iObjNr++;
//    }
//    else
//    {
//        strObjNr = QString::number(iObjNr);
//    }
//
//    QString strObjBaseName = strObjId;
//
//    strObjId = strObjBaseName + strObjNr;
//
//    while( i_dctpGraphObjs.contains(strObjId) )
//    {
//        iObjNr++;
//        strObjNr = QString::number(iObjNr);
//        strObjId = strObjBaseName + strObjNr;
//    }
//
//    return strObjId;
//
//} // FindUniqueGraphObjId

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
    m_editTool(EEditTool::Undefined),
    m_editMode(EEditMode::Undefined),
    m_editResizeMode(EEditResizeMode::Undefined),
    m_pGraphicsItemSelectionArea(nullptr),
    m_pObjFactory(nullptr),
    m_pGraphicsItemCreating(nullptr),
    m_pGraphObjCreating(nullptr),
    m_pGraphicsItemAddingShapePoints(nullptr),
    m_pGraphObjAddingShapePoints(nullptr),
    m_strGraphObjNameSeparator("/"),
    m_pGraphObjsIdxTree(nullptr),
    m_pGraphObjsIdxTreeClipboard(nullptr),
    m_arpGraphicsItemsAcceptingHoverEvents(),
    m_fRotAngleRes_degree(1.0),
    m_fHitTolerance_px(2.0),
    m_bMouseDoubleClickEventInProcess(false),
    m_ptMouseEvScenePosOnMousePressEvent(),
    m_iEvKeyModifiers(0),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjMouseMoveEvent(nullptr),
    m_pTrcAdminObjPaintEvent(nullptr)
{
    setObjectName("DrawingScene");

    //if( m_strGraphObjNameSeparator.isEmpty() )
    //{
    //    m_strGraphObjNameSeparator = "/";
    //}

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pGraphObjsIdxTree = new CIdxTree(
        /* strObjName       */ objectName() + "-GraphObjs",
        /* pRootTreeEntry   */ nullptr,
        /* strNodeSeparator */ "::",
        /* bCreateMutex     */ false,
        /* pObjParent       */ nullptr,
        /* iTrcDetailLevel  */ ETraceDetailLevelNone );
    m_pGraphObjsIdxTreeClipboard = new CIdxTree(
        /* strObjName       */ objectName() + "-Clipboard-GraphObjs",
        /* pRootTreeEntry   */ nullptr,
        /* strNodeSeparator */ "::",
        /* bCreateMutex     */ false,
        /* pObjParent       */ nullptr,
        /* iTrcDetailLevel  */ ETraceDetailLevelNone );

    //setItemIndexMethod(NoIndex);

    m_pTrcAdminObjMouseMoveEvent = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName() + "-MouseMoveEvent");;
    m_pTrcAdminObjPaintEvent = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName() + "-PaintEvent");;

    // To update the mode labels:
    setMode( EMode::Edit/*, m_editTool, m_editMode, m_editResizeMode*/ );

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

    // Clear (and destroy graphical object items) here as dtor of CGraphObj
    // accesses the drawing scene to remove the object name from the hash.
    clear();

    try
    {
        delete m_pGraphObjsIdxTreeClipboard;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pGraphObjsIdxTree;
    }
    catch(...)
    {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseMoveEvent);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaintEvent);

    //m_drawSettings;
    m_fXResolution_dpmm = 0.0;
    m_fYResolution_dpmm = 0.0;
    m_mode = static_cast<ZS::System::EMode>(0);
    m_editTool = static_cast<EEditTool>(0);
    m_editMode = static_cast<EEditMode>(0);
    m_editResizeMode = static_cast<EEditResizeMode>(0);
    m_pGraphicsItemSelectionArea = nullptr;
    m_pObjFactory = nullptr;
    m_pGraphicsItemCreating = nullptr;
    m_pGraphObjCreating = nullptr;
    m_pGraphicsItemAddingShapePoints = nullptr;
    m_pGraphObjAddingShapePoints = nullptr;
    //m_strGraphObjNameSeparator;
    m_pGraphObjsIdxTree = nullptr;
    m_pGraphObjsIdxTreeClipboard = nullptr;
    //m_arpGraphicsItemsAcceptingHoverEvents;
    m_fRotAngleRes_degree = 0.0;
    m_fHitTolerance_px = 0.0;
    m_bMouseDoubleClickEventInProcess = false;
    //m_ptMouseEvScenePosOnMousePressEvent;
    m_iEvKeyModifiers = 0;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjMouseMoveEvent = nullptr;
    m_pTrcAdminObjPaintEvent = nullptr;

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

    // Clear internal clipboard.
    //CGraphObj* pGraphObj;
    //CIdxTree::iterator itIdxTree = m_pGraphObjsIdxTreeClipboard->begin(CIdxTree::iterator::ETraversalOrder::PreOrder);
    //while( itIdxTree != m_pGraphObjsIdxTreeClipboard->end() )
    //{
    //    pGraphObj = dynamic_cast<CGraphObj*>(*itIdxTree);
    //    deleteItem(pGraphObj);
    //    ++itIdxTree;
    //}
    m_pGraphObjsIdxTreeClipboard->clear();
    m_pGraphObjsIdxTree->clear();

    QGraphicsScene::clear();

    m_arpGraphicsItemsAcceptingHoverEvents.clear();

} // clear

//------------------------------------------------------------------------------
/*! Adds the given graph object to the drawing scene.

    The graph object will be added to the index tree as well as to the graphics scene.
    If a parent is passed the given graph object will also be added as a child to
    the parent object. This applies both for the tree entry in the index tree as well
    as for the graphics item of Qt's graphics scene.

    This means that the given graph object may not already be a child of the parent
    object (neither in the index tree nor in Qt's graphcis scene) and the parent object
    must have already been added before to the drawing scene.

    @param i_pGraphObj [in]
        Pointer to object to be added to the drawing scene.
        The object will be added to the index tree as well as to Qt's graphics scene.
    @param i_pGraphObjParent [in]
        Pointer to parent object or nullptr.
        If a parent object is given the parent object must have already been added
        before to the drawing scene. The given graph object will be added as a child
        to the parents index tree entry as well as to the parents graphics item.

    @note
        Selection points and labels should not belong as child to the graphics items.
        Otherwise the "boundingRect" call of groups (which implicitly calls childrenBoundingRect) does
        not work as the childs bounding rectangles would be included. But the selection points and
        labels should appear as childs in the index tree of the drawing scene.
*/
void CDrawingScene::addGraphObj( CGraphObj* i_pGraphObj, CGraphObj* i_pGraphObjParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
        strMthInArgs += ", Parent: " + QString(i_pGraphObjParent == nullptr ? "nullptr" : i_pGraphObjParent->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addGraphObj",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);

    if( pGraphicsItem == nullptr )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "dynamic_cast<QGraphicsItem*>(i_pGraphObj)");
    }

    // Please note that selection points and labels should not belong as child to the graphics items.
    // Otherwise the "boundingRect" call of groups (which implicitly calls childrenBoundingRect) does
    // not work as the childs bounding rectangles would be included. But the selection points and
    // labels should appear as childs in the index tree of the drawing scene.

    QGraphicsItem* pGraphicsItemParent = nullptr;

    if( i_pGraphObj->getType() != EGraphObjTypeSelectionPoint && i_pGraphObj->getType() != EGraphObjTypeLabel )
    {
        if( i_pGraphObjParent != nullptr )
        {
            pGraphicsItemParent = dynamic_cast<QGraphicsItem*>(i_pGraphObjParent);

            if( pGraphicsItemParent == nullptr )
            {
                throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "dynamic_cast<QGraphicsItem*>(i_pGraphObjParent)");
            }
        }
    }

    addItem(pGraphicsItem, pGraphicsItemParent);

    m_pGraphObjsIdxTree->add(i_pGraphObj, i_pGraphObjParent);

} // addGraphObj

//------------------------------------------------------------------------------
void CDrawingScene::removeGraphObj( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeGraphObj",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);

    if( pGraphicsItem != nullptr )
    {
        removeItem(pGraphicsItem);
    }

} // removeGraphObj

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Adds the given graphics item to Qt's graphic scene.

    This method is internally called by "addGraphObj".

    If a parent item is passed the given graphics item will also be added as a child to
    the parent graphics item.

    This means that the given graphics item may not already be a child of the parent item
    and the parent item must have already been added before to Qt's graphic scene.

    @param i_pGraphicsItem [in]
        Pointer to graphics item to be added to Qt's graphic scene.
    @param i_pGraphicsItemParent [in]
        Pointer to parent item or nullptr.
        If a parent item is given the parent item must have already been added
        before to Qt's graphic scene. The given graphics item will be added as a child
        to the parent item.
*/
void CDrawingScene::addItem( QGraphicsItem* i_pGraphicsItem, QGraphicsItem* i_pGraphicsItemParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(i_pGraphicsItemParent);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
        strMthInArgs += ", Parent: " + QString(pGraphObjParent == nullptr ? "nullptr" : pGraphObjParent->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "addItem",
        /* strAddInfo   */ strMthInArgs );

    if( i_pGraphicsItemParent != nullptr )
    {
        i_pGraphicsItem->setParentItem(i_pGraphicsItemParent);
    }

    QGraphicsScene::addItem(i_pGraphicsItem);

} // addItem

//------------------------------------------------------------------------------
void CDrawingScene::removeItem( QGraphicsItem* i_pGraphicsItem )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "removeItem",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsScene::removeItem(i_pGraphicsItem);

} // removeItem

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::deleteItem( QGraphicsItem* i_pGraphicsItem )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "deleteItem",
        /* strAddInfo   */ strMthInArgs );

    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "deleteItem",
        /* strAddInfo   */ strMthInArgs );

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
                    strGraphObjIdChild = pGraphObjChild->keyInTree();
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

////------------------------------------------------------------------------------
//void CDrawingScene::addChildItems( QGraphicsItem* i_pGraphicsItem )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
//        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "addChildItems",
//        /* strAddInfo   */ strMthInArgs );
//
//    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
//
//    if( i_pGraphicsItem != nullptr )
//    {
//        QList<QGraphicsItem*> arpGraphicsItemsChilds = i_pGraphicsItem->childItems();
//        QGraphicsItem*        pGraphicsItemChild;
//        CGraphObj*            pGraphObjChild;
//        int                   idxGraphObjChild;
//
//        for( idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++ )
//        {
//            pGraphicsItemChild = arpGraphicsItemsChilds[idxGraphObjChild];
//            pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
//
//            if( pGraphicsItemChild != nullptr && pGraphObjChild != nullptr )
//            {
//                addItem(pGraphicsItemChild);
//
//                onGraphObjCreationFinished(pGraphObjChild);
//
//                if( pGraphicsItemChild->childItems().size() > 0 )
//                {
//                    addChildItems(pGraphicsItemChild);
//                }
//            }
//        }
//    } // if( i_pGraphicsItem != nullptr )
//
//} // addChildItems

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CDrawingScene::load( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "FileName:" + i_strFileName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "load",
        /* strAddInfo   */ strMthInArgs );

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

            if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
            {
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
                        QString strFactoryGrpName;
                        QString strNameSpace;
                        QString strClassName;
                        QString strGraphObjType;
                        QString strObjName;
                        QString strObjId;

                        xmlStreamAttrs = xmlStreamReader.attributes();

                        if( xmlStreamAttrs.hasAttribute("FactoryGroupName") )
                        {
                            strFactoryGrpName = xmlStreamAttrs.value("FactoryGroupName").toString();;
                        }
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

                        CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGrpName, strGraphObjType);

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

                else // if( xmlStreamReader.isEndElement() )
                {
                }
            } // if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "FileName:" + i_strFileName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "save",
        /* strAddInfo   */ strMthInArgs );

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

            if( pGraphicsItem->type() != static_cast<int>(EGraphObjTypeSelectionPoint)
             && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeLabel)
             && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeConnectionLine) )
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

                if( pGraphicsItem->type() != static_cast<int>(EGraphObjTypeSelectionPoint)
                 && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeLabel)
                 && pGraphicsItem->type() == static_cast<int>(EGraphObjTypeConnectionLine) )
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

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "GraphObj:" + i_pGraphObj->nameSpace();
        strMthInArgs += "::" + i_pGraphObj->className();
        strMthInArgs += "::" + i_pGraphObj->name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "save",
        /* strAddInfo   */ strMthInArgs );

    SErrResultInfo errResultInfo;

    QString strNameSpace    = i_pGraphObj->nameSpace();
    QString strClassName    = i_pGraphObj->className();
    QString strGraphObjType = i_pGraphObj->getTypeAsString();
    QString strObjName      = i_pGraphObj->name();
    QString strObjId        = i_pGraphObj->keyInTree();

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(i_pGraphObj->getFactoryGroupName(), strGraphObjType);

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
/*! Setzt den Mode der Scene damit diese weiss, wie mit nachfolgenden Events zu
    verfahren ist.

    Um ein grafisches Objekt ueber Maus Events zu erzeugen, zu verschieben oder zu
    rotieren, seine Form zu �ndern, zu selektieren und zu gruppieren, muessen verschiedene
    Modi korrekt gesetzt werden.

    @param i_mode [in] Range [Edit, Simulation und Ignore]
        Ignore ist zu �bergeben, wenn der Mode nicht ge�ndert werden soll und das Argument
        somit zu ignorieren ist.
        Nur im Edit Mode k�nnen Objekte auf der grafischen Oberfl�che durch die Maus angelegt
        und ver�ndert werden.
        Der Simulation Mode dient dazu, die Maus-Events an die Objekte selbst weiterzuleiten,
        die entsprechend darauf reagieren k�nnen. Ist z.B. eine ComboBox als Objekt der Empf�nger
        der Maus-Events verarbeitet die ComboBox die Events, um z.B. die PullDown-List zu �ffnen.
        Bei dem Objekt kann es sich aber auch um ein vollst�ndig benutzerdefiniertes Objekt wie
        einen grafischen Schalter handeln, der �ber Maus-Klicks seinen On/Off Zustand �ndert.
    @param i_editTool [in] Range [Select, CreateObjects, Ignore, Undefined]
        Ignore ist zu �bergeben, wenn das EditTool nicht ge�ndert werden soll und das Argument
        somit zu ignorieren ist.
        Undefined zeigt an, dass kein EditTool ausgew�hlt ist.
        Wird Select �bergeben, sollen nachfolgende Maus-Events dazu dienen, Objekte zu selektieren.
        Mit CreateObjects wird angezeigt, dass �ber nachfolgende Maus-Events Objekte erzeugt werden sollen.
    @param i_editMode [in] Range [Creating, Move, Resize, Rotate, MoveShapePoint, EditText, Ignore, Undefined]
        Ignore ist zu �bergeben, wenn der EditMode nicht ge�ndert werden soll und das Argument
        somit zu ignorieren ist.
        Undefined zeigt an, dass kein EditMode ausgew�hlt ist.
        Die anderen EditModes zeigen an, auf welche Art und Weise das Objekt zu modifizieren ist und h�ngen
        im wesentlichen davon ab, an welchem Eckpunkt (SelectionPoint) das Objekt mit der Maus gepackt wurde.
    @param i_editResizeMode [in] Range [ResizeAll, ResizeHor, ResizeVer, Ignore, Undefined]
        Ignore ist zu �bergeben, wenn der ResizeMode nicht ge�ndert werden soll und das Argument
        somit zu ignorieren ist.
        Undefined zeigt an, dass kein ResizeMode ausgew�hlt ist.
    @param i_bObjFactoryTypeChanged [in]
        true, falls der grafische Object Type und damit die Objekt Factory zur Erzeugung des Objekt ver�ndert wurde.
        falls otherwise.
*/
void CDrawingScene::setMode(
    const ZS::System::CEnumMode& i_mode,
    const CEnumEditTool&         i_editTool,
    const CEnumEditMode&         i_editMode,
    const CEnumEditResizeMode&   i_editResizeMode,
    bool                         i_bObjFactoryTypeChanged )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "Mode:" + i_mode.toString();
        strMthInArgs += ", EditTool:" + i_editTool.toString();
        strMthInArgs += ", EditMode:" + i_editMode.toString();
        strMthInArgs += ", ResizeMode:" + i_editResizeMode.toString();
        strMthInArgs += ", ObjFactoryChanged:" + bool2Str(i_bObjFactoryTypeChanged);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setMode",
        /* strAddInfo   */ strMthInArgs );

    bool bModeChanged = false;

    if( i_mode != EMode::Ignore && m_mode != i_mode )
    {
        bModeChanged = true;

        m_mode = i_mode;

        if( m_mode == EMode::Simulation )
        {
            if( i_editTool == EEditTool::Ignore )
            {
                m_editTool = EEditTool::Undefined;
            }
            if( i_editMode == EEditMode::Ignore )
            {
                m_editMode = EEditMode::Undefined;
            }
            if( i_editResizeMode == EEditResizeMode::Ignore )
            {
                m_editResizeMode = EEditResizeMode::Undefined;
            }
        }

        clearSelection();
    }

    if( i_editTool != EEditTool::Ignore && m_editTool != i_editTool )
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

    if( m_editTool != EEditTool::CreateObjects )
    {
        m_pGraphicsItemCreating = nullptr;
        m_pGraphObjCreating = nullptr;
    }

    if( i_editMode != EEditMode::Ignore && m_editMode != i_editMode )
    {
        bModeChanged = true;

        m_editMode = i_editMode;
    }

    if( i_editResizeMode != EEditResizeMode::Ignore && m_editResizeMode != i_editResizeMode )
    {
        bModeChanged = true;

        m_editResizeMode = i_editResizeMode;
    }

    if( bModeChanged || i_bObjFactoryTypeChanged )
    {
        update();

        emit modeChanged();
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", GraphObjCreating: " + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->path());
        mthTracer.trace(strAddTrcInfo);
    }

} // setMode

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Activates the given drawing tool by activating the corresponding object factory.

    The method may only be called if the drawing scene is in Edit mode.

    If a new object factory is activated the scenes mode will be adjusted
    so that a graphical object will be created by the following mouse click event.

    @param i_pObjFactory [in] Pointer to Object Factory instance to be used as the
                              drawing tool's object factory.
                              nullptr may be passed to deactivate any currently
                              selected object factory.

    @note Throws a critical exception
          - with Result = InvalidMethodCall if the scene is not in Edit mode.
*/
void CDrawingScene::setCurrentDrawingTool( CObjFactory* i_pObjFactory )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pObjFactory == nullptr ? "nullptr" : i_pObjFactory->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", EditResizeMode:" + m_editResizeMode.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_mode != EMode::Edit )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidMethodCall, "m_mode != EMode::Edit");
    }

    int iGraphObjTypePrev = static_cast<int>(EGraphObjTypeUndefined);

    if( m_pObjFactory != nullptr )
    {
        if( !QObject::disconnect(
            /* pObjSender   */ m_pObjFactory,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onGraphObjFactoryDestroyed(QObject*)) ) )
        {
            throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
        }

        iGraphObjTypePrev = m_pObjFactory->getGraphObjType();
    }

    int iGraphObjTypeCurr = iGraphObjTypePrev;

    m_pObjFactory = i_pObjFactory;

    if( m_pObjFactory != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pObjFactory,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onGraphObjFactoryDestroyed(QObject*)) ) )
        {
            throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
        }

        iGraphObjTypeCurr = m_pObjFactory->getGraphObjType();
    }
    else
    {
        iGraphObjTypeCurr = static_cast<int>(EGraphObjTypeUndefined);
    }

    if( iGraphObjTypePrev != iGraphObjTypeCurr )
    {
        if( iGraphObjTypeCurr == static_cast<int>(EGraphObjTypeUndefined) )
        {
            CEnumEditTool editTool = m_editTool;

            if( m_editTool != EEditTool::Select )
            {
                editTool = EEditTool::Undefined;
            }
            setMode(EMode::Ignore, editTool, EEditMode::Undefined, EEditResizeMode::Undefined, true);
        }
        else
        {
            setMode(EMode::Ignore, EEditTool::CreateObjects, EEditMode::Undefined, EEditResizeMode::Undefined, true);
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        mthTracer.trace(strAddTrcInfo);
    }

} // setCurrentDrawingTool

//------------------------------------------------------------------------------
/*! Activates the given drawing tool by activating the corresponding object factory.

    The method may only be called if the drawing scene is in Edit mode.

    The object factory with the given group name and graph object type must have
    been created before.

    @param i_strFactoryGrpName [in] Group name of the drawing tool's object factory.
    @param i_strGraphObjType [in] Name of the drawing tool's object class.
                                  An empty string may be passed to deactivate any
                                  currently selected object factory.

    @note Throws a critical exception
          - with Result = InvalidMethodCall if the scene is not in Edit mode.
          - with Result = ObjNotRegistered if no object factory with the given group name
                          and object type is not registered.
*/
void CDrawingScene::setCurrentDrawingTool(
    const QString& i_strFactoryGrpName,
    const QString& i_strGraphObjType )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strFactoryGrpName + "::" + i_strGraphObjType;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    if( m_mode != EMode::Edit )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidMethodCall, "m_mode != EMode::Edit");
    }

    CObjFactory* pObjFactory = CObjFactory::FindObjFactory(i_strFactoryGrpName, i_strGraphObjType);

    if( pObjFactory == nullptr )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotRegistered, "pObjFactory == nullptr");
    }

    setCurrentDrawingTool(pObjFactory);

} // setCurrentDrawingTool

//------------------------------------------------------------------------------
/*! Returns the type of the currently selected drawing tool as an integer value.

    @return Type as integer value of the currently selected drawing tool.
*/
int CDrawingScene::getCurrentDrawingToolGraphObjType() const
//------------------------------------------------------------------------------
{
    int iObjFactoryType = static_cast<int>(EGraphObjTypeUndefined);

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
/*! Checks whether a connection point is at the given position.

    @param i_ptScenePos [in] Scene coordinates to be checked.

    @return Pointer to connection point item if there is a connection point at
            the given position. nullptr otherwise.
*/
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

        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeConnectionPoint) )
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
/*! Returns the proposed mouse cursor shape.

    The mouse cursor shape depends on the graphic item at the given position
    and the currently selected edit tool and edit mode.

    @param i_ptPos [in] Scene coordinates to be checked.

    @return Proposed cursor shape.
*/
QCursor CDrawingScene::getProposedCursor( const QPointF& i_ptPos ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseMoveEvent != nullptr && m_pTrcAdminObjMouseMoveEvent->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Pos:" + point2Str(i_ptPos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvent,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "getProposedCursor",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", GraphObjAddingShapePoints:" + QString(m_pGraphObjAddingShapePoints == nullptr ? "nullptr" : m_pGraphObjAddingShapePoints->name());
        mthTracer.trace(strAddTrcInfo);
    }

    int iObjFactoryType = getCurrentDrawingToolGraphObjType();

    QCursor cursor = Qt::ArrowCursor;

    if( m_mode == EMode::Simulation )
    {
        cursor = Qt::ArrowCursor;
    }
    else if( m_mode == EMode::Edit )
    {
        QList<QGraphicsItem*> arpGraphicsItemsUnderCursor = items(i_ptPos);
        QGraphicsItem*        pGraphicsItem;
        int                   idxGraphObj;

        switch( m_editTool.enumerator() )
        {
            case EEditTool::Select:
            {
                switch( m_editMode.enumerator() )
                {
                    case EEditMode::Move:
                    {
                        cursor = Qt::ArrowCursor;
                        break;
                    }
                    case EEditMode::Resize:
                    {
                        switch( m_editResizeMode.enumerator() )
                        {
                            case EEditResizeMode::ResizeAll:
                            {
                                break;
                            }
                            case EEditResizeMode::ResizeHor:
                            {
                                break;
                            }
                            case EEditResizeMode::ResizeVer:
                            {
                                break;
                            }
                            case EEditResizeMode::Undefined:
                            default:
                            {
                                break;
                            }
                        }
                        break;
                    }
                    case EEditMode::Rotate:
                    {
                        break;
                    }
                    case EEditMode::MoveShapePoint:
                    {
                        break;
                    }
                    case EEditMode::EditText:
                    {
                        cursor = Qt::IBeamCursor;
                        break;
                    }
                    case EEditMode::Creating: // this mode is only used by graphical object but not by the scene
                    case EEditMode::Undefined:
                    default:
                    {
                        cursor = Qt::ArrowCursor;
                        break;
                    }
                } // switch( m_editMode )
                break;
            } // case EEditTool::Select:

            case EEditTool::CreateObjects:
            {
                switch( m_editMode.enumerator() )
                {
                    case EEditMode::Move:
                    {
                        break;
                    }
                    case EEditMode::Resize:
                    {
                        switch( m_editResizeMode.enumerator() )
                        {
                            case EEditResizeMode::ResizeAll:
                            {
                                break;
                            }
                            case EEditResizeMode::ResizeHor:
                            {
                                break;
                            }
                            case EEditResizeMode::ResizeVer:
                            {
                                break;
                            }
                            case EEditResizeMode::Undefined:
                            default:
                            {
                                break;
                            }
                        }
                        break;
                    }
                    case EEditMode::Rotate:
                    {
                        break;
                    }
                    case EEditMode::MoveShapePoint:
                    {
                        if( iObjFactoryType != static_cast<int>(EGraphObjTypeUndefined) )
                        {
                            cursor = Qt::CrossCursor;
                        }
                        break;
                    }
                    case EEditMode::EditText:
                    {
                        cursor = Qt::IBeamCursor;
                        break;
                    }
                    case EEditMode::Creating: // this mode is only used by graphical object but not by the scene
                    case EEditMode::Undefined:
                    default:
                    {
                        if( iObjFactoryType != static_cast<int>(EGraphObjTypeUndefined) )
                        {
                            // Special case for connection lines which may only be created on connection points.
                            if( iObjFactoryType == static_cast<int>(EGraphObjTypeConnectionLine) )
                            {
                                cursor = Qt::ArrowCursor;

                                for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsUnderCursor.size(); idxGraphObj++ )
                                {
                                    pGraphicsItem = arpGraphicsItemsUnderCursor[idxGraphObj];

                                    if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeConnectionPoint) )
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
            } // case EEditTool::CreateObjects

            case EEditTool::Undefined:
            default:
            {
                cursor = Qt::ArrowCursor;
                break;
            }
        } // switch( m_editTool )
    } // if( m_mode == EMode::Edit )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(qCursorShape2Str(cursor.shape()));
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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
        strMthInArgs += ", ScenePos:" + point2Str(i_ptScenePos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "bringToFront",
        /* strAddInfo   */ strMthInArgs );

    QList<QGraphicsItem*> arpGraphicsItems = items(i_ptScenePos);

    return bringToFront( i_pGraphicsItem, arpGraphicsItems );

} // bringToFront

//------------------------------------------------------------------------------
double CDrawingScene::sendToBack( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
        strMthInArgs += ", ScenePos:" + point2Str(i_ptScenePos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "sendToBack",
        /* strAddInfo   */ strMthInArgs );

    QList<QGraphicsItem*> arpGraphicsItems = items(i_ptScenePos);

    return sendToBack( i_pGraphicsItem, arpGraphicsItems );

} // sendToBack

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Brings the given graphic item into front of the given list of graphic items.
    If an empty list is passed the given graphic item will be pushed into front
    of all graphic items currently available in the scene.

    The order of the graphic items is defined by the zValue. After calling this method
    the zValue of the given graphic is 0.1. higher than the highest zValue of the
    given list of graphic items.

    @param i_pGraphicsItem [in] Pointer to item which should be pushed to front
    @param i_arpGraphicsItems [in] List of graphic items the item should be pushed into front.
                                   If the list is empty the item will be pushed into front of
                                   all items currently in the scene.

    @return Resulting zValue assigned to the given graphic item.
*/
double CDrawingScene::bringToFront( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "bringToFront",
        /* strAddInfo   */ strMthInArgs );

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

        if( pGraphicsItem->type() != static_cast<int>(EGraphObjTypeSelectionPoint)
         && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeLabel) )
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
/*! Sends the given graphic item into back of the given list of graphic items.
    If an empty list is passed the given graphic item will be pushed to the bottom
    of all graphic items currently available in the scene.

    The order of the graphic items is defined by the zValue. After calling this method
    the zValue of the given graphic is 0.1. lower than the lowest zValue of the
    given list of graphic items.

    @param i_pGraphicsItem [in] Pointer to item which should be pushed to back.
    @param i_arpGraphicsItems [in] List of graphic items the item should be pushed to back.
                                   If the list is empty the item will be pushed behind
                                   all items currently in the scene.

    @return Resulting zValue assigned to the given graphic item.
*/
double CDrawingScene::sendToBack( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItem);
        strMthInArgs = QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "sendToBack",
        /* strAddInfo   */ strMthInArgs );

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

        if( pGraphicsItem->type() != static_cast<int>(EGraphObjTypeSelectionPoint)
         && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeLabel) )
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
/*! This method has to be called by graphical objects (polylines and connection lines)
    if new shape points will be inserted at the beginning or appended at the end
    of the polylines so that the drawing scene will further on dispatch mouse events
    "directly" to the object "under construction". On extended polylines the object
    is not hit and the graphics scene does not dispatch the mouse event to the objects
    or their selection points (similar to newly creating objects).

    @param i_pGraphObj [in] Pointer to graphic item.
*/
void CDrawingScene::onGraphObjAddingShapePointsStarted( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjAddingShapePointsStarted",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", GraphObjAddingShapePoints:" + QString(m_pGraphObjAddingShapePoints == nullptr ? "nullptr" : m_pGraphObjAddingShapePoints->name());
        mthTracer.trace(strAddTrcInfo);
    }

    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr" );
    }

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
/*! This method has to be called by graphical objects (polylines and connection lines)
    if new shape points will be inserted at the beginning or appended at the end
    of the polylines so that the drawing scene will further on dispatch mouse events
    "directly" to the object "under construction". On extended polylines the object
    is not hit and the graphics scene does not dispatch the mouse event to the objects
    or their selection points (similar to newly creating objects).

    @param i_pGraphObj [in] Pointer to graphic item.
*/
void CDrawingScene::onGraphObjAddingShapePointsFinished( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjAddingShapePointsFinished",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->keyInTree());
        strAddTrcInfo += ", GraphObjAddingShapePoints:" + QString(m_pGraphObjAddingShapePoints == nullptr ? "nullptr" : m_pGraphObjAddingShapePoints->keyInTree());
        mthTracer.trace(strAddTrcInfo);
    }

    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr" );
    }

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
/*! This method has to be called by graphical objects AFTER they have been
    initially created (after they have been completely created e.g. after
    a rectangle has been resized and the mouse button has been released).
    After creating the object via mouse events the edit mode of the drawing
    has to be reset.

    @param i_pGraphObj [in] Pointer to graphic item.
*/
void CDrawingScene::onGraphObjCreationFinished( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjCreationFinished",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", GraphObjAddingShapePoints:" + QString(m_pGraphObjAddingShapePoints == nullptr ? "nullptr" : m_pGraphObjAddingShapePoints->name());
        mthTracer.trace(strAddTrcInfo);
    }

    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr" );
    }

    // Reset drawings scenes edit modes:
    //----------------------------------

    if( m_mode == EMode::Edit )
    {
        if( m_editTool == EEditTool::CreateObjects )
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

                setMode(EMode::Ignore, EEditTool::Ignore, EEditMode::Undefined, EEditResizeMode::Undefined, false);
            }
        }
    }

    //emit graphObjCreated(i_pGraphObj);

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        QString strAddTrcInfo = "GraphObjId:" + i_pGraphObj->keyInTree();
        mthTracer.trace(strAddTrcInfo);
    }

} // onGraphObjCreationFinished

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjDestroying( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "GraphObjId:" + i_strObjId;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjDestroying",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        mthTracer.trace(strAddTrcInfo);
    }

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

    //if( m_dctpGraphObjs.contains(i_strObjId) )
    //{
    //    //emit graphObjDestroying(i_strObjId);
    //
    //    m_dctpGraphObjs.remove(i_strObjId);
    //
    //    if( m_dctpGraphObjsClipboard.contains(i_strObjId) )
    //    {
    //        m_dctpGraphObjs.remove(i_strObjId);
    //    }
    //}

    if( m_pGraphObjCreating != nullptr && m_pGraphObjCreating->keyInTree() == i_strObjId )
    {
        m_pGraphicsItemCreating = nullptr;
        m_pGraphObjCreating = nullptr;
    }

    if( m_pGraphObjAddingShapePoints != nullptr && m_pGraphObjAddingShapePoints->keyInTree() == i_strObjId )
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

            if( pGraphObj != nullptr && pGraphObj->keyInTree() == i_strObjId )
            {
                m_arpGraphicsItemsAcceptingHoverEvents.removeAt(idxGraphObj);
            }
        }

    } // if( m_arpGraphicsItemsAcceptingHoverEvents.size() > 0 )

} // onGraphObjDestroying

////------------------------------------------------------------------------------
//void CDrawingScene::onGraphObjDestroyed( const QString& i_strObjId )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strMthInArgs  = "GraphObjId:" + i_strObjId;
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onGraphObjDestroyed",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo = "Mode:" + m_mode.toString();
//        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
//        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
//        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
//        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
//    // dtor of QGraphicsItem has already been executed. The order the dtors
//    // of inherited classes are called depends on the order the classes
//    // appear in the list of the inherited classes on defining the
//    // class implementation. So we can't call "removeItem" here but must
//    // remove the graphics item from the drawing scene's item list before
//    // the dtor of class QGraphicsItem is called. And this is only always
//    // the case in the dtor of the class derived from QGraphicsItem.
//
//    //QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    //removeItem(pGraphicsItem);
//
//    //emit graphObjDestroyed(i_strObjId);
//
//} // onGraphObjDestroyed

////------------------------------------------------------------------------------
//void CDrawingScene::onGraphObjIdChanged( const QString& i_strObjIdOld, const QString& i_strObjIdNew )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strMthInArgs  = "GraphObjIdOld:" + i_strObjIdOld;
//        strMthInArgs += ", GraphObjIdNew:" + i_strObjIdNew;
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onGraphObjIdChanged",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "Mode:" + m_mode.toString();
//        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
//        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
//        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
//        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    //emit graphObjIdChanged(i_strObjIdOld, i_strObjIdNew);
//
//} // onGraphObjIdChanged

////------------------------------------------------------------------------------
//void CDrawingScene::onGraphObjNameChanged(
//    const QString& i_strObjId,          // !!! Contains the OLD object id   !!!
//    const QString& i_strObjNameOld,     // !!! NOT including name of parents !!!
//    const QString& i_strObjNameNew )    // !!! NOT including name of parents !!!
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strMthInArgs  = "GraphObjId:" + i_strObjId;
//        strMthInArgs += ", GraphObjNameOld:" + i_strObjNameOld;
//        strMthInArgs += ", GraphObjNameNew:" + i_strObjNameNew;
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onGraphObjNameChanged",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "Mode:" + m_mode.toString();
//        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
//        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
//        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
//        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    //emit graphObjNameChanged(i_strObjId, i_strObjNameOld, i_strObjNameNew);
//
//} // onGraphObjNameChanged

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QGraphicsItem* CDrawingScene::findGraphicsItem( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    QGraphicsItem* pGraphicsItem = nullptr;
    CIdxTreeEntry* pTreeEntry = m_pGraphObjsIdxTree->findEntry(i_strObjId);
    if( pTreeEntry != nullptr )
    {
        pGraphicsItem = dynamic_cast<QGraphicsItem*>(pTreeEntry);
    }
    return pGraphicsItem;
}

//------------------------------------------------------------------------------
CGraphObj* CDrawingScene::findGraphObj( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = nullptr;
    CIdxTreeEntry* pTreeEntry = m_pGraphObjsIdxTree->findEntry(i_strObjId);
    if( pTreeEntry != nullptr )
    {
        pGraphObj = dynamic_cast<CGraphObj*>(pTreeEntry);
    }
    return pGraphObj;
}

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

    QString strObjName = i_pGraphObj->name();
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

    if( i_pGraphObj->parentGraphObj() != nullptr )
    {
        strObjNameParent = i_pGraphObj->parentGraphObj()->name();
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
                if( strObjName.compare(pGraphObj->name(),Qt::CaseInsensitive) == 0 )
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
/*! Combines the currently selected objects into a group.

    A graphical group item is created and the selected objects will be added
    as childs of this group.

    The coordinates of the child objects are adjusted so that they are relative
    to their new parent group.
*/
int CDrawingScene::groupGraphObjsSelected()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "groupGraphObjsSelected",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        mthTracer.trace(strAddTrcInfo);
    }

    int iObjsGroupedCount = 0;

    QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();

    if( arpGraphicsItemsSelected.size() > 1 )
    {
        CObjFactory* pObjFactoryTmp = CObjFactory::FindObjFactory(CObjFactory::c_strGroupNameStandardShapes, graphObjType2Str(EGraphObjTypeGroup));

        CObjFactoryGroup* pObjFactoryGroup = dynamic_cast<CObjFactoryGroup*>(pObjFactoryTmp);

        if( pObjFactoryGroup != nullptr )
        {
            CEnumEditTool editToolPrev = m_editTool;

            m_editTool = EEditTool::CreateObjects;

            m_pGraphObjCreating = pObjFactoryGroup->createGraphObj(
                /* pDrawingScene */ this,
                /* ptItemPos     */ QPointF(0.0,0.0),
                /* drawSettings  */ m_drawSettings );

            m_pGraphicsItemCreating = dynamic_cast<QGraphicsItem*>(m_pGraphObjCreating);

            if( m_pGraphicsItemCreating == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "m_pGraphicsItemCreating == nullptr" );
            }

            CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(m_pGraphObjCreating);

            if( pGraphObjGroup == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjGroup == nullptr" );
            }

            QGraphicsItemGroup* pGraphicsItemGroup = dynamic_cast<QGraphicsItemGroup*>(pGraphObjGroup);

            if( pGraphicsItemGroup == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItemGroup == nullptr" );
            }

            // Add new (empty) group to graphics scene.
            addGraphObj(pGraphObjGroup);

            // Start creation of group.
            pGraphObjGroup->setEditMode(EEditMode::Creating);

            // Child objects are positioned relative to the top left corner of the
            // bounding rectangle of the parent object. On newly creating an object
            // they will be positioned relative to the top left corner of the drawing.
            // On adding the object to a parent group object they will be positioned
            // relative to the top left corner of the group's bounding rectangle.

            QRectF         rctGroupSceneCoors;
            double         fXLeftMin   = INT_MAX;
            double         fYTopMin    = INT_MAX;
            double         fXRightMax  = INT_MIN;
            double         fYBottomMax = INT_MIN;

            //SGraphObjAlignment alignmentWidth(
            //    /* refChild  */ EAlignmentRef::Width,
            //    /* refParent */ EAlignmentRef::Width,
            //    /* bAbsolute */ false );
            //SGraphObjAlignment alignmentHeight(
            //    /* refChild  */ EAlignmentRef::Height,
            //    /* refParent */ EAlignmentRef::Height,
            //    /* bAbsolute */ false );
            //SGraphObjAlignment alignmentLeft(
            //    /* refChild  */ EAlignmentRef::Left,
            //    /* refParent */ EAlignmentRef::Left,
            //    /* bAbsolute */ false );
            //SGraphObjAlignment alignmentTop(
            //    /* refChild  */ EAlignmentRef::Top,
            //    /* refParent */ EAlignmentRef::Top,
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
            for( auto* pGraphicsItemSelected : arpGraphicsItemsSelected )
            {
                CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);

                if( pGraphObjSelected == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjSelected == nullptr" );
                }

                if( pGraphObjSelected->getType() != EGraphObjTypeConnectionLine )
                {
                    QPointF posItem = pGraphObjSelected->getPos();
                    QSizeF  sizItem = pGraphObjSelected->getSize();
                    QRectF  rctItem = QRectF(posItem, sizItem);

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
                } // if( pGraphObjSelected->getType() != EGraphObjTypeConnectionLine )
            } // for( auto* pGraphicsItemSelected : arpGraphicsItemsSelected )

            rctGroupSceneCoors.setLeft(fXLeftMin);
            rctGroupSceneCoors.setTop(fYTopMin);
            rctGroupSceneCoors.setRight(fXRightMax);
            rctGroupSceneCoors.setBottom(fYBottomMax);

            pGraphObjGroup->setPos( rctGroupSceneCoors.topLeft() );

            // Add child items to group.
            for( auto* pGraphicsItemSelected : arpGraphicsItemsSelected )
            {
                CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);

                if( pGraphObjSelected->getType() != EGraphObjTypeConnectionLine )
                {
                    QPointF posItem = pGraphObjSelected->getPos(); // for debugging purposes also called here before adding the item to the group
                    QSizeF  sizItem = pGraphObjSelected->getSize();

                    pGraphicsItemGroup->addToGroup(pGraphicsItemSelected);

                    posItem = pGraphObjSelected->getPos();
                    sizItem = pGraphObjSelected->getSize();

                    m_pGraphObjsIdxTree->move(pGraphObjSelected, pGraphObjGroup);

                    pGraphObjSelected->acceptCurrentAsOriginalCoors();

                    // Unselect object to destroy selection points.
                    pGraphicsItemSelected->setSelected(false);

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

                } // if( pGraphObj->getType() != EGraphObjTypeConnectionLine )
            } // for( auto* pGraphicsItem : arpGraphicsItemsSelected )

            // Finish creation of group.
            pGraphObjGroup->setSize( rctGroupSceneCoors.size() );
            pGraphObjGroup->acceptCurrentAsOriginalCoors();
            pGraphObjGroup->setEditMode(EEditMode::Undefined);
            pGraphObjGroup->setEditResizeMode(EEditResizeMode::Undefined);

            onGraphObjCreationFinished(pGraphObjGroup);

            pGraphObjGroup->setSelected(true);

            m_editTool = editToolPrev;

        } // if( pObjFactoryGroup != nullptr )
    } // if( arpGraphicsItemsSelected.size() > 1 )

    return iObjsGroupedCount;

} // groupGraphObjsSelected

//------------------------------------------------------------------------------
/*! Dissolves all currently selected group objects.

    The child objects are retained and are assigned to the drawing scene as
    immediate child objects. The coordinates of the child objects are adjusted so
    that they become relative to the drawing scene.
*/
int CDrawingScene::ungroupGraphObjsSelected()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ungroupGraphObjsSelected",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        mthTracer.trace(strAddTrcInfo);
    }

    int iObjsUngroupedCount = 0;

    QList<QGraphicsItem*> arpGraphicsItemsSelected = selectedItems();

    for( auto* pGraphicsItemSelected : arpGraphicsItemsSelected )
    {
        CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);

        if( pGraphObjSelected == nullptr )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjSelected == nullptr" );
        }

        if( pGraphObjSelected->getType() == EGraphObjTypeGroup )
        {
            CGraphObjGroup* pGraphObjGroupSelected = dynamic_cast<CGraphObjGroup*>(pGraphicsItemSelected);

            if( pGraphObjGroupSelected == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjGroup == nullptr" );
            }

            QGraphicsItemGroup* pGraphicsItemGroupSelected = dynamic_cast<QGraphicsItemGroup*>(pGraphObjGroupSelected);

            if( pGraphicsItemGroupSelected == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItemGroup == nullptr" );
            }

            for( auto* pGraphicsItemChild : pGraphicsItemGroupSelected->childItems() )
            {
                CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);

                if( pGraphObjChild == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjChild == nullptr" );
                }

                QPointF posItem = pGraphObjSelected->getPos(); // for debugging purposes also called here before removing the item from the group
                QSizeF  sizItem = pGraphObjSelected->getSize();

                pGraphicsItemGroupSelected->removeFromGroup(pGraphicsItemChild);

                posItem = pGraphObjSelected->getPos();
                sizItem = pGraphObjSelected->getSize();

                m_pGraphObjsIdxTree->move(pGraphObjChild, nullptr);
            }

            delete pGraphObjGroupSelected;
            pGraphObjGroupSelected = nullptr;

            iObjsUngroupedCount++;

        } // if( pGraphObjSelected->getType() == EGraphObjTypeGroup )
    } // for( auto* pGraphicsItemSelected : arpGraphicsItemsSelected )

    return iObjsUngroupedCount;

} // ungroupGraphObjsSelected

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::setDrawSettings( const CDrawSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDrawSettings",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_clr.name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setPenColor",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_iPenWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setPenWidth",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLineStyle(i_lineStyle).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineStyle",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_clr.name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFillColor",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumFillStyle(i_fillStyle).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFillStyle",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLineRecordType(i_lineRecordType).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineRecordType",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_iLineExtent);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineExtent",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndStyle(i_endStyle).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndStyle",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndBaseLineType(i_baseLineType).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndBaseLineType",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndFillStyle(i_fillStyle).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndFillStyle",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndWidth(i_width).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndWidth",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndLength(i_length).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setLineEndLength",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_clr.name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextColor",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_fnt.family();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextFont",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = textSize2Str(i_size);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextSize",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumTextStyle(i_style).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextStyle",
        /* strAddInfo   */ strMthInArgs );

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
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumTextEffect(i_effect).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setTextEffect",
        /* strAddInfo   */ strMthInArgs );

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

    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", EvPossibleActions:" + qDropActions2Str(i_pEv->possibleActions());
        strMthInArgs += ", EvProposedAction:" + qDropActions2Str(i_pEv->proposedAction());
        strMthInArgs += ", EvDropAction:" + qDropActions2Str(i_pEv->dropAction());
        strMthInArgs += ", MimeDataText:" + strMimeData;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", GraphObjAddingShapePoints:" + QString(m_pGraphObjAddingShapePoints == nullptr ? "nullptr" : m_pGraphObjAddingShapePoints->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

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

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", EvPossibleActions:" + qDropActions2Str(i_pEv->possibleActions());
        strMthInArgs += ", EvProposedAction:" + qDropActions2Str(i_pEv->proposedAction());
        strMthInArgs += ", EvDropAction:" + qDropActions2Str(i_pEv->dropAction());
        strMthInArgs += ", MimeDataText:" + strMimeData;

        strMthInArgs += ", Mode:" + m_mode.toString();
        strMthInArgs += ", EditTool:" + m_editTool.toString();
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", EditResizeMode:" + m_editResizeMode.toString();

        strMthInArgs += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strMthInArgs += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->path());

        strMthInArgs += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strMthInArgs += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragMoveEvent",
        /* strAddInfo   */ strMthInArgs );

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

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", EvPossibleActions:" + qDropActions2Str(i_pEv->possibleActions());
        strMthInArgs += ", EvProposedAction:" + qDropActions2Str(i_pEv->proposedAction());
        strMthInArgs += ", EvDropAction:" + qDropActions2Str(i_pEv->dropAction());
        strMthInArgs += ", MimeDataText:" + strMimeData;

        strMthInArgs += ", Mode:" + m_mode.toString();
        strMthInArgs += ", EditTool:" + m_editTool.toString();
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", EditResizeMode:" + m_editResizeMode.toString();

        strMthInArgs += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strMthInArgs += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->path());

        strMthInArgs += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strMthInArgs += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

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

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strMthInArgs += ", EvPossibleActions:" + qDropActions2Str(i_pEv->possibleActions());
        strMthInArgs += ", EvProposedAction:" + qDropActions2Str(i_pEv->proposedAction());
        strMthInArgs += ", EvDropAction:" + qDropActions2Str(i_pEv->dropAction());
        strMthInArgs += ", MimeDataText:" + strMimeData;

        strMthInArgs += ", Mode:" + m_mode.toString();
        strMthInArgs += ", EditTool:" + m_editTool.toString();
        strMthInArgs += ", EditMode:" + m_editMode.toString();
        strMthInArgs += ", EditResizeMode:" + m_editResizeMode.toString();

        strMthInArgs += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strMthInArgs += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->path());

        strMthInArgs += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strMthInArgs += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dropEvent",
        /* strAddInfo   */ strMthInArgs );

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

                                addGraphObj(pGraphObj);

                                pGraphicsItem->setPos( i_pEv->scenePos() );
                                pGraphicsItem->setSelected(true);

                                onGraphObjCreationFinished(pGraphObj);

                                setMode( EMode::Ignore, EEditTool::Select, EEditMode::Move, EEditResizeMode::Undefined, false );
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

                addGraphObj(pGraphObjImage);

                pGraphObjImage->setPos( i_pEv->scenePos() );

                onGraphObjCreationFinished(pGraphObjImage);

                setMode( EMode::Ignore, EEditTool::Select, EEditMode::Move, EEditResizeMode::Undefined, false );
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
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

    bool bEventHandled = false;

    QPointF ptMouseScenePos = i_pEv->scenePos();

    m_ptMouseEvScenePosOnMousePressEvent = ptMouseScenePos;

    emit mousePosChanged(m_ptMouseEvScenePosOnMousePressEvent);

    int iObjFactoryType = static_cast<int>(EGraphObjTypeUndefined);

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }

    if( m_mode != EMode::Edit )
    {
        if( m_pGraphObjCreating != nullptr || iObjFactoryType != static_cast<int>(EGraphObjTypeUndefined) )
        {
            throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError);
        }
    }

    if( m_mode == EMode::Edit )
    {
        // If currently an object is "under construction" ...
        if( m_pGraphObjCreating != nullptr )
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

            sendEvent(m_pGraphicsItemCreating, &evMouse);

            bEventHandled = true;

        } // if( m_pGraphObjCreating != nullptr )

        // If currently an object is "under construction" ...
        else if( m_pGraphObjAddingShapePoints != nullptr )
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

            sendEvent(m_pGraphicsItemAddingShapePoints, &evMouse);

            bEventHandled = true;

        } // if( m_pGraphObjAddingShapePoints != nullptr )

        // If currently no object is "under construction" ...
        else // if( m_pGraphObjCreating == nullptr && m_pGraphObjAddingShapePoints == nullptr )
        {
            if( m_editTool == EEditTool::CreateObjects )
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
                    if( iObjFactoryType == static_cast<int>(EGraphObjTypeConnectionLine) )
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

                            addGraphObj(m_pGraphObjCreating);

                            m_pGraphObjCreating->setEditMode(EEditMode::Creating);

                            if( iObjFactoryType == static_cast<int>(EGraphObjTypeConnectionLine)
                             && m_pGraphicsItemCreating->type() == static_cast<int>(EGraphObjTypeConnectionLine) )
                            {
                                pGraphObjCnctLineCreating = dynamic_cast<CGraphObjConnectionLine*>(m_pGraphObjCreating);
                            }
                            else if( iObjFactoryType == static_cast<int>(EGraphObjTypeConnectionPoint)
                                  && m_pGraphicsItemCreating->type() == static_cast<int>(EGraphObjTypeConnectionPoint) )
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
                                pGraphObjCnctLineCreating->setConnectionPoint(ELinePoint::Start,pGraphObjCnctPtHit);
                            }

                            emit modeChanged();

                        } // if( m_pGraphObjCreating != nullptr )
                    } // if( bCreateObj )
                } // if( m_pObjFactory != nullptr )

                // If an object has been newly created and is still under construction ...
                if( m_pGraphObjCreating != nullptr )
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

                    sendEvent(m_pGraphicsItemCreating, &evMouse);

                    bEventHandled = true;

                } // if( m_pGraphObjCreating != nullptr )
            } // if( m_editTool == EEditTool::CreateObjects )
        } // if( m_pGraphObjCreating == nullptr && m_pGraphObjAddingShapePoints == nullptr )
    } // if( m_mode == EMode::Edit )

    if( !bEventHandled )
    {
        // Dispatch mouse move events to objects under mouse cursor. Some items may completely overlap (encircle)
        // other objects (a big rectangle may completely enclose a smaller rectangle or the bounding rectangle of
        // a polyline may enclose other objects). In this case Qt's graphic scene does not dispatch the mouse events
        // as hover events to the enclosed objects if they are not "on top" (according to the scene's item list Z order)
        // of the enclosing items. If the outer object does not hide the inner object (fill style solid pattern) this
        // behavior is not what we want and what the user expects. E.g. if a rectangle with "FillStyle = NoFill" would
        // enclose another object the user would expect that the inner object can be selected by mouse clicks.
        // And if you consider polylines which are never "filled" objects the inner objects should always be selectable
        // by mouse clicks. The polyline as the outer object should only be selected if one of its line segments would be hit.

        QList<QGraphicsItem*> arpGraphicsItemsIntersected = items( i_pEv->scenePos(), Qt::IntersectsItemShape, Qt::AscendingOrder );
        QList<QGraphicsItem*> arpGraphicsItemsHit;
        QStringList           strlstKeysInTreeGraphicsItemsBroughtToFront;
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
                                        if( pGraphicsItem->type() != static_cast<int>(EGraphObjTypeSelectionPoint)
                                         && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeLabel)
                                         && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeConnectionPoint)
                                         && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeGroup) )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iSelectionPoints )
                                    {
                                        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeSelectionPoint) )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iLabels )
                                    {
                                        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeLabel) )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iConnectionPoints )
                                    {
                                        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeConnectionPoint) )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iGroups )
                                    {
                                        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeGroup) )
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
                                        if( pGraphicsItem->type() != static_cast<int>(EGraphObjTypeSelectionPoint)
                                         && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeLabel)
                                         && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeConnectionPoint)
                                         && pGraphicsItem->type() != static_cast<int>(EGraphObjTypeGroup) )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iSelectionPoints )
                                    {
                                        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeSelectionPoint) )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iLabels )
                                    {
                                        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeLabel) )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iConnectionPoints )
                                    {
                                        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeConnectionPoint) )
                                        {
                                            bBringToFront = true;
                                        }
                                    }
                                    else if( idxObjType == c_iGroups )
                                    {
                                        if( pGraphicsItem->type() == static_cast<int>(EGraphObjTypeGroup) )
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
                                strlstKeysInTreeGraphicsItemsBroughtToFront.append(pGraphObj->keyInTree());
                            }
                        }
                    }
                }
            }
        } // if( arpGraphicsItemsHit.size() > 0 )

        // Dispatch mouse press event to objects which have been hit.
        QGraphicsScene::mousePressEvent(i_pEv);

        // After the mouse event has been dispatched the Z-order of the objects temporarily brought to front must be restored.
        // Because while dispatching the mouse press event to the graphics scene objects some of the object may have been destroyed
        // (e.g. selection points or labels) not the pointer to the objects but their keys (object ids) have been stored.
        // The dtor of CGraphObj will remove themselves from the index tree and they cannot be found anymore.
        for( const auto& strKeyInTree : strlstKeysInTreeGraphicsItemsBroughtToFront )
        {
            pGraphObj = findGraphObj(strKeyInTree);

            if( pGraphObj != nullptr )
            {
                pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
                pGraphicsItem->setZValue( pGraphObj->getStackingOrderValue() );
            }
        }

        // In edit mode ..
        if( m_mode == EMode::Edit )
        {
            // If no item has been hit ...
            if( selectedItems().size() == 0 )
            {
                setMode( EMode::Ignore, EEditTool::Ignore, EEditMode::Undefined, EEditResizeMode::Undefined, false );

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
        strAddTrcInfo  = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", EditResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CDrawingScene::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseMoveEvent != nullptr && m_pTrcAdminObjMouseMoveEvent->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().x()) + ")";
        strMthInArgs += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().x()) + ")";
        strMthInArgs += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().x()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvent,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        mthTracer.trace(strAddTrcInfo);
    }

    int iMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;

    QPointF ptMouseScenePos = i_pEv->scenePos();

    emit mousePosChanged(ptMouseScenePos);

    int iObjFactoryType = static_cast<int>(EGraphObjTypeUndefined);

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }

    if( m_mode != EMode::Edit )
    {
        if( m_pGraphObjCreating != nullptr || iObjFactoryType != static_cast<int>(EGraphObjTypeUndefined) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultInternalProgramError);
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
        if( m_pGraphObjCreating != nullptr )
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

            sendEvent(m_pGraphicsItemCreating, &evMouse);

        } // if( m_pGraphObjCreating != nullptr )

        // If currently an object is "under construction" ...
        else if( m_pGraphObjAddingShapePoints != nullptr )
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

            sendEvent(m_pGraphicsItemAddingShapePoints, &evMouse);

        } // if( m_pGraphObjAddingShapePoints != nullptr )

        // If currently no object is "under construction" ...
        else // if( m_pGraphObjCreating == nullptr && m_pGraphObjAddingShapePoints == nullptr )
        {
            bool bDispatchMouseEvents2ObjectsUnderMouseCursor = false;
            bool bDispatchMouseEvents2ConnectionPointsUnderMouseCursor = false;

            if( m_editTool == EEditTool::CreateObjects )
            {
                // If the left mouse button is pressed ...
                if( iMouseButtonState & Qt::LeftButton )
                {
                    // If the drawing tool "Point" is selected ...
                    if( m_pObjFactory != nullptr && iObjFactoryType == static_cast<int>(EGraphObjTypePoint) )
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

                        addGraphObj(m_pGraphObjCreating);

                        m_pGraphObjCreating->setEditMode(EEditMode::Creating);

                        m_pGraphicsItemCreating->setPos(ptMouseScenePos);
                        m_pGraphicsItemCreating->setSelected(true);

                        // The current "mouseMove" event is not dispatched to the newly created object but to the
                        // object previously created by the "mousePress" event (even if the previously created
                        // object was explicitly unselected by the "onGraphObjCreationFinished" method). So we call
                        // "onGraphObjCreationFinished" on our own hoping that the object states set by the ctor are
                        // sufficient for the graphical object.
                        onGraphObjCreationFinished(m_pGraphObjCreating);

                    } // if( m_pObjFactory != nullptr && iObjFactoryType == EGraphObjTypePoint )
                } // if( iMouseButtonState & Qt::LeftButton )

                // If no mouse button is pressed ...
                else if( iMouseButtonState == Qt::NoButton )
                {
                    // If the drawing tool "ConnectionLine" is selected ...
                    if( m_pObjFactory != nullptr && iObjFactoryType == static_cast<int>(EGraphObjTypeConnectionLine) )
                    {
                        bDispatchMouseEvents2ConnectionPointsUnderMouseCursor = true;
                    }

                } // if( iMouseButtonState == Qt::NoButton )
            } // if( m_editTool == EEditTool::CreateObjects )

            else if( m_editTool == EEditTool::Select )
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
            } // if( m_editTool == EEditTool::Select )

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
                        } // if( pGraphicsItem != nullptr && pGraphObj != nullptr )
                    } // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsIntersected.size(); idxGraphObj++ )
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
                    } // if( pGraphicsItemHit != nullptr && pGraphObjHit != nullptr )
                } // for( idxGraphObjHit = 0; idxGraphObjHit < arpGraphicsItemsHit.size(); idxGraphObjHit++ )

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
        } // if( m_pGraphObjCreating == nullptr && m_pGraphObjAddingShapePoints == nullptr )
    } // if( m_mode == EMode::Edit )

    else if( m_mode == EMode::Simulation )
    {
        // ... dispatch mouse event to objects "under cursor".
        QGraphicsScene::mouseMoveEvent(i_pEv);
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", EditResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CDrawingScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().x()) + ")";
        strMthInArgs += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().x()) + ")";
        strMthInArgs += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().x()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

    int iObjFactoryType = static_cast<int>(EGraphObjTypeUndefined);

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }

    if( m_mode != EMode::Edit )
    {
        if( m_pGraphObjCreating != nullptr || iObjFactoryType != static_cast<int>(EGraphObjTypeUndefined) )
        {
            throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError);
        }
    }

    QPointF ptScenePosMouseEvent = i_pEv->scenePos();

    emit mousePosChanged(ptScenePosMouseEvent);

    if( m_mode == EMode::Edit )
    {
        // If currently an object is "under construction" ...
        if( m_pGraphObjCreating != nullptr )
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
            sendEvent(m_pGraphicsItemCreating, pEv);
            delete pEv;
            pEv = nullptr;

        } // if( m_pGraphObjCreating != nullptr )

        // If currently an object is "under construction" ...
        else if( m_pGraphObjAddingShapePoints != nullptr )
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
            sendEvent(m_pGraphicsItemAddingShapePoints, pEv);
            delete pEv;
            pEv = nullptr;

        } // if( m_pGraphObjAddingShapePoints != nullptr )

        // If currently no object is "under construction" ...
        else // if( m_pGraphObjCreating == nullptr && m_pGraphObjAddingShapePoints == nullptr )
        {
            // ... dispatch mouse press event to objects "under cursor".
            QGraphicsScene::mouseReleaseEvent(i_pEv);

            if( m_pGraphicsItemSelectionArea != nullptr )
            {
                removeItem(m_pGraphicsItemSelectionArea);

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
        } // if( m_pGraphObjCreating == nullptr && m_pGraphObjAddingShapePoints == nullptr )
    } // if( m_mode == EMode::Edit )

    else if( m_mode == EMode::Simulation )
    {
        // ... dispatch mouse event to objects "under cursor".
        QGraphicsScene::mouseReleaseEvent(i_pEv);
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", EditResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CDrawingScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "EvItemPos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().x()) + ")";
        strMthInArgs += ", EvScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().x()) + ")";
        strMthInArgs += ", EvScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().x()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

    // Not a nice hack: mouse double click events will not be dispatched to the graphical objects
    // but will be converted to mouse press and mouse release events which are just dispatched
    // to the selection points. The "sceneEventFilter" functions of the graphical objects may
    // check the "DoubleClickInProcessFlag" to correct again the mouse press event into a mouse
    // double click event.
    m_bMouseDoubleClickEventInProcess = true;

    int iObjFactoryType = static_cast<int>(EGraphObjTypeUndefined);

    if( m_pObjFactory != nullptr )
    {
        iObjFactoryType = m_pObjFactory->getGraphObjType();
    }

    if( m_mode != EMode::Edit )
    {
        if( m_pGraphObjCreating != nullptr || iObjFactoryType != static_cast<int>(EGraphObjTypeUndefined) )
        {
            throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError);
        }
    }

    QPointF ptScenePosMouseEvent = i_pEv->scenePos();

    emit mousePosChanged(ptScenePosMouseEvent);

    if( m_mode == EMode::Edit )
    {
        // If currently an object is "under construction" ...
        if( m_pGraphObjCreating != nullptr )
        {
            // If currently a connection line is "under construction" ...
            if( m_pGraphicsItemCreating->type() == static_cast<int>(EGraphObjTypeConnectionLine) )
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
                    bIsValidEndPoint = pGraphObjCnctLine->setConnectionPoint(ELinePoint::End,pGraphObjCnctPt);
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
            } // if( m_pGraphicsItemCreating->type() == EGraphObjTypeConnectionLine )

            // ... dispatch mouse event to object "under construction".
            if( m_pGraphObjCreating != nullptr )
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
                sendEvent(m_pGraphicsItemCreating, pEv);
                delete pEv;
                pEv = nullptr;
            }
        } // if( m_pGraphObjCreating != nullptr )

        // If currently an object is "under construction" ...
        else if( m_pGraphObjAddingShapePoints != nullptr )
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
            sendEvent(m_pGraphicsItemAddingShapePoints, pEv);
            delete pEv;
            pEv = nullptr;

        } // if( m_pGraphObjAddingShapePoints != nullptr )

        // If currently no object is "under construction" ...
        else // if( m_pGraphObjCreating == nullptr && m_pGraphObjAddingShapePoints == nullptr )
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
        strAddTrcInfo  = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", EditResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
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
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "Key:" + qKeyCode2Str(i_pEv->key()) + " (" + i_pEv->text() + ")";
        strMthInArgs += ", Count:" + QString::number(i_pEv->count());
        strMthInArgs += ", IsAutoRepeat:" + QString::number(i_pEv->count());
        strMthInArgs += ", Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

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
            if( m_editTool == EEditTool::Select )
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
            } // if( m_editTool == EEditTool::Select )
        } // if( i_pEv->key() == Qt::Key_Left || i_pEv->key() == Qt::Key_Right || i_pEv->key() == Qt::Key_Up || i_pEv->key() == Qt::Key_Down )

        // Pressed any key for moving or changing the shape of the objects ...
        else if( i_pEv->key() == Qt::Key_Plus || i_pEv->key() == Qt::Key_Minus )
        {
            if( m_editTool == EEditTool::Select )
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
            } // if( m_editTool == EEditTool::Select )
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
                        strlstGraphObjIdsSelected.append( pGraphObj->keyInTree() );
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
                //// Clear internal clipboard.
                //if( m_dctpGraphObjsClipboard.size() > 0 )
                //{
                //    QMap<QString,CGraphObj*>::iterator itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

                //    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                //    {
                //        pGraphObj = itGraphObjsClipboard.value();

                //        if( pGraphObj != nullptr )
                //        {
                //            deleteItem(pGraphObj);
                //        }
                //        ++itGraphObjsClipboard;
                //    }
                //}
                //m_dctpGraphObjsClipboard.clear();

                //// Add selected objects to the internal clipboard. Not only the references
                //// are copied but the objects will be newly created (cloned). By simply storing
                //// the references changing attributes on currently selected objects would take
                //// effect on objects in the clipboard.
                //// Only those objects will be added to the clipboard which don't belong to a
                //// group (which don't have parent objects) as the parent object must clone its
                //// children.
                //// Cloning connection points and connection lines is not that simply as cloning
                //// a group with children. Connection lines can only be cloned if both of its
                //// connection points will be cloned and the cloned connection line must be
                //// connected with the clones of its original connection points.

                //QList<QGraphicsItem*>     arpGraphicsItemsSelected = selectedItems();
                //CGraphObj*                pGraphObjClone;
                //CGraphObjConnectionLine*  pGraphObjCnctLine;
                //CGraphObjConnectionLine*  pGraphObjCnctLineClone;
                //CGraphObjConnectionPoint* pGraphObjCnctPt1;
                //CGraphObjConnectionPoint* pGraphObjCnctPt2;
                //CGraphObjConnectionPoint* pGraphObjCnctPt1Clone;
                //CGraphObjConnectionPoint* pGraphObjCnctPt2Clone;
                //QString                   strGraphObjName;
                //QString                   strGraphObjId;
                //QList<CGraphObj*>         arpGraphObjsOrigin;
                //QList<CGraphObj*>         arpGraphObjsClones;
                //int                       idxGraphObjTmp;

                //// During the first loop connection lines will not be cloned. Instead we are going to keep
                //// a list with original connection points and their clones. This way we are able in the
                //// second loop on cloning connection lines to detect the clones of the connection points.
                //for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                //{
                //    pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];

                //    // Only copy top level parent objects ..
                //    if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
                //    {
                //        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                //        if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                //        {
                //            pGraphObjClone = pGraphObj->clone();

                //            if( pGraphObjClone != nullptr )
                //            {
                //                // The name of the clone should end with " Copy <Nr>". If not ..
                //                strGraphObjName = pGraphObjClone->name();
                //                if( !strGraphObjName.endsWith(" Copy 1",Qt::CaseInsensitive) )
                //                {
                //                    strGraphObjName += " Copy 1";
                //                    pGraphObjClone->setName(strGraphObjName);
                //                }
                //                strGraphObjName = findUniqueGraphObjName(pGraphObjClone);
                //                pGraphObjClone->setName(strGraphObjName);

                //                strGraphObjId = pGraphObjClone->keyInTree();
                //                strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                //                pGraphObjClone->setKeyInTree(strGraphObjId);
                //                m_dctpGraphObjsClipboard[strGraphObjId] = pGraphObjClone;

                //                if( pGraphObjClone->getType() == EGraphObjTypeConnectionPoint )
                //                {
                //                    arpGraphObjsOrigin.append(pGraphObj);
                //                    arpGraphObjsClones.append(pGraphObjClone);
                //                }

                //            } // if( pGraphObjClone != nullptr )
                //        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                //    } // if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
                //} // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )

                //for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
                //{
                //    pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
                //    pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                //    if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                //    {
                //        pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

                //        if( pGraphObjCnctLine != nullptr )
                //        {
                //            pGraphObjCnctPt1 = pGraphObjCnctLine->getConnectionPoint(ELinePoint::Start);
                //            pGraphObjCnctPt2 = pGraphObjCnctLine->getConnectionPoint(ELinePoint::End);
                //            pGraphObjCnctPt1Clone = nullptr;
                //            pGraphObjCnctPt2Clone = nullptr;

                //            // Look for the original connection points.
                //            for( idxGraphObjTmp = 0; idxGraphObjTmp < arpGraphObjsOrigin.size(); idxGraphObjTmp++ )
                //            {
                //                if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt1 )
                //                {
                //                    pGraphObjCnctPt1Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                //                }
                //                if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt2 )
                //                {
                //                    pGraphObjCnctPt2Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                //                }
                //                if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //                {
                //                    break;
                //                }
                //            }

                //            // The references to the connection points cannot be copied with the
                //            // connection lines as the connection points have also been cloned.
                //            // Instead the cloned connection line got to be connected to the clones
                //            // of the connection points 
                //            if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //            {
                //                pGraphObjCnctLineClone = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj->clone());

                //                if( pGraphObjCnctLineClone != nullptr )
                //                {
                //                    pGraphObjCnctLineClone->setConnectionPoint(ELinePoint::Start,pGraphObjCnctPt1Clone);
                //                    pGraphObjCnctLineClone->setConnectionPoint(ELinePoint::End,pGraphObjCnctPt2Clone);

                //                    // The name of the clone should end with " Copy <Nr>". If not ..
                //                    strGraphObjName = pGraphObjCnctLineClone->name();
                //                    if( !strGraphObjName.endsWith(" Copy 1",Qt::CaseInsensitive) )
                //                    {
                //                        strGraphObjName += " Copy 1";
                //                        pGraphObjCnctLineClone->setName(strGraphObjName);
                //                    }
                //                    strGraphObjName = findUniqueGraphObjName(pGraphObjCnctLineClone);
                //                    pGraphObjCnctLineClone->setName(strGraphObjName);

                //                    strGraphObjId = pGraphObjCnctLineClone->keyInTree();
                //                    strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                //                    pGraphObjCnctLineClone->setKeyInTree(strGraphObjId);
                //                    m_dctpGraphObjsClipboard[strGraphObjId] = pGraphObjCnctLineClone;
                //                }
                //            } // if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //        } // if( pGraphObjCnctLine != nullptr )
                //    } // if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                //} // for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            } // if( m_iEvKeyModifiers == Qt::ControlModifier )
        } // if( i_pEv->key() == Qt::Key_C )

        // Pressed key to insert the objects from the (internal) clipboard ...
        else if( i_pEv->key() == Qt::Key_V )
        {
            if( m_iEvKeyModifiers == Qt::ControlModifier )
            {
                //CGraphObj*                pGraphObjClone;
                //QGraphicsItem*            pGraphicsItemClone;
                //CGraphObjConnectionLine*  pGraphObjCnctLine;
                //CGraphObjConnectionLine*  pGraphObjCnctLineClone;
                //CGraphObjConnectionPoint* pGraphObjCnctPt1;
                //CGraphObjConnectionPoint* pGraphObjCnctPt2;
                //CGraphObjConnectionPoint* pGraphObjCnctPt1Clone;
                //CGraphObjConnectionPoint* pGraphObjCnctPt2Clone;
                //QString                   strGraphObjName;
                //QString                   strGraphObjId;
                //QList<CGraphObj*>         arpGraphObjsOrigin;
                //QList<CGraphObj*>         arpGraphObjsClones;
                //int                       idxGraphObjTmp;
                //QPointF                   ptPos;

                // Add objects from the internal clipboard to the drawing scene. Not only the references
                // have been copied but the objects have already been newly created (cloned).
                // We leave the clones in the clipboard as the objects may be copied several times.
                // But for this we have to clone the clone.
                // Concerning cloning group objects, connection points and their connection lines the
                // same applies as on cloning the objects to the clipboard (see "Key_C" above).

                //if( m_dctpGraphObjsClipboard.size() > 0 )
                //{
                //    clearSelection();

                //    QMap<QString,CGraphObj*>::iterator itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

                //    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                //    {
                //        pGraphObj = itGraphObjsClipboard.value();

                //        if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )
                //        {
                //            // We leave the clones in the clipboard as the objects may be copied several times.
                //            // But for this we have to clone the clone.
                //            pGraphObjClone = pGraphObj->clone();

                //            if( pGraphObjClone != nullptr )
                //            {
                //                strGraphObjName = pGraphObjClone->name();
                //                strGraphObjName = findUniqueGraphObjName(pGraphObjClone);
                //                pGraphObjClone->setName(strGraphObjName);

                //                strGraphObjId = pGraphObjClone->keyInTree();
                //                strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjs, strGraphObjId );
                //                pGraphObjClone->setKeyInTree(strGraphObjId);

                //                pGraphicsItemClone = dynamic_cast<QGraphicsItem*>(pGraphObjClone);

                //                if( pGraphicsItemClone == nullptr )
                //                {
                //                    throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                //                }

                //                addGraphObj(pGraphObjClone);

                //                onGraphObjCreationFinished(pGraphObjClone);

                //                ptPos = pGraphicsItemClone->pos();

                //                ptPos.setX( ptPos.x() + 10 );
                //                ptPos.setY( ptPos.y() + 10 );

                //                pGraphicsItemClone->setPos(ptPos);
                //                pGraphicsItemClone->setSelected(true);

                //                if( pGraphicsItemClone->childItems().size() > 0 )
                //                {
                //                    // Rekursively add child items to the drawings scene's list of graphics items.
                //                    addChildItems(pGraphicsItemClone);
                //                }

                //                if( pGraphObjClone->getType() == EGraphObjTypeConnectionPoint )
                //                {
                //                    arpGraphObjsOrigin.append(pGraphObj);
                //                    arpGraphObjsClones.append(pGraphObjClone);
                //                }

                //            } // if( pGraphObjClone != nullptr )
                //        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )

                //        ++itGraphObjsClipboard;

                //    } // while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )

                //    itGraphObjsClipboard = m_dctpGraphObjsClipboard.begin();

                //    while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                //    {
                //        pGraphObj = itGraphObjsClipboard.value();

                //        if( pGraphObj != nullptr && pGraphObj->getType() == EGraphObjTypeConnectionLine )
                //        {
                //            pGraphObjCnctLine = dynamic_cast<CGraphObjConnectionLine*>(pGraphObj);

                //            if( pGraphObjCnctLine != nullptr )
                //            {
                //                pGraphObjCnctPt1 = pGraphObjCnctLine->getConnectionPoint(ELinePoint::Start);
                //                pGraphObjCnctPt2 = pGraphObjCnctLine->getConnectionPoint(ELinePoint::End);
                //                pGraphObjCnctPt1Clone = nullptr;
                //                pGraphObjCnctPt2Clone = nullptr;

                //                // Look for the "original" connection points in the clipboard.
                //                for( idxGraphObjTmp = 0; idxGraphObjTmp < arpGraphObjsOrigin.size(); idxGraphObjTmp++ )
                //                {
                //                    if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt1 )
                //                    {
                //                        pGraphObjCnctPt1Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                //                    }
                //                    if( arpGraphObjsOrigin[idxGraphObjTmp] == pGraphObjCnctPt2 )
                //                    {
                //                        pGraphObjCnctPt2Clone = dynamic_cast<CGraphObjConnectionPoint*>(arpGraphObjsClones[idxGraphObjTmp]);
                //                    }
                //                    if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //                    {
                //                        break;
                //                    }
                //                }

                //                // The references to the connection points cannot be copied with the
                //                // connection lines as the connection points have also been cloned.
                //                // Instead the cloned connection line got to be connected to the clones
                //                // of the connection points 
                //                if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //                {
                //                    // We leave the clones in the clipboard as the objects may be copied several times.
                //                    // But for this we have to clone the clone.
                //                    pGraphObjClone = pGraphObj->clone();
                //                    pGraphObjCnctLineClone = dynamic_cast<CGraphObjConnectionLine*>(pGraphObjClone);

                //                    if( pGraphObjCnctLineClone != nullptr )
                //                    {
                //                        pGraphObjCnctLineClone->setConnectionPoint(ELinePoint::Start,pGraphObjCnctPt1Clone);
                //                        pGraphObjCnctLineClone->setConnectionPoint(ELinePoint::End,pGraphObjCnctPt2Clone);

                //                        strGraphObjName = pGraphObjCnctLineClone->name();
                //                        strGraphObjName = findUniqueGraphObjName(pGraphObjCnctLineClone);
                //                        pGraphObjCnctLineClone->setName(strGraphObjName);

                //                        strGraphObjId = pGraphObjCnctLineClone->keyInTree();
                //                        strGraphObjId = FindUniqueGraphObjId( m_dctpGraphObjsClipboard, strGraphObjId );
                //                        pGraphObjCnctLineClone->setKeyInTree(strGraphObjId);

                //                        pGraphicsItemClone = dynamic_cast<QGraphicsItem*>(pGraphObjCnctLineClone);

                //                        if( pGraphicsItemClone == nullptr )
                //                        {
                //                            throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphicsItem == nullptr" );
                //                        }

                //                        addGraphObj(pGraphObjClone);

                //                        onGraphObjCreationFinished(pGraphObjCnctLineClone);

                //                        // Please note that the position for connection lines is defined by the connection points
                //                        // and that connection lines never have children (calling "addChildItems" is not necessary).
                //                        pGraphicsItemClone->setSelected(true);

                //                    } // if( pGraphObjCnctLineClone != nullptr )
                //                } // if( pGraphObjCnctPt1Clone != nullptr && pGraphObjCnctPt2Clone != nullptr )
                //            } // if( pGraphObjClone != nullptr )
                //        } // if( pGraphObj != nullptr && pGraphObj->getType() != EGraphObjTypeConnectionLine )

                //        ++itGraphObjsClipboard;

                //    } // while( itGraphObjsClipboard != m_dctpGraphObjsClipboard.end() )
                //} // if( m_dctpGraphObjsClipboard.size() > 0 )
            } // if( m_iEvKeyModifiers == Qt::ControlModifier )
        } // if( i_pEv->key() == Qt::Key_V )
    } // if( !i_pEv->isAccepted() )

} // keyPressEvent

//------------------------------------------------------------------------------
void CDrawingScene::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "Key:" + qKeyCode2Str(i_pEv->key()) + " (" + i_pEv->text() + ")";
        strMthInArgs += ", Count:" + QString::number(i_pEv->count());
        strMthInArgs += ", IsAutoRepeat:" + QString::number(i_pEv->count());
        strMthInArgs += ", Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Mode:" + m_mode.toString();
        strAddTrcInfo += ", EditTool:" + m_editTool.toString();
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", ObjFactory:" + QString(m_pObjFactory == nullptr ? "nullptr" : m_pObjFactory->path());
        strAddTrcInfo += ", GraphObjCreating:" + QString(m_pGraphObjCreating == nullptr ? "nullptr" : m_pGraphObjCreating->name());
        strAddTrcInfo += ", SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

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
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjPaintEvent != nullptr && m_pTrcAdminObjPaintEvent->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "Rect(x,y,w,h):(" + QString::number(i_rect.x()) + "," + QString::number(i_rect.y());
        strMthInArgs += "," + QString::number(i_rect.width()) + "," + QString::number(i_rect.height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaintEvent,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "drawBackground",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

    QGraphicsScene::drawBackground(i_pPainter,i_rect);

    QRectF rctScene = sceneRect();

    i_pPainter->setPen(Qt::NoPen);
    i_pPainter->setBrush(Qt::white);
    i_pPainter->drawRect(rctScene);

} // drawBackground

//------------------------------------------------------------------------------
void CDrawingScene::drawForeground( QPainter* i_pPainter, const QRectF& i_rect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjPaintEvent != nullptr && m_pTrcAdminObjPaintEvent->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "Rect(x,y,w,h):(" + QString::number(i_rect.x()) + "," + QString::number(i_rect.y());
        strMthInArgs += "," + QString::number(i_rect.width()) + "," + QString::number(i_rect.height()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaintEvent,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "drawForeground",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "SceneRect(x,y,w,h):(" + QString::number(sceneRect().x()) + "," + QString::number(sceneRect().y());
        strAddTrcInfo += "," + QString::number(sceneRect().width()) + "," + QString::number(sceneRect().height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

    i_pPainter->setClipRect(i_rect);

    QGraphicsScene::drawForeground(i_pPainter,i_rect);

} // drawForeground

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingScene::onGraphObjFactoryDestroyed( QObject* i_pObjFactory )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pObjFactory == nullptr ? "nullptr" : i_pObjFactory->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjFactoryDestroyed",
        /* strAddInfo   */ strMthInArgs );

    if( m_pObjFactory == i_pObjFactory )
    {
        m_pObjFactory = nullptr;
    }
} // onGraphObjFactoryDestroyed
