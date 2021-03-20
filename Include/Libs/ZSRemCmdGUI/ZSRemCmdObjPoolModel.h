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

#ifndef ZSRemCmdGUI_ModelObjPool_h
#define ZSRemCmdGUI_ModelObjPool_h

#include "ZSRemCmdGUI/ZSRemCmdGUIDllMain.h"
#include "ZSSysGUI/ZSSysObjPoolModel.h"

namespace ZS
{
namespace RemCmd
{
class CRemCmdObjPool;

namespace GUI
{
//******************************************************************************
class ZSREMCMDGUIDLL_API CRemCmdObjPoolModel : public ZS::System::GUI::CModelObjPool
//******************************************************************************
{
public: // type definitions and constants
    enum EColumn {
        EColumnNodeName    = 0,
        EColumnType        = 1,
        EColumnObjId       = 2,
        EColumnSyntax      = 3,
        EColumnIsQuery     = 4,
        EColumnIsEvent     = 5,
        //EColumnDescription = 6,
        EColumnCount
    };
public: // ctors and dtor
    CRemCmdObjPoolModel( QObject* i_pObjParent = nullptr );
    virtual ~CRemCmdObjPoolModel();
public: // instance methods
    void setRemCmdObjPool ( CRemCmdObjPool* i_pObjPool );
    CRemCmdObjPool* getRemCmdObjPool();
public: // instance methods
    virtual void reset();
public: // overridables of base class QAbstractItemModel
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;

}; // class CRemCmdObjPoolModel

} // namespace GUI

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmdGUI_ModelObjPool_h
