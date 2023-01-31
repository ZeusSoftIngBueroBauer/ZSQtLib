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

//class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
//class QPushButton;
class QSpinBox;
//class QSplitter;
//class QTableView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
class CSepLine;
}
}
namespace PhysVal
{
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
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtDrawingViewProperties();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setDimensionUnit( EDrawingDimensionUnit i_dimensionUnit );
    void setMetricUnit( const QString& i_strMetricUnitSymbol );
protected slots:
    void onDrawingViewDrawingSizeChanged(const QSize& i_size);
    void onCmbDimensionUnitCurrentIndexChanged(int i_idx);
    void onCmbImageMetricScaleFactorDividendCurrentTextChanged(const QString& i_strText);
    void onCmbImageMetricScaleFactorDivisorCurrentTextChanged(const QString& i_strText);
    void onCmbImageMetricUnitCurrentIndexChanged(int i_idx);
    void onEdtImageMetricWidthEditingFinished();
    void onEdtImageMetricHeightEditingFinished();
    void onEdtImageSizeWidthPxEditingFinished();
    void onEdtImageSizeHeightPxEditingFinished();
protected: // instance members
    CDrawingView* m_pDrawingView;
    QVBoxLayout* m_pLyt;
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
    QHBoxLayout* m_pLytLineImageMetricScaleFactor;
    QLabel* m_pLblImageMetricScaleFactor;
    QComboBox* m_pCmbImageMetricScaleFactorDividend;
    QLabel* m_pLblImageMetricScaleFactorHyphen;
    QComboBox* m_pCmbImageMetricScaleFactorDivisor;
    QLabel* m_pLblImageMetricUnit;
    QComboBox* m_pCmbImageMetricUnit;
    QHBoxLayout* m_pLytLineMetricSize;
    QLabel* m_pLblImageMetricWidth;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtImageMetricWidth;
    QLabel* m_pLblImageMetricHeight;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtImageMetricHeight;
    // Image Size in Pixels
    QHBoxLayout* m_pLytSepLineImageSize_px;
    QLabel* m_pLblSepLineImageSize_px;
    ZS::System::GUI::CSepLine* m_pSepLineImageSize_px;
    QHBoxLayout* m_pLytLineImageSize_px;
    QLabel* m_pLblImageSizeWidth_px;
    QSpinBox* m_pEdtImageSizeWidth_px;
    QLabel* m_pLblImageSizeHeight_px;
    QSpinBox* m_pEdtImageSizeHeight_px;
    // Caching values
    EDrawingDimensionUnit m_dimensionUnit;
    int m_iMetricScaleFactorDividend;
    int m_iMetricScaleFactorDivisor;
    QString m_strMetricUnitSymbol;
    double m_fImageMetricWidth;
    double m_fImageMetricHeight;
    int m_cxImageSizeWidth_px;
    int m_cyImageSizeHeight_px;

}; // class CWdgtDrawingViewProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingViewPropertiesWdgt_h
