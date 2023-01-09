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

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSPhysValGUI/ZSPhysValDlgEditPhysVal.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CDlgEditPhysVal : public QDialog
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgEditPhysVal::CDlgEditPhysVal(
    const CPhysVal& i_physVal,
    double          i_fMin,
    double          i_fMax,
    double          i_iDecimals,
    QWidget*        i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent),
    m_physValOld(i_physVal),
    m_pPhysSize(i_physVal.physSize()),
    m_pPhysUnit(i_physVal.physUnit()),
    m_fMin(i_fMin),
    m_fMax(i_fMax),
    m_iDecimals(i_iDecimals),
    m_pLytDlg(nullptr),
    m_pLytLine(nullptr),
    m_pEdtVal(nullptr),
    m_pCmbUnit(nullptr),
    m_pLytButtons(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnCancel(nullptr)
{
    setWindowTitle( QApplication::applicationName() + ": Enter Value");

    m_pLytDlg = new QVBoxLayout();
    setLayout(m_pLytDlg);

    m_pLytLine = new QHBoxLayout();
    m_pLytDlg->addLayout(m_pLytLine);

    // Edit field
    //-----------

    m_pEdtVal = new QLineEdit();
    m_pEdtVal->setText( QString::number(m_physValOld.getVal()) );
    m_pEdtVal->setValidator( new QDoubleValidator(m_fMin,m_fMax,m_iDecimals,m_pEdtVal) );
    m_pLytLine->addWidget(m_pEdtVal);

    // Unit Combo
    //-----------

    if( m_pPhysSize != nullptr && m_pPhysUnit != nullptr && m_pPhysSize->count() > 0 )
    {
        int idxUnit;

        m_pCmbUnit = new QComboBox();
        m_pLytLine->addWidget(m_pCmbUnit);

        for( idxUnit = 0; idxUnit < static_cast<int>(m_pPhysSize->count()); idxUnit++ )
        {
            m_pCmbUnit->addItem(m_pPhysSize->getPhysUnit(idxUnit)->symbol());
        }
        m_pCmbUnit->setCurrentIndex( m_pCmbUnit->findText(m_pPhysUnit->symbol()) );

        if( !connect(
            /* pObjSender   */ m_pCmbUnit,
            /* szSignal     */ SIGNAL(activated(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbUnitActivated(int)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    // Ok, Accept and Cancel buttons
    //-------------------------------

    m_pLytButtons = new QHBoxLayout();
    m_pLytDlg->addSpacing(10);
    m_pLytDlg->addLayout(m_pLytButtons);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytButtons->addWidget(m_pBtnOk);
    m_pBtnOk->setDefault(true);
    if( !connect(
        /* pObjSender   */ m_pBtnOk,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(accept()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pBtnReset = new QPushButton("Reset");
    m_pLytButtons->addWidget(m_pBtnReset);
    if( !connect(
        /* pObjSender   */ m_pBtnReset,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDlgBtnResetClicked()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pBtnCancel = new QPushButton("Cancel");
    m_pLytButtons->addWidget(m_pBtnCancel);
    if( !connect(
        /* pObjSender   */ m_pBtnCancel,
        /* szSignal     */ SIGNAL(clicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(reject()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pLytButtons->addStretch();

} // ctor

//------------------------------------------------------------------------------
CDlgEditPhysVal::~CDlgEditPhysVal()
//------------------------------------------------------------------------------
{
    m_pPhysSize = nullptr;
    m_pPhysUnit = nullptr;
    m_pLytDlg = nullptr;
    m_pLytLine = nullptr;
    m_pEdtVal = nullptr;
    m_pCmbUnit = nullptr;
    m_pLytButtons = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnCancel = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal CDlgEditPhysVal::getPhysVal() const
//------------------------------------------------------------------------------
{
    CPhysVal   physVal = m_physValOld;

    QString    strSymbol = m_pCmbUnit->currentText();
    CPhysUnit* pPhysUnit = m_pPhysSize->findPhysUnitBySymbol(strSymbol);
    double     fVal = m_pEdtVal->text().toDouble();

    if( pPhysUnit != nullptr )
    {
        physVal = CPhysVal( fVal, pPhysUnit );
    }
    return physVal;

} // getPhysVal

/*==============================================================================
protected slots: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgEditPhysVal::accept()
//------------------------------------------------------------------------------
{
    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgEditPhysVal::reject()
//------------------------------------------------------------------------------
{
    QDialog::reject();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgEditPhysVal::onDlgBtnResetClicked()
//------------------------------------------------------------------------------
{
} // onDlgBtnResetClicked

//------------------------------------------------------------------------------
void CDlgEditPhysVal::onCmbUnitActivated( int /*i_idx*/ )
//------------------------------------------------------------------------------
{
    QString    strSymbol = m_pCmbUnit->currentText();
    CPhysUnit* pPhysUnit = m_pPhysSize->findPhysUnitBySymbol(strSymbol);
    double     fVal = m_pEdtVal->text().toDouble();

    if( pPhysUnit != nullptr && pPhysUnit != m_pPhysUnit )
    {
        fVal = CPhysVal(fVal,m_pPhysUnit).getVal(pPhysUnit);
        m_pPhysUnit = pPhysUnit;
        m_pEdtVal->setText( QString::number(fVal) );
    }

} // onCmbUnitActivated
