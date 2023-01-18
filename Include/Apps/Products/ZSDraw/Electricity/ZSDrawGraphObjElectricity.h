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

#ifndef ZSDraw_GraphObjElectricity_h
#define ZSDraw_GraphObjElectricity_h

#include <QtCore/qglobal.h>

#if QT_VERSION >= 0x040000 && QT_VERSION < 0x050000
#include <QtGui/QGraphicsRectItem>
#elif QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#include <QtWidgets/QGraphicsRectItem>
#endif

#include "ZSDraw/GraphObjs/ZSDrawGraphObjGroup.h"

namespace ZS
{
namespace Draw
{
namespace Electricity
{
//******************************************************************************
class CGraphObjElectricity : public CGraphObjGroup
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw::Electricity"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjElectricity"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // class members
    /*! Group name of the factories to create the standard shapes (Line, Point, Rect, etc).
        The group name together with the type as string of the graphical objects must be unique
        and is used to add the object factory to the index tree of all registered factories.
        Defined here as there is no base class for the widget factories. */
    static const QString c_strFactoryGroupName; // = "Electricity"
protected: // ctor
    CGraphObjElectricity(
        CDrawingScene*       i_pDrawingScene,
        const QString&       i_strType,
        const QString&       i_strObjName,
        const CDrawSettings& i_drawSettings );
public: // dtor
    virtual ~CGraphObjElectricity();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // overridables of base class QGraphicsItem
    virtual int type() const { return EGraphObjTypeUserDefined; }
public: // overridables of base class CGraphObj
    //virtual void setObjId( const QString& i_strObjId );

}; // class CGraphObjElectricity

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjElectricity_h
