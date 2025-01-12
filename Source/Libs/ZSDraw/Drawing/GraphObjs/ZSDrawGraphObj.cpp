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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryAngle.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryDX.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryDY.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryLength.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabelGeometryPosition.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/QBitmap>
#include <QtWidgets/QMenu>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CGraphObj
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CGraphObj::c_strLabelName = "Name";
const QString CGraphObj::c_strGeometryLabelNameP = "P";
const QString CGraphObj::c_strGeometryLabelNameP1 = "P1";
const QString CGraphObj::c_strGeometryLabelNameP2 = "P2";
const QString CGraphObj::c_strGeometryLabelNameTopLeft = "TL";
const QString CGraphObj::c_strGeometryLabelNameTopRight = "TR";
const QString CGraphObj::c_strGeometryLabelNameBottomRight = "BR";
const QString CGraphObj::c_strGeometryLabelNameBottomLeft = "BL";
const QString CGraphObj::c_strGeometryLabelNameCenter = "Center";
const QString CGraphObj::c_strGeometryLabelNameDX = "dX";
const QString CGraphObj::c_strGeometryLabelNameDY = "dY";
const QString CGraphObj::c_strGeometryLabelNameLength = "Length";
const QString CGraphObj::c_strGeometryLabelNameWidth = "Width";
const QString CGraphObj::c_strGeometryLabelNameHeight = "Height";
const QString CGraphObj::c_strGeometryLabelNameAngle = "Angle";

/*==============================================================================
protected: // class members
==============================================================================*/

QColor CGraphObj::s_selectionColor = QColor::fromHsv(300, 30, 200);
QColor CGraphObj::s_highlightColor = QColor::fromHsv(240, 30, 200);

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates a graphical object instance.

    The ctor does neither add the instance as a graphic item to Qt's grapics scene
    nor to the index tree of the drawing scene.

    Please also note that the constructor of the base class does not add the
    predefined label "Name". Othwerwise labels would also be added to labels which
    again would have labels and so on (stack overflow). Also selection points should
    not get a label. For this, and to be flexible, the "Name" label usually invisible
    as default should be added in the constructor of the derived class - if desired.

    @param i_pDrawingScene [in]
        Pointer to drawing scene from which the object is created.
    @param i_strFactoryGroupName [in]
        The group name of the object factory used to create the graphical objects
        of the given type (e.g. "Draw::Standard Shapes", "Draw::Widgets", "Draw::Electricity").
    @param i_type [in]
        Type as int of the graphical item.
    @param i_strType [in]
        Type as string of the graphical item (e.g. "Line", "Rect", "CheckBox", "Resistor").
    @param i_strObjName [in]
        Object name (e.g. "Line1", "Rect5", "CheckBoxUseFeature", "Resistor500OhmToGnd").
    @param i_drawSettings [in]
        Draw settings used to paint the item on the scene.
    @param idxTreeEntryType [in]
        Entry type. As default the entry type of the graph objects is Branch as all of
        them may have children. Groups anyway have children. But also any other types
        as if objects are selected they will create selection points. Also labels may
        be created as children. Only for selection points and labels the entry type
        will be set to Leave.
*/
CGraphObj::CGraphObj(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strFactoryGroupName,
    EGraphObjType i_type,
    const QString& i_strType,
    const QString& i_strObjName,
    CIdxTreeEntry::EEntryType i_idxTreeEntryType ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(i_idxTreeEntryType, i_strObjName),
    m_bDtorInProgress(false),
    m_bAboutToBeDestroyedEmitted(false),
    m_bForceConversionToSceneCoors(false),
    m_pDrawingScene(i_pDrawingScene),
    m_strFactoryGroupName(i_strFactoryGroupName),
    m_type(i_type),
    m_strType(i_strType),
    m_drawSettings(i_type),
    m_pDrawSettingsTmp(nullptr),
    m_physValSizeMinimum(*i_pDrawingScene),
    m_physValSizeMaximum(*i_pDrawingScene),
    m_physValSizeFixed(*i_pDrawingScene),
    m_arAlignments(),
    m_bIsHighlighted(false),
    m_editMode(EEditMode::None),
    //m_editResizeMode(EEditResizeMode::None),
    m_bMouseReleaseEventFinishesObjectCreation(true),
    m_arfZValues(CEnumRowVersion::count(), 0.0),
    m_physValRotationAngle(0.0, Units.Angle.Degree, 0.1),
    m_pGraphObjGroupParent(nullptr),
    m_physValRectParentGroupOrig(*i_pDrawingScene),
    m_fParentGroupScaleX(1.0),
    m_fParentGroupScaleY(1.0),
    //m_transform(),
    //m_transformByGroup(),
    //m_ptScenePos(),
    //m_idxSelPtSelectedPolygon(-1),
    m_arpSelPtsPolygon(),
    //m_selPtSelectedBoundingRect(ESelectionPoint::None),
    m_arpSelPtsBoundingRect(CEnumSelectionPoint::count()),
    m_strlstPredefinedLabelNames(),
    m_hshLabelDscrs(),
    m_hshpLabels(),
    m_strlstGeometryLabelNames(),
    m_hshGeometryLabelDscrs(),
    m_hshpGeometryLabels(),
    m_strToolTip(),
    m_strEditInfo(),
    m_pMenuContext(nullptr),
    m_pActionMenuContextFormat(nullptr),
    m_pActionMenuContextModifyPoints(nullptr),
    m_pActionMenuContextDeletePoint(nullptr),
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_bCoorsDirty(true),
    m_rctCurr(),
    m_fRotAngleCurr_deg(0.0),
    m_ptRotOriginCurr(),
    // Original coordinates and transform values:
    m_bHasValidOrigCoors(false),
    m_sizOrig(),
    m_fRotAngleOrig_deg(0.0),
    m_ptRotOriginOrig(),
#endif
    // Coordinates stored on mouse press events:
    //m_ptScenePosOnMousePressEvent(),
    //m_ptMouseEvScenePosOnMousePressEvent(),
    //m_rctOnMousePressEvent(),
    //m_ptRotOriginOnMousePressEvent(),
    // Simulation Functions:
    //m_arMousePressEventFunctions(),
    //m_arMouseReleaseEventFunctions(),
    //m_arMouseDoubleClickEventFunctions(),
    //m_arMouseMoveEventFunctions(),
    //m_arKeyPressEventFunctions(),
    //m_arKeyReleaseEventFunctions(),
    m_iItemChangeBlockedCounter(0),
    m_iItemChangeUpdatePhysValCoorsBlockedCounter(0),
    m_iGeometryOnSceneChangedSignalBlockedCounter(0),
    m_iIgnoreParentGeometryChange(0),
    m_iTraceBlockedCounter(0),
    m_iTracePositionInfoBlockedCounter(0),
    m_iTraceThisPositionInfoInfoBlockedCounter(0),
    m_iTraceParentGroupPositionInfoInfoBlockedCounter(0),
    m_iTraceGraphicsItemStatesInfoBlockedCounter(0),
    m_iTraceGraphObjStatesInfoBlockedCounter(0),
    m_pTrcAdminObjCtorsAndDtor(nullptr),
    m_pTrcAdminObjItemChange(nullptr),
    m_pTrcAdminObjBoundingRect(nullptr),
    m_pTrcAdminObjCoordinateConversions(nullptr),
    //m_pTrcAdminObjIsHit(nullptr),
    m_pTrcAdminObjCursor(nullptr),
    m_pTrcAdminObjPaint(nullptr),
    //m_pTrcAdminObjSceneEvent(nullptr),
    m_pTrcAdminObjSceneEventFilter(nullptr),
    m_pTrcAdminObjHoverEvents(nullptr),
    m_pTrcAdminObjMouseClickEvents(nullptr),
    m_pTrcAdminObjMouseMoveEvents(nullptr),
    m_pTrcAdminObjKeyEvents(nullptr)
{
    QObject::connect(
        m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
        this, &CGraphObj::onDrawingSizeChanged);
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Destroys the graphical object.

    All selection points and labels will be destroyed together with the object.
    The destructor of the CIdxTreeEntry will remove the item from the index tree.

    @note The destructor of the CGraphObj base class does not remove the graphical
          item from the graphics scene. This must be done in the destructor of
          the derived classes. For this the signal "aboutToBeDestroyed" has to
          be called in the derived classes. A counter ensures that the signal is
          just emitted once by the top most class in the class hierarchy.
          Please also note that the destructor of CIdxTreeEntry removes the
          grahical object from the index tree - if the object was added to the
          index tree.

    The dynamic cast to QGraphicsItem may return nullptr if the dtor of QGraphicsItem
    has already been executed. As CGraphObj is not derived from QGraphicsItem but
    only the derived classes inherit QGraphicsItem the destructor of QGraphicsItem
    may have already been processed before the destructor of the CGraphObj base class.
    The order the dtors of inherited classes are called depend on the order the classes
    appear in the list of the inherited classes on defining the class implementation.
    We cannot (or we don't want) to rely on that the order is defined that way that
    the destructor of QGraphicsItem is called after the destructor of CGraphObj.
    So we cannot remove the QGraphicsItem by calling "QGraphicsScene::removeItem" from
    within the destructor of CGraphObj. The  graphics item must be removed from the
    graphics scene's item list before the dtor of class QGraphicsItem is called.
    And this is only always the case in the dtor of the class derived from QGraphicsItem.

    Inheritance Graph:

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
*/
CGraphObj::~CGraphObj()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;

    // Should already have been called by derived classes. If not, this may lead
    // to exceptions when removing the graphics item from the graphics scene.
    emit_aboutToBeDestroyed();

    try {
        delete m_pMenuContext;
    } catch (...) {
    }

    try {
        delete m_pDrawSettingsTmp;
    } catch (...) {
    }

    if (m_arpSelPtsBoundingRect.size() > 0) {
        for (int idxSelPt = m_arpSelPtsBoundingRect.size()-1; idxSelPt >= 0; idxSelPt--) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
            if (pGraphObjSelPt != nullptr) {
                m_arpSelPtsBoundingRect[idxSelPt] = nullptr;
                try {
                    delete pGraphObjSelPt;
                }
                catch(...) {
                }
                pGraphObjSelPt = nullptr;
            }
        }
    }

    if (m_arpSelPtsPolygon.size() > 0) {
        for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt--) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
            if (pGraphObjSelPt != nullptr) {
                m_arpSelPtsPolygon[idxSelPt] = nullptr;
                try {
                    delete pGraphObjSelPt;
                }
                catch(...) {
                }
                pGraphObjSelPt = nullptr;
            }
        }
    }

    QStringList strlstLabelNames = m_hshpLabels.keys();
    for (const QString& strLabelName : strlstLabelNames) {
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(strLabelName, nullptr);
        if (pGraphObjLabel != nullptr) {
            try {
                delete pGraphObjLabel;
            }
            catch(...) {
            }
            pGraphObjLabel = nullptr;
        }
    }

    QStringList strlstValueLabelNames = m_hshpGeometryLabels.keys();
    for (const QString& strLabelName : strlstValueLabelNames) {
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(strLabelName, nullptr);
        if (pGraphObjLabel != nullptr) {
            try {
                delete pGraphObjLabel;
            }
            catch(...) {
            }
            pGraphObjLabel = nullptr;
        }
    }

    // Please note that "onGraphObjAboutToBeDestroyed" is used to remove the graphical object from
    // the dictionary, the index list, and the sorted object pools of the drawing scene.
    // But selection points and labels have only been added to the item list of the graphics scene
    // and therefore don't have valid keys in the index tree.
    //if( m_pDrawingScene != nullptr )
    //{
    //    if( !m_strKeyInTree.isEmpty() )
    //    {
    //        try
    //        {
    //            m_pDrawingScene->onGraphObjAboutToBeDestroyed(m_strKeyInTree);
    //        }
    //        catch(...)
    //        {
    //        }
    //    }
    //    else
    //    {
    //        //QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    //        //m_pDrawingScene->removeItem(pGraphicsItem);
    //    }
    //} // if( m_pDrawingScene != nullptr )

    mthTracer.onAdminObjAboutToBeReleased();

    releaseTraceAdminObjs();

    m_bDtorInProgress = false;
    m_bAboutToBeDestroyedEmitted = false;
    m_bForceConversionToSceneCoors = false;
    m_pDrawingScene = nullptr;
    //m_strFactoryGroupName;
    m_type = static_cast<EGraphObjType>(0);
    //m_strType;
    //m_drawSettings;
    m_pDrawSettingsTmp = nullptr;
    //m_physValSizeMinimum;
    //m_physValSizeMaximum;
    //m_physValSizeFixed;
    //m_arAlignments;
    m_bIsHighlighted = false;
    m_editMode = static_cast<EEditMode>(0);
    //m_editResizeMode = static_cast<EEditResizeMode>(0);
    m_bMouseReleaseEventFinishesObjectCreation = false;
    //m_arfZValues.clear();
    //m_physValRotationAngle;
    m_pGraphObjGroupParent = nullptr;
    //m_physValRectParentGroupOrig;
    m_fParentGroupScaleX = 0.0;
    m_fParentGroupScaleY = 0.0;
    //m_transform;
    //m_transformByGroup;
    //m_ptScenePos;
    //m_idxSelPtSelectedPolygon = 0;
    //m_arpSelPtsPolygon;
    //m_selPtSelectedBoundingRect = static_cast<ESelectionPoint>(0);
    //m_arpSelPtsBoundingRect;
    //m_strlstPredefinedLabelNames;
    //m_hshLabelDscrs;
    //m_hshpLabels;
    //m_strlstGeometryLabelNames;
    //m_hshGeometryLabelDscrs;
    //m_hshpGeometryLabels;
    //m_strToolTip;
    //m_strEditInfo;
    m_pMenuContext = nullptr;
    m_pActionMenuContextFormat = nullptr;
    m_pActionMenuContextModifyPoints = nullptr;
    m_pActionMenuContextDeletePoint = nullptr;
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_bCoorsDirty = false;
    //m_rctCurr;
    m_fRotAngleCurr_deg = 0.0;
    //m_ptRotOriginCurr;
    // Original item coordinates and transform values:
    m_bHasValidOrigCoors = false;
    //m_sizOrig;
    m_fRotAngleOrig_deg = 0.0;
    //m_ptRotOriginOrig;
#endif
    // Coordinates stored on mouse press events:
    //m_ptScenePosOnMousePressEvent;
    //m_ptMouseEvScenePosOnMousePressEvent;
    //m_rctOnMousePressEvent;
    //m_ptRotOriginOnMousePressEvent;
    // Simulation Functions:
    //m_arMousePressEventFunctions;
    //m_arMouseReleaseEventFunctions;
    //m_arMouseDoubleClickEventFunctions;
    //m_arMouseMoveEventFunctions;
    //m_arKeyPressEventFunctions;
    //m_arKeyReleaseEventFunctions;
    m_iItemChangeBlockedCounter = 0;
    m_iItemChangeUpdatePhysValCoorsBlockedCounter = 0;
    m_iGeometryOnSceneChangedSignalBlockedCounter = 0;
    m_iIgnoreParentGeometryChange = 0;
    m_iTraceBlockedCounter = 0;
    m_iTracePositionInfoBlockedCounter = 0;
    m_iTraceThisPositionInfoInfoBlockedCounter = 0;
    m_iTraceParentGroupPositionInfoInfoBlockedCounter = 0;
    m_iTraceGraphicsItemStatesInfoBlockedCounter = 0;
    m_iTraceGraphObjStatesInfoBlockedCounter = 0;
    // Method Tracing
    m_pTrcAdminObjCtorsAndDtor = nullptr;
    m_pTrcAdminObjItemChange = nullptr;
    m_pTrcAdminObjBoundingRect = nullptr;
    m_pTrcAdminObjCoordinateConversions = nullptr;
    //m_pTrcAdminObjIsHit = nullptr;
    m_pTrcAdminObjCursor = nullptr;
    m_pTrcAdminObjPaint = nullptr;
    //m_pTrcAdminObjSceneEvent = nullptr;
    m_pTrcAdminObjSceneEventFilter = nullptr;
    m_pTrcAdminObjHoverEvents = nullptr;
    m_pTrcAdminObjMouseClickEvents = nullptr;
    m_pTrcAdminObjMouseMoveEvents = nullptr;
    m_pTrcAdminObjKeyEvents = nullptr;

} // dtor

/*==============================================================================
protected: // instance methods (trace admin objects have to be created in ctor of "final" class)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the trace admin objects for method tracing.

    This method has to be called in the constructor of the "final" class.

    Please note that the counterpart "releaseTraceAdminObjs" is called by the
    destructor of the base class CGraphObj. !! Don't call "releaseTraceAdminObjs"
    in the destructor of the derived class. !!

    @param [in] i_strClassName
        Class name of the derived class. The class name may be preceded with
        a group name like "StandardShapes", "Electricity" or "QtWidget" followed
        by a double colon like "createTraceAdminObjs("StandardShapes::" + ClassName)".
*/
void CGraphObj::createTraceAdminObjs(const QString& i_strClassName)
//------------------------------------------------------------------------------
{
    if (m_pTrcAdminObjCtorsAndDtor == nullptr) {
        m_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::CtorsAndDtor");
        m_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::ItemChange");
        m_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::BoundingRect");
        m_pTrcAdminObjCoordinateConversions = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::CoordinateConversions");
        //m_pTrcAdminObjIsHit = CTrcServer::GetTraceAdminObj(
        //    NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::IsHit");
        m_pTrcAdminObjCursor = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::Cursor");
        m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::Paint");
        //m_pTrcAdminObjSceneEvent = CTrcServer::GetTraceAdminObj(
        //    NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::SceneEvent");
        m_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::SceneEventFilter");
        m_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::HoverEvents");
        m_pTrcAdminObjMouseClickEvents = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::MouseClickEvents");
        m_pTrcAdminObjMouseMoveEvents = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::MouseMoveEvents");
        m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(
            NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::KeyEvents");
    }
}

//------------------------------------------------------------------------------
/*! @brief Releases the trace admin objects for method tracing.

    This method is called by the destructor of the base class CGraphObj and
    therefore ! MUST NOT ! be called by the destructor of the derived class.

    Please note that the counterpart "createTraceAdminObjs" !MUST BE! called
    by the constructor of the derived class.
*/
void CGraphObj::releaseTraceAdminObjs()
//------------------------------------------------------------------------------
{
    if (m_pTrcAdminObjCtorsAndDtor != nullptr) {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCtorsAndDtor);
        m_pTrcAdminObjCtorsAndDtor = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjItemChange);
        m_pTrcAdminObjItemChange = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjBoundingRect);
        m_pTrcAdminObjBoundingRect = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCoordinateConversions);
        m_pTrcAdminObjCoordinateConversions = nullptr;
        //CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjIsHit);
        //m_pTrcAdminObjIsHit = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCursor);
        m_pTrcAdminObjCursor = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaint);
        m_pTrcAdminObjPaint = nullptr;
        //CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEvent);
        //m_pTrcAdminObjSceneEvent = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEventFilter);
        m_pTrcAdminObjSceneEventFilter = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjHoverEvents);
        m_pTrcAdminObjHoverEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseClickEvents);
        m_pTrcAdminObjMouseClickEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseMoveEvents);
        m_pTrcAdminObjMouseMoveEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjKeyEvents);
        m_pTrcAdminObjKeyEvents = nullptr;
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the object type.
*/
EGraphObjType CGraphObj::type() const
//------------------------------------------------------------------------------
{
    return m_type;
}

//------------------------------------------------------------------------------
/*! @brief Returns the object type as a string.
*/
QString CGraphObj::typeAsString() const
//------------------------------------------------------------------------------
{
    return m_strType;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a line.
*/
bool CGraphObj::isLine() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeLine;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a rectangle.
*/
bool CGraphObj::isRect() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeRect;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is an ellipse.
*/
bool CGraphObj::isEllipse() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeEllipse;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a polygon.
*/
bool CGraphObj::isPolygon() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypePolygon;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a polyline.
*/
bool CGraphObj::isPolyline() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypePolyline;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a text.
*/
bool CGraphObj::isText() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeText;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is an image.
*/
bool CGraphObj::isImage() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeImage;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a connection point.
*/
bool CGraphObj::isConnectionPoint() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeConnectionPoint;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a connection line.
*/
bool CGraphObj::isConnectionLine() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeConnectionLine;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a group.
*/
bool CGraphObj::isGroup() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeGroup;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a selection point.
*/
bool CGraphObj::isSelectionPoint() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypeSelectionPoint;
}

//------------------------------------------------------------------------------
/*! @brief Convenience method to returning true if this object is a label.
*/
bool CGraphObj::isLabel() const
//------------------------------------------------------------------------------
{
    return ((m_type >= EGraphObjTypeLabelMin) && (m_type <= EGraphObjTypeLabelMax));
}

//------------------------------------------------------------------------------
/*! @brief Returns whether a mouse release event finishes the object creation.

    For polygons the object creation is not finished if the mouse is released but
    instead a mouse double click is used to add the last polygon point and which
    finishes creation of the object.
*/
bool CGraphObj::mouseReleaseEventFinishesObjectCreation() const
//------------------------------------------------------------------------------
{
    return m_bMouseReleaseEventFinishesObjectCreation;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the drawing scene the object belongs to.
*/
CDrawingScene* CGraphObj::drawingScene() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene;
}

//------------------------------------------------------------------------------
/*! @brief Returns the parent object the object belongs to.

    If the object is added as a child to a group the parent is the group object.

    @return Parent object or nullptr, if the object does not have a parent.
*/
//------------------------------------------------------------------------------
//CGraphObj* CGraphObj::parentGraphObj()
//{
//    return dynamic_cast<CGraphObj*>(m_pParentBranch);
//}

//------------------------------------------------------------------------------
/*! @brief Returns the parent object the object belongs to.

    For selection points this is the object creating the selection points and
    to which the selection points are linked to.
    For labels this is also the object creating the labels and to which the
    labels are linked to.
    If the object is added as a child to a group the parent is the group object.

    @return nullptr, if the object is neither a selection point, a label or is not
            added as a child to a group, pointer to parent otherwise.
*/
//------------------------------------------------------------------------------
CGraphObjGroup* CGraphObj::parentGroup() const
{
    return dynamic_cast<CGraphObjGroup*>(m_pParentBranch);
}

//------------------------------------------------------------------------------
/*! @brief Informs the object that the parent item of the item has been changed
           to another parent item.

    The method connects the slot onGraphObjParentGeometryOnSceneChanged to the
    geometryOnSceneChanged signal of the parent item. If the item already had
    a parent the signal/slot connection to the previous parent is removed.

    The itemChange method of QGraphicsItem with ItemParentHasChanged cannot be used
    for this as item has already been removed for the previous parent if the itemChange
    method is called.

    Items need to inform e.g. their labels about the geometry changes of the items
    the labels are linked to. For this the item emits the geometryOnSceneChanged
    signal if the geometry of the item changes.

    If the item belongs to a group and the group changes its geometry on the scene,
    also the child items geometry on the scene is changed and all labels linked to
    the items must update their geometry information and their position on the scene.
    The default implementation of the onGraphObjParentGeometryOnSceneChanged slot
    emits the geometryOnSceneChanged signal to update the labels.

    When resizing a group all children of the group should be resized and positioned so
    that they keep their relative positions and sizes within the group. If the item
    is added to a new group the current rectangle of the parent group is stored as
    the original group rectangle.
    If the item is removed from a group (but not added to a new group) the original
    parent group rectangle is invalidated.
    This allows the default implementation of onGraphObjParentGeometryOnSceneChanged
    to calculate the current scale factor of the parent group.

    @param [in] i_pGraphObjGroupPrev
        Pointer to group the item will be removed from or nullptr, if the item don't
        belong to a group yet.
    @param [in] i_pGraphObjGroupNew
        Pointer to group the item will be added to or nullptr, if the item will just
        be removed from the current group but not added to another group.
*/
void CGraphObj::onParentGroupChanged(CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "PrevGroup: " + QString(i_pGraphObjGroupPrev == nullptr ? "null" : i_pGraphObjGroupPrev->path()) +
            ", NewGroup: " + QString(i_pGraphObjGroupNew == nullptr ? "null" : i_pGraphObjGroupNew->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onParentGroupChanged",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    {   CRefCountGuard refCountGuardTracePositionInfo(&m_iTracePositionInfoBlockedCounter);

        if (i_pGraphObjGroupPrev != nullptr) {
            QObject::disconnect(
                i_pGraphObjGroupPrev, &CGraphObj::geometryOnSceneChanged,
                this, &CGraphObj::onGraphObjParentGeometryOnSceneChanged);
        }
        if (i_pGraphObjGroupNew != nullptr) {
            QObject::connect(
                i_pGraphObjGroupNew, &CGraphObj::geometryOnSceneChanged,
                this, &CGraphObj::onGraphObjParentGeometryOnSceneChanged);
        }
        m_pGraphObjGroupParent = i_pGraphObjGroupNew;
        initParentTransform();
        updateTransformedCoorsOnParentChanged(i_pGraphObjGroupPrev, i_pGraphObjGroupNew);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
///*! @brief Sets the parent object the object belongs to.
//
//    For selection points this is the object creating the selection points and
//    to which the selection points are linked to.
//    For labels this is also the object creating the labels and to which the
//    labels are linked to.
//    If the object is added as a child to a group the parent is the group object.
//*/
//void CGraphObj::setParentGraphObj(CGraphObj* i_pGraphObjParent)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = QString(i_pGraphObjParent == nullptr ? "null" : i_pGraphObjParent->keyInTree());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::rename",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_pGraphObjParent != i_pGraphObjParent) {
//        if (m_pGraphObjParent != nullptr) {
//            QObject::disconnect(
//                m_pGraphObjParent, &CGraphObj::geometryOnSceneChanged,
//                this, &CGraphObj::onGraphObjParentGeometryOnSceneChanged);
//            QObject::disconnect(
//                m_pGraphObjParent, &CGraphObj::zValueChanged,
//                this, &CGraphObj::onGraphObjParentZValueChanged);
//        }
//        m_pGraphObjParent = i_pGraphObjParent;
//        if (m_pGraphObjParent != nullptr) {
//            QObject::connect(
//                m_pGraphObjParent, &CGraphObj::geometryOnSceneChanged,
//                this, &CGraphObj::onGraphObjParentGeometryOnSceneChanged);
//            QObject::connect(
//                m_pGraphObjParent, &CGraphObj::zValueChanged,
//                this, &CGraphObj::onGraphObjParentZValueChanged);
//        }
//    }
//}

//------------------------------------------------------------------------------
/*! This method is provided for convinience to rename an entry without the need
    of accessing the index tree and invoking the rename method of the index tree.

    On changing the name of an index tree entry also its key in the index tree
    and the key of all child entries got to be updated. This is handled by the
    index tree. For this this method call is delegated to the index tree of the
    entry. As a precondition the entry (the graphical object) must have been
    added before to the index tree of the drawing scene.

    As a reentry the index tree calls "setName" of the tree entry. "setName"
    is overridden by the graphical object to update the label text (if visible).

    @param i_strNameNew [in] New name of the tree entry.

    @note Throws a critical exception
          - with Result = ObjNotInList if the tree entry does not belong to the
            index tree of the drawing scene.
*/
void CGraphObj::rename( const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strNameNew;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::rename",
        /* strAddInfo   */ strMthInArgs );

    if (m_pTree == nullptr) {
        SErrResultInfo errResultInfo = ErrResultInfoCritical("rename", EResultObjNotInList, m_strName);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if (m_strName != i_strNameNew) {
        // "setName" is called as reentry by the index tree.
        m_pTree->rename(this, i_strNameNew);
    }
}

/*==============================================================================
protected: // overridables of base class CIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the current name of the object.

    This method is for internal use only and is called by the index tree
    if an entry is renamed.

    Changing the name also changes its key.
    If the name of a branch is changed also the keys of all children must
    be set correspondingly. This is handled by the index tree.

    If the name of the object is set (either after initially creating the object
    or after renaming the object) the name and path indicated in visible labels
    must be updated.

    @param i_strName [in] New name of the tree entry.
*/
void CGraphObj::setName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setName",
        /* strAddInfo   */ strMthInArgs );

    if (m_strName != i_strName)
    {
        CIdxTreeEntry::setName(i_strName);
        if (m_hshLabelDscrs.contains(c_strLabelName)) {
            m_hshLabelDscrs[c_strLabelName].m_strText = i_strName;
        }
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(c_strLabelName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setText(name());
        }
        //updateEditInfo();
        //updateToolTip();
    }
}

//------------------------------------------------------------------------------
/*! Sets the key of the entry in the index tree.

    This method is for internal use only and is implicitely called by the
    index tree if the name or the parent of an entry is changed.

    If the name or the parent of the object is changed the name and path indicated
    in visible labels must be updated.

    @param i_strKey [in] New key of the tree entry.
*/
void CGraphObj::setKeyInTree( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKey;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setKeyInTree",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree != i_strKey)
    {
        CIdxTreeEntry::setKeyInTree(i_strKey);
        if (m_hshLabelDscrs.contains(c_strLabelName)) {
            m_hshLabelDscrs[c_strLabelName].m_strText = name();
        }
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(c_strLabelName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setText(name());
        }
        //updateEditInfo();
        //updateToolTip();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the group name of the class factory used to create objects.

    The factory group name and the type of the object uniquely define the object factory.
*/
QString CGraphObj::getFactoryGroupName() const
//------------------------------------------------------------------------------
{
    return m_strFactoryGroupName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the currently selected index of the polygons shape point
           if the form of the object is modified by moving a polygon shape point.
*/
//int CGraphObj::getSelectedPolygonShapePointIndex() const
////------------------------------------------------------------------------------
//{
//    return m_idxSelPtSelectedPolygon;
//}

//------------------------------------------------------------------------------
/*! @brief Returns the currently selected point at the bounding rectangle if
           if the form of the object is modified by moving a selection point
           at the bounding rectangle.

     PolygonPoint is returned if a polygon point is selected.
*/
//CEnumSelectionPoint CGraphObj::getSelectedBoundingRectPoint() const
////------------------------------------------------------------------------------
//{
//    return m_selPtSelectedBoundingRect;
//}

//------------------------------------------------------------------------------
/*! @brief Returns a human readable tool tip string.
*/
//QString CGraphObj::getToolTip() const
////------------------------------------------------------------------------------
//{
//    return m_strToolTip;
//}

//------------------------------------------------------------------------------
/*! @brief Returns a human readable string with information about the current editing mode.
*/
//QString CGraphObj::getEditInfo() const
////------------------------------------------------------------------------------
//{
//    return m_strEditInfo;
//}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to show the context popup menu.
    Usually opened by right clicking on the the object.
*/
void CGraphObj::createContextMenu()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::createContextMenu",
        /* strAddInfo   */ "" );
    if (m_pMenuContext == nullptr) {
        m_pMenuContext = new QMenu(path());

        m_pActionMenuContextFormat = new QAction("Format", this);
        m_pMenuContext->addAction(m_pActionMenuContextFormat);
        QObject::connect(
            m_pActionMenuContextFormat, &QAction::triggered,
            this, &CGraphObj::onActionFormatTriggered);

        m_pActionMenuContextModifyPoints = new QAction("Modify Points", this);
        m_pMenuContext->addAction(m_pActionMenuContextModifyPoints);
        QObject::connect(
            m_pActionMenuContextModifyPoints, &QAction::triggered,
            this, &CGraphObj::onActionModifyPointsTriggered);
        m_pActionMenuContextModifyPoints->setEnabled(false);
        m_pActionMenuContextModifyPoints->setVisible(false);

        m_pActionMenuContextDeletePoint = new QAction("Delete Point", this);
        m_pMenuContext->addAction(m_pActionMenuContextDeletePoint);
        QObject::connect(
            m_pActionMenuContextDeletePoint, &QAction::triggered,
            this, &CGraphObj::onActionDeletePointTriggered);
        m_pActionMenuContextDeletePoint->setEnabled(false);
        m_pActionMenuContextDeletePoint->setVisible(false);
    }
    m_pMenuContext->setTitle(path());
}

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to show the context popup menu.
    Usually opened by right clicking on the the object.
*/
void CGraphObj::showContextMenu(const QPointF& i_ptScreenPos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pos {" + qPoint2Str(i_ptScreenPos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::showContextMenu",
        /* strAddInfo   */ strMthInArgs );

    if (m_pMenuContext == nullptr) {
        createContextMenu();
    }
    m_pMenuContext->setTitle(path());
    m_pMenuContext->popup(i_ptScreenPos.toPoint());
}

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObj::openFormatGraphObjsDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::openFormatGraphObjsDialog",
        /* strAddInfo   */ "" );
    throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented, "should become pure virtual");
}

//------------------------------------------------------------------------------
/* @brief

    Base implemention does nothing.
*/
void CGraphObj::openDeletePointDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::openDeletePointDialog",
        /* strAddInfo   */ "" );
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the draw settings like pen color, pen style, brush style etc.
           used to paint the object.
*/
void CGraphObj::setDrawSettings( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setDrawSettings",
        /* strAddInfo   */ strMthInArgs );

    // When emitting the drawSettingsChanged signal new settings may be applied
    // as reentries by the signal receivers. Those new settings may have to be
    // temporarily stored and a new temporary buffer has to be allocated.
    // The current buffer pointer will be locally saved and freed after emitting
    // the signals. If new settings got to be saved a new buffer is allocated.
    CDrawSettings* pDrawSettingsTmp = m_pDrawSettingsTmp;
    m_pDrawSettingsTmp = nullptr;

    if (m_drawSettings != i_drawSettings) {
        CDrawSettings drawSettingsOld = m_drawSettings;
        m_drawSettings = i_drawSettings;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
        onDrawSettingsChanged(drawSettingsOld);
    }
    delete pDrawSettingsTmp;
    pDrawSettingsTmp = nullptr;
}

//------------------------------------------------------------------------------
/*! @brief Returns the currently used draw settings like pen color, pen style,
           brush style etc. used to paint the object.
*/
CDrawSettings CGraphObj::getDrawSettings() const
//------------------------------------------------------------------------------
{
    return m_drawSettings;
}

//------------------------------------------------------------------------------
/*! @brief Emits the drawSettingsChanged signal.

    If there are temporary stored changes those changes will first be taken
    over before the signal is emitted.

    The temporary cache is used if several draw setting attributes got to be
    modified one after another. For this the flag i_bImmediatelyApplySetting
    has been passed with false to the "set<DrawSettingProperty>" methods.
    After modifying all attributes "updateDrawSettings" must be called to take
    over the modified settings, to repaint the graphics item with the modified
    attributes and to emit the drawSettingsChanged signal.

    @return true, if there were changes, false otherwise.
*/
bool CGraphObj::updateDrawSettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::updateDrawSettings",
        /* strAddInfo   */ "" );

    bool bChanged = false;
    if (m_pDrawSettingsTmp != nullptr) {
        if (*m_pDrawSettingsTmp != m_drawSettings) {
            bChanged = true;
            // setDrawSettings will delete the temporary buffer.
            setDrawSettings(*m_pDrawSettingsTmp);
        }
        else {
            delete m_pDrawSettingsTmp;
            m_pDrawSettingsTmp = nullptr;
        }
    }

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChanged);
    }
    return bChanged;
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    Must be overridden to apply the draw settings at the graphics item in derived classes.
*/
void CGraphObj::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    bool bDrawSettingsChanged = (m_drawSettings != i_drawSettingsOld);
    if (bDrawSettingsChanged) {
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        if (pGraphicsItemThis != nullptr) {
            pGraphicsItemThis->update(); // force paint method
        }
        emit_drawSettingsChanged(); // inform others of changes
    }
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the pen color used to paint the object.

    If several draw setting attributes got to modified one after another
    the flag i_bImmediatelyApplySetting should be set to false.
    In this case the modified draw setting attribute will be stored in a
    temporary cache, the signal "drawSettingsChanged" is not emitted and
    the object is not forced to be repainted.

    After setting all draw attributes the method updateDrawSettings must be
    called to take over the modified draw attributes from the temporary cache
    to the current draw settings. The updateDrawSettings method will update
    (repaint) the graphics item and emits the drawSettingsChanged signal.

    @param [in] i_clr
        New pen color to be used.
    @param [in] i_bImmediatelyApplySetting (default: true)
        Set this flag to false if further set<DrawAttribute> will follow and you
        want to update all changes at once later on.
*/
void CGraphObj::setPenColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setPenColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getPenColor() != i_clr) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setPenColor(i_clr);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setPenColor(i_clr);
    }
}

//------------------------------------------------------------------------------
/*! @brief 
*/
QColor CGraphObj::getPenColor() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getPenColor();
}

//------------------------------------------------------------------------------
/*! @brief Sets the pen width used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setPenWidth( int i_iLineWidth, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iLineWidth) + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setPenWidth",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getPenWidth() != i_iLineWidth) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setPenWidth(i_iLineWidth);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setPenWidth(i_iLineWidth);
    }
}

//------------------------------------------------------------------------------
/*! @brief 
*/
int CGraphObj::getPenWidth() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getPenWidth();
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the line style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineStyle( const CEnumLineStyle& i_lineStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_lineStyle.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setLineStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getLineStyle() != i_lineStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setLineStyle(i_lineStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineStyle(i_lineStyle);
    }
} // setLineStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumLineStyle CGraphObj::getLineStyle() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineStyle();
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the fill color width used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setFillColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setFillColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getFillColor() != i_clr) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setFillColor(i_clr);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setFillColor(i_clr);
    }
} // setFillColor

//------------------------------------------------------------------------------
/*! @brief 
*/
QColor CGraphObj::getFillColor() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getFillColor();
}

//------------------------------------------------------------------------------
/*! @brief Sets the fill style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setFillStyle( const CEnumFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fillStyle.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setFillStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getFillStyle() != i_fillStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setFillStyle(i_fillStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setFillStyle(i_fillStyle);
    }
} // setFillStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumFillStyle CGraphObj::getFillStyle() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getFillStyle();
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the line record type used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineRecordType( const CEnumLineRecordType& i_recordType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_recordType.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setLineRecordType",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getLineRecordType() != i_recordType) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setLineRecordType(i_recordType);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineRecordType(i_recordType);
    }
} // setLineRecordType

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumLineRecordType CGraphObj::getLineRecordType() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineRecordType();
}

//------------------------------------------------------------------------------
/*! @brief Sets the line extent used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineExtent( int i_iLineExtent, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iLineExtent) + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setLineExtent",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getLineExtent() != i_iLineExtent) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setLineExtent(i_iLineExtent);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineExtent(i_iLineExtent);
    }
} // setLineExtent

//------------------------------------------------------------------------------
/*! @brief 
*/
int CGraphObj::getLineExtent() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineExtent();
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the line end style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineEndStyle(
    const CEnumLinePoint& i_linePoint, const CEnumLineEndStyle& i_endStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_endStyle.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setLineEndStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getLineEndStyle(i_linePoint) != i_endStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setLineEndStyle(i_linePoint, i_endStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setLineEndStyle(i_linePoint, i_endStyle);
    }
} // setLineEndStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumLineEndStyle CGraphObj::getLineEndStyle(const CEnumLinePoint& i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineEndStyle(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end base line type used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setArrowHeadBaseLineType(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadBaseLineType& i_baseLineType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_baseLineType.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setArrowHeadBaseLineType",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getArrowHeadBaseLineType(i_linePoint) != i_baseLineType) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setArrowHeadBaseLineType(i_linePoint, i_baseLineType);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadBaseLineType(i_linePoint, i_baseLineType);
    }
} // setArrowHeadBaseLineType

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumArrowHeadBaseLineType CGraphObj::getArrowHeadBaseLineType( const CEnumLinePoint& i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getArrowHeadBaseLineType(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end fill style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setArrowHeadFillStyle(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadFillStyle& i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_fillStyle.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setArrowHeadFillStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getArrowHeadFillStyle(i_linePoint) != i_fillStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setArrowHeadFillStyle(i_linePoint, i_fillStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadFillStyle(i_linePoint, i_fillStyle);
    }
} // setArrowHeadFillStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumArrowHeadFillStyle CGraphObj::getArrowHeadFillStyle(
    const CEnumLinePoint& i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getArrowHeadFillStyle(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end width used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setArrowHeadWidth(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadWidth& i_width, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_width.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setArrowHeadWidth",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getArrowHeadWidth(i_linePoint) != i_width) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setArrowHeadWidth(i_linePoint, i_width);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadWidth(i_linePoint, i_width);
    }
} // setArrowHeadWidth

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumArrowHeadWidth CGraphObj::getArrowHeadWidth(const CEnumLinePoint& i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getArrowHeadWidth(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end length used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setArrowHeadLength(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadLength& i_length, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_linePoint.toString() + ", " + i_length.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setArrowHeadLength",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getArrowHeadLength(i_linePoint) != i_length) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setArrowHeadLength(i_linePoint, i_length);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setArrowHeadLength(i_linePoint, i_length);
    }
} // setArrowHeadLength

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumArrowHeadLength CGraphObj::getArrowHeadLength( const CEnumLinePoint& i_linePoint ) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getArrowHeadLength(i_linePoint);
}

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the text color used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_clr.name() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setTextColor",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextColor() != i_clr) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextColor(i_clr);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextColor(i_clr);
    }
} // setTextColor

//------------------------------------------------------------------------------
/*! @brief 
*/
QColor CGraphObj::getTextColor() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextColor();
}

//------------------------------------------------------------------------------
/*! @brief Sets the font used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextFont( const QFont& i_fnt, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_fnt.family() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setTextFont",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextFont() != i_fnt) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextFont(i_fnt);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextFont(i_fnt);
    }
} // setTextFont

//------------------------------------------------------------------------------
/*! @brief 
*/
QFont CGraphObj::getTextFont() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextFont();
}

//------------------------------------------------------------------------------
/*! @brief Sets the text size used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextSize( ETextSize i_size, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = textSize2Str(i_size) + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setTextSize",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextSize() != i_size) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextSize(i_size);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextSize(i_size);
    }
} // setTextSize

//------------------------------------------------------------------------------
/*! @brief 
*/
ETextSize CGraphObj::getTextSize() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextSize();
}

//------------------------------------------------------------------------------
/*! @brief Sets the text style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextStyle( const CEnumTextStyle& i_textStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textStyle.toString() + ", eApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setTextStyle",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextStyle() != i_textStyle) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextStyle(i_textStyle);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextStyle(i_textStyle);
    }
} // setTextStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumTextStyle CGraphObj::getTextStyle() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextStyle();
}

//------------------------------------------------------------------------------
/*! @brief Sets the text effect used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextEffect( const CEnumTextEffect& i_textEffect, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_textEffect.toString() + ", Apply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setTextEffect",
        /* strAddInfo   */ strMthInArgs );

    if (i_bImmediatelyApplySetting) {
        if (m_drawSettings.getTextEffect() != i_textEffect) {
            CDrawSettings drawSettingsOld = m_drawSettings;
            m_drawSettings.setTextEffect(i_textEffect);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged(drawSettingsOld);
        }
    }
    else {
        if (m_pDrawSettingsTmp == nullptr) {
            m_pDrawSettingsTmp = new CDrawSettings(m_drawSettings);
        }
        m_pDrawSettingsTmp->setTextEffect(i_textEffect);
    }
} // setTextEffect

//------------------------------------------------------------------------------
/*! @brief 
*/
CEnumTextEffect CGraphObj::getTextEffect() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextEffect();
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setMinimumWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setMinimumWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum.setWidth(i_physValWidth);

    //if (m_physValSizeMinimum.width() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMinimum.unit());
    //    if (physValSize.width() < m_physValSizeMinimum.width()) {
    //        physValSize.setWidth(m_physValSizeMinimum.width());
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.width() > 0.0) {
        bHas = true;
    }
    else if (m_physValSizeMinimum.width() > 0.0) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getMinimumWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if (m_physValSizeMinimum.width() > 0.0) {
        physValWidth = m_physValSizeMinimum.width();
    }
    physValWidth.convertValue(i_unit);
    return physValWidth;
}

//------------------------------------------------------------------------------
void CGraphObj::setMinimumHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setMinimumHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum.setHeight(i_physValHeight);

    //if (m_physValSizeMinimum.height() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMinimum.unit());
    //    if (physValSize.height() < m_physValSizeMinimum.height()) {
    //        physValSize.setHeight(m_physValSizeMinimum.height());
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.height() > 0.0) {
        bHas = true;
    }
    else if (m_physValSizeMinimum.height() > 0.0) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getMinimumHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if (m_physValSizeMinimum.height() > 0.0) {
        physValHeight = m_physValSizeMinimum.height();
    }
    physValHeight.convertValue(i_unit);
    return physValHeight;
}

//------------------------------------------------------------------------------
void CGraphObj::setMinimumSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setMinimumSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum = i_physValSize;

    //if (m_physValSizeMinimum.isValid()) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMinimum.unit());
    //    if (physValSize.width() < m_physValSizeMinimum.width() || physValSize.height() < m_physValSizeMinimum.height()) {
    //        if (physValSize.width() < m_physValSizeMinimum.width()) {
    //            physValSize.setWidth( m_physValSizeMinimum.width() );
    //        }
    //        if (physValSize.height() < m_physValSizeMinimum.height()) {
    //            physValSize.setHeight( m_physValSizeMinimum.height() );
    //        }
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumSize() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.isValid() ) {
        bHas = true;
    }
    else if (m_physValSizeMinimum.isValid()) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObj::getMinimumSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize(*m_pDrawingScene);
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMinimum.isValid()) {
        physValSize = m_physValSizeMinimum;
    }
    if (parentGroup() != nullptr) {
        parentGroup()->convert(physValSize, i_unit);
    }
    else {
        m_pDrawingScene->convert(physValSize, i_unit);
    }
    return physValSize;
}

//------------------------------------------------------------------------------
void CGraphObj::setMaximumWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setMaximumWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum.setWidth(i_physValWidth);

    //if (m_physValSizeMaximum.width() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMaximum.unit());
    //    if (physValSize.width() > m_physValSizeMaximum.width()) {
    //        physValSize.setWidth( m_physValSizeMaximum.width() );
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.width() > 0.0) {
        bHas = true;
    }
    else if (m_physValSizeMaximum.width() > 0.0) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getMaximumWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if (m_physValSizeMaximum.width() > 0.0) {
        physValWidth = m_physValSizeMaximum.width();
    }
    physValWidth.convertValue(i_unit);
    return physValWidth;
}

//------------------------------------------------------------------------------
void CGraphObj::setMaximumHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setMaximumHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum.setHeight(i_physValHeight);

    //if (m_physValSizeMaximum.height() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMaximum.unit());
    //    if (physValSize.height() > m_physValSizeMaximum.height()) {
    //        physValSize.setHeight( m_physValSizeMaximum.height() );
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.height() > 0.0) {
        bHas = true;
    }
    else if (m_physValSizeMaximum.height() > 0.0) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getMaximumHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if (m_physValSizeMaximum.height() > 0.0) {
        physValHeight = m_physValSizeMaximum.height();
    }
    physValHeight.convertValue(i_unit);
    return physValHeight;
}

//------------------------------------------------------------------------------
void CGraphObj::setMaximumSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setMaximumSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum = i_physValSize;

    //if (m_physValSizeMaximum.isValid()) {
    //    CPhysValSize physValSize = getSize(m_physValSizeMaximum.unit());
    //    if (physValSize.width() > m_physValSizeMaximum.width()
    //     || physValSize.height() > m_physValSizeMaximum.height()) {
    //        if (physValSize.width() > m_physValSizeMaximum.width()) {
    //            physValSize.setWidth( m_physValSizeMaximum.width() );
    //        }
    //        if (physValSize.height() > m_physValSizeMaximum.height()) {
    //            physValSize.setHeight( m_physValSizeMaximum.height() );
    //        }
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumSize() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.isValid()) {
        bHas = true;
    }
    else if (m_physValSizeMaximum.isValid()) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObj::getMaximumSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize(*m_pDrawingScene);
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMaximum.isValid()) {
        physValSize = m_physValSizeMaximum;
    }
    if (parentGroup() != nullptr) {
        parentGroup()->convert(physValSize, i_unit);
    }
    else {
        m_pDrawingScene->convert(physValSize, i_unit);
    }
    return physValSize;
}

//------------------------------------------------------------------------------
void CGraphObj::setFixedWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setFixedWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed.setWidth(i_physValWidth);

    //if (m_physValSizeFixed.width() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeFixed.unit());
    //    if (physValSize.width() != m_physValSizeFixed.width()) {
    //        physValSize.setWidth( m_physValSizeFixed.width() );
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.width() > 0.0) {
        bHas = true;
    }
    else if ((m_physValSizeMinimum.width() > 0.0)
          && (m_physValSizeMinimum.width() == m_physValSizeMaximum.width())) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getFixedWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if ((m_physValSizeMinimum.width() > 0.0)
          && (m_physValSizeMinimum.width() == m_physValSizeMaximum.width())) {
        physValWidth = m_physValSizeMinimum.width();
    }
    physValWidth.convertValue(i_unit);
    return physValWidth;
}

//------------------------------------------------------------------------------
void CGraphObj::setFixedHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setFixedHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed.setHeight(i_physValHeight);

    //if (m_physValSizeFixed.height() > 0.0) {
    //    CPhysValSize physValSize = getSize(m_physValSizeFixed.unit());
    //    if (physValSize.height() != m_physValSizeFixed.height()) {
    //        physValSize.setHeight( m_physValSizeFixed.height() );
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.height() > 0.0) {
        bHas = true;
    }
    else if ((m_physValSizeMinimum.height() > 0.0)
          && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
        bHas = true;
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getFixedHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.imageCoorsResolution(drawingSize.unit()));
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if ((m_physValSizeMinimum.height() > 0.0)
          && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
        physValHeight = m_physValSizeMinimum.height();
    }
    physValHeight.convertValue(i_unit);
    return physValHeight;
}

//------------------------------------------------------------------------------
void CGraphObj::setFixedSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setFixedSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed = i_physValSize;

    //if (m_physValSizeFixed.isValid()) {
    //    CPhysValSize physValSize = getSize(m_physValSizeFixed.unit());
    //    if (physValSize.width() != m_physValSizeFixed.width()
    //     || physValSize.height() != m_physValSizeFixed.height()) {
    //        if (physValSize.width() != m_physValSizeFixed.width()) {
    //            physValSize.setWidth( m_physValSizeFixed.width() );
    //        }
    //        if (physValSize.height() != m_physValSizeFixed.height()) {
    //            physValSize.setHeight( m_physValSizeFixed.height() );
    //        }
    //        setSize(physValSize);
    //    }
    //}
}

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedSize() const
//------------------------------------------------------------------------------
{
    bool bHas = false;
    if (m_physValSizeFixed.isValid()) {
        bHas = true;
    }
    else if (m_physValSizeMinimum.isValid() && m_physValSizeMaximum.isValid()) {
        if ((m_physValSizeMinimum.width() == m_physValSizeMaximum.width())
         && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
            bHas = true;
        }
    }
    return bHas;
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObj::getFixedSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize(*m_pDrawingScene);
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMinimum.isValid() && m_physValSizeMaximum.isValid()) {
        if ((m_physValSizeMinimum.width() == m_physValSizeMaximum.width())
         && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
            physValSize = m_physValSizeMinimum;
        }
    }
    if (parentGroup() != nullptr) {
        parentGroup()->convert(physValSize, i_unit);
    }
    else {
        m_pDrawingScene->convert(physValSize, i_unit);
    }
    return physValSize;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    The alignments will be adjusted in the order they are added.
    The order takes effect on the result. Usually the size should be adjusted
    before the positions to get relative adjustments working as expected.
*/
int CGraphObj::addAlignment( const SGraphObjAlignment& i_alignment )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_alignment.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::addAlignment",
        /* strAddInfo   */ strMthInArgs );

    int idx = m_arAlignments.size();
    m_arAlignments.append(i_alignment);

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
    return idx;
}

//------------------------------------------------------------------------------
int CGraphObj::getAlignmentCount() const
//------------------------------------------------------------------------------
{
    return m_arAlignments.size();
}

//------------------------------------------------------------------------------
SGraphObjAlignment CGraphObj::getAlignment( int i_idx ) const
//------------------------------------------------------------------------------
{
    if (i_idx < 0 || i_idx >= m_arAlignments.size()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
    }
    return m_arAlignments[i_idx];
}

//------------------------------------------------------------------------------
void CGraphObj::setAlignment( int i_idx, const SGraphObjAlignment& i_alignment )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx) + ": " + i_alignment.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setAlignment",
        /* strAddInfo   */ strMthInArgs );

    if (i_idx < 0 || i_idx >= m_arAlignments.size()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx));
    }
    m_arAlignments[i_idx] = i_alignment;

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
void CGraphObj::removeAlignment( int i_idx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::removeAlignment",
        /* strAddInfo   */ strMthInArgs );

    if (i_idx < 0 || i_idx >= m_arAlignments.size()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
    }
    m_arAlignments.removeAt(i_idx);

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
void CGraphObj::clearAlignments()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::clearAlignments",
        /* strAddInfo   */ "" );

    m_arAlignments.clear();

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//------------------------------------------------------------------------------
/*! Accepts the current coordindates of the graphic item as the original coordinates.

    The current coordinates are in scene coordinates and define the position and size
    of the item on the scene.

    When rotating an item or resizing a group the original coordinates are important
    and will be used to calculate the resulting position and size of the item and
    its children on the scene.

    If a transformation, grouping and/or ungrouping items, resizing a group, initial
    creating or changing the position of an item has been finished it is essential
    to accept the current as the original coordinates.

    Items belonging to a group may not be absolutely positioned within the group but
    relative with alignment parameters like aligned to top, to bottom, center etc.
    If the group is resized the relative distance to the border should be kept while
    resizing the group. For this "applyGeometryChangeToChildrens" is using the
    original coordinates.
*/
void CGraphObj::acceptCurrentAsOriginalCoors()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::acceptCurrentAsOriginalCoors",
        /* strAddInfo   */ "" );

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        //setPosOrig(pGraphicsItem->pos());
    }

    m_sizOrig = m_rctCurr.size();
    m_fRotAngleOrig_deg = m_fRotAngleCurr_deg;
    m_ptRotOriginOrig = m_ptRotOriginCurr;
    //m_fScaleFacXOrig = m_fScaleFacXCurr;
    //m_fScaleFacYOrig = m_fScaleFacYCurr;

    m_bHasValidOrigCoors = true;

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
} // acceptCurrentAsOriginalCoors
#endif

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Maps the given point given in local coordinates relative to the origin
           point of the item's bounding rectangle to the top left corner of the
           item's bounding rectangle.

          -3        -2        -1         0         1         2         3
           0987654321098765432109876543210123456789012345678901234567890
       -10 +-----------------------------------------------------------+  0
        -9 |      (-20,-8) => (10,2)                                   |  1
        -8 |         X                                                 |  2
        -7 |                                                           |  3
        -6 |                                                           |  4
        -5 |                                                           |  5
        -4 |                                                           |  6
        -3 |                                                           |  7
        -2 |                                                           |  8
        -1 |                       (0,0) => (30,10)                    |  9
         0 |                             O                             | 10
         1 |                                                           | 11
         2 |                                                           | 12
         3 |                                                           | 13
         4 |                                                           | 14
         5 |                                                           | 15
         6 |                                                           | 16
         7 |                                         (20,8) => (50,18) | 17
         8 |                                                 X         | 18
         9 |                                                           | 19
        10 +-----------------------------------------------------------+ 20
           0         1         2         3         4         5         6
           0123456789012345678901234567890123456789012345678901234567890

    @param [in] i_pt
        Point in item's local coordinates relative to the origin of the
        item's bounding rectangle.

    @return Point relative to the top left corner of the item's bounding rectangle.
*/
QPointF CGraphObj::mapToTopLeftOfBoundingRect(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding = getBoundingRect();   // (-30, -10, 60, 20)
    QPointF ptOrigin = rctBounding.topLeft(); // (-30, -10)
    return i_pt - ptOrigin;                   // (-20, -8) - (-30, -10) = (10, 2)
}

//------------------------------------------------------------------------------
QLineF CGraphObj::mapToTopLeftOfBoundingRect(const QLineF& i_line) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding = getBoundingRect();
    QPointF ptOrigin = rctBounding.topLeft();
    return QLineF(i_line.p1() - ptOrigin, i_line.p2() - ptOrigin);
}

//------------------------------------------------------------------------------
QRectF CGraphObj::mapToTopLeftOfBoundingRect(const QRectF& i_rect) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding = getBoundingRect();
    QPointF ptOrigin = rctBounding.topLeft();
    return QRectF(i_rect.topLeft() - ptOrigin, i_rect.bottomRight() - ptOrigin);
}

//------------------------------------------------------------------------------
/*! @brief Maps the given point given in local coordinates relative to the top left
           corner of the item's bounding rectangle to the center point of the
           item's bounding rectangle.

          0         1         2         3         4         5         6
          0123456789012345678901234567890123456789012345678901234567890
        0 +-----------------------------------------------------------+ -10
        1 |      (10,2) => (-20,-8)                                   |  -9
        2 |         X                                                 |  -8
        3 |                                                           |  -7
        4 |                                                           |  -6
        5 |                                                           |  -5
        6 |                                                           |  -4
        7 |                                                           |  -3
        8 |                                                           |  -2
        9 |                       (30,10) => (0,0)                    |  -1
       10 |                             O                             |   0
       11 |                                                           |   1
       12 |                                                           |   2
       13 |                                                           |   3
       14 |                                                           |   4
       15 |                                                           |   5
       16 |                                                           |   6
       17 |                                         (50,18) => (20,8) |   7
       18 |                                                 X         |   8
       19 |                                                           |   9
       20 +-----------------------------------------------------------+  10
         -3        -2        -1         0         1         2         3
          0987654321098765432109876543210123456789012345678901234567890

    @param [in] i_pt
        Point in item's local coordinates relative to the top left corner
        of the item's bounding rectangle.

    @return Point relative to the center point of the item's bounding rectangle.
*/
QPointF CGraphObj::mapFromTopLeftOfBoundingRect(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding = getBoundingRect();   // (-30, -10, 60, 20)
    QPointF ptOrigin = rctBounding.topLeft(); // (-30, -10)
    return i_pt + ptOrigin;                   // (10, 2) + (-30, -10) = (-20, -8)
}

//------------------------------------------------------------------------------
QLineF CGraphObj::mapFromTopLeftOfBoundingRect(const QLineF& i_line) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding = getBoundingRect();
    QPointF ptOrigin = rctBounding.topLeft();
    return QLineF(i_line.p1() + ptOrigin, i_line.p2() + ptOrigin);
}

//------------------------------------------------------------------------------
QRectF CGraphObj::mapFromTopLeftOfBoundingRect(const QRectF& i_rect) const
//------------------------------------------------------------------------------
{
    QRectF rctBounding = getBoundingRect();
    QPointF ptOrigin = rctBounding.topLeft();
    return QRectF(i_rect.topLeft() + ptOrigin, i_rect.bottomRight() + ptOrigin);
}

/*==============================================================================
public: // overridables
==============================================================================*/

#if 0
//------------------------------------------------------------------------------
/*! @brief Maps the given point which is relative to the top left or bottom left
           corner of the item's bounding rectangle to the graphics item local
           coordinate system whose origin is in the center of the item's bounding
           rectangle.

    YScaleAxisOrientation: TopDown

           0         1         2         3         4         5         6
           0123456789012345678901234567890123456789012345678901234567890
         0 +-----------------------------------------------------------+ -10
         1 |      (10/2) => (-20/-8)                                   |  -9
         2 |         X                                                 |  -8
         3 |                                                           |  -7
         4 |                                                           |  -6
         5 |                                                           |  -5
         6 |                                                           |  -4
         7 |                                                           |  -3
         8 |                                                           |  -2
         9 |                       (30/10) => (0/0)                    |  -1
        10 |                             O                             |   0
        11 |                                                           |   1
        12 |                                                           |   2
        13 |                                                           |   3
        14 |                                                           |   4
        15 |                                                           |   5
        16 |                                                           |   6
        17 |                                         (50/18) => (20/7) |   7
        18 |                                                 X         |   8
        19 |                                                           |   9
        20 +-----------------------------------------------------------+  10
           0987654321098765432109876543210123456789012345678901234567890
          -3        -2        -1         0         1         2         3

    YScaleAxisOrientation: BottomUp

           0         1         2         3         4         5         6
           0123456789012345678901234567890123456789012345678901234567890
        20 +-----------------------------------------------------------+ -10
        19 |      (10/18) => (-20/-8)                                  |  -9
        18 |         X                                                 |  -8
        17 |                                                           |  -7
        16 |                                                           |  -6
        15 |                                                           |  -5
        14 |                                                           |  -4
        13 |                                                           |  -3
        12 |                                                           |  -2
        11 |                       (0/0) => (30/10)                    |  -1
        10 |                             O                             |   0
         9 |                                                           |   1
         8 |                                                           |   2
         7 |                                                           |   3
         6 |                                                           |   4
         5 |                                                           |   5
         4 |                                                           |   6
         3 |                                         (50/2) => (20/8)  |   7
         2 |                                                 X         |   8
         1 |                                                           |   9
         0 +-----------------------------------------------------------+  10
           0987654321098765432109876543210123456789012345678901234567890
          -3        -2        -1         0         1         2         3

    @param i_physValPoint [in]
        Point coordinates as physical value with unit which is passed, depending
        on the Y-Axis-Scale-Orientation, either relative to the top left or bottom
        left corner of the item's bounding rectangle.

    @return Point in pixels in the item's local coordinate system.
*/
QPointF CGraphObj::toLocalCoors(const CPhysValPoint& i_physValPoint) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pt {" + i_physValPoint.toString() + "} " + i_physValPoint.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::toLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QPointF pt;
    if (parentGroup() != nullptr) {
        pt = parentGroup()->convert(i_physValPoint, Units.Length.px).toQPointF();       // (10/2)
    }
    else {
        pt = m_pDrawingScene->convert(i_physValPoint, Units.Length.px).toQPointF();     // (10/2)
    }
    QRectF rctBounding = getBoundingRect();                                             // -30, -10, 60, 20
    QPointF ptOrigin;
    if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        ptOrigin = rctBounding.topLeft();                                               // (-30/-10)
    }
    else {
        ptOrigin = rctBounding.bottomLeft();
    }
    pt += ptOrigin;                                                                     // (10/2) + (-30/-10) = (-20/-8)
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Pt {" + qPoint2Str(pt) + "} px");
    }
    return pt;
}

//------------------------------------------------------------------------------
QLineF CGraphObj::toLocalCoors(const CPhysValLine& i_physValLine) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::toLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QLineF lineF;
    if (parentGroup() != nullptr) {
        lineF = parentGroup()->convert(i_physValLine, Units.Length.px).toQLineF();
    }
    else {
        lineF = m_pDrawingScene->convert(i_physValLine, Units.Length.px).toQLineF();
    }
    QRectF rctBounding = getBoundingRect();
    QPointF ptOrigin;
    if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        ptOrigin = rctBounding.topLeft();
    }
    else {
        ptOrigin = rctBounding.bottomLeft();
    }
    lineF.setP1(lineF.p1() + ptOrigin);
    lineF.setP2(lineF.p2() + ptOrigin);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Line {" + qLine2Str(lineF) + "} px");
    }
    return lineF;
}

//------------------------------------------------------------------------------
QRectF CGraphObj::toLocalCoors(const CPhysValRect& i_physValRect) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::toLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QRectF rectF;
    if (parentGroup() != nullptr) {
        rectF = parentGroup()->convert(i_physValRect, Units.Length.px).toQRectF();
    }
    else {
        rectF = m_pDrawingScene->convert(i_physValRect, Units.Length.px).toQRectF();
    }
    QRectF rctBounding = getBoundingRect();
    QPointF ptOrigin;
    if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        ptOrigin = rctBounding.topLeft();
    }
    else {
        ptOrigin = rctBounding.bottomLeft();
    }
    rectF.setTopLeft(rectF.topLeft() + ptOrigin);
    rectF.setBottomRight(rectF.bottomRight() + ptOrigin);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Rect {" + qRect2Str(rectF) + "} px");
    }
    return rectF;
}
#endif

#if 0
//------------------------------------------------------------------------------
/*! @brief Maps the given point from local coordinates given relative to the
           center of the item's bounding rectangle to the physical value,
           depending on the YScaleAxisOrientation either relative to the top left
           or bottom left corner of the item's bounding rectangle.

    YScaleAxisOrientation: TopDown

          -3        -2        -1         0         1         2         3
           0987654321098765432109876543210123456789012345678901234567890
       -10 +-----------------------------------------------------------+  0
        -9 |      (-20/-8) => (10/2)                                   |  1
        -8 |         X                                                 |  2
        -7 |                                                           |  3
        -6 |                                                           |  4
        -5 |                                                           |  5
        -4 |                                                           |  6
        -3 |                                                           |  7
        -2 |                                                           |  8
        -1 |                       (0/0) => (30/10)                    |  9
         0 |                             O                             | 10
         1 |                                                           | 11
         2 |                                                           | 12
         3 |                                                           | 13
         4 |                                                           | 14
         5 |                                                           | 15
         6 |                                                           | 16
         7 |                                         (20/8) => (50/18) | 17
         8 |                                                 X         | 18
         9 |                                                           | 19
        10 +-----------------------------------------------------------+ 20
           0         1         2         3         4         5         6
           0123456789012345678901234567890123456789012345678901234567890

    YScaleAxisOrientation: BottomUp

            0987654321098765432109876543210123456789012345678901234567890
           -3        -2        -1         0         1         2         3
        -10 +-----------------------------------------------------------+ 20
         -9 |      (-20/-8) => (10/18)                                  | 19
         -8 |         X                                                 | 18
         -7 |                                                           | 17
         -6 |                                                           | 16
         -5 |                                                           | 15
         -4 |                                                           | 14
         -3 |                                                           | 13
         -2 |                                                           | 12
         -1 |                       (0/0) => (30/10)                    | 11
          0 |                             O                             | 10
          1 |                                                           |  9
          2 |                                                           |  8
          3 |                                                           |  7
          4 |                                                           |  6
          5 |                                                           |  5
          6 |                                                           |  4
          7 |                                         (20/8) => (50/2)  |  3
          8 |                                                 X         |  2
          9 |                                                           |  1
         10 +-----------------------------------------------------------+  0
            0         1         2         3         4         5         6
            0123456789012345678901234567890123456789012345678901234567890

    @param i_physValPoint [in]
        Point coordinates as physical value with unit which is passed, depending
        on the Y-Axis-Scale-Orientation, either relative to the top left or bottom
        left corner of the item's bounding rectangle.

    @return Point in pixels in the item's local coordinate system.
*/
CPhysValPoint CGraphObj::fromLocalCoors(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pt {" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::fromLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QRectF rctBounding = getBoundingRect();     // -30, -10, 60, 20
    QPointF ptOrigin = rctBounding.topLeft();   // (-30/-10)
    QPointF pt = i_pt - ptOrigin;               // (-20/-8) - (-30/-10) = (10/2)
    //QPointF ptOrigin;
    //if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
    //    ptOrigin = rctBounding.topLeft();     // (-30/-10)
    //    pt = i_pt - ptOrigin;                 // (-20/-8) - (-30/-10) = (10/2)
    //}
    //else {
    //    ptOrigin = rctBounding.bottomLeft();  // (-30/10)
    //    pt.setX(i_pt.x() - ptOrigin.x());     // x: (-20) - (-30) = (10)
    //    pt.setY(ptOrigin.y() - i_pt.y());     // y: (10) - (-8) = (18)
    //}                                         // Pt: (-30/-10) => (10/18)

    // When converting the pixel coordinates into physical values the parent group
    // or the drawing scene will take the Y-Axis-Scale Orientation into account.
    CPhysValPoint physValPoint;
    if (parentGroup() != nullptr) {                 // TopDown          | BottomUp
        physValPoint = parentGroup()->convert(pt);  // (10/2) => (10/2) | (10/2) => (10/18)
    }
    else {
        physValPoint = m_pDrawingScene->convert(pt);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Pt {" + physValPoint.toString() + "} " + physValPoint.unit().symbol());
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
/*! @brief Maps the given line from local coordinates given relative to the
           center of the item's bounding rectangle to the physical line,
           depending on the YScaleAxisOrientation either relative to the top left
           or bottom left corner of the item's bounding rectangle.

    @see fromLocalCoors(const QPointF&)
*/
CPhysValLine CGraphObj::fromLocalCoors(const QLineF& i_line) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + qLine2Str(i_line) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::fromLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QRectF rctBounding = getBoundingRect();
    QPointF ptOrigin;
    if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        ptOrigin = rctBounding.topLeft();
    }
    else {
        ptOrigin = rctBounding.bottomLeft();
    }
    QLineF lineF(i_line.p1() - ptOrigin, i_line.p2() - ptOrigin);
    CPhysValLine physValLine;
    if (parentGroup() != nullptr) {
        physValLine = parentGroup()->convert(lineF);
    }
    else {
        physValLine = m_pDrawingScene->convert(lineF);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Line {" + physValLine.toString() + "} " + physValLine.unit().symbol());
    }
    return physValLine;
}

//------------------------------------------------------------------------------
/*! @brief Maps the given rectangle from local coordinates given relative to the
           center of the item's bounding rectangle to the physical rectangle,
           depending on the YScaleAxisOrientation either relative to the top left
           or bottom left corner of the item's bounding rectangle.

    @see fromLocalCoors(const QPointF&)
*/
CPhysValRect CGraphObj::fromLocalCoors(const QRectF& i_rect) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::fromLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QRectF rctBounding = getBoundingRect();
    QPointF ptOrigin;
    if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
        ptOrigin = rctBounding.topLeft();
    }
    else {
        ptOrigin = rctBounding.bottomLeft();
    }
    QRectF rectF(i_rect.topLeft() - ptOrigin, i_rect.bottomRight() - ptOrigin);
    CPhysValRect physValRect;
    if (parentGroup() != nullptr) {
        physValRect = parentGroup()->convert(rectF);
    }
    else {
        physValRect = m_pDrawingScene->convert(rectF);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Rect {" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}
#endif

/*==============================================================================
public: // overridables
==============================================================================*/

#if 0
//------------------------------------------------------------------------------
CPhysValPoint CGraphObj::mapToScene(const CPhysValPoint& i_physValPoint) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValPoint, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical point, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the parent's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValPoint
        Point to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Point in scene coordinates in the desired unit.
*/
CPhysValPoint CGraphObj::mapToScene(const CPhysValPoint& i_physValPoint, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pt {" + i_physValPoint.toString() + "} " + i_physValPoint.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::mapToScene",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QPointF pt = toLocalCoors(i_physValPoint);
    pt = pGraphicsItemThis->mapToScene(pt);
    CPhysValPoint physValPoint = m_pDrawingScene->convert(pt);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Pt {" + physValPoint.toString() + "} " + physValPoint.unit().symbol());
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
CPhysValLine CGraphObj::mapToScene(const CPhysValLine& i_physValLine) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValLine, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical line, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the item's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValLine
        Line to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Line in scene coordinates in the desired unit.
*/
CPhysValLine CGraphObj::mapToScene(const CPhysValLine& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::mapToScene",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QLineF lineF = toLocalCoors(i_physValLine);
    lineF.setP1(pGraphicsItemThis->mapToScene(lineF.p1()));
    lineF.setP2(pGraphicsItemThis->mapToScene(lineF.p2()));
    CPhysValLine physValLine = m_pDrawingScene->convert(lineF);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Line {" + physValLine.toString() + "} " + physValLine.unit().symbol());
    }
    return physValLine;
}

//------------------------------------------------------------------------------
CPhysValRect CGraphObj::mapToScene(const CPhysValRect& i_physValRect) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValRect, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical point, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the item's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValPoint
        Point to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Point in scene coordinates in the desired unit.
*/
CPhysValRect CGraphObj::mapToScene(const CPhysValRect& i_physValRect, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::mapToScene",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QRectF rectF = toLocalCoors(i_physValRect);
    rectF = pGraphicsItemThis->mapRectToScene(rectF);
    CPhysValRect physValRect = m_pDrawingScene->convert(rectF);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Rect {" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}
#endif

/*==============================================================================
public: // overridables
==============================================================================*/

#if 0
//------------------------------------------------------------------------------
CPhysValPoint CGraphObj::mapToParent(const CPhysValPoint& i_physValPoint) const
//------------------------------------------------------------------------------
{
    return mapToParent(i_physValPoint, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical point, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the item's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValPoint
        Point to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Point in scene coordinates in the desired unit.
*/
CPhysValPoint CGraphObj::mapToParent(const CPhysValPoint& i_physValPoint, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pt {" + i_physValPoint.toString() + "} " + i_physValPoint.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::mapToParent",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjGroup* pGraphObjGroupParent = parentGroup();
    CPhysValPoint physValPoint;
    if (pGraphObjGroupParent == nullptr) {
        physValPoint = mapToScene(i_physValPoint, i_unitDst);
    }
    else {
        QPointF pt = toLocalCoors(i_physValPoint);
        pt = pGraphicsItemThis->mapToParent(pt);
        physValPoint = pGraphObjGroupParent->fromLocalCoors(pt);
        physValPoint = pGraphObjGroupParent->convert(physValPoint);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Pt {" + physValPoint.toString() + "} " + physValPoint.unit().symbol());
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
CPhysValLine CGraphObj::mapToParent(const CPhysValLine& i_physValLine) const
//------------------------------------------------------------------------------
{
    return mapToParent(i_physValLine, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical line, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the item's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValLine
        Line to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Line in scene coordinates in the desired unit.
*/
CPhysValLine CGraphObj::mapToParent(const CPhysValLine& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::mapToParent",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjGroup* pGraphObjGroupParent = parentGroup();
    CPhysValLine physValLine;
    if (pGraphObjGroupParent == nullptr) {
        physValLine = mapToScene(i_physValLine, i_unitDst);
    }
    else {
        QLineF lineF = toLocalCoors(i_physValLine);
        lineF.setP1(pGraphicsItemThis->mapToParent(lineF.p1()));
        lineF.setP2(pGraphicsItemThis->mapToParent(lineF.p2()));
        physValLine = pGraphObjGroupParent->fromLocalCoors(lineF);
        physValLine = pGraphObjGroupParent->convert(physValLine);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Line {" + physValLine.toString() + "} " + physValLine.unit().symbol());
    }
    return physValLine;
}

//------------------------------------------------------------------------------
CPhysValRect CGraphObj::mapToParent(const CPhysValRect& i_physValRect) const
//------------------------------------------------------------------------------
{
    return mapToParent(i_physValRect, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical point, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the item's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValPoint
        Point to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Point in scene coordinates in the desired unit.
*/
CPhysValRect CGraphObj::mapToParent(const CPhysValRect& i_physValRect, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::mapToParent",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjGroup* pGraphObjGroupParent = parentGroup();
    CPhysValRect physValRect;
    if (pGraphObjGroupParent == nullptr) {
        physValRect = mapToScene(i_physValRect, i_unitDst);
    }
    else {
        QRectF rectF = toLocalCoors(i_physValRect);
        rectF = pGraphicsItemThis->mapRectToParent(rectF);
        physValRect = pGraphObjGroupParent->fromLocalCoors(rectF);
        physValRect = pGraphObjGroupParent->convert(physValRect);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Rect {" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}
#endif

/*==============================================================================
public: // must overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Pure virtual method which must be overridden by derived classes to
           return the current bounding rectangle of the object in local coordinates.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObj::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::getBoundingRect",
        /* strAddInfo   */ "" );

#pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");

    QRectF rctBounding;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the effective (resulting) bounding rectangle of this item
           on the drawing scene.

    To get the effective bounding rectangle the left most, the right most
    as well as the top most and bottom most shape points of the transformed
    (rotated and scaled) object are are taken into account.

    If the object is rotated the effective bounding rectangle is not the
    bounding rectangle (in item coordinates) mapped to the scene.
    Before mapping the points to the scene the TopMost, BottomMost, LeftMost
    and RightMost points of the rotated object have to be calculated and each
    point has to be mapped to the scene.

    E.g. rotated trapez on the scene:

                             + TopMost
                            / \
                           /   \
                          /     \
                LeftMost +       + RightMost
                          \     /
                           \   /
                            \ /
                             + BottomMost
*/
QRectF CGraphObj::getEffectiveBoundingRectOnScene() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::getEffectiveBoundingRectOnScene",
        /* strAddInfo   */ "" );

#pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");

    QRectF rctBounding;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotated, physical bounding rectangle.
*/
CPhysValRect CGraphObj::getPhysValBoundingRect() const
//------------------------------------------------------------------------------
{
    return getPhysValBoundingRect(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotated, physical bounding rectangle.
*/
CPhysValRect CGraphObj::getPhysValBoundingRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::getPhysValBoundingRect",
        /* strAddInfo   */ strMthInArgs );

#pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");

    CPhysValRect physValRectBounding(*m_pDrawingScene, getBoundingRect());
    if (parentGroup() != nullptr) {
        physValRectBounding = parentGroup()->convert(physValRectBounding, i_unit);
    }
    else {
        physValRectBounding = m_pDrawingScene->convert(physValRectBounding, i_unit);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValRectBounding.toString(true) + "}");
    }
    return physValRectBounding;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the item's bounding rectangle to the given position.

    The position must be passed, depending on the y-Axis-Scale-Orientation, either
    relative to the top left or bottom left corner of the parent group or scene,
    if the item has no parent group.

    @param [in] i_physValPos
        New position of the item's bounding rectangle center point.
*/
void CGraphObj::setPosition(const CPhysValPoint& i_physValPos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pos {" + i_physValPos.toString() + "} " + i_physValPos.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setPosition",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptCenter;
    if (parentGroup() != nullptr) {
        ptCenter = parentGroup()->convert(i_physValPos, Units.Length.px).toQPointF();
        ptCenter = mapFromTopLeftOfBoundingRect(ptCenter);
    }
    else {
        ptCenter = m_pDrawingScene->convert(i_physValPos, Units.Length.px).toQPointF();
    }
    QGraphicsItem_setPos(ptCenter);
}

//------------------------------------------------------------------------------
/*! @brief Returns the center point of the item's bounding rectangle in the
           unit of the drawing scene.
*/
CPhysValPoint CGraphObj::position() const
//------------------------------------------------------------------------------
{
    return position(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Returns the center point of the item's bounding rectangle in the
           desired unit.
*/
CPhysValPoint CGraphObj::position(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPointF ptPos = pGraphicsItemThis->pos();
    CPhysValPoint physValPos(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        ptPos = parentGroup()->mapToTopLeftOfBoundingRect(ptPos);
        physValPos = parentGroup()->convert(ptPos, i_unit);
    }
    else {
        physValPos = m_pDrawingScene->convert(ptPos, i_unit);
    }
    return physValPos;
}

////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to set
//           the width of the item.
//
//    @param [in] i_physValWidth
//        New width of the object.
//*/
//void CGraphObj::setLength( const CPhysVal& i_physValLength )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValLength.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::setLength",
//        /* strAddInfo   */ strMthInArgs );
//
//    #pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//}
//
////------------------------------------------------------------------------------
///*! @brief Returns the length of the item in the unit of the drawing scene.
//
//    The default implementation returns the width of the item's bounding rectangle.
//*/
//CPhysVal CGraphObj::length() const
////------------------------------------------------------------------------------
//{
//    return length(m_pDrawingScene->drawingSize().unit());
//}
//
////------------------------------------------------------------------------------
///*! @brief Returns the length of the item in the desired unit.
//
//    The default implementation returns the width of the item's bounding rectangle.
//*/
//CPhysVal CGraphObj::length(const CUnit& i_unit) const
////------------------------------------------------------------------------------
//{
//    return width(i_unit);
//}
//
////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to set
//           the width of the item.
//
//    @param [in] i_physValWidth
//        New width of the object.
//*/
//void CGraphObj::setWidth( const CPhysVal& i_physValWidth )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValWidth.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::setWidth",
//        /* strAddInfo   */ strMthInArgs );
//
//    #pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//}
//
////------------------------------------------------------------------------------
///*! @brief Returns the width of the item in the unit of the drawing scene.
//
//    The default implementation returns the width of the item's bounding rectangle.
//*/
//CPhysVal CGraphObj::width() const
////------------------------------------------------------------------------------
//{
//    return width(m_pDrawingScene->drawingSize().unit());
//}
//
////------------------------------------------------------------------------------
///*! @brief Returns the width of the item in the desired unit.
//
//    The default implementation returns the width of the item's bounding rectangle.
//*/
//CPhysVal CGraphObj::width(const CUnit& i_unit) const
////------------------------------------------------------------------------------
//{
//    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
//    double fWidth_px = getBoundingRect().width();
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//    CPhysVal physValWidth(fWidth_px, Units.Length.px, drawingSize.imageCoorsResolutionInPx());
//    // A value like width and height can be directly converted without the drawing size.
//    physValWidth.convertValue(i_unit);
//    return physValWidth;
//}
//
////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived class to set
//           the height of the item.
//
//    @param [in] i_physValWidth
//        New height of the i_physValHeight.
//*/
//void CGraphObj::setHeight( const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::setHeight",
//        /* strAddInfo   */ strMthInArgs );
//
//    #pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem == nullptr) {
//        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
//    }
//}
//
////------------------------------------------------------------------------------
///*! @brief Returns the height of the item in the unit of the drawing scene.
//
//    The default implementation returns the height of the item's bounding rectangle.
//*/
//CPhysVal CGraphObj::height() const
////------------------------------------------------------------------------------
//{
//    return height(m_pDrawingScene->drawingSize().unit());
//}
//
////------------------------------------------------------------------------------
///*! @brief Returns the height of the item in the desired unit.
//
//    The default implementation returns the height of the item's bounding rectangle.
//*/
//CPhysVal CGraphObj::height(const CUnit& i_unit) const
////------------------------------------------------------------------------------
//{
//    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
//    double fHeight_px = getBoundingRect().height();
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//    CPhysVal physValHeight(fHeight_px, Units.Length.px, drawingSize.imageCoorsResolutionInPx());
//    // A value like width and height can be directly converted without the drawing size.
//    physValHeight.convertValue(i_unit);
//    return physValHeight;
//}

////------------------------------------------------------------------------------
//void CGraphObj::setGroupScale(double i_fXScale, double i_fYScale)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = QString::number(i_fXScale) + ", " + QString::number(i_fYScale);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::setGroupScale",
//        /* strAddInfo   */ strMthInArgs );
//    #pragma message(__TODO__"Pure virtual")
//    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
//    m_transformByGroup.scale(i_fXScale, i_fYScale);
//    QGraphicsItem_setScale();
//}

//------------------------------------------------------------------------------
/*! @brief Overloaded method to set the clockwise rotation angle, in degrees,
           around the Z axis.

    @param [in] i_fAngle_degree
        Rotation angle in degree.
*/
void CGraphObj::setRotationAngle(double i_fAngle_degree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fAngle_degree, 'f', 3);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setRotationAngle",
        /* strAddInfo   */ strMthInArgs );

    setRotationAngle(CPhysVal(i_fAngle_degree, Units.Angle.Degree, 0.1));
}

//------------------------------------------------------------------------------
/*! @brief Sets the clockwise rotation angle.

    The default value is 0 which is at 3o'clock (the item is not rotated).
    Assigning a negative value will rotate the item counter-clockwise.
    Normally the rotation angle is in the range (-360, 360), but it's also possible
    to assign values outside of this range (e.g., a rotation of 370 degrees is the
    same as a rotation of 10 degrees).

    @param [in] i_physValAngle
        Angle to be set.
*/
void CGraphObj::setRotationAngle(const CPhysVal& i_physValAngle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValAngle.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setRotationAngle",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

#pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    if (m_physValRotationAngle != i_physValAngle) {
        {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);
            m_physValRotationAngle = i_physValAngle;
            QGraphicsItem_setRotation(m_physValRotationAngle.getVal(Units.Angle.Degree));
        }
        emit_geometryOnSceneChanged();
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotation angle of this item in Degree.
*/
CPhysVal CGraphObj::rotationAngle() const
//------------------------------------------------------------------------------
{
    return rotationAngle(Units.Angle.Degree);
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotation angle of this item in the desired unit.

    @param [in] i_unit Desired unit which could be either Degree or Rad.
*/
CPhysVal CGraphObj::rotationAngle(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysVal physValAngle = m_physValRotationAngle;
    physValAngle.convertValue(i_unit);
    return physValAngle;
}

//------------------------------------------------------------------------------
/*! @brief Returns the effective (resulting) rotation angle of this item on the
           drawing scene in Degree.

    To get the effective rotation angle on the scene the rotation angle of all
    parent groups of this item will be added to the the rotation angle of this item.
*/
CPhysVal CGraphObj::getEffectiveRotationAngleOnScene() const
//------------------------------------------------------------------------------
{
    return getEffectiveRotationAngleOnScene(Units.Angle.Degree);
}

//------------------------------------------------------------------------------
/*! @brief Returns the effective (resulting) rotation angle of this item on the
           drawing scene in the desired unit.

    To get the effective rotation angle on the scene the rotation angle of all
    parent groups of this item will be added to the the rotation angle of this item.

    @param [in] i_unit Desired unit which could be either Degree or Rad.
*/
CPhysVal CGraphObj::getEffectiveRotationAngleOnScene(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysVal physValRotationAngle = m_physValRotationAngle;
    if (parentGroup() != nullptr)
    {
        physValRotationAngle += parentGroup()->getEffectiveRotationAngleOnScene(i_unit);
    }
    physValRotationAngle.convertValue(i_unit);
    return physValRotationAngle;
}

/*==============================================================================
public: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObj::setScaleFacX( double i_fScaleFacX )
////------------------------------------------------------------------------------
//{
//    m_fScaleFacXCurr = i_fScaleFacX;
//
//    updateTransform();
//
//} // setScaleFacX

////------------------------------------------------------------------------------
//void CGraphObj::setScaleFacY( double i_fScaleFacY )
////------------------------------------------------------------------------------
//{
//    m_fScaleFacYCurr = i_fScaleFacY;
//
//    updateTransform();
//
//} // setScaleFacY

////------------------------------------------------------------------------------
//void CGraphObj::setScaleFactors( double i_fScaleFacX, double i_fScaleFacY )
////------------------------------------------------------------------------------
//{
//    m_fScaleFacXCurr = i_fScaleFacX;
//    m_fScaleFacYCurr = i_fScaleFacY;
//
//    updateTransform();
//
//} // setScaleFactors

////------------------------------------------------------------------------------
//double CGraphObj::getScaleFacX( ECoordinatesVersion i_version )
////------------------------------------------------------------------------------
//{
//    double fScaleFacX = 0.0;
//
//    if( i_version == ECoordinatesVersion::Original )
//    {
//        fScaleFacX = m_fScaleFacXOrig;
//    }
//    else
//    {
//        fScaleFacX = m_fScaleFacXCurr;
//    }
//    return fScaleFacX;
//
//} // getScaleFacX

////------------------------------------------------------------------------------
//double CGraphObj::getScaleFacY( ECoordinatesVersion i_version )
////------------------------------------------------------------------------------
//{
//    double fScaleFacY = 0.0;
//
//    if( i_version == ECoordinatesVersion::Original )
//    {
//        fScaleFacY = m_fScaleFacYOrig;
//    }
//    else
//    {
//        fScaleFacY = m_fScaleFacYCurr;
//    }
//    return fScaleFacY;
//
//} // getScaleFacY

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the current edit mode of the object.

    The edit mode defines how mouse and key events are handled.
*/
void CGraphObj::setEditMode(const CEnumEditMode& i_eMode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eMode.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setEditMode",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }
    if (m_editMode != i_eMode) {
        m_editMode = i_eMode;
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        if (m_editMode == EEditMode::CreatingByMouseEvents) {
            // Immeadiately select the object to create the selection points.
            // Following mouse move press, mouse move and mouse release events will
            // be forwarded by the scene to the selection point responsible for
            // resizing the obejct (the top most selection point most recently created).
            pGraphicsItemThis->setSelected(true);
            // The object is under construction. Hover events will not be accepted.
            // Only when no drawing tool is selected in the drawing scene hover
            // events may be accepted.
            pGraphicsItemThis->setAcceptHoverEvents(false);
        }
        else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
            // The "itemChange" method will be blocked as that would create also
            // selection points at the bounding rectangle. But in this edit mode only
            // the selection points for the polygon points should be created.
            CRefCountGuard refCountGuardItemChange(&m_iItemChangeBlockedCounter);
            hideSelectionPoints(c_uSelectionPointsBoundingRectAll|c_uSelectionPointsLineCenter);
            showSelectionPoints(c_uSelectionPointsPolygonPoints);
            if (!pGraphicsItemThis->isSelected()) {
                pGraphicsItemThis->setSelected(true);
                emit_selectedChanged(true);
            }
        }
        else {
            pGraphicsItemThis->setAcceptHoverEvents(true);
        }
        emit_editModeChanged(m_editMode);
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the current edit mode of the object.

    The edit mode defines how mouse and key events are handled.
*/
CEnumEditMode CGraphObj::editMode() const
//------------------------------------------------------------------------------
{
    return m_editMode;
}

////------------------------------------------------------------------------------
//void CGraphObj::setEditResizeMode( EEditResizeMode i_editResizeMode )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = CEnumEditResizeMode(i_editResizeMode).toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::setEditResizeMode",
//        /* strAddInfo   */ strMthInArgs );
//
//    m_editResizeMode = i_editResizeMode;
//}

//------------------------------------------------------------------------------
/*! @brief Returns the current edit mode of the object.

    If the object is currently being resized (editMode == Resize) the resize mode
    defines how the object will be resized.
*/
//CEnumEditResizeMode CGraphObj::editResizeMode() const
////------------------------------------------------------------------------------
//{
//    return m_editResizeMode;
//}

/*==============================================================================
public: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObj::setIsHit(bool i_bIsHit)
////-----------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = bool2Str(i_bIsHit);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::setIsHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_bIsHit != i_bIsHit) {
//        m_bIsHit = i_bIsHit;
//        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
//        if (pGraphicsItemThis != nullptr) {
//            pGraphicsItemThis->update();
//        }
//    }
//}

////------------------------------------------------------------------------------
//bool CGraphObj::isHit() const
////------------------------------------------------------------------------------
//{
//    return m_bIsHit;
//}

////------------------------------------------------------------------------------
//bool CGraphObj::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Point:" + point2Str(i_pt) +
//            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo));
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bIsHit = false;
//
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr) {
//        if (pGraphicsItem->isSelected()) {
//            bIsHit = isPolygonSelectionPointHit(i_pt,o_pHitInfo);
//            if (!bIsHit) {
//                bIsHit = isBoundingRectSelectionPointHit(
//                    /* pt               */ i_pt,
//                    /* iSelPtsCount     */ -1,
//                    /* pSelPts          */ nullptr,
//                    /* pGraphObjHitInfo */ o_pHitInfo );
//            }
//        }
//        if (!bIsHit) {
//            if (pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern) {
//                bIsHit = pGraphicsItem->contains(i_pt);
//                if (o_pHitInfo != nullptr) {
//                    //o_pHitInfo->m_editMode = EEditMode::Move;
//                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//                    o_pHitInfo->m_idxPolygonShapePoint = -1;
//                    o_pHitInfo->m_idxLineSegment = -1;
//                    o_pHitInfo->m_ptHit = i_pt;
//                }
//            }
//        }
//        if (bIsHit && o_pHitInfo != nullptr) {
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            o_pHitInfo->setCursor( Math::degree2Rad(m_fRotAngleCurr_deg) );
//#endif
//        }
//    }
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        QString strMthOutArgs;
//        if (o_pHitInfo != nullptr) {
//            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
//            mthTracer.setMethodOutArgs(strMthOutArgs);
//        }
//        mthTracer.setMethodReturn(bIsHit);
//    }
//    return bIsHit;
//}

//------------------------------------------------------------------------------
void CGraphObj::setIsHighlighted(bool i_bIsHighlighted)
//-----------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bIsHighlighted);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setIsHighlighted",
        /* strAddInfo   */ strMthInArgs );

    if (m_bIsHighlighted != i_bIsHighlighted) {
        m_bIsHighlighted = i_bIsHighlighted;
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        if (pGraphicsItemThis != nullptr) {
            m_pDrawingScene->update();
            pGraphicsItemThis->update(pGraphicsItemThis->boundingRect());
        }
        if (m_bIsHighlighted) {
            tracePositionInfo(mthTracer);
        }
    }
}

//------------------------------------------------------------------------------
bool CGraphObj::isHighlighted() const
//------------------------------------------------------------------------------
{
    return m_bIsHighlighted;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the proposed cursor shape for the given point.

    The cursor shape depends which shape point of the object has been hit.
    If a selection point has been hit, the position and type of selection point
    defines the cursor shape.

    The base implementation just checks whether a selection point has been hit
    and returns the cursor shape proposed by the selection point.

    @param i_pt [in] Point to be check in local coordinates.
*/
QCursor CGraphObj::getProposedCursor(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point {" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::getProposedCursor",
        /* strAddInfo   */ strMthInArgs );

    QCursor cursor = Qt::ArrowCursor;
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        CGraphObjSelectionPoint* pGraphObjSelPtHit = getSelectionPointHit(i_pt);
        if (pGraphObjSelPtHit != nullptr) {
            cursor = pGraphObjSelPtHit->getProposedCursor(i_pt);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qCursorShape2Str(cursor.shape()));
    }
    return cursor;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Brings the object into front of all objects intersecting the bounding
           rectangle of this object.

    The method does not set the member "m_fZValue" as this member is used to
    restore the stacking order on deselecting the object.

    @return The new ZValue.
*/
double CGraphObj::bringToFront()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::bringToFront",
        /* strAddInfo   */ "" );

    double fZValue = 0.0;
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QRectF rctBounding = pGraphicsItem->boundingRect();
        rctBounding = pGraphicsItem->mapRectToScene(rctBounding);
        QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rctBounding);
        m_pDrawingScene->bringToFront(pGraphicsItem, arpGraphicsItemsIntersected);
        fZValue = pGraphicsItem->zValue();
        m_arfZValues[static_cast<int>(ERowVersion::Current)] = fZValue;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString::number(fZValue));
    }
    return fZValue;
}

//------------------------------------------------------------------------------
/*! @brief Sets the stacking order value (zValue) of the graphical object.

    You must call this method instead of setZValue of QGraphicsItem.

    @param [in] i_fZValue
    @param [in] i_version
         Two values are stored. The original version is the ZValue which
         will be initially used and if resetStackingOrderValueToOriginalValue
         is called. The current version is the ZValue which is currently set
         at the graphics item. The current and original versions are mainly
         used to temporarily modify the ZValue to bring the object in front
         and back again.

    @return zValue which was set before at the graphics item
*/
double CGraphObj::setStackingOrderValue(double i_fZValue, ERowVersion i_version)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fZValue, 'f', 1) + ", " + CEnumRowVersion(i_version).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setStackingOrderValue",
        /* strAddInfo   */ strMthInArgs );

    double fZValuePrev = 0.0;
    m_arfZValues[static_cast<int>(i_version)] = i_fZValue;
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        fZValuePrev = pGraphicsItem->zValue();
        pGraphicsItem->setZValue(i_fZValue);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fZValuePrev, 'f', 1));
    }
    return fZValuePrev;
}

//------------------------------------------------------------------------------
double CGraphObj::getStackingOrderValue(ERowVersion i_version) const
//------------------------------------------------------------------------------
{
    return m_arfZValues[static_cast<int>(i_version)];
}

//------------------------------------------------------------------------------
/*! @brief Sets the stacking order value (zValue) of the graphical object
           to the original value.

    Two values are stored. The original version is the ZValue which
    will be initially used and if resetStackingOrderValueToOriginalValue
    is called. The current version is the ZValue which is currently set
    at the graphics item. The current and original versions are mainly
    used to temporarily modify the ZValue to bring the object in front
    and back again.

    @return zValue which was set before at the graphics item
*/
double CGraphObj::resetStackingOrderValueToOriginalValue()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Orig: " + QString::number(m_arfZValues[static_cast<int>(ERowVersion::Original)], 'f', 1);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::resetStackingOrderValueToOriginalValue",
        /* strAddInfo   */ strMthInArgs );

    double fZValuePrev = 0.0;
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        fZValuePrev = pGraphicsItem->zValue();
        m_arfZValues[static_cast<int>(ERowVersion::Current)] =
            m_arfZValues[static_cast<int>(ERowVersion::Original)];
        pGraphicsItem->setZValue(m_arfZValues[static_cast<int>(ERowVersion::Original)]);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fZValuePrev, 'f', 1));
    }
    return fZValuePrev;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the selection point object which is under the given point.

    @return Selection point hit or nullptr, if no selection point is under the given point.
*/
CGraphObjSelectionPoint* CGraphObj::getSelectionPointHit(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    CGraphObjSelectionPoint* pGraphObjSelPtHit = nullptr;
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsPolygon) {
            if (pGraphObjSelPt != nullptr) {
                QPointF ptTmp = pGraphicsItemThis->mapToItem(pGraphObjSelPt, i_pt);
                if (pGraphObjSelPt->contains(ptTmp)) {
                    pGraphObjSelPtHit = pGraphObjSelPt;
                    break;
                }
            }
        }
        if (pGraphObjSelPtHit == nullptr) {
            for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsBoundingRect) {
                if (pGraphObjSelPt != nullptr) {
                    QPointF ptTmp = pGraphicsItemThis->mapToItem(pGraphObjSelPt, i_pt);
                    if (pGraphObjSelPt->contains(ptTmp)) {
                        pGraphObjSelPtHit = pGraphObjSelPt;
                        break;
                    }
                }
            }
        }
    }
    return pGraphObjSelPtHit;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the position of the given selection point in parent coordinates
           in the unit of the drawing scene. If the item does not have a parent
           the returned position is in scence coordinates.
*/
CPhysValPoint CGraphObj::getPositionOfSelectionPoint(
    ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    return getPositionOfSelectionPoint(i_selPtType, i_selPt, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Returns the position of the given selection point in parent coordinates
           in the given unit. If the item does not have a parent the returned
           position is in scence coordinates.
*/
CPhysValPoint CGraphObj::getPositionOfSelectionPoint(
    ESelectionPointType i_selPtType, ESelectionPoint i_selPt, const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::BoundingRectangle) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QRectF rctBounding = getBoundingRect();
    QPointF ptPos = ZS::Draw::getSelectionPointCoors(rctBounding, i_selPt);
    CPhysValPoint physValPos(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        ptPos = pGraphicsItemThis->mapToParent(ptPos);
        ptPos = parentGroup()->mapToTopLeftOfBoundingRect(ptPos);
        physValPos = parentGroup()->convert(ptPos, i_unit);
    }
    else {
        ptPos = pGraphicsItemThis->mapToScene(ptPos);
        physValPos = m_pDrawingScene->convert(ptPos, i_unit);
    }
    return physValPos;
    //CPhysValRect physValRctBounding = getPhysValBoundingRect(i_unit);
    //CPhysValPoint physValPoint(*m_pDrawingScene);
    //if (i_selPt == ESelectionPoint::RotateTop) {
    //    physValPoint = physValRctBounding.topCenter();
    //    CPhysVal physValDistance(getSelectionPointRotateDistance(), Units.Length.px);
    //    physValPoint.setY(physValPoint.y().getVal() - physValDistance.getVal(i_unit));
    //}
    //else if (i_selPt == ESelectionPoint::RotateBottom) {
    //    physValPoint = physValRctBounding.bottomCenter();
    //    CPhysVal physValDistance(getSelectionPointRotateDistance(), Units.Length.px);
    //    physValPoint.setY(physValPoint.y().getVal() - physValDistance.getVal(i_unit));
    //}
    //else {
    //    physValPoint = physValRctBounding.selectionPoint(i_selPt);
    //}
    //return physValPoint;
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObj::getPositionOfSelectionPoint(
    ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    return getPositionOfSelectionPoint(i_selPtType, i_idxPt, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObj::getPositionOfSelectionPoint(
    ESelectionPointType i_selPtType, int i_idxPt, const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::PolygonPoint && i_selPtType != ESelectionPointType::LineCenterPoint) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
#pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
}

//------------------------------------------------------------------------------
/*! @brief Returns the coordinates of the selection point in scene coordinates.
*/
QPointF CGraphObj::getPositionOfSelectionPointInSceneCoors(
    ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::BoundingRectangle) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
    QPointF ptScenePos;
    QRectF rctBounding = getBoundingRect();
    QPointF ptPos = ZS::Draw::getSelectionPointCoors(rctBounding, i_selPt);
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        ptScenePos = pGraphicsItem->mapToScene(ptPos);
    }
    return ptScenePos;
    //CPhysValRect physValRctBounding = getPhysValBoundingRect(Units.Length.px);
    //CPhysValPoint physValPoint = physValRctBounding.selectionPoint(i_selPt);
    //if (parentGroup() != nullptr) {
    //    physValPoint = parentGroup()->mapToScene(physValPoint);
    //}
    //return physValPoint.toQPointF();
}

//------------------------------------------------------------------------------
/*! @brief Returns the coordinates of the selection point in scene coordinates.

    For polygon shapes the method got to be overridden and should return
    the corresponding coordinates of the polygon point.
    The default implementation returns the center of the bounding rectangle.
*/
QPointF CGraphObj::getPositionOfSelectionPointInSceneCoors(
    ESelectionPointType i_selPtType, int i_idxPt ) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::PolygonPoint && i_selPtType != ESelectionPointType::LineCenterPoint) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
#pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates in length and angle of the given point
           to the selection point of the graphical object.

    How the angle of anchor lines to selection points is interpreted depends on
    the graphical object type and the selection point.

    This method must be reimplemented by derived classes to ensure that labels
    linked to the selection point are always at the same relative position
    no matter how the object will be rotated.

    The default implementation calculates the polar coordinates for selection points
    on the bounding rectangle of the item if the center point and all selection points
    on the bounding rectangle are allowed.

    @note This method is used to keep the relative position of labels to the
          graphical object they are linked to if the linked object is resized,
          rotated or moved.

    Example:

                       + Pt
                      / Calculated Angle: 60°
                     /           TC
                 TL +------------x------------+ TR
                    |                         |
                    |         Center          |
                 LC x            x            x RC
                    |                         |
                    |                         |
                 BL +------------x------------+ BR
                                 BC

          For example if the item would be rotated by 90°:
                                RC
                      TR +-------x--------+ BR
                         |                |
                         |                |
                         |                |
                         |    Center      |
                      TC x       x        x BC
                         |                |
                         |                |
                Pt + 60° |                |
                     \   |                |
                      TL +-------x--------+ BL
                                 LC

    @param [in] i_pt
        Point in scene coordinates to which the polar coordinates from the
        given selection point have to be returned.
    @param [in] i_selPt
        Selection point on the bounding rectangle.
*/
SPolarCoors CGraphObj::getPolarCoorsToSelectionPointFromSceneCoors(
    const QPointF& i_pt, ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::BoundingRectangle) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
    CPhysValRect physValRctBounding = getPhysValBoundingRect(Units.Length.px);
    CPhysValPoint physValPointP1 = physValRctBounding.selectionPoint(i_selPt);
    CPhysValPoint physValPointP2(*m_pDrawingScene);
    if (i_selPt == ESelectionPoint::TopLeft) {
        physValPointP2 = physValRctBounding.topRight();
    }
    else if (i_selPt == ESelectionPoint::TopRight) {
        physValPointP2 = physValRctBounding.topLeft();
    }
    else if (i_selPt == ESelectionPoint::BottomRight) {
        physValPointP2 = physValRctBounding.bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::BottomLeft) {
        physValPointP2 = physValRctBounding.bottomRight();
    }
    else if (i_selPt == ESelectionPoint::TopCenter) {
        physValPointP2 = physValRctBounding.topRight();
    }
    else if (i_selPt == ESelectionPoint::RightCenter) {
        physValPointP2 = physValRctBounding.bottomRight();
    }
    else if (i_selPt == ESelectionPoint::BottomCenter) {
        physValPointP2 = physValRctBounding.bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::LeftCenter) {
        physValPointP2 = physValRctBounding.topLeft();
    }
    else /*if (i_selPt == ESelectionPoint::Center)*/ {
        physValPointP2 = physValRctBounding.rightCenter();
    }
    if (parentGroup() != nullptr) {
        physValPointP1 = parentGroup()->mapToScene(physValPointP1);
        physValPointP2 = parentGroup()->mapToScene(physValPointP2);
    }
    QLineF lineSceneCoors(physValPointP1.toQPointF(), physValPointP2.toQPointF());
    return ZS::Draw::getPolarCoors(lineSceneCoors, i_pt);
}

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates in length and angle of the given point
           to the selection point of the graphical object.

    How the angle of anchor lines to selection points is interpreted depends on
    the graphical object type and the selection point.

    Example: Horizontal Line

           + Pt
          /
         / Calculated Angle: 60°
        +------------x------------+
        P1         Center         P2

    @note This method is used to keep the relative position of labels to the
          graphical object they are linked to if the linked object is resized,
          rotated or moved.

          For example if the line would be rotated by 180°:

            P2         Center         P1
            +------------x------------+
                          Angle: 60° /
                                    /
                                Pt +
*/
SPolarCoors CGraphObj::getPolarCoorsToSelectionPointFromSceneCoors(
    const QPointF& i_pt, ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::PolygonPoint && i_selPtType != ESelectionPointType::LineCenterPoint) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
#pragma message(__TODO__"pure virtual")
    SPolarCoors polarCoors;
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    return polarCoors;
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle with the start point (P1)
           at the given selection point in scene coordinates.

    This method must be reimplemented by derived classes to ensure that labels
    linked to the selection point are always at the same relative position
    no matter how the object will be rotated.

    This behaviour can only be managed if the labels save the length and the
    angle of the anchor lines to the selection point. But how the angle for
    a selection point is interpreted depends on the graphical object type.
    The example below shows how a line would interprete the angle to the
    line end points.
                                  +-------+      AnchorLine
                                  | Label |          + P2
                                  +---|---+          |
                                   90°| Length       | 
            +-- Line -----------------+              + P1
            P1                        P2

            If rotated by -90 (or +270) degrees:

                 P1 +
                    |
                   Line
                    |
                    | 90° +-------+
                 P2 +-----|-Label |
                   Length +-------+

       AnchorLine   +-----+
                   P1     P2
*/
QLineF CGraphObj::getAnchorLineToSelectionPointFromPolarInSceneCoors(
    const SPolarCoors& i_polarCoors, ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::BoundingRectangle) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
    CPhysValRect physValRctBounding = getPhysValBoundingRect(Units.Length.px);
    CPhysValPoint physValPointP1 = physValRctBounding.selectionPoint(i_selPt);
    CPhysValPoint physValPointP2(*m_pDrawingScene);
    if (i_selPt == ESelectionPoint::TopLeft) {
        physValPointP2 = physValRctBounding.topRight();
    }
    else if (i_selPt == ESelectionPoint::TopRight) {
        physValPointP2 = physValRctBounding.topLeft();
    }
    else if (i_selPt == ESelectionPoint::BottomRight) {
        physValPointP2 = physValRctBounding.bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::BottomLeft) {
        physValPointP2 = physValRctBounding.bottomRight();
    }
    else if (i_selPt == ESelectionPoint::TopCenter) {
        physValPointP2 = physValRctBounding.topRight();
    }
    else if (i_selPt == ESelectionPoint::RightCenter) {
        physValPointP2 = physValRctBounding.bottomRight();
    }
    else if (i_selPt == ESelectionPoint::BottomCenter) {
        physValPointP2 = physValRctBounding.bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::LeftCenter) {
        physValPointP2 = physValRctBounding.topLeft();
    }
    else /*if (i_selPt == ESelectionPoint::Center)*/ {
        physValPointP2 = physValRctBounding.rightCenter();
    }
    if (parentGroup() != nullptr) {
        physValPointP1 = parentGroup()->mapToScene(physValPointP1);
        physValPointP2 = parentGroup()->mapToScene(physValPointP2);
    }
    physValPointP1 = m_pDrawingScene->convert(physValPointP1, Units.Length.px);
    physValPointP2 = m_pDrawingScene->convert(physValPointP2, Units.Length.px);
    QLineF lineSceneCoors(physValPointP1.toQPointF(), physValPointP2.toQPointF());
    return ZS::Draw::getLineFromPolar(
        i_polarCoors.m_fLength_px, i_polarCoors.m_fAngle_degrees, lineSceneCoors);
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle with the start point (P1)
           at the given selection point in scene coordinates.

    See above for more details.
*/
QLineF CGraphObj::getAnchorLineToSelectionPointFromPolarInSceneCoors(
    const SPolarCoors& i_polarCoors, ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::PolygonPoint && i_selPtType != ESelectionPointType::LineCenterPoint) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
#pragma message(__TODO__"pure virtual")
    QLineF anchorLine;
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");
    return anchorLine;
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    Not just hides but also destroys the selection points.
*/
void CGraphObj::hideSelectionPoints(TSelectionPointTypes i_selPts)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = selectionPointTypes2Str(i_selPts);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::hideSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++) {
            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
            bool bHideSelPt = false;
            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
                if (i_selPts & c_uSelectionPointsBoundingRectCorner) {
                    bHideSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
                if (i_selPts & c_uSelectionPointsBoundingRectLineCenter) {
                    bHideSelPt = true;
                }
            }
            else if (selPt == ESelectionPoint::Center) {
                if (i_selPts & c_uSelectionPointsBoundingRectCenter) {
                    bHideSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
                if (i_selPts & c_uSelectionPointsBoundingRectRotate) {
                    bHideSelPt = true;
                }
            }
            if (bHideSelPt) {
                // Deleting child objects leads to itemChange and an updateToolTip call
                // accessing the array of selection points.
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    m_arpSelPtsBoundingRect[idxSelPt] = nullptr;
                    //m_pDrawingScene->removeGraphObj(pGraphObjSelPt); // the dtor of the selection point removes itself from the drawing scene
                    delete pGraphObjSelPt;
                    pGraphObjSelPt = nullptr;
                }
            }
        }
        if (i_selPts & c_uSelectionPointsPolygonPoints) {
            if (m_arpSelPtsPolygon.size() > 0) {
                for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt--) {
                    // Deleting child objects leads to itemChange and an updateToolTip call
                    // accessing the array of selection points.
                    CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                    m_arpSelPtsPolygon[idxSelPt] = nullptr;
                    //m_pDrawingScene->removeGraphObj(pGraphObjSelPt); // the dtor of the selection point (dtor of CGraphObj) removes itself from the drawing scene
                    delete pGraphObjSelPt;
                    pGraphObjSelPt = nullptr;
                }
                m_arpSelPtsPolygon.clear();
            }
        }
    }
} // hideSelectionPoints

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Connects the onSelectionPointGeometryOnSceneChanged slot with the
           geometryOnSceneChanged signals of the visible (created) selection points.
*/
void CGraphObj::connectGeometryOnSceneChangedSlotWithSelectionPoints()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "connectGeometryOnSceneChangedSlotWithSelectionPoints",
        /* strAddInfo   */ "" );

    for (int idxSelPt = 0; idxSelPt < m_arpSelPtsBoundingRect.size(); ++idxSelPt) {
        CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
        if (pGraphObjSelPt != nullptr) {
            QObject::connect(
                pGraphObjSelPt, &CGraphObj::geometryOnSceneChanged,
                this, &CGraphObj::onSelectionPointGeometryOnSceneChanged);
        }
    }
    for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); ++idxSelPt) {
        CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
        if (pGraphObjSelPt != nullptr) {
            QObject::connect(
                pGraphObjSelPt, &CGraphObj::geometryOnSceneChanged,
                this, &CGraphObj::onSelectionPointGeometryOnSceneChanged);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Disconnects the onSelectionPointGeometryOnSceneChanged slot from the
           geometryOnSceneChanged signals of the visible (created) selection points.

    Moving a selection point will modify the shape of the object and the position
    of all other selection points got to be updated. If the position of the other
    selection points will be changed, those selection points are emitting the
    geometryOnSceneChanged signal whereupon the slot method would be called again
    for each other selection point. This will not end up in an endless loop but
    is useless and anything else but performant. So the slot should be temporarily
    disconnected from the geometryOnSceneChanged signal of the selection points.
*/
void CGraphObj::disconnectGeometryOnSceneChangedSlotFromSelectionPoints()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "disconnectGeometryOnSceneChangedSlotFromSelectionPoints",
        /* strAddInfo   */ "" );

    for (int idxSelPt = 0; idxSelPt < m_arpSelPtsBoundingRect.size(); ++idxSelPt) {
        CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
        if (pGraphObjSelPt != nullptr) {
            QObject::disconnect(
                pGraphObjSelPt, &CGraphObj::geometryOnSceneChanged,
                this, &CGraphObj::onSelectionPointGeometryOnSceneChanged);
        }
    }
    for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); ++idxSelPt) {
        CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
        if (pGraphObjSelPt != nullptr) {
            QObject::disconnect(
                pGraphObjSelPt, &CGraphObj::geometryOnSceneChanged,
                this, &CGraphObj::onSelectionPointGeometryOnSceneChanged);
        }
    }
}

////------------------------------------------------------------------------------
//void CGraphObj::bringSelectionPointsToFront( ESelectionPoints i_selPts )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "{" + selectionPointTypes2Str(i_selPts) + "}";
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::bringSelectionPointsToFront",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr)
//    {
//        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++)
//        {
//            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
//            bool bBringToFront = false;
//
//            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
//                if (i_selPts & ESelectionPointsBoundingRectCorner) {
//                    bBringToFront = true;
//                }
//            }
//            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
//                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
//                    bBringToFront = true;
//                }
//            }
//            else if (selPt == ESelectionPoint::Center) {
//                if (i_selPts & ESelectionPointsBoundingRectCenter) {
//                    bBringToFront = true;
//                }
//            }
//            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
//                if (i_selPts & ESelectionPointsBoundingRectRotate) {
//                    bBringToFront = true;
//                }
//            }
//
//            if (bBringToFront) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QRectF rct = pGraphObjSelPt->rect();
//                    rct = pGraphObjSelPt->mapRectToScene(rct);
//                    QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rct);
//                    m_pDrawingScene->bringToFront(pGraphObjSelPt, arpGraphicsItemsIntersected);
//                }
//            }
//        }
//
//        if (i_selPts & ESelectionPointsPolygonShapePoints) {
//            for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QRectF rct = pGraphObjSelPt->rect();
//                    rct = pGraphObjSelPt->mapRectToScene(rct);
//                    QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rct);
//                    m_pDrawingScene->bringToFront(pGraphObjSelPt, arpGraphicsItemsIntersected);
//                }
//            }
//        }
//    }
//} // bringSelectionPointsToFront

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the selection points if not yet created and adds them to
           the graphics scene.
*/
void CGraphObj::showSelectionPointsOfBoundingRect(const QRectF& i_rct, TSelectionPointTypes i_selPts)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + qRect2Str(i_rct) + "}" + ", SelPts {" + selectionPointTypes2Str(i_selPts) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::showSelectionPointsOfBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr) {
        QGraphicsItem_prepareGeometryChange(); // as the boundingRect is changed
        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++) {
            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
            bool bShowSelPt = false;
            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
                if (i_selPts & c_uSelectionPointsBoundingRectCorner) {
                    bShowSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
                if (i_selPts & c_uSelectionPointsBoundingRectLineCenter) {
                    bShowSelPt = true;
                }
            }
            else if (selPt == ESelectionPoint::Center) {
                if (i_selPts & c_uSelectionPointsBoundingRectCenter) {
                    bShowSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
                if (i_selPts & c_uSelectionPointsBoundingRectRotate) {
                    bShowSelPt = true;
                }
            }
            if (bShowSelPt) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
                if (pGraphObjSelPt == nullptr) {
                    pGraphObjSelPt = new CGraphObjSelectionPoint(
                        m_pDrawingScene, SGraphObjSelectionPoint(this, ESelectionPointType::BoundingRectangle, selPt));
                    m_arpSelPtsBoundingRect[idxSelPt] = pGraphObjSelPt;

                    // Please note that selection points should not belong as child to the graphics items
                    // for which the selection points are created. Otherwise the "boundingRect" call
                    // of groups (which implicitly calls childrenBoundingRect) does not work as expected
                    // as the selection points would be included.
                    // In addition selection points must be directly added to the graphics scene as they
                    // should not be indicated in the index tree.
                    m_pDrawingScene->addItem(pGraphObjSelPt);

                    // Event filters can only be installed on items in a scene.
                    //pGraphObjSelPt->installSceneEventFilter(pGraphicsItem);

                    QObject::connect(
                        pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
                        this, &CGraphObj::onSelectionPointAboutToBeDestroyed);
                    QObject::connect(
                        pGraphObjSelPt, &CGraphObj::geometryOnSceneChanged,
                        this, &CGraphObj::onSelectionPointGeometryOnSceneChanged);
                }
            }
        }
    }
} // showSelectionPointsOfBoundingRect

////------------------------------------------------------------------------------
//void CGraphObj::updateSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = rect2Str(i_rct);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::updateSelectionPointsOfBoundingRect",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr)
//    {
//        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++) {
//            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
//            bool bUpdateSelPt = false;
//            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
//                if (i_selPts & ESelectionPointsBoundingRectCorner) {
//                    bUpdateSelPt = true;
//                }
//            }
//            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
//                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
//                    bUpdateSelPt = true;
//                }
//            }
//            else if (selPt == ESelectionPoint::Center) {
//                if (i_selPts & ESelectionPointsBoundingRectCenter) {
//                    bUpdateSelPt = true;
//                }
//            }
//            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
//                if (i_selPts & ESelectionPointsBoundingRectRotate) {
//                    bUpdateSelPt = true;
//                }
//            }
//            if (bUpdateSelPt) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QPointF ptSel;
//                    if (selPt == ESelectionPoint::RotateTop /*&& m_fScaleFacYCurr != 0.0*/) {
//                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,ESelectionPoint::TopCenter);
//                        ptSel.setY( ptSel.y() - getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
//                        ptSel = pGraphicsItem->mapToScene(ptSel);
//                    }
//                    else if (selPt == ESelectionPoint::RotateBottom /*&& m_fScaleFacYCurr != 0.0*/) {
//                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,ESelectionPoint::BottomCenter);
//                        ptSel.setY( ptSel.y() + getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
//                        ptSel = pGraphicsItem->mapToScene(ptSel);
//                    }
//                    else {
//                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,selPt);
//                        ptSel = pGraphicsItem->mapToScene(ptSel);
//                    }
//                    pGraphObjSelPt->setPos(ptSel);
//                }
//            }
//        }
//    }
//} // updateSelectionPointsOfBoundingRect

//------------------------------------------------------------------------------
/*! @brief Creates the selection points if not yet created and adds them to
           the graphics scene.
*/
void CGraphObj::showSelectionPointsOfPolygon( const QPolygonF& i_plg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Polygon {" + qPolygon2Str(i_plg) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::showSelectionPointsOfPolygon",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr) {
        QGraphicsItem_prepareGeometryChange(); // as the boundingRect is changed
        if (m_arpSelPtsPolygon.size() > i_plg.size()) {
            for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= i_plg.size(); --idxSelPt) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                m_arpSelPtsPolygon[idxSelPt] = nullptr;
                // The dtor of the selection point (dtor of CGraphObj) removes itself from the drawing scene.
                //m_pDrawingScene->removeGraphObj(pGraphObjSelPt);
                delete pGraphObjSelPt;
                pGraphObjSelPt = nullptr;
            }
            for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= i_plg.size(); --idxSelPt) {
                m_arpSelPtsPolygon.removeAt(idxSelPt);
            }
        }
        else if (m_arpSelPtsPolygon.size() < i_plg.size()) {
            for (int idxSelPt = m_arpSelPtsPolygon.size(); idxSelPt < i_plg.size(); ++idxSelPt) {
                m_arpSelPtsPolygon.append(nullptr);
            }
        }
        for (int idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
            if (pGraphObjSelPt == nullptr) {
                pGraphObjSelPt = new CGraphObjSelectionPoint(
                    m_pDrawingScene, SGraphObjSelectionPoint(this, ESelectionPointType::PolygonPoint, idxSelPt));
                m_arpSelPtsPolygon[idxSelPt] = pGraphObjSelPt;

                // Please note that selection points should not belong as child to the graphics items
                // for which the selection points are created. Otherwise the "boundingRect" call
                // of groups (which implicitly calls childrenBoundingRect) does not work as expected
                // as the selection points would be included.
                // In addition selection points must be directly added to the graphics scene as they
                // should not be indicated in the index tree.
                m_pDrawingScene->addItem(pGraphObjSelPt);

                // Event filters can only be installed on items in a scene.
                //pGraphObjSelPt->installSceneEventFilter(pGraphicsItem);

                QObject::connect(
                    pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
                    this, &CGraphObj::onSelectionPointAboutToBeDestroyed);
                QObject::connect(
                    pGraphObjSelPt, &CGraphObj::geometryOnSceneChanged,
                    this, &CGraphObj::onSelectionPointGeometryOnSceneChanged);
            }
        }
    }
} // showSelectionPointsOfPolygon

////------------------------------------------------------------------------------
//void CGraphObj::updateSelectionPointsOfPolygon( const QPolygonF& i_plg )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = polygon2Str(i_plg);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::updateSelectionPointsOfPolygon",
//        /* strAddInfo   */ strMthInArgs );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr) {
//        if (i_plg.size() == m_arpSelPtsPolygon.size()) {
//            for (int idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++) {
//                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//                if (pGraphObjSelPt != nullptr) {
//                    QPointF ptSel = i_plg[idxSelPt];
//                    ptSel = pGraphicsItem->mapToScene(ptSel);
//                    pGraphObjSelPt->setPos(ptSel);
//                }
//            }
//        }
//    }
//}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the list of all existing label names.

    @return List of existing label names.
*/
QString CGraphObj::findUniqueLabelName(const QString& i_strPrefix) const
//------------------------------------------------------------------------------
{
    QString strName = i_strPrefix;
    if (strName.isEmpty()) {
        strName = "Label";
    }
    strName += QString::number(m_hshLabelDscrs.size());
    if (m_hshLabelDscrs.contains(strName)) {
        strName = i_strPrefix;
        if (strName.isEmpty()) {
            strName = "Label";
        }
        int iRow = 0;
        QString strNameTmp = strName + QString::number(iRow);
        while (m_hshLabelDscrs.contains(strNameTmp)) {
            ++iRow;
            strNameTmp = strName + QString::number(iRow);
        }
        strName = strNameTmp;
    }
    return strName;
}

//------------------------------------------------------------------------------
/*! @brief Returns the list of all existing label names.

    The returned list starts with the predefined labels.

    @return List of existing label names.
*/
QStringList CGraphObj::getLabelNames() const
//------------------------------------------------------------------------------
{
    QStringList strlstLabelNames;
    // The predefined labels should be at the beginning of the table.
    QStringList strlstLabelNamesTmp = m_hshLabelDscrs.keys();
    if (strlstLabelNamesTmp.size() > 0) {
        // The predefined labels should be at the beginning of the table.
        // Insert those first if the corresponding label has been added.
        QStringList strlstPredefinedLabelNames = getPredefinedLabelNames();
        QSet<QString> strlstLabelNamesAdded;
        for (const QString& strLabelName : strlstPredefinedLabelNames) {
            if (isLabelAdded(strLabelName)) {
                strlstLabelNames.append(strLabelName);
                strlstLabelNamesAdded.insert(strLabelName);
            }
        }
        // The user defined labels should follow the predefined labels.
        // Add those after the predefined labels.
        for (const QString& strLabelName : strlstLabelNamesTmp) {
            if (isLabelAdded(strLabelName)) {
                // If label has not already been added as a predefined label ...
                if (!strlstLabelNamesAdded.contains(strLabelName)) {
                    strlstLabelNames.append(strLabelName);
                    strlstLabelNamesAdded.insert(strLabelName);
                }
            }
        }
    }
    return strlstLabelNames;
}

//------------------------------------------------------------------------------
/*! @brief Returns the list of the reserved (predefined) label names.

    For all object types "Name" is reserved to indicate the name of the object.

    To extend the list of predefined label names the derived classes should add
    the label names in their conto the member m_strlstPredefinedLabelNames
    This method may be overriden by derived classes if there are further reserved
    names beside "Name" (e.g. the "Line" object reserves "P1" and "P2" as names).
    If user defined labels got to be added the reserved names may not be used.

    @return List of reserved label names.
*/
QStringList CGraphObj::getPredefinedLabelNames() const
//------------------------------------------------------------------------------
{
    return m_strlstPredefinedLabelNames;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether the passed name belongs to the predefined names.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the given name belongs to the list of predefined names, false otherwise.
*/
bool CGraphObj::isPredefinedLabelName(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    return m_strlstPredefinedLabelNames.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Returns the label descriptor for the given label name.
*/
CGraphObjLabel* CGraphObj::getLabel(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    return m_hshpLabels.value(i_strName, nullptr);
}

//------------------------------------------------------------------------------
/*! @brief Returns the label descriptor for the given label name.
*/
SLabelDscr CGraphObj::getLabelDescriptor(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    SLabelDscr labelDscr = m_hshLabelDscrs.value(i_strName, SLabelDscr());
    CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
    if (pGraphObjLabel != nullptr) {
        labelDscr.m_polarCoorsToLinkedSelPt = pGraphObjLabel->polarCoorsToLinkedSelectionPoint();
    }
    return labelDscr;
}

//------------------------------------------------------------------------------
/*! @brief Returns the list of the possible selection points a label may be anchored to.

    The method has to be overridden by the specialized classes.

    Please note that the most common used selection points should be at the
    beginning of the list so that combo boxes to select the selection point
    start with those.

    @param [in] i_strName
        Unique name of the label. May be omitted (empty string).

    @return List of possbile selection points.
        If PolygonPoint is contained in the list of returned selection points the possible
        anchor points depend on the number of line points of the polygon or polyline.
*/
QList<SGraphObjSelectionPoint> CGraphObj::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    static QList<SGraphObjSelectionPoint> s_arSelPts;
    if (s_arSelPts.isEmpty()) {
        s_arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObj*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
    }
    static QHash<QString, QList<SGraphObjSelectionPoint>> s_hshSelPtsPredefined;
    if (s_hshSelPtsPredefined.isEmpty()) {
        s_hshSelPtsPredefined.insert(c_strLabelName, s_arSelPts);
    }
    return s_hshSelPtsPredefined.value(i_strName, QList<SGraphObjSelectionPoint>());
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the label with the passed name has been modified or still
           has its default values.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the label still has its default values, false otherwise.
*/
bool CGraphObj::labelHasDefaultValues(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    #pragma message(__TODO__"Pure virtual")
    return true;
}

//------------------------------------------------------------------------------
/*! @brief Returns whether a label with the passed name has been added to the
           list of text labels.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the given name belongs to the list of text labels, false otherwise.
*/
bool CGraphObj::isLabelAdded(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    return m_hshLabelDscrs.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Creates a new text label with the given name linked to a selection point
           at the bounding rectangle.

    The label is not added to the graphics scene and remains invisible.
    To add the label also to the graphics scene the label must be shown.

    The predefined labels (including "Name") got to be added in the constructor
    of the derived class. Adding labels in the constructor of the base class
    would lead to crashes as "QGraphicsItem" is not yet created and the constructor
    of the label object tries to access the parent (the linked) "QGraphicsItem".

    @param [in] i_strName
        Name of the label. The name must be unique otherwise no label is created.
    @param [in] i_strText (optional)
        If not empty defines the text to be shown.
    @param [in] i_selPtType
        Selection point type.
        Range [BoundingRectangle]
    @param [in] i_selPt
        Selection point the label should be anchored to.

    @return true, if the label has been created and added, false otherwise.
*/
bool CGraphObj::addLabel(
    const QString& i_strName, const QString& i_strText,
    ESelectionPointType i_selPtType, ESelectionPoint i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strText +
            ", " + CEnumSelectionPointType(i_selPtType).toString() +
            ", " + CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::addLabel",
        /* strAddInfo   */ strMthInArgs );

    bool bCanAdd = !m_hshLabelDscrs.contains(i_strName);
    if (bCanAdd) {
        QString strText = i_strText;
        if (i_strName == c_strLabelName) {
            strText = m_strName;
        }
        SLabelDscr labelDscr(EGraphObjTypeLabel, i_strName, strText, SGraphObjSelectionPoint(this, i_selPtType, i_selPt));
        m_hshLabelDscrs.insert(i_strName, labelDscr);
        emit_labelAdded(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
/*! @brief Creates a new text label with the given name linked to a polygon shape point.

    The label is not added to the graphics scene and remains invisible.
    To add the label also to the graphics scene the label must be shown.

    The predefined labels (including "Name") got to be added in the constructor
    of the derived class. Adding labels in the constructor of the base class
    would lead to crashes as "QGraphicsItem" is not yet created and the constructor
    of the label object tries to access the parent (the linked) "QGraphicsItem".

    @param [in] i_strName
        Name of the label. The name must be unique otherwise no label is created.
    @param [in] i_strText (optional)
        If not empty defines the text to be shown.
    @param [in] i_selPtType
        Selection point type.
        Range [PolygonPoint, LineCenterPoint]
    @param [in] i_idxPt
        Selection point the label should be anchored to.
        Defines either the index of a polygon (or line) point or the index
        of the line segment of a polygon.

    @return true, if the label has been created and added, false otherwise.
*/
bool CGraphObj::addLabel(
    const QString& i_strName, const QString& i_strText, ESelectionPointType i_selPtType, int i_idxPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strText + + ", " +
            CEnumSelectionPointType(i_selPtType).toString() + ", P" + QString::number(i_idxPt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::addLabel",
        /* strAddInfo   */ strMthInArgs );

    bool bCanAdd = !m_hshLabelDscrs.contains(i_strName);
    if (bCanAdd) {
        QString strText = i_strText;
        if (i_strName == c_strLabelName) {
            strText = m_strName;
        }
        SLabelDscr labelDscr(EGraphObjTypeLabel, i_strName);
        labelDscr.m_strText = strText;
        labelDscr.m_selPt1 = SGraphObjSelectionPoint(this, i_selPtType, i_idxPt);
        m_hshLabelDscrs.insert(i_strName, labelDscr);
        emit_labelAdded(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
/*! Removes the label with the given name.

    The label is destroyed and also removed from the graphics scene and becomes invisible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
bool CGraphObj::removeLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::removeLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bCanRemove = !isPredefinedLabelName(i_strName);
    if (bCanRemove) {
        m_hshLabelDscrs.remove(i_strName);
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            // "onLabelAboutToBeDestroyed" is called which removes the label from the hash.
            QGraphicsItem* pGraphicsItemLabel = dynamic_cast<QGraphicsItem*>(pGraphObjLabel);
            pGraphicsItemLabel->hide();
            delete pGraphObjLabel;
            pGraphObjLabel = nullptr;
        }
        emit_labelRemoved(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanRemove);
    }
    return bCanRemove;
}

//------------------------------------------------------------------------------
/*! Renames the label with the given name.

    If a label with passed new name already exists, the label is not renamed and
    the method returns false.

    @param [in] i_strName
        Current name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_strNameNew
        New name of the label. If a label with the new name already exists, the
        label will not be renamed.

    @return true, if the label has been renamed, false otherwise.
*/
bool CGraphObj::renameLabel(const QString& i_strName, const QString& i_strNameNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strNameNew;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::renameLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bCanRename = !isPredefinedLabelName(i_strName);
    if (bCanRename) {
        SLabelDscr labelDscr = m_hshLabelDscrs[i_strName];
        m_hshLabelDscrs.remove(i_strName);
        labelDscr.m_strKey = i_strNameNew;
        m_hshLabelDscrs.insert(i_strNameNew, labelDscr);
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel == nullptr) {
            m_hshpLabels.remove(i_strName);
            pGraphObjLabel->setKey(i_strNameNew);
            m_hshpLabels.insert(i_strNameNew, pGraphObjLabel);
            if (i_strNameNew == c_strLabelName) {
                pGraphObjLabel->setText(m_strName);
            }
        }
        emit_labelRenamed(i_strName, i_strNameNew);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanRename);
    }
    return bCanRename;
}

//------------------------------------------------------------------------------
/*! Sets the text to be shown by the label.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_strText
        Text to be shown.
*/
void CGraphObj::setLabelText(const QString& i_strName, const QString& i_strText)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setLabelText",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    if (m_hshLabelDscrs[i_strName].m_strText != i_strText) {
        m_hshLabelDscrs[i_strName].m_strText = i_strText;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setText(i_strText);
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the text of the label.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
QString CGraphObj::labelText(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshLabelDscrs[i_strName].m_strText;
}

//------------------------------------------------------------------------------
/*! @brief Sets the anchor point of the label with the given name to a selection
           point at the bounding rectangle.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_selPtType
        Selection point type.
        Range [BoundingRectangle]
    @param [in] i_selPt
        Selection point the label should be anchored to.
*/
void CGraphObj::setLabelAnchorPoint(
    const QString& i_strName, ESelectionPointType i_selPtType, ESelectionPoint i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName +
            ", " + CEnumSelectionPointType(i_selPtType).toString() +
            ", " + CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setLabelAnchorPoint",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bSelPtAllowed = false;
    QList<SGraphObjSelectionPoint> arSelPts = getPossibleLabelAnchorPoints(i_strName);
    for (const SGraphObjSelectionPoint& selPt : arSelPts) {
        if (selPt.m_selPtType == i_selPtType && selPt.m_selPt == i_selPt) {
            bSelPtAllowed = true;
            break;
        }
    }
    if (!bSelPtAllowed) {
        SGraphObjSelectionPoint selPt(this, i_selPtType, i_selPt);
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, i_strName + ": Invalid selection point " + selPt.toString());
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (labelDscr.m_selPt1.m_selPtType != i_selPtType || labelDscr.m_selPt1.m_selPt != i_selPt) {
        labelDscr.m_selPt1 = SGraphObjSelectionPoint(this, i_selPtType, i_selPt);
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setSelectionPoint1(labelDscr.m_selPt1);
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the anchor point of the label with the given name to a shape point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_selPtType
        Selection point type.
        Range [PolygonPoint, LineCenterPoint]
    @param [in] i_idxPt
        Selection point the label should be anchored to.
*/
void CGraphObj::setLabelAnchorPoint(
    const QString& i_strName, ESelectionPointType i_selPtType, int i_idxPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName +
            ", " + CEnumSelectionPointType(i_selPtType).toString() +
            ", " + QString::number(i_idxPt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setLabelAnchorPoint",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bSelPtAllowed = false;
    QList<SGraphObjSelectionPoint> arSelPts = getPossibleLabelAnchorPoints(i_strName);
    for (const SGraphObjSelectionPoint& selPt : arSelPts) {
        if (selPt.m_selPtType == i_selPtType && selPt.m_idxPt == i_idxPt) {
            bSelPtAllowed = true;
            break;
        }
    }
    if (!bSelPtAllowed) {
        SGraphObjSelectionPoint selPt(this, i_selPtType, i_idxPt);
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, i_strName + ": Invalid selection point " + selPt.toString());
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (labelDscr.m_selPt1.m_selPtType != i_selPtType || labelDscr.m_selPt1.m_idxPt != i_idxPt) {
        labelDscr.m_selPt1.m_selPtType = i_selPtType;
        labelDscr.m_selPt1.m_idxPt = i_idxPt;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setSelectionPoint1(labelDscr.m_selPt1);
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the anchor point of the label.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
SGraphObjSelectionPoint CGraphObj::labelAnchorPoint(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshLabelDscrs[i_strName].m_selPt1;
}

//------------------------------------------------------------------------------
/*! Shows the label (set visible) with the given name.

    The label is added to the graphics scene and becomes visible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::showLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::showLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
    if (pGraphObjLabel == nullptr) {
        labelDscr.m_bLabelIsVisible = true;
        pGraphObjLabel = new CGraphObjLabel(
            m_pDrawingScene, i_strName, labelDscr.m_strText, labelDscr.m_selPt1);
        m_hshpLabels.insert(i_strName, pGraphObjLabel);
        QObject::connect(
            pGraphObjLabel, &CGraphObj::aboutToBeDestroyed,
            this, &CGraphObj::onLabelAboutToBeDestroyed);
        // Please note that labels should not belong as child to the graphics items
        // for which the labels are created. Otherwise the "boundingRect" call of groups
        // (which implicitly calls childrenBoundingRect) does not work as expected as
        // the labels would be included.
        // In addition the labels must be directly added to the graphics scene as they
        // should not be indicated in the index tree.
        m_pDrawingScene->addItem(pGraphObjLabel);
        pGraphObjLabel->setVisible(true);
        pGraphObjLabel->setPolarCoorsToLinkedSelectionPoint(labelDscr.m_polarCoorsToLinkedSelPt);
        labelDscr.m_bShowAnchorLine ? pGraphObjLabel->showAnchorLines() : pGraphObjLabel->hideAnchorLines();
        // The labels anchor line should be drawn before the object is drawn.
        // Otherwise the anchor lines may cover the painting of this object.
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
} // showLabel

//------------------------------------------------------------------------------
/*! Hides the label.

    The label is removed from the graphics scene and becomes invisible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::hideLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::hideLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
    if (pGraphObjLabel != nullptr) {
        if (pGraphObjLabel->scene() == nullptr) {
            throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
        }
        // Save current distance of label to selection point.
        labelDscr = getLabelDescriptor(i_strName);
        labelDscr.m_bLabelIsVisible = false;

        // "onLabelAboutToBeDestroyed" is called which removes the label from the hash.
        // The destructor also removes the label from the graphics scene.
        delete pGraphObjLabel;
        pGraphObjLabel = nullptr;
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns whether the visibility of the label with the given name.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return true, if the label is visible. false otherwise.
*/
bool CGraphObj::isLabelVisible(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshpLabels.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Sets the relative position of the label in polar coordinates
           (length in pixels, angle in degrees) of the labels position to
           the linked selection point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_polarCoors
        Polar coordinates (length in pixels, angle in degrees)
        as a relative position of the label to the linked selection point.
*/
void CGraphObj::setLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName, const SPolarCoors& i_polarCoors)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", {" + i_polarCoors.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setLabelPolarCoorsToLinkedSelectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (labelDscr.m_polarCoorsToLinkedSelPt != i_polarCoors) {
        labelDscr.m_polarCoorsToLinkedSelPt = i_polarCoors;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setPolarCoorsToLinkedSelectionPoint(i_polarCoors);
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates (length in pixels, angle in degrees)
           as a relative position of the label to the linked selection point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return Relative position of the label to the linked selection point.
*/
SPolarCoors CGraphObj::labelPolarCoorsToLinkedSelectionPoint(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshLabelDscrs[i_strName].m_polarCoorsToLinkedSelPt;
}

//------------------------------------------------------------------------------
/*! Shows the line between the label and the selection point the label is aligned to.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::showLabelAnchorLine(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::showLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (!labelDscr.m_bShowAnchorLine) {
        labelDscr.m_bShowAnchorLine = true;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->showAnchorLines();
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Hides the line between the description label and the selection point the label is aligned to.

    The method just sets a flag. If the description label is not visible the method has no effect.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::hideLabelAnchorLine(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::hideLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
    if (labelDscr.m_bShowAnchorLine) {
        labelDscr.m_bShowAnchorLine = false;
        CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->hideAnchorLines();
        }
        emit_labelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Returns the visibility of the line between the label and the selection point
    the label is aligned to.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return true, if the anchor line is visible, false otherwise.
*/
bool CGraphObj::isLabelAnchorLineVisible(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshLabelDscrs[i_strName].m_bShowAnchorLine;
}

/*==============================================================================
public: // geometry labels
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method must be overridden by derived classes to return the names
           for all geometry values which can be edited or indicated.

    Usually for each geometry value a pair of X and Y values can be set.
    Most of the geometry values specify shape points with X and Y coordinates.
    For the "Size" value X means width and Y means height.

    But there are also specific geometry values with a special meaning like
    "Length" and "Angle". For those only one value can be set and indicated.
    For those the X value is used and the Y value is invalid.

    A Line object for example supports the following geometry values:

    - P1 (x/y), P2 (x/y), Center (x/y), dX("x"), dY("y"), Length("l"), Angle ("°").

    @return Supported value names.
*/
QStringList CGraphObj::getGeometryLabelNames() const
//------------------------------------------------------------------------------
{
    return m_strlstGeometryLabelNames;
}

//------------------------------------------------------------------------------
/*! @brief Returns the label descriptor for the given label name.
*/
bool CGraphObj::isValidGeometryLabelName(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    return m_hshGeometryLabelDscrs.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Returns the label descriptor for the given label name.
*/
CGraphObjLabel* CGraphObj::getGeometryLabel(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    return m_hshpGeometryLabels.value(i_strName, nullptr);
}

//------------------------------------------------------------------------------
/*! @brief Returns the label descriptor for the given label name.
*/
SLabelDscr CGraphObj::getGeometryLabelDescriptor(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    SLabelDscr labelDscr = m_hshGeometryLabelDscrs.value(i_strName, SLabelDscr());
    CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
    if (pGraphObjLabel != nullptr) {
        labelDscr.m_polarCoorsToLinkedSelPt = pGraphObjLabel->polarCoorsToLinkedSelectionPoint();
    }
    return labelDscr;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the label with the passed name has been modified or still
           has its default values.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the label still has its default values, false otherwise.
*/
bool CGraphObj::geometryLabelHasDefaultValues(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    #pragma message(__TODO__"Pure virtual")
    return true;
}

//------------------------------------------------------------------------------
/*! Shows the label (set visible) with the given name.

    The label is added to the graphics scene and becomes visible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::showGeometryLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::showGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
    if (pGraphObjLabel == nullptr) {
        CGraphObjLabel* pGraphObjLabel = nullptr;
        if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryPosition) {
            pGraphObjLabel = new CGraphObjLabelGeometryPosition(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1);
        }
        else if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryDX) {
            pGraphObjLabel = new CGraphObjLabelGeometryDX(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1, labelDscr.m_selPt2);
        }
        else if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryDY) {
            pGraphObjLabel = new CGraphObjLabelGeometryDY(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1, labelDscr.m_selPt2);
        }
        else if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryLength) {
            pGraphObjLabel = new CGraphObjLabelGeometryLength(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1, labelDscr.m_selPt2);
        }
        else if (labelDscr.m_labelType == EGraphObjTypeLabelGeometryAngle) {
            pGraphObjLabel = new CGraphObjLabelGeometryAngle(
                m_pDrawingScene, i_strName, labelDscr.m_selPt1, labelDscr.m_selPt2);
        }
        if (pGraphObjLabel != nullptr) {
            labelDscr.m_bLabelIsVisible = true;
            pGraphObjLabel->setVisible(false);
            pGraphObjLabel->setPolarCoorsToLinkedSelectionPoint(labelDscr.m_polarCoorsToLinkedSelPt);
            labelDscr.m_bShowAnchorLine ? pGraphObjLabel->showAnchorLines() : pGraphObjLabel->hideAnchorLines();
            m_hshpGeometryLabels.insert(i_strName, pGraphObjLabel);
            QObject::connect(
                pGraphObjLabel, &CGraphObj::aboutToBeDestroyed,
                this, &CGraphObj::onGeometryLabelAboutToBeDestroyed);
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
        }
        // Please note that labels should not belong as child to the graphics items
        // for which the labels are created. Otherwise the "boundingRect" call of groups
        // (which implicitly calls childrenBoundingRect) does not work as expected as
        // the labels would be included.
        // In addition the labels must be directly added to the graphics scene as they
        // should not be indicated in the index tree.
        m_pDrawingScene->addItem(pGraphObjLabel);
        pGraphObjLabel->setVisible(true);
        // The labels anchor line should be drawn before the object is drawn.
        // Otherwise the anchor lines may cover the painting of this object.
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Hides the label.

    The label is removed from the graphics scene and becomes invisible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::hideGeometryLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::hideGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
    if (pGraphObjLabel != nullptr) {
        if (pGraphObjLabel->scene() == nullptr) {
            throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
        }

        // Save current distance of label to selection point.
        labelDscr = getGeometryLabelDescriptor(i_strName);
        labelDscr.m_bLabelIsVisible = false;

        // "onGeometryLabelAboutToBeDestroyed" is called which removes the label from the hash.
        // The destructor also removes the label from the graphics scene.
        QGraphicsItem* pGraphicsItemLabel = dynamic_cast<QGraphicsItem*>(pGraphObjLabel);
        pGraphicsItemLabel->hide();

        delete pGraphObjLabel;
        pGraphObjLabel = nullptr;
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @Returns whether the visibility of the label with the given name.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return true, if the label is visible. false otherwise.
*/
bool CGraphObj::isGeometryLabelVisible(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshpGeometryLabels.contains(i_strName);
}

//------------------------------------------------------------------------------
/*! @brief Sets the relative position of the label in polar coordinates
           (length in pixels, angle in degrees) of the labels position to
           the linked selection point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
    @param [in] i_polarCoors
        Polar coordinates (length in pixels, angle in degrees)
        as a relative position of the label to the linked selection point.
*/
void CGraphObj::setGeometryLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName, const SPolarCoors& i_polarCoors)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", {" + i_polarCoors.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setGeometryLabelPolarCoorsToLinkedSelectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    if (labelDscr.m_polarCoorsToLinkedSelPt != i_polarCoors) {
        labelDscr.m_polarCoorsToLinkedSelPt = i_polarCoors;
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->setPolarCoorsToLinkedSelectionPoint(i_polarCoors);
        }
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates (length in pixels, angle in degrees)
           as a relative position of the label to the linked selection point.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return Relative position of the label to the linked selection point.
*/
SPolarCoors CGraphObj::geometryLabelPolarCoorsToLinkedSelectionPoint(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshGeometryLabelDscrs[i_strName].m_polarCoorsToLinkedSelPt;
}

//------------------------------------------------------------------------------
/*! Shows the line between the label and the selection point the label is aligned to.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::showGeometryLabelAnchorLine(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::showGeometryLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    if (!labelDscr.m_bShowAnchorLine) {
        labelDscr.m_bShowAnchorLine = true;
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->showAnchorLines();
        }
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Hides the line between the description label and the selection point the label is aligned to.

    The method just sets a flag. If the description label is not visible the method has no effect.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
void CGraphObj::hideGeometryLabelAnchorLine(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::hideGeometryLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
    if (labelDscr.m_bShowAnchorLine) {
        labelDscr.m_bShowAnchorLine = false;
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->hideAnchorLines();
        }
        emit_geometryLabelChanged(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Returns the visibility of the line between the label and the selection point
    the label is aligned to.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.

    @return true, if the anchor line is visible, false otherwise.
*/
bool CGraphObj::isGeometryLabelAnchorLineVisible(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    return m_hshGeometryLabelDscrs[i_strName].m_bShowAnchorLine;
}

/*==============================================================================
protected: // overridables (geometry labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a new geometry label with the given name linked to one or two
           selection point at the bounding rectangle.

    The label is not added to the graphics scene and remains invisible.
    To add the label also to the graphics scene the label must be shown.

    @param [in] i_strName
        Name of the label. The name must be unique otherwise no label is created.
    @param [in] i_labelType
        Range [EGraphObjTypeLabelGeometryPosition,
               EGraphObjTypeLabelGeometryDX, EGraphObjTypeLabelGeometryDY,
               EGraphObjTypeLabelGeometryWidth, EGraphObjTypeLabelGeometryHeight,
               EGraphObjTypeLabelGeometryLength, EGraphObjTypeLabelGeometryAngle]
        If not empty defines the text to be shown.
    @param [in] i_selPt1
        First selection point the label should use to indicate the geometry.
    @param [in] i_selPt2 (optional)
        Second selection point the label should use to indicate the geometry.
        For position label only one selection point is used and the second selection
        point is set to invalid.

    @return true, if the label has been created and added, false otherwise.
*/
bool CGraphObj::addGeometryLabel(
    const QString& i_strName, EGraphObjType i_labelType,
    ESelectionPoint i_selPt1, ESelectionPoint i_selPt2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + graphObjType2Str(i_labelType)
            + ", " + CEnumSelectionPoint(i_selPt1).toString()
            + ", " + CEnumSelectionPoint(i_selPt2).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::addGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    bool bCanAdd = !m_hshGeometryLabelDscrs.contains(i_strName);
    if (bCanAdd) {
        SLabelDscr labelDscr(i_labelType, i_strName);
        labelDscr.m_selPt1 = SGraphObjSelectionPoint(this, ESelectionPointType::BoundingRectangle, i_selPt1);
        if (i_selPt2 != ESelectionPoint::None) {
            labelDscr.m_selPt2 = SGraphObjSelectionPoint(this, ESelectionPointType::BoundingRectangle, i_selPt2);
        }
        m_hshGeometryLabelDscrs.insert(i_strName, labelDscr);
        emit_geometryLabelAdded(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
/*! @brief Creates a new geometry label with the given name linked to one or two
           shape points.

    The label is not added to the graphics scene and remains invisible.
    To add the label also to the graphics scene the label must be shown.

    @param [in] i_strName
        Name of the label. The name must be unique otherwise no label is created.
    @param [in] i_labelType
        Range [EGraphObjTypeLabelGeometryPosition,
               EGraphObjTypeLabelGeometryDX, EGraphObjTypeLabelGeometryDY,
               EGraphObjTypeLabelGeometryWidth, EGraphObjTypeLabelGeometryHeight,
               EGraphObjTypeLabelGeometryLength, EGraphObjTypeLabelGeometryAngle]
        If not empty defines the text to be shown.
    @param [in] i_idxPt1
        First selection point the label should use to indicate the geometry.
    @param [in] i_idxPt2 (optional)
        Second selection point the label should use to indicate the geometry.
        For position labels only one selection point is used and the second selection
        point is set to invalid.

    @return true, if the label has been created and added, false otherwise.
*/
bool CGraphObj::addGeometryLabel(
    const QString& i_strName, EGraphObjType i_labelType,
    int i_idxPt1, int i_idxPt2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + graphObjType2Str(i_labelType)
            + ", Pt1: " + QString::number(i_idxPt1) + ", Pt2: " + QString::number(i_idxPt2);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::addGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    bool bCanAdd = !m_hshGeometryLabelDscrs.contains(i_strName);
    if (bCanAdd) {
        SLabelDscr labelDscr(i_labelType, i_strName);
        labelDscr.m_selPt1 = SGraphObjSelectionPoint(this, ESelectionPointType::PolygonPoint, i_idxPt1);
        if (i_idxPt2 >= 0) {
            labelDscr.m_selPt2 = SGraphObjSelectionPoint(this, ESelectionPointType::PolygonPoint, i_idxPt2);
        }
        m_hshGeometryLabelDscrs.insert(i_strName, labelDscr);
        emit_geometryLabelAdded(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
/*! Removes the label with the given name.

    The label is destroyed and also removed from the graphics scene and becomes invisible.

    @param [in] i_strName
        Name of the label. If no label with the name exists an exception is thrown.
*/
bool CGraphObj::removeGeometryLabel(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::removeGeometryLabel",
        /* strAddInfo   */ strMthInArgs );

    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    bool bCanRemove = true; //!isPredefinedLabelName(i_strName);
    if (bCanRemove) {
        m_hshGeometryLabelDscrs.remove(i_strName);
        CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(i_strName, nullptr);
        if (pGraphObjLabel != nullptr) {
            // "onGeometryLabelAboutToBeDestroyed" is called which removes the label from the hash.
            QGraphicsItem* pGraphicsItemLabel = dynamic_cast<QGraphicsItem*>(pGraphObjLabel);
            pGraphicsItemLabel->hide();
            delete pGraphObjLabel;
            pGraphObjLabel = nullptr;
        }
        emit_geometryLabelRemoved(i_strName);
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanRemove);
    }
    return bCanRemove;
}

/*==============================================================================
public: // instance methods (simulation methods)
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObj::addMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::addMousePressEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arMousePressEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMousePressEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arMousePressEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::removeMousePressEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arMousePressEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMousePressEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arMousePressEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::addMouseReleaseEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arMouseReleaseEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseReleaseEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arMouseReleaseEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::removeMouseReleaseEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arMouseReleaseEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseReleaseEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arMouseReleaseEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::addMouseDoubleClickEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arMouseDoubleClickEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseDoubleClickEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arMouseDoubleClickEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::removeMouseDoubleClickEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arMouseDoubleClickEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseDoubleClickEventFunctions[idxFct];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arMouseDoubleClickEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::addMouseMoveEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arMouseMoveEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseMoveEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arMouseMoveEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::removeMouseMoveEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arMouseMoveEventFunctions.size(); idx++) {
//        SGraphObjMouseEventFct fctEntry = m_arMouseMoveEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arMouseMoveEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::addKeyPressEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arKeyPressEventFunctions.size(); idx++) {
//        SGraphObjKeyEventFct fctEntry = m_arKeyPressEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arKeyPressEventFunctions.append( SGraphObjKeyEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::removeKeyPressEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arKeyPressEventFunctions.size(); idx++) {
//        SGraphObjKeyEventFct fctEntry = m_arKeyPressEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arKeyPressEventFunctions.removeAt(idxFct);
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::addKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::addKeyReleaseEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    for (int idx = 0; idx < m_arKeyReleaseEventFunctions.size(); idx++) {
//        SGraphObjKeyEventFct fctEntry = m_arKeyReleaseEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, "Event function already added");
//        }
//    }
//    m_arKeyReleaseEventFunctions.append( SGraphObjKeyEventFct(i_pFct,i_pvThis,i_pvData) );
//}
//
////------------------------------------------------------------------------------
//void CGraphObj::removeKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::removeKeyReleaseEventFunction",
//        /* strAddInfo   */ strMthInArgs );
//
//    int idxFct = -1;
//    for (int idx = 0; idx < m_arKeyReleaseEventFunctions.size(); idx++) {
//        SGraphObjKeyEventFct fctEntry = m_arKeyReleaseEventFunctions[idx];
//        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
//            idxFct = idx;
//            break;
//        }
//    }
//    if (idxFct < 0) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultObjNotInList, "Event function not added");
//    }
//    m_arKeyReleaseEventFunctions.removeAt(idxFct);
//}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Called by the drawing scene if the drawing size is changed.

    When changing the drawing size in metric unit dimension
    (e.g. on changing the Y Scale Orientation) the scene coordinates must be
    newly calculated even if the original values stored in metric units have not
    been changed. On changing the drawing size the the drawing scene will emit
    the signal "drawingSizeChanged" and the method MUST set the flag
    "m_bForceConversionToSceneCoors" to true before converting the coordinates
    and setting the converted values.
*/
void CGraphObj::onDrawingSizeChanged(const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    m_bForceConversionToSceneCoors = true;

    // Here add code in your derived class to convert and recalculate the coordinates.
    // In the derived class the signal "geometryValuesUnitChanged" has to be emitted
    // if the unit of the drawing scene's size has been changed.

    m_bForceConversionToSceneCoors = false;
}

////------------------------------------------------------------------------------
//void CGraphObj::onGraphObjParentScenePosChanged(CGraphObj* i_pGraphObjParent)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_pGraphObjParent->keyInTree();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::onGraphObjParentScenePosChanged",
//        /* strAddInfo   */ strMthInArgs );
//}

//------------------------------------------------------------------------------
/*! @brief The slot method is called if the parent item of the item changes
           its geometry on the scene and emits the geometryOnSceneChanged signal.

    When resizing a group all children of the group should be resized and positioned so
    that they keep their relative positions and sizes within the group. If the item
    was added to a new group the current rectangle of the parent group was stored as
    the original group rectangle.

    If the item is removed from a group (but not added to a new group) the original
    parent group rectangle is invalidated.

    The default implementation just calculates the current scale factors of the
    parent group and emits the signal geometryOnSceneChanged.

    Labels override this method to udpate their positions on the scene and to update
    the geometry information of the items they are linked to but do not emit the
    geometryOnSceneChanged signal again.

    Other graphical objects must override this method to update their local graphics
    item coordinates and to update their position in the parent group.

    @note For usual standard shapes (not labels or selection points) this method must
          return immediately if the parent group is about to add another child
          (see flag m_iIgnoreParentGeometryChange).

    @param [in] i_pGraphObjParent
        Pointer to parent item whose geometry on the scene has been changed.
    @param [in] i_bParentOfParentChanged
        false (default), if the geometry of the parent has been changed directly.
        true if the geometry has been changed because the parent got a new parent.
*/
void CGraphObj::onGraphObjParentGeometryOnSceneChanged(
    CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged)
//------------------------------------------------------------------------------
{
    if (m_iIgnoreParentGeometryChange > 0) {
        return;
    }
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree() + ", ParentOfParentChanged: " + bool2Str(i_bParentOfParentChanged);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onGraphObjParentGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

#pragma message(__TODO__"Pure virtual")

    bool bGeometryOnSceneChanged = false;

    {   CRefCountGuard refCountGuardTracePositionInfo(&m_iTracePositionInfoBlockedCounter);

        if (i_pGraphObjParent->isGroup()) {
            CGraphObjGroup* pGraphObjGroupParent = dynamic_cast<CGraphObjGroup*>(i_pGraphObjParent);
            if (i_bParentOfParentChanged) {
                initParentTransform();
                updateTransformedCoorsOnParentGeometryChanged();
            }
            CPhysValRect physValRectGroupParentCurr = pGraphObjGroupParent->getRect(m_physValRectParentGroupOrig.unit());
            if (m_physValRectParentGroupOrig != physValRectGroupParentCurr) {
                if (m_physValRectParentGroupOrig.width().getVal() > 0.0) {
                    setParentGroupScaleX(physValRectGroupParentCurr.width().getVal() / m_physValRectParentGroupOrig.width().getVal());
                }
                if (m_physValRectParentGroupOrig.height().getVal() > 0.0) {
                    setParentGroupScaleY(physValRectGroupParentCurr.height().getVal() / m_physValRectParentGroupOrig.height().getVal());
                }
            }
        }
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);

    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
void CGraphObj::onGraphObjParentZValueChanged(CGraphObj* i_pGraphObjParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onGraphObjParentZValueChanged",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
void CGraphObj::onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pSelectionPoint->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onSelectionPointGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );
}

//------------------------------------------------------------------------------
/*! Informs the grapical object that a selection point is going to be destroyed.

    If currently objects are selected their selection points are created.
    On clearing the graphics scene all items including selection points will
    be destroyed. Selection points may be destroyed before its parent object
    the selection points belong to. The parent object got to be informed if the
    selection point will be destroyed by someone else.

    @param i_pSelectionPoint [in]
        Pointer to selection point which will be destroyed.
*/
void CGraphObj::onSelectionPointAboutToBeDestroyed(CGraphObj* i_pSelectionPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pSelectionPoint == nullptr ? "nullptr" : i_pSelectionPoint->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onSelectionPointAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    if (m_arpSelPtsBoundingRect.size() > 0) {
        for (int idxSelPt = m_arpSelPtsBoundingRect.size()-1; idxSelPt >= 0; idxSelPt--) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
            if (pGraphObjSelPt == i_pSelectionPoint) {
                m_arpSelPtsBoundingRect[idxSelPt] = nullptr;
            }
        }
    }
    if (m_arpSelPtsPolygon.size() > 0) {
        for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt--) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
            if (pGraphObjSelPt == i_pSelectionPoint) {
                m_arpSelPtsPolygon[idxSelPt] = nullptr;
            }
        }
    }
}

//------------------------------------------------------------------------------
/*! Informs the graphical object that one of its labels is going to be destroyed.

    On clearing the drawing scene all graphical objects will be destroyed.
    Labels may be destroyed before its parent object the labels belong to.
    The parent object got to be informed if the label will be destroyed by
    someone else.

    @param i_pLabel [in]
        Pointer to label which will be destroyed.
*/
void CGraphObj::onLabelAboutToBeDestroyed(CGraphObj* i_pLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pLabel == nullptr ? "nullptr" : i_pLabel->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onLabelAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLabel* pGraphObjLabel = dynamic_cast<CGraphObjLabel*>(i_pLabel);
    if( m_hshpLabels.contains(pGraphObjLabel->key())) {
        m_hshpLabels.remove(pGraphObjLabel->key());
    }
}

//------------------------------------------------------------------------------
/*! @brief Informs the graphical object that one of its labels is going to be destroyed.

    On clearing the drawing scene all graphical objects will be destroyed.
    Labels may be destroyed before its parent object the labels belong to.
    The parent object got to be informed if the label will be destroyed by
    someone else.

    @param i_pLabel [in]
        Pointer to label which will be destroyed.
*/
void CGraphObj::onGeometryLabelAboutToBeDestroyed(CGraphObj* i_pLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pLabel == nullptr ? "nullptr" : i_pLabel->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onGeometryLabelAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLabel* pGraphObjLabel = dynamic_cast<CGraphObjLabel*>(i_pLabel);
    if (m_hshpGeometryLabels.contains(pGraphObjLabel->key())) {
        m_hshpGeometryLabels.remove(pGraphObjLabel->key());
    }
}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::onActionFormatTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onActionFormatTriggered",
        /* strAddInfo   */ "" );

    openFormatGraphObjsDialog();
}

//------------------------------------------------------------------------------
void CGraphObj::onActionModifyPointsTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onActionModifyPointsTriggered",
        /* strAddInfo   */ "" );

    if (m_editMode == EEditMode::None) {
        setEditMode(EEditMode::ModifyingPolygonPoints);
    }
}

//------------------------------------------------------------------------------
void CGraphObj::onActionDeletePointTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onActionDeletePointTriggered",
        /* strAddInfo   */ "" );
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CGraphObj::blockItemChangeUpdatePhysValCoors(bool i_bBlock)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bBlock);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::blockItemChangeUpdatePhysValCoors",
        /* strAddInfo   */ strMthInArgs );

    int iCounterPrev = m_iItemChangeUpdatePhysValCoorsBlockedCounter;
    if (i_bBlock) {
        ++m_iItemChangeUpdatePhysValCoorsBlockedCounter;
    }
    else if (m_iItemChangeUpdatePhysValCoorsBlockedCounter <= 0) {
        QString strExcAddInfo = "m_iItemChangeUpdatePhysValCoorsBlockedCounter (" + QString::number(m_iItemChangeUpdatePhysValCoorsBlockedCounter) + ") <= 0";
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, strExcAddInfo);
    }
    else {
        --m_iItemChangeUpdatePhysValCoorsBlockedCounter;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(iCounterPrev);
    }
    return iCounterPrev;
}

//------------------------------------------------------------------------------
int CGraphObj::blockGeometryOnSceneChangedSignal(bool i_bBlock)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bBlock);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::blockGeometryOnSceneChangedSignal",
        /* strAddInfo   */ strMthInArgs );

    int iCounterPrev = m_iGeometryOnSceneChangedSignalBlockedCounter;
    if (i_bBlock) {
        ++m_iGeometryOnSceneChangedSignalBlockedCounter;
    }
    else if (m_iGeometryOnSceneChangedSignalBlockedCounter <= 0) {
        QString strExcAddInfo = "m_iGeometryOnSceneChangedSignalBlockedCounter (" + QString::number(m_iGeometryOnSceneChangedSignalBlockedCounter) + ") <= 0";
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, strExcAddInfo);
    }
    else {
        --m_iGeometryOnSceneChangedSignalBlockedCounter;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(iCounterPrev);
    }
    return iCounterPrev;
}

//------------------------------------------------------------------------------
/*  @brief Sets the flag that geometry changes of the parent group should be ignored
           and the item should not try to resposition and rescale itself it the
           geometry of the parent is changed.

    When adding a new child already existing childs should not calculate new positions
    and must not resize themselves if the geometry of the parent group is changed by
    adding the new child. The size of the already existing childs does not change.
    Only their position within the group. But the group will set the new position of
    the already existing childs. For this the childs must not react on the
    "parentGeometryOnSceneChanged" signal if the groups rectangle is set.

    Same applies if the parent is resized to its content.

    @param [in] i_bSet
        true, if the parent groups starts adding a child, false if the child has been added.

    @return Previous value of the flag.
*/
int CGraphObj::setIgnoreParentGeometryChange(bool i_bSet)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bSet);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setIgnoreParentGeometryChange",
        /* strAddInfo   */ strMthInArgs );

    int iIgnorePrev = m_iIgnoreParentGeometryChange;
    if (i_bSet) {
        ++m_iIgnoreParentGeometryChange;
    }
    else if (m_iIgnoreParentGeometryChange <= 0) {
        QString strExcAddInfo = "m_iIgnoreParentGeometryChange (" + QString::number(m_iIgnoreParentGeometryChange) + ") <= 0";
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, strExcAddInfo);
    }
    else {
        --m_iIgnoreParentGeometryChange;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(iIgnorePrev);
    }
    return iIgnorePrev;
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Initializes (resets) the items scale transformation parameters within its parent.

    The method saves the current pyhsical rectangle of the parent group as the parent's
    original, physical coordinates. Also the graphics item position within the parent
    is stored as the original position (relative to the origin (center point) of the
    parents bounding). The group scale parameters are reset to 1.0.

    Called by the onParentGroupChanged method if the parent is going to been changed.

    This method is also called by the parent group if the bounding rectangle
    of the parent group is changed on adding childs or resizing the parent
    group to its content as in this case the parent group is considered to
    bo no longer scaled but has got a new original size.
*/
void CGraphObj::initParentTransform()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::initParentTransform",
        /* strAddInfo   */ "" );
    traceParentGroupPositionInfo(mthTracer, EMethodDir::Enter);

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    CGraphObjGroup* pGraphObjGroupParent = parentGroup();
    if (pGraphObjGroupParent != nullptr) {
        setPhysValRectParentGroupOrig(pGraphObjGroupParent->getRect());
    }
    else {
        setPhysValRectParentGroupOrig(CPhysValRect(*m_pDrawingScene));
    }
    setParentGroupScaleX(1.0);
    setParentGroupScaleY(1.0);

    traceParentGroupPositionInfo(mthTracer, EMethodDir::Leave);
}

//------------------------------------------------------------------------------
/*! @brief Pure virtual method which must be overridden by derived classes to
           initialize the physical coordinates of the item depending on the items
           local coordinates.

    This method is also called if the parent group of the item is changed.
*/
void CGraphObj::updateTransformedCoorsOnParentChanged(
    CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "PrevGroup: " + QString(i_pGraphObjGroupPrev == nullptr ? "null" : i_pGraphObjGroupPrev->path()) +
            ", NewGroup: " + QString(i_pGraphObjGroupNew == nullptr ? "null" : i_pGraphObjGroupNew->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::updateTransformedCoorsOnParentChanged",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    #pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");

    // Before mapping to parent or scene, the rotation will be reset.
    // Otherwise transformed coordinates will be returned.
    // In addition itemChange will be called but should not emit the geometryOnSceneChanged signal.
    // The childs were not notified about the geometry change (see above) but has to reset
    // the scale transformation parameters of the parent.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
}

//------------------------------------------------------------------------------
/*! @brief Pure virtual method which must be overridden by derived classes to
           update the physical coordinates of the item depending on the items
           local coordinates.

    This method is also called by the parent group if the bounding rectangle
    of the parent group is changed on adding childs or resizing the parent
    group to its content. If the parent changes its size the position of the
    childs in pixels may not change. The group will force the child to update
    it's physical coordinates relative to either the top left or bottom
    left corner of the parents bounding rectangle.
*/
void CGraphObj::updateTransformedCoorsOnParentGeometryChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::updateTransformedCoorsOnParentGeometryChanged",
        /* strAddInfo   */ "" );
    traceThisPositionInfo(mthTracer, EMethodDir::Enter);

    #pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");

    // Before mapping to parent or scene, the rotation will be reset.
    // Otherwise transformed coordinates will be returned.
    // In addition itemChange will be called but should not emit the geometryOnSceneChanged signal.
    // The childs were not notified about the geometry change (see above) but has to reset
    // the scale transformation parameters of the parent.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

    }
    traceThisPositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Pure virtual method which must be overridden by derived classes to
           update the physical coordinates of the item depending on the items
           local coordinates.

    Called by the itemChange method if the position of the item has been changed.
*/
void CGraphObj::updateTransformedCoorsOnItemPositionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::updateTransformedCoorsOnItemPositionChanged",
        /* strAddInfo   */ "" );
    traceThisPositionInfo(mthTracer, EMethodDir::Enter);

    #pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");

    traceThisPositionInfo(mthTracer, EMethodDir::Leave);
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the effective (resulting) bounding rectangle of the given
           physical rectangle (which may be rotated) on the drawing scene.

    To get the effective bounding rectangle the left most, the right most
    as well as the top most and bottom most points of the transformed
    (rotated and scaled) physical rectangle are are taken into account.

    @param [in] i_physValRectBounding
        Scaled and rotated, physical rectangle given in parent or scene coordinates.

    @return Bounding rectangle in scene coordinates.
*/
QRectF CGraphObj::getEffectiveBoundingRectOnScene(const CPhysValRect& i_physValRectBounding) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValRectBounding.toString() + "} " + i_physValRectBounding.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::getEffectiveBoundingRectOnScene",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = i_physValRectBounding;
    if (parentGroup() != nullptr) {
        physValRect = parentGroup()->convert(physValRect, Units.Length.px);
        physValRect = parentGroup()->mapToScene(physValRect);
    }
    else {
        physValRect = m_pDrawingScene->convert(physValRect, Units.Length.px);
    }
    CPhysValPoint physValPointTL = physValRect.topLeft();
    CPhysValPoint physValPointTR = physValRect.topRight();
    CPhysValPoint physValPointBR = physValRect.bottomRight();
    CPhysValPoint physValPointBL = physValRect.bottomLeft();
    QPointF ptTL = physValPointTL.toQPointF();
    QPointF ptTR = physValPointTR.toQPointF();
    QPointF ptBR = physValPointBR.toQPointF();
    QPointF ptBL = physValPointBL.toQPointF();
    QPolygonF plg({ptTL, ptTR, ptBR, ptBL});
    QRectF rctBounding = plg.boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

/*==============================================================================
protected: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObj::updateTransform()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::updateTransform",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr)
//    {
//        QTransform trs;
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        trs.translate( m_ptRotOriginCurr.x(), m_ptRotOriginCurr.y() );
//        trs.rotate(-m_fRotAngleCurr_deg);
//        trs.translate( -m_ptRotOriginCurr.x(), -m_ptRotOriginCurr.y() );
//        //trsNew.scale( m_fScaleFacXCurr, m_fScaleFacYCurr );
//#endif
//        pGraphicsItem->resetTransform();
//        pGraphicsItem->setTransform(trs);
//    }
//} // updateTransform

////------------------------------------------------------------------------------
//void CGraphObj::updateToolTip()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::updateToolTip",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr)
//    {
//        m_strToolTip = "ObjPath:\t\t" + path();
//
//        // "scenePos" returns mapToScene(0,0). This is NOT equivalent to the
//        // position of the item's top left corner before applying the rotation
//        // transformation matrix but includes the transformation. What we want
//        // (or what I want) is the position of the item before rotating the item
//        // around the rotation origin point. In contrary it looks like "pos"
//        // always returns the top left corner before rotating the object.
//
//        QPointF ptPos;
//        if (pGraphicsItem->parentItem() != nullptr) {
//            ptPos = pGraphicsItem->pos();
//            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
//        }
//        else {
//            ptPos = pGraphicsItem->pos(); // don't use "scenePos" here (see comment above)
//            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
//        }
//
//        m_strToolTip += "\nSize:\t\t" + getSize(Units.Length.px).toString();
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + c_chSymbolDegree;
//#endif
//        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());
//
//        pGraphicsItem->setToolTip(m_strToolTip);
//    }
//} // updateToolTip

////------------------------------------------------------------------------------
//void CGraphObj::updateEditInfo()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::updateEditInfo",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr)
//    {
//        QString strAngleSymbol = QString(QChar(8738));
//        m_strEditInfo  = "C:" + point2Str( pGraphicsItem->mapToScene(pGraphicsItem->boundingRect().center()) );
//        m_strEditInfo += ", W:" + QString::number(pGraphicsItem->boundingRect().width(),'f',1);
//        m_strEditInfo += ", H:" + QString::number(pGraphicsItem->boundingRect().height(),'f',1);
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        m_strEditInfo += ", " + strAngleSymbol + ":" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + c_chSymbolDegree;
//#endif
//    }
//}

/*==============================================================================
protected: // overridable auxiliary instance methods (method tracing)
==============================================================================*/

////------------------------------------------------------------------------------
///*! @brief Internal method to calculate and set the current scene position of the object.
//*/
//void CGraphObj::updateInternalScenePos()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::updateInternalScenePos",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//    if (pGraphicsItem != nullptr) {
//        QPointF ptScenePos = pGraphicsItem->scenePos();
//        setInternalScenePos(ptScenePos);
//    }
//}

////------------------------------------------------------------------------------
///*! @brief Internal method to set the current scene position of the object.
//
//    If the current scene position has been changed the scenePosChanged signal
//    is emitted.
//
//    @param [in] i_pos
//        New scene position of the object.
//*/
//void CGraphObj::setInternalScenePos(const QPointF& i_pos)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = point2Str(i_pos);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::setInternalScenePos",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_ptScenePos != i_pos) {
//        m_ptScenePos = i_pos;
//        emit_scenePosChanged();
//    }
//}

//------------------------------------------------------------------------------
/*! @brief Internal method to trace setting the parent group's physical rectangle.

    The original parent group's physical rectangle is invalid as long as the item is not
    added to a parent group or the parents physical rectangle at the time the item is
    added to a parent.

    @param [in] i_physValRect
        The parent's physical rectangle.

    @return Previous original parent's physical rectangle.
*/
CPhysValRect CGraphObj::setPhysValRectParentGroupOrig(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setPhysValRectParentGroupOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectParentGroupOrig;
    m_physValRectParentGroupOrig = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method to trace setting the current scale factor of the parent group.

    The scale factor is set each time the parent is resized. The geometry of the child
    has to be updated to the scale factors by keeping their relative distance to the
    center point of the parent's bounding rectangle. Without scale factors it would not
    be possible to resize groups by moving corner points beyond their opposite corners
    as this may temporarily result in width and height of 0 pixels. Afterwards the child
    may not resize themselves if the parent is resized (division by zero).

    @param [in] i_fScaleX
        The parent's X scale factor.

    @return Previous scale factor.
*/
double CGraphObj::setParentGroupScaleX(double i_fScaleX)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New: " + QString::number(i_fScaleX, 'f', 3);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setParentGroupScaleX",
        /* strAddInfo   */ strMthInArgs );

    double fScaleXPrev = m_fParentGroupScaleX;
    m_fParentGroupScaleX = i_fScaleX;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fScaleXPrev, 'f', 3));
    }
    return fScaleXPrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method to trace setting the current scale factor of the parent group.

    The scale factor is set each time the parent is resized. The geometry of the child
    has to be updated to the scale factors by keeping their relative distance to the
    center point of the parent's bounding rectangle. Without scale factors it would not
    be possible to resize groups by moving corner points beyond their opposite corners
    as this may temporarily result in width and height of 0 pixels. Afterwards the child
    may not resize themselves if the parent is resized (division by zero).

    @param [in] i_fScaleY
        The parent's Y scale factor.

    @return Previous scale factor.
*/
double CGraphObj::setParentGroupScaleY(double i_fScaleY)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New: " + QString::number(i_fScaleY, 'f', 3);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::setParentGroupScaleY",
        /* strAddInfo   */ strMthInArgs );

    double fScaleYPrev = m_fParentGroupScaleY;
    m_fParentGroupScaleY = i_fScaleY;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fScaleYPrev, 'f', 3));
    }
    return fScaleYPrev;
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*!< @brief Emits the aboutToBeDestroyed signal if the signal has not already
            been emitted.

     Only the top most class in the hierachy of class inheritance should emit the signal.
     For this the signal may not be emitted "directly" by the derived classes but the class
     must call "emit_aboutToBeDestroyed" in their destructors.
*/
void CGraphObj::emit_aboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    if (!m_bAboutToBeDestroyedEmitted) {
        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObjItemChange,
            /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strObjName   */ path(),
            /* strMethod    */ "CGraphObj::emit_aboutToBeDestroyed",
            /* strAddInfo   */ "" );
        emit aboutToBeDestroyed(this);
        m_bAboutToBeDestroyedEmitted = true;
    }
}

//------------------------------------------------------------------------------
void CGraphObj::emit_typeChanged(EGraphObjType i_graphObjType)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = graphObjType2Str(i_graphObjType);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_typeChanged",
        /* strAddInfo   */ strMthInArgs );
    emit typeChanged(this, i_graphObjType);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_editModeChanged(const CEnumEditMode& i_eMode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eMode.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_editModeChanged",
        /* strAddInfo   */ strMthInArgs );
    emit editModeChanged(this, i_eMode);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_selectedChanged(bool i_bIsSelected)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bIsSelected);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_selectedChanged",
        /* strAddInfo   */ strMthInArgs );
    emit selectedChanged(this, i_bIsSelected);
}

////------------------------------------------------------------------------------
//void CGraphObj::emit_scenePosChanged()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::emit_scenePosChanged",
//        /* strAddInfo   */ "" );
//    emit scenePosChanged(this);
//}

//------------------------------------------------------------------------------
/*! @brief Emits the "geometryOnSceneChanged" signal (with optional trace output).

    The signal is only emitted if not blocked (BlockedCounter == 0).
*/
void CGraphObj::emit_geometryOnSceneChanged(bool i_bParentOfParentChanged)
//------------------------------------------------------------------------------
{
    if (m_iGeometryOnSceneChangedSignalBlockedCounter > 0) {
        return;
    }
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ParentOfParentChanged: " + bool2Str(i_bParentOfParentChanged);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_geometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );
    emit geometryOnSceneChanged(this, i_bParentOfParentChanged);
}

////------------------------------------------------------------------------------
//void CGraphObj::emit_geometryValuesUnitChanged()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "CGraphObj::emit_geometryValuesUnitChanged",
//        /* strAddInfo   */ "" );
//    emit geometryValuesUnitChanged(this);
//}

//------------------------------------------------------------------------------
void CGraphObj::emit_zValueChanged(double i_fZValue)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fZValue);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_zValueChanged",
        /* strAddInfo   */ strMthInArgs );
    emit zValueChanged(this, i_fZValue);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_drawSettingsChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_drawSettingsChanged",
        /* strAddInfo   */ "" );
    emit drawSettingsChanged(this);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_labelAdded(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_labelAdded",
        /* strAddInfo   */ strMthInArgs );
    emit labelAdded(this, i_strName);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_labelRemoved(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_labelRemoved",
        /* strAddInfo   */ strMthInArgs );
    emit labelRemoved(this, i_strName);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_labelRenamed(const QString& i_strName, const QString& i_strNameNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName + ", " + i_strNameNew;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_labelRenamed",
        /* strAddInfo   */ strMthInArgs );
    emit labelRenamed(this, i_strName, i_strNameNew);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_labelChanged(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_labelChanged",
        /* strAddInfo   */ strMthInArgs );
    emit labelChanged(this, i_strName);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_geometryLabelAdded(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_geometryLabelAdded",
        /* strAddInfo   */ strMthInArgs );
    emit geometryLabelAdded(this, i_strName);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_geometryLabelRemoved(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_geometryLabelRemoved",
        /* strAddInfo   */ strMthInArgs );
    emit geometryLabelRemoved(this, i_strName);
}

//------------------------------------------------------------------------------
void CGraphObj::emit_geometryLabelChanged(const QString& i_strName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::emit_geometryLabelChanged",
        /* strAddInfo   */ strMthInArgs );
    emit geometryLabelChanged(this, i_strName);
}

/*==============================================================================
protected: // overridable auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the prepareGeometryChange method of
           graphics item to trace the method call.

    As the prepareGeometryChange method is a protected method of QGraphicsItem
    this method must be reimplemented by the derived classes.
*/
void CGraphObj::QGraphicsItem_prepareGeometryChange()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::QGraphicsItem_prepareGeometryChange",
        /* strAddInfo   */ "" );

    // As the prepareGeometryChange method is a protected method of QGraphicsItem
    // this method must be reimplemented by the derived classes.
    #pragma message(__TODO__"Pure virtual")
    throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "Should become pure virtual");

    //QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    //if (pGraphicsItemThis != nullptr) {
    //    pGraphicsItemThis->prepareGeometryChange();
    //}
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the setPos method of graphics item to
           trace the method call.

    @param [in] i_pos
        New position of the object in parent coordinates.
*/
QPointF CGraphObj::QGraphicsItem_setPos(const QPointF& i_pos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pos {" + qPoint2Str(i_pos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::QGraphicsItem_setPos",
        /* strAddInfo   */ strMthInArgs );

    QPointF posPrev;
    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        posPrev = pGraphicsItemThis->pos();
        pGraphicsItemThis->setPos(i_pos);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qPoint2Str(posPrev) + "}");
    }
    return posPrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the setScale method of graphics item to
           trace the method call.

    @param [in] i_fFactor
*/
double CGraphObj::QGraphicsItem_setScale(double i_fFactor)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Factor: " + QString::number(i_fFactor) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::QGraphicsItem_setScale",
        /* strAddInfo   */ strMthInArgs );

    double fFactorPrev = 1.0;
    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        fFactorPrev = pGraphicsItemThis->scale();
        pGraphicsItemThis->setScale(i_fFactor);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fFactorPrev, 'f', 3));
    }
    return fFactorPrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the setRotation method of graphics item to
           trace the method call.

    @param [in] i_fAngle_degree
*/
double CGraphObj::QGraphicsItem_setRotation(double i_fAngle_degree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Angle: " + QString::number(i_fAngle_degree) + " " + QString(Math::c_chSymbolDegree);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::QGraphicsItem_setRotation",
        /* strAddInfo   */ strMthInArgs );

    double fAnglePrev_degree = 0.0;
    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        fAnglePrev_degree = pGraphicsItemThis->rotation();
        pGraphicsItemThis->setRotation(i_fAngle_degree);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString::number(fAnglePrev_degree, 'f', 3) + " " + QString(Math::c_chSymbolDegree));
    }
    return fAnglePrev_degree;
}

//------------------------------------------------------------------------------
QGraphicsItem* CGraphObj::QGraphicsItem_setParentItem(QGraphicsItem* i_pGraphicsItemParent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObjGroup* pGraphObjGroupParent = dynamic_cast<CGraphObjGroup*>(i_pGraphicsItemParent);
        strMthInArgs = "ParentGroup: " + QString(pGraphObjGroupParent == nullptr ? "null" : pGraphObjGroupParent->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::QGraphicsItem_setParentItem",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjGroup* pGraphObjGroupParentPrev = nullptr;
    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        pGraphObjGroupParentPrev = dynamic_cast<CGraphObjGroup*>(pGraphicsItemThis->parentItem());
        pGraphicsItemThis->setParentItem(i_pGraphicsItemParent);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev: " + QString(pGraphObjGroupParentPrev == nullptr ? "null" : pGraphObjGroupParentPrev->path()));
    }
    return pGraphObjGroupParentPrev;
}

//------------------------------------------------------------------------------
void CGraphObj::QGraphicsItem_setCursor(const QCursor& i_cursor)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCursorShape2Str(i_cursor.shape());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::QGraphicsItem_setCursor",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        pGraphicsItemThis->setCursor(i_cursor);
    }
}

//------------------------------------------------------------------------------
void CGraphObj::QGraphicsItem_unsetCursor()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::QGraphicsItem_unsetCursor",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        pGraphicsItemThis->unsetCursor();
    }
}

/*==============================================================================
protected: // overridable auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::tracePositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir,
    const QString& i_strFilter, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0) {
        return;
    }
    traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter, i_detailLevel);
    traceParentGroupPositionInfo(i_mthTracer, i_mthDir, i_strFilter, i_detailLevel);
}

//------------------------------------------------------------------------------
void CGraphObj::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir,
    const QString& i_strFilter, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        QPointF ptPos = pGraphicsItemThis->pos();
        QPointF ptScenePos = pGraphicsItemThis->scenePos();
        QRectF rectBounding = getBoundingRect();
        QPointF ptCenterPos = rectBounding.center();
        QString strRuntimeInfo;
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "RotationAngle: " + m_physValRotationAngle.toString();
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "BoundingRect {" + qRect2Str(rectBounding) + "}, Center {" + qPoint2Str(ptCenterPos) + "}";
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "Pos {" + qPoint2Str(ptPos) + "}, ScenePos {" + qPoint2Str(ptScenePos) + "}";
        i_mthTracer.trace(strRuntimeInfo);
    }
}

//------------------------------------------------------------------------------
void CGraphObj::traceParentGroupPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir,
    const QString& i_strFilter, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceParentGroupPositionInfoInfoBlockedCounter > 0) {
        return;
    }
    QGraphicsItem* pGraphicsItemGroupParent = dynamic_cast<QGraphicsItemGroup*>(m_pGraphObjGroupParent);
    //CGraphObjGroup* pGraphObjGroupParent = parentGroup();
    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = "   ";
    strRuntimeInfo += "ParentGroup: " + QString(m_pGraphObjGroupParent == nullptr ? "null" : m_pGraphObjGroupParent->path());
    if (pGraphicsItemGroupParent == nullptr && m_pGraphObjGroupParent != nullptr) {
        strRuntimeInfo += ", !! ParentGraphicsItem: null !!";
    }
    i_mthTracer.trace(strRuntimeInfo);
    if (pGraphicsItemGroupParent != nullptr || m_pGraphObjGroupParent != nullptr) {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += " . ParentTransform PhysValRectOrig {" + m_physValRectParentGroupOrig.toString() + " " + m_physValRectParentGroupOrig.unit().symbol() + "}" +
            ", Scale {X: " + QString::number(m_fParentGroupScaleX, 'f', 3) + ", Y: " + QString::number(m_fParentGroupScaleY, 'f', 3) + "}";
        i_mthTracer.trace(strRuntimeInfo);
        QPointF ptPos = pGraphicsItemGroupParent == nullptr ? QPointF() : pGraphicsItemGroupParent->pos();
        QPointF ptScenePos = pGraphicsItemGroupParent == nullptr ? QPointF() : pGraphicsItemGroupParent->scenePos();
        QRectF rectBounding = m_pGraphObjGroupParent == nullptr ? QRectF() : m_pGraphObjGroupParent->getBoundingRect();
        QPointF ptCenterPos = rectBounding.center();
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += " . Parent PhysValRect {" + QString(m_pGraphObjGroupParent == nullptr ? "null" : m_pGraphObjGroupParent->getRect().toString()) + "}";
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += " .. BoundingRect {" + qRect2Str(rectBounding) + "}, Center {" + qPoint2Str(ptCenterPos) + "}";
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += " .. ItemPos {" + qPoint2Str(ptPos) + "}, ScenePos {" + qPoint2Str(ptScenePos) + "}";
        i_mthTracer.trace(strRuntimeInfo);
    }
}

//------------------------------------------------------------------------------
void CGraphObj::traceGraphicsItemStates(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir,
    const QString& i_strFilter, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTraceGraphicsItemStatesInfoBlockedCounter > 0) {
        return;
    }
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        QString strRuntimeInfo;
        if (i_strFilter.isEmpty() || i_strFilter.contains("Common")) {
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
            CGraphObj* pGraphObjMouseGrabber = dynamic_cast<CGraphObj*>(pGraphicsItemMouseGrabber);
            strRuntimeInfo += "IsSelected: " + bool2Str(pGraphicsItemThis->isSelected()) +
                ", IsVisible: " + bool2Str(pGraphicsItemThis->isVisible()) +
                ", IsEnabled: " + bool2Str(pGraphicsItemThis->isEnabled()) +
                ", MouseGrabber: " + QString(pGraphObjMouseGrabber == nullptr ? "null" : pGraphObjMouseGrabber->path());
            i_mthTracer.trace(strRuntimeInfo);
        }
        if (i_strFilter.isEmpty() || i_strFilter.contains("HoverEvents")) {
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "AcceptHoverEvents: " + bool2Str(pGraphicsItemThis->acceptHoverEvents());
            i_mthTracer.trace(strRuntimeInfo);
        }
        if (i_strFilter.isEmpty() || i_strFilter.contains("MouseButtons")) {
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "AcceptedMouseButtons {" + qMouseButtons2Str(pGraphicsItemThis->acceptedMouseButtons()) + "}";
            i_mthTracer.trace(strRuntimeInfo);
        }
        if (i_strFilter.isEmpty() || i_strFilter.contains("Flags")) {
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "Flags {" + qGraphicsItemFlags2Str(pGraphicsItemThis->flags()) + "}";
            i_mthTracer.trace(strRuntimeInfo);
        }
    }
}

//------------------------------------------------------------------------------
void CGraphObj::traceGraphObjStates(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir,
    const QString& i_strFilter, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTraceGraphObjStatesInfoBlockedCounter > 0) {
        return;
    }
    QString strRuntimeInfo;
    if (i_strFilter.isEmpty() || i_strFilter.contains("Common")) {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "EditMode: " + m_editMode.toString() +
            ", IsHighlighted: " + bool2Str(m_bIsHighlighted) +
            ", ForceConversion: " + bool2Str(m_bForceConversionToSceneCoors);
        i_mthTracer.trace(strRuntimeInfo);
    }
    if (i_strFilter.isEmpty() || i_strFilter.contains("BlockedCounter")) {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "SignalBlockedCounter {ItemChange: " + QString::number(m_iItemChangeBlockedCounter) +
            ", ItemChangeUpdatePhysValCoors: " + QString::number(m_iItemChangeUpdatePhysValCoorsBlockedCounter) +
            ", GeometryOnSceneChanged: " + QString::number(m_iGeometryOnSceneChangedSignalBlockedCounter) +
            ", IgnoreParentGeometryChange: " + QString::number(m_iIgnoreParentGeometryChange) + "}";
        i_mthTracer.trace(strRuntimeInfo);
    }
}
