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
    m_pDrawingScene(i_pDrawingScene),
    m_pGraphObj(nullptr),
    m_hshName2RowIdx(),
    m_ariRowIdx2Name()
{
} // ctor

//------------------------------------------------------------------------------
CModelGraphObjLabels::~CModelGraphObjLabels()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    m_pGraphObj = nullptr;
    //m_hshName2RowIdx;
    //m_ariRowIdx2Name;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjLabels::setGraphObj(CGraphObj* i_pGraphObj)
//------------------------------------------------------------------------------
{
    if (m_pGraphObj != i_pGraphObj) {
        clear();
    }
    m_pGraphObj = i_pGraphObj;

    if (m_pGraphObj != nullptr) {
        fill();
    }
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelGraphObjLabels::rowCount(const QModelIndex& i_modelIdxParent) const
//------------------------------------------------------------------------------
{
    return m_ariRowIdx2Name.size();
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

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjLabels::clear()
//------------------------------------------------------------------------------
{
    if (m_ariRowIdx2Name.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_ariRowIdx2Name.size()-1);
        m_hshName2RowIdx.clear();
        m_ariRowIdx2Name.clear();
        endRemoveRows();
    }
}

//------------------------------------------------------------------------------
void CModelGraphObjLabels::fill()
//------------------------------------------------------------------------------
{
    if (m_pGraphObj != nullptr) {
        QStringList strlstLabelNames = m_pGraphObj->getLabelNames();
        if (strlstLabelNames.size() > 0) {
            beginInsertRows(QModelIndex(), 0, strlstLabelNames.size()-1);
            // The predefined labels should be at the beginning of the table.
            // Insert those first if the corresponding label has been added.
            QStringList strlstPredefinedLabelNames = m_pGraphObj->getPredefinedLabelNames();
            int iRow = 0;
            for (const QString& strName : strlstPredefinedLabelNames) {
                if (m_pGraphObj->isLabelAdded(strName)) {
                    m_hshName2RowIdx.insert(strName, iRow);
                    m_ariRowIdx2Name.append(strName);
                    ++iRow;
                }
            }
            // The user defined labels should follow the predefined labels.
            // Add those after the predefined labels.
            for (const QString& strName : strlstLabelNames) {
                if (m_pGraphObj->isLabelAdded(strName)) {
                    // If label has not already been added as a predefined label ...
                    if (!m_hshName2RowIdx.contains(strName)) {
                        m_hshName2RowIdx.insert(strName, iRow);
                        m_ariRowIdx2Name.append(strName);
                        ++iRow;
                    }
                }
            }
            endInsertRows();
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelGraphObjLabels::onGraphObjLabelAdded(const QString& i_strName)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CModelGraphObjLabels::onGraphObjLabelRemoved(const QString& i_strName)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CModelGraphObjLabels::onGraphObjLabelChanged(const QString& i_strName)
//------------------------------------------------------------------------------
{
}

