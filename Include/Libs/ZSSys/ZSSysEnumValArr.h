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

#ifndef ZSSys_EnumValArr_h
#define ZSSys_EnumValArr_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysEnumVal.h"

namespace ZS
{
namespace System
{
class CEnumeration;

//******************************************************************************
class ZSSYSDLL_API CEnumValArr
//******************************************************************************
{
public: // ctors and dtor
    CEnumValArr();
    CEnumValArr( int i_iValCount );
    CEnumValArr( CEnumeration* i_pEnum );
    CEnumValArr( CEnumeration* i_pEnum, int i_iValCount );
    CEnumValArr( CEnumeration* i_pEnum, int i_iValCount, int i_iEnumEntryIdx );
    CEnumValArr( CEnumeration* i_pEnum, int i_iValCount, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );  // if EEnumEntryAliasStrUndefined all string list entries will be compared
    CEnumValArr( CEnumeration* i_pEnum, int i_iValCount, const QVariant& i_valEnumEntry );
    CEnumValArr( CEnumeration* i_pEnum, const QVector<int>& i_ariEnumEntryIdx );
    CEnumValArr( CEnumeration* i_pEnum, const QStringList& i_strlstIdxNames, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );  // if EEnumEntryAliasStrUndefined all string list entries will be compared
    CEnumValArr( CEnumeration* i_pEnum, const QVector<QVariant>& i_arValsEnumEntries );
    CEnumValArr( int i_iValCount, const CEnumVal& i_enumVal );
    CEnumValArr( const QVector<CEnumVal>& i_arEnumVals );
    CEnumValArr( const QString& i_strEnumUId );
    CEnumValArr( const QString& i_strEnumUId, int i_iValCount );
    CEnumValArr( const QString& i_strEnumUId, int i_iValCount, int i_iEnumEntryIdx );
    CEnumValArr( const QString& i_strEnumUId, const QVector<int>& i_ariEnumEntryIdx );
    CEnumValArr( const CEnumValArr& i_other );
    virtual ~CEnumValArr();
public: // operators
    bool operator == ( const CEnumValArr& i_other ) const;
    bool operator != ( const CEnumValArr& i_other ) const;
public: // operators
    CEnumValArr& operator = ( const CEnumValArr& i_other );
    CEnumValArr& operator = ( const QVector<CEnumVal>& i_arEnumVals );
    CEnumValArr& operator = ( const QVector<int>& i_ariEnumEntryIdx );
    CEnumValArr& operator = ( const QStringList& i_strlstIdxNames );
    CEnumValArr& operator = ( const QVector<QVariant>& i_arValsEnumEntries );
public: // instance methods
    void setEnumeration( CEnumeration* i_pEnum );
    void setEnumerationUId( const QString& i_strUId );
    CEnumeration* getEnumeration() const { return m_pEnum; }
    QString getEnumerationName() const;
    QString getEnumerationUId() const { return m_strEnumUId; }
public: // instance methods
    void invalidateObjectReferences();
public: // instance methods
    int size() const { return m_ariEnumEntryIdx.size(); }
    void resize( int i_iSize );
public: // instance methods
    void clear();
public: // instance methods
    bool isValid( int i_idxVal ) const;
    bool isNull( int i_idxVal ) const;
    void setValidity( int i_idxVal, EValueValidity i_validity );
    EValueValidity getValidity( int i_idxVal ) const;
public: // instance methods
    void setValidities( const QVector<quint8>& i_arValdities );
    QVector<quint8> getValidities() const { return m_arValidities; }
public: // instance methods
    void setEnumEntryValidityByEnumEntryIndex( int i_idxVal, int i_iEnumEntryIdx, EEnumEntryValidity i_validity );
    void setEnumEntryValidityByEnumEntryName( int i_idxVal, const QString& i_strName, EEnumEntryValidity i_validity, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );
    void setEnumEntryValidityByEnumEntryVal( int i_idxVal, const QVariant& i_valEnumEntry, EEnumEntryValidity i_validity );
    void setEnumEntryValidities( int i_idxVal, const QVector<quint8> i_arValidities );
public: // instance methods
    EEnumEntryValidity getEnumEntryValidityByEnumEntryIndex( int i_idxVal, int i_iEnumEntryIdx ) const;
    EEnumEntryValidity getEnumEntryValidityByEnumEntryName( int i_idxVal, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined ) const;
    EEnumEntryValidity getEnumEntryValidityByEnumEntryVal( int i_idxVal, const QVariant& i_valEnumEntry ) const;
    QVector<quint8> getEnumEntryValidities( int i_idxVal ) const;
public: // instance methods (to set values)
    void setValByEnumEntryIndex( int i_idxVal, int i_iEnumEntryIdx );
    void setValByEnumEntryName( int i_idxVal, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );
    void setValByEnumEntryVal( int i_idxVal, const QVariant& i_valEnumEntry );
    void setVal( int i_idxVal, const CEnumVal& i_enumVal );
public: // instance methods (to set values)
    void insertValByEnumEntryIndex( int i_idxVal, int i_iEnumEntryIdx );
    void insertValByEnumEntryName( int i_idxVal, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );
    void insertValByEnumEntryVal( int i_idxVal, const QVariant& i_valEnumEntry );
    void insertVal( int i_idxVal, const CEnumVal& i_enumVal );
public: // instance methods (to set values)
    void appendValByEnumEntryIndex( int i_iEnumEntryIdx );
    void appendValByEnumEntryName( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );
    void appendValByEnumEntryVal( const QVariant& i_valEnumEntry );
    void appendVal( const CEnumVal& i_enumVal );
public: // instance methods (to remove values)
    void removeVal( int i_idxVal );
public: // instance methods
    void setValuesByEnumEntryIndex( int i_idxStart, int i_iValCount, int i_iEnumEntryIdx );
    void setValuesByEnumEntryName( int i_idxStart, int i_iValCount, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );
    void setValuesByEnumEntryVal( int i_idxStart, int i_iValCount, const QVariant& i_valEnumEntry );
    void setValues( int i_idxStart, int i_iValCount, const CEnumVal& i_enumVal );
public: // instance methods
    void setValuesByEnumEntryIndices( int i_idxStart, const QVector<int>& i_ariEnumEntryIdx );
    void setValuesByEnumEntryNames( int i_idxStart, const QStringList& i_strlstIdxNames, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive, int i_alias = ZS::System::EEnumEntryAliasStrUndefined );
    void setValuesByEnumEntryVals( int i_idxStart, const QVector<QVariant>& i_arValsEnumEntries );
    void setValues( int i_idxStart, const QVector<CEnumVal>& i_arEnumVals );
    void setValues( int i_idxStart, const CEnumValArr& i_enumValArr );
public: // instance methods (to remove values)
    void removeValues( int i_idxValStart, int i_iValCount );
public: // instance methods
    CEnumVal toEnumVal( int i_idxVal ) const;
public: // instance methods
    int getEnumEntryIndex( int i_idxVal ) const;
public: // instance methods (methods can only convert the index into the name or assigned value if the enumeration is known)
    QString enumEntryIndex2Name( int i_idxVal, int i_alias = ZS::System::EEnumEntryAliasStrName, EValueValidity* o_pValidity = nullptr ) const;
    QVariant enumEntryIndex2Val( int i_idxVal, EValueValidity* o_pValidity = nullptr ) const;
public: // instance methods (if the enumeration is not known the index value is converted into the string)
    QString toString( int i_idxVal, int i_alias = ZS::System::EEnumEntryAliasStrName ) const;
public: // instance methods (to get values)
    CEnumValArr mid( int i_idxStart, int i_iValCount ) const;
public: // instance methods (to get values)
    QVector<int> getEnumEntryIndices( int i_idxStart = 0, int i_iValCount = ZS::System::EArrayIndexCountAllElements ) const;
    QStringList enumEntryIndices2Names( int i_idxStart = 0, int i_iValCount = ZS::System::EArrayIndexCountAllElements, int i_alias = ZS::System::EEnumEntryAliasStrName ) const;
    QVector<QVariant> enumEntryIndices2ValsVec( int i_idxStart = 0, int i_iValCount = ZS::System::EArrayIndexCountAllElements ) const;
    QVector<CEnumVal> toEnumValsVec( int i_idxStart = 0, int i_iValCount = ZS::System::EArrayIndexCountAllElements ) const;
private: // operators (to avoid that anyone use the statement "arEnumVals[4] = 5;" as this does not assign a value to the enum array at index 4)
    CEnumValArr& operator [] ( int i_idxVal );
    const CEnumValArr& operator [] ( int i_idxVal ) const;
protected: // instance members
    // Please note that object references are only valid if the referenced objects
    // are living in the same thread context (and therefore within the same process
    // and same machine). If the value has to be exchanged between threads, processes
    // or to remote machines unique keys are necessary.
    CEnumeration*              m_pEnum;
    QString                    m_strEnumUId;
    QVector<int>               m_ariEnumEntryIdx;     // Current value (enum array index: 0 >= iEnumIdx < m_pEnum->count()).
    QVector<quint8>            m_arValidities;
    // Please note:
    // When compiling with gcc to avoid compiliation error:
    //   '>>' should be '> >' within a nested argument list.
    QVector< QVector<quint8> > m_ararEnumEntryValidities;

}; // class CEnumValArr

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_EnumValArr_h
