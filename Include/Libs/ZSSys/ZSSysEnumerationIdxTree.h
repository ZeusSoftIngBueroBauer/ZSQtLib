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

#ifndef ZSSys_EnumerationIdxTree_h
#define ZSSys_EnumerationIdxTree_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QtCore/qmutex.h>
#endif

namespace ZS
{
namespace System
{
//******************************************************************************
class ZSSYSDLL_API CEnumerationIdxTree : public CIdxTree
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CEnumerationIdxTree"; }
public: // class methods
    static CEnumerationIdxTree* GetInstance( const QString& i_strName = "ZS::System::EnumerationIdxTree" );
    static CEnumerationIdxTree* CreateInstance(
        const QString& i_strName = "ZS::System::EnumerationIdxTree",
        CIdxTreeEntry* i_pRootTreeEntry = nullptr,
        const QString& i_strNodeSeparator = "::",
        bool           i_bCreateMutex = true,
        QObject*       i_pObjParent = nullptr );
    static void DestroyInstance( const QString& i_strName = "ZS::System::EnumerationIdxTree" );
    static void DestroyInstance( CEnumerationIdxTree* i_pIdxTree );
protected: // ctors and dtor
    CEnumerationIdxTree(
        const QString& i_strName,
        CIdxTreeEntry* i_pRootTreeEntry,
        const QString& i_strNodeSeparator,
        bool           i_bCreateMutex,
        QObject*       i_pObjParent );
    virtual ~CEnumerationIdxTree();
protected: // class members
    /*!< Mutex to protect the class and instance methods of the class for multithreaded access. */
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    static QRecursiveMutex s_mtx;
    #else
    static QMutex s_mtx;
    #endif
    /*!< Hash with all created enumeration index trees (key is name of instance). */
    static QHash<QString, CEnumerationIdxTree*> s_hshpInstances;

}; // class CEnumerationIdxTree

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_EnumerationIdxTree_h
