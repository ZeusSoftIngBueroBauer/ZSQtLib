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

#include "ZSPhysValGUI/ZSPhysTreeEntryUserDefinedQuantityWdgt.h"
#include "ZSPhysVal/ZSPhysSizesIdxTree.h"

#if QT_VERSION < 0x050000
//#include <QtGui/qcheckbox.h>
//#include <QtGui/qcombobox.h>
//#include <QtGui/qheaderview.h>
//#include <QtGui/qlayout.h>
//#include <QtGui/qlabel.h>
//#include <QtGui/qlineedit.h>
//#include <QtGui/qsplitter.h>
//#include <QtGui/qtableview.h>
#else
//#include <QtWidgets/qcheckbox.h>
//#include <QtWidgets/qcombobox.h>
//#include <QtWidgets/qheaderview.h>
//#include <QtWidgets/qlayout.h>
//#include <QtWidgets/qlabel.h>
//#include <QtWidgets/qlineedit.h>
//#include <QtWidgets/qsplitter.h>
//#include <QtWidgets/qtableview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;


/*******************************************************************************
class CWdgtUserDefinedQuantity : public QTableView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtUserDefinedQuantity::CWdgtUserDefinedQuantity(
    CIdxTreePhysSizes* i_pIdxTree, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    CWdgtAbstractTreeEntry(i_pIdxTree, i_pWdgtParent)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

} // ctor

//------------------------------------------------------------------------------
CWdgtUserDefinedQuantity::~CWdgtUserDefinedQuantity()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtUserDefinedQuantity::setKeyInTreeOfRootEntry( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTreeOfRootEntry != i_strKeyInTree )
    {
        m_strKeyInTreeOfRootEntry = i_strKeyInTree;

        //m_pModel->setKeyInTreeOfRootEntry(i_strKeyInTree);
    }
}
