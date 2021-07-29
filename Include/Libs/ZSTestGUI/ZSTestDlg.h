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

#ifndef ZSTestGUI_Dlg_h
#define ZSTestGUI_Dlg_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

#include "ZSTestGUI/ZSTestGUIDllMain.h"

class QVBoxLayout;

namespace ZS
{
namespace Test
{
class CTest;

namespace GUI
{
class CWdgtTest;

//******************************************************************************
class ZSTESTGUIDLL_API CDlgTest : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CDlgTest(
        const QString&  i_strObjName,
        CTest*          i_pTest = nullptr,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CDlgTest();
public: // instance methods
    void show();
    void hide();
protected: // overridables of base class QDialog
    virtual void closeEvent( QCloseEvent* i_pEv );
protected: // instance members
    QString      m_strSettingsKey;
    QVBoxLayout* m_pLytMain;
    CWdgtTest*   m_pWdgtTest;

}; // class CDlgTest

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_Wdgt_h
