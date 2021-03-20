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

#ifndef ZSRemCmdGUI_ObjPoolWdgt_h
#define ZSRemCmdGUI_ObjPoolWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qitemdelegate.h>
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qitemdelegate.h>
#include <QtWidgets/qwidget.h>
#endif

#include "ZSRemCmdGUI/ZSRemCmdGUIDllMain.h"
#include "ZSRemCmd/ZSRemCmdServer.h"

class QGroupBox;
class QPushButton;
class QTextEdit;
class QTreeView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace RemCmd
{
class CRemCmdObjPool;

namespace GUI
{
class CRemCmdObjPoolModel;

//******************************************************************************
class ZSREMCMDGUIDLL_API CDelegateRemCmdObjPool : public QItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CDelegateRemCmdObjPool( QObject* i_pObjParent = nullptr );
    virtual ~CDelegateRemCmdObjPool();
public: // instance methods
    bool isCheckBoxIsQueryHit(
        const QRect&       i_rectVisual,
        const QPoint&      i_ptPos,
        const QModelIndex& i_modelIdx );
    bool isCheckBoxIsEventHit(
        const QRect&       i_rectVisual,
        const QPoint&      i_ptPos,
        const QModelIndex& i_modelIdx );
public: // overridables of base class QItemDelegate
    virtual void paint(
        QPainter*                   i_pPainter,
        const QStyleOptionViewItem& i_styleOption,
        const QModelIndex&          i_modelIdx ) const;
private: // copy ctor not implemented
    CDelegateRemCmdObjPool( const CDelegateRemCmdObjPool& );
private: // assignment operator not implemented
    CDelegateRemCmdObjPool& operator = ( const CDelegateRemCmdObjPool& );
protected: // instance members
    mutable QRect m_rectChkBoxIsQuery;
    mutable QRect m_rectChkBoxIsEvent;

}; // class CDelegateRemCmdObjPool

//******************************************************************************
class ZSREMCMDGUIDLL_API CWdgtRemCmdObjPool : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtRemCmdObjPool( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtRemCmdObjPool();
signals:
    void remCmdObjSelected( const QString& i_strCmd, const QString& i_strSyntax, const QString& i_strDscr );
public: // instance methods
    void setObjPool( CRemCmdObjPool* i_pObjPool );
public: // instance methods
    QTreeView* getTreeView() { return m_pTreeViewObjPool; }
    CRemCmdObjPoolModel* getModel() { return m_pObjPoolModel; }
public slots:
    void onTreeViewObjPoolCurrentChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
    void onBtnObjPoolResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnObjPoolExpandAllClicked( bool i_bChecked );
    void onBtnObjPoolCollapseAllClicked( bool i_bChecked );
protected: // instance members
    CRemCmdObjPool*          m_pObjPool;
    CRemCmdObjPoolModel*     m_pObjPoolModel;
    QVBoxLayout*             m_pLyt;
    CDelegateRemCmdObjPool*  m_pDelegate;
    QTreeView*               m_pTreeViewObjPool;
    QHBoxLayout*             m_pLytLineObjPoolBtns;
    QPushButton*             m_pBtnObjPoolResizeRowsAndColumnsToContents;
    QPushButton*             m_pBtnObjPoolExpandAll;
    QPushButton*             m_pBtnObjPoolCollapseAll;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtRemCmdObjPool

} // namespace GUI

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmdGUI_ObjPoolWdgt_h
