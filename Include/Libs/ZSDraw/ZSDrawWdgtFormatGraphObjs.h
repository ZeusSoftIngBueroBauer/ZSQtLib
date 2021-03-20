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

#ifndef ZSDraw_WdgtFormatGraphObjs_h
#define ZSDraw_WdgtFormatGraphObjs_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawSettings.h"

namespace ZS
{
namespace Draw
{
class CDrawingScene;
class CGraphObj;

//******************************************************************************
class ZSDRAWDLL_API CWdgtFormatGraphObjs : public QWidget
//******************************************************************************
{
public: // ctors and dtor
    CWdgtFormatGraphObjs(
        CDrawingScene* i_pDrawingScene,
        CGraphObj*     i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjs();
public: // must overridables
    virtual void applyChanges() = 0;
    virtual void resetChanges() = 0;
    virtual bool hasChanges() const = 0;
public: // overridables
    virtual void hidePopups( QWidget* i_pWdgtKeepVisible = nullptr );
protected: // instance members
    CDrawingScene*  m_pDrawingScene;
    CGraphObj*      m_pGraphObj;
    QGraphicsItem*  m_pGraphicsItem;
    QString         m_strObjName;
    QString         m_strObjId;
    CDrawSettings   m_drawSettings;
    // List of popup widgets which have to be closed on activating other widgets
    QList<QWidget*> m_arpWdgtsPopup;

}; // class CWdgtFormatGraphObjs

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjs_h
