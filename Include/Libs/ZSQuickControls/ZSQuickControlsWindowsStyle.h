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

#ifndef ZSQuickControls_ThemeFlatStyle_h
#define ZSQuickControls_ThemeFlatStyle_h

#include "ZSQuickControls/ZSQuickControlsDllMain.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysErrResult.h"

#include <QtCore/qhash.h>
#include <QtCore/qobject.h>
#include <QtGui/qcolor.h>

class QQmlApplicationEngine;

namespace ZS
{
namespace System
{
class CIdxTree;
class CTrcAdminObj;
}
namespace QuickControls
{
//******************************************************************************
/*! @brief

    @note

    Steps that are necessary to add a new property using buttonText as an example:

    - Add signal:
        void buttonTextColorChanged(const QColor& i_color, const QString& i_strTheme);
    - Add auxiliary instance method for method tracing to header file and implement the
      method in cpp file:
        void emit_buttonTextColorChanged(const QColor& i_color, const QString& i_strTheme);
    - Add instance member as vector with original (default) and current version of values
      with hash entries for each theme in header file:
        QVector<QHash<QString, QColor>> m_arhshButtonTextColors;
    - Set size of vector of row versions and theme hashes in ctors initializer list in cpp file:
        m_arhshButtonTextColors(CEnumRowVersion::count()),
    - Initialize vector of row versions and theme hashes in ctor with original (default values) in cpp file:
        m_arhshButtonTextColors[iRowVersion][c_strDayTheme] = palette.color(QPalette::Normal, QPalette::ButtonText);
        m_arhshButtonTextColors[iRowVersion][c_strNightTheme] = palette.color(QPalette::Normal, QPalette::ButtonText).lighter();
    - Add Q_PROPERTY section to header file and implement the methods in cpp file:
        Q_PROPERTY(QColor buttonTextColor READ buttonTextColor WRITE setButtonTextColor NOTIFY buttonTextColorChanged)
        Q_INVOKABLE QColor buttonTextColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
        Q_INVOKABLE void setButtonTextColor(const QColor& i_color, const QString& i_strTheme = "");
        Q_INVOKABLE bool isButtonTextColorSetToDefault(const QString& i_strTheme = "") const;
        Q_INVOKABLE void setButtonTextColorToDefault(const QString& i_strTheme = "");
    - Add method call to set property to default value to "setToDefault" method
      for the whole style in cpp file:
        for( const QString& strTheme : strlstThemes ) {
            ...
            setButtonTextColorToDefault(strTheme);
            ...
        }
    - Add property to save and recall methods.
    - Emit property changed signal if curren theme changes to method "onCurrentThemeChanged"
      in cpp file:
        ...
        emit_buttonTextColorChanged(
            m_arhshButtonTextColors[iRowVersionCurr][m_strCurrentTheme], m_strCurrentTheme);
        ...
*/
class ZSQUICKCONTROLSDLL_API CWindowsStyle : public QObject
//******************************************************************************
{
    Q_OBJECT
    Q_PROPERTY(QString nameSpace READ NameSpace CONSTANT)
    Q_PROPERTY(QString className READ ClassName CONSTANT)
    Q_PROPERTY(QString objectName READ objectName CONSTANT)
public: // type definitions and constants
    // Must correspond to the theme names defined in ZSStyleWindows qml file.
    static const QString c_strDayTheme;
    static const QString c_strNightTheme;
    static const QString c_strDefaultTheme;
    Q_INVOKABLE static QStringList themes();
public: // class methods
    /*! Returns the namespace of the class.
        @note The static class functions name must be different from the instance method "nameSpace". */
    static QString NameSpace() { return "ZS::QuickControls"; }
    /*! Returns the class name.
        @note The static class functions name must be different from the instance method "className". */
    static QString ClassName() { return "CWindowsStyle"; }
    /*! Returns the name of the style.
        @note The name of the style must correspond to the name of the sub directory where
              qml files of the controls are located. */
    static QString StyleName() { return "ZSStyleWindows"; }
    /*! Returns the name of the fallback style.
        @note The controls implemented in the ZSWindowsStyle are based on the controls from
              Qt's Fusion style. This is therefore the fallback style. */
    static QString FallbackStyleName() { return "Fusion"; }
    /*! Returns the context property name by which the singleton instance is accessible by the qml modules.
        @note The following naming convention is followed:
              _<NameSpace>_theme<StyleNameAbbreviation>Style
              NameSpace ... Namespace of class without colons (e.g. ZSQuickControls).
              StyleNameAbbreviation ... Style name without ZSStyle (e.g. Windows). */
    static QString ContextPropertyName() { return "_ZSQuickControls_windowsStyle"; }
public: // class methods
    static CWindowsStyle* GetInstance();
    static CWindowsStyle* CreateInstance(QQmlApplicationEngine* i_pQmlAppEngine, ZS::System::CIdxTree* i_pIdxTreeStyles);
    static void ReleaseInstance();
public: // class methods
    Q_INVOKABLE static bool isValidThemeName(const QString& i_strTheme);
    Q_INVOKABLE static QColor mergedColors(const QColor& i_colorA, const QColor& i_colorB, int i_iFactor = 50);
protected: // ctors and dtor
    CWindowsStyle(QQmlApplicationEngine* i_pQmlAppEngine, ZS::System::CIdxTree* i_pIdxTreeStyles);
    virtual ~CWindowsStyle();
signals:
    /*! Signal which is emitted if the theme has been changed.
        @param i_strTheme [in] Name of the currently used theme. */
    void currentThemeChanged(const QString& i_strTheme);
    /*! Signal which is emitted if the theme has been changed from
        a dark theme to a light theme or vice versa. */
    void isLightThemeChanged();
    /*! Signal which is emitted if the theme has been changed from
        a dark theme to a light theme or vice versa. */
    void isDarkThemeChanged();
    /*! Signal which is emitted if the base color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void baseColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the popup window color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void popupWindowColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the backgrounds gradient start color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void backgroundStartColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the backgrounds gradient stop color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void backgroundStopColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the button color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void buttonColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the shadow color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void shadowColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the top shadow color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void topShadowColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the inner contrast line color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void innerContrastLineColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the highlight color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void highlightColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the highlighted text color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void highlightedTextColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the outline color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void outlineColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the highlighted outline color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void highlightedOutlineColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the text color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void textColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the window text color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void windowTextColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the link color has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void linkColorChanged(const QColor& i_color, const QString& i_strTheme);
    /*! Signal which is emitted if the button text has been changed.
        @param i_color [in]
        @param i_strTheme [in] Theme for which the color has been changed. */
    void buttonTextColorChanged(const QColor& i_color, const QString& i_strTheme);
public: // instance methods (save/recall)
    Q_PROPERTY(QString saveRecallAbsoluteFilePath READ getSaveRecallAbsoluteFilePath CONSTANT)
    Q_INVOKABLE QString getSaveRecallAbsoluteFilePath() const;
    Q_INVOKABLE ZS::System::SErrResultInfo save() const;
    Q_INVOKABLE ZS::System::SErrResultInfo recall(const QString& i_strTheme = "");
    Q_INVOKABLE void setToDefault(const QString& i_strTheme = "");
public: // instance methods (current theme)
    Q_PROPERTY(QString currentTheme READ currentTheme WRITE setCurrentTheme NOTIFY currentThemeChanged)
    Q_PROPERTY(bool isLightTheme READ isLightTheme NOTIFY isLightThemeChanged)
    Q_PROPERTY(bool isDarkTheme READ isDarkTheme NOTIFY isDarkThemeChanged)
    QString currentTheme() const;
    void setCurrentTheme(const QString& i_strTheme);
    bool isLightTheme() const;
    bool isDarkTheme() const;
public: // instance methods
    Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor NOTIFY baseColorChanged)
    Q_INVOKABLE QColor baseColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setBaseColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isBaseColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setBaseColorToDefault(const QString& i_strTheme = "");
public: // instance methods
    Q_PROPERTY(QColor popupWindowColor READ popupWindowColor WRITE setPopupWindowColor NOTIFY popupWindowColorChanged)
    Q_INVOKABLE QColor popupWindowColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setPopupWindowColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isPopupWindowColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setPopupWindowColorToDefault(const QString& i_strTheme = "");
public: // instance methods
    Q_PROPERTY(QColor backgroundStartColor READ backgroundStartColor WRITE setBackgroundStartColor NOTIFY backgroundStartColorChanged)
    Q_INVOKABLE QColor backgroundStartColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setBackgroundStartColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isBackgroundStartColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setBackgroundStartColorToDefault(const QString& i_strTheme = "");
public: // instance methods
    Q_PROPERTY(QColor backgroundStopColor READ backgroundStopColor WRITE setBackgroundStopColor NOTIFY backgroundStopColorChanged)
    Q_INVOKABLE QColor backgroundStopColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setBackgroundStopColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isBackgroundStopColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setBackgroundStopColorToDefault(const QString& i_strTheme = "");
public: // instance methods
    Q_INVOKABLE QColor panelStartColor(bool i_bHighlighted = false, bool i_bDown = false, bool i_bHovered = false, const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE QColor panelStopColor(bool i_bHighlighted = false, bool i_bDown = false, bool i_bHovered = false, const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
public: // instance methods
    Q_PROPERTY(QColor buttonBaseColor READ buttonBaseColor WRITE setButtonColor NOTIFY buttonColorChanged);
    Q_INVOKABLE QColor buttonBaseColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE QColor buttonColor(bool i_bHighlighted = false, bool i_bDown = false, bool i_bHovered = false, const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setButtonColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isButtonColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setButtonColorToDefault(const QString& i_strTheme = "");
public: // instance methods
    Q_INVOKABLE QColor buttonOutlineColor(bool i_bHighlighted, bool i_bEnabled, const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
public: // instance method
    //Q_PROPERTY(QColor lightShadeColor READ lightShadeColor WRITE setLightShadeColor NOTIFY lightShadeColorChanged)
    //Q_INVOKABLE QColor lightShadeColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
public: // instance method
    //Q_PROPERTY(QColor darkShadeColor READ darkShadeColor WRITE setDarkShadeColor NOTIFY darkShadeColorChanged)
    //Q_INVOKABLE QColor darkShadeColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
public: // instance method
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
    Q_INVOKABLE QColor shadowColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setShadowColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isShadowColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setShadowColorToDefault(const QString& i_strTheme = "");
public: // instance method
    Q_PROPERTY(QColor topShadowColor READ topShadowColor WRITE setTopShadowColor NOTIFY topShadowColorChanged)
    Q_INVOKABLE QColor topShadowColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setTopShadowColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isTopShadowColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setTopShadowColorToDefault(const QString& i_strTheme = "");
public: // instance method
    Q_PROPERTY(QColor innerContrastLineColor READ innerContrastLineColor WRITE setInnerContrastLineColor NOTIFY innerContrastLineColorChanged)
    Q_INVOKABLE QColor innerContrastLineColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setInnerContrastLineColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isInnerContrastLineColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setInnerContrastLineColorToDefault(const QString& i_strTheme = "");
public: // instance method
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)
    Q_INVOKABLE QColor highlightColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setHighlightColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isHighlightColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setHighlightColorToDefault(const QString& i_strTheme = "");
public: // instance method
    Q_PROPERTY(QColor highlightedTextColor READ highlightedTextColor WRITE setHighlightedTextColor NOTIFY highlightedTextColorChanged)
    Q_INVOKABLE QColor highlightedTextColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setHighlightedTextColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isHighlightedTextColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setHighlightedTextColorToDefault(const QString& i_strTheme = "");
public: // instance method
    Q_PROPERTY(QColor outlineColor READ outlineColor WRITE setOutlineColor NOTIFY outlineColorChanged)
    Q_INVOKABLE QColor outlineColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setOutlineColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isOutlineColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setOutlineColorToDefault(const QString& i_strTheme = "");
public: // instance method
    Q_PROPERTY(QColor highlightedOutlineColor READ highlightedOutlineColor WRITE setHighlightedOutlineColor NOTIFY highlightedOutlineColorChanged)
    Q_INVOKABLE QColor highlightedOutlineColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setHighlightedOutlineColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isHighlightedOutlineColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setHighlightedOutlineColorToDefault(const QString& i_strTheme = "");
public: // instance method
    //Q_INVOKABLE QColor tabFrameColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current);
public: // instance methods
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_INVOKABLE QColor textColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setTextColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isTextColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setTextColorToDefault(const QString& i_strTheme = "");
public: // instance methods
    Q_PROPERTY(QColor windowTextColor READ windowTextColor WRITE setWindowTextColor NOTIFY windowTextColorChanged)
    Q_INVOKABLE QColor windowTextColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setWindowTextColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isWindowTextColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setWindowTextColorToDefault(const QString& i_strTheme = "");
public: // instance methods
    Q_PROPERTY(QColor linkColor READ linkColor WRITE setLinkColor NOTIFY linkColorChanged)
    Q_INVOKABLE QColor linkColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setLinkColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isLinkColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setLinkColorToDefault(const QString& i_strTheme = "");
public: // instance methods
    Q_PROPERTY(QColor buttonTextColor READ buttonTextColor WRITE setButtonTextColor NOTIFY buttonTextColorChanged)
    Q_INVOKABLE QColor buttonTextColor(const QString& i_strTheme = "", ZS::System::ERowVersion i_rowVersion = ZS::System::ERowVersion::Current) const;
    Q_INVOKABLE void setButtonTextColor(const QColor& i_color, const QString& i_strTheme = "");
    Q_INVOKABLE bool isButtonTextColorSetToDefault(const QString& i_strTheme = "") const;
    Q_INVOKABLE void setButtonTextColorToDefault(const QString& i_strTheme = "");
protected: // instance methods (reference counter)
    int getRefCount() const;
    int incrementRefCount();
    int decrementRefCount();
protected slots:
    void onCurrentThemeChanged(const QString& i_strTheme);
protected: // auxiliary instance methods (method tracing)
    void emit_currentThemeChanged(const QString& i_strTheme);
    void emit_baseColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_popupWindowColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_backgroundStartColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_backgroundStopColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_buttonColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_shadowColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_topShadowColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_innerContrastLineColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_highlightColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_highlightedTextColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_outlineColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_highlightedOutlineColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_textColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_windowTextColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_linkColorChanged(const QColor& i_color, const QString& i_strTheme);
    void emit_buttonTextColorChanged(const QColor& i_color, const QString& i_strTheme);
protected: // class members
    static CWindowsStyle* s_pTheInst;
    static QStringList s_strlstThemes;
protected: // instance members
    /*<! Reference counter for createInstance and releaseInstance. */
    int m_iRefCount;
    /*<! Reference to Qml Engine. Needed to retrieve context and set the reference
         to this theme instance as context property. */
    QQmlApplicationEngine* m_pQmlAppEngine;
    /*<! Currently used theme. */
    QString m_strCurrentTheme;
    /*<! Base colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshBaseColors;
    /*<! Shadow colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshPopupWindowColors;
    /*<! Gradient background start colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshBackgroundStartColors;
    /*<! Gradient background stop colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshBackgroundStopColors;
    /*<! Button colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshButtonColors;
    ///*<! Light shade colors of the themes. */
    //QVector<QHash<QString, QColor>> m_arhshLightShadeColors;
    ///*<! Dark shade colors of the themes. */
    //QVector<QHash<QString, QColor>> m_arhshDarkShadeColors;
    /*<! Shadow colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshShadowColors;
    /*<! Top shadow colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshTopShadowColors;
    /*<! Inner contrast line colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshInnerContrastLineColors;
    /*<! Highlight colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshHighlightColors;
    /*<! Highlighted text colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshHighlightedTextColors;
    /*<! Outline colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshOutlineColors;
    /*<! Highlighted outline colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshHighlightedOutlineColors;
    ///*<! Tab frame colors of the themes. */
    //QVector<QHash<QString, QColor>> m_arhshTabFrameColors;
    /*<! Text colors (text fields) of the themes. */
    QVector<QHash<QString, QColor>> m_arhshTextColors;
    /*<! Window (label) text colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshWindowTextColors;
    /*<! Link colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshLinkColors;
    /*<! Button text colors of the themes. */
    QVector<QHash<QString, QColor>> m_arhshButtonTextColors;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    /*!< Trace admin object to control trace outputs for noisy methods (getters) of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;

}; // class CWindowsStyle

} // namespace QuickControls

} // namespace ZS

#endif // #ifndef ZSQuickControls_ThemeFlatStyle_h
