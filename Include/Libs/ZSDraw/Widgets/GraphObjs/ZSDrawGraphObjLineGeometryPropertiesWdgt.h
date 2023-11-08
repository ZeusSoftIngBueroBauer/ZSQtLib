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

class QTableView;

namespace ZS
{
namespace Draw
{
class CDlgGraphObjLineGeometryEditProperty;
class CModelGraphObjGeometry;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjLineGeometryProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    static const QString c_strCoorPoint1;
    static const QString c_strCoorPoint2;
    static const QString c_strCoorCenter;
    static const QString c_strCoorWidth;
    static const QString c_strCoorHeight;
    static const QString c_strCoorLength;
    static const QString c_strCoorAngle;
    static const QString c_strCoorX;
    static const QString c_strCoorY;
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGraphObjLineGeometryProperties"; }
public: // ctors and dtor
    CWdgtGraphObjLineGeometryProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjLineGeometryProperties();
public: // instance methods
    void expand(bool i_bExpand);
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool setKeyInTree(const QString& i_strKeyInTree) override;
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasErrors() const override;
    bool hasChanges() const override;
    void applySettings(bool i_bImmediatelyApplySettings = true) override;
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void fillEditControls() override;
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize) override;
protected slots:
    void onBtnCollapseClicked(bool i_bChecked = false);
    void onBtnMetricResizeRowsAndColumnsToContentsClicked(bool i_bChecked = false);
    void onBtnPixelsResizeRowsAndColumnsToContentsClicked(bool i_bChecked = false);
protected slots:
    void onModelMetricGeometryContentChanged();
    void onModelPixelsGeometryContentChanged();
    void onEdtMetricPt1XValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricPt1YValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricPt2XValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricPt2YValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricPtCenterXValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricPtCenterYValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricWidthValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricHeightValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricLengthValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtMetricAngleValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPt1XValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPt1YValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPt2XValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPt2YValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPtCenterXValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsPtCenterYValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsWidthValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsHeightValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsLengthValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onEdtPixelsAngleValueChanged(const ZS::PhysVal::CPhysVal& i_physVal);
    void onDlgEditPropertyDestroyed(QObject* i_pObj = nullptr);
private: // auxiliary instance methods
    void fillEditControls(const CPhysValLine& i_physValLine);
    void registerEditPropertyDialog(ZS::PhysVal::GUI::CWdgtEditPhysVal* i_pEdtWidget);
protected: // overridables of base class QObject
    bool eventFilter(QObject* i_pObjWatched, QEvent* i_pEv) override;
private: // instance members
    /*!< Cached coordinates of the graphical object.
         The values are stored in the unit of the drawing size.
         If the drawing size changes the coordinates are updated and converted if necessary. */
    CPhysValLine m_physValLine;
    /*!< Edit controls. */
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
    /*!< Geometry in Metric System */
    QWidget* m_pWdgtMetric;
    QVBoxLayout* m_pLytWdgtMetric;
    QWidget* m_pWdgtSepLineMetricGeometry;
    QHBoxLayout* m_pLytSepLineMetricGeometry;
    QLabel* m_pLblSepLineMetricGeometry;
    ZS::System::GUI::CSepLine* m_pSepLineMetricGeometry;
    /*!< Metric Table View */
    QHBoxLayout* m_pLytLineMetricTableViewButtons;
    QPushButton* m_pBtnMetricResizeRowsAndColumnsToContents;
    QVBoxLayout* m_pLytMetricGeometryListView;
    QTableView* m_pTableViewMetricGeometry;
    CModelGraphObjGeometry* m_pModelMetricGeometry;
    /*!< Metric Edit Controls */
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
    QHBoxLayout* m_pLytLineMetricPtCenter;
    QLabel* m_pLblMetricPtCenter;
    QLabel* m_pLblMetricPtCenterX;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricPtCenterX;
    QLabel* m_pLblMetricPtCenterY;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricPtCenterY;
    QHBoxLayout* m_pLytLineMetricSize;
    QLabel* m_pLblMetricWidth;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricWidth;
    QLabel* m_pLblMetricHeight;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricHeight;
    QHBoxLayout* m_pLytLineMetricAngle;
    QLabel* m_pLblMetricLength;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricLength;
    QLabel* m_pLblMetricAngle;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtMetricAngle;
    /*!< Geometry in Pixels */
    QWidget* m_pWdgtPixels;
    QVBoxLayout* m_pLytWdgtPixels;
    QWidget* m_pWdgtSepLinePixelsGeometry;
    QHBoxLayout* m_pLytSepLinePixelsGeometry;
    QLabel* m_pLblSepLinePixelsGeometry;
    ZS::System::GUI::CSepLine* m_pSepLinePixelsGeometry;
    /*!< Pixels Table View */
    QHBoxLayout* m_pLytLinePixelsTableViewButtons;
    QPushButton* m_pBtnPixelsResizeRowsAndColumnsToContents;
    QVBoxLayout* m_pLytPixelsGeometryListView;
    QTableView* m_pTableViewPixelsGeometry;
    CModelGraphObjGeometry* m_pModelPixelsGeometry;
    /*!< Pixels Edit Controls */
    QHBoxLayout* m_pLytLinePixelsPt1;
    QLabel* m_pLblPixelsPt1;
    QLabel* m_pLblPixelsPt1X;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsPt1X;
    QLabel* m_pLblPixelsPt1Y;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsPt1Y;
    QHBoxLayout* m_pLytLinePixelsPt2;
    QLabel* m_pLblPixelsPt2;
    QLabel* m_pLblPixelsPt2X;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsPt2X;
    QLabel* m_pLblPixelsPt2Y;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsPt2Y;
    QHBoxLayout* m_pLytLinePixelsPtCenter;
    QLabel* m_pLblPixelsPtCenter;
    QLabel* m_pLblPixelsPtCenterX;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsPtCenterX;
    QLabel* m_pLblPixelsPtCenterY;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsPtCenterY;
    QHBoxLayout* m_pLytLinePixelsSize;
    QLabel* m_pLblPixelsWidth;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsWidth;
    QLabel* m_pLblPixelsHeight;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsHeight;
    QHBoxLayout* m_pLytLinePixelsAngle;
    QLabel* m_pLblPixelsLength;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsLength;
    QLabel* m_pLblPixelsAngle;
    ZS::PhysVal::GUI::CWdgtEditPhysVal* m_pEdtPixelsAngle;
    // Dialog to edit and immediately apply changed properties.
    QHash<QString, QWidget*> m_hshpRegisteredEditPropertyDialogs;
    CDlgGraphObjLineGeometryEditProperty* m_pDlgEditProperty;

}; // class CWdgtGraphObjLineGeometryProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjLineGeometryPropertiesWdgt_h
