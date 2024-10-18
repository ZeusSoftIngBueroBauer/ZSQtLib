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
    m_bYAxisTopDown(i_drawingScene.drawingSize().yScaleAxisOrientation().enumerator() == EYScaleAxisOrientation::TopDown),
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
    m_bYAxisTopDown(!Units.Length.isMetricUnit(i_unit) || i_drawingScene.drawingSize().yScaleAxisOrientation().enumerator() == EYScaleAxisOrientation::TopDown),
    m_unit(i_unit),
    m_idxInDrawingScene(-1)
{
}

//------------------------------------------------------------------------------
CPhysValShape::CPhysValShape(const CPhysValShape& i_physValShapeOther) :
//------------------------------------------------------------------------------
    m_pDrawingScene(i_physValShapeOther.m_pDrawingScene),
    m_bYAxisTopDown(i_physValShapeOther.m_bYAxisTopDown),
    m_unit(i_physValShapeOther.m_unit),
    m_idxInDrawingScene(-1)
{
}

//------------------------------------------------------------------------------
CPhysValShape::~CPhysValShape()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    m_bYAxisTopDown = false;
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
    m_bYAxisTopDown = i_physValShapeOther.m_bYAxisTopDown;
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
    else if (m_bYAxisTopDown != i_physValShapeOther.m_bYAxisTopDown) {
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
    return m_bYAxisTopDown ? EYScaleAxisOrientation::TopDown : EYScaleAxisOrientation::BottomUp;
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of the rectangle coordinates.

    The resolution will be retrieved from the drawing scene's drawing size and
    depends on the current unit.
*/
double CPhysValShape::resolution() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit).getVal();
}

//------------------------------------------------------------------------------
/*! @brief Returns the current unit the coordinates are stored.
*/
CUnit CPhysValShape::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

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
