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

#include "ZSSysGUI/ZSSysLastUsedFileTreeWdgtItem.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include <QtCore/qfileinfo.h>
#include <QtGui/qicon.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CTreeWdgtItemLastUsedFile : public QTreeWidgetItem
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeWdgtItemLastUsedFile::CTreeWdgtItemLastUsedFile(
    QTreeWidgetItem* i_pParentItem, const SLastUsedFile& i_lastUsedFile, const QIcon& i_icon) :
//------------------------------------------------------------------------------
    QTreeWidgetItem(i_pParentItem, QStringList({i_lastUsedFile.m_strAbsFilePath})),
    m_lastUsedFile(i_lastUsedFile)
{
    QFileInfo fileInfo(i_lastUsedFile.m_strAbsFilePath);
    QString strText = {
        fileInfo.fileName() + "\t" + i_lastUsedFile.m_dtLastUsed.toString("yyyy-MM-dd mm:hh:ss")
        + "\n" + fileInfo.absolutePath()};
    setText(0, strText);
    setIcon(0, i_icon);
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeWdgtItemLastUsedFile::~CTreeWdgtItemLastUsedFile()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
