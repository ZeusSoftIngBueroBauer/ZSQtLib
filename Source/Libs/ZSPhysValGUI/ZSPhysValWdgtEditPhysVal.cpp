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

#include <float.h>

#include <QtCore/qcoreapplication.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qspinbox.h>
#endif

#include "ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h"
#include "ZSPhysVal/ZSPhysSize.h"
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
    //QLineEdit* pLineEdit = lineEdit();

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
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
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
    m_pUnitGrp = nullptr;
    m_pUnit = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::clear()
//------------------------------------------------------------------------------
{
    setUnitGroup(nullptr);

    m_physValOld = CPhysVal();
    m_physValMin = CPhysVal();
    m_physValMax = CPhysVal();
    m_physValRes = CPhysValRes();

    m_pEdt->setValue(0);

    m_pEdt->setMinimum(DBL_MIN);
    m_pEdt->setMaximum(DBL_MAX);
    m_pEdt->setDecimals(DBL_DIG);

} // clear

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setUnitGroup( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_pUnitGrp )
    {
        m_pUnitGrp = i_pUnitGrp;

        setUnit(nullptr);
    }

} // setUnitGroup

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setUnit( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    if( m_pUnit != i_pUnit )
    {
        m_pUnit = i_pUnit;

        if( m_pUnit == nullptr )
        {
            m_pUnitGrp = nullptr;
            m_pEdt->setSuffix("");
        }
        else
        {
            m_pUnitGrp = m_pUnit->getUnitGroup();
            m_pEdt->setSuffix( " " + m_pUnit->getSymbol() );
        }
    } // if( m_pUnit != i_pUnit )

} // setUnit

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setValue( const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp != i_physVal.getUnitGroup() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultDifferentPhysSizes );
    }
    if( m_pUnitGrp == nullptr && i_physVal.getUnitGroup() != nullptr )
    {
        m_pUnitGrp = i_physVal.getUnitGroup();
    }
    if( m_pUnit == nullptr && i_physVal.getUnit() != nullptr )
    {
        setUnit( i_physVal.getUnit() );
    }

    m_physValOld = i_physVal;

    if( m_physValOld.getUnit() != m_pUnit )
    {
        m_physValOld.convertValue(m_pUnit);
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
    double  fVal      = m_pEdt->value();
    QString strSymbol = m_pEdt->suffix();
    CUnit*  pUnit     = nullptr;

    if( m_pUnitGrp != nullptr )
    {
        if( strSymbol.startsWith(" ") )
        {
            strSymbol.remove(0,1);
        }
        pUnit = m_pUnitGrp->findUnitBySymbol(strSymbol);
    }

    return CPhysVal( fVal, pUnit );

} // value

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setMinimum( const CPhysVal& i_physValMin )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp != i_physValMin.getUnitGroup() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultDifferentPhysSizes );
    }
    if( m_pUnitGrp == nullptr && i_physValMin.getUnitGroup() != nullptr )
    {
        m_pUnitGrp = i_physValMin.getUnitGroup();
    }
    if( m_pUnit == nullptr && i_physValMin.getUnit() != nullptr )
    {
        setUnit( i_physValMin.getUnit() );
    }

    m_physValMin = i_physValMin;

    if( m_physValMin.isValid() && !m_physValMin.isNull() )
    {
        double fValMin = m_physValMin.getVal(m_pUnit);
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
    if( m_pUnitGrp != nullptr && m_pUnitGrp != i_physValMax.getUnitGroup() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultDifferentPhysSizes );
    }
    if( m_pUnitGrp == nullptr && i_physValMax.getUnitGroup() != nullptr )
    {
        m_pUnitGrp = i_physValMax.getUnitGroup();
    }
    if( m_pUnit == nullptr && i_physValMax.getUnit() != nullptr )
    {
        setUnit( i_physValMax.getUnit() );
    }

    m_physValMax = i_physValMax;

    if( m_physValMax.isValid() && !m_physValMax.isNull() )
    {
        double fValMax = m_physValMax.getVal(m_pUnit);
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
    if( i_physValRes.getUnitGroup() != nullptr )
    {
        if( m_pUnitGrp != nullptr && m_pUnitGrp != i_physValRes.getUnitGroup() )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultDifferentPhysSizes );
        }
    }
    if( m_pUnitGrp == nullptr && i_physValRes.getUnitGroup() != nullptr )
    {
        m_pUnitGrp = i_physValRes.getUnitGroup();
    }
    if( m_pUnit == nullptr && i_physValRes.getUnit() != nullptr )
    {
        setUnit( i_physValRes.getUnit() );
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
                strVal = m_physValOld.toString(EUnitFindNone,EPhysValSubStrVal);
            }
            catch( CPhysValException& )
            {
            }
        }
        else
        {
            try
            {
                strVal = m_physValRes.toString(EUnitFindNone,EPhysValSubStrVal);
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
    CPhysVal physVal(i_fVal,m_pUnit);

    emit valueChanged(physVal);

} // onEdtValueChanged

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtValueChanged( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    emit valueChanged(i_strVal);
}
