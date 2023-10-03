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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"
#include "ZSDraw/Common/ZSDrawGridSettings.h"

namespace ZS
{
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
        const QString& i_strObjName,
        bool i_bCreateButtonsWidget = true,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtDrawingViewProperties();
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasChanges() const override;
    void acceptChanges() override;
    void rejectChanges() override;
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize) override;
protected slots:
    void onDrawUnitsScreenResolutionInPxPerMMChanged();
    void onDrawingViewGridSettingsChanged(const CDrawGridSettings& i_gridSettings);
protected slots:
    void onCmbDimensionUnitCurrentIndexChanged(int i_idx);
    void onEdtScreenPixelResolutionPxPerMMValueChanged(double i_fRes_px_mm);
    void onEdtImageCoorsDecimalsValueChanged(int i_iDecimals);
    void onCmbImageMetricUnitCurrentIndexChanged(int i_idx);
    void onEdtImageMetricWidthValueChanged(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void onEdtImageMetricHeightValueChanged(const ZS::PhysVal::CPhysVal& i_physValHeight);
    void onCmbImageMetricNormedPaperSizesCurrentIndexChanged(int i_idx);
    void onCmbImageMetricNormedPaperOrientationCurrentIndexChanged(int i_idx);
    void onCmbImageMetricScaleFactorDividendCurrentTextChanged(const QString& i_strDividend);
    void onCmbImageMetricScaleFactorDividendEditTextChanged(const QString& i_strDividend);
    void onCmbImageMetricScaleFactorDivisorCurrentTextChanged(const QString& i_strDivisor);
    void onCmbImageMetricScaleFactorDivisorEditTextChanged(const QString& i_strDivisor);
    void onCmbImageMetricYScaleAxisOrientationCurrentIndexChanged(int i_idx);
    void onEdtImageSizeWidthPxValueChanged(int i_cxWidth_px);
    void onEdtImageSizeHeightPxValueChanged(int i_cyHeight_px);
    void onChkGridLinesVisibleStateChanged(int i_iState);
    void onEdtGridLinesDistMinValueChanged(int i_iVal);
    void onCmbGridLinesStyleCurrentIndexChanged(int i_idx);
    void onEdtGridLinesWidthValueChanged(int i_iVal);
    void onBtnGridLinesColorClicked(bool i_bChecked);
    void onChkGridScaleLabelsVisibleStateChanged(int i_iState);
    void onCmbGridScaleLabelsCurrentFontChanged(const QFont& i_fnt);
    void onCmbGridScaleLabelsFontSizeCurrentIndexChanged(int i_iCurrentIndex);
    void onBtnGridScaleLabelsTextColorClicked(bool i_bChecked);
    void onBtnGridScaleLabelsFontStyleBoldToggled(int i_iState);
    void onBtnGridScaleLabelsFontStyleItalicToggled(int i_iState);
    void onBtnGridScaleLabelsTextEffectUnderlineToggled(int i_iState);
    void onBtnGridScaleLabelsTextEffectStrikeoutToggled(int i_iState);
protected: // instance methods
    void setDimensionUnit( const ZS::System::CEnumScaleDimensionUnit& i_eDimensionUnit );
    void setScreenPixelResolutionInPxPerMM(double i_fRes_px_mm);
    void setImageCoorsDecimals(int i_iDecimals);
    void setMetricUnit( const ZS::PhysVal::CUnit& i_metricUnit );
    void setNormedPaperSize( const CEnumNormedPaperSize& i_ePaperSize );
    void setNormedPaperOrientation( const ZS::System::CEnumOrientation& i_eOrientation );
    void setScaleFactor( int i_iDividend, int i_iDivisor );
    void setYScaleAxisOrientation( const ZS::System::CEnumYScaleAxisOrientation& i_eOrientation );
    void setImageSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight );
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
    void updateDimensionUnit();
    void updateResolutions();
    void updateImageSizeInPixels();
    void updateImageSizeMetrics();
    void updatePaperFormat();
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
protected: // instance members
    CDrawingView* m_pDrawingView;
    /*!< Caching values. */
    CDrawingSize m_drawingSize;
    CDrawGridSettings m_gridSettings;
    // Edit Controls
    // Geometry
    QGroupBox* m_pGrpGeometry;
    QVBoxLayout* m_pLytGrpGeometry;
    QHBoxLayout* m_pLytLineDimensionUnit;
    QLabel* m_pLblDimensionUnit;
    QComboBox* m_pCmbDimensionUnit;
    // Metric system
    QWidget* m_pWdgtMetric;
    QVBoxLayout* m_pLytWdgtMetric;
    // Resolutions of pixel on screen, editing and indicating coordinates
    QHBoxLayout* m_pLytSepLineResolution;
    QLabel* m_pLblSepLineResolution;
    ZS::System::GUI::CSepLine* m_pSepLineResolution;
    QHBoxLayout* m_pLytLineResolutions;
    QLabel* m_pLblScreenPixelResolution_pxpmm;
    QDoubleSpinBox* m_pEdtScreenPixelResolution_pxpmm;
    QLabel* m_pLblImageCoorsDecimals;
    QSpinBox* m_pEdtImageCoorsDecimals;
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
    QLabel* m_pLblImageMetricYScaleAxisOrientation;
    QComboBox* m_pCmbImageMetricYScaleAxisOrientation;
    // Image Size in Pixels
    QWidget* m_pWdgtImageSize_px;
    QVBoxLayout* m_pLytWdgtImageSize_px;
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
    QLabel* m_pLblGridLinesDistMin;
    QSpinBox* m_pEdtGridLinesDistMin;
    QComboBox* m_pCmbGridLinesStyle;
    QStandardItemModel* m_pModelGridLinesStyles;
    QSpinBox* m_pEdtGridLinesWidth;
    QPixmap* m_pPxmBtnGridLinesColor;
    QRect m_rctBtnGridLinesColor;
    QPushButton* m_pBtnGridLinesColor;
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

}; // class CWdgtDrawingViewProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingViewPropertiesWdgt_h
