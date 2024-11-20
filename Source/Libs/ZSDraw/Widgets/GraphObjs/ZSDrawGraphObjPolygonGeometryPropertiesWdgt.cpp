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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPolygonGeometryPropertiesWdgt.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPolygonGeometryModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysValGUI/ZSPhysValEditItemDelegate.h"
#include "ZSSysGUI/ZSSysCheckBoxItemDelegate.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSysGUI/ZSSysTableView.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjPolygonGeometryProperties : public CWdgtGraphObjPropertiesAbstract
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjPolygonGeometryProperties::CWdgtGraphObjPolygonGeometryProperties(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strNameSpace,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(
        i_pDrawingScene,
        i_strNameSpace, "StandardShapes::Group", ClassName(),
        i_strObjName, i_pWdgtParent),
    // Headline with collapse button.
    m_pWdgtHeadline(nullptr),
    m_pLytWdgtHeadline(nullptr),
    m_pxmBtnDown(":/ZS/Button/ButtonArrowDown.png"),
    m_pxmBtnUp(":/ZS/Button/ButtonArrowUp.png"),
    m_pBtnCollapse(nullptr),
    m_pLblHeadlineIcon(nullptr),
    m_pLblHeadline(nullptr),
    m_pSepHeadline(nullptr),
    // Table View Widget
    m_pWdgtGeometry(nullptr),
    m_pLytWdgtGeometry(nullptr),
    m_pLytLineTableViewButtons(nullptr),
    m_pBtnResizeRowsAndColumnsToContents(nullptr),
    m_pLytGeometryTableView(nullptr),
    m_pTableViewGeometry(nullptr),
    m_pEdtPhysValDelegateXVal(nullptr),
    m_pEdtPhysValDelegateYVal(nullptr),
    m_pModelGeometry(nullptr)
    // Edit dialog
    //m_hshpRegisteredEditPropertyDialogs(),
    //m_pDlgEditProperty(nullptr)
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // We need to fill the edit controls if the graphical object emits the geometryOnSceneChanged signal.
    // The flag is checked if "setKeyInTree" is called.
    m_bContentUpdateOnGeometryChanged = true;

    // <Widget> Headline
    //==================

    m_pWdgtHeadline = new QWidget();
    m_pLytWdgtHeadline = new QHBoxLayout();
    m_pLytWdgtHeadline->setContentsMargins(0, 0, 0, 0);
    m_pWdgtHeadline->setLayout(m_pLytWdgtHeadline);
    m_pLyt->addWidget(m_pWdgtHeadline);

    m_pBtnCollapse = new QPushButton();
    if (s_bWdgtGeometryVisible) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
    }
    else {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
    }
    m_pLytWdgtHeadline->addWidget(m_pBtnCollapse);

    QObject::connect(
        m_pBtnCollapse, &QPushButton::clicked,
        this, &CWdgtGraphObjPolygonGeometryProperties::onBtnCollapseClicked);

    QPixmap pxmHeadline(":/ZS/Draw/DrawSettingsGeometry16x16.png");
    m_pLblHeadlineIcon = new QLabel();
    m_pLblHeadlineIcon->setPixmap(pxmHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadlineIcon);
    m_pLblHeadline = new QLabel("Geometry");
    QFont fntHeadline = m_pLblHeadline->font();
    fntHeadline.setPointSize(fntHeadline.pointSize() + 2);
    m_pLblHeadline->setFont(fntHeadline);
    m_pLytWdgtHeadline->addWidget(m_pLblHeadline);
    m_pSepHeadline = new CSepLine();
    m_pLytWdgtHeadline->addWidget(m_pSepHeadline, 1);
    m_pLytWdgtHeadline->addStretch();

    const QString strMetric = CEnumScaleDimensionUnit(EScaleDimensionUnit::Metric).toString();
    const QString strPixels = CEnumScaleDimensionUnit(EScaleDimensionUnit::Pixels).toString();

    // <Widget> Geometry
    //==================

    m_pWdgtGeometry = new QWidget();
    m_pLytWdgtGeometry = new QVBoxLayout();
    m_pLytWdgtGeometry->setContentsMargins(0, 0, 0, 0);
    m_pWdgtGeometry->setLayout(m_pLytWdgtGeometry);
    m_pLyt->addWidget(m_pWdgtGeometry);

    // <Line> Table View Buttons
    //--------------------------

    m_pLytLineTableViewButtons = new QHBoxLayout();
    m_pLytWdgtGeometry->addLayout(m_pLytLineTableViewButtons);

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");
    m_pBtnResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnResizeRowsAndColumnsToContents->setFixedSize(QSize(24, 24));
    m_pBtnResizeRowsAndColumnsToContents->setToolTip("Press to resize the rows and columns to their contents");
    m_pLytLineTableViewButtons->addWidget(m_pBtnResizeRowsAndColumnsToContents);
    m_pLytLineTableViewButtons->addStretch();
    QObject::connect(
        m_pBtnResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtGraphObjPolygonGeometryProperties::onBtnResizeRowsAndColumnsToContentsClicked );

    // Table View
    //------------------

    m_pLytGeometryTableView = new QVBoxLayout();
    m_pLytWdgtGeometry->addLayout(m_pLytGeometryTableView, 1);

    m_pModelGeometry = new CModelGraphObjPolygonGeometry(
        m_pDrawingScene, i_strNameSpace, "StandardShapes::Polygon",
        i_strObjName, CEnumScaleDimensionUnit(), this);
    m_pTableViewGeometry = new CTableView(i_strObjName);
    m_pTableViewGeometry->setModel(m_pModelGeometry);

    m_pEdtPhysValDelegateXVal =
        new CEditPhysValtemDelegate(m_pModelGeometry, m_pTableViewGeometry);
    m_pTableViewGeometry->setItemDelegateForColumn(
        CModelGraphObjPolygonGeometry::EColumnXVal, m_pEdtPhysValDelegateXVal);
    m_pTableViewGeometry->addKeyAsEditTriggerForColumn(
        CModelGraphObjPolygonGeometry::EColumnXVal, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pEdtPhysValDelegateYVal =
        new CEditPhysValtemDelegate(m_pModelGeometry, m_pTableViewGeometry);
    m_pTableViewGeometry->setItemDelegateForColumn(
        CModelGraphObjPolygonGeometry::EColumnYVal, m_pEdtPhysValDelegateYVal);
    m_pTableViewGeometry->addKeyAsEditTriggerForColumn(
        CModelGraphObjPolygonGeometry::EColumnYVal, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableViewGeometry->setItemDelegateForColumn(
        CModelGraphObjPolygonGeometry::EColumnShowVals,
        new CCheckBoxItemDelegate(m_pModelGeometry, m_pTableViewGeometry));
    m_pTableViewGeometry->addKeyAsEditTriggerForColumn(
        CModelGraphObjPolygonGeometry::EColumnShowVals, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableViewGeometry->setItemDelegateForColumn(
        CModelGraphObjPolygonGeometry::EColumnShowLine,
        new CCheckBoxItemDelegate(m_pModelGeometry, m_pTableViewGeometry));
    m_pTableViewGeometry->addKeyAsEditTriggerForColumn(
        CModelGraphObjPolygonGeometry::EColumnShowLine, CTableView::SEditTriggerKey(Qt::Key_Return));

    m_pTableViewGeometry->setEditTriggers(
        QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked|QAbstractItemView::EditKeyPressed);
    m_pTableViewGeometry->resizeColumnsToContents();
    m_pTableViewGeometry->resizeRowsToContents();
    m_pLytGeometryTableView->addWidget(m_pTableViewGeometry);

    QObject::connect(
        m_pModelGeometry, &CModelGraphObjPolygonGeometry::contentChanged,
        this, &CWdgtGraphObjPolygonGeometryProperties::onModelGeometryContentChanged);

    // Update controls depending on drawing size (dimension unit etc.)
    //----------------------------------------------------------------

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    onDrawingSceneDrawingSizeChanged(drawingSize);

    // Restore visibility
    //-------------------

    if (!s_bWdgtGeometryVisible) {
        m_pWdgtGeometry->hide();
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjPolygonGeometryProperties::~CWdgtGraphObjPolygonGeometryProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    //if (m_pDlgEditProperty != nullptr) {
    //    m_pDlgEditProperty->close();
    //    try {
    //        delete m_pDlgEditProperty;
    //    }
    //    catch (...) {
    //    }
    //}

    // Headline with collapse button.
    m_pWdgtHeadline = nullptr;
    m_pLytWdgtHeadline = nullptr;
    //m_pxmBtnDown;
    //m_pxmBtnUp;
    m_pBtnCollapse = nullptr;
    m_pLblHeadlineIcon = nullptr;
    m_pLblHeadline = nullptr;
    m_pSepHeadline = nullptr;
    // Table View Widget.
    m_pWdgtGeometry = nullptr;
    m_pLytWdgtGeometry = nullptr;
    m_pLytLineTableViewButtons = nullptr;
    m_pBtnResizeRowsAndColumnsToContents = nullptr;
    m_pLytGeometryTableView = nullptr;
    m_pTableViewGeometry = nullptr;
    m_pEdtPhysValDelegateXVal = nullptr;
    m_pEdtPhysValDelegateYVal = nullptr;
    m_pModelGeometry = nullptr;
    // Edit dialog
    //m_hshpRegisteredEditPropertyDialogs;
    //m_pDlgEditProperty = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPolygonGeometryProperties::expand(bool i_bExpand)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bExpand);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expand",
        /* strAddInfo   */ strMthInArgs );

    if (i_bExpand && m_pWdgtGeometry->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnUp);
        m_pWdgtGeometry->show();
        s_bWdgtGeometryVisible = true;
    }
    else if (!i_bExpand && !m_pWdgtGeometry->isHidden()) {
        m_pBtnCollapse->setIcon(m_pxmBtnDown);
        m_pWdgtGeometry->hide();
        s_bWdgtGeometryVisible = false;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Expanded: " + bool2Str(s_bWdgtFillStyleVisible));
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjPolygonGeometryProperties::setKeyInTree(const QString& i_strKeyInTree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setKeyInTree",
        /* strAddInfo   */ strMthInArgs );
    bool bObjectChanged = false;
    if (m_strKeyInTree != i_strKeyInTree) {
        bObjectChanged = true;
        CWdgtGraphObjPropertiesAbstract::setKeyInTree(i_strKeyInTree);
        if (m_pModelGeometry != nullptr) {
            m_pModelGeometry->setKeyInTree(i_strKeyInTree);
            m_pTableViewGeometry->resizeColumnsToContents();
            m_pTableViewGeometry->resizeRowsToContents();
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bObjectChanged);
    }
    return bObjectChanged;
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjPolygonGeometryProperties::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasErrors",
        /* strAddInfo   */ "" );
    bool bHasErrors = false;
    if (!bHasErrors && m_pModelGeometry != nullptr) {
        bHasErrors = m_pModelGeometry->hasErrors();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
bool CWdgtGraphObjPolygonGeometryProperties::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );
    bool bHasChanges = false;
    if (!bHasChanges && m_pModelGeometry != nullptr) {
        bHasChanges = m_pModelGeometry->hasChanges();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPolygonGeometryProperties::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (!hasErrors())
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            if (m_pModelGeometry != nullptr) {
                m_pModelGeometry->acceptChanges();
            }
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            emit_contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPolygonGeometryProperties::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

        if (m_pModelGeometry != nullptr) {
            m_pModelGeometry->rejectChanges();
        }
    }

    // If the "contentChanged" signal is no longer blocked and the content of
    // properties widget has been changed ...
    if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
        // .. emit the contentChanged signal and update the enabled state
        // of the Apply and Reset buttons.
        emit_contentChanged();
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPolygonGeometryProperties::onBtnCollapseClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnCollapseClicked",
        /* strAddInfo   */ "" );

    if (m_pWdgtGeometry->isHidden()) {
        expand(true);
    }
    else {
        expand(false);
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPolygonGeometryProperties::onBtnResizeRowsAndColumnsToContentsClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onBtnResizeRowsAndColumnsToContentsClicked",
        /* strAddInfo   */ "" );

    m_pTableViewGeometry->resizeColumnsToContents();
    m_pTableViewGeometry->resizeRowsToContents();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPolygonGeometryProperties::onModelGeometryContentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onModelGeometryContentChanged",
        /* strAddInfo   */ "" );

    emit_contentChanged();
}

////------------------------------------------------------------------------------
//void CWdgtGraphObjPolygonGeometryProperties::onDlgEditPropertyDestroyed(QObject* i_pObj)
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onDlgEditPropertyDestroyed",
//        /* strAddInfo   */ "" );
//
//    m_pDlgEditProperty = nullptr;
//}

//------------------------------------------------------------------------------
/*! @brief Registers the given edit widget for the edit property dialog by
           installing an event filter for the given edit widget for the
           mouse double click event.

    @param [in] i_pEdtWidget
        Edit widget for which the edit dialog should be opened by a mouse
        double click. As a precondition an object name must have been set
        at the edit widget as follows:
        <DimensionUnit>.<CoordinatePart1>[.<CoordinatePart2>]
        The sections of the object name will be used by the edit dialog
        to create the labels and to decide, which property has to be
        edited. E.g. for "Pixels.Point 1.X" the x coordinate of Point 1
        in dimension unit pixels will be edited.
*/
//void CWdgtGraphObjPolygonGeometryProperties::registerEditPropertyDialog(CWdgtEditPhysVal* i_pEdtWidget)
//------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_pEdtWidget->objectName();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "registerEditPropertyDialog",
//        /* strAddInfo   */ strMthInArgs );
//
//    QString strObjName = i_pEdtWidget->objectName();
//    m_hshpRegisteredEditPropertyDialogs.insert(strObjName, i_pEdtWidget);
//    // To filter mouse events the event filter must also be set at the spin box
//    // and the line edit of the spin box. So we must call the installEventFilter
//    // method of class CWdgtEditPhysVal which not just installs the event filter
//    // on the EditPhysVal widget but also forwards the method to both the
//    // spin box and the line edit.
//    i_pEdtWidget->installEventFilter(this);
//}

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Opens the edit property dialog on pressing Ctrl + MouseDblClick.
*/
//bool CWdgtGraphObjPolygonGeometryProperties::eventFilter(QObject* i_pObjWatched, QEvent* i_pEv)
//------------------------------------------------------------------------------
//{
//    bool bHandled = false;
//    if (i_pEv->type() == QEvent::MouseButtonDblClick) {
//        QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(i_pEv);
//        if (pMouseEvent->modifiers() & Qt::ControlModifier) {
//            QString strObjName = i_pObjWatched->objectName();
//            QWidget* pEdtWidget = m_hshpRegisteredEditPropertyDialogs.value(strObjName, nullptr);
//            if (pEdtWidget != nullptr) {
//                if (m_pDlgEditProperty == nullptr) {
//                    m_pDlgEditProperty = new CDlgGraphObjLineGeometryEditProperty(m_pDrawingScene, this);
//                    m_pDlgEditProperty->setAttribute(Qt::WA_DeleteOnClose, true);
//                    QObject::connect(
//                        m_pDlgEditProperty, &QDialog::destroyed,
//                        this, &CWdgtGraphObjPolygonGeometryProperties::onDlgEditPropertyDestroyed);
//                }
//                CGraphObjLine* pGraphObjLine = nullptr;
//                if (m_pGraphObj != nullptr) {
//                    pGraphObjLine = dynamic_cast<CGraphObjLine*>(m_pGraphObj);
//                }
//                m_pDlgEditProperty->setCoordinate(pGraphObjLine, pEdtWidget->objectName());
//                m_pDlgEditProperty->show();
//                bHandled = true;
//            }
//        }
//    }
//    if (!bHandled) {
//        bHandled = CWdgtGraphObjPropertiesAbstract::eventFilter(i_pObjWatched, i_pEv);
//    }
//    return bHandled;
//}
