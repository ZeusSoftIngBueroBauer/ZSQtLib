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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/ZSDrawObjFactoryGroup.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class CObjFactoryGroup : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryGroup::CObjFactoryGroup( const QIcon& i_toolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ c_strGroupNameStandardShapes,
        /* iGraphObjType   */ EGraphObjTypeGroup,
        /* strGraphObjType */ ZS::Draw::graphObjType2Str(EGraphObjTypeGroup),
        /* toolIcon        */ i_toolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryGroup::~CObjFactoryGroup()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryGroup::createGraphObj(
    CDrawingScene*       i_pDrawingScene,
    const QPointF&       /*i_ptItemPos*/,
    const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjGroup* pGraphObj = new CGraphObjGroup(i_pDrawingScene,i_drawSettings);

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryGroup::saveGraphObj(
    CGraphObj*        i_pGraphObj,
    QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObj:" + i_pGraphObj->nameSpace();
        strAddTrcInfo += "::" + i_pGraphObj->className();
        strAddTrcInfo += "::" + i_pGraphObj->name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "saveGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(i_pGraphObj);

    if( pGraphObjGroup == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjGroup == nullptr" );
    }

    // Draw Attributes
    //----------------

    CDrawSettings drawSettings = pGraphObjGroup->getDrawSettings();
    i_xmlStreamWriter.writeStartElement("DrawSettings");
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    // Geometry
    //-------------

    QPointF ptPos         = pGraphObjGroup->pos();
    double  fRotAngle_deg = pGraphObjGroup->getRotationAngleInDegree();

    i_xmlStreamWriter.writeStartElement("Geometry");
    i_xmlStreamWriter.writeTextElement( "Pos", point2Str(ptPos) );
    i_xmlStreamWriter.writeTextElement( "RotAngleDeg", QString::number(fRotAngle_deg) );
    i_xmlStreamWriter.writeEndElement();

    // Z-Value
    //---------------

    i_xmlStreamWriter.writeTextElement( "ZValue", QString::number(pGraphObjGroup->getStackingOrderValue()) );

    // Labels
    //----------------

    QHash<QString,SGraphObjLabel*> arpLabels = pGraphObjGroup->getLabels();

    if( arpLabels.size() > 0 )
    {
        i_xmlStreamWriter.writeStartElement("Labels");
        errResultInfo = saveGraphObjLabels( arpLabels, i_xmlStreamWriter );
        i_xmlStreamWriter.writeEndElement();
    }

    // Group members (childrens)
    //--------------------------

    QGraphicsItem* pGraphicsItemChild;
    CGraphObj*     pGraphObjChild;
    int            idxGraphObjChild;
    QString        strNameSpaceChild;
    QString        strClassNameChild;
    QString        strObjTypeChild;
    QString        strObjNameChild;
    QString        strObjIdChild;
    CObjFactory*   pObjFactoryChild;

    // Connection points need to be recalled before the connection lines as on
    // creating the connection lines their connection points must already exist.
    // For this the connection lines will be saved at the end of the XML file.
    for( idxGraphObjChild = 0; idxGraphObjChild < pGraphObjGroup->childItems().size(); idxGraphObjChild++ )
    {
        pGraphicsItemChild = pGraphObjGroup->childItems()[idxGraphObjChild];

        if( pGraphicsItemChild->type() != EGraphObjTypeSelectionPoint
         && pGraphicsItemChild->type() != EGraphObjTypeLabel
         && pGraphicsItemChild->type() != EGraphObjTypeConnectionLine )
        {
            pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);

            strNameSpaceChild = pGraphObjChild->nameSpace();
            strClassNameChild = pGraphObjChild->className();
            strObjTypeChild   = pGraphObjChild->getTypeAsString();
            strObjNameChild   = pGraphObjChild->name();
            strObjIdChild     = pGraphObjChild->keyInTree();

            pObjFactoryChild = CObjFactory::FindObjFactory(pGraphObjChild->getFactoryGroupName(), strObjTypeChild);

            if( pObjFactoryChild != nullptr )
            {
                i_xmlStreamWriter.writeStartElement("GraphObj");

                i_xmlStreamWriter.writeAttribute( "NameSpace", strNameSpaceChild );
                i_xmlStreamWriter.writeAttribute( "ClassName", strClassNameChild );
                i_xmlStreamWriter.writeAttribute( "ObjectType", strObjTypeChild );
                i_xmlStreamWriter.writeAttribute( "ObjectName", strObjNameChild );
                i_xmlStreamWriter.writeAttribute( "ObjectId", strObjIdChild );

                errResultInfo = pObjFactoryChild->saveGraphObj(pGraphObjChild,i_xmlStreamWriter);

                i_xmlStreamWriter.writeEndElement();

                if( errResultInfo.isErrorResult() )
                {
                    break;
                }
            }
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        for( idxGraphObjChild = 0; idxGraphObjChild < pGraphObjGroup->childItems().size(); idxGraphObjChild++ )
        {
            pGraphicsItemChild = pGraphObjGroup->childItems()[idxGraphObjChild];

            if( pGraphicsItemChild->type() != EGraphObjTypeSelectionPoint
             && pGraphicsItemChild->type() != EGraphObjTypeLabel
             && pGraphicsItemChild->type() == EGraphObjTypeConnectionLine )
            {
                pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);

                strNameSpaceChild = pGraphObjChild->nameSpace();
                strClassNameChild = pGraphObjChild->className();
                strObjTypeChild   = pGraphObjChild->getTypeAsString();
                strObjNameChild   = pGraphObjChild->name();
                strObjIdChild     = pGraphObjChild->keyInTree();

                pObjFactoryChild = CObjFactory::FindObjFactory(pGraphObjChild->getFactoryGroupName(), strObjTypeChild);

                if( pObjFactoryChild != nullptr )
                {
                    i_xmlStreamWriter.writeStartElement("GraphObj");

                    i_xmlStreamWriter.writeAttribute( "NameSpace", strNameSpaceChild );
                    i_xmlStreamWriter.writeAttribute( "ClassName", strClassNameChild );
                    i_xmlStreamWriter.writeAttribute( "ObjectType", strObjTypeChild );
                    i_xmlStreamWriter.writeAttribute( "ObjectName", strObjNameChild );
                    i_xmlStreamWriter.writeAttribute( "ObjectId", strObjIdChild );

                    errResultInfo = pObjFactoryChild->saveGraphObj(pGraphObjChild,i_xmlStreamWriter);

                    i_xmlStreamWriter.writeEndElement();

                    if( errResultInfo.isErrorResult() )
                    {
                        break;
                    }
                }
            }
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryGroup::loadGraphObj(
    CDrawingScene*    i_pDrawingScene,
    CGraphObjGroup*   i_pGraphObjGroup,
    const QString&    i_strObjName,
    const QString&    i_strObjId,
    QXmlStreamReader& i_xmlStreamReader,
    SErrResultInfo&   io_errResultInfo )
//------------------------------------------------------------------------------
{
    if( i_pDrawingScene == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pDrawingScene == nullptr" );
    }

    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "loadGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjGroup* pGraphObjGroup = nullptr;

    if( i_pDrawingScene->findGraphObj(i_strObjId) == nullptr )
    {
        QXmlStreamAttributes           xmlStreamAttrs;
        QString                        strElemName;
        QString                        strElemText;
        bool                           bConverted;
        CDrawSettings                  drawSettings(EGraphObjTypeGroup);
        QPointF                        ptPos;
        bool                           bPosValid = false;
        double                         fRotAngle_deg = 0.0;
        double                         fZValue = 0.0;
        QHash<QString,SGraphObjLabel*> arpLabels;

        pGraphObjGroup = new CGraphObjGroup(
            /* pDrawingScene */ i_pDrawingScene,
            /* drawSettings  */ drawSettings,
            /* strObjName    */ i_strObjName );

        // Start creation of group.
        pGraphObjGroup->setEditMode(EEditMode::Creating);

        while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
        {
            //xmlStreamTokenType = i_xmlStreamReader.readNext();
            strElemName = i_xmlStreamReader.name().toString();

            if( i_xmlStreamReader.isStartElement() )
            {
                if( strElemName == "DrawSettings" )
                {
                    drawSettings.load(i_xmlStreamReader);
                }

                else if( strElemName == "Geometry" )
                {
                }

                else if( strElemName == "Pos" )
                {
                    strElemText = i_xmlStreamReader.readElementText();

                    QPointF ptTmp = str2PointF(strElemText,&bConverted);

                    if( bConverted )
                    {
                        ptPos = ptTmp;
                        bPosValid = true;
                    }

                } // if( strElemName == "Pos" )

                else if( strElemName == "RotAngleDeg" )
                {
                    strElemText = i_xmlStreamReader.readElementText();

                    double fValTmp = strElemText.toDouble(&bConverted);

                    if( bConverted )
                    {
                        fRotAngle_deg = fValTmp;
                    }

                } // if( strElemName == "RotAngleDeg" )

                else if( strElemName == "ZValue" )
                {
                    strElemText = i_xmlStreamReader.readElementText();

                    double fTmp = strElemText.toDouble(&bConverted);

                    if( bConverted )
                    {
                        fZValue = fTmp;
                    }

                } // if( strElemName == "ZValue" )

                else if( strElemName == "Labels" )
                {
                    SErrResultInfo errResultInfo;
                    arpLabels = loadGraphObjLabels(i_xmlStreamReader,errResultInfo);

                } // if( strElemName == "Labels" )

                //--------------------------------
                else if( strElemName == "GraphObj" )
                //--------------------------------
                {
                    QString strFactoryGroupNameChild;
                    QString strNameSpaceChild;
                    QString strClassNameChild;
                    QString strObjTypeChild;
                    QString strObjNameChild;
                    QString strObjIdChild;

                    xmlStreamAttrs = i_xmlStreamReader.attributes();

                    if( xmlStreamAttrs.hasAttribute("FactoryGroupName") )
                    {
                        strFactoryGroupNameChild = xmlStreamAttrs.value("FactoryGroupName").toString();;
                    }
                    if( xmlStreamAttrs.hasAttribute("NameSpace") )
                    {
                        strNameSpaceChild = xmlStreamAttrs.value("NameSpace").toString();;
                    }
                    if( xmlStreamAttrs.hasAttribute("ClassName") )
                    {
                        strClassNameChild = xmlStreamAttrs.value("ClassName").toString();;
                    }
                    if( xmlStreamAttrs.hasAttribute("ObjectType") )
                    {
                        strObjTypeChild = xmlStreamAttrs.value("ObjectType").toString();
                    }
                    if( xmlStreamAttrs.hasAttribute("ObjectName") )
                    {
                        strObjNameChild = xmlStreamAttrs.value("ObjectName").toString();;
                    }
                    if( xmlStreamAttrs.hasAttribute("ObjectId") )
                    {
                        strObjIdChild = xmlStreamAttrs.value("ObjectId").toString();;
                    }

                    CObjFactory* pObjFactoryChild = CObjFactory::FindObjFactory(strFactoryGroupNameChild, strObjTypeChild);

                    if( pObjFactoryChild != nullptr )
                    {
                        pObjFactoryChild->loadGraphObj(
                            /* pDrawingScene   */ i_pDrawingScene,
                            /* pGraphObjGroup  */ pGraphObjGroup,
                            /* strObjName      */ strObjNameChild,
                            /* strObjId        */ strObjIdChild,
                            /* xmlStreamReader */ i_xmlStreamReader,
                            /* errResultInfo   */ io_errResultInfo );
                    }

                } // if( strElemName == "GraphObj" )

            } // if( xmlStreamReader.isStartElement() )

            else if( i_xmlStreamReader.isEndElement() )
            {
                if( strElemName == "GraphObj" )
                {
                    break;
                }

            } // if( i_xmlStreamReader.isEndElement() )

        } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

        if( bPosValid )
        {
            QList<QGraphicsItem*> arpGraphicsItems = pGraphObjGroup->childItems();

            QGraphicsItem* pGraphicsItem;
            CGraphObj*     pGraphObj;
            int            idxGraphObj;
            QPointF        posItem;
            QSizeF         sizItem;
            QRectF         rctItem;
            QRectF         rctGroup;
            double         fXLeftMin   = INT_MAX;
            double         fYTopMin    = INT_MAX;
            double         fXRightMax  = INT_MIN;
            double         fYBottomMax = INT_MIN;

            // Calculate resulting size of group (without selection rectangle and selection points).
            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItems[idxGraphObj];
                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                if( pGraphicsItem->type() != EGraphObjTypeConnectionLine )
                {
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

            // TopLeft should be at (0.0/0.0). Otherwise the child object
            // coordinates where not correctly stored in the XML file.
            rctGroup.setLeft(fXLeftMin);
            rctGroup.setTop(fYTopMin);
            rctGroup.setRight(fXRightMax);
            rctGroup.setBottom(fYBottomMax);

            pGraphObjGroup->setDrawSettings(drawSettings);

            pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObjGroup);

            i_pDrawingScene->addItem(pGraphicsItem);

            pGraphObjGroup->setPos(ptPos);
            pGraphObjGroup->setSize(rctGroup.size());
            pGraphObjGroup->setRotationAngleInDegree(fRotAngle_deg);
            pGraphObjGroup->setStackingOrderValue(fZValue);

            // Before calling "onGraphObjCreated" the object must have been added
            // to its parent group. Otherwise the drawing scene is not able to retrieve
            // the unique object id and add the object to the hash.
            if( i_pGraphObjGroup != nullptr )
            {
                i_pGraphObjGroup->addGraphObj(pGraphObjGroup);
            }

            i_pDrawingScene->onGraphObjCreated(pGraphObjGroup);

            pGraphObjGroup->acceptCurrentAsOriginalCoors();
            pGraphObjGroup->setEditMode(EEditMode::Undefined);
            pGraphObjGroup->setEditResizeMode(EEditResizeMode::Undefined);

            if( arpLabels.size() > 0 )
            {
                pGraphObjGroup->addLabels(arpLabels);
            }

        } // if( bPosValid )

        else
        {
            delete pGraphObjGroup;
            pGraphObjGroup = nullptr;
        }

        if( arpLabels.size() > 0 )
        {
            QHashIterator<QString,SGraphObjLabel*> itLabels(arpLabels);
            SGraphObjLabel*                        pGraphObjLabel;

            while( itLabels.hasNext() )
            {
                itLabels.next();

                pGraphObjLabel = itLabels.value();

                arpLabels.remove(pGraphObjLabel->m_strKey);

                delete pGraphObjLabel;
                pGraphObjLabel = nullptr;
            }
        }

    } // if( i_pDrawingScene->findGraphObj(i_strObjId) == nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(io_errResultInfo);
    }

    return pGraphObjGroup;

} // loadGraphObj