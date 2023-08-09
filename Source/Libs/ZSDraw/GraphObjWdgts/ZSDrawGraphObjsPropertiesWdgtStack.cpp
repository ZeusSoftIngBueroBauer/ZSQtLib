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
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjLinePropertiesWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ZSDrawingViewPropertiesWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

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
    m_hshGraphObjType2StackWdgtIndex()
{
    setObjectName(i_pDrawingView->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::GraphObjWdgts", ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

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
    m_pEdtPath->setReadOnly(true);
    m_pLytHeadLine->addWidget(m_pEdtPath, 1);

    // Content of selected tree node
    //------------------------------

    m_pStackedWdgtGraphObjsProperties = new QStackedWidget();
    m_pLytMain->addWidget(m_pStackedWdgtGraphObjsProperties, 1);

    EGraphObjType graphObjType = EGraphObjTypeUndefined;
    int idxWdgt = m_pStackedWdgtGraphObjsProperties->addWidget(
        new CWdgtGraphObjPropertiesAbstract(
            m_pDrawingView->drawingScene(),
            "Empty::CWdgtGraphObjPropertiesAbstract",
            "WdgtStackGraphObjProperties"));
    m_hshGraphObjType2StackWdgtIndex.insert(graphObjType2Str(graphObjType), idxWdgt);

    idxWdgt = m_pStackedWdgtGraphObjsProperties->addWidget(
        new CWdgtDrawingViewProperties(
            m_pDrawingView,
            "WdgtStackGraphObjProperties"));
    m_hshGraphObjType2StackWdgtIndex.insert("DrawingView", idxWdgt);

    graphObjType = EGraphObjTypeLine;
    idxWdgt = m_pStackedWdgtGraphObjsProperties->addWidget(
        new CWdgtGraphObjLineProperties(
            m_pDrawingView->drawingScene(),
            "WdgtStackGraphObjProperties"));
    m_hshGraphObjType2StackWdgtIndex.insert(graphObjType2Str(graphObjType), idxWdgt);

    m_pStackedWdgtGraphObjsProperties->setCurrentIndex(0);

} // ctor

//------------------------------------------------------------------------------
CWdgtStackGraphObjsProperties::~CWdgtStackGraphObjsProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "CWdgtGraphObjPropertiesAbstract::dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pDrawingView = nullptr;
    m_pIdxTree = nullptr;
    //m_strKeyInTree;
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pEdtPath = nullptr;
    m_pStackedWdgtGraphObjsProperties = nullptr;
    //m_hshGraphObjType2StackWdgtIndex.clear();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackGraphObjsProperties::setKeyInTree( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strKeyInTree;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setKeyInTree",
        /* strAddInfo   */ strMthInArgs );

    if (m_strKeyInTree != i_strKeyInTree) {
        m_strKeyInTree = i_strKeyInTree;
        if (m_pIdxTree != nullptr) {
            int idxStackWdgt = 0;
            QString strEntryPath;
            CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(i_strKeyInTree);
            if (pTreeEntry != nullptr) {
                QString strGraphObjType;
                strEntryPath = pTreeEntry->path();
                if (pTreeEntry->isRoot()) {
                    strGraphObjType = "DrawingView";
                }
                else {
                    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pTreeEntry);
                    if (pGraphObj != nullptr) {
                        strGraphObjType = pGraphObj->typeAsString();
                    }
                }
                idxStackWdgt = graphObjType2StackWdgtIndex(strGraphObjType);
            }
            m_pEdtPath->setText(strEntryPath);
            m_pStackedWdgtGraphObjsProperties->setCurrentIndex(idxStackWdgt);
            CWdgtGraphObjPropertiesAbstract* pWdgtProperties =
                dynamic_cast<CWdgtGraphObjPropertiesAbstract*>(
                    m_pStackedWdgtGraphObjsProperties->widget(idxStackWdgt));
            if (pWdgtProperties != nullptr) {
                pWdgtProperties->setKeyInTree(m_strKeyInTree);
            }
        }
    }
} // setKeyInTree

//------------------------------------------------------------------------------
QString CWdgtStackGraphObjsProperties::getKeyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
int CWdgtStackGraphObjsProperties::graphObjType2StackWdgtIndex(const QString& i_strGraphObjType) const
//------------------------------------------------------------------------------
{
    return m_hshGraphObjType2StackWdgtIndex.value(i_strGraphObjType, 0);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackGraphObjsProperties::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onIdxTreeAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pIdxTree = nullptr;
}
