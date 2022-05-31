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

#ifndef ZSSys_EnumEntry_h
#define ZSSys_EnumEntry_h

//#include <QtCore/qhash.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qvariant.h>
#include <QtCore/qvector.h>

#include "ZSSys/ZSSysDllMain.h"


class QMutex;


namespace ZS
{
namespace System
{
//==============================================================================
/*! Predefined index values for selecting a string representation of an enum value.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
*/
enum EEnumEntryAliasStr
//==============================================================================
{
    EEnumEntryAliasStrUndefined = -1,   /*!< Which string is to be used is not specified and depends on the conversion routine called. */
    EEnumEntryAliasStrName      =  0,   /*!< Select name of enumerator (eg "Normal"). */
    EEnumEntryAliasStrSymbol    =  1,   /*!< Select symbol string of enum index (eg "N"). */
    EEnumEntryAliasStrText      =  2,   /*!< Select text string of enum index (eg "Normal range"). */
    EEnumEntryAliasStrSCPIShort =  3,   /*!< Select SCPI short string of enum index (eg "NORM"). */
    EEnumEntryAliasStrSCPILong  =  4,   /*!< Select SCPI long string of enum index (eg "NORMal"). */
    EEnumEntryAliasStrAlias6    =  5,   /*!< Select Alias6  of enum index (if there is any). */
    EEnumEntryAliasStrAlias7    =  6,   /*!< Select Alias7  of enum index (if there is any). */
    EEnumEntryAliasStrAlias8    =  7,   /*!< Select Alias8  of enum index (if there is any). */
    EEnumEntryAliasStrAlias9    =  8,   /*!< Select Alias9  of enum index (if there is any). */
    EEnumEntryAliasStrAlias10   =  9    /*!< Select Alias10 of enum index (if there is any). */
};

ZSSYSDLL_API QString enumEntryAlias2Str( EEnumEntryAliasStr i_alias );

//==============================================================================
/*! Defines the validity of an enumerator within an enumeration.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    For each enumerator within an enumeration you can use the validity flag
    to determine how the enumerator is to be displayed in a pulldown list or
    whether the enumerator should not be included in the pulldown list.

    This flag is actually only used if the enumerations state can be dynamically
    changed at runtime if e.g. certain states within an application depend on
    certain options.
*/
enum EEnumEntryValidity
//==============================================================================
{
    EEnumEntryValidityNormal   = 0, /*! This is the default setting. */
    EEnumEntryValidityDisabled = 1, /*! Currently not selectable (e.g. disabled in the pulldown list). */
    EEnumEntryValidityInvalid  = 2, /*! Currently not available (e.g. not shown in the pulldownlist). */
    EEnumEntryValidityCount    = 3, /*! Number of valid enumerators in the enumeration.
                                        Count may be used to program for and while loops. */
    EEnumEntryValidityUndefined     /*! Defines an undefined validity value and may be used
                                        to indicate that the variable has not yet been initialised. */
};

ZSSYSDLL_API QString enumEntryValidity2Str( EEnumEntryValidity i_validity, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );
ZSSYSDLL_API EEnumEntryValidity str2EnumEntryValidity( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );


//******************************************************************************
/*! @brief The structure is used to define an enum value together with up to
           10 optional strings and an optional variant value.

    In many cases, an enum value should not always be the output with the same
    string. Which string to use often depends on the use case.

    For output in log files it makes sense to use the enum value with its index name
    or output as a short, symbolic string.

    A longer string is required for output in a help text.

    Another string can be used to offer the enum value within a combo box.
    Depending on the available space, it may even be necessary to offer different
    strings for this.

    SCPI-compliant strings are also required for measuring devices that can be
    remotely controlled with SCPI commands.

    For this reason there is the possibility to define several strings for each
    enumerator. An index value is used to select which of these strings is to be used.
    The enum EEnumEntryAliasStr defines such index values in order to allow the
    programmers and users a uniform procedure.

    Optionally, a real value can be assigned to the enumerator (e.g. a double value
    as a real frequency value etc.).

    In addition, a validity can optionally be defined for the enumerator, with which
    it is possible to display the enumerator in a combo box differently than the other
    enumerators (e.g. remove from the pulldown list or greyed out and not selectable).

    Example:

    @code
    enum class EProcessorClock {
        Low    = 0,   // = 5.12 MHz
        Normal = 1,   // = 512.0 MHz
        High   = 2    // = 5.12 GHz
    }
    @endcode

    Definition of the associated strings and real numerical values in the source file:

    @code
    const SEnumEntry s_arEnumEntriesProcessorClock[] =
    {                        // Enumerator,                             Name,  Symbol, Text,                SCIPShort, SCPILong, Value
        SEnumEntry( static_cast<int>(EProcessorClock::Low),    "Low",    "L", "Low (5.12 MHz)",     "LOW",    "LOW",    5.12e6  ),
        SEnumEntry( static_cast<int>(EProcessorClock::Medium), "Medium", "M", "Medium (512.0 MHz)", "MED",    "MEDium", 512.0e6 ),
        SEnumEntry( static_cast<int>(EProcessorClock::High),   "High",   "H", "High (5.12 GHz)",    "HIGH",   "HIGH",   5.12e9  )
    };
    @endcode

    Routines to convert enum value to string and vice versa.

    @code
    QString processorClock2Str( EProcessorClock i_eVal, int i_idxAlias, bool* o_pbOk )
    {
        return SEnumEntry::enumerator2Str(s_arEnumEntriesProcessorClock, _ZSArrLen(s_arEnumEntriesProcessorClock), i_eVal, i_idxAlias, o_pbOk);
    }

    EProcessorClock str2ProcessorClock( const QString& i_str, int i_idxAlias, bool* o_pbOk )
    {
        return static_cast<EProcessorClock>(SEnumEntry::str2Enumerator(s_arEnumEntriesProcessorClock, _ZSArrLen(s_arEnumEntriesProcessorClock), i_str, i_idxAlias, Qt::CaseInsensitive, o_pbOk));
    }
    @endcode

    @see _GRP_BasicConcepts_Enumerations
    @see ZS::System::CEnum
*/
struct ZSSYSDLL_API SEnumEntry
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the structure belongs to. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the name of the structure. */
    static QString ClassName() { return "SEnumEntry"; }
public: // class methods
    static QString enumerator2Str(
        const SEnumEntry* i_pEnumArr,
        int               i_iArrLen,
        int               i_iEnumerator,
        int               i_idxAlias = ZS::System::EEnumEntryAliasStrName );
    static QVariant enumerator2Val(
        const SEnumEntry* i_pEnumArr,
        int               i_iArrLen,
        int               i_iEnumerator,
         QVariant::Type   i_type = QVariant::Invalid,
        bool*             o_pbOk = nullptr );
    static int str2Enumerator(
        const SEnumEntry*   i_pEnumArr,
        int                 i_iArrLen,
        const QString&      i_strName,
        int                 i_idxAlias = ZS::System::EEnumEntryAliasStrUndefined, // if Undefined all string list entries will be compared
        Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive,
        bool*               o_pbOk = nullptr );
    static QVariant str2Val(
        const SEnumEntry*   i_pEnumArr,
        int                 i_iArrLen,
        const QString&      i_strName,
        QVariant::Type      i_type = QVariant::Invalid,
        int                 i_idxAlias = ZS::System::EEnumEntryAliasStrUndefined, // if Undefined all string list entries will be compared
        Qt::CaseSensitivity i_caseSensitivity = Qt::CaseInsensitive,
        bool*               o_pbOk = nullptr );
    static int val2Enumerator(
        const SEnumEntry* i_pEnumArr,
        int               i_iArrLen,
        const QVariant&   i_val,
        bool*             o_pbOk = nullptr );
    static QString val2Str(
        const SEnumEntry* i_pEnumArr,
        int               i_iArrLen,
        const QVariant&   i_val,
        int               i_idxAlias = ZS::System::EEnumEntryAliasStrName,
        bool*             o_pbOk = nullptr );
    static bool isValidEnumerator(
        const SEnumEntry* i_pEnumArr,
        int               i_iArrLen,
        int               i_iEnumerator );
public: // class methods
    static void initStr2EnumeratorMaps(
        const SEnumEntry*             i_pEnumArr,
        int                           i_iArrLen,
        QVector<QHash<QString, int>>& io_arMaps,
        QMutex*                       i_pMtxMaps = nullptr );
public: // class methods
    static int str2Enumerator(
        const QVector<QHash<QString, int>>& i_arMaps,
        const QString&                      i_strName,
        int                                 i_idxAlias = ZS::System::EEnumEntryAliasStrUndefined, // if Undefined all string list entries will be compared
        Qt::CaseSensitivity                 i_caseSensitivity = Qt::CaseInsensitive,
        bool*                               o_pbOk = nullptr );
    static QVariant str2Val(
        const QVector<QHash<QString, int>>& i_arMaps,
        const SEnumEntry*                   i_pEnumArr,
        int                                 i_iArrLen,
        const QString&                      i_strName,
        QVariant::Type                      i_type = QVariant::Invalid,
        int                                 i_idxAlias = ZS::System::EEnumEntryAliasStrUndefined, // if Undefined all string list entries will be compared
        Qt::CaseSensitivity                 i_caseSensitivity = Qt::CaseInsensitive,
        bool*                               o_pbOk = nullptr );
public: // default ctor
    SEnumEntry();
public: // ctors
    explicit SEnumEntry( // Name becomes QString::number(i_iEnumerator)
        int                i_iEnumerator,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
public: // ctors
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const char*        i_strAliasSymbol,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const char*        i_strAliasSymbol,
        const char*        i_strAliasText,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const char*        i_strAliasSymbol,
        const char*        i_strAliasText,
        const char*        i_strAliasSCPIShort,
        const char*        i_strAliasSCIPLong,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const char*        i_strAliasSymbol,
        const char*        i_strAliasText,
        const char*        i_strAliasSCPIShort,
        const char*        i_strAliasSCIPLong,
        const char*        i_strAlias6,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const char*        i_strAliasSymbol,
        const char*        i_strAliasText,
        const char*        i_strAliasSCPIShort,
        const char*        i_strAliasSCIPLong,
        const char*        i_strAlias6,
        const char*        i_strAlias7,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const char*        i_strAliasSymbol,
        const char*        i_strAliasText,
        const char*        i_strAliasSCPIShort,
        const char*        i_strAliasSCIPLong,
        const char*        i_strAlias6,
        const char*        i_strAlias7,
        const char*        i_strAlias8,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const char*        i_strAliasSymbol,
        const char*        i_strAliasText,
        const char*        i_strAliasSCPIShort,
        const char*        i_strAliasSCIPLong,
        const char*        i_strAlias6,
        const char*        i_strAlias7,
        const char*        i_strAlias8,
        const char*        i_strAlias9,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const char*        i_strName,
        const char*        i_strAliasSymbol,
        const char*        i_strAliasText,
        const char*        i_strAliasSCPIShort,
        const char*        i_strAliasSCIPLong,
        const char*        i_strAlias6,
        const char*        i_strAlias7,
        const char*        i_strAlias8,
        const char*        i_strAlias9,
        const char*        i_strAlias10,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
public: // ctors
    explicit SEnumEntry(
        int                i_iEnumerator,
        const QString&     i_strName,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QString&     i_strAlias6,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
        const QString&     i_strName,
        const QString&     i_strAliasSymbol,
        const QString&     i_strAliasText,
        const QString&     i_strAliasSCPIShort,
        const QString&     i_strAliasSCIPLong,
        const QString&     i_strAlias6,
        const QString&     i_strAlias7,
        const QString&     i_strAlias8,
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
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
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
    explicit SEnumEntry(
        int                i_iEnumerator,
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
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
public: // ctors
    explicit SEnumEntry(
        int                i_iEnumerator,
        const QStringList& i_strlstNames, // [0: Name, 1: Symbol, 2: Text, 3: SCPIShort, 4: SCPILong, 5: Alias6, .., 9: Alias10]
        const QVariant&    i_val = QVariant(),
        EEnumEntryValidity i_validity = ZS::System::EEnumEntryValidityNormal );
public: // dtor
    virtual ~SEnumEntry();
public: // instance methods
    /*! Returns the namespace of the structure. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! Returns the class name of the structure. */
    virtual QString className() const { return ClassName(); }
public: // methods
    QString getName( int i_idxAlias = ZS::System::EEnumEntryAliasStrName ) const;
public: // struct members
    int                m_iEnumerator;   /*!< Initialization value of the enumerator within the enumeration. The enumerator initialization values must start with 0 and must be incremented by 1 when defining the list of enumerators. */
    QStringList        m_strlstNames;   /*!< List with up as many different string representations as desired. But it is recommend to use the list indexes [0: Name, 1: Symbol, 2: Text, 3: SCPIShort, 4: SCPILong, 5: Alias6, .., 9: Alias10]. */
    QVariant           m_val;           /*!< Real value of enumerator (e.g. Frequency as double value). */
    EEnumEntryValidity m_validity;      /*!< Validity of the enum index value. Used to mark an enumerator as not selectable or not available. */

}; // struct SEnumEntry

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_EnumEntry_h
