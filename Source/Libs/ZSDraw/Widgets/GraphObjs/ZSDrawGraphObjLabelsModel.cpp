/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

*******************************************************************************/

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLabelsModel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class ZSDRAWDLL_API CModelGraphObjLabels : public ZS::System::GUI::CModelIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelGraphObjLabels::CModelGraphObjLabels(
    CDrawingScene* i_pDrawingScene,
    QObject*       i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(i_pObjParent),
    m_pDrawingScene(i_pDrawingScene)
{
} // ctor

//------------------------------------------------------------------------------
CModelGraphObjLabels::~CModelGraphObjLabels()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelGraphObjLabels::rowCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    int iRowCount = 1;
    return iRowCount;
}

//------------------------------------------------------------------------------
int CModelGraphObjLabels::columnCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelGraphObjLabels::data(const QModelIndex& i_modelIdx, int i_iRole) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if (i_modelIdx.isValid()) {
        switch (i_modelIdx.column()) {
            case EColumnName: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = QString::number(i_modelIdx.row()) + ", " + QString::number(i_modelIdx.column());
                }
                break;
            }
            case EColumnText: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = QString::number(i_modelIdx.row()) + ", " + QString::number(i_modelIdx.column());
                }
                break;
            }
            case EColumnVisible: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = QString::number(i_modelIdx.row()) + ", " + QString::number(i_modelIdx.column());
                }
                break;
            }
            case EColumnAnchor: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = QString::number(i_modelIdx.row()) + ", " + QString::number(i_modelIdx.column());
                }
                break;
            }
            case EColumnAnchorLineVisible: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = QString::number(i_modelIdx.row()) + ", " + QString::number(i_modelIdx.column());
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    return varData;
}

//------------------------------------------------------------------------------
QVariant CModelGraphObjLabels::headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if (i_orientation == Qt::Horizontal) {
        switch (i_iSection) {
            case EColumnName: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Name";
                }
                break;
            }
            case EColumnText: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Text";
                }
                break;
            }
            case EColumnVisible: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Visible";
                }
                break;
            }
            case EColumnAnchor: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Anchor";
                }
                break;
            }
            case EColumnAnchorLineVisible: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "AnchorLineVisible";
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    return varData;
}

