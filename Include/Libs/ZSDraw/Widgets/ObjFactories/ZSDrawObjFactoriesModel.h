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

#ifndef ZSDraw_ObjFactoriesModel_h
#define ZSDraw_ObjFactoriesModel_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

namespace ZS
{
namespace Draw
{
class CObjFactory;

//******************************************************************************
class ZSDRAWDLL_API CObjFactoriesModel : public ZS::System::GUI::CModelIdxTree
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CObjFactoriesModel"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    typedef enum
    {
        EColumnMin      =  0,
        EColumnNodeName =  0,
        EColumnMax      =  0,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CObjFactoriesModel( ZS::System::CIdxTree* i_pIdxTree, QObject* i_pObjParent = nullptr );
    virtual ~CObjFactoriesModel();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // overridables of base class QAbstractItemModel, overriding implementation in class CModelIdxTree
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QStringList mimeTypes() const;
    virtual QMimeData* mimeData( const QModelIndexList& i_modelIdxList ) const;

}; // class CObjFactoriesModel

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_ObjFactoriesModel_h
