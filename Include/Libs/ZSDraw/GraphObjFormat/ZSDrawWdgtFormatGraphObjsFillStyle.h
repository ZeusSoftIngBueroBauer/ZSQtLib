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

#ifndef ZSDraw_WdgtFormatGraphObjsFillStyle_h
#define ZSDraw_WdgtFormatGraphObjsFillStyle_h

#include <QtCore/qglobal.h>

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.h"

class QLabel;
class QLineEdit;
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
class ZSDRAWDLL_API CWdgtFormatGraphObjsFillStyle : public CWdgtFormatGraphObjs
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtFormatGraphObjsFillStyle"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
protected: // type definitions and constants
    enum EItemDataRole
    {
        EItemDataRoleFillStyle = Qt::UserRole+1,
        EItemDataRoleCount,
        EItemDataRoleUndefined
    };
public: // ctors and dtor
    CWdgtFormatGraphObjsFillStyle(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjsFillStyle();
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
    void fillFillStylesModel( QStandardItemModel* i_pModel );
protected: // overridables of base class QDialog
    virtual void mousePressEvent( QMouseEvent* i_pEv );
protected slots: // Fill Style
    void onBtnFillColorClicked();
    void onBtnFillStyleClicked();
    void onViewFillStylesClicked( const QModelIndex& i_modelIdx );
protected: // instance methods
    void setFillColor();
    void setFillStyle();
protected: // instance members
    QVBoxLayout*                  m_pLyt;
    QLabel*                       m_pLblHeadLine;
    QHBoxLayout*                  m_pLytFillColor;
    QLabel*                       m_pLblFillColor;
    QPixmap*                      m_pPxmBtnFillColor;
    QRect                         m_rctBtnFillColor;
    QLine                         m_lineBtnFillColor1;
    QLine                         m_lineBtnFillColor2;
    QLine                         m_lineBtnFillColor3;
    QPushButton*                  m_pBtnFillColor;
    QHBoxLayout*                  m_pLytFillStyle;
    QLabel*                       m_pLblFillStyle;
    ZS::System::GUI::CPushButton* m_pBtnFillStyle;
    QTableView*                   m_pViewFillStyles;
    QStandardItemModel*           m_pModelFillStyles;
    // Trace
    ZS::System::CTrcAdminObj*      m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsFillStyle

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsFillStyle_h
