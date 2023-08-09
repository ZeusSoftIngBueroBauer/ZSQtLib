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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjPropertiesLabelsWdgt.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
//#include "ZSDraw/Drawing/ZSDrawingScene.h"
//#include "ZSSys/ZSSysErrResult.h"
//#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

//#include <QtCore/qglobal.h>
//#include <QtCore/qtimer.h>
//#include <QtGui/qbitmap.h>
//#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
//#include <QtGui/qapplication.h>
//#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#else
//#include <QtWidgets/qapplication.h>
//#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjPropertiesLabels : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesLabels::CWdgtGraphObjPropertiesLabels(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strParentClassName,
    const QString& i_strObjName,
    EMode i_mode,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    CWdgtGraphObjPropertiesAbstract(i_pDrawingScene, i_strParentClassName + "::" + ClassName(), i_strObjName, i_mode, i_pWdgtParent),
    m_pLblHeadLine(nullptr),
    m_pLytName(nullptr),
    m_pLblName(nullptr),
    m_pEdtName(nullptr),
    m_pLytPath(nullptr),
    m_pLblPath(nullptr),
    m_pEdtPath(nullptr),
    m_pLytDescription(nullptr),
    m_pLblDescription(nullptr),
    m_pEdtDescription(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    int cxLblWidth = 60;
    //int cxEdtWidth = 80;

    QFont fntHeadLine;

    m_pLblHeadLine = new QLabel("Object Name");
    fntHeadLine = m_pLblHeadLine->font();
    fntHeadLine.setPointSize(fntHeadLine.pointSize()+4);
    m_pLblHeadLine->setFont(fntHeadLine);
    m_pLyt->addWidget(m_pLblHeadLine);
    m_pLyt->addSpacing(4);

    // <Line> Name
    //============

    m_pLytName = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytName);

    // <LineEdit> Name
    //----------------

    m_pLblName = new QLabel("Name:");
    m_pLblName->setFixedWidth(cxLblWidth);
    m_pLytName->addWidget(m_pLblName);
    m_pEdtName = new QLineEdit();
    m_pLytName->addWidget(m_pEdtName);

    // <Line> Path
    //============

    m_pLytPath = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytPath);

    // <LineEdit> Path
    //----------------

    m_pLblPath = new QLabel("Path:");
    m_pLblPath->setFixedWidth(cxLblWidth);
    m_pLytPath->addWidget(m_pLblPath);
    m_pEdtPath = new QLineEdit();
    m_pEdtPath->setEnabled(false);
    m_pLytPath->addWidget(m_pEdtPath);

    // <Line> Description
    //===================

    m_pLytDescription = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDescription);

    // <LineEdit> Path
    //----------------

    m_pLblDescription = new QLabel("Description:");
    m_pLblDescription->setFixedWidth(cxLblWidth);
    m_pLytDescription->addWidget(m_pLblDescription);
    m_pEdtDescription = new QLineEdit();
    m_pLytDescription->addWidget(m_pEdtDescription);

    // Set Mode
    //---------

    setMode(i_mode);

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesLabels::~CWdgtGraphObjPropertiesLabels()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pLblHeadLine = nullptr;
    m_pLytName = nullptr;
    m_pLblName = nullptr;
    m_pEdtName = nullptr;
    m_pLytPath = nullptr;
    m_pLblPath = nullptr;
    m_pEdtPath = nullptr;
    m_pLytDescription = nullptr;
    m_pLblDescription = nullptr;
    m_pEdtDescription = nullptr;

} // dtor

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtGraphObjPropertiesLabels::hasChanges() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ "" );

    bool bHasChanges = false;

    if (m_pGraphObj != nullptr)
    {
        if( m_pGraphObj->name() != m_pEdtName->text() )
        {
            bHasChanges = true;
        }
        else if( m_pGraphObj->path() != m_pEdtPath->text() )
        {
            bHasChanges = true;
        }
        else if( m_pGraphObj->getDescription() != m_pEdtDescription->text() )
        {
            bHasChanges = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHasChanges);
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::acceptChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "acceptChanges",
        /* strAddInfo   */ "" );

    if (m_pGraphObj != nullptr)
    {
        if( m_pEdtName->text() != m_pGraphObj->name() )
        {
            m_pGraphObj->rename(m_pEdtName->text());
        }
        if( m_pEdtDescription->text() != m_pGraphObj->getDescription() )
        {
            m_pGraphObj->setDescription(m_pEdtDescription->text());
        }
    }
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::rejectChanges()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "rejectChanges",
        /* strAddInfo   */ "" );

    if (m_pGraphObj != nullptr)
    {
        m_pEdtName->setText("");
        m_pEdtPath->setText("");
        m_pEdtDescription->setText("");
    }
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::setMode(EMode i_mode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumMode(i_mode).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMode",
        /* strAddInfo   */ strMthInArgs );

    if (m_mode != i_mode)
    {
        // Don't take over mode to member as base class method is also called and also needs
        // to check whether the mode has been changed to modify the edit button widget.
        if (i_mode == EMode::Edit)
        {
        }
        else // if (i_mode == EMode::View)
        {
        }
    }

    CWdgtGraphObjPropertiesAbstract::setMode(i_mode);
}

/*==============================================================================
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::setKeyInTree(const QString& i_strKeyInTree)
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

    CWdgtGraphObjPropertiesAbstract::setKeyInTree(i_strKeyInTree);

    onGraphObjChanged();
}

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesLabels::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onGraphObjChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_pGraphObj == nullptr)
    {
        m_pEdtName->setText("");
        m_pEdtPath->setText("");
        m_pEdtDescription->setText("");
    }
    else
    {
        m_pEdtName->setText(m_pGraphObj->name());
        m_pEdtPath->setText(m_pGraphObj->path());
        m_pEdtDescription->setText(m_pGraphObj->getDescription());
    }
}
