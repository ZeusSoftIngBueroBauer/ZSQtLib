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
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"

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
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_resType),
    m_arfValues()
{
} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( CUnitGrp* i_pUnitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_pUnitGrp,i_resType),
    m_arfValues()
{
    if( i_pUnitGrp->classType() == EUnitClassType::PhysSize )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(i_pUnitGrp)->getSIUnit();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( CUnit* i_pUnit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(i_pUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes,i_pUnit,i_resType),
    m_arfValues()
{
    if( m_pUnit != nullptr )
    {
        m_pUnitGrp = m_pUnit->unitGroup();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, CUnitGrp* i_pUnitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_pUnitGrp,i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues.resize(i_iValCount);
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, CUnit* i_pUnit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(i_pUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes,i_pUnit,i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount,0.0);
    }

    if( m_pUnit != nullptr )
    {
        m_pUnitGrp = m_pUnit->unitGroup();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, const CPhysVal& i_physValInit ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_physValInit.unitGroup()),
    m_pUnit(i_physValInit.unit()),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(i_physValInit.getValidity()),
    m_physValRes(i_physValInit.getRes()),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>( i_iValCount, i_physValInit.getVal() );
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, double i_fInitVal, CUnit* i_pUnit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(i_pUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_physValRes(i_fRes,i_pUnit,i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount,i_fInitVal);
    }

    if( m_pUnit != nullptr )
    {
        m_pUnitGrp = m_pUnit->unitGroup();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, double* i_arfVals, CUnit* i_pUnit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(i_pUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_physValRes(i_fRes,i_pUnit,i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount);
        memcpy( m_arfValues.data(), i_arfVals, i_iValCount );
    }

    if( m_pUnit != nullptr )
    {
        m_pUnitGrp = m_pUnit->unitGroup();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( const QVector<double>& i_arfVals, CUnit* i_pUnit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(i_pUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_physValRes(i_fRes,i_pUnit,i_resType),
    m_arfValues(i_arfVals)
{
    if( m_pUnit != nullptr )
    {
        m_pUnitGrp = m_pUnit->unitGroup();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( const QStringList& i_strlstVals, CUnit* i_pUnit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(i_pUnit),
    m_strUnitGrpKey(),
    m_strUnitKey(),
    m_validity(EValueValidity::Valid),
    m_physValRes(i_fRes,i_pUnit,i_resType),
    m_arfValues()
{
    if( m_pUnit != nullptr )
    {
        m_pUnitGrp = m_pUnit->unitGroup();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    *this = i_strlstVals;

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( const QString& i_strUnitKey, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes,i_strUnitKey,i_resType),
    m_arfValues()
{
} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, const QString& i_strUnitKey, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes,i_strUnitKey,i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount);
    }
} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, double i_fInitVal, const QString& i_strUnitKey, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes,i_strUnitKey,i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount,i_fInitVal);
    }
} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( int i_iValCount, double* i_arfVals, const QString& i_strUnitKey, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes,i_strUnitKey,i_resType),
    m_arfValues()
{
    if( i_iValCount > 0 )
    {
        m_arfValues = QVector<double>(i_iValCount);
        memcpy( m_arfValues.data(), i_arfVals, i_iValCount );
    }
} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( const QVector<double>& i_arfVals, const QString& i_strUnitKey, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes,i_strUnitKey,i_resType),
    m_arfValues(i_arfVals)
{
} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( const QStringList& i_strlstVals, const QString& i_strUnitKey, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey),
    m_validity(EValueValidity::Invalid),
    m_physValRes(i_fRes,i_strUnitKey,i_resType),
    m_arfValues()
{
    *this = i_strlstVals;

} // ctor

//------------------------------------------------------------------------------
CPhysValArr::CPhysValArr( const CPhysValArr& i_physValArr ) :
//------------------------------------------------------------------------------
    m_pUnitGrp(i_physValArr.m_pUnitGrp),
    m_pUnit(i_physValArr.m_pUnit),
    m_strUnitGrpKey(i_physValArr.m_strUnitGrpKey),
    m_strUnitKey(i_physValArr.m_strUnitKey),
    m_validity(i_physValArr.m_validity),
    m_physValRes(i_physValArr.m_physValRes),
    m_arfValues(i_physValArr.m_arfValues)
{
} // copy ctor

//------------------------------------------------------------------------------
CPhysValArr::~CPhysValArr()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;
    m_pUnit = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValArr::operator == ( const CPhysValArr& i_physValArrOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_strUnitGrpKey != i_physValArrOther.m_strUnitGrpKey )
    {
        bEqual = false;
    }
    else if( m_validity != i_physValArrOther.m_validity )
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

        if( m_pUnit == i_physValArrOther.m_pUnit )
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
        else // if( m_pUnit != i_physValArrOther.m_pUnit )
        {
            #ifdef _WINDOWS
            #pragma message(__TODO__"Resolution should be taken into account.")
            #endif
            // But this implies converting the values into strings and compare the resulting strings.
            // This takes a lot of processor time.

            //CPhysValArr physValArrOther = i_physValArrOther;

            //physValArrOther.convertValues(m_pUnit);

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
    m_pUnitGrp      = i_physValArr.m_pUnitGrp;
    m_pUnit         = i_physValArr.m_pUnit;
    m_strUnitGrpKey = i_physValArr.m_strUnitGrpKey;
    m_strUnitKey    = i_physValArr.m_strUnitKey;
    m_validity      = i_physValArr.m_validity;
    m_physValRes    = i_physValArr.m_physValRes;
    m_arfValues     = i_physValArr.m_arfValues;

    return *this;

} // operator =

//------------------------------------------------------------------------------
CPhysValArr& CPhysValArr::operator = ( const QVector<CPhysVal>& i_arPhysVals )
//------------------------------------------------------------------------------
{
    m_arfValues = QVector<double>( i_arPhysVals.size(), 0.0 );

    if( i_arPhysVals.size() > 0 )
    {
        int idx;

        for( idx = 0; idx < i_arPhysVals.size(); idx++ )
        {
            m_arfValues[idx] = i_arPhysVals[idx].getVal(m_pUnit);
        }
    }

    return *this;

} // operator =

//------------------------------------------------------------------------------
CPhysValArr& CPhysValArr::operator = ( const QVector<double>& i_arfVals )
//------------------------------------------------------------------------------
{
    m_arfValues = i_arfVals;

    return *this;

} // operator =

//------------------------------------------------------------------------------
CPhysValArr& CPhysValArr::operator = ( const QStringList& i_strlstVals )
//------------------------------------------------------------------------------
{
    if( i_strlstVals.size() > 0 )
    {
        CPhysVal physVal;
        int      idx;

        if( m_pUnit != nullptr )
        {
            physVal.setUnit(m_pUnit);
        }
        else if( !m_strUnitKey.isEmpty() )
        {
            physVal.setUnitKey(m_strUnitKey);
        }

        if( m_pUnitGrp != nullptr )
        {
            physVal.setUnitGroup(m_pUnitGrp);
        }
        else if( !m_strUnitGrpKey.isEmpty() )
        {
            physVal.setUnitGroupKey(m_strUnitGrpKey);
        }

        for( idx = 0; idx < i_strlstVals.size(); idx++ )
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

} // operator =

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
void CPhysValArr::setUnitGroup( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_pUnitGrp )
    {
        clear();

        m_validity = EValueValidity::Invalid;

        m_pUnitGrp = i_pUnitGrp;

        if( m_pUnitGrp == nullptr )
        {
            m_pUnit = nullptr;
        }
        else if( m_pUnitGrp->classType() == EUnitClassType::PhysSize )
        {
            m_pUnit = dynamic_cast<CPhysSize*>(m_pUnitGrp)->getSIUnit();
        }

        if( m_pUnitGrp != nullptr )
        {
            m_strUnitGrpKey = m_pUnitGrp->keyInTree();
        }
        if( m_pUnit != nullptr )
        {
            m_strUnitKey = m_pUnit->keyInTree();
        }
    }

    m_physValRes.setUnitGroup(m_pUnitGrp);

} // setUnitGroup

//------------------------------------------------------------------------------
void CPhysValArr::setUnitGroupKey( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp->keyInTree() != i_strUnitGrpKey )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CPhysValArr::setUnitGroupKey";
        strArgs   = "UnitGrpKey: " + i_strUnitGrpKey;
        strErr    = "UnitGrpKey is different from key of already set unit group " + m_pUnitGrp->keyInTree();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }

    m_strUnitGrpKey = i_strUnitGrpKey;

    m_physValRes.setUnitGroupKey(i_strUnitGrpKey);

} // setUnitGroupKey

//------------------------------------------------------------------------------
CUnitGrp* CPhysValArr::unitGroup() const
//------------------------------------------------------------------------------
{
    return m_pUnitGrp;
}

//------------------------------------------------------------------------------
QString CPhysValArr::getUnitGroupName( bool i_bInsertParentNames ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pUnitGrp == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = m_pUnitGrp->keyInTree();
    }
    return strName;

} // getUnitGroupName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setUnit( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pUnit;

    CUnitGrp* pUnitGrp = m_pUnitGrp;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else
    {
        m_pUnitGrp = m_pUnit->unitGroup();
    }

    if( m_pUnitGrp != pUnitGrp )
    {
        clear();
        m_validity = EValueValidity::Invalid;
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    if( m_physValRes.isValid() )
    {
        if( m_physValRes.unit() != nullptr && m_pUnit != nullptr )
        {
            m_physValRes.unit()->convertValue(m_physValRes.getVal(),m_pUnit);
        }
        else
        {
            m_physValRes.setUnit(m_pUnit);
        }
    }
    else
    {
        m_physValRes.setUnit(m_pUnit);
    }

} // setUnit

//------------------------------------------------------------------------------
void CPhysValArr::setUnitKey( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr && m_pUnit->keyInTree() != i_strUnitKey )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CPhysValArr::setUnitKey";
        strArgs   = "UnitKey: " + i_strUnitKey;
        strErr    = "UnitKey is different from key of already set unit " + m_pUnit->keyInTree();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }

    m_strUnitKey = i_strUnitKey;

    m_physValRes.setUnitKey(i_strUnitKey);

} // setUnitKey

//------------------------------------------------------------------------------
CUnit* CPhysValArr::unit() const
//------------------------------------------------------------------------------
{
    return m_pUnit;
}

//------------------------------------------------------------------------------
QString CPhysValArr::getUnitName( bool i_bInsertParentNames ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pUnit == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = m_pUnit->keyInTree();
    }
    return strName;

} // getUnitName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;
    m_pUnit = nullptr;

    m_physValRes.invalidateObjectReferences();

} // invalidateObjectReferences

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
void CPhysValArr::setVal( int i_idx, double i_fVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    double fVal = i_fVal;
    CUnit* pUnit = i_pUnit;

    if( pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnitGrp,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_pUnit != nullptr && !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( m_pUnit == nullptr )
    {
        m_pUnit = pUnit;
    }

    if( pUnit != nullptr && m_pUnit != pUnit )
    {
        fVal = pUnit->convertValue(fVal,m_pUnit);
    }

    setVal(i_idx,fVal);

} // setVal

//------------------------------------------------------------------------------
void CPhysValArr::setVal( int i_idx, const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    setVal( i_idx, i_physVal.getVal(m_pUnit) );
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

} // insertVal

//------------------------------------------------------------------------------
void CPhysValArr::insertVal( int i_idx, double i_fVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    double fVal = i_fVal;
    CUnit* pUnit = i_pUnit;

    if( pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnitGrp,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_pUnit != nullptr && !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( m_pUnit == nullptr )
    {
        m_pUnit = i_pUnit;
    }

    if( pUnit != nullptr && m_pUnit != pUnit )
    {
        fVal = pUnit->convertValue(fVal,m_pUnit);
    }

    insertVal( i_idx, fVal );

} // insertVal

//------------------------------------------------------------------------------
void CPhysValArr::insertVal( int i_idx, const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    insertVal( i_idx, i_physVal.getVal(m_pUnit) );

} // insertVal

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
void CPhysValArr::appendVal( double i_fVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    double fVal = i_fVal;
    CUnit* pUnit = i_pUnit;

    if( pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnitGrp,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_pUnit != nullptr && !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( m_pUnit == nullptr )
    {
        m_pUnit = i_pUnit;
    }

    if( pUnit != nullptr && m_pUnit != pUnit )
    {
        fVal = pUnit->convertValue(fVal,m_pUnit);
    }

    appendVal(fVal);

} // appendVal

//------------------------------------------------------------------------------
void CPhysValArr::appendVal( const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    appendVal( i_physVal.getVal(m_pUnit) );
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
    if( !areOfSameUnitGroup(m_pUnitGrp,i_physValArr.unit()) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_physValArr.unit()->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_pUnit != nullptr && !areOfSameUnitGroup(m_pUnit,i_physValArr.unit()) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_physValArr.unit()->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( m_pUnit == nullptr )
    {
        m_pUnit = i_physValArr.unit();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->keyInTree();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->keyInTree();
    }

    const CPhysValArr* pPhysValArr = &i_physValArr;

    CPhysValArr physValArrTmp;

    if( m_pUnit != i_physValArr.unit() )
    {
        physValArrTmp = i_physValArr;

        physValArrTmp.convertValues(m_pUnit);

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
    setValues( i_idxStart, i_arfVals, m_pUnit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, const QVector<double>& i_arfVals, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    CUnit* pUnit = i_pUnit;

    if( i_pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnitGrp,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_pUnit != nullptr && !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( m_pUnit == nullptr )
    {
        m_pUnit = i_pUnit;
    }

    if( m_pUnit != pUnit )
    {
        setUnit(pUnit);
    }

    if( i_idxStart + i_arfVals.size() > m_arfValues.size() )
    {
        m_arfValues.resize( i_idxStart + i_arfVals.size() );
    }

    int idx;

    for( idx = 0; idx < i_arfVals.size(); idx++ )
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
    setValues( i_idxStart, i_iValCount, i_arfVals, m_pUnit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, int i_iValCount, double* i_arfVals, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    QVector<double> arfVals(i_iValCount);

    memcpy( arfVals.data(), i_arfVals, i_iValCount*sizeof(double) );

    setValues( i_idxStart, arfVals, i_pUnit );

} // setValues

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
    CPhysVal physVal( m_arfValues[i_idx], m_pUnit, m_physValRes );
    physVal.setValidity(m_validity);
    return physVal;
}

//------------------------------------------------------------------------------
CPhysVal CPhysValArr::getPhysVal( int i_idx, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CPhysVal physVal( m_arfValues[i_idx], m_pUnit, m_physValRes );

    if( i_pUnit != nullptr && i_pUnit != m_pUnit )
    {
        physVal.convertValue(i_pUnit);
    }
    physVal.setValidity(m_validity);

    return physVal;

} // getPhysVal

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValArr CPhysValArr::mid( int i_idxValStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    CPhysValArr physValArr;

    if( m_pUnit != nullptr )
    {
        physValArr.setUnit(m_pUnit);
    }
    else if( m_pUnitGrp != nullptr )
    {
        physValArr.setUnitGroup(m_pUnitGrp);
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
CPhysValArr CPhysValArr::mid( int i_idxValStart, int i_iValCount, CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CPhysValArr physValArr;

    if( m_pUnit != nullptr )
    {
        physValArr.setUnit(m_pUnit);
    }
    else if( m_pUnitGrp != nullptr )
    {
        physValArr.setUnitGroup(m_pUnitGrp);
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

    if( i_pUnit != nullptr )
    {
        physValArr.convertValues(i_pUnit);
    }

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
double CPhysValArr::toDouble( int i_idx, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    const CUnit* pUnit = i_pUnit;

    if( pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnitGrp,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    double fVal = m_arfValues[i_idx];

    if( m_pUnit != nullptr && pUnit != m_pUnit )
    {
        fVal = m_pUnit->convertValue(fVal,pUnit);
    }
    return fVal;

} // toDouble

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
QVector<double> CPhysValArr::toDoubleVec() const
//------------------------------------------------------------------------------
{
    return toDoubleVec( 0, EArrayIndexCountAllElements, m_pUnit );
}

//------------------------------------------------------------------------------
QVector<double> CPhysValArr::toDoubleVec( const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    return toDoubleVec( 0, EArrayIndexCountAllElements, i_pUnit );
}

//------------------------------------------------------------------------------
QVector<double> CPhysValArr::toDoubleVec( int i_idxStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    return toDoubleVec( i_idxStart, i_iValCount, m_pUnit );
}

//------------------------------------------------------------------------------
QVector<double> CPhysValArr::toDoubleVec( int i_idxStart, int i_iValCount, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    const CUnit* pUnit = i_pUnit;

    if( pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
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

        if( pUnit != nullptr && m_pUnit != pUnit )
        {
            double fVal;

            for( idx = 0; idx < arfValues.size(); idx++ )
            {
                fVal = arfValues[idx];
                fVal = m_pUnit->convertValue(fVal,pUnit);
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
    CPhysVal physVal = CPhysVal( m_arfValues[i_idx], m_pUnit, m_physValRes );

    physVal.setValidity(m_validity);

    QString strVal = physVal.toString();

    return strVal;

} // toString

//------------------------------------------------------------------------------
QString CPhysValArr::toString( int i_idx, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    const CUnit* pUnit = i_pUnit;

    if( pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysVal physVal = CPhysVal( m_arfValues[i_idx], m_pUnit, m_physValRes );

    physVal.setValidity(m_validity);

    QString strVal = physVal.toString(pUnit);

    return strVal;

} // toString

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setVal( int i_idx, const QString& i_strVal )
//------------------------------------------------------------------------------
{
    setVal( i_idx, i_strVal, m_pUnit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setVal( int i_idx, const QString& i_strVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    bool bConverted;

    double fVal = i_strVal.toDouble(&bConverted);

    if( bConverted )
    {
        setVal( i_idx, fVal, i_pUnit );
    }

} // setVal

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setValues( const QStringList& i_strlstVals )
//------------------------------------------------------------------------------
{
    return setValues( 0, i_strlstVals, m_pUnit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setValues( const QStringList& i_strlstVals, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    return setValues( 0, i_strlstVals, i_pUnit );
}

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, const QStringList& i_strlstVals )
//------------------------------------------------------------------------------
{
    return setValues( i_idxStart, i_strlstVals, m_pUnit );
}

//------------------------------------------------------------------------------
void CPhysValArr::setValues( int i_idxStart, const QStringList& i_strlstVals, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    CUnit* pUnit = i_pUnit;

    if( pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnitGrp,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_pUnit != nullptr && !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    if( m_pUnit == nullptr )
    {
        m_pUnit = i_pUnit;
    }

    if( m_pUnit != pUnit )
    {
        setUnit(pUnit);
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
    return toStringList( 0, EArrayIndexCountAllElements, m_pUnit );
}

//------------------------------------------------------------------------------
QStringList CPhysValArr::toStringList( const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    return toStringList( 0, EArrayIndexCountAllElements, i_pUnit );
}

//------------------------------------------------------------------------------
QStringList CPhysValArr::toStringList( int i_idxStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    return toStringList( i_idxStart, i_iValCount, m_pUnit );
}

//------------------------------------------------------------------------------
QStringList CPhysValArr::toStringList( int i_idxStart, int i_iValCount, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    const CUnit* pUnit = i_pUnit;

    if( pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }

    if( !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnit->parentBranchName();
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
            strlstVals.append( toString(idx,pUnit) );
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
void CPhysValArr::setRes( double i_fRes, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    m_physValRes.setVal(i_fRes,i_pUnit);
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
void CPhysValArr::convertValues( CUnit* i_pUnitDst )
//------------------------------------------------------------------------------
{
    if( isValid() )
    {
        if( !areOfSameUnitGroup(m_pUnitGrp,i_pUnitDst) )
        {
            QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnitDst->parentBranchName();
            throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
        }
        if( m_pUnit != nullptr && !areOfSameUnitGroup(m_pUnit,i_pUnitDst) )
        {
            QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnitDst->parentBranchName();
            throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
        }
    }

    if( m_pUnit == nullptr )
    {
        if( m_pUnitGrp != nullptr )
        {
            if( !areOfSameUnitGroup(m_pUnitGrp,i_pUnitDst) )
            {
                QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + i_pUnitDst->parentBranchName();
                throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
            }
        }
        m_pUnit = i_pUnitDst;
        m_pUnitGrp = m_pUnit->unitGroup();

        if( m_pUnitGrp != nullptr )
        {
            m_strUnitGrpKey = m_pUnitGrp->keyInTree();
        }
        if( m_pUnit != nullptr )
        {
            m_strUnitKey = m_pUnit->keyInTree();
        }
    }

    if( i_pUnitDst != nullptr && m_pUnit != nullptr && i_pUnitDst != m_pUnit )
    {
        double fVal;
        int    idx;

        for( idx = 0; idx < m_arfValues.size(); idx++ )
        {
            fVal = m_arfValues[idx];
            fVal = m_pUnit->convertValue(fVal,i_pUnitDst);
            m_arfValues[idx] = fVal;
        }
    }

    m_pUnit = i_pUnitDst;

} // convertValues
