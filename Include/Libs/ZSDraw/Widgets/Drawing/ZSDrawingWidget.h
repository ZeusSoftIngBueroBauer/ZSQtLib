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

#ifndef ZSDraw_DrawingWidget_h
#define ZSDraw_DrawingWidget_h

#include "ZSDraw/Common/ZSDrawDllMain.h"

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
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

//******************************************************************************
/*! @brief Class containing the drawing view which again contains the graphics
           view in a scroll area.

    Around the drawing view additional widgets are created to label the grid lines.
    Those widgets should not be in the scroll area but should always be visible either
    - on top to show the X scale for pixel dimension drawings or
    - at bottom to show the X scale for metrics dimension drawings and
    - on the left side to show the Y scale for both pixel dimension and metric
      dimension drawings.
*/
class ZSDRAWDLL_API CWdgtDrawing : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtDrawing"; }
public: // ctors and dtor
    CWdgtDrawing(QWidget* i_pWdgtParent = nullptr);
    ~CWdgtDrawing();
public: // instance methods
    CDrawingView* drawingView() { return m_pDrawingView; }
    CDrawingScene* drawingScene() { return m_pDrawingScene; }
protected: // instance members
    QVBoxLayout* m_pLyt;
    CDrawingView* m_pDrawingView;
    CDrawingScene* m_pDrawingScene;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtDrawing

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingWidget_h
