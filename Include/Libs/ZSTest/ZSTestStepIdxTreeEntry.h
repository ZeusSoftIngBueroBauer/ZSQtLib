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

#ifndef ZSTest_TestStepIdxTreeEntry_h
#define ZSTest_TestStepIdxTreeEntry_h

#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"
#include "ZSTest/ZSTestDllMain.h"

namespace ZS
{
namespace Test
{
class CTest;
class CTestStepGroup;

//******************************************************************************
class ZSTESTDLL_API CAbstractTestStepIdxTreeEntry : public QObject, public ZS::System::CIdxTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test"; }
    static QString ClassName() { return "CAbstractTestStepIdxTreeEntry"; }
protected: // ctor
    CAbstractTestStepIdxTreeEntry(
        CTest* i_pTest,
        ZS::System::CIdxTreeEntry::EEntryType i_entryType,
        const QString& i_strName,
        CTestStepGroup* i_pTSGrpParent );
public: // dtor
    virtual ~CAbstractTestStepIdxTreeEntry();
public: // overridables
    virtual QString nameSpace() const { return CAbstractTestStepIdxTreeEntry::NameSpace(); }
    virtual QString className() const { return CAbstractTestStepIdxTreeEntry::ClassName(); }
signals:
    void changed( ZS::System::CIdxTreeEntry* i_pTreeEntry );
public: // overridables
    virtual bool isGroup() const;
    virtual CTestStepGroup* getParentGroup();
public: // instance methods
    CTest* getTest() { return m_pTest; }
public: // overridables
    virtual void setEnabled( ZS::System::EEnabled i_enabled );
    virtual ZS::System::EEnabled getEnabled() const { return m_enabled; }
    virtual bool isEnabled() const { return m_enabled == ZS::System::EEnabled::Yes; }
public: // overridables
    virtual void setToolTip( const QString& i_strToolTip );
    virtual QString getToolTip() const { return m_strToolTip; }
public: // overridables
    virtual int getDataRowCount() const { return m_arDataRows.size(); }
    virtual void addDataRow(const QHash<QString, QVariant>& i_dataRow);
    virtual void addDataRow(const QHash<QString, QVariant>&& i_dataRow);
    virtual QHash<QString, QVariant> getDataRow(int i_idxRow) const;
public: // must overridables
    virtual CEnumTestResult getTestResult() const = 0;
    virtual double getTestDurationInSec() const = 0;
public: // overridables
    virtual QString testDuration2StrInBestUnit() const;
protected: // overridables
    virtual void emit_changed();
private: // default ctor not allowed
    CAbstractTestStepIdxTreeEntry();
private: // copy ctor not allowed
    CAbstractTestStepIdxTreeEntry( const CAbstractTestStepIdxTreeEntry& );
protected: // instance members
    CTest* m_pTest;
    ZS::System::EEnabled m_enabled;
    QString m_strToolTip;
    CEnumTestResult m_testResult;
    QList<QHash<QString, QVariant>> m_arDataRows;

}; // class CAbstractTestStepIdxTreeEntry

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTest_TestStepIdxTreeEntry_h
