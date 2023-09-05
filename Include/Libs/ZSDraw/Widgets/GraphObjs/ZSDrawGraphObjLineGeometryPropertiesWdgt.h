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

#ifndef ZSDraw_DrawGraphObjLineGeometryPropertiesWdgt_h
#define ZSDraw_DrawGraphObjLineGeometryPropertiesWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjLineGeometryProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGraphObjLineGeometryProperties"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // ctors and dtor
    CWdgtGraphObjLineGeometryProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strParentClassName,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjLineGeometryProperties();
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasChanges() const override;
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize) override;
protected slots:
    void onBtnCollapseClicked(bool i_bChecked = false);
protected slots:
    void onEdtMetricPt1XValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricPt1YValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricPt2XValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricPt2YValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricLengthValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricAngleValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPt1XValueChanged(int i_iVal_px);
    void onEdtMetricWidthValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricHeightValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPt1YValueChanged(int i_iVal_px);
    void onEdtPixelsPt2XValueChanged(int i_iVal_px);
    void onEdtPixelsPt2YValueChanged(int i_iVal_px);
    void onEdtPixelsLengthValueChanged(int i_iVal_px);
    void onEdtPixelsAngleValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPosXValueChanged(int i_iVal_px);
    void onEdtPixelsPosYValueChanged(int i_iVal_px);
    void onEdtPixelsWidthValueChanged(int i_iVal_px);
    void onEdtPixelsHeightValueChanged(int i_iVal_px);
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void fillEditControls() override;
    void applySettings() override;
private: // auxiliary instance methods
    void updateDrawingSize();
private: // instance members
    QWidget* m_pWdgtHeadline;
    QHBoxLayout* m_pLytWdgtHeadline;
    QPixmap m_pxmBtnDown;
    QPixmap m_pxmBtnUp;
    QPushButton* m_pBtnCollapse;
    QLabel* m_pLblHeadlineIcon;
    QLabel* m_pLblHeadline;
    ZS::System::GUI::CSepLine* m_pSepHeadline;
    QWidget* m_pWdgtGeometry;
    QVBoxLayout* m_pLytWdgtGeometry;
    // Geometry in Metric System
    QWidget* m_pWdgtMetric;
    QVBoxLayout* m_pLytWdgtMetric;
    QWidget* m_pWdgtSepLineMetricGeometry;
    QHBoxLayout* m_pLytSepLineMetricGeometry;
    QLabel* m_pLblSepLineMetricGeometry;
    ZS::System::GUI::CSepLine* m_pSepLineMetricGeometry;
    QHBoxLayout* m_pLytLineMetricPt1;
    QLabel* m_pLblMetricPt1;
    QLabel* m_pLblMetricPt1X;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricPt1X;
    QLabel* m_pLblMetricPt1Y;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricPt1Y;
    QHBoxLayout* m_pLytLineMetricPt2;
    QLabel* m_pLblMetricPt2;
    QLabel* m_pLblMetricPt2X;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricPt2X;
    QLabel* m_pLblMetricPt2Y;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricPt2Y;
    QHBoxLayout* m_pLytLineMetricLengthAngle;
    QLabel* m_pLblMetricLength;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricLength;
    QLabel* m_pLblMetricAngle;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricAngle;
    QHBoxLayout* m_pLytLineMetricSize;
    QLabel* m_pLblMetricWidth;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricWidth;
    QLabel* m_pLblMetricHeight;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricHeight;
    // Geometry in Pixels
    QWidget* m_pWdgtPixels;
    QVBoxLayout* m_pLytWdgtPixels;
    QWidget* m_pWdgtSepLinePixelsGeometry;
    QHBoxLayout* m_pLytSepLinePixelsGeometry;
    QLabel* m_pLblSepLinePixelsGeometry;
    ZS::System::GUI::CSepLine* m_pSepLinePixelsGeometry;
    QHBoxLayout* m_pLytLinePixelsPt1;
    QLabel* m_pLblPixelsPt1;
    QLabel* m_pLblPixelsPt1X;
    QSpinBox* m_pEdtPixelsPt1X;
    QLabel* m_pLblPixelsPt1Y;
    QSpinBox* m_pEdtPixelsPt1Y;
    QHBoxLayout* m_pLytLinePixelsPt2;
    QLabel* m_pLblPixelsPt2;
    QLabel* m_pLblPixelsPt2X;
    QSpinBox* m_pEdtPixelsPt2X;
    QLabel* m_pLblPixelsPt2Y;
    QSpinBox* m_pEdtPixelsPt2Y;
    QHBoxLayout* m_pLytLinePixelsLengthAngle;
    QLabel* m_pLblPixelsLength;
    QSpinBox* m_pEdtPixelsLength;
    QLabel* m_pLblPixelsAngle;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsAngle;
    QHBoxLayout* m_pLytLinePixelsPos;
    QLabel* m_pLblPixelsPos;
    QLabel* m_pLblPixelsPosX;
    QSpinBox* m_pEdtPixelsPosX;
    QLabel* m_pLblPixelsPosY;
    QSpinBox* m_pEdtPixelsPosY;
    QHBoxLayout* m_pLytLinePixelsSize;
    QLabel* m_pLblPixelsWidth;
    QSpinBox* m_pEdtPixelsWidth;
    QLabel* m_pLblPixelsHeight;
    QSpinBox* m_pEdtPixelsHeight;

}; // class CWdgtGraphObjLineGeometryProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjLineGeometryPropertiesWdgt_h
