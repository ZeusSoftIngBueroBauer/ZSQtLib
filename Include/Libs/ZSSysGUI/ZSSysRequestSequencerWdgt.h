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

#ifndef ZSSysGUI_RequestSequencerWdgt_h
#define ZSSysGUI_RequestSequencerWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

class QHBoxLayout;
class QVBoxLayout;
class QItemSelectionModel;
class QLabel;
class QPushButton;
class QTreeView;

namespace ZS
{
namespace System
{
class CRequestSequencer;

namespace GUI
{
class CModelRequestSequencer;

//******************************************************************************
class ZSSYSGUIDLL_API CWdgtRequestSequencer : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtRequestSequencer(
        const QString& i_strHeadline = "",
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtRequestSequencer();
public: // instance methods (content of edit fields)
    void setSequencer( CRequestSequencer* i_pReqSeq );
protected slots:
    void onBtnClearClicked( bool i_bChecked );
    void onBtnDeleteRowClicked( bool i_bChecked );
    void onBtnResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnExpandAllClicked( bool i_bChecked );
    void onBtnCollapseAllClicked( bool i_bChecked );
protected: // instance members
    CModelRequestSequencer* m_pModelRequests;
    QString                 m_strHeadline;
    QVBoxLayout*            m_pLyt;
    QLabel*                 m_pLblHeadline;
    QTreeView*              m_pTreeView;
    QItemSelectionModel*    m_pItemSelectionModel;
    QHBoxLayout*            m_pLytLineBtns;
    QPushButton*            m_pBtnClear;
    QPushButton*            m_pBtnDeleteRow;
    QPushButton*            m_pBtnResizeRowsAndColumnsToContents;
    QPushButton*            m_pBtnExpandAll;
    QPushButton*            m_pBtnCollapseAll;

}; // class CWdgtRequestSequencer

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_RequestSequencerWdgt_h
