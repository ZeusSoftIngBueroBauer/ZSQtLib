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
class CDrawingSize;
class CGraphObj;
class CGraphObjLabel;
class CGraphObjSelectionPoint;


//==============================================================================
/*!
*/
struct ZSDRAWDLL_API SGraphObjSelectionPoint
//==============================================================================
{
public: // struct methods
    static SGraphObjSelectionPoint fromString(const QString& i_str, bool* i_pbOk = nullptr);
public: // ctors
    SGraphObjSelectionPoint();
    SGraphObjSelectionPoint(ESelectionPoint i_selPt);
    SGraphObjSelectionPoint(int idxPt);
    SGraphObjSelectionPoint(const SGraphObjSelectionPoint& i_other);
public: // operators
    SGraphObjSelectionPoint& operator = (const SGraphObjSelectionPoint& i_other);
    SGraphObjSelectionPoint& operator = (ESelectionPoint i_selPt);
    SGraphObjSelectionPoint& operator = (int i_idxPt);
    bool operator == (const SGraphObjSelectionPoint& i_other) const;
    bool operator != (const SGraphObjSelectionPoint& i_other) const;
public: // struct methods
    QString name() const;
    QString toString() const;
public: // struct members
    /*!< Type of the selection point. Selection points are differentiated into
         selection points on the bounding rectangle around the graphical object
         or into polygon shape points. */
    ESelectionPointType m_selPtType;
    /*!< For selection points on the bounding rectangle specifies the position
         on (or within) the bounding rectangle.
         For polygon shape points the enum is set to PolygonPoint. */
    ESelectionPoint m_selPt;
    /*!< For selection points on a polygon the index of the polygon point
         is stored the selection point is assigned to. */
    int m_idxPt;

}; // struct SGraphObjSelectionPoint


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

    Class hierarchy
    ===============

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

    Pixel and world coordinates
    ===========================

    In a 1:1 "wysiwyg" scenario with a screen resolution of 4 px/mm

    - a line with 2.0 mm length would have 8 pixels on the screen and, vice versa,
    - a line with 8 pixels on the screen would measure 2.0 mm in length.

    But that's only true for vertical or horizontal lines.

    The following figure shows a line

    - starting at [0.0/0.0] and ending at [2.0/0.0] mm in metric system and
    - starting at [0/0] and ending at [7/0] px in pixel system.

           0.0             1.0             2.0 mm
            | 0   1   2   3 | 4   5   6   7 |  px
          - +---+---+---+---+---+---+---+---+
          0 | X | X | X | X | X | X | X | X |
          - +---+---+---+---+---+---+---+---+

    If the line would be rotated by 45 degrees keeping the length of the line
    at 2.0 mm, the end points can not be exactly shown on the screen but have
    to be rounded to a whole number.

    The width and the height of the lines bounding rectangle would be
    - sqrt(2.0 mm²) = 1.41421... mm = 5,65685... px = 6 pixels
    and the resulting length of the line on the screen becomes longer than 2.0 mm.

           0.0     0.5     1.0     1.5     2.0 mm
            | 0   1   2   3 | 4   5   6   7 |  px
      0.0 - +---+---+---+---+---+---+---+---+
          0 | X |   |   |   |   |   |   |   |
          - +---+---+---+---+---+---+---+---+
          1 |   | X |   |   |   |   |   |   |
      0.5 - +---+---+---+---+---+---+---+---+
          2 |   |   | X |   |   |   |   |   |
          - +---+---+---+---+---+---+---+---+
          3 |   |   |   | X |   |   |   |   |
      1.0 - +---+---+---+---+---+---+---+---+
          4 |   |   |   |   | X |   |   |   |
          - +---+---+---+---+---+---+---+---+
          5 |   |   |   |   |   | X |   |   |
      1.5 - +---+---+---+---+---+---+---+---+
          6 |   |   |   |   |   |   | X |   |
          - +---+---+---+---+---+---+---+---+
          7 |   |   |   |   |   |   |   |   |
      2.0 - +---+---+---+---+---+---+---+---+
      mm  px

    This means, that the coordinates of the graphical objects must be saved and kept
    with floating point format. As each transformation 

    Since every arithmetic operation also brings in some inaccuracy the original values
    will be stored in floating point format and the transformations are applied to the
    original values.

    !Additional notes!

    - When calculation Y coordinates the metric coordinate system is from bottom to top
      whereas the pixel coordinate system is from top to bottom. As shown in the example
      above to get the real Y coordinate the resulting Y coordinate from the unit conversion
      routine must be calculated as follows:

      Ypx = ImageHeight/px - P1y/px

    Selection Points
    ================

    Selection points are used to change the shape of the objects.
    If located at the edge or center points of the outer lines of the bounding rectangle
    they are used to resize the objects.
    For polygons they are also located on the polygon shape points and will be used
    to move this single shape point.
    Selection points will be dynamically created by the object if the object is selected.
    When moving the parent the selection points must also be moved to keep their position
    on the bounding rectangle or on the polygon shape points.
    When moving a selection point the parent got to be informed that the selection point
    has been moved to adapt its shape.
    This information path could be implemented in two ways:
    - the parent could install a scene filter and listen to mouse events
    - the parent can connect to the geometryChanged signal of the selection point.
    Currently the first way is implemented. But the second way is preferrable as this
    is a common way also used by other scenarios like moving labels or resizing groups.

    Labels
    ======

    Several labels may be assigned to a graphical object:

    - Text labels
    - Position labels
    - Dimension labels

    Text Labels
    -----------

    There are two different text label types:

    - Predefined labels are created together with the graphical object in their constructors.
      The number and name of predefined labels depend on the graphical object type. For example
      the line object adds the predefined labels "P1" and "P2". Predefined labels cannot be removed.
      Only their visibility can be changed.
      A unique name is assigned to each predefined label which is stored in the list of predefined
      label names. The names of the predefined labels cannot be changed during runtime.
      But the text indicated by the predefined labels can be changed during runtime.
      The "Name" label is a special case and is used to indicate the name of the graphical object.
    - Any number of user defined labels can be added to the graphical object. The name of those
      labels can be changed during runtime. But their names must be unique for all labels as the
      name is used to uniquely identify the label for each graphical object they are linked to.

    A visible label is added to the graphics scene. A hidden label is removed from the grahics scene.
    The text the labels are indicating can be changed during runtime. Only for "Name" labels the
    text always indicates the name of the object.
    Labels are anchored to a selection point of the graphical objects they belong to.
    An "anchor line" between the label and the graphical object of the label may also be shown.

    Labels can be moved. After initially creating or moving a label the labels stores the distance
    to the anchor point of the graphical object. When moving the graphical object the labels will
    also be moved keeping the stored distance.

    Example for CGraphObjLine:

    - Label "Name" with text "Horizontal Line" anchored to the center point.
    - Label "P1" with text "Start" anchored to P1.
    - Label "P2" with text "Stop" anchored to P2.

      Texts:           "Start"     "Hello World"    "Stop"
                          |             |              |
                          +-------------+--------------+
      Selection Points:  P1           Center           P2

    Position and Dimension Labels
    -----------------------------

    It is also possible to show the geometry information for the graphical objects
    within the drawing view.

*/
class ZSDRAWDLL_API CGraphObj : public QObject, public ZS::System::CIdxTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    static const QString c_strLabelName;
public: // type definitions and constants
    static const QString c_strGeometryLabelNameP1;
    static const QString c_strGeometryLabelNameP2;
    static const QString c_strGeometryLabelNameCenter;
    static const QString c_strGeometryLabelNameWidth;
    static const QString c_strGeometryLabelNameHeight;
    static const QString c_strGeometryLabelNameLength;
    static const QString c_strGeometryLabelNameAngle;
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObj"; }
protected: // ctor
    CGraphObj(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strFactoryGroupName,
        EGraphObjType i_type,
        const QString& i_strType,
        const QString& i_strObjName,
        ZS::System::CIdxTreeEntry::EEntryType i_idxTreeEntryType = ZS::System::CIdxTreeEntry::EEntryType::Branch);
public: // dtor
    virtual ~CGraphObj();
signals:
    /*!< This signal is emitted if the object is going to be destroyed.
         Only the top most class in the hierachy of class inheritance should emit the signal.
         For this the signal may not be emitted "directly" by the derived classes but the class
         must call "emit_aboutToBeDestroyed" in their destructors. */
    void aboutToBeDestroyed(CGraphObj* i_pGraphObj);
    /*!< This signal is emitted if the selected state of the object has been changed. */
    void selectedChanged(CGraphObj* i_pGraphObj);
    /*!< This signal is emitted if the geometry of the object has been changed. */
    void geometryChanged(CGraphObj* i_pGraphObj);
    /*!< This signal is emitted if the physical unit of the drawing scene's size has been changed
         and the geometry values of the graphical object are converted into a new unit and
         viewers need to update the value strings correspondingly. */
    void geometryValuesUnitChanged(CGraphObj* i_pGraphObj);
    /*!< This signal is emitted if the Z-value (stacking order) of the object has been changed. */
    void zValueChanged(CGraphObj* i_pGraphObj);
    /*!< This signal is emitted if the drawing settings (pen style, etc.) of the object has been changed. */
    void drawSettingsChanged(CGraphObj* i_pGraphObj);
    /*!< This signal is emitted if a new text label has been added. */
    void labelAdded(CGraphObj* i_pGraphObj, const QString& i_strName);
    /*!< This signal is emitted if a text label has been removed. */
    void labelRemoved(CGraphObj* i_pGraphObj, const QString& i_strName);
    /*!< This signal is emitted if a text label has been renamed. */
    void labelRenamed(CGraphObj* i_pGraphObj, const QString& i_strName, const QString& i_strNameNew);
    /*!< This signal is emitted if a text label has been changed. */
    void labelChanged(CGraphObj* i_pGraphObj, const QString& i_strName);
    /*!< This signal is emitted if a geometry label has been changed. */
    void geometryLabelChanged(CGraphObj* i_pGraphObj, const QString& i_strName);
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
    CDrawingScene* drawingScene();
    CGraphObj* parentGraphObj();
public: // overridables
    virtual void setParentGraphObj(CGraphObj* i_pGraphObjParent);
    virtual void rename( const QString& i_strNameNew );
protected: // overridables of base class CIdxTreeEntry
    virtual void setName( const QString& i_strName ) override;
    virtual void setKeyInTree( const QString& i_strKey ) override;
public: // instance methods
    QString getFactoryGroupName() const;
    CEnumEditMode getEditMode() const;
    CEnumEditResizeMode getEditResizeMode() const;
public: // overridables (for subsystem test)
    virtual QString getScenePolygonShapePointsString() const;
public: // instance methods
    int getSelectedPolygonShapePointIndex() const;
    CEnumSelectionPoint getSelectedBoundingRectPoint() const;
    QString getToolTip() const;
    QString getEditInfo() const;
public: // overridables
    virtual void onCreateAndExecDlgFormatGraphObjs();
public: // overridables
    virtual void setDrawSettings(const CDrawSettings& i_drawSettings);
    virtual CDrawSettings getDrawSettings() const;
    virtual bool updateDrawSettings();
protected: // overridables
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld);
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setPenColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getPenColor() const;
    virtual void setPenWidth( int i_iLineWidth, bool i_bImmediatelyApplySetting = true );
    virtual int getPenWidth() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineStyle( const CEnumLineStyle& i_lineStyle, bool i_bImmediatelyApplySetting = true );
    virtual CEnumLineStyle getLineStyle() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setFillColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getFillColor() const;
    virtual void setFillStyle( const CEnumFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting = true );
    virtual CEnumFillStyle getFillStyle() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineRecordType( const CEnumLineRecordType& i_lineRecordType, bool i_bImmediatelyApplySetting = true );
    virtual CEnumLineRecordType getLineRecordType() const;
    virtual void setLineExtent( int i_iLineExtent_px, bool i_bImmediatelyApplySetting = true );
    virtual int getLineExtent() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineEndStyle( const CEnumLinePoint& i_linePoint, const CEnumLineEndStyle& i_endStyle, bool i_bImmediatelyApplySetting = true );
    virtual CEnumLineEndStyle getLineEndStyle( const CEnumLinePoint& i_linePoint ) const;
    virtual void setArrowHeadBaseLineType( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadBaseLineType& i_baseLineType, bool i_bImmediatelyApplySetting = true );
    virtual CEnumArrowHeadBaseLineType getArrowHeadBaseLineType( const CEnumLinePoint& i_linePoint ) const;
    virtual void setArrowHeadFillStyle( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting = true );
    virtual CEnumArrowHeadFillStyle getArrowHeadFillStyle( const CEnumLinePoint& i_linePoint ) const;
    virtual void setArrowHeadWidth( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadWidth& i_width, bool i_bImmediatelyApplySetting = true );
    virtual CEnumArrowHeadWidth getArrowHeadWidth( const CEnumLinePoint& i_linePoint ) const;
    virtual void setArrowHeadLength( const CEnumLinePoint& i_linePoint, const CEnumArrowHeadLength& i_length, bool i_bImmediatelyApplySetting = true );
    virtual CEnumArrowHeadLength getArrowHeadLength( const CEnumLinePoint& i_linePoint ) const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setTextColor( const QColor& i_clr, bool i_bImmediatelyApplySetting = true );
    virtual QColor getTextColor() const;
    virtual void setTextFont( const QFont& i_fnt, bool i_bImmediatelyApplySetting = true );
    virtual QFont getTextFont() const;
    virtual void setTextSize( ETextSize i_size, bool i_bImmediatelyApplySetting = true );
    virtual ETextSize getTextSize() const;
    virtual void setTextStyle( const CEnumTextStyle& i_style, bool i_bImmediatelyApplySetting = true );
    virtual CEnumTextStyle getTextStyle() const;
    virtual void setTextEffect( const CEnumTextEffect& i_effect, bool i_bImmediatelyApplySetting = true );
    virtual CEnumTextEffect getTextEffect() const;
public: // overridables
    virtual void setMinimumWidth( const ZS::PhysVal::CPhysVal& i_physValWidth );
    virtual bool hasMinimumWidth() const;
    virtual ZS::PhysVal::CPhysVal getMinimumWidth(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMinimumHeight( const ZS::PhysVal::CPhysVal& i_physValHeight );
    virtual bool hasMinimumHeight() const;
    virtual ZS::PhysVal::CPhysVal getMinimumHeight(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMinimumSize( const CPhysValSize& i_physValSize );
    virtual bool hasMinimumSize() const;
    virtual CPhysValSize getMinimumSize(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMaximumWidth( const ZS::PhysVal::CPhysVal& i_physValWidth );
    virtual bool hasMaximumWidth() const;
    virtual ZS::PhysVal::CPhysVal getMaximumWidth(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMaximumHeight( const ZS::PhysVal::CPhysVal& i_physValHeight );
    virtual bool hasMaximumHeight() const;
    virtual ZS::PhysVal::CPhysVal getMaximumHeight(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMaximumSize( const CPhysValSize& i_physValSize );
    virtual bool hasMaximumSize() const;
    virtual CPhysValSize getMaximumSize(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setFixedWidth( const ZS::PhysVal::CPhysVal& i_physValWidth );
    virtual bool hasFixedWidth() const;
    virtual ZS::PhysVal::CPhysVal getFixedWidth(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setFixedHeight( const ZS::PhysVal::CPhysVal& i_physValHeight );
    virtual bool hasFixedHeight() const;
    virtual ZS::PhysVal::CPhysVal getFixedHeight(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setFixedSize( const CPhysValSize& i_physValSize );
    virtual bool hasFixedSize() const;
    virtual CPhysValSize getFixedSize(const ZS::PhysVal::CUnit& i_unit) const;
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
    virtual CPhysValPoint getPos( const ZS::PhysVal::CUnit& i_unit, ECoordinatesVersion i_version = ECoordinatesVersion::Transformed ) const;
    virtual ZS::PhysVal::CPhysVal getWidth( const ZS::PhysVal::CUnit& i_unit, ECoordinatesVersion i_version = ECoordinatesVersion::Transformed ) const;
    virtual ZS::PhysVal::CPhysVal getHeight( const ZS::PhysVal::CUnit& i_unit, ECoordinatesVersion i_version = ECoordinatesVersion::Transformed ) const;
    virtual CPhysValSize getSize( const ZS::PhysVal::CUnit& i_unit, ECoordinatesVersion i_version = ECoordinatesVersion::Transformed ) const;
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
    virtual CPhysValPoint getSelectionPointCoors( const SGraphObjSelectionPoint& i_selPt ) const;
protected: // must overridables
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) = 0;
protected: // overridables
    virtual void hideSelectionPoints( ESelectionPoints i_selPts = ESelectionPointsAll );
    virtual void bringSelectionPointsToFront( ESelectionPoints i_selPts = ESelectionPointsAll );
protected: // overridables
    virtual void showSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts = ESelectionPointsBoundingRectAll );
    virtual void showSelectionPointsOfPolygon( const QPolygonF& i_plg );
public: // overridables (text labels)
    QString findUniqueLabelName(const QString& i_strPrefix = "") const;
    QStringList getLabelNames() const;
    virtual QStringList getPredefinedLabelNames() const;
    virtual bool isPredefinedLabelName(const QString& i_strName) const;
    const CGraphObjLabel* getLabel(const QString& i_strName) const;
    virtual QList<SGraphObjSelectionPoint> getPossibleLabelAnchorPoints(const QString& i_strName) const;
    virtual bool isLabelAdded(const QString& i_strName) const;
    virtual bool addLabel(const QString& i_strName, const QString& i_strText = "", const SGraphObjSelectionPoint& i_selPt = ESelectionPoint::Center);
    virtual bool removeLabel(const QString& i_strName);
    virtual bool renameLabel(const QString& i_strName, const QString& i_strNameNew);
    virtual void setLabelText(const QString& i_strName, const QString& i_strText);
    virtual QString labelText(const QString& i_strName) const;
    virtual void setLabelAnchorPoint(const QString& i_strName, const SGraphObjSelectionPoint& i_selPt);
    virtual SGraphObjSelectionPoint labelAnchorPoint(const QString& i_strName) const;
    virtual void showLabel(const QString& i_strName);
    virtual void hideLabel(const QString& i_strName);
    virtual bool isLabelVisible(const QString& i_strName) const;
    virtual void setLabelDistanceToLinkedSelPt(const QString& i_strName, const QSizeF& i_size);
    virtual QSizeF labelDistanceToLinkedSelPt(const QString& i_strName) const;
    virtual void showLabelAnchorLine(const QString& i_strName);
    virtual void hideLabelAnchorLine(const QString& i_strName);
    virtual bool isLabelAnchorLineVisible(const QString& i_strName) const;
public: // overridables (geometry labels)
    virtual QStringList getGeometryLabelNames() const;
    virtual void showGeometryLabel(const QString& i_strName);
    virtual void hideGeometryLabel(const QString& i_strName);
    virtual bool isGeometryLabelVisible(const QString& i_strName) const;
    virtual void setGeometryLabelDistance(const QString& i_strName, const QSizeF& i_size);
    virtual QSizeF geometryLabelDistance(const QString& i_strName) const;
    virtual void showGeometryLabelAnchorLine(const QString& i_strName);
    virtual void hideGeometryLabelAnchorLine(const QString& i_strName);
    virtual bool isGeometryLabelAnchorLineVisible(const QString& i_strName) const;
protected: // overridables (geometry labels)
    virtual bool addGeometryLabel(const QString& i_strName, EGraphObjType i_labelType, const SGraphObjSelectionPoint& i_selPt = ESelectionPoint::Center);
protected slots: // overridables
    virtual void onDrawingSizeChanged(const CDrawingSize& i_drawingSize);
public slots: // overridables
    virtual void onGraphObjParentGeometryChanged(CGraphObj* i_pGraphObjParent);
    virtual void onGraphObjParentZValueChanged(CGraphObj* i_pGraphObjParent);
protected slots: // overridables
    virtual void onSelectionPointAboutToBeDestroyed(CGraphObj* i_pSelectionPoint);
    virtual void onLabelAboutToBeDestroyed(CGraphObj* i_pLabel);
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
    //virtual void updateLabelPositionsAndContents();
protected: // overridables
    //virtual void updateLabelPositions(QList<CGraphObjLabel*>& i_arpLabels);
    //virtual void updateLabelPositions(QHash<QString, CGraphObjLabel*>& i_arpLabels);
    //virtual void updatePositionLabelsContent();
protected: // auxiliary instance methods
    //void showLabel(QHash<QString, CGraphObjLabel*>& i_arpLabels, const QString& i_strKey, const QString& i_strText, ESelectionPoint i_selPt);
    //void hideLabel(QHash<QString, CGraphObjLabel*>& i_arpLabels, const QString& i_strKey);
    //void destroyLabels();
protected: // auxiliary instance methods
    void emit_aboutToBeDestroyed();
protected: // auxiliary instance methods (method tracing)
    void emit_selectedChanged();
    void emit_geometryChanged();
    void emit_geometryValuesUnitChanged();
    void emit_zValueChanged();
    void emit_drawSettingsChanged();
    void emit_labelAdded(const QString& i_strName);
    void emit_labelRemoved(const QString& i_strName);
    void emit_labelRenamed(const QString& i_strName, const QString& i_strNameNew);
    void emit_labelChanged(const QString& i_strName);
    void emit_geometryLabelChanged(const QString& i_strName);
protected: // overridable auxiliary instance methods (method tracing)
    virtual void QGraphicsItem_setPos(const QPointF& i_pos);
    virtual void traceInternalStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
protected: // instance members
    /*!< Flag to indicate that the destructor has been called. */
    bool m_bDtorInProgress;
    /*!< Flag to indicate that the signal aboutToDestroyed has been emitted.
         This flag should ensure that the signal is emitted just once by
         the top most class in the hierachy of class inheritance. */
    bool m_bAboutToBeDestroyedEmitted;
    /*!< When changing the drawing size in metric unit dimension
         (e.g. on changing the Y Scale Orientation) the scene coordinates
         must be newly calculated even if the original values stored in
         metric units have not been changed. On changing the drawing size
         the drawing scene will emit "drawingSizeChanged" and the slot method
         "onDrawingSizeChanged" of the graphical object must set this flag to true. */
    bool m_bForceConversionToSceneCoors;
    /*!< Pointer to drawing scene the graphical object belongs to.
         Is set if the graphical object is added to the scene. */
    CDrawingScene* m_pDrawingScene;
    /*!< Graphical objects are called via factories which must be registered at the drawing scene.
         Using registered factories allows to also create user defined graphical objects.
         The name of the factories group name is stored here. */
    QString m_strFactoryGroupName;
    /*!< Type of the graphical object. */
    EGraphObjType m_type;
    /*!< Type of the graphical object. */
    QString m_strType;
    /*!< Graphical parent object.
         nullptr, if the object is neither a selection point, a label or is not
         added as a child to a group.
         For selection points this is the object creating the selection points and
         to which the selection points are linked to.
         For labels this is also the object creating the labels and to which the
         labels are linked to.
         If the object is added as a child to a group the parent is the group object. */
    CGraphObj* m_pGraphObjParent;
    /*!< Draw settings like pen and brush used to draw the graphical object.
         Set by ctor or setSettings. Changed also by graphics items methods "setPen", "setBrush", etc..
         Call "updateSettings" before accessing settings to keep this struct up to date with graphics
         item settings. */
    CDrawSettings m_drawSettings;
    /*!< Several draw setting attributes may be modified one after another.
         To avoid that for each single change a repaint follows and that the drawSettingsChanged
         signal is emitted just once after all attributes have been set, it is possible first to
         modify each attribute one after another and afterwards force a repaint and emit the
         changes signal just once after all attributes have been set. For this the flag
         "i_bImmediatelyApplySetting" has to be set to false when setting a new draw attribute.
         If this flag is false the changed property is temporarily stored in "m_drawSettingsTmp".
         After all settings have been changed the method "updateDrawSettings" takes over the
         settings from the temporary buffer.
         For the first change to be cached the temporary buffer will be allocated.
         After updating the changes the temporary buffer will be deleted. */
    CDrawSettings* m_pDrawSettingsTmp;
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
    /*!< List with predefined (reserved) label names.
         For all object types "Name" is reserved to indicate the name of the object.
         The predefined label names (including "Name") got to be added in the constructors
         of the derived classes. E.g. the "Line" adds the predefined label names "Name",
         "P1" and "P2" in its constructor. If user defined labels got to be added the reserved
         names may not be used. */
    QStringList m_strlstPredefinedLabelNames;
    /*!< Labels which may be assigned to and indicated by the graphical object.
         For each label a unique name has to be assigned.
         Some names are reserved for internal use. E.g. "Name" is used to indicate
         the name of the object. The value of the "Name" label is not editable but is
         used to indicate the name of the object.
         Derived graphical object classes may further on reserve names. E.g. the Line object
         uses "P1" and "P2" to address the line end points. The names for "P1" and "P2" is
         editable but are defaulting to "P1" and "P2".
         In addition to those predefined labels additional labels may be added by defining a
         unique name and assigning a text. Both the name and the text are stored in the Label object. */
    QHash<QString, CGraphObjLabel*> m_hshpLabels;
    /*!< List with the value names. Got to be initialised in the constructor of derived classes
         in addition to the hash with geometry values. Keeping the values names also a string list
         should verify that the returned value names are always in the same order as the order
         in a hash is arbitrary. */
    QStringList m_strlstGeometryLabelNames;
    /*!< Hash with geometry values which may be indicated. The number of value label depend on
         the object type. E.g. for the Line object the positions "P1", "P2" and "Center" as well as
         the "Size" (width and height), "Length" and "Angle" may be shown. */
    QHash<QString, CGraphObjLabel*> m_hshpGeometryLabels;
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
    /*!< Counts how ofter the "itemChange" method has been blocked from updating
         the original object coordinates. A value greater than 0 for the counter means
         that the itemChange method must not update the original coordinates as they
         have been expliticely set by a method call whereupon the objects, transformed
         item coordinates in pixels should be updated by itemChange. */
    int m_iItemChangeUpdateOriginalCoorsBlockedCounter;
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
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseClickEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjKeyEvents;

}; // class CGraphObj

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObj_h
