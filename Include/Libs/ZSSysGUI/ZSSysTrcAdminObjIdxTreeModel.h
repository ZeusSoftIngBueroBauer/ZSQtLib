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

#ifndef ZSSysGUI_TrcAdminObjIdxTreeModel_h
#define ZSSysGUI_TrcAdminObjIdxTreeModel_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
class CIdxTreeTrcAdminObjs;

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CModelIdxTreeTrcAdminObjs : public ZS::System::GUI::CModelIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CModelIdxTreeTrcAdminObjs"; }
public: // type definitions and constants
    enum EColumn {
        EColumnRefCount               = CModelIdxTree::EColumnCount +  0,
        EColumnEnabled                = CModelIdxTree::EColumnCount +  1,
        EColumnMethodCallsDetailLevel = CModelIdxTree::EColumnCount +  2,
        EColumnRuntimeInfoDetailLevel = CModelIdxTree::EColumnCount +  3,
        EColumnObjNameFilter          = CModelIdxTree::EColumnCount +  4,
        EColumnMethodNameFilter       = CModelIdxTree::EColumnCount +  5,
        EColumnDataFilter             = CModelIdxTree::EColumnCount +  6,
        EColumnNameSpace              = CModelIdxTree::EColumnCount +  7,
        EColumnClassName              = CModelIdxTree::EColumnCount +  8,
        EColumnObjName                = CModelIdxTree::EColumnCount +  9,
        EColumnObjThreadName          = CModelIdxTree::EColumnCount + 10,
        EColumnObjAddress             = CModelIdxTree::EColumnCount + 11,
        EColumnCount
    };
public: // class methods
    static QIcon GetIcon( const QString& i_strEntryType );
public: // ctors and dtor
    CModelIdxTreeTrcAdminObjs(
        CIdxTreeTrcAdminObjs* i_pIdxTree = nullptr,
        QObject* i_pObjParent = nullptr );
    virtual ~CModelIdxTreeTrcAdminObjs();
public: // instance methods
    CIdxTreeTrcAdminObjs* idxTree();
public: // instance methods
    //virtual void reset();
public: // overridables of base class QAbstractItemModel
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const override;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
    virtual bool setData( const QModelIndex& i_modelIdx, const QVariant& i_value, int i_iRole = Qt::EditRole ) override;
protected: // class members (hiding class members of base class CModelIdxTree)
    static bool     s_bIconsCreated;
    static QPixmap* s_pPxmBranchEntryNormalOff;
    static QPixmap* s_pPxmBranchEntrySelectedOff;
    static QPixmap* s_pPxmLeaveEntryNormalOff;
    static QPixmap* s_pPxmLeaveEntrySelectedOff;
    static QIcon*   s_pIconBranch;
    static QIcon*   s_pIconLeave;

}; // class CModelIdxTreeTrcAdminObjs

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_TrcAdminObjIdxTreeModel_h
