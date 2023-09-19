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

#ifndef ZSSys_Enumeration_h
#define ZSSys_Enumeration_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

namespace ZS
{
namespace System
{
//******************************************************************************
/*! The class is a wrapper around a C++ enumeration.

The class manages a list with the defined enum index entries and offers useful
functions for enums that a normal definition of C enums does not offer.

Enumerations are not only required to define symbolic constants for a 1 out of M
selection and thus to make the source code more readable. It is also often necessary
to display the enum value as a readable string within a GUI or to transmit it in streams.

Furthermore, an enum value is used to select from a list of other values with a
non-integer data type. Possibly to select a frequency from a frequency list:

    enum EUKWRadioFreqs {
        EUKWRadioFreq_Bayern1,  // 92.4 MHz
        EUKWRadioFreq_Bayern3,  // 99.2 MHz
        ....
    };

Furthermore, an enum value is used to select from a list of other values with
a non-integer data type. Possibly for selection So that an enumeration can be
accessed from any position, all enumerations are managed in the
ZS::System::CEnumerationIdxTree class. For this, the combination of NameSpace
and name of the enumeration must be unique within an application.

So that the class can also be used within a graphical table editor, methods are
provided to dynamically add and remove enum entries. However, such dynamically
generated enum values cannot be used in C constructs such as switch cases.
*/
class ZSSYSDLL_API CEnumeration : public CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CEnumeration"; }
public: // ctors and dtor
    CEnumeration( const QString& i_strNameSpace, const QString& i_strEnumName );
    CEnumeration( const QString& i_strNameSpace, const QString& i_strEnumName, const SEnumEntry* i_pEnumArr, int i_iArrLen );
    //CEnumeration( const CEnumeration& i_other );
    ~CEnumeration();
public: // operators
    //CEnumeration& operator = ( const CEnumeration& i_other );
public: // operators
    bool operator == ( const CEnumeration& i_other ) const;
    bool operator != ( const CEnumeration& i_other ) const;
public: // instance methods
    CEnumeration* clone() const;
public: // instance methods
    QString getNameSpace() const;
    QString getName() const;
public: // instance methods
    // "idxRow" is the index of the entry within the array m_arpEntries and !! NOT !! the enum entry index "entry.m_idx".
    // To get the enum entry by its index you must use "index2EnumEntry".
    int count() const;  // size of m_pEnum->m_arpEntries (number of rows)
    SEnumEntry* getEntry( int i_idxRow ) const;
    EEnumEntryValidity getEnumEntryValidity( int i_idxRow ) const;
    void removeEntry( int i_idxRow );
public: // instance methods
    QVector<SEnumEntry*> getEntries() const { return m_arpEntries; }
    void setEntries( const SEnumEntry* i_pEnumArr, int i_iArrLen );
public: // instance methods
    int getMaxEnumEntryIndex() const;  // Not sizeof "m_pEnum->m_arpEntries" but the highest enumerator value.
    int getMaxEnumEntryStrAliasCount() const;
public: // instance methods
    SEnumEntry* index2EnumEntry( int i_idx ) const;
    QString index2Name( int i_idx, int i_alias = EEnumEntryAliasStrName ) const;
    QVariant index2Val( int i_idx ) const;
public: // instance methods
    SEnumEntry* name2EnumEntry(
        const QString&      i_strName,
        Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive,
        int                 i_alias = EEnumEntryAliasStrUndefined ) const; // if EEnumEntryAliasStrUndefined all string list entries will be compared
    int name2Index(
        const QString&      i_strName,
        Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive,
        int                 i_alias = EEnumEntryAliasStrUndefined ) const;
public: // instance methods
    SEnumEntry* val2EnumEntry( const QVariant& i_val ) const;
    int val2Index( const QVariant& i_val ) const;
public: // instance methods
    QString val2Name( const QVariant& i_val, int i_alias = EEnumEntryAliasStrName ) const;
    QVariant name2Val(
        const QString&      i_strName,
        Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive,
        int                 i_alias = EEnumEntryAliasStrUndefined ) const;
public: // instance methods (automatically resizing the enumeration if necessary)
    // The enum entry is inserted at "m_arpEntries[i_idx]" and entry index becomes equal to its row index.
    void setEntry( SEnumEntry* i_pEntry ); // The entry will not be copied but the ownership will be taken over.
    void setEntry( const SEnumEntry& i_entry );
    void setEntry(
        int                i_idx,  // IdxName becomes QString::number(i_idx)
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QString&     i_strAlias6,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QString&     i_strAlias9,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QString&     i_strAlias9,
        const QString&     i_strAlias10,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void setEntry(
        int                i_idx,
        const QStringList& i_strlstNames, // [0: IdxName, 1: Text, 2: Symbol, 3: SCPI, 4: IdxName5, .., IdxName10]
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
public: // instance methods (appending the enum entry at the end of "m_arpEntries")
    // If entry idx == -1 the entry idx will be set to the row index.
    // If entry idx >= 0 the entry idx remains unchanged and entry index might be different from row index.
    void appendEntry( SEnumEntry* i_pEntry ); // The entry will not be copied but the ownership will be taken over.
    void appendEntry( const SEnumEntry& i_entry );
    void appendEntry(
        int                i_idx,  // Name becomes QString::number(i_idx)
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QString&     i_strAlias9,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QString&     i_strAlias9,
        const QString&     i_strAlias10,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void appendEntry(
        int                i_idx,
        const QStringList& i_strlstNames, // [0: IdxName, 1: Text, 2: Symbol, 3: SCPI, 4: IdxName5, .., IdxName10]
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
public: // instance methods (inserting the enum entry at the specified row index into "m_arpEntries[idxRow]")
    // If entry idx == -1 the entry idx will be set to the row index.
    // If entry idx >= 0 the entry idx remains unchanged and entry index might be different from row index.
    void insertEntry( int i_idxRow, SEnumEntry* i_pEntry ); // The entry will not be copied but the ownership will be taken over.
    void insertEntry( int i_idxRow, const SEnumEntry& i_entry );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,  // IdxName becomes QString::number(i_idx)
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QString&     i_strAlias9,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCPILong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QString&     i_strAlias9,
        const QString&     i_strAlias10,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
    void insertEntry(
        int                i_idxRow,
        int                i_idx,
        const QStringList& i_strlstNames, // [0: IdxName, 1: Text, 2: Symbol, 3: SCPI, 4: IdxName5, .., IdxName10]
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = EEnumEntryValidityNormal );
public: // instance methods
    void removeEntryByIdx( int i_idx );
    void removeEntryByName(
        const QString&      i_strName,
        Qt::CaseSensitivity i_caseSensitivity = Qt::CaseSensitive,
        int                 i_alias = EEnumEntryAliasStrUndefined ); // if Undefined all string list entries will be compared
    void removeEntryByVal( const QVariant& i_val );
public: // instance methods
    void clear(); // keeps Name and UId
protected: // instance members
    QVector<SEnumEntry*> m_arpEntries;
    mutable bool         m_bMaxEnumEntryIndexDirty;
    mutable int          m_iMaxEnumEntryIndex;
    mutable bool         m_bMaxEnumEntryStrAliasCountDirty;
    mutable int          m_iMaxEnumEntryStrAliasCount;

}; // class CEnumeration

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Enumeration_h
