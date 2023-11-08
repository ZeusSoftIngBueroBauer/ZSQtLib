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

#include "ZSSys/ZSSysEnumerationIdxTree.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
class CEnumerationIdxTree : public ZS::System::CIdxTree
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QRecursiveMutex CEnumerationIdxTree::s_mtx;
QHash<QString, CEnumerationIdxTree*> CEnumerationIdxTree::s_hshpInstances;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CEnumerationIdxTree* CEnumerationIdxTree::GetInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return s_hshpInstances.value(i_strName, nullptr);
}

//------------------------------------------------------------------------------
/*! @brief

    @param i_pRootTreeEntry [in]
        If null a root entry is implicitely created.
    @param i_pObjParent [in]
        If true each access to member variables will be protected by a mutex.
*/
CEnumerationIdxTree* CEnumerationIdxTree::CreateInstance(
    const QString& i_strName,
    CIdxTreeEntry* i_pRootTreeEntry,
    const QString& i_strNodeSeparator,
    bool           i_bCreateMutex,
    QObject*       i_pObjParent )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( s_hshpInstances.contains(i_strName) )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "CEnumerationIdxTree::" + i_strName);
    }

    CEnumerationIdxTree* pIdxTree = new CEnumerationIdxTree(
        /* strName          */ i_strName,
        /* pRootEntry       */ i_pRootTreeEntry,
        /* strNodeSeparator */ i_strNodeSeparator,
        /* bCreateMutex     */ i_bCreateMutex,
        /* pObjParent       */ i_pObjParent );

    s_hshpInstances[i_strName] = pIdxTree;

    return pIdxTree;

} // CreateInstance

//------------------------------------------------------------------------------
void CEnumerationIdxTree::DestroyInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CEnumerationIdxTree* pIdxTree = s_hshpInstances.value(i_strName, nullptr);

    if( pIdxTree == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CEnumerationIdxTree::" + i_strName);
    }

    s_hshpInstances.remove(i_strName);

    delete pIdxTree;
    pIdxTree = nullptr;

} // DestroyInstance

//------------------------------------------------------------------------------
void CEnumerationIdxTree::DestroyInstance( CEnumerationIdxTree* i_pIdxTree )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strName = i_pIdxTree->objectName();

    if( !s_hshpInstances.contains(strName) )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CEnumerationIdxTree::" + strName);
    }

    s_hshpInstances.remove(strName);

    delete i_pIdxTree;
    i_pIdxTree = nullptr;

} // DestroyInstance

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CEnumerationIdxTree::CEnumerationIdxTree(
    const QString& i_strName,
    CIdxTreeEntry* i_pRootTreeEntry,
    const QString& i_strNodeSeparator,
    bool           i_bCreateMutex,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    CIdxTree(
        /* strIdxTreeName   */ i_strName,
        /* pRootTreeEntry   */ i_pRootTreeEntry,
        /* strNodeSeparator */ i_strNodeSeparator,
        /* bCreateMutex     */ i_bCreateMutex,
        /* pObjParent       */ i_pObjParent )
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strAddTrcInfo );

} // ctor

//------------------------------------------------------------------------------
CEnumerationIdxTree::~CEnumerationIdxTree()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strAddTrcInfo );

} // dtor
