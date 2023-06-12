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

#ifndef ZSDiagram_ItemPropertiesBaseWdgt_h
#define ZSDiagram_ItemPropertiesBaseWdgt_h

#include "ZSDiagramGUI/ZSDiagramGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
class CSepLine; // as often used forward declaration here in base widget
}
}
namespace PhysVal
{
namespace GUI
{
class CWdgtEditPhysVal; // as often used forward declaration here in base widget
}
}

namespace Diagram
{
class CWdgtDiagram;

namespace GUI
{
//******************************************************************************
class ZSDIAGRAMGUIDLL_API CWdgtDiagramItemPropertiesBase : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Diagram::GUI"; }
    static QString ClassName() { return "CWdgtDiagramItemPropertiesBase"; }
public: // ctors and dtor
    CWdgtDiagramItemPropertiesBase(
        const QString& i_strWdgtClassName,
        CWdgtDiagram* i_pDiagram,
        const QString& i_strDiagItemClassName,
        ZS::System::EMode i_mode = ZS::System::EMode::View,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtDiagramItemPropertiesBase();
public: // instance methods
    QString className() const { return m_strWdgtClassName; }
signals:
    void modeChanged();
    void diagItemDestroyed(QObject* i_pObj);
    void diagItemObjNameChanged();
    void diagItemPropertyChanged();
protected: // overridable init methods
    virtual void setMode(ZS::System::EMode i_mode);
    virtual void addButtonsWidget();
public: // instance methods
    QString diagItemClassName() const { return m_strDiagItemClassName; };
public: // overridables
    virtual void setDiagItemObjName( const QString& i_strObjName );
    QString diagItemObjName() const;
public: // overridables
    virtual bool hasChanges() const;
    virtual void acceptChanges();
    virtual void rejectChanges();
protected slots:
    virtual void onBtnEditClicked(bool i_bChecked = false);
protected slots:
    virtual void onDiagramAboutToBeDestroyed(const QString& i_strObjName);
protected: // instance methods (method tracing)
    void emit_modeChanged();
    void emit_diagItemDestroyed(QObject* i_pObj);
    void emit_diagItemObjNameChanged();
    void emit_diagItemPropertyChanged();
protected: // instance members
    QString m_strWdgtClassName;
    CWdgtDiagram* m_pDiagram;
    ZS::System::EMode m_mode;
    QString m_strDiagItemClassName;
    QString m_strDiagItemObjName;
    int m_cxLblWidthClm1;
    int m_cxEdtWidthClm1;
    int m_cxLblWidthClm2;
    int m_cxEdtWidthClm2;
    int m_cxLblWidthClm3;
    int m_cxEdtWidthClm3;
    int m_cxLblWidthClm4;
    int m_cxEdtWidthClm4;
    int m_cxClmSpacing;
    /*!< Blocking signals counter. */
    int m_iValueChangedSignalsBlocked;
    // Edit Controls
    QVBoxLayout* m_pLyt;
    // Button Line
    QWidget* m_pWdgtButtons;
    QHBoxLayout* m_pLytWdgtButtons;
    QPushButton* m_pBtnEdit;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtDiagramItemPropertiesBase

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ItemPropertiesBaseWdgt_h
