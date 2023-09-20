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

#ifndef ZSDraw_DrawingView_h
#define ZSDraw_DrawingView_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsview.h>
#else
#include <QtWidgets/qgraphicsview.h>
#endif

#include "ZSDraw/Common/ZSDrawingSize.h"
#include "ZSDraw/Drawing/ZSDrawGridSettings.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;

//******************************************************************************
class ZSDRAWDLL_API CDrawingView : public QGraphicsView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CDrawingView"; }
public: // ctors and dtor
    CDrawingView( CDrawingScene* i_pDrawingScene, QWidget* i_pWdgtParent = nullptr );
    ~CDrawingView();
signals:
    void drawingSizeChanged( const ZS::Draw::CDrawingSize& i_size );
    void gridSettingsChanged( const ZS::Draw::CDrawGridSettings& i_settings );
    void mousePosChanged( const QPointF& i_ptMousePos );
    void contentAreaChanged();
    //void viewportMarginsChanged( const QMargins& i_margins );
public: // instance methods
    CDrawingScene* drawingScene() { return m_pDrawingScene; }
public: // instance methods (drawing area)
    void setDrawingSize( const CDrawingSize& i_size );
    CDrawingSize drawingSize() const;
public: // instance methods (grid lines with labels)
    void setGridSettings( const CDrawGridSettings& i_settings );
    CDrawGridSettings gridSettings() const;
public: // instance methods (drawing area)
    //void setViewportMargins( int i_iLeft, int i_iTop, int i_iRight, int i_iBottom );
    //void setViewportMargins( const QMargins& i_margins );
public: // overridables of base class QWidget
    virtual void mousePressEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QMouseEvent* i_pEv ) override;
public: // overridables of base class QWidget
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
    virtual void keyReleaseEvent( QKeyEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsView
    virtual void scrollContentsBy( int i_dx, int i_dy ) override;
    virtual void resizeEvent( QResizeEvent* i_pEv ) override;
    virtual void paintEvent( QPaintEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsView
    virtual void setupViewport(QWidget* i_pWdgt);
protected: // auxiliary methods
    void adjustCursor(QMouseEvent* i_pEv);
    void paintGridLabels(QPainter* i_pPainter);
protected slots:
    void onSceneDrawingSizeChanged( const ZS::Draw::CDrawingSize& i_size );
    void onSceneGridSettingsChanged( const ZS::Draw::CDrawGridSettings& i_settings );
protected: // instance methods (method tracing)
    void emit_mousePosChanged( const QPointF& i_ptMousePos );
    void emit_drawingSizeChanged( const ZS::Draw::CDrawingSize& i_size );
    void emit_gridSettingsChanged( const ZS::Draw::CDrawGridSettings& i_settings );
    void emit_contentAreaChanged();
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjPaintEvent;

}; // class CDrawingView

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingView_h
