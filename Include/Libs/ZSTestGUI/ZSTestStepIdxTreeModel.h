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

#ifndef ZSTestGUI_TestStepIdxTreeModel_h
#define ZSTestGUI_TestStepIdxTreeModel_h

#include "ZSTestGUI/ZSTestGUIDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

class QDomNode;

namespace ZS
{
#ifdef ZS_TRACE_GUI_MODELS
namespace System
{
class CTrcAdminObj;
}
#endif

namespace Test
{
class CTest;
class CTestStep;
class CTestStepIdxTree;

namespace GUI
{
//******************************************************************************
class ZSTESTGUIDLL_API CModeldxTreeTestSteps : public ZS::System::GUI::CModelIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Test::GUI"; }
    static QString ClassName() { return "CModeldxTreeTestSteps"; }
public: // type definitions and constants
    enum EColumn {
        EColumnName           = 0,
        EColumnBreakpoint     = 1,
        EColumnEnabled        = 2,
        EColumnResult         = 3,
        EColumnDuration       = 4,
        EColumnOperation      = 5,
        EColumnDescription    = 6,
        EColumnExpectedValues = 7,
        EColumnResultValues   = 8,
        EColumnCount
    };
    static QString column2Str( int i_iClm, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );
public: // ctors and dtor
    CModeldxTreeTestSteps( CTestStepIdxTree* i_pIdxTree, QObject* i_pObjParent = nullptr );
    virtual ~CModeldxTreeTestSteps();
public: // instance methods
    CTestStepIdxTree* idxTree();
public: // overridables
    virtual QString nameSpace() const override { return NameSpace(); }
    virtual QString className() const override { return ClassName(); }
public: // instance methods
    bool isSetShowExpectedAndResultValuesOnlyIfTestStepFailed() const { return m_bShowExpectedAndResultValuesOnlyIfTestStepFailed; }
    void setShowExpectedAndResultValuesOnlyIfTestStepFailed( bool i_bSet );
//public: // instance methods
//    void reset(); // to call protected reset method
protected slots: // overridables
    virtual void onTestCurrentTestStepChanged( ZS::Test::CTestStep* i_pTestStep );
public: // overridables of base class QAbstractItemModel
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const override;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
private: // copy ctor not allowed
    CModeldxTreeTestSteps( const CModeldxTreeTestSteps& );
private: // assignment operator not allowed
    CModeldxTreeTestSteps& operator = ( const CModeldxTreeTestSteps& );
protected: // instance members
    CTest*     m_pTest;
    CTestStep* m_pTestStepCurr;
    bool       m_bShowExpectedAndResultValuesOnlyIfTestStepFailed;
    #ifdef ZS_TRACE_GUI_MODELS
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    #endif

}; // class CModeldxTreeTestSteps

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepIdxTreeModel_h
