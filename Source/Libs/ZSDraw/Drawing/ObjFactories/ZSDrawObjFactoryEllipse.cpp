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

#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryEllipse.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysAux.h"
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
class CObjFactoryEllipse : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryEllipse::CObjFactoryEllipse( const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ c_strGroupNameStandardShapes,
        /* strClassName    */ ClassName(),
        /* iGraphObjType   */ EGraphObjTypeEllipse,
        /* strGraphObjType */ ZS::Draw::graphObjType2Str(EGraphObjTypeEllipse),
        /* toolIcon        */ i_pxmToolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryEllipse::~CObjFactoryEllipse()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryEllipse::createGraphObj(
    CDrawingScene* i_pDrawingScene,
    const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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

    CDrawSettings drawSettings = i_drawSettings;
    drawSettings.setGraphObjType(EGraphObjTypeEllipse);
    CGraphObjEllipse* pGraphObj = new CGraphObjEllipse(i_pDrawingScene);
    pGraphObj->setDrawSettings(drawSettings);

#if 0
    QPointF ptStart = i_ptItemPos;
    QPointF ptEnd( i_ptItemPos.x()+1, i_ptItemPos.y()+1 );
    pGraphObj->setRect( QRectF(ptStart,ptEnd) );
#endif

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryEllipse::createGraphObj(
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

    CDrawSettings drawSettings = i_drawSettings;
    drawSettings.setGraphObjType(EGraphObjTypeEllipse);
    CGraphObjEllipse* pGraphObj = new CGraphObjEllipse(i_pDrawingScene);
    pGraphObj->setDrawSettings(drawSettings);

#if 0
    QPointF ptStart = i_ptItemPos;
    QPointF ptEnd( i_ptItemPos.x()+1, i_ptItemPos.y()+1 );
    pGraphObj->setRect( QRectF(ptStart,ptEnd) );
#endif

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryEllipse::saveGraphObj(
    CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter) const
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

    CGraphObjEllipse* pGraphObj = dynamic_cast<CGraphObjEllipse*>(i_pGraphObj);

    if( pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjEllipse == nullptr" );
    }

    // Draw Attributes
    //----------------

    CDrawSettings drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameDrawSettings);
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    // Geometry
    //-------------

    QPointF ptPos         = pGraphObj->pos();
    QRectF  rct           = pGraphObj->rect(); // coordinates are in the objects coordinate system (LT = 0.0/0.0)
    double  fRotAngle_deg = 0.0; //pGraphObj->getRotationAngleInDegree();

    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGeometry);
    i_xmlStreamWriter.writeTextElement( "Pos", qPoint2Str(ptPos) );
    i_xmlStreamWriter.writeTextElement( "Size", qSize2Str(rct.size()) );
    i_xmlStreamWriter.writeTextElement( "RotAngleDeg", QString::number(fRotAngle_deg) );
    i_xmlStreamWriter.writeEndElement();

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
CGraphObj* CObjFactoryEllipse::loadGraphObj(
    CDrawingScene*    i_pDrawingScene,
    CGraphObjGroup*   i_pGraphObjGroupParent,
    const QString&    i_strObjName,
    QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    if( i_pDrawingScene == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pDrawingScene == nullptr" );
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ParentGroup: " + QString(i_pGraphObjGroupParent == nullptr ? "null" : i_pGraphObjGroupParent->path())
            + ", ObjName: " + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "loadGraphObj",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjEllipse* pGraphObj = nullptr;

    QString       strElemName;
    QString       strElemText;
    bool          bConverted;
    CDrawSettings drawSettings(EGraphObjTypeEllipse);
    QPointF       ptPos;
    QSizeF        siz;
    bool          bPosValid = false;
    bool          bSizeValid = false;
    double        fRotAngle_deg = 0.0;
    double        fZValue = 0.0;

    QList<SLabelDscr> arTextLabels;

    while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
    {
        //xmlStreamTokenType = i_xmlStreamReader.readNext();
        strElemName = i_xmlStreamReader.name().toString();

        if( i_xmlStreamReader.isStartElement() )
        {
            if( strElemName == XmlStreamParser::c_strXmlElemNameDrawSettings )
            {
                drawSettings.load(i_xmlStreamReader);
            }

            else if( strElemName == XmlStreamParser::c_strXmlElemNameGeometry )
            {
            }

            else if( strElemName == "Pos" )
            {
                strElemText = i_xmlStreamReader.readElementText();

                QPointF ptTmp = str2QPointF(strElemText, &bConverted);

                if( bConverted )
                {
                    ptPos = ptTmp;
                    bPosValid = true;
                }

            } // if( strElemName == "Pos" )

            else if( strElemName == "Size" )
            {
                strElemText = i_xmlStreamReader.readElementText();

                QSizeF sizTmp = str2QSizeF(strElemText, &bConverted);

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

            else if( strElemName == XmlStreamParser::c_strXmlElemNameTextLabels )
            {
                arTextLabels = loadGraphObjTextLabels(i_xmlStreamReader);

            } // if( strElemName == XmlStreamParser::c_strXmlElemNameTextLabels )

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
        pGraphObj = new CGraphObjEllipse(i_pDrawingScene, i_strObjName);
        pGraphObj->setDrawSettings(drawSettings);

        pGraphObj->setRect( QRectF( QPointF(0.0,0.0), siz ) );

        i_pDrawingScene->addGraphObj(pGraphObj);

        pGraphObj->setPos(ptPos);
        //pGraphObj->setRotationAngleInDegree(fRotAngle_deg);
        pGraphObj->setStackingOrderValue(fZValue);

        // Before calling "onGraphObjCreationFinished" the object must have been added
        // to its parent group. Otherwise the drawing scene is not able to retrieve
        // the unique object id and add the object to the hash.
        if( i_pGraphObjGroupParent != nullptr )
        {
            throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
            //i_pGraphObjGroup->addGraphObj(pGraphObj);
        }

        //i_pDrawingScene->onGraphObjCreationFinished(pGraphObj);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        pGraphObj->acceptCurrentAsOriginalCoors();
#endif

    } // if( bPosValid && bSizeValid )

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(i_xmlStreamReader.errorString());
        QString strMthRet = QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
        mthTracer.setMethodReturn(strMthRet);
    }
    return pGraphObj;

} // loadGraphObj
