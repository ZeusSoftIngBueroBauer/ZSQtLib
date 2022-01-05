/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qfileinfo.h>
#include <QtGui/qevent.h>

#include "Test.h"
#include "MainWindow.h"

#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSDraw/ZSDrawGraphObjLine.h"
#include "ZSDraw/ZSDrawGraphObjText.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Trace;


/*==============================================================================
protected slots: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::createTestGroupAndroidWallpaperTemplate( int& io_idxGroup )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthOutArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "IdxGroup:" + QString::number(io_idxGroup);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createTestGroupAndroidWallpaperTemplate",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrpGridLines = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++io_idxGroup) + " Grid Lines",
        /* pTSGrpParent    */ nullptr );

    int idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Page Setup",
        /* strOperation    */ "Width: 640 px, Height: 480 px", // For gx10: 2048 px, 1280 px
        /* pTSGrpParent    */ pGrpGridLines,
        /* szDoTestStepFct */ SLOT(doTestStepAndroidWallpaperTemplatePageSetup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Draw Grid Lines",
        /* strOperation    */ "xOffs: 10 px, yOffs: 10 px",
        /* pTSGrpParent    */ pGrpGridLines,
        /* szDoTestStepFct */ SLOT(doTestStepAndroidWallpaperTemplateDrawGridLines(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Save as wallpaper_template.png",
        /* strOperation    */ "saveAs(wallpaper_temmplate.png)",
        /* pTSGrpParent    */ pGrpGridLines,
        /* szDoTestStepFct */ SLOT(doTestStepAndroidWallpaperTemplateSaveAsWallpaperPng(ZS::Test::CTestStep*)) );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
    }

} // createTestGroupAndroidWallpaperTemplate

//------------------------------------------------------------------------------
void CTest::doTestStepAndroidWallpaperTemplatePageSetup( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->path();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepAndroidWallpaperTemplatePageSetup",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues = { "Width: ? px", "Height: ? px" };

    /* strOperation might be:

       "Width: 1234.5 px, Height: 12345.6 px",
    */

    QString strOperation = i_pTestStep->getOperation();

    QSize sizeDrawing;

    if( strOperation.contains("Width:") && strOperation.contains("Height:") )
    {
        CPhysVal physValWidth(Geometry::GraphDevice());
        CPhysVal physValHeight(Geometry::GraphDevice());

        QString strCoors = strOperation;

        QStringList strlstCoors = strCoors.split(",");

        if( strlstCoors.size() == 2 )
        {
            for( auto& strCoor : strlstCoors )
            {
                strCoor = strCoor.trimmed();

                if( strCoor.startsWith("Width:") )
                {
                    strCoor = strCoor.remove("Width:");
                    physValWidth = strCoor;
                }
                else if( strCoor.startsWith("Height:") )
                {
                    strCoor = strCoor.remove("Height:");
                    physValHeight = strCoor;
                }
            }

            try
            {
                int cxWidth = physValWidth.getVal(Geometry::GraphDevice()->Pixel());
                int cyHeight = physValHeight.getVal(Geometry::GraphDevice()->Pixel());

                strlstExpectedValues[0] = "Width: " + QString::number(cxWidth) + " px";
                strlstExpectedValues[1] = "Height: " + QString::number(cyHeight) + " px";

                sizeDrawing.setWidth(cxWidth);
                sizeDrawing.setHeight(cyHeight);
            }
            catch( CPhysValException& )
            {
            }
        }
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pDrawingScene->clear();

    m_pDrawingView->setDrawingSize(sizeDrawing);

    // Result Values
    //--------------

    QStringList strlstResultValues;

    sizeDrawing = m_pDrawingView->getDrawingSizeInPixels();

    strlstResultValues.append(QString("Width: " + QString::number(sizeDrawing.width()) + " px"));
    strlstResultValues.append(QString("Height: " + QString::number(sizeDrawing.height()) + " px"));

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepAndroidWallpaperTemplatePageSetup

//------------------------------------------------------------------------------
void CTest::doTestStepAndroidWallpaperTemplateDrawGridLines( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->path();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepAndroidWallpaperTemplateDrawGridLines",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //---------------

    QStringList strlstExpectedValues;
    QString     strExpectedValues;

    /* strOperation might be:

        "xOffs: 10 px, yOffs: 10 px",
    */

    QString strOperation = i_pTestStep->getOperation();

    int xOffs_px = 0;
    int yOffs_px = 0;

    if( strOperation.contains("xOffs:") && strOperation.contains("yOffs:") )
    {
        CPhysVal physValXOffs(Geometry::GraphDevice());
        CPhysVal physValYOffs(Geometry::GraphDevice());

        QString strCoors = strOperation;

        QStringList strlstCoors = strCoors.split(",");

        if( strlstCoors.size() == 2 )
        {
            for( auto& strCoor : strlstCoors )
            {
                strCoor = strCoor.trimmed();

                if( strCoor.startsWith("xOffs:") )
                {
                    strCoor = strCoor.remove("xOffs:");
                    physValXOffs = strCoor;
                }
                else if( strCoor.startsWith("yOffs:") )
                {
                    strCoor = strCoor.remove("yOffs:");
                    physValYOffs = strCoor;
                }
            }

            try
            {
                xOffs_px = physValXOffs.getVal(Geometry::GraphDevice()->Pixel());
                yOffs_px = physValYOffs.getVal(Geometry::GraphDevice()->Pixel());
            }
            catch( CPhysValException& )
            {
            }
        }
    }

    QSize sizeDrawing = m_pDrawingView->getDrawingSizeInPixels();

    // Horizontal Grid Lines
    //----------------------

    int x1_px = 0;
    int y1_px = 0;
    int x2_px = sizeDrawing.width();
    int y2_px = 0;

    if( yOffs_px > 0 )
    {
        while( y1_px <= sizeDrawing.height() )
        {
            if( !strExpectedValues.isEmpty() ) strExpectedValues += ", ";
            QLine line(x1_px, y1_px, x2_px, y2_px);
            strExpectedValues += QString::number(y1_px);
            y1_px += yOffs_px;
            y2_px = y1_px;
        }
    }

    strlstExpectedValues += strExpectedValues;
    strExpectedValues.clear();

    // Vertical Grid Lines
    //----------------------

    x1_px = 0;
    y1_px = 0;
    x2_px = 0;
    y2_px = sizeDrawing.height();

    if( xOffs_px > 0 )
    {
        while( x1_px <= sizeDrawing.width() )
        {
            if( !strExpectedValues.isEmpty() ) strExpectedValues += ", ";
            QLine line(x1_px, y1_px, x2_px, y2_px);
            strExpectedValues += QString::number(x1_px);
            x1_px += xOffs_px;
            x2_px = x1_px;
        }
    }

    strlstExpectedValues += strExpectedValues;
    strExpectedValues.clear();

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QVector<CGraphObj*> arpGraphObjs;

    CDrawSettings drawSettingsLine(EGraphObjTypeLine);

    // Horizontal Grid Lines
    //----------------------

    int idxLine = 0;

    x1_px = 0;
    y1_px = 0;
    x2_px = sizeDrawing.width();
    y2_px = 0;

    if( yOffs_px > 0 )
    {
        while( y1_px <= sizeDrawing.height() )
        {
            if( idxLine % 10 == 0 )
            {
                drawSettingsLine.setPenColor(Qt::black);
            }
            else if( idxLine % 5 == 0 )
            {
                drawSettingsLine.setPenColor(Qt::darkGray);
            }
            else
            {
                drawSettingsLine.setPenColor(Qt::gray);
            }

            CGraphObjLine* pGraphObjLine = new CGraphObjLine(
                /* pDrawingScene */ m_pDrawingScene,
                /* drawSettings  */ drawSettingsLine,
                /* strObjName    */ "HL" + QString::number(y1_px));
            arpGraphObjs.append(pGraphObjLine);

            pGraphObjLine->setLine( QLine(x1_px, y1_px, x2_px, y2_px) );
            m_pDrawingScene->addGraphObj(pGraphObjLine);
            m_pDrawingScene->onGraphObjCreationFinished(pGraphObjLine);
            pGraphObjLine->acceptCurrentAsOriginalCoors();

            y1_px += yOffs_px;
            y2_px = y1_px;

            ++idxLine;

            //if( idxLine > 70) break;
        }
    }

    // Vertical Grid Lines
    //--------------------

    idxLine = 0;

    x1_px = 0;
    y1_px = 0;
    x2_px = 0;
    y2_px = sizeDrawing.height();

    if( xOffs_px > 0 )
    {
        while( x1_px <= sizeDrawing.width() )
        {
            if( idxLine % 10 == 0 )
            {
                drawSettingsLine.setPenColor(Qt::black);
            }
            else if( idxLine % 5 == 0 )
            {
                drawSettingsLine.setPenColor(Qt::darkGray);
            }
            else
            {
                drawSettingsLine.setPenColor(Qt::gray);
            }

            CGraphObjLine* pGraphObjLine = new CGraphObjLine(
                /* pDrawingScene */ m_pDrawingScene,
                /* drawSettings  */ drawSettingsLine,
                /* strObjName    */ "VL" + QString::number(x1_px));
            arpGraphObjs.append(pGraphObjLine);

            pGraphObjLine->setLine( QLine(x1_px, y1_px, x2_px, y2_px) );

            m_pDrawingScene->addGraphObj(pGraphObjLine);
            m_pDrawingScene->onGraphObjCreationFinished(pGraphObjLine);
            pGraphObjLine->acceptCurrentAsOriginalCoors();

            x1_px += xOffs_px;
            x2_px = x1_px;

            ++idxLine;

            //if( idxLine > 50) break;
        }
    }

    // Label Horizontal Grid Lines
    //----------------------------

    CDrawSettings drawSettingsText(EGraphObjTypeText);

    drawSettingsText.setLineStyle(ELineStyle::NoLine);
    drawSettingsText.setTextSize(ETextSize20);

    idxLine = 0;

    x1_px = 0;
    y1_px = 0;
    x2_px = sizeDrawing.width();
    y2_px = 0;

    if( yOffs_px > 0 )
    {
        while( y1_px <= sizeDrawing.height() )
        {
            if( idxLine % 5 == 0 )
            {
                CGraphObjText* pGraphObjText = new CGraphObjText(
                    /* pDrawingScene */ m_pDrawingScene,
                    /* drawSettings  */ drawSettingsText,
                    /* strObjName    */ "HLblL" + QString::number(y1_px));

                pGraphObjText->setPos( QPoint(x1_px, y1_px) );
                pGraphObjText->setPlainText(QString::number(y1_px));
                m_pDrawingScene->addGraphObj(pGraphObjText);
                m_pDrawingScene->onGraphObjCreationFinished(pGraphObjText);
                pGraphObjText->acceptCurrentAsOriginalCoors();

                QRectF boundingRect = pGraphObjText->boundingRect();
                QPointF ptCenter = boundingRect.center();
                pGraphObjText->moveBy(0.0, -ptCenter.y());

                pGraphObjText = new CGraphObjText(
                    /* pDrawingScene */ m_pDrawingScene,
                    /* drawSettings  */ drawSettingsText,
                    /* strObjName    */ "HLblC" + QString::number(y1_px));

                pGraphObjText->setPos( QPoint(sizeDrawing.width()/2, y1_px) );
                pGraphObjText->setPlainText(QString::number(y1_px));
                m_pDrawingScene->addGraphObj(pGraphObjText);
                m_pDrawingScene->onGraphObjCreationFinished(pGraphObjText);
                pGraphObjText->acceptCurrentAsOriginalCoors();

                boundingRect = pGraphObjText->boundingRect();
                ptCenter = boundingRect.center();
                pGraphObjText->moveBy(0.0, -ptCenter.y());

                pGraphObjText = new CGraphObjText(
                    /* pDrawingScene */ m_pDrawingScene,
                    /* drawSettings  */ drawSettingsText,
                    /* strObjName    */ "HLblR" + QString::number(y1_px));

                pGraphObjText->setPos( QPoint(x2_px, y1_px) );
                pGraphObjText->setPlainText(QString::number(y1_px));
                m_pDrawingScene->addGraphObj(pGraphObjText);
                m_pDrawingScene->onGraphObjCreationFinished(pGraphObjText);
                pGraphObjText->acceptCurrentAsOriginalCoors();

                boundingRect = pGraphObjText->boundingRect();
                ptCenter = boundingRect.center();
                pGraphObjText->moveBy(-boundingRect.width(), -ptCenter.y());
            }

            y1_px += yOffs_px;
            y2_px = y1_px;

            ++idxLine;
        }
    }

    // Label Vertical Grid Lines
    //----------------------------

    idxLine = 0;

    x1_px = 0;
    y1_px = 0;
    x2_px = 0;
    y2_px = sizeDrawing.height();

    if( xOffs_px > 0 )
    {
        while( x1_px <= sizeDrawing.width() )
        {
            if( idxLine % 5 == 0 )
            {
                CGraphObjText* pGraphObjText = new CGraphObjText(
                    /* pDrawingScene */ m_pDrawingScene,
                    /* drawSettings  */ drawSettingsText,
                    /* strObjName    */ "VLblT" + QString::number(x1_px));

                pGraphObjText->setPos( QPoint(x1_px, y1_px) );
                pGraphObjText->setPlainText(QString::number(x1_px));
                m_pDrawingScene->addGraphObj(pGraphObjText);
                m_pDrawingScene->onGraphObjCreationFinished(pGraphObjText);
                pGraphObjText->acceptCurrentAsOriginalCoors();

                QRectF boundingRect = pGraphObjText->boundingRect();
                QPointF ptCenter = boundingRect.center();
                pGraphObjText->moveBy(-ptCenter.x(), 0.0);

                pGraphObjText = new CGraphObjText(
                    /* pDrawingScene */ m_pDrawingScene,
                    /* drawSettings  */ drawSettingsText,
                    /* strObjName    */ "VLblC" + QString::number(x1_px));

                pGraphObjText->setPos( QPoint(x1_px, sizeDrawing.height()/2) );
                pGraphObjText->setPlainText(QString::number(x1_px));
                m_pDrawingScene->addGraphObj(pGraphObjText);
                m_pDrawingScene->onGraphObjCreationFinished(pGraphObjText);
                pGraphObjText->acceptCurrentAsOriginalCoors();

                boundingRect = pGraphObjText->boundingRect();
                ptCenter = boundingRect.center();
                pGraphObjText->moveBy(-ptCenter.x(), 0.0);

                pGraphObjText = new CGraphObjText(
                    /* pDrawingScene */ m_pDrawingScene,
                    /* drawSettings  */ drawSettingsText,
                    /* strObjName    */ "VLblB" + QString::number(x1_px));

                pGraphObjText->setPos( QPoint(x1_px, y2_px) );
                pGraphObjText->setPlainText(QString::number(x1_px));
                m_pDrawingScene->addGraphObj(pGraphObjText);
                m_pDrawingScene->onGraphObjCreationFinished(pGraphObjText);
                pGraphObjText->acceptCurrentAsOriginalCoors();

                boundingRect = pGraphObjText->boundingRect();
                ptCenter = boundingRect.center();
                pGraphObjText->moveBy(-ptCenter.x(), -boundingRect.height());
            }

            x1_px += xOffs_px;
            x2_px = x1_px;

            ++idxLine;
        }
    }

    // Rectangle for putting the wallpaper
    //------------------------------------

    //CDrawSettings drawSettingsRect(EGraphObjTypeRect);

    //QColor colWallpaperBackground(Qt::yellow);
    //colWallpaperBackground.setAlpha(64);

    //drawSettingsRect.setFillStyle(EFillStyle::SolidPattern);
    //drawSettingsRect.setFillColor(colWallpaperBackground);

    //// Factor ScreenResolutionWidth / ScreenResolutionHeight;
    //double fFacScreenRes = 1280.0 / 800.0;

    //// Visible height of wallpaper
    //double fWallpaperVisibleHeight = pageSetup.getDrawingHeightInPixels() / fFacScreenRes;

    //// Visible width of wallpaper
    //double fWallpaperVisibleWidth = fFacScreenRes * fWallpaperVisibleHeight;

    //// The visible part of the wallpaper is vertically centered.
    //double fy1_px = (pageSetup.getDrawingHeightInPixels() - fWallpaperVisibleHeight) / 2.0;

    //// The visible part of the wallpaper is adjusted to the left side.
    //double fx1_px = 0.0;

    //CGraphObjRect* pGraphObjWallpaperRect = new CGraphObjRect(
    //    /* pDrawingScene */ m_pDrawingScene,
    //    /* drawSettings  */ drawSettingsRect,
    //    /* strObjName    */ "WallpaperRect" );

    //pGraphObjWallpaperRect->setRect( QRectF(fx1_px, fy1_px, fWallpaperVisibleWidth, fWallpaperVisibleHeight) );
    //m_pDrawingScene->addGraphObj(pGraphObjWallpaperRect);
    //m_pDrawingScene->onGraphObjCreationFinished(pGraphObjWallpaperRect);
    //pGraphObjWallpaperRect->acceptCurrentAsOriginalCoors();

    // Result Values
    //--------------

    QStringList strlstResultValues;
    QString     strResultValues;

    // Horizontal Grid Lines
    //----------------------

    for( const auto& pGraphObj : arpGraphObjs )
    {
        CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);

        if( pGraphObjLine != nullptr && pGraphObjLine->name().startsWith("HL") )
        {
            if( !strResultValues.isEmpty() ) strResultValues += ", ";
            QLineF line = pGraphObjLine->getLine();
            strResultValues += QString::number(line.y1());
        }
    }

    strlstResultValues += strResultValues;
    strResultValues.clear();

    // Vertical Grid Lines
    //----------------------

    for( const auto& pGraphObj : arpGraphObjs )
    {
        CGraphObjLine* pGraphObjLine = dynamic_cast<CGraphObjLine*>(pGraphObj);

        if( pGraphObjLine != nullptr && pGraphObjLine->name().startsWith("VL") )
        {
            if( !strResultValues.isEmpty() ) strResultValues += ", ";
            QLineF line = pGraphObjLine->getLine();
            strResultValues += QString::number(line.x1());
        }
    }

    strlstResultValues += strResultValues;
    strResultValues.clear();

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepAndroidWallpaperTemplateDrawGridLines

//------------------------------------------------------------------------------
void CTest::doTestStepAndroidWallpaperTemplateSaveAsWallpaperPng( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->path();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepAndroidWallpaperTemplateSaveAsWallpaperPng",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //---------------

    QStringList strlstExpectedValues;

    QSize sizeDrawing = m_pDrawingView->getDrawingSizeInPixels();

    if( sizeDrawing.width() == 640 )
    {
        strlstExpectedValues += "FileSize: 19743";
    }
    else if( sizeDrawing.width() == 2048 )
    {
        strlstExpectedValues += "FileSize: 121926";
    }
    else
    {
        strlstExpectedValues += "FileSize: ?";
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    // Range of IniFileScope: ["AppDir", "User", "System"]
    #ifdef __linux__
    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
    // where the application has not write access rights. Stupid ...
    QString strIniFileScope = "User";
    #else
    QString strIniFileScope = "System"; // Default
    #endif

    QString strDim = QString::number(sizeDrawing.width()) + "x" + QString::number(sizeDrawing.height());

    QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);
    QString strFileName = strAppLogDir + "/" + "test_wallpaper_template_" + strDim + ".png";

    QImage img(m_pDrawingView->getDrawingWidthInPixels(), m_pDrawingView->getDrawingHeightInPixels(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    m_pDrawingScene->render(&painter);
    painter.end();
    img.save(strFileName);

    // Result Values
    //--------------

    QStringList strlstResultValues;
    QFileInfo fileInfo(strFileName);
    strlstResultValues += "FileSize: " + QString::number(fileInfo.size());

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepAndroidWallpaperTemplateSaveAsWallpaperPng
