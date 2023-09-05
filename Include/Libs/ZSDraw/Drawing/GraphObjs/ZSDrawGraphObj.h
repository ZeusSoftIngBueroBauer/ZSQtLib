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

#ifndef ZSDraw_GraphObj_h
#define ZSDraw_GraphObj_h

#include "ZSSys/ZSSysIdxTreeEntry.h"
#include "ZSDraw/Common/ZSDrawPhysValLine.h"
#include "ZSDraw/Common/ZSDrawPhysValRect.h"
#include "ZSDraw/Common/ZSDrawPhysValSize.h"
#include "ZSDraw/Common/ZSDrawSettings.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEventFct.h"

#include <QtGui/qcursor.h>

namespace ZS
{
namespace System
{
class CTrcAdminObj;
class CMethodTracer;
}

namespace Draw
{
class CDrawingScene;
class CGraphObj;
class CGraphObjLabel;
class CGraphObjSelectionPoint;


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
    void setCursor(double i_fGraphObjRotAngle_rad);
public: // struct methods
    QString toString() const;
public: // struct members
    CEnumEditMode m_editMode;
    CEnumEditResizeMode m_editResizeMode;
    CEnumSelectionPoint m_selPtBoundingRect;
    int m_idxPolygonShapePoint;
    int m_idxLineSegment;
    QPointF m_ptSelected;
    QCursor m_cursor;

}; // struct SGraphObjHitInfo


//==============================================================================
struct ZSDRAWDLL_API SGraphObjAlignment
//==============================================================================
{
public: // ctor
    SGraphObjAlignment() :
        m_alignmentRefChild(EAlignmentRef::None),
        m_alignmentRefParent(EAlignmentRef::None),
        m_bAlignAbsolute(true),
        m_fVal(0.0)
    {
    }
    SGraphObjAlignment(EAlignmentRef i_refChild, EAlignmentRef i_refParent, bool i_bAbsolute, double i_fVal = 0.0) :
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
    bool m_bAlignAbsolute;
    /*!< If aligned absolute: value in pixels.
         If aligned relative: scale factor to width or height of parent (usually ranging from 0..1) */
    double m_fVal;

}; // struct SGraphObjAlignment


//******************************************************************************
/*! @brief Abstract base class for all graphical objects within ZS::Draw.

    Please note that this class is not derived from QGraphicsItem to avoid double
    inheritance from QGraphicsItem as the specialized classes like CGraphObjLine
    are derived from specialized QGraphicsItem classes like QQGraphicsLineItem
    which are already derived from QGraphicsItem.

    Example Inheritance Graph for CGraphObjLine:

      +---------------+     +----------------+
      | CIdxTreeEntry |     | QGraphicsItem  |
      +---------------+     +----------------+
              +                      +
              |                      |
      +---------------+    +-------------------+
      |   CGraphObj   |    | QGraphicsLineItem |
      +---------------+    +-------------------+
                 +             +
                  \           /
               +-----------------+
               |  CGraphObjLine  |
               +-----------------+

    As CGraphObj is derived from CIdxTreeEntry each graphical object can be added
    to an index tree and can be shown and accessed through a tree view.

    Several labels may be assigned to a graphical object. The labels may be shown
    within the drawing view. The labels may be anchored to different points of
    the graphical labels and may be shown within the drawing view. To which points
    the labels can be anchored depend on the type of the graphical object. For lines
    for example the labels may be anchored to the start and end point or to the center
    point of the line. Anchor lines drawn between the labels and the anchor points may
    also be shown for each label separately. The name of the graphical object is treated
    as a special label as - in contrary to the other labels - each graphical object
    must contain a unique name. All other labels are optional.

    Example for CGraphObjLine:

    - Name label "Horizontal Line" anchored to the center point.
    - Optionally added label "Start" anchored to the start point.
    - Optionally added label "Stop" anchored to the stop point.

      Start      Horizontal Line    Stop
        |             |              |
        ------------------------------

    It is also possible to show the geometry information for the graphical objects
    within the drawing view.


    The following pure virtual methods must be overriden by derived classes:

    - CGraphObj* clone();

    - void setWidth(const CPhysVal& i_physValWidth);

    - void setHeight(const CPhysVal& i_physValHeight);

    - void setSize(const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight);

    - void setSize(const CPhysValSizeF& i_physValSize);

    - bool hasBoundingRect() const;

    - bool hasLineShapePoints() const;

    - bool hasRotationSelectionPoints() const;

    - void setIsHit(bool i_bHit);
      To indicate that object is hit by mouse cursor by either showing selection points
      or a dotted bounding rectangle or ...

    - void showSelectionPoints(unsigned char i_selPts = ESelectionPointsAll);
      Also creates the selection points if not yet created.

    - void updateSelectionPoints(unsigned char i_selPts = ESelectionPointsAll);
*/
class ZSDRAWDLL_API CGraphObj : public QObject, public ZS::System::CIdxTreeEntry
//******************************************************************************
{
//friend class CGraphObjLabel;
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObj"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
protected: // ctor
    CGraphObj(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strFactoryGroupName,
        EGraphObjType i_type,
        const QString& i_strType,
        const QString& i_strObjName,
        const CDrawSettings& i_drawSettings,
        ZS::System::CIdxTreeEntry::EEntryType i_idxTreeEntryType = ZS::System::CIdxTreeEntry::EEntryType::Branch );
public: // dtor
    virtual ~CGraphObj();
signals:
    void selectedChanged();
    void geometryChanged();
protected: // instance methods (trace admin objects for method tracing)
    void createTraceAdminObjs(const QString& i_strClassName);
    void releaseTraceAdminObjs();
public: // must overridables
    virtual CGraphObj* clone() = 0;
public: // instance methods
    EGraphObjType type() const;
    QString typeAsString() const;
    bool isPoint() const;
    bool isLine() const;
    bool isRect() const;
    bool isEllipse() const;
    bool isPolygon() const;
    bool isPolyline() const;
    bool isText() const;
    bool isImage() const;
    bool isConnectionPoint() const;
    bool isConnectionLine() const;
    bool isGroup() const;
    bool isSelectionPoint() const;
    bool isLabel() const;
public: // instance methods
    CDrawingScene* getDrawingScene();
public: // overridables
    virtual void rename( const QString& i_strNameNew );
protected: // overridables of base class CIdxTreeEntry
    virtual void setName( const QString& i_strName ) override;
    virtual void setKeyInTree( const QString& i_strKey ) override;
public: // instance methods
    QString getFactoryGroupName() const;
    CEnumEditMode getEditMode() const;
    CEnumEditResizeMode getEditResizeMode() const;
public: // overridables
    virtual QString getScenePolygonShapePointsString() const;
public: // instance methods
    int getSelectedPolygonShapePointIndex() const;
    CEnumSelectionPoint getSelectedBoundingRectPoint() const;
    QString getToolTip() const;
    QString getEditInfo() const;
public: // instance methods
    CGraphObj* parentGraphObj();
public: // overridables
    virtual void onCreateAndExecDlgFormatGraphObjs();
public: // overridables
    virtual void setDrawSettings( const CDrawSettings& i_drawSettings );
    virtual CDrawSettings getDrawSettings() const;
    virtual void onDrawSettingsChanged();
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setPenColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getPenColor() const;
    virtual void setPenWidth( int i_iLineWidth, bool i_bImmediatelyApplySetting = true );
    virtual int getPenWidth() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineStyle( ELineStyle i_lineStyle, bool i_bImmediatelyApplySetting = true );
    virtual ELineStyle getLineStyle() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setFillColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getFillColor() const;
    virtual void setFillStyle( EFillStyle i_fillStyle, bool i_bImmediatelyApplySetting = true );
    virtual EFillStyle getFillStyle() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineRecordType( ELineRecordType i_lineRecordType, bool i_bImmediatelyApplySetting = true );
    virtual ELineRecordType getLineRecordType() const;
    virtual void setLineExtent( int i_iLineExtent_px, bool i_bImmediatelyApplySetting = true );
    virtual int getLineExtent() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineEndStyle( ELinePoint i_linePoint, ELineEndStyle i_endStyle, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndStyle getLineEndStyle( ELinePoint i_linePoint ) const;
    virtual void setLineEndBaseLineType( ELinePoint i_linePoint, ELineEndBaseLineType i_baseLineType, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndBaseLineType getLineEndBaseLineType( ELinePoint i_linePoint ) const;
    virtual void setLineEndFillStyle( ELinePoint i_linePoint, ELineEndFillStyle i_fillStyle, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndFillStyle getLineEndFillStyle( ELinePoint i_linePoint ) const;
    virtual void setLineEndWidth( ELinePoint i_linePoint, ELineEndWidth i_width, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndWidth getLineEndWidth( ELinePoint i_linePoint ) const;
    virtual void setLineEndLength( ELinePoint i_linePoint, ELineEndLength i_length, bool i_bImmediatelyApplySetting = true );
    virtual ELineEndLength getLineEndLength( ELinePoint i_linePoint ) const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setTextColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getTextColor() const;
    virtual void setTextFont( const QFont& i_fnt, bool i_bImmediatelyApplySetting = true );
    virtual QFont getTextFont() const;
    virtual void setTextSize( ETextSize i_size, bool i_bImmediatelyApplySetting = true );
    virtual ETextSize getTextSize() const;
    virtual void setTextStyle( ETextStyle i_style, bool i_bImmediatelyApplySetting = true );
    virtual ETextStyle getTextStyle() const;
    virtual void setTextEffect( ETextEffect i_effect, bool i_bImmediatelyApplySetting = true );
    virtual ETextEffect getTextEffect() const;
public: // overridables
    virtual void setMinimumWidth( const ZS::PhysVal::CPhysVal& i_physValWidth );
    virtual bool hasMinimumWidth() const;
    virtual ZS::PhysVal::CPhysVal getMinimumWidth() const;
    virtual void setMinimumHeight( const ZS::PhysVal::CPhysVal& i_physValHeight );
    virtual bool hasMinimumHeight() const;
    virtual ZS::PhysVal::CPhysVal getMinimumHeight() const;
    virtual void setMinimumSize( const CPhysValSize& i_physValSize );
    virtual bool hasMinimumSize() const;
    virtual CPhysValSize getMinimumSize() const;
    virtual void setMaximumWidth( const ZS::PhysVal::CPhysVal& i_physValWidth );
    virtual bool hasMaximumWidth() const;
    virtual ZS::PhysVal::CPhysVal getMaximumWidth() const;
    virtual void setMaximumHeight( const ZS::PhysVal::CPhysVal& i_physValHeight );
    virtual bool hasMaximumHeight() const;
    virtual ZS::PhysVal::CPhysVal getMaximumHeight() const;
    virtual void setMaximumSize( const CPhysValSize& i_physValSize );
    virtual bool hasMaximumSize() const;
    virtual CPhysValSize getMaximumSize() const;
    virtual void setFixedWidth( const ZS::PhysVal::CPhysVal& i_physValWidth );
    virtual bool hasFixedWidth() const;
    virtual ZS::PhysVal::CPhysVal getFixedWidth() const;
    virtual void setFixedHeight( const ZS::PhysVal::CPhysVal& i_physValHeight );
    virtual bool hasFixedHeight() const;
    virtual ZS::PhysVal::CPhysVal getFixedHeight() const;
    virtual void setFixedSize( const CPhysValSize& i_physValSize );
    virtual bool hasFixedSize() const;
    virtual CPhysValSize getFixedSize() const;
public: // overridables
    virtual int addAlignment( const SGraphObjAlignment& i_alignment );
    virtual int getAlignmentCount() const;
    virtual SGraphObjAlignment getAlignment( int i_idx ) const;
    virtual void setAlignment( int i_idx, const SGraphObjAlignment& i_alignment );
    virtual void removeAlignment( int i_idx );
    virtual void clearAlignments();
public: // overridables
    //virtual void acceptCurrentAsOriginalCoors();
    //virtual bool hasValidOrigCoors() const { return m_bHasValidOrigCoors; }
public: // must overridables
    virtual void setWidth( const ZS::PhysVal::CPhysVal& i_physValWidth ) = 0;
    virtual void setHeight( const ZS::PhysVal::CPhysVal& i_physValHeight ) = 0;
    virtual void setSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight ) = 0;
    virtual void setSize( const CPhysValSize& i_physValSize ) = 0;
    virtual bool hasBoundingRect() const = 0;
    virtual bool hasLineShapePoints() const = 0;
    virtual bool hasRotationSelectionPoints() const = 0;
public: // overridables
    virtual CPhysValPoint getPos( ECoordinatesVersion i_version = ECoordinatesVersion::Transformed ) const;
    virtual ZS::PhysVal::CPhysVal getWidth( ECoordinatesVersion i_version = ECoordinatesVersion::Transformed ) const;
    virtual ZS::PhysVal::CPhysVal getHeight( ECoordinatesVersion i_version = ECoordinatesVersion::Transformed ) const;
    virtual CPhysValSize getSize( ECoordinatesVersion i_version = ECoordinatesVersion::Transformed ) const;
public: // overridables
    virtual void setRotationAngleInDegree( double i_fRotAngle_deg );
    virtual double getRotationAngleInDegree( ECoordinatesVersion i_version = ECoordinatesVersion::Transformed );
public: // overridables
    virtual void setEditMode( EEditMode i_editMode );
    virtual void setEditResizeMode( EEditResizeMode i_editResizeMode );
public: // must overridables
    virtual void setIsHit( bool i_bHit ) = 0;
public: // overridables
    virtual bool isHit() const;
    virtual bool isHit(const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr) const;
public: // overridables
    virtual double bringToFront();
    virtual void setStackingOrderValue( double i_fZValue );
    double getStackingOrderValue() const;
public: // overridables
    virtual void showBoundingRect();
    virtual void hideBoundingRect();
    virtual bool isBoundingRectVisible() const;
public: // overridables
    virtual bool isBoundingRectSelectionPointHit( const QPointF& i_pt, int i_iSelPtsCount, const ESelectionPoint* i_pSelPts, SGraphObjHitInfo* o_pHitInfo ) const;
    virtual bool isPolygonSelectionPointHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const;
public: // overridables
    virtual QPointF getSelectionPointCoors( ESelectionPoint i_selPt ) const;
protected: // must overridables
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) = 0;
    virtual void updateSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) = 0;
protected: // overridables
    virtual void hideSelectionPoints( ESelectionPoints i_selPts = ESelectionPointsAll );
    virtual void bringSelectionPointsToFront( ESelectionPoints i_selPts = ESelectionPointsAll );
protected: // overridables
    virtual void showSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts = ESelectionPointsBoundingRectAll );
    virtual void updateSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts = ESelectionPointsBoundingRectAll );
    virtual void showSelectionPointsOfPolygon( const QPolygonF& i_plg );
    virtual void updateSelectionPointsOfPolygon( const QPolygonF& i_plg );
public: // overridables
    virtual QList<ESelectionPoint> getPossibleLabelAnchorPoints() const;
public: // overridables
    virtual void showNameLabel(ESelectionPoint i_selPt = ESelectionPoint::TopCenter);
    virtual void hideNameLabel();
    virtual bool isNameLabelVisible() const;
    virtual ESelectionPoint nameLabelAnchorPoint() const;
    virtual void showNameLabelAnchorLine();
    virtual void hideNameLabelAnchorLine();
    virtual bool isNameLabelAnchorLineVisible() const;
public: // overridables
    virtual int showLabel(const QString& i_strLabel, ESelectionPoint i_selPt = ESelectionPoint::TopCenter);
    virtual void hideLabel(int i_idxLabel);
    virtual bool isLabelVisible(int i_idxLabel) const;
    virtual ESelectionPoint labelAnchorPoint(int i_idxLabel) const;
    virtual void showLabelAnchorLine(int i_idxLabel);
    virtual void hideLabelAnchorLine(int i_idxLabel);
    virtual bool isLabelAnchorLineVisible(int i_idxLabel) const;
public: // overridables (KeyLabel = "Position<SelPt>")
    virtual void showPositionLabel(ESelectionPoint i_selPt = ESelectionPoint::TopLeft);
    virtual void hidePositionLabel(ESelectionPoint i_selPt = ESelectionPoint::TopLeft);
    virtual bool isPositionLabelVisible(ESelectionPoint i_selPt = ESelectionPoint::TopLeft) const;
    virtual void showPositionLabelAnchorLine(ESelectionPoint i_selPt = ESelectionPoint::TopLeft);
    virtual void hidePositionLabelAnchorLine(ESelectionPoint i_selPt = ESelectionPoint::TopLeft);
    virtual bool isPositionLabelAnchorLineVisible(ESelectionPoint i_selPt = ESelectionPoint::TopLeft) const;
protected: // overridables
    virtual void updateLabelDistance(CGraphObjLabel* i_pGraphObjLabel);
//public: // overridables
//    virtual void showLabels(); // also creates the labels if not yet created
//    virtual void hideLabels(); // not just hides but also destroys the labels
//public: // instance methods
//    QStringList getLabelsKeys() const;
//    CGraphObjLabel* getLabel( const QString& i_strKey ) const;
//    QHash<QString, CGraphObjLabel*> getLabels() { return m_arpLabels; }
//    void addLabels( QHash<QString, CGraphObjLabel*> i_arpLabels );
public: // overridables
    virtual void onParentItemCoorsHasChanged( CGraphObj* /*i_pGraphObjParent*/ );
    virtual void onSelectionPointDestroying( CGraphObjSelectionPoint* i_pSelectionPoint );
    virtual void onLabelAboutToBeDestroyed( CGraphObjLabel* i_pLabel );
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
    virtual void updateLabelPositionsAndContents();
protected: // overridables
    virtual void updateLabelPositions(QList<CGraphObjLabel*>& i_arpLabels);
    virtual void updateLabelPositions(QHash<QString, CGraphObjLabel*>& i_arpLabels);
    virtual void updatePositionLabelsContent();
protected: // auxiliary instance methods
    void showLabel(QHash<QString, CGraphObjLabel*>& i_arpLabels, const QString& i_strKey, const QString& i_strText, ESelectionPoint i_selPt);
    void hideLabel(QHash<QString, CGraphObjLabel*>& i_arpLabels, const QString& i_strKey);
    void destroyLabels();
protected: // auxiliary instance methods (method tracing)
    void emit_selectedChanged();
    void emit_geometryChanged();
protected: // overridable auxiliary instance methods (method tracing)
    virtual void traceInternalStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
protected: // instance members
    /*!< Flag to indicate that the destructor has been called. */
    bool m_bDtorInProgress;
    /*!< Pointer to drawing scene the graphical object belongs to. */
    CDrawingScene* m_pDrawingScene;
    /*!< Graphical objects are called via factories which must be registered at the drawing scene.
         Using registered factories allows to also create user defined graphical objects.
         The name of the factories group name is stored here. */
    QString m_strFactoryGroupName;
    /*!< Type of the graphical object. */
    EGraphObjType m_type;
    /*!< Type of the graphical object. */
    QString m_strType;
    //QString m_strDescription;
    /*!< Draw settings like pen and brush used to draw the graphical object.
         Set by ctor or setSettings. Changed also by graphics items methods "setPen", "setBrush", etc..
         Call "updateSettings" before accessing settings to keep this struct up to date with graphics
         item settings. */
    CDrawSettings m_drawSettings;
    /*!< If valid defines the minimum size of the graphical object. */
    CPhysValSize m_physValSizeMinimum;
    /*!< If valid defines the maximum size of the graphical object. */
    CPhysValSize m_physValSizeMaximum;
    /*!< If valid defines the fixed size of the graphical object. */
    CPhysValSize m_physValSizeFixed;
    /*!< Alignments of the graphical object to the parent group.. */
    QList<SGraphObjAlignment> m_arAlignments;
    /*!< Flag indicating whether the graphical object is hit by the mouse cursor. */
    bool m_bIsHit;
    /*!< Current edit mode. The current edit mode defines how the graphical object handels
         incoming events like moving the mouse cursor. */
    CEnumEditMode m_editMode;
    /*!< If the graphical object is currently being resized this member defines how the object
         will be resized. */
    CEnumEditResizeMode m_editResizeMode;
    /*!< Defines the Z-Value which again defines the drawing order within the list
         of graphics item of the drawing scene. */
    double m_fZValue;
    /*!< Flag indicating whether the bounding rectangle is drawn and visible. */
    bool m_bBoundRectVisible;
    /*!< Currently selected selection point of the items polygon. */
    int m_idxSelPtSelectedPolygon;
    /*!< List of selections points. Selection points are used to resize the graphical object
         using the mouse. The number and type of selection poionts depend on the type of the
         graphical object. A line only has two selection points at the start and end  point.
         A rectangle has at least four selection points - one at each corner. */
    QList<CGraphObjSelectionPoint*> m_arpSelPtsPolygon;
    /*!< Currently selected selection point at the bounding rectangle. */
    CEnumSelectionPoint m_selPtSelectedBoundingRect;
    /*!< List of selection points at the bounding rectangle. */
    QVector<CGraphObjSelectionPoint*> m_arpSelPtsBoundingRect;
    /*!< The name label is created if the name got to be shown in the drawing view. */
    CGraphObjLabel* m_pNameLabel;
    /*!< List of additional labels which may be assigned to the graphical object.
         Please note that the key assigned to the label is the index of the label in the
         list of labels. */
    QList<CGraphObjLabel*> m_arpLabels;
    /*!< Hash with position labels which may be indicated. As positions may be indicated for
         each possible selection point the key into the hash is the name of the selection point. */
    QHash<QString, CGraphObjLabel*> m_arpPosLabels;
    /*!< Hash with dimension line labels which may be indicated. As dimension lines are drawn between
         selection points the key into the hash is the name of one of the selection points. */
    QHash<QString, CGraphObjLabel*> m_arpDimLineLabels;
    /*!< The tool tip contains various interesting information about the graphical object like the name,
         the position and the dimension. But also other information which depends on the type of the object. */
    QString m_strToolTip;
    /*!< Current edit info. */
    QString m_strEditInfo;
protected: // !!! OBSOLETE !!! instance members
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    /*!< Flag to indicate that the coordinates need to be updated. */
    bool m_bCoorsDirty;
    /*!< Current item coordinates and transform values:
         In item's coordinate system (during mouse resize events topLeft may not be at 0.0/0.0). */
    QRectF m_rctCurr;
    /*!< Concerning range see "Draw::getAngleRad" (Q1: 0..90°, Q2: 90..180°, Q3: -90°..-180°, Q4: 0..-90°) */
    double m_fRotAngleCurr_deg;
    /*!< In item's coordinate system. */
    QPointF m_ptRotOriginCurr;
    /*!< Original item coordinates and transform values:
        - after initially creating the item
        - changing position, size and rotation angle of objects with no parents
        - on adding items to groups
        - explicitly calling method "acceptCurrentAsOriginalCoors" */
    bool m_bHasValidOrigCoors;
    /*!< In parent's coordinate system. */
    QPointF m_ptPosOrig;
    QSizeF m_sizOrig;
    /*!< Concerning range see "Draw::getAngleRad" (Q1: 0..90°, Q2: 90..180°, Q3: -90°..-180°, Q4: 0..-90°) */
    double m_fRotAngleOrig_deg;
    /*!< In item's coordinate system. */
    QPointF m_ptRotOriginOrig;
#endif
protected: // instance members
    /*!< Coordinates stored on mouse press events:
         In scene's coordinate system (for moving by my mouse move events). */
    QPointF m_ptScenePosOnMousePressEvent;
    /*!< In scene's coordinate system (for moving by my mouse move events). */
    QPointF m_ptMouseEvScenePosOnMousePressEvent;
    /*!< In item's coordinate system (for resizing by mouse move events). */
    QRectF m_rctOnMousePressEvent;
    /*!< In scene's coordinate system (for rotation by mouse move events). */
    QPointF m_ptRotOriginOnMousePressEvent;
    /*!< Simulation Functions. */
    QList<SGraphObjMouseEventFct> m_arMousePressEventFunctions;
    QList<SGraphObjMouseEventFct> m_arMouseReleaseEventFunctions;
    QList<SGraphObjMouseEventFct> m_arMouseDoubleClickEventFunctions;
    QList<SGraphObjMouseEventFct> m_arMouseMoveEventFunctions;
    QList<SGraphObjKeyEventFct> m_arKeyPressEventFunctions;
    QList<SGraphObjKeyEventFct> m_arKeyReleaseEventFunctions;
    /*!< Method Tracing (trace admin objects have to be created in ctor of "final" class)
         by calling "createTraceAdminObjs". */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjCtorsAndDtor;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjItemChange;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjBoundingRect;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjIsHit;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjPaint;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjSceneEvent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjSceneEventFilter;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjHoverEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjKeyEvents;

}; // class CGraphObj

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObj_h
