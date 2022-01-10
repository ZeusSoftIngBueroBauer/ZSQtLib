/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_DrawingScene_h
#define ZSDraw_DrawingScene_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsscene.h>
#else
#include <QtWidgets/qgraphicsscene.h>
#endif

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawCommon.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSDraw/ZSDrawSettings.h"

class QXmlStreamWriter;

namespace ZS
{
namespace System
{
class CIdxTree;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
class CGraphObj;
class CGraphObjConnectionPoint;
class CObjFactory;

//******************************************************************************
class ZSDRAWDLL_API CDrawingScene : public QGraphicsScene
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CDrawingScene"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // class methods
    //static QString FindUniqueGraphObjId(
    //    const QMap<QString,CGraphObj*>& i_dctpGraphObjs,
    //    const QString&                  i_strObjIdCurr );
public: // ctors and dtor
    CDrawingScene( QObject* i_pObjParent = nullptr );
    ~CDrawingScene();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void mousePosChanged( const QPointF& i_ptMousePos );
    void modeChanged();
    void drawSettingsChanged( const ZS::Draw::CDrawSettings& i_drawSettings );
signals:
    //void graphObjCreated( ZS::Draw::CGraphObj* i_pGraphObj );
    //void graphObjDestroying( const QString& i_strObjId );
    //void graphObjDestroyed( const QString& i_strObjId );
    //void graphObjIdChanged( const QString& i_strObjIdOld, const QString& i_strObjIdNew );
    //void graphObjNameChanged( const QString& i_strObjId, const QString& i_strObjNameOld, const QString& i_strObjNameNew );
public: // instance methods
    ZS::System::CIdxTree* getGraphObjsIdxTree() { return m_pGraphObjsIdxTree; }
public: // instance methods
    void clear();
    void addGraphObj( CGraphObj* i_pGraphObj, CGraphObj* i_pGraphObjParent = nullptr );
    void removeGraphObj( CGraphObj* i_pGraphObj );
protected: // instance methods
    void addItem( QGraphicsItem* i_pGraphicsItem, QGraphicsItem* i_pGraphicsItemParent = nullptr );
    void removeItem( QGraphicsItem* i_pGraphicsItem );
    void deleteItem( QGraphicsItem* i_pGraphicsItem );
    void deleteItem( CGraphObj* i_pGraphObj );
protected: // instance methods
    //void addChildItems( QGraphicsItem* i_pGraphicsItem );
public: // instance methods
    ZS::System::SErrResultInfo load( const QString& i_strFileName );
    ZS::System::SErrResultInfo save( const QString& i_strFileName );
protected: // instance methods
    ZS::System::SErrResultInfo save( CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter );
public: // instance methods
    void setXResolutionInDpmm( double i_fRes_dpmm ) { m_fXResolution_dpmm = i_fRes_dpmm; }
    double getXResolutionInDpmm() const { return m_fXResolution_dpmm; }
    void setYResolutionInDpmm( double i_fRes_dpmm ) { m_fYResolution_dpmm = i_fRes_dpmm; }
    double getYResolutionInDpmm() const { return m_fYResolution_dpmm; }
public: // instance methods
    void setMode(
        const ZS::System::CEnumMode& i_mode,
        const CEnumEditTool&         i_editTool = EEditTool::Ignore,
        const CEnumEditMode&         i_editMode = EEditMode::Ignore,
        const CEnumEditResizeMode&   i_editResizeMode = EEditResizeMode::Ignore,
        bool                         i_bObjFactoryTypeChanged = false );
    ZS::System::CEnumMode getMode() const { return m_mode; }
    CEnumEditTool getEditTool() const { return m_editTool; }
    CEnumEditMode getEditMode() const { return m_editMode; }
    CEnumEditResizeMode getEditResizeMode() const { return m_editResizeMode; }
public: // instance methods
    void setCurrentDrawingTool( CObjFactory* i_pObjFactory );
    void setCurrentDrawingTool( const QString& i_strFactoryGrpName, const QString& i_strGraphObjType );
    CObjFactory* getCurrentDrawingTool() const { return m_pObjFactory; }
    int getCurrentDrawingToolGraphObjType() const;
    QGraphicsItem* getGraphicsItemCreating() const { return m_pGraphicsItemCreating; }  // same object as GraphObjCreating (just a different name for the same thing)
    CGraphObj* getGraphObjCreating() const { return m_pGraphObjCreating; }              // same object as GraphicsItemCreating (just a different name for the same thing)
public: // instance methods
    CGraphObjConnectionPoint* getConnectionPoint( const QPointF& i_ptScenePos );
public: // instance methods
    QCursor getProposedCursor( const QPointF& i_ptPos ) const;
public: // instance methods
    double bringToFront( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos );
    double sendToBack( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos );
public: // instance methods
    double bringToFront( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems = QList<QGraphicsItem*>() ); // usually list of items under mouse cursor, if list is empty all scene's items will be checked
    double sendToBack( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems = QList<QGraphicsItem*>() );   // usually list of items under mouse cursor, if list is empty all scene's items will be checked
public: // to be called by graphical objects (as graphical objects are not derived from QObject and cannot emit signals)
    void onGraphObjAddingShapePointsStarted( CGraphObj* i_pGraphObj );
    void onGraphObjAddingShapePointsFinished( CGraphObj* i_pGraphObj );
public: // to be called by graphical objects (as graphical objects are not derived from QObject and cannot emit signals)
    void onGraphObjCreationFinished( CGraphObj* i_pGraphObj );
    void onGraphObjDestroying( const QString& i_strObjId );
    //void onGraphObjDestroyed( const QString& i_strObjId );
    //void onGraphObjIdChanged( const QString& i_strObjIdOld, const QString& i_strObjIdNew );
    //void onGraphObjNameChanged(
    //    const QString& i_strObjId,
    //    const QString& i_strObjNameOld,     // !!! NOT including name of parents !!!
    //    const QString& i_strObjNameNew );   // !!! NOT including name of parents !!!
public: // instance methods
    QGraphicsItem* findGraphicsItem( const QString& i_strObjId );
    CGraphObj* findGraphObj( const QString& i_strObjId );
public: // instance methods
    QString findUniqueGraphObjName( CGraphObj* i_pGraphObj );
public: // instance methods
    QString getGraphObjNameNodeSeparator() const { return m_strGraphObjNameSeparator; }
public: // instance methods
    int groupGraphObjsSelected();
    int ungroupGraphObjsSelected();
public: // instance methods
    double getRotationAngleResolutionInDegree() const { return m_fRotAngleRes_degree; }
    double getHitToleranceInPx() const { return m_fHitTolerance_px; }
public: // instance methods
    void setDrawSettings( const CDrawSettings& i_settings );
    CDrawSettings getDrawSettings() const { return m_drawSettings; }
public: // instance methods
    void setPenColor( const QColor& i_clr );
    QColor getPenColor() const { return m_drawSettings.getPenColor(); }
    void setPenWidth( int i_iPenWidth );
    int getPenWidth() const { return m_drawSettings.getPenWidth(); }
public: // instance methods
    void setLineStyle( ELineStyle i_lineStyle );
    ELineStyle getLineStyle() const { return m_drawSettings.getLineStyle(); }
public: // instance methods
    void setFillColor( const QColor& i_clr );
    QColor getFillColor() const { return m_drawSettings.getFillColor(); }
    void setFillStyle( EFillStyle i_fillStyle );
    EFillStyle getFillStyle() const { return m_drawSettings.getFillStyle(); }
public: // instance methods
    void setLineRecordType( ELineRecordType i_lineRecordType );
    ELineRecordType getLineRecordType() const { return m_drawSettings.getLineRecordType(); }
    void setLineExtent( int i_iLineExtent );
    int  getLineExtent() const { return m_drawSettings.getLineExtent(); }
public: // instance methods
    void setLineEndStyle( ELinePoint i_linePoint, ELineEndStyle i_endStyle );
    ELineEndStyle getLineEndStyle( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndStyle(i_linePoint); }
    void setLineEndBaseLineType( ELinePoint i_linePoint, ELineEndBaseLineType i_baseLineType );
    ELineEndBaseLineType getLineEndBaseLineType( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndBaseLineType(i_linePoint); }
    void setLineEndFillStyle( ELinePoint i_linePoint, ELineEndFillStyle i_fillStyle );
    ELineEndFillStyle getLineEndFillStyle( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndFillStyle(i_linePoint); }
    void setLineEndWidth( ELinePoint i_linePoint, ELineEndWidth i_arrowWidth );
    ELineEndWidth getLineEndWidth( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndWidth(i_linePoint); }
    void setLineEndLength( ELinePoint i_linePoint, ELineEndLength i_arrowLength );
    ELineEndLength getLineEndLength( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndLength(i_linePoint); }
public: // instance methods
    void setTextColor( const QColor& i_clr );
    QColor getTextColor() const { return m_drawSettings.getTextColor(); }
    void setTextFont( const QFont& i_fnt );
    QFont getTextFont() const { return m_drawSettings.getTextFont(); }
    void setTextSize( ETextSize i_size );
    ETextSize getTextSize() const { return m_drawSettings.getTextSize(); }
    void setTextStyle( ETextStyle i_style );
    ETextStyle getTextStyle() const { return m_drawSettings.getTextStyle(); }
    void setTextEffect( ETextEffect i_effect );
    ETextEffect getTextEffect() const { return m_drawSettings.getTextEffect(); }
public: // overridables of base class QGraphicsScene
    virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* i_pEv );
    virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* i_pEv );
    virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent* i_pEv );
    virtual void dropEvent( QGraphicsSceneDragDropEvent* i_pEv );
public: // overridables of base class QGraphicsScene
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv );
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv );
    bool isMouseDoubleClickEventInProcess() const { return m_bMouseDoubleClickEventInProcess; }
public: // overridables of base class QGraphicsScene
    virtual void keyPressEvent( QKeyEvent* i_pEv );
    virtual void keyReleaseEvent( QKeyEvent* i_pEv );
protected: // overridables of base class QGraphicsScene
    virtual void drawBackground( QPainter* i_pPainter, const QRectF& i_rect );
    virtual void drawForeground( QPainter* i_pPainter, const QRectF& i_rect );
protected slots:
    void onGraphObjFactoryDestroyed( QObject* i_pObjFactory );
protected: // instance members
    CDrawSettings            m_drawSettings;
    double                   m_fXResolution_dpmm;
    double                   m_fYResolution_dpmm;
    ZS::System::CEnumMode    m_mode;
    CEnumEditTool            m_editTool;
    CEnumEditMode            m_editMode;
    CEnumEditResizeMode      m_editResizeMode;
    QGraphicsRectItem*       m_pGraphicsItemSelectionArea;
    CObjFactory*             m_pObjFactory;                    /*! Corresponds to the selected drawing tool. */
    QGraphicsItem*           m_pGraphicsItemCreating;          /*! Same object as GraphObjCreating (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    CGraphObj*               m_pGraphObjCreating;              /*! Same object as GraphicsItemCreating (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    QGraphicsItem*           m_pGraphicsItemAddingShapePoints; /*! Same object as GraphObjAddingShapePoints (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    CGraphObj*               m_pGraphObjAddingShapePoints;     /*! Same object as GraphicsItemAddingShapePoints (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    QString                  m_strGraphObjNameSeparator;
    ZS::System::CIdxTree*    m_pGraphObjsIdxTree;
    ZS::System::CIdxTree*    m_pGraphObjsIdxTreeClipboard;
    QList<QGraphicsItem*>    m_arpGraphicsItemsAcceptingHoverEvents;
    double                   m_fRotAngleRes_degree;
    double                   m_fHitTolerance_px;
    bool                     m_bMouseDoubleClickEventInProcess;
    QPointF                  m_ptMouseEvScenePosOnMousePressEvent;
    int                      m_iEvKeyModifiers;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvent;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObjPaintEvent;

}; // class CDrawingScene

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingScene_h
