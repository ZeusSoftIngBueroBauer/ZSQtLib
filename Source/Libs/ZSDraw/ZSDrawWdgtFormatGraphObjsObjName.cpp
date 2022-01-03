/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDraw/ZSDrawWdgtFormatGraphObjsObjName.h"
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
class CWdgtFormatGraphObjsObjName : public CWdgtFormatGraphObjs
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsObjName::CWdgtFormatGraphObjsObjName(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtFormatGraphObjs(i_pDrawingScene,i_pGraphObj,i_pWdgtParent),
    m_pLyt(nullptr),
    m_pLblHeadLine(nullptr),
    m_pLytObjName(nullptr),
    m_pLblObjName(nullptr),
    m_pEdtObjName(nullptr),
    m_pChkObjNameVisible(nullptr),
    m_pLytObjId(nullptr),
    m_pLblObjId(nullptr),
    m_pEdtObjId(nullptr),
    m_pChkObjIdVisible(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("WdgtFormatGraphObjsObjName");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

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

    // <Line> Object Name
    //===================

    m_pLytObjName = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytObjName);

    // <LineEdit> Object Name
    //-----------------------

    m_pLblObjName = new QLabel("Name:");
    m_pLblObjName->setFixedWidth(cxLblWidth);
    m_pLytObjName->addWidget(m_pLblObjName);
    m_pEdtObjName = new QLineEdit();
    m_pLytObjName->addWidget(m_pEdtObjName);

    if( !connect(
        /* pObjSender   */ m_pEdtObjName,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtObjNameTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Object Name
    //-----------------------------

    m_pChkObjNameVisible = new QCheckBox("Visible");
    m_pLytObjName->addWidget(m_pChkObjNameVisible);

    // <Line> Object Id
    //=================

    m_pLytObjId = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytObjId);

    // <LineEdit> Object Id
    //-----------------------

    m_pLblObjId = new QLabel("Id:");
    m_pLblObjId->setFixedWidth(cxLblWidth);
    m_pLytObjId->addWidget(m_pLblObjId);
    m_pEdtObjId = new QLineEdit();
    m_pLytObjId->addWidget(m_pEdtObjId);

    if( !connect(
        /* pObjSender   */ m_pEdtObjId,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtObjIdTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <CheckBox> Show Object Id
    //-----------------------------

    m_pChkObjIdVisible = new QCheckBox("Visible");
    m_pLytObjId->addWidget(m_pChkObjIdVisible);

    // Set settings at GUI controls
    //-----------------------------

    if( m_pGraphObj == nullptr )
    {
        m_strObjName = "";
        m_strObjId   = "";

        m_pEdtObjName->setEnabled(false);
        m_pEdtObjId->setEnabled(false);

        if( m_pChkObjNameVisible != nullptr )
        {
            m_pChkObjNameVisible->setEnabled(false);
        }
        if( m_pChkObjIdVisible != nullptr )
        {
            m_pChkObjIdVisible->setEnabled(false);
        }
    }
    else // if( m_pGraphObj != nullptr )
    {
        m_strObjName = m_pGraphObj->name();
        m_strObjId   = m_pGraphObj->keyInTree();

        m_pEdtObjName->setEnabled(true);
        m_pEdtObjId->setEnabled(true);

        if( m_pChkObjNameVisible != nullptr )
        {
            m_pChkObjNameVisible->setChecked( m_pGraphObj->isObjNameVisible() );
            m_pChkObjNameVisible->setEnabled(true);
        }
        if( m_pChkObjIdVisible != nullptr )
        {
            m_pChkObjIdVisible->setChecked( m_pGraphObj->isObjIdVisible() );
            m_pChkObjIdVisible->setEnabled(true);
        }
    } // if( m_pGraphObj != nullptr )

    m_pEdtObjName->setText(m_strObjName);
    m_pEdtObjId->setText(m_strObjId);

    // <Stretch> at bottom of Widget
    //==============================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjsObjName::~CWdgtFormatGraphObjsObjName()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pLyt = nullptr;
    m_pLblHeadLine = nullptr;
    m_pLytObjName = nullptr;
    m_pLblObjName = nullptr;
    m_pEdtObjName = nullptr;
    m_pChkObjNameVisible = nullptr;
    m_pLytObjId = nullptr;
    m_pLblObjId = nullptr;
    m_pEdtObjId = nullptr;
    m_pChkObjIdVisible = nullptr;

    // Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CWdgtFormatGraphObjs
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsObjName::applyChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "applyChanges",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pGraphObj != nullptr )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);

        //m_pGraphObj->setName(m_strObjName);
        //m_pGraphObj->setKeyInTree(m_strObjId);

        if( m_pChkObjNameVisible != nullptr )
        {
            if( m_pChkObjNameVisible->isChecked() )
            {
                if( !m_pGraphObj->isObjNameVisible() )
                {
                    m_pGraphObj->showObjName();
                }
            }
            else
            {
                if( m_pGraphObj->isObjNameVisible() )
                {
                    m_pGraphObj->hideObjName();
                }
            }
        }

        if( m_pChkObjIdVisible != nullptr )
        {
            if( m_pChkObjIdVisible->isChecked() )
            {
                if( !m_pGraphObj->isObjIdVisible() )
                {
                    m_pGraphObj->showObjId();
                }
            }
            else
            {
                if( m_pGraphObj->isObjIdVisible() )
                {
                    m_pGraphObj->hideObjId();
                }
            }
        }

    } // if( m_pGraphObj != nullptr )

} // applyChanges

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsObjName::resetChanges()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pGraphObj == nullptr )
    {
        m_strObjName = "";
        m_strObjId   = "";
    }
    else // if( m_pGraphObj != nullptr )
    {
        m_strObjName = m_pGraphObj->name();
        m_strObjId   = m_pGraphObj->keyInTree();

        if( m_pChkObjNameVisible != nullptr )
        {
            m_pChkObjNameVisible->setChecked( m_pGraphObj->isObjNameVisible() );
        }
        if( m_pChkObjIdVisible != nullptr )
        {
            m_pChkObjIdVisible->setChecked( m_pGraphObj->isObjIdVisible() );
        }
    } // if( m_pGraphObj != nullptr )

    m_pEdtObjName->setText(m_strObjName);
    m_pEdtObjId->setText(m_strObjId);

} // resetChanges

//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjsObjName::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    if( m_pGraphObj != nullptr )
    {
        if( m_pGraphObj->name() != m_strObjName )
        {
            bHasChanges = true;
        }
        else if( m_pGraphObj->keyInTree() != m_strObjId )
        {
            bHasChanges = true;
        }
        if( !bHasChanges && m_pChkObjNameVisible != nullptr )
        {
            bHasChanges = (m_pGraphObj->isObjNameVisible() != m_pChkObjNameVisible->isChecked());
        }
        if( !bHasChanges && m_pChkObjIdVisible != nullptr )
        {
            bHasChanges = (m_pGraphObj->isObjIdVisible() != m_pChkObjIdVisible->isChecked());
        }
    }
    return bHasChanges;

} // hasChanges

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsObjName::onEdtObjNameTextChanged( const QString& /*i_strText*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtObjNameTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_strObjName = m_pEdtObjName->text();

} // onEdtObjNameTextChanged

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjsObjName::onEdtObjIdTextChanged( const QString& /*i_strText*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtObjIdTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_strObjId = m_pEdtObjId->text();

} // onEdtObjIdTextChanged
