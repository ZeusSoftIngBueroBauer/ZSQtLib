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

#include "ZSSys/ZSSysEnumTemplate.h"

/*******************************************************************************
template <typename E> class CEnum
*******************************************************************************/

#if 0
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
QString ZS::System::CEnum<E>::toString(E i_enumerator, int i_idxAlias)
//------------------------------------------------------------------------------
{
    QString strMth = "toString(E " + QString::number(static_cast<int>(i_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    return SEnumEntry::enumerator2Str(s_arEnumEntries.data(), s_arEnumEntries.count(), static_cast<int>(i_enumerator), i_idxAlias);
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
E ZS::System::CEnum<E>::toEnumerator( const QString& i_strName, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toEnumerator(i_strName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, o_pbOk);
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
E ZS::System::CEnum<E>::toEnumerator( const QString& i_strName, int i_idxAlias, bool* o_pbOk )
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
E ZS::System::CEnum<E>::toEnumerator( const QString& i_strName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    E enumerator = static_cast<E>(EInvalidEnumerator);
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, i_idxAlias, i_caseSensitivity, &bOk);
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
E ZS::System::CEnum<E>::toEnumerator( const QVariant& i_val, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    E enumerator = static_cast<E>(EInvalidEnumerator);
    bool bOk = false;
    int iEnumerator = SEnumEntry::val2Enumerator(s_arEnumEntries.data(), s_arEnumEntries.count(), i_val, &bOk);
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
QVariant ZS::System::CEnum<E>::toValue( E i_enumerator, bool* o_pbOk )
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
QVariant ZS::System::CEnum<E>::toValue( E i_enumerator, QVariant::Type i_type, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    QString strMth = "toValue(E " + QString::number(static_cast<int>(i_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    return SEnumEntry::enumerator2Val(s_arEnumEntries.data(), s_arEnumEntries.count(), static_cast<int>(i_enumerator), i_type, o_pbOk);
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
QVariant ZS::System::CEnum<E>::toValue( const QString& i_strName, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, QVariant::Invalid, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, o_pbOk);
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
QVariant ZS::System::CEnum<E>::toValue( const QString& i_strName, QVariant::Type i_type, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, i_type, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, o_pbOk);
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
QVariant ZS::System::CEnum<E>::toValue( const QString& i_strName, int i_idxAlias, bool* o_pbOk )
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
QVariant ZS::System::CEnum<E>::toValue( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, QVariant::Invalid, EEnumEntryAliasStrUndefined, i_caseSensitivity, o_pbOk);
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
QVariant ZS::System::CEnum<E>::toValue( const QString& i_strName, QVariant::Type i_type, int i_idxAlias, bool* o_pbOk )
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
QVariant ZS::System::CEnum<E>::toValue( const QString& i_strName, QVariant::Type i_type, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return toValue(i_strName, i_type, EEnumEntryAliasStrUndefined, i_caseSensitivity, o_pbOk);
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
QVariant ZS::System::CEnum<E>::toValue( const QString& i_strName, QVariant::Type i_type, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    QVariant val = SEnumEntry::str2Val(s_armapsStr2Enumerators, s_arEnumEntries.data(), s_arEnumEntries.count(), i_strName, i_type, i_idxAlias, i_caseSensitivity, &bOk);
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
ZS::System::CEnum<E> ZS::System::CEnum<E>::fromString( const QString& i_strName, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    return fromString(i_strName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, o_pbOk);
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
ZS::System::CEnum<E> ZS::System::CEnum<E>::fromString( const QString& i_strName, int i_idxAlias, bool* o_pbOk )
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
ZS::System::CEnum<E> ZS::System::CEnum<E>::fromString( const QString& i_strName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    CEnum enumVal;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, i_idxAlias, i_caseSensitivity, &bOk);
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
ZS::System::CEnum<E> ZS::System::CEnum<E>::fromValue( const QVariant& i_val, bool* o_pbOk )
//------------------------------------------------------------------------------
{
    CEnum enumVal;
    bool bOk = false;
    int iEnumerator = SEnumEntry::val2Enumerator(s_arEnumEntries.data(), s_arEnumEntries.count(), i_val, &bOk);
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
int ZS::System::CEnum<E>::count()
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
ZS::System::CEnum<E>::CEnum() :
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
ZS::System::CEnum<E>::CEnum( E i_enumerator ) :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
    QString strMth = "ctor(E " + QString::number(static_cast<int>(i_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
    m_enumerator = i_enumerator;
}

//------------------------------------------------------------------------------
/*! Constructs a CEnum instance with the given selected enumerator.

    @param i_iEnumerator [in] Enumerator to be set.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range
*/
template <typename E>
ZS::System::CEnum<E>::CEnum( int i_iEnumerator ) :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
    QString strMth = "ctor(E " + QString::number(i_iEnumerator) + ")";
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, i_iEnumerator, strMth);
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
ZS::System::CEnum<E>::CEnum( const char* i_szName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity ) :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, i_idxAlias, i_caseSensitivity, &bOk);
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
ZS::System::CEnum<E>::CEnum( const QString& i_strName, int i_idxAlias, Qt::CaseSensitivity i_caseSensitivity ) :
//------------------------------------------------------------------------------
    m_enumerator(static_cast<E>(EInvalidEnumerator))
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, i_idxAlias, i_caseSensitivity, &bOk);
    if( bOk )
    {
        m_enumerator = static_cast<E>(iEnumerator);
    }
}

//------------------------------------------------------------------------------
/*! Copy constructor.
*/
template <typename E>
ZS::System::CEnum<E>::CEnum( const CEnum& i_other ) :
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator = ( E i_enumerator )
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " = E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator = ( int i_iEnumerator )
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " = E " + QString::number(i_iEnumerator);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, i_iEnumerator, strMth);
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator = ( const char* i_szName )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator = ( const QString& i_strName )
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator = ( const CEnum& i_other )
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
bool ZS::System::CEnum<E>::operator == ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    return( m_enumerator == i_enumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is not equal to the given enumerator,
    otherwise returns false.
*/
template <typename E>
bool ZS::System::CEnum<E>::operator != ( E i_enumerator ) const
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
bool ZS::System::CEnum<E>::operator < ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    QString strMth = "operator < E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
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
bool ZS::System::CEnum<E>::operator > ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    QString strMth = "operator > E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
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
bool ZS::System::CEnum<E>::operator <= ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    QString strMth = "operator <= E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
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
bool ZS::System::CEnum<E>::operator >= ( E i_enumerator ) const
//------------------------------------------------------------------------------
{
    QString strMth = "operator >= E " + QString::number(static_cast<int>(i_enumerator));
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_enumerator), strMth);
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
bool ZS::System::CEnum<E>::operator == ( int i_iEnumerator ) const
//------------------------------------------------------------------------------
{
    return( static_cast<int>(m_enumerator) == i_iEnumerator );
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is not equal to the given enumerator,
    otherwise returns false.
*/
template <typename E>
bool ZS::System::CEnum<E>::operator != ( int i_iEnumerator ) const
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
bool ZS::System::CEnum<E>::operator < ( int i_iEnumerator ) const
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
bool ZS::System::CEnum<E>::operator > ( int i_iEnumerator ) const
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
bool ZS::System::CEnum<E>::operator <= ( int i_iEnumerator ) const
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
bool ZS::System::CEnum<E>::operator >= ( int i_iEnumerator ) const
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
bool ZS::System::CEnum<E>::operator == ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator != ( const char* i_szName ) const
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
bool ZS::System::CEnum<E>::operator < ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator > ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator <= ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator >= ( const char* i_szName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_szName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator == ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator != ( const QString& i_strName ) const
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
bool ZS::System::CEnum<E>::operator < ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator > ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator <= ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator >= ( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( s_armapsStr2Enumerators.size() == 0 ) // Use mutex only if maps are not yet initialized.
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntries.data(), s_arEnumEntries.count(), s_armapsStr2Enumerators, &s_mtxArMapsStr2Enumerators);
    }
    bool bResult = false;
    bool bOk = false;
    int iEnumerator = SEnumEntry::str2Enumerator(s_armapsStr2Enumerators, i_strName, EEnumEntryAliasStrUndefined, Qt::CaseSensitive, &bOk);
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
bool ZS::System::CEnum<E>::operator == ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    return m_enumerator == i_other.m_enumerator;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this instance is not equal to the enumerator
    of the other enum instance, otherwise returns false.
*/
template <typename E>
bool ZS::System::CEnum<E>::operator != ( const CEnum& i_other ) const
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
bool ZS::System::CEnum<E>::operator < ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " < Other(" + QString::number(static_cast<int>(i_other.m_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_other.m_enumerator), strMth);
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
bool ZS::System::CEnum<E>::operator > ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " > Other(" + QString::number(static_cast<int>(i_other.m_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_other.m_enumerator), strMth);
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
bool ZS::System::CEnum<E>::operator <= ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " <= Other(" + QString::number(static_cast<int>(i_other.m_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_other.m_enumerator), strMth);
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
bool ZS::System::CEnum<E>::operator >= ( const CEnum& i_other ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + " >= Other(" + QString::number(static_cast<int>(i_other.m_enumerator)) + ")";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    throwExceptionIfEnumeratorIsInvalid(EResultIdxOutOfRange, static_cast<int>(i_other.m_enumerator), strMth);
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator ++ ()
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + "++";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator ++ (int)
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + "++";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator -- ()
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + "--";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
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
ZS::System::CEnum<E>& ZS::System::CEnum<E>::operator -- (int)
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + "--";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
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
E ZS::System::CEnum<E>::enumerator() const
//------------------------------------------------------------------------------
{
    return m_enumerator;
}

//------------------------------------------------------------------------------
/*! Returns true if the enumerator of this enum instance is valid.

    A valid enumerator must be greater than 0 and lower than the number of
    enumerators within the enumeration.
*/
template <typename E>
bool ZS::System::CEnum<E>::isValid() const
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
QString ZS::System::CEnum<E>::toString( int i_idxAlias ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + ".toString";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    return SEnumEntry::enumerator2Str(s_arEnumEntries.data(), s_arEnumEntries.count(), static_cast<int>(m_enumerator), i_idxAlias);
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
QVariant ZS::System::CEnum<E>::toValue( QVariant::Type i_type, bool* o_pbOk ) const
//------------------------------------------------------------------------------
{
    QString strMth = QString::number(static_cast<int>(m_enumerator)) + ".toString";
    throwExceptionIfEnumeratorIsInvalid(EResultInvalidMethodCall, static_cast<int>(m_enumerator), strMth);
    return SEnumEntry::enumerator2Val(s_arEnumEntries.data(), s_arEnumEntries.count(), static_cast<int>(m_enumerator), i_type, o_pbOk);
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
void ZS::System::CEnum<E>::throwExceptionIfEnumeratorIsInvalid( EResult i_result, int i_iEnumerator, const QString& i_strMth )
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
        SErrResultInfo errResultInfo(
            /* errSource  */ "ZS::System", "CEnum", "", i_strMth,
            /* result     */ i_result,
            /* severity   */ EResultSeverityCritical,
            /* strAddInfo */ strAddErrInfo );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }
}
#endif
