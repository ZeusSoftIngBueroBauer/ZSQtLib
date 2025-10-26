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

#ifndef ZSDraw_DrawGraphObjPaintOptionsPropertiesWdgt_h
#define ZSDraw_DrawGraphObjPaintOptionsPropertiesWdgt_h

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h"

namespace ZS
{
namespace System
{
namespace GUI
{
class CTableView;
}
}
namespace Draw
{
class CModelGraphObjPaintOptions;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjPaintOptionsProperties : public CWdgtGraphObjPropertiesAbstract
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtGraphObjPaintOptionsProperties"; }
public: // ctors and dtor
    CWdgtGraphObjPaintOptionsProperties(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtGraphObjPaintOptionsProperties();
public: // instance methods
    void expand(bool i_bExpand);
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool setKeyInTree(const QString& i_strKeyInTree) override;
public: // overridables of base class CWdgtGraphObjPropertiesAbstract
    bool hasErrors() const override;
    bool hasChanges() const override;
    void acceptChanges() override;
    void rejectChanges() override;
protected slots:
    void onBtnCollapseClicked(bool i_bChecked = false);
    void onBtnResizeRowsAndColumnsToContentsClicked(bool i_bChecked = false);
protected slots:
    void onModelPaintOptionsContentChanged();
private: // instance members
    /*!< Edit controls. */
    QWidget* m_pWdgtHeadline = nullptr;
    QHBoxLayout* m_pLytWdgtHeadline = nullptr;
    QPixmap m_pxmBtnDown;
    QPixmap m_pxmBtnUp;
    QPushButton* m_pBtnCollapse = nullptr;
    QLabel* m_pLblHeadlineIcon = nullptr;
    QLabel* m_pLblHeadline = nullptr;
    ZS::System::GUI::CSepLine* m_pSepHeadline = nullptr;
    QWidget* m_pWdgtPaintOptions = nullptr;
    QVBoxLayout* m_pLytWdgtPaintOptions = nullptr;
    QHBoxLayout* m_pLytLineEditButtons = nullptr;
    QPushButton* m_pBtnResizeRowsAndColumnsToContents = nullptr;
    QVBoxLayout* m_pLytListView = nullptr;
    ZS::System::GUI::CTableView* m_pTableView = nullptr;
    CModelGraphObjPaintOptions* m_pModel = nullptr;

}; // class CWdgtGraphObjPaintOptionsProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjPaintOptionsPropertiesWdgt_h
