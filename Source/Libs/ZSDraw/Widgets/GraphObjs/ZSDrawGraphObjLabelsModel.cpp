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
#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
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

    if (m_pGraphObj != nullptr && i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < m_ariRowIdx2Name.size())) {
            QString strName = m_ariRowIdx2Name[iRow];
            switch (iClm) {
                case EColumnName: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = strName;
                    }
                    break;
                }
                case EColumnText: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = m_pGraphObj->labelText(strName);
                    }
                    break;
                }
                case EColumnVisible: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = m_pGraphObj->isLabelVisible(strName);
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = m_pGraphObj->isLabelVisible(strName) ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
                    }
                    break;
                }
                case EColumnAnchor: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = CEnumSelectionPoint(m_pGraphObj->labelAnchorPoint(strName)).toString();
                    }
                    else if (i_iRole == Qt::AccessibleTextRole) {
                        QList<ESelectionPoint> arSelPts = m_pGraphObj->getPossibleLabelAnchorPoints(strName);
                        QList<SComboBoxItem> arItems;
                        for (const ESelectionPoint& selPt : arSelPts) {
                            arItems.append(SComboBoxItem(CEnumSelectionPoint(selPt).toString()));
                        }
                        varData.setValue(arItems);
                    }
                    break;
                }
                case EColumnAnchorLineVisible: {
                    if (i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole) {
                        varData = m_pGraphObj->isLabelAnchorLineVisible(strName);
                    }
                    else if (i_iRole == Qt::CheckStateRole) {
                        varData = m_pGraphObj->isLabelAnchorLineVisible(strName) ? Qt::Checked : Qt::Unchecked;
                    }
                    else if (i_iRole == Qt::TextAlignmentRole) {
                        varData = static_cast<int>(Qt::AlignHCenter | Qt::AlignVCenter);
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    return varData;
}

//------------------------------------------------------------------------------
bool CModelGraphObjLabels::setData(
    const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole)
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if (m_pGraphObj != nullptr && i_modelIdx.isValid()) {
        int iRow = i_modelIdx.row();
        int iClm = i_modelIdx.column();
        if ((iRow >= 0) && (iRow < m_ariRowIdx2Name.size())) {
            QString strName = m_ariRowIdx2Name[iRow];
            switch (iClm) {
                case EColumnName: {
                    if (i_iRole == Qt::EditRole) {
                        m_pGraphObj->renameLabel(strName, i_varData.toString());
                    }
                    break;
                }
                case EColumnText: {
                    if (i_iRole == Qt::EditRole) {
                        m_pGraphObj->setLabelText(strName, i_varData.toString());
                    }
                    break;
                }
                case EColumnVisible: {
                    if (i_iRole == Qt::EditRole) {
                        if (i_varData.toBool()) {
                            m_pGraphObj->showLabel(strName);
                        }
                        else {
                            m_pGraphObj->hideLabel(strName);
                        }
                    }
                    break;
                }
                case EColumnAnchor: {
                    if (i_iRole == Qt::EditRole) {
                        ESelectionPoint selPt = CEnumSelectionPoint(i_varData.toString()).enumerator();
                        m_pGraphObj->setLabelAnchorPoint(strName, selPt);
                    }
                    break;
                }
                case EColumnAnchorLineVisible: {
                    if (i_iRole == Qt::EditRole) {
                        if (i_varData.toBool()) {
                            m_pGraphObj->showLabelAnchorLine(strName);
                        }
                        else {
                            m_pGraphObj->hideLabelAnchorLine(strName);
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    return bOk;
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
                    varData = "Key";
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
                    varData = "Anchored To";
                }
                break;
            }
            case EColumnAnchorLineVisible: {
                if (i_iRole == Qt::DisplayRole) {
                    varData = "Line";
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
Qt::ItemFlags CModelGraphObjLabels::flags(const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    // The base class implementation returns a combination of flags that enables
    // the item (ItemIsEnabled) and allows it to be selected (ItemIsSelectable).
    Qt::ItemFlags uFlags = uFlags = QAbstractItemModel::flags(i_modelIdx);
    if (i_modelIdx.isValid()) {
        switch (i_modelIdx.column()) {
            case EColumnName: {
                break;
            }
            case EColumnText: {
                break;
            }
            case EColumnVisible: {
                uFlags = uFlags | Qt::ItemIsUserCheckable;
                break;
            }
            case EColumnAnchor: {
                uFlags = uFlags | Qt::ItemIsEditable;
                break;
            }
            case EColumnAnchorLineVisible: {
                uFlags = uFlags | Qt::ItemIsUserCheckable;
                break;
            }
            default: {
                break;
            }
        }
    }
    return uFlags;
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

