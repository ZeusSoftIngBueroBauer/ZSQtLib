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
#include "ZSDraw/Common/ZSDrawPhysValPolygon.h"
#include "ZSDraw/Common/ZSDrawSettings.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEventFct.h"

#include <QtGui/qcursor.h>
#include <QtGui/qpainter.h>

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
class CGraphObjGroup;
class CGraphObjLabel;
class CGraphObjSelectionPoint;

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

    - void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll);
      Also creates the selection points if not yet created.

    Pixel and world coordinates
    ===========================

    At item's position is the coordinate of the item's center point in its parent's coordinate system;
    sometimes referred to as parent coordinates. The scene is in this sense regarded as all parent-less
    items' "parent". Top level items' position are in scene coordinates.

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

    Coordinate system(s)
    ====================

    If a graphics item doesn't have a parent item the position of the item is in scene
    coordinates with the origin point at (0/0) (top left corner of the drawing scene).
    If the graphics item is added as a child to the parent the position of the item
    is relative to the parents center point.

    +-Drawing-Scene------------------------------------------------------------+
    |(0/0)                                                                     |
    |                                                                          |
    |                                                                          |
    |                              +--Group0---------x-----------------+       |
    |                              |(-30/-30)        |                 |       |
    |  (10/50)                     |    +-Rect1-+    |                 |       |
    |     +-Rect0-+                |    |       |    |(0/0)            |       |
    |     |       |                x - -|       |- - x - - - - - - - - x       |
    |     |       |                |    +-------+    |                 |       |
    |     +-------+                |                 |                 |       |
    |                              |                 |                 |       |
    |                              +-----------------x-----------------+       |
    |                                                                          |
    |                                                                          |
    |                                                                          |
    +--------------------------------------------------------------------------+

    But when providing the position to the user in GUI controls not the relative
    position to the parent's group center point but the position relative to
    the groups top left corner should be indicated. Depending on the unit system
    either in pixels or in metric unit.

    Instead of indicating (-30/-30) for Rect1 in the figure above the user wants
    to see and change the position of the top left corner from Rect1 relative to
    the top left corner of Group0 (instead of (-30/-10) the coordinates (20/20)
    for example.

    Selection Points
    ================

    Selection points are used to change the shape of the objects.

    Selection points will be dynamically created if an object is selected and
    will never belong as childs to groups.

    The position of selection point's is always defined in scene coordinates.

    If located at the edge or center points of the outer lines of the object's
    bounding rectangle, selection points are used to resize the objects.
    For polygons selection points are also located on the polygon shape points
    and will be used to move this single shape point.

    If the geometry or position of an object is changed, the selection points
    must be informed about this geometry and position change to keep their
    positions on the bounding rectangle or on the polygon shape points of the object.
    This also applies if the object's geometry is not changed but just moved on the
    drawing scene as a child of a group. If the object is a child of a group and the
    group's shape is not changed but just moved on the scene, the geometry and the
    relative position of the object within the parent group remains the same.
    To be able to detect the scene position change the graphical object need to track
    their current scene position, emitting a scenePosChanged signal if the scene
    position has been changed, and the parent group must inform their childs if the
    group's scene position has been changed.

    When moving a selection point the object they belong to got to be informed that
    the selection point has been moved to adapt its shape.

    When creating objects on the drawing scene using mouse move events, the mouse events
    will be passed to the selection points and the selection points will be moved.
    The object under construction will receive the geometry changed signals by the
    selection point and will update its shape.

    The mouse event finishing the creation of the object depends on the type of graphical
    object being created. For lines for example, the creation is finished if the mouse release
    event is received by the selection point previously moved. To handle the mouse event, the
    object under construction installs a scene event filter on the selection point.

    In order to handle the mouse event correctly the graphical object need to know that it
    is currently being created. For this the edit mode is used which is set if the object
    has been created on the drawing scene.

    Labels
    ======

    Several labels may be assigned to a graphical object:

    - Text labels
    - Position labels
    - Dimension labels

    The position of labels is always defined in scene coordinates.
    Labels will never belong as childs to groups.

    If the geometry or position of a labels graphical object is changed, the labels must be
    informed about this geometry and position change to keep their relative positions of their
    graphical objects on the drawing scene. This also applies if the graphical object's
    geometry is not changed but just moved on the drawing scene as a child of a group.
    If the graphical object is a child of a group and the group's shape is not changed but just
    moved on the scene, the geometry and the relative position of the object within the parent
    group remains the same. To be able to detect the scene position change the graphical object
    need to track their current scene position, emitting a scenePosChanged signal if the scene
    position has been changed, and the parent group must inform their childs if the group's
    scene position has been changed.

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

    Current and Original Coordinates
    ================================

    Items may be added to groups. If the group is resized or rotated the geometry
    of the childs must also be adjusted coorrespondingly. If a group is resized
    to twice the size also the childrens should become twice the size. Same applies
    when shrinking the group's bounding rectangle.

    If a group is going to be resized, at any time the group's width or height may
    become zero. Once the width or height of the group becomes zero resizing the
    children to fit the new groups size would no longer be possible.
    For this the original size of the child item must be stored and the current
    scale factor to resize the children is calculated using the current and
    the original size.

    To avoid rounding errors and to be able to calculate a scale factor the items
    must keep the original coordinates in addition to the current, transformed
    coordinates of their shape points.

    Depending on the Y-Axis-Scale-Orientation the coordinates are relative either
    to the top left or bottom left corner of the parent item's bounding rectange.
    If the item does not have another item as a  parent (does not belong to a group),
    the coordinates are in scene coordinates.
    As long as the item is not added to a group, the original and current
    coordinates are equal.

    If the item is not a group (and not a label or selection point)
    - and the item is not a child of a group
      - the current (transformed) and original coordinates are equal.
    - If the item is added to or removed from a group as a child
      - the current (transformed) coordinates are set as the original coordinates.
    - If the item already belongs as a child to a group
      - only the current (transformed) coordinates are updated.

    If the item is a group
    - the current (transformed) coordinates are set as the original coordinates
      right after the group has been initially created or right after adding the
      group to or removing the group from another group.
    - At any other time the current (transformed) coordinates are not taken over
      as the original coordinates. When modifying (resizing) the group, the current
      scale factor can be evaluated by the parent group using the current and
      original coordinates and can than be used to apply the geometry changes to
      the group items without rounding errors.
*/
class ZSDRAWDLL_API CGraphObj : public QObject, public ZS::System::CIdxTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    static const QString c_strLabelName;
public: // type definitions and constants
    static const QString c_strGeometryLabelNameP;
    static const QString c_strGeometryLabelNameP1;
    static const QString c_strGeometryLabelNameP2;
    static const QString c_strGeometryLabelNameTopLeft;
    static const QString c_strGeometryLabelNameTopRight;
    static const QString c_strGeometryLabelNameBottomRight;
    static const QString c_strGeometryLabelNameBottomLeft;
    static const QString c_strGeometryLabelNameCenter;
    static const QString c_strGeometryLabelNameDX;
    static const QString c_strGeometryLabelNameDY;
    static const QString c_strGeometryLabelNameLength;
    static const QString c_strGeometryLabelNameWidth;
    static const QString c_strGeometryLabelNameHeight;
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
    /*!< This signal is emitted if the edit mode of the object has been changed. */
    void editModeChanged(CGraphObj* i_pGraphObj, const CEnumEditMode& i_eMode);
    /*!< This signal is emitted if the selected state of the object has been changed. */
    void selectedChanged(CGraphObj* i_pGraphObj, bool i_bIsSelected);
    /*!< This signal is emitted if the position of the object on the drawing scene is changed.
         Used to update the position of selection points and labels if the object's geometry
         is not changed but has been moved on the drawing scene as a child of a group. */
    //void scenePosChanged(CGraphObj* i_pGraphObj);
    /*!< This signal is emitted if the geometry of the object on the scene has been changed.
         If a group item is rotated, resized or moved the group itself and all its children
         change their geometry on the scene. The signal is emitted for the changed group and
         also by its children to inform all connected labels and selection points that their
         parents scene coordinates may have been changed.
        @param [in] i_pGraphObjParent
            Pointer to parent item whose geometry on the scene has been changed.
        @param [in] i_bParentOfParentChanged
            false (default), if the geometry of the parent has been changed directly.
            true if the geometry has been changed because the parent got a new parent. */
    void geometryOnSceneChanged(CGraphObj* i_pGraphObj, bool i_bParentOfParentChanged = false);
    /*!< This signal is emitted if the physical unit of the drawing scene's size has been changed
         and the geometry values of the graphical object are converted into a new unit and
         viewers need to update the value strings correspondingly. */
    //void geometryValuesUnitChanged(CGraphObj* i_pGraphObj);
    /*!< This signal is emitted if the Z-value (stacking order) of the object has been changed. */
    void zValueChanged(CGraphObj* i_pGraphObj, double i_fZValue);
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
    /*!< This signal is emitted if a new geometry label has been added. */
    void geometryLabelAdded(CGraphObj* i_pGraphObj, const QString& i_strName);
    /*!< This signal is emitted if a geometry label has been removed. */
    void geometryLabelRemoved(CGraphObj* i_pGraphObj, const QString& i_strName);
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
    CDrawingScene* drawingScene() const;
    CGraphObjGroup* parentGroup() const;
    void onParentGroupChanged(CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew);
public: // overridables
    //virtual void setParentGraphObj(CGraphObj* i_pGraphObjParent);
    virtual void rename(const QString& i_strNameNew);
protected: // overridables of base class CIdxTreeEntry
    virtual void setName(const QString& i_strName) override;
    virtual void setKeyInTree(const QString& i_strKey) override;
public: // instance methods
    QString getFactoryGroupName() const;
public: // overridables
    virtual void onCreateAndExecDlgFormatGraphObjs();
public: // overridables
    virtual void setDrawSettings(const CDrawSettings& i_drawSettings);
    virtual CDrawSettings getDrawSettings() const;
    virtual bool updateDrawSettings();
protected: // overridables
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld);
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setPenColor(const QColor& i_clr, bool i_bImmediatelyApplySetting = true);
    virtual QColor getPenColor() const;
    virtual void setPenWidth(int i_iLineWidth, bool i_bImmediatelyApplySetting = true);
    virtual int getPenWidth() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineStyle(const CEnumLineStyle& i_lineStyle, bool i_bImmediatelyApplySetting = true);
    virtual CEnumLineStyle getLineStyle() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setFillColor(const QColor& i_clr, bool i_bImmediatelyApplySetting = true);
    virtual QColor getFillColor() const;
    virtual void setFillStyle(const CEnumFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting = true);
    virtual CEnumFillStyle getFillStyle() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineRecordType(const CEnumLineRecordType& i_lineRecordType, bool i_bImmediatelyApplySetting = true);
    virtual CEnumLineRecordType getLineRecordType() const;
    virtual void setLineExtent(int i_iLineExtent_px, bool i_bImmediatelyApplySetting = true);
    virtual int getLineExtent() const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setLineEndStyle(const CEnumLinePoint& i_linePoint, const CEnumLineEndStyle& i_endStyle, bool i_bImmediatelyApplySetting = true);
    virtual CEnumLineEndStyle getLineEndStyle(const CEnumLinePoint& i_linePoint) const;
    virtual void setArrowHeadBaseLineType(const CEnumLinePoint& i_linePoint, const CEnumArrowHeadBaseLineType& i_baseLineType, bool i_bImmediatelyApplySetting = true);
    virtual CEnumArrowHeadBaseLineType getArrowHeadBaseLineType(const CEnumLinePoint& i_linePoint) const;
    virtual void setArrowHeadFillStyle(const CEnumLinePoint& i_linePoint, const CEnumArrowHeadFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting = true);
    virtual CEnumArrowHeadFillStyle getArrowHeadFillStyle( const CEnumLinePoint& i_linePoint) const;
    virtual void setArrowHeadWidth(const CEnumLinePoint& i_linePoint, const CEnumArrowHeadWidth& i_width, bool i_bImmediatelyApplySetting = true);
    virtual CEnumArrowHeadWidth getArrowHeadWidth( const CEnumLinePoint& i_linePoint) const;
    virtual void setArrowHeadLength(const CEnumLinePoint& i_linePoint, const CEnumArrowHeadLength& i_length, bool i_bImmediatelyApplySetting = true);
    virtual CEnumArrowHeadLength getArrowHeadLength(const CEnumLinePoint& i_linePoint) const;
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") to keep the settings synchronized with QGraphicsItem attributes)
    virtual void setTextColor(const QColor& i_clr, bool i_bImmediatelyApplySetting = true);
    virtual QColor getTextColor() const;
    virtual void setTextFont(const QFont& i_fnt, bool i_bImmediatelyApplySetting = true);
    virtual QFont getTextFont() const;
    virtual void setTextSize(ETextSize i_size, bool i_bImmediatelyApplySetting = true);
    virtual ETextSize getTextSize() const;
    virtual void setTextStyle(const CEnumTextStyle& i_style, bool i_bImmediatelyApplySetting = true);
    virtual CEnumTextStyle getTextStyle() const;
    virtual void setTextEffect(const CEnumTextEffect& i_effect, bool i_bImmediatelyApplySetting = true);
    virtual CEnumTextEffect getTextEffect() const;
public: // overridables
    virtual void setMinimumWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    virtual bool hasMinimumWidth() const;
    virtual ZS::PhysVal::CPhysVal getMinimumWidth(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMinimumHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    virtual bool hasMinimumHeight() const;
    virtual ZS::PhysVal::CPhysVal getMinimumHeight(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMinimumSize(const CPhysValSize& i_physValSize);
    virtual bool hasMinimumSize() const;
    virtual CPhysValSize getMinimumSize(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMaximumWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    virtual bool hasMaximumWidth() const;
    virtual ZS::PhysVal::CPhysVal getMaximumWidth(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMaximumHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    virtual bool hasMaximumHeight() const;
    virtual ZS::PhysVal::CPhysVal getMaximumHeight(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setMaximumSize(const CPhysValSize& i_physValSize);
    virtual bool hasMaximumSize() const;
    virtual CPhysValSize getMaximumSize(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setFixedWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    virtual bool hasFixedWidth() const;
    virtual ZS::PhysVal::CPhysVal getFixedWidth(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setFixedHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    virtual bool hasFixedHeight() const;
    virtual ZS::PhysVal::CPhysVal getFixedHeight(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setFixedSize( const CPhysValSize& i_physValSize );
    virtual bool hasFixedSize() const;
    virtual CPhysValSize getFixedSize(const ZS::PhysVal::CUnit& i_unit) const;
public: // overridables
    virtual int addAlignment(const SGraphObjAlignment& i_alignment);
    virtual int getAlignmentCount() const;
    virtual SGraphObjAlignment getAlignment(int i_idx) const;
    virtual void setAlignment(int i_idx, const SGraphObjAlignment& i_alignment);
    virtual void removeAlignment(int i_idx);
    virtual void clearAlignments();
public: // overridables
    virtual QPointF mapToTopLeftOfBoundingRect(const QPointF& i_pt) const;
    virtual QLineF mapToTopLeftOfBoundingRect(const QLineF& i_line) const;
    virtual QRectF mapToTopLeftOfBoundingRect(const QRectF& i_rect) const;
    virtual QPointF mapFromTopLeftOfBoundingRect(const QPointF& i_pt) const;
    virtual QLineF mapFromTopLeftOfBoundingRect(const QLineF& i_line) const;
    virtual QRectF mapFromTopLeftOfBoundingRect(const QRectF& i_rect) const;
public: // must overridables
    virtual QRectF getBoundingRect() const;
    virtual QRectF getEffectiveBoundingRectOnScene() const;
    virtual CPhysValRect getPhysValBoundingRect() const;
    virtual CPhysValRect getPhysValBoundingRect(const ZS::PhysVal::CUnit& i_unit) const;
public: // overridables
    virtual void setPosition(const CPhysValPoint& i_physValPos);
    virtual CPhysValPoint position() const;
    virtual CPhysValPoint position(const ZS::PhysVal::CUnit& i_unit) const;
    virtual void setRotationAngle(double i_fAngle_degree);
    virtual void setRotationAngle(const ZS::PhysVal::CPhysVal& i_physValAngle);
    virtual ZS::PhysVal::CPhysVal rotationAngle() const;
    virtual ZS::PhysVal::CPhysVal rotationAngle(const ZS::PhysVal::CUnit& i_unit) const;
    virtual ZS::PhysVal::CPhysVal getEffectiveRotationAngleOnScene() const;
    virtual ZS::PhysVal::CPhysVal getEffectiveRotationAngleOnScene(const ZS::PhysVal::CUnit& i_unit) const;
public: // overridables
    virtual void setEditMode(const CEnumEditMode& i_eMode);
    CEnumEditMode editMode() const;
    //virtual void setEditResizeMode(CEnumEditResizeMode i_eMode);
    //CEnumEditResizeMode editResizeMode() const;
    virtual void setIsHit(bool i_bIsHit);
    bool isHit() const;
    //virtual bool isHit(const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo = nullptr) const;
    virtual void setIsHighlighted(bool i_bIsHighlighted);
    bool isHighlighted() const;
public: // overridables
    virtual QCursor getProposedCursor(const QPointF& i_pt) const;
public: // overridables
    virtual double bringToFront();
    virtual double setStackingOrderValue(double i_fZValue, ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Current);
    double getStackingOrderValue(ZS::System::ERowVersion i_version = ZS::System::ERowVersion::Current) const;
    double resetStackingOrderValueToOriginalValue();
public: // overridables
    CGraphObjSelectionPoint* getSelectionPointHit(const QPointF& i_pt) const;
public: // overridables
    virtual CPhysValPoint getPositionOfSelectionPoint(ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const;
    virtual CPhysValPoint getPositionOfSelectionPoint(ESelectionPointType i_selPtType, ESelectionPoint i_selPt, const ZS::PhysVal::CUnit& i_unit) const;
    virtual CPhysValPoint getPositionOfSelectionPoint(ESelectionPointType i_selPtType, int i_idxPt) const;
    virtual CPhysValPoint getPositionOfSelectionPoint(ESelectionPointType i_selPtType, int i_idxPt, const ZS::PhysVal::CUnit& i_unit) const;
    virtual QPointF getPositionOfSelectionPointInSceneCoors(ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const;
    virtual QPointF getPositionOfSelectionPointInSceneCoors(ESelectionPointType i_selPtType, int i_idxPt) const;
public: // must overridables
    virtual SPolarCoors getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const;
    virtual SPolarCoors getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPointType i_selPtType, int i_idxPt) const;
    virtual QLineF getAnchorLineToSelectionPointFromPolarInSceneCoors(const SPolarCoors& i_polarCoors, ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const;
    virtual QLineF getAnchorLineToSelectionPointFromPolarInSceneCoors(const SPolarCoors& i_polarCoors, ESelectionPointType i_selPtType, int i_idxPt) const;
protected: // must overridables
    virtual void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll) = 0;
protected: // overridables
    virtual void hideSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll);
protected: // auxiliary instance methods
    void connectGeometryOnSceneChangedSlotWithSelectionPoints();
    void disconnectGeometryOnSceneChangedSlotFromSelectionPoints();
protected: // overridables
    virtual void showSelectionPointsOfBoundingRect(const QRectF& i_rct, TSelectionPointTypes i_selPts = c_uSelectionPointsBoundingRectAll);
    virtual void showSelectionPointsOfPolygon(const QPolygonF& i_plg);
public: // overridables (text labels)
    QString findUniqueLabelName(const QString& i_strPrefix = "") const;
    QStringList getLabelNames() const;
    virtual QStringList getPredefinedLabelNames() const;
    virtual bool isPredefinedLabelName(const QString& i_strName) const;
    CGraphObjLabel* getLabel(const QString& i_strName) const;
    SLabelDscr getLabelDescriptor(const QString& i_strName) const;
    virtual QList<SGraphObjSelectionPoint> getPossibleLabelAnchorPoints(const QString& i_strName) const;
    virtual bool labelHasDefaultValues(const QString& i_strName) const;
    virtual bool isLabelAdded(const QString& i_strName) const;
    virtual bool addLabel(const QString& i_strName, const QString& i_strText, ESelectionPointType i_selPtType, ESelectionPoint i_selPt1);
    virtual bool addLabel(const QString& i_strName, const QString& i_strText, ESelectionPointType i_selPtType, int i_idxPt);
    virtual bool removeLabel(const QString& i_strName);
    virtual bool renameLabel(const QString& i_strName, const QString& i_strNameNew);
    virtual void setLabelText(const QString& i_strName, const QString& i_strText);
    virtual QString labelText(const QString& i_strName) const;
    virtual void setLabelAnchorPoint(const QString& i_strName, ESelectionPointType i_selPtType, ESelectionPoint i_selPt);
    virtual void setLabelAnchorPoint(const QString& i_strName, ESelectionPointType i_selPtType, int i_idxPt);
    virtual SGraphObjSelectionPoint labelAnchorPoint(const QString& i_strName) const;
    virtual void showLabel(const QString& i_strName);
    virtual void hideLabel(const QString& i_strName);
    virtual bool isLabelVisible(const QString& i_strName) const;
    virtual void setLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName, const SPolarCoors& i_polarCoors);
    virtual SPolarCoors labelPolarCoorsToLinkedSelectionPoint(const QString& i_strName) const;
    virtual void showLabelAnchorLine(const QString& i_strName);
    virtual void hideLabelAnchorLine(const QString& i_strName);
    virtual bool isLabelAnchorLineVisible(const QString& i_strName) const;
public: // overridables (geometry labels)
    virtual QStringList getGeometryLabelNames() const;
    virtual bool isValidGeometryLabelName(const QString& i_strName) const;
    CGraphObjLabel* getGeometryLabel(const QString& i_strName) const;
    SLabelDscr getGeometryLabelDescriptor(const QString& i_strName) const;
    virtual bool geometryLabelHasDefaultValues(const QString& i_strName) const;
    virtual void showGeometryLabel(const QString& i_strName);
    virtual void hideGeometryLabel(const QString& i_strName);
    virtual bool isGeometryLabelVisible(const QString& i_strName) const;
    virtual void setGeometryLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName, const SPolarCoors& i_polarCoors);
    virtual SPolarCoors geometryLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName) const;
    virtual void showGeometryLabelAnchorLine(const QString& i_strName);
    virtual void hideGeometryLabelAnchorLine(const QString& i_strName);
    virtual bool isGeometryLabelAnchorLineVisible(const QString& i_strName) const;
protected: // overridables (geometry labels)
    virtual bool addGeometryLabel(const QString& i_strName, EGraphObjType i_labelType, ESelectionPoint i_selPt1, ESelectionPoint i_selPt2 = ESelectionPoint::None);
    virtual bool addGeometryLabel(const QString& i_strName, EGraphObjType i_labelType, int i_idxPt1, int i_idxPt2 = -1);
    virtual bool removeGeometryLabel(const QString& i_strName);
public: // instance methods (simulation methods)
    //void addMousePressEventFunction(TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void removeMousePressEventFunction(TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void addMouseReleaseEventFunction(TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void removeMouseReleaseEventFunction(TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void addMouseDoubleClickEventFunction(TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void removeMouseDoubleClickEventFunction(TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void addMouseMoveEventFunction(TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void removeMouseMoveEventFunction(TFctMouseEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void addKeyPressEventFunction(TFctKeyEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void removeKeyPressEventFunction(TFctKeyEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void addKeyReleaseEventFunction(TFctKeyEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
    //void removeKeyReleaseEventFunction(TFctKeyEvent i_pFct, void* i_pvThis = nullptr, void* i_pvData = nullptr);
protected slots: // overridables
    virtual void onDrawingSizeChanged(const CDrawingSize& i_drawingSize);
    virtual void onGraphObjParentGeometryOnSceneChanged(CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged = false);
    virtual void onGraphObjParentZValueChanged(CGraphObj* i_pGraphObjParent);
    virtual void onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint);
    virtual void onSelectionPointAboutToBeDestroyed(CGraphObj* i_pSelectionPoint);
    virtual void onLabelAboutToBeDestroyed(CGraphObj* i_pLabel);
    virtual void onGeometryLabelAboutToBeDestroyed(CGraphObj* i_pLabel);
public: // instance methods
    int blockItemChangeUpdatePhysValCoors(bool i_bBlock);
    int blockGeometryOnSceneChangedSignal(bool i_bBlock);
    int setIgnoreParentGeometryChange(bool i_bSet);
public: // overridables
    virtual void initParentTransform();
    virtual void updateTransformedCoorsOnParentChanged(CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew);
    virtual void updateTransformedCoorsOnParentGeometryChanged();
    virtual void updateTransformedCoorsOnItemPositionChanged();
protected: // auxiliary instance methods
    QRectF getEffectiveBoundingRectOnScene(const CPhysValRect& i_physValRectBounding) const;
protected: // overridables
    //virtual void updateTransform();
    //virtual void updateToolTip();
    //virtual void updateEditInfo();
protected: // auxiliary instance methods (method tracing)
    void emit_aboutToBeDestroyed();
    void emit_editModeChanged(const CEnumEditMode& i_eMode);
    void emit_selectedChanged(bool i_bIsSelected);
    //void emit_scenePosChanged();
    void emit_geometryOnSceneChanged(bool i_bParentOfParentChanged = false);
    //void emit_geometryValuesUnitChanged();
    void emit_zValueChanged(double i_fZValue);
    void emit_drawSettingsChanged();
    void emit_labelAdded(const QString& i_strName);
    void emit_labelRemoved(const QString& i_strName);
    void emit_labelRenamed(const QString& i_strName, const QString& i_strNameNew);
    void emit_labelChanged(const QString& i_strName);
    void emit_geometryLabelAdded(const QString& i_strName);
    void emit_geometryLabelRemoved(const QString& i_strName);
    void emit_geometryLabelChanged(const QString& i_strName);
    CPhysValRect setPhysValRectParentGroupOrig(const CPhysValRect& i_physValRect);
    double setParentGroupScaleX(double i_fScaleX);
    double setParentGroupScaleY(double i_fScaleY);
    virtual void QGraphicsItem_prepareGeometryChange();
protected: // overridable auxiliary instance methods (method tracing)
    virtual QPointF QGraphicsItem_setPos(const QPointF& i_pos);
    virtual double QGraphicsItem_setScale(double i_fFactor);
    virtual double QGraphicsItem_setRotation(double i_fAngle_degree);
    virtual QGraphicsItem* QGraphicsItem_setParentItem(QGraphicsItem* i_pGraphicsItemParent);
public: // overridable auxiliary instance methods (method tracing)
    virtual void tracePositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
    virtual void traceThisPositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
    virtual void traceParentGroupPositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
    virtual void traceGraphicsItemStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
    virtual void traceGraphObjStates(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const;
protected: // class members
    static QColor s_selectionColor;
    static QColor s_highlightColor;
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
         nullptr, if the object is not added as a child to a group.
         If the object is added as a child to a group the parent is the group object. */
    //CGraphObj* m_pGraphObjParent;
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
    /*!< If valid, defines the minimum size of the graphical object. */
    CPhysValSize m_physValSizeMinimum;
    /*!< If valid, defines the maximum size of the graphical object. */
    CPhysValSize m_physValSizeMaximum;
    /*!< If valid, defines the fixed size of the graphical object. */
    CPhysValSize m_physValSizeFixed;
    /*!< Alignments of the graphical object to the parent group.. */
    QList<SGraphObjAlignment> m_arAlignments;
    /*!< Flag indicating whether the graphical object is hit by the mouse cursor
         (between hoverEnterEvent and hoverLeaveEvent). */
    bool m_bIsHit;
    /*!< Flag indicating whether the graphical object is highlighted.
         Objects are highlighted if they are selected via the graph objects index tree.
         A highlighted but not selected object does not show selection points. */
    bool m_bIsHighlighted;
    /*!< Current edit mode. The current edit mode defines how the graphical object for example
         handels mouse events passed to the selection points. */
    CEnumEditMode m_editMode;
    ///*!< If the graphical object is currently being resized this member defines how the object
    //     will be resized. */
    //CEnumEditResizeMode m_editResizeMode;
    /*!< Defines the Z-Value which again defines the drawing order within the list of graphics item
         of the drawing scene.
         Two values are stored. The original version is the ZValue which will be initially used
         and if resetStackingOrderValueToOriginalValue is called. The current version is the ZValue
         which is currently set at the graphics item. The current and original versions are mainly
         used to temporarily modify the ZValue to bring the object in front and back again. */
    QVector<double> m_arfZValues;
    /*!< Rotation angle of this item. */
    ZS::PhysVal::CPhysVal m_physValRotationAngle;
    /*!< Current parent group of the item. Updated by the slot "onParentGroupChanged". Keeping this
         pointer allows to access the previous parent group if the object is reparented to retrieve
         and update (move) transformation parameters from the previous to the new parent group. */
    CGraphObjGroup* m_pGraphObjGroupParent;
    /*!< When adding the item to a group the current group rectangle is taken over as the
         original group rectangle. If the parent group is resized the scale factor is calculated
         using the original group rectangle and current group rectangle. As long as the item
         does not have a parent group, the rectangle is invalid. */
    CPhysValRect m_physValRectParentGroupOrig;
    /*!< Current scale factor for width and height of the parent group.
         Calculated by taking m_physValRectGroupOrig into account. */
    double m_fParentGroupScaleX;
    double m_fParentGroupScaleY;
    /*!< Transformations as applied by modifying (scaling, rotating, shearing) the object directly. */
    //QTransform m_transform;
    ///*!< Current scene position of the object. To keep the relative position of selection points
    //     and labels to their "parent" objects up to date on the scene, the objects need to track
    //     their current scene position. If the objects are childs of groups and the groups are
    //     moved, the geometry of the object within their parent groups is not changed. But their
    //     scene position is changed and this change must be forwarded to their selection points
    //     and labels by emitting the scenePosChanged signal. */
    //QPointF m_ptScenePos;
    /*!< Currently selected selection point of the items polygon. */
    //int m_idxSelPtSelectedPolygon;
    /*!< List of selections points. Selection points are used to resize the graphical object
         using the mouse. The number and type of selection poionts depend on the type of the
         graphical object. A line only has two selection points at the start and end  point.
         A rectangle has at least four selection points - one at each corner. */
    QList<CGraphObjSelectionPoint*> m_arpSelPtsPolygon;
    /*!< Currently selected selection point at the bounding rectangle. */
    //CEnumSelectionPoint m_selPtSelectedBoundingRect;
    /*!< List of selection points at the bounding rectangle. */
    QVector<CGraphObjSelectionPoint*> m_arpSelPtsBoundingRect;
    /*!< List with predefined (reserved) label names.
         For all object types "Name" is reserved to indicate the name of the object.
         The predefined label names (including "Name") may be added in the constructors
         of the derived classes. E.g. the "Line" adds the predefined label names "Name",
         "P1" and "P2" in its constructor. Predefined label names may also be added
         during runtime. E.g. when added or removing points to a polygon. For this label
         names "P<IdxPt>" are predefined label names and may not be used as user defined label names.
         If user defined labels got to be added the reserved names may not be used. */
    QStringList m_strlstPredefinedLabelNames;
    /*!< Hash with descriptors for labels which may be assigned to and indicated by the graphical object.
         For each label a unique name has to be assigned.
         Some names are reserved for internal use. E.g. "Name" is used to indicate
         the name of the object. The value of the "Name" label is not editable but is
         used to indicate the name of the object.
         Derived graphical object classes may further on reserve names. E.g. the Line object
         uses "P1" and "P2" to address the line end points. The names for "P<IdxPt>" are
         editable but are defaulting to "P<IdxPt>".
         In addition to those predefined labels additional labels may be added by defining a
         unique name and assigning a text. Both the name and the text are stored in the Label object.
         When showing labels (adding them to the graphics scene) the desriptors are used to set
         the properties of the label objects. */
    QHash<QString, SLabelDscr> m_hshLabelDscrs;
    /*!< Hash with text labels which may be assigned to and indicated by the graphical object.
         Created on demand from the label descriptors if the labels are added to the graphics scene. */
    QHash<QString, CGraphObjLabel*> m_hshpLabels;
    /*!< List with the geometry label names. May be initialised in the constructor of derived classes
         in addition to the hash with geometry labels. Keeping the geometry label names also a string list
         should verify that the returned value names are always in the same order (as the order
         in a hash is arbitrary). For polygons the geometry label names may be added or removed during runtime. */
    QStringList m_strlstGeometryLabelNames;
    /*!< Hash with descriptors for geometry labels which may be indicated by the graphical object.
         The number of geometry labels depend on the object type.
         E.g. for the Line object the positions "P1", "P2" and "Center" as well as the "Width" and "Height",
         "Length" and "Angle" may be shown.
         When showing labels (adding them to the graphics scene) the desriptors are used to set
         the properties of the label objects. */
    QHash<QString, SLabelDscr> m_hshGeometryLabelDscrs;
    /*!< Hash with geometry labels which may be indicated by the graphical object.
         Created on demand from the geometry label descriptors if the labels are added to the graphics scene. */
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
    /*!< Concerning range see "Draw::getAngleRad" */
    double m_fRotAngleCurr_deg;
    /*!< In item's coordinate system. */
    QPointF m_ptRotOriginCurr;
    /*!< Original item coordinates and transform values:
        - after initially creating the item
        - changing position, size and rotation angle of objects with no parents
        - on adding items to groups
        - explicitly calling method "acceptCurrentAsOriginalCoors" */
    bool m_bHasValidOrigCoors;
    QSizeF m_sizOrig;
    /*!< Concerning range see "Draw::getAngleRad" */
    double m_fRotAngleOrig_deg;
    /*!< In item's coordinate system. */
    QPointF m_ptRotOriginOrig;
    /*!< Coordinates stored on mouse press events:
         In scene's coordinate system (for moving by my mouse move events). */
    //QPointF m_ptScenePosOnMousePressEvent;
    /*!< In scene's coordinate system (for moving by my mouse move events). */
    //QPointF m_ptMouseEvScenePosOnMousePressEvent;
    /*!< In item's coordinate system (for resizing by mouse move events). */
    //QRectF m_rctOnMousePressEvent;
    /*!< In scene's coordinate system (for rotation by mouse move events). */
    //QPointF m_ptRotOriginOnMousePressEvent;
    /*!< Counts how ofter the "itemChange" method has been blocked from updating
         the original object coordinates. A value greater than 0 for the counter means
         that the itemChange method must not update the original coordinates as they
         have been expliticely set by a method call whereupon the objects, transformed
         item coordinates in pixels should be updated by itemChange. */
    /*!< Simulation Functions. */
    //QList<SGraphObjMouseEventFct> m_arMousePressEventFunctions;
    //QList<SGraphObjMouseEventFct> m_arMouseReleaseEventFunctions;
    //QList<SGraphObjMouseEventFct> m_arMouseDoubleClickEventFunctions;
    //QList<SGraphObjMouseEventFct> m_arMouseMoveEventFunctions;
    //QList<SGraphObjKeyEventFct> m_arKeyPressEventFunctions;
    //QList<SGraphObjKeyEventFct> m_arKeyReleaseEventFunctions;
#endif
protected: // instance members
    /*!< When modifying the geometry of graphical objects via external "setGeometry" methods
         like "setLine" or "setRect", the position of the item may be changed and "itemChange"
         will be triggered with "positionHasChanged". The "itemChange" method with flag
         "positionHasChanged" may also be triggered by the graphics item system internally.
         If called internally the original coordinates need to be updated.
         But when calling the external "setGeometry" methods the original coordinates may have
         already been updated and may not be overridden by the "itemChange" method triggered by
         the external "setGeometry" updating the position of the item.
         In this case the BlockedCounter will (or has to) be set to a value greater than 0
         (e.g. using CRefCountGuard). */
    int m_iItemChangeUpdatePhysValCoorsBlockedCounter;
    /*!< To avoid that a signal is emitted twice for the same reason, the signal blocked counter
         may be set to a value greater than 0 (e.g. using CRefCountGuard). */
    int m_iGeometryOnSceneChangedSignalBlockedCounter;
    /*!< When adding a new child already existing childs should not calculate new positions and must not
         resize themselves if the geometry of the parent group is changed by adding the new child.
         The size of the already existing childs does not change. Only their position within the group.
         But the group will set the new position of the already existing childs. For this the childs
         must not react on the "parentGeometryOnSceneChanged" signal if the groups rectangle is set.
         Same applies if the parent resizes itself to its content. */
    int m_iIgnoreParentGeometryChange;
    /*!< Counters to block debug trace outputs for internal position and state infos. */
    int m_iTraceBlockedCounter;
    int m_iTracePositionInfoBlockedCounter;
    int m_iTraceThisPositionInfoInfoBlockedCounter;
    int m_iTraceParentGroupPositionInfoInfoBlockedCounter;
    int m_iTraceGraphicsItemStatesInfoBlockedCounter;
    int m_iTraceGraphObjStatesInfoBlockedCounter;
protected: // instance members (method tracing)
    /*!< Method Tracing (trace admin objects have to be created in ctor of "final" class)
         by calling "createTraceAdminObjs". */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjCtorsAndDtor;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjItemChange;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjBoundingRect;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjCoordinateConversions;
    //ZS::System::CTrcAdminObj* m_pTrcAdminObjIsHit;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjCursor;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjPaint;
    //ZS::System::CTrcAdminObj* m_pTrcAdminObjSceneEvent;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjSceneEventFilter;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjHoverEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseClickEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjMouseMoveEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjKeyEvents;

}; // class CGraphObj

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObj_h
