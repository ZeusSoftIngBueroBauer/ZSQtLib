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

#ifndef ZSDataGUI_DataSetModel_h
#define ZSDataGUI_DataSetModel_h

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qhash.h>
#include <QtCore/qlist.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSDataGUI/ZSDataGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDataSet;
class CDataSetObject;
class CDataSetObjGroup;
//class CDSSchemaChgDscr;
//class CDSObjSchemaChgDscr;
//class CDSObjDataChgDscr;

namespace GUI
{
//class CDataSetModelNode;

//******************************************************************************
class ZSDATAGUIDLL_API CDataSetModel : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::DataGUI::DS"; }
    static QString ClassName() { return "CDataSetModel"; }
public: // type definitions and constants
    typedef enum
    {
        EColumnName = 0,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CDataSetModel( const QString& i_strObjName, CDataSet* i_pDS );
    virtual ~CDataSetModel();
private: // copy ctor not allowed
    CDataSetModel( const CDataSetModel& );
private: // assignment operator not allowed
    CDataSetModel& operator = ( const CDataSetModel& );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setDataSet( CDataSet* i_pDS );
    CDataSet* getDataSet() { return m_pDS; }
public: // instance methods
    void setViewMode( ZS::System::GUI::EViewMode i_viewMode );
    ZS::System::GUI::EViewMode getViewMode() const { return m_viewMode; }
public: // instance methods
    //CDataSetModelNode* findNodeByUId( const QString& i_strUId );
    //CDataSetModelNode* findNodeByKey( const QString& i_strKey );
protected slots:
    void onDSDestroyed( QObject* i_pDS );
protected slots:
    void onDSObjAdded( ZS::Data::CDataSetObject* i_pDSObj );
    void onDSObjAdded( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
    void onDSObjAboutToBeRemoved( ZS::Data::CDataSetObject* i_pDSObj );
    void onDSObjAboutToBeRemoved( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowVersion i_version );
protected slots:
//    void onDSStateChanged( ZS::Data::CDataSetObject* i_pDSObj );
//    void onDSSchemaChanged( ZS::Data::CDSObjSchemaChgDscr* i_pDSSchemaChangeEvArgs );
//protected slots:
//    void onDSObjRowStateChanged( ZS::Data::CDataSetObject* i_pDSObj, ZS::System::ERowState i_rowSate );
//    void onDSObjSchemaChanged( ZS::Data::CDSObjSchemaChgDscr* i_pSchema );
//    void onDSObjDataChanged( ZS::Data::CDSObjDataChgDscr* pDSObjDataChgDscr );
protected: // instance methods
    //CDataSetModelNode* addNode( CDataSetModelNode* i_pDSNodeParent, CDataSetObject* i_pDSObj, bool i_bRecursive );
    //void deleteNodeByUId( CDataSetModelNode* i_pDSNodeParent, const QString& i_strUId );
    //void deleteNodeByKey( CDataSetModelNode* i_pDSNodeParent, const QString& i_strKey );
    //void deleteNode( CDataSetModelNode* i_pDSNodeParent, CDataSetModelNode* i_pDSNode );
protected: // instance methods
    //void clearNode( CDataSetModelNode* i_pModelNode );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual QModelIndex index( int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
public: // declare protected methods of base class QAbstractItemModel as public
    QModelIndex createIndex( int i_iRow, int i_iClm, void* i_pvData = nullptr ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const;
    virtual Qt::DropActions supportedDropActions() const;
    virtual QStringList mimeTypes() const;
    virtual QMimeData* mimeData( const QModelIndexList& i_arModelIdxs ) const;
    virtual bool dropMimeData( const QMimeData* i_pMimeData, Qt::DropAction i_dropAction, int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent );
protected: // instance members
    CDataSet*                           m_pDS;
    ZS::System::GUI::EViewMode          m_viewMode;
    //CDataSetModelNode*                m_pDSNodeRoot;            // Root node (model node of data set)
    //QHash<QString,CDataSetModelNode*> m_arpHshDSNodesByUId;     // To be unique the key is the complete path of the object (incl. parent object names)
    //QHash<QString,CDataSetModelNode*> m_arpHshDSNodesByKey;     // To be unique the key is the complete path of the object (incl. parent object names)
    Trace::CTrcAdminObj*                m_pTrcAdminObj;

}; // class CDataSetModel

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUI_DataSetModel_h
