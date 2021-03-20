/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSRemCmd_ObjPool_h
#define ZSRemCmd_ObjPool_h

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSSys/ZSSysObjPool.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;

} // namespace Trace

namespace RemCmd
{
class CRemCmdObj;

//******************************************************************************
class ZSREMCMDDLL_API CRemCmdObjPool : public ZS::System::CObjPool
//******************************************************************************
{
public: // ctors and dtor
    CRemCmdObjPool( const QString& i_strObjName, char i_chNodeSeparator = ':' );
    virtual ~CRemCmdObjPool();
public: // overridables of base class CModelObjPool
    virtual void clear(); // clears the whole tree and the list and destroys the command objects
protected: // overridables of base class CModelObjPool
    virtual void clearTreeEntry( ZS::System::CObjPoolTreeEntry* i_pTreeEntry ); // clears the sub tree and destroys the command objects
public: // instance methods
    QString toXmlString() const;
protected: // instance methods to send objects to clients connected at the control channel
    QString toXmlString( ZS::System::CObjPoolTreeEntry* i_pTreeEntry, QString& i_strXml ) const;
protected: // instance members
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CRemCmdObjPool

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmd_ObjPool_h
