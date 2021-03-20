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

#ifndef ZSTest_TestStepAdminObj_h
#define ZSTest_TestStepAdminObj_h

#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

#include "ZSSys/ZSSysAux.h"
#include "ZSTest/ZSTestDllMain.h"

namespace ZS
{
namespace System
{
class CObjPoolListEntry;
class CObjPoolTreeEntry;
}

namespace Test
{
class CTest;
class CTestStepAdminObjPool;
class CTestStep;
class CTestStepGroup;

//******************************************************************************
class ZSTESTDLL_API CTestStepAdminObj : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CTestStepAdminObj"; }
protected: // ctor (obsolete)
    CTestStepAdminObj(
        CTestStepAdminObjPool*         i_pObjPool,
        const QString&                 i_strName,
        ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
protected: // ctor
    CTestStepAdminObj( CTest* i_pTest, const QString& i_strName );
public: // dtor
    virtual ~CTestStepAdminObj();
public: // overridables
    virtual QString nameSpace() const { return CTestStepAdminObj::NameSpace(); }
    virtual QString className() const { return CTestStepAdminObj::ClassName(); }
public: // must overridables
    virtual bool isGroup() const = 0;
public: // instance methods
    void setName( const QString& i_strName );
    QString getName( bool i_bIncludeGroupNameParents = false ) const;
    QString getParentName( bool i_bIncludeGroupNameParents = false ) const;
    CTestStepGroup* getParentGroup();
public: // instance methods
    void setEnabled( ZS::System::EEnabled i_enabled );
    ZS::System::EEnabled getEnabled() const { return m_enabled; }
    bool isEnabled() const;
public: // overridables
    virtual ETestResult getTestResult() const { return m_testResult; }
public: // must overridables
    virtual void setTestResult( ETestResult i_testResult ) = 0;
public: // must overridables
    virtual void testStarted() = 0;
    virtual void testEnded( bool i_bIgnoreTestResult = false ) = 0; // Implicitly updates test end time if not already updated.
public: // overridables
    virtual void reset();
    virtual bool isFinished() { return false; }
public: // overridables
    virtual void updateTestEndTime();
public: // instance methods
    bool isTestRunning() const { return m_bTestRunning; }
    double getTestDurationInSec() const;
    double getTestDurationInMilliSec() const;
    double getTestDurationInMicroSec() const;
    double getTestDurationInNanoSec() const;
    QString testDuration2StrInBestUnit() const;
public: // instance methods
    virtual QString getToolTip() const { return m_strToolTip; }
    virtual void setToolTip( const QString& i_strToolTip );
public: // instance methods
    virtual void update() = 0; // triggers the "nodeChanged" and "dataChanged" signals of the model
public: // instance methods (experts use only)
    void setTreeEntry( ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
    ZS::System::CObjPoolTreeEntry* getTreeEntry() { return m_pTreeEntry; }
private: // default ctor not allowed
    CTestStepAdminObj();
private: // copy ctor not allowed
    CTestStepAdminObj( const CTestStepAdminObj& );
protected: // instance members
    CTest*                          m_pTest;
    CTestStepAdminObjPool*          m_pObjPool;
    QString                         m_strName;
    ZS::System::CObjPoolTreeEntry*  m_pTreeEntry;
    ZS::System::EEnabled            m_enabled;
    bool                            m_bTestRunning;
    ETestResult                     m_testResult;
    double                          m_fTimeTestStart_s;
    double                          m_fTimeTestEnd_s;
    QString                         m_strToolTip;

}; // class CTestStepAdminObj

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStepAdminObj_h
