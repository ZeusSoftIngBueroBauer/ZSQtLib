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

#ifndef ZSPhysValGUI_PhysUnitsWdgt_h
#define ZSPhysValGUI_PhysUnitsWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

class QModelIndex;
class QPushButton;
class QSplitter;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;

namespace GUI
{
class CWdgtUnitsTree;
class CWdgtTreeEntries;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtUnits : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtUnits"; }
public: // ctors and dtor
    CWdgtUnits(
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtUnits();
protected slots:
    void onTreeViewCurrentRowChanged(
        const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev);
protected: // instance members
    QVBoxLayout* m_pLytMain;
    QSplitter* m_pSplitter;
    CWdgtUnitsTree* m_pWdgtTreeView;
    CWdgtTreeEntries* m_pWdgtTreeEntries;

}; // class CWdgtUnits

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysUnitsWdgt_h
