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

#include <QtCore/qglobal.h>
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#endif

#include "ZSDraw/ZSDrawWdgtFormatGraphObjsLabels.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class CWdgtFormatGraphObjsLabels : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsLabels::CWdgtFormatGraphObjsLabels(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene, i_pGraphObj, i_pWdgtParent),
    m_pLyt(nullptr),
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
    setObjectName("WdgtFormatGraphObjsObjName");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "nullptr" : i_pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    QVBoxLayout* m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

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

    // Set settings at GUI controls
    //-----------------------------

    if( m_pGraphObj == nullptr )
    {
        m_pEdtName->setEnabled(false);
    }
    else // if( m_pGraphObj != nullptr )
    {
        onGraphObjChanged();
    }

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsLabels::~CWdgtFormatGraphObjsLabels()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
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

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabels::applyChanges()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "applyChanges",
        /* strAddInfo   */ strMthInArgs );

    // On changing a setting of the graphical object "onGraphObjChanged" will be called as reentry.
    // "onGraphObjChanged" will modify the states of the GUI controls. E.g. if the name and
    // the description should be changed on calling "rename" as reenty "onGraphObjChanged"
    // is immediately called resetting the description.
    // To avoid this undesired correction the "onGraphObjChanged" method must be disabled while applying
    // changes but will be called after all changes have been applied. For this the flag "m_bApplyingChanges"
    // of the base class "CWdgtFormatGraphObjs" is set at the beginning, reset at the end of "applyChanges"
    // and afterwards "onGraphObjChanged" is explicitly called.

    m_bApplyingChanges = true;

    if( m_pGraphObj != nullptr )
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

    // At the end update the user controls with the last changes from the graphical object.
    //=====================================================================================

    // This might be necessary if on setting one property the graphical object itself
    // internally corrects other settings.

    m_bApplyingChanges = false;

    onGraphObjChanged();

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabels::resetChanges()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resetChanges",
        /* strAddInfo   */ strMthInArgs );

    if( m_pGraphObj == nullptr )
    {
        m_pEdtName->setText("");
        m_pEdtPath->setText("");
        m_pEdtDescription->setText("");
    }
    else // if( m_pGraphObj != nullptr )
    {
        onGraphObjChanged();
    }

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsLabels::hasChanges() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "hasChanges",
        /* strAddInfo   */ strMthInArgs );

    bool bHasChanges = false;

    if( m_pGraphObj != nullptr )
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

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(bHasChanges);
    }

    return bHasChanges;

} // hasChanges

/*==============================================================================
protected: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsLabels::onGraphObjChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onGraphObjChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_pEdtName->text() != m_pGraphObj->name() )
    {
        m_pEdtName->setText(m_pGraphObj->name());
    }
    if( m_pEdtPath->text() != m_pGraphObj->path() )
    {
        m_pEdtPath->setText(m_pGraphObj->path());
    }
    if( m_pEdtDescription->text() != m_pGraphObj->getDescription() )
    {
        m_pEdtDescription->setText(m_pGraphObj->getDescription());
    }
} // onGraphObjChanged
