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

#include "ZSDiagramGUI/ZSDiagramObjsPropertiesWdgtStack.h"
#include "ZSDiagramGUI/ZSDiagramObjPropertiesAbstractWdgt.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
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
using namespace ZS::Diagram;
using namespace ZS::Diagram::GUI;


/*******************************************************************************
class CWdgtStackDiagramObjsProperties : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtStackDiagramObjsProperties::CWdgtStackDiagramObjsProperties(
    CWdgtDiagram* i_pDiagram,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pDiagram(i_pDiagram),
    m_strCurrentDiagObjClassName(),
    m_strCurrentDiagObjName(),
    m_pLyt(nullptr),
    m_pLytHeadLine(nullptr),
    m_pEdtPath(nullptr),
    m_pStackedWdgtObjsProperties(nullptr),
    m_hshpWdgtsObjProperties(),
    m_hshIdxWdgtsObjProperties(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_pDiagram->objectName());

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pDiagram->objectName();
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    QObject::connect(
        m_pDiagram, &CWdgtDiagram::aboutToBeDestroyed,
        this, &CWdgtStackDiagramObjsProperties::onDiagramAboutToBeDestroyed);

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // Headline
    //---------

    m_pLytHeadLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytHeadLine);

    m_pEdtPath = new QLineEdit();
    m_pEdtPath->setReadOnly(true);
    m_pLytHeadLine->addWidget(m_pEdtPath, 1);

    // Content of selected tree node
    //------------------------------

    m_pStackedWdgtObjsProperties = new QStackedWidget();
    m_pLyt->addWidget(m_pStackedWdgtObjsProperties, 1);

    m_hshpWdgtsObjProperties[m_pDiagram->className()] =
        new CWdgtDiagramObjPropertiesAbstract(m_pDiagram, m_pDiagram->className());
    m_hshpWdgtsObjProperties["Scales"] =
        new CWdgtDiagramObjPropertiesAbstract(m_pDiagram, "Scales");
    m_hshpWdgtsObjProperties["Traces"] =
        new CWdgtDiagramObjPropertiesAbstract(m_pDiagram, "Traces");
    m_hshpWdgtsObjProperties["DiagObjs"] =
        new CWdgtDiagramObjPropertiesAbstract(m_pDiagram, "DiagObjs");

    for (CWdgtDiagramObjPropertiesAbstract* pWdgt : m_hshpWdgtsObjProperties)
    {
        m_hshIdxWdgtsObjProperties[pWdgt->diagObjClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);
    }
    m_pStackedWdgtObjsProperties->setCurrentIndex(m_hshIdxWdgtsObjProperties[m_pDiagram->className()]);

} // ctor

//------------------------------------------------------------------------------
CWdgtStackDiagramObjsProperties::~CWdgtStackDiagramObjsProperties()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDiagram = nullptr;
    //m_strCurrentDiagObjClassName;
    //m_strCurrentDiagObjName;
    m_pLyt = nullptr;
    m_pLytHeadLine = nullptr;
    m_pEdtPath = nullptr;
    m_pStackedWdgtObjsProperties = nullptr;
    m_hshpWdgtsObjProperties.clear();
    m_hshIdxWdgtsObjProperties.clear();
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackDiagramObjsProperties::setCurrentDiagObj(
    const QString& i_strClassName, const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strClassName + "::" + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setCurrentDiagObj",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_strCurrentDiagObjClassName != i_strClassName || m_strCurrentDiagObjName != i_strObjName )
    {
        m_strCurrentDiagObjClassName = i_strClassName;
        m_strCurrentDiagObjName = i_strObjName;

        QString strPath = m_strCurrentDiagObjClassName;
        if (!m_strCurrentDiagObjName.isEmpty()) {
            strPath += "::" + m_strCurrentDiagObjName;
        }
        m_pEdtPath->setText(strPath);

        int idxWdgt = -1;

        if( m_hshpWdgtsObjProperties.contains(m_strCurrentDiagObjClassName) )
        {
            m_hshpWdgtsObjProperties[m_strCurrentDiagObjClassName]->setDiagObjName(m_strCurrentDiagObjName);
        }
        if( m_hshIdxWdgtsObjProperties.contains(m_strCurrentDiagObjClassName) )
        {
            idxWdgt = m_hshIdxWdgtsObjProperties[m_strCurrentDiagObjClassName];
        }

        m_pStackedWdgtObjsProperties->setCurrentIndex(idxWdgt);
    }
}

//------------------------------------------------------------------------------
QString CWdgtStackDiagramObjsProperties::currentDiagObjClassName() const
//------------------------------------------------------------------------------
{
    return m_strCurrentDiagObjClassName;
}

//------------------------------------------------------------------------------
QString CWdgtStackDiagramObjsProperties::currentDiagObjName() const
//------------------------------------------------------------------------------
{
    return m_strCurrentDiagObjName;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackDiagramObjsProperties::onDiagramAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onDiagramAboutToBeDestroyed",
        /* strMethodInArgs    */ strMthInArgs );

    m_pDiagram = nullptr;
}
