/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestObjPoolModel_WidgetCentral_h
#define ZSApps_TestObjPoolModel_WidgetCentral_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QTableView;
class QTreeView;
class QSplitter;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CObjPool;

namespace GUI
{
class CModelObjPool;
class CWdgtErrLog;
}
}

namespace Apps
{
namespace Test
{
namespace ObjPoolModel
{
//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CWidgetCentral* GetInstance() { return s_pThis; } // singleton class
public: // ctors and dtor
    CWidgetCentral(
        ZS::System::CObjPool* i_pObjPool,
        QWidget*              i_pWdgtParent = nullptr,
        Qt::WindowFlags       i_wflags = 0 );
    ~CWidgetCentral();
public: // instance members
    QTreeView* getTreeView() { return m_pTreeView; }
protected: // class members
    static CWidgetCentral* s_pThis; // singleton class
protected: // instance members
    QVBoxLayout*                    m_pLyt;
    QSplitter*                      m_pSplitter;
    ZS::System::CObjPool*           m_pObjPool;
    ZS::System::GUI::CModelObjPool* m_pObjPoolModel;
    QTreeView*                      m_pTreeView;
    ZS::System::GUI::CWdgtErrLog*   m_pWdgtErrLog;

}; // class CWidgetCentral

} // namespace ObjPoolModel

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestObjPoolModel_WidgetCentral_h
