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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsWdgt.h"
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjAbstractWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qstackedwidget.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qstackedwidget.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtGraphObjs : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtGraphObjs::CWdgtGraphObjs(
    CDrawingScene* i_pDrawingScene,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pDrawingScene(i_pDrawingScene),
    m_pIdxTree(i_pDrawingScene->getGraphObjsIdxTree()),
    m_strKeyInTree(),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pEdtPath(nullptr),
    m_pStackedWdgtGraphObjs(nullptr),
    m_arpWdgtsGraphObj(EGraphObjTypeCount, nullptr)
{
    setObjectName(m_pIdxTree->objectName());

    QObject::connect(
        m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
        this, &CWdgtGraphObjs::onIdxTreeAboutToBeDestroyed);

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    // Headline
    //---------

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    m_pEdtPath = new QLineEdit();
    m_pLytHeadLine->addWidget(m_pEdtPath, 1);

    // Content of selected tree node
    //------------------------------

    m_pStackedWdgtGraphObjs = new QStackedWidget();
    m_pLytMain->addWidget(m_pStackedWdgtGraphObjs, 1);

    m_arpWdgtsGraphObj[EGraphObjTypeUndefined] =
        new CWdgtAbstractGraphObj(m_pDrawingScene);

    for( int idxGraphObjType = 0; idxGraphObjType < EGraphObjTypeCount; idxGraphObjType++ )
    {
        if( m_arpWdgtsGraphObj[idxGraphObjType] != nullptr )
        {
            m_pStackedWdgtGraphObjs->addWidget(m_arpWdgtsGraphObj[idxGraphObjType]);
        }
    }
    m_pStackedWdgtGraphObjs->setCurrentIndex(EGraphObjTypeUndefined);

} // ctor

//------------------------------------------------------------------------------
CWdgtGraphObjs::~CWdgtGraphObjs()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
    //m_strKeyInTree;
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pEdtPath = nullptr;
    m_pStackedWdgtGraphObjs = nullptr;
    //m_arpWdgtsGraphObj.clear();

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjs::setKeyInTree( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_strKeyInTree != i_strKeyInTree )
    {
        m_strKeyInTree = i_strKeyInTree;

        EGraphObjType graphObjType = EGraphObjTypeUndefined;
        QString       strEntryPath;

        if( m_pIdxTree != nullptr )
        {
            CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(i_strKeyInTree);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->isRoot() || pTreeEntry->isBranch() )
                {
                }
                else if( pTreeEntry->isLeave() )
                {
                }
                strEntryPath += pTreeEntry->path();

            } // if( pTreeEntry != nullptr )
        } // if( m_pIdxTree != nullptr )

        m_pEdtPath->setText(strEntryPath);

        for( int idxGraphObjType = 0; idxGraphObjType < EGraphObjTypeCount; idxGraphObjType++ )
        {
            if( m_arpWdgtsGraphObj[idxGraphObjType] != nullptr )
            {
                m_arpWdgtsGraphObj[idxGraphObjType]->setKeyInTree(m_strKeyInTree);
            }
        }

        m_pStackedWdgtGraphObjs->setCurrentIndex(graphObjType);

    } // if( m_strKeyInTree != i_strKeyInTree )
} // setKeyInTreeOfRootEntry

//------------------------------------------------------------------------------
QString CWdgtGraphObjs::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtGraphObjs::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
