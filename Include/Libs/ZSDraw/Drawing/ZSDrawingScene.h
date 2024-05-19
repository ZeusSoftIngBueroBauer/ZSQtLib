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

#ifndef ZSDraw_DrawingScene_h
#define ZSDraw_DrawingScene_h

#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSDraw/Common/ZSDrawGridSettings.h"
#include "ZSDraw/Common/ZSDrawingSize.h"
#include "ZSDraw/Common/ZSDrawPhysValPoint.h"
#include "ZSDraw/Common/ZSDrawPhysValSize.h"
#include "ZSDraw/Common/ZSDrawSettings.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSSysGUI/ZSSysGUIMathScaleDivLinesMetrics.h"
#include "ZSSys/ZSSysCommon.h"

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgraphicsscene.h>
#else
#include <QtWidgets/qgraphicsscene.h>
#endif

class QXmlStreamWriter;
class QXmlStreamAttributes;

namespace ZS
{
namespace System
{
class CIdxTree;
class CIdxTreeEntry;
}
namespace System
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
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CDrawingScene"; }
public: // class methods
    static QString getGraphObjNameNodeSeparator() { return s_strGraphObjNameSeparator; }
    //static QString FindUniqueGraphObjId(
    //    const QMap<QString,CGraphObj*>& i_dctpGraphObjs,
    //    const QString&                  i_strKeyInTreeCurr );
public: // ctors and dtor
    CDrawingScene(const QString& i_strName, QObject* i_pObjParent = nullptr);
    ~CDrawingScene();
signals:
    /*! Signal emitted if the drawing is about to be cleared and all graphical objects will be removed. */
    void aboutToBeCleared();
    /*! Signal emitted if the drawing size has been changed.
        @param i_size [in] Contains the new drawing size (pixel dimension, metric, etc.). */
    void drawingSizeChanged(const ZS::Draw::CDrawingSize& i_size);
    /*! Signal emitted if the grid settings has been changed.
        @param i_settings [in] Contains the new grid settings (visibilities of lines, labels, etc.). */
    void gridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings);
    /*! Signal emitted if the mouse positions has been changed.
        @param i_ptMousePos [in] Current mouse position in pixels. */
    void mousePosChanged(const QPointF& i_ptMousePos);
    /*! Signal emitted if the mode has been changed.
        See also method CDrawingScene::setMode. */
    void modeChanged(const ZS::System::CEnumMode& i_eMode);
    /*! Signal emitted if the mode has been changed.
        See also method CDrawingScene::setMode. */
    void drawingToolChanged(const QString& i_strFactoryGrpName, const QString& i_strGraphObjType);
    /*! Signal emitted if the draw set has been changed.
        @param i_drawSettings [in] Contains the new draw settings (pen style, line color, font, etc.). */
    void drawSettingsChanged(const ZS::Draw::CDrawSettings& i_drawSettings);
public: // instance methods
    void setDrawingSize( const CDrawingSize& i_drawingSize);
    const CDrawingSize& drawingSize() const;
public: // instance methods
    CPhysValPoint convert(const QPointF& i_pt) const;
    CPhysValPoint convert(const QPointF& i_pt, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValPoint convert(const CPhysValPoint& i_physValPoint) const;
    CPhysValPoint convert(const CPhysValPoint& i_physValPoint, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValSize convert(const QSizeF& i_size) const;
    CPhysValSize convert(const QSizeF& i_size, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValSize convert(const CPhysValSize& i_physValSize) const;
    CPhysValSize convert(const CPhysValSize& i_physValSize, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValLine convert(const QLineF& i_line) const;
    CPhysValLine convert(const QLineF& i_line, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValLine convert(const CPhysValLine& i_physValLine) const;
    CPhysValLine convert(const CPhysValLine& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValRect convert(const QRectF& i_rect) const;
    CPhysValRect convert(const QRectF& i_rect, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValRect convert(const CPhysValRect& i_physValRect) const;
    CPhysValRect convert(const CPhysValRect& i_physValRect, const ZS::PhysVal::CUnit& i_unitDst) const;
public: // instance methods
    void setGridSettings( const CDrawGridSettings& i_gridSettings);
    const CDrawGridSettings& gridSettings() const;
public: // instance methods
    const ZS::System::GUI::Math::CScaleDivLinesMetrics& divLinesMetricsX() const { return m_divLinesMetricsX; }
    const ZS::System::GUI::Math::CScaleDivLinesMetrics& divLinesMetricsY() const { return m_divLinesMetricsY; }
public: // instance methods
    ZS::System::CIdxTree* getGraphObjsIdxTree() { return m_pGraphObjsIdxTree; }
public: // instance methods
    ZS::System::SErrResultInfo load( const QString& i_strFileName );
    ZS::System::SErrResultInfo save( const QString& i_strFileName );
protected: // instance methods
    ZS::System::SErrResultInfo save( CGraphObj* i_pGraphObj, QXmlStreamWriter& i_xmlStreamWriter );
public: // instance methods
    void clear();
    void addGraphObj( CGraphObj* i_pGraphObj, CGraphObj* i_pGraphObjParent = nullptr );
    QGraphicsItem* findGraphicsItem( const QString& i_strKeyInTree );
    CGraphObj* findGraphObj( const QString& i_strKeyInTree );
public: // instance methods (replacing methods of QGraphicScene)
    void removeItem(QGraphicsItem* i_pGraphicsItem);
public: // instance methods
    int addPhysValShape(CPhysValShape* i_pPhysValShape, const CDrawSettings& i_drawSettings = CDrawSettings());
    void removePhysValShape(CPhysValShape* i_pPhysValShape);
    int physValShapesCount() const;
    CPhysValShape* getPhysValShape(int i_idxShape) const;
    void removeAndDeleteAllPhysValShapes();
public: // instance methods
    void setMode(const ZS::System::CEnumMode& i_mode);
        //const CEnumEditTool&         i_editTool = EEditTool::Undefined,
        //const CEnumEditMode&         i_editMode = EEditMode::Undefined,
        //const CEnumEditResizeMode&   i_editResizeMode = EEditResizeMode::Undefined,
        //bool                         i_bObjFactoryTypeChanged = false );
    ZS::System::CEnumMode getMode() const { return m_mode; }
    //CEnumEditTool getEditTool() const { return m_editTool; }
    //CEnumEditMode getEditMode() const { return m_editMode; }
    //CEnumEditResizeMode getEditResizeMode() const { return m_editResizeMode; }
public: // instance methods
    void setCurrentDrawingTool(CObjFactory* i_pObjFactory);
    void setCurrentDrawingTool(const QString& i_strFactoryGrpName, const QString& i_strGraphObjType);
    CObjFactory* getCurrentDrawingTool() const { return m_pObjFactory; }
    int getCurrentDrawingToolGraphObjType() const;
    //QGraphicsItem* getGraphicsItemCreating() const { return m_pGraphicsItemCreating; }
    //CGraphObj* getGraphObjCreating() const { return m_pGraphObjCreating; }
public: // instance methods
    CGraphObjConnectionPoint* getConnectionPoint( const QPointF& i_ptScenePos );
public: // instance methods
    QCursor getProposedCursor( const QPointF& i_ptPos ) const;
public: // instance methods
    double bringToFront( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos );
    double sendToBack( QGraphicsItem* i_pGraphicsItem, const QPointF& i_ptScenePos );
public: // instance methods
    double bringToFront( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems = QList<QGraphicsItem*>() );
    double sendToBack( QGraphicsItem* i_pGraphicsItem, const QList<QGraphicsItem*>& i_arpGraphicsItems = QList<QGraphicsItem*>() );
public: // to be called by graphical objects
    //void onGraphObjAddingShapePointsStarted( CGraphObj* i_pGraphObj );
    //void onGraphObjAddingShapePointsFinished( CGraphObj* i_pGraphObj );
    //void onGraphObjCreationFinished( CGraphObj* i_pGraphObj );
public: // instance methods
    QString findUniqueGraphObjName( CGraphObj* i_pGraphObj );
public: // instance methods
    int groupGraphObjsSelected();
    int ungroupGraphObjsSelected();
public: // instance methods
    double getHitToleranceInPx() const { return m_fHitTolerance_px; }
public: // instance methods
    void setDrawSettings( const CDrawSettings& i_settings );
    const CDrawSettings& drawSettings() const { return m_drawSettings; }
    bool updateDrawSettings();
public: // instance methods
    void setPenColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    QColor getPenColor() const { return m_drawSettings.getPenColor(); }
    void setPenWidth( int i_iPenWidth, bool i_bImmediatelyApplySetting = true );
    int getPenWidth() const { return m_drawSettings.getPenWidth(); }
public: // instance methods
    void setLineStyle( const CEnumLineStyle& i_lineStyle, bool i_bImmediatelyApplySetting = true );
    CEnumLineStyle getLineStyle() const { return m_drawSettings.getLineStyle(); }
public: // instance methods
    void setFillColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    QColor getFillColor() const { return m_drawSettings.getFillColor(); }
    void setFillStyle( const CEnumFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting = true );
    CEnumFillStyle getFillStyle() const { return m_drawSettings.getFillStyle(); }
public: // instance methods
    void setLineRecordType( const CEnumLineRecordType& i_recordType, bool i_bImmediatelyApplySetting = true );
    CEnumLineRecordType getLineRecordType() const { return m_drawSettings.getLineRecordType(); }
    void setLineExtent( int i_iExtent, bool i_bImmediatelyApplySetting = true );
    int  getLineExtent() const { return m_drawSettings.getLineExtent(); }
public: // instance methods
    void setLineEndStyle( const CEnumLinePoint& i_linePoint, const CEnumLineEndStyle& i_endStyle, bool i_bImmediatelyApplySetting = true );
    CEnumLineEndStyle getLineEndStyle( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getLineEndStyle(i_linePoint); }
    void setArrowHeadBaseLineType( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadBaseLineType& i_baseLineType, bool i_bImmediatelyApplySetting = true );
    CEnumArrowHeadBaseLineType getArrowHeadBaseLineType( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getArrowHeadBaseLineType(i_linePoint); }
    void setArrowHeadFillStyle( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting = true );
    CEnumArrowHeadFillStyle getArrowHeadFillStyle( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getArrowHeadFillStyle(i_linePoint); }
    void setArrowHeadWidth( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadWidth& i_arrowWidth, bool i_bImmediatelyApplySetting = true );
    CEnumArrowHeadWidth getArrowHeadWidth( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getArrowHeadWidth(i_linePoint); }
    void setArrowHeadLength( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadLength& i_arrowLength, bool i_bImmediatelyApplySetting = true );
    CEnumArrowHeadLength getArrowHeadLength( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getArrowHeadLength(i_linePoint); }
public: // instance methods
    void setTextColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    QColor getTextColor() const { return m_drawSettings.getTextColor(); }
    void setTextFont( const QFont& i_fnt, bool i_bImmediatelyApplySetting = true );
    QFont getTextFont() const { return m_drawSettings.getTextFont(); }
    void setTextSize( const ETextSize i_textSize, bool i_bImmediatelyApplySetting = true );
    ETextSize getTextSize() const { return m_drawSettings.getTextSize(); }
    void setTextStyle( const CEnumTextStyle& i_textStyle, bool i_bImmediatelyApplySetting = true );
    CEnumTextStyle getTextStyle() const { return m_drawSettings.getTextStyle(); }
    void setTextEffect( const CEnumTextEffect& i_textEffect, bool i_bImmediatelyApplySetting = true );
    CEnumTextEffect getTextEffect() const { return m_drawSettings.getTextEffect(); }
public: // overridables of base class QGraphicsScene
    virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* i_pEv ) override;
    virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* i_pEv ) override;
    virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent* i_pEv ) override;
    virtual void dropEvent( QGraphicsSceneDragDropEvent* i_pEv ) override;
public: // overridables of base class QGraphicsScene
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
public: // overridables of base class QGraphicsScene
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
    virtual void keyReleaseEvent( QKeyEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsScene
    virtual void drawBackground( QPainter* i_pPainter, const QRectF& i_rect ) override;
    virtual void drawForeground( QPainter* i_pPainter, const QRectF& i_rect ) override;
protected slots:
    void onDrawUnitsScreenResolutionInPxPerMMChanged();
    void onDrawUnitsScaleFactorChanged();
protected slots:
    void onGraphObjFactoryDestroyed( QObject* i_pObjFactory );
    void onGraphObjAboutToBeDestroyed( CGraphObj* i_pGraphObj );
protected: // auxiliary instance methods
    //void forwardMouseEvent(QGraphicsItem* i_pGraphicsItem, QGraphicsSceneMouseEvent* i_pEv);
    //void forwardMouseEventToObjectsHit(QGraphicsSceneMouseEvent* i_pEv);
    //void invalidateItemInAcceptingHoverEventsList(QGraphicsItem* i_pGraphicsItem);
    //void invalidateItemInBroughtToFrontList(QGraphicsItem* i_pGraphicsItem);
    void drawGridLines(QPainter* i_pPainter, const QRectF& i_rect);
    void drawPhysValShapes(QPainter* i_pPainter, const QRectF& i_rect);
    QRectF getBoundingRect(const QList<QGraphicsItem*>& i_arpGraphicsItems) const;
    void unselectGraphicsItems(const QList<QGraphicsItem*>& i_arpGraphicsItems) const;
protected: // auxiliary instance methods (trace emitting signals)
    void emit_aboutToBeCleared();
    void emit_drawingSizeChanged(const ZS::Draw::CDrawingSize& i_size);
    void emit_gridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings);
    void emit_mousePosChanged(const QPointF& i_ptMousePos);
    void emit_modeChanged(const ZS::System::CEnumMode& i_eMode);
    void emit_drawingToolChanged(const QString& i_strFactoryGrpName, const QString& i_strGraphObjType);
    void emit_drawSettingsChanged(const ZS::Draw::CDrawSettings& i_drawSettings);
public: // auxiliary instance methods (method tracing)
    //void traceInternalStates(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_mthDir, ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug);
    void traceItemsStates(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_mthDir, ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug);
protected: // class members
    static const QString s_strGraphObjNameSeparator;
protected: // instance members
    /*!< Settings concerning the drawing scene like used dimension unit (pixels or metrics),
         width and height of the drawing, X- and Y-Scaling as well as the resolution of
         a pixel on the screen. */
    CDrawingSize m_drawingSize;
    /*!< Mathematic component to calculate the division lines of the X-Scale.
         Also used to convert pixel values into metric values and vice versa. */
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetricsX;
    /*!< Mathematic component to calculate the division lines of the Y-Scale.
         Also used to convert pixel values into metric values and vice versa. */
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetricsY;
    /*!< Settings about the grid lines (visibility, colors, font, etc.). */
    CDrawGridSettings m_gridSettings;
    /*!< Keeps the current draw settings like pen and brush style and colors etc.
         used to draw the graphical objects. */
    CDrawSettings m_drawSettings;
    /*!< Several draw setting attributes may be modified one after another.
         To avoid that for each single change the drawSettingsChanged signal is emitted,
         it is possible first to modify each attribute one after another and afterwards
         emit the changed signal just once after all attributes have been set. For this the
         flag "i_bImmediatelyApplySetting" has be set to false when setting a new draw attribute.
         If this flag is false the changed property is temporarily stored in "m_drawSettingsTmp".
         After all settings have been changed the method "updateDrawSettings" takes over the
         settings from the temporary buffer.
         For the first change to be cached the temporary buffer will be allocated.
         After updating the changes the temporary buffer will be deleted. */
    CDrawSettings* m_pDrawSettingsTmp;
    /*!< The drawing scene knows two modes: Edit and View. In edit mode the drawing may be
         modified by adding and removing objects and by editing their shapes. In view mode
         editing the drawing is rejecting. */
    ZS::System::CEnumMode m_mode;
    //CEnumEditTool m_editTool;
    //CEnumEditMode m_editMode;
    //CEnumEditResizeMode m_editResizeMode;
    /*!< Corresponds to the selected drawing tool. */
    CObjFactory* m_pObjFactory;
    /*!< Same object as GraphObjCreating (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    //QGraphicsItem* m_pGraphicsItemCreating;
    /*! Same object as GraphicsItemCreating (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    //CGraphObj* m_pGraphObjCreating;
    /*! Same object as GraphObjAddingShapePoints (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    //QGraphicsItem* m_pGraphicsItemAddingShapePoints;
    /*! Same object as GraphicsItemAddingShapePoints (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    //CGraphObj* m_pGraphObjAddingShapePoints;
    /*!< The graphical objects are added to the index tree. */
    ZS::System::CIdxTree* m_pGraphObjsIdxTree;
    //ZS::System::CIdxTree* m_pGraphObjsIdxTreeClipboard;
    //QList<QGraphicsItem*> m_arpGraphicsItemsAcceptingHoverEvents;
    //QList<QGraphicsItem*> m_arpGraphicsItemsBroughtToFront;
    /*!< Hit tolerance to be used by the graphical objects to check whether a certain shape point has been hit.
         Currently initialised with 3.0 pixels. But might become a changeable property in the future. */
    double m_fHitTolerance_px;
    /*!< Stores the position on the drawing scene of a mouse press event.
        Used to setup and resize the rectangle area to select a group of objects. */
    QPointF m_ptMouseEvScenePosOnMousePressEvent;
    /*!< If the user clicks on an empty area and moves the mouse a selection area is created
         which is a graphics rectangle item. */
    QGraphicsRectItem* m_pGraphicsItemSelectionArea;
    /*!< List with physical shapes to be drawing in the background or foreground layer of the drawing scene. */
    QVector<CPhysValShape*> m_arpPhysValShapes;
    QVector<CDrawSettings> m_arDrawSettingsPhysValShapes;
    /*!< Objects to control method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjConversions;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjPaintEvent;

}; // class CDrawingScene

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingScene_h
