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

#ifndef ZSPhysVal_PhysValRes_h
#define ZSPhysVal_PhysValRes_h

#include <QtCore/qstring.h>
#include "ZSPhysVal/ZSPhysUnitsRatio.h"
#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"

namespace ZS
{
namespace PhysVal
{
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
public: // ctors and dtor
    CPhysValRes( EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, EResType i_resType = EResTypeResolution );
    CPhysValRes( const CPhysValRes& i_physValRes );
    CPhysValRes( CUnitGrp* i_pUnitGrp, EResType i_resType = EResTypeResolution );
    CPhysValRes( CUnit* i_pUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( CUnitRatio* i_pUnitRatio, EResType i_resType = EResTypeResolution );
    CPhysValRes( CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType = EResTypeResolution );
    CPhysValRes( CPhysUnit* i_pPhysUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strUnitKey, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, CUnitGrp* i_pUnitGrp, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, CUnit* i_pUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, CUnitRatio* i_pUnitRatio, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, CPhysUnit* i_pPhysUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( double i_fVal, const QString& i_strUnitKey, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, CUnitGrp* i_pUnitGrp, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, CUnit* i_pUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, CUnitRatio* i_pUnitRatio, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, CPhysUnit* i_pPhysUnit, EResType i_resType = EResTypeResolution );
    CPhysValRes( const QString& i_strVal, const QString& i_strUnitKey, EResType i_resType = EResTypeResolution );
    virtual ~CPhysValRes();
public: // instance methods
    bool isValid() const;
    EResType type() const;
public: // instance methods
    void setUnitGroup( CUnitGrp* i_pUnitGrp );
    void setUnitGroupKey( const QString& i_strUnitGrpKey );
    CUnitGrp* getUnitGroup() const;
    CPhysSize* getPhysSize() const;
    QString getUnitGroupName( bool i_bInsertParentNames = false ) const;
    QString getUnitGroupKey() const { return m_strUnitGrpKey; }
public: // instance methods
    void setUnit( CUnit* i_pUnit );
    void setUnitRatio( CUnitRatio* i_pUnitRatio );
    void setUnitDataQuantity( CUnitDataQuantity* i_pUnitDataQuantity );
    void setPhysUnit( CPhysUnit* i_pPhysUnit );
    void setUnitKey( const QString& i_strUnitKey );
    CUnit* getUnit() const;
    CUnitRatio* getUnitRatio() const;
    CUnitDataQuantity* getUnitDataQuantity() const;
    CPhysUnit* getPhysUnit() const;
    QString getUnitName( bool i_bInsertParentNames = false ) const;
    QString getUnitKey() const { return m_strUnitKey; }
public: // instance methods
    void invalidateObjectReferences();
public: // instance methods
    void setVal( double i_fVal );
    void setVal( double i_fVal, CUnit* i_pUnit );
    void setVal( double i_fVal, CUnitRatio* i_pUnitRatio );
    void setVal( double i_fVal, CUnitDataQuantity* i_pUnitDataQuantity );
    void setVal( double i_fVal, CPhysUnit* i_pPhysUnit );
    EFormatResult setVal( const QString& i_strVal );
    EFormatResult setVal( const QString& i_strVal, CUnit* i_pUnit );
    EFormatResult setVal( const QString& i_strVal, CUnitRatio* i_pUnitRatio );
    EFormatResult setVal( const QString& i_strVal, CUnitDataQuantity* i_pUnitDataQuantity );
    EFormatResult setVal( const QString& i_strVal, CPhysUnit* i_pPhysUnit );
    double getVal() const;
    double getVal( const CUnit* i_pUnit ) const;
public: // instance methods (to convert the value into a string)
    QString toString( EUnitFind i_unitFind = EUnitFindNone, int i_iSubStrVisibility = EPhysValSubStrVal ) const;
    QString toString( const CUnit* i_pUnit, int i_iSubStrVisibility = EPhysValSubStrVal ) const;
public: // operators
    bool operator == ( const CPhysValRes& i_physValResOther ) const;
    bool operator != ( const CPhysValRes& i_physValResOther ) const;
    CPhysValRes& operator = ( const CPhysValRes& i_physValResNew );
    CPhysValRes operator + ( const CPhysValRes& i_physValResOp ) const;
    CPhysValRes& operator += ( const CPhysValRes& i_physValResOp );
    CPhysValRes operator * ( double i_fOp ) const;
    CPhysValRes& operator *= ( double i_fOp );
protected: // instance members
    EResType  m_resType;
    double    m_fVal;
    // Please note that object references are only valid if the referenced objects
    // are living in the same thread context (and therefore within the same process
    // and same machine). If the value has to be exchanged between threads, processes
    // or to remote machines unique keys are necessary.
    CUnitGrp* m_pUnitGrp;
    CUnit*    m_pUnit;
    QString   m_strUnitGrpKey;
    QString   m_strUnitKey;

}; // class CPhysValRes

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysValRes_h
