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

#ifndef ZSSysGUI_TableView_h
#define ZSSysGUI_TableView_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qtableview.h>
#endif


namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
//******************************************************************************
/*! @brief Reimplements QTableView to add keys (like the Return/Enter Key) as edit triggers.
*/
class ZSSYSGUIDLL_API CTableView : public QTableView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CTableView"; }
public: // type definitions and constants
    struct ZSSYSGUIDLL_API SEditTriggerKey {
        SEditTriggerKey();
        SEditTriggerKey(Qt::Key i_key, Qt::KeyboardModifiers i_modifiers = Qt::NoModifier);
        bool operator == (const SEditTriggerKey& i_other) const;
        bool operator != (const SEditTriggerKey& i_other) const;
        QString toString() const;
        Qt::Key m_key;
        Qt::KeyboardModifiers m_modifiers;
    };
public: // ctors and dtor
    CTableView( const QString& i_strObjName, QWidget* i_pWdgtParent = nullptr );
    virtual ~CTableView();
public: // instance methods
    bool addKeyAsEditTriggerForTable(const SEditTriggerKey& i_key);
    bool addKeyAsEditTriggerForColumn(int i_iColumn, const SEditTriggerKey& i_key);
    bool addKeyAsEditTriggerForCell(int i_iRow, int i_iColumn, const SEditTriggerKey& i_key);
    bool removeKeyAsEditTriggerForTable(const SEditTriggerKey& i_key);
    bool removeKeyAsEditTriggerForColumn(int i_iColumn, const SEditTriggerKey& i_key);
    bool removeKeyAsEditTriggerForCell(int i_iRow, int i_iColumn, const SEditTriggerKey& i_key);
    bool isEditTriggerKeyForTable(const SEditTriggerKey& i_key);
    bool isEditTriggerKeyForColumn(int i_iColumn, const SEditTriggerKey& i_key);
    bool isEditTriggerKeyForCell(int i_iRow, int i_iColumn, const SEditTriggerKey& i_key);
protected: // overridables of base class QTableView
    void keyPressEvent(QKeyEvent* i_pEv) override;
protected: // instance members
    struct SEditTrigger {
        SEditTrigger() : m_key(), m_iRow(-1), m_iColumn(-1) { }
        SEditTrigger(const SEditTriggerKey& i_key) : m_key(i_key), m_iRow(-1), m_iColumn(-1) { }
        SEditTrigger(int i_iColumn, const SEditTriggerKey& i_key) : m_key(i_key), m_iRow(-1), m_iColumn(i_iColumn) { }
        SEditTrigger(int i_iRow, int i_iColumn, const SEditTriggerKey& i_key) : m_key(i_key), m_iRow(i_iRow), m_iColumn(i_iColumn) { }
        bool operator == (const SEditTrigger& i_other) const {
            return (m_key == i_other.m_key && m_iRow == i_other.m_iRow && m_iColumn == i_other.m_iColumn);
        }
        bool operator != (const SEditTrigger& i_other) const {
            return !(*this == i_other);
        }
        SEditTriggerKey m_key;
        int m_iRow;
        int m_iColumn;
    };
    QList<SEditTrigger> m_editTriggers;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTableView

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_TableView_h
