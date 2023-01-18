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

#ifndef ZSDraw_DlgFormatGraphObjs_h
#define ZSDraw_DlgFormatGraphObjs_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Drawing/ZSDrawSettings.h"

class QButtonGroup;
class QFrame;
class QGroupBox;
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
namespace System
{
namespace GUI
{
class CPushButton;
}
}
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CGraphObj;
class CWdgtFormatGraphObjs;

//******************************************************************************
class ZSDRAWDLL_API CDlgFormatGraphObjs : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CDlgFormatGraphObjs"; }  // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    static const QString c_strWdgtLabels;
    static const QString c_strWdgtLineStyle;
    static const QString c_strWdgtFillStyle;
    static const QString c_strWdgtTextStyle;
    static const QString c_strWdgtGeometry;
    static const QString c_strWdgtText;
    static const QString c_strWdgtLabelVisibilities;
public: // ctors and dtor
    CDlgFormatGraphObjs(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CDlgFormatGraphObjs();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void addWidget( const QIcon& i_icon, const QString& i_strWdgtName, CWdgtFormatGraphObjs* i_pWdgt );
    void setCurrentWidget( const QString& i_strWdgtName );
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
protected: // instance methods
    void hidePopups();
    void setWindowTitle();
private: // instance members
    CDrawingScene*                       m_pDrawingScene;
    CGraphObj*                           m_pGraphObj;
    QVBoxLayout*                         m_pLyt;
    QHBoxLayout*                         m_pLytSettings;
    QListWidget*                         m_pListWdgt;
    QStackedWidget*                      m_pStackedWdgt;
    // Format Widgets
    QHash<QString,QListWidgetItem*>      m_arpListWdgtItems;
    QHash<QString,CWdgtFormatGraphObjs*> m_arpWdgtsFormatGraphObjs;
    // Buttons
    QHBoxLayout*                         m_pLytBtns;
    QPushButton*                         m_pBtnOk;
    QPushButton*                         m_pBtnAccept;
    QPushButton*                         m_pBtnReset;
    QPushButton*                         m_pBtnCancel;
    QTimer*                              m_pTmrBtnsStateRefresh;
    // Trace
    ZS::System::CTrcAdminObj*             m_pTrcAdminObj;

}; // class CDlgFormatGraphObjs

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DlgFormatGraphObjs_h
