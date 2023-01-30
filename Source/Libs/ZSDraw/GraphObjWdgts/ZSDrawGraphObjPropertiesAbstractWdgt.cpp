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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjPropertiesAbstractWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjPropertiesAbstract : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesAbstract::CWdgtGraphObjPropertiesAbstract(
    CDrawingScene* i_pDrawingScene,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_pIdxTree(i_pDrawingScene->getGraphObjsIdxTree()),
    m_strKeyInTree(),
    m_cxLblWidthClm1(80),
    m_cxEdtWidthClm1(160),
    m_cxLblWidthClm2(80),
    m_cxEdtWidthClm2(160),
    m_cxClmSpacing(30)
{
    setObjectName(m_pIdxTree->objectName());

    QObject::connect(
        m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
        this, &CWdgtGraphObjPropertiesAbstract::onIdxTreeAboutToBeDestroyed);

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjPropertiesAbstract::~CWdgtGraphObjPropertiesAbstract()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    m_pIdxTree = nullptr;
    //m_strKeyInTree;
    m_cxLblWidthClm1 = 0;
    m_cxEdtWidthClm1 = 0;
    m_cxLblWidthClm2 = 0;
    m_cxEdtWidthClm2 = 0;
    m_cxClmSpacing = 0;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::saveState(QSettings& i_settings) const
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::restoreState(const QSettings& i_settings)
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::setKeyInTree( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTree != i_strKeyInTree )
    {
        m_strKeyInTree = i_strKeyInTree;
    }
}

//------------------------------------------------------------------------------
QString CWdgtGraphObjPropertiesAbstract::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::resizeToContents()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjPropertiesAbstract::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
