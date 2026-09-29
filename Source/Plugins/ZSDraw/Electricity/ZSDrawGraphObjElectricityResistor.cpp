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

#include "Electricity/ZSDrawGraphObjElectricityResistor.h"
#include "Electricity/ZSDrawWdgtFormatGraphObjsElectricityResistor.h"

#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjRect.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/QBitmap>
#include <QtGui/qevent.h>
#include <QtGui/QPainter>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QStyleOption>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::PhysVal;


/*******************************************************************************
class CGraphObjCapacitor : public CGraphObjElectricity
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QSize CGraphObjResistor::c_sizInitial(42.0, 8.0);

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjResistor::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjResistor::CGraphObjResistor(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObjElectricity(
        /* pDrawingScene */ i_pDrawingScene,
        /* strType       */ "Resistor",
        /* strObjName    */ i_strObjName.isEmpty() ? "R" + QString::number(s_iInstCount) : i_strObjName)
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    createTraceAdminObjs("Electricity::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "DrawingScene: " + pointer2Str(i_pDrawingScene) + ", ObjName: " + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);

    /*
    CnctPt1    Body    CnctPt2
            +--------+
    +--+    |        |    +--+
    |  |----|  -  -  |----|  |  <-- CnctLine (hidden below Body)
    +--+    |        |    +--+
            +--------+
    */

    // Please note that before adding graphic items to groups they must have
    // been added to the drawing scene. Otherwise their coordinates will not
    // be mapped correctly to the new parent.

    // Before adding the items to the drawing scene they will be created in their
    // own coordinate system starting at (0.0/0.0). After adding the items to the
    // drawing scene they will be moved by "setPos" relative within the group.
    // The group itself will be positioned by the caller of the ctor relative to
    // the caller's coordinate system.

    // The alignments will be adjusted in the order they are added. The order
    // takes effect on the result. Usually the size should be adjusted before
    // the positions to get relative adjustments working as expected.

    QRectF rctBounding(QPointF(0.0,0.0), c_sizInitial);
    double fBodyWidth = rctBounding.width() / 3.0;
    QRectF rctBody(rctBounding.center().x() - fBodyWidth/2.0, rctBounding.top(), fBodyWidth, rctBounding.height());

    SGraphObjAlignment alignment;

    QList<CGraphObj*> arpGraphObjs;

    // Draw settings for group item
    //-----------------------------

    //m_drawSettings.setAttributesAreUsed( true, EDrawAttributePenMin, EDrawAttributePenCount );
    ////m_drawSettings.setAttributesAreUsed( true, EDrawAttributeLineStyleMin, EDrawAttributeLineStyleCount );
    //m_drawSettings.setAttributesAreUsed( true, EDrawAttributeFillMin, EDrawAttributeFillCount );

    //m_drawSettings.setPenColor(Qt::darkYellow);
    //m_drawSettings.setPenWidth(1);
    //m_drawSettings.setLineStyle(ELineStyle::DotLine);
    //m_drawSettings.setFillColor(Qt::white);
    //m_drawSettings.setFillStyle(EFillStyle::SolidPattern);

    // Draw settings for elements
    //---------------------------

    //CDrawSettings drawSettingsLine(EGraphObjTypeLine);
    //drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineRecordTypeMin, EDrawAttributeLineRecordTypeCount );
    //drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineEndStyleMin, EDrawAttributeLineEndStyleCount );

    //CDrawSettings drawSettingsBody(EGraphObjTypeRect);
    //drawSettingsBody.setFillStyle(EFillStyle::SolidPattern);

    CDrawSettings drawSettingsCnctPt(EGraphObjTypeConnectionPoint);

    // Connection Line with ConnectionPoints
    //--------------------------------------

    m_pGraphObjLine = new CGraphObjLine(m_pDrawingScene, "CnctLine");
    //m_pGraphObjLine->setDrawSettings(drawSettingsLine);
    m_pGraphObjLine->setLine(CPhysValLine(*m_pDrawingScene,
        QPointF(rctBounding.left(), rctBounding.center().y()),
        QPointF(rctBounding.right(), rctBounding.center().y())));
    m_pDrawingScene->addGraphObj(m_pGraphObjLine);
    //m_pGraphObjLine->setPos(QPointF(0.0, rctBounding.center().y()));
    arpGraphObjs.append(m_pGraphObjLine);
    //m_pGraphObjLine->addConnectionPoint("CnctPt1", ESelectionPointType::PolygonPoint, 0);
    //m_pGraphObjLine->addConnectionPoint("CnctPt2", ESelectionPointType::PolygonPoint, 1);

    // Body
    //-----

    m_pGraphObjRectBody = new CGraphObjRect(m_pDrawingScene, "Body");
    //m_pGraphObjRectBody->setDrawSettings(drawSettingsBody);
    m_pGraphObjRectBody->setRect(CPhysValRect(*m_pDrawingScene,
        QPointF(rctBody.left(), rctBody.center().y()), rctBody.size()));
    m_pDrawingScene->addGraphObj(m_pGraphObjRectBody);
    //m_pGraphObjRectBody->setPos(rctBody.topLeft());
    arpGraphObjs.append(m_pGraphObjRectBody);

    // Create resistor as object group
    //--------------------------------

    //addToGroup(arpGraphObjs);

    //alignment = SGraphObjAlignment(EAlignmentRefWidth, EAlignmentRefWidth, false, 1.0);
    //m_pLinCnct->addAlignment(alignment);
    //alignment = SGraphObjAlignment(EAlignmentRef::Left, EAlignmentRef::Left, true, 0.0);
    //m_pLinCnct->addAlignment(alignment);
    //alignment = SGraphObjAlignment(EAlignmentRef::VCenter, EAlignmentRef::VCenter, true, 0.0);
    //m_pLinCnct->addAlignment(alignment);

    //alignment = SGraphObjAlignment(EAlignmentRefWidth, EAlignmentRefWidth, false, rctBody.width()/rctBounding.width());
    //m_pRctBody->addAlignment(alignment);
    //alignment = SGraphObjAlignment(EAlignmentRefHeight, EAlignmentRefHeight, false, rctBody.height()/rctBounding.height());
    //m_pRctBody->addAlignment(alignment);
    //alignment = SGraphObjAlignment(EAlignmentRef::HCenter, EAlignmentRef::HCenter, true, 0.0);
    //m_pRctBody->addAlignment(alignment);
    //alignment = SGraphObjAlignment(EAlignmentRef::VCenter, EAlignmentRef::VCenter, true, 0.0);
    //m_pRctBody->addAlignment(alignment);

    //alignment = SGraphObjAlignment(EAlignmentRef::Left, EAlignmentRef::Left, true, 0.0);
    //m_pCnctPt1->addAlignment(alignment);
    //alignment = SGraphObjAlignment(EAlignmentRef::VCenter, EAlignmentRef::VCenter, true, 0.0);
    //m_pCnctPt1->addAlignment(alignment);

    //alignment = SGraphObjAlignment(EAlignmentRef::Right, EAlignmentRef::Right, true, 0.0);
    //m_pCnctPt2->addAlignment(alignment);
    //alignment = SGraphObjAlignment(EAlignmentRef::VCenter, EAlignmentRef::VCenter, true, 0.0);
    //m_pCnctPt2->addAlignment(alignment);
}

//------------------------------------------------------------------------------
CGraphObjResistor::~CGraphObjResistor()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit_aboutToBeDestroyed();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjResistor::setResistance( double i_fResistance_Ohm )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strAddTrcInfo = "Resistance:" + QString::number(i_fResistance_Ohm) + " Ohm";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setResistance",
        /* strAddInfo   */ strAddTrcInfo );
    if (m_fResistance_Ohm != i_fResistance_Ohm) {
        m_fResistance_Ohm = i_fResistance_Ohm;
        //setDescription(QString::number(m_fResistance_Ohm) + " Ohm");
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjResistor::showResistance( ESelectionPoint i_selPtPos )
//------------------------------------------------------------------------------
{
    //showDescriptionLabel(i_selPtPos);
}

//------------------------------------------------------------------------------
void CGraphObjResistor::hideResistance()
//------------------------------------------------------------------------------
{
    //hideDescriptionLabel();
}

//------------------------------------------------------------------------------
bool CGraphObjResistor::isResistanceVisible( ESelectionPoint i_selPtPos ) const
//------------------------------------------------------------------------------
{
    return false; //isDescriptionLabelVisible(i_selPtPos);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjResistor::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjResistor* pGraphObj = nullptr;
    return pGraphObj;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjResistor::openFormatGraphObjsDialog()
//------------------------------------------------------------------------------
{
    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene, this);

    QIcon icon;
    QPixmap pxm(":/ZS/Draw/Electricity/Resistor16x16.bmp");
    pxm.setMask(pxm.createHeuristicMask());
    icon.addPixmap(pxm);

    CWdgtFormatGraphObjsResistor* pWdgt = new CWdgtFormatGraphObjsResistor(m_pDrawingScene, this);
    pDlgFormatGraphObjs->addWidget( icon, "Resistor", pWdgt );
    pDlgFormatGraphObjs->setCurrentWidget("Resistor");
    pDlgFormatGraphObjs->exec();
    delete pDlgFormatGraphObjs;
    pDlgFormatGraphObjs = nullptr;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjResistor::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    //CDrawSettings drawSettingsLine = m_pGraphObjLine->drawSettings();

    //drawSettingsLine.setPenColor(m_drawSettings.penColor());
    //drawSettingsLine.setPenWidth(m_drawSettings.penWidth());
    //drawSettingsLine.setLineStyle(m_drawSettings.lineStyle());

    //m_pGraphObjLine->setDrawSettings(drawSettingsLine);

    //CDrawSettings drawSettingsBody = m_pGraphObjRectBody->drawSettings();

    //drawSettingsBody.setPenColor(m_drawSettings.penColor());
    //drawSettingsBody.setPenWidth(m_drawSettings.penWidth());
    //drawSettingsBody.setLineStyle(m_drawSettings.lineStyle());
    //drawSettingsBody.setFillColor(m_drawSettings.fillColor());
    ////drawSettingsBody.setFillStyle(m_drawSettings.fillStyle()); keep SolidPattern

    //m_pGraphObjRectBody->setDrawSettings(drawSettingsBody);

    //CDrawSettings drawSettingsCnctPt = m_pCnctPt1->drawSettings();

    //drawSettingsCnctPt.setPenColor(m_drawSettings.penColor());
    //drawSettingsCnctPt.setPenWidth(m_drawSettings.penWidth());
    //drawSettingsCnctPt.setLineStyle(m_drawSettings.lineStyle());
    //drawSettingsCnctPt.setFillColor(m_drawSettings.fillColor()); keep black
    //drawSettingsCnctPt.setFillStyle(m_drawSettings.fillStyle()); keep SolidPattern

    //m_pCnctPt1->setDrawSettings(drawSettingsCnctPt);
    //m_pCnctPt2->setDrawSettings(drawSettingsCnctPt);

} // onDrawSettingsChanged

/*==============================================================================
protected: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjResistor::updateToolTip()
////------------------------------------------------------------------------------
//{
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//
//    if( pGraphicsItem != nullptr )
//    {
//        QString strNodeSeparator = CDrawingScene::getGraphObjNameNodeSeparator();
//        QPointF ptPos;
//
//        m_strToolTip  = "ObjName:\t" + name();
//        m_strToolTip += "\nObjId:\t\t" + keyInTree();
//
//        m_strToolTip += "Resistance:\t" + QString::number(m_fResistance_Ohm) + " Ohm";
//
//        // "scenePos" returns mapToScene(0,0). This is NOT equivalent to the
//        // position of the item's top left corner before applying the rotation
//        // transformation matrix but includes the transformation. What we want
//        // (or what I want) is the position of the item before rotating the item
//        // around the rotation origin point. In contrary it looks like "pos"
//        // always returns the top left corner before rotating the object.
//
//        if( pGraphicsItem->parentItem() != nullptr )
//        {
//            ptPos = pGraphicsItem->pos();
//            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
//        }
//        else
//        {
//            ptPos = pGraphicsItem->pos(); // don't use "scenePos" here (see comment above)
//            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
//        }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        m_strToolTip += "\nSize:\t\t" + size2Str(getSize());
//        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + ZS::System::Math::c_chSymbolDegree;
//#endif
//        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());
//
//        pGraphicsItem->setToolTip(m_strToolTip);
//
//    } // if( pGraphicsItem != nullptr )
//
//} // updateToolTip
