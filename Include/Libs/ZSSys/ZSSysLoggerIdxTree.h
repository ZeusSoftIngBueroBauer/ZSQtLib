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
class ZSSYSDLL_API CIdxTreeLoggers : public CIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CIdxTreeLoggers"; }
public: // ctors and dtor
    CIdxTreeLoggers( const QString& i_strObjName, QObject* i_pObjParent = nullptr );
    virtual ~CIdxTreeLoggers();
public: // instance methods to get and release admin objects
    CLogger* getLogger(
        const QString& i_strName, // Use node separator "::" to include name space, class name etc.
        EEnabled i_bEnabledAsDefault = EEnabled::Undefined,
        ELogDetailLevel i_eDefaultDetailLevel = ELogDetailLevel::Undefined,
        const QString& i_strDefaultDataFilter = QString() ); // Use QString() (null) to ignore
    CLogger* getLogger( int i_idxInTree );
public: // instance methods to insert branch nodes and logger leaves
    CIdxTreeEntry* insertBranch(
        int            i_iParentBranchIdxInTree,
        const QString& i_strBranchName,
        int            i_idxInTree );
    CLogger* insertLogger(
        int            i_iParentBranchIdxInTree,
        const QString& i_strLeaveName,
        int            i_idxInTree );
public: // instance methods to recursively modify loggers via index of node entries
    void setEnabled( int i_idxInTree, EEnabled i_enabled );
    void setDetailLevel( int i_idxInTree, ELogDetailLevel i_eDetailLevel = ELogDetailLevel::None );
    void setDataFilter( int i_idxInTree, const QString& i_strDataFilter );
public: // instance methods to recursively modify loggers via branch node entries
    void setEnabled( CIdxTreeEntry* i_pBranch, EEnabled i_enabled );
    void setDetailLevel( CIdxTreeEntry* i_pBranch, ELogDetailLevel i_eDetailLevel = ELogDetailLevel::None );
    void setDataFilter( CIdxTreeEntry* i_pBranch, const QString& i_strDataFilter );
public: // overridables
    virtual SErrResultInfo save( const QString& i_strAbsFilePath ) const;
    virtual SErrResultInfo recall( const QString& i_strAbsFilePath );
protected: // auxiliary instance methods
    virtual void save( QXmlStreamWriter& xmlStreamWriter, CIdxTreeEntry* i_pTreeEntry ) const;

}; // class CIdxTreeLoggers

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_LoggerIdxTree_h
