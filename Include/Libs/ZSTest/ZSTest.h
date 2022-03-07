/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace System
{
class CObjPoolListEntry;
class CObjPoolTreeEntry;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Test
{
class CTestStepAdminObjPool;
class CTestStep;
class CTestStepGroup;

//******************************************************************************
class ZSTESTDLL_API CTest : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTest"; }
public: // type definitions and constants
    typedef void (*TFctDoTestStepGroup)( CTest* i_pThis, ZS::Test::CTestStepGroup* i_pTestStepGrpParent );
public: // type definitions and constants
    enum EState {
        EStateIdle    = 0,
        EStateInit    = 1,
        EStateRunning = 2,
        EStatePaused  = 3,
        EStateCount,
        EStateUndefined
    };
    static QString State2Str( int i_iState );
public: // ctors and dtor
    CTest(
        const QString& i_strName,
        const QString& i_strTestStepsFileName = "",
        const QString& i_strNodeSeparator = "\\",
        int            i_iTestStepInterval_ms = 0 );
    ~CTest();
signals:
    void stateChanged( int i_iStateNew, int i_iStatePrev );
    void runModeChanged( int i_iRunModeNew, int i_iRunModePrev );
    void currentTestStepChanged( ZS::Test::CTestStep* i_pTestStep );
public: // overridables
    virtual QString nameSpace() const { return CTest::NameSpace(); }
    virtual QString className() const { return CTest::ClassName(); }
public: // instance methods
    ZS::Trace::CTrcAdminObj* getTraceAdminObj() { return m_pTrcAdminObj; }
public: // instance methods
    CTestStepGroup* getTestStepGroupRoot();
    CTestStepGroup* getTestStepGroup( const QString& i_strGroupName, const QString& i_strGroupNameParent );
    CTestStepGroup* getTestStepGroup( const QString& i_strGroupName, CTestStepGroup* i_pTestGroupParent );
    CTestStep* getTestStep( const QString& i_strName, const QString& i_strGroupNameParent );
    CTestStep* getTestStep( const QString& i_strName, CTestStepGroup* i_pTestGroupParent );
public: // instance methods
    void setTestStepInterval( int i_iTestStepInterval_ms );
    int getTestStepIntervalInMs() const;
public: // overridables
    EState getState() const { return m_state; }
    bool isRunning() const { return (m_state == EStateRunning); }
public: // overridables
    virtual void init();
    virtual void start();
    virtual void step();
    virtual void stop();
    virtual void abort();
    virtual void pause();
    virtual void resume();
public: // overridables
    virtual void triggerDoTestStep( int i_iInterval_ms = -1 );
    virtual void triggerNextTestStep( int i_iInterval_ms = -1 );
public: // instance methods
    CTestStepAdminObjPool* getAdminObjIdxTree() { return m_pAdminObjPool; }
    CTestStep* getCurrentTestStep() { return m_pTestStepCurr; }
public: // overridables
    void onTestStepGroupChanged( CTestStepGroup* i_pTSGrp );
    void onTestStepChanged( CTestStep* i_pTestStep );
protected slots: // overridables
    virtual void doTestStep();
    virtual void onCurrentTestStepFinished( ZS::Test::CTestStep* i_pTestStep );
protected: // instance methods
    void setState( EState i_state );
    void setRunMode( ZS::System::ERunMode i_runMode );
protected: // instance methods
    CTestStep* getNextTestStep( CTestStep* i_pTestStepCurr );
    CTestStep* getNextTestStep( ZS::System::CObjPoolTreeEntry* i_pTreeEntryParent, ZS::System::CObjPoolTreeEntry* i_pTreeEntryFinished );
    void setCurrentTestStep( CTestStep* i_pTestStep );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pMsg );
protected: // instance members
    CTestStepAdminObjPool*       m_pAdminObjPool;
    CTestStep*                   m_pTestStepCurr;
    int                          m_iTestStepGroup;
    QVector<TFctDoTestStepGroup> m_arfctsDoTestStepGroups;
    int                          m_iTestStepInterval_ms;
    EState                       m_state;
    ZS::System::ERunMode         m_runMode;
    bool                         m_bDoTestStepPending;
    ZS::Trace::CTrcAdminObj*     m_pTrcAdminObj;

}; // class CTest

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_Test_h
