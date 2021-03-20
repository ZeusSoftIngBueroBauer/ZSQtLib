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

#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
Enum EMode
==============================================================================*/

QMutex CEnum<EMode>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EMode>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EMode>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumMode
{
public: // ctor
    CInitEnumMode() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EMode>::s_arEnumEntries);
                                                     // Enumerator,                       Name,      Symbol, Text,  SCIPShort, SCPILong, Alias6, Alias7
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(EMode::Edit),       "Edit",       "E", "Edit",       "", "", "Design",     "Edit"      ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(EMode::Simulation), "Simulation", "S", "Simulation", "", "", "Simulation", "View"      ) );
        pVEnumEntries->append( /* 3 */ SEnumEntry( static_cast<int>(EMode::Undefined),  "Undefined",  "?", "Undefined",  "", "", "Undefined",  "Undefined" ) );
    }
};
static CInitEnumMode s_initEnumMode;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EMode>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                       Name,      Symbol, Text,  SCIPShort, SCPILong, Alias6, Alias7
    /* 0 */ SEnumEntry( static_cast<int>(EMode::Edit),       "Edit",       "E", "Edit",       "", "", "Design",     "Edit"      ),
    /* 1 */ SEnumEntry( static_cast<int>(EMode::Simulation), "Simulation", "S", "Simulation", "", "", "Simulation", "View"      ),
    /* 3 */ SEnumEntry( static_cast<int>(EMode::Undefined),  "Undefined",  "?", "Undefined",  "", "", "Undefined",  "Undefined" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum ERunMode
==============================================================================*/

QMutex CEnum<ERunMode>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<ERunMode>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ERunMode>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumRunMode
{
public: // ctor
    CInitEnumRunMode() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<ERunMode>::s_arEnumEntries);
        pVEnumEntries->append( /*  0 */ SEnumEntry( static_cast<int>(ERunMode::Continuous), "Continuous", "Cont", "Continuous", "CONT", "CONTinuous" ) );
        pVEnumEntries->append( /*  1 */ SEnumEntry( static_cast<int>(ERunMode::SingleStep), "SingleStep", "Sing", "SingleStep", "SST",  "SSTep"      ) );
        pVEnumEntries->append( /*  2 */ SEnumEntry( static_cast<int>(ERunMode::Undefined),  "Undefined",  "?",    "Undefined",  "UND",  "UNDefined"  ) );
    }
};
static CInitEnumRunMode s_initEnumRunMode;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ERunMode>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                        // Enumerator,                          Name,         Symbol, Text,      SCPIShort, SCPILong
    /*  0 */ SEnumEntry( static_cast<int>(ERunMode::Continuous), "Continuous", "Cont", "Continuous", "CONT", "CONTinuous" ),
    /*  1 */ SEnumEntry( static_cast<int>(ERunMode::SingleStep), "SingleStep", "Sing", "SingleStep", "SST",  "SSTep"      ),
    /*  2 */ SEnumEntry( static_cast<int>(ERunMode::Undefined),  "Undefined",  "?",    "Undefined",  "UND",  "UNDefined"  )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EYesNo
==============================================================================*/

QMutex CEnum<EYesNo>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EYesNo>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EYesNo>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumYesNo
{
public: // ctor
    CInitEnumYesNo() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EYesNo>::s_arEnumEntries);
        pVEnumEntries->append( /*  0 */ SEnumEntry( static_cast<int>(EYesNo::No),        "No",        "No",  "No",        "NO",  "NO",       "false",      "False"     ) );
        pVEnumEntries->append( /*  1 */ SEnumEntry( static_cast<int>(EYesNo::Yes),       "Yes",       "Yes", "Yes",       "YES", "YES",       "true",      "True"      ) );
        pVEnumEntries->append( /*  2 */ SEnumEntry( static_cast<int>(EYesNo::Undefined), "Undefined", "?",   "Undefined", "UND", "UNDefined", "Undefined", "Undefined" ) );
    }
};
static CInitEnumYesNo s_initEnumYesNo;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EYesNo>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                        // Enumerator,                       Name,      Symbol,  Text,   SCPIShort, SCPILong,     Alias6,      Alias7
    /*  0 */ SEnumEntry( static_cast<int>(EYesNo::No),        "No",        "No",  "No",        "NO",  "NO",       "false",      "False"     ),
    /*  1 */ SEnumEntry( static_cast<int>(EYesNo::Yes),       "Yes",       "Yes", "Yes",       "YES", "YES",       "true",      "True"      ),
    /*  2 */ SEnumEntry( static_cast<int>(EYesNo::Undefined), "Undefined", "?",   "Undefined", "UND", "UNDefined", "Undefined", "Undefined" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EStateOnOff
==============================================================================*/

QMutex CEnum<EStateOnOff>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EStateOnOff>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EStateOnOff>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumStateOnOff
{
public: // ctor
    CInitEnumStateOnOff() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EStateOnOff>::s_arEnumEntries);
        pVEnumEntries->append( /*  0 */ SEnumEntry( static_cast<int>(EStateOnOff::Off),        "Off"       ) );
        pVEnumEntries->append( /*  1 */ SEnumEntry( static_cast<int>(EStateOnOff::On),         "On"        ) );
        pVEnumEntries->append( /*  2 */ SEnumEntry( static_cast<int>(EStateOnOff::Undefined),  "Undefined" ) );
    }
};
static CInitEnumStateOnOff s_initEnumStateOnOff;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EStateOnOff>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                        // Enumerator,                             Name
    /*  0 */ SEnumEntry( static_cast<int>(EStateOnOff::Off),        "Off"       ),
    /*  1 */ SEnumEntry( static_cast<int>(EStateOnOff::On),         "On"        ),
    /*  2 */ SEnumEntry( static_cast<int>(EStateOnOff::Undefined),  "Undefined" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EEnabled
==============================================================================*/

QMutex CEnum<EEnabled>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EEnabled>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EEnabled>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumEnabled
{
public: // ctor
    CInitEnumEnabled() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EEnabled>::s_arEnumEntries);
        pVEnumEntries->append( /*  0 */ SEnumEntry( static_cast<int>(EEnabled::No),        "No"  ) );
        pVEnumEntries->append( /*  1 */ SEnumEntry( static_cast<int>(EEnabled::Yes),       "Yes" ) );
        pVEnumEntries->append( /*  2 */ SEnumEntry( static_cast<int>(EEnabled::Undefined), "Undefined" ) );
    }
};
static CInitEnumEnabled s_initEnumEnabled;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EEnabled>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                        // Enumerator,                         Name
    /*  0 */ SEnumEntry( static_cast<int>(EEnabled::No),        "No"  ),
    /*  1 */ SEnumEntry( static_cast<int>(EEnabled::Yes),       "Yes" ),
    /*  2 */ SEnumEntry( static_cast<int>(EEnabled::Undefined), "Undefined" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum ECopyDepth
==============================================================================*/

QMutex CEnum<ECopyDepth>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<ECopyDepth>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ECopyDepth>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumCopyDepth
{
public: // ctor
    CInitEnumCopyDepth() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<ECopyDepth>::s_arEnumEntries);
        pVEnumEntries->append( /*  0 */ SEnumEntry( static_cast<int>(ECopyDepth::FlatKeepOwnership),     "FlatKeepOwnership",     "FKO" ) );
        pVEnumEntries->append( /*  1 */ SEnumEntry( static_cast<int>(ECopyDepth::FlatReleaseOwnwership), "FlatReleaseOwnwership", "FRO" ) );
        pVEnumEntries->append( /*  2 */ SEnumEntry( static_cast<int>(ECopyDepth::Deep),                  "Deep",                  "DEE" ) );
    }
};
static CInitEnumCopyDepth s_initEnumCopyDepth;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ECopyDepth>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                        // Enumerator,                                       Name,                    Symbol
    /*  0 */ SEnumEntry( static_cast<int>(ECopyDepth::FlatKeepOwnership),     "FlatKeepOwnership",     "FKO" ),
    /*  1 */ SEnumEntry( static_cast<int>(ECopyDepth::FlatReleaseOwnwership), "FlatReleaseOwnwership", "FRO" ),
    /*  2 */ SEnumEntry( static_cast<int>(ECopyDepth::Deep),                  "Deep",                  "DEE" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EObjState
==============================================================================*/

QMutex CEnum<EObjState>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EObjState>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EObjState>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumObjState
{
public: // ctor
    CInitEnumObjState() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EObjState>::s_arEnumEntries);
        pVEnumEntries->append( /*  0 */ SEnumEntry( static_cast<int>(EObjState::Detached),   "Detached"   ) );
        pVEnumEntries->append( /*  1 */ SEnumEntry( static_cast<int>(EObjState::Creating),   "Creating"   ) );
        pVEnumEntries->append( /*  2 */ SEnumEntry( static_cast<int>(EObjState::Created),    "Created"    ) );
        pVEnumEntries->append( /*  3 */ SEnumEntry( static_cast<int>(EObjState::Destroying), "Destroying" ) );
        pVEnumEntries->append( /*  4 */ SEnumEntry( static_cast<int>(EObjState::Destroyed),  "Destroyed"  ) );
        pVEnumEntries->append( /*  5 */ SEnumEntry( static_cast<int>(EObjState::Undefined),  "Undefined"  ) );
    }
};
static CInitEnumObjState s_initEnumObjState;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EObjState>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                        // Enumerator,                           Name
    /*  0 */ SEnumEntry( static_cast<int>(EObjState::Detached),   "Detached"   ),
    /*  1 */ SEnumEntry( static_cast<int>(EObjState::Creating),   "Creating"   ),
    /*  2 */ SEnumEntry( static_cast<int>(EObjState::Created),    "Created"    ),
    /*  3 */ SEnumEntry( static_cast<int>(EObjState::Destroying), "Destroying" ),
    /*  4 */ SEnumEntry( static_cast<int>(EObjState::Destroyed),  "Destroyed"  ),
    /*  5 */ SEnumEntry( static_cast<int>(EObjState::Undefined),  "Undefined"  )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
enum ERowVersion
==============================================================================*/

QMutex CEnum<ERowVersion>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<ERowVersion>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ERowVersion>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumRowVersion
{
public: // ctor
    CInitEnumRowVersion() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<ERowVersion>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(ERowVersion::Original),  "Original",  "O",    "Original",  "ORIG",    "ORIGinal",  "Ori" ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(ERowVersion::Current),   "Current",   "C",    "Current",   "CURR",    "CURRent",   "Cur" ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(ERowVersion::Proposed),  "Proposed",  "P",    "Proposed",  "PROP",    "PROPosed",  "Prp" ) );
        pVEnumEntries->append( /* 3 */ SEnumEntry( static_cast<int>(ERowVersion::Default),   "Default",   "D",    "Default",   "DEF",     "DEFault",   "Def" ) );
        pVEnumEntries->append( /* 4 */ SEnumEntry( static_cast<int>(ERowVersion::Undefined), "Undefined", "?",    "Undefined", "UND",     "UNDefined", "Und" ) );
    }
};
static CInitEnumRowVersion s_initEnumRowVersion;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ERowVersion>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                            Name,        Symbol, Text,        SCPIShort, SCPILong,    Alias6
    /* 0 */ SEnumEntry( static_cast<int>(ERowVersion::Original),  "Original",  "O",    "Original",  "ORIG",    "ORIGinal",  "Ori" ),
    /* 1 */ SEnumEntry( static_cast<int>(ERowVersion::Current),   "Current",   "C",    "Current",   "CURR",    "CURRent",   "Cur" ),
    /* 2 */ SEnumEntry( static_cast<int>(ERowVersion::Proposed),  "Proposed",  "P",    "Proposed",  "PROP",    "PROPosed",  "Prp" ),
    /* 3 */ SEnumEntry( static_cast<int>(ERowVersion::Default),   "Default",   "D",    "Default",   "DEF",     "DEFault",   "Def" ),
    /* 4 */ SEnumEntry( static_cast<int>(ERowVersion::Undefined), "Undefined", "?",    "Undefined", "UND",     "UNDefined", "Und" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
enum ERowState
==============================================================================*/

QMutex CEnum<ERowState>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<ERowState>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ERowState>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumRowState
{
public: // ctor
    CInitEnumRowState() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<ERowState>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(ERowState::Unchanged), "Unchanged", "",    "Unchanged", "UNCH",    "UNCHanged", "Unc" ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(ERowState::Detached),  "Detached",  "~",   "Detached",  "DET",     "DETached",  "Det" ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(ERowState::Added),     "Added",     "+",   "Added",     "ADD",     "ADDed",     "Add" ) );
        pVEnumEntries->append( /* 3 */ SEnumEntry( static_cast<int>(ERowState::Removed),   "Removed",   "-",   "Removed",   "REM",     "REMoved",   "Rem" ) );
        pVEnumEntries->append( /* 4 */ SEnumEntry( static_cast<int>(ERowState::Modified),  "Modified",  "*",   "Modified",  "MOD",     "MODified",  "Mod" ) );
        pVEnumEntries->append( /* 5 */ SEnumEntry( static_cast<int>(ERowState::Undefined), "Undefined", "?",   "Undefined", "UND",     "UNDefined", "Und" ) );
    }
};
static CInitEnumRowState s_initEnumRowState;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ERowState>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                          Name,       Symbol, Text,       SCPIShort, SCPILong,    Alias6
    /* 0 */ SEnumEntry( static_cast<int>(ERowState::Unchanged), "Unchanged", "",    "Unchanged", "UNCH",    "UNCHanged", "Unc" ),
    /* 1 */ SEnumEntry( static_cast<int>(ERowState::Detached),  "Detached",  "~",   "Detached",  "DET",     "DETached",  "Det" ),
    /* 2 */ SEnumEntry( static_cast<int>(ERowState::Added),     "Added",     "+",   "Added",     "ADD",     "ADDed",     "Add" ),
    /* 3 */ SEnumEntry( static_cast<int>(ERowState::Removed),   "Removed",   "-",   "Removed",   "REM",     "REMoved",   "Rem" ),
    /* 4 */ SEnumEntry( static_cast<int>(ERowState::Modified),  "Modified",  "*",   "Modified",  "MOD",     "MODified",  "Mod" ),
    /* 5 */ SEnumEntry( static_cast<int>(ERowState::Undefined), "Undefined", "?",   "Undefined", "UND",     "UNDefined", "Und" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EContentToStrFormat
==============================================================================*/

QMutex CEnum<EContentToStrFormat>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EContentToStrFormat>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EContentToStrFormat>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumContentToStrFormat
{
public: // ctor
    CInitEnumContentToStrFormat() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EContentToStrFormat>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(EContentToStrFormat::PlainText), "PlainText" ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(EContentToStrFormat::HtmlText),  "HtmlText"  ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(EContentToStrFormat::Undefined), "Undefined" ) );
    }
};
static CInitEnumContentToStrFormat s_initEnumContentToStrFormat;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EContentToStrFormat>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                    Name
    /* 0 */ SEnumEntry( static_cast<int>(EContentToStrFormat::PlainText), "PlainText" ),
    /* 1 */ SEnumEntry( static_cast<int>(EContentToStrFormat::HtmlText),  "HtmlText"  ),
    /* 2 */ SEnumEntry( static_cast<int>(EContentToStrFormat::Undefined), "Undefined" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EMethodDir
==============================================================================*/

QMutex CEnum<EMethodDir>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EMethodDir>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EMethodDir>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumMethodDir
{
public: // ctor
    CInitEnumMethodDir() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EMethodDir>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(EMethodDir::Enter),     "Enter",     "E" ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(EMethodDir::Leave),     "Leave",     "L" ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(EMethodDir::None),      "None",      "N" ) );
        pVEnumEntries->append( /* 3 */ SEnumEntry( static_cast<int>(EMethodDir::Undefined), "Undefined", "?" ) );
    }
};
static CInitEnumMethodDir s_initEnumMethodDir;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EMethodDir>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                           Name,       Symbol
    /* 0 */ SEnumEntry( static_cast<int>(EMethodDir::Enter),     "Enter",     "E" ),
    /* 1 */ SEnumEntry( static_cast<int>(EMethodDir::Leave),     "Leave",     "L" ),
    /* 2 */ SEnumEntry( static_cast<int>(EMethodDir::None),      "None",      "N" ),
    /* 3 */ SEnumEntry( static_cast<int>(EMethodDir::Undefined), "Undefined", "?" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum ETransmitDir
==============================================================================*/

QMutex CEnum<ETransmitDir>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<ETransmitDir>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ETransmitDir>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumTransmitDir
{
public: // ctor
    CInitEnumTransmitDir() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<ETransmitDir>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(ETransmitDir::Receive),   "Receive",   "RCV",  "In",  "I", "Read",  "R" ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(ETransmitDir::Send),      "Send",      "SND",  "Out", "O", "Write", "W" ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(ETransmitDir::Undefined), "Undefined", "?",    "",    "",  "",      ""  ) );
    }
};
static CInitEnumTransmitDir s_initEnumTransmitDir;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<ETransmitDir>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                             Name,        Symbol
    /* 0 */ SEnumEntry( static_cast<int>(ETransmitDir::Receive),   "Receive",   "RCV",  "In",  "I", "Read",  "R" ),
    /* 1 */ SEnumEntry( static_cast<int>(ETransmitDir::Send),      "Send",      "SND",  "Out", "O", "Write", "W" ),
    /* 2 */ SEnumEntry( static_cast<int>(ETransmitDir::Undefined), "Undefined", "?",    "",    "",  "",      ""  )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EIODir
==============================================================================*/

QMutex CEnum<EIODir>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EIODir>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EIODir>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumIODir
{
public: // ctor
    CInitEnumIODir() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EIODir>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(EIODir::Out),       "In",        "I",    "In",        "IN",      "IN"        ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(EIODir::In),        "Out",       "O",    "Out",       "OUT",     "OUT"       ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(EIODir::Undefined), "Undefined", "?",    "Undefined", "UND",     "UNDefined" ) );
    }
};
static CInitEnumIODir s_initEnumIODir;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EIODir>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                       Name,     Symbol,    Text,        SCPIShort, SCPILong
    /* 0 */ SEnumEntry( static_cast<int>(EIODir::Out),       "In",        "I",    "In",        "IN",      "IN"        ),
    /* 1 */ SEnumEntry( static_cast<int>(EIODir::In),        "Out",       "O",    "Out",       "OUT",     "OUT"       ),
    /* 2 */ SEnumEntry( static_cast<int>(EIODir::Undefined), "Undefined", "?",    "Undefined", "UND",     "UNDefined" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EDirection
==============================================================================*/

QMutex CEnum<EDirection>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EDirection>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EDirection>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumDirection
{
public: // ctor
    CInitEnumDirection() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EDirection>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(EDirection::Vertical),   "Vertical",   "Ver"  ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(EDirection::Horizontal), "Horizontal", "Hor"  ) );
        pVEnumEntries->append( /* 3 */ SEnumEntry( static_cast<int>(EDirection::Undefined),  "Undefined",  "?"    ) );
    }
};
static CInitEnumDirection s_initEnumDirection;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EDirection>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                            Name,         Symbol
    /* 0 */ SEnumEntry( static_cast<int>(EDirection::Vertical),   "Vertical",   "Ver"  ),
    /* 1 */ SEnumEntry( static_cast<int>(EDirection::Horizontal), "Horizontal", "Hor"  ),
    /* 3 */ SEnumEntry( static_cast<int>(EDirection::Undefined),  "Undefined",  "?"    )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EComparisonOperator
==============================================================================*/

QMutex CEnum<EComparisonOperator>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EComparisonOperator>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EComparisonOperator>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumComparisonOperator
{
public: // ctor
    CInitEnumComparisonOperator() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EComparisonOperator>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(EComparisonOperator::Equal),     "Equal",     "==" ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(EComparisonOperator::Unequal),   "Unequal",   "!=" ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(EComparisonOperator::Undefined), "Undefined", "?"  ) );
    }
};
static CInitEnumComparisonOperator s_initEnumComparisonOperator;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EComparisonOperator>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                    Name,      Symbol
    /* 0 */ SEnumEntry( static_cast<int>(EComparisonOperator::Equal),     "Equal",     "==" ),
    /* 1 */ SEnumEntry( static_cast<int>(EComparisonOperator::Unequal),   "Unequal",   "!=" ),
    /* 2 */ SEnumEntry( static_cast<int>(EComparisonOperator::Undefined), "Undefined", "?"  )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
Enum EValueValidity
==============================================================================*/

QMutex CEnum<EValueValidity>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EValueValidity>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EValueValidity>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumValueValidity
{
public: // ctor
    CInitEnumValueValidity() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EValueValidity>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(EValueValidity::Invalid),   "Invalid",   "---",    "Invalid",   "INV",     "INValid",   "Inv",      "-" ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(EValueValidity::Null),      "Null",      "(null)", "Null",      "nullptr", "nullptr",   "N",        "n" ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(EValueValidity::Valid),     "Valid",     "",       "Valid",     "VAL",     "VALid",     "Val",       "" ) );
        pVEnumEntries->append( /* 3 */ SEnumEntry( static_cast<int>(EValueValidity::Undefined), "Undefined", "?",      "Undefined", "UND",     "UNDefined", "Undefined", "" ) );
    }
};
static CInitEnumValueValidity s_initEnumValueValidity;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EValueValidity>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                               Name,        Symbol,   Text,        SCPIShort, SCPILong,    Alias6,    Alias7
    /* 0 */ SEnumEntry( static_cast<int>(EValueValidity::Invalid),   "Invalid",   "---",    "Invalid",   "INV",     "INValid",   "Inv",      "-" ),
    /* 1 */ SEnumEntry( static_cast<int>(EValueValidity::Null),      "Null",      "(null)", "Null",      "nullptr", "nullptr",   "N",        "n" ),
    /* 2 */ SEnumEntry( static_cast<int>(EValueValidity::Valid),     "Valid",     "",       "Valid",     "VAL",     "VALid",     "Val",       "" ),
    /* 3 */ SEnumEntry( static_cast<int>(EValueValidity::Undefined), "Undefined", "?",      "Undefined", "UND",     "UNDefined", "Undefined", "" )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
enum EDimensionType
==============================================================================*/

QMutex CEnum<EDimensionType>::s_mtxArMapsStr2Enumerators;
QVector<QHash<QString, int>> CEnum<EDimensionType>::s_armapsStr2Enumerators;

#if QT_VERSION < 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EDimensionType>::s_arEnumEntries;
//------------------------------------------------------------------------------
class CInitEnumDimensionType
{
public: // ctor
    CInitEnumDimensionType() {
        QVector<SEnumEntry>* pVEnumEntries = const_cast<QVector<SEnumEntry>*>(&CEnum<EDimensionType>::s_arEnumEntries);
        pVEnumEntries->append( /* 0 */ SEnumEntry( static_cast<int>(EDimensionType::SingleValue),     "SingleValue",     "S", "Single Value",     "SING", "SINGle"       ) );
        pVEnumEntries->append( /* 1 */ SEnumEntry( static_cast<int>(EDimensionType::OneDimensional),  "OneDimensional",  "O", "One Dimensional",  "ONED", "ODIMensional" ) );
        pVEnumEntries->append( /* 2 */ SEnumEntry( static_cast<int>(EDimensionType::MoreDimensional), "MoreDimensional", "M", "More Dimensional", "MDIM", "MDIMensional" ) );
        pVEnumEntries->append( /* 3 */ SEnumEntry( static_cast<int>(EDimensionType::Undefined),       "Undefined",       "?", "Undefined",        "UND",  "UNDefined"    ) );
    }
};
static CInitEnumDimensionType s_initEnumDimensionType;
#else // #if QT_VERSION >= 0x050000
//------------------------------------------------------------------------------
const QVector<SEnumEntry> CEnum<EDimensionType>::s_arEnumEntries =
//------------------------------------------------------------------------------
{                       // Enumerator,                                     Name,           Symbol, Text,            SCPIShort, SCPILong
    /* 0 */ SEnumEntry( static_cast<int>(EDimensionType::SingleValue),     "SingleValue",     "S", "Single Value",     "SING", "SINGle"       ),
    /* 1 */ SEnumEntry( static_cast<int>(EDimensionType::OneDimensional),  "OneDimensional",  "O", "One Dimensional",  "ONED", "ODIMensional" ),
    /* 2 */ SEnumEntry( static_cast<int>(EDimensionType::MoreDimensional), "MoreDimensional", "M", "More Dimensional", "MDIM", "MDIMensional" ),
    /* 3 */ SEnumEntry( static_cast<int>(EDimensionType::Undefined),       "Undefined",       "?", "Undefined",        "UND",  "UNDefined"    )
};
#endif // #if QT_VERSION < 0x050000


/*==============================================================================
enum EArrayIndex
==============================================================================*/

//------------------------------------------------------------------------------
/*! Negative values for array indices are invalid. Some of those negative values
    are used to indicate special ranges or special elements within the array.
    This method checks whether the provide index is one of those special range
    indices and returns the special range in human readable string represantion.
    If the index is greater or equal than zero this index value is converted.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @param i_idx [in] Index to be converted.
    @param i_alias [in] Desired type of string. If set to Symbol a short version
                        will be returned. Otherwise a longer string.

    @return String representation of the given enumerator.
*/
QString ZS::System::arrayIndex2Str( int i_idx, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_idx >= 0 )
    {
        str = QString::number(i_idx);
    }
    else if( i_idx == EArrayIndexUndefined )
    {
        if( i_alias == EEnumEntryAliasStrSymbol )
        {
            str = "U";
        }
        else
        {
            str = "Undefined";
        }
    }
    else if( i_idx == EArrayIndexFirstElement )
    {
        if( i_alias == EEnumEntryAliasStrSymbol )
        {
            str = "F";
        }
        else
        {
            str = "First";
        }
    }
    else if( i_idx == EArrayIndexLastElement )
    {
        if( i_alias == EEnumEntryAliasStrSymbol )
        {
            str = "L";
        }
        else
        {
            str = "Last";
        }
    }
    else if( i_idx == EArrayIndexCountAllElements )
    {
        if( i_alias == EEnumEntryAliasStrSymbol )
        {
            str = "A";
        }
        else
        {
            str = "All";
        }
    }
    else
    {
        str = "OutOfRange (" + QString::number(i_idx) + ")";
    }

    return str;

} // arrayIndex2Str

//------------------------------------------------------------------------------
/*! Checks whether the given index is in the allowed range.

    @ingroup _GRP_Namespace_ZS_System_AuxiliaryMethods

    @param i_idx [in] Index to be checked.
    @param i_iArrLen [in] Array length for the index. The index must be in range [0..(ArrLen-1)].
    @param i_bAllowSpecialArrayIndices [in] Defines whether the special array index definitions
                      (some negative index values) are allowed.

    @return true if the index is in the allowed range - false otherwise.
*/
bool ZS::System::isValidIndex( int i_idx, int i_iArrLen, bool i_bAllowSpecialArrayIndices )
//------------------------------------------------------------------------------
{
    bool bIsValid = false;

    if( i_iArrLen > 0 )
    {
        if( i_idx >= 0 && i_idx < i_iArrLen )
        {
            bIsValid = true;
        }
        else if( i_bAllowSpecialArrayIndices )
        {
            if( i_idx == EArrayIndexFirstElement || i_idx == EArrayIndexLastElement )
            {
                bIsValid = true;
            }
        }
    }

    return bIsValid;

} // isValidIndex

//------------------------------------------------------------------------------
/*! Checks whether a section within an array defined by start index and number
    of indices is in the allowed range.

    @ingroup _GRP_Namespace_ZS_System_AuxiliaryMethods

    @param i_idxStart [in] Start index of the section to be checked.
    @param i_iIdxCount [in] Number of indices of the section to be checked.
    @param i_iArrLen [in] Array length for the index. The index must be in range [0..(ArrLen-1)].
    @param i_bAllowSpecialArrayIndices [in] Defines whether the special array index definitions
                      (some negative index values) are allowed.

    @return true if the section is in the allowed range - false otherwise.
*/
bool ZS::System::isValidRange( int i_idxStart, int i_iIdxCount, int i_iArrLen, bool i_bAllowSpecialArrayIndices )
//------------------------------------------------------------------------------
{
    bool bIsValid = false;

    if( i_iArrLen > 0 )
    {
        int idxStart  = i_idxStart;
        int iIdxCount = i_iIdxCount;

        bIsValid = true;

        if( i_idxStart < 0 )
        {
            bIsValid = false;

            if( i_bAllowSpecialArrayIndices )
            {
                if( i_idxStart == EArrayIndexFirstElement )
                {
                    idxStart = 0;
                    bIsValid = true;
                }
                else if( i_idxStart == EArrayIndexLastElement )
                {
                    idxStart = i_iArrLen-1;
                    bIsValid = true;
                }
            }
        }

        if( bIsValid && i_iIdxCount < 0 )
        {
            bIsValid = false;

            if( i_iIdxCount == EArrayIndexCountAllElements )
            {
                iIdxCount = i_iArrLen - idxStart;
                bIsValid = true;
            }
        }

        if( bIsValid )
        {
            bIsValid = false;

            if( i_bAllowSpecialArrayIndices )
            {
                if( idxStart + iIdxCount <= i_iArrLen )
                {
                    bIsValid = true;
                }
            }
        }
    }

    return bIsValid;

} // isValidRange

//------------------------------------------------------------------------------
/*! Aligns the given index count to the given range.

    @ingroup _GRP_Namespace_ZS_System_AuxiliaryMethods

    @param i_idxCount [in] Index count of the section to be aligned.
                     EArrayIndexCountAllElements may also be used to adjust
                     the index to the maximum count.
    @param i_iArrLen [in] Array length for the index. The resulting index will be in range [0..(ArrLen-1)].
    @param i_bLimitToArrLen [in] Defines whether the returned index value should
                      be adjusted to the maximum allowed value.

    @return Aligned index.
*/
int ZS::System::toArrayLen( int i_idxCount, int i_iArrLen, bool i_bLimitToArrLen )
//------------------------------------------------------------------------------
{
    int idxCount = 0;

    if( i_idxCount == EArrayIndexCountAllElements )
    {
        idxCount = i_iArrLen;
    }
    else if( i_idxCount >= 0 )
    {
        idxCount = i_idxCount;

        if( i_bLimitToArrLen && i_idxCount > i_iArrLen )
        {
            idxCount = i_iArrLen;
        }
    }
    return idxCount;

} // toArrayLen

//------------------------------------------------------------------------------
/*! Aligns the given index count to the given range.

    @ingroup _GRP_Namespace_ZS_System_AuxiliaryMethods

    @param i_idx [in] Index to be aligned.
                     EArrayIndexFirstElement may also be used to adjust the index to the minimum possible value.
                     EArrayIndexLastElement may also be used to adjust the index to the maximum possible value.
    @param i_iArrLen [in] Array length for the index. The resulting index will be in range [0..(ArrLen-1)].
    @param i_bLimitToArrLen [in] Defines whether the returned index value should
                      be adjusted to be within the minimum and maximum allowed value.

    @return Aligned index.
*/
int ZS::System::toArrayIndex( int i_idx, int i_iArrLen, bool i_bLimitToArrLen )
//------------------------------------------------------------------------------
{
    int idx = i_idx;

    if( i_idx == EArrayIndexFirstElement )
    {
        if( i_iArrLen > 0 )
        {
            idx = 0;
        }
        else
        {
            idx = -1;
        }
    }
    else if( i_idx == EArrayIndexLastElement )
    {
        if( i_iArrLen > 0 )
        {
            idx = i_iArrLen-1;
        }
        else
        {
            idx = -1;
        }
    }
    else if( i_idx >= 0 )
    {
        idx = i_idx;

        if( i_bLimitToArrLen && idx >= i_iArrLen )
        {
            idx = i_iArrLen > 0 ? i_iArrLen-1 : 0;
        }
    }
    return idx;

} // toArrayIndex

//------------------------------------------------------------------------------
/*! Returns the minumum and maximul index values of the given section.

    @ingroup _GRP_Namespace_ZS_System_AuxiliaryMethods

    @param i_iArrIdx [in] Index to be aligned.
                     EArrayIndexFirstElement may also be used to adjust the range
                     to just comprise the first element in the array.
                     EArrayIndexLastElement may also be used to adjust the range
                     to just comprise the last element in the array.
                     EArrayIndexCountAllElements may also be used to adjust the range
                     to comprise the whole valid range.
    @param i_iArrLen [in] Array length for the index. The resulting index will be in range [0..(ArrLen-1)].
    @param o_pIdxMin [out] Minimum index value of the range.
    @param o_pIdxMax [out] Maximum index value of the range.
*/
void ZS::System::getMinMaxArrayRange(
    int  i_iArrIdx,
    int  i_iArrLen,
    int* o_pIdxMin,
    int* o_pIdxMax )
//------------------------------------------------------------------------------
{
    int idxMin = 0;
    int idxMax = -1;

    if( i_iArrLen > 0 )
    {
        if( i_iArrIdx >= 0 && i_iArrIdx < i_iArrLen )
        {
            idxMin = i_iArrIdx;
            idxMax = i_iArrIdx;
        }
        else if( i_iArrIdx == EArrayIndexFirstElement )
        {
            idxMin = 0;
            idxMax = 0;
        }
        else if( i_iArrIdx == EArrayIndexLastElement )
        {
            idxMin = i_iArrLen-1;
            idxMax = i_iArrLen-1;
        }
        else if( i_iArrIdx == EArrayIndexCountAllElements )
        {
            idxMin = 0;
            idxMax = i_iArrLen-1;
        }
    }

    if( o_pIdxMin != nullptr )
    {
        *o_pIdxMin = idxMin;
    }
    if( o_pIdxMax != nullptr )
    {
        *o_pIdxMax = idxMax;
    }

} // getMinMaxArrayRange

//------------------------------------------------------------------------------
/*! Returns the minumum and maximul index values of the given section.

    @ingroup _GRP_Namespace_ZS_System_AuxiliaryMethods

    @param i_idxStart [in] Start index of the section to be aligned.
                     EArrayIndexFirstElement may also be used to adjust the range
                     to start from the first element in the array.
                     EArrayIndexLastElement may also be used to adjust the range
                     to start from the last element in the array.
    @param i_iIdxCount [in] Number of indices in the section to be aligned.
                     EArrayIndexCountAllElements may also be used to adjust the range
                     to comprise the whole valid range.
    @param i_iArrLen [in] Array length for the index. The resulting index will be in range [0..(ArrLen-1)].
    @param o_pIdxMin [out] Minimum index value of the range.
    @param o_pIdxMax [out] Maximum index value of the range.
*/
void ZS::System::getMinMaxArrayRange(
    int  i_idxStart,
    int  i_iIdxCount,
    int  i_iArrLen,
    int* o_pIdxMin,
    int* o_pIdxMax )
//------------------------------------------------------------------------------
{
    int idxMin = 0;
    int idxMax = -1;

    if( i_iArrLen > 0 )
    {
        int idxStart  = i_idxStart;
        int iIdxCount = i_iIdxCount;

        if( i_idxStart < 0 )
        {
            if( i_idxStart == EArrayIndexFirstElement )
            {
                idxStart = 0;
            }
            else if( i_idxStart == EArrayIndexLastElement )
            {
                idxStart = i_iArrLen-1;
            }
        }

        if( i_iIdxCount == EArrayIndexCountAllElements )
        {
            iIdxCount = i_iArrLen - idxStart;
        }

        if( idxStart >= 0 && (idxStart + iIdxCount <= i_iArrLen) )
        {
            idxMin = idxStart;
            idxMax = idxStart + iIdxCount - 1;
        }
    }

    if( o_pIdxMin != nullptr )
    {
        *o_pIdxMin = idxMin;
    }
    if( o_pIdxMax != nullptr )
    {
        *o_pIdxMax = idxMax;
    }

} // getMinMaxArrayRange

//------------------------------------------------------------------------------
/*! Returns the minumum index value and the number of indices of the given section.

    @ingroup _GRP_Namespace_ZS_System_AuxiliaryMethods

    @param i_idxStart [in] Start index of the section to be aligned.
                     EArrayIndexFirstElement may also be used to adjust the range
                     to start from the first element in the array.
                     EArrayIndexLastElement may also be used to adjust the range
                     to start from the last element in the array.
    @param i_iArrLen [in] Array length for the index. The resulting index will be in range [0..(ArrLen-1)].
    @param o_pIdxMin [out] Minimum index value of the range.
    @param o_piIdxCount [out] Number of indices in the range.
*/
void ZS::System::getMinCountArrayRange(
    int  i_idxStart,
    int  i_iArrLen,
    int* o_pIdxMin,
    int* o_piIdxCount )
//------------------------------------------------------------------------------
{
    int idxMin;
    int idxMax;

    getMinMaxArrayRange( i_idxStart, i_iArrLen, &idxMin, &idxMax );

    if( o_pIdxMin != nullptr )
    {
        *o_pIdxMin = idxMin;
    }
    if( o_piIdxCount != nullptr )
    {
        *o_piIdxCount = idxMax - idxMin + 1;
    }

} // getMinCountArrayRange

//------------------------------------------------------------------------------
/*! Returns the minumum index value and the number of indices of the given section.

    @ingroup _GRP_Namespace_ZS_System_AuxiliaryMethods

    @param i_idxStart [in] Start index of the section to be aligned.
                     EArrayIndexFirstElement may also be used to adjust the range
                     to start from the first element in the array.
                     EArrayIndexLastElement may also be used to adjust the range
                     to start from the last element in the array.
    @param i_iIdxCount [in] Number of indices in the section to be aligned.
                     EArrayIndexCountAllElements may also be used to adjust the range
                     to comprise the whole valid range.
    @param i_iArrLen [in] Array length for the index. The resulting index will be in range [0..(ArrLen-1)].
    @param o_pIdxMin [out] Minimum index value of the range.
    @param o_piIdxCount [out] Number of indices in the range.
*/
void ZS::System::getMinCountArrayRange(
    int  i_idxStart,
    int  i_iIdxCount,
    int  i_iArrLen,
    int* o_pIdxMin,
    int* o_piIdxCount )
//------------------------------------------------------------------------------
{
    int idxMin;
    int idxMax;

    getMinMaxArrayRange( i_idxStart, i_iIdxCount, i_iArrLen, &idxMin, &idxMax );

    if( o_pIdxMin != nullptr )
    {
        *o_pIdxMin = idxMin;
    }
    if( o_piIdxCount != nullptr )
    {
        *o_piIdxCount = idxMax - idxMin + 1;
    }

} // getMinCountArrayRange


/*==============================================================================
Enum ETraceDetailLevel
==============================================================================*/

//------------------------------------------------------------------------------
static const SEnumEntry s_arEnumStrTrcDetailLevel[] =
//------------------------------------------------------------------------------
{
                         // Enumerator,                              Name,            Symbol, Text,           SCPIShort, SCPILong
    /*  0 */ SEnumEntry( ZS::Trace::ETraceDetailLevelNone,           "None",           "-",    "None",            "NONE", "NONE"        ),
    /*  1 */ SEnumEntry( ZS::Trace::ETraceDetailLevelMethodCalls,    "MethodCalls",    "Mth",  "Method Calls",    "METH", "METHodcalls" ),
    /*  2 */ SEnumEntry( ZS::Trace::ETraceDetailLevelMethodArgs,     "MethodArgs",     "Args", "Method Args",     "ARG",  "ARGuments"   ),
    /*  3 */ SEnumEntry( ZS::Trace::ETraceDetailLevelInternalStates, "InternalStates", "Int",  "Internal States", "STAT", "STATes"      ),
    /*  4 */ SEnumEntry( ZS::Trace::ETraceDetailLevelRuntimeInfo,    "RuntimeInfo",    "Inf",  "Runtime Info",    "INFO", "INFO"        ),
    /*  5 */ SEnumEntry( ZS::Trace::ETraceDetailLevelVerbose,        "Verbose",        "Vbs",  "Verbose",         "VERB", "VERBose"     ),
    /*  6 */ SEnumEntry( ZS::Trace::ETraceDetailLevelCount,          "Count",          "Cnt",  "Count",           "COUN", "COUNt"       )
};

//------------------------------------------------------------------------------
/*! Converts the given enumerator into a string.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @param i_iDetailLevel [in] Detail level to be converted.
    @param i_alias [in] Range [IdxName, Symbol, Text, SCPIShort, SCPILong].
                        Selects one of the available strings.

    @return String representation of the given enumerator.
*/
QString ZS::Trace::traceDetailLevel2Str( int i_iDetailLevel, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_iDetailLevel == ETraceDetailLevelUndefined )
    {
        if( i_alias == EEnumEntryAliasStrSymbol )
        {
            str = "?";
        }
        else if( i_alias == EEnumEntryAliasStrText )
        {
            str = "Undefined";
        }
        else if( i_alias == EEnumEntryAliasStrSCPIShort )
        {
            str = "UND";
        }
        else if( i_alias == EEnumEntryAliasStrSCPILong )
        {
            str = "UNDefined";
        }
        else // if( i_alias == EEnumEntryAliasStrName )
        {
            str = "Undefined";
        }
    }
    else if( i_iDetailLevel >= 0 && i_iDetailLevel < _ZSArrLen(s_arEnumStrTrcDetailLevel) )
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrTrcDetailLevel, _ZSArrLen(s_arEnumStrTrcDetailLevel), i_iDetailLevel, i_alias);
    }
    else
    {
        str = QString::number(i_iDetailLevel);
    }
    return str;

} // traceDetailLevel2Str

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
int ZS::Trace::str2TraceDetailLevel( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    bool bConverted = false;

    int iDetailLevel = i_str.toInt(&bConverted);

    if( !bConverted )
    {
        iDetailLevel = ETraceDetailLevelUndefined;

        if( i_str.isEmpty() )
        {
            iDetailLevel = ETraceDetailLevelNone;
        }
        else if( i_str.compare("-", Qt::CaseInsensitive) == 0 )
        {
            iDetailLevel = ETraceDetailLevelNone;
        }

        if( iDetailLevel == ETraceDetailLevelUndefined )
        {
            if( i_str.compare("?", Qt::CaseInsensitive) == 0 )
            {
                iDetailLevel = ETraceDetailLevelUndefined;
            }
            else if( i_str.compare("Und", Qt::CaseInsensitive) == 0 )
            {
                iDetailLevel = ETraceDetailLevelUndefined;
            }
            else if( i_str.compare("Undefined", Qt::CaseInsensitive) == 0 )
            {
                iDetailLevel = ETraceDetailLevelUndefined;
            }
            else
            {
                iDetailLevel = SEnumEntry::str2Enumerator(s_arEnumStrTrcDetailLevel, _ZSArrLen(s_arEnumStrTrcDetailLevel), i_str, i_alias, Qt::CaseInsensitive);
                if( iDetailLevel < 0 || iDetailLevel >= ETraceDetailLevelCount )
                {
                    iDetailLevel = ETraceDetailLevelUndefined;
                }
            }
        }
    } // if( !bConverted )

    return iDetailLevel;

} // str2TraceDetailLevel
