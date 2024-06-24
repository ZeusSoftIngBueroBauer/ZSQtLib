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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjPropertiesAbstract : public QWidget
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

bool CWdgtGraphObjPropertiesAbstract::s_bWdgtLabelsVisible = false;
bool CWdgtGraphObjPropertiesAbstract::s_bWdgtGeometryVisible = false;
bool CWdgtGraphObjPropertiesAbstract::s_bWdgtLineStyleVisible = false;
bool CWdgtGraphObjPropertiesAbstract::s_bWdgtFillStyleVisible = false;
bool CWdgtGraphObjPropertiesAbstract::s_bWdgtTextStyleVisible = false;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesAbstract::CWdgtGraphObjPropertiesAbstract(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strNameSpace,
    const QString& i_strGraphObjType,
    const QString& i_strClassName,
    const QString& i_strObjName,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_strKeyInTree(),
    m_pGraphObj(nullptr),
    m_graphObjTypeCurr(EGraphObjTypeUndefined),
    m_graphObjTypePrev(EGraphObjTypeUndefined),
    m_bContentUpdateOnSelectedChanged(false),
    m_bContentUpdateOnGeometryChanged(false),
    m_bContentUpdateOnDrawSettingsChanged(false),
    m_bContentChanged(false),
    m_iContentChangedSignalBlockedCounter(0),
    m_bDrawingSceneDrawingSizeChangedSignalConnected(false),
    m_bDrawingSceneDrawSettingsChangedSignalConnected(false),
    // Edit Controls
    m_pLyt(nullptr),
    m_cxSpacing(5),
    m_ariClmWidths(),
    // Button Line
    m_pWdgtButtons(nullptr),
    m_pLytWdgtButtons(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnReset(nullptr),
    // Trace admin object for method tracing
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        i_strNameSpace + "::" + i_strGraphObjType, i_strClassName, i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::ctor",
        /* strAddInfo   */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    /*  Grid Layout with 11 columns. The last column is a stretch.
        To apply the layout to the grid layout use the following code in the constructor
        of the derived classes:

        for (int idxClm = 0; idxClm < m_ariClmWidths.size(); ++idxClm) {
            m_pGridLayout->setColumnMinimumWidth(idxClm, m_ariClmWidths[idxClm]);
        }
        m_pGridLayout->setColumnStretch(m_ariClmWidths.size(), 1);

        Example of the grid Layout used by the TextStyle widget:

           |     0    |1| 2 |3|  4  |5| 6    |7| 8    |9|  10   | 
         --+----------+-+---+-+-----+-+------+-+------+-+-------+----
         0 |Font:     | |   | |Font          | |Size  | |Color  |<-->
         1 |Effects:  | |   | |Bold | |Kursiv| |UnderL| |StrikeT|<-->
    */
    m_ariClmWidths = {
         /*  0 */ 50,
         /*  1 */ m_cxSpacing,
         /*  2 */ 32,
         /*  3 */ m_cxSpacing,
         /*  4 */ 60,
         /*  5 */ m_cxSpacing,
         /*  6 */ 60,
         /*  7 */ m_cxSpacing,
         /*  8 */ 60,
         /*  9 */ m_cxSpacing,
         /* 10 */ 60
    };

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesAbstract::~CWdgtGraphObjPropertiesAbstract()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDrawingScene = nullptr;
    //m_strKeyInTree;
    m_pGraphObj = nullptr;
    m_graphObjTypeCurr = static_cast<EGraphObjType>(0);
    m_graphObjTypePrev = static_cast<EGraphObjType>(0);
    m_bContentUpdateOnSelectedChanged = false;
    m_bContentUpdateOnGeometryChanged = false;
    m_bContentUpdateOnDrawSettingsChanged = false;
    m_bContentChanged = false;
    m_iContentChangedSignalBlockedCounter = 0;
    m_bDrawingSceneDrawingSizeChangedSignalConnected = false;
    m_bDrawingSceneDrawSettingsChangedSignalConnected = false;
    // Edit Controls
    m_pLyt = nullptr;
    m_cxSpacing = 0;
    //m_ariClmWidths;
    // Button Line
    m_pWdgtButtons = nullptr;
    m_pLytWdgtButtons = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnReset = nullptr;
    // Trace admin object for method tracing
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected: // ctor auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method may be called by derived class to create a widget with
           the two buttons Apply and Reject.

    @return Pointer to created widget. The caller must add the widget to an
           appropriate layout.
*/
QWidget* CWdgtGraphObjPropertiesAbstract::createButtonsLineWidget()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::createButtonsLineWidget",
        /* strAddInfo   */ "" );

    m_pWdgtButtons = new QWidget();
    m_pLytWdgtButtons = new QHBoxLayout();
    m_pWdgtButtons->setLayout(m_pLytWdgtButtons);
    m_pLytWdgtButtons->setContentsMargins(0, 0, 0, 0);

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setEnabled(false);
    m_pLytWdgtButtons->addWidget(m_pBtnApply);
    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CWdgtGraphObjPropertiesAbstract::onBtnApplyClicked);

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setEnabled(false);
    m_pLytWdgtButtons->addWidget(m_pBtnReset);
    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CWdgtGraphObjPropertiesAbstract::onBtnResetClicked);

    m_pLytWdgtButtons->addStretch();

    return m_pWdgtButtons;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Selects the object to be indicated and edited in the widget.

    "fillEditControls" is called to fill the edit controls with the settings
    of the graphical object.

    @param i_strKeyInTree [in]
        Unique key of the graphical object. Stored and set as key string to also
        support changing geometry of CDrawingView.
*/
bool CWdgtGraphObjPropertiesAbstract::setKeyInTree( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::setKeyInTree",
        /* strAddInfo   */ strMthInArgs );

    bool bObjectChanged = false;

    if (m_strKeyInTree != i_strKeyInTree)
    {
        bObjectChanged = true;

        if (m_pGraphObj != nullptr) {
            QObject::disconnect(
                m_pGraphObj, &CGraphObj::aboutToBeDestroyed,
                this, &CWdgtGraphObjPropertiesAbstract::onGraphObjAboutToBeDestroyed);
        }
        if (m_bContentUpdateOnSelectedChanged) {
            if (m_pGraphObj != nullptr) {
                QObject::disconnect(
                    m_pGraphObj, &CGraphObj::selectedChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onGraphObjSelectedChanged);
            }
        }
        if (m_bContentUpdateOnGeometryChanged) {
            if (m_pGraphObj != nullptr) {
                QObject::disconnect(
                    m_pGraphObj, &CGraphObj::geometryOnSceneChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onGraphObjGeometryOnSceneChanged);
            }
            if (m_bDrawingSceneDrawingSizeChangedSignalConnected) {
                QObject::disconnect(
                    m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneDrawingSizeChanged);
                m_bDrawingSceneDrawingSizeChangedSignalConnected = false;
            }
        }
        if (m_bContentUpdateOnDrawSettingsChanged) {
            if (m_pGraphObj != nullptr) {
                QObject::disconnect(
                    m_pGraphObj, &CGraphObj::drawSettingsChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onGraphObjDrawSettingsChanged);
            }
            if (m_bDrawingSceneDrawSettingsChangedSignalConnected) {
                QObject::disconnect(
                    m_pDrawingScene, &CDrawingScene::drawSettingsChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneDrawSettingsChanged);
                m_bDrawingSceneDrawSettingsChangedSignalConnected = false;
            }
        }

        m_strKeyInTree = i_strKeyInTree;

        if (m_strKeyInTree.isEmpty()) {
            m_pGraphObj = nullptr;
        }
        else {
            m_pGraphObj = m_pDrawingScene->findGraphObj(i_strKeyInTree);
        }
        if (m_pGraphObj == nullptr) {
            m_graphObjTypeCurr = EGraphObjTypeUndefined;
        }
        else {
            m_graphObjTypeCurr = m_pGraphObj->type();
        }

        if (m_pGraphObj != nullptr) {
            QObject::connect(
                m_pGraphObj, &CGraphObj::aboutToBeDestroyed,
                this, &CWdgtGraphObjPropertiesAbstract::onGraphObjAboutToBeDestroyed);
        }
        if (m_bContentUpdateOnSelectedChanged) {
            if (m_pGraphObj != nullptr) {
                QObject::connect(
                    m_pGraphObj, &CGraphObj::selectedChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onGraphObjSelectedChanged);
            }
        }
        if (m_bContentUpdateOnGeometryChanged) {
            if (m_pGraphObj != nullptr) {
                QObject::connect(
                    m_pGraphObj, &CGraphObj::geometryOnSceneChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onGraphObjGeometryOnSceneChanged);
            }
            if (!m_bDrawingSceneDrawingSizeChangedSignalConnected) {
                QObject::connect(
                    m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneDrawingSizeChanged);
                m_bDrawingSceneDrawingSizeChangedSignalConnected = true;
            }
        }
        if (m_bContentUpdateOnDrawSettingsChanged) {
            if (m_pGraphObj != nullptr) {
                QObject::connect(
                    m_pGraphObj, &CGraphObj::drawSettingsChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onGraphObjDrawSettingsChanged);
            }
            if (!m_bDrawingSceneDrawSettingsChangedSignalConnected) {
                QObject::connect(
                    m_pDrawingScene, &CDrawingScene::drawSettingsChanged,
                    this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneDrawSettingsChanged);
                m_bDrawingSceneDrawSettingsChangedSignalConnected = true;
            }
        }

        // Fill the content of the edit controls.
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            fillEditControls();
        }
        m_graphObjTypePrev = m_graphObjTypeCurr;
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bObjectChanged);
    }
    return bObjectChanged;
}

//------------------------------------------------------------------------------
/*! @brief Returns the unique key of the graphical object currently shown in
           the widget.
*/
QString CWdgtGraphObjPropertiesAbstract::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Virtual method which may be overridden by derived classes to check
           whether the user has entered meaningful data.

    @return true, if at least one edit control has an erronous setting that
            cannot be applied to the graphical object, false otherwise.
*/
bool CWdgtGraphObjPropertiesAbstract::hasErrors() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::hasErrors",
        /* strAddInfo   */ "" );

    bool bHasErrors = false;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasErrors);
    }
    return bHasErrors;
}

//------------------------------------------------------------------------------
/*! @brief Virtual method which has to be overridden by derived classes to check
           whether the user has changed settings which haven't been applied
           yet to the graphical object.

    Please note that if the properties widget hosts other property widgets and
    the graphical object is about to be destroyed, "onGraphObjChanged" is called
    to update the content of the widget. But the child property widgets may not
    have been informed yet that the graphical object is about to be destroyed
    and "m_pGraphObj" of the child widget may point to an already destroyed
    object. The child widgets slot may be called sometimes afterwards. For this
    the parent widget must check whether "m_pGraphObj" is not null before
    forwarding the "hasChanges" call to the childs.

    @return true, if at least one edit control has a different setting than the
            grahical object which need to be applied to take effect.
            false, if the settings shown are the current settings of the
            graphical object.
*/
bool CWdgtGraphObjPropertiesAbstract::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
/*! @brief This method is called to apply the changes made in the edit controls
           of the properties widget to the graphical object.

    The values from the properties widget are applied one after another at the
    graphical object.

    For each changed property the method "onGraphObjChanged" is called as an
    reentry. "onGraphObjChanged" is also used to fill the edit controls with the
    current property values of the graphical object if a new graphical object
    has been set or if the settings got to be reset.

    To avoid that "onGraphObjChanged" overwrites settings in edit controls which
    haven't been applied yet the m_iContentChangedSignalBlockedCounter is incremented
    before applying the changes from the edit controls.

    E.g. if the line width and the pen color are changed, both values will be set
    one after another at the graphical object. When applying line width "onGraphObjChanged"
    is called and the current pen color setting of the user control would be overwritten
    with the current pen color of the graphical object.
*/
void CWdgtGraphObjPropertiesAbstract::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::acceptChanges",
        /* strAddInfo   */ "" );

    if (!hasErrors())
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // Apply the settings from the edit controls at the graphical object.
            applySettings();
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            updateButtonsEnabled();
            emit_contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Resets the current settings of the edit controls to the current values
           of the graphical object.

    Resetting is done be invoking "onGraphObjChanged", which fills the edit
    controls with the current property values of the graphical object.
*/
void CWdgtGraphObjPropertiesAbstract::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::rejectChanges",
        /* strAddInfo   */ "" );

    // "fillEditControls" is used to fill the edit controls with the current settings
    // of the graphical object. To avoid that the signal "contentChanged" is emitted
    // for each property of the graphical object set at the edit controls the
    // ContentChangedSignalBlockedCounter is incremented. After fillEditControls has
    // been executed the contentChanged flag is checked and the contentChanged signal
    // is emitted if necessary.

    {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

        // Fill the content of the edit controls.
        fillEditControls();
    }

    // If the "contentChanged" signal is no longer blocked and the content of
    // properties widget has been changed ...
    if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
        // .. emit the contentChanged signal and update the enabled state
        // of the Apply and Reset buttons.
        updateButtonsEnabled();
        emit_contentChanged();
    }
}

//------------------------------------------------------------------------------
/*! @brief This method is called by acceptChanges to apply the settings from
           the edit controls at the graphical object.

    Derived classes should overwrite this method.

    The ContentChangedSignalBlocked counter has been incrememented by the
    acceptChanges method as on changing properties of the graphical object
    the on<Properties>Changed slots are called as reentries when applying the
    changes at the graphical object. The on<Properties>Changed slots should
    not call fillEditControls when applying the changes at the graphical object
    as that would overwrite current settings with old setting.

    The widget may be used to modify only part of the settings and may be
    a child of a widget used to modify also other settings.
    In this case the parent widget is responsibly for updating all settings at
    the drawing scene or the graphical object. For this the parent widget
    will set the flag i_bImmediatelyApplySettings to false and will update the
    changed settings after all child widgets have forwarded the changes to
    either the graphics scene or the graphical object.

    @param [in] i_bImmediatelyApplySetting (default: true)
        Set this flag to false if further set<DrawAttribute> will follow and
        all changes have to be updated at once later on.
*/
void CWdgtGraphObjPropertiesAbstract::applySettings(bool i_bImmediatelyApplySettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ImmediatelyApply: " + bool2Str(i_bImmediatelyApplySettings);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::applySettings",
        /* strAddInfo   */ strMthInArgs );
}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onBtnApplyClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onBtnApplyClicked",
        /* strAddInfo   */ "" );

    acceptChanges();
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onBtnResetClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onBtnResetClicked",
        /* strAddInfo   */ "" );

    rejectChanges();
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method is called by onGraphObjChanged or by rejectChanges
           to fill the edit controls with the current settings from the changed
           graphical object.

    Derived classes should overwrite this method.

    The ContentChangedSignalBlocked counter has been incrememented by the
    onGraphObjChanged and rejectChanges methods as the "contentChanged" signal
    should not be emitted for each property applied to the edit controls.
    The contentChanged signal should just be emitted once after all properties
    from the graphical objects have been set. In the fillEditControls method of
    the derived class the flag "contentChanged" has to be set if the
    ContentChangedSignalBlocked counter is greater than 0 and the property has
    been applied to the edit control instead of emitting the contentChanged signal.

    After invoking fillEditControls the methods onGraphObjChanged and rejectChanges
    are checking the contentChanged flag and emit the contentChanged signal if needed.
*/
void CWdgtGraphObjPropertiesAbstract::fillEditControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::fillEditControls",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
/*! @brief This method is called by the onGraphObjChanged or rejectChanges
           after fillEditControls has been called to update the enabled state
           of the Apply and Reset buttons.

    After invoking fillEditControls the methods onGraphObjChanged and rejectChanges
    are checking the contentChanged flag and invoke the updateButtonsEnabled method
    if the contentChanged flag is set.
*/
void CWdgtGraphObjPropertiesAbstract::updateButtonsEnabled()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::updateButtonsEnabled",
        /* strAddInfo   */ "" );

    if (m_pBtnApply != nullptr || m_pBtnReset != nullptr)
    {
        bool bEnabled = hasChanges();
        if (m_pBtnApply != nullptr) {
            if (hasErrors()) {
                m_pBtnApply->setEnabled(false);
            }
            else {
                m_pBtnApply->setEnabled(bEnabled);
            }
        }
        if (m_pBtnReset != nullptr) {
            m_pBtnReset->setEnabled(bEnabled);
        }
    }
}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Slot method connected to the drawingSizeChanged signal of the drawingScene.

    The default implementation in the base class does nothing. If the derived class
    should react on the signal the method must be reimplemented to call fillEditControls.

    Derived classes should overwrite the fillEditControls method to fill their edit
    controls with the current data of the drawing scene.

    The ContentChangedSignalBlocked counter should be incrememented before fillEditControls
    is called as the "contentChanged" signal should not be emitted for each applied
    property. This signal should just be emitted once after all properties from the
    have been set at all edit controls of the property widget and fillEditControls returns.

    Instead of emitting the contentChanged signal the derived class should set the flag
    "contentChanged" in the fillEditControls method if the ContentChangedSignalBlocked
    counter is greater than 0 and the property has been applied to the edit control.

    After invoking fillEditControls the method onDrawingSceneDrawingSizeChanged should
    check the "contentChanged" flag and emits the contentChanged signal if needed.
*/
void CWdgtGraphObjPropertiesAbstract::onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onDrawingSceneDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    #if 0 // Template for copy and paste to the derived class implementation
    //----------------------------------------------------------------------

    // When applying the changes from the edit controls by invoking "acceptChanges"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that
    // "onDrawingSceneDrawingSizeChanged" overwrites settings in edit controls which
    // haven't been applied yet.
    if (m_drawingSize != i_drawingSizes && m_iContentChangedSignalBlockedCounter == 0)
    {
        m_drawingSize = i_drawingSizes;

        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // Here the derived class should apply the properties from the graphical
            // object to the edit controls.
            fillEditControls();
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            updateButtonsEnabled();
            emit_contentChanged();
        }
    }
    #endif
}

//------------------------------------------------------------------------------
/*! @brief Slot method connected to the drawSettingsChanged signal of the drawingScene.

    The default implementation in the base class does nothing. If the derived class
    should react on the signal the method must be reimplemented to call fillEditControls.

    Derived classes should overwrite the fillEditControls method to fill their edit
    controls with the current data of the drawing scene.

    The ContentChangedSignalBlocked counter should be incrememented before fillEditControls
    is called as the "contentChanged" signal should not be emitted for each applied
    property. This signal should just be emitted once after all properties from the
    have been set at all edit controls of the property widget and fillEditControls returns.

    Instead of emitting the contentChanged signal the derived class should set the flag
    "contentChanged" in the fillEditControls method if the ContentChangedSignalBlocked
    counter is greater than 0 and the property has been applied to the edit control.

    After invoking fillEditControls the method onDrawingSceneDrawSettingsChanged should
    check the "contentChanged" flag and emits the contentChanged signal if needed.
*/
void CWdgtGraphObjPropertiesAbstract::onDrawingSceneDrawSettingsChanged(const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onDrawingSceneDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    #if 0 // Template for copy and paste to the derived class implementation
    //----------------------------------------------------------------------

    // When applying the changes from the edit controls by invoking "acceptChanges"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that
    // "onDrawingSceneDrawSettingsChanged" overwrites settings in edit controls which
    // haven't been applied yet.
    if (m_drawSettings != i_drawSettings && m_iContentChangedSignalBlockedCounter == 0)
    {
        m_drawSettings = i_drawSettings;

        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // Here the derived class should apply the properties from the graphical
            // object to the edit controls.
            fillEditControls();
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            updateButtonsEnabled();
            emit_contentChanged();
        }
    }
    #endif
}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Slot method connected to the selectedChanged signal of the graphical object.

    The method calls fillEditControls. Derived classes should overwrite the
    fillEditControls method to fill their edit controls with the current data
    of the graphical object.

    The ContentChangedSignalBlocked counter is incrememented before fillEditControls
    is called as the "contentChanged" signal should not be emitted for each applied
    property. This signal should just be emitted once after all properties from the
    graphical objects have been set at all edit controls of the property widget and
    fillEditControls returns.

    Instead of emitting the contentChanged signal the derived class should set the flag
    "contentChanged" in the fillEditControls method if the ContentChangedSignalBlocked
    counter is greater than 0 and the property has been applied to the edit control.

    After invoking fillEditControls the method onGraphObjSelectedChanged checks the
    "contentChanged" flag and emits the contentChanged signal if needed.
*/
void CWdgtGraphObjPropertiesAbstract::onGraphObjSelectedChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onGraphObjSelectedChanged",
        /* strAddInfo   */ "" );

    // When applying the changes from the edit controls by invoking "acceptChanges"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that
    // "onGraphObjChanged" overwrites settings in edit controls which haven't been
    // applied yet. E.g. if the line width and the pen color are changed, both values
    // will be set one after another at the graphical object. When applying line width
    // "onGraphObjChanged" is called and the current pen color setting of the user
    // control would be overwritten with the current pen color of the graphical object.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // Here the derived class should apply the properties from the graphical
            // object to the edit controls.
            fillEditControls();
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            updateButtonsEnabled();
            emit_contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Slot method connected to the geometryOnSceneChanged signal of the graphical object.

    See description of method onGraphObjSelectedChanged on how derived classes
    should handle the content changed flag and the content changed signal blocked
    counter in method fillEditControls.
*/
void CWdgtGraphObjPropertiesAbstract::onGraphObjGeometryOnSceneChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onGraphObjGeometryOnSceneChanged",
        /* strAddInfo   */ "" );

    // When applying the changes from the edit controls by invoking "acceptChanges"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that
    // "onGraphObjChanged" overwrites settings in edit controls which haven't been
    // applied yet. E.g. if the line width and the pen color are changed, both values
    // will be set one after another at the graphical object. When applying line width
    // "onGraphObjChanged" is called and the current pen color setting of the user
    // control would be overwritten with the current pen color of the graphical object.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // Here the derived class should apply the properties from the graphical
            // object to the edit controls.
            fillEditControls();
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            updateButtonsEnabled();
            emit_contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Slot method connected to the drawSettingsChanged signal of the graphical object.

    See description of method onGraphObjSelectedChanged on how derived classes
    should handle the content changed flag and the content changed signal blocked
    counter in method fillEditControls.
*/
void CWdgtGraphObjPropertiesAbstract::onGraphObjDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onGraphObjDrawSettingsChanged",
        /* strAddInfo   */ "" );

    // When applying the changes from the edit controls by invoking "acceptChanges"
    // the ContentChangedSignalBlockedCounter is incremented to avoid that
    // "onGraphObjChanged" overwrites settings in edit controls which haven't been
    // applied yet. E.g. if the line width and the pen color are changed, both values
    // will be set one after another at the graphical object. When applying line width
    // "onGraphObjChanged" is called and the current pen color setting of the user
    // control would be overwritten with the current pen color of the graphical object.
    if (m_iContentChangedSignalBlockedCounter == 0)
    {
        {   CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

            // Here the derived class should apply the properties from the graphical
            // object to the edit controls.
            fillEditControls();
        }

        // If the "contentChanged" signal is no longer blocked and the content of
        // properties widget has been changed ...
        if (m_iContentChangedSignalBlockedCounter == 0 && m_bContentChanged) {
            // .. emit the contentChanged signal and update the enabled state
            // of the Apply and Reset buttons.
            updateButtonsEnabled();
            emit_contentChanged();
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onGraphObjAboutToBeDestroyed(CGraphObj*)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onGraphObjAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pGraphObj = nullptr;

    fillEditControls();
}

/*==============================================================================
protected: // instance methods (tracing emitting signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Emits the contentChanged signal and resets the content changed flags.
*/
void CWdgtGraphObjPropertiesAbstract::emit_contentChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::emit_contentChanged",
        /* strAddInfo   */ "" );
    m_bContentChanged = false;
    emit contentChanged();
}
