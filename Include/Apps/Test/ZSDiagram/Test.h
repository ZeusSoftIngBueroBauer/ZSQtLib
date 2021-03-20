/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestDiagram_Test_h
#define ZSApps_TestDiagram_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSDiagram/ZSDiagramAux.h"

class QFrame;
class QTimer;

namespace ZS
{
namespace Diagram
{
class CWdgtDiagram;
class CDiagScale;
class CDiagTrace;
class CDiagObjGrid;
class CDiagObjAxisLabel;
class CDiagObjCurve;
class CDiagObjMarker;
class CDiagObjRect;
class CDiagObjLabel;
}

namespace Apps
{
namespace Test
{
namespace Diagram
{
//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTest( const QString& i_strTestStepsFileName );
    ~CTest();
private slots: // test step methods (Signal Generators)
    void doTestStepSigGenCreateDiagram( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenDestroyDiagram( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenAddScales( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenRemoveScales( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenAddTraces( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenRemoveTraces( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenAddGrid( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenRemoveGrid( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenAddAxisLabels( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenRemoveAxisLabels( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenAddCurves( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenRemoveCurves( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenAddMarkers( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenRemoveMarkers( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSigGenSetValues( ZS::Test::CTestStep* i_pTestStep );
private slots:
    void onTimerSigGenTimeout();
private slots: // test step methods (SBarrier interference checks)
    void doTestStepSBarCreateDiagram( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSBarDestroyDiagram( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSBarITabDef( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSBarLock( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSBarUnlock( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSBarInterferenceCheck( ZS::Test::CTestStep* i_pTestStep );
private slots:
    void onTimerTestStepTimeout();
private: // instance members
    QTimer*                                    m_pTmrTestStepTimeout;
    QTimer*                                    m_pTimerSigGen;
    QFrame*                                    m_pFrameDiagram;
    ZS::Diagram::CWdgtDiagram*                 m_pWdgtDiagram;
    ZS::Diagram::CDiagScale*                   m_pDiagScaleX;
    ZS::Diagram::CDiagScale*                   m_pDiagScaleY;
    ZS::Diagram::CDiagObjGrid*                 m_pDiagObjGrid;
    ZS::Diagram::CDiagObjAxisLabel*            m_pDiagObjAxisLabelX;
    ZS::Diagram::CDiagObjAxisLabel*            m_pDiagObjAxisLabelY;
    ZS::Diagram::CDiagTrace*                   m_pDiagTraceSigGen1;
    ZS::Diagram::CDiagTrace*                   m_pDiagTraceSigGen2;
    ZS::Diagram::CDiagObjCurve*                m_pDiagObjCurveSigGen1;
    ZS::Diagram::CDiagObjCurve*                m_pDiagObjCurveSigGen2;
    ZS::Diagram::CDiagObjMarker*               m_pDiagObjMarkerSigGen1;
    ZS::Diagram::CDiagObjMarker*               m_pDiagObjMarkerSigGen2;
    QMap<QString, QColor>                      m_mapSBarCols;
    QMap<QString, int>                         m_mapSBarLabelsPosOffsY;
    QVector<QVector<double>>                   m_ararfIsoTab_dB;
    QMap<QString, int>                         m_mapiPathsSBars;
    QMap<QString, ZS::Diagram::CDiagTrace*>    m_mappDiagTracesSBars;
    QMap<QString, ZS::Diagram::CDiagObjRect*>  m_mappDiagObjRectsSBars;
    QMap<QString, ZS::Diagram::CDiagObjLabel*> m_mappDiagObjLabelsSBars;
    QMap<QString, ZS::Diagram::CDiagTrace*>    m_mappDiagTracesSBarsIfCheck;
    QMap<QString, ZS::Diagram::CDiagObjRect*>  m_mappDiagObjRectsSBarsIfCheck;
    QStringList                                m_strlstSBarsLocked;

}; // class CTest

} // namespace Diagram

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestDiagram_Test_h
