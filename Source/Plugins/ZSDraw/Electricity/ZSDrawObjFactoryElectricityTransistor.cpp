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

#include "Electricity/ZSDrawObjFactoryElectricityTransistor.h"
#include "Electricity/ZSDrawGraphObjElectricityTransistor.h"

#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Plugins::Electricity;


/*******************************************************************************
class CObjFactoryTransistor : public CObjFactoryElectricity
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryTransistor::CObjFactoryTransistor( const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    CObjFactoryElectricity(ClassName(), "Transistor", i_pxmToolIcon)
{
}

//------------------------------------------------------------------------------
CObjFactoryTransistor::~CObjFactoryTransistor()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
int CObjFactoryTransistor::getNumberOfCreatedGraphObjects() const
//------------------------------------------------------------------------------
{
    return CGraphObjTransistor::s_iInstCount;
}

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryTransistor::createGraphObj(
    CDrawingScene* i_pDrawingScene,
    const CDrawSettings& i_drawSettings )
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
    drawSettings.setGraphObjType(EGraphObjTypeGroup);
    CGraphObj* pGraphObj = new CGraphObjTransistor(i_pDrawingScene);
    QString strObjKey = pGraphObj->keyInTree();
    if (i_pDrawingScene->getGraphObj(strObjKey) != nullptr) {
        QString strObjName = i_pDrawingScene->findUniqueGraphObjName(pGraphObj);
        pGraphObj->setName(strObjName);
    }
    // Don't set the passed draw settings. The plugin object uses its own settings.
    //pGraphObj->setDrawSettings(drawSettings);
    return pGraphObj;
}

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryTransistor::createGraphObj(
    CDrawingScene* i_pDrawingScene,
    const CPhysValPoint& i_physValPoint,
    const CDrawSettings& i_drawSettings )
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
    CGraphObj* pGraphObj = new CGraphObjTransistor(i_pDrawingScene);
    QString strObjKey = pGraphObj->keyInTree();
    if (i_pDrawingScene->getGraphObj(strObjKey) != nullptr) {
        QString strObjName = i_pDrawingScene->findUniqueGraphObjName(pGraphObj);
        pGraphObj->setName(strObjName);
    }
    // Don't set the passed draw settings. The plugin object uses its own settings.
    //pGraphObj->setDrawSettings(drawSettings);
    return pGraphObj;
}
