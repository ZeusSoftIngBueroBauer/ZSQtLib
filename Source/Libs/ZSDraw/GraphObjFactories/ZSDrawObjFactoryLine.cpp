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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryLine.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CObjFactoryLine : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryLine::CObjFactoryLine( const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ c_strGroupNameStandardShapes,
        /* strClassName    */ ClassName(),
        /* iGraphObjType   */ EGraphObjTypeLine,
        /* strGraphObjType */ ZS::Draw::graphObjType2Str(EGraphObjTypeLine),
        /* toolIcon        */ i_pxmToolIcon )
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
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strAddTrcInfo = "ItemPos {" + point2Str(i_ptItemPos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    // We need to create an object with at least one pixel size so that the
    // drawing scene can "find" the object and can dispatch events to it.
    // It is not important whether width or height is set to one pixel.
    // For a line it makes sense to set both extents to one pixel.
    QPointF ptStart = i_ptItemPos;
    QPointF ptEnd( i_ptItemPos.x()+1, i_ptItemPos.y()+1 );

    CGraphObjLine* pGraphObj = new CGraphObjLine(
        i_pDrawingScene, i_drawSettings, "", i_ptItemPos, i_ptItemPos);

    pGraphObj->setLine( QLineF(ptStart,ptEnd) );

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryLine::saveGraphObj(
    CGraphObj* i_pGraphObj,
    QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    if (i_pGraphObj == nullptr) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strAddTrcInfo = i_pGraphObj->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "saveGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    CGraphObjLine* pGraphObj = dynamic_cast<CGraphObjLine*>(i_pGraphObj);
    if (pGraphObj == nullptr) {
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

    //QHash<QString, CGraphObjLabel*> arpLabels = i_pGraphObj->getLabels();

    //if( arpLabels.size() > 0 )
    //{
    //    i_xmlStreamWriter.writeStartElement("Labels");
    //    errResultInfo = saveGraphObjLabels( arpLabels, i_xmlStreamWriter );
    //    i_xmlStreamWriter.writeEndElement();
    //}

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryLine::loadGraphObj(
    CDrawingScene*    i_pDrawingScene,
    CGraphObjGroup*   i_pGraphObjGroup,
    const QString&    i_strObjName,
    QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    if (i_pDrawingScene == nullptr) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pDrawingScene == nullptr" );
    }

    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strAddTrcInfo = ", ObjName: " + i_strObjName +
            "ParentGroup: " + QString(i_pGraphObjGroup == nullptr ? "null" : i_pGraphObjGroup->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "loadGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjLine* pGraphObj = nullptr;

    CDrawSettings drawSettings(EGraphObjTypeLine);
    QPointF pt1;
    QPointF pt2;
    bool bPt1Valid = false;
    bool bPt2Valid = false;
    double fZValue = 0.0;
    QHash<QString, CGraphObjLabel*> arpLabels;

    enum ELevel {
        ELevelGraphObj = 0, // expecting DrawSettings, Geometry, ZValue, Labels
        ELevelGeometry = 1, // expecting ShapePoints
        ELevelShapePoints = 2 // expecting P1, P2
    };
    int iLevel = ELevelGraphObj;

    while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd()) {
        QXmlStreamReader::TokenType xmlStreamTokenType = i_xmlStreamReader.readNext();
        if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement()) {
            QString strElemName = i_xmlStreamReader.name().toString();
            if (iLevel == ELevelGraphObj) {
                if (i_xmlStreamReader.isStartElement()) {
                    if (strElemName == CDrawingScene::c_strXmlElemNameDrawSettings) {
                        drawSettings.load(i_xmlStreamReader);
                    }
                    else if (strElemName == CDrawingScene::c_strXmlElemNameGeometry) {
                        if (i_xmlStreamReader.isStartElement()) {
                            iLevel++;
                        }
                    }
                    else if (strElemName == CDrawingScene::c_strXmlElemNameZValue) {
                        if (i_xmlStreamReader.isStartElement()) {
                            QString strElemText = i_xmlStreamReader.readElementText();
                            bool bConverted = false;
                            double fTmp = strElemText.toDouble(&bConverted);
                            if (bConverted) {
                                fZValue = fTmp;
                            }
                        }
                    }
                    else if (strElemName == CDrawingScene::c_strXmlElemNameLabels) {
                        if (i_xmlStreamReader.isStartElement()) {
                            arpLabels = loadGraphObjLabels(i_xmlStreamReader);
                        }
                    }
                    else {
                        i_xmlStreamReader.raiseError(
                            "Invalid format in XML object: Element \"" + strElemName + "\" not expected.");
                    }
                }
                else if (i_xmlStreamReader.isEndElement()) {
                    if (strElemName == "GraphObj") {
                        break;
                    }
                }
            }
            else if (iLevel == ELevelGeometry) {
                if (i_xmlStreamReader.isStartElement()) {
                    if (strElemName == CDrawingScene::c_strXmlElemNameShapePoints) {
                        iLevel++;
                    }
                    else {
                        i_xmlStreamReader.raiseError(
                            "Invalid format in XML object: Element \"" + strElemName + "\" not expected.");
                    }
                }
                else /*if (i_xmlStreamReader.isEndElement())*/ {
                    iLevel--;
                }
            }
            else if (iLevel == ELevelShapePoints) {
                if (i_xmlStreamReader.isStartElement()) {
                    if ((strElemName == CDrawingScene::c_strXmlElemNameShapePointP1) 
                     || (strElemName == CDrawingScene::c_strXmlElemNameShapePointP2)) {
                        QString strElemText = i_xmlStreamReader.readElementText();
                        bool bConverted = false;
                        QPointF ptTmp = str2PointF(strElemText, &bConverted);
                        if (!bConverted) {
                            i_xmlStreamReader.raiseError(
                                "Element \"" + strElemName + "\" (" + strElemText + ") cannot be converted to Point");
                        }
                        else if (strElemName == CDrawingScene::c_strXmlElemNameShapePointP1) {
                            pt1 = ptTmp;
                            bPt1Valid = true;
                        }
                        else if (strElemName == CDrawingScene::c_strXmlElemNameShapePointP2) {
                            pt2 = ptTmp;
                            bPt2Valid = true;
                        }
                    }
                    else {
                        i_xmlStreamReader.raiseError(
                            "Invalid format in XML object: Element \"" + strElemName + "\" not expected.");
                    }
                }
                else /*if (i_xmlStreamReader.isEndElement())*/ {
                    iLevel--;
                }
            }
        } // if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement())
    } // while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd())

    if (!bPt1Valid || !bPt2Valid) {
        i_xmlStreamReader.raiseError("Incomplete geometry: not all necessary shape points defined.");
    }
    else {
        pGraphObj = new CGraphObjLine(
            i_pDrawingScene, drawSettings, i_strObjName, pt1, pt2);
        //QLineF lin(pt1, pt2);
        //lin.translate(-pt1.x(), -pt1.y());
        //pGraphObj->setLine(lin);

        // Before calling "onGraphObjCreationFinished" the object must have been added
        // to its parent group. Otherwise the drawing scene is not able to retrieve
        // the unique object id and add the object to the hash.
        if (i_pGraphObjGroup != nullptr) {
            throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
            //i_pGraphObjGroup->addGraphObj(pGraphObj);
        }
        else {
            i_pDrawingScene->addGraphObj(pGraphObj);
        }
        pGraphObj->setStackingOrderValue(fZValue);

        //#pragma message(__TODO__"BEGIN: Shouldn't be necessary to call")
        //pGraphObj->setPos(pt1);
        //i_pDrawingScene->onGraphObjCreationFinished(pGraphObj);
        //pGraphObj->acceptCurrentAsOriginalCoors();
        //#pragma message(__TODO__"END: Shouldn't be necessary to call")

        //if (arpLabels.size() > 0) {
        //    pGraphObj->addLabels(arpLabels);
        //}
    } // if( bPt1Valid && bPt2Valid )

    if (arpLabels.size() > 0) {
        QHashIterator<QString, CGraphObjLabel*> itLabels(arpLabels);
        while (itLabels.hasNext()) {
            itLabels.next();
            CGraphObjLabel* pGraphObjLabel = itLabels.value();
            arpLabels.remove(pGraphObjLabel->getKey());
            delete pGraphObjLabel;
            pGraphObjLabel = nullptr;
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(i_xmlStreamReader.errorString());
        QString strMthRet = QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
        mthTracer.setMethodReturn(strMthRet);
    }
    return pGraphObj;

} // loadGraphObj
