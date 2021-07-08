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

#include <QtCore/qfile.h>
#include <QtCore/qmutex.h>
#include <QtCore/qsize.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qtextstream.h>
#include <QtXml/qdom.h>

#include "ZSSys/ZSSysObjPool.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
namespace ZS
*******************************************************************************/

/*==============================================================================
EObjPoolEntryType
==============================================================================*/

static const SEnumEntry s_arEnumStrObjPoolEntryType[EObjPoolEntryTypeCount] = {
    /*  0 */ SEnumEntry( EObjPoolEntryTypeRoot,      "Root"      ),
    /*  1 */ SEnumEntry( EObjPoolEntryTypeObject,    "Object"    ),
    /*  2 */ SEnumEntry( EObjPoolEntryTypeNameSpace, "NameSpace" )
};

//------------------------------------------------------------------------------
QString ZS::System::objPoolEntryType2Str( int i_entryType )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrObjPoolEntryType,EObjPoolEntryTypeCount,i_entryType);
}

//------------------------------------------------------------------------------
EObjPoolEntryType ZS::System::str2ObjPoolEntryType( const QString& i_str )
//------------------------------------------------------------------------------
{
    EObjPoolEntryType entryType = EObjPoolEntryTypeUndefined;

    int iEntryType = SEnumEntry::str2Enumerator(s_arEnumStrObjPoolEntryType,EObjPoolEntryTypeCount,i_str);

    if( iEntryType >= 0 && iEntryType < EObjPoolEntryTypeCount )
    {
        entryType = static_cast<EObjPoolEntryType>(iEntryType);
    }
    return entryType;
}


/*******************************************************************************
class CObjPool : public QObject
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CObjPool::BuildPathStr(
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3,
    const QString& i_str4,
    const QString& i_str5 )
//------------------------------------------------------------------------------
{
    QString strPath = i_str1;

    if( !strPath.isEmpty() && !i_str2.isEmpty() )
    {
        strPath += "::";
    }
    if( !i_str2.isEmpty() )
    {
        strPath += i_str2;
    }
    if( !strPath.isEmpty() && !i_str3.isEmpty() )
    {
        strPath += "::";
    }
    if( !i_str3.isEmpty() )
    {
        strPath += i_str3;
    }
    if( !strPath.isEmpty() && !i_str4.isEmpty() )
    {
        strPath += "::";
    }
    if( !i_str4.isEmpty() )
    {
        strPath += i_str4;
    }
    if( !strPath.isEmpty() && !i_str5.isEmpty() )
    {
        strPath += "::";
    }
    if( !i_str5.isEmpty() )
    {
        strPath += i_str5;
    }
    return strPath;

} // buildPathStr

//------------------------------------------------------------------------------
bool CObjPool::IsDescendant(
    const QString& i_strObjPoolName,
    const QString& i_strParentName,
    const QString& i_strChildName,
    bool           i_bRecursive,
    const QString& i_strNodeSeparator )
//------------------------------------------------------------------------------
{
    bool        bIsParentChildRelation = true;
    QStringList strListParentNodeNames;
    QStringList strListChildNodeNames;
    QString     strParentNodeName;
    QString     strChildNodeName;
    int         idxNodeName;

    // Node names of the parent:
    if( !i_strParentName.isEmpty() )
    {
        strListParentNodeNames = i_strParentName.split(i_strNodeSeparator,QString::SkipEmptyParts);
    }
    if( strListParentNodeNames.count() > 0 && strListParentNodeNames[0].compare(i_strObjPoolName,Qt::CaseInsensitive) == 0 )
    {
        strListParentNodeNames.removeAt(0);
    }

    // Node names of the child:
    if( !i_strChildName.isEmpty() )
    {
        strListChildNodeNames = i_strChildName.split(i_strNodeSeparator,QString::SkipEmptyParts);
    }
    if( strListChildNodeNames.count() > 0 && strListChildNodeNames[0].compare(i_strObjPoolName,Qt::CaseInsensitive) == 0 )
    {
        strListChildNodeNames.removeAt(0);
    }

    if( strListParentNodeNames.count() >= strListChildNodeNames.count() )
    {
        bIsParentChildRelation = false;
    }
    else
    {
        int idxNodeCount = strListParentNodeNames.count();

        if( !i_bRecursive )
        {
            if( strListParentNodeNames.count() != (strListChildNodeNames.count() + 1) )
            {
                bIsParentChildRelation = false;
            }
        }

        // Check whether each node of the parent is part of the child name.
        // For all parent node names ...
        if( bIsParentChildRelation )
        {
            for( idxNodeName = 0; idxNodeName < idxNodeCount; idxNodeName++ )
            {
                strParentNodeName = strListParentNodeNames[idxNodeName];
                strChildNodeName = strListChildNodeNames[idxNodeName];

                if( strParentNodeName != strChildNodeName )
                {
                    bIsParentChildRelation = false;
                    break;
                }
            }
        }
    }
    return bIsParentChildRelation;

} // isDescendant

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjPool::CObjPool(
    const QString& i_strObjPoolName,
    const QString& i_strNodeSeparator,
    bool           i_bSortTreeEntriesByName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjPoolName(i_strObjPoolName),
    m_strNodeSeparator(i_strNodeSeparator),
    m_bSortTreeEntriesByName(i_bSortTreeEntriesByName),
    m_pMtxObjs(nullptr),
    m_arpListObjs(),
    m_pTreeObjs(nullptr),
    m_pTreeEntryCurr(nullptr),
    m_strFileName()
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    m_pMtxObjs = new QMutex(QMutex::Recursive);

    m_pTreeObjs = new CObjPoolTreeEntry(
        /* pObjPool    */ this,
        /* type        */ EObjPoolEntryTypeRoot,
        /* strNodeName */ m_strObjPoolName );

    // Please note that the root entry is a name space entry and does not
    // become part of the list.
    m_pTreeObjs->setRowId(0);

    // Temporarily store the last accessed tree entry:
    m_pTreeEntryCurr = m_pTreeObjs;

} // ctor

//------------------------------------------------------------------------------
CObjPool::~CObjPool()
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    m_pMtxObjs->lock();

    clear();

    // The root entry has not be deleted by the clear method and must
    // be explicitly destroyed.
    try
    {
        delete m_pTreeObjs;
    }
    catch(...)
    {
    }
    m_pTreeObjs = nullptr;
    m_pTreeEntryCurr = nullptr;

    m_pMtxObjs->unlock();

    try
    {
        delete m_pMtxObjs;
    }
    catch(...)
    {
    }
    m_pMtxObjs = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjPool::lock()
//------------------------------------------------------------------------------
{
    m_pMtxObjs->lock();
}

//------------------------------------------------------------------------------
void CObjPool::unlock()
//------------------------------------------------------------------------------
{
    m_pMtxObjs->unlock();
}

//------------------------------------------------------------------------------
void CObjPool::setObjPoolName( const QString& i_strObjPoolName )
//------------------------------------------------------------------------------
{
    m_strObjPoolName = i_strObjPoolName;
}

//------------------------------------------------------------------------------
int CObjPool::getObjCount()
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    return m_arpListObjs.size();
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CObjPool::clear()
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
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CObjPool::addObjNode(
    const QString& i_strObjPath,
    EObjState      i_objState )
//------------------------------------------------------------------------------
{
    QStringList strListNodeNames;
    int         idxNodeName;
    QString     strNameSpace;
    QString     strObjName;

    // Create list of node names:
    if( !i_strObjPath.isEmpty() )
    {
        strListNodeNames = i_strObjPath.split(m_strNodeSeparator,QString::SkipEmptyParts);
    }
    if( strListNodeNames.count() <= 1 )
    {
        strObjName = i_strObjPath;
    }
    else // if( strListNodeNames.count() > 1 )
    {
        strObjName = strListNodeNames[strListNodeNames.count()-1];
        strListNodeNames.removeAt(strListNodeNames.count()-1);

        for( idxNodeName = 0; idxNodeName < strListNodeNames.count()-1; idxNodeName++ )
        {
            strNameSpace += strListNodeNames[idxNodeName] + m_strNodeSeparator;
        }
        strNameSpace += strListNodeNames[idxNodeName];
    }

    return addObjNode(
        /* strNameSpace */ strNameSpace,
        /* strObjName   */ strObjName,
        /* objState     */ i_objState );

} // addObjNode

//------------------------------------------------------------------------------
int CObjPool::addObjNode(
    const QString& i_strNameSpace,
    const QString& i_strObjName,
    EObjState      i_objState )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry*  pTreeEntry  = nullptr;
    CObjPoolListEntry*  pListEntry  = nullptr;
    int                 iObjId      = -1;

    // Search entry (may have once been added but removed again).
    pTreeEntry = getTreeEntry(i_strNameSpace,i_strObjName);

    // If the object once has already been added to the object pool ...
    if( pTreeEntry != nullptr )
    {
        iObjId = pTreeEntry->getObjId();

        if( iObjId >= 0 && iObjId < m_arpListObjs.size() )
        {
            pListEntry = m_arpListObjs[iObjId];

            if( pListEntry != nullptr )
            {
                if( pListEntry->getObjState() != i_objState )
                {
                    setObjState(iObjId,i_objState);
                }
            }
            else
            {
                iObjId = -1;
            }
        }
        else
        {
            iObjId = -1;
        }
    }

    // If a new object node has to be created ..
    else // if( pTreeEntry == nullptr )
    {
        iObjId = m_arpListObjs.size();

        // Create new tree entry and add the entry to the tree:
        pTreeEntry = addTreeEntry(
            /* strNameSpace */ i_strNameSpace,
            /* strObjName   */ i_strObjName,
            /* entryType    */ EObjPoolEntryTypeObject,
            /* objState     */ i_objState );
        pTreeEntry->setObjId(iObjId);

        // Create new list entry and add the entry to the list:
        pListEntry = addListEntry(
            /* pTreeEntry   */ pTreeEntry,
            /* strNameSpace */ i_strNameSpace,
            /* strObjName   */ i_strObjName,
            /* objState     */ i_objState );
        pListEntry->setObjId(iObjId);

        // Temporarily store the last accessed tree entry:
        m_pTreeEntryCurr = pTreeEntry;

        // Please note that for the parent name space entries the signals
        // "nodeInserted" and "nameSpaceNodeInserted" have already been
        // emitted by the "addTreeEntry" method above (recursively).

        // The signal may only be emitted after the object has been added
        // to the list and to the tree of objects and if a valid object id
        // has been assigned to the list entry.
        emit nodeInserted(this,pTreeEntry);
        emit objectNodeInserted(this,pListEntry);

        iObjId = pListEntry->getObjId();

    } // if( pTreeEntry == nullptr )

    return iObjId;

} // addObjNode

//------------------------------------------------------------------------------
int CObjPool::addObjNode(
    const QString& i_strNameSpace,
    const QString& i_strObjName,
    void*          i_pvObj )
//------------------------------------------------------------------------------
{
    int iObjId = addObjNode(
        /* strNameSpace */ i_strNameSpace,
        /* strObjName   */ i_strObjName,
        /* objState     */ EObjState::Undefined );

    CObjPoolListEntry* pListEntry = getListEntry(iObjId);

    if( pListEntry != nullptr && pListEntry->getObj() != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Ambiguous object name "+i_strNameSpace+m_strNodeSeparator+i_strObjName);
    }

    setObj(iObjId,i_pvObj);

    return iObjId;

} // addObjNode

//------------------------------------------------------------------------------
int CObjPool::addObjNode(
    const QString& i_strNameSpace,
    QString*       i_pstrObjName,
    void*          i_pvObj )
//------------------------------------------------------------------------------
{
    if( i_pstrObjName == nullptr || i_pvObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    QString             strObjName = *i_pstrObjName;
    CObjPoolTreeEntry*  pTreeEntry  = nullptr;
    CObjPoolListEntry*  pListEntry  = nullptr;
    int                 iObjId      = -1;

    // Search entry (may have once been added but removed again).
    // On adding an object the next time the object must get the same
    // object id as on adding the object to the pool at the first time.
    pTreeEntry = getTreeEntry(i_strNameSpace,strObjName );

    // If the object once has already been added to the object pool ...
    if( pTreeEntry != nullptr )
    {
        pListEntry = getListEntry( pTreeEntry->getObjId() );

        // If another object with the same name is already alive ...
        if( pListEntry != nullptr && pListEntry->getObj() != nullptr )
        {
            // .. we correct the name of the object to become unique.
            int  idxClone = 1;
            bool bUniqueNodeNameFound = false;

            strObjName = *i_pstrObjName + "Clone" + QString::number(idxClone);

            while( !bUniqueNodeNameFound )
            {
                bUniqueNodeNameFound = true;
                pTreeEntry = getTreeEntry(i_strNameSpace,strObjName);

                if( pTreeEntry != nullptr )
                {
                    pListEntry = getListEntry( pTreeEntry->getObjId() );

                    // If another object with the same name is already alive ...
                    if( pListEntry != nullptr && pListEntry->getObj() != nullptr )
                    {
                        bUniqueNodeNameFound = false;
                        idxClone++;
                        strObjName = *i_pstrObjName + "Clone" + QString::number(idxClone);
                    }
                }
            }
            *i_pstrObjName = strObjName;
        }
    } // if( pTreeEntry != nullptr )

    // If it is the first time the object will be added to the object pool ...
    if( pTreeEntry == nullptr )
    {
        iObjId = m_arpListObjs.size();

        // Create new tree entry and add the entry to the tree:
        pTreeEntry = addTreeEntry(
            /* strNameSpace */ i_strNameSpace,
            /* strObjName   */ strObjName,
            /* entryType    */ EObjPoolEntryTypeObject );
        pTreeEntry->setObjId(iObjId);
        pTreeEntry->setObj(i_pvObj);

        // Create new list entry and add the entry to the list:
        pListEntry = addListEntry(
            /* pTreeEntry   */ pTreeEntry,
            /* strNameSpace */ i_strNameSpace,
            /* strObjName   */ strObjName );
        pListEntry->setObjId(iObjId);
        pListEntry->setObj(i_pvObj);

        // Here we set the object reference directly at the tree and list
        // entries as we are going to emit the "nodeInserted" signals.

        // Please note that for the parent name space entries the signals
        // "nodeInserted" and "nameSpaceNodeInserted" have already been
        // emitted by the "addTreeEntry" method above (recursively).

        // The signal may only be emitted after the object has been added
        // to the list and to the tree of objects and if a valid object id
        // has been assigned to the list entry.
        emit nodeInserted(this,pTreeEntry);
        emit objectNodeInserted(this,pListEntry);
    }
    else
    {
        // Get "old" list entry:
        pListEntry = getListEntry( pTreeEntry->getObjId() );

        // Implicitly sets the object state and emits the "nodeChanged" signals:
        setObj( pListEntry->getObjId(), nullptr );
    }
    if( pListEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    // Temporarily store the last accessed tree entry:
    m_pTreeEntryCurr = pTreeEntry;

    return pListEntry->getObjId();

} // addObjNode

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::addObjNode(
    const QString& i_strObjPath,
    int            i_iObjId,
    EObjState      i_objState )
//------------------------------------------------------------------------------
{
    QStringList strListNodeNames;
    int         idxNodeName;
    QString     strNameSpace;
    QString     strObjName;

    // Create list of node names:
    if( !i_strObjPath.isEmpty() )
    {
        strListNodeNames = i_strObjPath.split(m_strNodeSeparator,QString::SkipEmptyParts);
    }
    if( strListNodeNames.count() <= 1 )
    {
        strObjName = i_strObjPath;
    }
    else // if( strListNodeNames.count() > 1 )
    {
        strObjName = strListNodeNames[strListNodeNames.count()-1];
        strListNodeNames.removeAt(strListNodeNames.count()-1);

        for( idxNodeName = 0; idxNodeName < strListNodeNames.count()-1; idxNodeName++ )
        {
            strNameSpace += strListNodeNames[idxNodeName] + m_strNodeSeparator;
        }
        strNameSpace += strListNodeNames[idxNodeName];
    }

    return addObjNode(
        /* strNameSpace */ strNameSpace,
        /* strObjName   */ strObjName,
        /* iObjId       */ i_iObjId,
        /* objState     */ i_objState );

} // addObjNode

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::addObjNode(
    const QString& i_strNameSpace,
    const QString& i_strObjName,
    int            i_iObjId,
    EObjState      i_objState )
//------------------------------------------------------------------------------
{
    if( i_iObjId < 0 )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry  = nullptr;
    CObjPoolListEntry* pListEntry  = nullptr;
    int                idxObj;

    // Search entry (may have once been added but removed again).
    pTreeEntry = getTreeEntry(
        /* strNameSpace */ i_strNameSpace,
        /* strObjName   */ i_strObjName );

    // If the object once has already been added to the object pool ...
    if( pTreeEntry != nullptr )
    {
        // On adding an object with a predefined id the object may not
        // have already been added to the pool.
        return nullptr;
    }

    // If the ID of the object to be added is within the current range
    // of the already added objects ...
    if( i_iObjId >= 0 && i_iObjId < m_arpListObjs.size() )
    {
        pListEntry = m_arpListObjs[i_iObjId];
    }

    // If the object once has already been added to the object pool ...
    if( pListEntry != nullptr )
    {
        // On adding an object with a predefined id the object may not
        // have already been added to the pool.
        return nullptr;
    }

    if( i_iObjId >= m_arpListObjs.size() )
    {
        idxObj = m_arpListObjs.size();

        m_arpListObjs.resize(i_iObjId+1);

        for( ; idxObj < m_arpListObjs.size(); idxObj++ )
        {
            m_arpListObjs[idxObj] = nullptr;
        }
    }

    // Create new tree entry and add the entry to the tree:
    pTreeEntry = addTreeEntry(
        /* strNameSpace */ i_strNameSpace,
        /* strObjName   */ i_strObjName,
        /* entryType    */ EObjPoolEntryTypeObject,
        /* objState     */ i_objState );
    pTreeEntry->setObjId(i_iObjId);

    // Create new list entry and add the entry to the list:
    pListEntry = new CObjPoolListEntry(
        /* pObjPool     */ this,
        /* pTreeEntry   */ pTreeEntry,
        /* strNameSpace */ i_strNameSpace,
        /* strObjName   */ i_strObjName );
    pListEntry->setObjState(i_objState);
    pListEntry->setObjId(i_iObjId);

    // Here we set the object state directly at the tree and list
    // entries as we are going to emit the "nodeInserted" signals.

    m_arpListObjs[i_iObjId] = pListEntry;

    // Temporarily store the last accessed tree entry:
    m_pTreeEntryCurr = pTreeEntry;

    // Please note that for the parent name space entries the signals
    // "nodeInserted" and "nameSpaceNodeInserted" have already been
    // emitted by the "addTreeEntry" method above (recursively).

    // The signal may only be emitted after the object has been added
    // to the list and to the tree of objects and if a valid object id
    // has been assigned to the list entry.
    emit nodeInserted(this,pTreeEntry);
    emit objectNodeInserted(this,pListEntry);

    return pTreeEntry;

} // addObjNode

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::addNameSpaceNode( const QString& i_strNameSpace )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry  = nullptr;

    // Search entry (may have once been added but removed again).
    pTreeEntry = getTreeEntry(
        /* strPath   */ i_strNameSpace,
        /* entryType */ EObjPoolEntryTypeNameSpace );

    // If the entry is not already existing ...
    if( pTreeEntry == nullptr )
    {
        // Create new tree entry and add the entry to the tree:
        pTreeEntry = addTreeEntry(
            /* strPath   */ i_strNameSpace,
            /* entryType */ EObjPoolEntryTypeNameSpace,
            /* objState  */ EObjState::Undefined );

        // Please note that for the name space entries the signals
        // "nodeInserted" and "nameSpaceNodeInserted" have already been
        // emitted by the "addTreeEntry" method above (recursively).

        // Temporarily store the last accessed tree entry:
        m_pTreeEntryCurr = pTreeEntry;
    }

    return pTreeEntry;

} // addNameSpaceNode

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::addNameSpaceNode(
    const QString& i_strParentNameSpace,
    const QString& i_strNodeName )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry  = nullptr;

    // Search entry (may have once been added but removed again).
    pTreeEntry = getTreeEntry(
        /* strParentNameSpace */ i_strParentNameSpace,
        /* strNodeName        */ i_strNodeName,
        /* entryType          */ EObjPoolEntryTypeNameSpace );

    // If the entry is not already existing ...
    if( pTreeEntry == nullptr )
    {
        // Create new tree entry and add the entry to the tree:
        pTreeEntry = addTreeEntry(
            /* strParentNameSpace */ i_strParentNameSpace,
            /* strNodeName        */ i_strNodeName,
            /* entryType          */ EObjPoolEntryTypeNameSpace );

        // Please note that for the name space entries the signals
        // "nodeInserted" and "nameSpaceNodeInserted" have already been
        // emitted by the "addTreeEntry" method above (recursively).

        // Temporarily store the last accessed tree entry:
        m_pTreeEntryCurr = pTreeEntry;
    }

    return pTreeEntry;

} // addNameSpaceNode

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::addNode(
    const QString&    i_strPath,
    EObjPoolEntryType i_entryType )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry = nullptr;
    CObjPoolListEntry* pListEntry = nullptr;

    // Search entry (may have once been added but removed again).
    pTreeEntry = getTreeEntry(
        /* strPath   */ i_strPath,
        /* entryType */ i_entryType );

    // If the entry is not already existing ...
    if( pTreeEntry == nullptr )
    {
        if( i_entryType == EObjPoolEntryTypeObject )
        {
            int iObjId = addObjNode(i_strPath);
            pListEntry = getListEntry(iObjId);
            pTreeEntry = pListEntry->getTreeEntry();
        }
        else if( i_entryType == EObjPoolEntryTypeNameSpace )
        {
            pTreeEntry = addNameSpaceNode(i_strPath);
        }
    }

    return pTreeEntry;

} // addNode

//------------------------------------------------------------------------------
void CObjPool::invalidateObjNode( int i_iObjId )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry;
    CObjPoolListEntry* pListEntry = getListEntry(i_iObjId);

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    pTreeEntry = pListEntry->getTreeEntry();
    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    // The signal may only be emitted after both the list entry and the
    // tree entry have been updated.
    if( pListEntry->getObj() != nullptr )
    {
        // Implicitly sets the object state and emits the "nodeChanged" signals:
        setObj( pListEntry->getObjId(), nullptr );
    }

} // invalidateObjNode

//------------------------------------------------------------------------------
void CObjPool::invalidateObjNode(
    const QString& i_strNameSpace,
    const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolListEntry* pListEntry = nullptr;
    CObjPoolTreeEntry* pTreeEntry = nullptr;

    pTreeEntry = getTreeEntry(
        /* strNameSpace */ i_strNameSpace,
        /* strObjName   */ i_strObjName );
    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    pListEntry = getListEntry( pTreeEntry->getObjId() );
    if( pListEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    // Please note that removing the object does neither really remove the
    // entry from the pools list nor from the pools tree. The entry will only
    // be marked as "not used".

    // The signal may only be emitted after both the list entry and the
    // tree entry have been updated.
    if( pListEntry->getObj() != nullptr )
    {
        // Implicitly sets the object state and emits the "nodeChanged" signals:
        setObj( pListEntry->getObjId(), nullptr );
    }

} // invalidateObjNode

//------------------------------------------------------------------------------
void CObjPool::invalidateObjNode( void* i_pvObj )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry;
    CObjPoolListEntry* pListEntry = getListEntry(i_pvObj);

    if( pListEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    pTreeEntry = pListEntry->getTreeEntry();
    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    // Please note that removing the object does neither really remove the
    // entry from the pools list nor from the pools tree. The entry will only
    // be marked as "not used".

    // The signal may only be emitted after both the list entry and the
    // tree entry have been updated.
    if( pListEntry->getObj() != nullptr )
    {
        // Implicitly sets the object state and emits the "nodeChanged" signals:
        setObj( pListEntry->getObjId(), nullptr );
    }

} // invalidateObjNode

//------------------------------------------------------------------------------
int CObjPool::getObjId(
    const QString& i_strNameSpace,
    const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry*  pTreeEntry = nullptr;
    int                 iObjId     = -1;

    pTreeEntry = getTreeEntry(
        /* strNameSpace */ i_strNameSpace,
        /* strObjName   */ i_strObjName );
    if( pTreeEntry != nullptr )
    {
        iObjId = pTreeEntry->getObjId();
    }
    return iObjId;

} // getObjId

//------------------------------------------------------------------------------
int CObjPool::getObjId( const QString& i_strObjPath )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry*  pTreeEntry = nullptr;
    int                 iObjId     = -1;

    pTreeEntry = getTreeEntry(i_strObjPath);

    if( pTreeEntry != nullptr )
    {
        iObjId = pTreeEntry->getObjId();
    }
    return iObjId;

} // getObjId

//------------------------------------------------------------------------------
void CObjPool::setObjState( int i_iObjId, EObjState i_objState )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry;
    CObjPoolListEntry* pListEntry = getListEntry(i_iObjId);

    if( pListEntry != nullptr )
    {
        pTreeEntry = pListEntry->getTreeEntry();
        if( pTreeEntry == nullptr )
        {
            throw CException(__FILE__,__LINE__,EResultObjNotInList);
        }

        // The signal may only be emitted after both the list entry and the
        // tree entry have been updated.
        if( pListEntry->getObjState() != i_objState )
        {
            pListEntry->setObjState(i_objState);
            pTreeEntry->setObjState(i_objState);

            emit nodeChanged(this,pTreeEntry);
            emit objectNodeChanged(this,pListEntry);
        }
    }

} // setObjState

//------------------------------------------------------------------------------
EObjState CObjPool::getObjState( int i_iObjId ) const
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    EObjState          objState = EObjState::Undefined;
    CObjPoolListEntry* pListEntry = getListEntry(i_iObjId);

    if( pListEntry != nullptr )
    {
        objState = pListEntry->getObjState();
    }
    return objState;

} // getObjState

//------------------------------------------------------------------------------
void CObjPool::setObj( int i_iObjId, void* i_pvObj )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolTreeEntry* pTreeEntry;
    CObjPoolListEntry* pListEntry = getListEntry(i_iObjId);

    if( pListEntry != nullptr )
    {
        pTreeEntry = pListEntry->getTreeEntry();
        if( pTreeEntry == nullptr )
        {
            throw CException(__FILE__,__LINE__,EResultObjNotInList);
        }

        // The signal may only be emitted after both the list entry and the
        // tree entry have been updated.
        if( pListEntry->getObj() != i_pvObj )
        {
            pListEntry->setObj(i_pvObj);
            pTreeEntry->setObj(i_pvObj);

            emit nodeChanged(this,pTreeEntry);
            emit objectNodeChanged(this,pListEntry);
        }
    }

} // setObj

//------------------------------------------------------------------------------
void* CObjPool::getObj( int i_iObjId )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    void*              pvObj = nullptr;
    CObjPoolListEntry* pListEntry = getListEntry(i_iObjId);

    if( pListEntry != nullptr )
    {
        pvObj = pListEntry->getObj();
    }
    return pvObj;

} // getObj

//------------------------------------------------------------------------------
void* CObjPool::getObj(
    const QString& i_strNameSpace,
    const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    void*              pvObj = nullptr;
    CObjPoolListEntry* pListEntry = nullptr;
    CObjPoolTreeEntry* pTreeEntry = nullptr;

    pTreeEntry = getTreeEntry(
        /* strNameSpace */ i_strNameSpace,
        /* strObjName   */ i_strObjName );

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->getObjId() >= 0 && pTreeEntry->getObjId() < m_arpListObjs.size() )
        {
            pListEntry = getListEntry( pTreeEntry->getObjId() );

            if( pListEntry != nullptr )
            {
                pvObj = pListEntry->getObj();
            }
        }
    }
    return pvObj;

} // getObj

//------------------------------------------------------------------------------
void* CObjPool::getObj( const QString& i_strObjPath )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    void*              pvObj = nullptr;
    CObjPoolListEntry* pListEntry;
    CObjPoolTreeEntry* pTreeEntry = getTreeEntry(i_strObjPath);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->getObjId() >= 0 && pTreeEntry->getObjId() < m_arpListObjs.size() )
        {
            pListEntry = getListEntry( pTreeEntry->getObjId() );

            if( pListEntry != nullptr )
            {
                pvObj = pListEntry->getObj();
            }
        }
    }
    return pvObj;

} // getObj

/*==============================================================================
public: // instance methods (for experts only)
==============================================================================*/

//------------------------------------------------------------------------------
CObjPoolListEntry* CObjPool::getListEntry( int i_iObjId ) const
//------------------------------------------------------------------------------
{
    if( i_iObjId < 0 || i_iObjId >= m_arpListObjs.size() )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_iObjId:" + QString::number(i_iObjId));
    }

    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolListEntry* pListEntry = m_arpListObjs[i_iObjId];

    if( pListEntry != nullptr && pListEntry->getObjId() != i_iObjId )
    {
        pListEntry = nullptr;
    }

    if( pListEntry == nullptr )
    {
        if( m_pTreeEntryCurr != nullptr && m_pTreeEntryCurr->getObjId() >= 0 && m_pTreeEntryCurr->getObjId() < m_arpListObjs.size() )
        {
            pListEntry = m_arpListObjs[m_pTreeEntryCurr->getObjId()];

            if( pListEntry != nullptr && pListEntry->getObjId() != i_iObjId )
            {
                pListEntry = nullptr;
            }
        }
    }

    if( pListEntry == nullptr )
    {
        int idxObj;

        for( idxObj = 0; idxObj < m_arpListObjs.size(); idxObj++ )
        {
            if( m_arpListObjs[idxObj] != nullptr && m_arpListObjs[idxObj]->getObjId() == i_iObjId )
            {
                pListEntry = m_arpListObjs[idxObj];
                break;
            }
        }
    }

    return pListEntry;

} // getListEntry

//------------------------------------------------------------------------------
CObjPoolListEntry* CObjPool::getListEntry( void* i_pvObj ) const
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    CObjPoolListEntry* pListEntry = nullptr;

    if( m_pTreeEntryCurr != nullptr && m_pTreeEntryCurr->getObjId() >= 0 && m_pTreeEntryCurr->getObjId() < m_arpListObjs.size() )
    {
        pListEntry = m_arpListObjs[m_pTreeEntryCurr->getObjId()];

        if( pListEntry != nullptr && pListEntry->getObj() != i_pvObj )
        {
            pListEntry = nullptr;
        }
    }

    if( pListEntry == nullptr )
    {
        int idxObj;

        for( idxObj = 0; idxObj < m_arpListObjs.size(); idxObj++ )
        {
            if( m_arpListObjs[idxObj] != nullptr && m_arpListObjs[idxObj]->getObj() == i_pvObj )
            {
                pListEntry = m_arpListObjs[idxObj];
                break;
            }
        }
    }

    return pListEntry;

} // getListEntry

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::getTreeEntry(
    const QString&    i_strNameSpace,
    const QString&    i_strObjName,
    EObjPoolEntryType i_entryType ) const
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    if( m_pTreeEntryCurr != nullptr )
    {
        if( (m_pTreeEntryCurr->getEntryType() == i_entryType)
         && (m_pTreeEntryCurr->getParentNameSpace(true).compare(i_strNameSpace,Qt::CaseInsensitive) == 0)
         && (m_pTreeEntryCurr->getNodeName(false).compare(i_strObjName,Qt::CaseInsensitive) == 0) )
        {
            return m_pTreeEntryCurr;
        }
    }

    CObjPoolTreeEntry*        pTreeEntryParent = m_pTreeObjs;
    CObjPoolTreeEntry*        pTreeEntry = nullptr;
    const CObjPoolTreeEntry*  pTreeEntryTmp;
    int                       iRow;

    if( i_entryType == EObjPoolEntryTypeRoot )
    {
        return m_pTreeObjs;
    }
    if( (m_pTreeObjs->getParentNameSpace(true).compare(i_strNameSpace,Qt::CaseInsensitive) == 0)
     && (m_pTreeObjs->getNodeName(false).compare(i_strObjName,Qt::CaseInsensitive) == 0) )
    {
        return m_pTreeObjs;
    }

    QStringList               strListRootNodeNames;
    QStringList               strListNodeNames;
    int                       idxNodeName = 0;
    QString                   strNodeName;
    const CObjPoolTreeEntry*  pTreeEntryChild;

    // Create list of node names of the root entry:
    if( !m_pTreeObjs->getNodeName(false).isEmpty() )
    {
        strListRootNodeNames = m_pTreeObjs->getNodeName(true).split(m_strNodeSeparator,QString::SkipEmptyParts);
    }

    // Create list of node names:
    if( !i_strNameSpace.isEmpty() )
    {
        strListNodeNames = i_strNameSpace.split(m_strNodeSeparator,QString::SkipEmptyParts);
    }

    // We want to find an entry below the root entry.
    if( strListRootNodeNames.count() > 0 && strListRootNodeNames.count() <= strListNodeNames.count() )
    {
        for( idxNodeName = static_cast<int>(strListRootNodeNames.count()); idxNodeName > 0; idxNodeName-- )
        {
            if( strListNodeNames[idxNodeName-1].compare(strListRootNodeNames[idxNodeName-1],Qt::CaseInsensitive) == 0 )
            {
                strListNodeNames.removeAt(idxNodeName-1);
            }
        }
    }

    // For all parent node names ...
    while( idxNodeName < strListNodeNames.count() )
    {
        strNodeName = strListNodeNames[idxNodeName];

        // Search current namespace in the child list of the current parent node ...
        pTreeEntryChild = nullptr;
        for( iRow = 0; iRow < static_cast<int>(pTreeEntryParent->getChildCount()); iRow++ )
        {
            pTreeEntryTmp = pTreeEntryParent->getChildEntry(iRow);

            if( (pTreeEntryTmp->getEntryType() == EObjPoolEntryTypeNameSpace)
             && (pTreeEntryTmp->getNodeName(false).compare(strNodeName,Qt::CaseInsensitive)) == 0 )
            {
                pTreeEntryChild = pTreeEntryTmp;
                break;
            }
        }

        // The found child becomes the new parent in which the next parent node name is searched as a child.
        pTreeEntryParent = const_cast<CObjPoolTreeEntry*>(pTreeEntryChild);

        // Child not found ..
        if( pTreeEntryChild == nullptr )
        {
            break;
        }

        idxNodeName++;
    }

    // Search node in the child list of the parent name space ...
    if( pTreeEntryParent != nullptr )
    {
        for( iRow = 0; iRow < static_cast<int>(pTreeEntryParent->getChildCount()); iRow++ )
        {
            pTreeEntryTmp = pTreeEntryParent->getChildEntry(iRow);

            if( (pTreeEntryTmp->getEntryType() == i_entryType)
             && (pTreeEntryTmp->getNodeName(false).compare(i_strObjName,Qt::CaseInsensitive) == 0) )
            {
                pTreeEntry = const_cast<CObjPoolTreeEntry*>(pTreeEntryTmp);
                break;
            }
        }
    }

    // Temporarily store the last accessed tree entry:
    if( pTreeEntry != nullptr )
    {
        m_pTreeEntryCurr = pTreeEntry;
    }

    return pTreeEntry;

} // getTreeEntry

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::getTreeEntry(
    const QString&    i_strPath,
    EObjPoolEntryType i_entryType ) const
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    if( m_pTreeEntryCurr != nullptr )
    {
        if( m_pTreeEntryCurr->getEntryType() == i_entryType
         && m_pTreeEntryCurr->getNodeName(true).compare(i_strPath,Qt::CaseInsensitive) == 0 )
        {
            return m_pTreeEntryCurr;
        }
    }

    QStringList        strListNodeNames;
    int                idxNodeName = 0;
    QString            strNameSpace;
    QString            strObjName;
    CObjPoolTreeEntry* pTreeEntry;

    if( i_entryType != EObjPoolEntryTypeObject && i_strPath == m_pTreeObjs->getNodeName(true) )
    {
        return m_pTreeObjs;
    }

    // Create list of node names:
    if( !i_strPath.isEmpty() )
    {
        strListNodeNames = i_strPath.split(m_strNodeSeparator,QString::SkipEmptyParts);
        strObjName = strListNodeNames[strListNodeNames.count()-1];

        if( strListNodeNames.count() > 1 )
        {
            strNameSpace = strListNodeNames[0];
            for( idxNodeName = 1; idxNodeName < static_cast<int>(strListNodeNames.count()-1); idxNodeName++ )
            {
                strNameSpace += m_strNodeSeparator;
                strNameSpace += strListNodeNames[idxNodeName];
            }
        }
    }

    pTreeEntry = getTreeEntry(
        /* strParentNameSpace */ strNameSpace,
        /* strNodeName        */ strObjName,
        /* entryType          */ i_entryType );

    return pTreeEntry;

} // getTreeEntry

/*==============================================================================
public: // overridables (for experts only)
==============================================================================*/

//------------------------------------------------------------------------------
void CObjPool::dataChanged( CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    if( i_pTreeEntry != nullptr )
    {
        emit nodeChanged( this, i_pTreeEntry );

        if( i_pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
        {
            CObjPoolListEntry* pListEntry = getListEntry(i_pTreeEntry->getObjId());

            if( pListEntry != nullptr )
            {
                emit objectNodeChanged( this, pListEntry );
            }
        }
        else
        {
            emit nameSpaceNodeChanged( this, i_pTreeEntry );
        }
    }

} // dataChanged

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjPool::setFileName( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    if( m_strFileName != i_strFileName )
    {
        m_strFileName = i_strFileName;
        emit adminObjsFileNameChanged(this,m_strFileName);
    }
}

//------------------------------------------------------------------------------
QString CObjPool::getFileName() const
//------------------------------------------------------------------------------
{
    return m_strFileName;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
SErrResult CObjPool::recall( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    QString strFileNamePrev = m_strFileName;

    if( m_strFileName.isEmpty() )
    {
        m_strFileName = i_strFileName;
    }

    QString strFileName = m_strFileName;

    if( i_strFileName.length() > 0 )
    {
        strFileName = i_strFileName;
    }

    QDomDocument domdoc;
    QFile fileXML(strFileName);
    if( !fileXML.open(QIODevice::ReadOnly) )
    {
        return SErrResult(EResultSeverityError,EResultFileOpenForRead);
    }
    if( !domdoc.setContent(&fileXML) )
    {
        fileXML.close();
        return SErrResult(EResultSeverityError,EResultFileReadContent);
    }
    fileXML.close();

    QDomElement  domelem = domdoc.documentElement();
    QDomNode     domnodeLev1, domnodeLev2, domnodeLev3, domnodeLev4;
    bool         bIsElement;
    QString      strDomNodeName;

    for( domnodeLev1 = domelem.firstChild(); !domnodeLev1.isNull(); domnodeLev1 = domnodeLev1.nextSibling() )
    {
        bIsElement  = domnodeLev1.isElement();
        strDomNodeName = domnodeLev1.nodeName();

        if( bIsElement && strDomNodeName == "Head" )
        {
        }
        else if( bIsElement && strDomNodeName == "Body" )
        {
            domelem = domnodeLev1.toElement();

            for( domnodeLev2 = domelem.firstChild(); !domnodeLev2.isNull(); domnodeLev2 = domnodeLev2.nextSibling() )
            {
                bIsElement     = domnodeLev2.isElement();
                strDomNodeName = domnodeLev2.nodeName();

                if( bIsElement && strDomNodeName == "TreeEntry" )
                {
                    EObjPoolEntryType entryType = EObjPoolEntryTypeUndefined;
                    QString           strEntryType;
                    QString           strNameSpace;
                    QString           strObjName;
                    QString           strParentNameSpace;
                    QString           strNodeName;

                    domelem = domnodeLev2.toElement();

                    for( domnodeLev3 = domelem.firstChild(); !domnodeLev3.isNull(); domnodeLev3 = domnodeLev3.nextSibling() )
                    {
                        bIsElement  = domnodeLev3.isElement();
                        strDomNodeName = domnodeLev3.nodeName();

                        if( bIsElement && strDomNodeName == "EntryType" )
                        {
                            domelem = domnodeLev3.toElement();
                            strEntryType = domelem.text();
                            entryType = str2ObjPoolEntryType(strEntryType);
                        }
                        else if( bIsElement && strDomNodeName == "ParentNameSpace" )
                        {
                            domelem = domnodeLev3.toElement();
                            strParentNameSpace = domelem.text();
                        }
                        else if( bIsElement && strDomNodeName == "NodeName" )
                        {
                            domelem = domnodeLev3.toElement();
                            strNodeName = domelem.text();
                        }
                        else if( bIsElement && strDomNodeName == "NameSpace" )
                        {
                            domelem = domnodeLev3.toElement();
                            strNameSpace = domelem.text();
                        }
                        else if( bIsElement && strDomNodeName == "ObjName" )
                        {
                            domelem = domnodeLev3.toElement();
                            strObjName = domelem.text();
                        }
                    } // for( domnodeLev3.nextSibling )

                    if( entryType == EObjPoolEntryTypeNameSpace )
                    {
                        addNameSpaceNode(
                            /* strParentNameSpace */ strParentNameSpace,
                            /* strNodeName        */ strNodeName );
                    }
                    else if( entryType == EObjPoolEntryTypeObject )
                    {
                        addObjNode(
                            /* strNameSpace */ strNameSpace,
                            /* strObjName   */ strObjName );
                    }

                } // if( strDomNodeName == "TreeEntry )
            } // for( domnodeLev2.nextSibling )
        } // if( strDomNodeName == "Body" )
    } // for( domnodeLev1.nextSibling(

    if( m_strFileName != strFileNamePrev )
    {
        emit adminObjsFileNameChanged(this,m_strFileName);
    }

    return ErrResultSuccess;

} // recall

//------------------------------------------------------------------------------
SErrResult CObjPool::save( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pMtxObjs);

    QString strFileNamePrev = m_strFileName;

    if( m_strFileName.isEmpty() )
    {
        m_strFileName = i_strFileName;
    }

    QString strFileName = m_strFileName;

    if( i_strFileName.length() > 0 )
    {
        strFileName = i_strFileName;
    }

    QDomDocument domdoc;

    QDomElement domelemRoot = domdoc.createElement( "opml" );
    domelemRoot.setAttribute("version","1.0");
    domdoc.appendChild( domelemRoot );

    QDomElement domelemHead = domdoc.createElement( "Head" );
    domelemRoot.appendChild( domelemHead );

    QDomElement domelemHeadTitle = domdoc.createElement( "Title" );
    domelemHead.appendChild( domelemHeadTitle );

    QDomText domtextHeadTitle = domdoc.createTextNode( "Admin Objects" );
    domelemHeadTitle.appendChild( domtextHeadTitle );

    QDomElement domelemBody = domdoc.createElement( "Body" );
    domelemRoot.appendChild( domelemBody );

    QDomElement domelem = domdoc.createElement("TreeEntry");
    domelemBody.appendChild(domelem);

    QDomElement domelemEntryType = domdoc.createElement("EntryType");
    domelem.appendChild(domelemEntryType);
    QDomText domtextEntryType = domdoc.createTextNode( "Root" );
    domelemEntryType.appendChild(domtextEntryType);

    // Recursively save the admin object tree
    saveTreeEntry(domdoc,domelemBody,m_pTreeObjs);

    QFile fileXML(strFileName);
    if( !fileXML.open(QIODevice::WriteOnly) )
    {
        return SErrResult(EResultSeverityError,EResultFileOpenForWrite);
    }
    QTextStream stream(&fileXML);
    stream << domdoc.toString(4);
    fileXML.close();

    if( m_strFileName != strFileNamePrev )
    {
        emit adminObjsFileNameChanged(this,m_strFileName);
    }

    return ErrResultSuccess;

} // save

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CObjPool::saveTreeEntry(
    QDomDocument&      i_domdoc,
    QDomElement&       i_domelemBody,
    CObjPoolTreeEntry* i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    CObjPoolTreeEntry* pTreeEntryChild;
    int                iRow;

    for( iRow = 0; iRow < static_cast<int>(i_pTreeEntry->getChildCount()); iRow++ )
    {
        pTreeEntryChild = i_pTreeEntry->getChildEntry(iRow);

        if( pTreeEntryChild != nullptr )
        {
            QDomElement domelem = i_domdoc.createElement("TreeEntry");
            i_domelemBody.appendChild(domelem);

            QDomElement domelemEntryType = i_domdoc.createElement("EntryType");
            domelem.appendChild(domelemEntryType);
            QDomText domtextEntryType = i_domdoc.createTextNode( objPoolEntryType2Str(pTreeEntryChild->getEntryType()) );
            domelemEntryType.appendChild(domtextEntryType);

            if( pTreeEntryChild->getEntryType() == EObjPoolEntryTypeObject )
            {
                QDomElement domelemNameSpace = i_domdoc.createElement("NameSpace");
                domelem.appendChild(domelemNameSpace);
                QDomText domtextNameSpace = i_domdoc.createTextNode( pTreeEntryChild->getParentNameSpace(false) );
                domelemNameSpace.appendChild(domtextNameSpace);

                QDomElement domelemObjName = i_domdoc.createElement("ObjName");
                domelem.appendChild(domelemObjName);
                QDomText domtextObjName = i_domdoc.createTextNode( pTreeEntryChild->getNodeName(false) );
                domelemObjName.appendChild(domtextObjName);
            }
            else
            {
                QDomElement domelemNameSpace = i_domdoc.createElement("ParentNameSpace");
                domelem.appendChild(domelemNameSpace);
                QDomText domtextNameSpace = i_domdoc.createTextNode( pTreeEntryChild->getParentNameSpace(false) );
                domelemNameSpace.appendChild(domtextNameSpace);

                QDomElement domelemNodeName = i_domdoc.createElement("NodeName");
                domelem.appendChild(domelemNodeName);
                QDomText domtextNodeName = i_domdoc.createTextNode( pTreeEntryChild->getNodeName(false) );
                domelemNodeName.appendChild(domtextNodeName);
            }

            if( pTreeEntryChild->getEntryType() == EObjPoolEntryTypeNameSpace )
            {
                saveTreeEntry(i_domdoc,i_domelemBody,pTreeEntryChild);
            }
        }
    }

} // saveTreeEntry

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::addTreeEntry(
    const QString&    i_strNameSpace,
    EObjPoolEntryType i_entryType,
    EObjState         i_objState )
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    QStringList strListNodeNames;
    int         idxNodeName;
    QString     strParentNameSpace;
    QString     strNodeName;

    // Create list of node names:
    if( !i_strNameSpace.isEmpty() )
    {
        strListNodeNames = i_strNameSpace.split(m_strNodeSeparator,QString::SkipEmptyParts);
    }
    if( strListNodeNames.count() <= 1 )
    {
        strNodeName = i_strNameSpace;
    }
    else // if( strListNodeNames.count() > 1 )
    {
        strNodeName = strListNodeNames[strListNodeNames.count()-1];
        strListNodeNames.removeAt(strListNodeNames.count()-1);

        for( idxNodeName = 0; idxNodeName < strListNodeNames.count()-1; idxNodeName++ )
        {
            strParentNameSpace += strListNodeNames[idxNodeName] + m_strNodeSeparator;
        }
        strParentNameSpace += strListNodeNames[idxNodeName];
    }

    return addTreeEntry(
        /* strParentNameSpace */ strParentNameSpace,
        /* strNodeName        */ strNodeName,
        /* entryType          */ i_entryType,
        /* objState           */ i_objState );

} // addTreeEntry

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::addTreeEntry(
    const QString&    i_strParentNameSpace,
    const QString&    i_strNodeName,
    EObjPoolEntryType i_entryType,
    EObjState         i_objState )
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    QStringList               strListNodeNames;
    int                       idxNodeName = 0;
    QString                   strParentNameSpace;
    QString                   strNodeName;
    CObjPoolTreeEntry*        pTreeEntryParent = m_pTreeObjs;
    const CObjPoolTreeEntry*  pTreeEntryChild;
    CObjPoolTreeEntry*        pTreeEntryNew;
    const CObjPoolTreeEntry*  pTreeEntryTmp;
    CObjPoolTreeEntry*        pTreeEntry = nullptr;
    int                       iRow;
    //QModelIndex               modelIdxParent;

    strParentNameSpace = "";

    // Create list of node names:
    if( !i_strParentNameSpace.isEmpty() )
    {
        strListNodeNames = i_strParentNameSpace.split(m_strNodeSeparator,QString::SkipEmptyParts);
    }
    if( strListNodeNames.count() > 0 && strListNodeNames[0] == m_pTreeObjs->getNodeName(false) )
    {
        strListNodeNames.removeAt(0);
    }

    // For all node names of the parent name space ...
    while( idxNodeName < strListNodeNames.count() )
    {
        strNodeName = strListNodeNames[idxNodeName];

        // Search current node name in the child list of the current node ...
        pTreeEntryChild = nullptr;
        for( iRow = 0; iRow < static_cast<int>(pTreeEntryParent->getChildCount()); iRow++ )
        {
            pTreeEntryTmp = pTreeEntryParent->getChildEntry(iRow);

            // If the node name has already been added ...
            if( pTreeEntryTmp->getEntryType() == EObjPoolEntryTypeNameSpace
             && pTreeEntryTmp->getNodeName(false) == strNodeName )
            {
                // ... further on continue with next section in the object name space string.
                pTreeEntryChild = pTreeEntryTmp;
                break;
            }
        }

        // Child not found ..
        if( pTreeEntryChild == nullptr )
        {
            // Need to add the new name space to the tree starting with the current
            // node index within the splitted list of name spaces.
            break;
        }

        // The found child becomes the new current entry in which the next
        // namespace (string section) is searched as a child.
        pTreeEntryParent = const_cast<CObjPoolTreeEntry*>(pTreeEntryChild);

        // Assemble the name of the next parent name space.
        if( strParentNameSpace.isEmpty() )
        {
            strParentNameSpace = strNodeName;
        }
        else
        {
            strParentNameSpace += m_strNodeSeparator + strNodeName;
        }

        // Next node.
        idxNodeName++;
    }

    // Starting with the node within the splitted list of node names of the parent
    // name space which have not been found within the object tree all not added
    // nodes of the parent name space will be added as childs to the object tree.
    for( ; idxNodeName < strListNodeNames.count(); idxNodeName++ )
    {
        strNodeName = strListNodeNames[idxNodeName];
        //modelIdxParent = pTreeEntryParent->getModelIdx();

        pTreeEntryNew = new CObjPoolTreeEntry(
            /* pObjPool    */ this,
            /* objType     */ EObjPoolEntryTypeNameSpace,
            /* strNodeName */ strNodeName );

        if( m_bSortTreeEntriesByName )
        {
            iRow = pTreeEntryParent->insertChildEntry(pTreeEntryNew); // the row id will be implicitly set on inserting the child entry
        }
        else
        {
            iRow = pTreeEntryParent->appendChildEntry(pTreeEntryNew); // the row id will be implicitly set on inserting the child entry
        }
        pTreeEntryNew->setParentEntry(pTreeEntryParent);

        emit nodeInserted(this,pTreeEntryNew);
        emit nameSpaceNodeInserted(this,pTreeEntryNew);

        // The current entry becomes the parent of the next not added parent node.
        pTreeEntryParent = pTreeEntryNew;

        // Assemble the name of the next parent name space.
        if( strParentNameSpace.isEmpty() )
        {
            strParentNameSpace = strNodeName;
        }
        else
        {
            strParentNameSpace += m_strNodeSeparator + strNodeName;
        }
    }

    // If an entry with the object name has not yet already been added ..
    if( pTreeEntry == nullptr )
    {
        //modelIdxParent = pTreeEntryParent->getModelIdx();
        iRow = pTreeEntryParent->getChildCount();

        // .. add the new object node entry to the tree.
        pTreeEntry = new CObjPoolTreeEntry(
            /* pObjPool    */ this,
            /* objType     */ i_entryType,
            /* strNodeName */ i_strNodeName );
        pTreeEntry->setObjState(i_objState);

        // Here we set the object reference directly at the tree
        // entry as we are going to emit the "nodeInserted" signals.

        if( m_bSortTreeEntriesByName )
        {
            iRow = pTreeEntryParent->insertChildEntry(pTreeEntry); // the row id will be implicitly set on inserting the child entry
        }
        else
        {
            iRow = pTreeEntryParent->appendChildEntry(pTreeEntry); // the row id will be implicitly set on inserting the child entry
        }
        pTreeEntry->setParentEntry(pTreeEntryParent);

        if( i_entryType == EObjPoolEntryTypeNameSpace )
        {
            // Please note that for object entry types the signals
            // "nodeInserted" and "objectNodeInserted" will be emitted
            // by the public method this method has been called from.
            emit nodeInserted(this,pTreeEntry);
            emit nameSpaceNodeInserted(this,pTreeEntry);
        }
    }

    return pTreeEntry;

} // addTreeEntry

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPool::addTreeEntry(
    CObjPoolTreeEntry* i_pTreeEntryParent,
    EObjPoolEntryType  i_entryType,
    const QString&     i_strNodeName,
    EObjState          i_objState )
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    CObjPoolTreeEntry*       pTreeEntryParent = i_pTreeEntryParent;
    QString                  strParentNameSpace;
    CObjPoolTreeEntry*       pTreeEntry;
    const CObjPoolTreeEntry* pTreeEntryTmp;
    int                      iRow;
    //QModelIndex              modelIdxParent;
    //QModelIndex              modelIdxTopLeft;
    //QModelIndex              modelIdxBottomRight;

    if( i_pTreeEntryParent == nullptr )
    {
        pTreeEntryParent = m_pTreeObjs;
    }

    // Search node name in the child list of the parent ...
    pTreeEntry = nullptr;
    for( iRow = 0; iRow < static_cast<int>(pTreeEntryParent->getChildCount()); iRow++ )
    {
        pTreeEntryTmp = pTreeEntryParent->getChildEntry(iRow);

        if( pTreeEntryTmp->getEntryType() == i_entryType
         && pTreeEntryTmp->getNodeName(false) == i_strNodeName )
        {
            pTreeEntry = const_cast<CObjPoolTreeEntry*>(pTreeEntryTmp);
            break;
        }
    }

    // If the node has not yet been added ..
    if( pTreeEntry == nullptr )
    {
        if( !(pTreeEntryParent == m_pTreeObjs) )
        {
            // "Root" will not be part of the parent name space.
            strParentNameSpace = pTreeEntryParent->getNodeName(false);
        }
        //modelIdxParent = pTreeEntryParent->getModelIdx();
        iRow = pTreeEntryParent->getChildCount();

        // Create the new node and add the node to the tree.
        pTreeEntry = new CObjPoolTreeEntry(
            /* pObjPool    */ this,
            /* entryType   */ i_entryType,
            /* strNodeName */ i_strNodeName );
        pTreeEntry->setObjState(i_objState);

        // Here we set the object reference directly at the tree
        // entry as we are going to emit the "nodeInserted" signals.

        if( m_bSortTreeEntriesByName )
        {
            iRow = pTreeEntryParent->insertChildEntry(pTreeEntry); // the row id will be implicitly set on inserting the child entry
        }
        else
        {
            iRow = pTreeEntryParent->appendChildEntry(pTreeEntry); // the row id will be implicitly set on inserting the child entry
        }
        pTreeEntry->setParentEntry(pTreeEntryParent);

        if( i_entryType == EObjPoolEntryTypeNameSpace )
        {
            // Please note that for object entry types the signals
            // "nodeInserted" and "objectNodeInserted" will be emitted
            // by the public method this method has been called from.
            emit nodeInserted(this,pTreeEntry);
            emit nameSpaceNodeInserted(this,pTreeEntry);
        }
    }

    return pTreeEntry;

} // addTreeEntry

//------------------------------------------------------------------------------
CObjPoolListEntry* CObjPool::addListEntry(
    CObjPoolTreeEntry* i_pTreeEntry,
    const QString&     i_strNameSpace,
    const QString&     i_strObjName,
    EObjState          i_objState )
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    CObjPoolListEntry* pListEntry = nullptr;

    pListEntry = new CObjPoolListEntry(
        /* pObjPool     */ this,
        /* pTreeEntry   */ i_pTreeEntry,
        /* strNameSpace */ i_strNameSpace,
        /* strObjName   */ i_strObjName );
    pListEntry->setObjState(i_objState);

    // Here we set the object reference directly at the list
    // entry as we are going to emit the "nodeInserted" signals.

    m_arpListObjs.append(pListEntry);

    return pListEntry;

} // addListEntry

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CObjPool::clearTreeEntry( CObjPoolTreeEntry* i_pTreeEntry )
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
        int                iRow;

        // Starting from end of list to first entry so there is
        // no need to correct the row id's of the childs ..
        for( iRow = static_cast<int>(i_pTreeEntry->getChildCount()-1); iRow >= 0; iRow-- )
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

            if( m_pTreeEntryCurr == pTreeEntryChild )
            {
                m_pTreeEntryCurr = nullptr;
            }

            delete pTreeEntryChild;
            pTreeEntryChild = nullptr;

        } // for( iRow = static_cast<int>(i_pTreeEntry->getChildCount()-1); iRow >= 0; iRow-- )

    } // if( i_pTreeEntry->getChildCount() > 0 )

} // clearTreeEntry


/*******************************************************************************
class CObjPoolTreeEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjPoolTreeEntry::CObjPoolTreeEntry( CObjPool* i_pObjPool ) :
//------------------------------------------------------------------------------
    m_pObjPool(i_pObjPool),
    m_entryType(EObjPoolEntryTypeUndefined),
    m_strNodeName(),
    m_objState(EObjState::Undefined),
    m_iObjId(-1),
    m_pvObj(nullptr),
    m_pEntryParent(nullptr),
    m_iRow(-1),
    m_arpChilds(),
    //m_modelIdx(),
    m_styleState(0)
{
} // default ctor

//------------------------------------------------------------------------------
CObjPoolTreeEntry::CObjPoolTreeEntry(
    CObjPool*         i_pObjPool,
    EObjPoolEntryType i_entryType,
    const QString&    i_strNodeName ) :
//------------------------------------------------------------------------------
    m_pObjPool(i_pObjPool),
    m_entryType(i_entryType),
    m_strNodeName(i_strNodeName),
    m_objState(EObjState::Undefined),
    m_iObjId(-1),
    m_pvObj(nullptr),
    m_pEntryParent(nullptr),
    m_iRow(-1),
    m_arpChilds(),
    //m_modelIdx(),
    m_styleState(0)
{
} // ctor

//------------------------------------------------------------------------------
CObjPoolTreeEntry::~CObjPoolTreeEntry()
//------------------------------------------------------------------------------
{
    m_entryType = EObjPoolEntryTypeUndefined;
    m_iObjId = -1;
    m_pvObj = nullptr;
    m_pEntryParent = nullptr;
    m_iRow = -1;
    m_arpChilds.clear();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setEntryType( EObjPoolEntryType i_entryType )
//------------------------------------------------------------------------------
{
    m_entryType = i_entryType;
}

//------------------------------------------------------------------------------
QString CObjPoolTreeEntry::getParentNameSpace( bool i_bInsertParentNames, bool i_bInsertRootNodeName ) const
//------------------------------------------------------------------------------
{
    QString strParentNameSpace;

    if( m_pEntryParent != nullptr )
    {
        strParentNameSpace = m_pEntryParent->getNodeName(false);

        if( i_bInsertParentNames )
        {
            CObjPoolTreeEntry* pTreeEntryParent = m_pEntryParent->m_pEntryParent;

            while( pTreeEntryParent != nullptr )
            {
                if( pTreeEntryParent != m_pObjPool->getRoot() )
                {
                    strParentNameSpace.insert( 0, pTreeEntryParent->getNodeName(false) + m_pObjPool->getNodeSeparator() );
                }
                else if( i_bInsertRootNodeName )
                {
                    strParentNameSpace.insert( 0, pTreeEntryParent->getNodeName(false) + m_pObjPool->getNodeSeparator() );
                }
                pTreeEntryParent = pTreeEntryParent->getParentEntry();
            }
        }
        else if( i_bInsertRootNodeName && m_pEntryParent == m_pObjPool->getRoot() )
        {
            strParentNameSpace.insert( 0, m_pEntryParent->getNodeName(false) + m_pObjPool->getNodeSeparator() );
        }
    }
    return strParentNameSpace;

} // getParentNameSpace

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setNodeName( const QString& i_strNodeName )
//------------------------------------------------------------------------------
{
    m_strNodeName = i_strNodeName;
}

//------------------------------------------------------------------------------
QString CObjPoolTreeEntry::getNodeName( bool i_bInsertParentNames, bool i_bInsertRootNodeName ) const
//------------------------------------------------------------------------------
{
    QString strNodeName = m_strNodeName;

    if( i_bInsertParentNames )
    {
        CObjPoolTreeEntry* pTreeEntryParent = m_pEntryParent;

        while( pTreeEntryParent != nullptr )
        {
            if( pTreeEntryParent != m_pObjPool->getRoot() )
            {
                strNodeName.insert( 0, pTreeEntryParent->getNodeName(false) + m_pObjPool->getNodeSeparator() );
            }
            else if( i_bInsertRootNodeName )
            {
                strNodeName.insert( 0, pTreeEntryParent->getNodeName(false) + m_pObjPool->getNodeSeparator() );
            }
            pTreeEntryParent = pTreeEntryParent->getParentEntry();
        }
    }
    else if( i_bInsertRootNodeName && this == m_pObjPool->getRoot() )
    {
        strNodeName = getNodeName(false);
    }
    return strNodeName;

} // getNodeName

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setObjState( EObjState i_objState )
//------------------------------------------------------------------------------
{
    m_objState = i_objState;
}

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setObjId( int i_iObjId )
//------------------------------------------------------------------------------
{
    m_iObjId = i_iObjId;
}

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setObj( void* i_pvObj )
//------------------------------------------------------------------------------
{
    m_pvObj = i_pvObj;

    if( m_pvObj == nullptr )
    {
        m_objState = EObjState::Destroyed;
    }
    else
    {
        m_objState = EObjState::Created;
    }

} // setObj

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setParentEntry( CObjPoolTreeEntry* i_pParentEntry )
//------------------------------------------------------------------------------
{
    m_pEntryParent = i_pParentEntry;
}

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setRowId( int i_iRow )
//------------------------------------------------------------------------------
{
    m_iRow = i_iRow;
}

//------------------------------------------------------------------------------
int CObjPoolTreeEntry::appendChildEntry( CObjPoolTreeEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    int iRow = m_arpChilds.size();

    m_arpChilds.append(i_pEntry);

    if( i_pEntry != nullptr )
    {
        i_pEntry->setRowId(iRow);
    }
    return iRow;

} // appendChildEntry

//------------------------------------------------------------------------------
int CObjPoolTreeEntry::insertChildEntry( CObjPoolTreeEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    int iRow = m_arpChilds.size();

    // Insert the entry to the list sorted by name:
    CObjPoolTreeEntry* pEntryExist;
    int                iRowTmp;

    // Name space entries should be indicated before object node entries and the entries should be sorted  names ...
    for( iRowTmp = 0; iRowTmp < m_arpChilds.size(); iRowTmp++ )
    {
        pEntryExist = m_arpChilds[iRowTmp];

        if( pEntryExist != nullptr )
        {
            if( i_pEntry->getEntryType() == EObjPoolEntryTypeNameSpace && pEntryExist->getEntryType() == EObjPoolEntryTypeObject )
            {
                iRow = iRowTmp;
                break;
            }
            else if( i_pEntry->getEntryType() == EObjPoolEntryTypeObject && pEntryExist->getEntryType() == EObjPoolEntryTypeNameSpace )
            {
            }
            else // if( i_pEntry->getEntryType() == pEntryExist->getEntryType() )
            {
                if( pEntryExist->getNodeName(false).compare(i_pEntry->getNodeName(false),Qt::CaseInsensitive) > 0 )
                {
                    iRow = iRowTmp;
                    break;
                }
            }
        }
    }

    // Add one "empty" entry at the end of the list.
    m_arpChilds.append(nullptr);

    // "Move" existing entries behind new entry one step downward ..
    if( m_arpChilds.size() > 1 )
    {
        for( iRowTmp = m_arpChilds.size()-2; iRowTmp >= iRow; iRowTmp-- )
        {
            pEntryExist = m_arpChilds[iRowTmp];

            if( pEntryExist != nullptr )
            {
                pEntryExist->setRowId(iRowTmp+1);
            }
            m_arpChilds[iRowTmp+1] = pEntryExist;
        }
    }

    if( iRow < m_arpChilds.size() )
    {
        m_arpChilds[iRow] = i_pEntry;
    }
    else
    {
        m_arpChilds.append(i_pEntry);
    }

    if( i_pEntry != nullptr )
    {
        i_pEntry->setRowId(iRow);
    }
    return iRow;

} // insertChildEntry

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::removeChildEntry( int i_iRow )
//------------------------------------------------------------------------------
{
    if( i_iRow < 0 || i_iRow >= m_arpChilds.size() )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    //const QAbstractItemModel* pModel;
    //QModelIndex               modelIdx;
    CObjPoolTreeEntry*        pEntryExist;
    int                       iRow;

    // "Move" entries behind removed entry one step forward ..
    for( iRow = i_iRow; iRow < m_arpChilds.size()-1; iRow++ )
    {
        m_arpChilds[iRow] = m_arpChilds[iRow+1];

        pEntryExist = m_arpChilds[iRow];

        if( pEntryExist != nullptr )
        {
            pEntryExist->setRowId(iRow);

            //modelIdx = pEntryExist->getModelIdx();

            //pModel = modelIdx.model();

            //if( pModel != nullptr )
            //{
            //    modelIdx = pModel->createIndex( iRow, modelIdx.column(), pEntryExist );
            //}
            //pEntryExist->setModelIdx(modelIdx);
        }
    }

    m_arpChilds.remove(m_arpChilds.size()-1);

} // removeChildEntry

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setChildEntry( int i_iRow, CObjPoolTreeEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    if( i_iRow < 0 || i_iRow >= m_arpChilds.size() )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    m_arpChilds[i_iRow] = i_pEntry;

} // setChildEntry

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPoolTreeEntry::getChildEntry( int i_iRow )
//------------------------------------------------------------------------------
{
    if( i_iRow < 0 || i_iRow >= m_arpChilds.size() )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }
    return m_arpChilds[i_iRow];

} // getChildEntry

//------------------------------------------------------------------------------
CObjPoolTreeEntry* CObjPoolTreeEntry::findChildEntry( const QString& i_strNodeName )
//------------------------------------------------------------------------------
{
    CObjPoolTreeEntry* pTreeEntry = nullptr;
    int                idxRow;

    for( idxRow = 0; idxRow < m_arpChilds.size(); idxRow++ )
    {
        if( m_arpChilds[idxRow] != nullptr && m_arpChilds[idxRow]->getNodeName(false) == i_strNodeName )
        {
            pTreeEntry = m_arpChilds[idxRow];
            break;
        }
    }
    return pTreeEntry;

} // findChildEntry

////------------------------------------------------------------------------------
//void CObjPoolTreeEntry::setModelIdx( const QModelIndex& i_modelIdx )
////------------------------------------------------------------------------------
//{
//    m_modelIdx = i_modelIdx;
//}

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::dataChanged()
//------------------------------------------------------------------------------
{
    if( m_pObjPool != nullptr )
    {
        m_pObjPool->dataChanged(this);
    }
}

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setStyleState( quint32 i_styleState )
//------------------------------------------------------------------------------
{
    m_styleState = i_styleState;
}

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::setStyleStateFlag( quint32 i_styleStateFlag )
//------------------------------------------------------------------------------
{
    m_styleState |= i_styleStateFlag;
}

//------------------------------------------------------------------------------
void CObjPoolTreeEntry::resetStyleStateFlag( quint32 i_styleStateFlag )
//------------------------------------------------------------------------------
{
    m_styleState &= (~i_styleStateFlag);
}


/*******************************************************************************
class CObjPoolListEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjPoolListEntry::CObjPoolListEntry(
    CObjPool*          i_pObjPool,
    CObjPoolTreeEntry* i_pTreeEntry ) :
//------------------------------------------------------------------------------
    m_pObjPool(i_pObjPool),
    m_pTreeEntry(i_pTreeEntry),
    m_strNameSpace(),
    m_strObjName(),
    m_objState(EObjState::Undefined),
    m_iObjId(-1),
    m_pvObj(nullptr)
{
} // default ctor

//------------------------------------------------------------------------------
CObjPoolListEntry::CObjPoolListEntry(
    CObjPool*          i_pObjPool,
    CObjPoolTreeEntry* i_pTreeEntry,
    const QString&     i_strNameSpace,
    const QString&     i_strObjName ) :
//------------------------------------------------------------------------------
    m_pObjPool(i_pObjPool),
    m_pTreeEntry(i_pTreeEntry),
    m_strNameSpace(i_strNameSpace),
    m_strObjName(i_strObjName),
    m_objState(EObjState::Undefined),
    m_iObjId(-1),
    m_pvObj(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CObjPoolListEntry::~CObjPoolListEntry()
//------------------------------------------------------------------------------
{
    m_pObjPool = nullptr;
    m_pTreeEntry = nullptr;
    m_iObjId = -1;
    m_pvObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CObjPoolListEntry::setNameSpace( const QString& i_strNameSpace )
//------------------------------------------------------------------------------
{
    m_strNameSpace = i_strNameSpace;
}

//------------------------------------------------------------------------------
void CObjPoolListEntry::setObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

//------------------------------------------------------------------------------
void CObjPoolListEntry::setObjState( EObjState i_objState )
//------------------------------------------------------------------------------
{
    m_objState = i_objState;
}

//------------------------------------------------------------------------------
void CObjPoolListEntry::setObjId( int i_iObjId )
//------------------------------------------------------------------------------
{
    m_iObjId = i_iObjId;
}

//------------------------------------------------------------------------------
void CObjPoolListEntry::setObj( void* i_pvObj )
//------------------------------------------------------------------------------
{
    m_pvObj = i_pvObj;

    if( m_pvObj == nullptr )
    {
        m_objState = EObjState::Destroyed;
    }
    else
    {
        m_objState = EObjState::Created;
    }

} // setObj
