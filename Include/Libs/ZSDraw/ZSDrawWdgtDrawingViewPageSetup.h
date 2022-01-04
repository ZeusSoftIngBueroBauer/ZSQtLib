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

#ifndef ZSDraw_WgdtDrawingViewPageSetup_h
#define ZSDraw_WgdtDrawingViewPageSetup_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/ZSDrawDllMain.h"

class QFrame;
class QLabel;
class QSpinBox;
class QVBoxLayout;
class QHBoxLayout;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingView;

//******************************************************************************
class ZSDRAWDLL_API CWdgtDrawingViewPageSetup : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtDrawingViewPageSetup"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // instance methods
    CWdgtDrawingViewPageSetup( CDrawingView* i_pDrawingView, QWidget* i_pWdgtParent = nullptr );
    ~CWdgtDrawingViewPageSetup();
signals:
    void settingsChanged();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // instance methods
    bool hasChanges() const;
    void acceptChanges() const;
protected slots:
    void onEdtSizeWidthEditingFinished();
    void onEdtSizeHeightEditingFinished();
    void onEdtMarginsLeftEditingFinished();
    void onEdtMarginsTopEditingFinished();
    void onEdtMarginsRightEditingFinished();
    void onEdtMarginsBottomEditingFinished();
private: // instance members
    CDrawingView*            m_pDrawingView;
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    QHBoxLayout*             m_pLytResolutionDpi;
    QLabel*                  m_pLblResolutionDpi;
    QLabel*                  m_pLblResolutionDpiVal;
    QLabel*                  m_pLblResolutionDpiUnit;
    QHBoxLayout*             m_pLytResolutionDpmm;
    QLabel*                  m_pLblResolutionDpmm;
    QLabel*                  m_pLblResolutionDpmmVal;
    QLabel*                  m_pLblResolutionDpmmUnit;
    QHBoxLayout*             m_pLytSepLineSize;
    QLabel*                  m_pLblSepLineSize;
    QFrame*                  m_pSepLineSize;
    QHBoxLayout*             m_pLytSizeWidth;
    QLabel*                  m_pLblSizeWidth;
    QSpinBox*                m_pEdtSizeWidth;
    QHBoxLayout*             m_pLytSizeHeight;
    QLabel*                  m_pLblSizeHeight;
    QSpinBox*                m_pEdtSizeHeight;
    QHBoxLayout*             m_pLytSepLineMargins;
    QLabel*                  m_pLblSepLineMargins;
    QFrame*                  m_pSepLineMargins;
    QHBoxLayout*             m_pLytMarginsLeft;
    QLabel*                  m_pLblMarginsLeft;
    QSpinBox*                m_pEdtMarginsLeft;
    QHBoxLayout*             m_pLytMarginsTop;
    QLabel*                  m_pLblMarginsTop;
    QSpinBox*                m_pEdtMarginsTop;
    QHBoxLayout*             m_pLytMarginsRight;
    QLabel*                  m_pLblMarginsRight;
    QSpinBox*                m_pEdtMarginsRight;
    QHBoxLayout*             m_pLytMarginsBottom;
    QLabel*                  m_pLblMarginsBottom;
    QSpinBox*                m_pEdtMarginsBottom;
    // Trace
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // CWdgtDrawingViewPageSetup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WgdtDrawingViewPageSetup_h