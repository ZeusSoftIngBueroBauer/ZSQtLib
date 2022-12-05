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


namespace ZS
{
namespace QuickControls
{
//******************************************************************************
/*! @brief
*/
class ZSQUICKCONTROLSDLL_API CThemeFlatStyle : public QObject
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
    static QString ClassName() { return "CThemeFlatStyle"; }
public: // class methods
    static CThemeFlatStyle* GetInstance();
    static CThemeFlatStyle* CreateInstance();
    static void ReleaseInstance();
protected: // ctors and dtor
    CThemeFlatStyle();
    virtual ~CThemeFlatStyle();
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
    static CThemeFlatStyle* s_pTheInst;
protected: // instance members
    /*<! Currently used main color. */
    QColor m_clrMain;
    /*<! Reference counter for createInstance and releaseInstance. */
    int m_iRefCount;

}; // class CThemeFlatStyle

} // namespace QuickControls

} // namespace ZS

#endif // #ifndef ZSQuickControls_ThemeFlatStyle_h
