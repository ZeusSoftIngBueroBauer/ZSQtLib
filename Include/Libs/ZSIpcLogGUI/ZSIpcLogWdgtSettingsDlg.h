/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSIpcLogGUI_LogWdgtSettingsDlg_h
#define ZSIpcLogGUI_LogWdgtSettingsDlg_h

#include "ZSIpcLogGUI/ZSIpcLogGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QCheckBox;
class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Log
{
namespace GUI
{
class CWdgtLog;

//******************************************************************************
class ZSIPCLOGGUIDLL_API CDlgWdgtLogSettings : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Log::GUI"; }
    static QString ClassName() { return "CDlgWdgtLogSettings"; }
public: // class methods
    static CDlgWdgtLogSettings* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgWdgtLogSettings* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgWdgtLogSettings(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgWdgtLogSettings();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // instance methods
    void setLogWidget( CWdgtLog* i_pWdgtLog );
protected slots:
    void onChkShowTimeInfoToggled( bool i_bChecked );
    void onBtnApplyClicked( bool i_bChecked );
    void onBtnOkClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
protected: // auxiliary methods
    bool hasChanges() const;
    void applyChanges();
protected: // instance members
    CWdgtLog*     m_pWdgtLog;
    QVBoxLayout*  m_pLyt;
    QHBoxLayout*  m_pLytLineShowTimeInfo;
    QLabel*       m_pLblShowTimeInfo;
    QCheckBox*    m_pChkShowTimeInfo;
    QHBoxLayout*  m_pLytBtns;
    QPushButton*  m_pBtnApply;
    QPushButton*  m_pBtnOk;
    QPushButton*  m_pBtnCancel;

}; // class CDlgWdgtLogSettings

} // namespace GUI

} // namespace Log

} // namespace ZS

#endif // #ifndef ZSIpcLogGUI_LogWdgtSettingsDlg_h
