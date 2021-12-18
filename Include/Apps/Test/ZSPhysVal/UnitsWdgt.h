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

#ifndef ZSApps_TestPhysVal_UnitsWdgt_h
#define ZSApps_TestPhysVal_UnitsWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QModelIndex;
class QSplitter;
class QStackedWidget;
class QTreeView;
class QVBoxLayout;

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace PhysVal
{
class CUnitsModel;
class CWdgtUnitView;

//******************************************************************************
class CWdgtUnits : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnits( const QString& i_strObjName, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnits();
protected: // type definitions and constants
    typedef enum {
        ETreeEntryTypeUndefined                = 0,
        ETreeEntryTypeGrpRatios                = 1,
        ETreeEntryTypeGrpDataQuantities        = 2,
        ETreeEntryTypeGrpPhysScienceFields     = 3,
        ETreeEntryTypeGrpUserDefinedQuantities = 4,
        ETreeEntryTypeUntRatio                 = 5,
        ETreeEntryTypeUntDataQuantity          = 6,
        ETreeEntryTypePhysUnit                 = 7,
        ETreeEntryTypeUntUserDefinedQuantity   = 8,
        ETreeEntryTypeCount
    }   ETreeEntryType;
protected: // overridables of base class QWidget
    virtual void showEvent( QShowEvent* i_pEv );
protected slots:
    void expandedTreeView( const QModelIndex& );
    void selectedTreeEntryChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected: // instance members
    QVBoxLayout*    m_pLyt;
    QSplitter*      m_pSplitter;
    CUnitsModel*    m_pUnitsModel;
    QTreeView*      m_pTreeView;
    ETreeEntryType  m_treeEntryTypeSelected;
    QStackedWidget* m_pStackedWdgtUnitViews;
    CWdgtUnitView*  m_arpWdgtUnitViews[ETreeEntryTypeCount];

}; // class CWdgtUnits

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_UnitsWdgt_h
