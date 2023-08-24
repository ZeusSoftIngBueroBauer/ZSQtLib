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

#include "ZSDiagramGUI/ZSDiagramItemsTreeModel.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSDiagram/ZSDiagObj.h"
#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/QBitmap>
#include <QtGui/QPainter>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::Diagram::GUI;


/*******************************************************************************
class CModelDiagramTreeItem
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelDiagramTreeItem::CModelDiagramTreeItem(
    const QString& i_strClassName,
    const QString& i_strObjName,
    CModelDiagramTreeItem* i_pParent) :
//------------------------------------------------------------------------------
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_pParent(i_pParent),
    m_arpChilds()
{
    if (m_pParent != nullptr) {
        m_pParent->m_arpChilds.append(this);
    }
}

//------------------------------------------------------------------------------
CModelDiagramTreeItem::~CModelDiagramTreeItem()
//------------------------------------------------------------------------------
{
    if (m_arpChilds.size() > 0)
    {
        for (int idxChild = m_arpChilds.size()-1; idxChild >= 0; --idxChild)
        {
            delete m_arpChilds[idxChild];
            // The dtor of the child already removed the object from the list
            // of childs and the index cannot be accessed anymore.
            //m_arpChilds[idxChild] = nullptr;
        }
    }
    if (m_pParent != nullptr)
    {
        m_pParent->m_arpChilds.removeOne(this);
    }
    //m_strClassName;
    //m_strObjName;
    m_pParent = nullptr;
    m_arpChilds.clear();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelDiagramTreeItem::className() const
//------------------------------------------------------------------------------
{
    return m_strClassName;
}

//------------------------------------------------------------------------------
QString CModelDiagramTreeItem::objectName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelDiagramTreeItem* CModelDiagramTreeItem::parent()
//------------------------------------------------------------------------------
{
    return m_pParent;
}

//------------------------------------------------------------------------------
int CModelDiagramTreeItem::row() const
//------------------------------------------------------------------------------
{
    int idxRow = 0;
    if (m_pParent != nullptr)
    {
        idxRow = m_pParent->m_arpChilds.indexOf(const_cast<CModelDiagramTreeItem*>(this));
    }
    return idxRow;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelDiagramTreeItem* CModelDiagramTreeItem::child(int i_idxRow)
//------------------------------------------------------------------------------
{
    CModelDiagramTreeItem* pChild = nullptr;
    if (i_idxRow >= 0 && i_idxRow < m_arpChilds.size())
    {
        pChild = m_arpChilds[i_idxRow];
    }
    return pChild;
}

//------------------------------------------------------------------------------
int CModelDiagramTreeItem::childsCount() const
//------------------------------------------------------------------------------
{
    return m_arpChilds.size();
}


/*******************************************************************************
class CModelDiagramItems : public QAbstractItemModel
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CModelDiagramItems::modelIdx2Str(const QModelIndex& i_modelIdx)
//------------------------------------------------------------------------------
{
    QString str;
    if( !i_modelIdx.isValid() ) {
        str = "Invalid (";
    }
    str += "Row: " + QString::number(i_modelIdx.row())
        + ", Column: " + QString::number(i_modelIdx.column());
    CModelDiagramTreeItem* pItem =
            static_cast<CModelDiagramTreeItem*>(i_modelIdx.internalPointer());
    if (pItem == nullptr) {
        str += ", null";
    }
    else {
        str += ", " + pItem->className() + "::" + pItem->objectName();
    }
    if( !i_modelIdx.isValid() ) {
        str += ")";
    }
    return str;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelDiagramItems::CModelDiagramItems(CWdgtDiagram* i_pDiagram, QObject* i_pObjParent) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pObjParent),
    m_pDiagram(i_pDiagram),
    m_pRoot(nullptr),
    m_pScales(nullptr),
    m_pTraces(nullptr),
    m_pDiagObjs(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pDiagram->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::NoisyMethods", objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs =  QString(i_pDiagram->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    if (m_pDiagram != nullptr) {
        m_pRoot = new CModelDiagramTreeItem(m_pDiagram->className(), m_pDiagram->objectName());
        m_pScales = new CModelDiagramTreeItem("Scales", "", m_pRoot);
        m_pTraces = new CModelDiagramTreeItem("Traces", "", m_pRoot);
        m_pDiagObjs = new CModelDiagramTreeItem("DiagObjs", "", m_pRoot);

        for (int idx = 0; idx < m_pDiagram->getDiagScalesCount(); ++idx) {
            CDiagScale* pDiagScale = m_pDiagram->getDiagScale(idx);
            new CModelDiagramTreeItem(pDiagScale->className(), pDiagScale->objectName(), m_pScales);
        }
        for (int idx = 0; idx < m_pDiagram->getDiagTracesCount(); ++idx) {
            CDiagTrace* pDiagTrace = m_pDiagram->getDiagTrace(idx);
            new CModelDiagramTreeItem(pDiagTrace->className(), pDiagTrace->objectName(), m_pTraces);
        }
        for (int idx = 0; idx < m_pDiagram->getDiagObjsCount(); ++idx) {
            CDiagObj* pDiagObj = m_pDiagram->getDiagObj(idx);
            new CModelDiagramTreeItem(pDiagObj->className(), pDiagObj->objectName(), m_pDiagObjs);
        }
        QObject::connect(
            m_pDiagram, &CWdgtDiagram::aboutToBeDestroyed,
            this, &CModelDiagramItems::onDiagramAboutToBeDestroyed);
        QObject::connect(
            m_pDiagram, &CWdgtDiagram::diagScaleAdded,
            this, &CModelDiagramItems::onDiagramScaleAdded);
        QObject::connect(
            m_pDiagram, &CWdgtDiagram::diagScaleRemoved,
            this, &CModelDiagramItems::onDiagramScaleRemoved);
        QObject::connect(
            m_pDiagram, &CWdgtDiagram::diagTraceAdded,
            this, &CModelDiagramItems::onDiagramTraceAdded);
        QObject::connect(
            m_pDiagram, &CWdgtDiagram::diagTraceRemoved,
            this, &CModelDiagramItems::onDiagramTraceRemoved);
        QObject::connect(
            m_pDiagram, &CWdgtDiagram::diagObjAdded,
            this, &CModelDiagramItems::onDiagramObjAdded);
        QObject::connect(
            m_pDiagram, &CWdgtDiagram::diagObjRemoved,
            this, &CModelDiagramItems::onDiagramObjRemoved);
    }

} // ctor

//------------------------------------------------------------------------------
CModelDiagramItems::~CModelDiagramItems()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try {
        delete m_pRoot;
    }
    catch(...) {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);

    m_pDiagram = nullptr;
    m_pRoot = nullptr;
    m_pScales = nullptr;
    m_pTraces = nullptr;
    m_pDiagObjs = nullptr;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelDiagramItems::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "rowCount",
        /* strMethodInArgs    */ strMthInArgs );

    int iRowCount = 1;

    if (i_modelIdxParent.isValid()) {
        CModelDiagramTreeItem* pParent =
            static_cast<CModelDiagramTreeItem*>(i_modelIdxParent.internalPointer());
        if (pParent == nullptr) {
            iRowCount = 0;
        }
        else {
            iRowCount = pParent->childsCount();
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(iRowCount);
    }
    return iRowCount;
}

//------------------------------------------------------------------------------
int CModelDiagramItems::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "columnCount",
        /* strMethodInArgs    */ strMthInArgs );

    int iColumnCount = EColumnCount;

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(iColumnCount);
    }
    return iColumnCount;
}

//------------------------------------------------------------------------------
QModelIndex CModelDiagramItems::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs  = "Row: " + QString::number(i_iRow);
        strMthInArgs += ", Clm: " + QString::number(i_iCol);
        strMthInArgs += ", ModelIdxParent {" + modelIdx2Str(i_modelIdxParent) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "index",
        /* strMethodInArgs    */ strMthInArgs );

    QModelIndex modelIdx;

    if (hasIndex(i_iRow, i_iCol, i_modelIdxParent))
    {
        CModelDiagramTreeItem* pItem = nullptr;
        if (!i_modelIdxParent.isValid()) {
            modelIdx = createIndex(i_iRow, i_iCol, m_pRoot);
        }
        else {
            CModelDiagramTreeItem* pParent =
                static_cast<CModelDiagramTreeItem*>(i_modelIdxParent.internalPointer());
            CModelDiagramTreeItem* pChild = pParent->child(i_iRow);
            if (pChild != nullptr) {
                modelIdx = createIndex(i_iRow, i_iCol, pChild);
            }
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(modelIdx2Str(modelIdx));
    }
    return modelIdx;
}

//------------------------------------------------------------------------------
QModelIndex CModelDiagramItems::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "parent",
        /* strMethodInArgs    */ strMthInArgs );

    QModelIndex modelIdxParent;

    if (i_modelIdx.isValid())
    {
        CModelDiagramTreeItem* pItem =
            static_cast<CModelDiagramTreeItem*>(i_modelIdx.internalPointer());
        if (pItem != m_pRoot) {
            CModelDiagramTreeItem* pParent = pItem->parent();
            modelIdxParent = createIndex(pParent->row(), 0, pParent);
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(modelIdx2Str(modelIdxParent));
    }
    return modelIdxParent;
}

//------------------------------------------------------------------------------
QVariant CModelDiagramItems::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* iFilterLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "data",
        /* strAddInfo   */ strMthInArgs );

    QVariant varData;

    CModelDiagramTreeItem* pItem = nullptr;

    if (i_modelIdx.isValid())
    {
        pItem = static_cast<CModelDiagramTreeItem*>(i_modelIdx.internalPointer());
    }
    if (pItem != nullptr)
    {
        switch (i_modelIdx.column())
        {
            case EColumnDiagObjClassName:
            {
                if (i_iRole == Qt::DisplayRole)
                {
                    varData = pItem->className();
                }
                break;
            }
            case EColumnDiagObjName:
            {
                if (i_iRole == Qt::DisplayRole)
                {
                    varData = pItem->objectName();
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(varData.toString());
    }
    return varData;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
Qt::ItemFlags CModelDiagramItems::flags(const QModelIndex& i_modelIdx) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "flags",
        /* strMethodInArgs    */ strMthInArgs );

    Qt::ItemFlags uFlags = Qt::NoItemFlags;
    if (i_modelIdx.isValid()) {
        uFlags = QAbstractItemModel::flags(i_modelIdx);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qItemFlags2Str(uFlags));
    }
    return uFlags;
}

//------------------------------------------------------------------------------
QVariant CModelDiagramItems::headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Section: " + QString::number(i_iSection);
        strMthInArgs += ", Orientation: " + qOrientation2Str(i_orientation);
        strMthInArgs += ", Role: " + qItemDataRole2Str(i_iRole);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "headerData",
        /* strMethodInArgs    */ strMthInArgs );

    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        switch( i_iSection )
        {
            case EColumnDiagObjClassName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Class";
                }
                break;
            }
            case EColumnDiagObjName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Name";
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(varData.toString());
    }
    return varData;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelDiagramItems::onDiagramAboutToBeDestroyed(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramAboutToBeDestroyed",
        /* strMethodInArgs    */ strMthInArgs );

    beginResetModel();

    delete m_pRoot;
    m_pRoot = nullptr;
    m_pScales = nullptr;
    m_pTraces = nullptr;
    m_pDiagObjs = nullptr;
    m_pDiagram = nullptr;

    endResetModel();
}

//------------------------------------------------------------------------------
void CModelDiagramItems::onDiagramScaleAdded(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramScaleAdded",
        /* strMethodInArgs    */ strMthInArgs );

    CDiagScale* pDiagScale = m_pDiagram->findDiagScale(i_strObjName);
    CModelDiagramTreeItem* pItem =
        new CModelDiagramTreeItem(pDiagScale->className(), pDiagScale->objectName(), m_pScales);
    QModelIndex modelIdxParent = createIndex(m_pScales->row(), 0, m_pScales);
    beginInsertRows(modelIdxParent, pItem->row(), pItem->row());
    endInsertRows();
}

//------------------------------------------------------------------------------
void CModelDiagramItems::onDiagramScaleRemoved(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramScaleRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    CModelDiagramTreeItem* pItem = findDiagScaleItem(i_strObjName);
    QModelIndex modelIdxParent = createIndex(m_pScales->row(), 0, m_pScales);
    beginRemoveRows(modelIdxParent, pItem->row(), pItem->row());
    delete pItem;
    pItem = nullptr;
    endRemoveRows();
}

//------------------------------------------------------------------------------
void CModelDiagramItems::onDiagramTraceAdded(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramTraceAdded",
        /* strMethodInArgs    */ strMthInArgs );

    CDiagTrace* pDiagTrace = m_pDiagram->findDiagTrace(i_strObjName);
    CModelDiagramTreeItem* pItem =
        new CModelDiagramTreeItem(pDiagTrace->className(), pDiagTrace->objectName(), m_pTraces);
    QModelIndex modelIdxParent = createIndex(m_pTraces->row(), 0, m_pTraces);
    beginInsertRows(modelIdxParent, pItem->row(), pItem->row());
    endInsertRows();
}

//------------------------------------------------------------------------------
void CModelDiagramItems::onDiagramTraceRemoved(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramTraceRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    CModelDiagramTreeItem* pItem = findDiagTraceItem(i_strObjName);
    QModelIndex modelIdxParent = createIndex(m_pTraces->row(), 0, m_pTraces);
    beginRemoveRows(modelIdxParent, pItem->row(), pItem->row());
    delete pItem;
    pItem = nullptr;
    endRemoveRows();
}

//------------------------------------------------------------------------------
void CModelDiagramItems::onDiagramObjAdded(const QString& i_strClassName, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramObjAdded",
        /* strMethodInArgs    */ strMthInArgs );

    CDiagObj* pDiagObj = m_pDiagram->findDiagObj(i_strClassName, i_strObjName);
    CModelDiagramTreeItem* pItem =
        new CModelDiagramTreeItem(pDiagObj->className(), pDiagObj->objectName(), m_pDiagObjs);
    QModelIndex modelIdxParent = createIndex(m_pDiagObjs->row(), 0, m_pDiagObjs);
    beginInsertRows(modelIdxParent, pItem->row(), pItem->row());
    endInsertRows();
}

//------------------------------------------------------------------------------
void CModelDiagramItems::onDiagramObjRemoved(const QString& i_strClassName, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObjNoisyMethods,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramObjRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    CModelDiagramTreeItem* pItem = findDiagObjItem(i_strClassName, i_strObjName);
    QModelIndex modelIdxParent = createIndex(m_pDiagObjs->row(), 0, m_pDiagObjs);
    beginRemoveRows(modelIdxParent, pItem->row(), pItem->row());
    delete pItem;
    pItem = nullptr;
    endRemoveRows();
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CModelDiagramTreeItem* CModelDiagramItems::findDiagScaleItem(const QString& i_strObjName) const
//------------------------------------------------------------------------------
{
    CModelDiagramTreeItem* pItem = nullptr;
    if (m_pScales != nullptr) {
        for (int idx = 0; idx < m_pScales->childsCount(); ++idx) {
            if (m_pScales->child(idx)->objectName() == i_strObjName) {
                pItem = m_pScales->child(idx);
                break;
            }
        }
    }
    return pItem;
}

//------------------------------------------------------------------------------
CModelDiagramTreeItem* CModelDiagramItems::findDiagTraceItem(const QString& i_strObjName) const
//------------------------------------------------------------------------------
{
    CModelDiagramTreeItem* pItem = nullptr;
    if (m_pTraces != nullptr) {
        for (int idx = 0; idx < m_pTraces->childsCount(); ++idx) {
            if (m_pTraces->child(idx)->objectName() == i_strObjName) {
                pItem = m_pTraces->child(idx);
                break;
            }
        }
    }
    return pItem;
}

//------------------------------------------------------------------------------
CModelDiagramTreeItem* CModelDiagramItems::findDiagObjItem(
    const QString& i_strClassName, const QString& i_strObjName) const
//------------------------------------------------------------------------------
{
    CModelDiagramTreeItem* pItem = nullptr;
    if (m_pDiagObjs != nullptr) {
        for (int idx = 0; idx < m_pDiagObjs->childsCount(); ++idx) {
            if (m_pDiagObjs->child(idx)->className() == i_strClassName
             && m_pDiagObjs->child(idx)->objectName() == i_strObjName) {
                pItem = m_pDiagObjs->child(idx);
                break;
            }
        }
    }
    return pItem;
}
