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

#ifndef ZSDraw_GraphObj_h
#define ZSDraw_GraphObj_h

#include <QtGui/qcursor.h>

#include "ZSSys/ZSSysIdxTreeEntry.h"
#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawSettings.h"
#include "ZSDraw/ZSDrawGraphObjEventFct.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CGraphObj;
class CGraphObjLabel;
class CGraphObjSelectionPoint;
struct SGraphObjLabel;


//==============================================================================
/*!
*/
struct ZSDRAWDLL_API SGraphObjHitInfo
//==============================================================================
{
public: // ctor
    SGraphObjHitInfo();
public: // struct methods
    bool isBoundingRectSelectionPointHit() const;
    bool isSelectionPointHit() const;
    bool isPolygonShapePointHit() const;
    bool isLineSegmentHit() const;
public: // struct methods
    void setCursor( double i_fGraphObjRotAngle_rad ); // calculated depending on editMode, editResizeMode and selected point (which must have been set before)
public: // struct members
    CEnumEditMode       m_editMode;
    CEnumEditResizeMode m_editResizeMode;
    CEnumSelectionPoint m_selPtBoundingRect;
    int                 m_idxPolygonShapePoint;
    int                 m_idxLineSegment;
    QPointF             m_ptSelected;
    QCursor             m_cursor;

}; // struct SGraphObjHitInfo


//==============================================================================
struct ZSDRAWDLL_API SGraphObjAlignment
//==============================================================================
{
public: // ctor
    SGraphObjAlignment() :
        m_alignmentRefChild(EAlignmentRef::Undefined),
        m_alignmentRefParent(EAlignmentRef::Undefined),
        m_bAlignAbsolute(true),
        m_fVal(0.0)
    {
    }
    SGraphObjAlignment(
        EAlignmentRef i_refChild,
        EAlignmentRef i_refParent,
        bool          i_bAbsolute,
        double        i_fVal = 0.0 ) :
        m_alignmentRefChild(i_refChild),
        m_alignmentRefParent(i_refParent),
        m_bAlignAbsolute(i_bAbsolute),
        m_fVal(i_fVal)
    {
    }
public: // struct methods
    QString toString() const;
public: // struct members
    CEnumAlignmentRef m_alignmentRefChild;
    CEnumAlignmentRef m_alignmentRefParent;
    bool              m_bAlignAbsolute;
    double            m_fVal; // if aligned absolute value in pixels, if aligned relative scale factor to width or height of parent (usually ranging from 0..1)

}; // struct SGraphObjAlignment


//******************************************************************************
class ZSDRAWDLL_API CGraphObj : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObj"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    static const QString c_strKeyLabelObjName;
    static const QString c_strKeyLabelObjId;
protected: // ctor
    CGraphObj(
        CDrawingScene*       i_pDrawingScene,
        const QString&       i_strFactoryGroupName,
        EGraphObjType        i_type,
        const QString&       i_strType,
        const QString&       i_strObjName,
        const CDrawSettings& i_drawSettings );
public: // dtor
    virtual ~CGraphObj();
public: // must overridables
    virtual CGraphObj* clone() = 0;
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
public: // instance methods
    CDrawingScene* getDrawingScene() { return m_pDrawingScene; }
protected: // overridables of base class CIdxTreeEntry
    virtual void setName( const QString& i_strName ) override;
    virtual void setKeyInTree( const QString& i_strKey ) override;
public: // instance methods
    QString getFactoryGroupName() const { return m_strFactoryGroupName; }
    EGraphObjType getType() const { return m_type; }
    QString getTypeAsString() const { return m_strType; }
    CEnumEditMode getEditMode() const { return m_editMode; }
    CEnumEditResizeMode getEditResizeMode() const { return m_editResizeMode; }
public: // overridables
    virtual QString getScenePolygonShapePointsString() const { return ""; } // for subsystem test
public: // instance methods
    int getSelectedPolygonShapePointIndex() const { return m_idxSelPtSelectedPolygon; }
    CEnumSelectionPoint getSelectedBoundingRectPoint() const { return m_selPtSelectedBoundingRect; }
    QString getToolTip() const { return m_strToolTip; }
    QString getEditInfo() const { return m_strEditInfo; }
public: // instance methods
    CGraphObj* parentGraphObj();
public: // overridables
    virtual void onCreateAndExecDlgFormatGraphObjs(); // must be overridden to create a user defined dialog
public: // overridables
    virtual void setDrawSettings( const CDrawSettings& i_drawSettings );
    virtual CDrawSettings getDrawSettings() const { return m_drawSettings; }
    virtual void onDrawSettingsChanged(); // must be overridden to apply the draw settings at the graphics item in derived classes
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setPenColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getPenColor() const { return m_drawSettings.getPenColor(); }
    virtual void setPenWidth( int i_iLineWidth, bool i_bImmediatelyApplySetting = true );
    virtual int getPenWidth() const { return m_drawSettings.getPenWidth(); }
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineStyle( ELineStyle i_lineStyle, bool i_bImmediatelyApplySetting = true );
    virtual ELineStyle getLineStyle() const { return m_drawSettings.getLineStyle(); }
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setFillColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getFillColor() const { return m_drawSettings.getFillColor(); }
    virtual void setFillStyle( EFillStyle i_fillStyle, bool i_bImmediatelyApplySetting = true );
    virtual EFillStyle getFillStyle() const { return m_drawSettings.getFillStyle(); }
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineRecordType( ELineRecordType i_lineRecordType, bool i_bImmediatelyApplySetting = true );
    virtual ELineRecordType getLineRecordType() const { return m_drawSettings.getLineRecordType(); }
    virtual void setLineExtent( int i_iLineExtent_px, bool i_bImmediatelyApplySetting = true );
    virtual int getLineExtent() const { return m_drawSettings.getLineExtent(); }
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineEndStyle( ELinePoint i_linePoint, ELineEndStyle i_endStyle, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndStyle getLineEndStyle( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndStyle(i_linePoint); }
    virtual void setLineEndBaseLineType( ELinePoint i_linePoint, ELineEndBaseLineType i_baseLineType, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndBaseLineType getLineEndBaseLineType( ELinePoint i_linePoint) const { return m_drawSettings.getLineEndBaseLineType(i_linePoint); }
    virtual void setLineEndFillStyle( ELinePoint i_linePoint, ELineEndFillStyle i_fillStyle, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndFillStyle getLineEndFillStyle( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndFillStyle(i_linePoint); }
    virtual void setLineEndWidth( ELinePoint i_linePoint, ELineEndWidth i_width, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndWidth getLineEndWidth( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndWidth(i_linePoint); }
    virtual void setLineEndLength( ELinePoint i_linePoint, ELineEndLength i_length, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndLength getLineEndLength( ELinePoint i_linePoint ) const { return m_drawSettings.getLineEndLength(i_linePoint); }
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setTextColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getTextColor() const { return m_drawSettings.getTextColor(); }
    virtual void setTextFont( const QFont& i_fnt, bool i_bImmediatelyApplySetting = true );
    virtual QFont getTextFont() const { return m_drawSettings.getTextFont(); }
    virtual void setTextSize( ETextSize i_size, bool i_bImmediatelyApplySetting = true );
    virtual ETextSize getTextSize() const { return m_drawSettings.getTextSize(); }
    virtual void setTextStyle( ETextStyle i_style, bool i_bImmediatelyApplySetting = true );
    virtual ETextStyle getTextStyle() const { return m_drawSettings.getTextStyle(); }
    virtual void setTextEffect( ETextEffect i_effect, bool i_bImmediatelyApplySetting = true );
    virtual ETextEffect getTextEffect() const { return m_drawSettings.getTextEffect(); }
public: // overridables
    virtual void setMinimumWidth( double i_fWidth );
    virtual bool hasMinimumWidth() const;
    virtual double getMinimumWidth() const;
    virtual void setMinimumHeight( double i_fHeight );
    virtual bool hasMinimumHeight() const;
    virtual double getMinimumHeight() const;
    virtual void setMinimumSize( const QSize& i_siz );
    virtual bool hasMinimumSize() const;
    virtual QSize getMinimumSize() const;
    virtual void setMaximumWidth( double i_fWidth );
    virtual bool hasMaximumWidth() const;
    virtual double getMaximumWidth() const;
    virtual void setMaximumHeight( double i_fHeight );
    virtual bool hasMaximumHeight() const;
    virtual double getMaximumHeight() const;
    virtual void setMaximumSize( const QSize& i_siz );
    virtual bool hasMaximumSize() const;
    virtual QSize getMaximumSize() const;
    virtual void setFixedWidth( double i_fWidth );
    virtual bool hasFixedWidth() const;
    virtual double getFixedWidth() const;
    virtual void setFixedHeight( double i_fHeight );
    virtual bool hasFixedHeight() const;
    virtual double getFixedHeight() const;
    virtual void setFixedSize( const QSize& i_siz );
    virtual bool hasFixedSize() const;
    virtual QSize getFixedSize() const;
public: // overridables
    // The alignments will be adjusted in the order they are added. The order takes effect on the result.
    // Usually the size should be adjusted before the positions to get relative adjustments working as expected.
    virtual int addAlignment( const SGraphObjAlignment& i_alignment );
    virtual int getAlignmentCount() const;
    virtual SGraphObjAlignment getAlignment( int i_idx ) const;
    virtual void setAlignment( int i_idx, const SGraphObjAlignment& i_alignment );
    virtual void removeAlignment( int i_idx );
    virtual void clearAlignments();
public: // overridables
    virtual void acceptCurrentAsOriginalCoors();
    virtual bool hasValidOrigCoors() const { return m_bHasValidOrigCoors; }
public: // must overridables
    virtual void setWidth( double i_fWidth ) = 0;
    virtual void setHeight( double i_fHeight ) = 0;
    virtual void setSize( double i_fWidth, double i_fHeight ) = 0;
    virtual void setSize( const QSizeF& i_size ) = 0;
public: // overridables
    virtual QPointF getPos( ECoordinatesVersion i_version = ECoordinatesVersion::Current ) const;
    virtual double getWidth( ECoordinatesVersion i_version = ECoordinatesVersion::Current ) const;
    virtual double getHeight( ECoordinatesVersion i_version = ECoordinatesVersion::Current ) const;
    virtual QSizeF getSize( ECoordinatesVersion i_version = ECoordinatesVersion::Current ) const;
public: // overridables
    virtual void setRotationAngleInDegree( double i_fRotAngle_deg );
    virtual double getRotationAngleInDegree( ECoordinatesVersion i_version = ECoordinatesVersion::Current );
public: // overridables
    virtual void setEditMode( EEditMode i_editMode );
    virtual void setEditResizeMode( EEditResizeMode i_editResizeMode );
public: // overridables
public: // must overridables
    virtual void setIsHit( bool i_bHit ) = 0; // to indicate that object is hit by mouse cursor by either showing selection points or a dotted bounding rectangle or ...
public: // overridables
    virtual bool isHit() const { return m_bIsHit; }
    virtual bool isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const;
public: // overridables
    virtual double bringToFront();                          // returns the new ZValue
    virtual void setStackingOrderValue( double i_fZValue ); // you must call this method instead of setZValue of QGraphicsItem
    double getStackingOrderValue() { return m_fZValue; }
public: // overridables
    virtual bool isBoundingRectSelectionPointHit( const QPointF& i_pt, int i_iSelPtsCount, const ESelectionPoint* i_pSelPts, SGraphObjHitInfo* o_pHitInfo ) const;
    virtual bool isPolygonSelectionPointHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const;
public: // overridables
    virtual QPointF getSelectionPoint( ESelectionPoint i_selPt ) const; // returns coordinates of selection point in item's coordinate system
protected: // must overridables
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) = 0; // also creates the selection points if not yet created
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) = 0;
protected: // overridables
    virtual void hideSelectionPoints( ESelectionPoints i_selPts = ESelectionPointsAll ); // not just hides but also destroys the selection points
    virtual void bringSelectionPointsToFront( ESelectionPoints i_selPts = ESelectionPointsAll );
protected: // overridables
    virtual void showSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts = ESelectionPointsBoundingRectAll );    // creates the selection points if not yet created
    virtual void updateSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts = ESelectionPointsBoundingRectAll );
    virtual void showSelectionPointsOfPolygon( const QPolygonF& i_plg );                                                                // creates the selection points if not yet created
    virtual void updateSelectionPointsOfPolygon( const QPolygonF& i_plg );
public: // overridables
    virtual void showObjName( ESelectionPoint i_selPtPos = ESelectionPoint::TopCenter );     // strKeyLabel = "ObjName"
    virtual void hideObjName();     // strKeyLabel = "ObjName"
    virtual bool isObjNameVisible() const;
    virtual void showObjId( ESelectionPoint i_selPtPos = ESelectionPoint::TopCenter );       // strKeyLabel = "ObjId"
    virtual void hideObjId();       // strKeyLabel = "ObjId"
    virtual bool isObjIdVisible() const;
public: // overridables
    virtual void addLabel( const QString& i_strKey, const QString& i_strText, ESelectionPoint i_selPtPos = ESelectionPoint::TopCenter );  // does not create the graph object label item
    virtual void removeLabel( const QString& i_strKey );    // also destroys the graph object label item
    virtual void showLabel( const QString& i_strKey );      // creates the graph object label item
    virtual void hideLabel( const QString& i_strKey );      // destroys the graph object label item
    virtual void setLabelText( const QString& i_strKey, const QString& i_strText );
    virtual bool isLabelVisible( const QString& i_strKey ) const;
    virtual void updateLabelDistance( const QString& i_strKey );
    virtual void updateLabelPositions();
public: // overridables
    virtual void showLabels(); // also creates the labels if not yet created
    virtual void hideLabels(); // not just hides but also destroys the labels
public: // instance methods
    QStringList getLabelsKeys() const;
    SGraphObjLabel* getLabel( const QString& i_strKey ) const;
    QHash<QString,SGraphObjLabel*> getLabels() { return m_arpLabels; }
    void addLabels( QHash<QString,SGraphObjLabel*> i_arpLabels );
public: // overridables
    virtual void onParentItemCoorsHasChanged( CGraphObj* /*i_pGraphObjParent*/ ) {}
    virtual void onSelectionPointDestroying( CGraphObjSelectionPoint* i_pSelectionPoint );
public: // instance methods (simulation methods)
    void addMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void removeMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void addMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void removeMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void addMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void removeMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void addMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void removeMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void addKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void removeKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void addKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
    void removeKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr );
protected: // overridables
    virtual void updateTransform();
    virtual void updateToolTip();
    virtual void updateEditInfo();
protected: // instance members
    CDrawingScene*                    m_pDrawingScene;
    QString                           m_strFactoryGroupName;
    EGraphObjType                     m_type;
    QString                           m_strType;
    CDrawSettings                     m_drawSettings;     // Set by ctor or setSettings. Changed also by graphics items methods "setPen", "setBrush", etc.. Call "updateSettings" before accessing settings to keep this struct up to date with graphics item settings.
    QSize                             m_sizMinimum;
    QSize                             m_sizMaximum;
    QSize                             m_sizFixed;
    QList<SGraphObjAlignment>         m_arAlignments;
    bool                              m_bIsHit;
    CEnumEditMode                     m_editMode;
    CEnumEditResizeMode               m_editResizeMode;
    double                            m_fZValue;
    bool                              m_bDtorInProgress;
    int                               m_idxSelPtSelectedPolygon;
    QList<CGraphObjSelectionPoint*>   m_arpSelPtsPolygon;
    CEnumSelectionPoint               m_selPtSelectedBoundingRect;
    QVector<CGraphObjSelectionPoint*> m_arpSelPtsBoundingRect;
    QHash<QString,SGraphObjLabel*>    m_arpLabels;        // Keys e.g. c_strKeyLabelObjName", "Resistance", etc.
    QString                           m_strToolTip;
    QString                           m_strEditInfo;
    // Current item coordinates and transform values:
    QRectF                            m_rctCurr;              // in item's coordinate system (during mouse resize events topLeft may not be at 0.0/0.0)
    double                            m_fRotAngleCurr_deg;    // concerning range see "Draw::getAngleRad" (Q1: 0..90°, Q2: 90..180°, Q3: -90°..-180°, Q4: 0..-90°)
    QPointF                           m_ptRotOriginCurr;      // in item's coordinate system
    // Original item coordinates and transform values:
    // - after initially creating the item
    // - changing position, size and rotation angle of objects with no parents
    // - on adding items to groups
    // - explicitly calling method "acceptCurrentAsOriginalCoors"
    bool                              m_bHasValidOrigCoors;
    QPointF                           m_ptPosOrig;            // in parent's coordinate system
    QSizeF                            m_sizOrig;
    double                            m_fRotAngleOrig_deg;    // concerning range see "Draw::getAngleRad" (Q1: 0..90°, Q2: 90..180°, Q3: -90°..-180°, Q4: 0..-90°)
    QPointF                           m_ptRotOriginOrig;      // in item's coordinate system
    // Coordinates stored on mouse press events:
    QPointF                           m_ptScenePosOnMousePressEvent;          // in scene's coordinate system (for moving by my mouse move events)
    QPointF                           m_ptMouseEvScenePosOnMousePressEvent;   // in scene's coordinate system (for moving by my mouse move events)
    QRectF                            m_rctOnMousePressEvent;                 // in item's coordinate system (for resizing by mouse move events)
    QPointF                           m_ptRotOriginOnMousePressEvent;         // in scene's coordinate system (for rotation by mouse move events)
    // Simulation Functions:
    QList<SGraphObjMouseEventFct>     m_arMousePressEventFunctions;
    QList<SGraphObjMouseEventFct>     m_arMouseReleaseEventFunctions;
    QList<SGraphObjMouseEventFct>     m_arMouseDoubleClickEventFunctions;
    QList<SGraphObjMouseEventFct>     m_arMouseMoveEventFunctions;
    QList<SGraphObjKeyEventFct>       m_arKeyPressEventFunctions;
    QList<SGraphObjKeyEventFct>       m_arKeyReleaseEventFunctions;
    // Method Tracing (has to be created in ctor of "final" class)
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjCtorsAndDtor;
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjItemChange;
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjBoundingRect;
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjPaint;
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjSceneEvent;
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjSceneEventFilter;
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjHoverEvents;
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjMouseEvents;
    ZS::Trace::CTrcAdminObj*          m_pTrcAdminObjKeyEvents;

}; // class CGraphObj

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObj_h
