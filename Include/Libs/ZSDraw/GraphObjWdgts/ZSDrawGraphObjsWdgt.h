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

#ifndef ZSDraw_GraphObjsWdgt_h
#define ZSDraw_GraphObjsWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/Common/ZSDrawDllMain.h"

class QLineEdit;
class QStackedWidget;
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
class CDrawingScene;
class CWdgtAbstractGraphObj;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjs : public QWidget
//******************************************************************************
{
    Q_OBJECT
protected: // type definitions and constants
    typedef enum {
        EGraphObjTypeUndefined = 0,
        EGraphObjTypeCount
    }   EGraphObjType;
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtGraphObj"; }
public: // ctors and dtor
    CWdgtGraphObjs(
        CDrawingScene* i_pDrawingScene,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtGraphObjs();
public: // overridables
    virtual void setKeyInTree( const QString& i_strKeyInTree );
    QString getKeyInTree() const;
protected slots: // overridables
    virtual void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    ZS::System::CIdxTree* m_pIdxTree;
    QString m_strKeyInTree;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QLineEdit* m_pEdtPath;
    QStackedWidget* m_pStackedWdgtGraphObjs;
    QVector<CWdgtAbstractGraphObj*> m_arpWdgtsGraphObj;

}; // class CWdgtTreeEntries

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjsWdgt_h
