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

#ifndef ZSDraw_DrawingView_h
#define ZSDraw_DrawingView_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsview.h>
#else
#include <QtWidgets/qgraphicsview.h>
#endif

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawPageSetup.h"

namespace ZS
{
namespace Trace
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
public: // ctors and dtor
    CDrawingView(
        CDrawingScene* i_pDrawingScene,
        QWidget*       i_pWdgtParent = nullptr );
    ~CDrawingView();
signals:
    void mousePosChanged( const QPointF& i_ptMousePos );
    void pageSetupChanged( const ZS::Draw::CPageSetup& i_pageSetup );
public: // instance methods
    double getXResolutionInDpmm();  // horizontal resolution of paint device in dots per milli meter
    double getYResolutionInDpmm();  // vertical resolution of paint device in dots per milli meter
public: // instance methods
    void setPageSetup( const CPageSetup& i_pageSetup );
    CPageSetup getPageSetup() const { return m_pageSetup; }
public: // instance methods
    //void setPaperFormat( EPaperFormat i_format ) { m_pageSetup.setPaperFormat(i_format); }
    //EPaperFormat getPaperFormat() const { return m_pageSetup.getPaperFormat(); }
    //void setPaperOrientation( EPaperOrientation i_orientation ) { m_pageSetup.setPaperOrientation(i_orientation); }
    //EPaperOrientation getPaperOrientation() const { return m_pageSetup.getPaperOrientation(); }
    //void setPrinterPaperSource( EPrinterPaperSource i_source ) { m_pageSetup.setPrinterPaperSource(i_source); }
    //EPrinterPaperSource getPrinterPaperSource() const { return m_pageSetup.getPrinterPaperSource(); }
public: // instance methods
    //void setZoomFactorInPerCent( double i_fZoomFactor_percent ) { m_pageSetup.setZoomFactorInPerCent(i_fZoomFactor_percent); }
    //double getZoomFactorInPerCent() const { return m_pageSetup.getZoomFactorInPerCent(); }
    //void setPaperFormatWidthInMilliMeter( double i_fWidth_mm ) { }
    //double getPaperFormatWidthInMilliMeter() const { return m_pageSetup.getFormatWidthInMilliMeter(); }
    //void setPaperFormatHeightInMilliMeter( double i_fHeight_mm ) { }
    //double getPaperFormatHeightInMilliMeter() const { return m_pageSetup.getFormatHeightInMilliMeter(); }
public: // instance methods (drawing area)
    //void setMarginTopInMilliMeter( double i_fMargin_mm ) { m_pageSetup.setMarginTopInMilliMeter(i_fMargin_mm); }
    //double getMarginTopInMilliMeter() const { return m_pageSetup.getMarginTopInMilliMeter(); };
    //void setMarginBottomInMilliMeter( double i_fMargin_mm ) { m_pageSetup.setMarginBottomInMilliMeter(i_fMargin_mm); }
    //double getMarginBottomInMilliMeter() const { return m_pageSetup.getMarginBottomInMilliMeter(); };
    //void setMarginLeftInMilliMeter( double i_fMargin_mm ) { m_pageSetup.setMarginLeftInMilliMeter(i_fMargin_mm); }
    //double getMarginLeftInMilliMeter() const { return m_pageSetup.getMarginLeftInMilliMeter(); };
    //void setMarginRightInMilliMeter( double i_fMargin_mm ) { m_pageSetup.setMarginRightInMilliMeter(i_fMargin_mm); }
    //double getMarginRightInMilliMeter() const { return m_pageSetup.getMarginRightInMilliMeter(); };
public: // instance methods (drawing area)
    CDrawingScene* getDrawingScene() { return m_pDrawingScene; }
    //SDrawArea getDrawArea() const { return m_pageSetup.getDrawArea(); }
    void setDrawingWidthInPixels( int i_iWidth_px ) { m_pageSetup.setDrawingWidthInPixels(i_iWidth_px); }
    //void setDrawingWidth( ZS::PhysVal::CPhysVal i_physValWidth ) { m_pageSetup.setDrawingWidth(i_physValWidth); }
    int getDrawingWidthInPixels() const { return m_pageSetup.getDrawingWidthInPixels(); }
    //ZS::PhysVal::CPhysVal getDrawingWidth() const { return m_pageSetup.getDrawingWidth(); }
    void setDrawingHeightInPixels( int i_iHeight_px ) { m_pageSetup.setDrawingHeightInPixels(i_iHeight_px); }
    //void setDrawingHeight( ZS::PhysVal::CPhysVal i_physValHeight ) { m_pageSetup.setDrawingHeight(i_physValHeight); }
    int getDrawingHeightInPixels() const { return m_pageSetup.getDrawingHeightInPixels(); }
    //ZS::PhysVal::CPhysVal getDrawingHeight() const { return m_pageSetup.getDrawingHeight(); }
    //void setDrawingXScaleFactor( double i_fScaleFactor ) { m_pageSetup.setDrawingXScaleFactor(i_fScaleFactor); }
    //double getDrawingXScaleFactor() const { return m_pageSetup.getDrawingXScaleFactor(); }
    //void setDrawingYScaleFactor( double i_fScaleFactor ) { m_pageSetup.setDrawingYScaleFactor(i_fScaleFactor); }
    //double getDrawingYScaleFactor() const { return m_pageSetup.getDrawingYScaleFactor(); }
public: // overridables of base class QWidget
    virtual void mousePressEvent( QMouseEvent* i_pEv );
    virtual void mouseMoveEvent( QMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv );
    virtual void mouseDoubleClickEvent( QMouseEvent* i_pEv );
public: // overridables of base class QWidget
    virtual void keyPressEvent( QKeyEvent* i_pEv );
    virtual void keyReleaseEvent( QKeyEvent* i_pEv );
protected: // overridables of base class QGraphicsView
    virtual void paintEvent( QPaintEvent* i_pEv );
protected: // instance members
    CDrawingScene*           m_pDrawingScene;
    CPageSetup               m_pageSetup;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDrawingView

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingView_h
