/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_GraphObjElectricity_h
#define ZSDraw_GraphObjElectricity_h

#include <QtCore/qglobal.h>

#if QT_VERSION >= 0x040000 && QT_VERSION < 0x050000
#include <QtGui/QGraphicsRectItem>
#elif QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#include <QtWidgets/QGraphicsRectItem>
#endif

#include "ZSDraw/ZSDrawGraphObjGroup.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawSettings;
class CGraphObjConnectionPoint;
class CGraphObjEllipse;
class CGraphObjImage;
class CGraphObjLine;
class CGraphObjPolygon;
class CGraphObjPolyline;
class CGraphObjRect;
class CGraphObjSelectionPoint;

namespace Electricity
{
//******************************************************************************
class CGraphObjElectricity : public CGraphObjGroup
//******************************************************************************
{
protected: // ctor
    CGraphObjElectricity(
        CDrawingScene*       i_pDrawingScene,
        const QString&       i_strNameSpace,
        const QString&       i_strClassName,
        const QString&       i_strType,
        const QString&       i_strObjName,
        const QString&       i_strObjId,
        const CDrawSettings& i_drawSettings );
public: // dtor
    virtual ~CGraphObjElectricity();
public: // overridables of base class QGraphicsItem
    virtual int type() const { return EGraphObjTypeUserDefined; }
public: // overridables of base class CGraphObj
    virtual void setObjId( const QString& i_strObjId );

}; // class CGraphObjElectricity

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjElectricity_h
