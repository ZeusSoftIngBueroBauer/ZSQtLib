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

#ifndef ZSDraw_DrawingViewPropertiesWdgt_h
#define ZSDraw_DrawingViewPropertiesWdgt_h

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjPropertiesAbstractWdgt.h"
#include "ZSDraw/Drawing/ZSDrawGridSettings.h"
#include "ZSDraw/Drawing/ZSDrawingSize.h"

class QCheckBox;
class QComboBox;
class QFontComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListView;
class QPushButton;
class QSpinBox;
class QStandardItemModel;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
class CSepLine;
}
}
namespace PhysVal
{
class CPhysVal;

namespace GUI
{
class CWdgtEditPhysVal;
}
}
namespace Draw
{
class CDrawingView;

//******************************************************************************
class ZSDRAWDLL_API CWdgtDrawingViewProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtDrawingViewProperties"; }
public: // ctors and dtor
    CWdgtDrawingViewProperties(
    CDrawingView* i_pDrawingView,
    ZS::System::EMode i_mode = ZS::System::EMode::View,
    QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtDrawingViewProperties();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void drawingSizeChanged(const ZS::Draw::CDrawingSize& i_size);
    void gridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings);
public: // instance methods
    bool hasChanges() const;
    void acceptChanges();
    void rejectChanges();
protected: // instance methods
    void setMode(ZS::System::EMode i_mode);
    ZS::System::EMode mode() const;
protected slots:
    void onDrawingViewDrawingSizeChanged(const ZS::Draw::CDrawingSize& i_size);
    void onDrawingViewGridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings);
protected slots:
    void onCmbDimensionUnitCurrentIndexChanged(int i_idx);
    void onCmbImageMetricUnitCurrentIndexChanged(int i_idx);
    void onEdtImageMetricWidthValueChanged(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void onEdtImageMetricHeightValueChanged(const ZS::PhysVal::CPhysVal& i_physValHeight);
    void onCmbImageMetricNormedPaperSizesCurrentIndexChanged(int i_idx);
    void onCmbImageMetricNormedPaperOrientationCurrentIndexChanged(int i_idx);
    void onCmbImageMetricScaleFactorDividendCurrentTextChanged(const QString& i_strDividend);
    void onCmbImageMetricScaleFactorDividendEditTextChanged(const QString& i_strDividend);
    void onCmbImageMetricScaleFactorDivisorCurrentTextChanged(const QString& i_strDivisor);
    void onCmbImageMetricScaleFactorDivisorEditTextChanged(const QString& i_strDivisor);
    void onEdtImageSizeWidthPxValueChanged(int i_cxWidth_px);
    void onEdtImageSizeHeightPxValueChanged(int i_cyHeight_px);
    void onChkGridLinesVisibleStateChanged(int i_iState);
    void onCmbGridLineStyleCurrentIndexChanged(int i_idx);
    void onEdtGridPenWidthValueChanged(int i_iVal);
    void onBtnGridPenColorClicked(bool i_bChecked);
    void onChkGridScaleLabelsVisibleStateChanged(int i_iState);
    void onCmbGridScaleLabelsCurrentFontChanged(const QFont& i_fnt);
    void onCmbGridScaleLabelsFontSizeCurrentIndexChanged(int i_iCurrentIndex);
    void onBtnGridScaleLabelsTextColorClicked(bool i_bChecked);
    void onBtnGridScaleLabelsFontStyleBoldToggled(int i_iState);
    void onBtnGridScaleLabelsFontStyleItalicToggled(int i_iState);
    void onBtnGridScaleLabelsTextEffectUnderlineToggled(int i_iState);
    void onBtnGridScaleLabelsTextEffectStrikeoutToggled(int i_iState);
    void onBtnEditClicked(bool i_bChecked = false);
protected: // instance methods
    void setDimensionUnit( const CEnumDrawingDimensionUnit& i_eDimensionUnit );
    void setMetricUnit( const ZS::PhysVal::CUnit& i_metricUnit );
    void setNormedPaperSize( const CEnumNormedPaperSize& i_ePaperSize );
    void setNormedPaperOrientation( const ZS::System::CEnumDirection& i_eDirection );
    void setScaleFactor( int i_iDividend, int i_iDivisor );
    void setImageSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight );
protected: // instance methods
    void setGridVisible(bool i_bVisible);
    void setGridLineStyle(ELineStyle i_lineStyle);
    void setGridPenColor(const QColor& i_color);
    void setGridPenWidth(int i_iWidth_px);
    void setGridLabelsVisible(bool i_bVisible);
    void setGridLabelsFont(const QFont& i_fnt);
    void setGridLabelsTextSize(ETextSize i_textSize);
    void setGridLabelsTextColor(const QColor& i_color);
    void setGridLabelsTextStyle(ETextStyle i_textStyle);
    void setGridLabelsTextEffect(const ETextEffect i_textEffect);
protected: // instance methods
    void updateDimensionUnit();
    void updateImageSizeInPixels();
    void updateImageSizeMetrics();
    void updatePaperFormat();
protected: // instance methods
    void updateGridSettings();
    QSize fillGridLineStylesModel();
    void updateGridPenColorButtonIcon();
    void updateGridLabelsTextColorButtonIcon();
    void updateGridLabelsTextStyleBoldButton();
    void updateGridLabelsTextStyleItalicButton();
    void updateGridLabelsTextEffectUnderlineButton();
    void updateGridLabelsTextEffectStrikeoutButton();
protected: // instance methods (method tracing)
    void emit_drawingSizeChanged(const ZS::Draw::CDrawingSize& i_size);
    void emit_gridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings);
    void traceValues(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_methodDir);
protected: // instance members
    CDrawingView* m_pDrawingView;
    ZS::System::EMode m_mode;
    // Caching values
    CDrawingSize m_drawingSize;
    CDrawGridSettings m_gridSettings;
    /*!< Blocking signals counter. */
    int m_iValueChangedSignalsBlocked;
    // Edit Controls
    QVBoxLayout* m_pLyt;
    // Geometry
    QGroupBox* m_pGrpGeometry;
    QVBoxLayout* m_pLytGrpGeometry;
    QHBoxLayout* m_pLytLineDimensionUnit;
    QLabel* m_pLblDimensionUnit;
    QComboBox* m_pCmbDimensionUnit;
    // Metric system
    QWidget* m_pWdgtMetric;
    QVBoxLayout* m_pLytWdgtMetric;
    // Resolution of a pixel on screen
    QHBoxLayout* m_pLytSepLineResolution;
    QLabel* m_pLblSepLineResolution;
    ZS::System::GUI::CSepLine* m_pSepLineResolution;
    QHBoxLayout* m_pLytLineResolution;
    QLabel* m_pLblResolution_pxpi;
    QLineEdit* m_pEdtResolution_pxpi;
    QLabel* m_pLblResolution_pxpmm;
    QLineEdit* m_pEdtResolution_pxpmm;
    // Image Size in Metric System
    QHBoxLayout* m_pLytSepLineImageMetric;
    QLabel* m_pLblSepLineImageMetric;
    ZS::System::GUI::CSepLine* m_pSepLineImageMetric;
    QHBoxLayout* m_pLytLineImageMetricUnit;
    QLabel* m_pLblImageMetricUnit;
    QComboBox* m_pCmbImageMetricUnit;
    QHBoxLayout* m_pLytLineMetricSize;
    QLabel* m_pLblImageMetricWidth;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtImageMetricWidth;
    QLabel* m_pLblImageMetricHeight;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtImageMetricHeight;
    QHBoxLayout* m_pLytLineImageMetricNormedPaper;
    QLabel* m_pLblImageMetricNormedPaperSizes;
    QComboBox* m_pCmbImageMetricNormedPaperSizes;
    QLabel* m_pLblImageMetricNormedPaperOrientation;
    QComboBox* m_pCmbImageMetricNormedPaperOrientation;
    QHBoxLayout* m_pLytLineImageMetricScaleFactor;
    QLabel* m_pLblImageMetricScaleFactor;
    QComboBox* m_pCmbImageMetricScaleFactorDividend;
    QLabel* m_pLblImageMetricScaleFactorHyphen;
    QComboBox* m_pCmbImageMetricScaleFactorDivisor;
    // Image Size in Pixels
    QHBoxLayout* m_pLytSepLineImageSize_px;
    QLabel* m_pLblSepLineImageSize_px;
    ZS::System::GUI::CSepLine* m_pSepLineImageSize_px;
    QHBoxLayout* m_pLytLineImageSize_px;
    QLabel* m_pLblImageSizeWidth_px;
    QSpinBox* m_pEdtImageSizeWidth_px;
    QLabel* m_pLblImageSizeHeight_px;
    QSpinBox* m_pEdtImageSizeHeight_px;
    // Grid
    QGroupBox* m_pGrpGrid;
    QHBoxLayout* m_pLytGrpGrid;
    QGridLayout* m_pLytGridSettings;
    QLabel* m_pLblGridLines;
    QCheckBox* m_pChkGridLinesVisible;
    QComboBox* m_pCmbGridLineStyle;
    QStandardItemModel* m_pModelGridLineStyles;
    QSpinBox* m_pEdtGridPenWidth;
    QPixmap* m_pPxmBtnGridPenColor;
    QRect m_rctBtnGridPenColor;
    QLine m_lineBtnGridPenColor1;
    QLine m_lineBtnGridPenColor2;
    QLine m_lineBtnGridPenColor3;
    QPushButton* m_pBtnGridPenColor;
    // Grid Scale Labels
    QLabel* m_pLblGridScaleLabels;
    QCheckBox* m_pChkGridScaleLabelsVisible;
    QLabel* m_pLblGridScaleLabelsFont;
    QFontComboBox* m_pCmbGridScaleLabelsFont;
    QComboBox* m_pCmbGridScaleLabelsFontSize;
    QPixmap* m_pPxmBtnGridScaleLabelsTextColor;
    QRect m_rctBtnGridScaleLabelsTextColor;
    QPushButton* m_pBtnGridScaleLabelsTextColor;
    QPushButton* m_pBtnGridScaleLabelsFontStyleBold;
    QPushButton* m_pBtnGridScaleLabelsFontStyleItalic;
    QPushButton* m_pBtnGridScaleLabelsTextEffectUnderline;
    QPushButton* m_pBtnGridScaleLabelsTextEffectStrikeout;
    // Button Line
    QWidget* m_pWdgtButtons;
    QHBoxLayout* m_pLytWdgtButtons;
    QPushButton* m_pBtnEdit;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtDrawingViewProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingViewPropertiesWdgt_h
