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

#include "ZSDraw/Drawing/ZSDrawingSize.h"

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
    /*! Returns the namespace the class belongs to.
        Please note that the static class functions name must be different from
        the non static virtual member function "nameSpace"*/
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name.
        Please note that the static class functions name must be different from
        the non static virtual member function "className"*/
    static QString ClassName() { return "CDrawingView"; }
public: // ctors and dtor
    CDrawingView( CDrawingScene* i_pDrawingScene, QWidget* i_pWdgtParent = nullptr );
    ~CDrawingView();
signals:
    void mousePosChanged( const QPointF& i_ptMousePos );
    void drawingSizeChanged( const ZS::Draw::CDrawingSize& i_size );
    //void viewportMarginsChanged( const QMargins& i_margins );
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CDrawingScene* drawingScene() { return m_pDrawingScene; }
public: // instance methods (drawing area)
    void setDrawingSize( const CDrawingSize& i_size );
    CDrawingSize drawingSize() const;
public: // instance methods (drawing area)
    //void setViewportMargins( int i_iLeft, int i_iTop, int i_iRight, int i_iBottom );
    //void setViewportMargins( const QMargins& i_margins );
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
protected slots:
    void onSceneDrawingSizeChanged( const ZS::Draw::CDrawingSize& i_size );
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjPaintEvent;

}; // class CDrawingView

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingView_h
