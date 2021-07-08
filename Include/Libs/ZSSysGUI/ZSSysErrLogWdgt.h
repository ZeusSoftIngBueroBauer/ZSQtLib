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

#ifndef ZSSysGUI_ErrLogWdgt_h
#define ZSSysGUI_ErrLogWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSysGUI/ZSSysErrLogModel.h"
#include "ZSSysGUI/ZSSysGUIDllMain.h"

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QItemSelectionModel;
class QPushButton;
class QTableView;

namespace ZS
{
namespace System
{
struct SErrLogEntry;

namespace GUI
{
class CModelErrLog;
class CTableViewErrLog;

//******************************************************************************
class ZSSYSGUIDLL_API CWdgtErrLog : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtErrLog( const QString& i_strHeadline = "", QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtErrLog();
public: // instance methods
    void showColumn( CModelErrLog::EColumn i_clm );
    void hideColumn( CModelErrLog::EColumn i_clm );
protected slots:
    //void onEntryAddedInView( SErrLogEntry* i_pEntry );
    void onBtnClearTableClicked( bool i_bChecked );
    void onBtnDeleteRowsClicked( bool i_bChecked );
    void onBtnResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
protected: // instance members
    CModelErrLog*        m_pModelErrLog;
    QString              m_strHeadline;
    QVBoxLayout*         m_pLyt;
    QLabel*              m_pLblHeadline;
    CTableViewErrLog*    m_pTableView;
    QItemSelectionModel* m_pItemSelectionModel;
    QHBoxLayout*         m_pLytLineBtns;
    QPushButton*         m_pBtnClearTable;
    QPushButton*         m_pBtnDeleteRows;
    QPushButton*         m_pBtnResizeRowsAndColumnsToContents;
    QLabel*              m_pLblFileName;
    QLabel*              m_pEdtFileName;

}; // class CWdgtErrLog

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ErrLogWdgt_h
