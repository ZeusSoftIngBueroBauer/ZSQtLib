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

#include "ZSPhysVal/ZSPhysValRes.h"
#include "ZSPhysVal/ZSPhysUnitsPool.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CPhysValRes
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const CPhysValRes& i_physValRes ) :
//------------------------------------------------------------------------------
    m_resType(i_physValRes.m_resType),
    m_fVal(i_physValRes.m_fVal),
    m_pUnitGrp(i_physValRes.m_pUnitGrp),
    m_pUnit(i_physValRes.m_pUnit),
    m_strUnitGrpKey(i_physValRes.m_strUnitGrpKey),
    m_strUnitKey(i_physValRes.m_strUnitKey)
{
} // copy ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( CUnitGrp* i_pUnitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(m_pUnitGrp)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( CUnit* i_pUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( i_pUnit != nullptr )
    {
        m_pUnitGrp = i_pUnit->getUnitGroup();
        m_pUnit = i_pUnit;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( CUnitRatio* i_pUnitRatio, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(Ratio()),
    m_pUnit(i_pUnitRatio),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(DataQuantity()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( CPhysUnit* i_pPhysUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(i_pPhysUnit->getPhysSize()),
    m_pUnit(i_pPhysUnit),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strUnitKey, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey)
{
} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, CUnitGrp* i_pUnitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(m_pUnitGrp)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, CUnit* i_pUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( i_pUnit != nullptr )
    {
        m_pUnitGrp = i_pUnit->getUnitGroup();
        m_pUnit = i_pUnit;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, CUnitRatio* i_pUnitRatio, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_pUnitGrp(Ratio()),
    m_pUnit(i_pUnitRatio),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_pUnitGrp(DataQuantity()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, CPhysUnit* i_pPhysUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_pUnitGrp(i_pPhysUnit->getPhysSize()),
    m_pUnit(i_pPhysUnit),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( double i_fVal, const QString& i_strUnitKey, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(i_fVal),
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey)
{
} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, CUnitGrp* i_pUnitGrp, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(i_pUnitGrp),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(m_pUnitGrp)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, CUnit* i_pUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( i_pUnit != nullptr )
    {
        m_pUnitGrp = i_pUnit->getUnitGroup();
        m_pUnit = i_pUnit;
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, CUnitRatio* i_pUnitRatio, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(Ratio()),
    m_pUnit(i_pUnitRatio),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(DataQuantity()),
    m_pUnit(i_pUnitDataQuantity),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, CPhysUnit* i_pPhysUnit, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(i_pPhysUnit->getPhysSize()),
    m_pUnit(i_pPhysUnit),
    m_strUnitGrpKey(),
    m_strUnitKey()
{
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::CPhysValRes( const QString& i_strVal, const QString& i_strUnitKey, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_resType(i_resType),
    m_fVal(0.0),
    m_pUnitGrp(nullptr),
    m_pUnit(nullptr),
    m_strUnitGrpKey(),
    m_strUnitKey(i_strUnitKey)
{
    setVal(i_strVal);

} // ctor

//------------------------------------------------------------------------------
CPhysValRes::~CPhysValRes()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;
    m_pUnit = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValRes::isValid() const
//------------------------------------------------------------------------------
{
    return (m_fVal != 0.0);
}

//------------------------------------------------------------------------------
EResType CPhysValRes::type() const
//------------------------------------------------------------------------------
{
    return m_resType;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::setUnitGroup( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    m_pUnitGrp = i_pUnitGrp;

    if( m_pUnitGrp == nullptr )
    {
        m_pUnit = nullptr;
    }
    else if( m_pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnit = dynamic_cast<CPhysSize*>(m_pUnitGrp)->getSIUnit();
    }
    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // setUnitGroup

//------------------------------------------------------------------------------
void CPhysValRes::setUnitGroupKey( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != nullptr && m_pUnitGrp->getKey() != i_strUnitGrpKey )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CPhysVal::setUnitGroupKey";
        strArgs   = "UnitGrpKey: " + i_strUnitGrpKey;
        strErr    = "UnitGrpKey is different from key of already set unit group " + m_pUnitGrp->getKey();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }

    m_strUnitGrpKey = i_strUnitGrpKey;

} // setUnitGroupKey

//------------------------------------------------------------------------------
CUnitGrp* CPhysValRes::getUnitGroup() const
//------------------------------------------------------------------------------
{
    return m_pUnitGrp;
}

//------------------------------------------------------------------------------
QString CPhysValRes::getUnitGroupName( bool i_bInsertParentNames ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pUnitGrp == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = m_pUnitGrp->getName(i_bInsertParentNames);
    }
    return strName;

} // getUnitGroupName

//------------------------------------------------------------------------------
CPhysSize* CPhysValRes::getPhysSize() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CPhysSize*>(m_pUnitGrp);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::setUnit( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pUnit;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else if( m_pUnit->classType() == EUnitClassTypeRatios )
    {
        m_pUnitGrp = Ratio();
    }
    else if( m_pUnit->classType() == EUnitClassTypeDataQuantity )
    {
        m_pUnitGrp = DataQuantity();
    }
    else if( m_pUnit->classType() == EUnitClassTypePhysScienceFields )
    {
        m_pUnitGrp = dynamic_cast<CPhysUnit*>(m_pUnit)->getPhysSize();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // setUnit

//------------------------------------------------------------------------------
void CPhysValRes::setUnitRatio( CUnitRatio* i_pUnitRatio )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pUnitRatio;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else
    {
        m_pUnitGrp = Ratio();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // setUnitRatio

//------------------------------------------------------------------------------
void CPhysValRes::setUnitDataQuantity( CUnitDataQuantity* i_pUnitDataQuantity )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pUnitDataQuantity;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else
    {
        m_pUnitGrp = DataQuantity();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // setUnitDataQuantity

//------------------------------------------------------------------------------
void CPhysValRes::setPhysUnit( CPhysUnit* i_pPhysUnit )
//------------------------------------------------------------------------------
{
    m_pUnit = i_pPhysUnit;

    if( m_pUnit == nullptr )
    {
        m_pUnitGrp = nullptr;
    }
    else
    {
        m_pUnitGrp = i_pPhysUnit->getPhysSize();
    }

    if( m_pUnitGrp != nullptr )
    {
        m_strUnitGrpKey = m_pUnitGrp->getKey();
    }
    if( m_pUnit != nullptr )
    {
        m_strUnitKey = m_pUnit->getKey();
    }

} // setPhysUnit

//------------------------------------------------------------------------------
void CPhysValRes::setUnitKey( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( m_pUnit != nullptr && m_pUnit->getKey() != i_strUnitKey )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CPhysVal::setUnitKey";
        strArgs   = "UnitKey: " + i_strUnitKey;
        strErr    = "UnitKey is different from key of already set unit " + m_pUnit->getKey();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }

    m_strUnitKey = i_strUnitKey;

} // setUnitKey

//------------------------------------------------------------------------------
CUnit* CPhysValRes::getUnit() const
//------------------------------------------------------------------------------
{
    return m_pUnit;
}

//------------------------------------------------------------------------------
CUnitRatio* CPhysValRes::getUnitRatio() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitRatio*>(m_pUnit);
}

//------------------------------------------------------------------------------
CUnitDataQuantity* CPhysValRes::getUnitDataQuantity() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitDataQuantity*>(m_pUnit);
}

//------------------------------------------------------------------------------
CPhysUnit* CPhysValRes::getPhysUnit() const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CPhysUnit*>(m_pUnit);
}

//------------------------------------------------------------------------------
QString CPhysValRes::getUnitName( bool i_bInsertParentNames ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pUnit == nullptr )
    {
        strName = "NoUnit";
    }
    else
    {
        strName = m_pUnit->getName(i_bInsertParentNames);
    }
    return strName;

} // getUnitName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_pUnitGrp = nullptr;
    m_pUnit = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRes::setVal( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fVal = i_fVal;
}

//------------------------------------------------------------------------------
void CPhysValRes::setVal( double i_fVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    setUnit(i_pUnit);
    setVal(i_fVal);
}

//------------------------------------------------------------------------------
void CPhysValRes::setVal( double i_fVal, CUnitRatio* i_pUnitRatio )
//------------------------------------------------------------------------------
{
    setUnitRatio(i_pUnitRatio);
    setVal(i_fVal);
}

//------------------------------------------------------------------------------
void CPhysValRes::setVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity )
//------------------------------------------------------------------------------
{
    setUnitDataQuantity(i_pUnitDataQuantity);
    setVal(i_fVal);
}

//------------------------------------------------------------------------------
void CPhysValRes::setVal( double i_fVal, CPhysUnit* i_pPhysUnit )
//------------------------------------------------------------------------------
{
    setPhysUnit(i_pPhysUnit);
    setVal(i_fVal);
}

//------------------------------------------------------------------------------
EFormatResult CPhysValRes::setVal( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    EFormatResult formatResult = EFormatResultError;
    bool          bValOk = false;
    double        fVal = 0.0;
    CUnitGrp*     pUnitGrpVal = m_pUnitGrp;
    CUnit*        pUnitVal = m_pUnit;

    formatResult = parseValStr(i_strVal,&bValOk,&fVal,&pUnitGrpVal,&pUnitVal,nullptr,nullptr,nullptr,nullptr);

    if( !(formatResult & EFormatResultError) && bValOk )
    {
        m_fVal = fVal;
        m_pUnitGrp = pUnitGrpVal;
        m_pUnit = pUnitVal;
    }

    return formatResult;

} // setVal

//------------------------------------------------------------------------------
EFormatResult CPhysValRes::setVal( const QString& i_strVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    setUnit(i_pUnit);
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
EFormatResult CPhysValRes::setVal( const QString& i_strVal, CUnitRatio* i_pUnitRatio )
//------------------------------------------------------------------------------
{
    setUnitRatio(i_pUnitRatio);
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
EFormatResult CPhysValRes::setVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity )
//------------------------------------------------------------------------------
{
    setUnitDataQuantity(i_pUnitDataQuantity);
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
EFormatResult CPhysValRes::setVal( const QString& i_strVal, CPhysUnit* i_pPhysUnit )
//------------------------------------------------------------------------------
{
    setPhysUnit(i_pPhysUnit);
    return setVal(i_strVal);
}

//------------------------------------------------------------------------------
double CPhysValRes::getVal() const
//------------------------------------------------------------------------------
{
    return m_fVal;
}

//------------------------------------------------------------------------------
double CPhysValRes::getVal( const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    double       fVal = m_fVal;
    const CUnit* pUnit = i_pUnit;

    if( i_pUnit == nullptr )
    {
        pUnit = m_pUnit;
    }
    if( !areOfSameUnitGroup(m_pUnit,pUnit) )
    {
        QString strAddErrInfo = "Src:" + getUnitGroupName() + ", Dst:" + ZS::PhysVal::getUnitGroupName(i_pUnit);
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }
    if( m_pUnit != nullptr && pUnit != nullptr && pUnit != m_pUnit )
    {
        fVal = m_pUnit->convertValue(fVal,pUnit);
    }
    return fVal;
}

/*==============================================================================
public: // instance methods (to convert the value into a string)
==============================================================================*/

//------------------------------------------------------------------------------
QString CPhysValRes::toString( EUnitFind i_unitFind, int i_iSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    EFormatResult formatResult;
    QString       strVal;
    CUnit*        pUnit = m_pUnit;

    if( i_unitFind == EUnitFindBest )
    {
        formatResult = formatValue(
            /* fVal                       */ m_fVal,
            /* pUnitVal                   */ m_pUnit,
            /* iDigitsMantissaMax         */ -1,
            /* bDigitsAccuracyLimitsMant. */ true,
            /* iDigitsAccuracy            */ 2,
            /* iDigitsExponent            */ 0,
            /* bUseEngineeringFormat      */ false,
            /* pfVal                      */ nullptr,
            /* pstr                       */ &strVal,
            /* ppUnitVal                  */ &pUnit );
        if( formatResult != EFormatResultOk )
        {
            strVal = "";
        }
        removeTrailingZeros(&strVal);
    }
    else
    {
        formatResult = formatValue(
            /* fVal                       */ m_fVal,
            /* pUnitVal                   */ m_pUnit,
            /* iDigitsMantissaMax         */ -1,
            /* bDigitsAccuracyLimitsMant. */ true,
            /* iDigitsAccuracy            */ 2,
            /* iDigitsExponent            */ 0,
            /* bUseEngineeringFormat      */ false,
            /* pfVal                      */ nullptr,
            /* pstrVal                    */ &strVal );
        if( formatResult != EFormatResultOk )
        {
            strVal = "";
        }
        removeTrailingZeros(&strVal);
    }

    if( !(i_iSubStrVisibility & EPhysValSubStrVal) )
    {
        strVal = "";
    }
    if( m_pUnit != nullptr )
    {
        QString strUnitGrp;
        QString strUnit;

        if( i_iSubStrVisibility & EPhysValSubStrUnitMask )
        {
            strVal += " ";
        }
        if( i_iSubStrVisibility & EPhysValSubStrUnitGrp )
        {
            // Use "key" instead of "name(IncludingParentNames)" as the path contains the
            // root nodes (UnitClassType) whereas the key starts with the name of the science field.
            strUnitGrp = m_pUnit->getUnitGroup()->getKey();
        }
        if( i_iSubStrVisibility & EPhysValSubStrUnitSymbol )
        {
            strUnit = m_pUnit->getSymbol();
        }
        else if( i_iSubStrVisibility & EPhysValSubStrUnitName )
        {
            strUnit = m_pUnit->getName();
        }
        if( !strUnitGrp.isEmpty() )
        {
            strVal += strUnitGrp;
            if( !strUnit.isEmpty() )
            {
                strVal += CUnitsPool::GetNameSeparator();
            }
        }
        if( !strUnit.isEmpty() )
        {
            strVal += strUnit;
        }
    }
    return strVal;

} // toString

//------------------------------------------------------------------------------
QString CPhysValRes::toString( const CUnit* i_pUnit, int i_iSubStrVisibility ) const
//------------------------------------------------------------------------------
{
    QString strVal;

    if( i_pUnit == m_pUnit )
    {
        strVal = toString(EUnitFindNone,i_iSubStrVisibility);
    }
    else
    {
        double fVal = getVal(i_pUnit);

        EFormatResult formatResult = formatValue(
            /* fVal                       */ fVal,
            /* pUnitVal                   */ i_pUnit,
            /* iDigitsMantissaMax         */ -1,
            /* bDigitsAccuracyLimitsMant. */ true,
            /* iDigitsAccuracy            */ 2,
            /* iDigitsExponent            */ 0,
            /* bUseEngineeringFormat      */ false,
            /* pfVal                      */ nullptr,
            /* pstrVal                    */ &strVal );
        if( formatResult != EFormatResultOk )
        {
            strVal = "";
        }
        removeTrailingZeros(&strVal);

        if( !(i_iSubStrVisibility & EPhysValSubStrVal) )
        {
            strVal = "";
        }

        QString strUnitGrp;
        QString strUnit;

        if( i_iSubStrVisibility & EPhysValSubStrUnitMask )
        {
            strVal += " ";
        }
        if( i_iSubStrVisibility & EPhysValSubStrUnitGrp )
        {
            // Use "key" instead of "name(IncludingParentNames)" as the path contains the
            // root nodes (UnitClassType) whereas the key starts with the name of the science field.
            strUnitGrp = i_pUnit->getUnitGroup()->getKey();
        }
        if( i_iSubStrVisibility & EPhysValSubStrUnitSymbol )
        {
            strUnit = i_pUnit->getSymbol();
        }
        else if( i_iSubStrVisibility & EPhysValSubStrUnitName )
        {
            strUnit = i_pUnit->getName();
        }
        if( !strUnitGrp.isEmpty() )
        {
            strVal += strUnitGrp;
            if( !strUnit.isEmpty() )
            {
                strVal += CUnitsPool::GetNameSeparator();
            }
        }
        if( !strUnit.isEmpty() )
        {
            strVal += strUnit;
        }
    }
    return strVal;

} // toSring

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValRes::operator == ( const CPhysValRes& i_physValResOther ) const
//------------------------------------------------------------------------------
{
    if( m_strUnitGrpKey != i_physValResOther.m_strUnitGrpKey )
    {
        return false;
    }
    if( m_resType != i_physValResOther.m_resType )
    {
        return false;
    }

    double fValOther;

    if( m_pUnit != nullptr )
    {
        fValOther = i_physValResOther.getVal(m_pUnit);
    }
    else
    {
        fValOther = i_physValResOther.getVal();
    }
    if( m_fVal == fValOther )
    {
        return true;
    }
    return false;

} // operator ==

//------------------------------------------------------------------------------
bool CPhysValRes::operator != ( const CPhysValRes& i_physValResOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValResOther);

} // operator !=

//------------------------------------------------------------------------------
CPhysValRes& CPhysValRes::operator = ( const CPhysValRes& i_physValResNew )
//------------------------------------------------------------------------------
{
    m_resType       = i_physValResNew.m_resType;
    m_fVal          = i_physValResNew.m_fVal;
    m_pUnitGrp      = i_physValResNew.m_pUnitGrp;
    m_pUnit         = i_physValResNew.m_pUnit;
    m_strUnitGrpKey = i_physValResNew.m_strUnitGrpKey;
    m_strUnitKey    = i_physValResNew.m_strUnitKey;

    return *this;

} // operator =

//------------------------------------------------------------------------------
CPhysValRes CPhysValRes::operator + ( const CPhysValRes& i_physValResOp ) const
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValResOp.m_pUnitGrp )
    {
        CPhysValRes physValResThis(*this);
        CPhysValRes physValResOther(i_physValResOp);
        QString     strAddErrInfo = physValResThis.toString() + " + " + physValResOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    CPhysValRes physValResSum(*this);

    physValResSum += i_physValResOp;

    return physValResSum;

} // operator +

//------------------------------------------------------------------------------
CPhysValRes& CPhysValRes::operator += ( const CPhysValRes& i_physValResOp )
//------------------------------------------------------------------------------
{
    if( m_pUnitGrp != i_physValResOp.m_pUnitGrp )
    {
        CPhysValRes physValResThis(*this);
        CPhysValRes physValResOther(i_physValResOp);
        QString     strAddErrInfo = physValResThis.toString() + " += " + physValResOther.toString();
        throw CUnitConversionException( __FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo );
    }

    // Same unit group, so both units are either nullptr or not.
    if( m_pUnit == nullptr )
    {
        m_fVal += i_physValResOp.getVal();
    }
    else
    {
        CUnit* pUnitOp = i_physValResOp.getUnit();

        if( m_pUnit->isLogarithmic() && !pUnitOp->isLogarithmic() )
        {
            QString strAddErrInfo = "Val:" + m_pUnit->getName() + ", Op:" + pUnitOp->getName();
            throw CUnitConversionException( __FILE__, __LINE__, EResultMixedLinLogInMathOp, strAddErrInfo );
        }
        if( pUnitOp->isLogarithmic() )
        {
            double fFactor = pow( 10.0, i_physValResOp.getVal()/pUnitOp->getLogarithmicFactor() );

            m_fVal *= fFactor;
        }
        else
        {
            m_fVal += i_physValResOp.getVal(m_pUnit);
        }
    }
    return *this;

} // operator +=

//------------------------------------------------------------------------------
CPhysValRes CPhysValRes::operator * ( double i_fOp ) const
//------------------------------------------------------------------------------
{
    CPhysValRes physValResMul(*this);

    physValResMul.m_fVal *= i_fOp;

    return physValResMul;

} // operator *

//------------------------------------------------------------------------------
CPhysValRes& CPhysValRes::operator *= ( double i_fOp )
//------------------------------------------------------------------------------
{
    m_fVal *= i_fOp;

    return *this;

} // operator *=
