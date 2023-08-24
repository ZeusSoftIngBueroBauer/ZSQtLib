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

#include "ZSDiagramGUI/ZSDiagramItemsPropertiesWdgtStack.h"
#include "ZSDiagramGUI/ZSDiagramPropertiesWdgt.h"
#include "ZSDiagramGUI/ZSDiagramObjAxisLabelPropertiesWdgt.h"
#include "ZSDiagramGUI/ZSDiagramScalePropertiesWdgt.h"
#include "ZSDiagramGUI/ZSDiagramTracePropertiesWdgt.h"
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
class CWdgtStackDiagramItemsProperties : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtStackDiagramItemsProperties::CWdgtStackDiagramItemsProperties(
    CWdgtDiagram* i_pDiagram,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pDiagram(i_pDiagram),
    m_strCurrentDiagItemClassName(),
    m_strCurrentDiagItemObjName(),
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
        this, &CWdgtStackDiagramItemsProperties::onDiagramAboutToBeDestroyed);

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

    createDiagItemPropertiesWdgts();

} // ctor

//------------------------------------------------------------------------------
CWdgtStackDiagramItemsProperties::~CWdgtStackDiagramItemsProperties()
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
    //m_strCurrentDiagItemClassName;
    //m_strCurrentDiagItemObjName;
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
void CWdgtStackDiagramItemsProperties::setCurrentDiagObj(
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

    if( m_strCurrentDiagItemClassName != i_strClassName || m_strCurrentDiagItemObjName != i_strObjName )
    {
        m_strCurrentDiagItemClassName = i_strClassName;
        m_strCurrentDiagItemObjName = i_strObjName;

        QString strPath = m_strCurrentDiagItemClassName;
        if (!m_strCurrentDiagItemObjName.isEmpty()) {
            strPath += "::" + m_strCurrentDiagItemObjName;
        }
        m_pEdtPath->setText(strPath);

        int idxWdgt = -1;

        if( m_hshpWdgtsObjProperties.contains(m_strCurrentDiagItemClassName) )
        {
            m_hshpWdgtsObjProperties[m_strCurrentDiagItemClassName]->setDiagItemObjName(m_strCurrentDiagItemObjName);
        }
        if( m_hshIdxWdgtsObjProperties.contains(m_strCurrentDiagItemClassName) )
        {
            idxWdgt = m_hshIdxWdgtsObjProperties[m_strCurrentDiagItemClassName];
            m_pStackedWdgtObjsProperties->setCurrentIndex(idxWdgt);
        }
    }
}

//------------------------------------------------------------------------------
QString CWdgtStackDiagramItemsProperties::currentDiagItemClassName() const
//------------------------------------------------------------------------------
{
    return m_strCurrentDiagItemClassName;
}

//------------------------------------------------------------------------------
QString CWdgtStackDiagramItemsProperties::currentDiagItemObjName() const
//------------------------------------------------------------------------------
{
    return m_strCurrentDiagItemObjName;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackDiagramItemsProperties::onDiagramAboutToBeDestroyed()
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

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtStackDiagramItemsProperties::createDiagItemPropertiesWdgts()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "createDiagItemPropertiesWdgts",
        /* strMethodInArgs    */ strMthInArgs );

    CWdgtDiagramItemPropertiesBase* pWdgt;

    pWdgt = new CWdgtDiagramItemPropertiesBase("CWdgtDiagScales", m_pDiagram, "Scales");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramItemPropertiesBase("CWdgtDiagTraces", m_pDiagram, "Traces");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramItemPropertiesBase("CWdgtDiagObjs", m_pDiagram, "DiagObjs");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramProperties(m_pDiagram);
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramScaleProperties(m_pDiagram);
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramTraceProperties(m_pDiagram);
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjAxisLabelProperties(m_pDiagram);
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    #pragma message(__TODO__"Implement concrete DiagObj Properties Widget classes")

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjCurveProperties", m_pDiagram, "CDiagObjCurve");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjErrInfoProperties", m_pDiagram, "CDiagObjErrInfo");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjGridProperties", m_pDiagram, "CDiagObjGrid");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjHistogramProperties", m_pDiagram, "CDiagObjHistogram");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjLabelProperties", m_pDiagram, "CDiagObjLabel");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjLineProperties", m_pDiagram, "CDiagObjLine");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjMarkerProperties", m_pDiagram, "CDiagObjMarker");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjProgressBarProperties", m_pDiagram, "CDiagObjProgressBar");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjRectProperties", m_pDiagram, "CDiagObjRect");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjTableProperties", m_pDiagram, "CDiagObjTable");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);

    pWdgt = new CWdgtDiagramObjPropertiesBase("CWdgtDiagramObjValueProviderProperties", m_pDiagram, "CDiagObjValueProvider");
    m_hshpWdgtsObjProperties[pWdgt->diagItemClassName()] = pWdgt;
    m_hshIdxWdgtsObjProperties[pWdgt->diagItemClassName()] = m_pStackedWdgtObjsProperties->addWidget(pWdgt);
}
