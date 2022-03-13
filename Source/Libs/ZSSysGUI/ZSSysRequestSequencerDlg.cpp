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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSSysGUI/ZSSysRequestSequencerDlg.h"
#include "ZSSysGUI/ZSSysRequestSequencerWdgt.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRequestSequencer.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgRequestSequencer : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgRequestSequencer* CDlgRequestSequencer::CreateInstance(
    const QString&  i_strDlgTitle,
    const QString&  i_strObjName,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    return new CDlgRequestSequencer(
        /* strObjName   */ i_strObjName,
        /* strDlgTitle  */ i_strDlgTitle,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgRequestSequencer* CDlgRequestSequencer::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgRequestSequencer*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CDlgRequestSequencer::CDlgRequestSequencer(
    const QString&  i_strDlgTitle,
    const QString&  i_strObjName,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags ) :
//------------------------------------------------------------------------------
    CDialog(
        /* strNameSpace */ NameSpace(),
        /* strClassName */ ClassName(),
        /* strObjName   */ i_strObjName,
        /* strDlgTitle  */ i_strDlgTitle,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags ),
    m_pLyt(nullptr),
    m_pReqSeq(nullptr),
    m_pWdgtReqSeq(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pWdgtReqSeq = new CWdgtRequestSequencer();
    m_pLyt->addWidget(m_pWdgtReqSeq);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgRequestSequencer::~CDlgRequestSequencer()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pReqSeq = nullptr;
    m_pWdgtReqSeq = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (content of edit fields)
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRequestSequencer::setSequencer( CRequestSequencer* i_pReqSeq )
//------------------------------------------------------------------------------
{
    if( m_pReqSeq != i_pReqSeq )
    {
        m_pReqSeq = i_pReqSeq;
        m_pWdgtReqSeq->setSequencer(m_pReqSeq);
    }

} // setSequencer
