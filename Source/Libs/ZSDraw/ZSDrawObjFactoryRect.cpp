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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/ZSDrawObjFactoryRect.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CObjFactoryRect : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryRect::CObjFactoryRect( const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ c_strGroupNameStandardShapes,
        /* iGraphObjType   */ EGraphObjTypeRect,
        /* strGraphObjType */ ZS::Draw::graphObjType2Str(EGraphObjTypeRect),
        /* toolIcon        */ i_pxmToolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryRect::~CObjFactoryRect()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryRect::createGraphObj(
    CDrawingScene*       i_pDrawingScene,
    const QPointF&       i_ptItemPos,
    const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "ItemPos:" + QString::number(i_ptItemPos.x()) + "," + QString::number(i_ptItemPos.y());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    // We need to create an object with at least one pixel size so that the
    // drawing scene can "find" the object and can dispatch events to it.
    // It is not important whether width or height is set to one pixel.
    // For a rectangle it makes sense to set both extents to one pixel.
    QPointF ptStart = i_ptItemPos;
    QPointF ptEnd( i_ptItemPos.x()+1, i_ptItemPos.y()+1 );

    CGraphObjRect* pGraphObj = new CGraphObjRect(i_pDrawingScene,i_drawSettings);

    pGraphObj->setRect( QRectF(ptStart,ptEnd) );

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryRect::saveGraphObj(
    CGraphObj*        i_pGraphObj,
    QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "GraphObj:" + i_pGraphObj->nameSpace();
        strAddTrcInfo += "::" + i_pGraphObj->className();
        strAddTrcInfo += "::" + i_pGraphObj->name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "saveGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    CGraphObjRect* pGraphObj = dynamic_cast<CGraphObjRect*>(i_pGraphObj);

    if( pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjRect == nullptr" );
    }

    // Draw Attributes
    //----------------

    CDrawSettings drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement("DrawSettings");
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    // Geometry
    //-------------

    QPointF ptPos         = pGraphObj->pos();
    QRectF  rct           = pGraphObj->rect(); // coordinates are in the objects coordinate system (LT = 0.0/0.0)
    double  fRotAngle_deg = pGraphObj->getRotationAngleInDegree();

    i_xmlStreamWriter.writeStartElement("Geometry");
    i_xmlStreamWriter.writeTextElement( "Pos", point2Str(ptPos) );
    i_xmlStreamWriter.writeTextElement( "Size", size2Str(rct.size()) );
    i_xmlStreamWriter.writeTextElement( "RotAngleDeg", QString::number(fRotAngle_deg) );
    i_xmlStreamWriter.writeEndElement();

    // Z-Value
    //---------------

    i_xmlStreamWriter.writeTextElement( "ZValue", QString::number(pGraphObj->getStackingOrderValue()) );

    // Labels
    //----------------

    //QHash<QString, CGraphObjLabel*> arpLabels = i_pGraphObj->getLabels();

    //if( arpLabels.size() > 0 )
    //{
    //    i_xmlStreamWriter.writeStartElement("Labels");
    //    errResultInfo = saveGraphObjLabels( arpLabels, i_xmlStreamWriter );
    //    i_xmlStreamWriter.writeEndElement();
    //}

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryRect::loadGraphObj(
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
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "loadGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjRect* pGraphObj = nullptr;

    if( i_pDrawingScene->findGraphObj(i_strObjId) == nullptr )
    {
        QXmlStreamReader::TokenType xmlStreamTokenType;

        QString       strElemName;
        QString       strElemText;
        bool          bConverted;
        CDrawSettings drawSettings(EGraphObjTypeRect);
        QPointF       ptPos;
        QSizeF        siz;
        bool          bPosValid = false;
        bool          bSizeValid = false;
        double        fRotAngle_deg = 0.0;
        double        fZValue = 0.0;

        QHash<QString, CGraphObjLabel*> arpLabels;

        while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
        {
            xmlStreamTokenType = i_xmlStreamReader.readNext();

            if( i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement() )
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
                    }
                    else if( strElemName == "Size" )
                    {
                        strElemText = i_xmlStreamReader.readElementText();

                        QSizeF sizTmp = str2SizeF(strElemText,&bConverted);

                        if( bConverted )
                        {
                            siz = sizTmp;
                            bSizeValid = true;
                        }
                    }
                    else if( strElemName == "RotAngleDeg" )
                    {
                        strElemText = i_xmlStreamReader.readElementText();

                        double fValTmp = strElemText.toDouble(&bConverted);

                        if( bConverted )
                        {
                            fRotAngle_deg = fValTmp;
                        }
                    }
                    else if( strElemName == "ZValue" )
                    {
                        strElemText = i_xmlStreamReader.readElementText();

                        double fTmp = strElemText.toDouble(&bConverted);

                        if( bConverted )
                        {
                            fZValue = fTmp;
                        }
                    }
                    else if( strElemName == "Labels" )
                    {
                        SErrResultInfo errResultInfo;
                        arpLabels = loadGraphObjLabels(i_xmlStreamReader,errResultInfo);
                    }
                } // if( xmlStreamReader.isStartElement() )

                else if( i_xmlStreamReader.isEndElement() )
                {
                    if( strElemName == "GraphObj" )
                    {
                        break;
                    }
                } // if( i_xmlStreamReader.isEndElement() )
            } // if( i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement() )
        } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

        if( bPosValid && bSizeValid )
        {
            pGraphObj = new CGraphObjRect(
                /* pDrawingScene */ i_pDrawingScene,
                /* drawSettings  */ drawSettings,
                /* strObjName    */ i_strObjName );

            pGraphObj->setRect( QRectF( QPointF(0.0,0.0), siz ) );

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

            pGraphObj->acceptCurrentAsOriginalCoors();

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

    } // if( i_pDrawingScene->findGraphObj(i_strObjId) == nullptr )

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(io_errResultInfo);
    }

    return pGraphObj;

} // loadGraphObj
