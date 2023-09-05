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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/QBitmap>
#include <QtGui/QPainter>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*==============================================================================
struct SGraphObjHitInfo
==============================================================================*/

/* public: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
SGraphObjHitInfo::SGraphObjHitInfo() :
//------------------------------------------------------------------------------
    m_editMode(EEditMode::None),
    m_editResizeMode(EEditResizeMode::None),
    m_selPtBoundingRect(ESelectionPoint::None),
    m_idxPolygonShapePoint(-1),
    m_idxLineSegment(-1),
    m_ptSelected(),
    m_cursor()
{
} // ctor

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isBoundingRectSelectionPointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;
    if (m_editMode == EEditMode::Resize && m_editResizeMode != EEditResizeMode::None) {
        bIsHit = (m_selPtBoundingRect >= ESelectionPointRectMin) && (m_selPtBoundingRect <= ESelectionPointRectMax);
    }
    return bIsHit;
}

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isSelectionPointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;
    if (m_editMode != EEditMode::None) {
        bIsHit = (m_selPtBoundingRect != ESelectionPoint::None);
    }
    return bIsHit;
}

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isPolygonShapePointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;
    if (m_editMode == EEditMode::MoveShapePoint) {
        bIsHit = (m_idxPolygonShapePoint >= 0);
    }
    return bIsHit;
}

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isLineSegmentHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;
    if (m_editMode != EEditMode::None) {
        bIsHit = (m_idxLineSegment >= 0);
    }
    return bIsHit;
}

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

    Calculated depending on editMode, editResizeMode and selected point
    (which must have been set before).
*/
void SGraphObjHitInfo::setCursor(double i_fGraphObjRotAngle_rad)
//------------------------------------------------------------------------------
{
    double fCursorAngle_rad = 0.0;
    bool   bSetResizeCursor = false;

    switch (m_editMode.enumerator())
    {
        case EEditMode::Move:
        {
            m_cursor = Qt::SizeAllCursor;
            break;
        }
        case EEditMode::Resize:
        {
            switch( m_editResizeMode.enumerator() )
            {
                case EEditResizeMode::ResizeAll:
                {
                    switch( m_selPtBoundingRect.enumerator() )
                    {
                        case ESelectionPoint::TopLeft:
                        case ESelectionPoint::BottomRight:
                        {
                            fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f135Degrees_rad; // 2nd quadrant: arrow from right/bottom -> top/left
                            m_cursor = Qt::SizeFDiagCursor; /*  \  */
                            bSetResizeCursor = true;
                            break;
                        }
                        case ESelectionPoint::TopRight:
                        case ESelectionPoint::BottomLeft:
                        {
                            fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f45Degrees_rad; // 1st quadrant: arrow from bottom/left -> top/right
                            m_cursor = Qt::SizeBDiagCursor; /*  /  */
                            bSetResizeCursor = true;
                            break;
                        }
                        case ESelectionPoint::RotateTop:
                        case ESelectionPoint::RotateBottom:
                        {
                            QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.bmp");
                            QBitmap bmpCursorMask = bmpCursor.createHeuristicMask();
                            m_cursor = QCursor(bmpCursor,bmpCursorMask);
                            break;
                        }
                        case ESelectionPoint::LeftCenter:
                        case ESelectionPoint::RightCenter:
                        case ESelectionPoint::TopCenter:
                        case ESelectionPoint::BottomCenter:
                        case ESelectionPoint::Center:
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case EEditResizeMode::ResizeHor:
                {
                    fCursorAngle_rad = i_fGraphObjRotAngle_rad;
                    m_cursor = Qt::SizeHorCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case EEditResizeMode::ResizeVer:
                {
                    fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f90Degrees_rad;
                    m_cursor = Qt::SizeVerCursor;
                    bSetResizeCursor = true;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case EEditMode::Rotate:
        {
            QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.bmp");
            QBitmap bmpCursorMask = bmpCursor.createHeuristicMask();
            m_cursor = QCursor(bmpCursor,bmpCursorMask);
            break;
        }
        case EEditMode::MoveShapePoint:
        {
            m_cursor = Qt::CrossCursor;
            break;
        }
        case EEditMode::EditText:
        {
            m_cursor = Qt::IBeamCursor;
            break;
        }
        default:
        {
            break;
        }
    } // switch( editMode )

    if( bSetResizeCursor )
    {
        // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180�)
        while( fCursorAngle_rad >= Math::c_f180Degrees_rad )
        {
            fCursorAngle_rad -= Math::c_f180Degrees_rad;
        }
        while( fCursorAngle_rad < 0.0 )
        {
            fCursorAngle_rad += Math::c_f180Degrees_rad;
        }
        if( fCursorAngle_rad >= 0.0 && fCursorAngle_rad < Math::c_f45Degrees_rad/2.0 ) // 0.0 .. 22.5�
        {
            m_cursor = Qt::SizeHorCursor;
        }
        else if( fCursorAngle_rad >= Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 3.0*Math::c_f45Degrees_rad/2.0 ) // 22.5� .. 67.5�
        {
            m_cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
        }
        else if( fCursorAngle_rad >= 3.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 5.0*Math::c_f45Degrees_rad/2.0 ) // 67.5� .. 112.5�
        {
            m_cursor = Qt::SizeVerCursor;
        }
        else if( fCursorAngle_rad >= 5.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 7.0*Math::c_f45Degrees_rad/2.0 ) // 112.5� .. 157.5�
        {
            m_cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
        }
        else if( fCursorAngle_rad >= 7.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < Math::c_f180Degrees_rad ) // 157.5� .. 180.0�
        {
            m_cursor = Qt::SizeHorCursor;
        }
    } // if( bSetResizeCursor )
} // setCursor

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

*/
QString SGraphObjHitInfo::toString() const
//------------------------------------------------------------------------------
{
    QString str = "EditMode:" + m_editMode.toString() +
        ", ResizeMode:" + m_editResizeMode.toString() +
        ", SelPtBoundingRect:" + m_selPtBoundingRect.toString() +
        ", PolygonShapePoint:" + QString::number(m_idxPolygonShapePoint) +
        ", LineSegment:" + QString::number(m_idxLineSegment) +
        ", PointSelected:" + point2Str(m_ptSelected) +
        ", Cursor:" + qCursorShape2Str(m_cursor.shape());
    return str;
}

/*==============================================================================
struct SGraphObjAlignment
==============================================================================*/

//------------------------------------------------------------------------------
QString SGraphObjAlignment::toString() const
//------------------------------------------------------------------------------
{
    QString str;
    str  = "RefChild:" + m_alignmentRefChild.toString();
    str += ", RefParent:" + m_alignmentRefParent.toString();
    str += ", Absolute:" + bool2Str(m_bAlignAbsolute);
    str += ", Val:" + QString::number(m_fVal);
    return str;
}



/*******************************************************************************
class CGraphObj
*******************************************************************************/

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates a graphical object instance.

    The ctor does neither add the instance as a graphic item to Qt's grapics scene
    nor to the index tree of the drawing scene.

    @param i_pDrawingScene [in]
        Pointer to drawing scene the object will be added.
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
    const CDrawSettings& i_settings,
    CIdxTreeEntry::EEntryType i_idxTreeEntryType ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(i_idxTreeEntryType, i_strObjName),
    m_bDtorInProgress(false),
    m_pDrawingScene(i_pDrawingScene),
    m_strFactoryGroupName(i_strFactoryGroupName),
    m_type(i_type),
    m_strType(i_strType),
    //m_strDescription(),
    m_drawSettings(i_type),
    m_physValSizeMinimum(),
    m_physValSizeMaximum(),
    m_physValSizeFixed(),
    m_arAlignments(),
    m_bIsHit(false),
    m_editMode(EEditMode::None),
    m_editResizeMode(EEditResizeMode::None),
    m_fZValue(0.0),
    m_bBoundRectVisible(false),
    m_idxSelPtSelectedPolygon(-1),
    m_arpSelPtsPolygon(),
    m_selPtSelectedBoundingRect(ESelectionPoint::None),
    m_arpSelPtsBoundingRect(CEnumSelectionPoint::count()),
    m_pNameLabel(nullptr),
    m_arpLabels(),
    m_arpPosLabels(),
    m_arpDimLineLabels(),
    m_strToolTip(),
    m_strEditInfo(),
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_bCoorsDirty(true),
    m_rctCurr(),
    m_fRotAngleCurr_deg(0.0),
    m_ptRotOriginCurr(),
    // Original coordinates and transform values:
    m_bHasValidOrigCoors(false),
    m_ptPosOrig(),
    m_sizOrig(),
    m_fRotAngleOrig_deg(0.0),
    m_ptRotOriginOrig(),
#endif
    // Coordinates stored on mouse press events:
    m_ptScenePosOnMousePressEvent(),
    m_ptMouseEvScenePosOnMousePressEvent(),
    m_rctOnMousePressEvent(),
    m_ptRotOriginOnMousePressEvent(),
    // Simulation Functions:
    m_arMousePressEventFunctions(),
    m_arMouseReleaseEventFunctions(),
    m_arMouseDoubleClickEventFunctions(),
    m_arMouseMoveEventFunctions(),
    m_arKeyPressEventFunctions(),
    m_arKeyReleaseEventFunctions(),
    m_pTrcAdminObjCtorsAndDtor(nullptr),
    m_pTrcAdminObjItemChange(nullptr),
    m_pTrcAdminObjBoundingRect(nullptr),
    m_pTrcAdminObjIsHit(nullptr),
    m_pTrcAdminObjPaint(nullptr),
    m_pTrcAdminObjSceneEvent(nullptr),
    m_pTrcAdminObjSceneEventFilter(nullptr),
    m_pTrcAdminObjHoverEvents(nullptr),
    m_pTrcAdminObjMouseEvents(nullptr),
    m_pTrcAdminObjKeyEvents(nullptr)
{
    for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
        if (m_drawSettings.isAttributeUsed(idxAttr)) {
            m_drawSettings.setAttribute(idxAttr, i_settings.getAttribute(idxAttr));
        }
    }
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj::~CGraphObj()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;

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

    // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
    // dtor of QGraphicsItem has already been executed. The desctructor of the
    // derived classes is called before the destructor of the base classes.
    // The base classes are CGraphObj and QGraphicsItem. The order the dtors
    // of inherited classes are called depend on the order the classes
    // appear in the list of the inherited classes on defining the
    // class implementation. So we can't call "removeItem" here as the
    // the destructor of QGraphicsItem may have already been called. The
    // graphics item must be removed from the drawing scene's item list before
    // the dtor of class QGraphicsItem is called. And this is only always
    // the case in the dtor of the class derived from QGraphicsItem.
    //
    // Inheritance Graph:
    //
    //  +---------------+     +----------------+
    //  | CIdxTreeEntry |     | QGraphicsItem  |
    //  +---------------+     +----------------+
    //          +                      +
    //          |                      |
    //  +---------------+    +-------------------+
    //  |   CGraphObj   |    | QGraphicsLineItem |
    //  +---------------+    +-------------------+
    //             +             +
    //              \           /
    //           +-----------------+
    //           |  CGraphObjLine  |
    //           +-----------------+

    // Must be called in derived classes:
    //destroyLabels();

    //if( m_pDrawingScene != nullptr )
    //{
    //    // Please note that "onGraphObjAboutToDestroyed" is used to remove the graphical object from
    //    // the dictionary, the index list, and the sorted object pools of the drawing scene.
    //    // But selection points have just been added to the drawing scene's item list and
    //    // therefore don't have valid object ids.
    //    if( !m_strKeyInTree.isEmpty() )
    //    {
    //        try
    //        {
    //            m_pDrawingScene->onGraphObjAboutToDestroyed(m_strKeyInTree);
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
    m_pDrawingScene = nullptr;
    //m_strFactoryGroupName;
    m_type = static_cast<EGraphObjType>(0);
    //m_strType;
    //m_strDescription;
    //m_drawSettings;
    //m_physValSizeMinimum;
    //m_physValSizeMaximum;
    //m_physValSizeFixed;
    //m_arAlignments;
    m_bIsHit = false;
    m_editMode = static_cast<EEditMode>(0);
    m_editResizeMode = static_cast<EEditResizeMode>(0);
    m_fZValue = 0.0;
    m_bBoundRectVisible = false;
    m_idxSelPtSelectedPolygon = 0;
    //m_arpSelPtsPolygon;
    m_selPtSelectedBoundingRect = static_cast<ESelectionPoint>(0);
    //m_arpSelPtsBoundingRect;
    //m_arpNameLabels;
    //m_arpPosLabels;
    //m_arpDimLineLabels;
    //m_strToolTip;
    //m_strEditInfo;
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_bCoorsDirty = false;
    //m_rctCurr;
    m_fRotAngleCurr_deg = 0.0;
    //m_ptRotOriginCurr;
    // Original item coordinates and transform values:
    m_bHasValidOrigCoors = false;
    //m_ptPosOrig;
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
    // Method Tracing
    m_pTrcAdminObjCtorsAndDtor = nullptr;
    m_pTrcAdminObjItemChange = nullptr;
    m_pTrcAdminObjBoundingRect = nullptr;
    m_pTrcAdminObjIsHit = nullptr;
    m_pTrcAdminObjPaint = nullptr;
    m_pTrcAdminObjSceneEvent = nullptr;
    m_pTrcAdminObjSceneEventFilter = nullptr;
    m_pTrcAdminObjHoverEvents = nullptr;
    m_pTrcAdminObjMouseEvents = nullptr;
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
    m_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::CtorsAndDtor");
    m_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::ItemChange");
    m_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::BoundingRect");
    m_pTrcAdminObjIsHit = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::IsHit");
    m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::Paint");
    m_pTrcAdminObjSceneEvent = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::SceneEvent");
    m_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::SceneEventFilter");
    m_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::HoverEvents");
    m_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::MouseEvents");
    m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs", i_strClassName + "::KeyEvents");
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
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCtorsAndDtor);
    m_pTrcAdminObjCtorsAndDtor = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjItemChange);
    m_pTrcAdminObjItemChange = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjBoundingRect);
    m_pTrcAdminObjBoundingRect = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjIsHit);
    m_pTrcAdminObjIsHit = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaint);
    m_pTrcAdminObjPaint = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEvent);
    m_pTrcAdminObjSceneEvent = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEventFilter);
    m_pTrcAdminObjSceneEventFilter = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjHoverEvents);
    m_pTrcAdminObjHoverEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseEvents);
    m_pTrcAdminObjMouseEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjKeyEvents);
    m_pTrcAdminObjKeyEvents = nullptr;
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
/*! @brief Convenience method to returning true if this object is a point.
*/
bool CGraphObj::isPoint() const
//------------------------------------------------------------------------------
{
    return m_type == EGraphObjTypePoint;
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
    return m_type == EGraphObjTypeLabel;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the drawing scene the object belongs to.
*/
CDrawingScene* CGraphObj::getDrawingScene()
//------------------------------------------------------------------------------
{
    return m_pDrawingScene;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setName",
        /* strAddInfo   */ strMthInArgs );

    if (m_strName != i_strName)
    {
        CIdxTreeEntry::setName(i_strName);
        if (m_pNameLabel != nullptr) {
            m_pNameLabel->setText(name());
        }
        updateEditInfo();
        updateToolTip();
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setKeyInTree",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree != i_strKey)
    {
        CIdxTreeEntry::setKeyInTree(i_strKey);
        if (m_pNameLabel != nullptr) {
            m_pNameLabel->setText(name());
        }
        updateEditInfo();
        updateToolTip();
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

//------------------------------------------------------------------------------
/*! @brief Returns the current edit mode of the object.

    The edit mode defines how mouse and key events are handled.
*/
CEnumEditMode CGraphObj::getEditMode() const
//------------------------------------------------------------------------------
{
    return m_editMode;
}

//------------------------------------------------------------------------------
/*! @brief Returns the current edit mode of the object.

    If the object is currently being resized (editMode == Resize) the resize mode
    defines how the object will be resized.
*/
CEnumEditResizeMode CGraphObj::getEditResizeMode() const
//------------------------------------------------------------------------------
{
    return m_editResizeMode;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns a human readable string containing the coordinates of all
           shape points of the object.

    This method is mainly used for debugging and test purposes.
*/
QString CGraphObj::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    return "";
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the currently selected index of the polygons shape point
           if the form of the object is modified by moving a polygon shape point.
*/
int CGraphObj::getSelectedPolygonShapePointIndex() const
//------------------------------------------------------------------------------
{
    return m_idxSelPtSelectedPolygon;
}

//------------------------------------------------------------------------------
/*! @brief Returns the currently selected point at the bounding rectangle if
           if the form of the object is modified by moving a selection point
           at the bounding rectangle.

     PolygonPoint is returned if a polygon point is selected.
*/
CEnumSelectionPoint CGraphObj::getSelectedBoundingRectPoint() const
//------------------------------------------------------------------------------
{
    return m_selPtSelectedBoundingRect;
}

//------------------------------------------------------------------------------
/*! @brief Returns a human readable tool tip string.
*/
QString CGraphObj::getToolTip() const
//------------------------------------------------------------------------------
{
    return m_strToolTip;
}

//------------------------------------------------------------------------------
/*! @brief Returns a human readable string with information about the current editing mode.
*/
QString CGraphObj::getEditInfo() const
//------------------------------------------------------------------------------
{
    return m_strEditInfo;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the parent object or nullptr, if the object is does not have a parent.

    Please note that selection points and labels should not belong as child to the
    graphics items. Otherwise the "boundingRect" call of groups (which implicitly calls
    childrenBoundingRect) does not work as the childs bounding rectangles would be
    included. But the selection points and labels should appear as childs in the
    index tree of the drawing scene.
*/
CGraphObj* CGraphObj::parentGraphObj()
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(parentBranch());
    if (m_type != EGraphObjTypeSelectionPoint && m_type != EGraphObjTypeLabel) {
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        QGraphicsItem* pGraphicsItemParent = pGraphicsItemThis->parentItem();
        CGraphObj* pGraphObjParentTmp = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
        if (pGraphObjParentTmp != pGraphObjParent) {
            throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, "pGraphObjParent != pGraphicsItemParent");
        }
    }
    return pGraphObjParent;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObj::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onCreateAndExecDlgFormatGraphObjs",
        /* strAddInfo   */ "" );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,this);
    pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtLabels);
    pDlgFormatGraphObjs->exec();
    delete pDlgFormatGraphObjs;
    pDlgFormatGraphObjs = nullptr;
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
        //strMthInArgs = i_drawSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setDrawSettings",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings != i_drawSettings) {
        m_drawSettings = i_drawSettings;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
        onDrawSettingsChanged();
    }
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
/*! @brief

    Must be overridden to apply the draw settings at the graphics item in derived classes.
*/
void CGraphObj::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onDrawSettingsChanged",
        /* strAddInfo   */ "" );

    /* Template for copy and paste in inherited classes:

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        if (m_drawSettings.isLineUsed()) {
            if (m_drawSettings.getLineStyle() != ELineStyle::NoLine) {
                QPen pen;
                pen.setColor( m_drawSettings.getPenColor() );
                pen.setWidth( m_drawSettings.getPenWidth() );
                pen.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );
                setPen(pen);
            }
            else {
                setPen(Qt::NoPen);
            }
        }
        else {
            setPen(Qt::NoPen);
        }

        if (m_drawSettings.isFillUsed()) {
            if (m_drawSettings.getFillStyle() != EFillStyle::NoFill) {
                QBrush brsh;
                brsh.setColor( m_drawSettings.getFillColor() );
                brsh.setStyle( fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()) );
                pGraphicsItem->setBrush(brsh);
            }
            else {
                setBrush(Qt::NoBrush);
            }
        }
        else {
            setBrush(Qt::NoBrush);
        }
    }
    */
} // onDrawSettingsChanged

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the pen color used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setPenColor",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getPenColor() != i_clr) {
        m_drawSettings.setPenColor(i_clr);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setPenWidth",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getPenWidth() != i_iLineWidth) {
        m_drawSettings.setPenWidth(i_iLineWidth);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
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
void CGraphObj::setLineStyle( ELineStyle i_lineStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLineStyle(i_lineStyle).toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineStyle",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getLineStyle() != i_lineStyle) {
        m_drawSettings.setLineStyle(i_lineStyle);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setLineStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
ELineStyle CGraphObj::getLineStyle() const
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
        strMthInArgs = i_clr.name() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFillColor",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getFillColor() != i_clr) {
        m_drawSettings.setFillColor(i_clr);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
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
void CGraphObj::setFillStyle( EFillStyle i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumFillStyle(i_fillStyle).toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFillStyle",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getFillStyle() != i_fillStyle) {
        m_drawSettings.setFillStyle(i_fillStyle);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setFillStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
EFillStyle CGraphObj::getFillStyle() const
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
void CGraphObj::setLineRecordType( ELineRecordType i_lineRecordType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLineRecordType(i_lineRecordType).toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineRecordType",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getLineRecordType() != i_lineRecordType) {
        m_drawSettings.setLineRecordType(i_lineRecordType);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setLineRecordType

//------------------------------------------------------------------------------
/*! @brief 
*/
ELineRecordType CGraphObj::getLineRecordType() const
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
        strMthInArgs = QString::number(i_iLineExtent) + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineExtent",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getLineExtent() != i_iLineExtent) {
        m_drawSettings.setLineExtent(i_iLineExtent);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
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
void CGraphObj::setLineEndStyle( ELinePoint i_linePoint, ELineEndStyle i_endStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString() +
            ", " + CEnumLineEndStyle(i_endStyle).toString() +
            ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineEndStyle",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getLineEndStyle(i_linePoint) != i_endStyle) {
        m_drawSettings.setLineEndStyle(i_linePoint, i_endStyle);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setLineEndStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
ELineEndStyle CGraphObj::getLineEndStyle(ELinePoint i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineEndStyle(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end base line type used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineEndBaseLineType( ELinePoint i_linePoint, ELineEndBaseLineType i_baseLineType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString() +
            ", " + CEnumLineEndBaseLineType(i_baseLineType).toString() +
            ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineEndBaseLineType",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getLineEndBaseLineType(i_linePoint) != i_baseLineType) {
        m_drawSettings.setLineEndBaseLineType(i_linePoint, i_baseLineType);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setLineEndBaseLineType

//------------------------------------------------------------------------------
/*! @brief 
*/
ELineEndBaseLineType CGraphObj::getLineEndBaseLineType( ELinePoint i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineEndBaseLineType(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end fill style used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineEndFillStyle( ELinePoint i_linePoint, ELineEndFillStyle i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString() +
            ", " + CEnumLineEndFillStyle(i_fillStyle).toString() +
            ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineEndFillStyle",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getLineEndFillStyle(i_linePoint) != i_fillStyle) {
        m_drawSettings.setLineEndFillStyle(i_linePoint,i_fillStyle);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setLineEndFillStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
ELineEndFillStyle CGraphObj::getLineEndFillStyle(ELinePoint i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineEndFillStyle(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end width used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineEndWidth( ELinePoint i_linePoint, ELineEndWidth i_width, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString() +
            ", " + CEnumLineEndWidth(i_width).toString() +
            ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineEndWidth",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getLineEndWidth(i_linePoint) != i_width) {
        m_drawSettings.setLineEndWidth(i_linePoint,i_width);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setLineEndWidth

//------------------------------------------------------------------------------
/*! @brief 
*/
ELineEndWidth CGraphObj::getLineEndWidth(ELinePoint i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineEndWidth(i_linePoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the line end length used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setLineEndLength( ELinePoint i_linePoint, ELineEndLength i_length, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString() +
            ", " + CEnumLineEndLength(i_length).toString() +
            ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setLineEndLength",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getLineEndLength(i_linePoint) != i_length) {
        m_drawSettings.setLineEndLength(i_linePoint, i_length);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setLineEndLength

//------------------------------------------------------------------------------
/*! @brief 
*/
ELineEndLength CGraphObj::getLineEndLength( ELinePoint i_linePoint ) const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getLineEndLength(i_linePoint);
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextColor",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getTextColor() != i_clr) {
        m_drawSettings.setTextColor(i_clr);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextFont",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getTextFont() != i_fnt) {
        m_drawSettings.setTextFont(i_fnt);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextSize",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getTextSize() != i_size) {
        m_drawSettings.setTextSize(i_size);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
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
void CGraphObj::setTextStyle( ETextStyle i_style, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumTextStyle(i_style).toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextStyle",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getTextStyle() != i_style) {
        m_drawSettings.setTextStyle(i_style);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setTextStyle

//------------------------------------------------------------------------------
/*! @brief 
*/
ETextStyle CGraphObj::getTextStyle() const
//------------------------------------------------------------------------------
{
    return m_drawSettings.getTextStyle();
}

//------------------------------------------------------------------------------
/*! @brief Sets the text effect used to paint the object.

    If further set specific draw settings call will follow the flag
    i_bImmediatelyApplySetting may be set to false to avoid unnecessary paint events.
*/
void CGraphObj::setTextEffect( ETextEffect i_effect, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumTextEffect(i_effect).toString() + ", ImmediateApply: " + bool2Str(i_bImmediatelyApplySetting);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setTextEffect",
        /* strAddInfo   */ strMthInArgs );

    if (m_drawSettings.getTextEffect() != i_effect) {
        m_drawSettings.setTextEffect(i_effect);
        if (i_bImmediatelyApplySetting) {
            if (m_pTree != nullptr) {
                m_pTree->onTreeEntryChanged(this);
            }
            onDrawSettingsChanged();
        }
    }
} // setTextEffect

//------------------------------------------------------------------------------
/*! @brief 
*/
ETextEffect CGraphObj::getTextEffect() const
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMinimumWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum.setWidth(i_physValWidth);

    if (m_physValSizeMinimum.width() > 0.0) {
        CPhysValSize physValSize = getSize();
        if (physValSize.width() < m_physValSizeMinimum.width()) {
            physValSize.setWidth(m_physValSizeMinimum.width());
            setSize(physValSize);
        }
    }
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
CPhysVal CGraphObj::getMinimumWidth() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.resolution());
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if (m_physValSizeMinimum.width() > 0.0) {
        physValWidth = m_physValSizeMinimum.width();
    }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMinimumHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum.setHeight(i_physValHeight);

    if (m_physValSizeMinimum.height() > 0.0) {
        CPhysValSize physValSize = getSize();
        if (physValSize.height() < m_physValSizeMinimum.height()) {
            physValSize.setHeight(m_physValSizeMinimum.height());
            setSize(physValSize);
        }
    }
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
CPhysVal CGraphObj::getMinimumHeight() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.resolution());
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if (m_physValSizeMinimum.height() > 0.0) {
        physValHeight = m_physValSizeMinimum.height();
    }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMinimumSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMinimum = i_physValSize;

    if (m_physValSizeMinimum.isValid()) {
        CPhysValSize physValSize = getSize();
        if (physValSize.width() < m_physValSizeMinimum.width() || physValSize.height() < m_physValSizeMinimum.height()) {
            if (physValSize.width() < m_physValSizeMinimum.width()) {
                physValSize.setWidth( m_physValSizeMinimum.width() );
            }
            if (physValSize.height() < m_physValSizeMinimum.height()) {
                physValSize.setHeight( m_physValSizeMinimum.height() );
            }
            setSize(physValSize);
        }
    }
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
CPhysValSize CGraphObj::getMinimumSize() const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize;
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMinimum.isValid()) {
        physValSize = m_physValSizeMinimum;
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMaximumWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum.setWidth(i_physValWidth);

    if (m_physValSizeMaximum.width() > 0.0) {
        CPhysValSize physValSize = getSize();
        if (physValSize.width() > m_physValSizeMaximum.width()) {
            physValSize.setWidth( m_physValSizeMaximum.width() );
            setSize(physValSize);
        }
    }
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
CPhysVal CGraphObj::getMaximumWidth() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.resolution());
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if (m_physValSizeMaximum.width() > 0.0) {
        physValWidth = m_physValSizeMaximum.width();
    }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMaximumHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum.setHeight(i_physValHeight);

    if (m_physValSizeMaximum.height() > 0.0) {
        CPhysValSize physValSize = getSize();
        if (physValSize.height() > m_physValSizeMaximum.height()) {
            physValSize.setHeight( m_physValSizeMaximum.height() );
            setSize(physValSize);
        }
    }
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
CPhysVal CGraphObj::getMaximumHeight() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.resolution());
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if (m_physValSizeMaximum.height() > 0.0) {
        physValHeight = m_physValSizeMaximum.height();
    }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setMaximumSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeMaximum = i_physValSize;

    if (m_physValSizeMaximum.isValid()) {
        CPhysValSize physValSize = getSize();
        if (physValSize.width() > m_physValSizeMaximum.width()
         || physValSize.height() > m_physValSizeMaximum.height()) {
            if (physValSize.width() > m_physValSizeMaximum.width()) {
                physValSize.setWidth( m_physValSizeMaximum.width() );
            }
            if (physValSize.height() > m_physValSizeMaximum.height()) {
                physValSize.setHeight( m_physValSizeMaximum.height() );
            }
            setSize(physValSize);
        }
    }
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
CPhysValSize CGraphObj::getMaximumSize() const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize;
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMaximum.isValid()) {
        physValSize = m_physValSizeMaximum;
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFixedWidth",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed.setWidth(i_physValWidth);

    if (m_physValSizeFixed.width() > 0.0) {
        CPhysValSize physValSize = getSize();
        if (physValSize.width() != m_physValSizeFixed.width()) {
            physValSize.setWidth( m_physValSizeFixed.width() );
            setSize(physValSize);
        }
    }
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
CPhysVal CGraphObj::getFixedWidth() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.resolution());
    if (m_physValSizeFixed.width() > 0.0) {
        physValWidth = m_physValSizeFixed.width();
    }
    else if ((m_physValSizeMinimum.width() > 0.0)
          && (m_physValSizeMinimum.width() == m_physValSizeMaximum.width())) {
        physValWidth = m_physValSizeMinimum.width();
    }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFixedHeight",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed.setHeight(i_physValHeight);

    if (m_physValSizeFixed.height() > 0.0) {
        CPhysValSize physValSize = getSize();
        if (physValSize.height() != m_physValSizeFixed.height()) {
            physValSize.setHeight( m_physValSizeFixed.height() );
            setSize(physValSize);
        }
    }
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
CPhysVal CGraphObj::getFixedHeight() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.resolution());
    if (m_physValSizeFixed.height() > 0.0) {
        physValHeight = m_physValSizeFixed.height();
    }
    else if ((m_physValSizeMinimum.height() > 0.0)
          && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
        physValHeight = m_physValSizeMinimum.height();
    }
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setFixedSize",
        /* strAddInfo   */ strMthInArgs );

    m_physValSizeFixed = i_physValSize;

    if (m_physValSizeFixed.isValid()) {
        CPhysValSize physValSize = getSize();
        if (physValSize.width() != m_physValSizeFixed.width()
         || physValSize.height() != m_physValSizeFixed.height()) {
            if (physValSize.width() != m_physValSizeFixed.width()) {
                physValSize.setWidth( m_physValSizeFixed.width() );
            }
            if (physValSize.height() != m_physValSizeFixed.height()) {
                physValSize.setHeight( m_physValSizeFixed.height() );
            }
            setSize(physValSize);
        }
    }
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
CPhysValSize CGraphObj::getFixedSize() const
//------------------------------------------------------------------------------
{
    CPhysValSize physValSize;
    if (m_physValSizeFixed.isValid()) {
        physValSize = m_physValSizeFixed;
    }
    else if (m_physValSizeMinimum.isValid() && m_physValSizeMaximum.isValid()) {
        if ((m_physValSizeMinimum.width() == m_physValSizeMaximum.width())
         && (m_physValSizeMinimum.height() == m_physValSizeMaximum.height())) {
            physValSize = m_physValSizeMinimum;
        }
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
        /* strObjName   */ m_strName,
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
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setAlignment",
        /* strAddInfo   */ strMthInArgs );

    if (i_idx < 0 || i_idx >= m_arAlignments.size()) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeAlignment",
        /* strAddInfo   */ strMthInArgs );

    if (i_idx < 0 || i_idx >= m_arAlignments.size()) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::acceptCurrentAsOriginalCoors",
        /* strAddInfo   */ "" );

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        m_ptPosOrig = pGraphicsItem->pos();
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
public: // must overridables
==============================================================================*/

/*------------------------------------------------------------------------------
void CGraphObj::setWidth( const CPhysVal& i_physValWidth )
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
void CGraphObj::setHeight( const CPhysVal& i_physValHeight )
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
void CGraphObj::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
void CGraphObj::setSize( const CPhysValSize& i_physValSize )
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
bool CGraphObj::hasBoundingRect() const
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
bool CGraphObj::hasLineShapePoints() const
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
bool CGraphObj::hasRotationSelectionPoints() const
------------------------------------------------------------------------------*/

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the position of the item in parent coordinates. If the item has no parent,
    its position is given in scene coordinates.

    The position of the item describes its origin (local coordinate (0, 0)) in parent
    coordinates; this function returns the same as mapToParent(0, 0).

    Items may be transformed (rotated, resized, etc.). Each rotation step may
    include inaccuracy when calculation the resulting position. The more often
    it is rotated, the greater the inaccuracy. If the original rotation angle of 0.0
    will be reached the resulting position in scene coordinates may not be the same
    as the original position. Therefore the original coordinates of the item are
    stored. The given version either selects the current (transformed) postion
    or the orignal (not transformed) position of the item.

    @param i_version [in]
        Original .. Return the original coordinates without transformation.
        Current ... Return the current coordinates calculated with transformation.

    @return Position of the item in parent coordinates.
*/
CPhysValPoint CGraphObj::getPos( ECoordinatesVersion i_version ) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValPoint physValPoint(drawingSize.unit(), drawingSize.resolution().getVal());
    if (i_version == ECoordinatesVersion::Original) {
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        ptPos = m_ptPosOrig;
#endif
    }
    else {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            physValPoint = pGraphicsItem->pos();
        }
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getWidth( ECoordinatesVersion i_version ) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValWidth(0.0, drawingSize.unit(), drawingSize.resolution().getVal());
    if (i_version == ECoordinatesVersion::Original) {
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        fWidth = m_sizOrig.width();
#endif
    }
    else {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            physValWidth = pGraphicsItem->boundingRect().width();
        }
    }
    return physValWidth;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObj::getHeight( ECoordinatesVersion i_version ) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValHeight(0.0, drawingSize.unit(), drawingSize.resolution().getVal());
    if (i_version == ECoordinatesVersion::Original) {
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        fHeight = m_sizOrig.height();
#endif
    }
    else {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            physValHeight = pGraphicsItem->boundingRect().height();
        }
    }
    return physValHeight;
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObj::getSize( ECoordinatesVersion i_version ) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValSize physValSize(drawingSize.unit(), drawingSize.resolution().getVal());
    if (i_version == ECoordinatesVersion::Original) {
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        siz = m_sizOrig;
#endif
    }
    else {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            physValSize = pGraphicsItem->boundingRect().size();
        }
    }
    return physValSize;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setRotationAngleInDegree( double i_fRotAngle_deg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fRotAngle_deg, 'f', 1);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setRotationAngleInDegree",
        /* strAddInfo   */ strMthInArgs );

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    m_fRotAngleCurr_deg = i_fRotAngle_deg;
#endif

    updateTransform();
}

//------------------------------------------------------------------------------
double CGraphObj::getRotationAngleInDegree( ECoordinatesVersion i_version )
//------------------------------------------------------------------------------
{
    double fRotAngle_deg = 0.0;
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    if (i_version == ECoordinatesVersion::Original) {
        fRotAngle_deg = m_fRotAngleOrig_deg;
    }
    else {
        fRotAngle_deg = m_fRotAngleCurr_deg;
    }
#endif
    return fRotAngle_deg;
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
void CGraphObj::setEditMode( EEditMode i_editMode )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumEditMode(i_editMode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setEditMode",
        /* strAddInfo   */ strMthInArgs );

    m_editMode = i_editMode;
}

//------------------------------------------------------------------------------
void CGraphObj::setEditResizeMode( EEditResizeMode i_editResizeMode )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumEditResizeMode(i_editResizeMode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setEditResizeMode",
        /* strAddInfo   */ strMthInArgs );

    m_editResizeMode = i_editResizeMode;
}

/*==============================================================================
public: // must overridables
==============================================================================*/

/*------------------------------------------------------------------------------
bool CGraphObj::setIsHit()
------------------------------------------------------------------------------*/

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObj::isHit() const
//------------------------------------------------------------------------------
{
    return m_bIsHit;
}

//------------------------------------------------------------------------------
bool CGraphObj::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point:" + point2Str(i_pt) +
            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjIsHit,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strMthInArgs );

    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        if (pGraphicsItem->isSelected()) {
            bIsHit = isPolygonSelectionPointHit(i_pt,o_pHitInfo);
            if (!bIsHit) {
                bIsHit = isBoundingRectSelectionPointHit(
                    /* pt               */ i_pt,
                    /* iSelPtsCount     */ -1,
                    /* pSelPts          */ nullptr,
                    /* pGraphObjHitInfo */ o_pHitInfo );
            }
        }
        if (!bIsHit) {
            if (pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern) {
                bIsHit = pGraphicsItem->contains(i_pt);
                if (o_pHitInfo != nullptr) {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                }
            }
        }
        if (bIsHit && o_pHitInfo != nullptr) {
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
            o_pHitInfo->setCursor( Math::deg2Rad(m_fRotAngleCurr_deg) );
#endif
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs;
        if (o_pHitInfo != nullptr) {
            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
            mthTracer.setMethodOutArgs(strMthOutArgs);
        }
        mthTracer.setMethodReturn(bIsHit);
    }
    return bIsHit;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    @return The new ZValue.
*/
double CGraphObj::bringToFront()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::bringToFront",
        /* strAddInfo   */ "" );

    double fZValue = m_fZValue;

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QRectF rctBounding = pGraphicsItem->boundingRect();
        rctBounding = pGraphicsItem->mapRectToScene(rctBounding);
        QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rctBounding);
        m_pDrawingScene->bringToFront( pGraphicsItem, arpGraphicsItemsIntersected );
        fZValue = pGraphicsItem->zValue();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString::number(fZValue));
    }
    return fZValue;
}

//------------------------------------------------------------------------------
/*! @brief

    You must call this method instead of setZValue of QGraphicsItem.
*/
void CGraphObj::setStackingOrderValue( double i_fZValue )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fZValue, 'f', 1);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::setStackingOrderValue",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        m_fZValue = i_fZValue;
        pGraphicsItem->setZValue(m_fZValue);
    }
}

//------------------------------------------------------------------------------
double CGraphObj::getStackingOrderValue() const
//------------------------------------------------------------------------------
{
    return m_fZValue;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::showBoundingRect()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showBoundingRect",
        /* strAddInfo   */ "" );

    if (!m_bBoundRectVisible) {
        m_bBoundRectVisible = true;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
void CGraphObj::hideBoundingRect()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideBoundingRect",
        /* strAddInfo   */ "" );

    if (m_bBoundRectVisible) {
        m_bBoundRectVisible = false;
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
bool CGraphObj::isBoundingRectVisible() const
//------------------------------------------------------------------------------
{
    return m_bBoundRectVisible;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObj::isBoundingRectSelectionPointHit(
    const QPointF&         i_pt,
    int                    i_iSelPtsCount,
    const ESelectionPoint* i_pSelPts,
    SGraphObjHitInfo*      o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr)
    {
        static const ESelectionPoint s_arSelPts[] = {
            ESelectionPoint::BottomRight,
            ESelectionPoint::TopRight,
            ESelectionPoint::BottomLeft,
            ESelectionPoint::TopLeft,
            ESelectionPoint::BottomCenter,
            ESelectionPoint::RightCenter,
            ESelectionPoint::TopCenter,
            ESelectionPoint::LeftCenter,
            ESelectionPoint::RotateTop,
            ESelectionPoint::RotateBottom,
            ESelectionPoint::Center
        };

        const ESelectionPoint* pSelPts = i_pSelPts;
        int iSelPtsCount = i_iSelPtsCount;

        if (pSelPts == nullptr) {
            iSelPtsCount = _ZSArrLen(s_arSelPts);
            pSelPts = s_arSelPts;
        }

        for (int idxSelPt = 0; idxSelPt < iSelPtsCount; idxSelPt++) {
            ESelectionPoint selPt = pSelPts[idxSelPt];
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[static_cast<int>(selPt)];
            if (pGraphObjSelPt != nullptr) {
                QPointF ptTmp = pGraphicsItem->mapToItem(pGraphObjSelPt,i_pt);
                if (pGraphObjSelPt->contains(ptTmp)) {
                    bIsHit = true;
                    if (o_pHitInfo != nullptr) {
                        ptTmp = pGraphObjSelPt->pos();
                        ptTmp = pGraphObjSelPt->mapToItem(pGraphicsItem,ptTmp);
                        o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                        o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                        o_pHitInfo->m_selPtBoundingRect = selPt;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = ptTmp;
                    }
                    break;
                }
            }
        }
    }
    return bIsHit;
}

//------------------------------------------------------------------------------
bool CGraphObj::isPolygonSelectionPointHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
            if (pGraphObjSelPt != nullptr) {
                QPointF ptTmp = pGraphicsItem->mapToItem(pGraphObjSelPt,i_pt);
                if (pGraphObjSelPt->contains(ptTmp)) {
                    bIsHit = true;
                    if (o_pHitInfo != nullptr) {
                        ptTmp = pGraphObjSelPt->pos();
                        ptTmp = pGraphObjSelPt->mapToItem(pGraphicsItem,ptTmp);
                        o_pHitInfo->m_editMode = EEditMode::MoveShapePoint;
                        o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                        o_pHitInfo->m_idxPolygonShapePoint = idxSelPt;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = ptTmp;
                    }
                    break;
                }
            }
        }
    }
    return bIsHit;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns coordinates of selection point in item's coordinate system.
*/
QPointF CGraphObj::getSelectionPointCoors( ESelectionPoint i_selPt ) const
//------------------------------------------------------------------------------
{
    QPointF pt;
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        pt = ZS::Draw::getSelectionPointCoors(pGraphicsItem->boundingRect(), i_selPt);
    }
    return pt;
}

/*==============================================================================
protected: // must overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

void CGraphObj::showSelectionPoints( unsigned char i_selPts )
------------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
/*! @brief

void CGraphObj::updateSelectionPoints( unsigned char i_selPts )
------------------------------------------------------------------------------*/

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    Not just hides but also destroys the selection points.
*/
void CGraphObj::hideSelectionPoints( ESelectionPoints i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = selectionPoints2Str(i_selPts);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr)
    {
        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++)
        {
            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
            bool bHideSelPt = false;

            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
                if (i_selPts & ESelectionPointsBoundingRectCorner) {
                    bHideSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
                    bHideSelPt = true;
                }
            }
            else if (selPt == ESelectionPoint::Center) {
                if (i_selPts & ESelectionPointsBoundingRectCenter) {
                    bHideSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
                if (i_selPts & ESelectionPointsBoundingRectRotate) {
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

        if (i_selPts & ESelectionPointsPolygonShapePoints) {
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

//------------------------------------------------------------------------------
void CGraphObj::bringSelectionPointsToFront( ESelectionPoints i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = selectionPoints2Str(i_selPts);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::bringSelectionPointsToFront",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (pGraphicsItem != nullptr)
    {
        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++)
        {
            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
            bool bBringToFront = false;

            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
                if (i_selPts & ESelectionPointsBoundingRectCorner) {
                    bBringToFront = true;
                }
            }
            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
                    bBringToFront = true;
                }
            }
            else if (selPt == ESelectionPoint::Center) {
                if (i_selPts & ESelectionPointsBoundingRectCenter) {
                    bBringToFront = true;
                }
            }
            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
                if (i_selPts & ESelectionPointsBoundingRectRotate) {
                    bBringToFront = true;
                }
            }

            if (bBringToFront) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    QRectF rct = pGraphObjSelPt->rect();
                    rct = pGraphObjSelPt->mapRectToScene(rct);
                    QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rct);
                    m_pDrawingScene->bringToFront( pGraphObjSelPt, arpGraphicsItemsIntersected );
                }
            }
        }

        if (i_selPts & ESelectionPointsPolygonShapePoints) {
            for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    QRectF rct = pGraphObjSelPt->rect();
                    rct = pGraphObjSelPt->mapRectToScene(rct);
                    QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rct);
                    m_pDrawingScene->bringToFront(pGraphObjSelPt, arpGraphicsItemsIntersected);
                }
            }
        }
    }
} // bringSelectionPointsToFront

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    Creates the selection points if not yet created.
*/
void CGraphObj::showSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = rect2Str(i_rct);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showSelectionPointsOfBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr)
    {
        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++)
        {
            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
            bool bShowSelPt = false;

            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
                if (i_selPts & ESelectionPointsBoundingRectCorner) {
                    bShowSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
                    bShowSelPt = true;
                }
            }
            else if (selPt == ESelectionPoint::Center) {
                if (i_selPts & ESelectionPointsBoundingRectCenter) {
                    bShowSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
                if (i_selPts & ESelectionPointsBoundingRectRotate) {
                    bShowSelPt = true;
                }
            }

            if (bShowSelPt) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
                if (pGraphObjSelPt == nullptr) {
                    m_arpSelPtsBoundingRect[idxSelPt] = pGraphObjSelPt = new CGraphObjSelectionPoint(
                        /* pDrawingScene     */ m_pDrawingScene,
                        /* pGraphObjSelected */ this,
                        /* selectionPoint    */ selPt );

                    // Please note that selection points should not belong as child to the graphics items
                    // for which the selection points are created. Otherwise the "boundingRect" call
                    // of groups (which implicitly calls childrenBoundingRect) does not work as the
                    // selection points of the bounding rectangle would be included. But the selection
                    // points should appear as childs in the index tree of the drawing scene. This has to
                    // be taken into account by the "addGraphObj" method of the drawing scene.
                    m_pDrawingScene->addGraphObj(pGraphObjSelPt, this);

                    //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                    pGraphObjSelPt->installSceneEventFilter(pGraphicsItem); // event filters can only be installed on items in a scene
                }

                if (pGraphObjSelPt != nullptr) {
                    QPointF ptSel;
                    if (selPt == ESelectionPoint::RotateTop /*&& m_fScaleFacYCurr != 0.0*/) {
                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,ESelectionPoint::TopCenter);
                        ptSel.setY( ptSel.y() - getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }
                    else if (selPt == ESelectionPoint::RotateBottom /*&& m_fScaleFacYCurr != 0.0*/) {
                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,ESelectionPoint::BottomCenter);
                        ptSel.setY( ptSel.y() + getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }
                    else {
                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,selPt);
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }

                    pGraphObjSelPt->setPos(ptSel);
                    pGraphObjSelPt->setZValue( pGraphicsItem->zValue()+0.05 );
                }
            }
        }
    }
} // showSelectionPointsOfBoundingRect

//------------------------------------------------------------------------------
void CGraphObj::updateSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = rect2Str(i_rct);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateSelectionPointsOfBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr)
    {
        for (int idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++)
        {
            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
            bool bUpdateSelPt = false;

            if (idxSelPt >= ESelectionPointCornerMin && idxSelPt <= ESelectionPointCornerMax) {
                if (i_selPts & ESelectionPointsBoundingRectCorner) {
                    bUpdateSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointLineCenterMin && idxSelPt <= ESelectionPointLineCenterMax) {
                if (i_selPts & ESelectionPointsBoundingRectLineCenter) {
                    bUpdateSelPt = true;
                }
            }
            else if (selPt == ESelectionPoint::Center) {
                if (i_selPts & ESelectionPointsBoundingRectCenter) {
                    bUpdateSelPt = true;
                }
            }
            else if (idxSelPt >= ESelectionPointRotateMin && idxSelPt <= ESelectionPointRotateMax) {
                if (i_selPts & ESelectionPointsBoundingRectRotate) {
                    bUpdateSelPt = true;
                }
            }

            if (bUpdateSelPt) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    QPointF ptSel;
                    if (selPt == ESelectionPoint::RotateTop /*&& m_fScaleFacYCurr != 0.0*/) {
                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,ESelectionPoint::TopCenter);
                        ptSel.setY( ptSel.y() - getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }
                    else if (selPt == ESelectionPoint::RotateBottom /*&& m_fScaleFacYCurr != 0.0*/) {
                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,ESelectionPoint::BottomCenter);
                        ptSel.setY( ptSel.y() + getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }
                    else {
                        ptSel = ZS::Draw::getSelectionPointCoors(i_rct,selPt);
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }

                    pGraphObjSelPt->setPos(ptSel);
                    pGraphObjSelPt->setZValue( pGraphicsItem->zValue()+0.05 );
                }
            }
        }
    }
} // updateSelectionPointsOfBoundingRect

//------------------------------------------------------------------------------
/*! @brief

    Creates the selection points if not yet created.
*/
void CGraphObj::showSelectionPointsOfPolygon( const QPolygonF& i_plg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = polygon2Str(i_plg);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showSelectionPointsOfPolygon",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr)
    {
        if (m_arpSelPtsPolygon.size() != i_plg.size())
        {
            if (m_arpSelPtsPolygon.size() > 0)
            {
                for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt--)
                {
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
            for (int idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++)
            {
                m_arpSelPtsPolygon.append(nullptr);
            }
        }

        for (int idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++)
        {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

            if (pGraphObjSelPt == nullptr)
            {
                m_arpSelPtsPolygon[idxSelPt] = pGraphObjSelPt = new CGraphObjSelectionPoint(
                    /* pDrawingScene     */ m_pDrawingScene,
                    /* pGraphObjSelected */ this,
                    /* idxPt             */ idxSelPt );
                pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                // Please note that the labels should not belong as child to the graphics items
                // for which the selection points are created. Otherwise the "boundingRect" call
                // of groups (which implicitly calls childrenBoundingRect) does not work as the
                // labels would be included. But the labels should appear as childs in the index
                // tree of the drawing scene. This has to be taken into account by the "addGraphObj"
                // method of the drawing scene.
                m_pDrawingScene->addGraphObj(pGraphObjSelPt, this);

                //pGraphObjSelPt->setParentItem(this); see comment in header file of class CGraphObjSelectionPoint
                pGraphObjSelPt->installSceneEventFilter(pGraphicsItem); // event filters can only be installed on items in a scene
            }

            if (pGraphObjSelPt != nullptr)
            {
                QPointF ptSel = i_plg[idxSelPt];
                ptSel = pGraphicsItem->mapToScene(ptSel);
                pGraphObjSelPt->setPos(ptSel);
                pGraphObjSelPt->setZValue( pGraphicsItem->zValue()+0.05 );
            }
        }
    }
} // showSelectionPointsOfPolygon

//------------------------------------------------------------------------------
void CGraphObj::updateSelectionPointsOfPolygon( const QPolygonF& i_plg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = polygon2Str(i_plg);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateSelectionPointsOfPolygon",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr) {
        if (i_plg.size() == m_arpSelPtsPolygon.size()) {
            for (int idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    QPointF ptSel = i_plg[idxSelPt];
                    ptSel = pGraphicsItem->mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                    pGraphObjSelPt->setZValue( pGraphicsItem->zValue()+0.05 );
                }
            }
        }
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the list of the possible selection points the labels may be  anchored to.

    For SelectionPoint == PolygonPoint the anchor points depends on the number
    of line points of the polygon or polyline.

    The method has to be overridden by the specialized classes.

    Please note that the most common used selection points should be at the
    beginning of the list so that combo boxes to select the selection point
    start with those.

    @return List of possbile selection points.
*/
QList<ESelectionPoint> CGraphObj::getPossibleLabelAnchorPoints() const
//------------------------------------------------------------------------------
{
    QList<ESelectionPoint> arSelPts = {
        ESelectionPoint::PolygonPoint
    };
    return arSelPts;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Shows the name in a text label linked to the given selection point.

    If the label is already visible at a different selection point the label
    remains visible but will be linked to the new given selection point.
    This is realized by destroying the current label and creating a new label.

    @param i_selPt
        Selection point the label should be linked to.
        None, Any, All, RotateTop and RotateBottom is not allowed.
*/
void CGraphObj::showNameLabel( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showNameLabel",
        /* strAddInfo   */ strMthInArgs );

    if (m_pNameLabel == nullptr || m_pNameLabel->getLinkedSelectionPoint() != i_selPt)
    {
        QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
        if (pGraphicsItem == nullptr) {
            throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "dynamic_cast<QGraphicsItem*>(this)");
        }

        if (m_pNameLabel == nullptr) {
            m_pNameLabel = new CGraphObjLabel(
                m_pDrawingScene, this, "Name", m_strName, i_selPt);
            m_pDrawingScene->addGraphObj(m_pNameLabel, this);
        }
        else {
            m_pNameLabel->setLinkedSelectionPoint(i_selPt);
        }

        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
} // showNameLabel

//------------------------------------------------------------------------------
/*! Hides the name label - if currently visible.
*/
void CGraphObj::hideNameLabel()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideNameLabel",
        /* strAddInfo   */ strMthInArgs );

    // the dtor of the label removes itself from the drawing scene
    //m_pDrawingScene->removeGraphObj(m_pNameLabel);

    delete m_pNameLabel;
    m_pNameLabel = nullptr;

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
} // hideNameLabel

//------------------------------------------------------------------------------
/*! Checks whether the name label is visible for the given selection point.

    @return true, if the label is visible or if None has been passed, if the label is not visible.
            false, if the label is not visible or if None has been passed, if the label is visible.
*/
bool CGraphObj::isNameLabelVisible() const
//------------------------------------------------------------------------------
{
    bool bIsVisible = false;
    if (m_pNameLabel != nullptr) {
        bIsVisible = m_pNameLabel->isVisible();
    }
    return bIsVisible;
}

//------------------------------------------------------------------------------
/*! @brief Returns the anchor point of the name label.
*/
ESelectionPoint CGraphObj::nameLabelAnchorPoint() const
//------------------------------------------------------------------------------
{
    ESelectionPoint selPt = ESelectionPoint::None;
    if (m_pNameLabel != nullptr) {
        selPt = m_pNameLabel->getLinkedSelectionPoint();
    }
    return selPt;
}

//------------------------------------------------------------------------------
/*! Shows the line between the name label and the selection point the label is aligned to.

    The method just sets a flag. The line becomes only visible if also
    the name label is visible.
*/
void CGraphObj::showNameLabelAnchorLine()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showNameLabelAnchorLine",
        /* strAddInfo   */ "" );

    if (m_pNameLabel != nullptr) {
        m_pNameLabel->showAnchorLine();
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Hides the line between the name label and the selection point the label is aligned to.

    The method just sets a flag. If the name label is not visible the method has no effect.
*/
void CGraphObj::hideNameLabelAnchorLine()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideNameLabelAnchorLine",
        /* strAddInfo   */ "" );

    if (m_pNameLabel != nullptr) {
        m_pNameLabel->hideAnchorLine();
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
    }
}

//------------------------------------------------------------------------------
/*! Checks whether the line between the name label and the selection point
    the label is aligned is visible.
*/
bool CGraphObj::isNameLabelAnchorLineVisible() const
//------------------------------------------------------------------------------
{
    bool bIsVisible = false;
    if (m_pNameLabel != nullptr) {
        bIsVisible = m_pNameLabel->isAnchorLineVisible();
    }
    return bIsVisible;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Adds a label to the graphical objects which will be shown linked to the given
    selection point.

    @param i_strLabel [in]
        Text of the label to be indicated.
    @param i_selPt [in]
        Selection point to which the label should be aligned to.
        None, Any, All, RotateTop and RotateBottom is not allowed.

    @return Index of the label in the list of labels.
*/
int CGraphObj::showLabel(const QString& i_strLabel, ESelectionPoint i_selPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strLabel + ", " + CEnumSelectionPoint::toString(i_selPt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showLabel",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "dynamic_cast<QGraphicsItem*>(this)");
    }

    int idxLabel = m_arpLabels.size();
    CGraphObjLabel* pGraphObjLabel = new CGraphObjLabel(
        m_pDrawingScene, this, QString::number(idxLabel), i_strLabel, i_selPt);
    m_pDrawingScene->addGraphObj(pGraphObjLabel, this);
    m_arpLabels.append(pGraphObjLabel);

    QPointF ptSelPt = getSelectionPointCoors(i_selPt);
    ptSelPt = pGraphicsItem->mapToScene(ptSelPt);

    QPointF ptLabelTmp = ptSelPt;
    if (i_selPt != ESelectionPoint::BottomRight &&
        i_selPt != ESelectionPoint::BottomLeft &&
        i_selPt != ESelectionPoint::BottomCenter )
    {
        ptLabelTmp.setY(ptLabelTmp.y() - pGraphicsItem->boundingRect().height());
    }

    bool bUniquePos = false;
    while (!bUniquePos) {
        bUniquePos = true;
        for (CGraphObjLabel* pGraphObjLabelTmp : m_arpLabels) {
            if (pGraphObjLabelTmp != pGraphObjLabel) {
                if (pGraphObjLabelTmp->scenePos() == ptLabelTmp) {
                    QGraphicsItem* pGraphicsItemTmp = dynamic_cast<QGraphicsItem*>(pGraphObjLabelTmp);
                    bUniquePos = false;
                    ptLabelTmp.setX(pGraphObjLabelTmp->scenePos().x() + pGraphicsItemTmp->boundingRect().width() + 4);
                    break;
                }
            }
        }
    }

    QSize sizeDist(ptLabelTmp.x() - ptSelPt.x(), ptLabelTmp.y() - ptSelPt.y());
    QPointF ptLabel(ptSelPt.x() + sizeDist.width(), ptSelPt.y() + sizeDist.height());

    pGraphObjLabel->setPos(ptLabel);
    pGraphObjLabel->setZValue(pGraphicsItem->zValue() + 0.02);

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(idxLabel);
    }
    return idxLabel;

} // showLabel

//------------------------------------------------------------------------------
/*! Hides the label and destroys it.

    @param i_idxLabel [in]
        Index of the label in the list of labels.
*/
void CGraphObj::hideLabel(int i_idxLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "IdxLabel: " + QString::number(i_idxLabel);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideLabel",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjLabel* pGraphObjLabel = m_arpLabels[i_idxLabel];
    m_arpLabels.removeAt(i_idxLabel);

    // the dtor of the label removes itself from the drawing scene
    //m_pDrawingScene->removeGraphObj(pGraphObjLabel);

    delete pGraphObjLabel;
    pGraphObjLabel = nullptr;

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
/*! Checks whether the label at the specified index is visible.

    @param i_idxLabel [in]
        Index of the label in the list of labels.

    @return true, if the label is visible. false otherwise.
*/
bool CGraphObj::isLabelVisible(int i_idxLabel) const
//------------------------------------------------------------------------------
{
    return m_arpLabels[i_idxLabel]->isVisible();
}

//------------------------------------------------------------------------------
/*! @brief Returns the anchor point of the label at the specified index.

    @param i_idxLabel [in]
        Index of the label in the list of labels.

    @return Selection point the label is linked (anchored) to.
*/
ESelectionPoint CGraphObj::labelAnchorPoint(int i_idxLabel) const
//------------------------------------------------------------------------------
{
    ESelectionPoint selPt = ESelectionPoint::None;
    if (m_pNameLabel != nullptr) {
        selPt = m_pNameLabel->getLinkedSelectionPoint();
    }
    return selPt;
}

//------------------------------------------------------------------------------
/*! Shows the line between the label and the selection point the label is aligned to.

    The method just sets a flag. The line becomes only visible if also
    the description label is visible.

    @param i_idxLabel [in]
        Index of the label in the list of labels.
*/
void CGraphObj::showLabelAnchorLine(int i_idxLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "IdxLabel: " + QString::number(i_idxLabel);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    m_arpLabels[i_idxLabel]->showAnchorLine();
    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
/*! Hides the line between the description label and the selection point the label is aligned to.

    The method just sets a flag. If the description label is not visible the method has no effect.

    @param i_idxLabel [in]
        Index of the label in the list of labels.
*/
void CGraphObj::hideLabelAnchorLine(int i_idxLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "IdxLabel: " + QString::number(i_idxLabel);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hideLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    m_arpLabels[i_idxLabel]->hideAnchorLine();
    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
}

//------------------------------------------------------------------------------
/*! Checks whether the line between the label and the selection point
    the label is aligned is visible.

    @param i_idxLabel [in]
        Index of the label in the list of labels.

    @return true, if the anchor line is visible, false otherwise.
*/
bool CGraphObj::isLabelAnchorLineVisible(int i_idxLabel) const
//------------------------------------------------------------------------------
{
    return m_arpLabels[i_idxLabel]->isAnchorLineVisible();
}

/*==============================================================================
public: // overridables (KeyLabel = "Position<SelPt>")
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates and shows position label(s) for the given selection point(s).

    @param i_selPt [in]
        Selection point(s) for which position labels should be created.
        None, Any, RotateTop and RotateBottom is not allowed.
        All .... Shows position labels for all selection points instead of
                 RotateTop and RotateBottom.
*/
void CGraphObj::showPositionLabel( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showPositionLabel",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (i_selPt == ESelectionPoint::All)
    {
        for (CEnumSelectionPoint selPt = 0; selPt < CEnumSelectionPoint::count(); ++selPt)
        {
            if( selPt != ESelectionPoint::None  && selPt != ESelectionPoint::Any && selPt != ESelectionPoint::All
             && selPt != ESelectionPoint::RotateTop && selPt != ESelectionPoint::RotateBottom )
            {
                QString strKey = selPt.toString();
                // Coordinates of the selection point in relation to the parent (thats me).
                QPointF ptSelPt = getSelectionPointCoors(selPt.enumerator());
                // Got to map the coordinates to my parent (which might be the scene).
                ptSelPt = pGraphicsItem->mapToParent(ptSelPt);
                QString strText = point2Str(ptSelPt);
                showLabel(m_arpPosLabels, strKey, strText, selPt.enumerator());
            }
        }
    }
    else
    {
        QString strKey = CEnumSelectionPoint::toString(i_selPt);
        // Coordinates of the selection point in relation to the parent (thats me).
        QPointF ptSelPt = getSelectionPointCoors(i_selPt);
        // Got to map the coordinates to my parent (which might be the scene).
        ptSelPt = pGraphicsItem->mapToParent(ptSelPt);
        QString strText = point2Str(ptSelPt);
        showLabel(m_arpPosLabels, strKey, strText, i_selPt);
    }
} // showPositionLabel

//------------------------------------------------------------------------------
/*! Hides and destroys position label(s) for the given selection point(s).

    @param i_selPt [in]
        Selection point(s) for which position labels should be destroyed.
        None, Any, RotateTop and RotateBottom is not allowed.
        All .... Shows position labels for all selection points instead of
                 RotateTop and RotateBottom.
*/
void CGraphObj::hidePositionLabel( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hidePositionLabel",
        /* strAddInfo   */ strMthInArgs );

    if (i_selPt == ESelectionPoint::All)
    {
        for (CEnumSelectionPoint selPt = 0; selPt < CEnumSelectionPoint::count(); ++selPt)
        {
            if( selPt != ESelectionPoint::None  && selPt != ESelectionPoint::Any && selPt != ESelectionPoint::All
             && selPt != ESelectionPoint::RotateTop && selPt != ESelectionPoint::RotateBottom )
            {
                QString strKey = selPt.toString();
                hideLabel(m_arpPosLabels, strKey);
            }
        }
    }
    else
    {
        QString strKey = CEnumSelectionPoint::toString(i_selPt);
        hideLabel(m_arpPosLabels, strKey);
    }
} // hidePositionLabel

//------------------------------------------------------------------------------
/*! Checks whether the position label for the given selection point is visible.

    @param i_selPt [in]
        Selection point(s) to be checked.
    @return
        RotateTop and RotateBottom is not allowed and is just ignored by returning false.
        None ... true is returned if no label is visible, false if any label is visible.
        All .... true is returned if all labels are visible, false if not all labels are visible.
        Any .... true is returned if any label is visible, false if no label is visible.
*/
bool CGraphObj::isPositionLabelVisible( ESelectionPoint i_selPt ) const
//------------------------------------------------------------------------------
{
    bool bIsVisible = false;
    if (i_selPt == ESelectionPoint::None) {
        for (auto* pGraphObjLabel : m_arpPosLabels) {
            bIsVisible = pGraphObjLabel->isVisible();
            if (bIsVisible) {
                bIsVisible = false;
                break;
            }
        }
    }
    else if (i_selPt == ESelectionPoint::All) {
        for (auto* pGraphObjLabel : m_arpPosLabels) {
            bIsVisible = pGraphObjLabel->isVisible();
            if (!bIsVisible) {
                break;
            }
        }
    }
    else if (i_selPt == ESelectionPoint::Any) {
        for (auto* pGraphObjLabel : m_arpPosLabels) {
            bIsVisible = pGraphObjLabel->isVisible();
            if (bIsVisible) {
                break;
            }
        }
    }
    else {
        QString strKey = CEnumSelectionPoint::toString(i_selPt);
        CGraphObjLabel* pGraphObjLabel = m_arpPosLabels.value(strKey, nullptr);
        bIsVisible = pGraphObjLabel == nullptr ? false : pGraphObjLabel->isVisible();
    }
    return bIsVisible;

} // isPositionLabelVisible

//------------------------------------------------------------------------------
/*! Shows the line between the position label and the selection point
    the label is aligned to.

    The method just sets a flag. The line becomes only visible if also
    the position label is visible.

    @param i_selPt [in]
        Selection point(s) for which anchor labels should be created.
        None, Any, RotateTop and RotateBottom is not allowed.
        All .... Shows anchor lines to labels for all selection points instead of
                 RotateTop and RotateBottom.
*/
void CGraphObj::showPositionLabelAnchorLine( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::showPositionLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (i_selPt == ESelectionPoint::All) {
        for (CEnumSelectionPoint selPt = 0; selPt < CEnumSelectionPoint::count(); ++selPt) {
            if( selPt != ESelectionPoint::None  && selPt != ESelectionPoint::Any && selPt != ESelectionPoint::All
             && selPt != ESelectionPoint::RotateTop && selPt != ESelectionPoint::RotateBottom )
            {
                QString strKey = selPt.toString();
                CGraphObjLabel* pGraphObjLabel = m_arpPosLabels.value(strKey, nullptr);
                if( pGraphObjLabel != nullptr )
                {
                    pGraphObjLabel->showAnchorLine();
                }
            }
        }
    }
    else {
        QString strKey = CEnumSelectionPoint::toString(i_selPt);
        CGraphObjLabel* pGraphObjLabel = m_arpPosLabels.value(strKey, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->showAnchorLine();
        }
    }
    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
} // showPositionLabelAnchorLine

//------------------------------------------------------------------------------
/*! Hides the line between the position label and the selection point
    the label is aligned to.

    The method just sets a flag. If the position lable is not visible the method
    has no effect.

    @param i_selPt [in]
        Selection point(s) for which anchor lines to labels should be created.
        None, Any, RotateTop and RotateBottom is not allowed.
        All .... Shows anchor lines to labels for all selection points instead of
                 RotateTop and RotateBottom.
*/
void CGraphObj::hidePositionLabelAnchorLine( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumSelectionPoint(i_selPt).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::hidePositionLabelAnchorLine",
        /* strAddInfo   */ strMthInArgs );

    if (i_selPt == ESelectionPoint::All) {
        for (CEnumSelectionPoint selPt = 0; selPt < CEnumSelectionPoint::count(); ++selPt) {
            if( selPt != ESelectionPoint::None  && selPt != ESelectionPoint::Any && selPt != ESelectionPoint::All
             && selPt != ESelectionPoint::RotateTop && selPt != ESelectionPoint::RotateBottom )
            {
                QString strKey = selPt.toString();
                CGraphObjLabel* pGraphObjLabel = m_arpPosLabels.value(strKey, nullptr);
                if( pGraphObjLabel != nullptr )
                {
                    pGraphObjLabel->hideAnchorLine();
                }
            }
        }
    }
    else {
        QString strKey = CEnumSelectionPoint::toString(i_selPt);
        CGraphObjLabel* pGraphObjLabel = m_arpPosLabels.value(strKey, nullptr);
        if (pGraphObjLabel != nullptr) {
            pGraphObjLabel->hideAnchorLine();
        }
    }

    if (m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }
} // hidePositionLabelAnchorLine

//------------------------------------------------------------------------------
/*! Checks whether the line between the position label and the selection point
    the label is aligned is visible.

    @param i_selPt [in]
        Selection point(s) to be checked.
    @return
        RotateTop and RotateBottom is not allowed and is just ignored by returning false.
        None ... true is returned if no anchor line is visible, false if any no anchor line is visible.
        All .... true is returned if all no anchor lines are visible, false if not all no anchor lines are visible.
        Any .... true is returned if any no anchor line is visible, false if no no anchor line is visible.
*/
bool CGraphObj::isPositionLabelAnchorLineVisible( ESelectionPoint i_selPt ) const
//------------------------------------------------------------------------------
{
    bool bIsVisible = false;
    if (i_selPt == ESelectionPoint::None) {
        for (auto* pGraphObjLabel : m_arpPosLabels) {
            bIsVisible = pGraphObjLabel->isAnchorLineVisible();
            if (bIsVisible) {
                bIsVisible = false;
                break;
            }
        }
    }
    else if (i_selPt == ESelectionPoint::All) {
        for (auto* pGraphObjLabel : m_arpPosLabels) {
            bIsVisible = pGraphObjLabel->isAnchorLineVisible();
            if (!bIsVisible) {
                break;
            }
        }
    }
    else if (i_selPt == ESelectionPoint::Any) {
        for (auto* pGraphObjLabel : m_arpPosLabels) {
            bIsVisible = pGraphObjLabel->isAnchorLineVisible();
            if (bIsVisible) {
                break;
            }
        }
    }
    else {
        QString strKey = CEnumSelectionPoint::toString(i_selPt);
        CGraphObjLabel* pGraphObjLabel = m_arpPosLabels.value(strKey, nullptr);
        bIsVisible = pGraphObjLabel == nullptr ? false : pGraphObjLabel->isAnchorLineVisible();
    }
    return bIsVisible;

} // isPositionLabelAnchorLineVisible

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! 

    @param i_pGraphObjLabel [in]
        Pointer to label which has been changed.
*/
void CGraphObj::updateLabelDistance(CGraphObjLabel* i_pGraphObjLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObjLabel == nullptr ? "nullptr" : i_pGraphObjLabel->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateLabelDistance",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    //QPointF ptSelPt = getSelectionPointCoors(i_pGraphObjLabel->m_selPt.enumerator());
    //QPointF ptLabel = i_pGraphObjLabel->scenePos();
    //ptSelPt = pGraphicsItem->mapToScene(ptSelPt);

    //i_pGraphObjLabel->m_sizDist.setWidth(ptLabel.x() - ptSelPt.x());
    //i_pGraphObjLabel->m_sizDist.setHeight(ptLabel.y() - ptSelPt.y());
    //i_pGraphObjLabel->m_bDistValid = true;
}

/*==============================================================================
public: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObj::showLabels()
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
//    {
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::showLabels",
//        /* strAddInfo   */ strMthInArgs );
//
//    QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);
//    CGraphObjLabel*                         pGraphObjLabel;
//
//    while( itLabels.hasNext() )
//    {
//        itLabels.next();
//
//        pGraphObjLabel = itLabels.value();
//
//        if( pGraphObjLabel->m_pGraphObjLabel == nullptr )
//        {
//            showLabel(pGraphObjLabel->m_strKey);
//        }
//    }
//
//} // showLabels

////------------------------------------------------------------------------------
//void CGraphObj::hideLabels()
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
//    {
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::hideLabels",
//        /* strAddInfo   */ strMthInArgs );
//
//    QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);
//    CGraphObjLabel*                         pGraphObjLabel;
//
//    while( itLabels.hasNext() )
//    {
//        itLabels.next();
//
//        pGraphObjLabel = itLabels.value();
//
//        if( pGraphObjLabel->m_pGraphObjLabel == nullptr )
//        {
//            hideLabel(pGraphObjLabel->m_strKey);
//        }
//    }
//
//} // hideLabels

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//QStringList CGraphObj::getLabelsKeys() const
////------------------------------------------------------------------------------
//{
//    return m_arpLabels.keys();
//}

////------------------------------------------------------------------------------
//CGraphObjLabel* CGraphObj::getLabel( const QString& i_strKey ) const
////------------------------------------------------------------------------------
//{
//    CGraphObjLabel* pGraphObjLabel = nullptr;
//
//    if( m_arpLabels.contains(i_strKey) )
//    {
//        pGraphObjLabel = m_arpLabels[i_strKey];
//    }
//    return pGraphObjLabel;
//
//} // getLabel

////------------------------------------------------------------------------------
//void CGraphObj::addLabels( QHash<QString, CGraphObjLabel*> i_arpLabels )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
//    {
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "CGraphObj::addLabels",
//        /* strAddInfo   */ strMthInArgs );
//
//    QHashIterator<QString, CGraphObjLabel*> itLabels(i_arpLabels);
//
//    CGraphObjLabel* pGraphObjLabel;
//    QString         strKey;
//    QString         strText;
//    ESelectionPoint selPt;
//    QSizeF          sizDist;
//    bool            bDistValid;
//    bool            bVisible;
//
//    while( itLabels.hasNext() )
//    {
//        itLabels.next();
//
//        pGraphObjLabel = itLabels.value();
//
//        strKey     = pGraphObjLabel->m_strKey;
//        strText    = pGraphObjLabel->m_strText;
//        selPt      = pGraphObjLabel->m_selPt.enumerator();
//        sizDist    = pGraphObjLabel->m_sizDist;
//        bDistValid = pGraphObjLabel->m_bDistValid;
//        bVisible   = pGraphObjLabel->m_bVisible;
//
//        if( !m_arpLabels.contains(strKey) )
//        {
//            addLabel(strKey,strText,selPt);
//        }
//
//        pGraphObjLabel = m_arpLabels[strKey];
//
//        pGraphObjLabel->m_sizDist    = sizDist;
//        pGraphObjLabel->m_bDistValid = bDistValid;
//        pGraphObjLabel->m_bVisible   = bVisible;
//
//        if( bVisible )
//        {
//            showLabel(strKey);
//        }
//    }
//
//} // addLabels

/*==============================================================================
public: // overridables
==============================================================================*/

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::onParentItemCoorsHasChanged( CGraphObj* /*i_pGraphObjParent*/ )
//------------------------------------------------------------------------------
{
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
void CGraphObj::onSelectionPointDestroying( CGraphObjSelectionPoint* i_pSelectionPoint )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pSelectionPoint == nullptr ? "nullptr" : i_pSelectionPoint->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onSelectionPointDestroying",
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
} // onSelectionPointDestroying

//------------------------------------------------------------------------------
/*! Informs the graphical object that one of its labels is going to be destroyed.

    On clearing the drawing scene all graphical objects will be destroyed.
    Labels may be destroyed before its parent object the labels belong to.
    The parent object got to be informed if the label will be destroyed by
    someone else.

    @param i_pLabel [in]
        Pointer to label which will be destroyed.
*/
void CGraphObj::onLabelAboutToBeDestroyed(CGraphObjLabel* i_pLabel)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pLabel == nullptr ? "nullptr" : i_pLabel->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::onLabelAboutToBeDestroyed",
        /* strAddInfo   */ strMthInArgs );

    if( m_arpLabels.contains(i_pLabel)) {
        m_arpLabels.removeOne(i_pLabel);
    }
    else if (m_arpPosLabels.contains(i_pLabel->getKey())) {
        m_arpPosLabels.remove(i_pLabel->getKey());
    }
    else if (m_arpDimLineLabels.contains(i_pLabel->getKey())) {
        m_arpDimLineLabels.remove(i_pLabel->getKey());
    }
}

/*==============================================================================
public: // instance methods (simulation methods)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::addMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addMousePressEventFunction",
        /* strAddInfo   */ strMthInArgs );

    for (int idx = 0; idx < m_arMousePressEventFunctions.size(); idx++) {
        SGraphObjMouseEventFct fctEntry = m_arMousePressEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }
    m_arMousePressEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
}

//------------------------------------------------------------------------------
void CGraphObj::removeMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeMousePressEventFunction",
        /* strAddInfo   */ strMthInArgs );

    int idxFct = -1;
    for (int idx = 0; idx < m_arMousePressEventFunctions.size(); idx++) {
        SGraphObjMouseEventFct fctEntry = m_arMousePressEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            idxFct = idx;
            break;
        }
    }
    if (idxFct < 0) {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }
    m_arMousePressEventFunctions.removeAt(idxFct);
}

//------------------------------------------------------------------------------
void CGraphObj::addMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addMouseReleaseEventFunction",
        /* strAddInfo   */ strMthInArgs );

    for (int idx = 0; idx < m_arMouseReleaseEventFunctions.size(); idx++) {
        SGraphObjMouseEventFct fctEntry = m_arMouseReleaseEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }
    m_arMouseReleaseEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
}

//------------------------------------------------------------------------------
void CGraphObj::removeMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeMouseReleaseEventFunction",
        /* strAddInfo   */ strMthInArgs );

    int idxFct = -1;
    for (int idx = 0; idx < m_arMouseReleaseEventFunctions.size(); idx++) {
        SGraphObjMouseEventFct fctEntry = m_arMouseReleaseEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            idxFct = idx;
            break;
        }
    }
    if (idxFct < 0) {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }
    m_arMouseReleaseEventFunctions.removeAt(idxFct);
}

//------------------------------------------------------------------------------
void CGraphObj::addMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addMouseDoubleClickEventFunction",
        /* strAddInfo   */ strMthInArgs );

    for (int idx = 0; idx < m_arMouseDoubleClickEventFunctions.size(); idx++) {
        SGraphObjMouseEventFct fctEntry = m_arMouseDoubleClickEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }
    m_arMouseDoubleClickEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
}

//------------------------------------------------------------------------------
void CGraphObj::removeMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeMouseDoubleClickEventFunction",
        /* strAddInfo   */ strMthInArgs );

    int idxFct = -1;
    for (int idx = 0; idx < m_arMouseDoubleClickEventFunctions.size(); idx++) {
        SGraphObjMouseEventFct fctEntry = m_arMouseDoubleClickEventFunctions[idxFct];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            idxFct = idx;
            break;
        }
    }
    if (idxFct < 0) {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }
    m_arMouseDoubleClickEventFunctions.removeAt(idxFct);
}

//------------------------------------------------------------------------------
void CGraphObj::addMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addMouseMoveEventFunction",
        /* strAddInfo   */ strMthInArgs );

    for (int idx = 0; idx < m_arMouseMoveEventFunctions.size(); idx++) {
        SGraphObjMouseEventFct fctEntry = m_arMouseMoveEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }
    m_arMouseMoveEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );
}

//------------------------------------------------------------------------------
void CGraphObj::removeMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeMouseMoveEventFunction",
        /* strAddInfo   */ strMthInArgs );

    int idxFct = -1;
    for (int idx = 0; idx < m_arMouseMoveEventFunctions.size(); idx++) {
        SGraphObjMouseEventFct fctEntry = m_arMouseMoveEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            idxFct = idx;
            break;
        }
    }
    if (idxFct < 0) {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }
    m_arMouseMoveEventFunctions.removeAt(idxFct);
}

//------------------------------------------------------------------------------
void CGraphObj::addKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addKeyPressEventFunction",
        /* strAddInfo   */ strMthInArgs );

    for (int idx = 0; idx < m_arKeyPressEventFunctions.size(); idx++) {
        SGraphObjKeyEventFct fctEntry = m_arKeyPressEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }
    m_arKeyPressEventFunctions.append( SGraphObjKeyEventFct(i_pFct,i_pvThis,i_pvData) );
}

//------------------------------------------------------------------------------
void CGraphObj::removeKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeKeyPressEventFunction",
        /* strAddInfo   */ strMthInArgs );

    int idxFct = -1;
    for (int idx = 0; idx < m_arKeyPressEventFunctions.size(); idx++) {
        SGraphObjKeyEventFct fctEntry = m_arKeyPressEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            idxFct = idx;
            break;
        }
    }
    if (idxFct < 0) {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }
    m_arKeyPressEventFunctions.removeAt(idxFct);
}

//------------------------------------------------------------------------------
void CGraphObj::addKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::addKeyReleaseEventFunction",
        /* strAddInfo   */ strMthInArgs );

    for (int idx = 0; idx < m_arKeyReleaseEventFunctions.size(); idx++) {
        SGraphObjKeyEventFct fctEntry = m_arKeyReleaseEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }
    m_arKeyReleaseEventFunctions.append( SGraphObjKeyEventFct(i_pFct,i_pvThis,i_pvData) );
}

//------------------------------------------------------------------------------
void CGraphObj::removeKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::removeKeyReleaseEventFunction",
        /* strAddInfo   */ strMthInArgs );

    int idxFct = -1;
    for (int idx = 0; idx < m_arKeyReleaseEventFunctions.size(); idx++) {
        SGraphObjKeyEventFct fctEntry = m_arKeyReleaseEventFunctions[idx];
        if (fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData) {
            idxFct = idx;
            break;
        }
    }
    if (idxFct < 0) {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }
    m_arKeyReleaseEventFunctions.removeAt(idxFct);
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::updateTransform()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateTransform",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (pGraphicsItem != nullptr)
    {
        QTransform trs;
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        trs.translate( m_ptRotOriginCurr.x(), m_ptRotOriginCurr.y() );
        trs.rotate(-m_fRotAngleCurr_deg);
        trs.translate( -m_ptRotOriginCurr.x(), -m_ptRotOriginCurr.y() );
        //trsNew.scale( m_fScaleFacXCurr, m_fScaleFacYCurr );
#endif
        pGraphicsItem->resetTransform();
        pGraphicsItem->setTransform(trs);
    }
} // updateTransform

//------------------------------------------------------------------------------
void CGraphObj::updateToolTip()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateToolTip",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (pGraphicsItem != nullptr)
    {
        QString strNodeSeparator = m_pDrawingScene->getGraphObjNameNodeSeparator();
        QPointF ptPos;

        m_strToolTip = "ObjPath:\t\t" + path();

        // "scenePos" returns mapToScene(0,0). This is NOT equivalent to the
        // position of the item's top left corner before applying the rotation
        // transformation matrix but includes the transformation. What we want
        // (or what I want) is the position of the item before rotating the item
        // around the rotation origin point. In contrary it looks like "pos"
        // always returns the top left corner before rotating the object.

        if( pGraphicsItem->parentItem() != nullptr )
        {
            ptPos = pGraphicsItem->pos();
            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
        }
        else
        {
            ptPos = pGraphicsItem->pos(); // don't use "scenePos" here (see comment above)
            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
        }

        m_strToolTip += "\nSize:\t\t" + getSize().toString();
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + c_strSymbolDegree;
#endif
        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());

        pGraphicsItem->setToolTip(m_strToolTip);
    }
} // updateToolTip

//------------------------------------------------------------------------------
void CGraphObj::updateEditInfo()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateEditInfo",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr)
    {
        QString strAngleSymbol = QString(QChar(8738));
        m_strEditInfo  = "C:" + point2Str( pGraphicsItem->mapToScene(pGraphicsItem->boundingRect().center()) );
        m_strEditInfo += ", W:" + QString::number(pGraphicsItem->boundingRect().width(),'f',1);
        m_strEditInfo += ", H:" + QString::number(pGraphicsItem->boundingRect().height(),'f',1);
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        m_strEditInfo += ", " + strAngleSymbol + ":" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + c_strSymbolDegree;
#endif
    }
}

//------------------------------------------------------------------------------
void CGraphObj::updateLabelPositionsAndContents()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateLabelPositionsAndContents",
        /* strAddInfo   */ "" );

    updateLabelPositions(m_arpLabels);
    updateLabelPositions(m_arpPosLabels);
    updateLabelPositions(m_arpDimLineLabels);

    updatePositionLabelsContent();
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::updateLabelPositions( QList<CGraphObjLabel*>& i_arpLabels )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Labels [" + QString::number(i_arpLabels.size()) + "]";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateLabelPositions",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    QPointF ptLabel;
    for (CGraphObjLabel* pGraphObjLabel : i_arpLabels)
    {
        QPointF ptSelPt = getSelectionPointCoors(pGraphObjLabel->getLinkedSelectionPoint());
        ptSelPt = pGraphicsItem->mapToScene(ptSelPt);
        QSizeF sizeDistance = pGraphObjLabel->getLinkedSelectionPointDistance();
        ptLabel.setX( ptSelPt.x() + sizeDistance.width() );
        ptLabel.setY( ptSelPt.y() + sizeDistance.height() );
        pGraphObjLabel->setPos(ptLabel);
    }
} // updateLabelPositions

//------------------------------------------------------------------------------
void CGraphObj::updateLabelPositions( QHash<QString, CGraphObjLabel*>& i_arpLabels )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Labels [" + QString::number(i_arpLabels.size()) + "]";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updateLabelPositions",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QHashIterator<QString, CGraphObjLabel*> itLabels(i_arpLabels);

        QPointF ptLabel;
        while (itLabels.hasNext())
        {
            itLabels.next();
            CGraphObjLabel* pGraphObjLabel = itLabels.value();
            if (pGraphObjLabel != nullptr)
            {
                QPointF ptSelPt = getSelectionPointCoors(pGraphObjLabel->getLinkedSelectionPoint());
                ptSelPt = pGraphicsItem->mapToScene(ptSelPt);
                QSizeF sizeDistance = pGraphObjLabel->getLinkedSelectionPointDistance();
                ptLabel.setX( ptSelPt.x() + sizeDistance.width() );
                ptLabel.setY( ptSelPt.y() + sizeDistance.height() );
                pGraphObjLabel->setPos(ptLabel);
            }
        }
    }
} // updateLabelPositions

//------------------------------------------------------------------------------
void CGraphObj::updatePositionLabelsContent()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::updatePositionLabelsContent",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr)
    {
        QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpPosLabels);
        while (itLabels.hasNext())
        {
            itLabels.next();
            CGraphObjLabel* pGraphObjLabel = itLabels.value();
            if (pGraphObjLabel != nullptr) {
                // Coordinates of the selection point in relation to the parent (thats me).
                QPointF ptSelPt = getSelectionPointCoors(pGraphObjLabel->getLinkedSelectionPoint());
                // Got to map the coordinates to my parent (which might be the scene).
                ptSelPt = pGraphicsItem->mapToParent(ptSelPt);
                QString strText = point2Str(ptSelPt);
                pGraphObjLabel->setText(strText);
            }
        }
    }
} // updatePositionLabelsContent

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::showLabel(
    QHash<QString, CGraphObjLabel*>& i_arpLabels,
    const QString& i_strKey,
    const QString& i_strText,
    ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    if (i_arpLabels.contains(i_strKey)) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultObjAlreadyInList, i_strKey);
    }

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "dynamic_cast<QGraphicsItem*>(this)");
    }

    CGraphObjLabel* pGraphObjLabel = new CGraphObjLabel(
        m_pDrawingScene, this, i_strKey, i_strText, i_selPt);
    m_pDrawingScene->addGraphObj(pGraphObjLabel, this);

    i_arpLabels.insert(i_strKey, pGraphObjLabel);

    QPointF ptSelPt = getSelectionPointCoors(i_selPt);

    ptSelPt = pGraphicsItem->mapToScene(ptSelPt);

    QPointF ptLabelTmp = ptSelPt;

    if (i_selPt != ESelectionPoint::BottomRight &&
        i_selPt != ESelectionPoint::BottomLeft &&
        i_selPt != ESelectionPoint::BottomCenter)
    {
        ptLabelTmp.setY(ptLabelTmp.y() - pGraphicsItem->boundingRect().height());
    }

    bool bUniquePos = false;

    while( !bUniquePos )
    {
        bUniquePos = true;

        QHashIterator<QString, CGraphObjLabel*> itLabels(i_arpLabels);

        while( itLabels.hasNext() )
        {
            itLabels.next();

            CGraphObjLabel* pGraphObjLabelTmp = itLabels.value();

            if( pGraphObjLabelTmp->getKey() != i_strKey )
            {
                if( pGraphObjLabelTmp->scenePos() == ptLabelTmp )
                {
                    bUniquePos = false;
                    ptLabelTmp.setX(pGraphObjLabelTmp->scenePos().x() + pGraphicsItem->boundingRect().width() + 4);
                    break;
                }
            }
        }
    }

    QSize sizeDist(ptLabelTmp.x() - ptSelPt.x(), ptLabelTmp.y() - ptSelPt.y());

    //pGraphObjLabel->m_bDistValid = true;

    QPointF ptLabel( ptSelPt.x() + sizeDist.width(), ptSelPt.y() + sizeDist.height() );

    pGraphObjLabel->setPos(ptLabel);
    pGraphObjLabel->setZValue(pGraphicsItem->zValue() + 0.02);

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

} // showLabel

//------------------------------------------------------------------------------
void CGraphObj::hideLabel( QHash<QString, CGraphObjLabel*>& i_arpLabels, const QString& i_strKey )
//------------------------------------------------------------------------------
{
    CGraphObjLabel* pGraphObjLabel = i_arpLabels.value(i_strKey, nullptr);

    //m_pDrawingScene->removeGraphObj(pGraphObjLabel); // the dtor of the label removes itself from the drawing scene

    delete pGraphObjLabel;
    pGraphObjLabel = nullptr;

    if( m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

} // hideLabel

//------------------------------------------------------------------------------
void CGraphObj::destroyLabels()
//------------------------------------------------------------------------------
{
    delete m_pNameLabel;
    m_pNameLabel = nullptr;

    // Local copy as "onLabelAboutToBeDestroyed" will remove the label from the list.
    QList<CGraphObjLabel*> arpLabels = m_arpLabels;
    for (CGraphObjLabel* pGraphObjLabel : arpLabels) {
        delete pGraphObjLabel;
        pGraphObjLabel = nullptr;
    }
    // Should already be empty.
    m_arpLabels.clear();

    // Local copy as "onLabelAboutToBeDestroyed" will remove the label from the hash.
    QHash<QString, CGraphObjLabel*> arpPosLabels = m_arpPosLabels;
    QHashIterator<QString, CGraphObjLabel*> itPosLabels(arpPosLabels);
    while (itPosLabels.hasNext()) {
        itPosLabels.next();
        CGraphObjLabel* pGraphObjLabel = itPosLabels.value();
        try {
            delete pGraphObjLabel;
        }
        catch(...) {
        }
        pGraphObjLabel = nullptr;
    }
    // Should already be empty.
    arpPosLabels.clear();

    // Local copy as "onLabelAboutToBeDestroyed" will remove the label from the hash.
    QHash<QString, CGraphObjLabel*> arpDimLineLabels = m_arpDimLineLabels;
    QHashIterator<QString, CGraphObjLabel*> itDimLineLabels(arpDimLineLabels);
    while (itDimLineLabels.hasNext()) {
        itDimLineLabels.next();
        CGraphObjLabel* pGraphObjLabel = itDimLineLabels.value();
        try {
            delete pGraphObjLabel;
        }
        catch(...) {
        }
        pGraphObjLabel = nullptr;
    }
    // Should already be empty.
    arpDimLineLabels.clear();

} // destroyLabels

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::emit_selectedChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_selectedChanged",
        /* strAddInfo   */ "" );

    emit selectedChanged();
}

//------------------------------------------------------------------------------
void CGraphObj::emit_geometryChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "CGraphObj::emit_geometryChanged",
        /* strAddInfo   */ "" );

    emit geometryChanged();
}

/*==============================================================================
protected: // overridable auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::traceInternalStates(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        QRectF rctBounding;
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            rctBounding = pGraphicsItem->boundingRect();
        }
        QString strMthInArgs;
        if (i_mthDir == EMethodDir::Enter) strMthInArgs = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strMthInArgs = "+- ";
        else strMthInArgs = "";
        strMthInArgs +=
            "IsHit: " + bool2Str(m_bIsHit) +
            ", EditMode: " + m_editMode.toString() +
            ", ResizeMode: " + m_editResizeMode.toString() +
            ", BoundRectVisible: " + bool2Str(m_bBoundRectVisible);
        i_mthTracer.trace(strMthInArgs);

        if (i_mthDir == EMethodDir::Enter) strMthInArgs = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strMthInArgs = "+- ";
        else strMthInArgs = "";
        strMthInArgs +=
            "SelPtPolygon: " + QString::number(m_idxSelPtSelectedPolygon) +
            ", SelPts [" + QString::number(m_arpSelPtsPolygon.size()) + "]";
        //if (m_arpSelPtsPolygon.size() > 0 && i_detailLevel > ELogDetailLevel::Debug) {
        //    strMthInArgs += "(";
        //    for (int idx = 0; idx < m_arpSelPtsPolygon.size(); ++idx) {
        //        if (idx > 0) strMthInArgs += ", ";
        //        strMthInArgs += "[" + QString::number(idx) + "] " +
        //            m_arpSelPtsPolygon[idx]->toolTip();
        //    }
        //    strMthInArgs += ")";
        //}
        i_mthTracer.trace(strMthInArgs);

        if (i_mthDir == EMethodDir::Enter) strMthInArgs = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strMthInArgs = "+- ";
        else strMthInArgs = "";
        strMthInArgs +=
            "SelPtBoundingRect: " + QString(m_selPtSelectedBoundingRect.isValid() ? m_selPtSelectedBoundingRect.toString() : "None") +
            ", SelPts [" + QString::number(m_arpSelPtsBoundingRect.size()) + "]";
        //if (m_arpSelPtsBoundingRect.size() > 0 && i_detailLevel > ELogDetailLevel::Debug) {
        //    strMthInArgs += "(";
        //    for (int idx = 0; idx < m_arpSelPtsBoundingRect.size(); ++idx) {
        //        if (idx > 0) strMthInArgs += ", ";
        //        strMthInArgs += "[" + QString::number(idx) + "] " +
        //            m_arpSelPtsBoundingRect[idx]->toolTip();
        //    }
        //    strMthInArgs += ")";
        //}
        i_mthTracer.trace(strMthInArgs);

        if (i_mthDir == EMethodDir::Enter) strMthInArgs = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strMthInArgs = "+- ";
        else strMthInArgs = "";
        strMthInArgs +=
            QString("CurrCoors {") +
                "Rect {" + qRect2Str(rctBounding) + "}" +
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
                ", RotAngle: " + QString::number(m_fRotAngleCurr_deg) + QString::fromLatin1("�") + "}" +
#endif
                ", RotPos {" + point2Str(rctBounding.center()) + "}" +
            "}";
        i_mthTracer.trace(strMthInArgs);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        if (i_mthDir == EMethodDir::Enter) strMthInArgs = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strMthInArgs = "+- ";
        else strMthInArgs = "";
        strMthInArgs += "HasValidOrigCoors: " + bool2Str(m_bHasValidOrigCoors);
        if (m_bHasValidOrigCoors ) {
            strMthInArgs +=
                QString(", OrigCoors {") +
                    "PtPos {" + qPoint2Str(m_ptPosOrig) + "}" +
                    ", Size {" + qSize2Str(m_sizOrig) + "}" +
                    ", RotAngle: " + QString::number(m_fRotAngleOrig_deg) + QString::fromLatin1("�") + "}" +
                    ", RotPos {" + qPoint2Str(m_ptRotOriginOrig) + "}" +
                "}";
        }
        i_mthTracer.trace(strMthInArgs);
#endif

        if (i_mthDir == EMethodDir::Enter) strMthInArgs = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strMthInArgs = "+- ";
        else strMthInArgs = "";
        strMthInArgs +=
            QString("MousePressEvents {") +
                "ScenePos {" + qPoint2Str(m_ptScenePosOnMousePressEvent) + "}" +
                ", MouseEvScenePos {" + qPoint2Str(m_ptMouseEvScenePosOnMousePressEvent) + "}" +
                ", Rect {" + qRect2Str(m_rctOnMousePressEvent) + "}" +
                ", RotPos {" + qPoint2Str(m_ptRotOriginOnMousePressEvent) +
            "}";
        i_mthTracer.trace(strMthInArgs);
    }
}