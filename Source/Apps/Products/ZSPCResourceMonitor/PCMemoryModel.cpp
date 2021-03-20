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

*******************************************************************************/

#include <QtCore/qdatetime.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qthread.h>
#include <QtXml/qdom.h>

#include <QtGui/qcolor.h>

#include "PCMemoryModel.h"

#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizes.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizeTime.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::PCResourceMonitor;


/*******************************************************************************
class CModelMemory : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelMemory::CModelMemory() :
//------------------------------------------------------------------------------
    QAbstractTableModel(),
    m_fPhysicalMemorySizeUpdSysTime_ms(0.0),
    m_physValPhysicalMemorySize(0.0,DataQuantity()->Byte()),
    m_fVirtualMemorySizeUpdSysTime_ms(0.0),
    m_physValVirtualMemorySize(0.0,DataQuantity()->Byte())
{
} // ctor

//------------------------------------------------------------------------------
CModelMemory::~CModelMemory()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelMemory::setUnit( ZS::PhysVal::CUnitDataQuantity* i_pUnit )
//------------------------------------------------------------------------------
{
    m_physValVirtualMemorySize.convertValue(i_pUnit);
    m_physValVirtualMemorySize.setRes(1.0,i_pUnit);
    m_physValPhysicalMemorySize.convertValue(i_pUnit);
    m_physValPhysicalMemorySize.setRes(1.0,i_pUnit);

    beginResetModel();
    endResetModel();

} // setUnit

/*==============================================================================
// Physical memory
==============================================================================*/

//------------------------------------------------------------------------------
QColor CModelMemory::getColorPhysicalMemory() const
//------------------------------------------------------------------------------
{
    QColor col;
    int    iHue = 50;
    int    iSat = 255;
    int    iVal = 191;
    col.setHsv(iHue,iSat,iVal,255);
    return col;
}

//------------------------------------------------------------------------------
void CModelMemory::setPhysicalMemorySizeInB( double i_fSize_B, double i_fSysTime_ms )
//------------------------------------------------------------------------------
{
    CUnitDataQuantity* pUnitDataQuantity = m_physValPhysicalMemorySize.getUnitDataQuantity();

    if( m_physValPhysicalMemorySize.getVal(DataQuantity()->Byte()) != i_fSize_B )
    {
        m_physValPhysicalMemorySize = CPhysVal(i_fSize_B,DataQuantity()->Byte(),1.0);
        m_physValPhysicalMemorySize.convertValue(pUnitDataQuantity);

        if( i_fSysTime_ms <= 0.0 )
        {
            m_fPhysicalMemorySizeUpdSysTime_ms = ZS::System::Time::getProcTimeInMilliSec();
        }
        else
        {
            m_fPhysicalMemorySizeUpdSysTime_ms = i_fSysTime_ms;
        }
        emit dataChanged( index(0,EClmPhysicalMemorySize), index(0,EClmPhysicalMemorySizeUpdSysTime) );
    }

} // setPhysicalMemorySizeInB

//------------------------------------------------------------------------------
CPhysVal CModelMemory::getPhysicalMemorySize() const
//------------------------------------------------------------------------------
{
    return m_physValPhysicalMemorySize;
}

//------------------------------------------------------------------------------
double CModelMemory::getPhysicalMemorySizeInB() const
//------------------------------------------------------------------------------
{
    return m_physValPhysicalMemorySize.getVal(DataQuantity()->Byte());
}

//------------------------------------------------------------------------------
double CModelMemory::getPhysicalMemorySizeUpdSysTimeInMs() const
//------------------------------------------------------------------------------
{
    return m_fPhysicalMemorySizeUpdSysTime_ms;
}

/*==============================================================================
Virtual memory
==============================================================================*/

//------------------------------------------------------------------------------
QColor CModelMemory::getColorVirtualMemory() const
//------------------------------------------------------------------------------
{
    QColor col;
    int    iHue = 100;
    int    iSat = 255;
    int    iVal = 191;
    col.setHsv(iHue,iSat,iVal,255);
    return col;
}

//------------------------------------------------------------------------------
void CModelMemory::setVirtualMemorySizeInB( double i_fSize_B, double i_fSysTime_ms )
//------------------------------------------------------------------------------
{
    CUnitDataQuantity* pUnitDataQuantity = m_physValVirtualMemorySize.getUnitDataQuantity();

    if( m_physValVirtualMemorySize.getVal(DataQuantity()->Byte()) != i_fSize_B )
    {
        m_physValVirtualMemorySize = CPhysVal(i_fSize_B,DataQuantity()->Byte(),1.0);
        m_physValVirtualMemorySize.convertValue(pUnitDataQuantity);

        if( i_fSysTime_ms <= 0.0 )
        {
            m_fVirtualMemorySizeUpdSysTime_ms = ZS::System::Time::getProcTimeInMilliSec();
        }
        else
        {
            m_fVirtualMemorySizeUpdSysTime_ms = i_fSysTime_ms;
        }
        emit dataChanged( index(0,EClmVirtualMemorySize), index(0,EClmVirtualMemorySizeUpdSysTime) );
    }

} // setVirtualMemorySizeInB

//------------------------------------------------------------------------------
CPhysVal CModelMemory::getVirtualMemorySize() const
//------------------------------------------------------------------------------
{
    return m_physValVirtualMemorySize;
}

//------------------------------------------------------------------------------
double CModelMemory::getVirtualMemorySizeInB() const
//------------------------------------------------------------------------------
{
    return m_physValVirtualMemorySize.getVal(DataQuantity()->Byte());
}

//------------------------------------------------------------------------------
double CModelMemory::getVirtualMemorySizeUpdSysTimeInMs() const
//------------------------------------------------------------------------------
{
    return m_fVirtualMemorySizeUpdSysTime_ms;
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelMemory::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( !i_modelIdxParent.isValid() )
    {
        iRowCount = 1;
    }
    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelMemory::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EClmCount;
}

//------------------------------------------------------------------------------
QVariant CModelMemory::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    int idxClm = i_modelIdx.column();
    int idxVal = i_modelIdx.row();

    switch( idxClm )
    {
        case EClmPhysicalMemorySizeUpdSysTime:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                varData = ZS::System::Time::sysTimeInMilliSec2QTime(m_fPhysicalMemorySizeUpdSysTime_ms).toString("hh:mm:ss:zzz");
            }
            break;
        }
        case EClmPhysicalMemorySize:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                varData = QString::number(m_physValPhysicalMemorySize.getVal(),10,0);
            }
            break;
        }
        case EClmVirtualMemorySizeUpdSysTime:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                varData = ZS::System::Time::sysTimeInMilliSec2QTime(m_fVirtualMemorySizeUpdSysTime_ms).toString("hh:mm:ss:zzz");
            }
            break;
        }
        case EClmVirtualMemorySize:
        {
            if( i_iRole == Qt::DisplayRole )
            {
                varData = QString::number(m_physValVirtualMemorySize.getVal(),10,0);
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return varData;

} // data

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelMemory::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        switch( i_iSection )
        {
            case EClmPhysicalMemorySizeUpdSysTime:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Physical Memory\nUpdate Time";
                }
                else if( i_iRole == Qt::TextColorRole )
                {
                    varData = getColorPhysicalMemory();
                }
                break;
            }
            case EClmPhysicalMemorySize:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Physical Memory\nSize/" + m_physValPhysicalMemorySize.getUnit()->getSymbol();
                }
                else if( i_iRole == Qt::TextColorRole )
                {
                    varData = getColorPhysicalMemory();
                }
                break;
            }
            case EClmVirtualMemorySizeUpdSysTime:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Virtual Memory\nUpdate Time";
                }
                else if( i_iRole == Qt::TextColorRole )
                {
                    varData = getColorVirtualMemory();
                }
                break;
            }
            case EClmVirtualMemorySize:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Virtual Memory\nSize/" + m_physValVirtualMemorySize.getUnit()->getSymbol();
                }
                else if( i_iRole == Qt::TextColorRole )
                {
                    varData = getColorVirtualMemory();
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else if( i_orientation == Qt::Vertical )
    {
        if( i_iRole == Qt::DisplayRole )
        {
            varData = QString::number(i_iSection);
        }
    }
    return varData;

} // headerData


/*******************************************************************************
class CModelMemoryValues : public QAbstractTableModel
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CModelMemoryValues::CModelMemoryValues( CModelMemory* i_pModelMemory, unsigned int i_uMaxValCount, unsigned int i_uMaxArrLen ) :
//------------------------------------------------------------------------------
    QAbstractTableModel(),
    m_pModelMemory(i_pModelMemory),
    m_bShowAllValues(false),
    m_iMaxValCount(i_uMaxValCount),
    m_iMaxArrLen(i_uMaxArrLen),
    m_physValArrPhysicalMemoryUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_physValArrPhysicalMemoryFreeSize(DataQuantity()->Byte(),1.0),
    m_physValArrPhysicalMemoryUsedSize(DataQuantity()->Byte(),1.0),
    m_physValArrVirtualMemoryUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_physValArrVirtualMemoryFreeSize(DataQuantity()->Byte(),1.0),
    m_physValArrVirtualMemoryUsedSize(DataQuantity()->Byte(),1.0)
{
} // ctor

//------------------------------------------------------------------------------
CModelMemoryValues::~CModelMemoryValues()
//------------------------------------------------------------------------------
{
    unsigned int uRowCountOld = rowCount();

    m_physValArrPhysicalMemoryUpdSysTime.clear();
    m_physValArrPhysicalMemoryFreeSize.clear();
    m_physValArrPhysicalMemoryUsedSize.clear();
    m_physValArrVirtualMemoryUpdSysTime.clear();
    m_physValArrVirtualMemoryFreeSize.clear();
    m_physValArrVirtualMemoryUsedSize.clear();

    if( uRowCountOld > 0 )
    {
        beginRemoveRows(QModelIndex(),0,uRowCountOld-1);
        endRemoveRows();
    }

    m_pModelMemory = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CModelMemoryValues::setUnit( ZS::PhysVal::CUnitDataQuantity* i_pUnit )
//------------------------------------------------------------------------------
{
    m_physValArrPhysicalMemoryFreeSize.convertValues(i_pUnit);
    m_physValArrPhysicalMemoryFreeSize.setRes(1.0,i_pUnit);
    m_physValArrPhysicalMemoryUsedSize.convertValues(i_pUnit);
    m_physValArrPhysicalMemoryUsedSize.setRes(1.0,i_pUnit);
    m_physValArrVirtualMemoryFreeSize.convertValues(i_pUnit);
    m_physValArrVirtualMemoryFreeSize.setRes(1.0,i_pUnit);
    m_physValArrVirtualMemoryUsedSize.convertValues(i_pUnit);
    m_physValArrVirtualMemoryUsedSize.setRes(1.0,i_pUnit);

    beginResetModel();
    endResetModel();

} // setUnit

//------------------------------------------------------------------------------
void CModelMemoryValues::showAllValues( bool i_bShowAll )
//------------------------------------------------------------------------------
{
    if( m_bShowAllValues != i_bShowAll )
    {
        int iRowCountOld = rowCount();
        int iRowCountNew = iRowCountOld;
        int iRowIdxBeg;
        int iRowIdxEnd;

        m_bShowAllValues = i_bShowAll;

        iRowCountNew = rowCount();

        if( iRowCountNew > iRowCountOld )
        {
            iRowIdxBeg = iRowCountOld;
            iRowIdxEnd = iRowCountNew;

            if( iRowCountNew > 0 )
            {
                iRowIdxEnd = iRowCountNew - 1;
            }
            beginInsertRows( QModelIndex(), iRowIdxBeg, iRowIdxEnd );
            endInsertRows();
        }
        else if( iRowCountNew < iRowCountOld )
        {
            iRowIdxBeg = iRowCountNew;
            iRowIdxEnd = iRowCountOld;

            if( iRowCountOld > 0 )
            {
                iRowIdxEnd = iRowCountOld - 1;
            }
            beginRemoveRows( QModelIndex(), iRowIdxBeg, iRowIdxEnd );
            endRemoveRows();
        }
    }

} // showAllValues

//------------------------------------------------------------------------------
unsigned int CModelMemoryValues::getMaxValCount() const
//------------------------------------------------------------------------------
{
    return m_iMaxValCount;
}

//------------------------------------------------------------------------------
unsigned int CModelMemoryValues::getMaxArrLen() const
//------------------------------------------------------------------------------
{
    return m_iMaxArrLen;
}

/*==============================================================================
// Physical memory
==============================================================================*/

//------------------------------------------------------------------------------
void CModelMemoryValues::setPhysicalMemoryFreeSizeInB( unsigned int i_udxVal, double i_fSize_B, double i_fSysTime_ms )
//------------------------------------------------------------------------------
{
    if( i_udxVal < static_cast<unsigned int>(m_physValArrPhysicalMemoryUsedSize.size()) )
    {
        double fAvailableSizeInB = m_pModelMemory->getPhysicalMemorySizeInB();

        m_physValArrPhysicalMemoryFreeSize.setVal(i_udxVal,i_fSize_B,DataQuantity()->Byte());
        m_physValArrPhysicalMemoryUsedSize.setVal(i_udxVal,fAvailableSizeInB-i_fSize_B,DataQuantity()->Byte());

        if( i_fSysTime_ms <= 0.0 )
        {
            m_physValArrPhysicalMemoryUpdSysTime.setVal(i_udxVal,ZS::System::Time::getProcTimeInMilliSec());
        }
        else
        {
            m_physValArrPhysicalMemoryUpdSysTime.setVal(i_udxVal,i_fSysTime_ms);
        }
        emit dataChanged( index(i_udxVal,EClmPhysicalMemoryFreeSize), index(i_udxVal,EClmPhysicalMemoryFreeSizeUpdSysTime) );
    }

} // setPhysicalMemoryFreeSizeInB

//------------------------------------------------------------------------------
void CModelMemoryValues::appendPhysicalMemoryFreeSizeInB( double i_fSize_B, double i_fSysTime_ms )
//------------------------------------------------------------------------------
{
    int iRowCountOld = rowCount();
    int iRowCountNew = iRowCountOld;
    int iRowIdxBeg;
    int iRowIdxEnd;

    if( m_iMaxArrLen > 0 && m_iMaxValCount > 0 && m_iMaxArrLen > m_iMaxValCount && m_physValArrPhysicalMemoryUsedSize.size() >= m_iMaxArrLen )
    {
        QVector<double> arfFreeSize   = m_physValArrPhysicalMemoryFreeSize.toDoubleVec( m_iMaxArrLen-m_iMaxValCount, m_iMaxValCount );
        QVector<double> arfUsedSize   = m_physValArrPhysicalMemoryUsedSize.toDoubleVec( m_iMaxArrLen-m_iMaxValCount, m_iMaxValCount );
        QVector<double> arfUpdSysTime = m_physValArrPhysicalMemoryUpdSysTime.toDoubleVec( m_iMaxArrLen-m_iMaxValCount, m_iMaxValCount );

        m_physValArrPhysicalMemoryFreeSize.clear();
        m_physValArrPhysicalMemoryUsedSize.clear();
        m_physValArrPhysicalMemoryUpdSysTime.clear();

        m_physValArrPhysicalMemoryFreeSize   = arfFreeSize;
        m_physValArrPhysicalMemoryUsedSize   = arfUsedSize;
        m_physValArrPhysicalMemoryUpdSysTime = arfUpdSysTime;

        iRowCountNew = rowCount();

        if( iRowCountNew < iRowCountOld )
        {
            iRowIdxBeg = iRowCountNew;
            iRowIdxEnd = iRowCountOld;

            if( iRowCountOld > 0 )
            {
                iRowIdxEnd = iRowCountOld - 1;
            }

            beginRemoveRows( QModelIndex(), iRowIdxBeg, iRowIdxEnd );
            endRemoveRows();

            iRowCountOld = rowCount();
        }
    }

    double fAvailableSizeInB = m_pModelMemory->getPhysicalMemorySizeInB();

    m_physValArrPhysicalMemoryFreeSize.appendVal(i_fSize_B,DataQuantity()->Byte());
    m_physValArrPhysicalMemoryUsedSize.appendVal(fAvailableSizeInB-i_fSize_B,DataQuantity()->Byte());

    if( i_fSysTime_ms <= 0.0 )
    {
        m_physValArrPhysicalMemoryUpdSysTime.appendVal(ZS::System::Time::getProcTimeInMilliSec());
    }
    else
    {
        m_physValArrPhysicalMemoryUpdSysTime.appendVal(i_fSysTime_ms);
    }

    iRowCountNew = rowCount();

    if( iRowCountNew > iRowCountOld )
    {
        iRowIdxBeg = iRowCountOld;
        iRowIdxEnd = iRowCountNew;

        if( iRowCountNew > 0 )
        {
            iRowIdxEnd = iRowCountNew - 1;
        }
        beginInsertRows( QModelIndex(), iRowIdxBeg, iRowIdxEnd );
        endInsertRows();
    }

    if( !m_bShowAllValues )
    {
        emit dataChanged( index(0,EClmPhysicalMemoryFreeSize), index(0,EClmPhysicalMemoryFreeSizeUpdSysTime) );
    }

} // appendPhysicalMemoryFreeSizeInB

//------------------------------------------------------------------------------
unsigned int CModelMemoryValues::getPhysicalMemoryValueCount() const
//------------------------------------------------------------------------------
{
    return m_physValArrPhysicalMemoryUpdSysTime.size();
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getPhysicalMemoryUpdSysTime( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    ZS::PhysVal::CPhysVal physValTime(m_physValArrPhysicalMemoryUpdSysTime.getUnit());

    if( i_udxVal < static_cast<unsigned int>(m_physValArrPhysicalMemoryUpdSysTime.size()) )
    {
        physValTime = m_physValArrPhysicalMemoryUpdSysTime.getPhysVal(i_udxVal);
    }
    return physValTime;

} // getPhysicalMemoryUpdSysTime

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getPhysicalMemoryFreeSize( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrPhysicalMemoryFreeSize.getUnit());

    if( i_udxVal < static_cast<unsigned int>(m_physValArrPhysicalMemoryFreeSize.size()) )
    {
        physVal = m_physValArrPhysicalMemoryFreeSize.getPhysVal(i_udxVal);
    }
    return physVal;

} // getPhysicalMemoryFreeSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getPhysicalMemoryFreeSizeInB( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( i_udxVal < static_cast<unsigned int>(m_physValArrPhysicalMemoryFreeSize.size()) )
    {
        fSize_byte = m_physValArrPhysicalMemoryFreeSize.getPhysVal(i_udxVal).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getPhysicalMemoryFreeSizeInB

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getPhysicalMemoryUsedSize( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrPhysicalMemoryUsedSize.getUnit());

    if( i_udxVal < static_cast<unsigned int>(m_physValArrPhysicalMemoryUsedSize.size()) )
    {
        physVal = m_physValArrPhysicalMemoryUsedSize.getPhysVal(i_udxVal);
    }
    return physVal;

} // getPhysicalMemoryUsedSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getPhysicalMemoryUsedSizeInB( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( i_udxVal < static_cast<unsigned int>(m_physValArrPhysicalMemoryUsedSize.size()) )
    {
        fSize_byte = m_physValArrPhysicalMemoryUsedSize.getPhysVal(i_udxVal).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getPhysicalMemoryUsedSizeInB

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getFirstPhysicalMemoryUpdSysTime() const
//------------------------------------------------------------------------------
{
    ZS::PhysVal::CPhysVal physValTime(m_physValArrPhysicalMemoryUpdSysTime.getUnit());

    if( m_physValArrPhysicalMemoryUpdSysTime.size() > 0 )
    {
        physValTime = m_physValArrPhysicalMemoryUpdSysTime.getPhysVal(0);
    }
    return physValTime;

} // getFirstPhysicalMemoryUpdSysTime

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getFirstPhysicalMemoryFreeSize() const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrPhysicalMemoryFreeSize.getUnit());

    if( m_physValArrPhysicalMemoryFreeSize.size() > 0 )
    {
        physVal = m_physValArrPhysicalMemoryFreeSize.getPhysVal(0);
    }
    return physVal;

} // getFirstPhysicalMemoryFreeSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getFirstPhysicalMemoryFreeSizeInB() const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( m_physValArrPhysicalMemoryFreeSize.size() > 0 )
    {
        fSize_byte = m_physValArrPhysicalMemoryFreeSize.getPhysVal(0).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getFirstPhysicalMemoryFreeSizeInB

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getLastPhysicalMemoryUpdSysTime() const
//------------------------------------------------------------------------------
{
    ZS::PhysVal::CPhysVal physValTime(m_physValArrPhysicalMemoryUpdSysTime.getUnit());

    if( m_physValArrPhysicalMemoryUpdSysTime.size() > 0 )
    {
        unsigned int udxVal = m_physValArrPhysicalMemoryUpdSysTime.size()-1;
        physValTime = m_physValArrPhysicalMemoryUpdSysTime.getPhysVal(udxVal);
    }
    return physValTime;

} // getLastPhysicalMemoryUpdSysTimeInMs

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getLastPhysicalMemoryFreeSize() const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrPhysicalMemoryFreeSize.getUnit());

    if( m_physValArrPhysicalMemoryFreeSize.size() > 0 )
    {
        unsigned int udxVal = m_physValArrPhysicalMemoryFreeSize.size()-1;
        physVal = m_physValArrPhysicalMemoryFreeSize.getPhysVal(udxVal);
    }
    return physVal;

} // getFirstPhysicalMemoryFreeSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getLastPhysicalMemoryFreeSizeInB() const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( m_physValArrPhysicalMemoryFreeSize.size() > 0 )
    {
        unsigned int udxVal = m_physValArrPhysicalMemoryFreeSize.size()-1;
        fSize_byte = m_physValArrPhysicalMemoryFreeSize.getPhysVal(udxVal).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getFirstPhysicalMemoryFreeSizeInB

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getLastPhysicalMemoryUsedSize() const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrPhysicalMemoryUsedSize.getUnit());

    if( m_physValArrPhysicalMemoryUsedSize.size() > 0 )
    {
        unsigned int udxVal = m_physValArrPhysicalMemoryUsedSize.size()-1;
        physVal = m_physValArrPhysicalMemoryUsedSize.getPhysVal(udxVal);
    }
    return physVal;

} // getFirstPhysicalMemoryUsedSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getLastPhysicalMemoryUsedSizeInB() const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( m_physValArrPhysicalMemoryUsedSize.size() > 0 )
    {
        unsigned int udxVal = m_physValArrPhysicalMemoryUsedSize.size()-1;
        fSize_byte = m_physValArrPhysicalMemoryUsedSize.getPhysVal(udxVal).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getFirstPhysicalMemoryUsedSizeInB

//------------------------------------------------------------------------------
QVector<double> CModelMemoryValues::getPhysicalMemoryFreeSizesInBytes() const
//------------------------------------------------------------------------------
{
    return m_physValArrPhysicalMemoryFreeSize.toDoubleVec( DataQuantity()->Byte() );
}

//------------------------------------------------------------------------------
QVector<double> CModelMemoryValues::getPhysicalMemoryUsedSizesInBytes() const
//------------------------------------------------------------------------------
{
    return m_physValArrPhysicalMemoryUsedSize.toDoubleVec( DataQuantity()->Byte() );
}

//------------------------------------------------------------------------------
QVector<double> CModelMemoryValues::getPhysicalMemoryUpdSysTimesInMs() const
//------------------------------------------------------------------------------
{
    return m_physValArrPhysicalMemoryUpdSysTime.toDoubleVec( Kinematics::Time()->MilliSeconds() );
}

/*==============================================================================
Virtual memory
==============================================================================*/

//------------------------------------------------------------------------------
void CModelMemoryValues::setVirtualMemoryFreeSizeInB( unsigned int i_udxVal, double i_fSize_B, double i_fSysTime_ms )
//------------------------------------------------------------------------------
{
    if( i_udxVal < static_cast<unsigned int>(m_physValArrVirtualMemoryUsedSize.size()) )
    {
        double fAvailableSizeInB = m_pModelMemory->getVirtualMemorySizeInB();

        if( fAvailableSizeInB > 0.0 )
        {
            m_physValArrVirtualMemoryFreeSize.setVal(i_udxVal,i_fSize_B,DataQuantity()->Byte());
            m_physValArrVirtualMemoryUsedSize.setVal(i_udxVal,fAvailableSizeInB-i_fSize_B,DataQuantity()->Byte());
        }
        else
        {
            CPhysVal physVal(DataQuantity()->Byte());
            m_physValArrVirtualMemoryFreeSize.setVal(i_udxVal,physVal);
            m_physValArrVirtualMemoryUsedSize.setVal(i_udxVal,physVal);
        }
        if( i_fSysTime_ms <= 0.0 )
        {
            m_physValArrVirtualMemoryUpdSysTime.setVal(i_udxVal,ZS::System::Time::getProcTimeInMilliSec());
        }
        else
        {
            m_physValArrVirtualMemoryUpdSysTime.appendVal(i_fSysTime_ms);
        }
        emit dataChanged( index(i_udxVal,EClmVirtualMemoryFreeSize), index(i_udxVal,EClmVirtualMemoryFreeSizeUpdSysTime) );
    }

} // setVirtualMemoryFreeSizeInB

//------------------------------------------------------------------------------
void CModelMemoryValues::appendVirtualMemoryFreeSizeInB( double i_fSize_B, double i_fSysTime_ms )
//------------------------------------------------------------------------------
{
    int iRowCountOld = rowCount();
    int iRowCountNew = iRowCountOld;
    int iRowIdxBeg;
    int iRowIdxEnd;

    if( m_iMaxArrLen > 0 && m_iMaxValCount > 0 && m_iMaxArrLen > m_iMaxValCount && m_physValArrVirtualMemoryUsedSize.size() >= m_iMaxArrLen )
    {
        QVector<double> arfFreeSize   = m_physValArrVirtualMemoryFreeSize.toDoubleVec( m_iMaxArrLen-m_iMaxValCount, m_iMaxValCount );
        QVector<double> arfUsedSize   = m_physValArrVirtualMemoryUsedSize.toDoubleVec( m_iMaxArrLen-m_iMaxValCount, m_iMaxValCount );
        QVector<double> arfUpdSysTime = m_physValArrVirtualMemoryUpdSysTime.toDoubleVec( m_iMaxArrLen-m_iMaxValCount, m_iMaxValCount );

        m_physValArrVirtualMemoryFreeSize.clear();
        m_physValArrVirtualMemoryUsedSize.clear();
        m_physValArrVirtualMemoryUpdSysTime.clear();

        m_physValArrVirtualMemoryFreeSize   = arfFreeSize;
        m_physValArrVirtualMemoryUsedSize   = arfUsedSize;
        m_physValArrVirtualMemoryUpdSysTime = arfUpdSysTime;

        iRowCountNew = rowCount();

        if( iRowCountNew < iRowCountOld )
        {
            iRowIdxBeg = iRowCountNew;
            iRowIdxEnd = iRowCountOld;

            if( iRowCountOld > 0 )
            {
                iRowIdxEnd = iRowCountOld - 1;
            }

            beginRemoveRows( QModelIndex(), iRowIdxBeg, iRowIdxEnd );
            endRemoveRows();

            iRowCountOld = rowCount();
        }
    }

    double fAvailableSizeInB = m_pModelMemory->getVirtualMemorySizeInB();

    if( fAvailableSizeInB > 0.0 )
    {
        m_physValArrVirtualMemoryFreeSize.appendVal(i_fSize_B,DataQuantity()->Byte());
        m_physValArrVirtualMemoryUsedSize.appendVal(fAvailableSizeInB-i_fSize_B,DataQuantity()->Byte());
    }
    else
    {
        CPhysVal physVal(DataQuantity()->Byte());
        m_physValArrVirtualMemoryFreeSize.appendVal(physVal);
        m_physValArrVirtualMemoryUsedSize.appendVal(physVal);
    }
    if( i_fSysTime_ms <= 0.0 )
    {
        m_physValArrVirtualMemoryUpdSysTime.appendVal(ZS::System::Time::getProcTimeInMilliSec());
    }
    else
    {
        m_physValArrVirtualMemoryUpdSysTime.appendVal(i_fSysTime_ms);
    }

    iRowCountNew = rowCount();

    if( iRowCountNew > iRowCountOld )
    {
        iRowIdxBeg = iRowCountOld;
        iRowIdxEnd = iRowCountNew;

        if( iRowCountNew > 0 )
        {
            iRowIdxEnd = iRowCountNew - 1;
        }
        beginInsertRows( QModelIndex(), iRowIdxBeg, iRowIdxEnd );
        endInsertRows();
    }

    if( !m_bShowAllValues )
    {
        emit dataChanged( index(0,EClmVirtualMemoryFreeSize), index(0,EClmVirtualMemoryFreeSizeUpdSysTime) );
    }

} // appendVirtualMemoryFreeSizeInB

//------------------------------------------------------------------------------
unsigned int CModelMemoryValues::getVirtualMemoryValueCount() const
//------------------------------------------------------------------------------
{
    return m_physValArrVirtualMemoryUpdSysTime.size();
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getVirtualMemoryUpdSysTime( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    ZS::PhysVal::CPhysVal physValTime(m_physValArrVirtualMemoryUpdSysTime.getUnit());

    if( i_udxVal < static_cast<unsigned int>(m_physValArrVirtualMemoryUpdSysTime.size()) )
    {
        physValTime = m_physValArrVirtualMemoryUpdSysTime.getPhysVal(i_udxVal);
    }
    return physValTime;

} // getVirtualMemoryUpdSysTime

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getVirtualMemoryFreeSize( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrVirtualMemoryFreeSize.getUnit());

    if( i_udxVal < static_cast<unsigned int>(m_physValArrVirtualMemoryFreeSize.size()) )
    {
        physVal = m_physValArrVirtualMemoryFreeSize.getPhysVal(i_udxVal);
    }
    return physVal;

} // getVirtualMemoryFreeSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getVirtualMemoryFreeSizeInB( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( i_udxVal < static_cast<unsigned int>(m_physValArrVirtualMemoryFreeSize.size()) )
    {
        fSize_byte = m_physValArrVirtualMemoryFreeSize.getPhysVal(i_udxVal).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getVirtualMemoryFreeSizeInB

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getVirtualMemoryUsedSize( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrVirtualMemoryUsedSize.getUnit());

    if( i_udxVal < static_cast<unsigned int>(m_physValArrVirtualMemoryUsedSize.size()) )
    {
        physVal = m_physValArrVirtualMemoryUsedSize.getPhysVal(i_udxVal);
    }
    return physVal;

} // getVirtualMemoryUsedSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getVirtualMemoryUsedSizeInB( unsigned int i_udxVal ) const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( i_udxVal < static_cast<unsigned int>(m_physValArrVirtualMemoryUsedSize.size()) )
    {
        fSize_byte = m_physValArrVirtualMemoryUsedSize.getPhysVal(i_udxVal).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getVirtualMemoryUsedSizeInB

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getFirstVirtualMemoryUpdSysTime() const
//------------------------------------------------------------------------------
{
    ZS::PhysVal::CPhysVal physValTime(m_physValArrVirtualMemoryUpdSysTime.getUnit());

    if( m_physValArrVirtualMemoryUpdSysTime.size() > 0 )
    {
        physValTime = m_physValArrVirtualMemoryUpdSysTime.getPhysVal(0);
    }
    return physValTime;

} // getFirstVirtualMemoryUpdSysTime

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getFirstVirtualMemoryFreeSize() const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrVirtualMemoryFreeSize.getUnit());

    if( m_physValArrVirtualMemoryFreeSize.size() > 0 )
    {
        physVal = m_physValArrVirtualMemoryFreeSize.getPhysVal(0);
    }
    return physVal;

} // getFirstVirtualMemoryFreeSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getFirstVirtualMemoryFreeSizeInB() const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( m_physValArrVirtualMemoryFreeSize.size() > 0 )
    {
        fSize_byte = m_physValArrVirtualMemoryFreeSize.getPhysVal(0).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getFirstVirtualMemoryFreeSizeInB

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getLastVirtualMemoryUpdSysTime() const
//------------------------------------------------------------------------------
{
    ZS::PhysVal::CPhysVal physValTime(m_physValArrVirtualMemoryUpdSysTime.getUnit());

    if( m_physValArrVirtualMemoryUpdSysTime.size() > 0 )
    {
        unsigned int udxVal = m_physValArrVirtualMemoryUpdSysTime.size()-1;
        physValTime = m_physValArrVirtualMemoryUpdSysTime.getPhysVal(udxVal);
    }
    return physValTime;

} // getLastVirtualMemoryUpdSysTimeInMs

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getLastVirtualMemoryFreeSize() const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrVirtualMemoryFreeSize.getUnit());

    if( m_physValArrVirtualMemoryFreeSize.size() > 0 )
    {
        unsigned int udxVal = m_physValArrVirtualMemoryFreeSize.size()-1;
        physVal = m_physValArrVirtualMemoryFreeSize.getPhysVal(udxVal);
    }
    return physVal;

} // getFirstVirtualMemoryFreeSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getLastVirtualMemoryFreeSizeInB() const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( m_physValArrVirtualMemoryFreeSize.size() > 0 )
    {
        unsigned int udxVal = m_physValArrVirtualMemoryFreeSize.size()-1;
        fSize_byte = m_physValArrVirtualMemoryFreeSize.getPhysVal(udxVal).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getFirstVirtualMemoryFreeSizeInB

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CModelMemoryValues::getLastVirtualMemoryUsedSize() const
//------------------------------------------------------------------------------
{
    CPhysVal physVal(m_physValArrVirtualMemoryUsedSize.getUnit());

    if( m_physValArrVirtualMemoryUsedSize.size() > 0 )
    {
        unsigned int udxVal = m_physValArrVirtualMemoryUsedSize.size()-1;
        physVal = m_physValArrVirtualMemoryUsedSize.getPhysVal(udxVal);
    }
    return physVal;

} // getFirstVirtualMemoryUsedSize

//------------------------------------------------------------------------------
double CModelMemoryValues::getLastVirtualMemoryUsedSizeInB() const
//------------------------------------------------------------------------------
{
    double fSize_byte = 0.0;

    if( m_physValArrVirtualMemoryUsedSize.size() > 0 )
    {
        unsigned int udxVal = m_physValArrVirtualMemoryUsedSize.size()-1;
        fSize_byte = m_physValArrVirtualMemoryUsedSize.getPhysVal(udxVal).getVal(DataQuantity()->Byte());
    }
    return fSize_byte;

} // getFirstVirtualMemoryUsedSizeInB

//------------------------------------------------------------------------------
QVector<double> CModelMemoryValues::getVirtualMemoryFreeSizesInBytes() const
//------------------------------------------------------------------------------
{
    return m_physValArrVirtualMemoryFreeSize.toDoubleVec( DataQuantity()->Byte() );
}

//------------------------------------------------------------------------------
QVector<double> CModelMemoryValues::getVirtualMemoryUsedSizesInBytes() const
//------------------------------------------------------------------------------
{
    return m_physValArrVirtualMemoryUsedSize.toDoubleVec( DataQuantity()->Byte() );
}

//------------------------------------------------------------------------------
QVector<double> CModelMemoryValues::getVirtualMemoryUpdSysTimesInMs() const
//------------------------------------------------------------------------------
{
    return m_physValArrVirtualMemoryUpdSysTime.toDoubleVec( Kinematics::Time()->MilliSeconds() );
}

/*==============================================================================
public: // must overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
int CModelMemoryValues::rowCount( const QModelIndex& i_modelIdxParent ) const
//------------------------------------------------------------------------------
{
    int iRowCount = 0;

    if( !i_modelIdxParent.isValid() )
    {
        if( m_bShowAllValues )
        {
            iRowCount = static_cast<int>(m_physValArrPhysicalMemoryUpdSysTime.size());

            if( static_cast<int>(m_physValArrVirtualMemoryUpdSysTime.size()) > iRowCount )
            {
                iRowCount = static_cast<int>(m_physValArrVirtualMemoryUpdSysTime.size());
            }
        }
        else
        {
            iRowCount = 1;
        }
    }
    return iRowCount;

} // rowCount

//------------------------------------------------------------------------------
int CModelMemoryValues::columnCount( const QModelIndex& /*i_modelIdxParent*/ ) const
//------------------------------------------------------------------------------
{
    return EClmCount;
}

//------------------------------------------------------------------------------
QVariant CModelMemoryValues::data( const QModelIndex& i_modelIdx, int i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( !i_modelIdx.isValid() )
    {
        return varData;
    }

    int idxClm = i_modelIdx.column();
    int idxVal = i_modelIdx.row();

    switch( idxClm )
    {
        case EClmPhysicalMemoryFreeSizeUpdSysTime:
        {
            if( m_bShowAllValues )
            {
                if( idxVal >= 0 && idxVal < static_cast<int>(m_physValArrPhysicalMemoryUpdSysTime.size()) )
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = ZS::System::Time::sysTimeInMilliSec2QTime(m_physValArrPhysicalMemoryUpdSysTime.toDouble(idxVal)).toString("hh:mm:ss:zzz");
                    }
                }
            }
            else if( m_physValArrPhysicalMemoryUpdSysTime.size() > 0 )
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = getLastPhysicalMemoryUpdSysTime().getVal(Kinematics::Time()->MilliSeconds());
                }
            }
            break;
        }
        case EClmPhysicalMemoryFreeSize:
        {
            if( m_bShowAllValues )
            {
                if( idxVal >= 0 && idxVal < static_cast<int>(m_physValArrPhysicalMemoryFreeSize.size()) )
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = QString::number(m_physValArrPhysicalMemoryFreeSize.toDouble(idxVal),10,0);
                    }
                }
            }
            else if( m_physValArrPhysicalMemoryFreeSize.size() > 0 )
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = QString::number(getLastPhysicalMemoryFreeSize().getVal(),10,0);
                }
            }
            break;
        }
        case EClmPhysicalMemoryUsedSize:
        {
            if( m_bShowAllValues )
            {
                if( idxVal >= 0 && idxVal < static_cast<int>(m_physValArrPhysicalMemoryUsedSize.size()) )
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = QString::number(m_physValArrPhysicalMemoryUsedSize.toDouble(idxVal),10,0);
                    }
                }
            }
            else if( m_physValArrPhysicalMemoryUsedSize.size() > 0 )
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = QString::number(getLastPhysicalMemoryUsedSize().getVal(),10,0);
                }
            }
            break;
        }
        case EClmVirtualMemoryFreeSizeUpdSysTime:
        {
            if( m_bShowAllValues )
            {
                if( idxVal >= 0 && idxVal < static_cast<int>(m_physValArrVirtualMemoryUpdSysTime.size()) )
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = ZS::System::Time::sysTimeInMilliSec2QTime(m_physValArrVirtualMemoryUpdSysTime.toDouble(idxVal)).toString("hh:mm:ss:zzz");
                    }
                }
            }
            else if( m_physValArrVirtualMemoryUpdSysTime.size() > 0 )
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = getLastVirtualMemoryUpdSysTime().getVal(Kinematics::Time()->MilliSeconds());
                }
            }
            break;
        }
        case EClmVirtualMemoryFreeSize:
        {
            if( m_bShowAllValues )
            {
                if( idxVal >= 0 && idxVal < static_cast<int>(m_physValArrVirtualMemoryFreeSize.size()) )
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = QString::number(m_physValArrVirtualMemoryFreeSize.toDouble(idxVal),10,0);
                    }
                }
            }
            else if( m_physValArrVirtualMemoryFreeSize.size() > 0 )
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = QString::number(getLastVirtualMemoryFreeSize().getVal(),10,0);
                }
            }
            break;
        }
        case EClmVirtualMemoryUsedSize:
        {
            if( m_bShowAllValues )
            {
                if( idxVal >= 0 && idxVal < static_cast<int>(m_physValArrVirtualMemoryUsedSize.size()) )
                {
                    if( i_iRole == Qt::DisplayRole )
                    {
                        varData = QString::number(m_physValArrVirtualMemoryUsedSize.toDouble(idxVal),10,0);
                    }
                }
            }
            else if( m_physValArrVirtualMemoryUsedSize.size() > 0 )
            {
                if( i_iRole == Qt::DisplayRole )
                {
                    varData = QString::number(getLastVirtualMemoryUsedSize().getVal(),10,0);
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return varData;

} // data

/*==============================================================================
public: // overridables of base class QAbstractItemModel
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CModelMemoryValues::headerData(
    int             i_iSection,
    Qt::Orientation i_orientation,
    int             i_iRole ) const
//------------------------------------------------------------------------------
{
    QVariant varData;

    if( i_orientation == Qt::Horizontal )
    {
        switch( i_iSection )
        {
            case EClmPhysicalMemoryFreeSizeUpdSysTime:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Physical Memory\nUpdate Time";
                }
                else if( i_iRole == Qt::TextColorRole )
                {
                    varData = m_pModelMemory->getColorPhysicalMemory();
                }
                break;
            }
            case EClmPhysicalMemoryFreeSize:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Physical Memory\nFree Size/" + m_physValArrPhysicalMemoryUsedSize.getUnit()->getSymbol();
                }
                break;
            }
            case EClmPhysicalMemoryUsedSize:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Physical Memory\nUsed Size/" + m_physValArrPhysicalMemoryUsedSize.getUnit()->getSymbol();
                }
                else if( i_iRole == Qt::TextColorRole )
                {
                    varData = m_pModelMemory->getColorPhysicalMemory();
                }
                break;
            }
            case EClmVirtualMemoryFreeSizeUpdSysTime:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Virtual Memory\nUpdate Time";
                }
                else if( i_iRole == Qt::TextColorRole )
                {
                    varData = m_pModelMemory->getColorVirtualMemory();
                }
                break;
            }
            case EClmVirtualMemoryFreeSize:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Virtual Memory\nFree Size/" + m_physValArrVirtualMemoryUsedSize.getUnit()->getSymbol();
                }
                break;
            }
            case EClmVirtualMemoryUsedSize:
            {
                if( i_iRole == Qt::DisplayRole || i_iRole == Qt::EditRole )
                {
                    varData = "Virtual Memory\nUsed Size/" + m_physValArrVirtualMemoryUsedSize.getUnit()->getSymbol();
                }
                else if( i_iRole == Qt::TextColorRole )
                {
                    varData = m_pModelMemory->getColorVirtualMemory();
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else if( i_orientation == Qt::Vertical )
    {
        if( i_iRole == Qt::DisplayRole )
        {
            varData = QString::number(i_iSection);
        }
    }
    return varData;

} // headerData
