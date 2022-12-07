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

#ifndef ZSQuickControls_ThemeFlatStyle_h
#define ZSQuickControls_ThemeFlatStyle_h

#include "ZSQuickControls/ZSQuickControlsDllMain.h"

#include <QtCore/qobject.h>
#include <QtGui/qcolor.h>

class QQmlApplicationEngine;

namespace ZS
{
namespace System
{
class CIdxTree;

namespace GUI
{
class CModelIdxTree;
}
}
namespace QuickControls
{
//******************************************************************************
/*! @brief
*/
class ZSQUICKCONTROLSDLL_API CThemeWindowsStyle : public QObject
//******************************************************************************
{
    Q_OBJECT
    Q_PROPERTY(QString nameSpace READ NameSpace CONSTANT)
    Q_PROPERTY(QString className READ ClassName CONSTANT)
    Q_PROPERTY(QString objectName READ objectName CONSTANT)
    Q_PROPERTY(QColor mainColor READ mainColor WRITE setMainColor NOTIFY mainColorChanged)
public: // class methods
    /*! Returns the namespace of the class.
        @note The static class functions name must be different from the instance method "nameSpace". */
    static QString NameSpace() { return "ZS::QuickControls"; }
    /*! Returns the class name.
        @note The static class functions name must be different from the instance method "className". */
    static QString ClassName() { return "CThemeWindowsStyle"; }
    /*! Returns the name of the style.
        @note The name of the style must correspond to the name of the sub directory where
              qml files of the controls are located. */
    static QString StyleName() { return "ZSStyleWindows"; }
    /*! Returns the context property name by which the singleton instance is accessible by the qml modules.
        @note The following naming convention is followed:
              _<NameSpace>_theme<StyleNameAbbreviation>Style
              NameSpace ... Namespace of class without colons (e.g. ZSQuickControls).
              StyleNameAbbreviation ... Style name without ZSStyle (e.g. Windows). */
    static QString ContextPropertyName() { return "_ZSQuickControls_themeWindowsStyle"; }
    /*! Returns the context property name by which the model containing the controls of this style
        is accessible by the qml modules.
        @note The following naming convention is followed:
              _<NameSpace>_theme<StyleNameAbbreviation>Style_<ModelName>Model
              NameSpace ... Namespace of class without colons (e.g. ZSQuickControls).
              StyleNameAbbreviation ... Style name without ZSStyle (e.g. Windows).
              ModelName ... Name of the model (e.g. "controls"). */
    static QString ContextPropertyNameControlsModel() { return "_ZSQuickControls_themeWindowsStyle_controlsModel"; }
public: // class methods
    static CThemeWindowsStyle* GetInstance();
    static CThemeWindowsStyle* CreateInstance(QQmlApplicationEngine* i_pQmlAppEngine);
    static void ReleaseInstance();
protected: // ctors and dtor
    CThemeWindowsStyle(QQmlApplicationEngine* i_pQmlAppEngine);
    virtual ~CThemeWindowsStyle();
signals:
    /*! Signal which is emitted if the main color has been changed.
        @param i_pTrcServer [in] Pointer to object emitting the signal. */
    void mainColorChanged( const QColor& i_color );
public: // instance methods
    QColor mainColor() const;
    void setMainColor( const QColor& i_color );
protected: // instance methods (reference counter)
    int getRefCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected: // class members
    static CThemeWindowsStyle* s_pTheInst;
protected: // instance members
    /*<! Reference to Qml Engine. Needed to retrieve context and set the reference
         to this theme instance as context property. */
    QQmlApplicationEngine* m_pQmlAppEngine;
    /*<! Currently used main color. */
    QColor m_clrMain;
    /*<! Reference counter for createInstance and releaseInstance. */
    int m_iRefCount;
    /*<! Index tree containing the names of the controls implemented by this theme.
         Each control must have a corresponding qml file in sub directory StyleName(). */
    ZS::System::CIdxTree* m_pIdxTree;
    /*<! The index tree model provides the data from the index tree to the viewers. */
    ZS::System::GUI::CModelIdxTree* m_pModelIdxTree;

}; // class CThemeWindowsStyle

} // namespace QuickControls

} // namespace ZS

#endif // #ifndef ZSQuickControls_ThemeFlatStyle_h
