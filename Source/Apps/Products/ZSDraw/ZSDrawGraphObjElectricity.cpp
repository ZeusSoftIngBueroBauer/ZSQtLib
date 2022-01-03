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

#include "ZSDrawGraphObjElectricity.h"

#include "ZSDraw/ZSDrawingScene.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;


/*******************************************************************************
class CGraphObjElectricity : public CGraphObjGroup
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

const QString CGraphObjElectricity::c_strFactoryGroupName = "Electricity";

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjElectricity::CGraphObjElectricity(
    CDrawingScene*       i_pDrawingScene,
    const QString&       i_strType,
    const QString&       i_strObjName,
    const CDrawSettings& i_drawSettings ) :
//------------------------------------------------------------------------------
    CGraphObjGroup(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ c_strFactoryGroupName,
        /* type                */ EGraphObjTypeUserDefined,
        /* strType             */ i_strType,
        /* strObjName          */ i_strObjName,
        /* drawSettings        */ i_drawSettings )
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjElectricity::~CGraphObjElectricity()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

} // dtor

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjElectricity::setObjId( const QString& i_strObjId )
////------------------------------------------------------------------------------
//{
//    if( m_strKeyInTree != i_strObjId )
//    {
//        CGraphObj::setKeyInTree(i_strObjId);
//
//        QList<QGraphicsItem*> arpGraphicsItemsChilds = childItems();
//        QGraphicsItem*        pGraphicsItemChild;
//        CGraphObj*            pGraphObjChild;
//        int                   idxGraphObjChild;
//
//        for( idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++ )
//        {
//            pGraphicsItemChild = arpGraphicsItemsChilds[idxGraphObjChild];
//            pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
//
//            if( pGraphicsItemChild != nullptr && pGraphObjChild != nullptr )
//            {
//                pGraphObjChild->setKeyInTree( m_strKeyInTree + m_pDrawingScene->getGraphObjNameNodeSeparator() + pGraphObjChild->getObjName() );
//            }
//        }
//    }
//
//} // setObjId

