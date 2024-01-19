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

#include "ZSSysGUI/ZSSysTableView.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/qevent.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CTableView : public QTableView
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
CTableView::SEditTriggerKey::SEditTriggerKey() :
//------------------------------------------------------------------------------
    m_key(), m_modifiers(Qt::NoModifier)
{
}

//------------------------------------------------------------------------------
CTableView::SEditTriggerKey::SEditTriggerKey(Qt::Key i_key, Qt::KeyboardModifiers i_modifiers) :
//------------------------------------------------------------------------------
    m_key(i_key), m_modifiers(i_modifiers)
{
}

//------------------------------------------------------------------------------
bool CTableView::SEditTriggerKey::operator == (const SEditTriggerKey& i_other) const
//------------------------------------------------------------------------------
{
    return (m_key == i_other.m_key && m_modifiers == i_other.m_modifiers);
}

//------------------------------------------------------------------------------
bool CTableView::SEditTriggerKey::operator != (const SEditTriggerKey& i_other) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

//------------------------------------------------------------------------------
QString CTableView::SEditTriggerKey::toString() const
//------------------------------------------------------------------------------
{
    return qKeyCode2Str(m_key) + ", " + qKeyboardModifiers2Str(m_modifiers);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTableView::CTableView( const QString& i_strObjName, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTableView(i_pWdgtParent),
    m_editTriggers(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName());

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ "" );
}

//------------------------------------------------------------------------------
CTableView::~CTableView()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    if( m_pTrcAdminObj != nullptr ) {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    //m_editTriggers;
    m_pTrcAdminObj = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CTableView::addKeyAsEditTriggerForTable(const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_key.toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "addKeyAsEditTriggerForTable",
        /* strMethodInArgs    */ strMthInArgs );

    bool bCanAdd = true;
    for (const SEditTrigger& key : m_editTriggers) {
        if (key.m_iRow == -1 && key.m_iColumn == -1 && key.m_key == i_key) {
            bCanAdd = false;
            break;
        }
    }
    if (bCanAdd) {
        m_editTriggers << SEditTrigger(i_key);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
bool CTableView::addKeyAsEditTriggerForColumn(int i_iColumn, const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iColumn) + ", " + i_key.toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "addKeyAsEditTriggerForColumn",
        /* strMethodInArgs    */ strMthInArgs );

    bool bCanAdd = true;
    for (const SEditTrigger& key : m_editTriggers) {
        if (key.m_iRow == -1 && key.m_iColumn == i_iColumn && key.m_key == i_key) {
            bCanAdd = false;
            break;
        }
    }
    if (bCanAdd) {
        m_editTriggers << SEditTrigger(i_iColumn, i_key);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
bool CTableView::addKeyAsEditTriggerForCell(int i_iRow, int i_iColumn, const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iRow) + ", " + QString::number(i_iColumn) + ", " + i_key.toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "addKeyAsEditTriggerForCell",
        /* strMethodInArgs    */ strMthInArgs );

    bool bCanAdd = true;
    for (const SEditTrigger& key : m_editTriggers) {
        if (key.m_iRow == i_iRow && key.m_iColumn == i_iColumn && key.m_key == i_key) {
            bCanAdd = false;
            break;
        }
    }
    if (bCanAdd) {
        m_editTriggers << SEditTrigger(i_iRow, i_iColumn, i_key);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bCanAdd);
    }
    return bCanAdd;
}

//------------------------------------------------------------------------------
bool CTableView::removeKeyAsEditTriggerForTable(const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_key.toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "removeKeyAsEditTriggerForTable",
        /* strMethodInArgs    */ strMthInArgs );

    int idxKey = -1;
    for (int idx = 0; idx < m_editTriggers.size(); ++idx) {
        const SEditTrigger& key = m_editTriggers[idx];
        if (key.m_iRow == -1 && key.m_iColumn == -1 && key.m_key == i_key) {
            idxKey = idx;
            break;
        }
    }
    if (idxKey >= 0 && idxKey < m_editTriggers.size()) {
        m_editTriggers.removeAt(idxKey);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(idxKey >= 0);
    }
    return (idxKey >= 0);
}

//------------------------------------------------------------------------------
bool CTableView::removeKeyAsEditTriggerForColumn(int i_iColumn, const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iColumn) + ", " + i_key.toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "removeKeyAsEditTriggerForColumn",
        /* strMethodInArgs    */ strMthInArgs );

    int idxKey = -1;
    for (int idx = 0; idx < m_editTriggers.size(); ++idx) {
        const SEditTrigger& key = m_editTriggers[idx];
        if (key.m_iRow == -1 && key.m_iColumn == i_iColumn && key.m_key == i_key) {
            idxKey = idx;
            break;
        }
    }
    if (idxKey >= 0 && idxKey < m_editTriggers.size()) {
        m_editTriggers.removeAt(idxKey);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(idxKey >= 0);
    }
    return (idxKey >= 0);
}

//------------------------------------------------------------------------------
bool CTableView::removeKeyAsEditTriggerForCell(int i_iRow, int i_iColumn, const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iRow) + ", " + QString::number(i_iColumn) + ", " + i_key.toString();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "removeKeyAsEditTriggerForCell",
        /* strMethodInArgs    */ strMthInArgs );

    int idxKey = -1;
    for (int idx = 0; idx < m_editTriggers.size(); ++idx) {
        const SEditTrigger& key = m_editTriggers[idx];
        if (key.m_iRow == i_iRow && key.m_iColumn == i_iColumn && key.m_key == i_key) {
            idxKey = idx;
            break;
        }
    }
    if (idxKey >= 0 && idxKey < m_editTriggers.size()) {
        m_editTriggers.removeAt(idxKey);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(idxKey >= 0);
    }
    return (idxKey >= 0);
}

//------------------------------------------------------------------------------
bool CTableView::isEditTriggerKeyForTable(const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    bool bAdded = false;
    for (const SEditTrigger& key : m_editTriggers) {
        if (key.m_iRow == -1 && key.m_iColumn == -1 && key.m_key == i_key) {
            bAdded = true;
            break;
        }
    }
    return bAdded;
}

//------------------------------------------------------------------------------
bool CTableView::isEditTriggerKeyForColumn(int i_iColumn, const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    bool bAdded = false;
    for (const SEditTrigger& key : m_editTriggers) {
        if (key.m_iRow == -1 && key.m_iColumn == i_iColumn && key.m_key == i_key) {
            bAdded = true;
            break;
        }
    }
    return bAdded;
}

//------------------------------------------------------------------------------
bool CTableView::isEditTriggerKeyForCell(int i_iRow, int i_iColumn, const SEditTriggerKey& i_key)
//------------------------------------------------------------------------------
{
    bool bAdded = false;
    for (const SEditTrigger& key : m_editTriggers) {
        if (key.m_iRow == i_iRow && key.m_iColumn == i_iColumn && key.m_key == i_key) {
            bAdded = true;
            break;
        }
    }
    return bAdded;
}

/*==============================================================================
protected: // overridables of base class QTableView
==============================================================================*/

//------------------------------------------------------------------------------
void CTableView::keyPressEvent(QKeyEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qKeyEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName         */ objectName(),
        /* strMethod          */ "keyPressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    QModelIndex modelIdxCurr = currentIndex();
    if (isEditTriggerKeyForTable(SEditTriggerKey(static_cast<Qt::Key>(i_pEv->key()), i_pEv->modifiers()))) {
        //if ((i_pEv->type() == QEvent::KeyPress) && ((i_pEv->key() == Qt::Key_Return) || (i_pEv->key() == Qt::Key_Enter))) {
            if (state() != QAbstractItemView::EditingState) {
                // The first edit with NoEditTriggers tries to forward the event directly to the delegate.
                // If this is not possible ..
                if (!edit(modelIdxCurr, NoEditTriggers, i_pEv)) {
                    // .. forward the event to the editor of the delegate.
                    edit(modelIdxCurr);
                }
            }
        //}
    }
    else if (isEditTriggerKeyForColumn(modelIdxCurr.column(), SEditTriggerKey(static_cast<Qt::Key>(i_pEv->key()), i_pEv->modifiers()))) {
        //if ((i_pEv->type() == QEvent::KeyPress) && ((i_pEv->key() == Qt::Key_Return) || (i_pEv->key() == Qt::Key_Enter))) {
            if (state() != QAbstractItemView::EditingState) {
                // The first edit with NoEditTriggers tries to forward the event directly to the delegate.
                // If this is not possible ..
                if (!edit(modelIdxCurr, NoEditTriggers, i_pEv)) {
                    // .. forward the event to the editor of the delegate.
                    edit(modelIdxCurr);
                }
            }
        //}
    }
    else if (isEditTriggerKeyForCell(modelIdxCurr.row(), modelIdxCurr.column(), SEditTriggerKey(static_cast<Qt::Key>(i_pEv->key()), i_pEv->modifiers()))) {
        //if ((i_pEv->type() == QEvent::KeyPress) && ((i_pEv->key() == Qt::Key_Return) || (i_pEv->key() == Qt::Key_Enter))) {
            if (state() != QAbstractItemView::EditingState) {
                // The first edit with NoEditTriggers tries to forward the event directly to the delegate.
                // If this is not possible ..
                if (!edit(modelIdxCurr, NoEditTriggers, i_pEv)) {
                    // .. forward the event to the editor of the delegate.
                    edit(modelIdxCurr);
                }
            }
        //}
    }
    QTableView::keyPressEvent(i_pEv);
}
