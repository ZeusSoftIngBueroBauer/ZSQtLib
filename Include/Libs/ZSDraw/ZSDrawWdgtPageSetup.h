/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#error Module not used anymore

#ifndef ZSDraw_WdgtPageSetup_h
#define ZSDraw_WdgtPageSetup_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawPageSetup.h"

namespace ZS
{
namespace Draw
{
class CDrawingView;

//******************************************************************************
class ZSDRAWDLL_API CWdgtPageSetup : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtPageSetup( CDrawingView* i_pDrawingView, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtPageSetup();
signals:
    void pageSetupChanged( const CPageSetup& i_pageSetup );
public: // overridables
    virtual void setPageSetup( const CPageSetup& i_pageSetup );
    CPageSetup pageSetup() const { return m_pageSetup; }
    //virtual void hidePopups( QWidget* i_pWdgtKeepVisible = nullptr );
protected: // instance members
    CDrawingView* m_pDrawingView;
    CPageSetup    m_pageSetup;
    // List of popup widgets which have to be closed on activating other widgets
    //QList<QWidget*> m_arpWdgtsPopup;

}; // class CWdgtPageSetup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtPageSetup_h
