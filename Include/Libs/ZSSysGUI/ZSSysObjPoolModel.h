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

#ifndef ZSSysGUI_ObjPoolModel_h
#define ZSSysGUI_ObjPoolModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysObjPool.h"

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CModelObjPool : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EColumn {
        EColumnMin         = 0,
        EColumnNodeName    = 0,
        EColumnType        = 1,
        EColumnObjId       = 2,
        EColumnObjAddress  = 3,
        EColumnObjState    = 4,
        EColumnMax         = 4,
        EColumnCount
    };
public: // class methods
    static QIcon GetIcon( EObjPoolEntryType i_entryType );
protected: // class methods
    static bool AreIconsCreated();
    static QPixmap* GetPxmTypeNameSpaceNormalOff();
    static QPixmap* GetPxmTypeNameSpaceSelectedOff();
    static QIcon* GetIconTypeNameSpace();
    static QPixmap* GetPxmTypeObjectNormalOff();
    static QPixmap* GetPxmTypeObjectSelectedOff();
    static QIcon* GetIconTypeObject();
public: // ctors and dtor
    CModelObjPool( CObjPool* i_pObjPool, QObject* i_pObjParent = nullptr );
    virtual ~CModelObjPool();
public: // instance methods
    void setObjPool( CObjPool* i_pObjPool );
    CObjPool* getObjPool() { return m_pObjPool; }
protected: // instance methods
    void addTreeEntry( ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
protected slots:
    void onObjPoolNodeInserted( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
    void onObjPoolNodeChanged( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
    void onObjPoolNodeRemoving( QObject* i_pObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
protected: // instance members
    CObjPool* m_pObjPool;

}; // class CModelObjPool

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ObjPoolModel_h
