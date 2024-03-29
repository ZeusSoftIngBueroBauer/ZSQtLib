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

#ifndef ZSTestGUI_TestStepIdxTreeView_h
#define ZSTestGUI_TestStepIdxTreeView_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSTestGUI/ZSTestGUIDllMain.h"

class QItemDelegate;

namespace ZS
{
namespace System
{
class CIdxTreeEntry;
}
namespace Test
{
namespace GUI
{
class CModeldxTreeTestSteps;

//******************************************************************************
class ZSTESTGUIDLL_API CTreeViewIdxTreeTestSteps : public QTreeView
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTreeViewIdxTreeTestSteps( CModeldxTreeTestSteps* i_pModel, QWidget* i_pWdgtParent = nullptr );
    virtual ~CTreeViewIdxTreeTestSteps();
protected: // overridables of base class QTreeView
    virtual void keyPressEvent( QKeyEvent* i_pEv );
    virtual void mousePressEvent( QMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv );
protected slots:
    void onHeaderCustomContextMenuRequested( const QPoint& i_pt );
protected slots:
    void onActionEnableTestGroupsTriggered( bool i_bChecked );
    void onActionDisableTestGroupsTriggered( bool i_bChecked );
protected: // instance members
    QItemDelegate*             m_pDelegate;
    QMenu*                     m_pMenuGroupContext;
    QAction*                   m_pActionTitle;
    QAction*                   m_pActionEnableTestGroups;
    QAction*                   m_pActionDisableTestGroups;
    ZS::System::CIdxTreeEntry* m_pTreeEntryGroupContextAction;

}; // class CTreeViewIdxTreeTestSteps


} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepIdxTreeView_h
