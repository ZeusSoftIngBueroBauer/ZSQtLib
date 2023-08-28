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

#ifndef ZSDraw_ObjFactoryElectricityResistor_h
#define ZSDraw_ObjFactoryElectricityResistor_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactory.h"

namespace ZS
{
namespace Draw
{
namespace Electricity
{
//******************************************************************************
class CObjFactoryResistor : public CObjFactory
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to.
        Please note that the static class functions name must be different
        from the non static virtual member function "nameSpace" */
    static QString NameSpace() { return "ZS::Draw::Electricity"; }
    /*! Returns the class name.
        Please note that the static class functions name must be different
        from the non static virtual member function "nameSpace" */
    static QString ClassName() { return "CObjFactoryResistor"; }
public: // ctors and dtor
    CObjFactoryResistor( const QPixmap& i_pxmToolIcon = QPixmap() );
    virtual ~CObjFactoryResistor();
public: // interface methods of base class CObjFactory
    virtual CGraphObj* createGraphObj(
        CDrawingScene* i_pDrawingScene,
        const QPointF& i_ptItemPos,
        const CDrawSettings& i_drawSettings ) override;
    virtual ZS::System::SErrResultInfo saveGraphObj(
        CGraphObj* i_pGraphObj,
        QXmlStreamWriter& i_xmlStreamWriter ) override;
    virtual CGraphObj* loadGraphObj(
        CDrawingScene* i_pDrawingScene,
        CGraphObjGroup* i_pGraphObjGroup,
        const QString& i_strObjName,
        QXmlStreamReader& i_xmlStreamReader ) override;

}; // class CObjFactoryResistor

} // namespace Electricity

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_ObjFactoryElectricityResistor_h
