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

#ifndef ZSRemCmd_SubSysNode_h
#define ZSRemCmd_SubSysNode_h

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysRequestQueue.h"

namespace ZS
{
namespace System
{
class CObjPoolTreeEntry;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace RemCmd
{
class CRemCmdObjPool;
class CRemCmdServer;


//******************************************************************************
class ZSREMCMDDLL_API CRemCmdSubSysNode : public QObject
//******************************************************************************
{
friend class CRemCmdObjPool;
    Q_OBJECT
public: // ctors and dtor
    CRemCmdSubSysNode( CRemCmdServer* i_pServer, const QString& i_strNode );
    CRemCmdSubSysNode( CRemCmdObjPool* i_pObjPool, const QString&  i_strNode );
    virtual ~CRemCmdSubSysNode();
signals:
    void changed( QObject* i_pRemCmdSubSysNode );
public: // instance methods
    QString getNodeStr() const { return m_strNode; }
public: // instance methods
    QString getDescription() const { return m_strDscr; }
    void setDescription( const QString& i_strDscr );
public: // instance methods
    QString toXmlString() const;
public: // instance methods (experts use only)
    ZS::System::CObjPoolTreeEntry* getTreeEntry() { return m_pTreeEntry; }
protected: // instance members
    CRemCmdServer*                 m_pServer;
    CRemCmdObjPool*                m_pObjPool;
    QString                        m_strNode;
    QString                        m_strDscr;
    ZS::System::CObjPoolTreeEntry* m_pTreeEntry;
    Trace::CTrcAdminObj*           m_pTrcAdminObj;

}; // class CRemCmdSubSysNode

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmd_SubSysNode_h

