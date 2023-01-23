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
    m_unit(),
    m_physValOld(),
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

    if( !QObject::connect(
        /* pObjSender   */ m_pEdt,
        /* szSignal     */ SIGNAL( editingFinished() ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onEdtEditingFinished() ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pEdt,
        /* szSignal     */ SIGNAL( valueChanged(double) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onEdtValueChanged(double) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pEdt,
        /* szSignal     */ SIGNAL( valueChanged(const QString&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onEdtValueChanged(const QString&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

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
    m_physValOld = CPhysVal();
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
void CWdgtEditPhysVal::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( m_unit != i_unit )
    {
        m_unit = i_unit;

        if( !m_unit.isValid() )
        {
            m_pEdt->setSuffix("");
        }
        else
        {
            m_pEdt->setSuffix( " " + m_unit.symbol() );
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setValue( const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    if( areOfSameUnitGroup(m_unit, i_physVal.unit()) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultDifferentPhysSizes );
    }
    if( !m_unit.isValid() && i_physVal.unit().isValid() )
    {
        setUnit( i_physVal.unit() );
    }

    m_physValOld = i_physVal;

    if( m_physValOld.unit() != m_unit )
    {
        m_physValOld.convertValue(m_unit);
    }

    m_pEdt->setValue( m_physValOld.getVal() );

    if( m_physValRes.isValid() )
    {
        setResolution(m_physValRes);
    }
    else if( m_physValOld.hasRes() )
    {
        setResolution( m_physValOld.getRes() );
    }
} // setValue

//------------------------------------------------------------------------------
CPhysVal CWdgtEditPhysVal::value() const
//------------------------------------------------------------------------------
{
    double fVal = m_pEdt->value();
    QString strSymbol = m_pEdt->suffix();

    if( strSymbol.startsWith(" ") )
    {
        strSymbol.remove(0,1);
    }
    CUnit unit(m_unit);
    unit.setSymbol(strSymbol);
    return CPhysVal(fVal, unit);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setMinimum( const CPhysVal& i_physValMin )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValMin.unit()) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultDifferentPhysSizes );
    }
    if( !m_unit.isValid() && i_physValMin.unit().isValid() )
    {
        setUnit(i_physValMin.unit());
    }

    m_physValMin = i_physValMin;

    if( m_physValMin.isValid() && !m_physValMin.isNull() )
    {
        double fValMin = m_physValMin.getVal(m_unit);
        m_pEdt->setMinimum(fValMin);
    }
    else
    {
        m_pEdt->setMinimum(DBL_MIN);
    }
} // setMinimum

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setMaximum( const CPhysVal& i_physValMax )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValMax.unit()) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultDifferentPhysSizes );
    }
    if( !m_unit.isValid() && i_physValMax.unit().isValid() )
    {
        setUnit( i_physValMax.unit() );
    }

    m_physValMax = i_physValMax;

    if( m_physValMax.isValid() && !m_physValMax.isNull() )
    {
        double fValMax = m_physValMax.getVal(m_unit);
        m_pEdt->setMaximum(fValMax);
    }
    else
    {
        m_pEdt->setMaximum(DBL_MAX);
    }
} // setMaximum

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setResolution( const CPhysValRes& i_physValRes )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_physValRes.unit()) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultDifferentPhysSizes );
    }
    if( !m_unit.isValid() && i_physValRes.unit().isValid() )
    {
        setUnit( i_physValRes.unit() );
    }

    m_physValRes = i_physValRes;

    int     iDecimals = DBL_DIG;
    double  fStep     = 1.0;
    QLocale locale(QLocale::C);
    QChar   chDecimalPoint = locale.decimalPoint();
    QString strVal;

    if( m_physValRes.isValid() )
    {
        iDecimals = 0;

        if( m_physValOld.isValid() )
        {
            m_physValOld.setRes(m_physValRes);

            try
            {
                strVal = m_physValOld.toString(EUnitFind::None,PhysValSubStr::Val);
            }
            catch( CPhysValException& )
            {
            }
        }
        else
        {
            try
            {
                strVal = m_physValRes.toString(EUnitFind::None,PhysValSubStr::Val);
            }
            catch( CPhysValException& )
            {
            }
        }

        if( !strVal.isEmpty() )
        {
            int iPosDecimalPoint = strVal.indexOf(chDecimalPoint);

            if( iPosDecimalPoint >= 0 )
            {
                iDecimals = strVal.length() - iPosDecimalPoint - 1;
            }
        }
    }

    m_pEdt->setDecimals(iDecimals);
    m_pEdt->setSingleStep(fStep);

} // setResolution

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtEditingFinished()
//------------------------------------------------------------------------------
{
    emit editingFinished();
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    CPhysVal physVal(i_fVal, m_unit);
    emit valueChanged(physVal);
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtValueChanged( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    emit valueChanged(i_strVal);
}
