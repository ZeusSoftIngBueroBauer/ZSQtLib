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

#include <QtCore/qfile.h>
#include <QtCore/qmutex.h>
#include <QtCore/qsize.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qtextstream.h>
#include <QtXml/qdom.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qicon.h>

#include "ZSSysGUI/ZSSysObjPoolModel.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CModelObjPool : public QAbstractItemModel
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

// Should be protected class members.
// But python bindings generated with shiboken will not be compilable then.

static int /*CModelObjPool::*/s_iInstCount = 0;

static bool /*CModelObjPool::*/s_bIconsCreated = false;

static QPixmap* /*CModelObjPool::*/s_pPxmTypeNameSpaceNormalOff = nullptr;
static QPixmap* /*CModelObjPool::*/s_pPxmTypeNameSpaceSelectedOff = nullptr;
static QIcon*   /*CModelObjPool::*/s_pIconTypeNameSpace = nullptr;
static QPixmap* /*CModelObjPool::*/s_pPxmTypeObjectNormalOff = nullptr;
static QPixmap* /*CModelObjPool::*/s_pPxmTypeObjectSelectedOff = nullptr;
static QIcon*   /*CModelObjPool::*/s_pIconTypeObject = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QIcon CModelObjPool::GetIcon( EObjPoolEntryType i_entryType )
//------------------------------------------------------------------------------
{
    QIcon icon;

    if( !s_bIconsCreated )
    {
        QSize sizePxm;

        s_pIconTypeNameSpace = new QIcon();
        s_pIconTypeObject = new QIcon();

        s_pPxmTypeNameSpaceNormalOff = new QPixmap(":/ZS/ObjPoolExplorer/ObjPoolEntryTypeNameSpaceNormalOff.bmp");
        s_pPxmTypeNameSpaceNormalOff->setMask(s_pPxmTypeNameSpaceNormalOff->createHeuristicMask());

        s_pPxmTypeNameSpaceSelectedOff = new QPixmap(":/ZS/ObjPoolExplorer/ObjPoolEntryTypeNameSpaceSelectedOff.bmp");
        s_pPxmTypeNameSpaceSelectedOff->setMask(s_pPxmTypeNameSpaceSelectedOff->createHeuristicMask());

        s_pPxmTypeObjectNormalOff = new QPixmap(":/ZS/ObjPoolExplorer/ObjPoolEntryTypeObjectNormalOff.bmp");
        s_pPxmTypeObjectNormalOff->setMask(s_pPxmTypeObjectNormalOff->createHeuristicMask());

        s_pPxmTypeObjectSelectedOff = new QPixmap(":/ZS/ObjPoolExplorer/ObjectPoolEntryTypeObjectSelectedOff.bmp");
        s_pPxmTypeObjectSelectedOff->setMask(s_pPxmTypeObjectSelectedOff->createHeuristicMask());

        sizePxm = s_pPxmTypeNameSpaceNormalOff->size();
        sizePxm = s_pPxmTypeNameSpaceSelectedOff->size();

        s_pIconTypeNameSpace->addPixmap(*s_pPxmTypeNameSpaceNormalOff,QIcon::Normal,QIcon::Off);
        s_pIconTypeNameSpace->addPixmap(*s_pPxmTypeNameSpaceSelectedOff,QIcon::Selected,QIcon::Off);
        s_pIconTypeNameSpace->addPixmap(*s_pPxmTypeNameSpaceNormalOff,QIcon::Normal,QIcon::On);
        s_pIconTypeNameSpace->addPixmap(*s_pPxmTypeNameSpaceSelectedOff,QIcon::Selected,QIcon::On);

        sizePxm = s_pPxmTypeObjectNormalOff->size();
        sizePxm = s_pPxmTypeObjectSelectedOff->size();

        s_pIconTypeObject->addPixmap(*s_pPxmTypeObjectNormalOff,QIcon::Normal,QIcon::Off);
        s_pIconTypeObject->addPixmap(*s_pPxmTypeObjectSelectedOff,QIcon::Selected,QIcon::Off);
        s_pIconTypeObject->addPixmap(*s_pPxmTypeObjectNormalOff,QIcon::Normal,QIcon::On);
        s_pIconTypeObject->addPixmap(*s_pPxmTypeObjectSelectedOff,QIcon::Selected,QIcon::On);

        s_bIconsCreated = true;
    }

    switch( i_entryType )
    {
        case EObjPoolEntryTypeRoot:
        case EObjPoolEntryTypeNameSpace:
        {
            if( s_pIconTypeNameSpace != nullptr )
            {
                icon = *s_pIconTypeNameSpace;
            }
            break;
        }
        case EObjPoolEntryTypeObject:
        {
            if( s_pIconTypeObject != nullptr )
            {
                icon = *s_pIconTypeObject;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return icon;

} // GetIcon

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CModelObjPool::AreIconsCreated()
//------------------------------------------------------------------------------
{
    return s_bIconsCreated;
}

//------------------------------------------------------------------------------
QPixmap* CModelObjPool::GetPxmTypeNameSpaceNormalOff()
//------------------------------------------------------------------------------
{
    return s_pPxmTypeNameSpaceNormalOff;
}

//------------------------------------------------------------------------------
QPixmap* CModelObjPool::GetPxmTypeNameSpaceSelectedOff()
//------------------------------------------------------------------------------
{
    return s_pPxmTypeNameSpaceSelectedOff;
}

//------------------------------------------------------------------------------
QIcon* CModelObjPool::GetIconTypeNameSpace()
//------------------------------------------------------------------------------
{
    return s_pIconTypeNameSpace;
}

//------------------------------------------------------------------------------
QPixmap* CModelObjPool::GetPxmTypeObjectNormalOff()
//------------------------------------------------------------------------------
{
    return s_pPxmTypeObjectNormalOff;
}

//------------------------------------------------------------------------------
QPixmap* CModelObjPool::GetPxmTypeObjectSelectedOff()
//------------------------------------------------------------------------------
{
    return s_pPxmTypeObjectSelectedOff;
}

//------------------------------------------------------------------------------
QIcon* CModelObjPool::GetIconTypeObject()
//------------------------------------------------------------------------------
{
    return s_pIconTypeObject;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelObjPool::CModelObjPool( CObjPool* i_pObjPool, QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QAbstractItemModel(i_pObjParent),
    m_pObjPool(nullptr)
{
    s_iInstCount++;

    if( i_pObjPool != nullptr )
    {
        setObjPool(i_pObjPool);
    }

} // ctor

//------------------------------------------------------------------------------
CModelObjPool::~CModelObjPool()
//------------------------------------------------------------------------------
{
    s_iInstCount--;

    if( s_iInstCount == 0 )
    {
        delete s_pPxmTypeNameSpaceNormalOff;
        s_pPxmTypeNameSpaceNormalOff = nullptr;
        delete s_pPxmTypeNameSpaceSelectedOff;
        s_pPxmTypeNameSpaceSelectedOff = nullptr;
        delete s_pIconTypeNameSpace;
        s_pIconTypeNameSpace = nullptr;
        delete s_pPxmTypeObjectNormalOff;
        s_pPxmTypeObjectNormalOff = nullptr;
        delete s_pPxmTypeObjectSelectedOff;
        s_pPxmTypeObjectSelectedOff = nullptr;
        delete s_pIconTypeObject;
        s_pIconTypeObject = nullptr;

        s_bIconsCreated = false;
    }

    m_pObjPool = nullptr;

} // dtor

//------------------------------------------------------------------------------
void CModelObjPool::setObjPool( CObjPool* i_pObjPool )
//------------------------------------------------------------------------------
{
    if( m_pObjPool != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pObjPool,
            /* szSignal     */ SIGNAL(nodeInserted(QObject*,ZS::System::CObjPoolTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onObjPoolNodeInserted(QObject*,ZS::System::CObjPoolTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pObjPool,
            /* szSignal     */ SIGNAL(nodeChanged(QObject*,ZS::System::CObjPoolTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onObjPoolNodeChanged(QObject*,ZS::System::CObjPoolTreeEntry*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pObjPool,
            /* szSignal     */ SIGNAL(nodeRemoving(QObject*,ZS::System::CObjPoolTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onObjPoolNodeRemoving(QObject*,ZS::System::CObjPoolTreeEntry*)) );

        m_pObjPool = nullptr;

        #if QT_VERSION < 0x050000
        reset();
        #else
        beginResetModel();
        endResetModel();
        #endif

    } // if( m_pObjPool != nullptr )

    m_pObjPool = i_pObjPool;

    if( m_pObjPool != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pObjPool,
            /* szSignal     */ SIGNAL(nodeInserted(QObject*,ZS::System::CObjPoolTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onObjPoolNodeInserted(QObject*,ZS::System::CObjPoolTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pObjPool,
            /* szSignal     */ SIGNAL(nodeChanged(QObject*,ZS::System::CObjPoolTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onObjPoolNodeChanged(QObject*,ZS::System::CObjPoolTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pObjPool,
            /* szSignal     */ SIGNAL(nodeRemoving(QObject*,ZS::System::CObjPoolTreeEntry*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onObjPoolNodeRemoving(QObject*,ZS::System::CObjPoolTreeEntry*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        addTreeEntry( m_pObjPool->getRoot() );

    } // if( m_pObjPool != nullptr )

} // setObjPool

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelObjPool::addTreeEntry( ZS::System::CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    onObjPoolNodeInserted(m_pObjPool, i_pTreeEntry);

    CObjPoolTreeEntry* pTreeEntry;
    int                idxEntry;

    for( idxEntry = 0; idxEntry < static_cast<int>(i_pTreeEntry->getChildCount()); idxEntry++ )
    {
        pTreeEntry = i_pTreeEntry->getChildEntry(idxEntry);

        addTreeEntry(pTreeEntry);
    }

} // addTreeEntry

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CModelObjPool::onObjPoolNodeInserted(
    QObject*                       /*i_pObjPool*/,
    ZS::System::CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    int iRow = i_pTreeEntry->getRowId();

    //CObjPoolTreeEntry* pTreeEntryParent = i_pTreeEntry->getParentEntry();

    //if( pTreeEntryParent != nullptr )
    //{
    //    modelIdxParent = pTreeEntryParent->getModelIdx();
    //}

    QModelIndex modelIdx = createIndex(iRow,0,i_pTreeEntry);

    QModelIndex modelIdxParent = parent(modelIdx);

    //i_pTreeEntry->setModelIdx( createIndex(iRow,0,i_pTreeEntry) );

    beginInsertRows( modelIdxParent, iRow, iRow );
    endInsertRows();

} // onObjPoolNodeInserted

//------------------------------------------------------------------------------
void CModelObjPool::onObjPoolNodeChanged(
    QObject*                       /*i_pObjPool*/,
    ZS::System::CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QModelIndex modelIdxTopLeft = createIndex( i_pTreeEntry->getRowId(), EColumnMin, i_pTreeEntry );
    QModelIndex modelIdxBottomRight = createIndex( i_pTreeEntry->getRowId(), EColumnMax, i_pTreeEntry );

    emit QAbstractItemModel::dataChanged(modelIdxTopLeft,modelIdxBottomRight);

} // onObjPoolNodeChanged

//------------------------------------------------------------------------------
void CModelObjPool::onObjPoolNodeRemoving(
    QObject*                       /*i_pObjPool*/,
    ZS::System::CObjPoolTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    int iRow = i_pTreeEntry->getRowId();

    //CObjPoolTreeEntry* pTreeEntryParent = i_pTreeEntry->getParentEntry();

    //if( pTreeEntryParent != nullptr )
    //{
    //    modelIdxParent = pTreeEntryParent->getModelIdx();
    //}

    QModelIndex modelIdx = createIndex(iRow,0,i_pTreeEntry);

    QModelIndex modelIdxParent = parent(modelIdx);

    beginRemoveRows( modelIdxParent, iRow, iRow );
    endRemoveRows();

} // onObjPoolNodeRemoving

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelObjPool::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( m_pObjPool != nullptr )
    {
        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pObjPool->getMutex());

        CObjPoolTreeEntry* pTreeEntryParent = nullptr;

        if( !i_modelIdxParent.isValid() )
        {
            iRowCount = 1;
        }
        else
        {
            pTreeEntryParent = static_cast<CObjPoolTreeEntry*>(i_modelIdxParent.internalPointer());

            if( pTreeEntryParent == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultInternalProgramError);
            }
            iRowCount = pTreeEntryParent->getChildCount();
        }
    } // if( m_pObjPool != nullptr )

    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelObjPool::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

//------------------------------------------------------------------------------
QVariant CModelObjPool::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
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

    if( m_pObjPool != nullptr )
    {
        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pObjPool->getMutex());

        switch( i_modelIdx.column() )
        {
            case EColumnNodeName:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pTreeEntry->getNodeName(false);
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = GetIcon(pTreeEntry->getEntryType());
                }
                break;
            }
            case EColumnType:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = objPoolEntryType2Str(pTreeEntry->getEntryType());
                }
                break;
            }
            case EColumnObjId:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
                    {
                        varData = pTreeEntry->getObjId();
                    }
                }
                break;
            }
            case EColumnObjAddress:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
                    {
                        if(  pTreeEntry->getObj() == nullptr )
                        {
                            varData = "nullptr";
                        }
                        else
                        {
                            void* pvObj = pTreeEntry->getObj();
                            if( sizeof(pvObj) == sizeof(quint32) ) // 32 bit machines
                            {
                                quint32 ulAddr;
                                quint32* pulAddr = &ulAddr;
                                memcpy( pulAddr, &pvObj, sizeof(pvObj) );
                                varData = "0x" + QString::number(ulAddr,16);
                            }
                            else if( sizeof(pvObj) == sizeof(quint64) ) // 64 bit machines
                            {
                                quint64 ulAddr;
                                quint64* pulAddr = &ulAddr;
                                memcpy( pulAddr, &pvObj, sizeof(pvObj) );
                                varData = "0x" + QString::number(ulAddr,16);
                            }
                        }
                    }
                }
                break;
            }
            case EColumnObjState:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
                    {
                        varData = CEnumObjState::toString(pTreeEntry->getObjState());
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }
    } // if( m_pObjPool != nullptr )

    return varData;

} // data

//------------------------------------------------------------------------------
QModelIndex CModelObjPool::index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdx;

    if( m_pObjPool != nullptr )
    {
        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pObjPool->getMutex());

        if( hasIndex(i_iRow,i_iCol,i_modelIdxParent) )
        {
            CObjPoolTreeEntry* pTreeEntry = nullptr;

            if( !i_modelIdxParent.isValid() )
            {
                pTreeEntry = m_pObjPool->getRoot();
            }
            else
            {
                CObjPoolTreeEntry* pTreeEntryParent = static_cast<CObjPoolTreeEntry*>(i_modelIdxParent.internalPointer());

                if( pTreeEntryParent != nullptr && i_iRow >= 0 && i_iRow < pTreeEntryParent->getChildCount() )
                {
                    pTreeEntry = pTreeEntryParent->getChildEntry(i_iRow);
                }
            }

            modelIdx = createIndex(i_iRow,i_iCol,pTreeEntry);

        } // if( hasIndex(i_iRow,i_iCol,i_modelIdxParent) )
    } // if( m_pObjPool != nullptr )

    return modelIdx;

} // index

//------------------------------------------------------------------------------
QModelIndex CModelObjPool::parent( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdxParent;

    if( m_pObjPool != nullptr )
    {
        // The list of objects must be protected as adding and removing
        // objects might be called from within different thread contexts.
        QMutexLocker mtxLocker(m_pObjPool->getMutex());

        if( i_modelIdx.isValid() )
        {
            CObjPoolTreeEntry* pTreeEntry = static_cast<CObjPoolTreeEntry*>(i_modelIdx.internalPointer());
            CObjPoolTreeEntry* pTreeEntryParent;

            if( pTreeEntry == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultInternalProgramError);
            }
            pTreeEntryParent = pTreeEntry->getParentEntry();

            if( pTreeEntryParent != nullptr )
            {
                modelIdxParent = createIndex( pTreeEntryParent->getRowId(), 0, pTreeEntryParent );
            }
        } // if( i_modelIdx.isValid() )
    } // if( m_pObjPool != nullptr )

    return modelIdxParent;

} // parent

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelObjPool::headerData(
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
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnType:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Type";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnObjId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "Id";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnObjAddress:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "ObjAddress";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
                }
                break;
            }
            case EColumnObjState:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = "ObjState";
                }
                else if( i_iRole == Qt::SizeHintRole )
                {
                    //varData = QSize(10,16);
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
