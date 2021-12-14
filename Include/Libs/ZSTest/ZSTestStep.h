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
class CTestStepGroup;

//******************************************************************************
class ZSTESTDLL_API CTestStep : public CAbstractTestStepIdxTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStep"; }
public: // type definitions and constants
    //typedef ETestResult (*TFctDoTestStep)( CTest* i_pTest, CTestStep* i_pTestStep );
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
    //void finished( ZS::Test::CTestStep* i_pTestStep ); // emitted if actual values are set
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
    QStringList getDesiredValues() const { return m_strlstDesiredValues; }
    void setDesiredValues( const QStringList& i_strlstDesiredValues = QStringList() );
    void setDesiredValue( const QString& i_strDesiredValue = QString() ); // Provided for convenience. Converted to String List.
public: // instance methods
    QStringList getActualValues() const { return m_strlstActualValues; }
    void setActualValues( const QStringList& i_strlstActualValues = QStringList() ); // finishes the test step
    void setActualValue( const QString& i_strDesiredValue = QString() ); // Provided for convenience. Converted to String List.
public: // instance methods
    void setBreakpoint();
    void removeBreakpoint();
    bool isBreakpointSet() const { return m_bBreakpoint; }
    void enableBreakpoint();
    void disableBreakpoint();
    bool isBreakpointEnabled() const { return m_bBreakpointEnabled; }
public: // instance methods
    void setTestResult( ETestResult i_testResult );
//public: // must overridables of base class CAbstractTestStepIdxTreeEntry
//    virtual void testStarted();
//    virtual void testEnded( bool i_bIgnoreTestResult = false ); // Implicitly updates test end time if not already updated.
//    virtual void reset();
//public: // must overridables of base class CAbstractTestStepIdxTreeEntry
//    virtual bool isFinished() const;
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
    virtual double getTestDurationInSec() const override;
    virtual double getTestDurationInMilliSec() const override;
    virtual double getTestDurationInMicroSec() const override;
    virtual double getTestDurationInNanoSec() const override;
public: // instance methods
    virtual void doTestStep();
private: // default ctor not allowed
    CTestStep();
private: // copy ctor not allowed
    CTestStep( const CTestStep& );
protected: // instance members
    //TFctDoTestStep m_fctDoTestStep;
    QString        m_strOperation;
    QString        m_strDescription;
    QStringList    m_strlstDesiredValues;
    QStringList    m_strlstActualValues;
    ETestResult    m_testResult;
    double         m_fTimeTestStart_s;
    double         m_fTimeTestEnd_s;
    bool           m_bBreakpoint;
    bool           m_bBreakpointEnabled;

}; // class CTestStep

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStep_h
