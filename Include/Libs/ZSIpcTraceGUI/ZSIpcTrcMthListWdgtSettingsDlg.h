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

#pragma once

#include "ZSIpcTraceGUI/ZSIpcTrcGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QCheckBox;
class QLabel;
class QPushButton;
class QSpinBox;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS::Trace::GUI
{
class CWdgtTrcMthList;

//******************************************************************************
class ZSIPCTRACEGUIDLL_API CDlgWdgtTrcMthListSettings : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Trace::GUI"; }
    static QString ClassName() { return "CDlgWdgtTrcMthListSettings"; }
public: // class methods
    static CDlgWdgtTrcMthListSettings* CreateInstance(
        const QString& i_strDlgTitle,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgWdgtTrcMthListSettings* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgWdgtTrcMthListSettings(
        const QString& i_strDlgTitle,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgWdgtTrcMthListSettings();
public: // instance methods
    void setTraceMethodListWidget(CWdgtTrcMthList* i_pWdgtTrcMthList);
protected slots:
    void onChkShowTimeInfoToggled(bool i_bChecked);
    void onEdtMaxEditItemsValueChanged(int i_iVal);
    void onBtnApplyClicked(bool i_bChecked);
    void onBtnOkClicked(bool i_bChecked);
    void onBtnCancelClicked(bool i_bChecked);
protected: // auxiliary methods
    bool hasChanges() const;
    void applyChanges();
protected: // instance members
    CWdgtTrcMthList* m_pWdgtTrcMthList = nullptr;
    QVBoxLayout* m_pLyt = nullptr;
    QHBoxLayout* m_pLytLineShowTimeInfo = nullptr;
    QLabel* m_pLblShowTimeInfo = nullptr;
    QCheckBox* m_pChkShowTimeInfo = nullptr;
    QHBoxLayout* m_pLytLineMaxEditItems = nullptr;
    QLabel* m_pLblMaxEditItems = nullptr;
    QSpinBox* m_pEdtMaxEditItems = nullptr;
    QHBoxLayout* m_pLytBtns = nullptr;
    QPushButton* m_pBtnApply = nullptr;
    QPushButton* m_pBtnOk = nullptr;
    QPushButton* m_pBtnCancel = nullptr;
};

} // ZS::Trace::GUI
