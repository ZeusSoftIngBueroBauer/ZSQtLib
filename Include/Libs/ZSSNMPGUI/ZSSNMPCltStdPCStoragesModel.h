/*******************************************************************************

Copyright 2004 - 2010 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifndef ZSSNMPCltStdPCStoragesModel_h
#define ZSSNMPCltStdPCStoragesModel_h

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qdatetime.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPC.h"

namespace ZS
{
namespace SNMP
{
class CCltStdPC;

//******************************************************************************
struct ZSSNMPDLL_API SStorageValues
//******************************************************************************
{
public: // default ctor
    SStorageValues() :
        m_dateTimeUpdate(),
        m_iInstNr(-1),
        m_oidType(),
        m_strTypeValue(),
        m_storageType(EStorageTypeUndefined),
        m_oidDescr(),
        m_strDescrValue(),
        m_oidAllocationUnits(),
        m_iAllocationUnits_byte(0),
        m_oidSize(0),
        m_iSize_AllocationUnits(0),
        m_oidUsed(),
        m_iUsed_AllocationUnits(0)
    {
    }
public: // struct members
    QDateTime    m_dateTimeUpdate;
    int          m_iInstNr;
    COid         m_oidType;
    QString      m_strTypeValue;
    EStorageType m_storageType;
    COid         m_oidDescr;
    QString      m_strDescrValue;
    COid         m_oidAllocationUnits;
    int          m_iAllocationUnits_byte;
    COid         m_oidSize;
    int          m_iSize_AllocationUnits;
    COid         m_oidUsed;
    int          m_iUsed_AllocationUnits;

}; // struct SStorageValues

//******************************************************************************
class ZSSNMPDLL_API CModelStorages : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnUpdateTime           =  0,
        EColumnInstNr               =  1,
        EColumnTypeOid              =  2,
        EColumnTypeValue            =  3,
        EColumnStorageType          =  4,
        EColumnDescrOid             =  5,
        EColumnDescrValue           =  6,
        EColumnAllocationUnitsOid   =  7,
        EColumnAllocationUnitsValue =  8,
        EColumnSizeOid              =  9,
        EColumnSizeValue            = 10,
        EColumnUsedOid              = 11,
        EColumnUsedValue            = 12,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelStorages( CCltStdPC* i_pCltStdPC );
    virtual ~CModelStorages();
public: // instance methods
    void clear();
    int addValues( SStorageValues* i_pValues, bool i_bTakeOwnerShip = true );
    void setValues( int i_iRow, SStorageValues* i_pValues, bool i_bTakeOwnerShip = true );
    void setValue( int i_iRow, const QDateTime& i_dateTime );
    void setValue( int i_iRow, int i_iClm, const COid& i_oid );
    void setValue( int i_iRow, int i_iClm, const QString& i_strValue );
    void setValue( int i_iRow, int i_iClm, int i_iValue );
    SStorageValues* getValues( int i_iRow );
    QDateTime getValueDateTime( int i_iRow ) const;
    COid* getValueOid( int i_iRow, int i_iClm );
    QString getValueStr( int i_iRow, int i_iClm ) const;
    int getValueInt( int i_iRow, int i_iClm ) const;
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelStorages( const CModelStorages& );
private: // assignment operator not allowed
    void operator = ( const CModelStorages& );
protected: // instance members
    CCltStdPC*             m_pCltStdPC;
    QList<SStorageValues*> m_ptrListValues;

}; // class CModelStorages

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPCltStdPCStoragesModel_h
