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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjTransformations.h"

#include <QtCore/qvariant.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Draw;

#error "Module very likely superfluous"

/*******************************************************************************
class CGraphObjTransformations
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjTransformations::CGraphObjTransformations() :
//------------------------------------------------------------------------------
    m_fTranslateDX(0.0),
    m_fTranslateDY(0.0),
    m_fScaleFactorWidth(1.0),
    m_fScaleFactorHeight(1.0),
    m_fShearHorizontal(0.0),
    m_fShearVertical(0.0),
    m_fRotationAngle_degrees(0.0),
    m_transform()
{
}

////------------------------------------------------------------------------------
//CGraphObjTransformations::CGraphObjTransformations(
//    const QVariant& i_translateDX, const QVariant& i_translateDY,
//    const QVariant& i_scaleFactorWidth, const QVariant& i_scaleFactorHeight,
//    const QVariant& i_shearHorizontal, const QVariant& i_shearVertical,
//    const QVariant& i_rotationAngle_degrees) :
////------------------------------------------------------------------------------
//    m_fTranslateDX(i_translateDX.isNull() ? 0.0 : i_translateDX.toDouble()),
//    m_fTranslateDY(i_translateDY.isNull() ? 0.0 : i_translateDY.toDouble()),
//    m_fScaleFactorWidth(i_scaleFactorWidth.isNull() ? 1.0 : i_scaleFactorWidth.toDouble()),
//    m_fScaleFactorHeight(i_scaleFactorHeight.isNull() ? 1.0 : i_scaleFactorHeight.toDouble()),
//    m_fShearHorizontal(i_shearHorizontal.isNull() ? 0.0 : i_shearHorizontal.toDouble()),
//    m_fShearVertical(i_shearVertical.isNull() ? 0.0 : i_shearVertical.toDouble()),
//    m_fRotationAngle_degrees(i_rotationAngle_degrees.isNull() ? 0.0 : i_rotationAngle_degrees.toDouble()),
//    m_transform()
//{
//    m_transform.translate(m_fTranslateDX, m_fTranslateDY);
//    m_transform.scale(m_fScaleFactorWidth, m_fScaleFactorHeight);
//    m_transform.shear(m_fShearHorizontal, m_fShearVertical);
//    m_transform.rotate(m_fRotationAngle_degrees);
//}

//------------------------------------------------------------------------------
CGraphObjTransformations::~CGraphObjTransformations()
//------------------------------------------------------------------------------
{
    m_fTranslateDX = 0.0;
    m_fTranslateDY = 0.0;
    m_fScaleFactorWidth = 0.0;
    m_fScaleFactorHeight = 0.0;
    m_fShearHorizontal = 0.0;
    m_fShearVertical = 0.0;
    m_fRotationAngle_degrees = 0.0;
    //m_transform;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjTransformations::reset()
//------------------------------------------------------------------------------
{
    m_fTranslateDX = 0.0;
    m_fTranslateDY = 0.0;
    m_fScaleFactorWidth = 1.0;
    m_fScaleFactorHeight = 1.0;
    m_fShearHorizontal = 0.0;
    m_fShearVertical = 0.0;
    m_fRotationAngle_degrees = 0.0;
    m_transform.reset();
}

//------------------------------------------------------------------------------
void CGraphObjTransformations::translate(const QVariant& i_translateDX, const QVariant& i_translateDY)
//------------------------------------------------------------------------------
{
    if (!i_translateDX.isNull()) {
        m_fTranslateDX = i_translateDX.toDouble();
    }
    if (!i_translateDY.isNull()) {
        m_fTranslateDY = i_translateDY.toDouble();
    }
    m_transform.translate(m_fTranslateDX, m_fTranslateDY);
}

//------------------------------------------------------------------------------
void CGraphObjTransformations::scale(const QVariant& i_scaleFactorWidth, const QVariant& i_scaleFactorHeight)
//------------------------------------------------------------------------------
{
    if (!i_scaleFactorWidth.isNull()) {
        m_fScaleFactorWidth = i_scaleFactorWidth.toDouble();
    }
    if (!i_scaleFactorHeight.isNull()) {
        m_fScaleFactorHeight = i_scaleFactorHeight.toDouble();
    }
    m_transform.scale(m_fScaleFactorWidth, m_fScaleFactorHeight);
}

//------------------------------------------------------------------------------
void CGraphObjTransformations::shear(const QVariant& i_shearHorizontal, const QVariant& i_shearVertical)
//------------------------------------------------------------------------------
{
    if (!i_shearHorizontal.isNull()) {
        m_fShearHorizontal = i_shearHorizontal.toDouble();
    }
    if (!i_shearVertical.isNull()) {
        m_fShearVertical = i_shearVertical.toDouble();
    }
    m_transform.shear(m_fShearHorizontal, m_fShearVertical);
}

//------------------------------------------------------------------------------
void CGraphObjTransformations::rotate(double i_fTranslateDX)
//------------------------------------------------------------------------------
{
    m_fRotationAngle_degrees = i_fTranslateDX;
    m_transform.rotate(m_fRotationAngle_degrees);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QPointF CGraphObjTransformations::map(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    return m_transform.map(i_pt);
}

//------------------------------------------------------------------------------
QLineF CGraphObjTransformations::map(const QLineF& i_line) const
//------------------------------------------------------------------------------
{
    return m_transform.map(i_line);
}

//------------------------------------------------------------------------------
QRectF CGraphObjTransformations::map(const QRectF& i_rect) const
//------------------------------------------------------------------------------
{
    return m_transform.mapRect(i_rect);
}
