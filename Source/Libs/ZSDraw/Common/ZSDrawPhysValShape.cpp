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

#include "ZSDraw/Common/ZSDrawPhysValShape.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::System;


/*******************************************************************************
class CPhysValShape
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical shape on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValShape::CPhysValShape(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_pDivLinesMetricsX(&i_drawingScene.divLinesMetricsX()),
    m_pDivLinesMetricsY(&i_drawingScene.divLinesMetricsY()),
    m_unit(i_drawingScene.drawingSize().unit()),
    m_idxInDrawingScene(-1)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical shape on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValShape::CPhysValShape(const CDrawingScene& i_drawingScene, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_pDivLinesMetricsX(&i_drawingScene.divLinesMetricsX()),
    m_pDivLinesMetricsY(&i_drawingScene.divLinesMetricsY()),
    m_unit(i_unit),
    m_idxInDrawingScene(-1)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical line on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValShape::CPhysValShape(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY ) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_pDivLinesMetricsX(&i_divLinesMetricsX),
    m_pDivLinesMetricsY(&i_divLinesMetricsY),
    m_unit(i_drawingScene.drawingSize().unit()),
    m_idxInDrawingScene(-1)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical line on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValShape::CPhysValShape(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_pDivLinesMetricsX(&i_divLinesMetricsX),
    m_pDivLinesMetricsY(&i_divLinesMetricsY),
    m_unit(i_unit),
    m_idxInDrawingScene(-1)
{
}

//------------------------------------------------------------------------------
CPhysValShape::CPhysValShape(const CPhysValShape& i_physValShapeOther) :
//------------------------------------------------------------------------------
    m_pDrawingScene(i_physValShapeOther.m_pDrawingScene),
    m_pDivLinesMetricsX(i_physValShapeOther.m_pDivLinesMetricsX),
    m_pDivLinesMetricsY(i_physValShapeOther.m_pDivLinesMetricsY),
    m_unit(i_physValShapeOther.m_unit),
    m_idxInDrawingScene(-1)
{
}

//------------------------------------------------------------------------------
CPhysValShape::~CPhysValShape()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    m_pDivLinesMetricsX = nullptr;
    m_pDivLinesMetricsY = nullptr;
    //m_unit;
    m_idxInDrawingScene = 0;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValShape::operator = ( const CPhysValShape& i_physValShapeOther )
//------------------------------------------------------------------------------
{
    m_pDrawingScene = i_physValShapeOther.m_pDrawingScene;
    m_pDivLinesMetricsX = i_physValShapeOther.m_pDivLinesMetricsX;
    m_pDivLinesMetricsY = i_physValShapeOther.m_pDivLinesMetricsY;
    m_unit = i_physValShapeOther.m_unit;
    return *this;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValShape::operator == ( const CPhysValShape& i_physValShapeOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (!areOfSameUnitGroup(m_unit, i_physValShapeOther.m_unit)) {
        bEqual = false;
    }
    else if (m_pDrawingScene != i_physValShapeOther.m_pDrawingScene) {
        bEqual = false;
    }
    else if (m_pDivLinesMetricsX != i_physValShapeOther.m_pDivLinesMetricsX) {
        bEqual = false;
    }
    else if (m_pDivLinesMetricsY != i_physValShapeOther.m_pDivLinesMetricsY) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValShape::operator != ( const CPhysValShape& i_physValShapeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValShapeOther);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
EYScaleAxisOrientation CPhysValShape::yScaleAxisOrientation() const
//------------------------------------------------------------------------------
{
    return m_pDivLinesMetricsY->yScaleAxisOrientation();
}

//------------------------------------------------------------------------------
void CPhysValShape::setIndexInDrawingScene(int i_idx)
//------------------------------------------------------------------------------
{
    m_idxInDrawingScene = i_idx;
}

//------------------------------------------------------------------------------
int CPhysValShape::indexInDrawingScene() const
//------------------------------------------------------------------------------
{
    return m_idxInDrawingScene;
}
