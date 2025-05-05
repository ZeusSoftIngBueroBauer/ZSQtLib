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
using namespace ZS::PhysVal;


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
}

//------------------------------------------------------------------------------
CObjFactoryConnectionLine::~CObjFactoryConnectionLine()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
int CObjFactoryConnectionLine::getNumberOfCreatedGraphObjects() const
//------------------------------------------------------------------------------
{
    return CGraphObjConnectionLine::s_iInstCount;
}

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryConnectionLine::createGraphObj(
    CDrawingScene* i_pDrawingScene, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "DrawSettings {" + i_drawSettings.toString() + "}";
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
    return pGraphObj;
}

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

    QPolygonF polygon({i_physValPoint.toQPointF(), i_physValPoint.toQPointF()});
    CPhysValPolygon physValPolygon(*i_pDrawingScene, polygon, i_physValPoint.unit());
    pGraphObj->setPolygon(physValPolygon);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(pGraphObj->path());
    }
    return pGraphObj;
}

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryConnectionLine::saveGraphObj(
    CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter) const
//------------------------------------------------------------------------------
{
    if (i_pGraphObj == nullptr) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObj->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "saveGraphObj",
        /* strAddInfo   */ strMthInArgs );

    SErrResultInfo errResultInfo;

    CGraphObjConnectionLine* pGraphObj = dynamic_cast<CGraphObjConnectionLine*>(i_pGraphObj);
    if (pGraphObj == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjCnctLine == nullptr");
    }

    CGraphObjConnectionPoint* pCnctPtStart = pGraphObj->getConnectionPoint(ELinePoint::Start);
    if (pCnctPtStart == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultConnectionError, "pCnctPtStart == nullptr");
    }
    CGraphObjConnectionPoint* pCnctPtEnd = pGraphObj->getConnectionPoint(ELinePoint::End);
    if (pCnctPtEnd == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultConnectionError, "pCnctPtEnd == nullptr");
    }

    // Connection lines don't belong to groups. But their connection points may.
    // To find the connection point it's not sufficient just to store the name in
    // the XML file but it's necessary to store the id of the connection point.
    i_xmlStreamWriter.writeTextElement("ObjIdCnctPtStart", pCnctPtStart->keyInTree());
    i_xmlStreamWriter.writeTextElement("ObjIdCnctPtEnd", pCnctPtEnd->keyInTree());

    const CDrawingScene* pDrawingScene = pGraphObj->drawingScene();
    const CDrawingSize& drawingSize = pDrawingScene->drawingSize();
    int iDecimals = 3;
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
        iDecimals = drawingSize.metricImageCoorsDecimals() + 3; // to avoid rounding errors add three digits
    }

    CDrawSettings drawSettings = pGraphObj->drawSettings();
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameDrawSettings);
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    CPhysValPolygon physValPolygon = pGraphObj->getPolygon(drawingSize.unit()); // rotated
    CPhysVal physValAngle = physValPolygon.angle();
    // The points will be saved unrotated and the rotation angle will be separately saved.
    // Otherwise the rotation angle cannot be restored.
    physValPolygon.setAngle(0.0);
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGeometry);
    for (int idxPt = 0; idxPt < physValPolygon.count(); ++idxPt) {
        QPointF pt = physValPolygon.at(idxPt).toQPointF();
        i_xmlStreamWriter.writeAttribute(
            XmlStreamParser::c_strXmlElemNameShapePointP + QString::number(idxPt+1), qPoint2Str(pt, ", ", 'f', iDecimals));
    }
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameAngle, physValAngle.toString());
    i_xmlStreamWriter.writeEndElement();

    i_xmlStreamWriter.writeTextElement(XmlStreamParser::c_strXmlElemNameZValue, QString::number(pGraphObj->getStackingOrderValue()));

    if (!i_pGraphObj->getLabelNames().isEmpty()) {
        i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameTextLabels);
        saveGraphObjTextLabels(i_pGraphObj, i_xmlStreamWriter);
        i_xmlStreamWriter.writeEndElement();
    }
    if (!i_pGraphObj->getGeometryLabelNames().isEmpty()) {
        i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGeometryLabels);
        saveGraphObjGeometryLabels(i_pGraphObj, i_xmlStreamWriter);
        i_xmlStreamWriter.writeEndElement();
    }
 
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;
}

//------------------------------------------------------------------------------
/*! @brief
    @note Connection lines never belong to groups.
*/
CGraphObj* CObjFactoryConnectionLine::loadGraphObj(
    CDrawingScene* i_pDrawingScene,
    CGraphObjGroup* i_pGraphObjGroupParent,
    const QString& i_strObjName,
    QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    if (i_pDrawingScene == nullptr)
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "pDrawingScene == nullptr");
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

    CGraphObjConnectionLine* pGraphObj = new CGraphObjConnectionLine(i_pDrawingScene, i_strObjName);
    i_pDrawingScene->addGraphObj(pGraphObj, i_pGraphObjGroupParent);

    CDrawSettings drawSettings(EGraphObjTypePolyline);
    CPhysValPolygon physValPolygon(*i_pDrawingScene, false);
    CPhysVal physValAngle(Units.Angle.Degree);
    double fZValue = 0.0;
    QList<SLabelDscr> arTextLabels;
    QList<SLabelDscr> arGeometryLabels;

    CGraphObjConnectionPoint* pCnctPtStart = nullptr;
    CGraphObjConnectionPoint* pCnctPtEnd   = nullptr;

    while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd()) {
        QXmlStreamReader::TokenType xmlStreamTokenType = i_xmlStreamReader.readNext();
        if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement()) {
            QString strElemName = i_xmlStreamReader.name().toString();
            QString strElemAttr;
            if (i_xmlStreamReader.isStartElement()) {
                if (strElemName == "ObjIdCnctPtStart") {
                    QString strObjId = i_xmlStreamReader.readElementText();
                    CGraphObj* pGraphObjTmp = i_pDrawingScene->findGraphObj(strObjId);
                    pCnctPtStart = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObjTmp);
                }
                else if (strElemName == "ObjIdCnctPtEnd") {
                    QString strObjId = i_xmlStreamReader.readElementText();
                    CGraphObj* pGraphObjTmp = i_pDrawingScene->findGraphObj(strObjId);
                    pCnctPtEnd = dynamic_cast<CGraphObjConnectionPoint*>(pGraphObjTmp);
                }
                else if (strElemName == XmlStreamParser::c_strXmlElemNameDrawSettings) {
                    drawSettings.load(i_xmlStreamReader);
                    pGraphObj->setDrawSettings(drawSettings);
                }
                else if (strElemName == XmlStreamParser::c_strXmlElemNameGeometry) {
                    QXmlStreamAttributes xmlStreamAttrs = i_xmlStreamReader.attributes();
                    int idxPt = 0;
                    QString strShapePointAttr = XmlStreamParser::c_strXmlElemNameShapePointP + QString::number(idxPt+1);
                    while (xmlStreamAttrs.hasAttribute(strShapePointAttr)) {
                        strElemAttr = xmlStreamAttrs.value(strShapePointAttr).toString();
                        bool bConverted = false;
                        CPhysValPoint physValPointTmp(*i_pDrawingScene);
                        try {
                            physValPointTmp = strElemAttr;
                            bConverted = true;
                        }
                        catch (...) {
                            bConverted = false;
                        }
                        if (!bConverted) {
                            i_xmlStreamReader.raiseError(
                                "Element \"" + strElemName + "\" (" + strElemAttr + ") cannot be converted to Point");
                            break;
                        }
                        physValPolygon.append(physValPointTmp);
                        strShapePointAttr = XmlStreamParser::c_strXmlElemNameShapePointP + QString::number(++idxPt+1);
                    }
                    if (physValPolygon.isEmpty()) {
                        XmlStreamParser::raiseErrorAttributeNotDefined(
                            i_xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameShapePoints);
                    }
                    if (!i_xmlStreamReader.hasError()) {
                        if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameAngle)) {
                            strElemAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameAngle).toString();
                            bool bConverted = false;
                            CPhysVal physValAngleTmp(Units.Angle.Degree);
                            try {
                                physValAngleTmp = strElemAttr;
                                bConverted = true;
                            }
                            catch (...) {
                                bConverted = false;
                            }
                            if (!bConverted) {
                                i_xmlStreamReader.raiseError(
                                    "Element \"" + strElemName + "\" (" + strElemAttr + ") cannot be converted to Angle");
                            }
                            else {
                                physValAngle = physValAngleTmp;
                            }
                        }
                    }
                    if (!i_xmlStreamReader.hasError()) {
                        if (physValPolygon.isValid()) {
                            pGraphObj->setPolygon(physValPolygon);
                        }
                        if (physValAngle.isValid()) {
                            pGraphObj->setRotationAngle(physValAngle);
                        }
                    }
                }
                else if (strElemName == XmlStreamParser::c_strXmlElemNameZValue) {
                    QString strElemText = i_xmlStreamReader.readElementText();
                    bool bConverted = false;
                    double fTmp = strElemText.toDouble(&bConverted);
                    if (!bConverted) {
                        i_xmlStreamReader.raiseError(
                            "Element \"" + strElemName + "\" (" + strElemText + ") cannot be converted to ZValue");
                    }
                    else {
                        fZValue = fTmp;
                        pGraphObj->setStackingOrderValue(fZValue);
                    }
                }
                else if (strElemName == XmlStreamParser::c_strXmlElemNameTextLabels) {
                    arTextLabels = loadGraphObjTextLabels(i_xmlStreamReader);
                }
                else if (strElemName == XmlStreamParser::c_strXmlElemNameGeometryLabels) {
                    arGeometryLabels = loadGraphObjGeometryLabels(i_xmlStreamReader);
                }
                else {
                    i_xmlStreamReader.raiseError(
                        "Invalid format in XML object: Element \"" + strElemName + "\" not expected.");
                }
            }
            else if (i_xmlStreamReader.isEndElement()) {
                if (strElemName == XmlStreamParser::c_strXmlElemNameGraphObj) {
                    break;
                }
            }
        } // if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement())
    } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

    if (pCnctPtStart == nullptr || pCnctPtEnd == nullptr || !physValPolygon.isValid()) {
        if (!i_xmlStreamReader.hasError()) {
            i_xmlStreamReader.raiseError("Incomplete geometry.");
        }
    }

    if (!i_xmlStreamReader.hasError()) {
        pGraphObj->setConnectionPoint(ELinePoint::Start, pCnctPtStart);
        pGraphObj->setConnectionPoint(ELinePoint::End, pCnctPtEnd);
        for (const SLabelDscr& labelDscr : arTextLabels) {
            if (!pGraphObj->isLabelAdded(labelDscr.m_strKey)) {
                if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
                    pGraphObj->addLabel(
                        labelDscr.m_strKey, labelDscr.m_strText, labelDscr.m_selPt1.m_selPtType, labelDscr.m_selPt1.m_selPt);
                }
                else {
                    pGraphObj->addLabel(
                        labelDscr.m_strKey, labelDscr.m_strText, labelDscr.m_selPt1.m_selPtType, labelDscr.m_selPt1.m_idxPt);
                }
            }
            else {
                pGraphObj->setLabelText(labelDscr.m_strKey, labelDscr.m_strText);
                if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
                    pGraphObj->setLabelAnchorPoint(
                        labelDscr.m_strKey, labelDscr.m_selPt1.m_selPtType, labelDscr.m_selPt1.m_selPt);
                }
                else {
                    pGraphObj->setLabelAnchorPoint(
                        labelDscr.m_strKey, labelDscr.m_selPt1.m_selPtType, labelDscr.m_selPt1.m_idxPt);
                }
            }
            pGraphObj->setLabelPolarCoorsToLinkedSelectionPoint(
                labelDscr.m_strKey, labelDscr.m_polarCoorsToLinkedSelPt);
            labelDscr.m_bLabelIsVisible ?
                pGraphObj->showLabel(labelDscr.m_strKey) :
                pGraphObj->hideLabel(labelDscr.m_strKey);
            labelDscr.m_bShowAnchorLine ?
                pGraphObj->showLabelAnchorLine(labelDscr.m_strKey) :
                pGraphObj->hideLabelAnchorLine(labelDscr.m_strKey);
        }
        // Geometry Labels
        for (const SLabelDscr& labelDscr : arGeometryLabels) {
            if (!pGraphObj->isValidGeometryLabelName(labelDscr.m_strKey)) {
                i_xmlStreamReader.raiseError(
                    "Invalid geometry label name \"" + labelDscr.m_strKey + "\".");
            }
            else {
                pGraphObj->setGeometryLabelPolarCoorsToLinkedSelectionPoint(
                    labelDscr.m_strKey, labelDscr.m_polarCoorsToLinkedSelPt);
                labelDscr.m_bLabelIsVisible ?
                    pGraphObj->showGeometryLabel(labelDscr.m_strKey) :
                    pGraphObj->hideGeometryLabel(labelDscr.m_strKey);
                labelDscr.m_bShowAnchorLine ?
                    pGraphObj->showGeometryLabelAnchorLine(labelDscr.m_strKey) :
                    pGraphObj->hideGeometryLabelAnchorLine(labelDscr.m_strKey);
            }
        }
    }
    else {
        delete pGraphObj;
        pGraphObj = nullptr;
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(i_xmlStreamReader.errorString());
        QString strMthRet = QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
        mthTracer.setMethodReturn(strMthRet);
    }
    return pGraphObj;
}
