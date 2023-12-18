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

#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryImage.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjImage.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CObjFactoryImage : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryImage::CObjFactoryImage( const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ c_strGroupNameStandardShapes,
        /* strClassName    */ ClassName(),
        /* iGraphObjType   */ EGraphObjTypeImage,
        /* strGraphObjType */ ZS::Draw::graphObjType2Str(EGraphObjTypeImage),
        /* toolIcon        */ i_pxmToolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryImage::~CObjFactoryImage()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryImage::createGraphObj(
    CDrawingScene* i_pDrawingScene,
    const CPhysValPoint& i_physValPoint,
    const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point {" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createGraphObj",
        /* strAddInfo   */ strMthInArgs );
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsDetailed)) {
        strMthInArgs = "DrawSettings {" + i_drawSettings.toString() + "}";
        mthTracer.trace(strMthInArgs);
    }

    QString strFilePath = getFilePath();

    CDrawSettings drawSettings = i_drawSettings;
    drawSettings.setGraphObjType(EGraphObjTypeImage);
    CGraphObjImage* pGraphObj = new CGraphObjImage(i_pDrawingScene);
    pGraphObj->setDrawSettings(drawSettings);

    pGraphObj->setImageFilePath(strFilePath);

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryImage::saveGraphObj(
    CGraphObj*        i_pGraphObj,
    QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "GraphObj:" + i_pGraphObj->NameSpace();
        strMthInArgs += "::" + i_pGraphObj->ClassName();
        strMthInArgs += "::" + i_pGraphObj->name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "saveGraphObj",
        /* strAddInfo   */ strMthInArgs );

    SErrResultInfo errResultInfo;

    CGraphObjImage* pGraphObj = dynamic_cast<CGraphObjImage*>(i_pGraphObj);

    if( pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjImage == nullptr" );
    }

    // Draw Attributes
    //----------------

    CDrawSettings drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement(CDrawingScene::c_strXmlElemNameDrawSettings);
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    // Image
    //-------------

    i_xmlStreamWriter.writeTextElement( "Image", pGraphObj->getImageFilePath() );

    // Geometry
    //-------------

    const CDrawingSize& drawingSize = i_pGraphObj->drawingScene()->drawingSize();

    // "pos" returns the position of the item in parent coordinates.
    // If the item has no parent, its position is given in scene coordinates.
    QPointF ptPos         = pGraphObj->pos();
    QSizeF  siz           = pGraphObj->getSize(drawingSize.unit()).toQSizeF();
    double  fRotAngle_deg = pGraphObj->getRotationAngleInDegree();

    i_xmlStreamWriter.writeStartElement(CDrawingScene::c_strXmlElemNameGeometry);
    i_xmlStreamWriter.writeTextElement( "Pos", point2Str(ptPos) );
    i_xmlStreamWriter.writeTextElement( "Size", size2Str(siz) );
    i_xmlStreamWriter.writeTextElement( "RotAngleDeg", QString::number(fRotAngle_deg) );
    i_xmlStreamWriter.writeEndElement();

    // Neither scaling nor rotating a point makes sense.
    // The move offset is given by the position of the point.

    // Z-Value
    //---------------

    i_xmlStreamWriter.writeTextElement( "ZValue", QString::number(pGraphObj->getStackingOrderValue()) );

    // Labels
    //----------------

    //QHash<QString, CGraphObjLabel*> arpLabels = i_pGraphObj->getLabels();

    //if( arpLabels.size() > 0 )
    //{
    //    i_xmlStreamWriter.writeStartElement(CDrawingScene::c_strXmlElemNameTextLabels);
    //    errResultInfo = saveGraphObjLabels( arpLabels, i_xmlStreamWriter );
    //    i_xmlStreamWriter.writeEndElement();
    //}

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryImage::loadGraphObj(
    CDrawingScene*    i_pDrawingScene,
    CGraphObjGroup*   i_pGraphObjGroup,
    const QString&    i_strObjName,
    QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    if( i_pDrawingScene == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pDrawingScene == nullptr" );
    }

    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "loadGraphObj",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjImage* pGraphObj = nullptr;

#if 0
    QString                         strElemName;
    QString                         strElemText;
    bool                            bConverted;
    CDrawSettings                   drawSettings(EGraphObjTypeImage);
    QString                         strImgFilePath;
    QPointF                         ptPos;
    QSizeF                          siz;
    bool                            bPosValid = false;
    bool                            bSizeValid = false;
    double                          fRotAngle_deg = 0.0;
    double                          fZValue = 0.0;
    QHash<QString, CGraphObjLabel*> arpLabels;

    while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
    {
        //xmlStreamTokenType = i_xmlStreamReader.readNext();
        strElemName = i_xmlStreamReader.name().toString();

        if( i_xmlStreamReader.isStartElement() )
        {
            if( strElemName == "Image" )
            {
                strImgFilePath = i_xmlStreamReader.readElementText();
            }

            else if( strElemName == CDrawingScene::c_strXmlElemNameDrawSettings )
            {
                drawSettings.load(i_xmlStreamReader);
            }

            else if( strElemName == CDrawingScene::c_strXmlElemNameGeometry )
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

            else if( strElemName == "Size" )
            {
                strElemText = i_xmlStreamReader.readElementText();

                QSizeF sizTmp = str2SizeF(strElemText,&bConverted);

                if( bConverted )
                {
                    siz = sizTmp;
                    bSizeValid = true;
                }

            } // if( strElemName == "Size" )

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

            else if( strElemName == CDrawingScene::c_strXmlElemNameTextLabels )
            {
                arpLabels = loadGraphObjLabels(i_xmlStreamReader);

            } // if( strElemName == CDrawingScene::c_strXmlElemNameTextLabels )

        } // if( xmlStreamReader.isStartElement() )

        else if( i_xmlStreamReader.isEndElement() )
        {
            if( strElemName == "GraphObj" )
            {
                break;
            }

        } // if( i_xmlStreamReader.isEndElement() )

    } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

    if( !strImgFilePath.isEmpty() && bPosValid && bSizeValid )
    {
        pGraphObj = new CGraphObjImage(
            /* pDrawingScene */ i_pDrawingScene,
            /* drawSettings  */ drawSettings,
            /* strObjName    */ i_strObjName );

        pGraphObj->setImageFilePath(strImgFilePath);
        pGraphObj->setSize(siz);

        i_pDrawingScene->addGraphObj(pGraphObj);

        pGraphObj->setPos(ptPos);
        pGraphObj->setRotationAngleInDegree(fRotAngle_deg);
        pGraphObj->setStackingOrderValue(fZValue);

        // Before calling "onGraphObjCreationFinished" the object must have been added
        // to its parent group. Otherwise the drawing scene is not able to retrieve
        // the unique object id and add the object to the hash.
        if( i_pGraphObjGroup != nullptr )
        {
            throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
            //i_pGraphObjGroup->addGraphObj(pGraphObj);
        }

        i_pDrawingScene->onGraphObjCreationFinished(pGraphObj);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        pGraphObj->acceptCurrentAsOriginalCoors();
#endif

        //if( arpLabels.size() > 0 )
        //{
        //    pGraphObj->addLabels(arpLabels);
        //}
    } // if( bPosValid && bSizeValid )

    if( arpLabels.size() > 0 )
    {
        QHashIterator<QString, CGraphObjLabel*> itLabels(arpLabels);
        CGraphObjLabel* pGraphObjLabel;

        while( itLabels.hasNext() )
        {
            itLabels.next();

            pGraphObjLabel = itLabels.value();

            arpLabels.remove(pGraphObjLabel->getKey());

            delete pGraphObjLabel;
            pGraphObjLabel = nullptr;
        }
    }
#endif

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(i_xmlStreamReader.errorString());
        QString strMthRet = QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
        mthTracer.setMethodReturn(strMthRet);
    }
    return pGraphObj;

} // loadGraphObj
