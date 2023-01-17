/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_WdgtFormatGraphObjsTextStyle_h
#define ZSDraw_WdgtFormatGraphObjsTextStyle_h

#include <QtCore/qglobal.h>

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawWdgtFormatGraphObjs.h"

class QComboBox;
class QCheckBox;
class QFontComboBox;
class QFrame;
class QLabel;
class QLineEdit;
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
class CTrcAdminObj;
}

namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtFormatGraphObjsTextStyle : public CWdgtFormatGraphObjs
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtFormatGraphObjsTextStyle"; }  // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // ctors and dtor
    CWdgtFormatGraphObjsTextStyle(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjsTextStyle();
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
protected slots: // Fill Style
    void onBtnTextColorClicked();
    void onCmbFontCurrentFontChanged( const QFont& );
    void onCmbSizeCurrentIndexChanged( int );
    void onChkFontStyleItalicStateChanged( int );
    void onChkFontStyleBoldStateChanged( int );
    void onChkTextEffectStrikeoutStateChanged( int );
    void onChkTextEffectUnderlineStateChanged( int );
protected: // instance methods
    void setTextColor( const CDrawSettings& i_drawSettings );
    void setFont( const CDrawSettings& i_drawSettings );
protected: // instance members
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    QHBoxLayout*             m_pLytTextColor;
    QLabel*                  m_pLblTextColor;
    QPixmap*                 m_pPxmBtnTextColor;
    QRect                    m_rctBtnTextColor;
    QPushButton*             m_pBtnTextColor;
    QHBoxLayout*             m_pLytFont;
    QLabel*                  m_pLblFont;
    QFontComboBox*           m_pCmbFont;
    QHBoxLayout*             m_pLytSize;
    QLabel*                  m_pLblSize;
    QComboBox*               m_pCmbSize;
    QHBoxLayout*             m_pLytSepLineFontStyles;
    QLabel*                  m_pLblSepLineFontStyles;
    QFrame*                  m_pSepLineFontStyles;
    QHBoxLayout*             m_pLytFontStyleItalic;
    QCheckBox*               m_pChkFontStyleItalic;
    QHBoxLayout*             m_pLytFontStyleBold;
    QCheckBox*               m_pChkFontStyleBold;
    QHBoxLayout*             m_pLytSepLineTextEffects;
    QLabel*                  m_pLblSepLineTextEffects;
    QFrame*                  m_pSepLineTextEffects;
    QHBoxLayout*             m_pLytTextEffectStrikeout;
    QCheckBox*               m_pChkTextEffectStrikeout;
    QHBoxLayout*             m_pLytTextEffectUnderline;
    QCheckBox*               m_pChkTextEffectUnderline;
    QHBoxLayout*             m_pLytSepLineSample;
    QLabel*                  m_pLblSepLineSample;
    QFrame*                  m_pSepLineSample;
    QHBoxLayout*             m_pLytSample;
    QLabel*                  m_pLblSample;
    // Trace
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtFormatGraphObjsTextStyle

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjsTextStyle_h
