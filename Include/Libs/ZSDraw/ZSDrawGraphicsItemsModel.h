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

#ifndef ZSDraw_GraphicsItemsModel_h
#define ZSDraw_GraphicsItemsModel_h

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CModelIdxTreeGraphicsItems : public ZS::System::GUI::CModelIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CModelIdxTreeGraphicsItems"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    typedef enum
    {
        EColumnGraphObjName =  0,
        EColumnGraphObjId   =  1,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelIdxTreeGraphicsItems( ZS::System::CIdxTree* i_pIdxTree, QObject* i_pObjParent = nullptr );
    virtual ~CModelIdxTreeGraphicsItems();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
//public: // instance methods
//    void clear();
//    void update();
//public: // instance methods
//    CGraphObjModelEntry* getModelEntry( CGraphObj* i_pGraphObj ) const;
//    CGraphObjModelEntry* getModelEntry( QGraphicsItem* i_pGraphicsItem ) const;
//    CGraphObjModelEntry* getModelEntry( const QString& i_strObjId ) const;
//    QModelIndex getModelIndex( CGraphObj* i_pGraphObj ) const;
//    QModelIndex getModelIndex( QGraphicsItem* i_pGraphicsItem ) const;
//    QModelIndex getModelIndex( const QString& i_strObjId ) const;
//public: // instance methods (model entries need access as friend class to protected createIndex method)
//    QModelIndex createIndex( int i_iRow, int i_iClm, void* i_pvData = nullptr ) const { return QAbstractItemModel::createIndex(i_iRow,i_iClm,i_pvData); }
//protected: // instance methods
//    void clearModelEntry( CGraphObjModelEntry* i_pModelEntry );
//protected slots:
//    void onDrawingSceneGraphObjCreated( ZS::Draw::CGraphObj* i_pGraphObj );
//    void onDrawingSceneGraphObjDestroying( const QString& i_strObjId );
//    void onDrawingSceneGraphObjIdChanged( const QString& i_strObjIdOld, const QString& i_strObjIdNew );
//    void onDrawingSceneGraphObjNameChanged( const QString& i_strObjId, const QString& i_strObjNameOld, const QString& i_strObjNameNew );
public: // overridables of base class QAbstractItemModel
    //virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    //virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    //virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
protected: // instance members
    QIcon                              m_iconRootEntry;
    //CDrawingScene*                     m_pDrawingScene;
    //CGraphObjModelEntry*               m_pRootEntry;
    //QMap<QString,CGraphObjModelEntry*> m_dctpGraphObjs;          // sorted by name (names including parents)
    ZS::Trace::CTrcAdminObj*           m_pTrcAdminObj;

}; // class CGraphicsItemsModel

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphicsItemsModel_h