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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
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

    createTraceAdminObjs("ZS::Draw::Drawing::GraphObjs::Plugins::Electricity", ClassName());

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

    #pragma message(__TODO__"Don't use connection lines")
    // Stattdessen eine durchgezogene Linie. Und zwei connection points an der Gruppe Left und RightCenter.
    /*
                          Body
                       +--------+
                 Line1 |        | Line2
    CnctPt1 X----------X        X----------X CnctPt2
                       |        |
                       +--------+
    */


    // Before adding the items to the drawing scene they will be created in their
    // own coordinate system starting at (0.0/0.0). After adding the items to the
    // drawing scene they will be moved by "setPos" relative within the group.
    // The group itself will be positioned by the caller of the ctor relative to
    // the caller's coordinate system.

    QRectF rctBounding(QPointF(0.0, 0.0), QSizeF(60.0, 10.0));
    CPhysValRect physValRect(*m_pDrawingScene, rctBounding.topLeft(), rctBounding.size());
    double fBodyWidth = rctBounding.width() / 3.0;
    QRectF rctBody(rctBounding.center().x() - fBodyWidth/2.0, rctBounding.top(), fBodyWidth, rctBounding.height());
    CPhysValRect physValRectBody(*m_pDrawingScene, rctBody.topLeft(), rctBody.size());

    // The group will be added to the drawing scene at position (0, 0) right after creating the group.
    // By adding child objects to the group the group will map the shape point coordinates of the child
    // to the group coordinates and will try to resize the group so that the newly added object fits into
    // the group. In order for the group to map the coordinates of the new child object, the group must
    // already have gotten its final size.
    setRect(physValRect);

    // Also a unique name must be assigned to the group.
    QString strEntryType = entryType2Str(EEnumEntryAliasStrSymbol);
    QString strObjKey = i_pDrawingScene->getGraphObjsIdxTree()->buildKeyInTreeStr(strEntryType, m_strName);
    CGraphObj* pGraphObjWithSameKey = i_pDrawingScene->getGraphObj(strObjKey);
    if ((pGraphObjWithSameKey != nullptr) && (pGraphObjWithSameKey != this)) {
        QString strObjName = i_pDrawingScene->findUniqueGraphObjName(this);
        setName(strObjName);
    }
    m_pDrawingScene->addGraphObj(this);

    QList<CGraphObj*> arpGraphObjs;

    CObjFactory* pObjFactoryRect =
        CObjFactory::findObjFactory(CObjFactory::c_strGroupNameStandardShapes, EGraphObjTypeRect);
    CObjFactory* pObjFactoryLine =
        CObjFactory::findObjFactory(CObjFactory::c_strGroupNameStandardShapes, EGraphObjTypeLine);

    m_drawSettings.setLineStyle(ELineStyle::NoLine);

    m_pGraphObjRectBody = dynamic_cast<CGraphObjRect*>(
        pObjFactoryRect->createGraphObj(m_pDrawingScene, physValRectBody.topLeft()));
    m_pGraphObjRectBody->setName("Body");
    m_pDrawingScene->addGraphObj(m_pGraphObjRectBody, this);
    m_pGraphObjRectBody->setRect(physValRectBody);

    m_pGraphObjLine1 = dynamic_cast<CGraphObjLine*>(pObjFactoryLine->createGraphObj(m_pDrawingScene));
    m_pGraphObjLine1->setName("WireP1");
    m_pDrawingScene->addGraphObj(m_pGraphObjLine1, this);
    m_pGraphObjLine1->setLine(CPhysValLine(physValRect.leftCenter(), physValRectBody.leftCenter()));
    m_pGraphObjLine2 = dynamic_cast<CGraphObjLine*>(pObjFactoryLine->createGraphObj(m_pDrawingScene));
    m_pGraphObjLine2->setName("WireP2");
    m_pDrawingScene->addGraphObj(m_pGraphObjLine2, this);
    m_pGraphObjLine2->setLine(CPhysValLine(physValRect.rightCenter(), physValRectBody.rightCenter()));

    QString strGraphObjNameCnctPt1 = addConnectionPoint(ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter);
    m_pGraphObjCnctPt1 = getConnectionPoint(strGraphObjNameCnctPt1);
    m_pGraphObjCnctPt1->setName("P1");
    m_pGraphObjCnctPt1->setFixedSize(CPhysValSize(*m_pDrawingScene, QSizeF(1.0, 1.0), Units.Length.px));
    QString strGraphObjNameCnctPt2 = addConnectionPoint(ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter);
    m_pGraphObjCnctPt2 = getConnectionPoint(strGraphObjNameCnctPt2);
    m_pGraphObjCnctPt2->setName("P2");
    m_pGraphObjCnctPt2->setFixedSize(CPhysValSize(*m_pDrawingScene, QSizeF(1.0, 1.0), Units.Length.px));
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
