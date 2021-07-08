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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSSysGUI/ZSSysFindTextDlg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDlgFindText : public QDialog
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgFindText* CDlgFindText::CreateInstance(
    const QString& i_strObjName,
    const QString& i_strDlgTitle,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wFlags )
//------------------------------------------------------------------------------
{
    if( CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName) != nullptr )
    {
        QString strKey = buildPathStr("::", NameSpace(), ClassName(), i_strObjName);
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKey);
    }

    return new CDlgFindText(
        /* strObjName   */ i_strObjName,
        /* strDlgTitle  */ i_strDlgTitle,
        /* pWdgtParent  */ i_pWdgtParent,
        /* wFlags       */ i_wFlags );

} // CreateInstance

//------------------------------------------------------------------------------
CDlgFindText* CDlgFindText::GetInstance( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CDlgFindText*>(CDialog::GetInstance(NameSpace(), ClassName(), i_strObjName));
}

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgFindText::CDlgFindText(
    const QString&  i_strObjName,
    const QString&  i_strDlgTitle,
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
    m_pEdt(nullptr),
    m_pLyt(nullptr),
    m_pLytFindText(nullptr),
    m_pEdtFindText(nullptr),
    m_pBtnFindNext(nullptr),
    m_pLytFindTextResult(nullptr),
    m_pLblFindTextResult(nullptr)
{
    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pLytFindText = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytFindText);

    m_pEdtFindText = new QLineEdit();
    m_pLytFindText->addWidget(m_pEdtFindText,1);

    m_pBtnFindNext = new QPushButton("Next");
    m_pLytFindText->addWidget(m_pBtnFindNext);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnFindNext,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnFindNextClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytFindTextResult = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytFindTextResult);

    m_pLblFindTextResult = new QLabel();
    m_pLytFindTextResult->addWidget(m_pLblFindTextResult,1);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgFindText::~CDlgFindText()
//------------------------------------------------------------------------------
{
    m_pEdt = nullptr;
    m_pLyt = nullptr;
    m_pLytFindText = nullptr;
    m_pEdtFindText = nullptr;
    m_pBtnFindNext = nullptr;
    m_pLytFindTextResult = nullptr;
    m_pLblFindTextResult = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgFindText::setTextEdit( QTextEdit* i_pEdt )
//------------------------------------------------------------------------------
{
    m_pEdt = i_pEdt;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgFindText::findNext()
//------------------------------------------------------------------------------
{
    if( m_pLblFindTextResult != nullptr )
    {
        m_pLblFindTextResult->setText("");
    }

    if( m_pEdtFindText != nullptr && m_pEdt != nullptr )
    {
        QString strExp = m_pEdtFindText->text();

        QTextDocument::FindFlags findFlags;

        //QTextDocument::FindBackward;
        //QTextDocument::FindCaseSensitively;
        //QTextDocument::FindWholeWords;

        bool bExpFound = m_pEdt->find(strExp,findFlags);

        if( m_pLblFindTextResult != nullptr )
        {
            if( bExpFound )
            {
                m_pLblFindTextResult->setText("Text found");
            }
            else
            {
                m_pLblFindTextResult->setText("Text not found");
            }
        }
    }

} // findNext

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgFindText::onBtnFindNextClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    findNext();
}
