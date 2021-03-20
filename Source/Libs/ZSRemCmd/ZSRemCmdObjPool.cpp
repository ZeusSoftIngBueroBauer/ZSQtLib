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

#include <QtCore/qmutex.h>
#include <QtCore/qfile.h>
#include <QtCore/qthread.h>
#include <QtXml/qdom.h>

#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdSubSysNode.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::RemCmd;
using namespace ZS::Trace;


/*******************************************************************************
class CRemCmdObjPool : public ZS::CObjPool
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdObjPool::CRemCmdObjPool( const QString& i_strObjName, char i_chNodeSeparator ) :
//------------------------------------------------------------------------------
    CObjPool(
        /* strObjPoolName         */ i_strObjName,
        /* strNodeSeparator       */ QString(QByteArray(1,i_chNodeSeparator)),
        /* bSortTreeEntriesByName */ false )
{
    // Create trace admin object
    //--------------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdObjPool", i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CRemCmdObjPool::~CRemCmdObjPool()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    clear();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CModelObjPool
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObjPool::clear()
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    int iRow;

    // Clearing is the only way to delete list or tree entries.
    // During "normal operation" neither name space nor object entries
    // may be removed. If a referenced object is destroyed only the
    // reference to that object will be invalidated within the list
    // and tree entry (set to nullptr in the list entry and in the tree
    // entry the object state will be marked as destroyed).
    emit clearing(this);

    // The root entry will not be deleted.
    if( m_pTreeObjs->getChildCount() > 0 )
    {
        CObjPoolTreeEntry* pTreeEntryChild;
        CRemCmdSubSysNode* pRemCmdSubSysNode;
        CRemCmdObj*        pRemCmdObj;

        for( iRow = m_pTreeObjs->getChildCount()-1; iRow >= 0; iRow-- )
        {
            pTreeEntryChild = m_pTreeObjs->getChildEntry(iRow);

            if( pTreeEntryChild->getEntryType() != EObjPoolEntryTypeObject )
            {
                clearTreeEntry(pTreeEntryChild);
            }

            emit nodeRemoving( this, pTreeEntryChild );

            if( pTreeEntryChild->getEntryType() != EObjPoolEntryTypeObject )
            {
                emit nameSpaceNodeRemoving( this, pTreeEntryChild );
            }

            // After the childs of the child have been removed the child may be removed.
            m_pTreeObjs->removeChildEntry(iRow);

            if( pTreeEntryChild->getEntryType() == EObjPoolEntryTypeObject )
            {
                pRemCmdObj = reinterpret_cast<CRemCmdObj*>(pTreeEntryChild->getObj());

                if( pRemCmdObj != nullptr )
                {
                    delete pRemCmdObj;
                    pRemCmdObj = nullptr;
                }
            }
            else
            {
                pRemCmdSubSysNode = reinterpret_cast<CRemCmdSubSysNode*>(pTreeEntryChild->getObj());

                if( pRemCmdSubSysNode != nullptr )
                {
                    delete pRemCmdSubSysNode;
                    pRemCmdSubSysNode = nullptr;
                }
            }

            if( m_pTreeEntryCurr == pTreeEntryChild )
            {
                m_pTreeEntryCurr = nullptr;
            }

            delete pTreeEntryChild;
            pTreeEntryChild = nullptr;
        }
    }
    m_pTreeEntryCurr = m_pTreeObjs;

    if( m_arpListObjs.size() > 0 )
    {
        CObjPoolListEntry* pListEntry;

        for( iRow = m_arpListObjs.size()-1; iRow >= 0; iRow-- )
        {
            pListEntry = m_arpListObjs[iRow];

            emit objectNodeRemoving( this, pListEntry );

            delete pListEntry;
            pListEntry = nullptr;
            m_arpListObjs[iRow] = nullptr;
        }
    }
    m_arpListObjs.clear();

    m_pTreeObjs->setNodeName(m_strObjPoolName);

} // clear

/*==============================================================================
protected: // overridables of base class CModelObjPool
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObjPool::clearTreeEntry( CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    // The "removeEntry" method removes the given tree entry from the model.
    // If the given tree entry has childs also all child entries will be removed
    // and deleted. But the given tree entry will not be deleted. This is the task
    // of the callee.

    // This "removeEntry" method is called:
    // - On clearing the model. In this case the entry together with all childs
    //   need to be deleted and removed from the model.

    if( i_pTreeEntry == nullptr )
    {
        return;
    }

    if( i_pTreeEntry->getChildCount() > 0 )
    {
        CObjPoolTreeEntry* pTreeEntryChild;
        CRemCmdSubSysNode* pRemCmdSubSysNode;
        CRemCmdObj*        pRemCmdObj;
        int                iRow;

        // Starting from end of list to first entry so there is
        // no need to correct the row id's of the childs ..
        for( iRow = i_pTreeEntry->getChildCount()-1; iRow >= 0; iRow-- )
        {
            pTreeEntryChild = i_pTreeEntry->getChildEntry(iRow);

            if( pTreeEntryChild != nullptr && pTreeEntryChild->getEntryType() != EObjPoolEntryTypeObject )
            {
                clearTreeEntry(pTreeEntryChild);
            }

            emit nodeRemoving( this, pTreeEntryChild );

            if( pTreeEntryChild->getEntryType() != EObjPoolEntryTypeObject )
            {
                emit nameSpaceNodeRemoving( this, pTreeEntryChild );
            }

            // After the childs of the child have been removed the child may be removed.
            i_pTreeEntry->removeChildEntry(iRow);

            if( pTreeEntryChild->getEntryType() == EObjPoolEntryTypeObject )
            {
                pRemCmdObj = reinterpret_cast<CRemCmdObj*>(pTreeEntryChild->getObj());

                if( pRemCmdObj != nullptr )
                {
                    delete pRemCmdObj;
                    pRemCmdObj = nullptr;
                }
            }
            else
            {
                pRemCmdSubSysNode = reinterpret_cast<CRemCmdSubSysNode*>(pTreeEntryChild->getObj());

                if( pRemCmdSubSysNode != nullptr )
                {
                    delete pRemCmdSubSysNode;
                    pRemCmdSubSysNode = nullptr;
                }
            }

            if( m_pTreeEntryCurr == pTreeEntryChild )
            {
                m_pTreeEntryCurr = nullptr;
            }

            delete pTreeEntryChild;
            pTreeEntryChild = nullptr;

        } // for( iRow = i_pTreeEntry->getChildCount()-1; iRow >= 0; iRow-- )

    } // if( i_pTreeEntry->getChildCount() > 0 )

} // clearTreeEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CRemCmdObjPool::toXmlString() const
//------------------------------------------------------------------------------
{
    QString            strXml;
    CObjPoolTreeEntry* pTreeEntryChild;
    int                idxChild;

    strXml = "<RemCmds>";

    for( idxChild = 0; idxChild < m_pTreeObjs->getChildCount(); idxChild++ )
    {
        pTreeEntryChild = m_pTreeObjs->getChildEntry(idxChild);

        if( pTreeEntryChild != nullptr )
        {
            strXml = toXmlString(pTreeEntryChild,strXml);
        }
    }

    strXml += "</RemCmds>";

    return strXml;

} // toXmlString

/*==============================================================================
protected: // instance methods to send objects to clients connected at the control channel
==============================================================================*/

//------------------------------------------------------------------------------
QString CRemCmdObjPool::toXmlString( CObjPoolTreeEntry* i_pTreeEntry, QString& i_strXml ) const
//------------------------------------------------------------------------------
{
    QString            strXml = i_strXml;
    CObjPoolListEntry* pListEntry;
    CObjPoolTreeEntry* pTreeEntryChild;
    int                idxChild;

    if( i_pTreeEntry != nullptr )
    {
        if( i_pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
        {
            pListEntry = getListEntry(i_pTreeEntry->getObjId());

            if( pListEntry != nullptr )
            {
                CRemCmdObj* pRemCmdObj = reinterpret_cast<CRemCmdObj*>(pListEntry->getObj());

                if( pRemCmdObj != nullptr )
                {
                    strXml += pRemCmdObj->toXmlString();
                }
            }
        }
        else
        {
            CRemCmdSubSysNode* pRemCmdSubSysNode = reinterpret_cast<CRemCmdSubSysNode*>(i_pTreeEntry->getObj());

            if( pRemCmdSubSysNode != nullptr )
            {
                strXml += pRemCmdSubSysNode->toXmlString();
            }

            for( idxChild = 0; idxChild < i_pTreeEntry->getChildCount(); idxChild++ )
            {
                pTreeEntryChild = i_pTreeEntry->getChildEntry(idxChild);

                if( pTreeEntryChild != nullptr )
                {
                    strXml = toXmlString(pTreeEntryChild,strXml);
                }
            }
        }
    }

    return strXml;

} // toXmlString
