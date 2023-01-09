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

#ifndef ZSPhysVal_PhysValArr_h
#define ZSPhysVal_PhysValArr_h

#include "ZSPhysVal/ZSPhysVal.h"

namespace ZS
{
namespace PhysVal
{
//******************************************************************************
class ZSPHYSVALDLL_API CPhysValArr
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CPhysValArr"; }
public: // ctors and dtor
    CPhysValArr( EResType i_resType = EResTypeResolution );
    CPhysValArr( CUnitGrp* i_pUnitGrp, EResType i_resType = EResTypeResolution );
    CPhysValArr( CUnit* i_pUnit, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( int i_iValCount, CUnitGrp* i_pUnitGrp, EResType i_resType = EResTypeResolution );
    CPhysValArr( int i_iValCount, CUnit* i_pUnit, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( int i_iValCount, const CPhysVal& i_physValInit );
    CPhysValArr( int i_iValCount, double i_fInitVal, CUnit* i_pUnit, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( int i_iValCount, double* i_arfVals, CUnit* i_pUnit, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( const QVector<double>& i_arfVals, CUnit* i_pUnit, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( const QStringList& i_strlstVals, CUnit* i_pUnit, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( const QString& i_strUnitKey, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( int i_iValCount, const QString& i_strUnitKey, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( int i_iValCount, double i_fInitVal, const QString& i_strUnitKey, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( int i_iValCount, double* i_arfVals, const QString& i_strUnitKey, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( const QVector<double>& i_arfVals, const QString& i_strUnitKey, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( const QStringList& i_strlstVals, const QString& i_strUnitKey, double i_fRes = 0.0, EResType i_resType = EResTypeResolution );
    CPhysValArr( const CPhysValArr& i_physValArr );
    virtual ~CPhysValArr();
public: // operators
    bool operator == ( const CPhysValArr& i_physValArrOther ) const;
    bool operator != ( const CPhysValArr& i_physValArrOther ) const;
public: // operators
    CPhysValArr& operator = ( const CPhysValArr& i_physValArrOther );
    CPhysValArr& operator = ( const QVector<CPhysVal>& i_arPhysValsOther );
    CPhysValArr& operator = ( const QVector<double>& i_arfValsOther );
    CPhysValArr& operator = ( const QStringList& i_strlstValsOther );
public: // instance methods
    int size() const { return m_arfValues.size(); }
    void resize( int i_iSize ) { m_arfValues.resize(i_iSize); }
public: // instance methods
    void clear();
public: // instance methods
    bool isValid() const;
    bool isNull() const;
    void setValidity( ZS::System::EValueValidity i_validity );
    ZS::System::EValueValidity getValidity() const;
public: // instance methods
    void setUnitGroup( CUnitGrp* i_pUnitGrp );
    void setUnitGroupKey( const QString& i_strUnitGrpKey );
    CUnitGrp* unitGroup() const;
    QString getUnitGroupName( bool i_bInsertParentNames = false ) const;
    QString getUnitGroupKey() const { return m_strUnitGrpKey; }
public: // instance methods
    void setUnit( CUnit* i_pUnit );
    void setUnitKey( const QString& i_strUnitKey );
    CUnit* unit() const;
    QString getUnitName( bool i_bInsertParentNames = false ) const;
    QString getUnitKey() const { return m_strUnitKey; }
public: // instance methods
    void invalidateObjectReferences();
public: // instance methods (to set values)
    void setVal( int i_idx, double i_fVal );
    void setVal( int i_idx, double i_fVal, CUnit* i_pUnit );
    void setVal( int i_idx, const CPhysVal& i_physVal );
public: // instance methods (to set values)
    void insertVal( int i_idx, double i_fVal );
    void insertVal( int i_idx, double i_fVal, CUnit* i_pUnit );
    void insertVal( int i_idx, const CPhysVal& i_physVal );
public: // instance methods (to set values)
    void appendVal( double i_fVal );
    void appendVal( double i_fVal, CUnit* i_pUnit );
    void appendVal( const CPhysVal& i_physVal );
public: // instance methods (to set values)
    void removeVal( int i_idx );
public: // instance methods (replacing all already existing values)
    void setValues( int i_idxStart, const CPhysValArr& i_physValArr );
public: // instance methods (replacing part of already existing values)
    void setValues( int i_idxStart, const QVector<double>& i_arfVals );
    void setValues( int i_idxStart, const QVector<double>& i_arfVals, CUnit* i_pUnit );
public: // instance methods (replacing all already existing values)
    void setValues( int i_idxStart, int i_iValCount, double* i_arfVals );
    void setValues( int i_idxStart, int i_iValCount, double* i_arfVals, CUnit* i_pUnit );
public: // instance methods (to remove values)
    void removeValues( int i_idxValStart, int i_iValCount );
public: // instance methods (to get values)
    CPhysVal getPhysVal( int i_idx ) const;
    CPhysVal getPhysVal( int i_idx, CUnit* i_pUnit ) const;
public: // instance methods (to get values)
    CPhysValArr mid( int i_idxValStart, int i_iValCount ) const;
    CPhysValArr mid( int i_idxValStart, int i_iValCount, CUnit* i_pUnit ) const;
public: // instance methods (to get values)
    double toDouble( int i_idx ) const;
    double toDouble( int i_idx, const CUnit* i_pUnit ) const;
public: // instance methods (to get values)
    QVector<double> toDoubleVec() const;
    QVector<double> toDoubleVec( const CUnit* i_pUnit ) const;
    QVector<double> toDoubleVec( int i_idxStart, int i_iValCount ) const;
    QVector<double> toDoubleVec( int i_idxStart, int i_iValCount, const CUnit* i_pUnit ) const;
public: // instance methods (to set values)
    void setVal( int i_idx, const QString& i_strVal );
    void setVal( int i_idx, const QString& i_strVal, CUnit* i_pUnit );
public: // instance methods (replacing all already existing values)
    void setValues( const QStringList& i_strlstVals );
    void setValues( const QStringList& i_strlstVals, CUnit* i_pUnit );
public: // instance methods (replacing part of already existing values)
    void setValues( int i_idxStart, const QStringList& i_strlstVals );
    void setValues( int i_idxStart, const QStringList& i_strlstVals, CUnit* i_pUnit );
public: // instance methods (to get values)
    QString toString( int i_idx ) const;
    QString toString( int i_idx, const CUnit* i_pUnit ) const;
public: // instance methods (to get values)
    QStringList toStringList() const;
    QStringList toStringList( const CUnit* i_pUnit ) const;
    QStringList toStringList( int i_idxValStart, int i_iValCount ) const;
    QStringList toStringList( int i_idxValStart, int i_iValCount, const CUnit* i_pUnit ) const;
public: // instance methods
    double* data() { return m_arfValues.data(); }
    const double* data() const { return m_arfValues.data(); }
public: // instance methods (to set the resolution of the values)
    void setRes( double i_fRes );
    void setRes( double i_fRes, CUnit* i_pUnit );
    void setRes( const CPhysValRes& i_physValRes );
public: // instance methods (to get the resolution of the values)
    bool hasRes() const;
    CPhysValRes getRes() const;
public: // instance methods (to convert the values into another unit)
    void convertValues( CUnit* i_pUnitDst );
private: // operators (to avoid that anyone use the statement "arPhysVal[4] = 5.0;" as this does not assign a value to the physical value array at index 4)
    CPhysVal& operator [] ( int i_idx );
    const CPhysVal& operator [] ( int i_idx ) const;
protected: // instance members
    // Please note that object references are only valid if the referenced objects
    // are living in the same thread context (and therefore within the same process
    // and same machine). If the value has to be exchanged between threads, processes
    // or to remote machines unique keys are necessary.
    CUnitGrp*       m_pUnitGrp; // could be either CUnitGrpRatio or CPhysSize
    CUnit*          m_pUnit;    // could be either CUnitRatio or CPhysUnit
    QString         m_strUnitGrpKey;
    QString         m_strUnitKey;
    ZS::System::EValueValidity  m_validity;
    CPhysValRes     m_physValRes;
    QVector<double> m_arfValues;

}; // class CPhysValArr

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysValArr_h
