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

#ifndef ZSDraw_DrawGraphObjTextStylePropertiesWdgt_h
#define ZSDraw_DrawGraphObjTextStylePropertiesWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"

class QFont;

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjTextStyleProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGraphObjTextStyleProperties"; }
public: // ctors and dtor
    CWdgtGraphObjTextStyleProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjTextStyleProperties();
public: // instance methods
    void expand(bool i_bExpand);
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasChanges() const override;
    void applySettings(bool i_bImmediatelyApplySettings = true) override;
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void fillEditControls() override;
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void onDrawingSceneDrawSettingsChanged(const CDrawSettings& i_drawingSettings) override;
protected slots:
    void onBtnCollapseClicked(bool i_bChecked = false);
protected slots:
    void onCmbFontCurrentFontChanged(const QFont& i_fnt);
    void onCmbFontSizeCurrentIndexChanged(int i_idx);
    void onBtnTextColorClicked(bool i_bChecked = false);
    void onBtnFontStyleBoldToggled(bool i_bChecked);
    void onBtnFontStyleItalicToggled(bool i_bChecked);
    void onBtnTextEffectUnderlineToggled(bool i_bChecked);
    void onBtnTextEffectStrikeoutToggled(bool i_bChecked);
private: // auxiliary instance methods
    void updateCmbFont(const QFont& i_font);
    void updateCmbTextSize(ETextSize i_textSize);
    void updateBtnTextColor(const QColor& i_clr);
    void updateBtnFontStyleBold(const CEnumTextStyle& i_textStyle);
    void updateBtnFontStyleItalic(const CEnumTextStyle& i_textStyle);
    void updateBtnTextEffectUnderline(const CEnumTextEffect& i_textEffect);
    void updateBtnTextEffectStrikeout(const CEnumTextEffect& i_textEffect);
protected: // instance methods (method tracing)
    void traceValues(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_methodDir);
private: // instance members
    /*!< Cached draw settings of the graphical object. */
    QColor m_textColor;
    QFont m_font;
    ETextSize m_textSize;
    CEnumTextStyle m_textStyle;
    CEnumTextEffect m_textEffect;
    /*!< Edit controls. */
    QWidget* m_pWdgtHeadline;
    QHBoxLayout* m_pLytWdgtHeadline;
    QPixmap m_pxmBtnDown;
    QPixmap m_pxmBtnUp;
    QPushButton* m_pBtnCollapse;
    QLabel* m_pLblHeadlineIcon;
    QLabel* m_pLblHeadline;
    ZS::System::GUI::CSepLine* m_pSepHeadline;
    QWidget* m_pWdgtTextStyleSettings;
    QGridLayout* m_pLytWdgtTextStyleSettings;
    // Text Style
    QLabel* m_pLblFont;
    QFontComboBox* m_pCmbFont;
    QComboBox* m_pCmbFontSize;
    QPixmap* m_pPxmBtnTextColor;
    QPushButton* m_pBtnTextColor;
    QLabel* m_pLblEffects;
    QPushButton* m_pBtnFontStyleBold;
    QPushButton* m_pBtnFontStyleItalic;
    QPushButton* m_pBtnTextEffectUnderline;
    QPushButton* m_pBtnTextEffectStrikeout;

}; // class CWdgtGraphObjTextStyleProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjTextStylePropertiesWdgt_h
