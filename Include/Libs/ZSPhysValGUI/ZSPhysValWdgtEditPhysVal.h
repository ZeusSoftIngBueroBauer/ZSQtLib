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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"

class QHBoxLayout;

namespace ZS
{
namespace PhysVal
{
namespace GUI
{
class CDoubleSpinBox;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtEditPhysVal : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtEditPhysVal"; }
public: // ctors and dtor
    CWdgtEditPhysVal( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtEditPhysVal();
signals:
    void editingFinished();
    void valueChanged( const ZS::PhysVal::CPhysVal& i_val );
    void textChanged( const QString& i_strVal );
public: // instance methods
    void clear();
public: // instance methods
    void setEnabled(bool i_bEnabled);
    bool isEnabled() const;
    void setReadOnly(bool i_bReadOnly);
    bool isReadOnly() const;
public: // instance methods
    void setUnit( const CUnit& i_unit );
    CUnit unit() const;
public: // instance methods
    void setValue( double i_fVal );
    CPhysVal value() const;
public: // instance methods
    void setMinimum( double i_fVal );
    CPhysVal minimum() const;
public: // instance methods
    void setMaximum( double i_fVal );
    CPhysVal maximum() const;
public: // instance methods
    void setResolution( double i_fVal );
    CPhysValRes resolution() const;
protected slots:
    void onEdtEditingFinished();
    void onEdtValueChanged( double i_fVal );
    void onEdtTextChanged( const QString& i_strText );
private: // instance members
    QHBoxLayout* m_pLyt;
    CDoubleSpinBox* m_pEdt;
    CPhysVal m_physVal;
    CPhysVal m_physValMin;
    CPhysVal m_physValMax;
    CPhysValRes m_physValRes;

}; // class CWdgtEditPhysVal

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_WdgtEditPhysVal_h
