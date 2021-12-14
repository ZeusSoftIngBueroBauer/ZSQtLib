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

#include "ZSDraw/ZSDrawObjFactoryPolygon.h"
#include "ZSDraw/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class CObjFactoryPolygon : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryPolygon::CObjFactoryPolygon(
    bool         i_bAddToToolBoxIdxTree,
    const QIcon& i_toolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName         */ "Draw::Standard Shapes",
        /* strGraphObjNameSpace */ "ZS::Draw",
        /* strGraphObjClassName */ "CGraphObjPolygon",
        /* iGraphObjType        */ EGraphObjTypePolygon,
        /* strGraphObjType      */ ZS::Draw::graphObjType2Str(EGraphObjTypePolygon),
        /* bAddToToolBoxIdxTree */ i_bAddToToolBoxIdxTree,
        /* toolIcon             */ i_toolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryPolygon::~CObjFactoryPolygon()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryPolygon::createGraphObj(
    CDrawingScene*       i_pDrawingScene,
    const QPointF&       i_ptItemPos,
    const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "ItemPos:" + QString::number(i_ptItemPos.x()) + "," + QString::number(i_ptItemPos.y());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF   ptStart = i_ptItemPos;
    QPointF   ptEnd( i_ptItemPos.x()+1, i_ptItemPos.y()+1 );
    QPolygonF plg;

    plg.append(ptStart);
    plg.append(ptEnd);

    CGraphObjPolygon* pGraphObj = new CGraphObjPolygon(i_pDrawingScene,i_drawSettings);

    pGraphObj->setPolygon(plg);

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryPolygon::saveGraphObj(
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
        strAddTrcInfo  = "GraphObj:" + i_pGraphObj->getNameSpace();
        strAddTrcInfo += "::" + i_pGraphObj->getClassName();
        strAddTrcInfo += "::" + i_pGraphObj->getObjName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "saveGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    CGraphObjPolygon* pGraphObj = dynamic_cast<CGraphObjPolygon*>(i_pGraphObj);

    if( pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjPolygon == nullptr" );
    }

    // Draw Attributes
    //----------------

    CDrawSettings drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement("DrawSettings");
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    // Geometry
    //-------------

    QPolygonF plg = pGraphObj->polygon(); // coordinates are in the objects coordinate system (LT = 0.0/0.0)
    QPointF   ptPos = pGraphObj->pos();
    double    fRotAngle_deg = pGraphObj->getRotationAngleInDegree();
    QPointF   pt;
    int       idxPt;

    i_xmlStreamWriter.writeStartElement("Geometry");
    i_xmlStreamWriter.writeTextElement( "Pos", point2Str(ptPos) );
    i_xmlStreamWriter.writeTextElement( "RotAngleDeg", QString::number(fRotAngle_deg) );

    for( idxPt = 0; idxPt < plg.size(); idxPt++ )
    {
        pt = plg[idxPt];
        i_xmlStreamWriter.writeTextElement( "Pt" + QString::number(idxPt), point2Str(pt) );
    }
    i_xmlStreamWriter.writeEndElement();

    // Z-Value
    //---------------

    i_xmlStreamWriter.writeTextElement( "ZValue", QString::number(pGraphObj->getStackingOrderValue()) );

    // Labels
    //----------------

    QHash<QString,SGraphObjLabel*> arpLabels = i_pGraphObj->getLabels();

    if( arpLabels.size() > 0 )
    {
        i_xmlStreamWriter.writeStartElement("Labels");
        errResultInfo = saveGraphObjLabels( arpLabels, i_xmlStreamWriter );
        i_xmlStreamWriter.writeEndElement();
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryPolygon::loadGraphObj(
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

    CGraphObjPolygon* pGraphObj = nullptr;

    if( i_pDrawingScene->findGraphObj(i_strObjId) == nullptr )
    {
        QString                        strElemName;
        QString                        strElemText;
        bool                           bConverted;
        CDrawSettings                  drawSettings(EGraphObjTypePolygon);
        QPolygonF                      plg;
        QPointF                        ptPos;
        bool                           bPosValid = false;
        double                         fRotAngle_deg = 0.0;
        double                         fZValue = 0.0;
        QHash<QString,SGraphObjLabel*> arpLabels;

        while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
        {
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

            } // if( xmlStreamReader.isStartElement() )

            else if( i_xmlStreamReader.isEndElement() )
            {
                if( strElemName == "GraphObj" )
                {
                    break;
                }

            } // if( i_xmlStreamReader.isEndElement() )

        } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

        if( bPosValid && plg.size() > 1 )
        {
            pGraphObj = new CGraphObjPolygon(
                /* pDrawingScene */ i_pDrawingScene,
                /* drawSettings  */ drawSettings,
                /* strObjName    */ i_strObjName,
                /* strObjId      */ i_strObjId );

            pGraphObj->setPolygon(plg);

            QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

            i_pDrawingScene->addItem(pGraphicsItem);

            pGraphObj->setPos(ptPos);
            pGraphObj->setRotationAngleInDegree(fRotAngle_deg);
            pGraphObj->setStackingOrderValue(fZValue);

            // Before calling "onGraphObjCreated" the object must have been added
            // to its parent group. Otherwise the drawing scene is not able to retrieve
            // the unique object id and add the object to the hash.
            if( i_pGraphObjGroup != nullptr )
            {
                i_pGraphObjGroup->addGraphObj(pGraphObj);
            }

            i_pDrawingScene->onGraphObjCreated(pGraphObj);

            pGraphObj->acceptCurrentAsOriginalCoors();

            if( arpLabels.size() > 0 )
            {
                pGraphObj->addLabels(arpLabels);
            }

        } // if( bPosValid && plg.size() > 1 )

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

    return pGraphObj;

} // loadGraphObj

//------------------------------------------------------------------------------
void CObjFactoryPolygon::ResetCtorsDtorsCounters()
//------------------------------------------------------------------------------
{
    CGraphObjPolygon::ResetCtorsDtorsCounters();
}
