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

#ifndef ZSDraw_DrawGraphObjFillStylePropertiesWdgt_h
#define ZSDraw_DrawGraphObjFillStylePropertiesWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjFillStyleProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGraphObjFillStyleProperties"; }
public: // ctors and dtor
    CWdgtGraphObjFillStyleProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjFillStyleProperties();
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
    void onCmbFillStyleCurrentIndexChanged(int i_idx);
    void onBtnFillColorClicked(bool i_bChecked = false);
private: // auxiliary instance methods
    QSize fillFillStylesModel();
private: // auxiliary instance methods
    void updateCmbFillStyle(const CEnumFillStyle& i_fillStyle);
    void updateBtnFillColor(const QColor& i_clr);
protected: // instance methods (method tracing)
    void traceValues(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_methodDir);
private: // instance members
    /*!< Cached draw settings of the graphical object. */
    QColor m_fillColor;
    CEnumFillStyle m_fillStyle;
    /*!< Edit controls. */
    QWidget* m_pWdgtHeadline;
    QHBoxLayout* m_pLytWdgtHeadline;
    QPixmap m_pxmBtnDown;
    QPixmap m_pxmBtnUp;
    QPushButton* m_pBtnCollapse;
    QLabel* m_pLblHeadlineIcon;
    QLabel* m_pLblHeadline;
    ZS::System::GUI::CSepLine* m_pSepHeadline;
    QWidget* m_pWdgtFillStyleSettings;
    QGridLayout* m_pLytWdgtFillStyleSettings;
    // Fill Style and Color
    QLabel* m_pLblFillStyle;
    QStandardItemModel* m_pModelFillStyles;
    QComboBox* m_pCmbFillStyle;
    QPixmap* m_pPxmBtnFillColor;
    QPushButton* m_pBtnFillColor;

}; // class CWdgtGraphObjFillStyleProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjFillStylePropertiesWdgt_h
