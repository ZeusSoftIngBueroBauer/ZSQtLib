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

#ifndef ZSPhysValGUI_PhysTreeEntryPhySizeWdgt_h
#define ZSPhysValGUI_PhysTreeEntryPhySizeWdgt_h

#include "ZSPhysValGUI/ZSPhysTreeEntryAbstractWdgt.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QSpacerItem;
class QTableView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
namespace GUI
{
class CModelPhysSize;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtPhysSize : public CWdgtAbstractTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtPhysSize"; }
public: // ctors and dtor
    CWdgtPhysSize( CIdxTreeUnits* i_pIdxTree, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtPhysSize();
public: // overridables
    virtual QString nameSpace() const { return CWdgtPhysSize::NameSpace(); }
    virtual QString className() const { return CWdgtPhysSize::ClassName(); }
public: // overridables of base class CWdgtAbstractTreeEntry
    virtual void setIdxTree(CIdxTreeUnits* i_pIdxTree) override;
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree ) override;
    virtual void resizeToContents() override;
protected slots:
    void onCmbViewModeCurrentIndexChanged( const QString& i_strCurrentText );
protected: // instance methods
    void updateTableView();
protected: // instance members
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
    QHBoxLayout* m_pLytViewMode;
    QLabel* m_pLblViewMode;
    QComboBox* m_pCmbViewMode;
    // Table view with the physical units of the physical size
    QVBoxLayout* m_pLytTableView;
    QWidget* m_pWdgtTableView;
    CModelPhysSize* m_pModel;
    QTableView* m_pTableView;
    QSpacerItem* m_pSpacerItemStretchAtBottom;

}; // class CWdgtPhysSize

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysTreeEntryPhySizeWdgt_h
