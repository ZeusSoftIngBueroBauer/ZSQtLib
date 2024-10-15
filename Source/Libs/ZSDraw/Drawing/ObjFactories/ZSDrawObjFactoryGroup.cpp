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

#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryGroup.h"
#include "ZSDraw/Common/ZSDrawAux.h"
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
using namespace ZS::PhysVal;


/*******************************************************************************
class CObjFactoryGroup : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryGroup::CObjFactoryGroup( const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ c_strGroupNameStandardShapes,
        /* strClassName    */ ClassName(),
        /* iGraphObjType   */ EGraphObjTypeGroup,
        /* strGraphObjType */ ZS::Draw::graphObjType2Str(EGraphObjTypeGroup),
        /* toolIcon        */ i_pxmToolIcon )
{
}

//------------------------------------------------------------------------------
CObjFactoryGroup::~CObjFactoryGroup()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryGroup::createGraphObj(
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
    drawSettings.setGraphObjType(EGraphObjTypeGroup);
    CGraphObjGroup* pGraphObj = new CGraphObjGroup(i_pDrawingScene);
    pGraphObj->setDrawSettings(drawSettings);
    return pGraphObj;
}

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryGroup::saveGraphObj(
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

    CGraphObjGroup* pGraphObj = dynamic_cast<CGraphObjGroup*>(i_pGraphObj);
    if (pGraphObj == nullptr) {
        throw CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjGroup == nullptr");
    }

    const CDrawingScene* pDrawingScene = pGraphObj->drawingScene();
    const CDrawingSize& drawingSize = pDrawingScene->drawingSize();
    int iDecimals = 3;
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric) {
        iDecimals = drawingSize.metricImageCoorsDecimals() + 3; // to avoid rounding errors add three digits
    }

    const CDrawGridSettings& gridSettings = pGraphObj->gridSettings();
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGridSettings);
    gridSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    const CDrawSettings& drawSettings = pGraphObj->getDrawSettings();
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameDrawSettings);
    drawSettings.save(i_xmlStreamWriter);
    i_xmlStreamWriter.writeEndElement();

    CPhysValRect physValRect = pGraphObj->getRect();
    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGeometry);
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameCenter, physValRect.center().toString(false, ", ", iDecimals));
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameSize, physValRect.size().toString(false, ", ", iDecimals));
    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameAngle, physValRect.angle().toString());
    i_xmlStreamWriter.writeEndElement();

    i_xmlStreamWriter.writeTextElement("ZValue", QString::number(pGraphObj->getStackingOrderValue()));

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

    // Connection points need to be recalled before the connection lines as on
    // creating the connection lines their connection points must already exist.
    // For this the connection lines will be saved at the end of the XML file.
    // Labels and selection points will not be saved at all (labels are created by their parents).
    for (CGraphObj* pGraphObjChild : pGraphObj->childs()) {
        if (!pGraphObjChild->isSelectionPoint() && !pGraphObjChild->isLabel() && !pGraphObjChild->isConnectionLine()) {
            QString strFactoryGroupName = pGraphObjChild->getFactoryGroupName();
            QString strGraphObjType = pGraphObjChild->typeAsString();
            QString strObjName = pGraphObjChild->name();
            CObjFactory* pObjFactoryChild = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
            if (pObjFactoryChild != nullptr) {
                i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGraphObj);
                i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName, strFactoryGroupName);
                i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjType, strGraphObjType);
                i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjName, strObjName);
                errResultInfo = pObjFactoryChild->saveGraphObj(pGraphObjChild, i_xmlStreamWriter);
                i_xmlStreamWriter.writeEndElement();
                if (errResultInfo.isErrorResult()) {
                    break;
                }
            }
        }
    }
    if (!errResultInfo.isErrorResult()) {
        for (CGraphObj* pGraphObjChild : pGraphObj->childs()) {
            if (pGraphObjChild->isConnectionLine()) {
                QString strFactoryGroupName = pGraphObjChild->getFactoryGroupName();
                QString strGraphObjType = pGraphObjChild->typeAsString();
                QString strObjName = pGraphObjChild->name();
                CObjFactory* pObjFactoryChild = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
                if (pObjFactoryChild != nullptr) {
                    i_xmlStreamWriter.writeStartElement(XmlStreamParser::c_strXmlElemNameGraphObj);
                    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName, strFactoryGroupName);
                    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjType, strGraphObjType);
                    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlElemNameGraphObjName, strObjName);
                    errResultInfo = pObjFactoryChild->saveGraphObj(pGraphObjChild, i_xmlStreamWriter);
                    i_xmlStreamWriter.writeEndElement();
                    if (errResultInfo.isErrorResult()) {
                        break;
                    }
                }
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryGroup::loadGraphObj(
    CDrawingScene*    i_pDrawingScene,
    CGraphObjGroup*   i_pGraphObjGroupParent,
    const QString&    i_strObjName,
    QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    if (i_pDrawingScene == nullptr) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pDrawingScene == nullptr" );
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ObjName: " + i_strObjName +
            "ParentGroup: " + QString(i_pGraphObjGroupParent == nullptr ? "null" : i_pGraphObjGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "loadGraphObj",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjGroup* pGraphObjGroup = new CGraphObjGroup(i_pDrawingScene, i_strObjName);
    i_pDrawingScene->addGraphObj(pGraphObjGroup, i_pGraphObjGroupParent);

    CDrawSettings drawSettings(EGraphObjTypeGroup);
    CDrawGridSettings gridSettings;
    CPhysValPoint physValPointCenter(*i_pDrawingScene);
    CPhysValSize physValSize(*i_pDrawingScene);
    CPhysVal physValAngle(Units.Angle.Degree);
    double fZValue = 0.0;
    QList<SLabelDscr> arTextLabels;
    QList<SLabelDscr> arGeometryLabels;
    bool bGeometrySet = false;
    bool bAddedToParentGroup = false;

    enum ELevel {
        ELevelThisGraphObj = 0,  // expecting GridSettings, DrawSettings, Geometry, ZValue, Labels, Childs
        ELevelChilds       = 1,  // expecting Childs
    };
    int iLevel = ELevelThisGraphObj;

    while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd()) {
        QXmlStreamReader::TokenType xmlStreamTokenType = i_xmlStreamReader.readNext();
        if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement()) {
            QString strElemName = i_xmlStreamReader.name().toString();
            QString strElemAttr;
            if (iLevel == ELevelThisGraphObj) {
                if (i_xmlStreamReader.isStartElement()) {
                    if (strElemName == XmlStreamParser::c_strXmlElemNameDrawSettings) {
                        drawSettings.load(i_xmlStreamReader);
                        pGraphObjGroup->setDrawSettings(drawSettings);
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameGridSettings) {
                        gridSettings.load(i_xmlStreamReader);
                        pGraphObjGroup->setGridSettings(gridSettings);
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameGeometry) {
                        QXmlStreamAttributes xmlStreamAttrs = i_xmlStreamReader.attributes();
                        if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameCenter)) {
                            strElemAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameCenter).toString();
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
                            }
                            else {
                                physValPointCenter = physValPointTmp;
                            }
                        }
                        else {
                            XmlStreamParser::raiseErrorAttributeNotDefined(
                                i_xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameCenter);
                        }
                        if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameSize)) {
                            strElemAttr = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameSize).toString();
                            bool bConverted = false;
                            CPhysValSize physValSizeTmp(*i_pDrawingScene);
                            try {
                                physValSizeTmp = strElemAttr;
                                bConverted = true;
                            }
                            catch (...) {
                                bConverted = false;
                            }
                            if (!bConverted) {
                                i_xmlStreamReader.raiseError(
                                    "Element \"" + strElemName + "\" (" + strElemAttr + ") cannot be converted to Size");
                            }
                            else if (!physValSizeTmp.isValid()) {
                                i_xmlStreamReader.raiseError(
                                    "Element \"" + strElemName + "\" (" + strElemAttr + ") cannot be converted to Size");
                            }
                            else {
                                physValSize = physValSizeTmp;
                            }
                        }
                        else {
                            XmlStreamParser::raiseErrorAttributeNotDefined(
                                i_xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameSize);
                        }
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
                        else {
                            XmlStreamParser::raiseErrorAttributeNotDefined(
                                i_xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameAngle);
                        }
                        if (!i_xmlStreamReader.hasError()) {
                            if (physValPointCenter.isValid() && physValSize.isValid()) {
                                CPhysValRect physValRect(*i_pDrawingScene);
                                physValRect.setSize(physValSize);
                                physValRect.setCenter(physValPointCenter);
                                pGraphObjGroup->setRect(physValRect);
                            }
                            if (physValAngle.isValid()) {
                                pGraphObjGroup->setRotationAngle(physValAngle);
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
                            pGraphObjGroup->setStackingOrderValue(fZValue);
                        }
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameTextLabels) {
                        arTextLabels = loadGraphObjTextLabels(i_xmlStreamReader);
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameGeometryLabels) {
                        arGeometryLabels = loadGraphObjGeometryLabels(i_xmlStreamReader);
                    }
                    else if (strElemName == XmlStreamParser::c_strXmlElemNameGraphObj) {
                        // The group has been added to the drawing scene at position (0, 0) right after creating the group.
                        // By adding child objects to the group the group will map the shape point coordinates of the child
                        // to the group coordinates and will try to resize the group so that the newly added object fits into
                        // the group. In order for the group to map the coordinates of the new child object, the group must
                        // already have gotten its final size.
                        if (!physValPointCenter.isValid() || !physValSize.isValid() || !physValAngle.isValid()) {
                            i_xmlStreamReader.raiseError("Incomplete geometry.");
                        }
                        else {
                            //if (i_pGraphObjGroupParent != nullptr) {
                            //    if (!bAddedToParentGroup) {
                            //        // The object has been added to the drawing scene at position (0, 0) right after creating the object.
                            //        // The shape points have not been set yet. When adding the object to the group, the group wants to
                            //        // map the shape point coordinates to the group coordinates in order to resize the group so that the
                            //        // newly added object fits into the group. For this to work, the object to be added must be able to
                            //        // provide its position and shape points in scene coordinates to the group and the group must already
                            //        // have gotten its final size so that the group is able to map the object coordinates to scene coordinates.
                            //        physValPointCenter = i_pGraphObjGroupParent->mapToScene(physValPointCenter);
                            //        CPhysValRect physValRect(*i_pDrawingScene);
                            //        physValRect.setSize(physValSize);
                            //        physValRect.setCenter(physValPointCenter);
                            //        physValRect.setAngle(physValAngle);
                            //        pGraphObj->setRect(physValRect);
                            //        bGeometrySet = true;
                            //        i_pGraphObjGroupParent->addToGroup(pGraphObj);
                            //        bAddedToParentGroup = true;
                            //    }
                            //}
                            //else if (!bGeometrySet) {
                            //    CPhysValRect physValRect(*i_pDrawingScene);
                            //    physValRect.setSize(physValSize);
                            //    physValRect.setCenter(physValPointCenter);
                            //    physValRect.setAngle(physValAngle);
                            //    pGraphObjGroup->setRect(physValRect);
                            //    bGeometrySet = true;
                            //}
                            iLevel++;
                            QString strFactoryGroupName;
                            QString strGraphObjType;
                            QString strObjName;
                            QXmlStreamAttributes xmlStreamAttrs = i_xmlStreamReader.attributes();
                            if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName)) {
                                strFactoryGroupName = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName).toString();
                            }
                            else {
                                XmlStreamParser::raiseErrorAttributeNotDefined(
                                    i_xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameGraphObjFactoryGroupName);
                            }
                            if (!i_xmlStreamReader.hasError()) {
                                if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameGraphObjType)) {
                                    strGraphObjType = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameGraphObjType).toString();
                                }
                                else {
                                    XmlStreamParser::raiseErrorAttributeNotDefined(
                                        i_xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameGraphObjType);
                                }
                            }
                            if (!i_xmlStreamReader.hasError()) {
                                if (xmlStreamAttrs.hasAttribute(XmlStreamParser::c_strXmlElemNameGraphObjName)) {
                                    strObjName = xmlStreamAttrs.value(XmlStreamParser::c_strXmlElemNameGraphObjName).toString();
                                }
                                else {
                                    XmlStreamParser::raiseErrorAttributeNotDefined(
                                        i_xmlStreamReader, strElemName, XmlStreamParser::c_strXmlElemNameGraphObjName);
                                }
                            }
                            if (!i_xmlStreamReader.hasError()) {
                                CObjFactory* pObjFactory = CObjFactory::FindObjFactory(strFactoryGroupName, strGraphObjType);
                                if (pObjFactory == nullptr) {
                                    i_xmlStreamReader.raiseError(
                                        "ObjectFactory \"" + strFactoryGroupName + "::" + strGraphObjType + "\" for element \"" + strElemName + "\" not found");
                                }
                                else {
                                    pObjFactory->loadGraphObj(
                                        /* pDrawingScene   */ i_pDrawingScene,
                                        /* pGraphObjGroup  */ pGraphObjGroup,
                                        /* strObjName      */ strObjName,
                                        /* xmlStreamReader */ i_xmlStreamReader );
                                }
                            }
                            iLevel--;
                        }
                    }
                    else {
                        i_xmlStreamReader.raiseError(
                            "Invalid format in XML object: Element \"" + strElemName + "\" not expected.");
                    }
                }
                else /*if (i_xmlStreamReader.isEndElement())*/ {
                    if (strElemName == XmlStreamParser::c_strXmlElemNameGraphObj) {
                        if (iLevel == ELevelThisGraphObj) {
                            break;
                        }
                        else {
                            --iLevel;
                        }
                    }
                }
            }
            else if (iLevel == ELevelChilds) {
                if (i_xmlStreamReader.isStartElement()) {
                    if (strElemName == XmlStreamParser::c_strXmlElemNameRectangle) {
                        iLevel++;
                    }
                    else {
                        i_xmlStreamReader.raiseError(
                            "Invalid format in XML object: Element \"" + strElemName + "\" not expected.");
                    }
                }
                else /*if (i_xmlStreamReader.isEndElement())*/ {
                    if (strElemName == XmlStreamParser::c_strXmlElemNameGeometry) {
                        if (!physValPointCenter.isValid() || !physValSize.isValid() || !physValAngle.isValid()) {
                            if (!i_xmlStreamReader.hasError()) {
                                i_xmlStreamReader.raiseError("Incomplete geometry.");
                            }
                        }
                    }
                    iLevel--;
                }
            }
        } // if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement()) {
    } // while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd())

    //if (!physValPointCenter.isValid() || !physValSize.isValid() || !physValAngle.isValid()) {
    //    if (!i_xmlStreamReader.hasError()) {
    //        i_xmlStreamReader.raiseError("Incomplete geometry: not all necessary shape points defined.");
    //    }
    //}

    if (!i_xmlStreamReader.hasError()) {
        //if (i_pGraphObjGroupParent != nullptr) {
        //    if (!bAddedToParentGroup) {
        //        // The object has been added to the drawing scene at position (0, 0) right after creating the object.
        //        // The shape points have not been set yet. When adding the object to the group, the group wants to
        //        // map the shape point coordinates to the group coordinates in order to resize the group so that the
        //        // newly added object fits into the group. For this to work, the object to be added must be able to
        //        // provide its position and shape points in scene coordinates to the group and the group must already
        //        // have gotten its final size so that the group is able to map the object coordinates to scene coordinates.
        //        physValPointCenter = i_pGraphObjGroupParent->mapToScene(physValPointCenter);
        //        CPhysValRect physValRect(*i_pDrawingScene);
        //        physValRect.setSize(physValSize);
        //        physValRect.setCenter(physValPointCenter);
        //        physValRect.setAngle(physValAngle);
        //        pGraphObj->setRect(physValRect);
        //        bGeometrySet = true;
        //        i_pGraphObjGroupParent->addToGroup(pGraphObj);
        //        bAddedToParentGroup = true;
        //    }
        //}
        //else if (!bGeometrySet) {
        //    CPhysValRect physValRect(*i_pDrawingScene);
        //    physValRect.setSize(physValSize);
        //    physValRect.setCenter(physValPointCenter);
        //    physValRect.setAngle(physValAngle);
        //    pGraphObj->setRect(physValRect);
        //    bGeometrySet = true;
        //}
        //pGraphObj->setDrawSettings(drawSettings);
        //pGraphObj->setGridSettings(gridSettings);
        //pGraphObj->setStackingOrderValue(fZValue);

        // Labels can only be added if the graphical object got its final position, size and rotation angle
        // as the labels position themselves depending on the position of the selection points they are linked to.
        for (const SLabelDscr& labelDscr : arTextLabels) {
            if (!pGraphObjGroup->isLabelAdded(labelDscr.m_strKey)) {
                if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
                    pGraphObjGroup->addLabel(labelDscr.m_strKey, labelDscr.m_strText, labelDscr.m_selPt1.m_selPt);
                }
                else if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
                    pGraphObjGroup->addLabel(labelDscr.m_strKey, labelDscr.m_strText, labelDscr.m_selPt1.m_idxPt);
                }
            }
            else {
                pGraphObjGroup->setLabelText(labelDscr.m_strKey, labelDscr.m_strText);
                if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::BoundingRectangle) {
                    pGraphObjGroup->setLabelAnchorPoint(labelDscr.m_strKey, labelDscr.m_selPt1.m_selPt);
                }
                else if (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonShapePoint) {
                    pGraphObjGroup->setLabelAnchorPoint(labelDscr.m_strKey, labelDscr.m_selPt1.m_idxPt);
                }
            }
            pGraphObjGroup->setLabelPolarCoorsToLinkedSelectionPoint(
                labelDscr.m_strKey, labelDscr.m_polarCoorsToLinkedSelPt);
            labelDscr.m_bLabelIsVisible ?
                pGraphObjGroup->showLabel(labelDscr.m_strKey) :
                pGraphObjGroup->hideLabel(labelDscr.m_strKey);
            labelDscr.m_bShowAnchorLine ?
                pGraphObjGroup->showLabelAnchorLine(labelDscr.m_strKey) :
                pGraphObjGroup->hideLabelAnchorLine(labelDscr.m_strKey);
        }

        // Geometry Labels
        for (const SLabelDscr& labelDscr : arGeometryLabels) {
            if (!pGraphObjGroup->isValidGeometryLabelName(labelDscr.m_strKey)) {
                i_xmlStreamReader.raiseError(
                    "Invalid geometry label name \"" + labelDscr.m_strKey + "\".");
            }
            else {
                pGraphObjGroup->setGeometryLabelPolarCoorsToLinkedSelectionPoint(
                    labelDscr.m_strKey, labelDscr.m_polarCoorsToLinkedSelPt);
                labelDscr.m_bLabelIsVisible ?
                    pGraphObjGroup->showGeometryLabel(labelDscr.m_strKey) :
                    pGraphObjGroup->hideGeometryLabel(labelDscr.m_strKey);
                labelDscr.m_bShowAnchorLine ?
                    pGraphObjGroup->showGeometryLabelAnchorLine(labelDscr.m_strKey) :
                    pGraphObjGroup->hideGeometryLabelAnchorLine(labelDscr.m_strKey);
            }
        }
    }
    else {
        delete pGraphObjGroup;
        pGraphObjGroup = nullptr;
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(i_xmlStreamReader.errorString());
        QString strMthRet = QString(pGraphObjGroup == nullptr ? "null" : pGraphObjGroup->path());
        mthTracer.setMethodReturn(strMthRet);
    }
    return pGraphObjGroup;

} // loadGraphObj
