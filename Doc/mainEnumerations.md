@page _PAGE_Main_Enumerations Enumerations

Here, a generally applicable approach to the definition of enumerations within an application is described.

An enumeration is a distinct type whose value is restricted to a range of values, which may include several explicitly
named constants ("enumerators"). The values of the constants are values of an integral type known as the underlying
type of the enumeration.

An enumeration is defined using the following syntax:

@code
enum-key enum-name(optional) enum-base(optional) {
    enumerator-list(optional)
};
@endcode

- enum-key<br>
  Either enum, enum class(since C++11), or enum struct(since C++11).
- enum-name<br>
  The name of the enumeration that's being declared. The name can be omitted only in unscoped enumeration declarations.
- enum-base(C++11)<br>
  colon (:), followed by a type-specifier-seq that names an integral type that will serve as the fixed underlying type
  for this enumeration type.
- enumerator-list<br>
  Comma-separated list of enumerator definitions, each of which is either simply an identifier, which becomes the name
  of the enumerator, or an identifier with an initializer: identifier = constexpr.

Enumeratoren and Enum Entries
=============================

Often it is desired to indicate the names of the enumerators or at least a meaningful abbreviation of the enumerators
as a string.

- E.g in log files the output of "ClockFreq = Medium" is more understandable than the output of "ClockFreq = 1".
- Combo boxes are usually used to select a value from an enumeration. In the pulldown list descriptive text should
  be offered instead of numeric index values. Since the available width for the pull-down list can often vary
  depending on the GUI control, it is sometimes necessary to offer alternative strings for the enumerators
  ("High Resolution (5.12 GHz)" or only "High").
- In order to fill the pulldown lists of the combo boxes with the corresponding string belonging to the enumerator,
  loops should be used. Since there can be several places in the system with such loops, a syntax should be used for
  this, which makes these loops resistant to a change in the enumeration. The addition of enumerators should not make
  it necessary to adjust all the code places in which the enumeration is used, but the necessary changes should be
  reduced to a minimum.
- For the resistant storage of setting values ​​or also for the transfer of data via streams, it is often desirable
  to use readable strings instead of hard-coded numbers. This increases the downward compatibility of software
  versions if enumerations are changed during a version jump.
- For remote control of devices, in addition to numerical values, character enums should also be possible.
  And this also in different forms. Both a SHORT and a LONG form should be permitted for the SCPI standard (NORM and NORMal).
- It may also happen that an enumeration is used to select a real, possibly a non-integer value (e.g. double).
- In order to increase the maintainability of the code, all definitions necessary for an enumeration
  (the strings and the real numerical values) should only be made at one point in the code. The definition of the
  enumeration and its enumerators in a header file, the associated strings and the real value in the source file
  of the same name.

The structure #ZS::System::SEnumEntry was introduced so that these definitions can be defined in one place and clearly.
This structure allows 1 to 10 different strings to be defined for each enumerator and optionally store a value of
almost any data type (QVariant).

An enumeration is e.g. defined as follows:

Definition of the enumeration with the enumerators in the header file:

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
    /* 0 */ SEnumEntry( static_cast<int>(EProcessorClock::Low),    "Low",    "L", "Low (5.12 MHz)",     "LOW",    "LOW",    5.12e6  ),
    /* 1 */ SEnumEntry( static_cast<int>(EProcessorClock::Medium), "Medium", "M", "Medium (512.0 MHz)", "MED",    "MEDium", 512.0e6 ),
    /* 2 */ SEnumEntry( static_cast<int>(EProcessorClock::High),   "High",   "H", "High (5.12 GHz)",    "HIGH",   "HIGH",   5.12e9  )
};
@endcode

@note
For better readability and maintainability of the code, the enumerators are numbered consecutively
with the enumeration constants. This can be very helpful for maintaining the list of enum entries,
especially for enumerations with many enumerators.

@note
The array with the enumerator entries is later defined with a QVector, because the enumeration from
the example is derived later from an enumeration template class.

The structure SEnumEntry has implemented static methods with which an enumerator can be converted into
a string (SEnumEntry::enumerator2Str) and a string back into the enumerator (SEnumEntry::str2Enumerator).
A pointer to the EnumEntry array and the number of defined enumerators in the array must be transferred
to these conversion methods. In addition, an index value for selecting one of the 10 possible string
variants must be specified.

@note
In order to standardize the selection of one of the strings belonging to an enumerator, the enumeration
EEnumEntryAliasStr was defined (name, symbol, text, SCPIShort, SCPILong, Alias6, .. Alias10).
The transfer to the string conversion routines takes place as an integer value so that the programmer
is free to use his own string alias constants for each enumeration.

Furthermore, the SEnumEntry structure offers static methods to get the real value of the enumerator
from the EnumEntry array (SEnumEntry :: enumerator2Val).
A static method is also provided for the opposite way (val2Enumerator).

@note
The conversion of strings into the enumerator takes place via the static method "SEnumEntry::str2Enumerator"
by comparing each individual string until the search finds an identical string. This is not really performant,
especially with longer enumerations. Therefore, hashes should be used for this backward search. The keys of
those hashes must be the respective strings. To search using hashes, the structure SEnumEntry offers
overloaded methods for SEnumEntry::str2Enumerator and SEnumEntry::str2Val as well as the method
SEnumEntry::initStr2EnumeratorMaps to initialize these maps from the EnumEntry array.

@note
These string hashes can be initialized at runtime via lazy initialization (i.e. when used for the first time),
as shown in the following example.

@note
When generating the string hashes, it is also a good idea to check the enum entries for consistency.
Caution should be exercised here if the conversion routines are accessed by different threads.
In this case, the initialization must be protected via a mutex (singleton)

@note
It is important that the enumerators are in ascending order starting with 0, because otherwise all conversion
routines will be inefficient. For this reason, an exception is thrown if one of the conversion routines is
used and this condition is violated.

Of course, a programmer does not want to have to refer to the EnumEntry array and pass it on to the conversion
routines at every point in the code in which a conversion of an enumerator is required. Therefore, separate
conversion routines should be provided for each enumeration, which implicitly access these arrays or alias string maps.

@code
QString processorClock2Str( EProcessorClock i_eVal, int i_idxAlias )
{
    return enumerator2Str(s_arEnumEntriesProcessorClock, _ZSArrLen(s_arEnumEntriesProcessorClock), static_cast<int>(i_eVal), i_idxAlias);
}

EProcessorClock str2ProcessorClock( const QString& i_str )
{
    static QVector<QHash<QString, int>> s_arAliasStrMaps;
    if( s_arAliasStrMaps.size() == 0 )
    {
        SEnumEntry::initStr2EnumeratorMaps(s_arEnumEntriesProcessorClock, _ZSArrLen(s_arEnumEntriesProcessorClock), s_arAliasStrMaps);
    }
    return static_cast<EProcessorClock>(str2Enumerator(s_arAliasStrMaps, i_str));
}
@endcode

@note
In order to increase the readability of the code examples, area checks were omitted in the above examples
and no flag was set as to whether the conversion was successful or not. However, this should be supplemented
if own versions of the conversion routines are implemented.

The Template Class ZS::System::CEnum
=====================================

Writing the conversion routines is a constantly repeating task and all conversion routines - except for the
different EnumEntry arrays - look identical. The template class ZS :: System :: CEnum has been implemented
so that you can save yourself this paperwork and also offer even more convenient uses with enumerations.

This template class relieves you of writing the conversion routines. The initialization of the string conversion
hashes is also done implicitly by the template class. The use of the Enum template class also allows you to write
a more easily readable code, since the template class offers a variety of useful methods for constructors and
overloaded operators.

The following steps are necessary to use the CEnum template class:

1. Definition of the enumeration as "scoped" enumeration with the keyword class (as already done in our example
   enum class EProcessorClock).

2. If the enumeration is to be exported from a Dll, set an export instruction before defining the template
   in the header file:
   @code
   template class ZSSYSDLL_API CEnum<EProcessorClock>;
   @endcode

   @note
   ZSSYSDLL_APP must be replaced accordingly.

3. A type definition in the header file to simplify the use of the implemented enum template class in the code:
   @code
   typedef CEnum<EProcessorClock> CEnumProcessorClock;
   @endcode

4. In the source file a definition of the static class variables required for the conversion routines.
   On the one hand, this is a vector for the alias string hashes and the mutex for protecting access
   to the alias string hashes from different threads:
   @code
   QMutex CEnum<EProcessorClock>::s_mtxArMapsStr2Enumerators;
   QVector<QHash<QString, int>> CEnum<EProcessorClock>::s_armapsStr2Enumerators;
   @endcode

5. Definition of the static enum entry array as QVector:
   @code
   const QVector<SEnumEntry> CEnum<EProcessorClock>::s_arEnumEntries =
   {   // idxRow,           Enumerator,                                Name,  Symbol, Text,                SCIPShort, SCPILong, Value
       /*  0 */ SEnumEntry( static_cast<int>(EProcessorClock::Low),    "Low",    "L", "Low (5.12 MHz)",     "LOW",    "LOW",    5.12e6  ),
       /*  1 */ SEnumEntry( static_cast<int>(EProcessorClock::Medium), "Medium", "M", "Medium (512.0 MHz)", "MED",    "MEDium", 512.0e6 ),
       /*  2 */ SEnumEntry( static_cast<int>(EProcessorClock::High),   "High",   "H", "High (5.12 GHz)",    "HIGH",   "HIGH",   5.12e9  )
   };
   @endcode

Steps 1 and 5 are also necessary when using the enum entry structure and the conversion routines
to be implemented separately for each enumeration.

If you now instantiate a template class derived for the enumeration, you can write the following code:

@code
CEnumProcessorClock eProcClock;

for( eProcClock = 0; eProcClock < CEnumProcessorClock::count(); ++eProcClock )
{
    pCmbProcClock->addItem(enumInst.toString(), enumInst.toValue());
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

@see ZS::System::SEnumEntry
@see ZS::System::CEnum
