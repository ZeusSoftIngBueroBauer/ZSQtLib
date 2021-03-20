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

#ifndef ZSTestGUI_TestStepAdminObjPoolModel_h
#define ZSTestGUI_TestStepAdminObjPoolModel_h

#include "ZSTestGUI/ZSTestGUIDllMain.h"
#include "ZSSysGUI/ZSSysObjPoolModel.h"

class QDomNode;

namespace ZS
{
namespace Test
{
class CTestStepAdminObjPool;

namespace GUI
{
//******************************************************************************
class ZSTESTGUIDLL_API CTestStepAdminObjPoolModel : public ZS::System::GUI::CModelObjPool
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum EColumn {
        EColumnName          = 0,
        EColumnObjId         = 1,
        EColumnBreakpoint    = 2,
        EColumnEnabled       = 3,
        EColumnResult        = 4,
        EColumnDuration      = 5,
        EColumnOperation     = 6,
        EColumnDescription   = 7,
        EColumnDesiredValues = 8,
        EColumnActualValues  = 9,
        EColumnCount
    };
    static QString column2Str( int i_iClm, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );
public: // class methods
    static QIcon GetIcon( ZS::System::CObjPoolTreeEntry* i_pTreeEntry );
public: // ctors and dtor
    CTestStepAdminObjPoolModel( CTestStepAdminObjPool* i_pAdminObjPool, QObject* i_pObjParent = nullptr );
    virtual ~CTestStepAdminObjPoolModel();
public: // instance methods
    CTestStepAdminObjPool* getAdminObjIdxTree();
public: // instance methods
    bool isSetShowDesiredAndActualValuesOnlyIfTestStepFailed() const { return m_bShowDesiredAndActualValuesOnlyIfTestStepFailed; }
    void setShowDesiredAndActualValuesOnlyIfTestStepFailed( bool i_bSet );
protected slots:
    void onTestStepGroupInserted( QObject* i_pAdminObjPool, QObject* i_pTSGrp );
    void onTestStepGroupChanged( QObject* i_pAdminObjPool, QObject* i_pTSGrp );
    void onTestStepInserted( QObject* i_pAdminObjPool, QObject* i_pTestStep );
    void onTestStepChanged( QObject* i_pAdminObjPool, QObject* i_pTestStep );
    void onNameSpaceNodeChanged( QObject* i_pAdminObjPool, ZS::System::CObjPoolTreeEntry* i_pTreeEntry, int i_enabled );
public: // instance methods
    void reset(); // to call protected reset method
public: // overridables of base class QAbstractItemModel
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CTestStepAdminObjPoolModel( const CTestStepAdminObjPoolModel& );
private: // assignment operator not allowed
    CTestStepAdminObjPoolModel& operator = ( const CTestStepAdminObjPoolModel& );
protected: // class members
    static bool   s_bIconsCreated;
    static int    s_iInstCount;
    static QIcon* s_pIconTypeNameSpace;
    static QIcon* s_pIconTypeObject;
protected: // instance members
    bool m_bShowDesiredAndActualValuesOnlyIfTestStepFailed;

}; // class CTestStepAdminObjPoolModel

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepAdminObjPoolModel_h
