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

#ifndef ZSTest_TestStep_h
#define ZSTest_TestStep_h

#include "ZSTest/ZSTestStepIdxTreeEntry.h"

namespace ZS
{
namespace Test
{
//******************************************************************************
/*! @brief Single test step. Test steps are organized as leaves of the index tree.
*/
class ZSTESTDLL_API CTestStep : public CAbstractTestStepIdxTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStep"; }
    static int testStepCount();
public: // ctors and dtor
    CTestStep(
        CTest*          i_pTest,
        const QString&  i_strName,
        const QString&  i_strOperation,
        CTestStepGroup* i_pTSGrpParent,
        const char*     i_szDoTestStepSlotFct );
    virtual ~CTestStep();
signals:
    /*! @brief This signal is emitted to trigger the next step. */
    void doTestStep( ZS::Test::CTestStep* i_pTestStep );
    /*! @brief This signal is emitted if result values are set. */
    void testStepFinished( ZS::Test::CTestStep* i_pTestStep );
public: // instance methods
    QString getOperation() const { return m_strOperation; }
    void setOperation( const QString& i_strOperation );
public: // instance methods
    QString getDescription() const { return m_strDescription; }
    void setDescription( const QString& i_strDescription );
public: // instance methods
    QStringList getConfigValueKeys() const;
    bool hasConfigValue( const QString& i_strKey ) const;
    QVariant getConfigValue( const QString& i_strKey ) const;
    void setConfigValue( const QString& i_strKey, const QVariant& i_val );
public: // instance methods
    QString getInstruction() const { return m_strInstruction; }
    void setInstruction( const QString& i_strInstruction );
public: // instance methods
    QStringList getExpectedValues() const { return m_strlstExpectedValues; }
    void setExpectedValues( const QStringList& i_strlstExpectedValues );
    void setExpectedValue( const QString& i_strExpectedValue );
public: // instance methods
    QStringList getResultValues() const { return m_strlstResultValues; }
    void setResultValues( const QStringList& i_strlstResultValues );
    void setResultValue( const QString& i_strResultValue );
public: // auxiliary instance methods
    CEnumTestResult detectTestResult( const QStringList& i_strlstExpectedValues, const QStringList& i_strlstResultValues );
    CEnumTestResult detectTestResult( const QString& i_strExpectedValue, const QString& i_strResultValue );
public: // instance methods
    bool isFinished() const;
public: // instance methods
    void setBreakpoint();
    void removeBreakpoint();
    bool isBreakpointSet() const { return m_bBreakpoint; }
    void setBreakpointEnabled( ZS::System::EEnabled i_enabled );
    ZS::System::EEnabled getBreakpointEnabled() const { return m_breakpointEnabled; };
    bool isBreakpointEnabled() const { return m_breakpointEnabled == ZS::System::EEnabled::Yes; }
public: // instance methods
    void reset();
public: // instance methods
    void setTestResult( const CEnumTestResult& i_result );
public: // must overridables of base class CAbstractTestStepIdxTreeEntry
    virtual CEnumTestResult getTestResult() const override { return m_testResult; }
    virtual double getTestDurationInSec() const override;
public: // instance methods
    virtual void doTestStep();
private: // default ctor not allowed
    CTestStep();
private: // copy ctor not allowed
    CTestStep( const CTestStep& );
private: // class members
    /*!< Counter for number of created instances. */
    static int s_iInstCount;
protected: // instance members
    /*!< Operation which will be executed (e.g. NameSpace::Class::Inst.method(args)). */
    QString m_strOperation;
    /*!< Description of the test step. */
    QString m_strDescription;
    /*!< Hash with config values assigned to the test step. */
    QHash<QString, QVariant> m_hshConfigValues;
    /*!< List with the expected result values. */
    QString m_strInstruction;
    /*!< List with the expected result values. */
    QStringList m_strlstExpectedValues;
    /*!< List with the result values. */
    QStringList m_strlstResultValues;
    /*!< Start time of the test step in seconds since start of the whole test. */
    double m_fTimeTestStart_s;
    /*!< End time of the test step in seconds since start of the whole test. */
    double m_fTimeTestEnd_s;
    /*!< True if the test execution should stop before executing the step. */
    bool m_bBreakpoint;
    /*!< True if the breakpoint is enabled. */
    ZS::System::EEnabled m_breakpointEnabled;

}; // class CTestStep

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStep_h
