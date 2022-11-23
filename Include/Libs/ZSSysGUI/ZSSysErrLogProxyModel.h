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

#ifndef ZSSysGUI_ErrLogProxyModel_h
#define ZSSysGUI_ErrLogProxyModel_h

#include <QtCore/qsortfilterproxymodel.h>

#include "ZSSysGUI/ZSSysErrLogModel.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CProxyModelErrLog : public QSortFilterProxyModel
//******************************************************************************
{
    Q_OBJECT
    Q_PROPERTY(CModelErrLog* errLogModel READ errLogModel CONSTANT)
    Q_CLASSINFO("DefaultProperty", "data")
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CProxyModelErrLog"; }
public: // ctors and dtor
    CProxyModelErrLog(CModelErrLog* i_pErrLogModel);
    virtual ~CProxyModelErrLog();
public: // instance methods
    CModelErrLog* errLogModel();
    Q_INVOKABLE int columnWidth(int i_iClm, const QFont* i_pFont = nullptr) const;
public: // overridables of base class QSortFilterProxyModel
    Q_INVOKABLE void sort(int i_iClm, Qt::SortOrder i_sortOrder = Qt::AscendingOrder) override;
    int columnCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    QModelIndex mapFromSource(const QModelIndex& i_modelIdxSource) const override;
    QModelIndex mapToSource(const QModelIndex& i_modelIdxProxy) const override;
protected: // instance members
    CModelErrLog*             m_pErrLogModel;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CProxyModelErrLog

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_ErrLogProxyModel_h
