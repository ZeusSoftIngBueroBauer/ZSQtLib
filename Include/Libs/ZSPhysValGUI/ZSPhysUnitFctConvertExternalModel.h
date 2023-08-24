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

#ifndef ZSPhysValGUI_PhysUnitFctConvertExternalModel_h
#define ZSPhysValGUI_PhysUnitFctConvertExternalModel_h

#include <QtCore/qabstractitemmodel.h>

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;
class CUnitsTreeEntryPhysUnit;

namespace GUI
{
//******************************************************************************
class CModelUnitFctConvertExternal : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnUnitGrp    = 0,
        EColumnFctConvert = 1,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelUnitFctConvertExternal( QObject* i_pObjParent = nullptr );
    virtual ~CModelUnitFctConvertExternal();
public: // instance methods
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
protected: // instance members
    QString m_strKeyInTreeOfRootEntry;
    CUnitsTreeEntryPhysUnit* m_pPhysUnit;

}; // class CModelUnitFctConvertExternal

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysUnitFctConvertExternalModel_h

