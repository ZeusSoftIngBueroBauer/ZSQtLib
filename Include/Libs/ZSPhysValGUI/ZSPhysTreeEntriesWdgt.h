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

#ifndef ZSPhysValGUI_PhysTreeEntriesWdgt_h
#define ZSPhysValGUI_PhysTreeEntriesWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"

class QLineEdit;
class QPushButton;
class QStackedWidget;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;

namespace GUI
{
class CWdgtAbstractTreeEntry;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtTreeEntries : public QWidget
//******************************************************************************
{
    Q_OBJECT
protected: // type definitions and constants
    typedef enum {
        ETreeEntryTypeUndefined = 0,
        ETreeEntryTypePhysSize  = 1,
        ETreeEntryTypeQuantity  = 2,
        ETreeEntryTypePhysUnit  = 3,
        ETreeEntryTypeRatio     = 4,
        ETreeEntryTypeCount
    }   ETreeEntryType;
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtTreeEntries"; }
public: // ctors and dtor
    CWdgtTreeEntries(
        CIdxTreeUnits* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtTreeEntries();
public: // instance methods
    void setIdxTree(CIdxTreeUnits* i_pIdxTree);
    CIdxTreeUnits* idxTree() { return m_pIdxTree; }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
    QString getKeyInTreeOfRootEntry() const;
protected slots:
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTreeUnits* m_pIdxTree;
    QString m_strKeyInTreeOfRootEntry;
    QSize m_szBtns;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QPushButton* m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QLineEdit* m_pEdtRootEntryPath;
    QStackedWidget* m_pStackedWdgtTreeEntryContent;
    QVector<CWdgtAbstractTreeEntry*> m_arpWdgtsTreeEntryContent;

}; // class CWdgtTreeEntries

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysTreeEntriesWdgt_h
