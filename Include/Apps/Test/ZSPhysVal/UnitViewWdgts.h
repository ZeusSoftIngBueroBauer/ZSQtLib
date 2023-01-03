/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestPhysVal_UnitViewWdgts_h
#define ZSApps_TestPhysVal_UnitViewWdgts_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "UnitModels.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QSpacerItem;
class QSplitter;
class QTableView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace PhysVal
{
//******************************************************************************
class CWdgtUnitView : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitView( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitView();
public: // overridables
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected: // instance members
    SUnitsModelNode* m_pNode;
    int              m_cxLblMinWidth;
    int              m_cxEdtMinWidth;
    int              m_cxEdtMaxWidth;
    QVBoxLayout*     m_pLyt;
    QHBoxLayout*     m_pLytPath;
    QLabel*          m_pLblPath;
    QLineEdit*       m_pEdtPath;
    QHBoxLayout*     m_pLytName;
    QLabel*          m_pLblName;
    QLineEdit*       m_pEdtName;
    QHBoxLayout*     m_pLytKey;
    QLabel*          m_pLblKey;
    QLineEdit*       m_pEdtKey;

}; // class CWdgtUnitView

//******************************************************************************
class CWdgtUnitViewUndefined : public CWdgtUnitView
//******************************************************************************
{
public: // ctors and dtor
    CWdgtUnitViewUndefined( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewUndefined();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected: // instance members
    QLabel*  m_pLblPxmZeusSoft;

}; // class CWdgtUnitViewUndefined

//******************************************************************************
class CWdgtUnitViewGrp : public CWdgtUnitView
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewGrp( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewGrp();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );

}; // class CWdgtUnitViewGrp

//******************************************************************************
class CWdgtUnitViewUnt : public CWdgtUnitView
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewUnt( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewUnt();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected: // instance members
    QHBoxLayout* m_pLytSymbol;
    QLabel*      m_pLblSymbol;
    QLineEdit*   m_pEdtSymbol;
    QHBoxLayout* m_pLytNextLower;
    QLabel*      m_pLblNextLower;
    QLineEdit*   m_pEdtNextLower;
    QHBoxLayout* m_pLytNextHigher;
    QLabel*      m_pLblNextHigher;
    QLineEdit*   m_pEdtNextHigher;

}; // class CWdgtUnitViewUnt

//******************************************************************************
class CWdgtUnitViewGrpRatios : public CWdgtUnitViewGrp
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewGrpRatios( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewGrpRatios();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected: // instance members
    // Table view with physical units of a physical size:
    QVBoxLayout*        m_pLytTableView;
    QWidget*            m_pWdgtTableView;
    CModelUnitGrpRatio  m_model;
    QTableView*         m_pTableView;

}; // class CWdgtUnitViewGrpRatios

//******************************************************************************
class CWdgtUnitViewGrpDataQuantities : public CWdgtUnitViewGrp
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewGrpDataQuantities( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewGrpDataQuantities();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected: // instance members
    // Table view with physical units of a physical size:
    QVBoxLayout*              m_pLytTableView;
    QWidget*                  m_pWdgtTableView;
    CModelUnitGrpDataQuantity m_model;
    QTableView*               m_pTableView;

}; // class CWdgtUnitViewGrpDataQuantities

//******************************************************************************
class CWdgtUnitViewGrpPhysScienceFields : public CWdgtUnitViewGrp
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewGrpPhysScienceFields( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewGrpPhysScienceFields();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected slots:
    void viewModeChanged( const QString& i_strCurrentText );
protected: // instance methods
    void updateTableView();
protected: // instance members
    QHBoxLayout*    m_pLytSIUnit;
    QLabel*         m_pLblSIUnit;
    QLineEdit*      m_pEdtSIUnit;
    QHBoxLayout*    m_pLytFormulaSymbol;
    QLabel*         m_pLblFormulaSymbol;
    QLineEdit*      m_pEdtFormulaSymbol;
    QHBoxLayout*    m_pLytIsPowerRelated;
    QLabel*         m_pLblIsPowerRelated;
    QCheckBox*      m_pChkIsPowerRelated;
    QHBoxLayout*    m_pLytViewMode;
    QLabel*         m_pLblViewMode;
    QComboBox*      m_pCmbViewMode;
    // Table view with the physical units of the physical size
    QVBoxLayout*    m_pLytTableView;
    QWidget*        m_pWdgtTableView;
    CModelPhysSize  m_model;
    QTableView*     m_pTableView;
    QSpacerItem*    m_pSpacerItemStretchAtBottom;

}; // class CWdgtUnitViewGrpPhysScienceFields

//******************************************************************************
class CWdgtUnitViewGrpUserDefinedQuantities : public CWdgtUnitViewGrp
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewGrpUserDefinedQuantities( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewGrpUserDefinedQuantities();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );

}; // class CWdgtUnitViewGrpUserDefinedQuantities

//******************************************************************************
class CWdgtUnitViewUntRatio : public CWdgtUnitViewUnt
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewUntRatio( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewUntRatio();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected: // instance members
    QHBoxLayout* m_pLytFactor;
    QLabel*      m_pLblFactor;
    QLineEdit*   m_pEdtFactor;

}; // class CWdgtUnitViewUntRatio

//******************************************************************************
class CWdgtUnitViewUntDataQuantity : public CWdgtUnitViewUnt
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewUntDataQuantity( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewUntDataQuantity();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected: // instance members
    QHBoxLayout* m_pLytFactor;
    QLabel*      m_pLblFactor;
    QLineEdit*   m_pEdtFactor;

}; // class CWdgtUnitViewUntDataQuantity

//******************************************************************************
class CWdgtUnitViewPhysUnit : public CWdgtUnitViewUnt
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewPhysUnit( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewPhysUnit();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );
protected: // instance members
    // PhysUnit
    QHBoxLayout*             m_pLytFctConvertFromSIUnit;
    QLabel*                  m_pLblFctConvertFromSIUnit;
    QLineEdit*               m_pEdtFctConvertFromSIUnit;
    QHBoxLayout*             m_pLytFctConvertIntoSIUnit;
    QLabel*                  m_pLblFctConvertIntoSIUnit;
    QLineEdit*               m_pEdtFctConvertIntoSIUnit;
    // Table views with conversion functions
    QSplitter*               m_pSplTableViewFctConverts;
    QVBoxLayout*             m_pLytTableViewFctConvertsInternal;
    QWidget*                 m_pWdgtTableViewFctConvertsInternal;
    QLabel*                  m_pLblTableViewFctConvertsInternal;
    CModelFctConvertInternal m_modelFctConvertsInternal;
    QTableView*              m_pTableViewFctConvertsInternal;
    QVBoxLayout*             m_pLytTableViewFctConvertsExternal;
    QWidget*                 m_pWdgtTableViewFctConvertsExternal;
    QLabel*                  m_pLblTableViewFctConvertsExternal;
    CModelFctConvertExternal m_modelFctConvertsExternal;
    QTableView*              m_pTableViewFctConvertsExternal;

}; // class CWdgtUnitViewPhysUnit

//******************************************************************************
class CWdgtUnitViewUntUserDefinedQuantity : public CWdgtUnitViewUnt
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtUnitViewUntUserDefinedQuantity( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtUnitViewUntUserDefinedQuantity();
public: // overridables of base class CWdgtUnitView
    virtual void setUnitsModelNode( SUnitsModelNode* i_pNode );

}; // class CWdgtUnitViewUntUserDefinedQuantity

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_UnitViewWdgts_h
