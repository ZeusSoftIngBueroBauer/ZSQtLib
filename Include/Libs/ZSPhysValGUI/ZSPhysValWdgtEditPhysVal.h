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
public: // ctors and dtor
    CWdgtEditPhysVal( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtEditPhysVal();
signals:
    void editingFinished();
    void valueChanged( const ZS::PhysVal::CPhysVal& i_val );
    void valueChanged( const QString& i_strVal );
public: // instance methods
    void clear();
public: // instance methods
    void setUnitGroup( PhysVal::CUnitGrp* i_pUnitGrp );
    PhysVal::CUnitGrp* unitGroup() const { return m_pUnitGrp; }
    void setUnit( PhysVal::CUnit* i_pUnit );
    PhysVal::CUnit* getUnit() const { return m_pUnit; }
public: // instance methods
    void setValue( const PhysVal::CPhysVal& i_physVal );
    PhysVal::CPhysVal value() const;
public: // instance methods
    void setMinimum( const PhysVal::CPhysVal& i_physValMin );
    PhysVal::CPhysVal minimum() const { return m_physValMin; }
public: // instance methods
    void setMaximum( const PhysVal::CPhysVal& i_physValMax );
    PhysVal::CPhysVal maximum() const { return m_physValMax; }
public: // instance methods
    void setResolution( const PhysVal::CPhysValRes& i_physValRes );
    PhysVal::CPhysValRes resolution() const { return m_physValRes; }
protected slots:
    void onEdtEditingFinished();
    void onEdtValueChanged( double i_fVal );
    void onEdtValueChanged( const QString& i_strVal );
private: // instance members
    QHBoxLayout*         m_pLyt;
    CDoubleSpinBox*      m_pEdt;
    PhysVal::CUnitGrp*   m_pUnitGrp;
    PhysVal::CUnit*      m_pUnit;
    PhysVal::CPhysVal    m_physValOld;
    PhysVal::CPhysVal    m_physValMin;
    PhysVal::CPhysVal    m_physValMax;
    PhysVal::CPhysValRes m_physValRes;

}; // class CWdgtEditPhysVal

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_WdgtEditPhysVal_h
