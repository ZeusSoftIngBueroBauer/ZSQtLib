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

#ifndef ZSApps_PCResourceMonitor_MemoryModel_h
#define ZSApps_PCResourceMonitor_MemoryModel_h

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qdatetime.h>

#include "ZSPhysVal/ZSPhysValArr.h"

namespace ZS
{
namespace Apps
{
namespace Products
{
namespace PCResourceMonitor
{
//******************************************************************************
class CModelMemory : public QAbstractTableModel
//******************************************************************************
{
    //-------------------------------------------------
    // !! The stores the total size of the memories. !!
    //---------------------------------------------------------------------
    Q_OBJECT
public: // type definitions and constants
    typedef enum
    {
        EClmPhysicalMemorySizeUpdSysTime =  0,
        EClmPhysicalMemorySize           =  1,
        EClmVirtualMemorySizeUpdSysTime  =  2,
        EClmVirtualMemorySize            =  3,
        EClmCount
    }   EClm;
public: // ctors and dtor
    CModelMemory();
    virtual ~CModelMemory();
public: // instance methods
    void setUnit( ZS::PhysVal::CUnitDataQuantity* i_pUnit );
    // Physical memory
    QColor getColorPhysicalMemory() const;
    void setPhysicalMemorySizeInB( double i_fSize_B, double i_fSysTime_ms = 0.0 );
    double getPhysicalMemorySizeInB() const;
    ZS::PhysVal::CPhysVal getPhysicalMemorySize() const;
    double getPhysicalMemorySizeUpdSysTimeInMs() const;
    // Virtual memory
    QColor getColorVirtualMemory() const;
    void setVirtualMemorySizeInB( double i_fSize_B, double i_fSysTime_ms = 0.0 );
    double getVirtualMemorySizeInB() const;
    ZS::PhysVal::CPhysVal getVirtualMemorySize() const;
    double getVirtualMemorySizeUpdSysTimeInMs() const;
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelMemory( const CModelMemory& );
private: // assignment operator not allowed
    void operator = ( const CModelMemory& );
protected: // instance members
    double                 m_fPhysicalMemorySizeUpdSysTime_ms;
    ZS::PhysVal::CPhysVal  m_physValPhysicalMemorySize;
    double                 m_fVirtualMemorySizeUpdSysTime_ms;
    ZS::PhysVal::CPhysVal  m_physValVirtualMemorySize;

}; // class CModelMemory

//******************************************************************************
class CModelMemoryValues : public QAbstractTableModel
//******************************************************************************
{
    //---------------------------------------------------------------------
    // !! This model is used to indicate the history of the used space.  !!
    // !! Virtual and physical memory in separated columns.              !!
    // !! The values are indicated in rows.                              !!
    //---------------------------------------------------------------------
    Q_OBJECT
    typedef enum
    {
        EClmPhysicalMemoryFreeSizeUpdSysTime =  0,
        EClmPhysicalMemoryFreeSize           =  1,
        EClmPhysicalMemoryUsedSize           =  2,
        EClmVirtualMemoryFreeSizeUpdSysTime  =  3,
        EClmVirtualMemoryFreeSize            =  4,
        EClmVirtualMemoryUsedSize            =  5,
        EClmCount
    }   EClm;
public: // ctors and dtor
    CModelMemoryValues( CModelMemory* i_pModelMemory, unsigned int i_uMaxValCount = 600, unsigned int i_uMaxArrLen = 700 );
    virtual ~CModelMemoryValues();
public: // instance methods
    void showAllValues( bool i_bShowAll );
    unsigned int getMaxValCount() const;
    unsigned int getMaxArrLen() const;
    void setUnit( ZS::PhysVal::CUnitDataQuantity* i_pUnit );
    // Physical memory
    void setPhysicalMemoryFreeSizeInB( unsigned int i_udxVal, double i_fSize_B, double i_fSysTime_ms = 0.0 );
    void appendPhysicalMemoryFreeSizeInB( double i_fSize_B, double i_fSysTime_ms = 0.0 );
    unsigned int getPhysicalMemoryValueCount() const;
    ZS::PhysVal::CPhysVal getPhysicalMemoryUpdSysTime( unsigned int i_udxVal ) const;
    ZS::PhysVal::CPhysVal getPhysicalMemoryFreeSize( unsigned int i_udxVal ) const;
    double getPhysicalMemoryFreeSizeInB( unsigned int i_udxVal ) const;
    ZS::PhysVal::CPhysVal getPhysicalMemoryUsedSize( unsigned int i_udxVal ) const;
    double getPhysicalMemoryUsedSizeInB( unsigned int i_udxVal ) const;
    ZS::PhysVal::CPhysVal getFirstPhysicalMemoryUpdSysTime() const; // first time value has been updated
    ZS::PhysVal::CPhysVal getFirstPhysicalMemoryFreeSize() const;   // first value (the "oldest" value)
    double getFirstPhysicalMemoryFreeSizeInB() const;               // first value (the "oldest" value)
    ZS::PhysVal::CPhysVal getFirstPhysicalMemoryUsedSize() const;   // first value (the "oldest" value)
    double getFirstPhysicalMemoryUsedSizeInB() const;               // first value (the "oldest" value)
    ZS::PhysVal::CPhysVal getLastPhysicalMemoryUpdSysTime() const;  // last time value has been updated
    ZS::PhysVal::CPhysVal getLastPhysicalMemoryFreeSize() const;    // last value (the "newest" value)
    double getLastPhysicalMemoryFreeSizeInB() const;                // last value (the "newest" value)
    ZS::PhysVal::CPhysVal getLastPhysicalMemoryUsedSize() const;    // last value (the "newest" value)
    double getLastPhysicalMemoryUsedSizeInB() const;                // last value (the "newest" value)
    QVector<double> getPhysicalMemoryFreeSizesInBytes() const;
    QVector<double> getPhysicalMemoryUsedSizesInBytes() const;
    QVector<double> getPhysicalMemoryUpdSysTimesInMs() const;
    // Virtual memory
    void setVirtualMemoryFreeSizeInB( unsigned int i_udxVal, double i_fSize_B, double i_fSysTime_ms = 0.0 );
    void appendVirtualMemoryFreeSizeInB( double i_fSize_B, double i_fSysTime_ms = 0.0 );
    unsigned int getVirtualMemoryValueCount() const;
    ZS::PhysVal::CPhysVal getVirtualMemoryUpdSysTime( unsigned int i_udxVal ) const;
    ZS::PhysVal::CPhysVal getVirtualMemoryFreeSize( unsigned int i_udxVal ) const;
    double getVirtualMemoryFreeSizeInB( unsigned int i_udxVal ) const;
    ZS::PhysVal::CPhysVal getVirtualMemoryUsedSize( unsigned int i_udxVal ) const;
    double getVirtualMemoryUsedSizeInB( unsigned int i_udxVal ) const;
    ZS::PhysVal::CPhysVal getFirstVirtualMemoryUpdSysTime() const;  // first time value has been updated
    ZS::PhysVal::CPhysVal getFirstVirtualMemoryFreeSize() const;    // first value (the "oldest" value)
    double getFirstVirtualMemoryFreeSizeInB() const;                // first value (the "oldest" value)
    ZS::PhysVal::CPhysVal getFirstVirtualMemoryUsedSize() const;    // first value (the "oldest" value)
    double getFirstVirtualMemoryUsedSizeInB() const;                // first value (the "oldest" value)
    ZS::PhysVal::CPhysVal getLastVirtualMemoryUpdSysTime() const;   // last time value has been updated
    ZS::PhysVal::CPhysVal getLastVirtualMemoryFreeSize() const;     // last value (the "newest" value)
    double getLastVirtualMemoryFreeSizeInB() const;                 // last value (the "newest" value)
    ZS::PhysVal::CPhysVal getLastVirtualMemoryUsedSize() const;     // last value (the "newest" value)
    double getLastVirtualMemoryUsedSizeInB() const;                 // last value (the "newest" value)
    QVector<double> getVirtualMemoryFreeSizesInBytes() const;
    QVector<double> getVirtualMemoryUsedSizesInBytes() const;
    QVector<double> getVirtualMemoryUpdSysTimesInMs() const;
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
private: // copy ctor not allowed
    CModelMemoryValues( const CModelMemoryValues& );
private: // assignment operator not allowed
    void operator = ( const CModelMemoryValues& );
protected: // instance members
    CModelMemory*            m_pModelMemory;
    bool                     m_bShowAllValues;
    int                      m_iMaxValCount;
    int                      m_iMaxArrLen;
    ZS::PhysVal::CPhysValArr m_physValArrPhysicalMemoryUpdSysTime;
    ZS::PhysVal::CPhysValArr m_physValArrPhysicalMemoryFreeSize;
    ZS::PhysVal::CPhysValArr m_physValArrPhysicalMemoryUsedSize;
    ZS::PhysVal::CPhysValArr m_physValArrVirtualMemoryUpdSysTime;
    ZS::PhysVal::CPhysValArr m_physValArrVirtualMemoryFreeSize;
    ZS::PhysVal::CPhysValArr m_physValArrVirtualMemoryUsedSize;

}; // class CModelMemoryValues

} // namespace PCResourceMonitor

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_PCResourceMonitor_MemoryModel_h
