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

#ifndef ZSDraw_ObjFactoryConnectionPoint_h
#define ZSDraw_ObjFactoryConnectionPoint_h

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawObjFactory.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CObjFactoryConnectionPoint : public CObjFactory
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CObjFactoryConnectionPoint"; }  // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // ctors and dtor
    CObjFactoryConnectionPoint( const QPixmap& i_pxmToolIcon = QPixmap() );
    virtual ~CObjFactoryConnectionPoint();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // interface methods of base class CObjFactory
    virtual CGraphObj* createGraphObj(
        CDrawingScene*       i_pDrawingScene,
        const QPointF&       i_ptItemPos, // usually QPointF(0.0,0.0)
        const CDrawSettings& i_drawSettings );
    virtual ZS::System::SErrResultInfo saveGraphObj(
        CGraphObj*        i_pGraphObj,
        QXmlStreamWriter& i_xmlStreamWriter );
    virtual CGraphObj* loadGraphObj(
        CDrawingScene*    i_pDrawingScene,
        CGraphObjGroup*   i_pGraphObjGroup,
        const QString&    i_strObjName,
        const QString&    i_strObjId,
        QXmlStreamReader& i_xmlStreamReader,
        ZS::System::SErrResultInfo&   io_errResultInfo );

}; // class CObjFactoryConnectionPoint

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_ObjFactoryConnectionPoint_h
