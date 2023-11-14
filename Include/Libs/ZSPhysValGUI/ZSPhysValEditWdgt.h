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

#ifndef ZSPhysValGUI_WdgtEditPhysVal_h
#define ZSPhysValGUI_WdgtEditPhysVal_h

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QDoubleSpinBox;
class QHBoxLayout;

namespace ZS
{
namespace PhysVal
{
namespace GUI
{
class CDoubleSpinBox;

//******************************************************************************
/*! @brief Wrapper around a double spin box to edit a physical value indicating
           the symbol of the unit as a suffix.

    @Note Setting the minimum value may change the maximum value and the resolution
          and may correct the value to fit the currently set constraints. Same
          applies when changing the maximum value or the resolution value whereupon
          the other properties constraining the value to be edited may be adjusted.
*/
class ZSPHYSVALGUIDLL_API CWdgtEditPhysVal : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtEditPhysVal"; }
public: // ctors and dtor
    CWdgtEditPhysVal(const QString& i_strName = "", QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtEditPhysVal();
signals:
    void editingFinished();
    void valueChanged(const ZS::PhysVal::CPhysVal& i_val);
    void textChanged(const QString& i_strVal);
public: // instance methods
    void clear();
public: // instance methods
    void installEventFilter(QObject* i_pObjEventWatcher);
    void setName(const QString& i_strName);
    void setEnabled(bool i_bEnabled);
    bool isEnabled() const;
    void setReadOnly(bool i_bReadOnly);
    bool isReadOnly() const;
public: // instance methods
    void setUnit( const CUnit& i_unit );
    CUnit unit() const;
public: // instance methods
    void setMinimum(double i_fVal);
    CPhysVal minimum() const;
    void setMaximum(double i_fVal);
    CPhysVal maximum() const;
    void setResolution(double i_fVal);
    CPhysValRes resolution() const;
    void setDecimals(int i_iPrecision);
    int decimals() const;
    void setSingleStep(double i_fVal);
    double singleStep() const;
public: // instance methods
    void setValue(double i_fVal);
    CPhysVal value() const;
protected slots:
    void onEdtEditingFinished();
    void onEdtValueChanged(double i_fVal);
    void onEdtTextChanged(const QString& i_strText);
private: // instance members
    /*!< Name which can be set for debugging purposes. */
    QString m_strName;
    /*!< The layout of the widget. */
    QHBoxLayout* m_pLyt;
    /*!< The value is edited by the double spin box.
         The suffix is set to the symbol of the unit to be used. */
    QDoubleSpinBox* m_pEdt;
    /*!< The cached current value. */
    CPhysVal m_physVal;
    /*!< The cached mimimum value. */
    CPhysVal m_physValMin;
    /*!< The cached maximum value. */
    CPhysVal m_physValMax;
    /*!< The cached resolution used. */
    CPhysValRes m_physValRes;
    /*!< When changing the unit, the minimum, the maximum or the resolution
         the value may be corrected by the aggregated double spin box
         several times to fit the constraints. To avoid that the "valueChanged"
         and the "textChanged" signals are emitted several times and with
         intermediate values which will be corrected afterwards the flag blocks
         the signals while adjusting the edit field to the new constraints.
         In addition the flag blocks adjusting the cached value if the signals
         are emitted by the aggregated spin box and will be adjusted after
         everything is setup correctly.
         As the methods may be recursively called the flag is realized as
         a counter counting the number of blocks. */
    int m_iValueChangedSignalsBlocked;

}; // class CWdgtEditPhysVal

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_WdgtEditPhysVal_h
