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

#ifndef ZSSysGUI_LoggerIdxTreeWdgt_h
#define ZSSysGUI_LoggerIdxTreeWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysAux.h"

class QAbstractItemModel;
class QModelIndex;
class QPushButton;
class QTreeView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CIdxTreeLoggers;

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CWdgtIdxTreeLoggers : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CWdgtIdxTreeLoggers"; }
public: // ctors and dtor
    CWdgtIdxTreeLoggers(CIdxTreeLoggers* i_pIdxTree, QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtIdxTreeLoggers();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
protected slots:
    void onTreeViewExpanded( const QModelIndex& i_modelIdx );
protected slots:
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnTreeViewExpandAllClicked( bool i_bChecked );
    void onBtnTreeViewCollapseAllClicked( bool i_bChecked );
protected: // instance members
    QSize               m_szBtns;
    QVBoxLayout*        m_pLytMain;
    QHBoxLayout*        m_pLytHeadLine;
    QPushButton*        m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton*        m_pBtnTreeViewExpandAll;
    QPushButton*        m_pBtnTreeViewCollapseAll;
    CIdxTreeLoggers*    m_pIdxTree;
    QAbstractItemModel* m_pModel;
    QTreeView*          m_pTreeView;

}; // class CWdgtIdxTreeLoggers

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_LoggerIdxTreeWdgt_h
