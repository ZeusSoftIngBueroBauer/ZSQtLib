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

#ifndef ZSDraw_GraphObjLabelsModel_h
#define ZSDraw_GraphObjLabelsModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSDraw/Common/ZSDrawDllMain.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CGraphObj;

//******************************************************************************
class ZSDRAWDLL_API CModelGraphObjLabels : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CModelGraphObjLabels"; }
public: // type definitions and constants
    typedef enum
    {
        EColumnName              = 0,
        EColumnText              = 1,
        EColumnVisible           = 2,
        EColumnAnchor            = 3,
        EColumnAnchorLineVisible = 4,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelGraphObjLabels(CDrawingScene* i_pDrawingScene, QObject* i_pObjParent = nullptr);
    virtual ~CModelGraphObjLabels();
public: // overridables of base class QAbstractItemModel
    virtual int rowCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole) const override;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
protected: // instance members
    CDrawingScene* m_pDrawingScene;

}; // class CModelGraphObjLabels

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabelsModel_h
