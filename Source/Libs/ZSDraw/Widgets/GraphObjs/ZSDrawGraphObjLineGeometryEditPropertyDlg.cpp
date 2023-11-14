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

#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLineGeometryEditPropertyDlg.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLineGeometryPropertiesWdgt.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSPhysValGUI/ZSPhysValEditWdgt.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtCore/qcoreapplication.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CDlgGraphObjLineGeometryEditProperty : public QDialog
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CTrcAdminObj* CDlgGraphObjLineGeometryEditProperty::s_pTrcAdminObj = nullptr;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgGraphObjLineGeometryEditProperty::CDlgGraphObjLineGeometryEditProperty(
    CDrawingScene* i_pDrawingScene,
    QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_pGraphObj(nullptr),
    m_iContentChangedSignalBlockedCounter(0),
    m_eDimensionUnit(),
    m_strCoorPart1(),
    m_strCoorPart2(),
    m_pLyt(nullptr),
    m_pLytHeadLine(nullptr),
    m_pEdtPath(nullptr),
    m_pLytLineEdtVal(nullptr),
    m_pLblCoorPart1(nullptr),
    m_pLblCoorPart2(nullptr),
    m_pEdtCoor(nullptr),
    m_pLytButtons(nullptr),
    m_pBtnOk(nullptr)
{
    setWindowTitle(QCoreApplication::applicationName() + ": Enter Value");

    s_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::GraphObjs::StandardShapes", ClassName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "ctor",
        /* strInArgs    */ "" );

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    m_pLytHeadLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytHeadLine);

    m_pEdtPath = new QLineEdit();
    m_pEdtPath->setReadOnly(true);
    m_pLytHeadLine->addWidget(m_pEdtPath, 1);

    m_pLytLineEdtVal = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineEdtVal);

    m_pLblCoorPart1 = new QLabel();
    m_pLytLineEdtVal->addWidget(m_pLblCoorPart1);

    m_pLblCoorPart2 = new QLabel();
    m_pLytLineEdtVal->addWidget(m_pLblCoorPart2);

    m_pEdtCoor = new CWdgtEditPhysVal();
    m_pEdtCoor->setFixedWidth(120);
    m_pLytLineEdtVal->addWidget(m_pEdtCoor);
    QObject::connect(
        m_pEdtCoor, &CWdgtEditPhysVal::valueChanged,
        this, &CDlgGraphObjLineGeometryEditProperty::onEdtCoorValueChanged);

    m_pLytButtons = new QHBoxLayout();
    m_pLyt->addSpacing(10);
    m_pLyt->addLayout(m_pLytButtons);

    m_pBtnOk = new QPushButton("Ok");
    m_pLytButtons->addWidget(m_pBtnOk);
    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CDlgGraphObjLineGeometryEditProperty::accept);

    m_pLytButtons->addStretch();

    QObject::connect(
        m_pDrawingScene, &CDrawingScene::drawingSizeChanged,
        this, &CDlgGraphObjLineGeometryEditProperty::onDrawingSceneDrawingSizeChanged );

} // ctor

//------------------------------------------------------------------------------
CDlgGraphObjLineGeometryEditProperty::~CDlgGraphObjLineGeometryEditProperty()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "dtor",
        /* strInArgs    */ "" );

    CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObj);

    m_pDrawingScene = nullptr;
    m_pGraphObj = nullptr;
    m_iContentChangedSignalBlockedCounter = 0;
    m_eDimensionUnit = static_cast<EScaleDimensionUnit>(0);
    //m_strCoorPart1;
    //m_strCoorPart2;
    m_pLyt = nullptr;
    m_pLytHeadLine = nullptr;
    m_pEdtPath = nullptr;
    m_pLytLineEdtVal = nullptr;
    m_pLblCoorPart1 = nullptr;
    m_pLblCoorPart2 = nullptr;
    m_pEdtCoor = nullptr;
    m_pLytButtons = nullptr;
    m_pBtnOk = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the graphical object and defines the coordinate which has to be edited.

    @param [in] i_pGraphObj
        Graphical object to be edited.
    @param [in] i_strCoorSpec
        The coordinate specifier has to be set as follows:
        <DimensionUnit>.<CoordinatePart1>[.<CoordinatePart2>]
        The sections of the object name will be used by the edit dialog
        to create the labels and to decide, which property has to be
        edited. E.g. for "Pixels.Point 1.X" the x coordinate of Point 1
        in dimension unit pixels will be edited.
*/
void CDlgGraphObjLineGeometryEditProperty::setCoordinate(
    CGraphObjLine* i_pGraphObj, const QString& i_strCoorSpec)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(s_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "null" : i_pGraphObj->keyInTree())
            + ", " + i_strCoorSpec;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "setCoordinate",
        /* strInArgs    */ strMthInArgs );

    bool bIsValidCoordinate = false;

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    m_pGraphObj = i_pGraphObj;
    if (m_pGraphObj == nullptr) {
        setObjectName("");
        m_pEdtPath->setText("");
    }
    else {
        setObjectName(m_pGraphObj->keyInTree());
        m_pEdtPath->setText(m_pGraphObj->path());
    }

    QStringList strlstCoors = i_strCoorSpec.split(".");

    m_eDimensionUnit = CEnumScaleDimensionUnit();
    m_strCoorPart1 = "";
    m_strCoorPart2 = "";

    if (m_pGraphObj != nullptr && strlstCoors.size() >= 2 && strlstCoors.size() <= 3) {
        m_eDimensionUnit = CEnumScaleDimensionUnit::fromString(strlstCoors[0], &bIsValidCoordinate);
        m_strCoorPart1 = strlstCoors[1];
        if (strlstCoors.size() == 3) {
            m_strCoorPart2 = strlstCoors[2];
        }
        if (bIsValidCoordinate) {
            update();
        }
    }
    if (!bIsValidCoordinate) {
        m_pLblCoorPart1->setText("");
        m_pLblCoorPart2->setText("");
        m_pEdtCoor->setEnabled(false);
        m_pEdtCoor->setReadOnly(true);
        m_pEdtCoor->setValue(0.0);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgGraphObjLineGeometryEditProperty::onEdtCoorValueChanged(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(s_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onEdtCoorValueChanged",
        /* strInArgs    */ strMthInArgs );

    if (m_iContentChangedSignalBlockedCounter == 0) {
        const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
        CPhysValLine physValLine = m_pGraphObj->getLine(drawingSize.unit());
        if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
            if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorPoint1) {
                CPhysValPoint physValP1 = physValLine.p1();
                if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                    physValP1.setX(m_pEdtCoor->value());
                }
                else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                    physValP1.setY(m_pEdtCoor->value());
                }
                physValLine.setP1(physValP1);
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorPoint2) {
                CPhysValPoint physValP2 = physValLine.p2();
                if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                    physValP2.setX(m_pEdtCoor->value());
                }
                else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                    physValP2.setY(m_pEdtCoor->value());
                }
                physValLine.setP2(physValP2);
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorWidth) {
                physValLine.setWidth(m_pEdtCoor->value());
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorHeight) {
                physValLine.setHeight(m_pEdtCoor->value());
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorLength) {
                physValLine.setLength(m_pEdtCoor->value());
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorAngle) {
                physValLine.setAngle(m_pEdtCoor->value());
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorCenter) {
                CPhysValPoint physValCenter = physValLine.center();
                if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                    physValCenter.setX(m_pEdtCoor->value());
                }
                else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                    physValCenter.setY(m_pEdtCoor->value());
                }
                physValLine.setCenter(physValCenter);
            }
            m_pGraphObj->setLine(physValLine);
        }
        else /*if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) */ {
            if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorPoint1) {
                CPhysValPoint physValP1 = physValLine.p1();
                if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                    physValP1.setX(m_pEdtCoor->value());
                }
                else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                    physValP1.setY(m_pEdtCoor->value());
                }
                physValLine.setP1(physValP1);
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorPoint2) {
                CPhysValPoint physValP2 = physValLine.p2();
                if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                    physValP2.setX(m_pEdtCoor->value());
                }
                else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                    physValP2.setY(m_pEdtCoor->value());
                }
                physValLine.setP2(physValP2);
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorWidth) {
                CPhysVal physValWidth = m_pEdtCoor->value();
                physValLine.setWidth(physValWidth);
            }
            else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorHeight) {
                CPhysVal physValHeight = m_pEdtCoor->value();
                physValLine.setHeight(physValHeight);
            }
            m_pGraphObj->setLine(physValLine);
        }
    }
}

//------------------------------------------------------------------------------
void CDlgGraphObjLineGeometryEditProperty::onDrawingSceneDrawingSizeChanged(
    const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(s_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onDrawingSceneDrawingSizeChanged",
        /* strInArgs    */ strMthInArgs );

    update();
}

//------------------------------------------------------------------------------
void CDlgGraphObjLineGeometryEditProperty::onGraphObjAboutToBeDestroyed(CGraphObj*)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onGraphObjAboutToBeDestroyed",
        /* strInArgs    */ "" );

    setCoordinate(nullptr, "");
}

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

*/
void CDlgGraphObjLineGeometryEditProperty::update()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "update",
        /* strInArgs    */ "" );

    bool bIsValidCoordinate = false;

    CRefCountGuard refCountGuard(&m_iContentChangedSignalBlockedCounter);

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValLine physValLine = m_pGraphObj->getLine(drawingSize.unit());
    bool bEnabled = true;
    bool bReadOnly = false;
    CUnit unit = physValLine.unit();
    double fResolution = 1.0;
    double fMinimum = 0.0;
    double fMaximum = 0.0;
    double fValue = 0.0;

    if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
        if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorPoint1) {
            if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                bIsValidCoordinate = true;
                fResolution = drawingSize.metricImageWidth().getRes().getVal(unit);
                fMaximum = drawingSize.metricImageWidth().getVal(unit);
                fValue = physValLine.p1().x().getVal(unit);
            }
            else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                bIsValidCoordinate = true;
                fResolution = drawingSize.metricImageHeight().getRes().getVal(unit);
                fMaximum =  drawingSize.metricImageHeight().getVal(unit);
                fValue = physValLine.p1().y().getVal(unit);
            }
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorPoint2) {
            if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                bIsValidCoordinate = true;
                fResolution = drawingSize.metricImageWidth().getRes().getVal(unit);
                fMaximum = drawingSize.metricImageWidth().getVal(unit);
                fValue = physValLine.p2().x().getVal(unit);
            }
            else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                bIsValidCoordinate = true;
                fResolution = drawingSize.metricImageHeight().getRes().getVal(unit);
                fMaximum =  drawingSize.metricImageHeight().getVal(unit);
                fValue = physValLine.p2().y().getVal(unit);
            }
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorWidth) {
            bIsValidCoordinate = true;
            fResolution = drawingSize.metricImageWidth().getRes().getVal(unit);
            fMaximum = drawingSize.metricImageWidth().getVal(unit);
            fMinimum = -fMaximum;
            fValue = physValLine.width().getVal(unit);
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorHeight) {
            bIsValidCoordinate = true;
            fResolution = drawingSize.metricImageHeight().getRes().getVal(unit);
            fMaximum = drawingSize.metricImageHeight().getVal(unit);
            fMinimum = -fMaximum;
            fValue = physValLine.height().getVal(unit);
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorLength) {
            bIsValidCoordinate = true;
            fResolution = drawingSize.metricImageWidth().getRes().getVal(unit);
            fMaximum = ZS::System::Math::sqrt(
                ZS::System::Math::sqr(drawingSize.metricImageWidth().getVal(unit))
                + ZS::System::Math::sqr(drawingSize.metricImageHeight().getVal(unit)));
            fValue = physValLine.length().getVal(unit);
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorAngle) {
            bIsValidCoordinate = true;
            unit = Units.Angle.Degree;
            fResolution = 0.1;
            fMinimum = -360.0;
            fMaximum = 360.0;
            fValue = physValLine.angle().getVal(unit);
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorCenter) {
            if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                bIsValidCoordinate = true;
                fResolution = 0.1;
                fMaximum = drawingSize.metricImageWidth().getVal(unit);
                fValue = physValLine.center().x().getVal(unit);
            }
            else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                bIsValidCoordinate = true;
                fResolution = 0.1;
                fMaximum =  drawingSize.metricImageHeight().getVal(unit);
                fValue = physValLine.center().y().getVal(unit);
            }
        }
    }
    else /*if (m_eDimensionUnit == EScaleDimensionUnit::Pixels)*/ {
        unit = Units.Length.px;
        if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorPoint1) {
            if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                bIsValidCoordinate = true;
                fMaximum = drawingSize.imageWidthInPixels();
                fValue = physValLine.p1().x().getVal(unit);
            }
            else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                bIsValidCoordinate = true;
                fMaximum =  drawingSize.imageHeightInPixels();
                fValue = physValLine.p1().y().getVal(unit);
            }
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorPoint2) {
            if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorX) {
                bIsValidCoordinate = true;
                fMaximum = drawingSize.imageWidthInPixels();
                fValue = physValLine.p2().x().getVal(unit);
            }
            else if (m_strCoorPart2 == CWdgtGraphObjLineGeometryProperties::c_strCoorY) {
                bIsValidCoordinate = true;
                fMaximum =  drawingSize.imageHeightInPixels();
                fValue = physValLine.p2().y().getVal(unit);
            }
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorWidth) {
            bIsValidCoordinate = true;
            fMaximum = drawingSize.imageWidthInPixels();
            fMinimum = -fMaximum;
            fValue = physValLine.width().getVal(unit);
        }
        else if (m_strCoorPart1 == CWdgtGraphObjLineGeometryProperties::c_strCoorHeight) {
            bIsValidCoordinate = true;
            fMaximum = drawingSize.imageHeightInPixels();
            fMinimum = -fMaximum;
            fValue = physValLine.height().getVal(unit);
        }
    }

    m_pEdtCoor->setUnit(unit);
    m_pEdtCoor->setResolution(fResolution);
    m_pEdtCoor->setMinimum(fMinimum);
    m_pEdtCoor->setMaximum(fMaximum);
    m_pEdtCoor->setValue(fValue);

    if (bIsValidCoordinate) {
        if (m_strCoorPart2.isEmpty()) {
            m_pLblCoorPart1->setText(m_strCoorPart1 + ":");
            m_pLblCoorPart1->setFixedWidth(40);
            m_pLblCoorPart2->setText(m_strCoorPart2);
            m_pLblCoorPart2->setFixedWidth(0);
        }
        else {
            m_pLblCoorPart1->setText(m_strCoorPart1);
            m_pLblCoorPart1->setFixedWidth(40);
            m_pLblCoorPart2->setText(m_strCoorPart2 + ":");
            m_pLblCoorPart2->setFixedWidth(20);
        }
        m_pEdtCoor->setEnabled(true);
        m_pEdtCoor->setReadOnly(false);
    }
    else {
        m_pLblCoorPart1->setText("");
        m_pLblCoorPart2->setText("");
        m_pEdtCoor->setEnabled(false);
        m_pEdtCoor->setReadOnly(true);
    }
}
