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

#ifndef ZSDraw_WdgtFormatGraphObjs_h
#define ZSDraw_WdgtFormatGraphObjs_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Common/ZSDrawSettings.h"

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
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CWdgtFormatGraphObjs"; }
public: // ctors and dtor
    CWdgtFormatGraphObjs(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strClassName,
        const QString& i_strObjName,
        CGraphObj* i_pGraphObj = nullptr,  // if nullptr the draw settings of the drawing scene are edited, if != nullptr the attributes of the grapic item are edited
        QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtFormatGraphObjs();
public: // must overridables
    virtual void applyChanges() = 0;
    virtual void resetChanges() = 0;
    virtual bool hasChanges() const = 0;
public: // overridables
    virtual void hidePopups( QWidget* i_pWdgtKeepVisible = nullptr );
protected: // must overridables
    virtual void onGraphObjChanged() = 0;
private slots:
    void onDrawingSceneGraphObjChanged(const QString& i_strKeyInTree);
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    CGraphObj* m_pGraphObj;
    QGraphicsItem* m_pGraphicsItem;
    CDrawSettings m_drawSettings;
    /*!< List of popup widgets which have to be closed on activating other widgets. */
    QList<QWidget*> m_arpWdgtsPopup;
    /*!< Flag to indicate the changes will be applied to the graphical object.
         This flag has to be set at the beginning of "applyChanges" and reset
         if all changes have been set. Afterwards "onGraphObjChanged" has to
         be explicitely called at the end of "applyChanges". */
    bool m_bApplyingChanges; 

}; // class CWdgtFormatGraphObjs

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtFormatGraphObjs_h
