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

#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryConnectionLine.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
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
class CObjFactoryConnectionLine : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryConnectionLine::CObjFactoryConnectionLine( const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ c_strGroupNameConnections,
        /* strClassName    */ ClassName(),
        /* iGraphObjType   */ EGraphObjTypeConnectionLine,
        /* strGraphObjType */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionLine),
        /* toolIcon        */ i_pxmToolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryConnectionLine::~CObjFactoryConnectionLine()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryConnectionLine::createGraphObj(
    CDrawingScene* i_pDrawingScene,
    const CPhysValPoint& i_physValPoint,
    const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point {" + i_physValPoint.toString() + "}" +
                        ", DrawSettings {" + i_drawSettings.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createGraphObj",
        /* strAddInfo   */ strMthInArgs );

    CDrawSettings drawSettings = i_drawSettings;
    drawSettings.setGraphObjType(EGraphObjTypeConnectionLine);
    CGraphObjConnectionLine* pGraphObj = new CGraphObjConnectionLine(i_pDrawingScene);
    pGraphObj->setDrawSettings(drawSettings);

#if 0
    QPolygonF plg;
    plg.append(i_ptItemPos);
    plg.append(i_ptItemPos);
    pGraphObj->setPolygon(plg);
#endif

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryConnectionLine::saveGraphObj(
    CGraphObj*        i_pGraphObj,
    QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
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

    CGraphObjConnectionLine* pGraphObj = dynamic_cast<CGraphObjConnectionLine*>(i_pGraphObj);

    if( pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjCnctLine == nullptr" );
    }

    CGraphObjConnectionPoint* pCnctPtStart = pGraphObj->getConnectionPoint(ELinePoint::Start);

    if( pCnctPtStart == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultConnectionError, "pCnctPtStart == nullptr" );
    }

    CGraphObjConnectionPoint* pCnctPtEnd = pGraphObj->getConnectionPoint(ELinePoint::End);

    if( pCnctPtEnd == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultConnectionError, "pCnctPtEnd == nullptr" );
    }

    // Connection points
    //------------------

    // Connection lines don't belong to groups. But their connection points may.
    // To find the connection point it's not sufficient just to store the name in
    // the XML file but it's necessary to store the id of the connection point.
    i_xmlStreamWriter.writeTextElement( "ObjIdCnctPtStart", pCnctPtStart->keyInTree() );
    i_xmlStreamWriter.writeTextElement( "ObjIdCnctPtEnd", pCnctPtEnd->keyInTree() );

    // Draw Attributes
    //----------------

    CDrawSettings drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameDrawSettings);
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    // Geometry
    //-------------

    QPolygonF plg = pGraphObj->polygon(); // coordinates are in the objects coordinate system (LT = 0.0/0.0)
    QPointF   pt;
    int       idxPt;

    // Shape points of connection lines will be stored in scene coordinates.
    plg = pGraphObj->mapToScene(plg);

    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGeometry);

    for( idxPt = 0; idxPt < plg.size(); idxPt++ )
    {
        pt = plg[idxPt];
        i_xmlStreamWriter.writeTextElement( "Pt" + QString::number(idxPt), point2Str(pt) );
    }
    i_xmlStreamWriter.writeEndElement();

    // Neither scaling nor rotating a connection line makes sense.
    // The position of connection lines is defined by the connection
    // points they are linked to.

    // Z-Value
    //---------------

    i_xmlStreamWriter.writeTextElement( "ZValue", QString::number(pGraphObj->getStackingOrderValue()) );

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryConnectionLine::loadGraphObj(
    CDrawingScene*    i_pDrawingScene,
    CGraphObjGroup*   /*i_pGraphObjGroup*/, // Connection lines never belong to groups.
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

    CGraphObjConnectionLine*  pGraphObj = nullptr;

    CGraphObjConnectionPoint* pCnctPtStart = nullptr;
    CGraphObjConnectionPoint* pCnctPtEnd   = nullptr;
    QXmlStreamAttributes      xmlStreamAttrs;
    QString                   strElemName;
    QString                   strElemText;
    QString                   strAttr;
    bool                      bConverted;
    CDrawSettings             drawSettings(EGraphObjTypeConnectionLine);
    QPolygonF                 plg;
    double                    fZValue = 0.0;

    QList<SLabelDscr> arTextLabels;

    while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
    {
        //xmlStreamTokenType = i_xmlStreamReader.readNext();
        strElemName = i_xmlStreamReader.name().toString();

        if( i_xmlStreamReader.isStartElement() )
        {
            if( strElemName == "ObjIdCnctPtStart" )
            {
                QString strObjId = i_xmlStreamReader.readElementText();
                CGraphObj* pGraphObjTmp = i_pDrawingScene->findGraphObj(strObjId);
                pCnctPtStart = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObjTmp);
            }
            else if( strElemName == "ObjIdCnctPtEnd" )
            {
                QString strObjId = i_xmlStreamReader.readElementText();
                CGraphObj* pGraphObjTmp = i_pDrawingScene->findGraphObj(strObjId);
                pCnctPtEnd = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObjTmp);
            }
            else if( strElemName == XmlStreamParser::c_strXmlElemNameDrawSettings )
            {
                drawSettings.load(i_xmlStreamReader);
            }
            else if( strElemName == XmlStreamParser::c_strXmlElemNameGeometry )
            {
            }
            else if( strElemName.contains("Pt",Qt::CaseInsensitive) )
            {
                QString strIdxPtTmp = strElemName.right(strElemName.size()-2);

                int idxPtTmp = strIdxPtTmp.toInt(&bConverted);

                if( bConverted && idxPtTmp < 10000 )
                {
                    strElemText = i_xmlStreamReader.readElementText();

                    QPointF ptTmp = str2PointF(strElemText,&bConverted);

                    if( bConverted )
                    {
                        if( plg.size() < idxPtTmp+1 )
                        {
                            plg.resize(idxPtTmp+1);
                        }
                        plg[idxPtTmp] = ptTmp;
                    }
                }
            } // if( strElemName == "Pt" )

            else if( strElemName == "ZValue" )
            {
                strElemText = i_xmlStreamReader.readElementText();

                double fTmp = strElemText.toDouble(&bConverted);

                if( bConverted )
                {
                    fZValue = fTmp;
                }
            } // if( strElemName == "ZValue" )

            else if( strElemName == XmlStreamParser::c_strXmlElemNameTextLabels )
            {
                arTextLabels = loadGraphObjTextLabels(i_xmlStreamReader);
            }
        } // if( xmlStreamReader.isStartElement() )

        else if( i_xmlStreamReader.isEndElement() )
        {
            if( strElemName == "GraphObj" )
            {
                break;
            }
        } // if( i_xmlStreamReader.isEndElement() )
    } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

    if( pCnctPtStart != nullptr && pCnctPtEnd != nullptr && plg.size() > 1 )
    {
        pGraphObj = new CGraphObjConnectionLine(i_pDrawingScene, i_strObjName);
        pGraphObj->setDrawSettings(drawSettings);

        i_pDrawingScene->addGraphObj(pGraphObj);

        pGraphObj->setConnectionPoint(ELinePoint::Start, pCnctPtStart);
        pGraphObj->setConnectionPoint(ELinePoint::End, pCnctPtEnd);

        plg = pGraphObj->mapFromScene(plg);

        pGraphObj->setPolygon(plg);

        pGraphObj->setStackingOrderValue(fZValue);

        //i_pDrawingScene->onGraphObjCreationFinished(pGraphObj);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        pGraphObj->acceptCurrentAsOriginalCoors();
#endif

    } // if( pCnctPtStart != nullptr && pCnctPtEnd != nullptr && plg.size() > 1 )

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(i_xmlStreamReader.errorString());
        QString strMthRet = QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
        mthTracer.setMethodReturn(strMthRet);
    }
    return pGraphObj;

} // loadGraphObj
