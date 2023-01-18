/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

*******************************************************************************/

#include "ZSDraw/ZSDrawGraphObjModelEntry.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawGraphicsItemsModel.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
struct CGraphObjModelEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjModelEntry::CGraphObjModelEntry(
    CGraphicsItemsModel* i_pModel,
    CDrawingScene*       i_pDrawingScene,
    EGraphObjType        i_graphObjType,
    const QString&       i_strGraphObjName,
    const QString&       i_strGraphObjId ) :
//------------------------------------------------------------------------------
    m_pModel(i_pModel),
    m_pDrawingScene(i_pDrawingScene),
    m_pParent(nullptr),
    m_modelIdx(),
    m_arpChilds(),
    m_graphObjType(i_graphObjType),
    m_strGraphObjName(i_strGraphObjName),
    m_strGraphObjId(i_strGraphObjId)
{
} // ctor

//------------------------------------------------------------------------------
CGraphObjModelEntry::~CGraphObjModelEntry()
//------------------------------------------------------------------------------
{
    m_pModel = nullptr;
    m_pDrawingScene = nullptr;
    m_pParent = nullptr;
    m_graphObjType = static_cast<EGraphObjType>(0);

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjModelEntry::setParentEntry( CGraphObjModelEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    m_pParent = i_pEntry;
}

//------------------------------------------------------------------------------
QModelIndex CGraphObjModelEntry::getParentModelIdx() const
//------------------------------------------------------------------------------
{
    QModelIndex modelIdxParent;

    if( m_pParent != nullptr )
    {
        modelIdxParent = m_pParent->getModelIdx();
    }
    return modelIdxParent;

} // getParentModelIdx

//------------------------------------------------------------------------------
void CGraphObjModelEntry::setRowId( int i_iRow )
//------------------------------------------------------------------------------
{
    m_modelIdx = m_pModel->createIndex(i_iRow,0,this);
}

//------------------------------------------------------------------------------
int CGraphObjModelEntry::appendChildEntry( CGraphObjModelEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    int iRow = m_arpChilds.size();

    m_arpChilds.append(i_pEntry);

    i_pEntry->setParentEntry(this);
    i_pEntry->setRowId(iRow);

    return iRow;

} // appendChildEntry

//------------------------------------------------------------------------------
int CGraphObjModelEntry::insertChildEntry( CGraphObjModelEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    int                  iRow = m_arpChilds.size();
    CGraphObj*           pGraphObj = i_pEntry->getGraphObj();
    CGraphObjModelEntry* pModelEntryTmp;
    CGraphObj*           pGraphObjTmp;
    int                  iRowTmp;

    if( pGraphObj != nullptr )
    {
        for( iRowTmp = 0; iRowTmp < m_arpChilds.size(); iRowTmp++ )
        {
            pModelEntryTmp = m_arpChilds[iRowTmp];

            if( pModelEntryTmp != nullptr )
            {
                pGraphObjTmp = pModelEntryTmp->getGraphObj();

                if( pGraphObjTmp != nullptr )
                {
                    if( pGraphObjTmp->name().compare(pGraphObj->name(),Qt::CaseInsensitive) == 0 )
                    {
                        iRow = iRowTmp;
                        break;
                    }
                }
            }
        }

    } // if( pGraphObj != nullptr )

    if( iRow >= 0 && iRow < m_arpChilds.size() )
    {
        m_arpChilds.insert(iRow,i_pEntry);

        // "Move" existing entries behind new entry one step downward ..
        for( iRowTmp = iRow+1; iRowTmp < m_arpChilds.size(); iRowTmp++ )
        {
            pModelEntryTmp = m_arpChilds[iRowTmp];

            if( pModelEntryTmp != nullptr )
            {
                pModelEntryTmp->setRowId(iRowTmp);
            }
        }
    }
    else
    {
        m_arpChilds.append(i_pEntry);
    }

    if( i_pEntry != nullptr )
    {
        i_pEntry->setRowId(iRow);
    }
    return iRow;

} // insertChildEntry

//------------------------------------------------------------------------------
void CGraphObjModelEntry::removeChildEntry( int i_iRow )
//------------------------------------------------------------------------------
{
    if( i_iRow < 0 || i_iRow >= m_arpChilds.size() )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange );
    }

    m_arpChilds.removeAt(i_iRow);

    CGraphObjModelEntry* pModelEntry;
    int                  iRow;

    // "Move" entries behind removed entry one step forward ..
    for( iRow = i_iRow; iRow < m_arpChilds.size(); iRow++ )
    {
        pModelEntry = m_arpChilds[iRow];

        if( pModelEntry != nullptr )
        {
            pModelEntry->setRowId(iRow);
        }
    }

} // removeChildEntry

//------------------------------------------------------------------------------
void CGraphObjModelEntry::setChildEntry( int i_iRow, CGraphObjModelEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    if( i_iRow < 0 || i_iRow >= m_arpChilds.size() )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange );
    }
    m_arpChilds[i_iRow] = i_pEntry;

} // setChildEntry

//------------------------------------------------------------------------------
CGraphObjModelEntry* CGraphObjModelEntry::getChildEntry( int i_iRow )
//------------------------------------------------------------------------------
{
    if( i_iRow < 0 || i_iRow >= m_arpChilds.size() )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange );
    }
    return m_arpChilds[i_iRow];

} // getChildEntry

//------------------------------------------------------------------------------
CGraphObjModelEntry* CGraphObjModelEntry::findChildEntry( const QString& i_strNodeName )
//------------------------------------------------------------------------------
{
    CGraphObjModelEntry* pModelEntry = nullptr;
    int                  iRow;

    for( iRow = 0; iRow < m_arpChilds.size(); iRow++ )
    {
        CGraphObjModelEntry* pModelEntryTmp = m_arpChilds[iRow];

        if( pModelEntryTmp != nullptr )
        {
            CGraphObj* pGraphObj = pModelEntryTmp->getGraphObj();

            if( pGraphObj != nullptr && pGraphObj->name() == i_strNodeName )
            {
                pModelEntry = pModelEntryTmp;
                break;
            }
        }
    }
    return pModelEntry;

} // findChildEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjModelEntry::setGraphObjName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strGraphObjName = i_strObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjModelEntry::setGraphObjId( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    m_strGraphObjId = i_strObjId;
}

//------------------------------------------------------------------------------
QGraphicsItem* CGraphObjModelEntry::getGraphicsItem()
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->findGraphicsItem(m_strGraphObjId);
}

//------------------------------------------------------------------------------
CGraphObj* CGraphObjModelEntry::getGraphObj()
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->findGraphObj(m_strGraphObjId);
}
