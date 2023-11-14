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

#include <QtWidgets/qapplication.h>

#include <QtGui/qevent.h>
#if QT_VERSION < 0x050000
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qspinbox.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qstyle.h>
#endif

#include "ZSPhysValGUI/ZSPhysValEditWdgt.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysRefCountGuard.h"

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
public: // ctors and dtor
    CDoubleSpinBox( QWidget* i_pWdgtParent = nullptr ) :
        QDoubleSpinBox(i_pWdgtParent)
    {
    }
    virtual ~CDoubleSpinBox() {}
public: // instance methdos
    QLineEdit* lineEdit() { return QDoubleSpinBox::lineEdit(); }
public: // overridables of base class QDoubleSpinBox
    virtual QValidator::State validate(QString& i_strVal, int& i_iPos) const override {
        return QDoubleSpinBox::validate(i_strVal, i_iPos);
    }
    virtual void keyPressEvent(QKeyEvent* i_pEv) override {
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
CWdgtEditPhysVal::CWdgtEditPhysVal(const QString& i_strName, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_strName(i_strName),
    m_pLyt(nullptr),
    m_pEdt(nullptr),
    m_physVal(),
    m_physValMin(),
    m_physValMax(),
    m_physValRes(),
    m_iValueChangedSignalsBlocked(0)
{
    if (!m_strName.isEmpty()) {
        setObjectName(m_strName);
    }

    m_pLyt = new QHBoxLayout();
    m_pLyt->setContentsMargins(0, 0, 0, 0);

    setLayout(m_pLyt);

    m_pEdt = new CDoubleSpinBox();
    CDoubleSpinBox* pSpinBox = dynamic_cast<CDoubleSpinBox*>(m_pEdt);
    if (!m_strName.isEmpty()) {
        pSpinBox->setObjectName(i_strName);
        pSpinBox->lineEdit()->setObjectName(i_strName);
    }

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
    //m_strName;
    m_pLyt = nullptr;
    m_pEdt = nullptr;
    //m_unit;
    //m_physValOld;
    //m_physValMin;
    //m_physValMax;
    //m_physValRes;
    m_iValueChangedSignalsBlocked = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the current edit value, the minimum and the maximum and
           also the resolution.

    - The unit will be set to an invalid unit.
    - The current edit value will be reset to 0.
    - The minimum value is set to DBL_MIN (=2.2250738585072014e-308).
    - The maximum value is set to DBL_MAX (=1.7976931348623158e+308).
    - The decimals of the edit field (which correspond to the resolution)
      are set to DBL_DIG (=15).
*/
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
/*! @brief Installs the event filter.

    To filter mouse events the event filter must also be set at the spin box
    and the line edit of the spin box.
*/
void CWdgtEditPhysVal::installEventFilter(QObject* i_pObjEventWatcher)
//------------------------------------------------------------------------------
{
    CDoubleSpinBox* pSpinBox = dynamic_cast<CDoubleSpinBox*>(m_pEdt);
    pSpinBox->installEventFilter(i_pObjEventWatcher);
    pSpinBox->lineEdit()->installEventFilter(i_pObjEventWatcher);
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setName(const QString& i_strName)
//------------------------------------------------------------------------------
{
    m_strName = i_strName;
    setObjectName(i_strName);
    CDoubleSpinBox* pSpinBox = dynamic_cast<CDoubleSpinBox*>(m_pEdt);
    pSpinBox->setObjectName(i_strName);
    pSpinBox->lineEdit()->setObjectName(i_strName);
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setEnabled(bool i_bEnabled)
//------------------------------------------------------------------------------
{
    QWidget::setEnabled(i_bEnabled);
    CDoubleSpinBox* pSpinBox = dynamic_cast<CDoubleSpinBox*>(m_pEdt);
    pSpinBox->setEnabled(i_bEnabled);
    pSpinBox->lineEdit()->setEnabled(i_bEnabled);
}

//------------------------------------------------------------------------------
bool CWdgtEditPhysVal::isEnabled() const
//------------------------------------------------------------------------------
{
    CDoubleSpinBox* pSpinBox = dynamic_cast<CDoubleSpinBox*>(m_pEdt);
    return pSpinBox->isEnabled();
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::setReadOnly(bool i_bReadOnly)
//------------------------------------------------------------------------------
{
    CDoubleSpinBox* pSpinBox = dynamic_cast<CDoubleSpinBox*>(m_pEdt);
    pSpinBox->setReadOnly(i_bReadOnly);
    pSpinBox->lineEdit()->setReadOnly(i_bReadOnly);
}

//------------------------------------------------------------------------------
bool CWdgtEditPhysVal::isReadOnly() const
//------------------------------------------------------------------------------
{
    CDoubleSpinBox* pSpinBox = dynamic_cast<CDoubleSpinBox*>(m_pEdt);
    return pSpinBox->isReadOnly();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the unit in which the value should be indicated and edited.

    The units of the current value, the minimum and maximum value as well as
    the resolution will be set to the given unit.

    If already set and valid, the current edit value, the minimum, maximum and
    resolution values will be converted into the new unit.

    E.g. if before the value "0.2 mm" was set and you change the unit to "m"
    the current edit value will be converted into "0.0002 m".
    Same applies to the minimum and maximum value and the resolution.

    @param [in] i_unit Unit to be used.
*/
void CWdgtEditPhysVal::setUnit(const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    // setUnit is not implicitly called. The reference counter must be 0.
    if (m_iValueChangedSignalsBlocked != 0) {
        throw CException(
            __FILE__, __LINE__, EResultInternalProgramError,
            "m_iValueChangedSignalsBlocked not 0");
    }

    QString strTextPrev = m_pEdt->text();
    double fValPrev = m_pEdt->value();

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    // Before setting the value the constraints are set.
    // If the aggregated double spin box intermediately corrects
    // the value the cached value is not affected as processing
    // the valueChanged and textChanged signals is suppressed
    // by setting the ValueChangedSignalsBlocked flag.

    if (m_physVal.isValid()) {
        if (areOfSameUnitGroup(m_physVal.unit(), i_unit)) {
            m_physVal.convertValue(i_unit);
        }
        else {
            m_physVal = CPhysVal(i_unit);
        }
    }
    else {
        m_physVal.setUnit(i_unit);
    }

    m_pEdt->setSuffix(" " + m_physVal.unit().symbol());

    if (m_physValMin.isValid()) {
        if (areOfSameUnitGroup(m_physValMin.unit(), i_unit)) {
            m_physValMin.convertValue(i_unit);
            setMinimum(m_physValMin.getVal());
        }
        else {
            m_physValMin = CPhysVal(i_unit);
        }
    }
    else {
        m_physValMin.setUnit(i_unit);
    }

    if (m_physValMax.isValid()) {
        if (areOfSameUnitGroup(m_physValMax.unit(), i_unit)) {
            m_physValMax.convertValue(i_unit);
            setMaximum(m_physValMax.getVal());
        }
        else {
            m_physValMax = CPhysVal(i_unit);
        }
    }
    else {
        m_physValMax.setUnit(i_unit);
    }

    if (m_physValRes.isValid()) {
        if (areOfSameUnitGroup(m_physValRes.unit(), i_unit)) {
            m_physValRes.convertValue(i_unit);
            setResolution(m_physValRes.getVal());
        }
        else {
            m_physValRes = CPhysValRes(i_unit);
        }
    }
    else {
        m_physValRes.setUnit(i_unit);
    }

    if (m_physVal.isValid()) {
        setValue(m_physVal.getVal());
    }

    if (m_iValueChangedSignalsBlocked == 0) {
        throw CException(
            __FILE__, __LINE__, EResultInternalProgramError,
            "m_iValueChangedSignalsBlocked already 0");
    }

    refCountGuard.decrementAndReleaseCounter();

   if (m_iValueChangedSignalsBlocked == 0) {
        // Processing the signals were block. Now emit the signals
        // if the text and/or the values were changed.
        if (strTextPrev != m_pEdt->text()) {
            emit textChanged(m_pEdt->text());
        }
        if (fValPrev != m_pEdt->value()) {
            emit valueChanged(m_physVal);
        }
    }
} // setUnit

//------------------------------------------------------------------------------
/*! @brief Returns the current unit the value is indicated and edited.

    @return Unit of the current value.
*/
CUnit CWdgtEditPhysVal::unit() const
//------------------------------------------------------------------------------
{
    return m_physVal.unit();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the minimum value for the value to be edited.

    Note: The resolution, the maximum value and the current edit value
          value might change as a result of changing this property.

    Please note that on setting the minimum value the current value might be
    corrected to fit the new constraint. If a value has not been set before
    and the current value of the double spin box (0) is lower than the
    minimum value the current edit value will be implicitly set to fit the
    new constraint given by the minimum value. The edit value becomes valid.
    In this case the aggregated spin box emits the "valueChanged" and
    "textChanged" signals and the slots "onValueChanged" and "onTextChanged"
    will implicitly set the minimum value as the new current value.

    Please also note that when also using a resolution (which implicitly defines
    the number of decimal digits to be edited) both the minimum value and the
    resolution should be the same. It doesn't make sense to set the minimum
    value to "0.01" and the resolution to "0.1" as in that case the value "0.02"
    will be rounded to "0.0". If you retrieve the value later on the value
    will be less than the defined minimum value.

    So you should either use a minimum value or a resolution but not both
    at the same time.

    @param [in] i_fVal Minimum value.
        The minimum value must be converted before into the
        unit of the edit field which must have been set before.
*/
void CWdgtEditPhysVal::setMinimum( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strTextPrev = m_pEdt->text();
    double fValPrev = m_pEdt->value();

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    // Before setting the value the constraints are set.
    // If the aggregated double spin box intermediately corrects
    // the value the cached value is not affected as processing
    // the valueChanged and textChanged signals is suppressed
    // by setting the ValueChangedSignalsBlocked flag.

    m_physValMin.setVal(i_fVal);
    if( m_physValMax.isValid() ) {
        if( m_physValMax.getVal() < m_physValMin.getVal()) {
            setMaximum(m_physValMin.getVal());
        }
    }
    if( m_physValRes.isValid() ) {
        if( m_physValRes.getVal() < m_physValMin.getVal()) {
            setResolution(m_physValMin.getVal());
        }
    }

    m_pEdt->setMinimum(i_fVal);

    if( m_iValueChangedSignalsBlocked == 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultInternalProgramError,
            "m_iValueChangedSignalsBlocked already 0");
    }

    refCountGuard.decrementAndReleaseCounter();

    if( m_iValueChangedSignalsBlocked == 0 ) {
        // Processing the signals were block. Now emit the signals
        // if the text and/or the values were changed.
        if( strTextPrev != m_pEdt->text() ) {
            emit textChanged(m_pEdt->text());
        }
        if( fValPrev != m_pEdt->value() ) {
            emit valueChanged(m_physVal);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum value set at the edit field.

    @return Minimum value in the unit set at the edit field.
*/
CPhysVal CWdgtEditPhysVal::minimum() const
//------------------------------------------------------------------------------
{
    return m_physValMin;
}

//------------------------------------------------------------------------------
/*! @brief Sets the maximum value for the value to be edited.

    Note: The resolution, the minimum value and the current edit value
          value might change as a result of changing this property.

    Please note that on setting the maximum value the current value might be
    corrected to fit the new constraint. If a value has not been set before
    and the current value of the double spin box (0) is greater than the
    maximum value the current edit value will be implicitly set to fit the
    new constraint given by the maximum value. The edit value becomes valid.
    In this case the aggregated spin box emits the "valueChanged" and
    "textChanged" signals and the slots "onValueChanged" and "onTextChanged"
    will implicitly set the maximum value as the new current value.

    @param [in] i_fVal Maximum value.
        The maximum value must be converted before into the
        unit of the edit field which must have been set before.
*/
void CWdgtEditPhysVal::setMaximum( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strTextPrev = m_pEdt->text();
    double fValPrev = m_pEdt->value();

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    // Before setting the value the constraints are set.
    // If the aggregated double spin box intermediately corrects
    // the value the cached value is not affected as processing
    // the valueChanged and textChanged signals is suppressed
    // by setting the ValueChangedSignalsBlocked flag.

    m_physValMax.setVal(i_fVal);
    if( m_physValMin.isValid() ) {
        if( m_physValMin.getVal() > m_physValMax.getVal()) {
            setMinimum(m_physValMax.getVal());
        }
    }
    else if( m_physValRes.isValid() ) {
        if( m_physValRes.getVal() > m_physValMax.getVal()) {
            setResolution(m_physValMax.getVal());
        }
    }
    m_pEdt->setMaximum(i_fVal);

    if( m_iValueChangedSignalsBlocked == 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultInternalProgramError,
            "m_iValueChangedSignalsBlocked already 0");
    }

    refCountGuard.decrementAndReleaseCounter();

    if( m_iValueChangedSignalsBlocked == 0 ) {
        // Processing the signals were block. Now emit the signals
        // if the text and/or the values were changed.
        if( strTextPrev != m_pEdt->text() ) {
            emit textChanged(m_pEdt->text());
        }
        if( fValPrev != m_pEdt->value() ) {
            emit valueChanged(m_physVal);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum value set at the edit field.

    @return Maximum value in the unit set at the edit field.
*/
CPhysVal CWdgtEditPhysVal::maximum() const
//------------------------------------------------------------------------------
{
    return m_physValMax;
}

//------------------------------------------------------------------------------
/*! @brief Sets the resolution of the value to be edited.

    The resolution defines the number of digits which are shown and can be
    changed by the edit field.

    Please also note that when also using a minimum value (which also implicitly
    defines the number of decimal digits to be edited) both the minimum value and
    the resolution should be the same. It doesn't make sense to set the minimum
    value to "0.01" and the resolution to "0.1" as in that case the value "0.02"
    will be rounded to "0.0". If you retrieve the value later on the value
    will be less than the defined minimum value.

    So you should either use a minimum value or a resolution but not both.

    @param [in] i_fVal
        Resolution value defining the number of valid decimals.
*/
void CWdgtEditPhysVal::setResolution( double i_fVal )
//------------------------------------------------------------------------------
{
    QString strTextPrev = m_pEdt->text();
    double fValPrev = m_pEdt->value();

    CRefCountGuard refCountGuard(&m_iValueChangedSignalsBlocked);

    // Before setting the value the constraints are set.
    // If the aggregated double spin box intermediately corrects
    // the value the cached value is not affected as processing
    // the valueChanged and textChanged signals is suppressed
    // by setting the ValueChangedSignalsBlocked flag.

    m_physValRes.setVal(i_fVal);

    if( m_physValMin.isValid() ) {
        if( m_physValMin.getVal() > m_physValRes.getVal()) {
            setMinimum(m_physValRes.getVal());
        }
    }

    int iDecimals = DBL_DIG;
    double fStep = 1.0;

    if( m_physValRes.isValid() ) {
        QString strVal;
        try {
            strVal = m_physValRes.toString(EUnitFind::None, PhysValSubStr::Val);
        }
        catch( CPhysValException& ) {
        }
        if( !strVal.isEmpty() ) {
            QLocale locale(QLocale::C);
            QChar chDecimalPoint = locale.decimalPoint();
            int iPosDecimalPoint = strVal.indexOf(chDecimalPoint);
            if( iPosDecimalPoint >= 0 ) {
                iDecimals = strVal.length() - iPosDecimalPoint - 1;
            }
            else {
                iDecimals = 0;
            }
        }
        fStep = m_physValRes.getVal();
    }

    m_pEdt->setDecimals(iDecimals);
    m_pEdt->setSingleStep(fStep);

    if( m_iValueChangedSignalsBlocked == 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultInternalProgramError,
            "m_iValueChangedSignalsBlocked already 0");
    }

    refCountGuard.decrementAndReleaseCounter();

    if( m_iValueChangedSignalsBlocked == 0 ) {
        // Processing the signals were block. Now emit the signals
        // if the text and/or the values were changed.
        if( strTextPrev != m_pEdt->text() ) {
            emit textChanged(m_pEdt->text());
        }
        if( fValPrev != m_pEdt->value() ) {
            emit valueChanged(m_physVal);
        }
    }
} // setResolution

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of the value to be edited.

    @return Resolution of the edited value.
*/
CPhysValRes CWdgtEditPhysVal::resolution() const
//------------------------------------------------------------------------------
{
    return m_physValRes;
}

//------------------------------------------------------------------------------
/*! @brief Sets how many decimals the spinbox will use for displaying and
           interpreting doubles.

    Use either "setResolution" or "setDecimals" but not both as the settings
    will overwrite each other.

    Note: The maximum, minimum and value might change as a result of changing
    this property.

    @param [in] i_iPrecision
        Number of decimals to be used.
*/
void CWdgtEditPhysVal::setDecimals( int i_iPrecision )
//------------------------------------------------------------------------------
{
    m_pEdt->setDecimals(i_iPrecision);
}

//------------------------------------------------------------------------------
/*! @brief Returns the number of decimals the spinbox uses for displaying and
           interpreting doubles.

    @return Number of decimals.
*/
int CWdgtEditPhysVal::decimals() const
//------------------------------------------------------------------------------
{
    return m_pEdt->decimals();
}

//------------------------------------------------------------------------------
/*! @brief Sets the value to increment/decrement the spin box's value using
           the arrow keys.

    Use either "setResolution" or "setSingleStep" but not both as the settings
    will overwrite each other.

    When the user uses the arrows to change the spin box's value the value will
    be incremented/decremented by the amount of the singleStep.
    The default value is 1.0. Setting a singleStep value of less than 0 does nothing.

    @param [in] i_fVal
        Step value to be used.
*/
void CWdgtEditPhysVal::setSingleStep( double i_fVal )
//------------------------------------------------------------------------------
{
    m_pEdt->setSingleStep(i_fVal);
}

//------------------------------------------------------------------------------
/*! @brief Returns the step value to be used to increment/decrement the
           value by arrow keys.

    @return Step value used.
*/
double CWdgtEditPhysVal::singleStep() const
//------------------------------------------------------------------------------
{
    return m_pEdt->singleStep();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the current value to be edited.

    If a minimum or maximum value has been set before the value may be corrected
    to fit those constraings.

    If a resolution has been set the value may be rounded to also fit this constraint.

    @param [in] i_fVal Value to be indicated and edited.
*/
void CWdgtEditPhysVal::setValue( double i_fVal )
//------------------------------------------------------------------------------
{
    m_physVal.setVal(i_fVal);
    m_pEdt->setValue(i_fVal);
}

//------------------------------------------------------------------------------
/*! @brief Returns the current value with its unit.

    @return Current value.
*/
CPhysVal CWdgtEditPhysVal::value() const
//------------------------------------------------------------------------------
{
    return m_physVal;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtEditingFinished()
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked == 0 ) {
        double fVal = m_pEdt->value();
        QString strSymbol = m_pEdt->suffix();
        if( strSymbol.startsWith(" ") ) {
            strSymbol.remove(0, 1);
        }
        m_physVal = CPhysVal(fVal, strSymbol);
        emit editingFinished();
    }
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtValueChanged( double i_fVal )
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked == 0 ) {
        double fVal = m_pEdt->value();
        QString strSymbol = m_pEdt->suffix();
        if( strSymbol.startsWith(" ") ) {
            strSymbol.remove(0, 1);
        }
        m_physVal = CPhysVal(fVal, strSymbol);
        emit valueChanged(m_physVal);
    }
}

//------------------------------------------------------------------------------
void CWdgtEditPhysVal::onEdtTextChanged( const QString& i_strText )
//------------------------------------------------------------------------------
{
    if( m_iValueChangedSignalsBlocked == 0 ) {
        double fVal = m_pEdt->value();
        QString strSymbol = m_pEdt->suffix();
        if( strSymbol.startsWith(" ") ) {
            strSymbol.remove(0, 1);
        }
        m_physVal = CPhysVal(fVal, strSymbol);
        emit textChanged(i_strText);
    }
}
