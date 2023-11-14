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

#ifndef ZSPhysVal_PhysValRes_h
#define ZSPhysVal_PhysValRes_h

#include "ZSPhysVal/ZSPhysUnit.h"

namespace ZS
{
namespace PhysVal
{
//******************************************************************************
class ZSPHYSVALDLL_API CPhysValRes
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysValRes"; }
public: // ctors and dtor
    CPhysValRes( EResType i_resType = EResType::Resolution );
    CPhysValRes( double i_fVal, EResType i_resType = EResType::Resolution );
    CPhysValRes( const CPhysValRes& i_physValRes );
    CPhysValRes( const CUnit& i_unit, EResType i_resType = EResType::Resolution );
    CPhysValRes( double i_fVal, const CUnit& i_unit, EResType i_resType = EResType::Resolution );
    CPhysValRes( const QString& i_strVal, const CUnit& i_unit, EResType i_resType = EResType::Resolution );
    virtual ~CPhysValRes();
public: // operators
    bool operator == ( const CPhysValRes& i_physValResOther ) const;
    bool operator != ( const CPhysValRes& i_physValResOther ) const;
    CPhysValRes& operator = ( const CPhysValRes& i_physValResNew );
    CPhysValRes operator + ( const CPhysValRes& i_physValResOp ) const;
    CPhysValRes& operator += ( const CPhysValRes& i_physValResOp );
    CPhysValRes operator * ( double i_fOp ) const;
    CPhysValRes& operator *= ( double i_fOp );
public: // instance methods
    bool isValid() const;
    EResType type() const;
public: // instance methods
    void setUnit( const CUnit& i_unit );
    CUnit unit() const;
public: // instance methods
    void invalidateObjectReferences();
public: // instance methods
    void setVal( double i_fVal );
    void setVal( double i_fVal, const CUnit& i_unit );
    TFormatResult setVal( const QString& i_strVal );
    TFormatResult setVal( const QString& i_strVal, const CUnit& i_unit );
    double getVal() const;
    double getVal( const CUnit& i_unit ) const;
public: // instance methods (to convert the value into a string)
    QString toString( EUnitFind i_unitFind = EUnitFind::None, int i_iSubStrVisibility = PhysValSubStr::Val ) const;
    QString toString( const CUnit& i_unit, int i_iSubStrVisibility = PhysValSubStr::Val ) const;
public: // instance methods (to convert the value into another unit)
    void convertValue( const CUnit& i_unitDst );
protected: // instance members
    EResType m_resType;
    double m_fVal;
    CUnit m_unit;

}; // class CPhysValRes

Q_DECLARE_METATYPE(CPhysValRes);

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysValRes_h
