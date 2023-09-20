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

#ifndef ZSSys_EnumTemplate_h
#define ZSSys_EnumTemplate_h

#include <QtCore/qmutex.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

// The static arrays "CEnum<>::s_arEnumEntries" are defined in the cpp file.
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4661 )
#elif defined __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic pop
#endif


//namespace ZS
//{
//namespace System
//{
const int EInvalidEnumerator = -1;

//******************************************************************************
/*! @brief Template class for defining a user defined enumeration class.

    Writing conversion routines that convert enumerators into readable strings and
    strings in turn into enumerators of an enumeration is a constantly repeating task.
    The structure ZS::System::SEnumEntry already helps here. If this structure is
    used, all conversion routines look very similar.

    The template class CEnum has been implemented to save paperwork and
    to offer even more convenient uses with enumerations.

    This template class relieves you of writing the conversion routines.
    The initialization of the string conversion hash tables is also done implicitly
    by the template class. The use of the Enum template class also allows you to write
    a more easily readable code, since the template class offers a variety of useful
    methods for constructors and overloaded operators.

    The following steps are necessary to use the CEnum template class:

    1. Definition of the enumeration as "scoped" enumeration with the keyword class
       (as already done in our example enum class EProcessorClock)..

    2. If the enumeration is to be exported from a Dll, set an export instruction
       before defining the template in the header file:
       @code
       template class ZSSYSDLL_API CEnum<EProcessorClock>;
       @endcode

       @note
       ZSSYSDLL_APP must be replaced accordingly.

    3. A type definition in the header file to simplify the use of the implemented
       enum template class in the code:
       @code
       typedef CEnum<EProcessorClock> CEnumProcessorClock;
       @endcode

    4. In the source file a definition of the static class variables required for the
       conversion routines. On the one hand, this is a vector for the alias string hashes
       and the mutex for protecting access to the alias string hashes from different threads:
       @code
       QMutex CEnum<EProcessorClock>::s_mtxArMapsStr2Enumerators;
       QVector<QHash<QString, int>> CEnum<EProcessorClock>::s_armapsStr2Enumerators;
       @endcode

    5. Definition of the static enum entry array as QVector:
       @code
       const QVector<SEnumEntry> CEnum<EProcessorClock>::s_arEnumEntries =
       {   // idxRow,           Enumerator,                                Name,  Symbol, Text,                SCIPShort, SCPILong, Value
            SEnumEntry( static_cast<int>(EProcessorClock::Low),    "Low",    "L", "Low (5.12 MHz)",     "LOW",    "LOW",    5.12e6  ),
            SEnumEntry( static_cast<int>(EProcessorClock::Medium), "Medium", "M", "Medium (512.0 MHz)", "MED",    "MEDium", 512.0e6 ),
            SEnumEntry( static_cast<int>(EProcessorClock::High),   "High",   "H", "High (5.12 GHz)",    "HIGH",   "HIGH",   5.12e9  )
       };
       @endcode

    Steps 1 and 5 are also necessary when using the enum entry structure and the
    conversion routines to be implemented separately for each enumeration..

    If you now instantiate a template class derived for the enumeration, you can
    write the following code:

    @code
    CEnumProcessorClock eProcClock;

    for( eProcClock = 0; eProcClock < CEnumProcessorClock::count(); ++eProcClock )
    {
        pCmbProcClock->addItem(eProcClock.toString(), eProcClock.toValue());
    }

    eProcClock = "Low";

    if( eProcClock > "Low" )
    {
        eProcClock--;
    }
    @endcode

    @attention Concerning the comparison operators <, >, <=, >=.
    Please note that if this is NOT < than i_iEnumerator it does not implicitely
    mean that this is >= than i_iEnumerator (as this might be Invalid and Invalid
    is neither lower nor greater than any int value).

    @note The static method "toString( const QVariant& i_val, .. )" is not implemented on purpose.
    An integer value passed as first argument will be implicitly converted to QVariant.
    But in most case the programmer wanted to call "toString( E i_enumerator )" instead.
    "toString( const QVariant& i_val, ..)" is not really necessary. If desired
    the programmer may use "fromValue".  If "toString(int, ..)" is used you will get a
    compile error and you have to cast the int argument to the Enumeration class type.

    @see ZS::System::SEnumEntry
    @see _GRP_BasicConcepts_Enumerations
*/
template <typename E>
class /*ZSSYSDLL_API*/ CEnum
//******************************************************************************
{
public: // class methods
    static QString toString( E i_enumerator, int i_idxAlias = ZS::System::EEnumEntryAliasStrName );
    static E toEnumerator( const QString& i_strName, bool* o_pbOk = nullptr );
    static E toEnumerator( const QString& i_strName, int i_idxAlias, bool* o_pbOk = nullptr );
    static E toEnumerator( const QString& i_strName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk = nullptr );
    static E toEnumerator( const QVariant& i_val, bool* o_pbOk = nullptr );
    static QVariant toValue( E i_enumerator, bool* o_pbOk = nullptr );
    static QVariant toValue( E i_enumerator, QVariant::Type i_type, bool* o_pbOk = nullptr );
    static QVariant toValue( const QString& i_strName, bool* o_pbOk = nullptr );
    static QVariant toValue( const QString& i_strName, QVariant::Type i_type, bool* o_pbOk = nullptr );
    static QVariant toValue( const QString& i_strName, int i_idxAlias, bool* o_pbOk = nullptr );
    static QVariant toValue( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk = nullptr );
    static QVariant toValue( const QString& i_strName, QVariant::Type i_type, int i_idxAlias, bool* o_pbOk = nullptr );
    static QVariant toValue( const QString& i_strName, QVariant::Type i_type, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk = nullptr );
    static QVariant toValue( const QString& i_strName, QVariant::Type i_type, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk = nullptr );
public: // class methods
    static CEnum fromString( const QString& i_strName, bool* o_pbOk = nullptr );
    static CEnum fromString( const QString& i_strName, int i_idxAlias, bool* o_pbOk = nullptr );
    static CEnum fromString( const QString& i_strName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk = nullptr );
    static CEnum fromValue( const QVariant& i_val, bool* o_pbOk = nullptr );
public: // class methods
    static int count();
public: // ctors
    CEnum();
    CEnum( E i_enumerator );
    CEnum( int i_iEnumerator );
    CEnum( const char* i_szName, int i_idxAlias = ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive );
    CEnum( const QString& i_strName, int i_idxAlias = ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive );
    CEnum( const CEnum& i_other );
public: // assignment operators
    CEnum& operator = ( E i_enumerator );
    CEnum& operator = ( int i_iEnumerator );
    CEnum& operator = ( const char* i_szName );
    CEnum& operator = ( const QString& i_strName );
    CEnum& operator = ( const CEnum& i_other );
public: // compare operators with E
    bool operator == ( E i_enumerator ) const;
    bool operator != ( E i_enumerator ) const;
    bool operator < ( E i_enumerator ) const;
    bool operator > ( E i_enumerator ) const;
    bool operator <= ( E i_enumerator ) const;
    bool operator >= ( E i_enumerator ) const;
public: // compare operators with int
    bool operator == ( int i_iEnumerator ) const;
    bool operator != ( int i_iEnumerator ) const;
    bool operator < ( int i_iEnumerator ) const;
    bool operator > ( int i_iEnumerator ) const;
    bool operator <= ( int i_iEnumerator ) const;
    bool operator >= ( int i_iEnumerator ) const;
public: // compare operators with const char*
    bool operator == ( const char* i_szName ) const;
    bool operator != ( const char* i_szName ) const;
    bool operator < ( const char* i_szName ) const;
    bool operator > ( const char* i_szName ) const;
    bool operator <= ( const char* i_szName ) const;
    bool operator >= ( const char* i_szName ) const;
public: // compare operators with QString
    bool operator == ( const QString& i_strName ) const;
    bool operator != ( const QString& i_strName ) const;
    bool operator < ( const QString& i_strName ) const;
    bool operator > ( const QString& i_strName ) const;
    bool operator <= ( const QString& i_strName ) const;
    bool operator >= ( const QString& i_strName ) const;
public: // compare operators with other EnumTemplate instance
    bool operator == ( const CEnum& i_other ) const;
    bool operator != ( const CEnum& i_other ) const;
    bool operator < ( const CEnum& i_other ) const;
    bool operator > ( const CEnum& i_other ) const;
    bool operator <= ( const CEnum& i_other ) const;
    bool operator >= ( const CEnum& i_other ) const;
public: // increment/decrement operators
    CEnum& operator ++ (); // Prefix increment operator.
    CEnum& operator ++ (int); // Postfix increment operator.
    CEnum& operator -- (); // Prefix decrement operator.
    CEnum& operator -- (int); // Postfix decrement operator.
public: // instance methods
    E enumerator() const;
    int enumeratorAsInt() const;
    bool isValid() const;
    QString toString( int i_idxAlias = ZS::System::EEnumEntryAliasStrName ) const;
    QVariant toValue( QVariant::Type i_type = QVariant::Invalid, bool* o_pbOk = nullptr ) const;
private: // auxiliary class methods
    static void throwExceptionIfEnumeratorIsInvalid( ZS::System::EResult i_result, int i_iEnumerator, const QString& i_strMth );
public: // class members
    /*!< Array with enum entries. One for each enumerator. Must be defined for each concrete Enum Template instanziation. */
    static const QVector<ZS::System::SEnumEntry> s_arEnumEntries;
    /*!< Mutex to protect the string conversion hashes as long the initialization routine is in progress. */
    static QMutex s_mtxArMapsStr2Enumerators;
    /*!< String hash tables to speed up string to enumerator conversions. */
    static QVector<QHash<QString, int>> s_armapsStr2Enumerators;
private: // instance members
    /*!< Currently selected enumerator. */
    E m_enumerator;

}; // template class CEnum


/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Converts the given enumerator into a string.

    @param i_enumerator [in] Enumerator to be converted.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                        Selects one of the available strings.

    @return String representation of the given enumerator.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
QString CEnum<E>::toString(E i_enumerator, int i_idxAlias)
//------------------------------------------------------------------------------
{
    QString strMth = "toString(E " + QString::number(static_cast<int>(i_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumEntries.data(), s_arEnumEntries.count(), static_cast<int>(i_enumerator), i_idxAlias);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding enumerator.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @param i_strName [in] String to be converted into the enumerator.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumerator corresponding to the given string. Undefined if the conversion failed.
*/
template <typename E>
E CEnum<E>::toEnumerator( const QString& i_strName, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toEnumerator(i_strName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding enumerator.

    The search is case sensitive.

    @param i_strName [in] String to be converted into the enumerator.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumerator corresponding to the given string. Undefined if the conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
E CEnum<E>::toEnumerator( const QString& i_strName, int i_idxAlias, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toEnumerator(i_strName, i_idxAlias, Qt::CaseSensitive, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding enumerator.

    @param i_strName [in] String to be converted into the enumerator.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param i_caseSensitivity [in] Range [Qt::CaseSensitive, Qt::CaseInsensitive]
                        Defines whether the string comparison should be case sensitive.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumerator corresponding to the given string. Undefined if the conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
E CEnum<E>::toEnumerator( const QString& i_strName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    E enumerator = static_cast<E>(EInvalidEnumerator);
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, i_idxAlias, i_caseSensitivity, &bOk);
    if( bOk )
    {
        enumerator = static_cast<E>(iEnumerator);
    }
    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }
    return enumerator;
}

//------------------------------------------------------------------------------
/*! Converts the given real value into the corresponding enumerator.

    @param i_val [in] Real value to be converted into the enumerator.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumerator corresponding to the given real value. Undefined if the conversion failed.
*/
template <typename E>
E CEnum<E>::toEnumerator( const QVariant& i_val, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    E enumerator = static_cast<E>(EInvalidEnumerator);
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::val2Enumerator(s_arEnumEntries.data(), s_arEnumEntries.count(), i_val, &bOk);
    if( bOk )
    {
        enumerator = static_cast<E>(iEnumerator);
    }
    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }
    return enumerator;
}

//------------------------------------------------------------------------------
/*! Returns the real value of the given enumerator.

    @param i_enumerator [in] Enumerator to be converted.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
QVariant CEnum<E>::toValue( E i_enumerator, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_enumerator, QVariant::Invalid, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Returns the real value of the given enumerator.

    @param i_enumerator [in] Enumerator to be converted.
    @param i_type [in] Desired data type of the real value.
                       QVariant::Invalid if the value sould not be converted.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
QVariant CEnum<E>::toValue( E i_enumerator, QVariant::Type i_type, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    QString strMth = "toValue(E " + QString::number(static_cast<int>(i_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    return ZS::System::SEnumEntry::enumerator2Val(s_arEnumEntries.data(), s_arEnumEntries.count(), static_cast<int>(i_enumerator), i_type, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the real value of the enumerator.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @param i_strName [in] String to be converted into the real value.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.
*/
template <typename E>
QVariant CEnum<E>::toValue( const QString& i_strName, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, QVariant::Invalid, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the real value of the enumerator.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @param i_strName [in] String to be converted into the real value.
    @param i_type [in] Desired data type of the real value.
                       QVariant::Invalid if the value sould not be converted.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.
*/
template <typename E>
QVariant CEnum<E>::toValue( const QString& i_strName, QVariant::Type i_type, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, i_type, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the real value of the enumerator.

    The search is case sensitive.

    @param i_strName [in] String to be converted into the real value.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
QVariant CEnum<E>::toValue( const QString& i_strName, int i_idxAlias, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, QVariant::Invalid, i_idxAlias, Qt::CaseSensitive, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the real value of the enumerator.

    The given string will be compared with all available alias strings.

    @param i_strName [in] String to be converted into the real value.
    @param i_caseSensitivity [in] Range [Qt::CaseSensitive, Qt::CaseInsensitive]
                        Defines whether the string comparison should be case sensitive.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.
*/
template <typename E>
QVariant CEnum<E>::toValue( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, QVariant::Invalid, ZS::System::EEnumEntryAliasStrUndefined, i_caseSensitivity, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the real value of the enumerator.

    The search is case sensitive.

    @param i_strName [in] String to be converted into the real value.
    @param i_type [in] Desired data type of the real value.
                       QVariant::Invalid if the value sould not be converted.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
QVariant CEnum<E>::toValue( const QString& i_strName, QVariant::Type i_type, int i_idxAlias, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, i_type, i_idxAlias, Qt::CaseSensitive, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the real value of the enumerator.

    @param i_strName [in] String to be converted into the real value.
    @param i_type [in] Desired data type of the real value.
                       QVariant::Invalid if the value sould not be converted.
    @param i_caseSensitivity [in] Range [Qt::CaseSensitive, Qt::CaseInsensitive]
                        Defines whether the string comparison should be case sensitive.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.
*/
template <typename E>
QVariant CEnum<E>::toValue( const QString& i_strName, QVariant::Type i_type, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, i_type, ZS::System::EEnumEntryAliasStrUndefined, i_caseSensitivity, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the real value of the enumerator.

    @param i_strName [in] String to be converted into the real value.
    @param i_type [in] Desired data type of the real value.
                       QVariant::Invalid if the value sould not be converted.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param i_caseSensitivity [in] Range [Qt::CaseSensitive, Qt::CaseInsensitive]
                        Defines whether the string comparison should be case sensitive.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
QVariant CEnum<E>::toValue( const QString& i_strName, QVariant::Type i_type, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    QVariant val = ZS::System::SEnumEntry::str2Val(s_armapsStr2Enumerators, s_arEnumEntries.data(), s_arEnumEntries.count(), i_strName, i_type, i_idxAlias, i_caseSensitivity, &bOk);
    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }
    return val;
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding instance of the CEnum template class.

    The search is case sensitive.
    The given string will be compared with all available alias strings.

    @param i_strName [in] String to be converted into the enumerator.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumeration instance with the enumerator set to the given string. Invalid if the conversion failed.
*/
template <typename E>
CEnum<E> CEnum<E>::fromString( const QString& i_strName, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return fromString(i_strName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding instance of the CEnum template class.

    The search is case sensitive.

    @param i_strName [in] String to be converted into the enumerator.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumeration instance with the enumerator set to the given string. Invalid if the conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
CEnum<E> CEnum<E>::fromString( const QString& i_strName, int i_idxAlias, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return fromString(i_strName, i_idxAlias, Qt::CaseSensitive, o_pbOk);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding instance of the CEnum template class.

    @param i_strName [in] String to be converted into the enumerator.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param i_caseSensitivity [in] Range [Qt::CaseSensitive, Qt::CaseInsensitive]
                        Defines whether the string comparison should be case sensitive.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumeration instance with the enumerator set to the given string. Invalid if the conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
CEnum<E> CEnum<E>::fromString( const QString& i_strName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    CEnum enumVal;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, i_idxAlias, i_caseSensitivity, &bOk);
    if( bOk )
    {
        enumVal.m_enumerator = static_cast<E>(iEnumerator);
    }
    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }
    return enumVal;
}

//------------------------------------------------------------------------------
/*! Converts the given real value into the corresponding instance of the CEnum template class.

    @param i_val [in] Real value to be converted into the enumerator.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumeration instance with the enumerator corresonding to  the given real value. Invalid if the conversion failed.
*/
template <typename E>
CEnum<E> CEnum<E>::fromValue( const QVariant& i_val, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    CEnum enumVal;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::val2Enumerator(s_arEnumEntries.data(), s_arEnumEntries.count(), i_val, &bOk);
    if( bOk )
    {
        enumVal.m_enumerator = static_cast<E>(iEnumerator);
    }
    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }
    return enumVal;
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the number of enumerators of this enumeration.

    This corresponds to the number of defined enum entries.
*/
template <typename E>
int CEnum<E>::count()
//------------------------------------------------------------------------------
{
    return s_arEnumEntries.count();
}

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an invalid CEnum instance.
*/
template <typename E>
CEnum<E>::CEnum() :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
}

//------------------------------------------------------------------------------
/*! Constructs a CEnum instance with the given selected enumerator.

    @param i_enumerator [in] Enumerator to be set.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
CEnum<E>::CEnum( E i_enumerator ) :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
    QString strMth = "ctor(E " + QString::number(static_cast<int>(i_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    m_enumerator = i_enumerator;
}

//------------------------------------------------------------------------------
/*! Constructs a CEnum instance with the given selected enumerator.

    @param i_iEnumerator [in] Enumerator to be set.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
CEnum<E>::CEnum( int i_iEnumerator ) :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
    QString strMth = "ctor(E " + QString::number(i_iEnumerator) + ")";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, i_iEnumerator, strMth);
    m_enumerator = static_cast<E>(i_iEnumerator);
}

//------------------------------------------------------------------------------
/*! Constructs a CEnum instance by converting the given string into the enumerator.

    If the conversion fails the enumerator is set to Invalid.

    @param i_szName [in] String to be converted into the enumerator.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param i_caseSensitivity [in] Range [Qt::CaseSensitive, Qt::CaseInsensitive]
                                  Defines whether the string comparison should be case sensitive.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
CEnum<E>::CEnum( const char* i_szName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity ) :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, i_idxAlias, i_caseSensitivity, &bOk);
    if( bOk )
    {
        m_enumerator = static_cast<E>(iEnumerator);
    }
}

//------------------------------------------------------------------------------
/*! Constructs a CEnum instance by converting the given string into the enumerator.

    If the conversion fails the enumerator is set to Invalid.

    @param i_strName [in] String to be converted into the enumerator.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param i_caseSensitivity [in] Range [Qt::CaseSensitive, Qt::CaseInsensitive]
                                  Defines whether the string comparison should be case sensitive.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
template <typename E>
CEnum<E>::CEnum( const QString& i_strName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity ) :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, i_idxAlias, i_caseSensitivity, &bOk);
    if( bOk )
    {
        m_enumerator = static_cast<E>(iEnumerator);
    }
}

//------------------------------------------------------------------------------
/*! Copy constructor.
*/
template <typename E>
CEnum<E>::CEnum( const CEnum& i_other ) :
//------------------------------------------------------------------------------
    m_enumerator(i_other.m_enumerator)
{
}

/*==============================================================================
public: // assignment operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! Assigns the enumerator to this enum instance and returns a reference to this enum instance.

    @param i_enumerator [in] Enumerator to be set.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
CEnum<E>& CEnum<E>::operator = ( E i_enumerator )
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " = E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    m_enumerator = i_enumerator;
    return *this;
}

//------------------------------------------------------------------------------
/*! Assigns the enumerator to this enum instance and returns a reference to this enum instance.

    @param i_iEnumerator [in] Enumerator to be set.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
CEnum<E>& CEnum<E>::operator = ( int i_iEnumerator )
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " = E " + QString::number(i_iEnumerator);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, i_iEnumerator, strMth);
    m_enumerator = static_cast<E>(i_iEnumerator);
    return *this;
}

//------------------------------------------------------------------------------
/*! Converts the given string into the enumerator, assigns the resulting enumerator
    to this enum instance and returns a reference to this enum instance.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    If the conversion fails the instance is set to Invalid.

    @param i_szName [in] String to be converted.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
CEnum<E>& CEnum<E>::operator = ( const char* i_szName )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk )
    {
        m_enumerator = static_cast<E>(iEnumerator);
    }
    else
    {
        m_enumerator = static_cast<E>(EInvalidEnumerator);
    }
    return *this;
}

//------------------------------------------------------------------------------
/*! Converts the given string into the enumerator, assigns the resulting enumerator
    to this enum instance and returns a reference to this enum instance.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    If the conversion fails the instance is set to Invalid.

    @param i_strName [in] String to be converted.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
CEnum<E>& CEnum<E>::operator = ( const QString& i_strName )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk )
    {
        m_enumerator = static_cast<E>(iEnumerator);
    }
    else
    {
        m_enumerator = static_cast<E>(EInvalidEnumerator);
    }
    return *this;
}

//------------------------------------------------------------------------------
/*! Assigns other to this enum instance and return a reference to this enum instance.
*/
template <typename E>
CEnum<E>& CEnum<E>::operator = ( const CEnum& i_other )
//------------------------------------------------------------------------------
{
    m_enumerator = i_other.m_enumerator;
    return *this;
}

/*==============================================================================
public: // compare operators with E
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is equal to the given enumerator,
    otherwise returns false.
*/
template <typename E>
bool CEnum<E>::operator == ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    return( m_enumerator == i_enumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is not equal to the given enumerator,
    otherwise returns false.
*/
template <typename E>
bool CEnum<E>::operator != ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    return( m_enumerator != i_enumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower than the given enumerator,
    otherwise returns false.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
          - with Result = IdxOutOutOfRange if the other enumerator is out of range
*/
template <typename E>
bool CEnum<E>::operator < ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    QString strMth = "operator < E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    return( m_enumerator < i_enumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greater than the given enumerator,
    otherwise returns false.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
          - with Result = IdxOutOutOfRange if the other enumerator is out of range
*/
template <typename E>
bool CEnum<E>::operator > ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    QString strMth = "operator > E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    return( m_enumerator > i_enumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower or equal than the given enumerator,
    otherwise returns false.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
          - with Result = IdxOutOutOfRange if the other enumerator is out of range
*/
template <typename E>
bool CEnum<E>::operator <= ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    QString strMth = "operator <= E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    return( m_enumerator <= i_enumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greater or equal than the given enumerator,
    otherwise returns false.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
          - with Result = IdxOutOutOfRange if the other enumerator is out of range
*/
template <typename E>
bool CEnum<E>::operator >= ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    QString strMth = "operator >= E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    return( m_enumerator >= i_enumerator );
}

/*==============================================================================
public: // compare operators with int
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is equal to the given enumerator,
    otherwise returns false.
*/
/*!
*/
template <typename E>
bool CEnum<E>::operator == ( int i_iEnumerator ) const
//------------------------------------------------------------------------------
{
    return( static_cast<int>(m_enumerator) == i_iEnumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is not equal to the given enumerator,
    otherwise returns false.
*/
template <typename E>
bool CEnum<E>::operator != ( int i_iEnumerator ) const
//------------------------------------------------------------------------------
{
    return( static_cast<int>(m_enumerator) != i_iEnumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower than the given enumerator,
    otherwise returns false.

    This comparison operator is especially useful in loops in combination with
    the increment operator ++ to check whether the end of loop has been reached
    and the enum instance is no longer lower than the number of enumerators of the
    enumerations as returned by the static method CEnum<E>::count.

    @note If this is NOT lower than the given i_iEnumerator it does not implicitely
          mean that this is greater or equal than i_iEnumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any int value.
*/
template <typename E>
bool CEnum<E>::operator < ( int i_iEnumerator ) const
//------------------------------------------------------------------------------
{
    bool bResult = false;
    if( static_cast<int>(m_enumerator) >= 0 && i_iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) < i_iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greater than the given enumerator,
    otherwise returns false.

    @note If this is NOT greater than the given i_iEnumerator it does not implicitely
          mean that this is lower or equal than i_iEnumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any int value.
*/
template <typename E>
bool CEnum<E>::operator > ( int i_iEnumerator ) const
//------------------------------------------------------------------------------
{
    bool bResult = false;
    if( static_cast<int>(m_enumerator) >= 0 && i_iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) > i_iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower er equal than the
    given enumerator, otherwise returns false.

    @note If this is NOT lower or equal than the given i_iEnumerator it does not implicitely
          mean that this is greater than i_iEnumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any int value.
*/
template <typename E>
bool CEnum<E>::operator <= ( int i_iEnumerator ) const
//------------------------------------------------------------------------------
{
    bool bResult = false;
    if( static_cast<int>(m_enumerator) >= 0 && i_iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) <= i_iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greater or equal than the
    given enumerator, otherwise returns false.

    This comparison operator is especially useful in loops in combination with
    the decrement operator -- to check whether the end of loop has been reached
    and the enum instance was set to the lowest possible value 0 which is the
    first enumerator in the enumerations list of enumerators.

    @note If this is NOT greater or equal than the given i_iEnumerator it does
          not implicitely mean that this is lower than i_iEnumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any int value.
*/
template <typename E>
bool CEnum<E>::operator >= ( int i_iEnumerator ) const
//------------------------------------------------------------------------------
{
    bool bResult = false;
    if( static_cast<int>(m_enumerator) >= 0 && i_iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) >= i_iEnumerator;
    }
    return bResult;
}

/*==============================================================================
public: // compare operators with const char*
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is equal to the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.
*/
template <typename E>
bool CEnum<E>::operator == ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk )
    {
        bResult = (static_cast<int>(m_enumerator) == iEnumerator);
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is not equal to the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.
*/
template <typename E>
bool CEnum<E>::operator != ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_szName);
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower than the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @note If this is NOT lower than the other enumerator converted from the given string
          it does not implicitely mean that this is greater than the other enumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any other enumerator.
*/
template <typename E>
bool CEnum<E>::operator < ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk && static_cast<int>(m_enumerator) >= 0 && iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) < iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greater than the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @note If this is NOT greater than the other enumerator converted from the given string
          it does not implicitely mean that this is lower or equal than the other enumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any other enumerator.
*/
template <typename E>
bool CEnum<E>::operator > ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk && static_cast<int>(m_enumerator) >= 0 && iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) > iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower or equal than the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @note If this is NOT lower or equal than the other enumerator converted from the given string
          it does not implicitely mean that this is greater than the other enumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any other enumerator.
*/
template <typename E>
bool CEnum<E>::operator <= ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk && static_cast<int>(m_enumerator) >= 0 && iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) <= iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greater or equal than the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @note If this is NOT greater or equal than the other enumerator converted from the given string
          it does not implicitely mean that this is lower than the other enumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any other enumerator.
*/
template <typename E>
bool CEnum<E>::operator >= ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk && static_cast<int>(m_enumerator) >= 0 && iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) >= iEnumerator;
    }
    return bResult;
}

/*==============================================================================
public: // compare operators with QString
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is equal to the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.
*/
template <typename E>
bool CEnum<E>::operator == ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk )
    {
        bResult = (static_cast<int>(m_enumerator) == iEnumerator);
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is not equal to the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.
*/
template <typename E>
bool CEnum<E>::operator != ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_strName);
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower than the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @note If this is NOT lower than the other enumerator converted from the given string
          it does not implicitely mean that this is greater than the other enumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any other enumerator.
*/
template <typename E>
bool CEnum<E>::operator < ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk && static_cast<int>(m_enumerator) >= 0 && iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) < iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greater than the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @note If this is NOT greater than the other enumerator converted from the given string
          it does not implicitely mean that this is lower or equal than the other enumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any other enumerator.
*/
template <typename E>
bool CEnum<E>::operator > ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk && static_cast<int>(m_enumerator) >= 0 && iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) > iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower or equal than the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @note If this is NOT lower or equal than the other enumerator converted from the given string
          it does not implicitely mean that this is greater than the other enumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any other enumerator.
*/
template <typename E>
bool CEnum<E>::operator <= ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk && static_cast<int>(m_enumerator) >= 0 && iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) <= iEnumerator;
    }
    return bResult;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greator or equal than the enumerator
    converted from the given string.

    The search is case sensitive. The given string will be compared with all
    available alias strings.

    @note If this is NOT greater or equal than the other enumerator converted from the given string
          it does not implicitely mean that this is lower than the other enumerator.
          Both this enum instance or the enumerator to be compared with might be Invalid
          and an Invalid enumerator is neither lower nor greater than any other enumerator.
*/
template <typename E>
bool CEnum<E>::operator >= ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        ZS::System::SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = ZS::System::SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, ZS::System::EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
    if( bOk && static_cast<int>(m_enumerator) >= 0 && iEnumerator >= 0 )
    {
        bResult = static_cast<int>(m_enumerator) >= iEnumerator;
    }
    return bResult;
}

/*==============================================================================
public: // compare operators with other EnumTemplate instance
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is equal to the enumerator
    of the other enum instance, otherwise returns false.
*/
template <typename E>
bool CEnum<E>::operator == ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    return m_enumerator == i_other.m_enumerator;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is not equal to the enumerator
    of the other enum instance, otherwise returns false.
*/
template <typename E>
bool CEnum<E>::operator != ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    return m_enumerator != i_other.m_enumerator;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower than the enumerator
    of the other enum instance, otherwise returns false.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
          - with Result = IdxOutOutOfRange if the other enumerator is out of range
*/
template <typename E>
bool CEnum<E>::operator < ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " < Other(" + QString::number(static_cast<int>(i_other.m_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_other.m_enumerator), strMth);
    return m_enumerator < i_other.m_enumerator;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greater than the enumerator
    of the other enum instance, otherwise returns false.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
          - with Result = IdxOutOutOfRange if the other enumerator is out of range
*/
template <typename E>
bool CEnum<E>::operator > ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " > Other(" + QString::number(static_cast<int>(i_other.m_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_other.m_enumerator), strMth);
    return m_enumerator > i_other.m_enumerator;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is lower or equal than the enumerator
    of the other enum instance, otherwise returns false.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
          - with Result = IdxOutOutOfRange if the other enumerator is out of range
*/
template <typename E>
bool CEnum<E>::operator <= ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " <= Other(" + QString::number(static_cast<int>(i_other.m_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_other.m_enumerator), strMth);
    return m_enumerator <= i_other.m_enumerator;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is greator or equal than the enumerator
    of the other enum instance, otherwise returns false.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
          - with Result = IdxOutOutOfRange if the other enumerator is out of range
*/
template <typename E>
bool CEnum<E>::operator >= ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " >= Other(" + QString::number(static_cast<int>(i_other.m_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultIdxOutOfRange, static_cast<int>(i_other.m_enumerator), strMth);
    return m_enumerator >= i_other.m_enumerator;
}

/*==============================================================================
public: // increment/decrement operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! Prefix increment operator.

    This increment operator is especially useful in loops in combination with the
    comparison operator < with an integer value (count) as the other value to be
    compared with.

    @note After incrementing by one the enumerator may become invalid.
          If this enumerator is already invalid it may not be incremented anymore.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
*/
template <typename E>
CEnum<E>& CEnum<E>::operator ++ ()
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + "++";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    int iEnumerator = static_cast<int>(m_enumerator);
    ++iEnumerator;
    m_enumerator = static_cast<E>(iEnumerator);
    return *this;
}

//------------------------------------------------------------------------------
/*! Postfix increment operator.

    This increment operator is especially useful in loops in combination with the
    comparison operator < with an integer value (count) as the other value to be
    compared with.

    @note After incrementing by one the enumerator may become invalid.
          If this enumerator is already invalid it may not be incremented anymore.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
*/
template <typename E>
CEnum<E>& CEnum<E>::operator ++ (int)
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + "++";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    int iEnumerator = static_cast<int>(m_enumerator);
    iEnumerator++;
    m_enumerator = static_cast<E>(iEnumerator);
    return *this;
}

//------------------------------------------------------------------------------
/*! Prefix decrement operator.

    This decrement operator is especially useful in loops in combination with the
    comparison operator >= with an integer value (0) as the other value to be
    compared with.

    @note After incrementing by one the enumerator may become invalid.
          If this enumerator is already invalid it may not be incremented anymore.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
*/
template <typename E>
CEnum<E>& CEnum<E>::operator -- ()
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + "--";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    int iEnumerator = static_cast<int>(m_enumerator);
    --iEnumerator;
    m_enumerator = static_cast<E>(iEnumerator);
    return *this;
}

//------------------------------------------------------------------------------
/*! Postfix decrement operator.

    This decrement operator is especially useful in loops in combination with the
    comparison operator >= with an integer value (0) as the other value to be
    compared with.

    @note After incrementing by one the enumerator may become invalid.
          If this enumerator is already invalid it may not be incremented anymore.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
*/
template <typename E>
CEnum<E>& CEnum<E>::operator -- (int)
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + "--";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    int iEnumerator = static_cast<int>(m_enumerator);
    iEnumerator--;
    m_enumerator = static_cast<E>(iEnumerator);
    return *this;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the enumerator of this enum instance.
*/
template <typename E>
E CEnum<E>::enumerator() const
//------------------------------------------------------------------------------
{
    return m_enumerator;
}

//------------------------------------------------------------------------------
/*! Returns the enumerator of this enum instance as an integer value to avoid
    to use "static_cast<int>(enumerator())" if the enumerator is e.g. needed to
    address array elements.
*/
template <typename E>
int CEnum<E>::enumeratorAsInt() const
//------------------------------------------------------------------------------
{
    return static_cast<int>(m_enumerator);
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this enum instance is valid.

    A valid enumerator must be greater than 0 and lower than the number of
    enumerators within the enumeration.
*/
template <typename E>
bool CEnum<E>::isValid() const
//------------------------------------------------------------------------------
{
    return (static_cast<int>(m_enumerator) >= 0) && (static_cast<int>(m_enumerator) < s_arEnumEntries.count());
}

//------------------------------------------------------------------------------
/*! Returns the string representation of the enumerator.

    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                        Selects one of the available strings.
                        If Undefined is used the string will be compared with all possible strings.
                        If a valid index is used the passed string will only be compared with this alias string.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
*/
template <typename E>
QString CEnum<E>::toString( int i_idxAlias ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + ".toString";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumEntries.data(), s_arEnumEntries.count(), static_cast<int>(m_enumerator), i_idxAlias);
}

//------------------------------------------------------------------------------
/*! Returns the real value of the enumerator.

    @param i_type [in] Desired data type of the real value.
                       QVariant::Invalid if the value sould not be converted.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @note Throws an Exception
          - with Result = InvalidMethodCall if the enumerator of this enum instance is out of range
*/
template <typename E>
QVariant CEnum<E>::toValue( QVariant::Type i_type, bool* o_pbOk ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + ".toString";
    throwExceptionIfEnumeratorIsInvalid(ZS::System::EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    return ZS::System::SEnumEntry::enumerator2Val(s_arEnumEntries.data(), s_arEnumEntries.count(), static_cast<int>(m_enumerator), i_type, o_pbOk);
}

/*==============================================================================
private: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Throws an exception if the given enumerator is out of range.

    An ErrResultInfo structure will be created with the given result code.
    The additional error info will contain the class name of the enumeration
    and a text desribing the current enumerator and the valid range.

    @param i_result [in] Error result code.
    @param i_iEnumerator [in] Enumerator to be checked against the valid enumeration range.
    @param i_strMth [in] The method name of the error source will be set to this method name.
*/
template <typename E>
void CEnum<E>::throwExceptionIfEnumeratorIsInvalid( ZS::System::EResult i_result, int i_iEnumerator, const QString& i_strMth )
//------------------------------------------------------------------------------
{
    if( i_iEnumerator < 0 || i_iEnumerator >= s_arEnumEntries.count() )
    {
        QString strAddErrInfo  = "Enumerator " + QString::number(i_iEnumerator);
        strAddErrInfo += " of enumeration " + QString(typeid(CEnum<E>).name());
        strAddErrInfo += " not in range [0.." + QString::number(s_arEnumEntries.count()-1) + "]";
        if( s_arEnumEntries.count() > 0 )
        {
            strAddErrInfo += "[";
            if( s_arEnumEntries[0].m_strlstNames.size() > 0 ) strAddErrInfo += s_arEnumEntries[0].m_strlstNames[0];
            if( s_arEnumEntries[s_arEnumEntries.count()-1].m_strlstNames.size() > 0 ) strAddErrInfo += ".." + s_arEnumEntries[s_arEnumEntries.count()-1].m_strlstNames[0];
            strAddErrInfo += "]";
        }
        ZS::System::SErrResultInfo errResultInfo(
            /* errSource  */ "ZS::System", "CEnum", "", i_strMth,
            /* result     */ i_result,
            /* severity   */ ZS::System::EResultSeverityCritical,
            /* strAddInfo */ strAddErrInfo );
        throw ZS::System::CException(__FILE__, __LINE__, errResultInfo);
    }
}

//} // namespace System

//} // namespace ZS

#ifdef _WINDOWS
#pragma warning( pop )
#endif


#endif // #ifndef ZSSys_EnumTemplate_h
