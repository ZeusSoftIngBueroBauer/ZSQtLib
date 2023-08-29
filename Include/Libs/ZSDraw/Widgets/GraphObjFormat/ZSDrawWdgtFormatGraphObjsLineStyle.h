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

#ifndef ZSDraw_WdgtFormatGraphObjsLineStyle_h
#define ZSDraw_WdgtFormatGraphObjsLineStyle_h

#include <QtCore/qglobal.h>

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.h"

class QFrame;
class QLabel;
class QLineEdit;
class QListView;
class QListWidgetItem;
class QPixmap;
class QPushButton;
class QStandardItemModel;
class QTableView;

class QLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
class CPushButton;
}
}
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtFormatGraphObjsLineStyle : public CWdgtFormatGraphObjs
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtFormatGraphObjsLineStyle"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
protected: // type definitions and constants
    enum EItemDataRole
    {
        EItemDataRoleLineStyle    = Qt::UserRole+1,
        EItemDataRoleLineEndStyle,
        EItemDataRoleLineEndBaseLineType,
        EItemDataRoleLineEndFillStyle,
        EItemDataRoleLineEndWidth,
        EItemDataRoleLineEndLength,
        EItemDataRoleCount,
        EItemDataRoleUndefined
    };
public: // ctors and dtor
    CWdgtFormatGraphObjsLineStyle(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjsLineStyle();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // must overridables of base class CWdgtFormatGraphObjs
    virtual void applyChanges() override;
    virtual void resetChanges() override;
    virtual bool hasChanges() const override;
protected: // must overridables of base class CWdgtFormatGraphObjs
    virtual void onGraphObjChanged() override;
protected: // instance methods
    void fillLineStylesModel( QStandardItemModel* i_pModel );
    void fillLineEndStylesModel( ELinePoint i_linePoint, QStandardItemModel* i_pModel );
    void fillLineEndSizesModel( ELinePoint i_linePoint, QStandardItemModel* i_pModel );
protected: // overridables of base class QWidget
    virtual void mousePressEvent( QMouseEvent* i_pEv );
protected slots: // Line Style
    void onBtnPenColorClicked();
    void onEdtPenWidthTextChanged( const QString& i_strText );
    void onBtnLineStyleClicked();
    void onViewLineStylesClicked( const QModelIndex& i_modelIdx );
protected slots: // Line End Styles
    void onBtnLineEndStylesLineStartClicked();
    void onViewLineEndStylesLineStartClicked( const QModelIndex& i_modelIdx );
    void onBtnLineEndStylesLineEndClicked();
    void onViewLineEndStylesLineEndClicked( const QModelIndex& i_modelIdx );
protected slots: // Line End Sizes
    void onBtnLineEndSizesLineStartClicked();
    void onViewLineEndSizesLineStartClicked( const QModelIndex& i_modelIdx );
    void onBtnLineEndSizesLineEndClicked();
    void onViewLineEndSizesLineEndClicked( const QModelIndex& i_modelIdx );
protected: // instance methods
    void setPenColor( const CDrawSettings& i_drawSettings );
    void setLineStyle( const CDrawSettings& i_drawSettings );
    void setLineEndStyle( ELinePoint i_linePoint, const CDrawSettings& i_drawSettings );
    void setLineEndSize( ELinePoint i_linePoint, const CDrawSettings& i_drawSettings );
protected: // instance members
    QVBoxLayout*                  m_pLyt;
    QLabel*                       m_pLblHeadLine;
    QHBoxLayout*                  m_pLytPenColor;
    QLabel*                       m_pLblPenColor;
    QPixmap*                      m_pPxmBtnPenColor;
    QRect                         m_rctBtnPenColor;
    QLine                         m_lineBtnPenColor1;
    QLine                         m_lineBtnPenColor2;
    QLine                         m_lineBtnPenColor3;
    QPushButton*                  m_pBtnPenColor;
    QHBoxLayout*                  m_pLytPenWidth;
    QLabel*                       m_pLblPenWidth;
    QLineEdit*                    m_pEdtPenWidth;
    QWidget*                      m_pWdgtLineStyle;
    QVBoxLayout*                  m_pLytWdgtLineStyle;
    QHBoxLayout*                  m_pLytLineStyle;
    QLabel*                       m_pLblLineStyle;
    ZS::System::GUI::CPushButton* m_pBtnLineStyle;
    QListView*                    m_pViewLineStyles;
    QStandardItemModel*           m_pModelLineStyles;
    QWidget*                      m_pWdgtLineEndSettings;
    QVBoxLayout*                  m_pLytWdgtLineEndSettings;
    QHBoxLayout*                  m_pLytLineEndSettings;
    QLabel*                       m_pLblLineEndSettings;
    QFrame*                       m_pSepLineEndSettings;
    QHBoxLayout*                  m_pLytLineEndStyles;
    QLabel*                       m_pLblLineEndStylesLineStart;
    ZS::System::GUI::CPushButton* m_pBtnLineEndStylesLineStart;
    QTableView*                   m_pViewLineEndStylesLineStart;
    QStandardItemModel*           m_pModelLineEndStylesLineStart;
    QLabel*                       m_pLblLineEndStylesLineEnd;
    ZS::System::GUI::CPushButton* m_pBtnLineEndStylesLineEnd;
    QTableView*                   m_pViewLineEndStylesLineEnd;
    QStandardItemModel*           m_pModelLineEndStylesLineEnd;
    QHBoxLayout*                  m_pLytLineEndSizes;
    QLabel*                       m_pLblLineEndSizesLineStart;
    ZS::System::GUI::CPushButton* m_pBtnLineEndSizesLineStart;
    QTableView*                   m_pViewLineEndSizesLineStart;
    QStandardItemModel*           m_pModelLineEndSizesLineStart;
    QLabel*                       m_pLblLineEndSizesLineEnd;
    ZS::System::GUI::CPushButton* m_pBtnLineEndSizesLineEnd;
    QTableView*                   m_pViewLineEndSizesLineEnd;
    QStandardItemModel*           m_pModelLineEndSizesLineEnd;
    // Trace
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsLineStyle

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsLineStyle_h
