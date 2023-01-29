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

#ifndef ZDraw_GraphObjsTreeWdgt_h
#define ZDraw_GraphObjsTreeWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSSys/ZSSysEnumEntry.h"

class QPushButton;
class QSettings;
class QSplitter;
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
class CDrawingScene;
class CWdgtIdxTreeViewGraphObjs;
class CWdgtGraphObjs;

//******************************************************************************
class ZSDRAWDLL_API CWdgtIdxTreeGraphObjs : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtIdxTreeGraphObjs"; }
public: // ctors and dtor
    CWdgtIdxTreeGraphObjs(
        CDrawingScene* i_pDrawingScene,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtIdxTreeGraphObjs();
public: // instance methods
    void saveState(QSettings& i_settings) const;
    void restoreState(const QSettings& i_settings);
protected slots:
    void onTreeViewViewModeChanged( const QString& i_strViewMode );
    void onTreeViewCurrentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    QVBoxLayout* m_pLytMain;
    QSplitter* m_pSplitter;
    CWdgtIdxTreeViewGraphObjs* m_pWdgtTreeView;
    CWdgtGraphObjs* m_pWdgtGraphObjs;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtIdxTree

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZDraw_GraphObjsTreeWdgt_h
