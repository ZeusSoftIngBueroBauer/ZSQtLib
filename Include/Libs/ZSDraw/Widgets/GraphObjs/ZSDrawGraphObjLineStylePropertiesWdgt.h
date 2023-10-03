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
private: // auxiliary instance methods
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
    QWidget* m_pWdgtStyle;
    QVBoxLayout* m_pLytWdgtStyle;
    QHBoxLayout* m_pLytPenColor;
    QLabel* m_pLblPenColor;
    QPixmap* m_pPxmBtnPenColor;
    QRect m_rctBtnPenColor;
    QPushButton* m_pBtnPenColor;
    QHBoxLayout* m_pLytPenWidth;
    QLabel* m_pLblPenWidth;
    QLineEdit* m_pEdtPenWidth;
    QWidget* m_pWdgtLineStyle;
    QVBoxLayout* m_pLytWdgtLineStyle;
    QHBoxLayout* m_pLytLineStyle;
    QLabel* m_pLblLineStyle;
    QPushButton* m_pBtnLineStyle;
    QListView* m_pViewLineStyles;
    QStandardItemModel* m_pModelLineStyles;
    QWidget* m_pWdgtLineEndSettings;
    QVBoxLayout* m_pLytWdgtLineEndSettings;
    QHBoxLayout* m_pLytLineEndSettings;
    QLabel* m_pLblLineEndSettings;
    ZS::System::GUI::CSepLine* m_pSepLineEndSettings;
    QHBoxLayout* m_pLytLineEndStyles;
    QLabel* m_pLblLineEndStylesLineStart;
    QPushButton* m_pBtnLineEndStylesLineStart;
    QTableView* m_pViewLineEndStylesLineStart;
    QStandardItemModel* m_pModelLineEndStylesLineStart;
    QLabel* m_pLblLineEndStylesLineEnd;
    QPushButton* m_pBtnLineEndStylesLineEnd;
    QTableView* m_pViewLineEndStylesLineEnd;
    QStandardItemModel* m_pModelLineEndStylesLineEnd;
    QHBoxLayout* m_pLytLineEndSizes;
    QLabel* m_pLblLineEndSizesLineStart;
    QPushButton* m_pBtnLineEndSizesLineStart;
    QTableView* m_pViewLineEndSizesLineStart;
    QStandardItemModel* m_pModelLineEndSizesLineStart;
    QLabel* m_pLblLineEndSizesLineEnd;
    QPushButton* m_pBtnLineEndSizesLineEnd;
    QTableView* m_pViewLineEndSizesLineEnd;
    QStandardItemModel* m_pModelLineEndSizesLineEnd;

}; // class CWdgtGraphObjLineStyleProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjLineStylePropertiesWdgt_h
