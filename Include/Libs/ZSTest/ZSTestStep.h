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

#ifndef ZSTest_TestStep_h
#define ZSTest_TestStep_h

#include "ZSTest/ZSTestStepIdxTreeEntry.h"

namespace ZS
{
namespace Test
{
//******************************************************************************
class ZSTESTDLL_API CTestStep : public CAbstractTestStepIdxTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStep"; }
public: // ctors and dtor
    //CTestStep(
    //    CTest*          i_pTest,
    //    const QString&  i_strName,
    //    const QString&  i_strOperation,
    //    CTestStepGroup* i_pTSGrpParent,
    //    TFctDoTestStep  i_fctDoTestStep );
    CTestStep(
        CTest*          i_pTest,
        const QString&  i_strName,
        const QString&  i_strOperation,
        CTestStepGroup* i_pTSGrpParent,
        const char*     i_szDoTestStepSlotFct );
    virtual ~CTestStep();
signals:
    void doTestStep( ZS::Test::CTestStep* i_pTestStep );
    void testStepFinished( ZS::Test::CTestStep* i_pTestStep ); // emitted if result values are set
public: // overridables
    virtual QString nameSpace() const { return CTestStep::NameSpace(); }
    virtual QString className() const { return CTestStep::ClassName(); }
public: // instance methods
    QString getOperation() const { return m_strOperation; }
    void setOperation( const QString& i_strOperation );
public: // instance methods
    QString getDescription() const { return m_strDescription; }
    void setDescription( const QString& i_strDescription );
public: // instance methods
    QStringList getExpectedValues() const { return m_strlstExpectedValues; }
    void setExpectedValues( const QStringList& i_strlstExpectedValues = QStringList() );
    void setExpectedValue( const QString& i_strExpectedValue = QString() ); // Provided for convenience. Converted to String List.
public: // instance methods
    QStringList getResultValues() const { return m_strlstResultValues; }
    void setResultValues( const QStringList& i_strlstResultValues = QStringList() ); // finishes the test step
    void setResultValue( const QString& i_strResultValue = QString() ); // Provided for convenience. Converted to String List.
public: // instance methods
    bool isFinished() const;
public: // instance methods
    void setBreakpoint();
    void removeBreakpoint();
    bool isBreakpointSet() const { return m_bBreakpoint; }
    void enableBreakpoint();
    void disableBreakpoint();
    bool isBreakpointEnabled() const { return m_bBreakpointEnabled; }
public: // instance methods
    void reset();
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
    virtual CEnumTestResult getTestResult() const override { return m_testResult; }
    virtual double getTestDurationInSec() const override;
public: // instance methods
    virtual void doTestStep();
protected: // instance methods
    //virtual void testStarted();
    //virtual void testEnded( bool i_bIgnoreTestResult = false ); // Implicitly updates test end time if not already updated.
    void onTestStepFinished();
private: // default ctor not allowed
    CTestStep();
private: // copy ctor not allowed
    CTestStep( const CTestStep& );
protected: // instance members
    //TFctDoTestStep m_fctDoTestStep;
    QString         m_strOperation;
    QString         m_strDescription;
    QStringList     m_strlstExpectedValues;
    QStringList     m_strlstResultValues;
    double          m_fTimeTestStart_s;
    double          m_fTimeTestEnd_s;
    bool            m_bBreakpoint;
    bool            m_bBreakpointEnabled;

}; // class CTestStep

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStep_h
