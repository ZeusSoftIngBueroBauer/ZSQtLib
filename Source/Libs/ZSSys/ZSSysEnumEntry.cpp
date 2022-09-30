/******************************************************************************

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

#include <QtCore/qmutex.h>

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
Enum EEnumEntryAliasStr
==============================================================================*/

//------------------------------------------------------------------------------
/*! Converts the given enumerator into a string.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @param i_alias [in] Enumerator to be converted.

    @return String representation of the given enumerator.
*/
QString ZS::System::enumEntryAlias2Str( EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_alias == EEnumEntryAliasStrUndefined ) str = "Undefined";
    else if( i_alias == EEnumEntryAliasStrName ) str = "Name";
    else if( i_alias == EEnumEntryAliasStrSymbol ) str = "Symbol";
    else if( i_alias == EEnumEntryAliasStrText ) str = "Text";
    else if( i_alias == EEnumEntryAliasStrSCPIShort ) str = "SCPIShort";
    else if( i_alias == EEnumEntryAliasStrSCPILong ) str = "SCPILong";
    else if( i_alias == EEnumEntryAliasStrAlias6 ) str = "Alias6";
    else if( i_alias == EEnumEntryAliasStrAlias7 ) str = "Alias7";
    else if( i_alias == EEnumEntryAliasStrAlias8 ) str = "Alias8";
    else if( i_alias == EEnumEntryAliasStrAlias9 ) str = "Alias9";
    else if( i_alias == EEnumEntryAliasStrAlias10 ) str = "Alias10";
    else str = "? (" + QString::number(i_alias) + ")";

    return str;

} // enumEntryAlias2Str

/*==============================================================================
Enum EEnumEntryValidity
==============================================================================*/

static const SEnumEntry s_arEnumStrValidities[] =
{                                                  // IdxName,     Symbol, Name,        SCIPShort, SCPILong
    /*  0 */ SEnumEntry( EEnumEntryValidityNormal,    "Normal",    "N",    "Normal",    "NORM",    "NORMal"    ),
    /*  1 */ SEnumEntry( EEnumEntryValidityDisabled,  "Disabled",  "D",    "Disabled",  "DIS",     "DISabled"  ),
    /*  2 */ SEnumEntry( EEnumEntryValidityInvalid,   "Invalid",   "I",    "Invalid",   "INV",     "INValid"   ),
    /*  3 */ SEnumEntry( EEnumEntryValidityCount,     "Count",     "",     "Count",     "COUN",    "COUNt"     ),
    /*  4 */ SEnumEntry( EEnumEntryValidityUndefined, "Undefined", "I",    "Undefined", "UND",     "UNDefined" )
};

//------------------------------------------------------------------------------
/*! Converts the given enumerator into a string.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @param i_validity [in] Enumerator to be converted.
    @param i_alias [in] Range [IdxName, Symbol, Text, SCPIShort, SCPILong].
                        Selects one of the available strings.

    @return String representation of the given enumerator.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range or
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QString ZS::System::enumEntryValidity2Str( EEnumEntryValidity i_validity, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrValidities, _ZSArrLen(s_arEnumStrValidities), i_validity, i_alias);
}

//------------------------------------------------------------------------------
/*! Converts the string into the corresponding enumerator.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @param i_str [in] String to be converted into the enumerator.
    @param i_alias [in] Range [IdxName, Symbol, Text, SCPIShort, SCPILong, Undefined].
                        Selects one of the available strings.
                        If Undefined is used the string will be compared with all possible strings.
                        If a valid index is used the passed string will only be compared with this alias string only.

    @return Enumerator corresponding to the given string. Undefined if the conversion failed.
*/
EEnumEntryValidity ZS::System::str2EnumEntryValidity( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    EEnumEntryValidity validity = EEnumEntryValidityUndefined;
    bool bOk = false;
    int iVal = SEnumEntry::str2Enumerator(s_arEnumStrValidities,_ZSArrLen(s_arEnumStrValidities),i_str,i_alias,Qt::CaseInsensitive, &bOk);
    if( bOk && iVal >= 0 && iVal < EEnumEntryValidityCount )
    {
        validity = static_cast<EEnumEntryValidity>(iVal);
    }
    return validity;

} // str2EnumEntryValidity


/*******************************************************************************
struct ZSSYSDLL_API SEnumEntry
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Converts the given enumerator into a string.

    @param i_pEnumArr [in] Pointer to array containing the enum entries.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
    @param i_iEnumerator [in] Enumerator to be converted.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                        Selects one of the available strings.

    @return String representation of the given enumerator.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range or
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QString SEnumEntry::enumerator2Str(
    const SEnumEntry* i_pEnumArr,
    int               i_iArrLen,
    int               i_iEnumerator,
    int               i_idxAlias )
//------------------------------------------------------------------------------
{
    QString strMth = "enumerator2Str";
    QString strAddErrInfo;

    if( i_iEnumerator < 0 || i_iEnumerator >= i_iArrLen )
    {
        strAddErrInfo  = "Enumerator " + QString::number(i_iEnumerator);
        strAddErrInfo += " not in range [0.." + QString::number(i_iArrLen-1) + "]";
        if( i_iArrLen > 0 )
        {
            strAddErrInfo += "[";
            if( i_pEnumArr[0].m_strlstNames.size() > 0 ) strAddErrInfo += i_pEnumArr[0].m_strlstNames[0];
            if( i_pEnumArr[i_iArrLen-1].m_strlstNames.size() > 0 ) strAddErrInfo += ".." + i_pEnumArr[i_iArrLen-1].m_strlstNames[0];
            strAddErrInfo += "]";
        }
        SErrResultInfo errResultInfo(
            /* errSource  */ NameSpace(), ClassName(), "", strMth,
            /* result     */ EResultIdxOutOfRange,
            /* severity   */ EResultSeverityCritical,
            /* strAddInfo */ strAddErrInfo );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_idxAlias < 0 )
    {
        strAddErrInfo  = "AliasIdx " + QString::number(i_idxAlias);
        strAddErrInfo += " out of range [0..N]";
        SErrResultInfo errResultInfo(
            /* errSource  */ NameSpace(), ClassName(), "", strMth,
            /* result     */ EResultArgOutOfRange,
            /* severity   */ EResultSeverityCritical,
            /* strAddInfo */ strAddErrInfo );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QString strName;

    const SEnumEntry* pEntry = &i_pEnumArr[i_iEnumerator];

    if( i_idxAlias >= 0 && i_idxAlias < pEntry->m_strlstNames.size() )
    {
        strName = pEntry->m_strlstNames[i_idxAlias];
    }
    else
    {
        strAddErrInfo  = "Enumerator " + pEntry->getName();
        strAddErrInfo += " does not have an alias string at index " + QString::number(i_idxAlias);
        SErrResultInfo errResultInfo(
            /* errSource  */ NameSpace(), ClassName(), pEntry->getName(), strMth,
            /* result     */ EResultArgOutOfRange,
            /* severity   */ EResultSeverityCritical,
            /* strAddInfo */ strAddErrInfo );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    return strName;

} // enumerator2Str

//------------------------------------------------------------------------------
/*! Returns the real value of the given enumerator.

    @param i_pEnumArr [in] Pointer to array containing the enum entries.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
    @param i_iEnumerator [in] Enumerator to be converted.
    @param i_type [in] Desired data type of the real value.
                       QVariant::Invalid if the value sould not be converted.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator. Invalid if conversion failed.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range or
*/
QVariant SEnumEntry::enumerator2Val(
    const SEnumEntry* i_pEnumArr,
    int               i_iArrLen,
    int               i_iEnumerator,
    QVariant::Type    i_type,
    bool*             o_pbOk )
//------------------------------------------------------------------------------
{
    QString strMth = "enumerator2Val";
    QString strAddErrInfo;

    if( i_iEnumerator < 0 || i_iEnumerator >= i_iArrLen )
    {
        strAddErrInfo  = "Enumerator " + QString::number(i_iEnumerator);
        strAddErrInfo += " not in range [0.." + QString::number(i_iArrLen-1) + "]";
        if( i_iArrLen > 0 )
        {
            strAddErrInfo += "[";
            if( i_pEnumArr[0].m_strlstNames.size() > 0 ) strAddErrInfo += i_pEnumArr[0].m_strlstNames[0];
            if( i_pEnumArr[i_iArrLen-1].m_strlstNames.size() > 0 ) strAddErrInfo += ".." + i_pEnumArr[i_iArrLen-1].m_strlstNames[0];
            strAddErrInfo += "]";
        }
        SErrResultInfo errResultInfo(
            /* errSource  */ NameSpace(), ClassName(), "", strMth,
            /* result     */ EResultIdxOutOfRange,
            /* severity   */ EResultSeverityCritical,
            /* strAddInfo */ strAddErrInfo );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QVariant val;

    bool bOk = false;

    const SEnumEntry* pEntry = &i_pEnumArr[i_iEnumerator];

    if( pEntry->m_val.isValid() )
    {
        val = pEntry->m_val;

        if( i_type != QVariant::Invalid )
        {
            bOk = val.canConvert(i_type);
        }
        else
        {
            bOk = true;
        }
    }

    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }

    return val;

} // enumerator2Val

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding enumerator.

    @param i_pEnumArr [in] Pointer to array containing the enum entries.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
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
int SEnumEntry::str2Enumerator(
    const SEnumEntry*   i_pEnumArr,
    int                 i_iArrLen,
    const QString&      i_strName,
    int                 i_idxAlias,
    Qt::CaseSensitivity i_caseSensitivity,
    bool*               o_pbOk )
//------------------------------------------------------------------------------
{
    QString strMth = "str2Enumerator";
    QString strAddErrInfo;

    int iEnumerator = -1;

    bool bOk = false;

    const SEnumEntry* pEntry;
    QString           strName;
    int               iAliasMin;
    int               iAliasMax;
    int               iAlias;

    for( int idxRow = 0; idxRow < i_iArrLen; idxRow++ )
    {
        pEntry = &i_pEnumArr[idxRow];

        if( i_idxAlias == EEnumEntryAliasStrUndefined )
        {
            iAliasMin = 0;
            iAliasMax = pEntry->m_strlstNames.size() - 1;
        }
        else
        {
            iAliasMin = i_idxAlias;
            iAliasMax = i_idxAlias;
        }

        if( iAliasMin >= 0 && iAliasMax < pEntry->m_strlstNames.size() )
        {
            for( iAlias = iAliasMin; iAlias <= iAliasMax; iAlias++ )
            {
                if( iAlias >= 0 && iAlias < pEntry->m_strlstNames.size() )
                {
                    strName = pEntry->m_strlstNames[iAlias];

                    if( i_strName.compare(strName,i_caseSensitivity) == 0 )
                    {
                        iEnumerator = idxRow;
                        bOk = true;
                        break;
                    }
                }
                else
                {
                    strAddErrInfo  = "Enumerator " + pEntry->getName();
                    strAddErrInfo += " does not have an alias string at index " + QString::number(iAlias);
                    SErrResultInfo errResultInfo(
                        /* errSource  */ NameSpace(), ClassName(), pEntry->getName(), strMth,
                        /* result     */ EResultListCorrupted,
                        /* severity   */ EResultSeverityCritical,
                        /* strAddInfo */ strAddErrInfo );
                    throw CException(__FILE__, __LINE__, errResultInfo);
                }
            }
        } // if( iAliasMin >= 0 && iAliasMax < pEntry->m_strlstNames.size() )

        if( bOk )
        {
            break;
        }
    } // for( idxRow = 0; idxRow < i_iArrLen; idxRow++ )

    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }

    return iEnumerator;

} // str2Enumerator

//------------------------------------------------------------------------------
/*! Converts the given string into the real value of the enumerator.

    @param i_pEnumArr [in] Pointer to array containing the enum entries.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
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
QVariant SEnumEntry::str2Val(
    const SEnumEntry*   i_pEnumArr,
    int                 i_iArrLen,
    const QString&      i_strName,
    QVariant::Type      i_type,
    int                 i_idxAlias,
    Qt::CaseSensitivity i_caseSensitivity,
    bool*               o_pbOk )
//------------------------------------------------------------------------------
{
    QVariant val;

    bool bOk = false;

    int iEnumerator = str2Enumerator(i_pEnumArr, i_iArrLen, i_strName, i_idxAlias, i_caseSensitivity, &bOk);

    if( bOk && iEnumerator >= 0 && iEnumerator < i_iArrLen )
    {
        const SEnumEntry* pEntry = &i_pEnumArr[iEnumerator];

        if( pEntry->m_val.isValid() )
        {
            val = pEntry->m_val;

            if( i_type != QVariant::Invalid )
            {
                bOk = val.canConvert(i_type);
            }
            else
            {
                bOk = true;
            }
        }
        else
        {
            bOk = false;
        }
    } // if( bOk && iEnumerator >= 0 && iEnumerator < i_iArrLen )

    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }

    return val;

} // str2Val

//------------------------------------------------------------------------------
/*! Converts the given real value into the corresponding enumerator.

    @param i_pEnumArr [in] Pointer to array containing the enum entries.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
    @param i_val [in] Real value to be converted into the enumerator.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumerator corresponding to the given real value. Undefined if the conversion failed.
*/
int SEnumEntry::val2Enumerator(
    const SEnumEntry* i_pEnumArr,
    int               i_iArrLen,
    const QVariant&   i_val,
    bool*             o_pbOk )
//------------------------------------------------------------------------------
{
    int iEnumerator = -1;

    const SEnumEntry* pEntry;

    bool bOk = false;

    for( int idxRow = 0; idxRow < i_iArrLen; idxRow++ )
    {
        pEntry = &i_pEnumArr[idxRow];

        if( pEntry->m_val.isValid() && i_val == pEntry->m_val )
        {
            iEnumerator = pEntry->m_iEnumerator;
            bOk = true;
            break;
        }
    }

    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }

    return iEnumerator;

} // val2Enumerator

//------------------------------------------------------------------------------
/*! Converts the given real value into the corresponding string representation of the enumerator.

    @param i_pEnumArr [in] Pointer to array containing the enum entries.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
    @param i_val [in] Real value to be converted into the enumerator.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                        Selects one of the available strings.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Enumerator corresponding to the given real value. Undefined if the conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QString SEnumEntry::val2Str(
    const SEnumEntry* i_pEnumArr,
    int               i_iArrLen,
    const QVariant&   i_val,
    int               i_idxAlias,
    bool*             o_pbOk )
//------------------------------------------------------------------------------
{
    QString strVal;

    bool bOk = false;

    int iEnumerator = val2Enumerator(i_pEnumArr, i_iArrLen, i_val, &bOk);

    if( bOk && iEnumerator >= 0 && iEnumerator < i_iArrLen )
    {
        // May throw an exception if there is no alias string.
        strVal = enumerator2Str(i_pEnumArr, i_iArrLen, iEnumerator, i_idxAlias);
    }

    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }

    return strVal;

} // val2Str

//------------------------------------------------------------------------------
/*! Checks whether the given enumerator is valid.

    @param i_pEnumArr [in] Pointer to array containing the enum entries.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
    @param i_iEnumerator [in] Enumerator to be checked.

    @return true if the Enumerator is in the valid range, false otherwise.
*/
bool SEnumEntry::isValidEnumerator(
    const SEnumEntry* i_pEnumArr,
    int               i_iArrLen,
    int               i_iEnumerator )
//------------------------------------------------------------------------------
{
    QString strMth = "isValidEnumerator";

    bool bOk = false;

    QString strAddErrInfo;

    const SEnumEntry* pEntry;

    if( i_iEnumerator >= 0 && i_iEnumerator < i_iArrLen )
    {
        pEntry = &i_pEnumArr[i_iEnumerator];

        if( pEntry->m_iEnumerator == i_iEnumerator )
        {
            bOk = true;
        }
    }

    return bOk;

} // isValidEnumerator

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! To speed up the routines converting strings into the enumerators string
    hash tables should be used. This method creates those maps.

    @param i_pEnumArr [in] Pointer to array containing the enum entries.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
    @param io_arMaps [in, out] String hash to be filled.
    @param i_pMtxMaps [in] If not nullptr the mutex will be locked so that no
                           other thread may access the string hashes as long as it is filled.
*/
void SEnumEntry::initStr2EnumeratorMaps(
    const SEnumEntry*             i_pEnumArr,
    int                           i_iArrLen,
    QVector<QHash<QString, int>>& io_arMaps,
    QMutex*                       i_pMtxMaps )
//------------------------------------------------------------------------------
{
    QString strMth = "initStr2EnumeratorMaps";
    QString strAddErrInfo;

    QMutexLocker mtxLocker(i_pMtxMaps);

    QString strName;
    QString strNameLowerCase;
    int     idxRow;
    int     iAlias;

    int iMaxAliases = 0;

    // Calculate maximum number of alias strings.
    for( idxRow = 0; idxRow < i_iArrLen; ++idxRow )
    {
        const SEnumEntry* pEntry = &i_pEnumArr[idxRow];

        if( pEntry->m_strlstNames.size() > iMaxAliases )
        {
            iMaxAliases = pEntry->m_strlstNames.size();
        }
    }

    io_arMaps.clear();
    io_arMaps.resize(iMaxAliases);

    for( idxRow = 0; idxRow < i_iArrLen; ++idxRow )
    {
        const SEnumEntry* pEntry = &i_pEnumArr[idxRow];

        if( pEntry->m_iEnumerator != idxRow )
        {
            strAddErrInfo  = "Enumerator " + QString::number(pEntry->m_iEnumerator);
            strAddErrInfo += " of " + pEntry->getName();
            strAddErrInfo += " does not correspond to row index " + QString::number(idxRow);
            SErrResultInfo errResultInfo(
                /* errSource  */ NameSpace(), ClassName(), pEntry->getName(), strMth,
                /* result     */ EResultListCorrupted,
                /* severity   */ EResultSeverityError,
                /* strAddInfo */ strAddErrInfo );
            throw CException(__FILE__, __LINE__, errResultInfo);
        }

        for( iAlias = 0; iAlias < pEntry->m_strlstNames.size(); ++iAlias )
        {
            strName = pEntry->m_strlstNames[iAlias];

            if( !strName.isEmpty() )
            {
                if( io_arMaps[iAlias].contains(strName) )
                {
                    strAddErrInfo  = "Alias string " + strName + " already used for enumerator " + QString::number(pEntry->m_iEnumerator);
                    SErrResultInfo errResultInfo(
                        /* errSource  */ NameSpace(), ClassName(), pEntry->getName(), strMth,
                        /* result     */ EResultListCorrupted,
                        /* severity   */ EResultSeverityCritical,
                        /* strAddInfo */ strAddErrInfo );
                    throw CException(__FILE__, __LINE__, errResultInfo);
                }

                io_arMaps[iAlias][strName] = pEntry->m_iEnumerator;

                strNameLowerCase = strName.toLower();

                if( !io_arMaps[iAlias].contains(strNameLowerCase) )
                {
                    io_arMaps[iAlias][strNameLowerCase] = pEntry->m_iEnumerator;
                }
            } // if( !strName.isEmpty() )
        }
    } // for( int idxRow = 0; idxRow < i_iArrLen; ++idxRow )

} // initStr2EnumeratorMaps

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding enumerator using the given string hash table.

    @param i_arMaps [in] Map with the string hash table.
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
int SEnumEntry::str2Enumerator(
    const QVector<QHash<QString, int>>& i_arMaps,
    const QString&                      i_strName,
    int                                 i_idxAlias,
    Qt::CaseSensitivity                 i_caseSensitivity,
    bool*                               o_pbOk )
//------------------------------------------------------------------------------
{
    QString strMth = "str2Enumerator";

    int iAliasMin = i_idxAlias;
    int iAliasMax = i_idxAlias;

    if( i_idxAlias == EEnumEntryAliasStrUndefined )
    {
        iAliasMin = 0;
        iAliasMax = i_arMaps.size() - 1;
    }
    else if( i_idxAlias < 0 || i_idxAlias >=  i_arMaps.size() )
    {
        SErrResultInfo errResultInfo(
            /* errSource  */ NameSpace(), ClassName(), "Maps", strMth,
            /* result     */ EResultArgOutOfRange,
            /* severity   */ EResultSeverityCritical,
            /* strAddInfo */ "Map does not contain alias strings at index " + QString::number(i_idxAlias) );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    int iEnumerator = -1;
    bool bOk = false;

    QString strNameLower;

    if( i_caseSensitivity == Qt::CaseInsensitive )
    {
        strNameLower = i_strName.toLower();
    }

    for( int iAlias = iAliasMin; iAlias <= iAliasMax; ++iAlias )
    {
        if( i_arMaps[iAlias].contains(i_strName) )
        {
            iEnumerator = i_arMaps[iAlias][i_strName];
            bOk = true;
            break;
        }
        else if( i_caseSensitivity == Qt::CaseInsensitive )
        {
            if( i_arMaps[iAlias].contains(strNameLower) )
            {
                iEnumerator = i_arMaps[iAlias][strNameLower];
                bOk = true;
                break;
            }
        }
    }

    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }

    return iEnumerator;

} // str2Enumerator

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding enumerator using the given string hash table.

    @param i_arMaps [in] Map with the string hash table.
    @param i_pEnumArr [in] Pointer to array containing the enum entries. This array is also
                           necessary as the string hash table returns the enumerator but
                           this enumerator has to be converted to the real value.
    @param i_iArrLen [in] Number of available enum entries (number of enumerators in the enumeration).
    @param i_strName [in] String to be converted into the enumerator.
    @param i_type [in] Desired data type of the real value.
                       QVariant::Invalid if the value sould not be converted.
    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.
                           If Undefined is used the string will be compared with all possible strings.
                           If a valid index is used the passed string will only be compared with this alias string.
    @param i_caseSensitivity [in] Range [Qt::CaseSensitive, Qt::CaseInsensitive]
                        Defines whether the string comparison should be case sensitive.
    @param o_pbOk [out] Result of the conversion if not nullptr.

    @return Real value of the given enumerator string. Invalid if the conversion failed.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QVariant SEnumEntry::str2Val(
    const QVector<QHash<QString, int>>& i_arMaps,
    const SEnumEntry*                   i_pEnumArr,
    int                                 i_iArrLen,
    const QString&                      i_strName,
    QVariant::Type                      i_type,
    int                                 i_idxAlias,
    Qt::CaseSensitivity                 i_caseSensitivity,
    bool*                               o_pbOk )
//------------------------------------------------------------------------------
{
    QString strMth = "str2Val";

    if( i_idxAlias != EEnumEntryAliasStrUndefined && (i_idxAlias < 0 || i_idxAlias >=  i_arMaps.size()) )
    {
        SErrResultInfo errResultInfo(
            /* errSource  */ NameSpace(), ClassName(), "Maps", strMth,
            /* result     */ EResultArgOutOfRange,
            /* severity   */ EResultSeverityCritical,
            /* strAddInfo */ "Map does not contain alias strings at index " + QString::number(i_idxAlias) );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QVariant val;

    bool bOk = false;

    int iEnumerator = str2Enumerator(i_arMaps, i_strName, i_idxAlias, i_caseSensitivity, &bOk);

    if( bOk && iEnumerator >= 0 && iEnumerator < i_iArrLen )
    {
        val = enumerator2Val(i_pEnumArr, i_iArrLen, iEnumerator, i_type, &bOk);

        if( val.isValid() )
        {
            if( i_type != QVariant::Invalid )
            {
                bOk = val.canConvert(i_type);
            }
            else
            {
                bOk = true;
            }
        }
        else
        {
            bOk = false;
        }
    }

    if( o_pbOk != nullptr )
    {
        *o_pbOk = bOk;
    }

    return val;

} // str2Val

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an empty (invalid) enum entry structure.
*/
SEnumEntry::SEnumEntry() :
//------------------------------------------------------------------------------
    m_iEnumerator(-1),
    m_strlstNames(),
    m_val(),
    m_validity(ZS::System::EEnumEntryValidityInvalid)
{
} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with one alias string.

    The string will be created from the enumerators initialisation value.

    @param i_iEnumerator [in] Enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append( QString::number(i_iEnumerator) );

} // ctor

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with one alias string.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] Name of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with two alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const char*        i_strAliasSymbol,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with three alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const char*        i_strAliasSymbol,
    const char*        i_strAliasText,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with five alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const char*        i_strAliasSymbol,
    const char*        i_strAliasText,
    const char*        i_strAliasSCPIShort,
    const char*        i_strAliasSCPILong,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with six alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const char*        i_strAliasSymbol,
    const char*        i_strAliasText,
    const char*        i_strAliasSCPIShort,
    const char*        i_strAliasSCPILong,
    const char*        i_strAlias6,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with seven alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_strAlias7 [in] Seventh alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const char*        i_strAliasSymbol,
    const char*        i_strAliasText,
    const char*        i_strAliasSCPIShort,
    const char*        i_strAliasSCPILong,
    const char*        i_strAlias6,
    const char*        i_strAlias7,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);
    m_strlstNames.append(i_strAlias7);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with eight alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_strAlias7 [in] Seventh alias string (user defined usage) of the enumerator.
    @param i_strAlias8 [in] Eighth alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const char*        i_strAliasSymbol,
    const char*        i_strAliasText,
    const char*        i_strAliasSCPIShort,
    const char*        i_strAliasSCPILong,
    const char*        i_strAlias6,
    const char*        i_strAlias7,
    const char*        i_strAlias8,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);
    m_strlstNames.append(i_strAlias7);
    m_strlstNames.append(i_strAlias8);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with nine alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_strAlias7 [in] Seventh alias string (user defined usage) of the enumerator.
    @param i_strAlias8 [in] Eighth alias string (user defined usage) of the enumerator.
    @param i_strAlias9 [in] Ninth alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const char*        i_strAliasSymbol,
    const char*        i_strAliasText,
    const char*        i_strAliasSCPIShort,
    const char*        i_strAliasSCPILong,
    const char*        i_strAlias6,
    const char*        i_strAlias7,
    const char*        i_strAlias8,
    const char*        i_strAlias9,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);
    m_strlstNames.append(i_strAlias7);
    m_strlstNames.append(i_strAlias8);
    m_strlstNames.append(i_strAlias9);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with ten alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_strAlias7 [in] Seventh alias string (user defined usage) of the enumerator.
    @param i_strAlias8 [in] Eighth alias string (user defined usage) of the enumerator.
    @param i_strAlias9 [in] Ninth alias string (user defined usage) of the enumerator.
    @param i_strAlias10 [in] Tenth alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const char*        i_strName,
    const char*        i_strAliasSymbol,
    const char*        i_strAliasText,
    const char*        i_strAliasSCPIShort,
    const char*        i_strAliasSCPILong,
    const char*        i_strAlias6,
    const char*        i_strAlias7,
    const char*        i_strAlias8,
    const char*        i_strAlias9,
    const char*        i_strAlias10,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);
    m_strlstNames.append(i_strAlias7);
    m_strlstNames.append(i_strAlias8);
    m_strlstNames.append(i_strAlias9);
    m_strlstNames.append(i_strAlias10);

} // ctor

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with one alias string.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] Name of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QString&     i_strName,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with two alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with three alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with four alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with six alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with seven alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_strAlias7 [in] Seventh alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);
    m_strlstNames.append(i_strAlias7);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with eight alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_strAlias7 [in] Seventh alias string (user defined usage) of the enumerator.
    @param i_strAlias8 [in] Eighth alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);
    m_strlstNames.append(i_strAlias7);
    m_strlstNames.append(i_strAlias8);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with nine alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_strAlias7 [in] Seventh alias string (user defined usage) of the enumerator.
    @param i_strAlias8 [in] Eighth alias string (user defined usage) of the enumerator.
    @param i_strAlias9 [in] Ninth alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QString&     i_strAlias9,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);
    m_strlstNames.append(i_strAlias7);
    m_strlstNames.append(i_strAlias8);
    m_strlstNames.append(i_strAlias9);

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with ten alias strings.

    @param i_iEnumerator [in] Enumerator.
    @param i_strName [in] First alias string (usually the name) of the enumerator.
    @param i_strAliasSymbol [in] Second alias string (usually symbolic name) of the enumerator.
    @param i_strAliasText [in] Third alias string (usually descriptive text) of the enumerator.
    @param i_strAliasSCPIShort [in] Fourth alias string (usually SCPI short version) of the enumerator.
    @param i_strAliasSCPILong [in] Fifth alias string (usually SCPI long version) of the enumerator.
    @param i_strAlias6 [in] Sixth alias string (user defined usage) of the enumerator.
    @param i_strAlias7 [in] Seventh alias string (user defined usage) of the enumerator.
    @param i_strAlias8 [in] Eighth alias string (user defined usage) of the enumerator.
    @param i_strAlias9 [in] Ninth alias string (user defined usage) of the enumerator.
    @param i_strAlias10 [in] Tenth alias string (user defined usage) of the enumerator.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
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
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(),
    m_val(i_val),
    m_validity(i_validity)
{
    m_strlstNames.append(i_strName);
    m_strlstNames.append(i_strAliasSymbol);
    m_strlstNames.append(i_strAliasText);
    m_strlstNames.append(i_strAliasSCPIShort);
    m_strlstNames.append(i_strAliasSCPILong);
    m_strlstNames.append(i_strAlias6);
    m_strlstNames.append(i_strAlias7);
    m_strlstNames.append(i_strAlias8);
    m_strlstNames.append(i_strAlias9);
    m_strlstNames.append(i_strAlias10);

} // ctor

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an enum entry structure with as many alias strings as desired.

    @param i_iEnumerator [in] Enumerator.
    @param i_strlstNames [in] String list with alias strings.
                             This list may contain more than 10 strings.
    @param i_val [in] Real value of the enumerator.
    @param i_validity [in] Validity of the enumerator.
*/
SEnumEntry::SEnumEntry(
    int                i_iEnumerator,
    const QStringList& i_strlstNames,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity ) :
//------------------------------------------------------------------------------
    m_iEnumerator(i_iEnumerator),
    m_strlstNames(i_strlstNames),
    m_val(i_val),
    m_validity(i_validity)
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Destroys an enum entry structure.
*/
SEnumEntry::~SEnumEntry()
//------------------------------------------------------------------------------
{
    m_iEnumerator = 0;
    //m_strlstNames.clear();
    m_val = 0;
    m_validity = static_cast<EEnumEntryValidity>(0);

} // dtor

/*==============================================================================
public: // methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the alias string at the given index.

    @param i_idxAlias [in] Range [depending on the configured enum entry array].
                           Selects one of the available strings.

    @return Desired alias string of the enumerator.

    @note Throws an Exception
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QString SEnumEntry::getName( int i_idxAlias ) const
//------------------------------------------------------------------------------
{
    QString strMth = "getName";

    QString strName;

    if( i_idxAlias >= 0 && i_idxAlias < m_strlstNames.size() )
    {
        strName = m_strlstNames[i_idxAlias];
    }
    else
    {
        strName = QString::number(m_iEnumerator);

        QString strAddErrInfo = "Enumerator " + getName() + " does not have an alias string at index " + QString::number(i_idxAlias);
        SErrResultInfo errResultInfo(
            /* errSource  */ NameSpace(), ClassName(), getName(), strMth,
            /* result     */ EResultListCorrupted,
            /* severity   */ EResultSeverityError,
            /* strAddInfo */ strAddErrInfo );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    return strName;

} // getName
