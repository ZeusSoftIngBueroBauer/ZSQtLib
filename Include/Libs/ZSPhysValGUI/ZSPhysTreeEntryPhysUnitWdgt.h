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

#ifndef ZSPhysValGUI_PhysTreeEntryPhysUnitWdgt_h
#define ZSPhysValGUI_PhysTreeEntryPhysUnitWdgt_h

#include "ZSPhysValGUI/ZSPhysTreeEntryAbstractWdgt.h"

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
class CWdgtEditPhysVal;
class CModelUnitFctConvertInternal;
class CModelUnitFctConvertExternal;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtPhysUnit : public CWdgtAbstractTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtPhysUnit"; }
public: // ctors and dtor
    CWdgtPhysUnit(QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtPhysUnit();
public: // overridables
    virtual QString nameSpace() const { return CWdgtPhysUnit::NameSpace(); }
    virtual QString className() const { return CWdgtPhysUnit::ClassName(); }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree ) override;
protected slots:
    void onEdtPhysValSrcEditingFinished();
    void onCmbUnitsDstCurrentIndexChanged(int i_iIdx);
    void onBtnTableViewFctConvertsInternalResizeToContentsClicked( bool i_bChecked );
    void onBtnTableViewFctConvertsExternalResizeToContentsClicked( bool i_bChecked );
protected: // instance members
    QSize m_szBtns;
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytSymbol;
    QLabel* m_pLblSymbol;
    QLineEdit* m_pEdtSymbol;
    QHBoxLayout* m_pLytNextLower;
    QLabel* m_pLblNextLower;
    QLineEdit* m_pEdtNextLower;
    QHBoxLayout* m_pLytNextHigher;
    QLabel* m_pLblNextHigher;
    QLineEdit* m_pEdtNextHigher;
    // PhysUnit
    QHBoxLayout* m_pLytFctConvertFromSIUnit;
    QLabel* m_pLblFctConvertFromSIUnit;
    QLineEdit* m_pEdtFctConvertFromSIUnit;
    QHBoxLayout* m_pLytFctConvertIntoSIUnit;
    QLabel* m_pLblFctConvertIntoSIUnit;
    QLineEdit* m_pEdtFctConvertIntoSIUnit;
    // Conversion test
    QHBoxLayout* m_pLytLinePhysValSrc;
    QLabel* m_pLblPhysValSrc;
    CWdgtEditPhysVal* m_pEdtPhysValSrc;
    QHBoxLayout* m_pLytLineValDst;
    QLabel* m_pLblValDst;
    QLineEdit* m_pEdtValDst;
    QComboBox* m_pCmbUnitsDst;
    // Table views with conversion functions
    QSplitter* m_pSplTableViewFctConverts;
    // Internal conversion functions
    QWidget* m_pWdgtTableViewFctConvertsInternal;
    QVBoxLayout* m_pLytTableViewFctConvertsInternal;
    QHBoxLayout* m_pLytHeadlineFctConvertsInternal;
    QLabel* m_pLblHeadlineFctConvertsInternal;
    QPushButton* m_pBtnTableViewFctConvertsInternalResizeToContents;
    CModelUnitFctConvertInternal* m_pModelFctConvertsInternal;
    QTableView* m_pTableViewFctConvertsInternal;
    // External conversion functions
    QWidget* m_pWdgtTableViewFctConvertsExternal;
    QVBoxLayout* m_pLytTableViewFctConvertsExternal;
    QHBoxLayout* m_pLytHeadlineFctConvertsExternal;
    QLabel* m_pLblHeadlineFctConvertsExternal;
    QPushButton* m_pBtnTableViewFctConvertsExternalResizeToContents;
    CModelUnitFctConvertExternal* m_pModelFctConvertsExternal;
    QTableView* m_pTableViewFctConvertsExternal;

}; // class CWdgtPhysUnit

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysTreeEntryPhysUnitWdgt_h
