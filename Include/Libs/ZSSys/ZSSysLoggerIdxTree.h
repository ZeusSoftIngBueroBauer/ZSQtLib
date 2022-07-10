/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSSys_LoggerIdxTree_h
#define ZSSys_LoggerIdxTree_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

class QXmlStreamWriter;

namespace ZS
{
namespace System
{
class CLogger;
class CLogServer;

//******************************************************************************
class ZSSYSDLL_API CIdxTreeLoggers : public ZS::System::CIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Log"; }
    static QString ClassName() { return "CIdxTreeLoggers"; }
public: // ctors and dtor
    CIdxTreeLoggers( const QString& i_strObjName, QObject* i_pObjParent = nullptr );
    virtual ~CIdxTreeLoggers();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods to get and release admin objects
    CLogger* getLogger(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        ZS::System::EEnabled i_bEnabledAsDefault,
        ZS::System::ELogDetailLevel i_eDefaultDetailLevel,
        const QString& i_strDefaultDataFilter, // Use QString() (null) to ignore
        bool i_bIncrementRefCount = true );
    CLogger* getLogger( int i_idxInTree, bool i_bIncrementRefCount = true );
    void renameLogger( CLogger** io_ppLogger, const QString& i_strNewObjName );
    void releaseLogger( CLogger* i_pLogger );
public: // instance methods to insert branch nodes and admin objects
    ZS::System::CIdxTreeEntry* insertBranch(
        int            i_iParentBranchIdxInTree,
        const QString& i_strBranchName,
        int            i_idxInTree );
    CLogger* insertLogger(
        int            i_iParentBranchIdxInTree,
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        int            i_idxInTree );
public: // instance methods to recursively modify objects via object index of node entries
    void setEnabled( int i_idxInTree, ZS::System::EEnabled i_enabled );
    void setDetailLevel( int i_idxInTree, ZS::System::ELogDetailLevel i_eDetailLevel = ZS::System::ELogDetailLevel::None );
    void setDataFilter( int i_idxInTree, const QString& i_strDataFilter );
public: // instance methods to recursively modify objects via namespace node entries
    void setEnabled( ZS::System::CIdxTreeEntry* i_pBranch, ZS::System::EEnabled i_enabled );
    void setDetailLevel( ZS::System::CIdxTreeEntry* i_pBranch, ZS::System::ELogDetailLevel i_eDetailLevel = ZS::System::ELogDetailLevel::None );
    void setDataFilter( ZS::System::CIdxTreeEntry* i_pBranch, const QString& i_strDataFilter );
public: // overridables
    virtual ZS::System::SErrResultInfo save( const QString& i_strAbsFilePath ) const;
    virtual ZS::System::SErrResultInfo recall( const QString& i_strAbsFilePath );
protected: // auxiliary instance methods
    virtual void save( QXmlStreamWriter& xmlStreamWriter, ZS::System::CIdxTreeEntry* i_pTreeEntry ) const;
    virtual void removeEmptyBranches( const QString& i_strBranchPath );

}; // class CIdxTreeLoggers

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_LoggerIdxTree_h
