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

#include "ZSDraw/Common/ZSDrawGridSettings.h"
#include "ZSDraw/Common/ZSDrawingSize.h"

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsview.h>
#else
#include <QtWidgets/qgraphicsview.h>
#endif

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CDrawSettings;

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
public: // instance methods
    CDrawingScene* drawingScene() { return m_pDrawingScene; }
public: // instance methods (drawing area)
    void setDrawingSize( const CDrawingSize& i_size );
    const CDrawingSize& drawingSize() const;
public: // instance methods (drawing area)
    void setZoomFactorInPerCent(int i_iFactor_perCent);
    int zoomFactorInPerCent() const;
public: // instance methods (grid lines with labels)
    void setGridSettings( const CDrawGridSettings& i_settings );
    const CDrawGridSettings& gridSettings() const;
public: // instance methods (grid lines with labels)
    void setDrawSettings( const CDrawSettings& i_settings );
    const CDrawSettings& drawSettings() const;
public: // reimplemented method of base class QWidget
    void setCursor(const QCursor& i_cursor);
    void unsetCursor();
public: // protected overridables of base class QWidget make public for subsystem test
    virtual void mousePressEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QMouseEvent* i_pEv ) override;
protected: // overridables of base class QWidget
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
    virtual void keyReleaseEvent( QKeyEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsView
    virtual void scrollContentsBy( int i_dx, int i_dy ) override;
    virtual void resizeEvent( QResizeEvent* i_pEv ) override;
protected: // auxiliary methods
    void adjustCursor(QMouseEvent* i_pEv);
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
    int m_iZoomFactor_perCent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjPaintEvent;

}; // class CDrawingView

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingView_h
