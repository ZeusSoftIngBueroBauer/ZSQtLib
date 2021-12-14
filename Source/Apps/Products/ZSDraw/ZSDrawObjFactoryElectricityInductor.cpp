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

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsItem>
#else
#include <QtWidgets/QGraphicsItem>
#endif

#include "ZSDrawObjFactoryElectricityInductor.h"
#include "ZSDrawGraphObjElectricityInductor.h"

#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::Trace;


/*******************************************************************************
class CObjFactoryInductor : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryInductor::CObjFactoryInductor(
    bool         i_bAddToToolBoxObjPool,
    const QIcon& i_toolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName         */ "Draw::Electricity",
        /* strGraphObjNameSpace */ "ZS::Draw::Electricity",
        /* strGraphObjClassName */ "CGraphObjInductor",
        /* iGraphObjType        */ EGraphObjTypeUserDefined,
        /* strGraphObjType      */ "Inductor",
        /* bAddToToolBoxObjPool */ i_bAddToToolBoxObjPool,
        /* toolIcon             */ i_toolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryInductor::~CObjFactoryInductor()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryInductor::createGraphObj(
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

    CGraphObj* pGraphObj = new CGraphObjInductor(
        /* pDrawingScene */ i_pDrawingScene,
        /* drawSettings  */ i_drawSettings );

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryInductor::saveGraphObj(
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

    CGraphObjInductor* pGraphObj = dynamic_cast<CGraphObjInductor*>(i_pGraphObj);

    if( pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjInductor == nullptr" );
    }

    // Electrical Parameters
    //----------------------

    i_xmlStreamWriter.writeStartElement("ElectricalParameters");
    i_xmlStreamWriter.writeTextElement( "Inductance", QString::number(pGraphObj->getInductance()) );
    i_xmlStreamWriter.writeEndElement();

    // Draw Attributes
    //----------------

    CDrawSettings drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement("DrawSettings");
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    // Geometry
    //-------------

    QPointF ptPos         = pGraphObj->pos();
    QSizeF  siz           = pGraphObj->getSize();
    double  fRotAngle_deg = pGraphObj->getRotationAngleInDegree();

    i_xmlStreamWriter.writeStartElement("Geometry");
    i_xmlStreamWriter.writeTextElement( "Pos", point2Str(ptPos) );
    i_xmlStreamWriter.writeTextElement( "Size", size2Str(siz) );
    i_xmlStreamWriter.writeTextElement( "RotAngleDeg", QString::number(fRotAngle_deg) );
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
CGraphObj* CObjFactoryInductor::loadGraphObj(
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

    CGraphObjInductor* pGraphObj = nullptr;

    if( i_pDrawingScene->findGraphObj(i_strObjId) == nullptr )
    {
        QString                          strElemName;
        QString                          strElemText;
        bool                             bConverted;
        CDrawSettings                    drawSettings(EGraphObjTypeText);
        double                           fInductance_H = 1.0;
        QPointF                          ptPos;
        QSizeF                           siz;
        bool                             bPosValid = false;
        bool                             bSizeValid = false;
        double                           fRotAngle_deg = 0.0;
        double                           fZValue = 0.0;
        QHash<QString,SGraphObjLabel*>   arpLabels;

        while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
        {
            strElemName = i_xmlStreamReader.name().toString();

            if( i_xmlStreamReader.isStartElement() )
            {
                if( strElemName == "ElectricalParameters" )
                {
                }

                else if( strElemName == "Inductance" )
                {
                    strElemText = i_xmlStreamReader.readElementText();

                    double fValTmp = strElemText.toDouble(&bConverted);

                    if( bConverted )
                    {
                        fInductance_H = fValTmp;
                    }
                }

                else if( strElemName == "DrawSettings" )
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

        if( bPosValid && bSizeValid )
        {
            pGraphObj = new CGraphObjInductor(
                /* pDrawingScene */ i_pDrawingScene,
                /* drawSettings  */ drawSettings,
                /* strObjName    */ i_strObjName,
                /* strObjId      */ i_strObjId );

            QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

            i_pDrawingScene->addItem(pGraphicsItem);

            pGraphObj->setInductance(fInductance_H);

            pGraphObj->setPos(ptPos);
            pGraphObj->setSize(siz);
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

        } // if( !strText.isEmpty() && bPosValid )

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
        strAddTrcInfo = "ErrResultInfo {" + io_errResultInfo.toString() + "}";
        mthTracer.setMethodOutArgs(strAddTrcInfo);
    }

    return pGraphObj;

} // loadGraphObj

//------------------------------------------------------------------------------
void CObjFactoryInductor::ResetCtorsDtorsCounters()
//------------------------------------------------------------------------------
{
    CGraphObjInductor::ResetCtorsDtorsCounters();
}
