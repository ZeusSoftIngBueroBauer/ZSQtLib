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

#ifndef ZSSys_EnumVal_h
#define ZSSys_EnumVal_h

#include <QtCore/qvariant.h>
#include <QtCore/qvector.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace System
{
class CEnumeration;

//******************************************************************************
class ZSSYSDLL_API CEnumVal
//******************************************************************************
{
public: // ctors and dtor
    CEnumVal();
    CEnumVal( CEnumeration* i_pEnum );
    CEnumVal( CEnumeration* i_pEnum, int i_iEnumEntryIdx );
    CEnumVal( CEnumeration* i_pEnum, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );    // if EEnumEntryAliasStrUndefined all string list entries will be compared
    CEnumVal( CEnumeration* i_pEnum, const QVariant& i_valEnumEntry );
    CEnumVal( const QString& i_strEnumUId );
    CEnumVal( const QString& i_strEnumUId, int i_iEnumEntryIdx );
    CEnumVal( const CEnumVal& i_other );
    ~CEnumVal();
public: // operators
    bool operator == ( const CEnumVal& i_other ) const;
    bool operator != ( const CEnumVal& i_other ) const;
public: // operators
    CEnumVal& operator = ( const CEnumVal& i_other );
public: // instance methods
    EValueValidity setEnumeration( CEnumeration* i_pEnum );
    void setEnumerationUId( const QString& i_strUId );
    CEnumeration* getEnumeration() const { return m_pEnum; }
    QString getEnumerationName() const;
    QString getEnumerationUId() const { return m_strEnumUId; }
public: // instance methods
    void invalidateObjectReferences();
public: // instance methods (using "EnumEntryIndex" to distinguish enum entry from enumeration as a collection of enum entries)
    EValueValidity setValByEnumEntryIndex( int i_iEnumEntryIdx );
    EValueValidity setValByEnumEntryName( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );  // if EEnumEntryAliasStrUndefined all string list entries will be compared
    EValueValidity setValByEnumEntryVal( const QVariant& i_valEnumEntry );
public: // instance methods
    int getEnumEntryIndex() const { return m_iEnumEntryIdx; }
public: // instance methods (methods can only convert the index into the name or assigned value if the enumeration is known)
    QString enumEntryIndex2Name( int i_alias = ZS::System::EEnumEntryAliasStrName, EValueValidity* o_pValidity = nullptr ) const;
    QVariant enumEntryIndex2Val( EValueValidity* o_pValidity = nullptr ) const;
public: // instance methods (if the enumeration is not known the index value is converted into the string)
    QString toString( int i_alias = ZS::System::EEnumEntryAliasStrName ) const;
public: // instance methods
    bool isValid() const { return (m_validity != EValueValidity::Invalid); }
    bool isNull() const { return (m_validity == EValueValidity::Null); }
    void setValidity( EValueValidity i_validity );
    EValueValidity getValidity() const { return m_validity; }
public: // instance methods
    void setEnumEntryValidityByEnumEntryIndex( int i_iEnumEntryIdx, EEnumEntryValidity i_validity );
    void setEnumEntryValidityByEnumEntryName( const QString& i_strName, EEnumEntryValidity i_validity, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );
    void setEnumEntryValidityByEnumEntryVal( const QVariant& i_valEnumEntry, EEnumEntryValidity i_validity );
    void setEnumEntryValidities( const QVector<quint8> i_arValidities );
public: // instance methods
    EEnumEntryValidity getEnumEntryValidityByEnumEntryIndex( int i_iEnumEntryIdx ) const;
    EEnumEntryValidity getEnumEntryValidityByEnumEntryName( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined ) const;
    EEnumEntryValidity getEnumEntryValidityByEnumEntryVal( const QVariant& i_valEnumEntry ) const;
    QVector<quint8> getEnumEntryValidities() { return m_arEnumEntryValidities; }
protected: // instance members
    // Please note that object references are only valid if the referenced objects
    // are living in the same thread context (and therefore within the same process
    // and same machine). If the value has to be exchanged between threads, processes
    // or to remote machines unique keys are necessary.
    CEnumeration*   m_pEnum;
    QString         m_strEnumUId;
    int             m_iEnumEntryIdx;    // Current value (enum array index: 0 >= iEnumIdx < m_pEnum->count()).
    EValueValidity  m_validity;
    QVector<quint8> m_arEnumEntryValidities;

}; // class CEnumVal

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_EnumVal_h
