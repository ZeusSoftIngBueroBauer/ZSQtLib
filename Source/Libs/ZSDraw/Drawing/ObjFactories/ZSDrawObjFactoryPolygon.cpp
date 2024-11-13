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

#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryPolygon.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryPolyline.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
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
class CObjFactoryPolygon : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryPolygon::CObjFactoryPolygon( const QPixmap& i_pxmToolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ c_strGroupNameStandardShapes,
        /* strClassName    */ ClassName(),
        /* iGraphObjType   */ EGraphObjTypePolygon,
        /* strGraphObjType */ ZS::Draw::graphObjType2Str(EGraphObjTypePolygon),
        /* toolIcon        */ i_pxmToolIcon )
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
CObjFactoryPolygon::~CObjFactoryPolygon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );
}

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryPolygon::createGraphObj(
    CDrawingScene* i_pDrawingScene, const CDrawSettings& i_drawSettings)
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

    CDrawSettings drawSettings = i_drawSettings;
    drawSettings.setGraphObjType(EGraphObjTypePolygon);
    CGraphObjPolygon* pGraphObj = new CGraphObjPolygon(i_pDrawingScene);
    pGraphObj->setDrawSettings(drawSettings);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(pGraphObj->path());
    }
    return pGraphObj;
}

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryPolygon::createGraphObj(
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
    drawSettings.setGraphObjType(EGraphObjTypePolygon);
    CGraphObjPolygon* pGraphObj = new CGraphObjPolygon(i_pDrawingScene);
    pGraphObj->setDrawSettings(drawSettings);

    QPolygonF polygon({i_physValPoint.toQPointF()});
    CPhysValPolygon physValPolygon(*i_pDrawingScene, polygon, i_physValPoint.unit());
    pGraphObj->setPolygon(physValPolygon);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(pGraphObj->path());
    }
    return pGraphObj;
}

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryPolygon::saveGraphObj(
    CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter) const
//------------------------------------------------------------------------------
{
    if( i_pGraphObj == nullptr )
    {
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

    SErrResultInfo errResultInfo = CObjFactoryPolyline::saveGraphObj(i_pGraphObj, EGraphObjTypePolygon, i_xmlStreamWriter);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;
}

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryPolygon::loadGraphObj(
    CDrawingScene* i_pDrawingScene,
    CGraphObjGroup* i_pGraphObjGroupParent,
    const QString& i_strObjName,
    QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    if (i_pDrawingScene == nullptr) {
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

    CGraphObj* pGraphObj = CObjFactoryPolyline::loadGraphObj(
        i_pDrawingScene, i_pGraphObjGroupParent, i_strObjName, EGraphObjTypePolygon, i_xmlStreamReader);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(i_xmlStreamReader.errorString());
        QString strMthRet = QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
        mthTracer.setMethodReturn(strMthRet);
    }
    return pGraphObj;

} // loadGraphObj
