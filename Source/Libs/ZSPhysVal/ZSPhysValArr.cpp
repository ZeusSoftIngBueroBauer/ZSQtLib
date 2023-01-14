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

#include "ZSPhysVal/ZSPhysValArr.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CPhysValArr
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_resType),
    m_arfValues()
{
}

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( const CUnit& i_unit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_arfValues()
{
}

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr(
    int i_iValCount, const CUnit& i_unit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount, 0.0);
    }
}

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, const CPhysVal& i_physValInit ) :
//------------------------------------------------------------------------------
    m_unit(i_physValInit.unit()),
    m_validity(i_physValInit.getValidity()),
    m_physValRes(i_physValInit.getRes()),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>( i_iValCount, i_physValInit.getVal() );
    }
}

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr(
    int i_iValCount, double i_fInitVal, const CUnit& i_unit,
    double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Valid),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount,i_fInitVal);
    }
}

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr(
    int i_iValCount, double* i_arfVals, const CUnit& i_unit,
    double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Valid),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount);
        memcpy( m_arfValues.data(), i_arfVals, i_iValCount );
    }
}

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr(
    const QVector<double>& i_arfVals, const CUnit& i_unit,
    double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Valid),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_arfValues(i_arfVals)
{
}

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr(
    const QStringList& i_strlstVals, const CUnit& i_unit,
    double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_validity(EValueValidity::Valid),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_arfValues()
{
    *this = i_strlstVals;
}

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( const CPhysValArr& i_physValArr ) :
//------------------------------------------------------------------------------
    m_unit(i_physValArr.m_unit),
    m_validity(i_physValArr.m_validity),
    m_physValRes(i_physValArr.m_physValRes),
    m_arfValues(i_physValArr.m_arfValues)
{
}

//------------------------------------------------------------------------------
CPhysValArr::~CPhysValArr()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValArr::operator == ( const CPhysValArr& i_physValArrOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_validity != i_physValArrOther.m_validity )
    {
        bEqual = false;
    }
    else if( m_physValRes != i_physValArrOther.m_physValRes )
    {
        bEqual = false;
    }
    else if( m_arfValues.size() != i_physValArrOther.m_arfValues.size() )
    {
        bEqual = false;
    }
    else
    {
        int idxVal;

        if( m_unit == i_physValArrOther.m_unit )
        {
            #ifdef _WINDOWS
            #pragma message(__TODO__"Resolution should be taken into account.")
            #endif
            // But this implies converting the values into strings and compare the resulting strings.
            // This takes a lot of processor time.

            if( memcmp( m_arfValues.data(), i_physValArrOther.m_arfValues.data(), m_arfValues.size()*sizeof(m_arfValues[0]) ) != 0 )
            {
                bEqual = false;
            }

            //if( !bEqual )
            //{
            //    bEqual = true;

            //    for( idxVal = 0; idxVal < size(); idxVal++ )
            //    {
            //        if( m_arfValues[idxVal] != i_physValArrOther.m_arfValues[idxVal] )
            //        {
            //            bEqual = false;
            //            break;
            //        }
            //    }
            //}
        }
        else // if( m_unit != i_physValArrOther.m_unit )
        {
            #ifdef _WINDOWS
            #pragma message(__TODO__"Resolution should be taken into account.")
            #endif
            // But this implies converting the values into strings and compare the resulting strings.
            // This takes a lot of processor time.

            //CPhysValArr physValArrOther = i_physValArrOther;

            //physValArrOther.convertValues(m_unit);

            bEqual = false;
            //bEqual = (physValArrOther == *this);

            if( !bEqual )
            {
                bEqual = true;

                for( idxVal = 0; idxVal < size(); idxVal++ )
                {
                    if( getPhysVal(idxVal) != i_physValArrOther.getPhysVal(idxVal) )
                    {
                        bEqual = false;
                        break;
                    }
                }
            }
        }
    }

    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CPhysValArr::operator != ( const CPhysValArr& i_physValArrOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValArrOther);
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValArr& CPhysValArr::operator = ( const CPhysValArr& i_physValArr )
//------------------------------------------------------------------------------
{
    m_unit = i_physValArr.m_unit;
    m_validity = i_physValArr.m_validity;
    m_physValRes = i_physValArr.m_physValRes;
    m_arfValues = i_physValArr.m_arfValues;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValArr& CPhysValArr::operator = ( const QVector<CPhysVal>& i_arPhysVals )
//------------------------------------------------------------------------------
{
    m_arfValues = QVector<double>( i_arPhysVals.size(), 0.0 );

    if( i_arPhysVals.size() > 0 )
    {
        for( int idx = 0; idx < i_arPhysVals.size(); idx++ )
        {
            m_arfValues[idx] = i_arPhysVals[idx].getVal(m_unit);
        }
    }
    return *this;
}

//------------------------------------------------------------------------------
CPhysValArr& CPhysValArr::operator = ( const QVector<double>& i_arfVals )
//------------------------------------------------------------------------------
{
    m_arfValues = i_arfVals;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValArr& CPhysValArr::operator = ( const QStringList& i_strlstVals )
//------------------------------------------------------------------------------
{
    if( i_strlstVals.size() > 0 )
    {
        CPhysVal physVal(m_unit);

        for( int idx = 0; idx < i_strlstVals.size(); idx++ )
        {
            try
            {
                physVal = i_strlstVals[idx];
                m_arfValues[idx] = physVal.getVal();
            }
            catch( CPhysValException& )
            {
            }
        }
    }
    return *this;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::clear()
//------------------------------------------------------------------------------
{
    m_arfValues.clear();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValArr::isValid() const
//------------------------------------------------------------------------------
{
    return (m_validity != EValueValidity::Invalid);
}

//------------------------------------------------------------------------------
bool CPhysValArr::isNull() const
//------------------------------------------------------------------------------
{
    return (m_validity == EValueValidity::Null);
}

//------------------------------------------------------------------------------
void CPhysValArr::setValidity( EValueValidity i_validity )
//------------------------------------------------------------------------------
{
    m_validity = i_validity;
}

//------------------------------------------------------------------------------
EValueValidity CPhysValArr::getValidity() const
//------------------------------------------------------------------------------
{
    return m_validity;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
    m_physValRes.setUnit(m_unit);
}

//------------------------------------------------------------------------------
CUnit CPhysValArr::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_unit = CUnit();
    m_physValRes.invalidateObjectReferences();
}

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setVal( int i_idx, double i_fVal )
//------------------------------------------------------------------------------
{
    m_arfValues[i_idx] = i_fVal;
    m_validity = EValueValidity::Valid;
}

//------------------------------------------------------------------------------
void CPhysValArr::setVal( int i_idx, double i_fVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit, i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    double fVal = i_fVal;
    if( m_unit != i_unit )
    {
        fVal = i_unit.convertValue(fVal, m_unit);
    }
    setVal(i_idx, fVal);
}

//------------------------------------------------------------------------------
void CPhysValArr::setVal( int i_idx, const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    setVal( i_idx, i_physVal.getVal(m_unit) );
}

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::insertVal( int i_idx, double i_fVal )
//------------------------------------------------------------------------------
{
    m_arfValues.insert( i_idx, i_fVal );
    m_validity = EValueValidity::Valid;
}

//------------------------------------------------------------------------------
void CPhysValArr::insertVal( int i_idx, double i_fVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() && !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    double fVal = i_fVal;
    if( m_unit != i_unit )
    {
        fVal = i_unit.convertValue(fVal,m_unit);
    }
    insertVal( i_idx, fVal );
}

//------------------------------------------------------------------------------
void CPhysValArr::insertVal( int i_idx, const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    insertVal( i_idx, i_physVal.getVal(m_unit) );
}

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::appendVal( double i_fVal )
//------------------------------------------------------------------------------
{
    m_arfValues.append(i_fVal);
    m_validity = EValueValidity::Valid;
}

//------------------------------------------------------------------------------
void CPhysValArr::appendVal( double i_fVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() && !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    double fVal = i_fVal;
    if( m_unit != i_unit )
    {
        fVal = i_unit.convertValue(fVal, m_unit);
    }
    appendVal(fVal);
}

//------------------------------------------------------------------------------
void CPhysValArr::appendVal( const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    appendVal( i_physVal.getVal(m_unit) );
}

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::removeVal( int i_idx )
//------------------------------------------------------------------------------
{
    m_arfValues.remove(i_idx);
}

/*==============================================================================
public: // instance methods (replacing all already existing values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, const CPhysValArr& i_physValArr )
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() && !areOfSameUnitGroup(m_unit,i_physValArr.unit()) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_physValArr.unit().keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( !m_unit.isValid() )
    {
        m_unit = i_physValArr.unit();
    }

    const CPhysValArr* pPhysValArr = &i_physValArr;

    CPhysValArr physValArrTmp;

    if( m_unit != i_physValArr.unit() )
    {
        physValArrTmp = i_physValArr;
        physValArrTmp.convertValues(m_unit);
        pPhysValArr = &physValArrTmp;
    }

    if( i_idxStart + i_physValArr.size() > m_arfValues.size() )
    {
        m_arfValues.resize( i_idxStart + i_physValArr.size() );
    }

    if( i_physValArr.size() > 0 )
    {
        memcpy( &m_arfValues.data()[i_idxStart], pPhysValArr->m_arfValues.data(), i_physValArr.size()*sizeof(pPhysValArr->m_arfValues[0]) );
    }

    m_validity = EValueValidity::Valid;

} // setValues

/*==============================================================================
public: // instance methods (replacing part of already existing values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, const QVector<double>& i_arfVals )
//------------------------------------------------------------------------------
{
    setValues( i_idxStart, i_arfVals, m_unit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, const QVector<double>& i_arfVals, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() && !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( m_unit != i_unit )
    {
        setUnit(i_unit);
    }

    if( i_idxStart + i_arfVals.size() > m_arfValues.size() )
    {
        m_arfValues.resize( i_idxStart + i_arfVals.size() );
    }

    for( int idx = 0; idx < i_arfVals.size(); idx++ )
    {
        m_arfValues[i_idxStart+idx] = i_arfVals[idx];
    }

    m_validity = EValueValidity::Valid;

} // setValues

/*==============================================================================
public: // instance methods (replacing all already existing values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, int i_iValCount, double* i_arfVals )
//------------------------------------------------------------------------------
{
    setValues( i_idxStart, i_iValCount, i_arfVals, m_unit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, int i_iValCount, double* i_arfVals, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    QVector<double> arfVals(i_iValCount);
    memcpy( arfVals.data(), i_arfVals, i_iValCount*sizeof(double) );
    setValues( i_idxStart, arfVals, i_unit );
}

/*==============================================================================
public: // instance methods (to remove values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::removeValues( int i_idxValStart, int i_iValCount )
//------------------------------------------------------------------------------
{
    m_arfValues.remove(i_idxValStart,i_iValCount);
}

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal CPhysValArr::getPhysVal( int i_idx ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal( m_arfValues[i_idx], m_unit, m_physValRes );
    physVal.setValidity(m_validity);
    return physVal;
}

//------------------------------------------------------------------------------
CPhysVal CPhysValArr::getPhysVal( int i_idx, const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal( m_arfValues[i_idx], m_unit, m_physValRes );

    if( i_unit != m_unit )
    {
        physVal.convertValue(i_unit);
    }
    physVal.setValidity(m_validity);

    return physVal;
}

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValArr CPhysValArr::mid( int i_idxValStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    CPhysValArr physValArr;

    if( m_unit.isValid() )
    {
        physValArr.setUnit(m_unit);
    }

    if( m_physValRes.isValid() )
    {
        physValArr.setRes(m_physValRes);
    }

    int idxValMin =  0;
    int idxValMax = -1;

    getMinMaxArrayRange( i_idxValStart ,i_iValCount, m_arfValues.size(), &idxValMin, &idxValMax );

    if( idxValMax >= idxValMin )
    {
        physValArr = m_arfValues.mid( idxValMin, (idxValMax-idxValMin+1) );
    }

    physValArr.setValidity(m_validity);

    return physValArr;

} // mid

//------------------------------------------------------------------------------
CPhysValArr CPhysValArr::mid( int i_idxValStart, int i_iValCount, const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    CPhysValArr physValArr;

    if( m_unit.isValid() )
    {
        physValArr.setUnit(m_unit);
    }

    if( m_physValRes.isValid() )
    {
        physValArr.setRes(m_physValRes);
    }

    int idxValMin =  0;
    int idxValMax = -1;

    getMinMaxArrayRange( i_idxValStart ,i_iValCount, m_arfValues.size(), &idxValMin, &idxValMax );

    if( idxValMax >= idxValMin )
    {
        physValArr = m_arfValues.mid( idxValMin, (idxValMax-idxValMin+1) );
    }

    physValArr.convertValues(i_unit);
    physValArr.setValidity(m_validity);

    return physValArr;

} // mid

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
double CPhysValArr::toDouble( int i_idx ) const
//------------------------------------------------------------------------------
{
    return m_arfValues[i_idx];
}

//------------------------------------------------------------------------------
double CPhysValArr::toDouble( int i_idx, const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    double fVal = m_arfValues[i_idx];
    if( i_unit != m_unit )
    {
        fVal = m_unit.convertValue(fVal,i_unit);
    }
    return fVal;
}

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
QVector<double> CPhysValArr::toDoubleVec() const
//------------------------------------------------------------------------------
{
    return toDoubleVec( 0, EArrayIndexCountAllElements, m_unit );
}

//------------------------------------------------------------------------------
QVector<double> CPhysValArr::toDoubleVec( const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    return toDoubleVec( 0, EArrayIndexCountAllElements, i_unit );
}

//------------------------------------------------------------------------------
QVector<double> CPhysValArr::toDoubleVec( int i_idxStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    return toDoubleVec( i_idxStart, i_iValCount, m_unit );
}

//------------------------------------------------------------------------------
QVector<double> CPhysValArr::toDoubleVec( int i_idxStart, int i_iValCount, const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    QVector<double> arfValues;

    int idxMin =  0;
    int idxMax = -1;
    int idx;

    if( i_idxStart == EArrayIndexFirstElement )
    {
        idxMin = 0;
    }
    else if( i_idxStart == EArrayIndexLastElement ) // not really meaningful
    {
        idxMin = m_arfValues.size() - 1;
    }
    else if( i_idxStart >= 0 && i_idxStart < m_arfValues.size() )
    {
        idxMin = i_idxStart;
    }

    if( i_iValCount == EArrayIndexCountAllElements )
    {
        idxMax = m_arfValues.size() - idxMin - 1;
    }
    else if( i_iValCount >= 0 )
    {
        idxMax = i_iValCount - idxMin - 1;
    }

    if( idxMax >= m_arfValues.size() )
    {
        idxMax = m_arfValues.size() - 1;
    }

    if( idxMax >= idxMin )
    {
        int iValCount = idxMax - idxMin + 1;

        if( idxMin == 0 && iValCount == m_arfValues.size() )
        {
            arfValues = m_arfValues;
        }
        else
        {
            arfValues.resize(iValCount);

            for( idx = 0; idx < iValCount; idx++ )
            {
                arfValues[idx] = m_arfValues[idxMin+idx];
            }
        }

        if( m_unit != i_unit )
        {
            for( idx = 0; idx < arfValues.size(); idx++ )
            {
                double fVal = arfValues[idx];
                fVal = m_unit.convertValue(fVal,i_unit);
                arfValues[idx] = fVal;
            }
        }
    }

    return arfValues;

} // toDoubleVec

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
QString CPhysValArr::toString( int i_idx ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal = CPhysVal( m_arfValues[i_idx], m_unit, m_physValRes );

    physVal.setValidity(m_validity);

    QString strVal = physVal.toString();

    return strVal;

} // toString

//------------------------------------------------------------------------------
QString CPhysValArr::toString( int i_idx, const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysVal physVal = CPhysVal( m_arfValues[i_idx], m_unit, m_physValRes );
    physVal.setValidity(m_validity);
    QString strVal = physVal.toString(i_unit);
    return strVal;
}

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setVal( int i_idx, const QString& i_strVal )
//------------------------------------------------------------------------------
{
    setVal( i_idx, i_strVal, m_unit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setVal( int i_idx, const QString& i_strVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    bool bConverted;

    double fVal = i_strVal.toDouble(&bConverted);

    if( bConverted )
    {
        setVal(i_idx, fVal, i_unit);
    }

} // setVal

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setValues( const QStringList& i_strlstVals )
//------------------------------------------------------------------------------
{
    return setValues( 0, i_strlstVals, m_unit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setValues( const QStringList& i_strlstVals, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    return setValues(0, i_strlstVals, i_unit);
}

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, const QStringList& i_strlstVals )
//------------------------------------------------------------------------------
{
    return setValues(i_idxStart, i_strlstVals, m_unit);
}

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, const QStringList& i_strlstVals, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() && !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( m_unit != i_unit )
    {
        setUnit(i_unit);
    }

    if( i_idxStart + i_strlstVals.size() > m_arfValues.size() )
    {
        m_arfValues.resize( i_idxStart + i_strlstVals.size() );
    }

    double fVal;
    bool   bConverted;
    int    idx;

    for( idx = 0; idx < i_strlstVals.size(); idx++ )
    {
        fVal = i_strlstVals[idx].toDouble(&bConverted);

        if( bConverted )
        {
            m_arfValues[i_idxStart+idx] = fVal;
        }
    }

    m_validity = EValueValidity::Valid;

} // setValues

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
QStringList CPhysValArr::toStringList() const
//------------------------------------------------------------------------------
{
    return toStringList(0, EArrayIndexCountAllElements, m_unit);
}

//------------------------------------------------------------------------------
QStringList CPhysValArr::toStringList( const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    return toStringList(0, EArrayIndexCountAllElements, i_unit);
}

//------------------------------------------------------------------------------
QStringList CPhysValArr::toStringList( int i_idxStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    return toStringList( i_idxStart, i_iValCount, m_unit );
}

//------------------------------------------------------------------------------
QStringList CPhysValArr::toStringList( int i_idxStart, int i_iValCount, const CUnit& i_unit ) const
//------------------------------------------------------------------------------
{
    if( !areOfSameUnitGroup(m_unit,i_unit) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unit.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    QStringList strlstVals;

    int idxMin =  0;
    int idxMax = -1;
    int idx;

    if( i_idxStart == EArrayIndexFirstElement )
    {
        idxMin = 0;
    }
    else if( i_idxStart == EArrayIndexLastElement ) // not really meaningful
    {
        idxMin = m_arfValues.size() - 1;
    }
    else if( i_idxStart >= 0 && i_idxStart < m_arfValues.size() )
    {
        idxMin = i_idxStart;
    }

    if( i_iValCount == EArrayIndexCountAllElements )
    {
        idxMax = m_arfValues.size() - idxMin - 1;
    }
    else if( i_iValCount >= 0 )
    {
        idxMax = i_iValCount - idxMin - 1;
    }

    if( idxMax >= m_arfValues.size() )
    {
        idxMax = m_arfValues.size() - 1;
    }

    if( idxMax >= idxMin )
    {
        int iValCount = idxMax - idxMin + 1;

        #if QT_VERSION >= 0x040700
        strlstVals.reserve(iValCount);
        #endif

        for( idx = 0; idx < iValCount; idx++ )
        {
            strlstVals.append( toString(idx,i_unit) );
        }
    }

    return strlstVals;

} // toStringList

/*==============================================================================
public: // instance methods (to set the resolution of the values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setRes( double i_fRes )
//------------------------------------------------------------------------------
{
    m_physValRes.setVal(i_fRes);
}

//------------------------------------------------------------------------------
void CPhysValArr::setRes( double i_fRes, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_physValRes.setVal(i_fRes, i_unit);
}

//------------------------------------------------------------------------------
void CPhysValArr::setRes( const CPhysValRes& i_physValRes )
//------------------------------------------------------------------------------
{
    m_physValRes = i_physValRes;
}

/*==============================================================================
public: // instance methods (to get the resolution of the values)
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValArr::hasRes() const
//------------------------------------------------------------------------------
{
    return m_physValRes.isValid();
}

//------------------------------------------------------------------------------
CPhysValRes CPhysValArr::getRes() const
//------------------------------------------------------------------------------
{
    return m_physValRes;
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::convertValues( const CUnit& i_unitDst )
//------------------------------------------------------------------------------
{
    if( m_unit.isValid() && !areOfSameUnitGroup(m_unit,i_unitDst) )
    {
        QString strAddErrInfo = "Src:" + m_unit.keyInTree() + ", Dst:" + i_unitDst.keyInTree();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( i_unitDst != m_unit )
    {
        for( int idx = 0; idx < m_arfValues.size(); idx++ )
        {
            double fVal = m_arfValues[idx];
            fVal = m_unit.convertValue(fVal,i_unitDst);
            m_arfValues[idx] = fVal;
        }
    }
    m_unit = i_unitDst;
}
