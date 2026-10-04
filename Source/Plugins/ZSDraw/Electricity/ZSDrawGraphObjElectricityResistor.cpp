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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjRect.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
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
using namespace ZS::Draw::Plugins::Electricity;
using namespace ZS::PhysVal;


/*******************************************************************************
class CGraphObjCapacitor : public CGraphObjElectricity
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QSize CGraphObjResistor::c_sizInitial(90.0, 20.0);

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

    createTraceAdminObjs("ZS::Draw::Plugins::Electricity::Drawing", ClassName());

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

    m_pDrawingScene->addGraphObj(this);

    /*
                          Body
                       +--------+
             CnctLine1 |        | CnctLine2
    CnctPt1 X----------X        X----------X CnctPt2
                       |        |
                       +--------+
    */


    // Before adding the items to the drawing scene they will be created in their
    // own coordinate system starting at (0.0/0.0). After adding the items to the
    // drawing scene they will be moved by "setPos" relative within the group.
    // The group itself will be positioned by the caller of the ctor relative to
    // the caller's coordinate system.

    QRectF rctBounding(QPointF(50.0, 50.0), c_sizInitial);
    double fBodyWidth = rctBounding.width() / 3.0;
    QRectF rctBody(rctBounding.center().x() - fBodyWidth/2.0, rctBounding.top(), fBodyWidth, rctBounding.height());
    CPhysValRect physValRectBody(*m_pDrawingScene, rctBody.topLeft(), rctBody.size());
    CPhysValPoint physValPointCnctPt1(*m_pDrawingScene, QPointF(rctBounding.left(), rctBounding.center().y()));
    CPhysValPoint physValPointCnctPt2(*m_pDrawingScene, QPointF(rctBounding.right(), rctBounding.center().y()));

    QList<CGraphObj*> arpGraphObjs;

    CObjFactory* pObjFactoryRect =
        CObjFactory::FindObjFactory(CObjFactory::c_strGroupNameStandardShapes, EGraphObjTypeRect);
    CObjFactory* pObjFactoryCnctPt =
        CObjFactory::FindObjFactory(CObjFactory::c_strGroupNameConnections, EGraphObjTypeConnectionPoint);
    CObjFactory* pObjFactoryCnctLine =
        CObjFactory::FindObjFactory(CObjFactory::c_strGroupNameConnections, EGraphObjTypeConnectionLine);

    m_drawSettings.setLineStyle(ELineStyle::NoLine);

    m_pGraphObjRectBody = dynamic_cast<CGraphObjRect*>(
        pObjFactoryRect->createGraphObj(m_pDrawingScene, physValRectBody.topLeft()));
    m_pDrawingScene->addGraphObj(m_pGraphObjRectBody);
    m_pGraphObjRectBody->rename("Body");
    m_pGraphObjRectBody->setRect(physValRectBody);
    QString strCnctPtNameRectBodyLeftCenter =
        m_pGraphObjRectBody->addConnectionPoint(ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter);
    m_pGraphObjCnctPtRectBodyLeftCenter = m_pGraphObjRectBody->getConnectionPoint(strCnctPtNameRectBodyLeftCenter);
    m_pGraphObjCnctPtRectBodyLeftCenter->setFixedSize(CPhysValSize(*m_pDrawingScene, QSizeF(1.0, 1.0), Units.Length.px));
    //QString strCnctPtNameRectBodyRightCenter =
    //    m_pGraphObjRectBody->addConnectionPoint(ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter);
    //m_pGraphObjCnctPtRectBodyRightCenter = m_pGraphObjRectBody->getConnectionPoint(strCnctPtNameRectBodyRightCenter);
    //m_pGraphObjCnctPtRectBodyRightCenter->setFixedSize(CPhysValSize(*m_pDrawingScene, QSizeF(1.0, 1.0), Units.Length.px));
    arpGraphObjs.append(m_pGraphObjRectBody);

    //m_pGraphObjCnctPt1 = dynamic_cast<CGraphObjConnectionPoint*>(
    //    pObjFactoryCnctPt->createGraphObj(m_pDrawingScene, physValPointCnctPt1));
    //m_pDrawingScene->addGraphObj(m_pGraphObjCnctPt1);
    //m_pGraphObjCnctPt1->rename("CnctPt1");
    //m_pGraphObjCnctPt1->setPosition(physValPointCnctPt1);
    //arpGraphObjs.append(m_pGraphObjCnctPt1);

    //m_pGraphObjCnctPt2 = dynamic_cast<CGraphObjConnectionPoint*>(
    //    pObjFactoryCnctPt->createGraphObj(m_pDrawingScene, physValPointCnctPt2));
    //m_pDrawingScene->addGraphObj(m_pGraphObjCnctPt2);
    //m_pGraphObjCnctPt2->rename("CnctPt2");
    //m_pGraphObjCnctPt2->setPosition(physValPointCnctPt2);
    //arpGraphObjs.append(m_pGraphObjCnctPt2);

    //m_pGraphObjCnctLine1 = new CGraphObjConnectionLine(m_pDrawingScene, "CnctLine1");
    //m_pDrawingScene->addGraphObj(m_pGraphObjCnctLine1);
    //m_pGraphObjCnctLine1->setConnectionPoint(ELinePoint::Start, m_pGraphObjCnctPt1);
    //m_pGraphObjCnctLine1->setConnectionPoint(ELinePoint::End, m_pGraphObjCnctPtRectBodyLeftCenter);

    //m_pGraphObjCnctLine2 = new CGraphObjConnectionLine(m_pDrawingScene, "CnctLine2");
    //m_pDrawingScene->addGraphObj(m_pGraphObjCnctLine2);
    //m_pGraphObjCnctLine2->setConnectionPoint(ELinePoint::Start, m_pGraphObjCnctPtRectBodyRightCenter);
    //m_pGraphObjCnctLine2->setConnectionPoint(ELinePoint::End, m_pGraphObjCnctPt2);

    // Create resistor as object group
    //--------------------------------

    //addToGroup(arpGraphObjs);

    // The alignments will be adjusted in the order they are added. The order
    // takes effect on the result. Usually the size should be adjusted before
    // the positions to get relative adjustments working as expected.

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
}
