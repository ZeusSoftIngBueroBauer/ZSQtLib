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

#include "ZSDraw/ZSDrawObjFactoryLine.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjLine.h"
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
class CObjFactoryLine : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryLine::CObjFactoryLine(
    bool         i_bAddToToolBoxIdxTree,
    const QIcon& i_toolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName         */ "Draw::Standard Shapes",
        /* strGraphObjNameSpace */ CGraphObjLine::NameSpace(),
        /* strGraphObjClassName */ CGraphObjLine::ClassName(),
        /* iGraphObjType        */ EGraphObjTypeLine,
        /* strGraphObjType      */ ZS::Draw::graphObjType2Str(EGraphObjTypeLine),
        /* bAddToToolBoxIdxTree */ i_bAddToToolBoxIdxTree,
        /* toolIcon             */ i_toolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryLine::~CObjFactoryLine()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryLine::createGraphObj(
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

    // We need to create an object with at least one pixel size so that the
    // drawing scene can "find" the object and can dispatch events to it.
    // It is not important whether width or height is set to one pixel.
    // For a line it makes sense to set both extents to one pixel.
    QPointF ptStart = i_ptItemPos;
    QPointF ptEnd( i_ptItemPos.x()+1, i_ptItemPos.y()+1 );

    CGraphObjLine* pGraphObj = new CGraphObjLine(i_pDrawingScene,i_drawSettings);

    pGraphObj->setLine( QLineF(ptStart,ptEnd) );

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryLine::saveGraphObj(
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

    CGraphObjLine* pGraphObj = dynamic_cast<CGraphObjLine*>(i_pGraphObj);

    if( pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjLine == nullptr" );
    }

    // Draw Attributes
    //----------------

    CDrawSettings drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement("DrawSettings");
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    // Geometry
    //-------------

    QLineF  lin = pGraphObj->line(); // coordinates are in the objects coordinate system (LT = 0.0/0.0)
    QPointF pt1 = pGraphObj->mapToParent(lin.p1());
    QPointF pt2 = pGraphObj->mapToParent(lin.p2());

    i_xmlStreamWriter.writeStartElement("Geometry");
    i_xmlStreamWriter.writeStartElement("ShapePoints");
    i_xmlStreamWriter.writeTextElement( "P1", point2Str(pt1) );
    i_xmlStreamWriter.writeTextElement( "P2", point2Str(pt2) );
    i_xmlStreamWriter.writeEndElement();
    i_xmlStreamWriter.writeEndElement();

    // It makes no sense to rotate or scale a line.

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
CGraphObj* CObjFactoryLine::loadGraphObj(
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

    CGraphObjLine* pGraphObj = nullptr;

    if( i_pDrawingScene->findGraphObj(i_strObjId) == nullptr )
    {
        QString                        strElemName;
        QString                        strElemText;
        bool                           bConverted;
        CDrawSettings                  drawSettings(EGraphObjTypeLine);
        QPointF                        pt1;
        QPointF                        pt2;
        bool                           bPt1Valid = false;
        bool                           bPt2Valid = false;
        double                         fZValue = 0.0;
        QHash<QString,SGraphObjLabel*> arpLabels;

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

                else if( strElemName == "ShapePoints" )
                {
                }

                else if( strElemName == "P1" )
                {
                    strElemText = i_xmlStreamReader.readElementText();

                    QPointF ptTmp = str2PointF(strElemText,&bConverted);

                    if( bConverted )
                    {
                        pt1 = ptTmp;
                        bPt1Valid = true;
                    }

                } // if( strElemName == "P1" )

                else if( strElemName == "P2" )
                {
                    strElemText = i_xmlStreamReader.readElementText();

                    QPointF ptTmp = str2PointF(strElemText,&bConverted);

                    if( bConverted )
                    {
                        pt2 = ptTmp;
                        bPt2Valid = true;
                    }

                } // if( strElemName == "P2" )

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

        if( bPt1Valid && bPt2Valid )
        {
            QLineF lin(pt1,pt2);

            pGraphObj = new CGraphObjLine(
                /* pDrawingScene */ i_pDrawingScene,
                /* drawSettings  */ drawSettings,
                /* strObjName    */ i_strObjName,
                /* strObjId      */ i_strObjId );

            lin.translate( -pt1.x(), -pt1.y() );

            pGraphObj->setLine(lin);

            QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

            i_pDrawingScene->addItem(pGraphicsItem);

            // Before calling "onGraphObjCreated" the object must have been added
            // to its parent group. Otherwise the drawing scene is not able to retrieve
            // the unique object id and add the object to the hash.
            if( i_pGraphObjGroup != nullptr )
            {
                i_pGraphObjGroup->addGraphObj(pGraphObj);
            }

            pGraphObj->setPos(pt1);
            pGraphObj->setStackingOrderValue(fZValue);

            i_pDrawingScene->onGraphObjCreated(pGraphObj);

            pGraphObj->acceptCurrentAsOriginalCoors();

            if( arpLabels.size() > 0 )
            {
                pGraphObj->addLabels(arpLabels);
            }

        } // if( bPt1Valid && bPt2Valid )

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
void CObjFactoryLine::ResetCtorsDtorsCounters()
//------------------------------------------------------------------------------
{
    CGraphObjLine::ResetCtorsDtorsCounters();
}
