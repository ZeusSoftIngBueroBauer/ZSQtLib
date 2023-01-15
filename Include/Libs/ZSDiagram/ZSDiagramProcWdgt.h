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

--------------------------------------------------------------------------------

From Qt's class documumentation for QPainter:

"When the paintdevice is a widget, QPainter can only be used inside a paintEvent()
function or in a function called by paintEvent(); that is unless the
Qt::WA_PaintOutsidePaintEvent widget attribute is set. On Mac OS X and Windows,
you can only paint in a paintEvent() function regardless of this attribute's setting."

Because of this restriction the following applies:

On requesting an instance of class CWdgtDiagram to update its content the process
depths need to be splitted. Only layout and data processing may be executed within
the update requests whereas pixmap processing must be executed within the context
of the paintEvent().

!!!!!!!!!!!!!!!!!!!!!!!!!?????????????????????????????????!!!!!!!!!!!!!!!!!!!!!!
?? 
?? Is this true ??? -> the following comment need to be verified !!!!
?? 
?? During data processing the layout regions of the diagram are already calculated
?? and the objects already know which widget region need to be marked as dirty and
?? the may call QWidget::update(QRect) to invalidate the widget regions.
?? 
!!!!!!!!!!!!!!!!!!!!!!!!!?????????????????????????????????!!!!!!!!!!!!!!!!!!!!!!

On requesting an instance of class CPixmapDiagram or CDataDiagram to update its
content the process depths don't need to be splitted and layout, data and pixmap
processing may be executed within the context of the update method.

*******************************************************************************/

#ifndef ZSDiagram_h
#define ZSDiagram_h

#include "ZSDiagram/ZSDiagramProcPixmap.h"

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QLabel;
#if QT_VERSION >= 0x040100
class QMenu;
#else
class QPopupMenu;
#endif

namespace ZS
{
namespace Diagram
{
//******************************************************************************
class ZSDIAGRAMDLL_API CWdgtDiagram : public QWidget, public CPixmapDiagram
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString ClassName() { return "CWdgtDiagram"; }
public: // ctors and dtor
    //CWdgtDiagram( QWidget* i_pWdgtParent );
    CWdgtDiagram( const QString& i_strObjName, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtDiagram();
public: // copy ctor not allowed but diagrams may be cloned
    virtual CDataDiagram* clone( EDiagramUpdateType i_diagramUpdateType ) const;
public: // instance methods
    CDiagObj* getDiagObjFocused();
    CDiagObj* getDiagObjEditing();
public: // instance methods to set optional attributes of the diagram
    void enableZooming( EScaleDir i_scaleDir = EScaleDirCount );
    void disableZooming( EScaleDir i_scaleDir = EScaleDirCount );
    bool isZoomingEnabled( EScaleDir i_scaleDir = EScaleDirCount ) const;
    void enableContextPopupMenu();
    void disableContextPopupMenu();
    bool isContextPopupMenuEnabled() const;
    void showTipOfTheDay();
    void hideTipOfTheDay();
    void setTipOfTheDay( const QString& i_str );
    void setTipOfTheDayIndentTop( int i_iIndent );
    void setTipOfTheDayIndentBottom( int i_iIndent );
    void setTipOfTheDayIndentLeft( int i_iIndent );
    void setTipOfTheDayIndentRight( int i_iIndent );
    void enableMoveKeyAcceleration(
        CDiagScale* i_pDiagScale,
        int         i_iTimeout_ms = 5000,
        int         i_iTimeoutSlowDown_ms = 1000,
        int         i_iTimeoutSpeedUp_ms = 200,
        int         i_iSpeedUpDelayRepCount = 5,
        double      i_fFactor = 2.0,
        double      i_fMaxStepScaleFactor = 0.02 );
    void disableMoveKeyAcceleration();
signals:
    void gotFocus( ZS::Diagram::CWdgtDiagram* i_pWdgtDiagram );
    void lostFocus( ZS::Diagram::CWdgtDiagram* i_pWdgtDiagram );
    void painted( ZS::Diagram::CWdgtDiagram* i_pWdgtDiagram );
public: // overridables
    virtual void update( CDiagObj* i_pDiagObj, const QRect& i_rect ); // used instead of QWidget::update(rect) to trace the calling diagram object
public: // overridables
    virtual void update();
    virtual void update( const QRect& i_rect );
    virtual void setMouseTracking( bool i_bEnable );
    virtual void setEraseColor( const QColor& i_col );
    virtual void setFocus();
    virtual void clearFocus();
public: // overridables of base class QWidget
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
    virtual void keyReleaseEvent( QKeyEvent* i_pEv ) override;
    virtual void mousePressEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QMouseEvent* i_pEv ) override;
    virtual void paintEvent( QPaintEvent* i_pEv ) override;
    virtual void resizeEvent( QResizeEvent* i_pEv ) override;
    virtual void focusInEvent( QFocusEvent* i_pEv ) override;
    virtual void focusOutEvent( QFocusEvent* i_pEv ) override;
protected: // instance methods
    bool processMoveKeyEvent( int i_iKey );
protected: // overridables of base class CPixmapDiagram
    virtual void updatePixmap() override;
protected: // overridables
    virtual void updateWidget();
protected slots: // overridable instance methods
    virtual void popupMenuContextItemSaveActivated( void );
    virtual void popupMenuContextItemPrintActivated( void );
private: // copy ctor not allowed
    CWdgtDiagram( const CWdgtDiagram& );
private: // assignment operator not allowed
    void operator=( const CWdgtDiagram& );
protected:  // instance members
    bool         m_bZoomingXScaleEnabled;
    bool         m_bZoomingYScaleEnabled;
    bool         m_bIsZooming;
    QPoint       m_ptZoomStart;
    QPoint       m_ptZoomEnd;
    QRect        m_rectZoom;
    int          m_iZoomCount;
    bool         m_bContextPopupMenuEnabled;
    #if QT_VERSION >= 0x040100
    QMenu*       m_pMenuContext;
    #else
    QPopupMenu*  m_pMenuContext;
    #endif
    QString      m_strTipOfTheDay;
    int          m_iTipOfTheDayIndentTop;
    int          m_iTipOfTheDayIndentBottom;
    int          m_iTipOfTheDayIndentLeft;
    int          m_iTipOfTheDayIndentRight;
    QLabel*      m_pLblTipOfTheDay;
    QString      m_strCurrentFileDir;
    bool         m_bMoveKeyAccelerationEnabled;
    CDiagScale*  m_pMoveKeyAccelerationDiagScale;
    int          m_iMoveKeyAccelerationTimeout_ms;
    int          m_iMoveKeyAccelerationTimeoutSlowDown_ms;
    int          m_iMoveKeyAccelerationTimeoutSpeedUp_ms;
    int          m_iMoveKeyAccelarationSpeedUpDelayRepCount;
    double       m_fMoveKeyAccelerationFactor;
    double       m_fMoveKeyAccelerationMaxStepScaleFactor;
    double       m_fMoveKeyEventTime_ms;
    double       m_fMoveKeyRepCount;
    int          m_iMoveKeySpeedUpDelayRepCount;
    int          m_iMoveKeyPrev;
    CDiagObj*    m_pDiagObjFocused;
    CDiagObj*    m_pDiagObjFocusedByKeyEvent;
    CDiagObj*    m_pDiagObjFocusedByMouseEvent;
    CDiagObj*    m_pDiagObjFocusedRecently;
    CDiagObj*    m_pDiagObjEditing;
    CDiagObj*    m_pDiagObjEditingByKeyEvent;
    CDiagObj*    m_pDiagObjEditingByMouseEvent;

}; // class CWdgtDiagram

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ProcWdgt_h
