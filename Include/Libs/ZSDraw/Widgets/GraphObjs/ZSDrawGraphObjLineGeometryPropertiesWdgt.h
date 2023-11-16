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
namespace PhysVal
{
namespace GUI
{
class CEditPhysValtemDelegate;
}
}
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
    void acceptChanges() override;
    void rejectChanges() override;
protected slots: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void onDrawingSceneDrawingSizeChanged(const CDrawingSize& i_drawingSize) override;
protected slots:
    void onBtnCollapseClicked(bool i_bChecked = false);
    void onBtnMetricResizeRowsAndColumnsToContentsClicked(bool i_bChecked = false);
    void onBtnPixelsResizeRowsAndColumnsToContentsClicked(bool i_bChecked = false);
protected slots:
    void onModelMetricGeometryContentChanged();
    void onModelPixelsGeometryContentChanged();
    //void onDlgEditPropertyDestroyed(QObject* i_pObj = nullptr);
private: // auxiliary instance methods
    //void registerEditPropertyDialog(ZS::PhysVal::GUI::CWdgtEditPhysVal* i_pEdtWidget);
protected: // overridables of base class QObject
    //bool eventFilter(QObject* i_pObjWatched, QEvent* i_pEv) override;
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
    ZS::PhysVal::GUI::CEditPhysValtemDelegate* m_pEdtPhysValDelegateMetricXVal;
    ZS::PhysVal::GUI::CEditPhysValtemDelegate* m_pEdtPhysValDelegateMetricYVal;
    CModelGraphObjGeometry* m_pModelMetricGeometry;
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
    ZS::PhysVal::GUI::CEditPhysValtemDelegate* m_pEdtPhysValDelegatePixelsXVal;
    ZS::PhysVal::GUI::CEditPhysValtemDelegate* m_pEdtPhysValDelegatePixelsYVal;
    CModelGraphObjGeometry* m_pModelPixelsGeometry;
    // Dialog to edit and immediately apply changed properties.
    //QHash<QString, QWidget*> m_hshpRegisteredEditPropertyDialogs;
    //CDlgGraphObjLineGeometryEditProperty* m_pDlgEditProperty;

}; // class CWdgtGraphObjLineGeometryProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjLineGeometryPropertiesWdgt_h
