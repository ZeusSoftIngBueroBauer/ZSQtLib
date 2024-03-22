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

#ifndef ZSDraw_DrawGraphObjGridStylePropertiesWdgt_h
#define ZSDraw_DrawGraphObjGridStylePropertiesWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"
#include "ZSDraw/Common/ZSDrawGridSettings.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjGridStyleProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGraphObjGridStyleProperties"; }
public: // ctors and dtor
    CWdgtGraphObjGridStyleProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjGridStyleProperties();
public: // instance methods
    void expand(bool i_bExpand);
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasChanges() const override;
    void applySettings(bool i_bImmediatelyApplySettings = true) override;
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void fillEditControls() override;
protected slots:
    void onBtnCollapseClicked(bool i_bChecked = false);
protected slots:
    void onChkGridLinesVisibleStateChanged(int i_iState);
    void onEdtGridLinesDistMinValueChanged(int i_iVal);
    void onCmbGridLinesStyleCurrentIndexChanged(int i_idx);
    void onEdtGridLinesWidthValueChanged(int i_iVal);
    void onBtnGridLinesColorClicked(bool i_bChecked);
    void onChkGridScaleLabelsVisibleStateChanged(int i_iState);
    void onCmbGridScaleLabelsCurrentFontChanged(const QFont& i_fnt);
    void onCmbGridScaleLabelsFontSizeCurrentIndexChanged(int i_iCurrentIndex);
    void onBtnGridScaleLabelsTextColorClicked(bool i_bChecked);
    void onBtnGridScaleLabelsFontStyleBoldToggled(bool i_bChecked);
    void onBtnGridScaleLabelsFontStyleItalicToggled(bool i_bChecked);
    void onBtnGridScaleLabelsTextEffectUnderlineToggled(bool i_bChecked);
    void onBtnGridScaleLabelsTextEffectStrikeoutToggled(bool i_bChecked);
protected: // instance methods
    void setGridLinesVisible(bool i_bVisible);
    void setGridLinesDistMin(int i_iDistMin_px);
    void setGridLinesStyle(ELineStyle i_lineStyle);
    void setGridLinesColor(const QColor& i_color);
    void setGridLinesWidth(int i_iWidth_px);
    void setGridLabelsVisible(bool i_bVisible);
    void setGridLabelsFont(const QFont& i_fnt);
    void setGridLabelsTextSize(ETextSize i_textSize);
    void setGridLabelsTextColor(const QColor& i_color);
    void setGridLabelsTextStyle(ETextStyle i_textStyle);
    void setGridLabelsTextEffect(const ETextEffect i_textEffect);
protected: // instance methods
    void updateGridSettings();
    QSize fillGridLineStylesModel();
    void updateGridLinesColorButtonIcon();
    void updateGridLabelsTextColorButtonIcon();
    void updateGridLabelsTextStyleBoldButton();
    void updateGridLabelsTextStyleItalicButton();
    void updateGridLabelsTextEffectUnderlineButton();
    void updateGridLabelsTextEffectStrikeoutButton();
protected: // instance methods (method tracing)
    void traceValues(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_methodDir);
private: // instance members
    /*!< Cached grid settings of the graphical object. */
    CDrawGridSettings m_gridSettings;
    /*!< Edit controls. */
    QWidget* m_pWdgtHeadline;
    QHBoxLayout* m_pLytWdgtHeadline;
    QPixmap m_pxmBtnDown;
    QPixmap m_pxmBtnUp;
    QPushButton* m_pBtnCollapse;
    QLabel* m_pLblHeadlineIcon;
    QLabel* m_pLblHeadline;
    ZS::System::GUI::CSepLine* m_pSepHeadline;
    QWidget* m_pWdgtGridStyleSettings;
    QGridLayout* m_pLytWdgtGridStyleSettings;
    // Grid Lines
    QLabel* m_pLblGridLines;
    QCheckBox* m_pChkGridLinesVisible;
    QLabel* m_pLblGridLinesDistMin;
    QSpinBox* m_pEdtGridLinesDistMin;
    QLabel* m_pLblGridLinesStyle;
    QComboBox* m_pCmbGridLinesStyle;
    QStandardItemModel* m_pModelGridLinesStyles;
    QSpinBox* m_pEdtGridLinesWidth;
    QPixmap* m_pPxmBtnGridLinesColor;
    QPushButton* m_pBtnGridLinesColor;
    // Grid Scale Labels
    QLabel* m_pLblGridScaleLabels;
    QCheckBox* m_pChkGridScaleLabelsVisible;
    QFontComboBox* m_pCmbGridScaleLabelsFont;
    QComboBox* m_pCmbGridScaleLabelsFontSize;
    QPixmap* m_pPxmBtnGridScaleLabelsTextColor;
    QRect m_rctBtnGridScaleLabelsTextColor;
    QPushButton* m_pBtnGridScaleLabelsTextColor;
    QLabel* m_pLblGridScaleLabelsTextStyle;
    QPushButton* m_pBtnGridScaleLabelsFontStyleBold;
    QPushButton* m_pBtnGridScaleLabelsFontStyleItalic;
    QPushButton* m_pBtnGridScaleLabelsTextEffectUnderline;
    QPushButton* m_pBtnGridScaleLabelsTextEffectStrikeout;

}; // class ZSDraw_DrawGraphObjGridStylePropertiesWdgt_h

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjFillStylePropertiesWdgt_h
