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

#ifndef ZSSys_TrcAdminObjIdxTree_h
#define ZSSys_TrcAdminObjIdxTree_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

class QXmlStreamWriter;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
class CTrcServer;

//******************************************************************************
class ZSSYSDLL_API CIdxTreeTrcAdminObjs : public ZS::System::CIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Trace"; }
    static QString ClassName() { return "CIdxTreeTrcAdminObjs"; }
public: // ctors and dtor
    CIdxTreeTrcAdminObjs(
        const QString& i_strObjName,
        QObject*       i_pObjParent,
        int            i_iTrcDetailLevel = ETraceDetailLevelNone );
    virtual ~CIdxTreeTrcAdminObjs();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods to get and release admin objects
    CTrcAdminObj* getTraceAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        bool           i_bIncrementRefCount = true );
    CTrcAdminObj* getTraceAdminObj( int i_idxInTree ) const;
    ZS::System::CBranchIdxTreeEntry* getBranch( int i_idxInTree ) const;
    void releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj );
public: // instance methods to insert branch nodes and admin objects
    ZS::System::CBranchIdxTreeEntry* insertBranch(
        int            i_iParentBranchIdxInTree,
        const QString& i_strBranchName,
        int            i_idxInTree );
    CTrcAdminObj* insertTraceAdminObj(
        int            i_iParentBranchIdxInTree,
        const QString& i_strObjName,
        int            i_idxInTree );
public: // instance methods to recursively modify admin objects via object index of node entries
    void setEnabled( int i_idxInTree, ZS::System::EEnabled i_enabled );
    void setTraceDetailLevel( int i_idxInTree, int i_iDetailLevel = -1 );
public: // instance methods to recursively modify admin objects via namespace node entries
    void setEnabled( ZS::System::CBranchIdxTreeEntry* i_pBranch, ZS::System::EEnabled i_enabled );
    void setTraceDetailLevel( ZS::System::CBranchIdxTreeEntry* i_pBranch, int i_iDetailLevel = -1 );
public: // overridables
    virtual ZS::System::SErrResultInfo save( const QString& i_strAbsFilePath ) const;
    virtual ZS::System::SErrResultInfo recall( const QString& i_strAbsFilePath );
protected: // auxiliary instance methods
    virtual void save( QXmlStreamWriter& xmlStreamWriter, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry ) const;

}; // class CIdxTreeTrcAdminObjs

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSSys_TrcAdminObjIdxTree_h
