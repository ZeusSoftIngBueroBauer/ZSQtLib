/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qmutex.h>
#include <QtCore/qfile.h>
#include <QtCore/qthread.h>
#include <QtXml/qdom.h>

#include "ZSRemCmdGUI/ZSRemCmdObjPoolModel.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdSubSysNode.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::RemCmd;
using namespace ZS::RemCmd::GUI;


/*******************************************************************************
class CRemCmdObjPoolModel : ZS::System::GUI::CModelObjPool
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdObjPoolModel::CRemCmdObjPoolModel( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    ZS::System::GUI::CModelObjPool(nullptr, i_pObjParent)
{
} // ctor

//------------------------------------------------------------------------------
CRemCmdObjPoolModel::~CRemCmdObjPoolModel()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObjPoolModel::setRemCmdObjPool( CRemCmdObjPool* i_pObjPool )
//------------------------------------------------------------------------------
{
    setObjPool(i_pObjPool);
}

//------------------------------------------------------------------------------
CRemCmdObjPool* CRemCmdObjPoolModel::getRemCmdObjPool()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CRemCmdObjPool*>(m_pObjPool);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObjPoolModel::reset()
//------------------------------------------------------------------------------
{
    beginResetModel();
    endResetModel();
}

/*==============================================================================
public: // overridables of base class CModelObjPool
==============================================================================*/

//------------------------------------------------------------------------------
int CRemCmdObjPoolModel::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CRemCmdObjPoolModel::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    // The list of objects must be protected as adding and removing
    // objects might be called from within different thread contexts.
    QMutexLocker mtxLocker(m_pObjPool->getMutex());

    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    CObjPoolTreeEntry* pTreeEntry = static_cast<CObjPoolTreeEntry*>(i_modelIdx.internalPointer());

    if( pTreeEntry == nullptr )
    {
        return varData;
    }

    CRemCmdObj* pRemCmdObj = nullptr;

    if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
    {
        pRemCmdObj = reinterpret_cast<CRemCmdObj*>(pTreeEntry->getObj());
    }

    switch( i_modelIdx.column() )
    {
        case EColumnNodeName:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                varData = pTreeEntry->getNodeName(false);
            }
            else if( i_iRole == Qt::DecorationRole )
            {
                varData = ZS::System::GUI::CModelObjPool::GetIcon(pTreeEntry->getEntryType());
            }
            break;
        }
        case EColumnType:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                varData = objPoolEntryType2Str(pTreeEntry->getEntryType());
            }
            break;
        }
        case EColumnObjId:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
                {
                    varData = pTreeEntry->getObjId();
                }
            }
            break;
        }
        case EColumnSyntax:
        {
            if( i_iRole == Qt::DisplayRole)
            {
                if( pRemCmdObj != nullptr )
                {
                    varData = pRemCmdObj->getSyntax();
                }
            }
            break;
        }
        case EColumnIsQuery:
        {
            if( i_iRole == Qt::DisplayRole)
            {
                if( pRemCmdObj != nullptr )
                {
                    varData = !pRemCmdObj->isEventOnly();
                }
            }
            break;
        }
        case EColumnIsEvent:
        {
            if( i_iRole == Qt::DisplayRole)
            {
                if( pRemCmdObj != nullptr )
                {
                    varData = !pRemCmdObj->isQueryOnly();
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return varData;

} // data

//------------------------------------------------------------------------------
QVariant CRemCmdObjPoolModel::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        switch( i_iSection )
        {
            case EColumnNodeName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Name";
                }
                break;
            }
            case EColumnType:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Type";
                }
                break;
            }
            case EColumnObjId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Id";
                }
                break;
            }
            case EColumnSyntax:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Syntax";
                }
                break;
            }
            case EColumnIsQuery:
            {
                if( i_iRole == Qt::DisplayRole)
                {
                    varData = "IsQuery";
                }
                break;
            }
            case EColumnIsEvent:
            {
                if( i_iRole == Qt::DisplayRole)
                {
                    varData = "IsEvent";
                }
                break;
            }
            //case EColumnDescription:
            //{
            //    if( i_iRole == Qt::DisplayRole )
            //    {
            //        varData = "Description";
            //    }
            //    break;
            //}
            default:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "?";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
        }
    }
    else // if( i_orientation == Qt::Vertical )
    {
    }
    return varData;

} // headerData

