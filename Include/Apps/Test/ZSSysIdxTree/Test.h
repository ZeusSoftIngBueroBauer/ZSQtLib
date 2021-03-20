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

#ifndef ZSApps_TestIdxTree_Test_h
#define ZSApps_TestIdxTree_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

class QTimer;

namespace ZS
{
namespace System
{
class CIdxTree;
class CAbstractIdxTreeEntry;
}
namespace Apps
{
namespace Test
{
namespace IdxTree
{
//******************************************************************************
struct STreeEntryDscr
//******************************************************************************
{
public: // ctors and dtor
    STreeEntryDscr();
    STreeEntryDscr( const ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry );
    STreeEntryDscr( const STreeEntryDscr& i_other );
public: // operators
    bool operator == (const STreeEntryDscr& i_other) const;
public: // instance methods
    QString toString( bool i_bWithKeyInTree = false, bool i_bWithIdxInParentBranch = false, bool i_bWithKeyInParentBranch = false ) const;
public: // instance members
    ZS::System::EIdxTreeEntryType m_entryType;
    QString                       m_strName;
    QString                       m_strKeyInTree;           // Key is: <EntryTypeSymbol>:<ParentPath>/<Name> (e.g. "L:ZS::Data::CDataTable::FDAC::RF1In")
    int                           m_idxInTree;              // Index of this entry in the tree's vector of entries ("global tree index").
    QString                       m_strParentBranchPath;
    QString                       m_strKeyInParentBranch;   // Key is: <EntryTypeSymbol>:<Name> (e.g. "L:RF1In")
    int                           m_idxInParentBranch;      // Index of this entry in this parent branch's vector of child entries ("local branch index").
};

//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    static const QString c_strTrcAdmObjIdxTreeName;
public: // ctors and dtor
    CTest( const QString& i_strTestStepsFileName );
    ~CTest();
signals:
    void idxTreeAdded( ZS::System::CIdxTree* i_pIdxTree );
protected slots:
    void onIdxTreeDestroyed( QObject* i_pIdxTree );
public slots: // test step methods
    void doTestStepCreateTree( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAddEntry( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepRemoveEntry( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMoveEntry( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepCopyEntry( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepRenameEntry( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepChangeViews( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepKeyboardInputs( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTreeViewContextMenus( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpTrcAdmObjTreeStepTreeViewDragAndDrop( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepTableViewBranchContentKeyboardInputs( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepTableViewBranchContentBranchContextMenu( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepTableViewBranchContentLeaveContextMenu( ZS::Test::CTestStep* i_pTestStep );
    //void doTestStepTableViewBranchContentDragDrop( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDeleteTree( ZS::Test::CTestStep* i_pTestStep );
public: // auxiliary methods
    STreeEntryDscr toTreeEntryDscr( const ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry ) const;
    QVector<STreeEntryDscr> toTreeEntryDscrs( const QVector<ZS::System::CAbstractIdxTreeEntry*>& i_arpTreeEntries ) const;
public: // auxiliary methods
    int indexOf(
        const QString&                 i_strKeyInTree,
        const QVector<STreeEntryDscr>& i_arTreeEntries ) const;
    void swap(
        int                      i_idxInTree1,
        int                      i_idxInTree2,
        QVector<STreeEntryDscr>& i_arTreeEntries ) const;
    int addEntry(
        const ZS::System::CIdxTree& i_idxTree,
        const QString&              i_strKeyInTree,
        const QString&              i_strNameSpace,
        const QString&              i_strClassName,
        const QString&              i_strObjName,
        QVector<STreeEntryDscr>&    i_arTreeEntries,
        QMap<int, int>&             i_mapFreeIdxs ) const;
    void removeEntry(
        const ZS::System::CIdxTree& i_idxTree,
        const STreeEntryDscr&       i_treeEntry,
        QVector<STreeEntryDscr>&    i_arTreeEntries,
        QMap<int, int>&             i_mapFreeIdxs ) const;
    void renameEntry(
        const ZS::System::CIdxTree& i_idxTree,
        const QString&              i_strNameNew,
        STreeEntryDscr&             i_treeEntry,
        QVector<STreeEntryDscr>&    i_arTreeEntries ) const;
    void moveEntry(
        const ZS::System::CIdxTree& i_idxTree,
        const STreeEntryDscr&       i_treeEntryTrg,
        STreeEntryDscr&             i_treeEntrySrc,
        QVector<STreeEntryDscr>&    i_arTreeEntries ) const;
    void copyEntry(
        const ZS::System::CIdxTree& i_idxTree,
        const STreeEntryDscr&       i_treeEntryTrg,
        STreeEntryDscr&             i_treeEntrySrc,
        QVector<STreeEntryDscr>&    i_arTreeEntries,
        QMap<int, int>&             i_mapFreeIdxs ) const;
    void onParentBranchRenamed(
        const ZS::System::CIdxTree& i_idxTree,
        const QString&              i_strParentBranchPathNew,
        const QString&              i_strParentBranchPathPrev,
        STreeEntryDscr&             i_treeEntry,
        QVector<STreeEntryDscr>&    i_arTreeEntries ) const;
public: // auxiliary methods
    void splitKey(
        const ZS::System::CIdxTree& i_idxTree,
        const QString&              i_strKey,
        QString&                    o_strNameSpace,
        QString&                    o_strClassName,
        QString&                    o_strObjName ) const;
    void splitKey(
        const ZS::System::CIdxTree& i_idxTree,
        const QString&              i_strKey,
        QStringList&                o_strlstPathNodes,
        QStringList&                o_strlstKeysDefined,
        QStringList&                o_strlstNameSpacesDefined,
        QStringList&                o_strlstClassNamesDefined,
        QStringList&                o_strlstObjNamesDefined ) const;
    void compare(
        QVector<STreeEntryDscr>&       i_arTreeEntriesDesired,
        const QVector<STreeEntryDscr>& i_arTreeEntriesActual,
        QStringList&                   io_strlstDesiredValues,          // Compare results will be added
        QStringList&                   io_strlstActualValues ) const;   // Compare results will be added
    void compare(
        const QMap<int, int>& i_mapFreeIdxsDesired,
        const QMap<int, int>& i_mapFreeIdxsActual,
        QStringList&          io_strlstDesiredValues,           // Compare results will be added
        QStringList&          io_strlstActualValues ) const;    // Compare results will be added
protected: // instance members
    ZS::System::CIdxTree* m_pIdxTree;

}; // class CTest

} // namespace IdxTree

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIdxTree_Test_h
