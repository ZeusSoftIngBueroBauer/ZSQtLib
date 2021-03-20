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

#ifndef ZSDraw_DlgPageSetup_h
#define ZSDraw_DlgPageSetup_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawPageSetup.h"
#include "ZSPhysVal/ZSPhysVal.h"

class QButtonGroup;
class QFrame;
class QComboBox;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QStackedWidget;
class QTimer;

class QLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingView;
class CWdgtPageSetup;

//******************************************************************************
class ZSDRAWDLL_API CDlgPageSetup : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EWidget
    {
        EWidgetDrawingPaper = 0,
        EWidgetDrawingScene = 1,
        EWidgetCount
    };
public: // instance methods
    CDlgPageSetup( CDrawingView* i_pDrawingView, QWidget* i_pWdgtParent = nullptr );
    ~CDlgPageSetup();
public: // instance methods
    void setCurrentWidget( EWidget i_wdgt );
protected: // overridables of base class QDialog
    virtual void closeEvent( QCloseEvent* i_pEv );
    virtual void showEvent( QShowEvent* i_pEv );
protected slots: // overridables of base class QDialog
    virtual void accept();
    virtual void reject();
protected slots: // instance methods
    void onBtnAcceptClicked();
    void onBtnResetClicked();
protected slots:
    void onTmrBtnsStateRefreshTimeout();
protected slots: // List Widget
    void onListWdgtCurrentRowChanged( int i_iRow );
protected slots: // Format Widgets
    void onPageSetupAttributeChanged( int i_iAttr, const QVariant& i_val );
protected: // instance methods
    void hidePopups();
private: // instance methods
    ZS::PhysVal::CPhysSize* getDrawingScenePhysSize();
    ZS::PhysVal::CPhysUnit* getDrawingScenePhysUnit();
    void updateDrawingSize();
private: // instance members
    CDrawingView*            m_pDrawingView;
    CPageSetup               m_pageSetupOld;
    CPageSetup               m_pageSetupNew;
    QVBoxLayout*             m_pLyt;
    QHBoxLayout*             m_pLytSettings;
    QListWidget*             m_pListWdgt;
    QStackedWidget*          m_pStackedWdgt;
    // Format Widgets
    QListWidgetItem*         m_arpListWdgtItems[EWidgetCount];
    CWdgtPageSetup*          m_arpWdgtsPageSetup[EWidgetCount];
    // Buttons
    QHBoxLayout*             m_pLytBtns;
    QPushButton*             m_pBtnOk;
    QPushButton*             m_pBtnAccept;
    QPushButton*             m_pBtnReset;
    QPushButton*             m_pBtnCancel;
    QTimer*                  m_pTmrBtnsStateRefresh;
    // Trace
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // CDlgPageSetup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DlgPageSetup_h
