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

#include <QtCore/qmimedata.h>
#include <QtCore/qmutex.h>
#include <QtCore/qfile.h>
#include <QtCore/qthread.h>
#include <QtXml/qdom.h>

#include "ZSDraw/ZSDrawObjFactoriesModel.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CObjFactoriesModel : ZS::System::GUI::CModelObjPool
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoriesModel::CObjFactoriesModel(
    CObjPool* i_pObjPool,
    QObject*  i_pObjParent ) :
//------------------------------------------------------------------------------
    ZS::System::GUI::CModelObjPool(
        /* pObjPool   */ i_pObjPool,
        /* pObjParent */ i_pObjParent )
{
} // ctor

//------------------------------------------------------------------------------
CObjFactoriesModel::~CObjFactoriesModel()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class QAbstractItemModel, overriding implementation in class CModelObjPool
==============================================================================*/

//------------------------------------------------------------------------------
int CObjFactoriesModel::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CObjFactoriesModel::data( const QModelIndex& i_modelIdx, int i_iRole ) const
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

    CObjFactory* pObjFactory = reinterpret_cast<CObjFactory*>(pTreeEntry->getObj());

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
                if( pObjFactory == nullptr )
                {
                    varData = ZS::System::GUI::CModelObjPool::GetIcon(pTreeEntry->getEntryType());
                }
                else
                {
                    varData = pObjFactory->getToolIcon();
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
Qt::ItemFlags CObjFactoriesModel::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(i_modelIdx);

    if( !i_modelIdx.isValid() )
    {
        return flags;
    }

    CObjPoolTreeEntry* pTreeEntry = static_cast<CObjPoolTreeEntry*>(i_modelIdx.internalPointer());
    CObjFactory*       pObjFactory = nullptr;

    if( pTreeEntry != nullptr )
    {
        pObjFactory = reinterpret_cast<CObjFactory*>(pTreeEntry->getObj());
    }
    if( pObjFactory == nullptr )
    {
        return flags;
    }

    flags |= Qt::ItemIsDragEnabled;

    return flags;

} // flags

//------------------------------------------------------------------------------
QVariant CObjFactoriesModel::headerData(
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

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QStringList CObjFactoriesModel::mimeTypes() const
//------------------------------------------------------------------------------
{
    QStringList strlstTypes;

    strlstTypes << "application/vnd.text.list";

    return strlstTypes;

} // mimeTypes

//------------------------------------------------------------------------------
QMimeData* CObjFactoriesModel::mimeData( const QModelIndexList& i_modelIdxList ) const
//------------------------------------------------------------------------------
{
    QMimeData* pMimeData = new QMimeData();

    CObjPoolTreeEntry* pTreeEntry;
    CObjFactory*       pObjFactory;
    QString            strObjFactoryGroupName;
    QString            strObjFactoryGraphObjType;
    QString            strMimeData;
    QStringList        strlstMimeData;
    QModelIndex        modelIdx;
    int                idx;

    for( idx = 0; idx < i_modelIdxList.size(); idx++ )
    {
        modelIdx = i_modelIdxList[idx];

        if( modelIdx.isValid() )
        {
            pTreeEntry = static_cast<CObjPoolTreeEntry*>(modelIdx.internalPointer());

            if( pTreeEntry != nullptr )
            {
                pObjFactory = reinterpret_cast<CObjFactory*>(pTreeEntry->getObj());

                if( pObjFactory != nullptr )
                {
                    strObjFactoryGroupName = pObjFactory->getGroupName();
                    strObjFactoryGraphObjType = pObjFactory->getGraphObjTypeAsString();

                    strMimeData = "ObjFactory::";

                    if( strObjFactoryGroupName.isEmpty() )
                    {
                        strMimeData += strObjFactoryGraphObjType;
                    }
                    else
                    {
                        strMimeData += strObjFactoryGroupName + "::"+ strObjFactoryGraphObjType;
                    }
                    strlstMimeData.append(strMimeData);
                }
            }
        }
    }

    pMimeData->setText( strlstMimeData.join("; ") );

    return pMimeData;

} // mimeData
