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

#ifndef ZSDraw_DrawGraphObjLineStylePropertiesWdgt_h
#define ZSDraw_DrawGraphObjLineStylePropertiesWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"

class QTableView;

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjLineStyleProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGraphObjLineStyleProperties"; }
protected: // type definitions and constants
protected: // type definitions and constants
    enum class EItemDataRoleModelLineEndStyle {
        Id = Qt::UserRole+1,
        LineEndStyle,
        ArrowHeadFillStyle,
        ArrowHeadBaseLineType
    };
public: // ctors and dtor
    CWdgtGraphObjLineStyleProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strParentClassName,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjLineStyleProperties();
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasChanges() const override;
protected: // overridables of base class CWdgtGraphObjPropertiesAbstract
    void fillEditControls() override;
    void applySettings() override;
protected slots:
    void onBtnCollapseClicked(bool i_bChecked = false);
protected slots:
    void onBtnPenColorClicked(bool i_bChecked = false);
    void onCmbLineStyleCurrentIndexChanged(int i_idx);
    void onEdtLineWidthValueChanged(int i_iVal);
    void onCmbP1LineEndStyleCurrentIndexChanged(int i_idx);
    void onCmbP1ArrowHeadWidthClicked(int i_idx);
    void onCmbP1ArrowHeadLengthClicked(int i_idx);
    void onCmbP2LineEndStyleCurrentIndexChanged(int i_idx);
    void onCmbP2ArrowHeadWidthClicked(int i_idx);
    void onCmbP2ArrowHeadLengthClicked(int i_idx);
private: // auxiliary instance methods
    QSize fillLineStylesModel();
    QSize fillLineEndStylesModel(const CEnumLinePoint& i_linePoint);
    void fillArrowHeadWidthModel(const CEnumLinePoint& i_linePoint);
    void fillArrowHeadLengthModel(const CEnumLinePoint& i_linePoint);
private: // auxiliary instance methods
    void updateBtnPenColor(const QColor& i_clr);
    void updateCmbLineStyle(const CEnumLineStyle& i_lineStyle);
    void updateCmbLineEndStyle(
        const CEnumLinePoint& i_linePoint,
        const CEnumLineEndStyle& i_lineEndStyle,
        const CEnumArrowHeadFillStyle& i_arrowHeadFillStyle,
        const CEnumArrowHeadBaseLineType& i_arrowHeadBaseLineType);
    void updateCmbArrowHeadWidth(
        const CEnumLinePoint& i_linePoint,
        const CEnumArrowHeadWidth& i_lineEndWidth);
    void updateCmbArrowHeadLength(
        const CEnumLinePoint& i_linePoint,
        const CEnumArrowHeadLength& i_lineEndLength);
protected: // instance methods (method tracing)
    void traceValues(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_methodDir);
private: // instance members
    /*!< Cached draw settings of the graphical object. */
    CDrawSettings m_drawSettings;
    /*!< Edit controls. */
    QWidget* m_pWdgtHeadline;
    QHBoxLayout* m_pLytWdgtHeadline;
    QPixmap m_pxmBtnDown;
    QPixmap m_pxmBtnUp;
    QPushButton* m_pBtnCollapse;
    QLabel* m_pLblHeadlineIcon;
    QLabel* m_pLblHeadline;
    ZS::System::GUI::CSepLine* m_pSepHeadline;
    QWidget* m_pWdgtLineStyleSettings;
    QGridLayout* m_pLytWdgtLineStyleSettings;
    // Whole Line
    QLabel* m_pLblLineStyle;
    QPixmap* m_pPxmBtnLineColor;
    QPushButton* m_pBtnLineColor;
    QStandardItemModel* m_pModelLineStyles;
    QComboBox* m_pCmbLineStyle;
    QSpinBox* m_pEdtLineWidth;
    // Arrow Heads
    QLabel* m_pLblLineEndStyles;
    // P1 (LineStart)
    QLabel* m_pLblP1LineEndStyles;
    QStandardItemModel* m_pModelP1LineEndStyles;
    QComboBox* m_pCmbP1LineEndStyle;
    QStandardItemModel* m_pModelP1ArrowHeadWidth;
    QComboBox* m_pCmbP1ArrowHeadWidth;
    QStandardItemModel* m_pModelP1ArrowHeadLength;
    QComboBox* m_pCmbP1ArrowHeadLength;
    // P2 (LineEnd)
    QLabel* m_pLblP2LineEndStyles;
    QStandardItemModel* m_pModelP2LineEndStyles;
    QComboBox* m_pCmbP2LineEndStyle;
    QStandardItemModel* m_pModelP2ArrowHeadWidth;
    QComboBox* m_pCmbP2ArrowHeadWidth;
    QStandardItemModel* m_pModelP2ArrowHeadLength;
    QComboBox* m_pCmbP2ArrowHeadLength;

}; // class CWdgtGraphObjLineStyleProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjLineStylePropertiesWdgt_h
