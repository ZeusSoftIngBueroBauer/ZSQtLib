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

#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryLine.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
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
    drawSettings.setGraphObjType(EGraphObjTypeLine);
    CGraphObjLine* pGraphObj = new CGraphObjLine(i_pDrawingScene);
    pGraphObj->setLine(i_physValPoint, i_physValPoint);
    pGraphObj->setDrawSettings(drawSettings);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(pGraphObj->path());
    }
    return pGraphObj;
}

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryLine::saveGraphObj(
    CGraphObj* i_pGraphObj,
    QXmlStreamWriter& i_xmlStreamWriter )
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

    CGraphObjLine* pGraphObj = dynamic_cast<CGraphObjLine*>(i_pGraphObj);
    if (pGraphObj == nullptr) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObj == nullptr" );
    }

    CDrawSettings drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameDrawSettings);
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    CPhysValPoint physValPoint1 = pGraphObj->getP1();
    CPhysValPoint physValPoint2 = pGraphObj->getP2();
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGeometry);
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameShapePoints);
    i_xmlStreamWriter.writeTextElement(XmlStreamParser::c_strXmlElemNameShapePointP1, physValPoint1.toString());
    i_xmlStreamWriter.writeTextElement(XmlStreamParser::c_strXmlElemNameShapePointP2, physValPoint2.toString());
    i_xmlStreamWriter.writeEndElement(); // ShapePoints
    i_xmlStreamWriter.writeEndElement(); // Geometry

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

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryLine::loadGraphObj(
    CDrawingScene* i_pDrawingScene,
    CGraphObjGroup* i_pGraphObjGroup,
    const QString& i_strObjName,
    QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    if (i_pDrawingScene == nullptr) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pDrawingScene == nullptr" );
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Group: " + QString(i_pGraphObjGroup == nullptr ? "null" : i_pGraphObjGroup->path()) +
            ", ObjName: " + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "loadGraphObj",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLine* pGraphObj = new CGraphObjLine(i_pDrawingScene, i_strObjName);
    i_pDrawingScene->addGraphObj(pGraphObj);

    CDrawSettings drawSettings(EGraphObjTypeLine);
    CPhysValPoint physValPoint1;
    CPhysValPoint physValPoint2;
    bool bPt1Valid = false;
    bool bPt2Valid = false;
    double fZValue = 0.0;
    QList<SLabelDscr> arTextLabels;
    QList<SLabelDscr> arGeometryLabels;

    enum ELevel {
        ELevelThisGraphObj = 0, // expecting DrawSettings, Geometry, ZValue, Labels
        ELevelGeometry = 1,     // expecting ShapePoints
        ELevelShapePoints = 2   // expecting P1, P2
    };
    int iLevel = ELevelThisGraphObj;

    while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd()) {
        QXmlStreamReader::TokenType xmlStreamTokenType = i_xmlStreamReader.readNext();
        if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement()) {
            QString strElemName = i_xmlStreamReader.name().toString();
            if (iLevel == ELevelThisGraphObj) {
                if (i_xmlStreamReader.isStartElement()) {
                    if (strElemName == XmlStreamParser::c_strXmlElemNameDrawSettings) {
                        drawSettings.load(i_xmlStreamReader);
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameGeometry) {
                        iLevel++;
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameZValue) {
                        QString strElemText = i_xmlStreamReader.readElementText();
                        bool bConverted = false;
                        double fTmp = strElemText.toDouble(&bConverted);
                        if (bConverted) {
                            fZValue = fTmp;
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
                else /*if (i_xmlStreamReader.isEndElement())*/ {
                    if (strElemName == XmlStreamParser::c_strXmlElemNameGraphObj) {
                        break;
                    }
                }
            }
            else if (iLevel == ELevelGeometry) {
                if (i_xmlStreamReader.isStartElement()) {
                    if (strElemName == XmlStreamParser::c_strXmlElemNameShapePoints) {
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
                    if ((strElemName == XmlStreamParser::c_strXmlElemNameShapePointP1) 
                     || (strElemName == XmlStreamParser::c_strXmlElemNameShapePointP2)) {
                        QString strElemText = i_xmlStreamReader.readElementText();
                        bool bConverted = false;
                        CPhysValPoint physValPointTmp(*i_pDrawingScene);
                        try {
                            physValPointTmp = strElemText;
                            bConverted = true;
                        }
                        catch (...) {
                            bConverted = false;
                        }
                        if (!bConverted) {
                            i_xmlStreamReader.raiseError(
                                "Element \"" + strElemName + "\" (" + strElemText + ") cannot be converted to Point");
                        }
                        else if (strElemName == XmlStreamParser::c_strXmlElemNameShapePointP1) {
                            physValPoint1 = physValPointTmp;
                            bPt1Valid = true;
                        }
                        else if (strElemName == XmlStreamParser::c_strXmlElemNameShapePointP2) {
                            physValPoint2 = physValPointTmp;
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
        if (!i_xmlStreamReader.hasError()) {
            i_xmlStreamReader.raiseError("Incomplete geometry: not all necessary shape points defined.");
        }
    }

    if (!i_xmlStreamReader.hasError()) {
        pGraphObj->setDrawSettings(drawSettings);
        if (i_pGraphObjGroup != nullptr) {
            // The object has been added to the drawing scene at position (0, 0) right after creating the object.
            // The shape points have not been set yet. When adding the object to the group, the group wants to
            // map the shape point coordinates to the group coordinates in order to resize the group so that the
            // newly added object fits into the group. For this to work, the object to be added must be able to
            // provide its position and shape points in scene coordinates to the group and the group must already
            // have gotten its final size so that the group is able to map the object coordinates to scene coordinates.
            physValPoint1 = dynamic_cast<CGraphObj*>(i_pGraphObjGroup)->mapToScene(physValPoint1);
            physValPoint2 = dynamic_cast<CGraphObj*>(i_pGraphObjGroup)->mapToScene(physValPoint2);
            pGraphObj->setLine(physValPoint1, physValPoint2);
            i_pGraphObjGroup->addToGroup(pGraphObj);
        }
        else {
            pGraphObj->setLine(physValPoint1, physValPoint2);
        }
        pGraphObj->setStackingOrderValue(fZValue);

        // Labels can only be added if the graphical object got its final position, size and rotation angle
        // as the labels position themselves depending on the position of the selection points they are linked to.
        for (const SLabelDscr& labelDscr : arTextLabels) {
            if (!pGraphObj->isLabelAdded(labelDscr.m_strKey)) {
                if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
                    pGraphObj->addLabel(labelDscr.m_strKey, labelDscr.m_strText, labelDscr.m_selPt1.m_selPt);
                }
                else if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
                    pGraphObj->addLabel(labelDscr.m_strKey, labelDscr.m_strText, labelDscr.m_selPt1.m_idxPt);
                }
            }
            else {
                pGraphObj->setLabelText(labelDscr.m_strKey, labelDscr.m_strText);
                if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
                    pGraphObj->setLabelAnchorPoint(labelDscr.m_strKey, labelDscr.m_selPt1.m_selPt);
                }
                else if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
                    pGraphObj->setLabelAnchorPoint(labelDscr.m_strKey, labelDscr.m_selPt1.m_idxPt);
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

} // loadGraphObj
