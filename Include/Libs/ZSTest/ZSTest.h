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

#ifndef ZSTest_Test_h
#define ZSTest_Test_h

#include <QtCore/qobject.h>
#include <QtCore/qvector.h>

#include "ZSTest/ZSTestDllMain.h"

namespace ZS
{
namespace System
{
class CIdxTreeEntry;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Test
{
class CTestStep;
class CTestStepGroup;
class CTestStepIdxTree;

//******************************************************************************
class ZSTESTDLL_API CTest : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTest"; }
public: // type definitions and constants
    //typedef void (*TFctDoTestStepGroup)( CTest* i_pThis, ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
public: // ctors and dtor
    CTest(
        const QString& i_strName,
        const QString& m_strTestStepsAbsFilePath,
        int            i_iTestStepInterval_ms = 0 );
    ~CTest();
signals:
    void stateChanged( const ZS::Test::CEnumTestState& i_state );
    void runModeChanged( const ZS::System::CEnumRunMode& i_runMode );
    void currentTestStepChanged( ZS::Test::CTestStep* i_pTestStep );
    void testStepIntervalChanged( int i_iInterval_ms );
public: // overridables
    virtual QString nameSpace() const { return CTest::NameSpace(); }
    virtual QString className() const { return CTest::ClassName(); }
public: // instance methods
    CTestStepIdxTree* getTestStepIdxTree() { return m_pIdxTree; }
public: // instance methods
    QString getTestStepsAbsFilePath() const { return m_strTestStepsAbsFilePath; }
    virtual ZS::System::SErrResultInfo save( const QString& i_strAbsFilePath = QString() );
    virtual ZS::System::SErrResultInfo recall( const QString& i_strAbsFilePath = QString() );
public: // instance methods
    void setTestStepInterval( int i_iTestStepInterval_ms );
    int getTestStepIntervalInMs() const { return m_iTestStepInterval_ms; }
public: // overridables
    CEnumTestState getState() const { return m_state; }
    bool isRunning() const { return (m_state == ETestState::Running); }
public: // overridables
    //virtual void init();
    virtual void start();
    virtual void step();
    virtual void stop();
    virtual void abort();
    virtual void pause();
    virtual void resume();
protected slots: // overridables
    virtual void doTestStep();
    virtual void onCurrentTestStepFinished( ZS::Test::CTestStep* i_pTestStep );
protected: // instance methods
    void setState( const CEnumTestState& i_state );
    void setRunMode( const ZS::System::CEnumRunMode& i_runMode );
protected: // instance methods
    void setCurrentTestStep( CTestStep* i_pTestStep );
    CTestStep* getCurrentTestStep() { return m_pTestStepCurr; }
protected: // instance methods
    CTestStep* getNextTestStep( CTestStep* i_pTestStepCurr );
    CTestStep* getNextTestStep( ZS::System::CIdxTreeEntry* i_pTreeEntryParent, ZS::System::CIdxTreeEntry* i_pTreeEntryFinished );
protected: // overridables
    virtual void triggerDoTestStep( int i_iInterval_ms = -1 );
    virtual void triggerNextTestStep( int i_iInterval_ms = -1 );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pMsg );
protected: // instance members
    CTestStepIdxTree*        m_pIdxTree;
    QString                  m_strTestStepsAbsFilePath;
    CTestStep*               m_pTestStepCurr;
    int                      m_iTestStepInterval_ms;
    CEnumTestState           m_state;
    ZS::System::CEnumRunMode m_runMode;
    bool                     m_bDoTestStepPending;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTest

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_Test_h
