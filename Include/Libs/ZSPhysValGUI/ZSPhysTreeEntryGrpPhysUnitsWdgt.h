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

#ifndef ZSPhysValGUI_PhysTreeEntryGrpPhysUnitsWdgt_h
#define ZSPhysValGUI_PhysTreeEntryGrpPhysUnitsWdgt_h

#include "ZSPhysValGUI/ZSPhysTreeEntryAbstractWdgt.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSplitter;
class QTableView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
namespace GUI
{
class CModelPhysUnitsGrp;
class CModelUnitFctConvertRefVals;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtPhysUnitsGrp : public CWdgtAbstractTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtPhysUnitsGrp"; }
public: // ctors and dtor
    CWdgtPhysUnitsGrp(QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtPhysUnitsGrp();
public: // overridables
    virtual QString nameSpace() const { return CWdgtPhysUnitsGrp::NameSpace(); }
    virtual QString className() const { return CWdgtPhysUnitsGrp::ClassName(); }
public: // overridables of base class CWdgtAbstractTreeEntry
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree ) override;
    virtual void resizeToContents() override;
protected slots:
    void onBtnTableViewRefValsResizeToContentsClicked( bool i_bChecked );
    void onBtnTableViewUnitsGrpResizeToContentsClicked( bool i_bChecked );
    void onCmbViewModeCurrentIndexChanged( const QString& i_strCurrentText );
protected: // instance methods
    void updateTableView();
protected: // instance members
    QSize m_szBtns;
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytSIUnit;
    QLabel* m_pLblSIUnit;
    QLineEdit* m_pEdtSIUnit;
    QHBoxLayout* m_pLytFormulaSymbol;
    QLabel* m_pLblFormulaSymbol;
    QLineEdit* m_pEdtFormulaSymbol;
    QHBoxLayout* m_pLytIsPowerRelated;
    QLabel* m_pLblIsPowerRelated;
    QCheckBox* m_pChkIsPowerRelated;
    // Table views
    QSplitter* m_pSplTableViews;
    // Table view with reference values
    QWidget* m_pWdgtTableViewRefVals;
    QVBoxLayout* m_pLytTableViewRefVals;
    QHBoxLayout* m_pLytHeadlineRefVals;
    QLabel* m_pLblHeadlineRefVals;
    QPushButton* m_pBtnTableViewRefValsResizeToContents;
    CModelUnitFctConvertRefVals* m_pModelRefVals;
    QTableView* m_pTableViewRefVals;
    // Table view with the units of the units group
    QWidget* m_pWdgtTableViewUnitsGrp;
    QVBoxLayout* m_pLytTableViewUnitsGrp;
    QHBoxLayout* m_pLytHeadlineUnitsGrp;
    QLabel* m_pLblHeadlineUnitsGrp;
    QLabel* m_pLblViewModeUnitsGrp;
    QComboBox* m_pCmbViewModeUnitsGrp;
    QPushButton* m_pBtnTableViewUnitsGrpResizeToContents;
    CModelPhysUnitsGrp* m_pModelUnitsGrp;
    QTableView* m_pTableViewUnitsGrp;

}; // class CWdgtPhysUnitsGrp

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysTreeEntryGrpPhysUnitsWdgt_h
