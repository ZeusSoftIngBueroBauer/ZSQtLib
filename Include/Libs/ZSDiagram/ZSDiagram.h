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

#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSDiagram/ZSDiagramAux.h"

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QLabel;
class QPrinter;
#if QT_VERSION >= 0x040100
class QMenu;
#else
class QPopupMenu;
#endif

namespace ZS
{
namespace System
{
class CTrcAdminObj;
//class CMsgAck;
}

namespace Diagram
{
struct SFrameStyle;
class CDiagObj;
class CDiagScale;
class CDiagTrace;

//******************************************************************************
class ZSDIAGRAMDLL_API CDataDiagram
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Diagram"; }
    static QString ClassName() { return "CDataDiagram"; }
public: // ctors and dtor
    //CDataDiagram( EDiagramUpdateType i_updateType = EDiagramUpdateTypeData );
    CDataDiagram(
        const QString&     i_strObjName,
        EDiagramUpdateType i_updateType = EDiagramUpdateTypeData );
    virtual ~CDataDiagram();
public: // copy ctor not allowed but diagrams may be cloned
    virtual CDataDiagram* clone( EDiagramUpdateType i_diagramUpdateType ) const;
public: // instance methods
    void setObjName( const QString& i_strObjName );
    QString getObjName() const;
    EDiagramUpdateType getUpdateType() const;
public: // overridables to update the content of the diagram
    virtual void updateDiagram( CDiagObj* i_pDiagObj = nullptr ); // need a method name different from QWidget::update
    virtual void invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags );
    virtual void validate( unsigned int i_uUpdateFlags );
public: // overridables to rescale the diagram or output new X/Y values
    virtual void setSpacing( EScaleDir i_scaleDir, ESpacing i_spacing );
public: // instance methods to add and remove scale objects
    void addDiagScale( CDiagScale* i_pDiagScale );
    void removeDiagScale( CDiagScale* i_pDiagScale );
    CDiagScale* removeDiagScale( const QString& i_strObjName );
    CDiagScale* getDiagScale( const QString& i_strObjName );
public: // instance methods to add and remove trace objects
    void addDiagTrace( CDiagTrace* i_pDiagTrace );
    void removeDiagTrace( CDiagTrace* i_pDiagTrace );
    CDiagTrace* removeDiagTrace( const QString& i_strObjName );
    CDiagTrace* getDiagTrace( const QString& i_strObjName );
public: // instance methods to add, remove, and change diagram objects
    void addDiagObj( CDiagObj* i_pDiagObj );
    void removeDiagObj( CDiagObj* i_pDiagObj );
    CDiagObj* removeDiagObj( const QString& i_strObjName );
    CDiagObj* removeDiagObj( int i_iObjId );
    CDiagObj* getDiagObj( const QString& i_strObjName ) const;
    CDiagObj* getDiagObj( int i_iObjId ) const;
    void showDiagObj( const QString& i_strObjName ) const;
    void showDiagObj( int i_iObjId ) const;
    void hideDiagObj( const QString& i_strObjName ) const;
    void hideDiagObj( int i_iObjId ) const;
    bool isDiagObjVisible( const QString& i_strObjName ) const;
    bool isDiagObjVisible( int i_iObjId ) const;
    void moveDiagObjInPaintList( const QString& i_strObjName, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
    void moveDiagObjInPaintList( int i_iObjId, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
    void moveDiagObjInPaintList( CDiagObj* i_pDiagObj, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
public: // overridables (callbacks from CDiagScale, CDiagTrace and CDiagObj)
    virtual void scaleChanged( ZS::Diagram::CDiagScale* i_pDiagScale );
    virtual void traceChanged( ZS::Diagram::CDiagTrace* i_pDiagTrace );
protected: // overridables to update the content of the diagram
    virtual void updateLayout();
    virtual void updateData();
private: // copy ctor not allowed
    CDataDiagram( const CDataDiagram& );
private: // assignment operator not allowed
    void operator=( const CDataDiagram& );
protected:  // instance members
    QString                   m_strObjName;
    EDiagramUpdateType        m_updateType;
    unsigned int              m_uUpdateFlags;
    EMeasState                m_measState;
    EMeasMode                 m_measMode;
    int                       m_iMeasType;
    ESpacing                  m_arSpacing[EScaleDirCount];
    int                       m_iDiagScaleCount;
    CDiagScale*               m_pDiagScaleFirst;
    CDiagScale*               m_pDiagScaleLast;
    int                       m_iDiagTraceCount;
    CDiagTrace*               m_pDiagTraceFirst;
    CDiagTrace*               m_pDiagTraceLast;
    int                       m_iDiagObjCount;
    CDiagObj*                 m_pDiagObjFirst;
    CDiagObj*                 m_pDiagObjLast;
    CDiagObj*                 m_pDiagObjPaintFirst;
    CDiagObj*                 m_pDiagObjPaintLast;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjValidate;

}; // class CDataDiagram

//******************************************************************************
class ZSDIAGRAMDLL_API CPixmapDiagram : public CDataDiagram
//******************************************************************************
{
public: // class methods
    static QString ClassName() { return "CPixmapDiagram"; }
public: // class members
    static const char* g_arszImageFormatFileFilter[EImageFormatCount+1]; // Last element set to nullptr
    static const char* g_arszImageFormatFileExt[EImageFormatCount+1];    // Last element set to nullptr
    static const int   g_ariImageFormat[EImageFormatCount+1];            // Last element set to -1
    static const char* g_arszQtImageFormat[EImageFormatCount+1];         // Last element set to nullptr
public: // class methods
    static EImageFormat GetImageFormat( const QString& i_strImageFormat );
public: // ctors and dtor
    //CPixmapDiagram( EDiagramUpdateType i_updateType = EDiagramUpdateTypePixmap );
    CPixmapDiagram(
        const QString&     i_strObjName,
        EDiagramUpdateType i_updateType  = EDiagramUpdateTypePixmap );
    virtual ~CPixmapDiagram();
public: // copy ctor not allowed but diagrams may be cloned
    virtual CDataDiagram* clone( EDiagramUpdateType i_diagramUpdateType ) const;
public: // instance methods
    bool save( const QString& i_strFileName, const QString& i_strQtImageFormat );
    bool save( const QString& i_strFileName, EImageFormat i_imageFormat );
    bool save( QIODevice* i_pDevice, EImageFormat i_imageFormat );
    bool save( QIODevice* i_pDevice, const QString& i_strQtImageFormat );
    void print( const QPrinter* i_pPrinter ) const;
public: // instance methods to set and get the attributes of the diagram
    void setWidth( int i_cxWidth );
    int getWidth() const; // kann nicht "width" verwenden wegen CustomXXXXyhdfViewer vom CompassGuiFramework
    void setHeight( int i_cyHeight );
    int getHeight() const; // kann nicht "height" verwenden wegen CustomXXXXyhdfViewer vom CompassGuiFramework
    void setSize( const QSize& i_size );
    QSize getSize() const; // kann nicht "size" verwenden wegen CustomXXXXyhdfViewer vom CompassGuiFramework
    void setMarginTop( int i_iMargin );
    int getMarginTop() const;
    void setMarginBottom( int i_iMargin );
    int getMarginBottom() const;
    void setMarginLeft( int i_iMargin );
    int getMarginLeft() const;
    void setMarginRight( int i_iMargin );
    int getMarginRight() const;
    QRect getRectPartTop() const;
    QRect getRectPartBottom() const;
    QRect getRectPartLeft() const;
    QRect getRectPartRight() const;
    QRect getRectPartCenter() const;
    void setMinimumHeightPartTop( int i_cyHeight );
    int getMinimumHeightPartTop() const;
    void setMinimumHeightPartBottom( int i_cyHeight );
    int getMinimumHeightPartBottom() const;
    void setMinimumWidthPartLeft( int i_cxWidth );
    int getMinimumWidthPartLeft() const;
    void setMinimumWidthPartRight( int i_cxWidth );
    int getMinimumWidthPartRight() const;
    void setColErase( const QColor& i_col );
    QColor getColErase() const;
    void setColBgPartCenter( const QColor& i_col );
    QColor getColBgPartCenter() const;
    void setFrameStyle( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStyle();
    void setFrameStylePartCenter( SFrameStyle* i_pFrameStyle );
    SFrameStyle* getFrameStylePartCenter();
    QPixmap* getPixmap();
public: // overridables of base class CDataDiagram
    //virtual void updateDiagram( CDiagObj* i_pDiagObj = nullptr ); // need a method name different from QWidget::update
    virtual void invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags );
protected: // overridables of base class CDataDiagram
    virtual void updateLayout();
protected: // overridables
    virtual void updatePixmap();
private: // copy ctor not allowed
    CPixmapDiagram( const CPixmapDiagram& );
private: // assignment operator not allowed
    void operator=( const CPixmapDiagram& );
protected:  // instance members
    QColor       m_colErase;
    QColor       m_colBgPartCenter;
    QPixmap*     m_pPixmap;
    QPrinter*    m_pPrinter;
    QSize        m_size;
    int          m_cyMinimumHeightPartTop;
    int          m_cyMinimumHeightPartBottom;
    int          m_cxMinimumWidthPartLeft;
    int          m_cxMinimumWidthPartRight;
    int          m_iMarginTop;
    int          m_iMarginBottom;
    int          m_iMarginLeft;
    int          m_iMarginRight;
    QRect        m_rectPartTop;
    QRect        m_rectPartBottom;
    QRect        m_rectPartLeft;
    QRect        m_rectPartRight;
    QRect        m_rectPartCenter;
    SFrameStyle* m_pFrameStyle;
    SFrameStyle* m_pFrameStylePartCenter;
    QRect        m_rectFramePartCenterTopLineInv;
    QRect        m_rectFramePartCenterBottomLineInv;
    QRect        m_rectFramePartCenterLeftLineInv;
    QRect        m_rectFramePartCenterRightLineInv;

}; // class CPixmapDiagram

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
public: // overridables of base class QWidget
    virtual void update();
    virtual void update( const QRect& i_rect );
    virtual void setMouseTracking( bool i_bEnable );
    virtual void setEraseColor( const QColor& i_col );
    virtual void setFocus();
    virtual void clearFocus();
    virtual void keyPressEvent( QKeyEvent* i_pEv );
    virtual void keyReleaseEvent( QKeyEvent* i_pEv );
    virtual void mousePressEvent( QMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv );
    virtual void mouseDoubleClickEvent( QMouseEvent* i_pEv );
    virtual void mouseMoveEvent( QMouseEvent* i_pEv );
    virtual void paintEvent( QPaintEvent* i_pEv );
    virtual void resizeEvent( QResizeEvent* i_pEv );
    virtual void focusInEvent( QFocusEvent* i_pEv );
    virtual void focusOutEvent( QFocusEvent* i_pEv );
protected: // instance methods
    bool processMoveKeyEvent( int i_iKey );
protected: // overridables of base class CPixmapDiagram
    virtual void updatePixmap();
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

#endif // #ifndef ZSDiagram_h
