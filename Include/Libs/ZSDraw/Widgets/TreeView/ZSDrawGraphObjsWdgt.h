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

#ifndef ZDraw_GraphObjsWdgt_h
#define ZDraw_GraphObjsWdgt_h

#include <QtCore/qabstractitemmodel.h>

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
class CDrawingView;
class CWdgtGraphObjsTree;
class CWdgtStackGraphObjsProperties;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjs : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtGraphObjs"; }
public: // ctors and dtor
    CWdgtGraphObjs(
        CDrawingView* i_pDrawingView,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtGraphObjs();
public: // instance methods
    void saveState(QSettings& i_settings) const;
    void restoreState(const QSettings& i_settings);
protected slots:
    void onWdgtTreeViewModeChanged(const QString& i_strViewMode);
    void onWdgtTreeViewCurrentRowChanged(const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev);
    void selectModelIdxPrev();
protected: // instance members
    CDrawingView* m_pDrawingView;
    QVBoxLayout* m_pLytMain;
    QSplitter* m_pSplitter;
    CWdgtGraphObjsTree* m_pWdgtTreeView;
    QModelIndex m_modelIdxPrev;
    CWdgtStackGraphObjsProperties* m_pWdgtStackGraphObjsProperties;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtGraphObjs

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZDraw_GraphObjsWdgt_h