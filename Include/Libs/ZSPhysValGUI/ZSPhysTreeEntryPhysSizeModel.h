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

#ifndef ZSPhysValGUI_PhysTreeEntryPhysSizeModel_h
#define ZSPhysValGUI_PhysTreeEntryPhysSizeModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModelBranchContent.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;
class CUnitsTreeEntryGrpPhysUnits;

namespace GUI
{
//******************************************************************************
class ZSPHYSVALGUIDLL_API CModelPhysSize : public ZS::System::GUI::CModelIdxTreeBranchContent
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CModelPhysSize"; }
public: // type definitions and constants
    typedef enum
    {
        EViewModeNormal           = 0,
        EViewModeContingencyTable = 1,
        EViewModeCount,
        EViewModeUndefined
    }   EViewMode;
    static QString viewMode2Str( int i_iViewMode );
    static EViewMode str2ViewMode( const QString& i_str );
public: // type definitions and constants
    typedef enum
    {
        EColumnUndefined = -1,
        EColumnSymbol = CModelIdxTreeBranchContent::EColumnCount,
        EColumnNextLower,
        EColumnNextHigher,
        EColumnFctConvertIntoSIUnit,
        EColumnFctConvertFromSIUnit,
        EColumnCount
    }   EColumn;
    static QString column2Str(EColumn i_clm);
public: // class methods
    static EColumn role2Column(int i_iRole);
    static QString role2Str(int i_iRole);
    static int byteArr2Role(const QByteArray& i_byteArrRole);
    static int column2Role(EColumn i_clm);
    Q_INVOKABLE static QString modelIdx2Str( const QModelIndex& i_modelIdx, int i_iRole = -1, bool i_bIncludeId = false );
public: // ctors and dtor
    CModelPhysSize( QObject* i_pObjParent = nullptr );
    CModelPhysSize( CIdxTreeUnits* i_pIdxTree, QObject*  i_pObjParent = nullptr );
    virtual ~CModelPhysSize();
public: // overridables
    virtual QString nameSpace() const { return CModelPhysSize::NameSpace(); }
    virtual QString className() const { return CModelPhysSize::ClassName(); }
public: // instance methods
    bool setViewMode( EViewMode i_viewMode );
    EViewMode getViewMode() const;
public: // overridables of base class CModelIdxTreeBranchContent
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree ) override;
public: // overridables of base class QAbstractItemModel
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
protected: // auxiliary instance methods
    void fillRoleNames();
protected: // class members
    static QHash<int, QByteArray> s_roleNames;
    static QHash<QByteArray, int> s_roleValues;
    static QHash<int, QByteArray> s_clm2Name;
protected: // instance members
    EViewMode m_viewMode;
    CUnitsTreeEntryGrpPhysUnits* m_pPhysSize;

}; // class CModelPhysSize

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysTreeEntryPhysSizeModel_h
