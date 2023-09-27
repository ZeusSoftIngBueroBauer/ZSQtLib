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

#ifndef ZSDiagram_ScalePropertiesWdgt_h
#define ZSDiagram_ScalePropertiesWdgt_h

#include "ZSDiagramGUI/ZSDiagramItemPropertiesBaseWdgt.h"
#include "ZSDiagram/ZSDiagScale.h"

class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSpinBox;

namespace ZS
{
namespace Diagram
{
class CDiagScale;

namespace GUI
{
//******************************************************************************
class ZSDIAGRAMGUIDLL_API CWdgtDiagramScaleProperties : public CWdgtDiagramItemPropertiesBase
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString ClassName() { return "CWdgtDiagramScaleProperties"; }
public: // ctors and dtor
    CWdgtDiagramScaleProperties(
        CWdgtDiagram* i_pDiagram,
        ZS::System::EMode i_mode = ZS::System::EMode::View,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtDiagramScaleProperties();
protected: // overridable init methods of base class CWdgtDiagramItemPropertiesBase
    void setMode(ZS::System::EMode i_mode) override;
public: // overridables of base class CWdgtDiagramItemPropertiesBase
    void setDiagItemObjName(const QString& i_strObjName) override;
public: // overridables of base class CWdgtDiagramItemPropertiesBase
    bool hasChanges() const override;
    void acceptChanges() override;
    void rejectChanges() override;
protected slots:
    void onDiagItemScaleSpacingChanged(const ZS::System::CEnumSpacing& i_spacing);
    void onDiagItemScaleGeometryChanged(int i_iMinVal_px, int i_iMaxVal_px);
    void onDiagItemScaleDivLineDistMinPixChanged(ZS::System::EDivLineLayer i_layer, int i_iDivLineDistMin_px);
    void onDiagItemScaleChanged(const CScale& i_scale);
protected slots:
    void onCmbSpacingCurrentIndexChanged(int i_idx);
    void onEdtDivLineDistMinMainLayerPixValueChanged(int i_iVal);
    void onEdtDivLineDistMinSubLayerPixValueChanged(int i_iVal);
    void onCmbScaleUnitCurrentIndexChanged(int i_idx);
    void onEdtScaleMinValValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtScaleMaxValValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtScaleRangeValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
protected: // instance methods
    void setScaleAxis(const ZS::System::CEnumScaleAxis& i_scaleAxis);
protected slots:
    void onDiagScaleDestroyed(QObject* i_pObj);
protected: // instance members
    CDiagScale* m_pDiagScale;
    // Caching values
    ZS::System::CEnumScaleAxis m_scaleAxis;
    ZS::System::CEnumSpacing m_spacing;
    int m_iMinVal_px;
    int m_iMaxVal_px;
    QVector<int> m_ariDivLineDistMin_px; //[EDivLineLayerCount];
    CScale m_scale;
    // Geometry
    QGroupBox* m_pGrpGeometry;
    QVBoxLayout* m_pLytGrpGeometry;
    QHBoxLayout* m_pLytLineScaleAxis;
    QLabel* m_pLblScaleAxis;
    QComboBox* m_pCmbScaleAxis;
    QLabel* m_pLblSpacing;
    QComboBox* m_pCmbSpacing;
    QLabel* m_pLblRange_px;
    QLineEdit* m_pEdtRange_px;
    QHBoxLayout* m_pLytSepLineDivLineDistMin;
    QLabel* m_pLblSepLineDivLineDistMin;
    ZS::System::GUI::CSepLine* m_pSepLineDivLineDistMin;
    QHBoxLayout* m_pLytLineDivLineDistMin;
    QLabel* m_pLblDivLineDistMinLayerMain_px;
    QSpinBox* m_pEdtDivLineDistMinLayerMain_px;
    QLabel* m_pLblDivLineDistMinLayerSub_px;
    QSpinBox* m_pEdtDivLineDistMinLayerSub_px;
    // Scale Values
    QGroupBox* m_pGrpScaleValues;
    QVBoxLayout* m_pLytGrpScaleValues;
    QHBoxLayout* m_pLytLineUnitAndRes;
    QLabel* m_pLblScaleUnit;
    QComboBox* m_pCmbScaleUnit;
    QLabel* m_pLblScaleRes;
    QLineEdit* m_pEdtScaleRes;
    QHBoxLayout* m_pLytLineScaleRange;
    QLabel* m_pLblScaleMinVal;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtScaleMinVal;
    QLabel* m_pLblScaleMaxVal;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtScaleMaxVal;
    QLabel* m_pLblScaleRangeVal;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtScaleRangeVal;

}; // class CWdgtDiagramScaleProperties

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ScalePropertiesWdgt_h
