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

#ifndef ZSDraw_DrawGridLabels_h
#define ZSDraw_DrawGridLabels_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSSysGUI/ZSSysGUIMathScaleDivLinesMetrics.h"
#include "ZSSys/ZSSysCommon.h"

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawGridSettings;
class CDrawingSize;
class CDrawingView;

//******************************************************************************
/*! @brief 
*/
class ZSDRAWDLL_API CWdgtGridLabels : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGridLabels"; }
public: // ctors and dtor
    CWdgtGridLabels(
        const QString& i_strName,
        CDrawingView* i_pDrawingView,
        ZS::System::EScaleDir i_scaleDir,
        ZS::System::ELayoutPos i_layoutPos,
        QWidget* i_pWdgtParent = nullptr);
    ~CWdgtGridLabels();
public: // overridables of base class QWidget
    QSize sizeHint() const override;
protected: // overridables of base class QWidget
    void paintEvent(QPaintEvent* i_pEv) override;
protected slots:
    void onDrawingViewDrawingSizeChanged(const ZS::Draw::CDrawingSize& i_size);
    void onDrawingViewGridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings);
protected: // auxiliary methods
    void paintGridLabels(QPainter* i_pPainter);
protected: // instance members
    CDrawingView* m_pDrawingView;
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetrics;
    ZS::System::EScaleDir m_scaleDir;
    ZS::System::ELayoutPos m_layoutPos;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtGridLabels

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGridLabels_h
