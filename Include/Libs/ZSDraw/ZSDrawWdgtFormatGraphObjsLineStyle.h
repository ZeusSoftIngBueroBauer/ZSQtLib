/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjs.h"

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
namespace Trace
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
public: // must overridables of base class CWdgtFormatGraphObjs
    virtual void applyChanges();
    virtual void resetChanges();
    virtual bool hasChanges() const;
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
    void onViewLineStylesClicked( QModelIndex i_modelIdx );
protected slots: // Line End Styles
    void onBtnLineEndStylesLineStartClicked();
    void onViewLineEndStylesLineStartClicked( QModelIndex i_modelIdx );
    void onBtnLineEndStylesLineEndClicked();
    void onViewLineEndStylesLineEndClicked( QModelIndex i_modelIdx );
protected slots: // Line End Sizes
    void onBtnLineEndSizesLineStartClicked();
    void onViewLineEndSizesLineStartClicked( QModelIndex i_modelIdx );
    void onBtnLineEndSizesLineEndClicked();
    void onViewLineEndSizesLineEndClicked( QModelIndex i_modelIdx );
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
    ZS::Trace::CTrcAdminObj*      m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsLineStyle

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsLineStyle_h
