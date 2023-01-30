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

#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsPropertiesWdgtStack.h"
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjPropertiesAbstractWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ZSDrawingViewPropertiesWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"

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
class CWdgtStackGraphObjsProperties : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtStackGraphObjsProperties::CWdgtStackGraphObjsProperties(
    CDrawingView* i_pDrawingView,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pDrawingView(i_pDrawingView),
    m_pIdxTree(i_pDrawingView->drawingScene()->getGraphObjsIdxTree()),
    m_strKeyInTree(),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pEdtPath(nullptr),
    m_pStackedWdgtGraphObjsProperties(nullptr),
    m_arpWdgtsGraphObjProperties(EGraphObjTypeCount, nullptr)
{
    setObjectName(m_pIdxTree->objectName());

    QObject::connect(
        m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
        this, &CWdgtStackGraphObjsProperties::onIdxTreeAboutToBeDestroyed);

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

    m_pStackedWdgtGraphObjsProperties = new QStackedWidget();
    m_pLytMain->addWidget(m_pStackedWdgtGraphObjsProperties, 1);

    m_arpWdgtsGraphObjProperties[EGraphObjTypeUndefined] =
        new CWdgtGraphObjPropertiesAbstract(m_pDrawingView->drawingScene());
    m_arpWdgtsGraphObjProperties[EGraphObjTypeDrawing] =
        new CWdgtDrawingViewProperties(m_pDrawingView);

    for( int idxGraphObjType = 0; idxGraphObjType < EGraphObjTypeCount; idxGraphObjType++ )
    {
        if( m_arpWdgtsGraphObjProperties[idxGraphObjType] != nullptr )
        {
            m_pStackedWdgtGraphObjsProperties->addWidget(m_arpWdgtsGraphObjProperties[idxGraphObjType]);
        }
    }
    m_pStackedWdgtGraphObjsProperties->setCurrentIndex(EGraphObjTypeUndefined);

} // ctor

//------------------------------------------------------------------------------
CWdgtStackGraphObjsProperties::~CWdgtStackGraphObjsProperties()
//------------------------------------------------------------------------------
{
    m_pDrawingView = nullptr;
    m_pIdxTree = nullptr;
    //m_strKeyInTree;
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pEdtPath = nullptr;
    m_pStackedWdgtGraphObjsProperties = nullptr;
    //m_arpWdgtsGraphObjProperties.clear();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackGraphObjsProperties::saveState(QSettings& i_settings) const
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CWdgtStackGraphObjsProperties::restoreState(const QSettings& i_settings)
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackGraphObjsProperties::setKeyInTree( const QString& i_strKeyInTree )
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
                if( pTreeEntry->isRoot() )
                {
                    graphObjType = EGraphObjTypeDrawing;
                }
                else if( pTreeEntry->isBranch() )
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
            if( m_arpWdgtsGraphObjProperties[idxGraphObjType] != nullptr )
            {
                m_arpWdgtsGraphObjProperties[idxGraphObjType]->setKeyInTree(m_strKeyInTree);
            }
        }

        m_pStackedWdgtGraphObjsProperties->setCurrentIndex(graphObjType);

    } // if( m_strKeyInTree != i_strKeyInTree )
} // setKeyInTree

//------------------------------------------------------------------------------
QString CWdgtStackGraphObjsProperties::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackGraphObjsProperties::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    m_pIdxTree = nullptr;
}
