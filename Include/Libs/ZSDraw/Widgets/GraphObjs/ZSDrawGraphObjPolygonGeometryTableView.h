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

#ifndef ZSDraw_DrawGraphObjPolygonGeometryTableView_h
#define ZSDraw_DrawGraphObjPolygonGeometryTableView_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSSysGUI/ZSSysTableView.h"

namespace ZS {
namespace System {
class CTrcAdminObj;
}
namespace Draw
{
class CModelGraphObjPolygonGeometry;

//******************************************************************************
class ZSDRAWDLL_API CTableViewGraphObjPolygonGeometry : public ZS::System::GUI::CTableView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CTableViewGraphObjPolygonGeometry"; }
public: // ctors and dtor
    CTableViewGraphObjPolygonGeometry(const QString& i_strObjName, QWidget* i_pWdgtParent = nullptr);
    virtual ~CTableViewGraphObjPolygonGeometry();
public: // overridables of base class QTableView
    void setModel(QAbstractItemModel* i_pModel) override;
protected: // overridables of base class QAbstractItemView
    virtual void mousePressEvent(QMouseEvent* i_pEv) override;
    virtual void mouseReleaseEvent(QMouseEvent* i_pEv) override;
protected slots:
    void onActionDeleteTriggered(bool i_bChecked);
    void onActionInsertBeforeTriggered(bool i_bChecked);
    void onActionInsertAfterTriggered(bool i_bChecked);
private: // instance members
    CModelGraphObjPolygonGeometry* m_pModel;
    QMenu* m_pMenu;
    QAction* m_pActionTitle;
    QAction* m_pActionDelete;
    QAction* m_pActionInsertBefore;
    QAction* m_pActionInsertAfter;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTableViewGraphObjPolygonGeometry

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawGraphObjPolygonGeometryTableView_h
