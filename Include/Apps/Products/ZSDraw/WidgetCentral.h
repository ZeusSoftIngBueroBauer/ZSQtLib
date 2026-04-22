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

#ifndef ZSApps_Draw_WidgetCentral_h
#define ZSApps_Draw_WidgetCentral_h

#include <QtCore/qglobal.h>
#include <QtCore/qhash.h>

#if QT_VERSION < 0x050000
#include <QtGui/qmainwindow.h>
#else
#include <QtWidgets/qmainwindow.h>
#endif

class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace Draw
{
class CDrawingScene;
class CDrawingView;
class CWdgtDrawing;
}

namespace Apps
{
namespace Products
{
namespace Draw
{
//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Products::Draw"; }
    static QString ClassName() { return "CWidgetCentral"; }
    static CWidgetCentral* GetInstance(); // singleton class
public: // ctors and dtor
    CWidgetCentral(
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    virtual ~CWidgetCentral();
public: // instance methods
    ZS::Draw::CWdgtDrawing* drawingWidget();
    ZS::Draw::CDrawingView* drawingView();
    ZS::Draw::CDrawingScene* drawingScene();
protected: // overridables of base class QWidget
    void closeEvent(QCloseEvent* i_pEv) override;
protected: // class members
    static CWidgetCentral* s_pThis; // singleton class
protected: // instance members
    QVBoxLayout* m_pLyt = nullptr;
    ZS::Draw::CWdgtDrawing* m_pWdgtDrawing = nullptr;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj = nullptr;

}; // class CWidgetCentral

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_WidgetCentral_h
