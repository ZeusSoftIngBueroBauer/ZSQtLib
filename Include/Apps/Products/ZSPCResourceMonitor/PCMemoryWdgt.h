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

#ifndef ZSApps_PCResourceMonitor_MemoryWdgt_h
#define ZSApps_PCResourceMonitor_MemoryWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDiagram/ZSDiagramAux.h"

class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QSplitter;
class QTableView;
class QTextEdit;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Diagram
{
class CWdgtDiagram;
class CDiagScale;
class CDiagScale;
class CDiagTrace;
class CDiagObjGrid;
class CDiagObjAxisLabel;
class CDiagObjCurve;
class CDiagObjLine;
class CDiagObjMarker;

} // namespace Diagram

namespace Apps
{
namespace Products
{
namespace PCResourceMonitor
{
class CPCResourceMonitor;
class CModelMemory;
class CModelMemoryValues;

//******************************************************************************
class CWdgtPCMemory : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtPCMemory(
        CPCResourceMonitor* i_pPCResourceMonitor,
        QWidget*            i_pWdgtParent = nullptr );
    virtual ~CWdgtPCMemory();
protected slots: // connected to the signals of my user controls
    void onChkMemoryValuesShowAllToggled( bool i_bChecked );
    void onCmbMemoryValuesUnitCurrentIndexChanged( const QString& i_strSymbol );
protected slots: // connected to the signals of the standard PC client
    void onCltStdPCMemoryChanged();
    void onCltStdPCMemoryValuesChanged();
protected: // instance members
    CPCResourceMonitor*             m_pPCResourceMonitor;
    QVBoxLayout*                    m_pLyt;
    QGroupBox*                      m_pGrpEquipment;
    QVBoxLayout*                    m_pLytGrpEquipment;
    QHBoxLayout*                    m_pLytLinePhysicalMemory;
    QLabel*                         m_pLblPhysicalMemorySize;
    QLabel*                         m_pEdtPhysicalMemorySize;
    QHBoxLayout*                    m_pLytLineVirtualMemory;
    QLabel*                         m_pLblVirtualMemorySize;
    QLabel*                         m_pEdtVirtualMemorySize;
    CModelMemory*                   m_pModelMemory;
    QGroupBox*                      m_pGrpLoadValues;
    QHBoxLayout*                    m_pLytGrpLoadValues;
    QSplitter*                      m_pSplitterLoadValues;
    QWidget*                        m_pWdgtMemoryValues;
    QVBoxLayout*                    m_pLytMemoryValues;
    QHBoxLayout*                    m_pLytMemoryValuesShowAll;
    QLabel*                         m_pLblMemoryValuesShowAll;
    QCheckBox*                      m_pChkMemoryValuesShowAll;
    QString                         m_strDataQuantityUnit;
    QLabel*                         m_pLblMemoryValuesUnit;
    QComboBox*                      m_pCmbMemoryValuesUnit;
    CModelMemoryValues*             m_pModelMemoryValues;
    QTableView*                     m_pTableViewMemoryValues;
    ZS::Diagram::CWdgtDiagram*      m_pWdgtDiagram;
    ZS::Diagram::SScale             m_scaleX;
    ZS::Diagram::SScale             m_scaleY;
    ZS::Diagram::CDiagScale*        m_pDiagScaleX;
    ZS::Diagram::CDiagScale*        m_pDiagScaleY;
    ZS::Diagram::CDiagObjGrid*      m_pDiagObjGrid;
    ZS::Diagram::CDiagObjAxisLabel* m_pDiagObjAxisLabelX;
    ZS::Diagram::CDiagObjAxisLabel* m_pDiagObjAxisLabelY;
    ZS::Diagram::CDiagObjLine*      m_pDiagObjLinePhysicalMemorySize;
    ZS::Diagram::CDiagTrace*        m_pDiagTracePhysicalMemory;
    ZS::Diagram::CDiagObjCurve*     m_pDiagObjCurvePhysicalMemory;
    ZS::Diagram::CDiagObjMarker*    m_pDiagObjMarkerPhysicalMemory;
    ZS::Diagram::CDiagObjLine*      m_pDiagObjLineVirtualMemorySize;
    ZS::Diagram::CDiagTrace*        m_pDiagTraceVirtualMemory;
    ZS::Diagram::CDiagObjCurve*     m_pDiagObjCurveVirtualMemory;
    ZS::Diagram::CDiagObjMarker*    m_pDiagObjMarkerVirtualMemory;

}; // class CWdgtPCMemory

} // namespace PCResourceMonitor

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_PCResourceMonitor_MemoryWdgt_h
