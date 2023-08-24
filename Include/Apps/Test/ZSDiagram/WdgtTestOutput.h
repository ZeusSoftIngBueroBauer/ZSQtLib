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

#ifndef ZSApps_TestDiagram_WdgtTestOutput_h
#define ZSApps_TestDiagram_WdgtTestOutput_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QFrame;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace Diagram
{
class CWdgtDiagram;
}

namespace Apps
{
namespace Test
{
namespace Diagram
{
//******************************************************************************
class CWdgtTestOutput : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtTestOutput( QWidget* i_pWdgtParent = nullptr, Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CWdgtTestOutput();
public: // instance methods
    void setDiagram(ZS::Diagram::CWdgtDiagram* i_pDiagram);
public: // overridables of base class QWidget
    void resizeEvent( QResizeEvent* i_pEv ) override;
protected slots:
    void onBtnDiagramPropertiesClicked(bool i_bChecked = false);
    void onDiagramDestroyed(QObject*);
protected: // instance members
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytLineHeader;
    QPushButton* m_pBtnDiagramProperties;
    QFrame* m_pFrameDiagram;
    ZS::Diagram::CWdgtDiagram* m_pDiagram;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtTestOutput

} // namespace Diagram

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestDiagram_WdgtTestOutput_h
