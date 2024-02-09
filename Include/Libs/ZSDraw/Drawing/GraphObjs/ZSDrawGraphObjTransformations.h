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

#ifndef ZSDraw_GraphObjTransformations_h
#define ZSDraw_GraphObjTransformations_h

#include "ZSDraw/Common/ZSDrawDllMain.h"

#include <QtGui/qtransform.h>

#error "Module very likely superfluous"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CGraphObjTransformations
//******************************************************************************
{
public: // ctors and dtor
    CGraphObjTransformations();
    ~CGraphObjTransformations();
public: // instance methods
    void reset();
    void translate(const QVariant& i_translateDX, const QVariant& i_translateDY);
    void scale(const QVariant& i_scaleFactorWidth, const QVariant& i_scaleFactorHeight);
    void shear(const QVariant& i_shearHorizontal, const QVariant& i_shearVertical);
    void rotate(double i_fRotationAngle_degrees);
public: // instance methods
    QPointF map(const QPointF& i_pt) const;
    QLineF map(const QLineF& i_line) const;
    QRectF map(const QRectF& i_rect) const;
private: // instance members
    double m_fTranslateDX;
    double m_fTranslateDY;
    double m_fScaleFactorWidth;
    double m_fScaleFactorHeight;
    double m_fShearHorizontal;
    double m_fShearVertical;
    double m_fRotationAngle_degrees;
    QTransform m_transform;

}; // class CGraphObjTransformations

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjTransformations_h
