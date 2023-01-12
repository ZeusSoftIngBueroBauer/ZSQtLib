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

#ifndef ZSPhysVal_PhysValRes_h
#define ZSPhysVal_PhysValRes_h

#include <QtCore/qstring.h>
#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSPhysVal/ZSPhysUnits.h"
//#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"

namespace ZS
{
namespace PhysVal
{
class CUnitGrp;
class CPhysSize;
class CUnit;
class CUnitRatio;
class CPhysUnit;

const QChar   c_chSymbolPlusMinus(0x00B1);
const QString c_strSymbolPlusMinus(c_chSymbolPlusMinus);

typedef enum
{
    EResTypeUndefined  = 0,
    EResTypeResolution = 1,
    EResTypeAccuracy   = 2,
    EResTypeCount
}   EResType;

//******************************************************************************
class ZSPHYSVALDLL_API CPhysValRes
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysValRes"; }
public: // ctors and dtor
    CPhysValRes( EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, EResType i_resType = EResTypeResolution );
    CPhysValRes( const CPhysValRes& i_physValRes );
    CPhysValRes( const CUnitGrp& i_unitGrp, EResType i_resType = EResTypeResolution );
    CPhysValRes( const CPhysSize& i_physSize, EResType i_resType = EResTypeResolution );
    CPhysValRes( const CUnit& i_unit, EResType i_resType = EResTypeResolution );
    CPhysValRes( const CUnitRatio& i_unitRatio, EResType i_resType = EResTypeResolution );
    //CPhysValRes( const CUnitDataQuantity& i_unitDataQuantity, EResType i_resType = EResTypeResolution );
    CPhysValRes( const CPhysUnit& i_physUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strUnitKey, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, const CUnitGrp& i_unitGrp, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, const CUnit& i_unit, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, const CUnitRatio& i_unitRatio, EResType i_resType = EResTypeResolution );
    //CPhysValRes( double i_fVal, const CUnitDataQuantity& i_unitDataQuantity, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, const CPhysUnit& i_physUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, const QString& i_strUnitKey, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, const CUnitGrp& i_unitGrp, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, const CUnit& i_unit, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, const CUnitRatio& i_unitRatio, EResType i_resType = EResTypeResolution );
    //CPhysValRes( const QString& i_strVal, const CUnitDataQuantity& i_unitDataQuantity, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, const CPhysUnit& i_physUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, const QString& i_strUnitKey, EResType i_resType = EResTypeResolution );
    virtual ~CPhysValRes();
public: // instance methods
    bool isValid() const;
    EResType type() const;
public: // instance methods
    void setUnitGroup( const CUnitGrp& i_unitGrp );
    void setUnitGroupKey( const QString& i_strUnitGrpKey );
    CUnitGrp unitGroup() const;
    CPhysSize physSize() const;
public: // instance methods
    void setUnit( const CUnit& i_unit );
    void setUnitRatio( const CUnitRatio& i_unitRatio );
    //void setUnitDataQuantity( const CUnitDataQuantity& i_unitDataQuantity );
    void setPhysUnit( const CPhysUnit& i_physUnit );
    void setUnitKey( const QString& i_strUnitKey );
    CUnit unit() const;
    CUnitRatio unitRatio() const;
    //CUnitDataQuantity* unitDataQuantity() const;
    CPhysUnit physUnit() const;
public: // instance methods
    void invalidateObjectReferences();
public: // instance methods
    void setVal( double i_fVal );
    void setVal( double i_fVal, const CUnit& i_unit );
    void setVal( double i_fVal, const CUnitRatio& i_unitRatio );
    //void setVal( double i_fVal, const CUnitDataQuantity& i_unitDataQuantity );
    void setVal( double i_fVal, const CPhysUnit& i_physUnit );
    TFormatResult setVal( const QString& i_strVal );
    TFormatResult setVal( const QString& i_strVal, const CUnit& i_unit );
    TFormatResult setVal( const QString& i_strVal, const CUnitRatio& i_unitRatio );
    //TFormatResult setVal( const QString& i_strVal, const CUnitDataQuantity& i_unitDataQuantity );
    TFormatResult setVal( const QString& i_strVal, const CPhysUnit& i_physUnit );
    double getVal() const;
    double getVal( const CUnit& i_unit ) const;
public: // instance methods (to convert the value into a string)
    QString toString( EUnitFind i_unitFind = EUnitFind::None, int i_iSubStrVisibility = PhysValSubStr::Val ) const;
    QString toString( const CUnit& i_unit, int i_iSubStrVisibility = PhysValSubStr::Val ) const;
public: // operators
    bool operator == ( const CPhysValRes& i_physValResOther ) const;
    bool operator != ( const CPhysValRes& i_physValResOther ) const;
    CPhysValRes& operator = ( const CPhysValRes& i_physValResNew );
    CPhysValRes operator + ( const CPhysValRes& i_physValResOp ) const;
    CPhysValRes& operator += ( const CPhysValRes& i_physValResOp );
    CPhysValRes operator * ( double i_fOp ) const;
    CPhysValRes& operator *= ( double i_fOp );
protected: // instance members
    EResType m_resType;
    double m_fVal;
    // Please note that object references are only valid if the referenced objects
    // are living in the same thread context (and therefore within the same process
    // and same machine). If the value has to be exchanged between threads, processes
    // or to remote machines unique keys are necessary.
    CUnitGrp m_unitGrp;
    CUnit m_unit;

}; // class CPhysValRes

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysValRes_h
