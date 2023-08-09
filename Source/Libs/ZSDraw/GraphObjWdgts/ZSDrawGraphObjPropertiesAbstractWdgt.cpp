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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjPropertiesAbstractWdgt.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
//#include <QtGui/qcheckbox.h>
//#include <QtGui/qcolordialog.h>
//#include <QtGui/qcombobox.h>
//#include <QtGui/qfontcombobox.h>
//#include <QtGui/qgroupbox.h>
//#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
//#include <QtGui/qlineedit.h>
//#include <QtGui/qlistview.h>
#include <QtGui/qpushbutton.h>
//#include <QtGui/qspinbox.h>
#else
//#include <QtWidgets/qcheckbox.h>
//#include <QtWidgets/qcolordialog.h>
//#include <QtWidgets/qcombobox.h>
//#include <QtWidgets/qfontcombobox.h>
//#include <QtWidgets/qgroupbox.h>
//#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
//#include <QtWidgets/qlineedit.h>
//#include <QtWidgets/qlistview.h>
#include <QtWidgets/qpushbutton.h>
//#include <QtWidgets/qspinbox.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjPropertiesAbstract : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesAbstract::CWdgtGraphObjPropertiesAbstract(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strClassName,
    const QString& i_strObjName,
    EMode i_mode,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_mode(EMode::Edit),
    m_strKeyInTree(),
    m_pGraphObj(nullptr),
    m_cxLblWidthClm1(80),
    m_cxEdtWidthClm1(160),
    m_cxLblWidthClm2(80),
    m_cxEdtWidthClm2(160),
    m_cxClmSpacing(30),
    // Blocking signals counter
    m_iValueChangedSignalsBlocked(0),
    // Edit Controls
    m_pLyt(nullptr),
    // Button Line
    m_pWdgtButtons(nullptr),
    m_pLytWdgtButtons(nullptr),
    m_pBtnEdit(nullptr),
    // Trace admin object for method tracing
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::GraphObjWdgts", i_strClassName, i_strObjName);

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QObject::connect(
        /* pObjSender   */ m_pDrawingScene, &CDrawingScene::graphObjChanged,
        /* pObjReceiver */ this, &CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjChanged );

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
    m_mode = static_cast<EMode>(0);
    //m_strKeyInTree;
    m_pGraphObj = nullptr;
    m_cxLblWidthClm1 = 0;
    m_cxEdtWidthClm1 = 0;
    m_cxLblWidthClm2 = 0;
    m_cxEdtWidthClm2 = 0;
    m_cxClmSpacing = 0;
    // Blocking signals counter
    m_iValueChangedSignalsBlocked = 0;
    // Edit Controls
    m_pLyt = nullptr;
    // Button Line
    m_pWdgtButtons = nullptr;
    m_pLytWdgtButtons = nullptr;
    m_pBtnEdit = nullptr;
    // Trace admin object for method tracing
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected: // ctor auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
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

    m_pBtnEdit = new QPushButton("Edit");
    m_pBtnEdit->setEnabled(false);
    m_pLytWdgtButtons->addWidget(m_pBtnEdit);
    QObject::connect(
        m_pBtnEdit, &QPushButton::clicked,
        this, &CWdgtGraphObjPropertiesAbstract::onBtnEditClicked);

    m_pLytWdgtButtons->addStretch();

    return m_pWdgtButtons;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
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
void CWdgtGraphObjPropertiesAbstract::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::acceptChanges",
        /* strAddInfo   */ "" );
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::rejectChanges",
        /* strAddInfo   */ "" );
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::setMode(EMode i_mode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::setMode",
        /* strAddInfo   */ strMthInArgs );

    if (m_mode != i_mode) {
        if (i_mode == EMode::Edit) {
            if (m_pWdgtButtons != nullptr) {
                m_pWdgtButtons->show();
                m_pBtnEdit->setEnabled(false);
            }
        }
        else {
            if (m_pWdgtButtons != nullptr) {
                m_pWdgtButtons->show();
                m_pBtnEdit->setEnabled(true);
            }
        }
        m_mode = i_mode;
    }
}

//------------------------------------------------------------------------------
EMode CWdgtGraphObjPropertiesAbstract::mode() const
//------------------------------------------------------------------------------
{
    return m_mode;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::setKeyInTree( const QString& i_strKeyInTree )
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

    if (m_strKeyInTree != i_strKeyInTree)
    {
        m_strKeyInTree = i_strKeyInTree;

        if (m_strKeyInTree.isEmpty())
        {
            m_pGraphObj = nullptr;
        }
        else
        {
            m_pGraphObj = m_pDrawingScene->findGraphObj(i_strKeyInTree);
        }
    }
}

//------------------------------------------------------------------------------
QString CWdgtGraphObjPropertiesAbstract::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onBtnEditClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::onBtnEditClicked",
        /* strAddInfo   */ "" );
}

/*==============================================================================
private slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjChanged",
        /* strAddInfo   */ "" );
}

/*==============================================================================
private slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! Slot connected to the graphObjChanged signal of the drawing scene.

    The signal is emitted for any changed object of the drawing scene.
    The slot checks whether the changed object is the object shown by the widget
    and invokes the pure virtual method onGraphObjChanged.
    For all other objects the slot will be ignored.

    @param i_strKeyInTree [in]
        Unique key of the changed object.
*/
void CWdgtGraphObjPropertiesAbstract::onDrawingSceneGraphObjChanged(const QString& i_strKeyInTree)
//------------------------------------------------------------------------------
{
    if (m_pGraphObj != nullptr && m_pGraphObj->keyInTree() == i_strKeyInTree)
    {
        /*! On changing a setting of the graphical object "onGraphObjChanged" will be called as reentry.
            "onGraphObjChanged" will modify the states of the GUI controls. E.g. if the "NameLabelVisible" and
            "NameLabelAnchorLineVisible" CheckBoxes are checked and you call "showLabel" "onGraphObjChanged"
            is immediately called resetting the check state of "NameLabelAnchorLineVisible".
            To avoid this undesired correction the "onGraphObjChanged" method must be disabled while applying
            changes but will be called after all changes have been applied. For this the flag "m_bApplyingChanges"
            of the base class "CWdgtFormatGraphObjs" is set at the beginning, reset at the end of "applyChanges"
            and afterwards "onGraphObjChanged" is explicitly called.
        */
        #pragma message(__TODO__"avoid recursive call")
        //if (!m_bApplyingChanges)
        //{
        //    onGraphObjChanged();
        //}
    }
}
