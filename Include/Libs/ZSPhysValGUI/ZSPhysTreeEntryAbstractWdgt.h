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

#ifndef ZSPhysValGUI_PhysTreeEntryAbstractWdgt_h
#define ZSPhysValGUI_PhysTreeEntryAbstractWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;

namespace GUI
{
//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtAbstractTreeEntry : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtAbstractTreeEntry"; }
public: // ctors and dtor
    CWdgtAbstractTreeEntry(QWidget* i_pWdgtParent = nullptr);
    virtual ~CWdgtAbstractTreeEntry();
public: // overridables
    virtual QString nameSpace() const { return CWdgtAbstractTreeEntry::NameSpace(); }
    virtual QString className() const { return CWdgtAbstractTreeEntry::ClassName(); }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
    QString getKeyInTreeOfRootEntry() const;
public: // overridables
    virtual void resizeToContents();
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTreeUnits* m_pIdxTree;
    QString m_strKeyInTreeOfRootEntry;
    int m_cxLblWidth;
    int m_cxEdtWidth;

}; // class CWdgtAbstractTreeEntry

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysTreeEntryAbstractWdgt_h
