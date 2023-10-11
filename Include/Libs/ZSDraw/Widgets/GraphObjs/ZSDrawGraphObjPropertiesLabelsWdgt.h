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

#ifndef ZSDraw_DrawGraphObjPropertiesLabelsWdgt_h
#define ZSDraw_DrawGraphObjPropertiesLabelsWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"

class QTableView;

namespace ZS
{
namespace Draw
{
class CModelGraphObjLabels;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjPropertiesLabels : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGraphObjPropertiesLabels"; }
public: // ctors and dtor
    CWdgtGraphObjPropertiesLabels(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjPropertiesLabels();
public: // instance methods
    void expand(bool i_bExpand);
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasErrors() const override;
    bool hasChanges() const override;
    void applySettings(bool i_bImmediatelyApplySettings = true) override;
protected slots:
    void onBtnCollapseClicked(bool i_bChecked = false);
protected slots:
    void onEdtNameTextChanged(const QString& i_strText);
    void onChkNameLabelVisibleStateChanged(int i_iState);
    void onCmbNameLabelAnchorSelPtCurrentIndexChanged(int i_idx);
    void onChkNameLabelAnchorLineVisibleStateChanged(int i_iState);
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void fillEditControls() override;
private: // auxiliary instance methods
    bool changedNameIsUnique() const;
    void fillComboNameLabelAnchorSelPt();
private: // instance members
    /*!< Edit controls. */
    QWidget* m_pWdgtHeadline;
    QHBoxLayout* m_pLytWdgtHeadline;
    QPixmap m_pxmBtnDown;
    QPixmap m_pxmBtnUp;
    QPushButton* m_pBtnCollapse;
    QLabel* m_pLblHeadlineIcon;
    QLabel* m_pLblHeadline;
    ZS::System::GUI::CSepLine* m_pSepHeadline;
    QWidget* m_pWdgtLabels;
    QVBoxLayout* m_pLytWdgtLabels;
    QHBoxLayout* m_pLytLineName;
    QLabel* m_pLblName;
    QLineEdit* m_pEdtName;
    QLabel* m_pLblNameError;
    QHBoxLayout* m_pLytLineNameVisibilities;
    QLabel* m_pLblNameVisible;
    QCheckBox* m_pChkNameLabelVisible;
    QLabel* m_pLblNameLabelAnchorSelPt;
    QComboBox* m_pCmbNameLabelAnchorSelPt;
    QLabel* m_pLblNameLabelAnchorLineVisible;
    QCheckBox* m_pChkNameLabelAnchorLineVisible;
    QVBoxLayout* m_pLytListView;
    QTableView* m_pTableView;
    CModelGraphObjLabels* m_pModel;

}; // class CWdgtGraphObjPropertiesLabels

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjPropertiesLabelsWdgt_h
