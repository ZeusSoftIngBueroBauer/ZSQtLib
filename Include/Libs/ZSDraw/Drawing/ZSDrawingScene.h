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

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSDraw/Common/ZSDrawGridSettings.h"
#include "ZSDraw/Common/ZSDrawingSize.h"
#include "ZSDraw/Common/ZSDrawPhysValPoint.h"
#include "ZSDraw/Common/ZSDrawPhysValSize.h"
#include "ZSDraw/Common/ZSDrawSettings.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSysGUI/ZSSysGUIMathScaleDivLinesMetrics.h"

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
public: // type definitions and constants
    static const QString c_strXmlElemNameDrawing;
    static const QString c_strXmlElemNameGraphObj;
    static const QString c_strXmlElemNameDrawSettings;
    static const QString c_strXmlElemNameGeometry;
    static const QString c_strXmlElemNameShapePoints;
    static const QString c_strXmlElemNameShapePointP1;
    static const QString c_strXmlElemNameShapePointP2;
    static const QString c_strXmlElemNameZValue;
    static const QString c_strXmlElemNameLabels;
public: // type definitions and constants
    static const QString c_strXmlAttrDimensionUnit;
    static const QString c_strXmlAttrScreenResolutionPxPerMilliMeter;
    static const QString c_strXmlAttrMetricImageCoorsDecimals;
    static const QString c_strXmlAttrUnit;
    static const QString c_strXmlAttrWidth;
    static const QString c_strXmlAttrHeight;
    static const QString c_strXmlAttrScaleFactor;
    static const QString c_strXmlAttrPaperSize;
    static const QString c_strXmlAttrPaperOrientation;
    static const QString c_strXmlAttrYScaleAxisOrientation;
    static const QString c_strXmlAttrGridLinesVisible;
    static const QString c_strXmlAttrGridLinesStyle;
    static const QString c_strXmlAttrGridLinesWidth;
    static const QString c_strXmlAttrGridLinesColor;
    static const QString c_strXmlAttrGridLabelsVisible;
    static const QString c_strXmlAttrGridLabelsFont;
    static const QString c_strXmlAttrGridLabelsTextSize;
    static const QString c_strXmlAttrGridLabelsTextColor;
    static const QString c_strXmlAttrGridLabelsTextStyle;
    static const QString c_strXmlAttrGridLabelsTextEffect;
    static const QString c_strXmlAttrGraphObjFactoryGroupName;
    static const QString c_strXmlAttrGraphObjType;
    static const QString c_strXmlAttrGraphObjName;
public: // class methods
    //static QString FindUniqueGraphObjId(
    //    const QMap<QString,CGraphObj*>& i_dctpGraphObjs,
    //    const QString&                  i_strKeyInTreeCurr );
public: // ctors and dtor
    CDrawingScene(const QString& i_strName, QObject* i_pObjParent = nullptr);
    ~CDrawingScene();
signals:
    /*! Signal emitted if the drawing size has been changed.
        @param i_size [in] Contains the new drawing size (pixel dimension, metric, etc.). */
    void drawingSizeChanged( const ZS::Draw::CDrawingSize& i_size );
    /*! Signal emitted if the grid settings has been changed.
        @param i_settings [in] Contains the new grid settings (visibilities of lines, labels, etc.). */
    void gridSettingsChanged( const ZS::Draw::CDrawGridSettings& i_settings );
    /*! Signal emitted if the mouse positions has been changed.
        @param i_ptMousePos [in] Current mouse position in pixels. */
    void mousePosChanged( const QPointF& i_ptMousePos );
    /*! Signal emitted if the mode has been changed.
        The signal is emitted if the mode changes between Edit and View mode (see enum ZS::System:EMode) or
        if the selected edit tool changed (see enum ZS::Draw::EEditTool) or
        if the edit mode changed (see enum ZS::Draw::EEditMode) or
        if the resize mode changed (see enum ZS::Draw::EEditResizeMode).
        See also method CDrawingScene::setMode. */
    void modeChanged();
    /*! Signal emitted if the draw set has been changed.
        @param i_drawSettings [in] Contains the new draw settings (pen style, line color, font, etc.). */
    void drawSettingsChanged( const ZS::Draw::CDrawSettings& i_drawSettings );
    /*! Signal emitted if an object has been added.
        @param i_strKeyInTree [in] Unique key of the graphical object. */
    void graphObjAdded(const QString& i_strKeyInTree);
    /*! Signal emitted if a property (labels, geometry, drawing settings, etc.) of a
        graphical object has been changed.
        Please note that this signal is not emitted if the object is renamed or moved to another parent.
        @param i_strKeyInTree [in] Unique key of the graphical object. */
    void graphObjChanged(const QString& i_strKeyInTree);
    /*! Signal emitted if a graphical object has been renamed.
        @param i_strNewKeyInTree [in] Unique key after moving the object.
        @param i_strOrigKeyInTree [in] Unique key before moving the object.
        @param i_strKeyInTreeOfTargetBranch [in]
            Name of the object in the new target branch after moving the object.
            Provided for convenience as this is the last section of i_strNewKeyInTree. */
    void graphObjMoved(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch);
    /*! Signal emitted if a graphical object has been renamed.
        @param i_strNewKeyInTree [in] Unique key after renaming the object.
        @param i_strOrigKeyInTree [in] Unique key before renaming the object.
        @param i_strOrigName [in]
            Name before renaming the object.
            Provided for convenience as this is the last section of i_strOrigKeyInTree. */
    void graphObjRenamed(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strOrigName);
    /*! Signal emitted if an object has been destroyed.
        Please note that if the signal is emitted you can no longer access the properties of the
        object and dynamic casts to CGraphObj or to QGraphicsItem cannot be used anymore.
        @param i_strKeyInTree [in] Unique key of the graphical object. */
    void graphObjAboutToBeDestroyed(const QString& i_strKeyInTree);
public: // instance methods
    void setDrawingSize( const CDrawingSize& i_drawingSize);
    const CDrawingSize& drawingSize() const;
public: // instance methods
    //ZS::PhysVal::CPhysVal toPhysValXCoor(double i_fXCoor_px) const;
    //ZS::PhysVal::CPhysVal toPhysValYCoor(double i_fYCoor_px) const;
    CPhysValPoint toPhysValPoint(const QPointF& i_pt_px) const;
    //CPhysValSize toPhysValSize(const QSizeF& i_size) const;
    CPhysValPoint convert(const CPhysValPoint& i_physValPoint, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValSize convert(const CPhysValSize& i_physValSize, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValLine convert(const CPhysValLine& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const;
    //double toPixelXCoor(const ZS::PhysVal::CPhysVal& i_physValXCoor) const;
    //double toPixelYCoor(const ZS::PhysVal::CPhysVal& i_physValYCoor) const;
    //CPhysValPoint toPixelCoor(const CPhysValPoint& i_physValPoint) const;
    //CPhysValSize toPixelCoor(const CPhysValSize& i_physValSize) const;
    //CPhysValLine toPixelCoor(const CPhysValLine& i_physValLine) const;
    //CPhysValRect toPixelCoor(const CPhysValRect& i_physValRect) const;
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
    void removeGraphObj( CGraphObj* i_pGraphObj );
protected: // instance methods
    void addItem( QGraphicsItem* i_pGraphicsItem, QGraphicsItem* i_pGraphicsItemParent = nullptr );
    void removeItem( QGraphicsItem* i_pGraphicsItem );
    void deleteItem( QGraphicsItem* i_pGraphicsItem );
    void deleteItem( CGraphObj* i_pGraphObj );
protected: // instance methods
    //void addChildItems( QGraphicsItem* i_pGraphicsItem );
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
    void onGraphObjAboutToBeDestroyed( const QString& i_strKeyInTree );
    //void onGraphObjDestroyed( const QString& i_strKeyInTree );
    //void onGraphObjIdChanged( const QString& i_strKeyInTreeOld, const QString& i_strKeyInTreeNew );
    //void onGraphObjNameChanged(
    //    const QString& i_strKeyInTree,
    //    const QString& i_strObjNameOld,     // !!! NOT including name of parents !!!
    //    const QString& i_strObjNameNew );   // !!! NOT including name of parents !!!
public: // instance methods
    QGraphicsItem* findGraphicsItem( const QString& i_strKeyInTree );
    CGraphObj* findGraphObj( const QString& i_strKeyInTree );
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
    void setLineStyle( const CEnumLineStyle& i_lineStyle );
    CEnumLineStyle getLineStyle() const { return m_drawSettings.getLineStyle(); }
public: // instance methods
    void setFillColor( const QColor& i_clr );
    QColor getFillColor() const { return m_drawSettings.getFillColor(); }
    void setFillStyle( const CEnumFillStyle& i_fillStyle );
    CEnumFillStyle getFillStyle() const { return m_drawSettings.getFillStyle(); }
public: // instance methods
    void setLineRecordType( const CEnumLineRecordType& i_recordType );
    CEnumLineRecordType getLineRecordType() const { return m_drawSettings.getLineRecordType(); }
    void setLineExtent( int i_iExtent );
    int  getLineExtent() const { return m_drawSettings.getLineExtent(); }
public: // instance methods
    void setLineEndStyle( const CEnumLinePoint& i_linePoint, const CEnumLineEndStyle& i_endStyle );
    CEnumLineEndStyle getLineEndStyle( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getLineEndStyle(i_linePoint); }
    void setLineEndBaseLineType( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadBaseLineType& i_baseLineType );
    CEnumArrowHeadBaseLineType getLineEndBaseLineType( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getLineEndBaseLineType(i_linePoint); }
    void setLineEndFillStyle( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadFillStyle& i_fillStyle );
    CEnumArrowHeadFillStyle getLineEndFillStyle( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getLineEndFillStyle(i_linePoint); }
    void setLineEndWidth( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadWidth& i_arrowWidth );
    CEnumArrowHeadWidth getLineEndWidth( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getLineEndWidth(i_linePoint); }
    void setLineEndLength( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadLength& i_arrowLength );
    CEnumArrowHeadLength getLineEndLength( const CEnumLinePoint& i_linePoint ) const { return m_drawSettings.getLineEndLength(i_linePoint); }
public: // instance methods
    void setTextColor( const QColor& i_clr );
    QColor getTextColor() const { return m_drawSettings.getTextColor(); }
    void setTextFont( const QFont& i_fnt );
    QFont getTextFont() const { return m_drawSettings.getTextFont(); }
    void setTextSize( const ETextSize i_textSize );
    ETextSize getTextSize() const { return m_drawSettings.getTextSize(); }
    void setTextStyle( const CEnumTextStyle& i_textStyle );
    CEnumTextStyle getTextStyle() const { return m_drawSettings.getTextStyle(); }
    void setTextEffect( const CEnumTextEffect& i_textEffect );
    CEnumTextEffect getTextEffect() const { return m_drawSettings.getTextEffect(); }
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
    void onDrawUnitsScreenResolutionInPxPerMMChanged();
    void onDrawUnitsScaleFactorChanged();
protected slots:
    void onGraphObjFactoryDestroyed( QObject* i_pObjFactory );
protected slots:
    void onGraphObjsIdxTreeEntryAdded( const QString& i_strKeyInTree );
    void onGraphObjsIdxTreeEntryChanged( const QString& i_strKeyInTree );
    void onGraphObjsIdxTreeEntryMoved( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch );
    void onGraphObjsIdxTreeEntryRenamed( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strOrigName );
protected: // auxiliary instance methods
    void paintGridLines(QPainter* i_pPainter);
protected: // auxiliary instance methods
    void raiseErrorAttributeNotDefined(QXmlStreamReader& i_xmlStreamReader, const QString& i_strElemName, const QString& i_strAttrName) const;
    void raiseErrorAttributeOutOfRange(QXmlStreamReader& i_xmlStreamReader, const QString& i_strElemName, const QString& i_strAttrName, const QString& i_strAttrVal) const;
protected: // auxiliary instance methods
    ZS::System::CEnumScaleDimensionUnit getDimensionUnit(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const ZS::System::CEnumScaleDimensionUnit& i_eDefaultVal = ZS::System::EScaleDimensionUnit::Pixels) const;
    CEnumNormedPaperSize getNormedPaperSize(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumNormedPaperSize& i_eDefaultVal = CEnumNormedPaperSize()) const;
    ZS::System::CEnumOrientation getOrientation(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const ZS::System::CEnumOrientation& i_eDefaultVal = ZS::System::CEnumOrientation()) const;
    CEnumLineStyle getLineStyle(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumLineStyle& i_eDefaultVal = ELineStyle::SolidLine) const;
    ETextSize getTextSize(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, ETextSize i_eDefaultVal = ETextSize11) const;
    CEnumTextStyle getTextStyle(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumTextStyle& i_eDefaultVal = ETextStyle::Normal) const;
    CEnumTextEffect getTextEffect(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const CEnumTextEffect& i_eDefaultVal = ETextEffect::None) const;
    ZS::PhysVal::CUnit getUnit(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const ZS::PhysVal::CUnit& i_unitDefault = Units.Length.mm) const;
    ZS::PhysVal::CPhysVal getPhysVal(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const ZS::PhysVal::CPhysVal& i_physValDefault = ZS::PhysVal::CPhysVal(0.0, Units.Length.mm)) const;
    QFont getFont(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const QFont& i_fntDefault = QFont()) const;
    QColor getColor(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, const QColor& i_clrDefault = Qt::black) const;
    bool getBoolVal(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, bool i_bDefaultVal = false) const;
    int getIntVal(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, int iValDefault = 0) const;
    std::pair<int, int> getIntPair(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, const QString& i_strDelimiter = ":", bool i_bAttrIsMandatory = true, const std::pair<int, int>& i_valDefault = std::make_pair(0, 0)) const;
    double getDoubleVal(QXmlStreamReader& i_xmlStreamReader, QXmlStreamAttributes& i_xmlStreamAttrs, const QString& i_strElemName, const QString& i_strAttrName, bool i_bAttrIsMandatory = true, double fValDefault = 0.0) const;
protected: // auxiliary instance methods (trace emitting signals)
    void emit_drawingSizeChanged( const ZS::Draw::CDrawingSize& i_size );
    void emit_gridSettingsChanged( const ZS::Draw::CDrawGridSettings& i_settings );
    void emit_mousePosChanged( const QPointF& i_ptMousePos );
    void emit_modeChanged();
    void emit_drawSettingsChanged( const ZS::Draw::CDrawSettings& i_drawSettings );
    void emit_graphObjAdded(const QString& i_strKeyInTree);
    void emit_graphObjChanged(const QString& i_strKeyInTree);
    void emit_graphObjMoved(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch);
    void emit_graphObjRenamed(const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strOrigName);
    void emit_graphObjAboutToBeDestroyed(const QString& i_strKeyInTree);
protected: // auxiliary instance methods (method tracing)
    void traceInternalStates(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_mthDir, ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug);
protected: // instance members
    CDrawingSize m_drawingSize;
    CDrawGridSettings m_gridSettings;
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetricsX;
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetricsY;
    CDrawSettings m_drawSettings;
    ZS::System::CEnumMode m_mode;
    CEnumEditTool m_editTool;
    CEnumEditMode m_editMode;
    CEnumEditResizeMode m_editResizeMode;
    QGraphicsRectItem* m_pGraphicsItemSelectionArea;
    /*!< Corresponds to the selected drawing tool. */
    CObjFactory* m_pObjFactory;
    /*!< Same object as GraphObjCreating (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    QGraphicsItem* m_pGraphicsItemCreating;
    /*! Same object as GraphicsItemCreating (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    CGraphObj* m_pGraphObjCreating;
    /*! Same object as GraphObjAddingShapePoints (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    QGraphicsItem* m_pGraphicsItemAddingShapePoints;
    /*! Same object as GraphicsItemAddingShapePoints (just a different name for the same thing to avoid unnecessary dynamic_casts). */
    CGraphObj* m_pGraphObjAddingShapePoints;
    QString m_strGraphObjNameSeparator;
    ZS::System::CIdxTree* m_pGraphObjsIdxTree;
    ZS::System::CIdxTree* m_pGraphObjsIdxTreeClipboard;
    QList<QGraphicsItem*> m_arpGraphicsItemsAcceptingHoverEvents;
    double m_fRotAngleRes_degree;
    double m_fHitTolerance_px;
    bool m_bMouseDoubleClickEventInProcess;
    QPointF m_ptMouseEvScenePosOnMousePressEvent;
    int m_iEvKeyModifiers;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjPaintEvent;

}; // class CDrawingScene

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingScene_h
