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

#ifndef ZSSNMPCltStdPCProcessorsModel_h
#define ZSSNMPCltStdPCProcessorsModel_h

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qdatetime.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPOid.h"

namespace ZS
{
namespace SNMP
{
class CCltStdPC;

//******************************************************************************
struct ZSSNMPDLL_API SProcessorValues
//******************************************************************************
{
public: // default ctor
    SProcessorValues() :
        m_dateTimeUpdate(),
        m_iInstNr(-1),
        m_oidLoad(),
        m_iLoad_PerCent(0)
    {
    }
public: // struct members
    QDateTime m_dateTimeUpdate;
    int       m_iInstNr;
    COid      m_oidLoad;
    int       m_iLoad_PerCent;

}; // struct SProcessorValues

//******************************************************************************
class ZSSNMPDLL_API CModelProcessors : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EColumnUpdateTime = 0,
        EColumnInstNr     = 1,
        EColumnLoadOid    = 2,
        EColumnLoadValue  = 3,
        EColumnCount
    }   EColumn;
public: // ctors and dtor
    CModelProcessors( CCltStdPC* i_pCltStdPC );
    virtual ~CModelProcessors();
public: // instance methods
    void clear();
    int addValues( SProcessorValues* i_pValues, bool i_bTakeOwnerShip = true );
    void setValues( int i_iRow, SProcessorValues* i_pValues, bool i_bTakeOwnerShip = true );
    void setValue( int i_iRow, const QDateTime& i_dateTime );
    void setValue( int i_iRow, int i_iClm, const COid& i_oid );
    void setValue( int i_iRow, int i_iClm, int i_iValue );
    SProcessorValues* getValues( int i_iRow );
    QDateTime getValueDateTime( int i_iRow ) const;
    COid* getValueOid( int i_iRow, int i_iClm );
    int getValueInt( int i_iRow, int i_iClm ) const;
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelProcessors( const CModelProcessors& );
private: // assignment operator not allowed
    void operator = ( const CModelProcessors& );
protected: // instance members
    CCltStdPC*               m_pCltStdPC;
    QList<SProcessorValues*> m_ptrListValues;

}; // class CModelProcessors

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPCltStdPCProcessorsModel_h
