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

#ifndef ZSSysGUI_LastUsedFileTreeWdgtItem_h
#define ZSSysGUI_LastUsedFileTreeWdgtItem_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysApp.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtreewidget.h>
#else
#include <QtWidgets/qtreewidget.h>
#endif

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
/*! @brief 

*/
class ZSSYSGUIDLL_API CTreeWdgtItemLastUsedFile : public QTreeWidgetItem
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CTreeWdgtItemLastUsedFile"; }
public: // ctors and dtor
    CTreeWdgtItemLastUsedFile(QTreeWidgetItem* i_pParentItem, const SLastUsedFile& i_lastUsedFile, const QIcon& i_icon);
    virtual ~CTreeWdgtItemLastUsedFile();
public: // instance methods
    SLastUsedFile const lastUsedFile() const { return m_lastUsedFile; }
protected: // instance members
    SLastUsedFile m_lastUsedFile;

}; // class CTreeWdgtItemLastUsedFile

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_LastUsedFileTreeWdgtItem_h
