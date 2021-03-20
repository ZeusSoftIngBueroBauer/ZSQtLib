/*******************************************************************************

Copyright 2004 - 2014 by    ZeusSoft, Ing. Buero Bauer
                            Gewerbepark 28
                            D-83670 Bad Heilbrunn
                            Tel: 0049 8046 9488
                            www.zeussoft.de
                            E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifndef ZSDraw_ObjFactoriesPool_h
#define ZSDraw_ObjFactoriesPool_h

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSSys/ZSSysObjPool.h"

namespace ZS
{
namespace Draw
{
class CObjFactory;

//******************************************************************************
class ZSDRAWDLL_API CObjFactoriesPool : public ZS::CObjPool
//******************************************************************************
{
public: // ctors and dtor
    CObjFactoriesPool();
    virtual ~CObjFactoriesPool();
public: // instance methods
    CObjFactory* findObjFactory(
        int            i_iGraphObjType,
        const QString& i_strGraphObjNameSpace = "",
        const QString& i_strGraphObjClassName = "" );
    CObjFactory* getObjFactory( int i_iObjId );

}; // class CObjFactoriesPool

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_ObjFactoriesPool_h
