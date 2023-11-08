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

#include <QtCore/qmutex.h>
#include <QtCore/qfile.h>
#include <QtCore/qthread.h>
#include <QtCore/qtextstream.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qicon.h>

#include "ZSSysGUI/ZSSysLoggerIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CModelIdxTreeLoggers : public CModelIdxTree
*******************************************************************************/

/*==============================================================================
protected: // class members (hiding class members of base class CModelIdxTree)
==============================================================================*/

bool CModelIdxTreeLoggers::s_bIconsCreated = false;

QPixmap* CModelIdxTreeLoggers::s_pPxmBranchEntryNormalOff;
QPixmap* CModelIdxTreeLoggers::s_pPxmBranchEntrySelectedOff;
QPixmap* CModelIdxTreeLoggers::s_pPxmLeaveEntryNormalOff;
QPixmap* CModelIdxTreeLoggers::s_pPxmLeaveEntrySelectedOff;
QIcon*   CModelIdxTreeLoggers::s_pIconBranch;
QIcon*   CModelIdxTreeLoggers::s_pIconLeave;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QIcon CModelIdxTreeLoggers::GetIcon( const QString& i_strEntryType )
//------------------------------------------------------------------------------
{
    QIcon icon;

    if( !s_bIconsCreated )
    {
        #ifdef _WINDOWS
        #pragma push_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
        #pragma warning( disable : 4005 )
        #define _ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE 0
        #endif

        s_pPxmBranchEntryNormalOff = new QPixmap(":/ZS/TreeView/Loggers/TreeViewLoggersBranchEntryNormalOff.bmp");
        s_pPxmBranchEntryNormalOff->setMask(s_pPxmBranchEntryNormalOff->createHeuristicMask());

        s_pPxmBranchEntrySelectedOff = new QPixmap(":/ZS/TreeView/Loggers/TreeViewLoggersBranchEntrySelectedOff.bmp");
        s_pPxmBranchEntrySelectedOff->setMask(s_pPxmBranchEntrySelectedOff->createHeuristicMask());

        s_pPxmLeaveEntryNormalOff = new QPixmap(":/ZS/TreeView/Loggers/TreeViewLoggersLeaveEntryNormalOff.bmp");
        s_pPxmLeaveEntryNormalOff->setMask(s_pPxmLeaveEntryNormalOff->createHeuristicMask());

        s_pPxmLeaveEntrySelectedOff = new QPixmap(":/ZS/TreeView/Loggers/TreeViewLoggersLeaveEntrySelectedOff.bmp");
        s_pPxmLeaveEntrySelectedOff->setMask(s_pPxmLeaveEntrySelectedOff->createHeuristicMask());

        s_pIconBranch = new QIcon();

        s_pIconBranch->addPixmap(*s_pPxmBranchEntryNormalOff,   QIcon::Normal,   QIcon::Off);
        s_pIconBranch->addPixmap(*s_pPxmBranchEntrySelectedOff, QIcon::Selected, QIcon::Off);
        s_pIconBranch->addPixmap(*s_pPxmBranchEntryNormalOff,   QIcon::Normal,   QIcon::On);
        s_pIconBranch->addPixmap(*s_pPxmBranchEntrySelectedOff, QIcon::Selected, QIcon::On);

        s_pIconLeave = new QIcon();

        s_pIconLeave->addPixmap(*s_pPxmLeaveEntryNormalOff,   QIcon::Normal,   QIcon::Off);
        s_pIconLeave->addPixmap(*s_pPxmLeaveEntrySelectedOff, QIcon::Selected, QIcon::Off);
        s_pIconLeave->addPixmap(*s_pPxmLeaveEntryNormalOff,   QIcon::Normal,   QIcon::On);
        s_pIconLeave->addPixmap(*s_pPxmLeaveEntrySelectedOff, QIcon::Selected, QIcon::On);

        #ifdef _WINDOWS
        #pragma warning( default : 4005 )
        #pragma pop_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
        #endif

        s_bIconsCreated = true;
    }

    if( i_strEntryType.startsWith("R") )
    {
        if( s_pIconRoot != nullptr )
        {
            icon = *s_pIconBranch;
        }
    }
    else if( i_strEntryType.startsWith("B") )
    {
        if( s_pIconBranch != nullptr )
        {
            icon = *s_pIconBranch;
        }
    }
    else if( i_strEntryType.startsWith("L") )
    {
        if( s_pIconLeave != nullptr )
        {
            icon = *s_pIconLeave;
        }
    }

    return icon;

} // GetIcon

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelIdxTreeLoggers::CModelIdxTreeLoggers(
    CIdxTreeLoggers* i_pLoggerIdxTree,
    QObject*         i_pObjParent ) :
//------------------------------------------------------------------------------
    CModelIdxTree(i_pLoggerIdxTree, i_pObjParent)
{
}

//------------------------------------------------------------------------------
CModelIdxTreeLoggers::~CModelIdxTreeLoggers()
//------------------------------------------------------------------------------
{
    // Please note that instance counter will be decremented by the base class CModelIdxTree.
    // If the instance counter is 1 it will be decremented to 0.
    if( s_iInstCount == 1 )
    {
        delete s_pIconBranch;
        s_pIconBranch = nullptr;
        delete s_pIconLeave;
        s_pIconLeave = nullptr;

        delete s_pPxmBranchEntryNormalOff;
        s_pPxmBranchEntryNormalOff = nullptr;
        delete s_pPxmBranchEntrySelectedOff;
        s_pPxmBranchEntrySelectedOff = nullptr;
        delete s_pPxmLeaveEntryNormalOff;
        s_pPxmLeaveEntryNormalOff = nullptr;
        delete s_pPxmLeaveEntrySelectedOff;
        s_pPxmLeaveEntrySelectedOff = nullptr;

        s_bIconsCreated = false;
    }
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreeLoggers* CModelIdxTreeLoggers::idxTree()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CIdxTreeLoggers*>(m_pIdxTree);
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelIdxTreeLoggers::columnCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    return EColumnCount;
}

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelIdxTreeLoggers::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        if( i_iSection >= 0 && i_iSection < CModelIdxTree::EColumnCount )
        {
            varData = CModelIdxTree::headerData(i_iSection, i_orientation, i_iRole);
        }
        else if( i_iSection >= CModelIdxTree::EColumnCount )
        {
            switch( i_iSection )
            {
                case EColumnEnabled:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "Enabled";
                    }
                    break;
                }
                case EColumnLogLevel:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "LogLevel";
                    }
                    break;
                }
                case EColumnDataFilter:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "DataFilter";
                    }
                    break;
                }
                case EColumnAddThreadName:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "AddThreadName";
                    }
                    break;
                }
                case EColumnAddDateTime:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "AddDateTime";
                    }
                    break;
                }
                case EColumnAddSystemTime:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "AddSystemTime";
                    }
                    break;
                }
                case EColumnNameSpace:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "NameSpace";
                    }
                    break;
                }
                case EColumnClassName:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "ClassName";
                    }
                    break;
                }
                case EColumnObjName:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "ObjName";
                    }
                    break;
                }
                case EColumnObjAddress:
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = "ObjAddress";
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
            } // switch( i_iSection )
        } // if( i_iSection >= CModelIdxTree::EColumnCount )
    } // if( i_orientation == Qt::Horizontal )

    else // if( i_orientation == Qt::Vertical )
    {
    }

    return varData;

} // headerData

//------------------------------------------------------------------------------
Qt::ItemFlags CModelIdxTreeLoggers::flags( const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    Qt::ItemFlags uFlags = QAbstractItemModel::flags(i_modelIdx);

    if( i_modelIdx.isValid() )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        CLogger* pLogger = nullptr;

        if( pModelTreeEntry != nullptr && pModelTreeEntry->isLeave() )
        {
            pLogger = dynamic_cast<CLogger*>(pModelTreeEntry->getIdxTreeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case EColumnEnabled:
            case EColumnLogLevel:
            case EColumnDataFilter:
            case EColumnAddThreadName:
            case EColumnAddDateTime:
            case EColumnAddSystemTime:
            {
                if( pLogger != nullptr )
                {
                    uFlags |= Qt::ItemIsEditable;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return uFlags;
}

//------------------------------------------------------------------------------
QVariant CModelIdxTreeLoggers::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( i_modelIdx.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    }

    if( pModelTreeEntry != nullptr )
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        CLogger* pLogger = nullptr;

        if( pModelTreeEntry->isLeave() )
        {
            pLogger = dynamic_cast<CLogger*>(pModelTreeEntry->getIdxTreeEntry());
        }

        switch( i_modelIdx.column() )
        {
            case EColumnTreeEntryName:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole || i_iRole == Qt::ToolTipRole || i_iRole == Qt::ForegroundRole)
                {
                    QString strKeyInTree = pModelTreeEntry->keyInTree();

                    if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                    {
                        varData = pModelTreeEntry->name();
                    }
                    else if( i_iRole == Qt::ToolTipRole )
                    {
                        QString strData;
                        strData  = "KeyInTree: " + strKeyInTree;
                        strData += "\nKeyInParentBranch: " + pModelTreeEntry->keyInParentBranch();
                        strData += "\nIdxInTree: " + QString::number(pModelTreeEntry->indexInTree());
                        strData += "\nIdxInParentBranch: " + QString::number(pModelTreeEntry->indexInParentBranch());
                        strData += "\nSelected: " + bool2Str(pModelTreeEntry->isSelected()) ;
                        if( pModelTreeEntry->isLeave() )
                        {
                            strData += "\nExpanded: " + bool2Str(pModelTreeEntry->isExpanded());
                        }
                        varData = strData;
                    }
                }
                else if( i_iRole == Qt::DecorationRole )
                {
                    varData = GetIcon(pModelTreeEntry->entryTypeSymbol());
                }
                break;
            }
            case EColumnInternalId:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = QString::number(i_modelIdx.internalId()) + " (" + pointer2Str(i_modelIdx.internalPointer()) + ")";
                }
                break;
            }
            case EColumnTreeEntryType:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->entryType2Str();
                }
                break;
            }
            case EColumnIdxInTree:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->indexInTree();
                }
                break;
            }
            case EColumnIdxInParentBranch:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->indexInParentBranch();
                }
                break;
            }
            case EColumnKeyInTree:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->keyInTree();
                }
                break;
            }
            case EColumnKeyInParentBranch:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = pModelTreeEntry->keyInParentBranch();
                }
                break;
            }
            case EColumnEnabled:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = pLogger->isEnabled();
                    }
                }
                break;
            }
            case EColumnLogLevel:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = CEnumLogDetailLevel(pLogger->getLogLevel()).toString();
                    }
                }
                break;
            }
            case EColumnDataFilter:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = pLogger->getDataFilter();
                    }
                }
                break;
            }
            case EColumnAddThreadName:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = pLogger->addThreadName();
                    }
                }
                break;
            }
            case EColumnAddDateTime:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = pLogger->addDateTime();
                    }
                }
                break;
            }
            case EColumnAddSystemTime:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = pLogger->addSystemTime();
                    }
                }
                break;
            }
            case EColumnNameSpace:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = pLogger->getNameSpace();
                    }
                }
                break;
            }
            case EColumnClassName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = pLogger->getClassName();
                    }
                }
                break;
            }
            case EColumnObjName:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pLogger != nullptr )
                    {
                        varData = pLogger->getObjectName();
                    }
                }
                break;
            }
            case EColumnObjAddress:
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    if( pLogger != nullptr )
                    {
                        if( sizeof(pLogger) == sizeof(quint32) ) // 32 bit machines
                        {
                            quint32 ulAddr;
                            quint32* pulAddr = &ulAddr;
                            memcpy( pulAddr, &pLogger, sizeof(pLogger) );
                            varData = "0x" + QString::number(ulAddr,16);
                        }
                        else if( sizeof(pLogger) == sizeof(quint64) ) // 64 bit machines
                        {
                            quint64 ulAddr;
                            quint64* pulAddr = &ulAddr;
                            memcpy( pulAddr, &pLogger, sizeof(pLogger) );
                            varData = "0x" + QString::number(ulAddr,16);
                        }
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        } // switch( i_modelIdx.column() )
    } // if( pModelTreeEntry != nullptr )

    return varData;

} // data

//------------------------------------------------------------------------------
bool CModelIdxTreeLoggers::setData( const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    if( i_iRole == Qt::EditRole && i_varData.isValid() && i_modelIdx.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = nullptr;

        if( i_modelIdx.isValid() )
        {
            pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        }

        if( pModelTreeEntry != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);

            CLogger* pLogger = nullptr;

            if( pModelTreeEntry->isLeave() )
            {
                pLogger = dynamic_cast<CLogger*>(pModelTreeEntry->getIdxTreeEntry());
            }

            switch( i_modelIdx.column() )
            {
                case EColumnEnabled:
                {
                    if( pLogger != nullptr )
                    {
                        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        if( i_varData.canConvert(QVariant::Bool) )
                        #else
                        // static_cast to avoid deprecation warning
                        if( i_varData.canConvert(static_cast<QMetaType>(QMetaType::Bool)) )
                        #endif
                        {
                            try
                            {
                                CEnumEnabled enabled = static_cast<EEnabled>(i_varData.toBool());
                                pLogger->setEnabled(enabled.enumerator());
                            }
                            catch(CException&)
                            {
                            }
                        }
                    }
                    break;
                }
                case EColumnLogLevel:
                {
                    if( pLogger != nullptr )
                    {
                        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        if( i_varData.type() == QVariant::Int )
                        #else
                        if( i_varData.typeId() == QMetaType::Int )
                        #endif
                        {
                            try
                            {
                                CEnumLogDetailLevel eDetailLevel(i_varData.toInt());
                                pLogger->setLogLevel(eDetailLevel.enumerator());
                            }
                            catch(CException&)
                            {
                            }
                        }
                        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        if( i_varData.type() == QVariant::String )
                        #else
                        if( i_varData.typeId() == QMetaType::QString )
                        #endif
                        {
                            try
                            {
                                CEnumLogDetailLevel eDetailLevel(i_varData.toString());
                                pLogger->setLogLevel(eDetailLevel.enumerator());
                            }
                            catch(CException&)
                            {
                            }
                        }
                    }
                    break;
                }
                case EColumnDataFilter:
                {
                    if( pLogger != nullptr )
                    {
                        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        if( i_varData.type() == QVariant::String )
                        #else
                        if( i_varData.typeId() == QMetaType::QString )
                        #endif
                        {
                            pLogger->setDataFilter(i_varData.toString());
                        }
                    }
                    break;
                }
                case EColumnAddThreadName:
                {
                    if( pLogger != nullptr )
                    {
                        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        if( i_varData.canConvert(QVariant::Bool) )
                        #else
                        // static_cast to avoid deprecation warning
                        if( i_varData.canConvert(static_cast<QMetaType>(QMetaType::Bool)) )
                        #endif
                        {
                            pLogger->setAddThreadName(i_varData.toBool());
                        }
                    }
                    break;
                }
                case EColumnAddDateTime:
                {
                    if( pLogger != nullptr )
                    {
                        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        if( i_varData.canConvert(QVariant::Bool) )
                        #else
                        // static_cast to avoid deprecation warning
                        if( i_varData.canConvert(static_cast<QMetaType>(QMetaType::Bool)) )
                        #endif
                        {
                            pLogger->setAddDateTime(i_varData.toBool());
                        }
                    }
                    break;
                }
                case EColumnAddSystemTime:
                {
                    if( pLogger != nullptr )
                    {
                        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        if( i_varData.canConvert(QVariant::Bool) )
                        #else
                        // static_cast to avoid deprecation warning
                        if( i_varData.canConvert(static_cast<QMetaType>(QMetaType::Bool)) )
                        #endif
                        {
                            pLogger->setAddSystemTime(i_varData.toBool());
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            } // switch( i_modelIdx.column() )
        } // if( pModelTreeEntry != nullptr )

        bOk = true;

    } // if( i_iRole == Qt::EditRole && i_varData.isValid() && i_modelIdx.isValid() )

    if( bOk )
    {
        // Please note that if the name has been changed there is no need to emit the
        // dataChanged signal here. The index tree will change the name of the node and
        // will emit the signals in the following order:
        // - treeEntryAboutToBeRenamed
        // - treeEntryKeyInTreeChanged
        // - treeEntryRenamed
        // This model is connected to the "treeEntryKeyInTreeChanged" signal
        // and emits the dataChanged signal for the changed tree node.
        // Please note that the index tree may be changed by anyone else
        // and not just be this model so this is the way it has to be.
        // But emitting the signal in case any other attribute has been changed
        // (e.g. the selection state) the data changed signal must be emitted.
        // So we do it here for all attributes.
        emit dataChanged(i_modelIdx, i_modelIdx);
    }

    return bOk;

} // setData
