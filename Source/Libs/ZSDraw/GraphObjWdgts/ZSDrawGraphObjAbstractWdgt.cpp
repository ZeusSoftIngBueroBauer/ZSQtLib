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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjAbstractWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtAbstractGraphObj : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtAbstractGraphObj::CWdgtAbstractGraphObj(
    CDrawingScene* i_pDrawingScene,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_pIdxTree(i_pDrawingScene->getGraphObjsIdxTree()),
    m_strKeyInTree(),
    m_cxLblWidth(140),
    m_cxEdtWidth(200)
{
    setObjectName(m_pIdxTree->objectName());

    QObject::connect(
        m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
        this, &CWdgtAbstractGraphObj::onIdxTreeAboutToBeDestroyed);

} // ctor

//------------------------------------------------------------------------------
CWdgtAbstractGraphObj::~CWdgtAbstractGraphObj()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    m_pIdxTree = nullptr;
    //m_strKeyInTree;
    m_cxLblWidth = 0;
    m_cxEdtWidth = 0;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtAbstractGraphObj::setKeyInTree( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTree != i_strKeyInTree )
    {
        m_strKeyInTree = i_strKeyInTree;
    }
}

//------------------------------------------------------------------------------
QString CWdgtAbstractGraphObj::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtAbstractGraphObj::resizeToContents()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtAbstractGraphObj::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
