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

#include <QtGui/QBitmap>

#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Trace;


/*==============================================================================
struct SGraphObjHitInfo
==============================================================================*/

/* public: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
SGraphObjHitInfo::SGraphObjHitInfo() :
//------------------------------------------------------------------------------
    m_editMode(EEditMode::Undefined),
    m_editResizeMode(EEditResizeMode::Undefined),
    m_selPtBoundingRect(ESelectionPoint::Undefined),
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

    if( m_editMode == EEditMode::Resize && m_editResizeMode != EEditResizeMode::Undefined )
    {
        bIsHit = (m_selPtBoundingRect >= ESelectionPoint::RectMin) && (m_selPtBoundingRect <= ESelectionPoint::RectMax);
    }
    return bIsHit;

} // isBoundingRectSelectionPointHit

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isSelectionPointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    if( m_editMode != EEditMode::Undefined )
    {
        bIsHit = (m_selPtBoundingRect != ESelectionPoint::Undefined);
    }
    return bIsHit;

} // isSelectionPointHit

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isPolygonShapePointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    if( m_editMode == EEditMode::MoveShapePoint )
    {
        bIsHit = (m_idxPolygonShapePoint >= 0);
    }
    return bIsHit;

} // isPolygonShapePointHit

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isLineSegmentHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    if( m_editMode != EEditMode::Undefined )
    {
        bIsHit = (m_idxLineSegment >= 0);
    }
    return bIsHit;

} // isLineSegmentHit

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
void SGraphObjHitInfo::setCursor( double i_fGraphObjRotAngle_rad )
//------------------------------------------------------------------------------
{
    double fCursorAngle_rad = 0.0;
    bool   bSetResizeCursor = false;

    switch( m_editMode.enumerator() )
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
        // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180°)
        while( fCursorAngle_rad >= Math::c_f180Degrees_rad )
        {
            fCursorAngle_rad -= Math::c_f180Degrees_rad;
        }
        while( fCursorAngle_rad < 0.0 )
        {
            fCursorAngle_rad += Math::c_f180Degrees_rad;
        }
        if( fCursorAngle_rad >= 0.0 && fCursorAngle_rad < Math::c_f45Degrees_rad/2.0 ) // 0.0 .. 22.5°
        {
            m_cursor = Qt::SizeHorCursor;
        }
        else if( fCursorAngle_rad >= Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 3.0*Math::c_f45Degrees_rad/2.0 ) // 22.5° .. 67.5°
        {
            m_cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
        }
        else if( fCursorAngle_rad >= 3.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 5.0*Math::c_f45Degrees_rad/2.0 ) // 67.5° .. 112.5°
        {
            m_cursor = Qt::SizeVerCursor;
        }
        else if( fCursorAngle_rad >= 5.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 7.0*Math::c_f45Degrees_rad/2.0 ) // 112.5° .. 157.5°
        {
            m_cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
        }
        else if( fCursorAngle_rad >= 7.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < Math::c_f180Degrees_rad ) // 157.5° .. 180.0°
        {
            m_cursor = Qt::SizeHorCursor;
        }

    } // if( bSetResizeCursor )

} // setCursor


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

} // graphObjAlignment2Str



/*******************************************************************************
class CGraphObj
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CGraphObj::c_strKeyLabelObjName = "ObjName";
const QString CGraphObj::c_strKeyLabelObjId   = "ObjId";

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates an graphic object instance.

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
    CDrawingScene*       i_pDrawingScene,
    const QString&       i_strFactoryGroupName,
    EGraphObjType        i_type,
    const QString&       i_strType,
    const QString&       i_strObjName,
    const CDrawSettings& i_settings,
    EIdxTreeEntryType    i_idxTreeEntryType ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(i_idxTreeEntryType, i_strObjName),
    m_pDrawingScene(i_pDrawingScene),
    m_strFactoryGroupName(i_strFactoryGroupName),
    m_type(i_type),
    m_strType(i_strType),
    m_drawSettings(i_type),
    m_sizMinimum(),
    m_sizMaximum(),
    m_sizFixed(),
    m_arAlignments(),
    m_bIsHit(false),
    m_editMode(EEditMode::Undefined),
    m_editResizeMode(EEditResizeMode::Undefined),
    m_fZValue(0.0),
    m_bDtorInProgress(false),
    m_idxSelPtSelectedPolygon(-1),
    m_arpSelPtsPolygon(),
    m_selPtSelectedBoundingRect(ESelectionPoint::Undefined),
    m_arpSelPtsBoundingRect(CEnumSelectionPoint::count()),
    m_arpLabels(),
    m_strToolTip(),
    m_strEditInfo(),
    // Current item coordinates and transform values:
    m_rctCurr(),
    m_fRotAngleCurr_deg(0.0),
    m_ptRotOriginCurr(),
    // Original coordinates and transform values:
    m_bHasValidOrigCoors(false),
    m_ptPosOrig(),
    m_sizOrig(),
    m_fRotAngleOrig_deg(0.0),
    m_ptRotOriginOrig(),
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
    m_pTrcAdminObjPaint(nullptr),
    m_pTrcAdminObjSceneEvent(nullptr),
    m_pTrcAdminObjSceneEventFilter(nullptr),
    m_pTrcAdminObjHoverEvents(nullptr),
    m_pTrcAdminObjMouseEvents(nullptr),
    m_pTrcAdminObjKeyEvents(nullptr)
{
    for( int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
    {
        if( m_drawSettings.isAttributeUsed(idxAttr) )
        {
            m_drawSettings.setAttribute( idxAttr, i_settings.getAttribute(idxAttr) );
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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;

    CGraphObjSelectionPoint* pGraphObjSelPt;
    int                      idxSelPt;

    if( m_arpSelPtsBoundingRect.size() > 0 )
    {
        for( idxSelPt = m_arpSelPtsBoundingRect.size()-1; idxSelPt >= 0; idxSelPt-- )
        {
            pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                m_arpSelPtsBoundingRect[idxSelPt] = nullptr;

                try
                {
                    delete pGraphObjSelPt;
                }
                catch(...)
                {
                }
                pGraphObjSelPt = nullptr;
            }
        }
    }

    if( m_arpSelPtsPolygon.size() > 0 )
    {
        for( idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt-- )
        {
            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                m_arpSelPtsPolygon[idxSelPt] = nullptr;

                try
                {
                    delete pGraphObjSelPt;
                }
                catch(...)
                {
                }
                pGraphObjSelPt = nullptr;
            }
        }
    }

    QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
    SGraphObjLabel* pGraphObjLabel;

    while( itLabels.hasNext() )
    {
        itLabels.next();
        pGraphObjLabel = itLabels.value();

        try
        {
            delete pGraphObjLabel->m_pGraphObjLabel;
        }
        catch(...)
        {
        }
        pGraphObjLabel->m_pGraphObjLabel = nullptr;

        try
        {
            delete pGraphObjLabel;
        }
        catch(...)
        {
        }
        pGraphObjLabel = nullptr;
    }

    //if( m_pDrawingScene != nullptr )
    //{
    //    // Please note that "onGraphObjDestroyed" is used to remove the graphical object from
    //    // the dictionary, the index list, and the sorted object pools of the drawing scene.
    //    // But selection points have just been added to the drawing scene's item list and
    //    // therefore don't have valid object ids.
    //    if( !m_strKeyInTree.isEmpty() )
    //    {
    //        try
    //        {
    //            m_pDrawingScene->onGraphObjDestroyed(m_strKeyInTree);
    //        }
    //        catch(...)
    //        {
    //        }
    //    }
    //    else
    //    {
    //        // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
    //        // dtor of QGraphicsItem has already been executed. The order the dtors
    //        // of QGraphicsItem has already been executed. The order the dtors
    //        // of inherited classes are called depend on the order the classes
    //        // appear in the list of the inherited classes on defining the
    //        // class implementation. So we can't call "removeItem" here but must
    //        // remove the graphics item from the drawing scene's item list before
    //        // the dtor of class QGraphicsItem is called. And this is only always
    //        // the case in the dtor of the class derived from QGraphicsItem.
    //        //QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    //        //m_pDrawingScene->removeItem(pGraphicsItem);
    //    }
    //} // if( m_pDrawingScene != nullptr )

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCtorsAndDtor);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjItemChange);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjBoundingRect);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaint);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEvent);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEventFilter);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjHoverEvents);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseEvents);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjKeyEvents);

    m_pDrawingScene = nullptr;
    //m_strFactoryGroupName;
    m_type = static_cast<EGraphObjType>(0);
    //m_strType;
    //m_drawSettings;
    //m_sizMinimum;
    //m_sizMaximum;
    //m_sizFixed;
    //m_arAlignments;
    m_bIsHit = false;
    m_editMode = static_cast<EEditMode>(0);
    m_editResizeMode = static_cast<EEditResizeMode>(0);
    m_fZValue = 0.0;
    m_bDtorInProgress = false;
    m_idxSelPtSelectedPolygon = 0;
    //m_arpSelPtsPolygon;
    m_selPtSelectedBoundingRect = static_cast<ESelectionPoint>(0);
    //m_arpSelPtsBoundingRect;
    //m_arpLabels;
    //m_strToolTip;
    //m_strEditInfo;
    // Current item coordinates and transform values:
    //m_rctCurr;
    m_fRotAngleCurr_deg = 0.0;
    //m_ptRotOriginCurr;
    // Original item coordinates and transform values:
    m_bHasValidOrigCoors = false;
    //m_ptPosOrig;
    //m_sizOrig;
    m_fRotAngleOrig_deg = 0.0;
    //m_ptRotOriginOrig;
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
    m_pTrcAdminObjPaint = nullptr;
    m_pTrcAdminObjSceneEvent = nullptr;
    m_pTrcAdminObjSceneEventFilter = nullptr;
    m_pTrcAdminObjHoverEvents = nullptr;
    m_pTrcAdminObjMouseEvents = nullptr;
    m_pTrcAdminObjKeyEvents = nullptr;

} // dtor

/*==============================================================================
protected: // overridables of base class CIdxTreeEntry
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setName",
        /* strAddInfo   */ strMthInArgs );

    if( m_strName != i_strName )
    {
        QString strNameOld = m_strName;
        QString strNameNew = i_strName;

        m_strName = i_strName;

        //m_pDrawingScene->onGraphObjNameChanged(m_strKeyInTree, strNameOld, strNameNew);

        if( m_arpLabels.contains(c_strKeyLabelObjName) )
        {
            SGraphObjLabel* pGraphObjLabel = m_arpLabels[c_strKeyLabelObjName];

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                pGraphObjLabel->m_pGraphObjLabel->setText(m_strName);
            }
        }

        updateEditInfo();
        updateToolTip();

    } // if( m_strName != i_strName )

} // setName

//------------------------------------------------------------------------------
void CGraphObj::setKeyInTree( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strKey;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setKeyInTree",
        /* strAddInfo   */ strMthInArgs );

    if( m_strKeyInTree != i_strKey )
    {
        QString strKeyOld = m_strKeyInTree;
        QString strKeyNew = i_strKey;

        m_strKeyInTree = i_strKey;

        //// Please note that on calling "onGraphObjIdChanged" the drawing scene
        //// may correct my object id on calling "setObjId" as a reentry. If we would
        //// pass a reference to "m_strObjId" the argument will be changed on executing
        //// the "onGraphObjIdChanged" method and after calling "setObjId" the new but
        //// not the old object id would be passed around by the drawing scene on emitting
        //// the "graphObjIdChanged" signal. Thats why we first assign the object id to
        //// a QString variable on the stack.

        //m_pDrawingScene->onGraphObjIdChanged(strKeyOld, strKeyNew);

        if( m_arpLabels.contains(c_strKeyLabelObjId) )
        {
            SGraphObjLabel* pGraphObjLabel = m_arpLabels[c_strKeyLabelObjId];

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                pGraphObjLabel->m_pGraphObjLabel->setText(m_strKeyInTree);
            }
        }

        updateEditInfo();
        updateToolTip();

    } // if( m_strObjId != i_strObjId )

} // setKeyInTree

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObj::parentGraphObj()
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(parentBranch());

    // Please note that selection points and labels should not belong as child to the graphics items.
    // Otherwise the "boundingRect" call of groups (which implicitly calls childrenBoundingRect) does
    // not work as the childs bounding rectangles would be included. But the selection points and
    // labels should appear as childs in the index tree of the drawing scene.

    if( m_type != EGraphObjTypeSelectionPoint && m_type != EGraphObjTypeLabel )
    {
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        QGraphicsItem* pGraphicsItemParent = pGraphicsItemThis->parentItem();
        CGraphObj*     pGraphObjParentTmp = dynamic_cast<CGraphObj*>(pGraphicsItemParent);

        if( pGraphObjParentTmp != pGraphObjParent )
        {
            throw ZS::System::CException(__FILE__, __LINE__, EResultInternalProgramError, "pGraphObjParent != pGraphicsItemParent");
        }
    }

    return pGraphObjParent;

} // parentGraphObj

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onCreateAndExecDlgFormatGraphObjs",
        /* strAddInfo   */ strMthInArgs );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,this);

    pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtObjName);

    pDlgFormatGraphObjs->exec();

    delete pDlgFormatGraphObjs;
    pDlgFormatGraphObjs = nullptr;

} // onCreateAndExecDlgFormatGraphObjs

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setDrawSettings( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setDrawSettings",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings = i_drawSettings;

    onDrawSettingsChanged();

} // setDrawSettings

//------------------------------------------------------------------------------
void CGraphObj::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    /* Template for copy and paste in inherited classes:

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( m_drawSettings.isLineUsed() )
        {
            if( m_drawSettings.getLineStyle() != ELineStyle::NoLine )
            {
                QPen pen;

                pen.setColor( m_drawSettings.getPenColor() );
                pen.setWidth( m_drawSettings.getPenWidth() );
                pen.setStyle( lineStyle2QtPenStyle(m_drawSettings.getLineStyle()) );

                setPen(pen);
            }
            else
            {
                setPen(Qt::NoPen);
            }
        }
        else
        {
            setPen(Qt::NoPen);
        }

        if( m_drawSettings.isFillUsed() )
        {
            if( m_drawSettings.getFillStyle() != EFillStyle::NoFill )
            {
                QBrush brsh;

                brsh.setColor( m_drawSettings.getFillColor() );
                brsh.setStyle( fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()) );

                pGraphicsItem->setBrush(brsh);
            }
            else
            {
                setBrush(Qt::NoBrush);
            }
        }
        else
        {
            setBrush(Qt::NoBrush);
        }

    } // if( pGraphicsItem != nullptr )
    */

} // onDrawSettingsChanged

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setPenColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_clr.name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setPenColor",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setPenColor(i_clr);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setPenColor

//------------------------------------------------------------------------------
void CGraphObj::setPenWidth( int i_iLineWidth, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_iLineWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setPenWidth",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setPenWidth(i_iLineWidth);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setPenWidth

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setLineStyle( ELineStyle i_lineStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLineStyle(i_lineStyle).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineStyle",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setLineStyle(i_lineStyle);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setLineStyle

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setFillColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_clr.name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setFillColor",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setFillColor(i_clr);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setFillColor

//------------------------------------------------------------------------------
void CGraphObj::setFillStyle( EFillStyle i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumFillStyle(i_fillStyle).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setFillStyle",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setFillStyle(i_fillStyle);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setFillStyle

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setLineRecordType( ELineRecordType i_lineRecordType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLineRecordType(i_lineRecordType).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineRecordType",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setLineRecordType(i_lineRecordType);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setLineRecordType

//------------------------------------------------------------------------------
void CGraphObj::setLineExtent( int i_iLineExtent, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_iLineExtent);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineExtent",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setLineExtent(i_iLineExtent);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setLineExtent

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setLineEndStyle( ELinePoint i_linePoint, ELineEndStyle i_endStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndStyle(i_endStyle).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineEndStyle",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setLineEndStyle(i_linePoint,i_endStyle);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setLineEndStyle

//------------------------------------------------------------------------------
void CGraphObj::setLineEndBaseLineType( ELinePoint i_linePoint, ELineEndBaseLineType i_baseLineType, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndBaseLineType(i_baseLineType).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineEndBaseLineType",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setLineEndBaseLineType(i_linePoint,i_baseLineType);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setLineEndBaseLineType

//------------------------------------------------------------------------------
void CGraphObj::setLineEndFillStyle( ELinePoint i_linePoint, ELineEndFillStyle i_fillStyle, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndFillStyle(i_fillStyle).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineEndFillStyle",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setLineEndFillStyle(i_linePoint,i_fillStyle);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setLineEndFillStyle

//------------------------------------------------------------------------------
void CGraphObj::setLineEndWidth( ELinePoint i_linePoint, ELineEndWidth i_width, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndWidth(i_width).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineEndWidth",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setLineEndWidth(i_linePoint,i_width);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setLineEndWidth

//------------------------------------------------------------------------------
void CGraphObj::setLineEndLength( ELinePoint i_linePoint, ELineEndLength i_length, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString();
        strMthInArgs += ", " + CEnumLineEndLength(i_length).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineEndLength",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setLineEndLength(i_linePoint,i_length);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setLineEndLength

/*==============================================================================
public: // overridables (you must call those methods (instead of e.g. "QGrahicsLineItem::setPen") ..
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setTextColor( const QColor& i_clr, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_clr.name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTextColor",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setTextColor(i_clr);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setTextColor

//------------------------------------------------------------------------------
void CGraphObj::setTextFont( const QFont& i_fnt, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_fnt.family();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTextFont",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setTextFont(i_fnt);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setTextFont

//------------------------------------------------------------------------------
void CGraphObj::setTextSize( ETextSize i_size, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = textSize2Str(i_size);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTextSize",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setTextSize(i_size);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setTextSize

//------------------------------------------------------------------------------
void CGraphObj::setTextStyle( ETextStyle i_style, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumTextStyle(i_style).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTextStyle",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setTextStyle(i_style);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setTextStyle

//------------------------------------------------------------------------------
void CGraphObj::setTextEffect( ETextEffect i_effect, bool i_bImmediatelyApplySetting )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumTextEffect(i_effect).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTextEffect",
        /* strAddInfo   */ strMthInArgs );

    m_drawSettings.setTextEffect(i_effect);

    if( i_bImmediatelyApplySetting )
    {
        onDrawSettingsChanged();
    }

} // setTextEffect

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setMinimumWidth( double i_fWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_fWidth, 'f', 1);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setMinimumWidth",
        /* strAddInfo   */ strMthInArgs );

    m_sizMinimum.setWidth(i_fWidth);

    if( m_sizMinimum.width() > 0.0 )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.width() < m_sizMinimum.width() )
        {
            sizCurr.setWidth( m_sizMinimum.width() );

            setSize(sizCurr);
        }
    }

} // setMinimumWidth

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( m_sizFixed.width() > 0.0 )
    {
        bHas = true;
    }
    else if( m_sizMinimum.width() > 0.0 )
    {
        bHas = true;
    }
    return bHas;

} // hasMinimumWidth

//------------------------------------------------------------------------------
double CGraphObj::getMinimumWidth() const
//------------------------------------------------------------------------------
{
    double fWidth = 0.0;

    if( m_sizFixed.width() > 0.0 )
    {
        fWidth = m_sizFixed.width();
    }
    else if( m_sizMinimum.width() > 0.0 )
    {
        fWidth = m_sizMinimum.width();
    }
    return fWidth;

} // getMinimumWidth

//------------------------------------------------------------------------------
void CGraphObj::setMinimumHeight( double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_fHeight, 'f', 1);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setMinimumHeight",
        /* strAddInfo   */ strMthInArgs );

    m_sizMinimum.setHeight(i_fHeight);

    if( m_sizMinimum.height() > 0.0 )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.height() < m_sizMinimum.height() )
        {
            sizCurr.setHeight( m_sizMinimum.height() );

            setSize(sizCurr);
        }
    }

} // setMinimumHeight

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( m_sizFixed.height() > 0.0 )
    {
        bHas = true;
    }
    else if( m_sizMinimum.height() > 0.0 )
    {
        bHas = true;
    }
    return bHas;

} // hasMinimumHeight

//------------------------------------------------------------------------------
double CGraphObj::getMinimumHeight() const
//------------------------------------------------------------------------------
{
    double fHeight = 0.0;

    if( m_sizFixed.height() > 0.0 )
    {
        fHeight = m_sizFixed.height();
    }
    else if( m_sizMinimum.height() > 0.0 )
    {
        fHeight = m_sizMinimum.height();
    }
    return fHeight;

} // getMinimumHeight

//------------------------------------------------------------------------------
void CGraphObj::setMinimumSize( const QSize& i_siz )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = size2Str(i_siz);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setMinimumSize",
        /* strAddInfo   */ strMthInArgs );

    m_sizMinimum = i_siz;

    if( m_sizMinimum.isValid() )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.width() < m_sizMinimum.width() || sizCurr.height() < m_sizMinimum.height() )
        {
            if( sizCurr.width() < m_sizMinimum.width() )
            {
                sizCurr.setWidth( m_sizMinimum.width() );
            }
            if( sizCurr.height() < m_sizMinimum.height() )
            {
                sizCurr.setHeight( m_sizMinimum.height() );
            }
            setSize(sizCurr);
        }
    }

} // setMinimumSize

//------------------------------------------------------------------------------
bool CGraphObj::hasMinimumSize() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( m_sizFixed.isValid() )
    {
        bHas = true;
    }
    else if( m_sizMinimum.isValid() )
    {
        bHas = true;
    }
    return bHas;

} // hasMinimumSize

//------------------------------------------------------------------------------
QSize CGraphObj::getMinimumSize() const
//------------------------------------------------------------------------------
{
    QSize siz;

    if( m_sizFixed.isValid() )
    {
        siz = m_sizFixed;
    }
    else if( m_sizMinimum.isValid() )
    {
        siz = m_sizMinimum;
    }
    return siz;

} // getMinimumSize

//------------------------------------------------------------------------------
void CGraphObj::setMaximumWidth( double i_fWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_fWidth, 'f', 1);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setMaximumWidth",
        /* strAddInfo   */ strMthInArgs );

    m_sizMaximum.setWidth(i_fWidth);

    if( m_sizMaximum.width() > 0.0 )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.width() > m_sizMaximum.width() )
        {
            sizCurr.setWidth( m_sizMaximum.width() );

            setSize(sizCurr);
        }
    }

} // setMaximumWidth

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( m_sizFixed.width() > 0.0 )
    {
        bHas = true;
    }
    else if( m_sizMaximum.width() > 0.0 )
    {
        bHas = true;
    }
    return bHas;

} // hasMaximumWidth

//------------------------------------------------------------------------------
double CGraphObj::getMaximumWidth() const
//------------------------------------------------------------------------------
{
    double fWidth = 0.0;

    if( m_sizFixed.width() > 0.0 )
    {
        fWidth = m_sizFixed.width();
    }
    else if( m_sizMaximum.width() > 0.0 )
    {
        fWidth = m_sizMaximum.width();
    }
    return fWidth;

} // getMaximumWidth

//------------------------------------------------------------------------------
void CGraphObj::setMaximumHeight( double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setMaximumHeight",
        /* strAddInfo   */ strMthInArgs );

    m_sizMaximum.setHeight(i_fHeight);

    if( m_sizMaximum.height() > 0.0 )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.height() > m_sizMaximum.height() )
        {
            sizCurr.setHeight( m_sizMaximum.height() );

            setSize(sizCurr);
        }
    }

} // setMaximumHeight

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( m_sizFixed.height() > 0.0 )
    {
        bHas = true;
    }
    else if( m_sizMaximum.height() > 0.0 )
    {
        bHas = true;
    }
    return bHas;

} // hasMaximumHeight

//------------------------------------------------------------------------------
double CGraphObj::getMaximumHeight() const
//------------------------------------------------------------------------------
{
    double fHeight = 0.0;

    if( m_sizFixed.height() > 0.0 )
    {
        fHeight = m_sizFixed.height();
    }
    else if( m_sizMaximum.height() > 0.0 )
    {
        fHeight = m_sizMaximum.height();
    }
    return fHeight;

} // getMaximumHeight

//------------------------------------------------------------------------------
void CGraphObj::setMaximumSize( const QSize& i_siz )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = size2Str(i_siz);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setMaximumSize",
        /* strAddInfo   */ strMthInArgs );

    m_sizMaximum = i_siz;

    if( m_sizMaximum.isValid() )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.width() > m_sizMaximum.width() || sizCurr.height() > m_sizMaximum.height() )
        {
            if( sizCurr.width() > m_sizMaximum.width() )
            {
                sizCurr.setWidth( m_sizMaximum.width() );
            }
            if( sizCurr.height() > m_sizMaximum.height() )
            {
                sizCurr.setHeight( m_sizMaximum.height() );
            }
            setSize(sizCurr);
        }
    }

} // setMaximumSize

//------------------------------------------------------------------------------
bool CGraphObj::hasMaximumSize() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( m_sizFixed.isValid() )
    {
        bHas = true;
    }
    else if( m_sizMaximum.isValid() )
    {
        bHas = true;
    }
    return bHas;

} // hasMaximumSize

//------------------------------------------------------------------------------
QSize CGraphObj::getMaximumSize() const
//------------------------------------------------------------------------------
{
    QSize siz;

    if( m_sizFixed.isValid() )
    {
        siz = m_sizFixed;
    }
    else if( m_sizMaximum.isValid() )
    {
        siz = m_sizMaximum;
    }
    return siz;

} // getMaximumSize

//------------------------------------------------------------------------------
void CGraphObj::setFixedWidth( double i_fWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_fWidth, 'f', 1);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setFixedWidth",
        /* strAddInfo   */ strMthInArgs );

    m_sizFixed.setWidth(i_fWidth);

    if( m_sizFixed.width() > 0.0 )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.width() != m_sizFixed.width() )
        {
            sizCurr.setWidth( m_sizFixed.width() );

            setSize(sizCurr);
        }
    }

} // setFixedWidth

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedWidth() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( m_sizFixed.width() > 0.0 )
    {
        bHas = true;
    }
    else if( (m_sizMinimum.width() > 0.0) && (m_sizMinimum.width() == m_sizMaximum.width()) )
    {
        bHas = true;
    }
    return bHas;

} // hasFixedWidth

//------------------------------------------------------------------------------
double CGraphObj::getFixedWidth() const
//------------------------------------------------------------------------------
{
    double fWidth = 0.0;

    if( m_sizFixed.width() > 0.0 )
    {
        fWidth = m_sizFixed.width();
    }
    else if( (m_sizMinimum.width() > 0.0) && (m_sizMinimum.width() == m_sizMaximum.width()) )
    {
        fWidth = m_sizMinimum.width();
    }
    return fWidth;

} // getFixedWidth

//------------------------------------------------------------------------------
void CGraphObj::setFixedHeight( double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_fHeight, 'f', 1);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setFixedHeight",
        /* strAddInfo   */ strMthInArgs );

    m_sizFixed.setHeight(i_fHeight);

    if( m_sizFixed.height() > 0.0 )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.height() != m_sizFixed.height() )
        {
            sizCurr.setHeight( m_sizFixed.height() );

            setSize(sizCurr);
        }
    }

} // setFixedHeight

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedHeight() const
//------------------------------------------------------------------------------
{
    bool bHas = false;

    if( m_sizFixed.height() > 0.0 )
    {
        bHas = true;
    }
    else if( (m_sizMinimum.height() > 0.0) && (m_sizMinimum.height() == m_sizMaximum.height()) )
    {
        bHas = true;
    }
    return bHas;

} // hasFixedHeight

//------------------------------------------------------------------------------
double CGraphObj::getFixedHeight() const
//------------------------------------------------------------------------------
{
    double fHeight = 0.0;

    if( m_sizFixed.height() > 0.0 )
    {
        fHeight = m_sizFixed.height();
    }
    else if( (m_sizMinimum.height() > 0.0) && (m_sizMinimum.height() == m_sizMaximum.height()) )
    {
        fHeight = m_sizMinimum.height();
    }
    return fHeight;

} // getFixedHeight

//------------------------------------------------------------------------------
void CGraphObj::setFixedSize( const QSize& i_siz )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = size2Str(i_siz);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setFixedSize",
        /* strAddInfo   */ strMthInArgs );

    m_sizFixed = i_siz;

    if( m_sizFixed.isValid() )
    {
        QSizeF sizCurr = getSize();

        if( sizCurr.width() != m_sizFixed.width() || sizCurr.height() != m_sizFixed.height() )
        {
            if( sizCurr.width() != m_sizFixed.width() )
            {
                sizCurr.setWidth( m_sizFixed.width() );
            }
            if( sizCurr.height() != m_sizFixed.height() )
            {
                sizCurr.setHeight( m_sizFixed.height() );
            }
            setSize(sizCurr);
        }
    }

} // setFixedSize

//------------------------------------------------------------------------------
bool CGraphObj::hasFixedSize() const
//------------------------------------------------------------------------------
{
    bool bHas;

    if( m_sizFixed.isValid() )
    {
        bHas = true;
    }
    else if( m_sizMinimum.isValid() && m_sizMaximum.isValid() )
    {
        if( (m_sizMinimum.width() == m_sizMaximum.width()) && (m_sizMinimum.height() == m_sizMaximum.height()) )
        {
            bHas = true;
        }
    }
    return bHas;

} // hasFixedSize

//------------------------------------------------------------------------------
QSize CGraphObj::getFixedSize() const
//------------------------------------------------------------------------------
{
    QSize siz;

    if( m_sizFixed.isValid() )
    {
        siz = m_sizFixed;
    }
    else if( m_sizMinimum.isValid() && m_sizMaximum.isValid() )
    {
        if( (m_sizMinimum.width() == m_sizMaximum.width()) && (m_sizMinimum.height() == m_sizMaximum.height()) )
        {
            siz = m_sizMinimum;
        }
    }
    return siz;

} // getFixedSize

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
int CGraphObj::addAlignment( const SGraphObjAlignment& i_alignment )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_alignment.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addAlignment",
        /* strAddInfo   */ strMthInArgs );

    int idx = m_arAlignments.size();
    m_arAlignments.append(i_alignment);
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
    if( i_idx < 0 || i_idx >= m_arAlignments.size() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
    }
    return m_arAlignments[i_idx];

} // getAlignment

//------------------------------------------------------------------------------
void CGraphObj::setAlignment( int i_idx, const SGraphObjAlignment& i_alignment )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_idx) + ": " + i_alignment.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setAlignment",
        /* strAddInfo   */ strMthInArgs );

    if( i_idx < 0 || i_idx >= m_arAlignments.size() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
    }
    m_arAlignments[i_idx] = i_alignment;

} // setAlignment

//------------------------------------------------------------------------------
void CGraphObj::removeAlignment( int i_idx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_idx);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeAlignment",
        /* strAddInfo   */ strMthInArgs );

    if( i_idx < 0 || i_idx >= m_arAlignments.size() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Idx:" + QString::number(i_idx) );
    }
    m_arAlignments.removeAt(i_idx);

} // removeAlignment

//------------------------------------------------------------------------------
void CGraphObj::clearAlignments()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clearAlignments",
        /* strAddInfo   */ strMthInArgs );

    m_arAlignments.clear();
}

/*==============================================================================
public: // overridables
==============================================================================*/

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
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "acceptCurrentAsOriginalCoors",
        /* strAddInfo   */ strMthInArgs );

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

} // acceptCurrentAsOriginalCoors

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
QPointF CGraphObj::getPos( ECoordinatesVersion i_version ) const
//------------------------------------------------------------------------------
{
    QPointF ptPos;

    if( i_version == ECoordinatesVersion::Original )
    {
        ptPos = m_ptPosOrig;
    }
    else
    {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

        if( pGraphicsItem != nullptr )
        {
            ptPos = pGraphicsItem->pos();
        }
    }
    return ptPos;

} // getPos

//------------------------------------------------------------------------------
double CGraphObj::getWidth( ECoordinatesVersion i_version ) const
//------------------------------------------------------------------------------
{
    double fWidth = 0.0;

    if( i_version == ECoordinatesVersion::Original )
    {
        fWidth = m_sizOrig.width();
    }
    else
    {
        fWidth = m_rctCurr.width();
    }
    return fWidth;

} // getWidth

//------------------------------------------------------------------------------
double CGraphObj::getHeight( ECoordinatesVersion i_version ) const
//------------------------------------------------------------------------------
{
    double fHeight = 0.0;

    if( i_version == ECoordinatesVersion::Original )
    {
        fHeight = m_sizOrig.height();
    }
    else
    {
        fHeight = m_rctCurr.height();
    }
    return fHeight;

} // getHeight

//------------------------------------------------------------------------------
QSizeF CGraphObj::getSize( ECoordinatesVersion i_version ) const
//------------------------------------------------------------------------------
{
    QSizeF siz;

    if( i_version == ECoordinatesVersion::Original )
    {
        siz = m_sizOrig;
    }
    else
    {
        siz = m_rctCurr.size();
    }
    return siz;

} // getSize

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::setRotationAngleInDegree( double i_fRotAngle_deg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_fRotAngle_deg, 'f', 1);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRotationAngleInDegree",
        /* strAddInfo   */ strMthInArgs );

    m_fRotAngleCurr_deg = i_fRotAngle_deg;

    updateTransform();

} // setRotationAngleInDegree

//------------------------------------------------------------------------------
double CGraphObj::getRotationAngleInDegree( ECoordinatesVersion i_version )
//------------------------------------------------------------------------------
{
    double fRotAngle_deg = 0.0;

    if( i_version == ECoordinatesVersion::Original )
    {
        fRotAngle_deg = m_fRotAngleOrig_deg;
    }
    else
    {
        fRotAngle_deg = m_fRotAngleCurr_deg;
    }
    return fRotAngle_deg;

} // getRotationAngleInDegree

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
//
////------------------------------------------------------------------------------
//void CGraphObj::setScaleFacY( double i_fScaleFacY )
////------------------------------------------------------------------------------
//{
//    m_fScaleFacYCurr = i_fScaleFacY;
//
//    updateTransform();
//
//} // setScaleFacY
//
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
//
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
//
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

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumEditMode(i_editMode).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setEditMode",
        /* strAddInfo   */ strMthInArgs );

    m_editMode = i_editMode;
}

//------------------------------------------------------------------------------
void CGraphObj::setEditResizeMode( EEditResizeMode i_editResizeMode )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumEditResizeMode(i_editResizeMode).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setEditResizeMode",
        /* strAddInfo   */ strMthInArgs );

    m_editResizeMode = i_editResizeMode;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObj::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( pGraphicsItem->isSelected() )
        {
            bIsHit = isPolygonSelectionPointHit(i_pt,o_pHitInfo);

            if( !bIsHit )
            {
                bIsHit = isBoundingRectSelectionPointHit(
                    /* pt               */ i_pt,
                    /* iSelPtsCount     */ -1,
                    /* pSelPts          */ nullptr,
                    /* pGraphObjHitInfo */ o_pHitInfo );

            } // if( !bIsHit )

        } // if( isSelected() )

        if( !bIsHit )
        {
            if( pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern )
            {
                bIsHit = pGraphicsItem->contains(i_pt);

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::Undefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::Undefined;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                }
            }
        }

        if( bIsHit && o_pHitInfo != nullptr )
        {
            o_pHitInfo->setCursor( Math::deg2Rad(m_fRotAngleCurr_deg) );
        }

    } // if( pGraphicsItem != nullptr )

    return bIsHit;

} // isHit

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
double CGraphObj::bringToFront()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "bringToFront",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    double fZValue = m_fZValue;

    if( pGraphicsItem != nullptr )
    {
        QRectF rctBounding = pGraphicsItem->boundingRect();

        rctBounding = pGraphicsItem->mapRectToScene(rctBounding);

        QList<QGraphicsItem*> arpGraphicsItemsIntersected = m_pDrawingScene->items(rctBounding);

        m_pDrawingScene->bringToFront( pGraphicsItem, arpGraphicsItemsIntersected );

        fZValue = pGraphicsItem->zValue();

    } // if( pGraphicsItem != nullptr )

    return fZValue;

} // bringToFront

//------------------------------------------------------------------------------
void CGraphObj::setStackingOrderValue( double i_fZValue )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString::number(i_fZValue, 'f', 1);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setStackingOrderValue",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        m_fZValue = i_fZValue;

        pGraphicsItem->setZValue(m_fZValue);
    }

} // setStackingOrderValue

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

    if( pGraphicsItem != nullptr )
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
        int                    iSelPtsCount = i_iSelPtsCount;

        if( pSelPts == nullptr )
        {
            iSelPtsCount = _ZSArrLen(s_arSelPts);
            pSelPts = s_arSelPts;
        }

        CGraphObjSelectionPoint* pGraphObjSelPt;
        int                      idxSelPt;
        ESelectionPoint          selPt;
        QPointF                  ptTmp;

        for( idxSelPt = 0; idxSelPt < iSelPtsCount; idxSelPt++ )
        {
            selPt = pSelPts[idxSelPt];

            pGraphObjSelPt = m_arpSelPtsBoundingRect[static_cast<int>(selPt)];

            if( pGraphObjSelPt != nullptr )
            {
                ptTmp = pGraphicsItem->mapToItem(pGraphObjSelPt,i_pt);

                if( pGraphObjSelPt->contains(ptTmp) )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
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

    } // if( pGraphicsItem != nullptr )

    return bIsHit;

} // isBoundingRectSelectionPointHit

//------------------------------------------------------------------------------
bool CGraphObj::isPolygonSelectionPointHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        CGraphObjSelectionPoint* pGraphObjSelPt;
        int                      idxSelPt;
        QPointF                  ptTmp;

        for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
        {
            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                ptTmp = pGraphicsItem->mapToItem(pGraphObjSelPt,i_pt);

                if( pGraphObjSelPt->contains(ptTmp) )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        ptTmp = pGraphObjSelPt->pos();
                        ptTmp = pGraphObjSelPt->mapToItem(pGraphicsItem,ptTmp);

                        o_pHitInfo->m_editMode = EEditMode::MoveShapePoint;
                        o_pHitInfo->m_editResizeMode = EEditResizeMode::Undefined;
                        o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::Undefined;
                        o_pHitInfo->m_idxPolygonShapePoint = idxSelPt;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = ptTmp;
                    }
                    break;
                }
            }
        }
    } // if( pGraphicsItem != nullptr )

    return bIsHit;

} // isPolygonSelectionPointHit

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
QPointF CGraphObj::getSelectionPoint( ESelectionPoint i_selPt ) const
//------------------------------------------------------------------------------
{
    QRectF rct( QPointF(0.0,0.0), getSize() );

    QPointF ptSel = ZS::Draw::getSelectionPoint(rct,i_selPt);

    return ptSel;

} // getSelectionPoint

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::hideSelectionPoints( ESelectionPoints i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hideSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        CGraphObjSelectionPoint* pGraphObjSelPt;
        ESelectionPoint          selPt;
        int                      idxSelPt;
        bool                     bHideSelPt;

        for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            bHideSelPt = false;

            if( selPt >= ESelectionPoint::CornerMin && selPt <= ESelectionPoint::CornerMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectCorner )
                {
                    bHideSelPt = true;
                }
            }
            else if( selPt >= ESelectionPoint::LineCenterMin && selPt <= ESelectionPoint::LineCenterMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectLineCenter )
                {
                    bHideSelPt = true;
                }
            }
            else if( selPt == ESelectionPoint::Center )
            {
                if( i_selPts & ESelectionPointsBoundingRectCenter )
                {
                    bHideSelPt = true;
                }
            }
            else if( selPt >= ESelectionPoint::RotateMin && selPt <= ESelectionPoint::RotateMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectRotate )
                {
                    bHideSelPt = true;
                }
            }

            if( bHideSelPt )
            {
                // Deleting child objects leads to itemChange and an updateToolTip call
                // accessing the array of selection points.
                pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    m_arpSelPtsBoundingRect[idxSelPt] = nullptr;

                    //m_pDrawingScene->removeGraphObj(pGraphObjSelPt); // the dtor of the selection point removes itself from the drawing scene

                    delete pGraphObjSelPt;
                    pGraphObjSelPt = nullptr;
                }
            }
        }

        if( i_selPts & ESelectionPointsPolygonShapePoints )
        {
            if( m_arpSelPtsPolygon.size() > 0 )
            {
                for( idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt-- )
                {
                    // Deleting child objects leads to itemChange and an updateToolTip call
                    // accessing the array of selection points.
                    pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                    m_arpSelPtsPolygon[idxSelPt] = nullptr;

                    //m_pDrawingScene->removeGraphObj(pGraphObjSelPt); // the dtor of the selection point (dtor of CGraphObj) removes itself from the drawing scene

                    delete pGraphObjSelPt;
                    pGraphObjSelPt = nullptr;
                }
                m_arpSelPtsPolygon.clear();
            }
        }
    } // if( pGraphicsItem != nullptr )

} // hideSelectionPoints

//------------------------------------------------------------------------------
void CGraphObj::bringSelectionPointsToFront( ESelectionPoints i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "bringSelectionPointsToFront",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QList<QGraphicsItem*>    arpGraphicsItemsIntersected;
        CGraphObjSelectionPoint* pGraphObjSelPt;
        QRectF                   rct;
        ESelectionPoint          selPt;
        int                      idxSelPt;
        bool                     bBringToFront;

        for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            bBringToFront = false;

            if( selPt >= ESelectionPoint::CornerMin && selPt <= ESelectionPoint::CornerMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectCorner )
                {
                    bBringToFront = true;
                }
            }
            else if( selPt >= ESelectionPoint::LineCenterMin && selPt <= ESelectionPoint::LineCenterMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectLineCenter )
                {
                    bBringToFront = true;
                }
            }
            else if( selPt == ESelectionPoint::Center )
            {
                if( i_selPts & ESelectionPointsBoundingRectCenter )
                {
                    bBringToFront = true;
                }
            }
            else if( selPt >= ESelectionPoint::RotateMin && selPt <= ESelectionPoint::RotateMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectRotate )
                {
                    bBringToFront = true;
                }
            }

            if( bBringToFront )
            {
                pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    rct = pGraphObjSelPt->rect();
                    rct = pGraphObjSelPt->mapRectToScene(rct);
                    arpGraphicsItemsIntersected = m_pDrawingScene->items(rct);
                    m_pDrawingScene->bringToFront( pGraphObjSelPt, arpGraphicsItemsIntersected );
                }
            }
        }

        if( i_selPts & ESelectionPointsPolygonShapePoints )
        {
            for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
            {
                pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    rct = pGraphObjSelPt->rect();
                    rct = pGraphObjSelPt->mapRectToScene(rct);
                    arpGraphicsItemsIntersected = m_pDrawingScene->items(rct);
                    m_pDrawingScene->bringToFront( pGraphObjSelPt, arpGraphicsItemsIntersected );
                }
            }
        }
    } // if( pGraphicsItem != nullptr )

} // bringSelectionPointsToFront

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::showSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = rect2Str(i_rct);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showSelectionPointsOfBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
    {
        CGraphObjSelectionPoint* pGraphObjSelPt;
        QPointF                  ptSel;
        ESelectionPoint          selPt;
        int                      idxSelPt;
        bool                     bShowSelPt;

        for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            bShowSelPt = false;

            if( selPt >= ESelectionPoint::CornerMin && selPt <= ESelectionPoint::CornerMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectCorner )
                {
                    bShowSelPt = true;
                }
            }
            else if( selPt >= ESelectionPoint::LineCenterMin && selPt <= ESelectionPoint::LineCenterMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectLineCenter )
                {
                    bShowSelPt = true;
                }
            }
            else if( selPt == ESelectionPoint::Center )
            {
                if( i_selPts & ESelectionPointsBoundingRectCenter )
                {
                    bShowSelPt = true;
                }
            }
            else if( selPt >= ESelectionPoint::RotateMin && selPt <= ESelectionPoint::RotateMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectRotate )
                {
                    bShowSelPt = true;
                }
            }

            if( bShowSelPt )
            {
                pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

                if( pGraphObjSelPt == nullptr )
                {
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

                if( pGraphObjSelPt != nullptr )
                {
                    if( selPt == ESelectionPoint::RotateTop /*&& m_fScaleFacYCurr != 0.0*/ )
                    {
                        ptSel = ZS::Draw::getSelectionPoint(i_rct,ESelectionPoint::TopCenter);
                        ptSel.setY( ptSel.y() - getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }
                    else if( selPt == ESelectionPoint::RotateBottom /*&& m_fScaleFacYCurr != 0.0*/ )
                    {
                        ptSel = ZS::Draw::getSelectionPoint(i_rct,ESelectionPoint::BottomCenter);
                        ptSel.setY( ptSel.y() + getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }
                    else
                    {
                        ptSel = ZS::Draw::getSelectionPoint(i_rct,selPt);
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }

                    pGraphObjSelPt->setPos(ptSel);
                    pGraphObjSelPt->setZValue( pGraphicsItem->zValue()+0.05 );
                }
            } // if( bShowSelPt )
        } // for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
    } // if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )

} // showSelectionPointsOfBoundingRect

//------------------------------------------------------------------------------
void CGraphObj::updateSelectionPointsOfBoundingRect( const QRectF& i_rct, unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = rect2Str(i_rct);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateSelectionPointsOfBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
    {
        CGraphObjSelectionPoint* pGraphObjSelPt;
        QPointF                  ptSel;
        ESelectionPoint          selPt;
        int                      idxSelPt;
        bool                     bUpdateSelPt;

        for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            bUpdateSelPt = false;

            if( selPt >= ESelectionPoint::CornerMin && selPt <= ESelectionPoint::CornerMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectCorner )
                {
                    bUpdateSelPt = true;
                }
            }
            else if( selPt >= ESelectionPoint::LineCenterMin && selPt <= ESelectionPoint::LineCenterMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectLineCenter )
                {
                    bUpdateSelPt = true;
                }
            }
            else if( selPt == ESelectionPoint::Center )
            {
                if( i_selPts & ESelectionPointsBoundingRectCenter )
                {
                    bUpdateSelPt = true;
                }
            }
            else if( selPt >= ESelectionPoint::RotateMin && selPt <= ESelectionPoint::RotateMax )
            {
                if( i_selPts & ESelectionPointsBoundingRectRotate )
                {
                    bUpdateSelPt = true;
                }
            }

            if( bUpdateSelPt )
            {
                pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    if( selPt == ESelectionPoint::RotateTop /*&& m_fScaleFacYCurr != 0.0*/ )
                    {
                        ptSel = ZS::Draw::getSelectionPoint(i_rct,ESelectionPoint::TopCenter);
                        ptSel.setY( ptSel.y() - getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }
                    else if( selPt == ESelectionPoint::RotateBottom /*&& m_fScaleFacYCurr != 0.0*/ )
                    {
                        ptSel = ZS::Draw::getSelectionPoint(i_rct,ESelectionPoint::BottomCenter);
                        ptSel.setY( ptSel.y() + getSelectionPointRotateDistance()/*/m_fScaleFacYCurr*/ );
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }
                    else
                    {
                        ptSel = ZS::Draw::getSelectionPoint(i_rct,selPt);
                        ptSel = pGraphicsItem->mapToScene(ptSel);
                    }

                    pGraphObjSelPt->setPos(ptSel);
                    pGraphObjSelPt->setZValue( pGraphicsItem->zValue()+0.05 );
                }
            } // if( bUpdateSelPt )
        } // for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
    } // if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )

} // updateSelectionPointsOfBoundingRect

//------------------------------------------------------------------------------
void CGraphObj::showSelectionPointsOfPolygon( const QPolygonF& i_plg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = polygon2Str(i_plg);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showSelectionPointsOfPolygon",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
    {
        CGraphObjSelectionPoint* pGraphObjSelPt;
        QPointF                  ptSel;
        int                      idxSelPt;

        if( m_arpSelPtsPolygon.size() != i_plg.size() )
        {
            if( m_arpSelPtsPolygon.size() > 0 )
            {
                for( idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt-- )
                {
                    // Deleting child objects leads to itemChange and an updateToolTip call
                    // accessing the array of selection points.
                    pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                    m_arpSelPtsPolygon[idxSelPt] = nullptr;

                    //m_pDrawingScene->removeGraphObj(pGraphObjSelPt); // the dtor of the selection point (dtor of CGraphObj) removes itself from the drawing scene

                    delete pGraphObjSelPt;
                    pGraphObjSelPt = nullptr;
                }
                m_arpSelPtsPolygon.clear();
            }
            for( idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++ )
            {
                m_arpSelPtsPolygon.append(nullptr);
            }
        }

        for( idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++ )
        {
            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

            if( pGraphObjSelPt == nullptr )
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

            if( pGraphObjSelPt != nullptr )
            {
                ptSel = i_plg[idxSelPt];
                ptSel = pGraphicsItem->mapToScene(ptSel);
                pGraphObjSelPt->setPos(ptSel);
                pGraphObjSelPt->setZValue( pGraphicsItem->zValue()+0.05 );
            }
        }
    } // if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )

} // showSelectionPointsOfPolygon

//------------------------------------------------------------------------------
void CGraphObj::updateSelectionPointsOfPolygon( const QPolygonF& i_plg )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = polygon2Str(i_plg);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateSelectionPointsOfPolygon",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )
    {
        if( i_plg.size() == m_arpSelPtsPolygon.size() )
        {
            CGraphObjSelectionPoint* pGraphObjSelPt;
            QPointF                  ptSel;
            int                      idxSelPt;

            for( idxSelPt = 0; idxSelPt < i_plg.size(); idxSelPt++ )
            {
                pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    ptSel = i_plg[idxSelPt];
                    ptSel = pGraphicsItem->mapToScene(ptSel);
                    pGraphObjSelPt->setPos(ptSel);
                    pGraphObjSelPt->setZValue( pGraphicsItem->zValue()+0.05 );
                }
            }
        }
    } // if( pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr )

} // updateSelectionPointsOfPolygon

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::showObjName( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumSelectionPoint(i_selPt).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showObjName",
        /* strAddInfo   */ strMthInArgs );

    if( !m_arpLabels.contains(c_strKeyLabelObjName) )
    {
        addLabel(c_strKeyLabelObjName,m_strName,i_selPt);
    }
    showLabel(c_strKeyLabelObjName);

} // showObjName

//------------------------------------------------------------------------------
void CGraphObj::hideObjName()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hideObjName",
        /* strAddInfo   */ strMthInArgs );

    hideLabel(c_strKeyLabelObjName);
}

//------------------------------------------------------------------------------
bool CGraphObj::isObjNameVisible() const
//------------------------------------------------------------------------------
{
    return isLabelVisible(c_strKeyLabelObjName);
}

//------------------------------------------------------------------------------
void CGraphObj::showObjId( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = CEnumSelectionPoint(i_selPt).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showObjId",
        /* strAddInfo   */ strMthInArgs );

    if( !m_arpLabels.contains(c_strKeyLabelObjId) )
    {
        addLabel(c_strKeyLabelObjId, m_strKeyInTree, i_selPt);
    }
    showLabel(c_strKeyLabelObjId);

} // showObjId

//------------------------------------------------------------------------------
void CGraphObj::hideObjId()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hideObjId",
        /* strAddInfo   */ strMthInArgs );

    hideLabel(c_strKeyLabelObjId);
}

//------------------------------------------------------------------------------
bool CGraphObj::isObjIdVisible() const
//------------------------------------------------------------------------------
{
    return isLabelVisible(c_strKeyLabelObjId);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::addLabel(
    const QString&  i_strKey,
    const QString&  i_strText,
    ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Key: " + i_strKey;
        strMthInArgs += ", Text: " + i_strText;
        strMthInArgs += ", SelPt: " + CEnumSelectionPoint(i_selPt).toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addLabel",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjLabel* pGraphObjLabel = nullptr;

    if( m_arpLabels.contains(i_strKey) )
    {
        pGraphObjLabel = m_arpLabels[i_strKey];
        pGraphObjLabel->m_strText = i_strText;
    }
    else
    {
        pGraphObjLabel = new SGraphObjLabel(i_strKey,i_strText,i_selPt);
        m_arpLabels.insert(i_strKey,pGraphObjLabel);
    }

    //pGraphObjLabel->m_sizDist = QSizeF();
    //pGraphObjLabel->m_bDistValid = false;
    //pGraphObjLabel->m_bVisible = false;
    //pGraphObjLabel->m_pGraphObjLabel = nullptr;

} // addLabel

//------------------------------------------------------------------------------
void CGraphObj::removeLabel( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strKey;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeLabel",
        /* strAddInfo   */ strMthInArgs );

    if( m_arpLabels.contains(i_strKey) )
    {
        hideLabel(i_strKey);

        m_arpLabels.remove(i_strKey);
    }

} // removeLabel

//------------------------------------------------------------------------------
void CGraphObj::showLabel( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strKey;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showLabel",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( m_arpLabels.contains(i_strKey) )
        {
            SGraphObjLabel* pGraphObjLabel = m_arpLabels[i_strKey];

            QPointF ptSelPt = getSelectionPoint(pGraphObjLabel->m_selPt.enumerator());
            QPointF ptLabel;

            ptSelPt = pGraphicsItem->mapToScene(ptSelPt);

            if( pGraphObjLabel->m_pGraphObjLabel == nullptr )
            {
                pGraphObjLabel->m_pGraphObjLabel = new CGraphObjLabel(
                    /* pDrawingScene     */ m_pDrawingScene,
                    /* pGraphObjSelected */ this,
                    /* strKey            */ pGraphObjLabel->m_strKey,
                    /* strText           */ pGraphObjLabel->m_strText,
                    /* selPt             */ pGraphObjLabel->m_selPt.enumerator() );
                m_pDrawingScene->addGraphObj(pGraphObjLabel->m_pGraphObjLabel, this);
                //pGraphObjLabel->m_pGraphObjLabel->setParentItem(this); see comment in header file of class CGraphObjLabel
            }

            if( !pGraphObjLabel->m_bDistValid )
            {
                QPointF ptLabelTmp = ptSelPt;

                if( pGraphObjLabel->m_selPt != ESelectionPoint::BottomRight
                 && pGraphObjLabel->m_selPt != ESelectionPoint::BottomLeft
                 && pGraphObjLabel->m_selPt != ESelectionPoint::BottomCenter )
                {
                    ptLabelTmp.setY( ptLabelTmp.y() - pGraphObjLabel->m_pGraphObjLabel->getHeight() );
                }

                bool bUniquePos = false;

                while( !bUniquePos )
                {
                    bUniquePos = true;

                    QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
                    SGraphObjLabel*                        pGraphObjLabelTmp;

                    while( itLabels.hasNext() )
                    {
                        itLabels.next();
                        pGraphObjLabelTmp = itLabels.value();

                        if( pGraphObjLabelTmp->m_strKey != i_strKey )
                        {
                            if( pGraphObjLabelTmp->m_pGraphObjLabel != nullptr && pGraphObjLabelTmp->m_pGraphObjLabel->scenePos() == ptLabelTmp )
                            {
                                bUniquePos = false;
                                ptLabelTmp.setX( pGraphObjLabelTmp->m_pGraphObjLabel->scenePos().x() + pGraphObjLabelTmp->m_pGraphObjLabel->getWidth() + 4 );
                                break;
                            }
                        }
                    }
                }

                pGraphObjLabel->m_sizDist.setWidth( ptLabelTmp.x() - ptSelPt.x() );
                pGraphObjLabel->m_sizDist.setHeight( ptLabelTmp.y() - ptSelPt.y() );

                pGraphObjLabel->m_bDistValid = true;

            } // if( !pGraphObjLabel->m_bDistValid )

            ptLabel.setX( ptSelPt.x() + pGraphObjLabel->m_sizDist.width() );
            ptLabel.setY( ptSelPt.y() + pGraphObjLabel->m_sizDist.height() );

            pGraphObjLabel->m_bVisible = true;

            pGraphObjLabel->m_pGraphObjLabel->setPos(ptLabel);
            pGraphObjLabel->m_pGraphObjLabel->setZValue(pGraphicsItem->zValue()+0.02);

        } // if( m_arpLabels.contains(i_strKey) )

    } // if( pGraphicsItem != nullptr )

} // showLabel

//------------------------------------------------------------------------------
void CGraphObj::hideLabel( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strKey;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hideLabel",
        /* strAddInfo   */ strMthInArgs );

    if( m_arpLabels.contains(i_strKey) )
    {
        SGraphObjLabel* pGraphObjLabel = m_arpLabels[i_strKey];

        //m_pDrawingScene->removeGraphObj(pGraphObjLabel); // the dtor of the label removes itself from the drawing scene

        delete pGraphObjLabel->m_pGraphObjLabel;
        pGraphObjLabel->m_pGraphObjLabel = nullptr;

        pGraphObjLabel->m_bVisible = false;

    } // if( m_arpLabels.contains(i_strKey) )

} // hideLabel

//------------------------------------------------------------------------------
void CGraphObj::setLabelText( const QString& i_strKey, const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strKey + ", " + i_strText;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLabelText",
        /* strAddInfo   */ strMthInArgs );

    if( m_arpLabels.contains(i_strKey) )
    {
        SGraphObjLabel* pGraphObjLabel = m_arpLabels[i_strKey];

        pGraphObjLabel->m_strText = i_strText;

        if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
        {
            pGraphObjLabel->m_pGraphObjLabel->setText(pGraphObjLabel->m_strText);
        }

    } // if( m_arpLabels.contains(i_strKey) )

} // setLabelText

//------------------------------------------------------------------------------
bool CGraphObj::isLabelVisible( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    bool bIsVisible = false;

    if( m_arpLabels.contains(i_strKey) )
    {
        SGraphObjLabel* pGraphObjLabel = m_arpLabels[i_strKey];

        bIsVisible = pGraphObjLabel->m_bVisible;

    } // if( m_arpLabels.contains(i_strKey) )

    return bIsVisible;

} // isLabelVisible

//------------------------------------------------------------------------------
void CGraphObj::updateLabelDistance( const QString& i_strKey )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strKey;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateLabelDistance",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( m_arpLabels.contains(i_strKey) )
        {
            SGraphObjLabel* pGraphObjLabel = m_arpLabels[i_strKey];

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                QPointF ptSelPt = getSelectionPoint(pGraphObjLabel->m_selPt.enumerator());
                QPointF ptLabel = pGraphObjLabel->m_pGraphObjLabel->scenePos();

                ptSelPt = pGraphicsItem->mapToScene(ptSelPt);

                pGraphObjLabel->m_sizDist.setWidth( ptLabel.x() - ptSelPt.x() );
                pGraphObjLabel->m_sizDist.setHeight( ptLabel.y() - ptSelPt.y() );

                pGraphObjLabel->m_bDistValid = true;

            } // if( pGraphObjLabel->m_pGraphObjLabel != nullptr )

        } // if( m_arpLabels.contains(i_strKey) )

    } // if( pGraphicsItem != nullptr )

} // updateLabelDistance

//------------------------------------------------------------------------------
void CGraphObj::updateLabelPositions()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateLabelPositions",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);

        SGraphObjLabel* pGraphObjLabel;
        QPointF         ptSelPt;
        QPointF         ptLabel;

        while( itLabels.hasNext() )
        {
            itLabels.next();

            pGraphObjLabel = itLabels.value();

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                ptSelPt = getSelectionPoint(pGraphObjLabel->m_selPt.enumerator());
                ptSelPt = pGraphicsItem->mapToScene(ptSelPt);

                ptLabel.setX( ptSelPt.x() + pGraphObjLabel->m_sizDist.width() );
                ptLabel.setY( ptSelPt.y() + pGraphObjLabel->m_sizDist.height() );

                pGraphObjLabel->m_pGraphObjLabel->setPos(ptLabel);
            }
        }

    } // if( pGraphicsItem != nullptr )

} // updateLabelPositions

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::showLabels()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showLabels",
        /* strAddInfo   */ strMthInArgs );

    QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
    SGraphObjLabel*                        pGraphObjLabel;

    while( itLabels.hasNext() )
    {
        itLabels.next();

        pGraphObjLabel = itLabels.value();

        if( pGraphObjLabel->m_pGraphObjLabel == nullptr )
        {
            showLabel(pGraphObjLabel->m_strKey);
        }
    }

} // showLabels

//------------------------------------------------------------------------------
void CGraphObj::hideLabels()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hideLabels",
        /* strAddInfo   */ strMthInArgs );

    QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
    SGraphObjLabel*                        pGraphObjLabel;

    while( itLabels.hasNext() )
    {
        itLabels.next();

        pGraphObjLabel = itLabels.value();

        if( pGraphObjLabel->m_pGraphObjLabel == nullptr )
        {
            hideLabel(pGraphObjLabel->m_strKey);
        }
    }

} // hideLabels

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QStringList CGraphObj::getLabelsKeys() const
//------------------------------------------------------------------------------
{
    return m_arpLabels.keys();
}

//------------------------------------------------------------------------------
SGraphObjLabel* CGraphObj::getLabel( const QString& i_strKey ) const
//------------------------------------------------------------------------------
{
    SGraphObjLabel* pGraphObjLabel = nullptr;

    if( m_arpLabels.contains(i_strKey) )
    {
        pGraphObjLabel = m_arpLabels[i_strKey];
    }
    return pGraphObjLabel;

} // getLabel

//------------------------------------------------------------------------------
void CGraphObj::addLabels( QHash<QString,SGraphObjLabel*> i_arpLabels )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addLabels",
        /* strAddInfo   */ strMthInArgs );

    QHashIterator<QString,SGraphObjLabel*> itLabels(i_arpLabels);

    SGraphObjLabel* pGraphObjLabel;
    QString         strKey;
    QString         strText;
    ESelectionPoint selPt;
    QSizeF          sizDist;
    bool            bDistValid;
    bool            bVisible;

    while( itLabels.hasNext() )
    {
        itLabels.next();

        pGraphObjLabel = itLabels.value();

        strKey     = pGraphObjLabel->m_strKey;
        strText    = pGraphObjLabel->m_strText;
        selPt      = pGraphObjLabel->m_selPt.enumerator();
        sizDist    = pGraphObjLabel->m_sizDist;
        bDistValid = pGraphObjLabel->m_bDistValid;
        bVisible   = pGraphObjLabel->m_bVisible;

        if( !m_arpLabels.contains(strKey) )
        {
            addLabel(strKey,strText,selPt);
        }

        pGraphObjLabel = m_arpLabels[strKey];

        pGraphObjLabel->m_sizDist    = sizDist;
        pGraphObjLabel->m_bDistValid = bDistValid;
        pGraphObjLabel->m_bVisible   = bVisible;

        if( bVisible )
        {
            showLabel(strKey);
        }
    }

} // addLabels

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! If currently objects are selected their selection points are created.
    When deleting the graphics scene all items including selection points will
    be destroyed. Selection points may be destroyed before its parent object
    the selection points belong to. The parent object got to be informed if the
    selection point will be destroyed by someone else.
*/
void CGraphObj::onSelectionPointDestroying( CGraphObjSelectionPoint* i_pSelectionPoint )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pSelectionPoint == nullptr ? "nullptr" : i_pSelectionPoint->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onSelectionPointDestroying",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjSelectionPoint* pGraphObjSelPt;
    int                      idxSelPt;

    if( m_arpSelPtsBoundingRect.size() > 0 )
    {
        for( idxSelPt = m_arpSelPtsBoundingRect.size()-1; idxSelPt >= 0; idxSelPt-- )
        {
            pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

            if( pGraphObjSelPt == i_pSelectionPoint )
            {
                m_arpSelPtsBoundingRect[idxSelPt] = nullptr;
            }
        }
    }

    if( m_arpSelPtsPolygon.size() > 0 )
    {
        for( idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= 0; idxSelPt-- )
        {
            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

            if( pGraphObjSelPt == i_pSelectionPoint )
            {
                m_arpSelPtsPolygon[idxSelPt] = nullptr;
            }
        }
    }

} // onSelectionPointDestroying

/*==============================================================================
public: // instance methods (simulation methods)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::addMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addMousePressEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjMouseEventFct fctEntry;
    int                    idxFct;

    for( idxFct = 0; idxFct < m_arMousePressEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arMousePressEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }

    m_arMousePressEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );

} // addMousePressEventFunction

//------------------------------------------------------------------------------
void CGraphObj::removeMousePressEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeMousePressEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjMouseEventFct fctEntry;
    int                    idxFct;

    for( idxFct = 0; idxFct < m_arMousePressEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arMousePressEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            break;
        }
    }
    if( idxFct >= m_arMousePressEventFunctions.size() )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }

    m_arMousePressEventFunctions.removeAt(idxFct);

} // removeMousePressEventFunction

//------------------------------------------------------------------------------
void CGraphObj::addMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addMouseReleaseEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjMouseEventFct fctEntry;
    int                    idxFct;

    for( idxFct = 0; idxFct < m_arMouseReleaseEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arMouseReleaseEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }

    m_arMouseReleaseEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );

} // addMouseReleaseEventFunction

//------------------------------------------------------------------------------
void CGraphObj::removeMouseReleaseEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeMouseReleaseEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjMouseEventFct fctEntry;
    int                    idxFct;

    for( idxFct = 0; idxFct < m_arMouseReleaseEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arMouseReleaseEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            break;
        }
    }
    if( idxFct >= m_arMouseReleaseEventFunctions.size() )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }

    m_arMouseReleaseEventFunctions.removeAt(idxFct);

} // removeMouseReleaseEventFunction

//------------------------------------------------------------------------------
void CGraphObj::addMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addMouseDoubleClickEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjMouseEventFct fctEntry;
    int                    idxFct;

    for( idxFct = 0; idxFct < m_arMouseDoubleClickEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arMouseDoubleClickEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }

    m_arMouseDoubleClickEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );

} // addMouseDoubleClickEventFunction

//------------------------------------------------------------------------------
void CGraphObj::removeMouseDoubleClickEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeMouseDoubleClickEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjMouseEventFct fctEntry;
    int                    idxFct;

    for( idxFct = 0; idxFct < m_arMouseDoubleClickEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arMouseDoubleClickEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            break;
        }
    }
    if( idxFct >= m_arMouseDoubleClickEventFunctions.size() )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }

    m_arMouseDoubleClickEventFunctions.removeAt(idxFct);

} // removeMouseDoubleClickEventFunction

//------------------------------------------------------------------------------
void CGraphObj::addMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addMouseMoveEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjMouseEventFct fctEntry;
    int                    idxFct;

    for( idxFct = 0; idxFct < m_arMouseMoveEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arMouseMoveEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }

    m_arMouseMoveEventFunctions.append( SGraphObjMouseEventFct(i_pFct,i_pvThis,i_pvData) );

} // addMouseMoveEventFunction

//------------------------------------------------------------------------------
void CGraphObj::removeMouseMoveEventFunction( TFctMouseEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeMouseMoveEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjMouseEventFct fctEntry;
    int                    idxFct;

    for( idxFct = 0; idxFct < m_arMouseMoveEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arMouseMoveEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            break;
        }
    }
    if( idxFct >= m_arMouseMoveEventFunctions.size() )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }

    m_arMouseMoveEventFunctions.removeAt(idxFct);

} // removeMouseMoveEventFunction

//------------------------------------------------------------------------------
void CGraphObj::addKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addKeyPressEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjKeyEventFct fctEntry;
    int                  idxFct;

    for( idxFct = 0; idxFct < m_arKeyPressEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arKeyPressEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }

    m_arKeyPressEventFunctions.append( SGraphObjKeyEventFct(i_pFct,i_pvThis,i_pvData) );

} // addKeyPressEventFunction

//------------------------------------------------------------------------------
void CGraphObj::removeKeyPressEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeKeyPressEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjKeyEventFct fctEntry;
    int                  idxFct;

    for( idxFct = 0; idxFct < m_arKeyPressEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arKeyPressEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            break;
        }
    }
    if( idxFct >= m_arKeyPressEventFunctions.size() )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }

    m_arKeyPressEventFunctions.removeAt(idxFct);

} // removeKeyPressEventFunction

//------------------------------------------------------------------------------
void CGraphObj::addKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addKeyReleaseEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjKeyEventFct fctEntry;
    int                  idxFct;

    for( idxFct = 0; idxFct < m_arKeyReleaseEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arKeyReleaseEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function already added");
        }
    }

    m_arKeyReleaseEventFunctions.append( SGraphObjKeyEventFct(i_pFct,i_pvThis,i_pvData) );

} // addKeyReleaseEventFunction

//------------------------------------------------------------------------------
void CGraphObj::removeKeyReleaseEventFunction( TFctKeyEvent i_pFct, void* i_pvThis, void* i_pvData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeKeyReleaseEventFunction",
        /* strAddInfo   */ strMthInArgs );

    SGraphObjKeyEventFct fctEntry;
    int                  idxFct;

    for( idxFct = 0; idxFct < m_arKeyReleaseEventFunctions.size(); idxFct++ )
    {
        fctEntry = m_arKeyReleaseEventFunctions[idxFct];

        if( fctEntry.m_pFct == i_pFct && fctEntry.m_pvThis == i_pvThis && fctEntry.m_pvData == i_pvData )
        {
            break;
        }
    }
    if( idxFct >= m_arKeyReleaseEventFunctions.size() )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjAlreadyInList,"Event function not added");
    }

    m_arKeyReleaseEventFunctions.removeAt(idxFct);

} // removeKeyReleaseEventFunction

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObj::updateTransform()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateTransform",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QTransform trs;

        trs.translate( m_ptRotOriginCurr.x(), m_ptRotOriginCurr.y() );
        trs.rotate(-m_fRotAngleCurr_deg);
        trs.translate( -m_ptRotOriginCurr.x(), -m_ptRotOriginCurr.y() );
        //trsNew.scale( m_fScaleFacXCurr, m_fScaleFacYCurr );

        pGraphicsItem->resetTransform();

        pGraphicsItem->setTransform(trs);

    } // if( pGraphicsItem != nullptr )

} // updateTransform

//------------------------------------------------------------------------------
void CGraphObj::updateToolTip()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateToolTip",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
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

        m_strToolTip += "\nSize:\t\t" + size2Str(getSize());
        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + ZS::PhysVal::c_strSymbolDegree;
        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());

        pGraphicsItem->setToolTip(m_strToolTip);

    } // if( pGraphicsItem != nullptr )

} // updateToolTip

//------------------------------------------------------------------------------
void CGraphObj::updateEditInfo()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateEditInfo",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QString strAngleSymbol = QString(QChar(8738));

        m_strEditInfo  = "C:" + point2Str( pGraphicsItem->mapToScene(m_rctCurr.center()) );
        m_strEditInfo += ", W:" + QString::number(m_rctCurr.width(),'f',1);
        m_strEditInfo += ", H:" + QString::number(m_rctCurr.height(),'f',1);
        m_strEditInfo += ", " + strAngleSymbol + ":" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + ZS::PhysVal::c_strSymbolDegree;

    } // if( pGraphicsItem != nullptr )

} // updateEditInfo
