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

#ifndef ZSDiagram_ItemsPropertiesWdgtStack_h
#define ZSDiagram_ItemsPropertiesWdgtStack_h

#include "ZSDiagramGUI/ZSDiagramGUIDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QLineEdit;
class QStackedWidget;
class QSettings;
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
class CWdgtDiagramItemPropertiesBase;

//******************************************************************************
class ZSDIAGRAMGUIDLL_API CWdgtStackDiagramItemsProperties : public QWidget
//******************************************************************************
{
    Q_OBJECT
protected: // type definitions and constants
    typedef enum {
        EObjTypeUndefined = 0,
        EObjTypeDiagram   = 1,
        EObjTypeCount
    }   EObjType;
public: // class methods
    static QString NameSpace() { return "ZS::Diagram::GUI"; }
    static QString ClassName() { return "CWdgtStackDiagramItemsProperties"; }
public: // ctors and dtor
    CWdgtStackDiagramItemsProperties(
        CWdgtDiagram* i_pDiagram,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtStackDiagramItemsProperties();
public: // instance methods
    void setCurrentDiagObj( const QString& i_strClassName, const QString& i_strObjName );
    QString currentDiagItemClassName() const;
    QString currentDiagItemObjName() const;
protected slots: // overridables
    virtual void onDiagramAboutToBeDestroyed();
protected: // auxiliary instance methods
    void createDiagItemPropertiesWdgts();
protected: // instance members
    CWdgtDiagram* m_pDiagram;
    QString m_strCurrentDiagItemClassName;
    QString m_strCurrentDiagItemObjName;
    QVBoxLayout* m_pLyt;
    QHBoxLayout* m_pLytHeadLine;
    QLineEdit* m_pEdtPath;
    QStackedWidget* m_pStackedWdgtObjsProperties;
    /*!< Hash with widget pointers.
         Key is class name of diagram object. Class names don't just include the
         classes derived from CDiagObj but also CWdgtDiagram, CDiagScale and CDiagTrace. */
    QHash<QString, CWdgtDiagramItemPropertiesBase*> m_hshpWdgtsObjProperties;
    /*!< Hash with index of properties widgets in stacked widget.
         Key is class name of diagram object. Class names don't just include the
         classes derived from CDiagObj but also CDiagScale and CDiagTrace. */
    QHash<QString, int> m_hshIdxWdgtsObjProperties;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtStackDiagramItemsProperties

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ItemsPropertiesWdgtStack_h
