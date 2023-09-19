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

#ifndef ZSSys_TrcAdminObjIdxTree_h
#define ZSSys_TrcAdminObjIdxTree_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

class QXmlStreamWriter;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
class CTrcServer;

//******************************************************************************
class ZSSYSDLL_API CIdxTreeTrcAdminObjs : public CIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CIdxTreeTrcAdminObjs"; }
public: // ctors and dtor
    CIdxTreeTrcAdminObjs(
        const QString& i_strObjName,
        QObject* i_pObjParent,
        EMethodTraceDetailLevel i_eTrcDetailLevel = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelMutex = EMethodTraceDetailLevel::None );
    virtual ~CIdxTreeTrcAdminObjs();
public: // instance methods to get and release admin objects
    CTrcAdminObj* getTraceAdminObj(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        EEnabled i_bEnabledAsDefault,
        EMethodTraceDetailLevel i_eDefaultDetailLevelMethodCalls,
        ELogDetailLevel i_eDefaultDetailLevelRuntimeInfo,
        const QString& i_strDefaultDataFilter, // Use QString() (null) to ignore
        bool i_bIncrementRefCount = true );
    CTrcAdminObj* getTraceAdminObj( int i_idxInTree, bool i_bIncrementRefCount = true );
    CTrcAdminObj* renameTraceAdminObj(
        CTrcAdminObj* i_pTrcAdminObj, const QString& i_strNewObjName );
    void releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj );
public: // instance methods to insert branch nodes and admin objects
    CIdxTreeEntry* insertBranch(
        int            i_iParentBranchIdxInTree,
        const QString& i_strBranchName,
        int            i_idxInTree );
    CTrcAdminObj* insertTraceAdminObj(
        int            i_iParentBranchIdxInTree,
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        int            i_idxInTree );
public: // instance methods to recursively modify admin objects via object index of node entries
    void setEnabled( int i_idxInTree, EEnabled i_enabled );
    void setMethodCallsTraceDetailLevel( int i_idxInTree, EMethodTraceDetailLevel i_eDetailLevel = EMethodTraceDetailLevel::None );
    void setRuntimeInfoTraceDetailLevel( int i_idxInTree, ELogDetailLevel i_eDetailLevel = ELogDetailLevel::None );
    void setTraceDataFilter( int i_idxInTree, const QString& i_strDataFilter );
public: // instance methods to recursively modify admin objects via namespace node entries
    void setEnabled( CIdxTreeEntry* i_pBranch, EEnabled i_enabled );
    void setMethodCallsTraceDetailLevel( CIdxTreeEntry* i_pBranch, EMethodTraceDetailLevel i_eDetailLevel = EMethodTraceDetailLevel::None );
    void setRuntimeInfoTraceDetailLevel( CIdxTreeEntry* i_pBranch, ELogDetailLevel i_eDetailLevel = ELogDetailLevel::None );
    void setTraceDataFilter( CIdxTreeEntry* i_pBranch, const QString& i_strDataFilter );
public: // overridables
    virtual SErrResultInfo save( const QString& i_strAbsFilePath ) const;
    virtual SErrResultInfo recall( const QString& i_strAbsFilePath );
protected: // auxiliary instance methods
    virtual void save( QXmlStreamWriter& xmlStreamWriter, CIdxTreeEntry* i_pTreeEntry ) const;
    virtual void removeEmptyBranches( const QString& i_strBranchPath );

}; // class CIdxTreeTrcAdminObjs

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_TrcAdminObjIdxTree_h
