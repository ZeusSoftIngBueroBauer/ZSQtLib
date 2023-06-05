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

#ifndef ZSDiagram_ObjPropertiesAbstractWdgt_h
#define ZSDiagram_ObjPropertiesAbstractWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDiagramGUI/ZSDiagramGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Diagram
{
class CWdgtDiagram;

namespace GUI
{
//******************************************************************************
class ZSDIAGRAMGUIDLL_API CWdgtDiagramObjPropertiesAbstract : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Diagram::GUI"; }
    static QString ClassName() { return "CWdgtDiagramObjPropertiesAbstract"; }
public: // ctors and dtor
    CWdgtDiagramObjPropertiesAbstract(
        CWdgtDiagram* i_pDiagram,
        const QString& i_strDiagObjClassName,
        ZS::System::EMode i_mode = ZS::System::EMode::View,
        QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtDiagramObjPropertiesAbstract();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // overridables
    QString diagObjClassName() const { return m_strDiagObjClassName; };
public: // overridables
    virtual void setDiagObjName( const QString& i_strObjName );
    QString diagObjName() const;
public: // overridables
    virtual bool hasChanges() const;
    virtual void acceptChanges();
    virtual void rejectChanges();
protected: // instance methods
    virtual void setMode(ZS::System::EMode i_mode);
    ZS::System::EMode mode() const;
protected slots:
    virtual void onBtnEditClicked(bool i_bChecked = false);
protected slots:
    virtual void onDiagramAboutToBeDestroyed(const QString& i_strObjName);
protected: // instance members
    CWdgtDiagram* m_pDiagram;
    ZS::System::EMode m_mode;
    QString m_strDiagObjClassName;
    QString m_strDiagObjName;
    int m_cxLblWidthClm1;
    int m_cxEdtWidthClm1;
    int m_cxLblWidthClm2;
    int m_cxEdtWidthClm2;
    int m_cxClmSpacing;
    // Edit Controls
    QVBoxLayout* m_pLyt;
    // Button Line
    QWidget* m_pWdgtButtons;
    QHBoxLayout* m_pLytWdgtButtons;
    QPushButton* m_pBtnEdit;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtDiagramObjPropertiesAbstract

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ObjPropertiesAbstractWdgt_h
