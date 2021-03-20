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

#ifndef ZSDataGUI_DataSetWdgt_h
#define ZSDataGUI_DataSetWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDataGUI/ZSDataGUIDllMain.h"

class QMenu;
class QModelIndex;
class QPushButton;
class QSplitter;
class QTreeView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDb;
class CDbClient;
class CDataSet;

namespace GUI
{
class CDataSetModel;
//class CDataSetModelNode;
//class CDlgDataSetSchema;
//class CDlgDataParameterSchema;
//class CDlgDataTableSchema;
//class CWdgtDataSetNodeContent;

//******************************************************************************
class ZSDATAGUIDLL_API CWdgtDataSet : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::DataGUI::DS"; }
    static QString ClassName() { return "CWdgtDataSet"; }
public: // ctors and dtor
    CWdgtDataSet( const QString& i_strDSName, QWidget* i_pWdgtParent = nullptr );
    //CWdgtDataSet( CDataSet* i_pDS, QWidget* i_pWdgtParent = nullptr );
    //CWdgtDataSet( CDb* i_pDb, QWidget* i_pWdgtParent = nullptr );
    //CWdgtDataSet( CDbClient* i_pDbClt, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtDataSet();
//signals:
//    void selectedModelNodeChanged( ZS::Data::GUI::CDataSetModelNode* i_pNodeSelected );
protected: // ctor
    void init();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setDataSet( CDataSet* i_pDS );
    //void setDb( CDb* i_pDb );
    //void setDbClient( CDbClient* i_pDbClt );
public: // instance methods
    //CDataSetModelNode* getDSModelNodeSelected() { return m_pDSModelNodeSelected; }
protected slots: // connected to the signals of the edit buttons
    void onBtnAddGroupClicked( bool i_bChecked = false );
    void onBtnAddEnumerationClicked( bool i_bChecked = false );
    void onBtnAddTableClicked( bool i_bChecked = false );
    void onBtnAddParameterClicked( bool i_bChecked = false );
    void onBtnDeleteClicked( bool i_bChecked = false );
    void onBtnEditClicked( bool i_bChecked = false );
protected slots: // connected to the signals of the tree view
    void onTreeViewContextMenuRequested( const QPoint& i_pt );
    void onTreeViewExpanded( const QModelIndex& );
    void onTreeViewDoubleClicked( const QModelIndex& i_modelIdx );
    void onTreeViewCurrentChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected: // instance members
    QString                    m_strDSName;
    CDataSet*                  m_pDS;
    CDb*                       m_pDb;
    CDbClient*                 m_pDbClt;
    CDataSetModel*             m_pDSModel;
    //CDataSetModelNode*       m_pDSModelNodeSelected;
    QVBoxLayout*               m_pLyt;
    QSplitter*                 m_pSplitter;
    QWidget*                   m_pWdgtTreeView;
    QVBoxLayout*               m_pLytTreeView;
    QHBoxLayout*               m_pLytToolBarEdit;
    QPushButton*               m_pBtnAddGroup;
    QPushButton*               m_pBtnAddEnumeration;
    QPushButton*               m_pBtnAddTable;
    QPushButton*               m_pBtnAddParameter;
    QPushButton*               m_pBtnDelete;
    QPushButton*               m_pBtnEdit;
    QTreeView*                 m_pTreeView;
    //CWdgtDataSetNodeContent* m_pWdgtNodeContent;
    QMenu*                     m_pMnuContextEdit;
    //CDlgDataSetSchema*       m_pDlgDSEditSchema;
    //CDlgDataParameterSchema* m_pDlgDPEditSchema;
    //CDlgDataTableSchema*     m_pDlgDTEditSchema;
    Trace::CTrcAdminObj*       m_pTrcAdminObj;

}; // class CWdgtDataSet

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUI_DataSetWdgt_h
