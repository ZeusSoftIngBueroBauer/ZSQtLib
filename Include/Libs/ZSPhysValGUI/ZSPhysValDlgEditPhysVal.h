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

#ifndef ZSPhysValGUI_DlgEditPhysVal_h
#define ZSPhysValGUI_DlgEditPhysVal_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"

class QComboBox;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
namespace GUI
{
//******************************************************************************
class ZSPHYSVALGUIDLL_API CDlgEditPhysVal : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CDlgEditPhysVal"; }
public: // ctors and dtor
    CDlgEditPhysVal(
        const CPhysVal& i_physVal,
        double i_fMin,
        double i_fMax,
        double i_iDecimals,
        QWidget* i_pWdgtParent = nullptr );
    virtual ~CDlgEditPhysVal();
public: // instance methods
    CPhysVal value() const;
protected slots: // overridables of base class QDialog
    virtual void accept();
    virtual void reject();
protected slots: // instance methods
    void onDlgBtnResetClicked();
    void onCmbUnitActivated( int i_idx );
private: // instance members
    CPhysVal m_physValOld;
    CPhysSize m_physSize;
    CPhysUnit m_physUnit;
    double m_fMin;
    double m_fMax;
    int m_iDecimals;
    QVBoxLayout* m_pLytDlg;
    QHBoxLayout* m_pLytLine;
    QLineEdit* m_pEdtVal;
    QComboBox* m_pCmbUnit;
    QHBoxLayout* m_pLytButtons;
    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnReset;
    QPushButton* m_pBtnCancel;

}; // class CDlgEditPhysVal

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_DlgEditPhysVal_h
