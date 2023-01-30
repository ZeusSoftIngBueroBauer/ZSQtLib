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

#ifndef ZSDraw_GraphObjsPropertiesWdgtStack_h
#define ZSDraw_GraphObjsPropertiesWdgtStack_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/Common/ZSDrawDllMain.h"

class QLineEdit;
class QStackedWidget;
class QSettings;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CIdxTree;
}

namespace Draw
{
class CDrawingView;
class CWdgtGraphObjPropertiesAbstract;

//******************************************************************************
class ZSDRAWDLL_API CWdgtStackGraphObjsProperties : public QWidget
//******************************************************************************
{
    Q_OBJECT
protected: // type definitions and constants
    typedef enum {
        EGraphObjTypeUndefined = 0,
        EGraphObjTypeDrawing   = 1,
        EGraphObjTypeCount
    }   EGraphObjType;
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtStackGraphObjsProperties"; }
public: // ctors and dtor
    CWdgtStackGraphObjsProperties(
        CDrawingView* i_pDrawingView,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtStackGraphObjsProperties();
public: // instance methods
    void saveState(QSettings& i_settings) const;
    void restoreState(const QSettings& i_settings);
public: // instance methods
    void setKeyInTree( const QString& i_strKeyInTree );
    QString getKeyInTree() const;
protected slots: // overridables
    virtual void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CDrawingView* m_pDrawingView;
    ZS::System::CIdxTree* m_pIdxTree;
    QString m_strKeyInTree;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QLineEdit* m_pEdtPath;
    QStackedWidget* m_pStackedWdgtGraphObjsProperties;
    QVector<CWdgtGraphObjPropertiesAbstract*> m_arpWdgtsGraphObjProperties;

}; // class CWdgtStackGraphObjsProperties

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjsPropertiesWdgtStack_h
