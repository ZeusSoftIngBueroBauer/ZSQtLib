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

#ifndef ZSDraw_ObjFactoryConnectionLine_h
#define ZSDraw_ObjFactoryConnectionLine_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CObjFactoryConnectionLine : public CObjFactory
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CObjFactoryConnectionLine"; }
public: // ctors and dtor
    CObjFactoryConnectionLine( const QPixmap& i_pxmToolIcon = QPixmap() );
    virtual ~CObjFactoryConnectionLine();
public: // interface methods of base class CObjFactory
    virtual CGraphObj* createGraphObj(
        CDrawingScene* i_pDrawingScene,
        const CPhysValPoint& i_physValPoint,
        const CDrawSettings& i_drawSettings ) override;
    virtual ZS::System::SErrResultInfo saveGraphObj(
        CGraphObj* i_pGraphObj,
        QXmlStreamWriter& i_xmlStreamWriter ) override;
    virtual CGraphObj* loadGraphObj(
        CDrawingScene* i_pDrawingScene,
        CGraphObjGroup* i_pGraphObjGroup,
        const QString& i_strObjName,
        QXmlStreamReader& i_xmlStreamReader ) override;

}; // class CObjFactoryConnectionLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_ObjFactoryConnectionLine_h
