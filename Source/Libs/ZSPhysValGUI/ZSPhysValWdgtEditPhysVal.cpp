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

#include <float.h>

#include <QtCore/qcoreapplication.h>

#include <QtGui/qevent.h>
#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qspinbox.h>
#endif

#include "ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


namespace ZS
{
namespace PhysVal
{
namespace GUI
{
//******************************************************************************
class CDoubleSpinBox : public QDoubleSpinBox
//******************************************************************************
{
//==============================================================================
public: // ctors and dtor
//==============================================================================

//------------------------------------------------------------------------------
CDoubleSpinBox( QWidget* i_pWdgtParent = nullptr ) :
//------------------------------------------------------------------------------
    QDoubleSpinBox(i_pWdgtParent)
{
} // ctor

//------------------------------------------------------------------------------
virtual ~CDoubleSpinBox()
//------------------------------------------------------------------------------
{
} // dtor

//==============================================================================
public: // overridables of base class QDoubleSpinBox
//==============================================================================

//------------------------------------------------------------------------------
virtual QValidator::State validate( QString& i_strVal, int& i_iPos ) const
//------------------------------------------------------------------------------
{
    return QDoubleSpinBox::validate(i_strVal,i_iPos);
}

//==============================================================================
public: // overridables of base class QAbstractSpinBox
//==============================================================================

virtual void keyPressEvent(QKeyEvent* i_pEv) override
{
    //QString strTxt = i_pEv->text();
    return QDoubleSpinBox::keyPressEvent(i_pEv);
}

}; // class CDoubleSpinBox

} // namespace GUI

} // namespace PhysVal

} // namespace ZS


/*******************************************************************************
class CWdgtEditPhysVal : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtEditPhysVal::CWdgtEditPhysVal( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pLyt(nullptr),
    m_pEdt(nullptr),
    m_physVal(),
    m_physValMin(),
    m_physValMax(),
    m_physValRes()
{
    m_pLyt = new QHBoxLayout();
    m_pLyt->setContentsMargins(0,0,0,0);

    setLayout(m_pLyt);

    m_pEdt = new CDoubleSpinBox();

    m_pEdt->setMinimum(DBL_MIN);
    m_pEdt->setMaximum(DBL_MAX);
    m_pEdt->setDecimals(DBL_DIG);

    m_pLyt->addWidget(m_pEdt);

    QObject::connect(
        m_pEdt, &QDoubleSpinBox::editingFinished,
        this, &CWdgtEditPhysVal::onEdtEditingFinished);
    QObject::connect(
        m_pEdt, static_cast<void (QDoubleSpinBox::*)(const QString&)>(&QDoubleSpinBox::textChanged),
        this, static_cast<void (CWdgtEditPhysVal::*)(const QString&)>(&CWdgtEditPhysVal::onEdtTextChanged));
    QObject::connect(
        m_pEdt, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
        this, static_cast<void (CWdgtEditPhysVal::*)(double)>(&CWdgtEditPhysVal::onEdtValueChanged));

} // ctor

//------------------------------------------------------------------------------
CWdgtEditPhysVal::~CWdgtEditPhysVal()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    m_pEdt = nullptr;
    //m_unit;
    //m_physValOld;
    //m_physValMin;
    //m_physValMax;
    //m_physValRes;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::clear()
//------------------------------------------------------------------------------
{
    m_physVal = CPhysVal();
    m_physValMin = CPhysVal();
    m_physValMax = CPhysVal();
    m_physValRes = CPhysValRes();

    m_pEdt->setValue(0);

    m_pEdt->setMinimum(DBL_MIN);
    m_pEdt->setMaximum(DBL_MAX);
    m_pEdt->setDecimals(DBL_DIG);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setEnabled(bool i_bEnabled)
//------------------------------------------------------------------------------
{
    m_pEdt->setEnabled(i_bEnabled);
}

//------------------------------------------------------------------------------
bool CWdgtEditPhysVal::isEnabled() const
//------------------------------------------------------------------------------
{
    return m_pEdt->isEnabled();
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setReadOnly(bool i_bReadOnly)
//------------------------------------------------------------------------------
{
    m_pEdt->setReadOnly(i_bReadOnly);
}

//------------------------------------------------------------------------------
bool CWdgtEditPhysVal::isReadOnly() const
//------------------------------------------------------------------------------
{
    return m_pEdt->isReadOnly();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_physVal.setUnit(i_unit);
    m_physValRes.setUnit(i_unit);
    m_physValMin.setUnit(i_unit);
    m_physValMax.setUnit(i_unit);

    m_pEdt->setSuffix( " " + m_physVal.unit().symbol() );
}

//------------------------------------------------------------------------------
CUnit CWdgtEditPhysVal::unit() const
//------------------------------------------------------------------------------
{
    return m_physVal.unit();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setValue( double i_fVal )
//------------------------------------------------------------------------------
{
    m_physVal.setVal(i_fVal);
    m_pEdt->setValue(i_fVal);
}

//------------------------------------------------------------------------------
CPhysVal CWdgtEditPhysVal::value() const
//------------------------------------------------------------------------------
{
    return m_physVal;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setMinimum( double i_fVal )
//------------------------------------------------------------------------------
{
    m_physValMin.setVal(i_fVal);
    m_pEdt->setMinimum(i_fVal);
}

//------------------------------------------------------------------------------
CPhysVal CWdgtEditPhysVal::minimum() const
//------------------------------------------------------------------------------
{
    return m_physValMin;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setMaximum( double i_fVal )
//------------------------------------------------------------------------------
{
    m_physValMax.setVal(i_fVal);
    m_pEdt->setMaximum(i_fVal);
}

//------------------------------------------------------------------------------
CPhysVal CWdgtEditPhysVal::maximum() const
//------------------------------------------------------------------------------
{
    return m_physValMax;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setResolution( double i_fVal )
//------------------------------------------------------------------------------
{
    m_physValRes.setVal(i_fVal);

    int     iDecimals = DBL_DIG;
    double  fStep     = 1.0;
    QLocale locale(QLocale::C);
    QChar   chDecimalPoint = locale.decimalPoint();
    QString strVal;

    if( m_physValRes.isValid() ) {
        iDecimals = 0;

        if( m_physVal.isValid() ) {
            m_physVal.setRes(m_physValRes);

            try {
                strVal = m_physVal.toString(EUnitFind::None,PhysValSubStr::Val);
            }
            catch( CPhysValException& ) {
            }
        }
        else {
            try {
                strVal = m_physValRes.toString(EUnitFind::None,PhysValSubStr::Val);
            }
            catch( CPhysValException& ) {
            }
        }

        if( !strVal.isEmpty() ) {
            int iPosDecimalPoint = strVal.indexOf(chDecimalPoint);

            if( iPosDecimalPoint >= 0 ) {
                iDecimals = strVal.length() - iPosDecimalPoint - 1;
            }
        }
    }

    m_pEdt->setDecimals(iDecimals);
    m_pEdt->setSingleStep(fStep);

} // setResolution

//------------------------------------------------------------------------------
CPhysValRes CWdgtEditPhysVal::resolution() const
//------------------------------------------------------------------------------
{
    return m_physValRes;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtEditingFinished()
//------------------------------------------------------------------------------
{
    double fVal = m_pEdt->value();
    QString strSymbol = m_pEdt->suffix();
    if( strSymbol.startsWith(" ") ) {
        strSymbol.remove(0,1);
    }
    m_physVal = CPhysVal(fVal, strSymbol);
    emit editingFinished();
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    double fVal = m_pEdt->value();
    QString strSymbol = m_pEdt->suffix();
    if( strSymbol.startsWith(" ") ) {
        strSymbol.remove(0,1);
    }
    m_physVal = CPhysVal(fVal, strSymbol);
    emit valueChanged(m_physVal);
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtTextChanged( const QString& i_strText )
//------------------------------------------------------------------------------
{
    double fVal = m_pEdt->value();
    QString strSymbol = m_pEdt->suffix();
    if( strSymbol.startsWith(" ") ) {
        strSymbol.remove(0,1);
    }
    m_physVal = CPhysVal(fVal, strSymbol);
    emit textChanged(i_strText);
}
