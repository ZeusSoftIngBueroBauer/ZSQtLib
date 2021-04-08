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

#ifndef ZSSys_ObjPool_h
#define ZSSys_ObjPool_h

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qvector.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"

class QDomDocument;
class QDomElement;
class QMutex;

namespace ZS
{
namespace System
{
class CObjPoolListEntry;
class CObjPoolTreeEntry;

enum EObjPoolEntryType
{
    EObjPoolEntryTypeRoot,
    EObjPoolEntryTypeObject,
    EObjPoolEntryTypeNameSpace,
    EObjPoolEntryTypeCount,
    EObjPoolEntryTypeUndefined
};
ZSSYSDLL_API QString objPoolEntryType2Str( int i_entryType );
ZSSYSDLL_API EObjPoolEntryType str2ObjPoolEntryType( const QString& i_str );

#define COBJPOOL_MUTEXLOCKER(_i_pObjPool) \
    QMutexLocker mutexLockerObjPool( (_i_pObjPool)->getMutex() )

//******************************************************************************
class ZSSYSDLL_API CObjPool : public QObject
/* Class to store references to objects which may be accessed through unique,
   sectioned names or through unique object ids.

   On adding a named object to the object pool the name of the object will be stored
   in the pool and an unique id will be assigned to the object. This unique id will
   never be changed throughout the whole life time of the system.

   On removing an object from the model the corresponding name entry will remain
   in the model and the entry will be marked as "not used" (the reference to the
   object will be invalidated (set to nullptr)). On adding the "same" object again
   (an object with the same name) the object will be inserted at the same
   place where it once has been added before. This way the "same" object
   will get the same object id through the whole life time of the system.

*******************************************************************************/
{
    Q_OBJECT
public: // class methods
    static QString BuildPathStr(
        const QString& i_str1,
        const QString& i_str2,
        const QString& i_str3 = QString(),
        const QString& i_str4 = QString(),
        const QString& i_str5 = QString() );
    static bool IsDescendant(
        const QString& i_strObjPoolName,
        const QString& i_strParentName,
        const QString& i_strChildName,
        bool           i_bRecursive,
        const QString& i_strNodeSeparator = "::" );
public: // ctors and dtor
    CObjPool(
        const QString& i_strObjPoolName,
        const QString& i_strNodeSeparator = "::",
        bool           i_bSortTreeEntriesByName = true );
    virtual ~CObjPool();
signals:
    void adminObjsFileNameChanged( QObject* i_pObjPool, const QString& i_strFileName );
    void clearing( QObject* i_pObjPool );
    void nodeInserted( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
    void nodeChanged( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
    void nodeRemoving( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
    void objectNodeInserted( QObject* i_pObjPool, ZS::System::CObjPoolListEntry* i_pListEntry );
    void objectNodeChanged( QObject* i_pObjPool, ZS::System::CObjPoolListEntry* i_pListEntry );
    void objectNodeRemoving( QObject* i_pObjPool, ZS::System::CObjPoolListEntry* i_pListEntry );
    void nameSpaceNodeInserted( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
    void nameSpaceNodeChanged( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
    void nameSpaceNodeRemoving( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
public: // instance methods
    QMutex* getMutex() { return m_pMtxObjs; }
    void lock();
    void unlock();
    void setObjPoolName( const QString& i_strObjPoolName );
    QString getObjPoolName() const { return m_strObjPoolName; }
    QString getNodeSeparator() const { return m_strNodeSeparator; }
    int getObjCount();
    CObjPoolTreeEntry* getRoot() { return m_pTreeObjs; }
public: // overridables
    virtual void clear(); // does not destroy but just invalidates the reference object
public: // instance methods
    int addObjNode(
        const QString& i_strObjPath,
        EObjState      i_objState = ZS::System::EObjStateUndefined );
    int addObjNode(
        const QString& i_strNameSpace,
        const QString& i_strObjName,
        EObjState      i_objState = ZS::System::EObjStateUndefined );
    int addObjNode(
        const QString& i_strNameSpace,
        const QString& i_strObjName,
        void*          i_pvObj );
    int addObjNode(
        const QString& i_strNameSpace,
        QString*       i_pstrObjName,
        void*          i_pvObj );
    CObjPoolTreeEntry* addObjNode(
        const QString& i_strObjPath,
        int            i_iObjId,
        EObjState      i_objState = ZS::System::EObjStateUndefined );
    CObjPoolTreeEntry* addObjNode(
        const QString& i_strNameSpace,
        const QString& i_strObjName,
        int            i_iObjId,
        EObjState      i_objState = ZS::System::EObjStateUndefined );
    CObjPoolTreeEntry* addNameSpaceNode(
        const QString& i_strNameSpace );
    CObjPoolTreeEntry* addNameSpaceNode(
        const QString& i_strParentNameSpace,
        const QString& i_strNodeName );
    CObjPoolTreeEntry* addNode(
        const QString&    i_strPath,
        EObjPoolEntryType i_entryType );
    // Please note that the "invalidateObjNode" methods are not destroying any list or tree entries.
    // Only the reference to the object will be invalidated (set to nullptr).
    void invalidateObjNode( int i_iObjId );
    void invalidateObjNode(
        const QString& i_strNameSpace,
        const QString& i_strObjName );
    void invalidateObjNode( void* i_pvObj );
public: // instance methods
    int getObjId( const QString& i_strNameSpace, const QString& i_strObjName );
    int getObjId( const QString& i_strObjPath ); // NameSpace & ObjName
    void setObjState( int i_iObjId, EObjState i_objState );
    EObjState getObjState( int i_iObjId ) const;
    void setObj( int i_iObjId, void* i_pvObj );
    void* getObj( int i_iObjId );
    void* getObj( const QString& i_strNameSpace, const QString& i_strObjName );
    void* getObj( const QString& i_strObjPath ); // NameSpace & ObjName
public: // instance methods (for experts only)
    CObjPoolListEntry* getListEntry( int i_iObjId ) const;
    CObjPoolListEntry* getListEntry( void* i_pvObj ) const;
    CObjPoolTreeEntry* getTreeEntry(
        const QString&    i_strNameSpace,
        const QString&    i_strObjName,
        EObjPoolEntryType i_entryType = ZS::System::EObjPoolEntryTypeObject ) const;
    CObjPoolTreeEntry* getTreeEntry(
        const QString&    i_strPath,
        EObjPoolEntryType i_entryType = ZS::System::EObjPoolEntryTypeObject ) const;
public: // overridables (for experts only)
    virtual void dataChanged( CObjPoolTreeEntry* i_pTreeEntry );
public: // instance methods
    void setFileName( const QString& i_strFileName );
    QString getFileName() const;
public: // overridables
    virtual SErrResult recall( const QString& i_strFileName );
    virtual SErrResult save( const QString& i_strFileName );
protected: // overridables
    virtual void saveTreeEntry(
        QDomDocument&      i_domdoc,
        QDomElement&       i_domelemBody,
        CObjPoolTreeEntry* i_pTreeEntry ) const;
protected: // instance methods
    CObjPoolTreeEntry* addTreeEntry(
        const QString&    i_strNameSpace,
        EObjPoolEntryType i_entryType,
        EObjState         i_objState = ZS::System::EObjStateUndefined );
    CObjPoolTreeEntry* addTreeEntry(
        const QString&    i_strParentNameSpace,
        const QString&    i_strNodeName,
        EObjPoolEntryType i_entryType,
        EObjState         i_objState = ZS::System::EObjStateUndefined );
    CObjPoolTreeEntry* addTreeEntry(
        CObjPoolTreeEntry* i_pTreeEntryParent,
        EObjPoolEntryType  i_entryType,
        const QString&     i_strNodeName,
        EObjState          i_objState = ZS::System::EObjStateUndefined );
    CObjPoolListEntry* addListEntry(
        CObjPoolTreeEntry* i_pTreeEntry,
        const QString&     i_strNameSpace,
        const QString&     i_strObjName,
        EObjState          i_objState = ZS::System::EObjStateUndefined );
protected: // overridables
    virtual void clearTreeEntry( CObjPoolTreeEntry* i_pTreeEntry ); // does not destroy but just invalidates the referenced object
private: // copy ctor not allowed
    CObjPool( const CObjPool& );
private: // assignment operator not allowed
    CObjPool& operator = ( const CObjPool& );
protected: // instance members
    QString                     m_strObjPoolName;
    QString                     m_strNodeSeparator;
    bool                        m_bSortTreeEntriesByName;
    QMutex*                     m_pMtxObjs;
    QVector<CObjPoolListEntry*> m_arpListObjs;
    CObjPoolTreeEntry*          m_pTreeObjs;
    mutable CObjPoolTreeEntry*  m_pTreeEntryCurr;
    QString                     m_strFileName;

}; // class CObjPool


//******************************************************************************
class ZSSYSDLL_API CObjPoolTreeEntry
/* Name space and object entries in the object tree of the object pool.
*******************************************************************************/
{
public: // ctors and dtor
    CObjPoolTreeEntry( CObjPool* i_pObjPool );
    CObjPoolTreeEntry(
        CObjPool*         i_pObjPool,
        EObjPoolEntryType i_entryType,
        const QString&    i_strNodeName );
    virtual ~CObjPoolTreeEntry();
public: // instance methods
    void setEntryType( EObjPoolEntryType i_entryType );
    EObjPoolEntryType getEntryType() const { return m_entryType; }
    QString getParentNameSpace( bool i_bInsertParentNames, bool i_bInsertRootNodeName = false ) const;
    void setNodeName( const QString& i_strNodeName );
    QString getNodeName( bool i_bInsertParentNames, bool i_bInsertRootNodeName = false ) const;
    void setObjState( EObjState i_objState );
    EObjState getObjState() const { return m_objState; }
    void setObjId( int i_iObjId );
    int getObjId() const { return m_iObjId; }
    void setObj( void* i_pvObj );
    void* getObj() { return m_pvObj; }
    void setParentEntry( CObjPoolTreeEntry* i_pParentEntry );
    CObjPoolTreeEntry* getParentEntry() { return m_pEntryParent; }
    void setRowId( int i_iRow );
    int getRowId() const { return m_iRow; }
    int getChildCount() const { return m_arpChilds.size(); }
    int appendChildEntry( CObjPoolTreeEntry* i_pEntry ); // appends the entry at the end
    int insertChildEntry( CObjPoolTreeEntry* i_pEntry ); // inserts the entry sorted by name
    void removeChildEntry( int i_iRow );
    void setChildEntry( int i_iRow, CObjPoolTreeEntry* i_pEntry );
    CObjPoolTreeEntry* getChildEntry( int i_iRow );
    CObjPoolTreeEntry* findChildEntry( const QString& i_strNodeName );
    void dataChanged();
    void setStyleState( quint32 i_styleState );
    void setStyleStateFlag( quint32 i_styleStateFlag );
    void resetStyleStateFlag( quint32 i_styleStateFlag );
    quint32 getStyleState() const { return m_styleState; }
private: // copy operator not allowed
    CObjPoolTreeEntry( const CObjPoolTreeEntry& );
private: // assignment operator not allowed
    CObjPoolTreeEntry& operator = ( const CObjPoolTreeEntry& );
protected: // instance members
    CObjPool*                   m_pObjPool;
    EObjPoolEntryType           m_entryType;
    // "entryType" may be either "Object" or "NameSpace".
    // If its a "real" object entry "ParentNameSpace" corresponds to the
    // (complete) name space (class name space, class name, object name space,
    // but without "Root") of the object (e.g. "ZS::PhysSizes::Electricity::Voltage")
    // and "NodeName" corresponds to the "real" object name (e.g. "MilliVolt").
    // If its a "NameSpace" entry "ParentNameSpace" corresponds to the parent
    // name space of the name space node (e.g. "ZS::PhysSizes::Electricity") and
    // "NodeName" corresponds to the last section of the name space (e.g. "Voltage").
    QString                     m_strNodeName;
    EObjState                   m_objState;
    // If the tree entry is a "real" object the object id corresponds to the
    // index of the corresponding entry in the list of the object pool. But
    // "pObj" !! DOES NOT !! point to the real object. For a name space tree
    // entry the member element "pObj" may be used for customized purposes
    // and may be explicitly set and read. E.g. a physical size instance which
    // is used as a name space entry whose childs are physical units may set
    // a reference to itself in the name space entry to allow searching the
    // object tree for the physical size instance by names.
    int                         m_iObjId;
    void*                       m_pvObj;
    CObjPoolTreeEntry*          m_pEntryParent;
    int                         m_iRow;
    QVector<CObjPoolTreeEntry*> m_arpChilds;
    quint32                     m_styleState;

}; // class CObjPoolTreeEntry


//******************************************************************************
class ZSSYSDLL_API CObjPoolListEntry
/* Object entries in the object list of the object pool.
*******************************************************************************/
{
public: // ctors and dtor
    CObjPoolListEntry(
        CObjPool*          i_pObjPool,
        CObjPoolTreeEntry* i_pTreeEntry );
    CObjPoolListEntry(
        CObjPool*          i_pObjPool,
        CObjPoolTreeEntry* i_pTreeEntry,
        const QString&     i_strNameSpace,
        const QString&     i_strObjName );
    virtual ~CObjPoolListEntry();
public: // instance methods
    CObjPoolTreeEntry* getTreeEntry() { return m_pTreeEntry; }
public: // instance methods
    void setNameSpace( const QString& i_strNameSpace );
    QString getNameSpace() const { return m_strNameSpace; }
    void setObjName( const QString& i_strObjName );
    QString getObjName() const { return m_strObjName; }
    void setObjState( EObjState i_objState );
    EObjState getObjState() const { return m_objState; }
    void setObjId( int i_iObjId );
    int getObjId() const { return m_iObjId; }
    void setObj( void* i_pvObj );
    void* getObj() { return m_pvObj; }
private: // copy operator not allowed
    CObjPoolListEntry( const CObjPoolListEntry& );
private: // assignment operator not allowed
    CObjPoolListEntry& operator = ( const CObjPoolListEntry& );
protected: // instance members
    CObjPool*          m_pObjPool;
    CObjPoolTreeEntry* m_pTreeEntry;
    QString            m_strNameSpace;
    QString            m_strObjName;
    EObjState          m_objState;
    int                m_iObjId;
    void*              m_pvObj;

}; // class CObjPoolListEntry

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_ObjPool_h
