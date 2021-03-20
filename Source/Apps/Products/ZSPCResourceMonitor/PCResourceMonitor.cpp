/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib,"wbemuuid.lib")

#include <QtCore/qcoreapplication.h>
#include <QtCore/qfile.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>
#include <QtCore/qtextstream.h>
#include <QtNetwork/qhostinfo.h>

#include "PCResourceMonitor.h"
#include "PCMemoryModel.h"

#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizes.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizeTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::PCResourceMonitor;


/*******************************************************************************
WBEM Error Messages
*******************************************************************************/

#define VALID EEnumEntryValidityNormal

static const SEnumEntry s_arEnumStrWbemErrCodes[] =
{
    /* 0x00040001 */ SEnumEntry(   0, "WBEM_S_ALREADY_EXISTS", WBEM_S_ALREADY_EXISTS ),
    /* 0x00040002 */ SEnumEntry(   1, "WBEM_S_RESET_TO_DEFAULT", WBEM_S_RESET_TO_DEFAULT ),
    /* 0x00040003 */ SEnumEntry(   2, "WBEM_S_DIFFERENT", WBEM_S_DIFFERENT ),
    /* 0x00040004 */ SEnumEntry(   3, "WBEM_S_TIMEDOUT", WBEM_S_TIMEDOUT ),
    /* 0x00040005 */ SEnumEntry(   4, "WBEM_S_NO_MORE_DATA", WBEM_S_NO_MORE_DATA ),
    /* 0x00040006 */ SEnumEntry(   5, "WBEM_S_OPERATION_CANCELLED", WBEM_S_OPERATION_CANCELLED ),
    /* 0x00040007 */ SEnumEntry(   6, "WBEM_S_PENDING", WBEM_S_PENDING ),
    /* 0x00040008 */ SEnumEntry(   7, "WBEM_S_DUPLICATE_OBJECTS", WBEM_S_DUPLICATE_OBJECTS ),
    /* 0x00040009 */ SEnumEntry(   8, "WBEM_S_ACCESS_DENIED", WBEM_S_ACCESS_DENIED ),
    /* 0x00040010 */ SEnumEntry(   9, "WBEM_S_PARTIAL_RESULTS", WBEM_S_PARTIAL_RESULTS ),
    /* 0x00040017 */ SEnumEntry(  10, "WBEM_S_SOURCE_NOT_AVAILABLE", WBEM_S_SOURCE_NOT_AVAILABLE ),
    /* 0x80041001 */ SEnumEntry(  11, "WBEM_E_FAILED", WBEM_E_FAILED ),
    /* 0x80041002 */ SEnumEntry(  12, "WBEM_E_NOT_FOUND", WBEM_E_NOT_FOUND ),
    /* 0x80041003 */ SEnumEntry(  13, "WBEM_E_ACCESS_DENIED", WBEM_E_ACCESS_DENIED ),
    /* 0x80041004 */ SEnumEntry(  14, "WBEM_E_PROVIDER_FAILURE", WBEM_E_PROVIDER_FAILURE ),
    /* 0x80041005 */ SEnumEntry(  15, "WBEM_E_TYPE_MISMATCH", WBEM_E_TYPE_MISMATCH ),
    /* 0x80041006 */ SEnumEntry(  16, "WBEM_E_OUT_OF_MEMORY", WBEM_E_OUT_OF_MEMORY ),
    /* 0x80041007 */ SEnumEntry(  17, "WBEM_E_INVALID_CONTEXT", WBEM_E_INVALID_CONTEXT ),
    /* 0x80041008 */ SEnumEntry(  18, "WBEM_E_INVALID_PARAMETER", WBEM_E_INVALID_PARAMETER ),
    /* 0x80041009 */ SEnumEntry(  19, "WBEM_E_NOT_AVAILABLE", WBEM_E_NOT_AVAILABLE ),
    /* 0x8004100a */ SEnumEntry(  20, "WBEM_E_CRITICAL_ERROR", WBEM_E_CRITICAL_ERROR ),
    /* 0x8004100b */ SEnumEntry(  21, "WBEM_E_INVALID_STREAM", WBEM_E_INVALID_STREAM ),
    /* 0x8004100c */ SEnumEntry(  22, "WBEM_E_NOT_SUPPORTED", WBEM_E_NOT_SUPPORTED ),
    /* 0x8004100d */ SEnumEntry(  23, "WBEM_E_INVALID_SUPERCLASS", WBEM_E_INVALID_SUPERCLASS ),
    /* 0x8004100e */ SEnumEntry(  24, "WBEM_E_INVALID_NAMESPACE", WBEM_E_INVALID_NAMESPACE ),
    /* 0x8004100f */ SEnumEntry(  25, "WBEM_E_INVALID_OBJECT", WBEM_E_INVALID_OBJECT ),
    /* 0x80041010 */ SEnumEntry(  26, "WBEM_E_INVALID_CLASS", WBEM_E_INVALID_CLASS ),
    /* 0x80041011 */ SEnumEntry(  27, "WBEM_E_PROVIDER_NOT_FOUND", WBEM_E_PROVIDER_NOT_FOUND ),
    /* 0x80041012 */ SEnumEntry(  28, "WBEM_E_INVALID_PROVIDER_REGISTRATION", WBEM_E_INVALID_PROVIDER_REGISTRATION ),
    /* 0x80041013 */ SEnumEntry(  29, "WBEM_E_PROVIDER_LOAD_FAILURE", WBEM_E_PROVIDER_LOAD_FAILURE ),
    /* 0x80041014 */ SEnumEntry(  30, "WBEM_E_INITIALIZATION_FAILURE", WBEM_E_INITIALIZATION_FAILURE ),
    /* 0x80041015 */ SEnumEntry(  31, "WBEM_E_TRANSPORT_FAILURE", WBEM_E_TRANSPORT_FAILURE ),
    /* 0x80041016 */ SEnumEntry(  32, "WBEM_E_INVALID_OPERATION", WBEM_E_INVALID_OPERATION ),
    /* 0x80041017 */ SEnumEntry(  33, "WBEM_E_INVALID_QUERY", WBEM_E_INVALID_QUERY ),
    /* 0x80041018 */ SEnumEntry(  34, "WBEM_E_INVALID_QUERY_TYPE", WBEM_E_INVALID_QUERY_TYPE ),
    /* 0x80041019 */ SEnumEntry(  35, "WBEM_E_ALREADY_EXISTS", WBEM_E_ALREADY_EXISTS ),
    /* 0x8004101a */ SEnumEntry(  36, "WBEM_E_OVERRIDE_NOT_ALLOWED", WBEM_E_OVERRIDE_NOT_ALLOWED ),
    /* 0x8004101b */ SEnumEntry(  37, "WBEM_E_PROPAGATED_QUALIFIER", WBEM_E_PROPAGATED_QUALIFIER ),
    /* 0x8004101c */ SEnumEntry(  38, "WBEM_E_PROPAGATED_PROPERTY", WBEM_E_PROPAGATED_PROPERTY ),
    /* 0x8004101d */ SEnumEntry(  39, "WBEM_E_UNEXPECTED", WBEM_E_UNEXPECTED ),
    /* 0x8004101e */ SEnumEntry(  40, "WBEM_E_ILLEGAL_OPERATION", WBEM_E_ILLEGAL_OPERATION ),
    /* 0x8004101f */ SEnumEntry(  41, "WBEM_E_CANNOT_BE_KEY", WBEM_E_CANNOT_BE_KEY ),
    /* 0x80041020 */ SEnumEntry(  42, "WBEM_E_INCOMPLETE_CLASS", WBEM_E_INCOMPLETE_CLASS ),
    /* 0x80041021 */ SEnumEntry(  43, "WBEM_E_INVALID_SYNTAX", WBEM_E_INVALID_SYNTAX ),
    /* 0x80041022 */ SEnumEntry(  44, "WBEM_E_NONDECORATED_OBJECT", WBEM_E_NONDECORATED_OBJECT ),
    /* 0x80041023 */ SEnumEntry(  45, "WBEM_E_READ_ONLY", WBEM_E_READ_ONLY ),
    /* 0x80041024 */ SEnumEntry(  46, "WBEM_E_PROVIDER_NOT_CAPABLE", WBEM_E_PROVIDER_NOT_CAPABLE ),
    /* 0x80041025 */ SEnumEntry(  47, "WBEM_E_CLASS_HAS_CHILDREN", WBEM_E_CLASS_HAS_CHILDREN ),
    /* 0x80041026 */ SEnumEntry(  48, "WBEM_E_CLASS_HAS_INSTANCES", WBEM_E_CLASS_HAS_INSTANCES ),
    /* 0x80041027 */ SEnumEntry(  49, "WBEM_E_QUERY_NOT_IMPLEMENTED", WBEM_E_QUERY_NOT_IMPLEMENTED ),
    /* 0x80041028 */ SEnumEntry(  50, "WBEM_E_ILLEGAL_NULL", WBEM_E_ILLEGAL_NULL ),
    /* 0x80041029 */ SEnumEntry(  51, "WBEM_E_INVALID_QUALIFIER_TYPE", WBEM_E_INVALID_QUALIFIER_TYPE ),
    /* 0x8004102a */ SEnumEntry(  52, "WBEM_E_INVALID_PROPERTY_TYPE", WBEM_E_INVALID_PROPERTY_TYPE ),
    /* 0x8004102b */ SEnumEntry(  53, "WBEM_E_VALUE_OUT_OF_RANGE", WBEM_E_VALUE_OUT_OF_RANGE ),
    /* 0x8004102c */ SEnumEntry(  54, "WBEM_E_CANNOT_BE_SINGLETON", WBEM_E_CANNOT_BE_SINGLETON ),
    /* 0x8004102d */ SEnumEntry(  55, "WBEM_E_INVALID_CIM_TYPE", WBEM_E_INVALID_CIM_TYPE ),
    /* 0x8004102e */ SEnumEntry(  56, "WBEM_E_INVALID_METHOD", WBEM_E_INVALID_METHOD ),
    /* 0x8004102f */ SEnumEntry(  57, "WBEM_E_INVALID_METHOD_PARAMETERS", WBEM_E_INVALID_METHOD_PARAMETERS ),
    /* 0x80041030 */ SEnumEntry(  58, "WBEM_E_SYSTEM_PROPERTY", WBEM_E_SYSTEM_PROPERTY ),
    /* 0x80041031 */ SEnumEntry(  59, "WBEM_E_INVALID_PROPERTY", WBEM_E_INVALID_PROPERTY ),
    /* 0x80041032 */ SEnumEntry(  60, "WBEM_E_CALL_CANCELLED", WBEM_E_CALL_CANCELLED ),
    /* 0x80041033 */ SEnumEntry(  61, "WBEM_E_SHUTTING_DOWN", WBEM_E_SHUTTING_DOWN ),
    /* 0x80041034 */ SEnumEntry(  62, "WBEM_E_PROPAGATED_METHOD", WBEM_E_PROPAGATED_METHOD ),
    /* 0x80041035 */ SEnumEntry(  63, "WBEM_E_UNSUPPORTED_PARAMETER", WBEM_E_UNSUPPORTED_PARAMETER ),
    /* 0x80041036 */ SEnumEntry(  64, "WBEM_E_MISSING_PARAMETER_ID", WBEM_E_MISSING_PARAMETER_ID ),
    /* 0x80041037 */ SEnumEntry(  65, "WBEM_E_INVALID_PARAMETER_ID", WBEM_E_INVALID_PARAMETER_ID ),
    /* 0x80041038 */ SEnumEntry(  66, "WBEM_E_NONCONSECUTIVE_PARAMETER_IDS", WBEM_E_NONCONSECUTIVE_PARAMETER_IDS ),
    /* 0x80041039 */ SEnumEntry(  67, "WBEM_E_PARAMETER_ID_ON_RETVAL", WBEM_E_PARAMETER_ID_ON_RETVAL ),
    /* 0x8004103a */ SEnumEntry(  68, "WBEM_E_INVALID_OBJECT_PATH", WBEM_E_INVALID_OBJECT_PATH ),
    /* 0x8004103b */ SEnumEntry(  69, "WBEM_E_OUT_OF_DISK_SPACE", WBEM_E_OUT_OF_DISK_SPACE ),
    /* 0x8004103c */ SEnumEntry(  70, "WBEM_E_BUFFER_TOO_SMALL", WBEM_E_BUFFER_TOO_SMALL ),
    /* 0x8004103d */ SEnumEntry(  71, "WBEM_E_UNSUPPORTED_PUT_EXTENSION", WBEM_E_UNSUPPORTED_PUT_EXTENSION ),
    /* 0x8004103e */ SEnumEntry(  72, "WBEM_E_UNKNOWN_OBJECT_TYPE", WBEM_E_UNKNOWN_OBJECT_TYPE ),
    /* 0x8004103f */ SEnumEntry(  73, "WBEM_E_UNKNOWN_PACKET_TYPE", WBEM_E_UNKNOWN_PACKET_TYPE ),
    /* 0x80041040 */ SEnumEntry(  74, "WBEM_E_MARSHAL_VERSION_MISMATCH", WBEM_E_MARSHAL_VERSION_MISMATCH ),
    /* 0x80041041 */ SEnumEntry(  75, "WBEM_E_MARSHAL_INVALID_SIGNATURE", WBEM_E_MARSHAL_INVALID_SIGNATURE ),
    /* 0x80041042 */ SEnumEntry(  76, "WBEM_E_INVALID_QUALIFIER", WBEM_E_INVALID_QUALIFIER ),
    /* 0x80041043 */ SEnumEntry(  77, "WBEM_E_INVALID_DUPLICATE_PARAMETER", WBEM_E_INVALID_DUPLICATE_PARAMETER ),
    /* 0x80041044 */ SEnumEntry(  78, "WBEM_E_TOO_MUCH_DATA", WBEM_E_TOO_MUCH_DATA ),
    /* 0x80041045 */ SEnumEntry(  79, "WBEM_E_SERVER_TOO_BUSY", WBEM_E_SERVER_TOO_BUSY ),
    /* 0x80041046 */ SEnumEntry(  80, "WBEM_E_INVALID_FLAVOR", WBEM_E_INVALID_FLAVOR ),
    /* 0x80041047 */ SEnumEntry(  81, "WBEM_E_CIRCULAR_REFERENCE", WBEM_E_CIRCULAR_REFERENCE ),
    /* 0x80041048 */ SEnumEntry(  82, "WBEM_E_UNSUPPORTED_CLASS_UPDATE", WBEM_E_UNSUPPORTED_CLASS_UPDATE ),
    /* 0x80041049 */ SEnumEntry(  83, "WBEM_E_CANNOT_CHANGE_KEY_INHERITANCE", WBEM_E_CANNOT_CHANGE_KEY_INHERITANCE ),
    /* 0x80041050 */ SEnumEntry(  84, "WBEM_E_CANNOT_CHANGE_INDEX_INHERITANCE", WBEM_E_CANNOT_CHANGE_INDEX_INHERITANCE ),
    /* 0x80041051 */ SEnumEntry(  85, "WBEM_E_TOO_MANY_PROPERTIES", WBEM_E_TOO_MANY_PROPERTIES ),
    /* 0x80041052 */ SEnumEntry(  86, "WBEM_E_UPDATE_TYPE_MISMATCH", WBEM_E_UPDATE_TYPE_MISMATCH ),
    /* 0x80041053 */ SEnumEntry(  87, "WBEM_E_UPDATE_OVERRIDE_NOT_ALLOWED", WBEM_E_UPDATE_OVERRIDE_NOT_ALLOWED ),
    /* 0x80041054 */ SEnumEntry(  88, "WBEM_E_UPDATE_PROPAGATED_METHOD", WBEM_E_UPDATE_PROPAGATED_METHOD ),
    /* 0x80041055 */ SEnumEntry(  89, "WBEM_E_METHOD_NOT_IMPLEMENTED", WBEM_E_METHOD_NOT_IMPLEMENTED ),
    /* 0x80041056 */ SEnumEntry(  90, "WBEM_E_METHOD_DISABLED", WBEM_E_METHOD_DISABLED ),
    /* 0x80041057 */ SEnumEntry(  91, "WBEM_E_REFRESHER_BUSY", WBEM_E_REFRESHER_BUSY ),
    /* 0x80041058 */ SEnumEntry(  92, "WBEM_E_UNPARSABLE_QUERY", WBEM_E_UNPARSABLE_QUERY ),
    /* 0x80041059 */ SEnumEntry(  93, "WBEM_E_NOT_EVENT_CLASS", WBEM_E_NOT_EVENT_CLASS ),
    /* 0x8004105a */ SEnumEntry(  94, "WBEM_E_MISSING_GROUP_WITHIN", WBEM_E_MISSING_GROUP_WITHIN ),
    /* 0x8004105b */ SEnumEntry(  95, "WBEM_E_MISSING_AGGREGATION_LIST", WBEM_E_MISSING_AGGREGATION_LIST ),
    /* 0x8004105c */ SEnumEntry(  96, "WBEM_E_PROPERTY_NOT_AN_OBJECT", WBEM_E_PROPERTY_NOT_AN_OBJECT ),
    /* 0x8004105d */ SEnumEntry(  97, "WBEM_E_AGGREGATING_BY_OBJECT", WBEM_E_AGGREGATING_BY_OBJECT ),
    /* 0x8004105f */ SEnumEntry(  98, "WBEM_E_UNINTERPRETABLE_PROVIDER_QUERY", WBEM_E_UNINTERPRETABLE_PROVIDER_QUERY ),
    /* 0x80041060 */ SEnumEntry(  99, "WBEM_E_BACKUP_RESTORE_WINMGMT_RUNNING", WBEM_E_BACKUP_RESTORE_WINMGMT_RUNNING ),
    /* 0x80041061 */ SEnumEntry( 100, "WBEM_E_QUEUE_OVERFLOW", WBEM_E_QUEUE_OVERFLOW ),
    /* 0x80041062 */ SEnumEntry( 101, "WBEM_E_PRIVILEGE_NOT_HELD", WBEM_E_PRIVILEGE_NOT_HELD ),
    /* 0x80041063 */ SEnumEntry( 102, "WBEM_E_INVALID_OPERATOR", WBEM_E_INVALID_OPERATOR ),
    /* 0x80041064 */ SEnumEntry( 103, "WBEM_E_LOCAL_CREDENTIALS", WBEM_E_LOCAL_CREDENTIALS ),
    /* 0x80041065 */ SEnumEntry( 104, "WBEM_E_CANNOT_BE_ABSTRACT", WBEM_E_CANNOT_BE_ABSTRACT ),
    /* 0x80041066 */ SEnumEntry( 105, "WBEM_E_AMENDED_OBJECT", WBEM_E_AMENDED_OBJECT ),
    /* 0x80041067 */ SEnumEntry( 106, "WBEM_E_CLIENT_TOO_SLOW", WBEM_E_CLIENT_TOO_SLOW ),
    /* 0x80041068 */ SEnumEntry( 107, "WBEM_E_NULL_SECURITY_DESCRIPTOR", WBEM_E_NULL_SECURITY_DESCRIPTOR ),
    /* 0x80041069 */ SEnumEntry( 108, "WBEM_E_TIMED_OUT", WBEM_E_TIMED_OUT ),
    /* 0x8004106a */ SEnumEntry( 109, "WBEM_E_INVALID_ASSOCIATION", WBEM_E_INVALID_ASSOCIATION ),
    /* 0x8004106b */ SEnumEntry( 110, "WBEM_E_AMBIGUOUS_OPERATION", WBEM_E_AMBIGUOUS_OPERATION ),
    /* 0x8004106c */ SEnumEntry( 111, "WBEM_E_QUOTA_VIOLATION", WBEM_E_QUOTA_VIOLATION ),
    /* 0x8004106d */ SEnumEntry( 112, "WBEM_E_RESERVED_001", WBEM_E_RESERVED_001 ),
    /* 0x8004106e */ SEnumEntry( 113, "WBEM_E_RESERVED_002", WBEM_E_RESERVED_002 ),
    /* 0x8004106f */ SEnumEntry( 114, "WBEM_E_UNSUPPORTED_LOCALE", WBEM_E_UNSUPPORTED_LOCALE ),
    /* 0x80041070 */ SEnumEntry( 115, "WBEM_E_HANDLE_OUT_OF_DATE", WBEM_E_HANDLE_OUT_OF_DATE ),
    /* 0x80041071 */ SEnumEntry( 116, "WBEM_E_CONNECTION_FAILED", WBEM_E_CONNECTION_FAILED ),
    /* 0x80041072 */ SEnumEntry( 117, "WBEM_E_INVALID_HANDLE_REQUEST", WBEM_E_INVALID_HANDLE_REQUEST ),
    /* 0x80041073 */ SEnumEntry( 118, "WBEM_E_PROPERTY_NAME_TOO_WIDE", WBEM_E_PROPERTY_NAME_TOO_WIDE ),
    /* 0x80041074 */ SEnumEntry( 119, "WBEM_E_CLASS_NAME_TOO_WIDE", WBEM_E_CLASS_NAME_TOO_WIDE ),
    /* 0x80041075 */ SEnumEntry( 120, "WBEM_E_METHOD_NAME_TOO_WIDE", WBEM_E_METHOD_NAME_TOO_WIDE ),
    /* 0x80041076 */ SEnumEntry( 121, "WBEM_E_QUALIFIER_NAME_TOO_WIDE", WBEM_E_QUALIFIER_NAME_TOO_WIDE ),
    /* 0x80041077 */ SEnumEntry( 122, "WBEM_E_RERUN_COMMAND", WBEM_E_RERUN_COMMAND ),
    /* 0x80041078 */ SEnumEntry( 123, "WBEM_E_DATABASE_VER_MISMATCH", WBEM_E_DATABASE_VER_MISMATCH ),
    /* 0x80041079 */ SEnumEntry( 124, "WBEM_E_VETO_DELETE", WBEM_E_VETO_DELETE ),
    /* 0x8004107a */ SEnumEntry( 125, "WBEM_E_VETO_PUT", WBEM_E_VETO_PUT ),
    /* 0x80041080 */ SEnumEntry( 126, "WBEM_E_INVALID_LOCALE", WBEM_E_INVALID_LOCALE ),
    /* 0x80041081 */ SEnumEntry( 127, "WBEM_E_PROVIDER_SUSPENDED", WBEM_E_PROVIDER_SUSPENDED ),
    /* 0x80041082 */ SEnumEntry( 128, "WBEM_E_SYNCHRONIZATION_REQUIRED", WBEM_E_SYNCHRONIZATION_REQUIRED ),
    /* 0x80041083 */ SEnumEntry( 129, "WBEM_E_NO_SCHEMA", WBEM_E_NO_SCHEMA ),
    /* 0x80041084 */ SEnumEntry( 130, "WBEM_E_PROVIDER_ALREADY_REGISTERED", WBEM_E_PROVIDER_ALREADY_REGISTERED ),
    /* 0x80041085 */ SEnumEntry( 131, "WBEM_E_PROVIDER_NOT_REGISTERED", WBEM_E_PROVIDER_NOT_REGISTERED ),
    /* 0x80041086 */ SEnumEntry( 132, "WBEM_E_FATAL_TRANSPORT_ERROR", WBEM_E_FATAL_TRANSPORT_ERROR ),
    /* 0x80041087 */ SEnumEntry( 133, "WBEM_E_ENCRYPTED_CONNECTION_REQUIRED", WBEM_E_ENCRYPTED_CONNECTION_REQUIRED ),
    /* 0x80041088 */ SEnumEntry( 134, "WBEM_E_PROVIDER_TIMED_OUT", WBEM_E_PROVIDER_TIMED_OUT ),
    /* 0x80041089 */ SEnumEntry( 135, "WBEM_E_NO_KEY", WBEM_E_NO_KEY ),
    /* 0x8004108a */ SEnumEntry( 136, "WBEM_E_PROVIDER_DISABLED", WBEM_E_PROVIDER_DISABLED ),
    /* 0x80042001 */ SEnumEntry( 137, "WBEMESS_E_REGISTRATION_TOO_BROAD", WBEMESS_E_REGISTRATION_TOO_BROAD ),
    /* 0x80042002 */ SEnumEntry( 138, "WBEMESS_E_REGISTRATION_TOO_PRECISE", WBEMESS_E_REGISTRATION_TOO_PRECISE ),
    /* 0x80042003 */ SEnumEntry( 139, "WBEMESS_E_AUTHZ_NOT_PRIVILEGED", WBEMESS_E_AUTHZ_NOT_PRIVILEGED ),
    /* 0x80044001 */ SEnumEntry( 140, "WBEMMOF_E_EXPECTED_QUALIFIER_NAME", WBEMMOF_E_EXPECTED_QUALIFIER_NAME ),
    /* 0x80044002 */ SEnumEntry( 141, "WBEMMOF_E_EXPECTED_SEMI", WBEMMOF_E_EXPECTED_SEMI ),
    /* 0x80044003 */ SEnumEntry( 142, "WBEMMOF_E_EXPECTED_OPEN_BRACE", WBEMMOF_E_EXPECTED_OPEN_BRACE ),
    /* 0x80044004 */ SEnumEntry( 143, "WBEMMOF_E_EXPECTED_CLOSE_BRACE", WBEMMOF_E_EXPECTED_CLOSE_BRACE ),
    /* 0x80044005 */ SEnumEntry( 144, "WBEMMOF_E_EXPECTED_CLOSE_BRACKET", WBEMMOF_E_EXPECTED_CLOSE_BRACKET ),
    /* 0x80044006 */ SEnumEntry( 145, "WBEMMOF_E_EXPECTED_CLOSE_PAREN", WBEMMOF_E_EXPECTED_CLOSE_PAREN ),
    /* 0x80044007 */ SEnumEntry( 146, "WBEMMOF_E_ILLEGAL_CONSTANT_VALUE", WBEMMOF_E_ILLEGAL_CONSTANT_VALUE ),
    /* 0x80044008 */ SEnumEntry( 147, "WBEMMOF_E_EXPECTED_TYPE_IDENTIFIER", WBEMMOF_E_EXPECTED_TYPE_IDENTIFIER ),
    /* 0x80044009 */ SEnumEntry( 148, "WBEMMOF_E_EXPECTED_OPEN_PAREN", WBEMMOF_E_EXPECTED_OPEN_PAREN ),
    /* 0x8004400a */ SEnumEntry( 149, "WBEMMOF_E_UNRECOGNIZED_TOKEN", WBEMMOF_E_UNRECOGNIZED_TOKEN ),
    /* 0x8004400b */ SEnumEntry( 150, "WBEMMOF_E_UNRECOGNIZED_TYPE", WBEMMOF_E_UNRECOGNIZED_TYPE ),
    /* 0x8004400c */ SEnumEntry( 151, "WBEMMOF_E_EXPECTED_PROPERTY_NAME", WBEMMOF_E_EXPECTED_PROPERTY_NAME ),
    /* 0x8004400d */ SEnumEntry( 152, "WBEMMOF_E_TYPEDEF_NOT_SUPPORTED", WBEMMOF_E_TYPEDEF_NOT_SUPPORTED ),
    /* 0x8004400e */ SEnumEntry( 153, "WBEMMOF_E_UNEXPECTED_ALIAS", WBEMMOF_E_UNEXPECTED_ALIAS ),
    /* 0x8004400f */ SEnumEntry( 154, "WBEMMOF_E_UNEXPECTED_ARRAY_INIT", WBEMMOF_E_UNEXPECTED_ARRAY_INIT ),
    /* 0x80044010 */ SEnumEntry( 155, "WBEMMOF_E_INVALID_AMENDMENT_SYNTAX", WBEMMOF_E_INVALID_AMENDMENT_SYNTAX ),
    /* 0x80044011 */ SEnumEntry( 156, "WBEMMOF_E_INVALID_DUPLICATE_AMENDMENT", WBEMMOF_E_INVALID_DUPLICATE_AMENDMENT ),
    /* 0x80044012 */ SEnumEntry( 157, "WBEMMOF_E_INVALID_PRAGMA", WBEMMOF_E_INVALID_PRAGMA ),
    /* 0x80044013 */ SEnumEntry( 158, "WBEMMOF_E_INVALID_NAMESPACE_SYNTAX", WBEMMOF_E_INVALID_NAMESPACE_SYNTAX ),
    /* 0x80044014 */ SEnumEntry( 159, "WBEMMOF_E_EXPECTED_CLASS_NAME", WBEMMOF_E_EXPECTED_CLASS_NAME ),
    /* 0x80044015 */ SEnumEntry( 160, "WBEMMOF_E_TYPE_MISMATCH", WBEMMOF_E_TYPE_MISMATCH ),
    /* 0x80044016 */ SEnumEntry( 161, "WBEMMOF_E_EXPECTED_ALIAS_NAME", WBEMMOF_E_EXPECTED_ALIAS_NAME ),
    /* 0x80044017 */ SEnumEntry( 162, "WBEMMOF_E_INVALID_CLASS_DECLARATION", WBEMMOF_E_INVALID_CLASS_DECLARATION ),
    /* 0x80044018 */ SEnumEntry( 163, "WBEMMOF_E_INVALID_INSTANCE_DECLARATION", WBEMMOF_E_INVALID_INSTANCE_DECLARATION ),
    /* 0x80044019 */ SEnumEntry( 164, "WBEMMOF_E_EXPECTED_DOLLAR", WBEMMOF_E_EXPECTED_DOLLAR ),
    /* 0x8004401a */ SEnumEntry( 165, "WBEMMOF_E_CIMTYPE_QUALIFIER", WBEMMOF_E_CIMTYPE_QUALIFIER ),
    /* 0x8004401b */ SEnumEntry( 166, "WBEMMOF_E_DUPLICATE_PROPERTY", WBEMMOF_E_DUPLICATE_PROPERTY ),
    /* 0x8004401c */ SEnumEntry( 167, "WBEMMOF_E_INVALID_NAMESPACE_SPECIFICATION", WBEMMOF_E_INVALID_NAMESPACE_SPECIFICATION ),
    /* 0x8004401d */ SEnumEntry( 168, "WBEMMOF_E_OUT_OF_RANGE", WBEMMOF_E_OUT_OF_RANGE ),
    /* 0x8004401e */ SEnumEntry( 169, "WBEMMOF_E_INVALID_FILE", WBEMMOF_E_INVALID_FILE ),
    /* 0x8004401f */ SEnumEntry( 170, "WBEMMOF_E_ALIASES_IN_EMBEDDED", WBEMMOF_E_ALIASES_IN_EMBEDDED ),
    /* 0x80044020 */ SEnumEntry( 171, "WBEMMOF_E_NULL_ARRAY_ELEM", WBEMMOF_E_NULL_ARRAY_ELEM ),
    /* 0x80044021 */ SEnumEntry( 172, "WBEMMOF_E_DUPLICATE_QUALIFIER", WBEMMOF_E_DUPLICATE_QUALIFIER ),
    /* 0x80044022 */ SEnumEntry( 173, "WBEMMOF_E_EXPECTED_FLAVOR_TYPE", WBEMMOF_E_EXPECTED_FLAVOR_TYPE ),
    /* 0x80044023 */ SEnumEntry( 174, "WBEMMOF_E_INCOMPATIBLE_FLAVOR_TYPES", WBEMMOF_E_INCOMPATIBLE_FLAVOR_TYPES ),
    /* 0x80044024 */ SEnumEntry( 175, "WBEMMOF_E_MULTIPLE_ALIASES", WBEMMOF_E_MULTIPLE_ALIASES ),
    /* 0x80044025 */ SEnumEntry( 176, "WBEMMOF_E_INCOMPATIBLE_FLAVOR_TYPES2", WBEMMOF_E_INCOMPATIBLE_FLAVOR_TYPES2 ),
    /* 0x80044026 */ SEnumEntry( 177, "WBEMMOF_E_NO_ARRAYS_RETURNED", WBEMMOF_E_NO_ARRAYS_RETURNED ),
    /* 0x80044027 */ SEnumEntry( 178, "WBEMMOF_E_MUST_BE_IN_OR_OUT", WBEMMOF_E_MUST_BE_IN_OR_OUT ),
    /* 0x80044028 */ SEnumEntry( 179, "WBEMMOF_E_INVALID_FLAGS_SYNTAX", WBEMMOF_E_INVALID_FLAGS_SYNTAX ),
    /* 0x80044029 */ SEnumEntry( 180, "WBEMMOF_E_EXPECTED_BRACE_OR_BAD_TYPE", WBEMMOF_E_EXPECTED_BRACE_OR_BAD_TYPE ),
    /* 0x8004402a */ SEnumEntry( 181, "WBEMMOF_E_UNSUPPORTED_CIMV22_QUAL_VALUE", WBEMMOF_E_UNSUPPORTED_CIMV22_QUAL_VALUE ),
    /* 0x8004402b */ SEnumEntry( 182, "WBEMMOF_E_UNSUPPORTED_CIMV22_DATA_TYPE", WBEMMOF_E_UNSUPPORTED_CIMV22_DATA_TYPE ),
    /* 0x8004402c */ SEnumEntry( 183, "WBEMMOF_E_INVALID_DELETEINSTANCE_SYNTAX", WBEMMOF_E_INVALID_DELETEINSTANCE_SYNTAX ),
    /* 0x8004402d */ SEnumEntry( 184, "WBEMMOF_E_INVALID_QUALIFIER_SYNTAX", WBEMMOF_E_INVALID_QUALIFIER_SYNTAX ),
    /* 0x8004402e */ SEnumEntry( 185, "WBEMMOF_E_QUALIFIER_USED_OUTSIDE_SCOPE", WBEMMOF_E_QUALIFIER_USED_OUTSIDE_SCOPE ),
    /* 0x8004402f */ SEnumEntry( 186, "WBEMMOF_E_ERROR_CREATING_TEMP_FILE", WBEMMOF_E_ERROR_CREATING_TEMP_FILE ),
    /* 0x80044030 */ SEnumEntry( 187, "WBEMMOF_E_ERROR_INVALID_INCLUDE_FILE", WBEMMOF_E_ERROR_INVALID_INCLUDE_FILE ),
    /* 0x80044031 */ SEnumEntry( 188, "WBEMMOF_E_INVALID_DELETECLASS_SYNTAX", WBEMMOF_E_INVALID_DELETECLASS_SYNTAX )
};

//------------------------------------------------------------------------------
static QString wbemErrCode2Str( int i_iErrCode )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrWbemErrCodes,_ZSArrLen(s_arEnumStrWbemErrCodes),i_iErrCode);
}


/*******************************************************************************
struct SAutoUpdateIntervals
*******************************************************************************/

/*==============================================================================
public: // default ctor
==============================================================================*/

//------------------------------------------------------------------------------
SAutoUpdateIntervals::SAutoUpdateIntervals() :
//------------------------------------------------------------------------------
    m_iComputerSystem_ms(600000),
    m_iOperatingSystem_ms(600000),
    m_iMemory_ms(60000),
    m_iMemoryValues_ms(5000),
    m_iProcessor_ms(600000),
    m_iProcessorValues_ms(5000),
    m_iLogicalDisks_ms(300000),
    m_iLogicalDisksValues_ms(30000)
{
} // ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool SAutoUpdateIntervals::operator == ( const SAutoUpdateIntervals& i_intervalsOther )
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_iComputerSystem_ms != i_intervalsOther.m_iComputerSystem_ms )
    {
        bEqual = false;
    }
    else if( m_iOperatingSystem_ms != i_intervalsOther.m_iOperatingSystem_ms )
    {
        bEqual = false;
    }
    else if( m_iProcessor_ms != i_intervalsOther.m_iProcessor_ms )
    {
        bEqual = false;
    }
    else if( m_iProcessorValues_ms != i_intervalsOther.m_iProcessorValues_ms )
    {
        bEqual = false;
    }
    else if( m_iMemory_ms != i_intervalsOther.m_iMemory_ms )
    {
        bEqual = false;
    }
    else if( m_iMemoryValues_ms != i_intervalsOther.m_iMemoryValues_ms )
    {
        bEqual = false;
    }
    else if( m_iLogicalDisks_ms != i_intervalsOther.m_iLogicalDisks_ms )
    {
        bEqual = false;
    }
    else if( m_iLogicalDisksValues_ms != i_intervalsOther.m_iLogicalDisksValues_ms )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SAutoUpdateIntervals::operator != ( const SAutoUpdateIntervals& i_intervalsOther )
//------------------------------------------------------------------------------
{
    return !(*this == i_intervalsOther);
}


/*******************************************************************************
class CPCResourceMonitor : public QObject
*******************************************************************************/

#define M_WBEM_INIT() \
    if( !m_bWbemInitialized ) { \
        initWbem(); \
    }

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* States
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[CPCResourceMonitor::EStateCount] =
//------------------------------------------------------------------------------
{
    SEnumEntry( CPCResourceMonitor::EStateIdle,    "Idle",    "Idle"    ),
    SEnumEntry( CPCResourceMonitor::EStateRunning, "Running", "Running" )
};

//------------------------------------------------------------------------------
QString CPCResourceMonitor::state2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,EStateCount,i_iState);
}

/* Drive Types
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrDriveTypes[CPCResourceMonitor::EDriveTypeCount] =
//------------------------------------------------------------------------------
{
    SEnumEntry( CPCResourceMonitor::EDriveTypeUnknown,       "Unknown",       "Unknown"        ),
    SEnumEntry( CPCResourceMonitor::EDriveTypeNoRootDir,     "NoRootDir",     "No Root Dir"    ),
    SEnumEntry( CPCResourceMonitor::EDriveTypeRemovableDisk, "RemovableDisk", "Removable Disk" ),
    SEnumEntry( CPCResourceMonitor::EDriveTypeLocalDisk,     "LocalDisk",     "Local Disk"     ),
    SEnumEntry( CPCResourceMonitor::EDriveTypeNetworkDrive,  "NetworkDrive",  "Network Drive"  ),
    SEnumEntry( CPCResourceMonitor::EDriveTypeCompactDisk,   "CompactDisk",   "Compact Disk"   ),
    SEnumEntry( CPCResourceMonitor::EDriveTypeRAMDisk,       "RAMDisk",       "RAM Disk"       )
};

////------------------------------------------------------------------------------
//const CA::StdPC::EStorageType s_arStorageTypes[CPCResourceMonitor::EDriveTypeCount] =
////------------------------------------------------------------------------------
//{
//    /* Unknown       */ CA::StdPC::EStorageTypeUndefined,
//    /* NoRootDir     */ CA::StdPC::EStorageTypeOther,
//    /* RemovableDisk */ CA::StdPC::EStorageTypeRemovableDisk,
//    /* LocalDisk     */ CA::StdPC::EStorageTypeFixedDisk,
//    /* NetworkDrive  */ CA::StdPC::EStorageTypeNetworkDisk,
//    /* CompactDisk   */ CA::StdPC::EStorageTypeCompactDisk,
//    /* RAMDisk       */ CA::StdPC::EStorageTypeRamDisk
//};

//------------------------------------------------------------------------------
QString CPCResourceMonitor::driveType2Str( int i_iDriveType )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrDriveTypes,EDriveTypeCount,i_iDriveType);
}

////------------------------------------------------------------------------------
//CA::StdPC::EStorageType CPCResourceMonitor::driveType2StorageType( int i_iDriveType )
////------------------------------------------------------------------------------
//{
//    CA::StdPC::EStorageType storageType = CA::StdPC::EStorageTypeUndefined;
//
//    if( i_iDriveType >= 0 && i_iDriveType < EDriveTypeCount )
//    {
//        storageType = s_arStorageTypes[i_iDriveType];
//    }
//    return storageType;
//
//} // driveType2StorageType

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPCResourceMonitor::CPCResourceMonitor( const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_pErrLog(nullptr),
    m_autoUpdateIntervals(),
    // Wbem interface (web base environment management via dcom)
    m_bWbemInitialized(false),
    m_pIWbemServices(nullptr),
    m_pIWbemRefresher(nullptr),
    // Win32_PerfFormattedData_PerfOS_Processor
    m_pIWbemHiPerfEnumOSProc(nullptr),
    m_bIWbemHiPerfEnumOSProcGotHandles(false),
    m_arpIWbemObjAccessOSProc(nullptr),
    m_uObjAccessOSProcCount(0),
    m_cimTypeOSProcLoad(CIM_EMPTY),
    m_lhndOSProcLoad(0),
    // Computer system
    m_stateAutoUpdateComputerSystem(EStateIdle),
    m_pTimerAutoUpdateComputerSystem(nullptr),
    m_physValComputerSystemManufacturerUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_strComputerSystemManufacturer(),
    m_physValComputerSystemModelUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_strComputerSystemModel(),
    m_physValComputerSerialNrUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_strComputerSerialNr(),
    m_physValComputerSystemDNSHostNameUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_strComputerSystemDNSHostName(),
    m_physValComputerSystemDomainUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_strComputerSystemDomain(),
    // Operating system
    m_stateAutoUpdateOperatingSystem(EStateIdle),
    m_pTimerAutoUpdateOperatingSystem(nullptr),
    m_physValOSCaptionUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_strOSCaption(),
    m_physValOSServicePackUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_strOSServicePack(),
    // Processor
    m_stateAutoUpdateProcessor(EStateIdle),
    m_pTimerAutoUpdateProcessor(nullptr),
    m_physValComputerSystemNumberOfProcessorsUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_uComputerSystemNumberOfProcessors(0),
    m_physValComputerSystemNumberOfLogicalProcessorsUpdSysTime(Kinematics::Time()->MilliSeconds()),
    m_uComputerSystemNumberOfLogicalProcessors(0),
    m_pModelProcessors(nullptr),
    // Processor Values
    m_stateAutoUpdateProcessorValues(EStateIdle),
    m_pTimerAutoUpdateProcessorValues(nullptr),
    m_pModelProcessorsValues(nullptr),
    // Memory
    m_stateAutoUpdateMemory(EStateIdle),
    m_pTimerAutoUpdateMemory(nullptr),
    m_pModelMemory(nullptr),
    // Memory values
    m_stateAutoUpdateMemoryValues(EStateIdle),
    m_pTimerAutoUpdateMemoryValues(nullptr),
    m_pModelMemoryValues(nullptr),
    // Logical Disks
    m_stateAutoUpdateLogicalDisks(EStateIdle),
    m_pTimerAutoUpdateLogicalDisks(nullptr),
    m_pModelLogicalDisks(nullptr),
    // Logical disks values
    m_stateAutoUpdateLogicalDisksValues(EStateIdle),
    m_pTimerAutoUpdateLogicalDisksValues(nullptr),
    m_pModelLogicalDisksValues(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
{
    if( m_strObjName.isEmpty() )
    {
        m_strObjName = "PCResourceMonitor";
    }

    setObjectName(m_strObjName);

    m_pTrcAdminObj = ZS::Trace::CTrcServer::GetTraceAdminObj("ZS::Apps::Products::PCResourceMonitor", "CPCResourceMonitor", m_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pErrLog = ZS::System::CErrLog::GetInstance();

    // Computer System
    //=================

    m_pTimerAutoUpdateComputerSystem = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerAutoUpdateComputerSystem,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAutoUpdateComputerSystemTimeout()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Operating System
    //=================

    m_pTimerAutoUpdateOperatingSystem = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerAutoUpdateOperatingSystem,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAutoUpdateOperatingSystemTimeout()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Processor
    //==========

    //m_pModelProcessors = new CModelProcessors();

    //m_pTimerAutoUpdateProcessor = new QTimer(this);

    //if( !connect(
    //    /* pObjSender   */ m_pTimerAutoUpdateProcessor,
    //    /* szSignal     */ SIGNAL(timeout()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onAutoUpdateProcessorTimeout()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // Processor Values
    //=================

    // !! The processors values model class is designed to be created after !!
    // !! the number of logical processors have been detected. The ctor     !!
    // !! queries the number of processors from the processors model.       !!
    //m_pModelProcessorsValues = new CModelProcessorsValues();

    //m_pTimerAutoUpdateProcessorValues = new QTimer(this);

    //if( !connect(
    //    /* pObjSender   */ m_pTimerAutoUpdateProcessorValues,
    //    /* szSignal     */ SIGNAL(timeout()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onAutoUpdateProcessorValuesTimeout()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // Memory
    //=======

    m_pModelMemory = new CModelMemory();

    m_pTimerAutoUpdateMemory = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerAutoUpdateMemory,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAutoUpdateMemoryTimeout()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Memory Values
    //==============

    m_pModelMemoryValues = new CModelMemoryValues(m_pModelMemory,200,300);

    m_pTimerAutoUpdateMemoryValues = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerAutoUpdateMemoryValues,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAutoUpdateMemoryValuesTimeout()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Logical Disks
    //==============

    //m_pModelLogicalDisks = new CModelLogicalDisks();

    //m_pTimerAutoUpdateLogicalDisks = new QTimer(this);

    //if( !connect(
    //    /* pObjSender   */ m_pTimerAutoUpdateLogicalDisks,
    //    /* szSignal     */ SIGNAL(timeout()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onAutoUpdateLogicalDisksTimeout()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // Logical Disks Values
    //=====================

    // !! The logical disks values model class is designed to be created after !!
    // !! the number of logical disks have been detected. The ctor             !!
    // !! queries the number of logical disks from the logical disks model.    !!
    //m_pModelLogicalDisksValues = new CModelLogicalDisksValues(m_pModelLogicalDisks);

    //m_pTimerAutoUpdateLogicalDisksValues = new QTimer(this);

    //if( !connect(
    //    /* pObjSender   */ m_pTimerAutoUpdateLogicalDisksValues,
    //    /* szSignal     */ SIGNAL(timeout()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onAutoUpdateLogicalDisksValuesTimeout()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    //if( m_pSettingsFile != nullptr )
    //{
    //    readAutoUpdateIntervals();
    //    saveAutoUpdateIntervals(); // to ensure that the settings file exists with at least default entries
    //}

} // ctor

//------------------------------------------------------------------------------
CPCResourceMonitor::~CPCResourceMonitor()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    releaseWbem();

    //try
    //{
    //    delete m_pModelProcessorsValues;
    //}
    //catch(...)
    //{
    //}
    //m_pModelProcessorsValues = nullptr;

    //try
    //{
    //    delete m_pModelProcessors;
    //}
    //catch(...)
    //{
    //}
    //m_pModelProcessors = nullptr;

    try
    {
        delete m_pModelMemoryValues;
    }
    catch(...)
    {
    }
    m_pModelMemoryValues = nullptr;

    try
    {
        delete m_pModelMemory;
    }
    catch(...)
    {
    }
    m_pModelMemory = nullptr;

    //try
    //{
    //    delete m_pModelLogicalDisksValues;
    //}
    //catch(...)
    //{
    //}
    //m_pModelLogicalDisksValues = nullptr;

    //try
    //{
    //    delete m_pModelLogicalDisks;
    //}
    //catch(...)
    //{
    //}
    //m_pModelLogicalDisks = nullptr;

    m_pTimerAutoUpdateOperatingSystem = nullptr;
    m_pTimerAutoUpdateComputerSystem = nullptr;
    m_pTimerAutoUpdateProcessor = nullptr;
    m_pTimerAutoUpdateProcessorValues = nullptr;
    m_pTimerAutoUpdateMemory = nullptr;
    m_pTimerAutoUpdateMemoryValues = nullptr;
    m_pTimerAutoUpdateLogicalDisks = nullptr;
    m_pTimerAutoUpdateLogicalDisksValues = nullptr;
    m_pErrLog = nullptr;

    ZS::Trace::CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
protected:
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::initWbem()
//------------------------------------------------------------------------------
{
    // Initialize the web base enterprise management
    //----------------------------------------------

    if( !m_bWbemInitialized )
    {
        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "initWbem",
            /* strAddInfo   */ "" );

        m_bWbemInitialized = true;

        HRESULT                  hResCoCreateInstanceIWbemLocator = MAKE_HRESULT(SEVERITY_ERROR,0,S_FALSE);
        HRESULT                  hResIWbemLocatorConnectServer = MAKE_HRESULT(SEVERITY_ERROR,0,S_FALSE);
        HRESULT                  hResCoCreateInstanceIWbemRefresher = MAKE_HRESULT(SEVERITY_ERROR,0,S_FALSE);
        HRESULT                  hResIWbemRefresherQueryInterface = MAKE_HRESULT(SEVERITY_ERROR,0,S_FALSE);
        HRESULT                  hResIWbemConfigureRefresherAddEnum = MAKE_HRESULT(SEVERITY_ERROR,0,S_FALSE);
        IWbemLocator*            pIWbemLocator = nullptr;
        IWbemConfigureRefresher* pIWbemConfigureRefresher = nullptr;
        LONG                     lIDTmp;

        hResCoCreateInstanceIWbemLocator = CoCreateInstance(
            /* rclsid       */ CLSID_WbemAdministrativeLocator,
            /* pUnkOuter    */ nullptr,
            /* dwClsContext */ CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER,
            /* riid         */ IID_IUnknown,
            /* ppv          */ (void**)&pIWbemLocator );

        if( FAILED(hResCoCreateInstanceIWbemLocator) )
        {
            m_bWbemInitialized = false;

            if( m_pErrLog != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "CoCreateInstance(CWbemAdministrativeLocator) failed with error code ";
                strAddErrInfo += QString::number(static_cast<unsigned int>(hResCoCreateInstanceIWbemLocator),16);
                strAddErrInfo += " (" + hResult2Str(hResCoCreateInstanceIWbemLocator) + ")";
                SErrResultInfo errResultInfo = ErrResultInfoError("initWbem", EResultClassNotInitialised, strAddErrInfo);
                m_pErrLog->addEntry(errResultInfo);
            }
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("CoCreateInstance(CWbemAdministrativeLocator) failed with error code " + QString::number(static_cast<unsigned int>(hResCoCreateInstanceIWbemLocator),16) + " (" + hResult2Str(hResCoCreateInstanceIWbemLocator) + ")");
            }
        }

        if( SUCCEEDED(hResCoCreateInstanceIWbemLocator) && pIWbemLocator != nullptr )
        {
            hResIWbemLocatorConnectServer = pIWbemLocator->ConnectServer(
                /* strNetworkResource */ _bstr_t(L"ROOT\\CIMV2"),
                /* strUser            */ nullptr,
                /* strPassword        */ nullptr,
                /* strLocale          */ nullptr,
                /* lSecurityFlags     */ 0,
                /* strAuthority       */ nullptr,
                /* pCtx               */ nullptr,
                /* ppNamespace        */ &m_pIWbemServices );

            if( FAILED(hResIWbemLocatorConnectServer) )
            {
                m_bWbemInitialized = false;

                if( m_pErrLog != nullptr )
                {
                    QString strAddErrInfo;
                    strAddErrInfo  = "IWbemLocator->ConnectServer() failed with error code ";
                    strAddErrInfo += QString::number(static_cast<unsigned int>(hResIWbemLocatorConnectServer),16);
                    strAddErrInfo += " (" + hResult2Str(hResIWbemLocatorConnectServer) + ")";
                    SErrResultInfo errResultInfo = ErrResultInfoError("initWbem", EResultClassNotInitialised, strAddErrInfo);
                    m_pErrLog->addEntry(errResultInfo);
                }
                if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
                {
                    mthTracer.trace("IWbemLocator->ConnectServer() failed with error code " + QString::number(static_cast<unsigned int>(hResIWbemLocatorConnectServer),16) + " (" + hResult2Str(hResIWbemLocatorConnectServer) + ")");
                }
            }
        }

        if( SUCCEEDED(hResIWbemLocatorConnectServer) )
        {
            hResCoCreateInstanceIWbemRefresher = CoCreateInstance(
              /* rclsid       */ CLSID_WbemRefresher,
              /* pUnkOuter    */ nullptr,
              /* dwClsContext */ CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER,
              /* riid         */ IID_IWbemRefresher,
              /* ppv          */ (void**)&m_pIWbemRefresher );

            if( FAILED(hResCoCreateInstanceIWbemRefresher) )
            {
                m_bWbemInitialized = false;

                if( m_pErrLog != nullptr )
                {
                    QString strAddErrInfo;
                    strAddErrInfo  = "CoCreateInstance(CWbemRefresher) failed with error code ";
                    strAddErrInfo += QString::number(static_cast<unsigned int>(hResCoCreateInstanceIWbemRefresher),16);
                    strAddErrInfo += " (" + hResult2Str(hResCoCreateInstanceIWbemRefresher) + ")";
                    SErrResultInfo errResultInfo = ErrResultInfoError("initWbem", EResultClassNotInitialised, strAddErrInfo);
                    m_pErrLog->addEntry(errResultInfo);
                }
                if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
                {
                    mthTracer.trace("CoCreateInstance(CWbemRefresher) failed with error code " + QString::number(static_cast<unsigned int>(hResCoCreateInstanceIWbemRefresher),16));
                }
            }
        }

        if( SUCCEEDED(hResCoCreateInstanceIWbemRefresher) && m_pIWbemRefresher != nullptr )
        {
            hResIWbemRefresherQueryInterface = m_pIWbemRefresher->QueryInterface(
                /* iid */ IID_IWbemConfigureRefresher,
                /* ppv */ (void**)&pIWbemConfigureRefresher );

            if( FAILED(hResIWbemRefresherQueryInterface) )
            {
                m_bWbemInitialized = false;

                if( m_pErrLog != nullptr )
                {
                    QString strAddErrInfo;
                    strAddErrInfo  = "WbemRefresher->QueryInterface(IWbemConfigureRefresher) failed with error code ";
                    strAddErrInfo += QString::number(static_cast<unsigned int>(hResIWbemRefresherQueryInterface),16);
                    strAddErrInfo += " (" + hResult2Str(hResIWbemRefresherQueryInterface) + ")";
                    SErrResultInfo errResultInfo = ErrResultInfoError("initWbem", EResultClassNotInitialised, strAddErrInfo);
                    m_pErrLog->addEntry(errResultInfo);
                }
                if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
                {
                    mthTracer.trace("WbemRefresher->QueryInterface(IWbemConfigureRefresher) failed with error code " + QString::number(static_cast<unsigned int>(hResIWbemRefresherQueryInterface),16) + " (" + hResult2Str(hResIWbemRefresherQueryInterface) + ")");
                }
            }
        }

        // Add "high performance" enumerators to the refresher.
        if( SUCCEEDED(hResIWbemRefresherQueryInterface) && pIWbemConfigureRefresher != nullptr )
        {
            hResIWbemConfigureRefresherAddEnum = pIWbemConfigureRefresher->AddEnum(
                /* pNameSpace   */ m_pIWbemServices,
                /* wszClassName */ L"Win32_PerfFormattedData_PerfOS_Processor",
                /* lFlags       */ 0,
                /* pContext     */ nullptr,
                /* ppEnum       */ &m_pIWbemHiPerfEnumOSProc,
                /* plId         */ &lIDTmp );

            if( FAILED(hResIWbemConfigureRefresherAddEnum) )
            {
                m_bWbemInitialized = false;

                if( m_pErrLog != nullptr )
                {
                    QString strAddErrInfo;
                    strAddErrInfo  = "IWbemConfigureRefresher->AddEnum(PerfFormattedData_PerfProc_Process) failed with error code ";
                    strAddErrInfo += QString::number(static_cast<unsigned int>(hResIWbemRefresherQueryInterface),16);
                    strAddErrInfo += " (" + hResult2Str(hResIWbemRefresherQueryInterface) + ")";
                    SErrResultInfo errResultInfo = ErrResultInfoError("initWbem", EResultClassNotInitialised, strAddErrInfo);
                    m_pErrLog->addEntry(errResultInfo);
                }
                if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
                {
                    mthTracer.trace("IWbemConfigureRefresher->AddEnum(PerfFormattedData_PerfProc_Process) failed with error code " + QString::number(static_cast<unsigned int>(hResIWbemConfigureRefresherAddEnum),16) + " (" + hResult2Str(hResIWbemConfigureRefresherAddEnum) + ")");
                }
            }
        }

        if( pIWbemLocator != nullptr )
        {
            pIWbemLocator->Release();
            pIWbemLocator = nullptr;
        }
        if( pIWbemConfigureRefresher != nullptr )
        {
            pIWbemConfigureRefresher->Release();
            pIWbemConfigureRefresher = nullptr;
        }
    }

} // initWbem

//------------------------------------------------------------------------------
void CPCResourceMonitor::releaseWbem()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "releaseWbem",
        /* strAddInfo   */ "" );

    ULONG udxObj;

    if( m_arpIWbemObjAccessOSProc != nullptr )
    {
        for( udxObj = 0; udxObj < m_uObjAccessOSProcCount; udxObj++ )
        {
            if( m_arpIWbemObjAccessOSProc[udxObj] != nullptr )
            {
                m_arpIWbemObjAccessOSProc[udxObj]->Release();
            }
            m_arpIWbemObjAccessOSProc[udxObj] = nullptr;
        }
        try
        {
            delete [] m_arpIWbemObjAccessOSProc;
        }
        catch(...)
        {
        }
        m_arpIWbemObjAccessOSProc = nullptr;
    }

    if( m_pIWbemHiPerfEnumOSProc != nullptr )
    {
        m_pIWbemHiPerfEnumOSProc->Release();
        m_pIWbemHiPerfEnumOSProc = nullptr;
    }

    if( m_pIWbemServices != nullptr )
    {
        m_pIWbemServices->Release();
        m_pIWbemServices = nullptr;
    }

    if( m_pIWbemRefresher != nullptr )
    {
        m_pIWbemRefresher->Release();
        m_pIWbemRefresher = nullptr;
    }

} // releaseWbem

/*==============================================================================
public: // instance methods retrieving values from the host
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::saveQueryValues( const QString& i_strWinClass )
//------------------------------------------------------------------------------
{
    M_WBEM_INIT();

    if( m_pIWbemServices != nullptr )
    {
        HRESULT               hRes;
        ULONG                 uReturn;
        VARIANT               vtProp;
        IEnumWbemClassObject* pEnumObject;
        IWbemClassObject*     pclsObj;
        QString               strVal;
        unsigned int          udxObj = 0;
        QString               strQuery( "SELECT * FROM " + i_strWinClass );

        BSTR bstrQuery = SysAllocStringLen( (OLECHAR*)strQuery.data(), strQuery.length() );

        hRes = m_pIWbemServices->ExecQuery(
            /* strQueryLanguage */ bstr_t("WQL"),
            /* strQuery         */ bstrQuery,
            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            /* pCtx             */ nullptr,
            /* ppEnum           */ &pEnumObject );

        SysFreeString(bstrQuery);

        if( FAILED(hRes) )
        {
            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
            if( hRes == WBEM_E_SHUTTING_DOWN )
            {
                resultSeverity = EResultSeverityInfo;
            }
            if( m_pErrLog != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IIWbemServices->ExecQuery(" + i_strWinClass + ") failed with error code ";
                strAddErrInfo += QString::number(static_cast<unsigned int>(hRes),16);
                strAddErrInfo += " (" + wbemErrCode2Str(hRes) + ")";
                SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "setQueryValues", EResultRequestRefused, resultSeverity, strAddErrInfo);
                m_pErrLog->addEntry(errResultInfo);
            }
        }

        while( pEnumObject != nullptr )
        {
            hRes = pEnumObject->Next(
                /* lTimeout   */ WBEM_INFINITE,
                /* uCount     */ 1,
                /* apObjects  */ &pclsObj,
                /* puReturned */ &uReturn );

            if( uReturn == 0 )
            {
                break;
            }

            QFile file( i_strWinClass + "_" + QString::number(udxObj) + ".txt" );

            file.open(QIODevice::WriteOnly|QIODevice::Text);

            QTextStream textStream(&file);

            SAFEARRAY* pstrNames;

            hRes = pclsObj->GetNames(
                /* wszQualifierName */ nullptr,
                /* lFlags           */ 0,
                /* pQualifierValue  */ nullptr,
                /* pstrNames        */ &pstrNames );

            BSTR    bstrName;
            QString strName;
            LONG    cdims = pstrNames->cDims;
            LONG    idxDim;
            LONG    idxVal;
            LONG    lbound;
            LONG    ubound;

            for( idxDim = 0; idxDim < cdims; idxDim++ )
            {
                SafeArrayGetLBound(pstrNames, idxDim + 1, &lbound);
                SafeArrayGetUBound(pstrNames, idxDim + 1, &ubound);

                for( idxVal = lbound; idxVal < (lbound + ubound + 1); idxVal++ )
                {
                    hRes = SafeArrayGetElement(
                        /* psa       */ pstrNames,
                        /* rgIndices */ &idxVal,
                        /* pv        */ &bstrName );

                    strName = QString( (QChar*)bstrName, static_cast<int>(static_cast<int>(wcslen(bstrName))) );
                    strVal = "";

                    hRes = pclsObj->Get(
                        /* wszName  */ bstrName,
                        /* lFlags   */ 0,
                        /* pVal     */ &vtProp,
                        /* pType    */ 0,
                        /* plFlavor */ 0 );

                    if( SUCCEEDED(hRes) )
                    {
                        strVal = ZS::System::oleVariant2Str(&vtProp);
                    }

                    textStream << strName << ":\t" << strVal << "\n";

                    VariantClear(&vtProp);
                }
            }

            pclsObj->Release();

            udxObj++;

        } // while( pEnumObject != nullptr )

        pEnumObject->Release();
    }

} // saveQueryValues

/* Computer System
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::updateComputerSystem()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateComputerSystem",
        /* strAddInfo   */ "" );

    M_WBEM_INIT();

    if( m_pIWbemServices != nullptr )
    {
        HRESULT               hRes;
        ULONG                 uReturn;
        VARIANT               vtProp;
        QString               strVal;
        IEnumWbemClassObject* pIEnumWbemClassObject = nullptr;
        IWbemClassObject*     pIWbemClassObject;

        // FROM System Enclosure
        //----------------------

        // If not explicitly set by ini file ..
        hRes = m_pIWbemServices->ExecQuery(
            /* strQueryLanguage */ bstr_t("WQL"),
            /* strQuery         */ bstr_t("SELECT SerialNumber FROM Win32_SystemEnclosure"),
            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            /* pCtx             */ nullptr,
            /* ppEnum           */ &pIEnumWbemClassObject );

        if( FAILED(hRes) )
        {
            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
            if( hRes == WBEM_E_SHUTTING_DOWN )
            {
                resultSeverity = EResultSeverityInfo;
            }
            if( m_pErrLog != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IWbemServices->ExecQuery(SELECT SerialNumber FROM Win32_SystemEnclosure) failed with error code ";
                strAddErrInfo += QString::number(static_cast<unsigned int>(hRes),16);
                strAddErrInfo += " (" + wbemErrCode2Str(hRes) + ")";
                SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateComputerSystem", EResultRequestRefused, resultSeverity, strAddErrInfo);
                m_pErrLog->addEntry(errResultInfo);
            }
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("IWbemServices->ExecQuery(SELECT SerialNumber FROM Win32_SystemEnclosure) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
            }
        }
        else if( pIEnumWbemClassObject != nullptr )
        {
            hRes = pIEnumWbemClassObject->Next(
                /* lTimeout   */ WBEM_INFINITE,
                /* uCount     */ 1,
                /* apObjects  */ &pIWbemClassObject,
                /* puReturned */ &uReturn );

            if( uReturn > 0 && pIWbemClassObject != nullptr )
            {
                // Serial Number
                //--------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"SerialNumber",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );

                if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    m_strComputerSerialNr = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    m_physValComputerSerialNrUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                }

                // The serial number reported by VMWare installations is "None" which is definitely
                // not a unique key. So we are going to create one (VMWare is only used within a
                // test environment and therefore it shouldn't be a problem to use a "manually"
                // created key for the component "VMWare PC").
                if( m_strComputerSerialNr.isEmpty() || m_strComputerSerialNr.compare("None",Qt::CaseInsensitive) == 0 )
                {
                    if( m_pErrLog != nullptr )
                    {
                        QString strAddErrInfo;
                        strAddErrInfo  = "Could not retrieve a valid SerialNumber from PC. Using automatically generated GUID instead.";
                        SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateComputerSystem", EResultRequestRefused, EResultSeverityWarning, strAddErrInfo);
                        m_pErrLog->addEntry(errResultInfo);
                    }
                    m_physValComputerSerialNrUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                    m_strComputerSerialNr = QHostInfo::localHostName();
                }
                else if( m_strComputerSerialNr.contains("To Be Filled By O.E.M.",Qt::CaseInsensitive) )
                {
                    if( m_pErrLog != nullptr )
                    {
                        QString strAddErrInfo;
                        strAddErrInfo  = "Could not retrieve a valid SerialNumber from PC (got " + m_strComputerSerialNr + "). Using automatically generated GUID instead.";
                        SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateComputerSystem", EResultOptionNotSupported, EResultSeverityWarning, strAddErrInfo);
                        m_pErrLog->addEntry(errResultInfo);
                    }
                    m_physValComputerSerialNrUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                    m_strComputerSerialNr = QHostInfo::localHostName();
                }

                VariantClear(&vtProp);

                // Release class object
                //---------------------

                pIWbemClassObject->Release();
                pIWbemClassObject = nullptr;
            }

            pIEnumWbemClassObject->Release();
            pIEnumWbemClassObject = nullptr;

        } // if( pIEnumWbemClassObject != nullptr )

        // FROM Computer System
        //-----------------------

        hRes = m_pIWbemServices->ExecQuery(
            /* strQueryLanguage */ bstr_t("WQL"),
            /* strQuery         */ bstr_t("SELECT * FROM Win32_ComputerSystem"),
            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            /* pCtx             */ nullptr,
            /* ppEnum           */ &pIEnumWbemClassObject );

        if( FAILED(hRes) )
        {
            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
            if( hRes == WBEM_E_SHUTTING_DOWN )
            {
                resultSeverity = EResultSeverityInfo;
            }
            if( m_pErrLog != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IWbemServices->ExecQuery(SELECT * FROM Win32_ComputerSystem) failed with error code ";
                strAddErrInfo += QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")";
                SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateComputerSystem", EResultRequestRefused, resultSeverity, strAddErrInfo);
                m_pErrLog->addEntry(errResultInfo);
            }
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("IWbemServices->ExecQuery(SELECT * FROM Win32_ComputerSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
            }
        }
        else if( pIEnumWbemClassObject != nullptr )
        {
            hRes = pIEnumWbemClassObject->Next(
                /* lTimeout   */ WBEM_INFINITE,
                /* uCount     */ 1,
                /* apObjects  */ &pIWbemClassObject,
                /* puReturned */ &uReturn );

            if( uReturn > 0 && pIWbemClassObject != nullptr )
            {
                // Manufacturer
                //-------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"Manufacturer",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    m_strComputerSystemManufacturer = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    m_physValComputerSystemManufacturerUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                }
                VariantClear(&vtProp);

                // Model
                //------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"Model",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    m_strComputerSystemModel = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    m_physValComputerSystemModelUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                }
                VariantClear(&vtProp);

                // Host Name
                //----------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"DNSHostName",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    m_strComputerSystemDNSHostName = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    m_physValComputerSystemDNSHostNameUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                }
                VariantClear(&vtProp);

                if( m_strComputerSystemDNSHostName.isEmpty() )
                {
                    hRes = pIWbemClassObject->Get(
                        /* wszName  */ L"Name",
                        /* lFlags   */ 0,
                        /* pVal     */ &vtProp,
                        /* pType    */ 0,
                        /* plFlavor */ 0 );
                    if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                    {
                        m_strComputerSystemDNSHostName = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                        m_physValComputerSystemDNSHostNameUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                    }
                    VariantClear(&vtProp);
                }

                // Domain
                //-------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"Domain",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    m_strComputerSystemDomain = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    m_physValComputerSystemDomainUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                }
                VariantClear(&vtProp);

                // Release class object
                //---------------------

                pIWbemClassObject->Release();
                pIWbemClassObject = nullptr;

            } // if( uReturn > 0 && pIWbemClassObject != nullptr )

            pIEnumWbemClassObject->Release();
            pIEnumWbemClassObject = nullptr;

        } // if( pIEnumWbemClassObject != nullptr )

        emit computerSystemChanged();

    } // if( m_pIWbemServices != nullptr )

} // updateComputerSystem

//------------------------------------------------------------------------------
void CPCResourceMonitor::startAutoUpdateComputerSystem()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startAutoUpdateComputerSystem",
        /* strAddInfo   */ "" );

    if( m_stateAutoUpdateComputerSystem == EStateIdle )
    {
        if( m_autoUpdateIntervals.m_iComputerSystem_ms > 0 && !m_pTimerAutoUpdateComputerSystem->isActive() )
        {
            m_pTimerAutoUpdateComputerSystem->start(m_autoUpdateIntervals.m_iComputerSystem_ms);
        }
        m_stateAutoUpdateComputerSystem = EStateRunning;
        emit autoUpdateComputerSystemStateChanged();
    }

} // startAutoUpdateComputerSystem

//------------------------------------------------------------------------------
void CPCResourceMonitor::stopAutoUpdateComputerSystem()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopAutoUpdateComputerSystem",
        /* strAddInfo   */ "" );

    if( m_pTimerAutoUpdateComputerSystem->isActive() )
    {
        m_pTimerAutoUpdateComputerSystem->stop();
    }
    if( m_stateAutoUpdateComputerSystem == EStateRunning )
    {
        m_stateAutoUpdateComputerSystem = EStateIdle;
        emit autoUpdateComputerSystemStateChanged();
    }

} // stopAutoUpdateComputerSystem

//------------------------------------------------------------------------------
void CPCResourceMonitor::setAutoUpdateIntervalComputerSystemInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    if( m_autoUpdateIntervals.m_iComputerSystem_ms != i_iInterval_ms )
    {
        if( m_pTimerAutoUpdateComputerSystem->isActive() )
        {
            m_pTimerAutoUpdateComputerSystem->stop();
        }

        m_autoUpdateIntervals.m_iComputerSystem_ms = i_iInterval_ms;
        saveAutoUpdateIntervals();
        emit autoUpdateIntervalComputerSystemChanged();

        if( m_stateAutoUpdateComputerSystem == EStateRunning && m_autoUpdateIntervals.m_iComputerSystem_ms > 0 )
        {
            m_pTimerAutoUpdateComputerSystem->start(m_autoUpdateIntervals.m_iComputerSystem_ms);
        }
    }

} // setAutoUpdateIntervalComputerSystemInMs

//------------------------------------------------------------------------------
int CPCResourceMonitor::getAutoUpdateIntervalComputerSystemInMs() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals.m_iComputerSystem_ms;
}

/* Operating System
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::updateOperatingSystem()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateOperatingSystem",
        /* strAddInfo   */ "" );

    M_WBEM_INIT();

    if( m_pIWbemServices != nullptr )
    {
        HRESULT               hRes;
        ULONG                 uReturn;
        VARIANT               vtProp;
        QString               strVal;
        IEnumWbemClassObject* pIEnumWbemClassObject = nullptr;
        IWbemClassObject*     pIWbemClassObject;

        // FROM Operating System
        //-----------------------

        hRes = m_pIWbemServices->ExecQuery(
            /* strQueryLanguage */ bstr_t("WQL"),
            /* strQuery         */ bstr_t("SELECT Caption, CSDVersion FROM Win32_OperatingSystem"),
            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            /* pCtx             */ nullptr,
            /* ppEnum           */ &pIEnumWbemClassObject );

        if( FAILED(hRes) )
        {
            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
            if( hRes == WBEM_E_SHUTTING_DOWN )
            {
                resultSeverity = EResultSeverityInfo;
            }
            if( m_pErrLog != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IWbemServices->ExecQuery(SELECT Caption, CSDVersion FROM Win32_OperatingSystem) failed with error code ";
                strAddErrInfo += QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")";
                SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateOperatingSystem", EResultRequestRefused, resultSeverity, strAddErrInfo);
                m_pErrLog->addEntry(errResultInfo);
                if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
                {
                    mthTracer.trace("IWbemServices->ExecQuery(SELECT Caption, CSDVersion FROM Win32_OperatingSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
                }
            }
        }
        else if( pIEnumWbemClassObject != nullptr )
        {
            hRes = pIEnumWbemClassObject->Next(
                /* lTimeout   */ WBEM_INFINITE,
                /* uCount     */ 1,
                /* apObjects  */ &pIWbemClassObject,
                /* puReturned */ &uReturn );

            if( uReturn > 0 && pIWbemClassObject != nullptr )
            {
                // Operating system
                //-----------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"Caption",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    m_strOSCaption = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    m_physValOSCaptionUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                }
                VariantClear(&vtProp);

                // Service pack
                //-------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"CSDVersion",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    m_strOSServicePack = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    m_physValOSServicePackUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();
                }
                VariantClear(&vtProp);

                pIWbemClassObject->Release();
                pIWbemClassObject = nullptr;
            }

            pIEnumWbemClassObject->Release();
            pIEnumWbemClassObject = nullptr;

        } // if( pIEnumWbemClassObject != nullptr )

        emit operatingSystemChanged();

    } // if( m_pIWbemServices != nullptr )

} // updateOperatingSystem

//------------------------------------------------------------------------------
void CPCResourceMonitor::startAutoUpdateOperatingSystem()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startAutoUpdateOperatingSystem",
        /* strAddInfo   */ "" );

    if( m_stateAutoUpdateOperatingSystem == EStateIdle )
    {
        if( m_autoUpdateIntervals.m_iOperatingSystem_ms > 0 && !m_pTimerAutoUpdateOperatingSystem->isActive() )
        {
            m_pTimerAutoUpdateOperatingSystem->start(m_autoUpdateIntervals.m_iOperatingSystem_ms);
        }
        m_stateAutoUpdateOperatingSystem = EStateRunning;
        emit autoUpdateOperatingSystemStateChanged();
    }

} // startAutoUpdateOperatingSystem

//------------------------------------------------------------------------------
void CPCResourceMonitor::stopAutoUpdateOperatingSystem()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopAutoUpdateOperatingSystem",
        /* strAddInfo   */ "" );

    if( m_pTimerAutoUpdateOperatingSystem->isActive() )
    {
        m_pTimerAutoUpdateOperatingSystem->stop();
    }
    if( m_stateAutoUpdateOperatingSystem == EStateRunning )
    {
        m_stateAutoUpdateOperatingSystem = EStateIdle;
        emit autoUpdateOperatingSystemStateChanged();
    }

} // stopAutoUpdateOperatingSystem

//------------------------------------------------------------------------------
CPCResourceMonitor::EState CPCResourceMonitor::getAutoUpdateOperatingSystemState() const
//------------------------------------------------------------------------------
{
    return m_stateAutoUpdateOperatingSystem;
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::setAutoUpdateIntervalOperatingSystemInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    if( m_autoUpdateIntervals.m_iOperatingSystem_ms != i_iInterval_ms )
    {
        if( m_pTimerAutoUpdateOperatingSystem->isActive() )
        {
            m_pTimerAutoUpdateOperatingSystem->stop();
        }

        m_autoUpdateIntervals.m_iOperatingSystem_ms = i_iInterval_ms;
        saveAutoUpdateIntervals();
        emit autoUpdateIntervalOperatingSystemChanged();

        if( m_stateAutoUpdateOperatingSystem == EStateRunning && m_autoUpdateIntervals.m_iOperatingSystem_ms > 0 )
        {
            m_pTimerAutoUpdateOperatingSystem->start(m_autoUpdateIntervals.m_iOperatingSystem_ms);
        }
    }

} // setAutoUpdateIntervalOperatingSystemInMs

//------------------------------------------------------------------------------
int CPCResourceMonitor::getAutoUpdateIntervalOperatingSystemInMs() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals.m_iOperatingSystem_ms;
}

//------------------------------------------------------------------------------
CPCResourceMonitor::EState CPCResourceMonitor::getAutoUpdateComputerSystemState() const
//------------------------------------------------------------------------------
{
    return m_stateAutoUpdateComputerSystem;
}

/* Processor
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::updateProcessor()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateProcessor",
        /* strAddInfo   */ "" );

    //M_WBEM_INIT();

    //if( m_pIWbemServices != nullptr )
    //{
    //    HRESULT               hRes;
    //    ULONG                 uReturn;
    //    VARIANT               vtProp;
    //    ULONG                 udxObj;
    //    QString               strVal;
    //    IEnumWbemClassObject* pIEnumWbemClassObject = nullptr;
    //    IWbemClassObject*     pIWbemClassObject;

    //    // FROM Computer System
    //    //----------------------

    //    hRes = m_pIWbemServices->ExecQuery(
    //        /* strQueryLanguage */ bstr_t("WQL"),
    //        /* strQuery         */ bstr_t("SELECT NumberOfLogicalProcessors FROM Win32_ComputerSystem"),
    //        /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
    //        /* pCtx             */ nullptr,
    //        /* ppEnum           */ &pIEnumWbemClassObject );

    //    if( FAILED(hRes) )
    //    {
    //        ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
    //        if( hRes == WBEM_E_SHUTTING_DOWN )
    //        {
    //            resultSeverity = EResultSeverityInfo;
    //        }
    //        if( m_pErrLog != nullptr )
    //        {
    //            m_pErrLog->addEntry( SErrResultInfo(
    //                /* severity    */ resultSeverity,
    //                /* iErrNr      */ EResultRequestRefused,
    //                /* strAddInfo  */ "IWbemServices->ExecQuery(SELECT NumberOfLogicalProcessors FROM Win32_ComputerSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")",
    //                /* strSource   */ m_strObjName ) );
    //        }
    //        if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //        {
    //            mthTracer.trace("IWbemServices->ExecQuery(SELECT NumberOfLogicalProcessors FROM Win32_ComputerSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
    //        }
    //    }
    //    else if( pIEnumWbemClassObject != nullptr )
    //    {
    //        hRes = pIEnumWbemClassObject->Next(
    //            /* lTimeout   */ WBEM_INFINITE,
    //            /* uCount     */ 1,
    //            /* apObjects  */ &pIWbemClassObject,
    //            /* puReturned */ &uReturn );

    //        if( uReturn > 0 && pIWbemClassObject != nullptr )
    //        {
    //            // Number of logical processors
    //            //-----------------------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"NumberOfLogicalProcessors",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
    //            {
    //                m_uComputerSystemNumberOfLogicalProcessors = static_cast<unsigned int>(vtProp.intVal);
    //                m_physValComputerSystemNumberOfLogicalProcessorsUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();

    //                if( m_pModelProcessors->getLogicalProcessorsCount() == 0 )
    //                {
    //                    m_pModelProcessors->setLogicalProcessorsCount(m_uComputerSystemNumberOfLogicalProcessors);
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            pIWbemClassObject->Release();
    //            pIWbemClassObject = nullptr;
    //        }

    //        pIEnumWbemClassObject->Release();
    //        pIEnumWbemClassObject = nullptr;

    //    } // if( pIEnumWbemClassObject != nullptr )

    //    // FROM Computer System
    //    //----------------------

    //    hRes = m_pIWbemServices->ExecQuery(
    //        /* strQueryLanguage */ bstr_t("WQL"),
    //        /* strQuery         */ bstr_t("SELECT NumberOfProcessors FROM Win32_ComputerSystem"),
    //        /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
    //        /* pCtx             */ nullptr,
    //        /* ppEnum           */ &pIEnumWbemClassObject );

    //    if( FAILED(hRes) )
    //    {
    //        ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
    //        if( hRes == WBEM_E_SHUTTING_DOWN )
    //        {
    //            resultSeverity = EResultSeverityInfo;
    //        }
    //        if( m_pErrLog != nullptr )
    //        {
    //            m_pErrLog->addEntry( SErrResultInfo(
    //                /* severity    */ resultSeverity,
    //                /* iErrNr      */ EResultRequestRefused,
    //                /* strAddInfo  */ "IWbemServices->ExecQuery(SELECT NumberOfProcessors FROM Win32_ComputerSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")",
    //                /* strSource   */ m_strObjName ) );
    //        }
    //        if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //        {
    //            mthTracer.trace("IWbemServices->ExecQuery(SELECT NumberOfProcessors FROM Win32_ComputerSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
    //        }
    //    }
    //    else if( pIEnumWbemClassObject != nullptr )
    //    {
    //        hRes = pIEnumWbemClassObject->Next(
    //            /* lTimeout   */ WBEM_INFINITE,
    //            /* uCount     */ 1,
    //            /* apObjects  */ &pIWbemClassObject,
    //            /* puReturned */ &uReturn );

    //        if( uReturn > 0 && pIWbemClassObject != nullptr )
    //        {
    //            // Number of processors
    //            //---------------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"NumberOfProcessors",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
    //            {
    //                m_uComputerSystemNumberOfProcessors = static_cast<unsigned int>(vtProp.intVal);
    //                m_physValComputerSystemNumberOfProcessorsUpdSysTime = ZS::System::Time::getProcTimeInMilliSec();

    //                if( m_pModelProcessors->getProcessorsCount() == 0 )
    //                {
    //                    m_pModelProcessors->setProcessorsCount(m_uComputerSystemNumberOfProcessors);
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            pIWbemClassObject->Release();
    //            pIWbemClassObject = nullptr;

    //        } // if( uReturn > 0 && pIWbemClassObject != nullptr )

    //        pIEnumWbemClassObject->Release();
    //        pIEnumWbemClassObject = nullptr;

    //    } // if( pIEnumWbemClassObject != nullptr )

    //    // Windows Server 2003, Windows XP, and Windows 2000: The property "NumberOfLogicalProcessors" is not available.
    //    if( m_pModelProcessors->getLogicalProcessorsCount() == 0 && m_uComputerSystemNumberOfProcessors > 0 )
    //    {
    //        m_pModelProcessors->setLogicalProcessorsCount(m_uComputerSystemNumberOfProcessors);
    //    }

    //    // !! The processors values model class is designed to be created after !!
    //    // !! the number of logical processors have been detected. The ctor     !!
    //    // !! queries the number of processors from the processors model.       !!
    //    if( m_pModelProcessors->getLogicalProcessorsCount() > 0 && m_pModelProcessorsValues == nullptr )
    //    {
    //        m_pModelProcessorsValues = new CModelProcessorsValues(m_pModelProcessors);
    //    }

    //    // FROM Processor
    //    //---------------

    //    hRes = m_pIWbemServices->ExecQuery(
    //        /* strQueryLanguage */ bstr_t("WQL"),
    //        /* strQuery         */ bstr_t("SELECT DeviceId, Manufacturer, Name, CurrentClockSpeed, NumberOfCores, NumberOfLogicalProcessors FROM Win32_Processor"),
    //        /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
    //        /* pCtx             */ nullptr,
    //        /* ppEnum           */ &pIEnumWbemClassObject );

    //    if( FAILED(hRes) )
    //    {
    //        ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
    //        if( hRes == WBEM_E_SHUTTING_DOWN )
    //        {
    //            resultSeverity = EResultSeverityInfo;
    //        }
    //        if( m_pErrLog != nullptr )
    //        {
    //            m_pErrLog->addEntry( SErrResultInfo(
    //                /* severity    */ resultSeverity,
    //                /* iErrNr      */ EResultRequestRefused,
    //                /* strErrNr    */ ZS::result2Str(EResultRequestRefused),
    //                /* strAddInfo  */ "IWbemServices->ExecQuery(SELECT DeviceId, Manufacturer, Name, CurrentClockSpeed, NumberOfCores, NumberOfLogicalProcessors FROM Win32_Processor) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")",
    //                /* strSource   */ m_strObjName ) );
    //        }
    //        if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //        {
    //            mthTracer.trace("IWbemServices->ExecQuery(SELECT DeviceId, Manufacturer, Name, CurrentClockSpeed, NumberOfCores, NumberOfLogicalProcessors FROM Win32_Processor) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
    //        }
    //    }
    //    else if( pIEnumWbemClassObject != nullptr )
    //    {
    //        udxObj = 0;

    //        while( true )
    //        {
    //            hRes = pIEnumWbemClassObject->Next(
    //                /* lTimeout   */ WBEM_INFINITE,
    //                /* uCount     */ 1,
    //                /* apObjects  */ &pIWbemClassObject,
    //                /* puReturned */ &uReturn );

    //            if( uReturn == 0 || pIWbemClassObject == nullptr )
    //            {
    //                break;
    //            }

    //            // Device ID
    //            //----------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"DeviceID",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
    //            {
    //                strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
    //                m_pModelProcessors->setDeviceId(static_cast<unsigned int>(udxObj),strVal);
    //            }
    //            VariantClear(&vtProp);

    //            // Manufacturer
    //            //-------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"Manufacturer",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
    //            {
    //                strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
    //                m_pModelProcessors->setManufacturer(static_cast<unsigned int>(udxObj),strVal);
    //            }
    //            VariantClear(&vtProp);

    //            // Name
    //            //-----

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"Name",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
    //            {
    //                strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
    //                m_pModelProcessors->setName(static_cast<unsigned int>(udxObj),strVal);
    //            }
    //            VariantClear(&vtProp);

    //            // Current clock speed
    //            //--------------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"CurrentClockSpeed",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
    //            {
    //                m_pModelProcessors->setCurrentClockSpeedInMHz(static_cast<unsigned int>(udxObj),static_cast<double>(vtProp.intVal));
    //            }
    //            VariantClear(&vtProp);

    //            // Number of cores
    //            //----------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"NumberOfCores",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
    //            {
    //                m_pModelProcessors->setCoresCount(static_cast<unsigned int>(udxObj),static_cast<unsigned int>(vtProp.intVal));
    //            }
    //            VariantClear(&vtProp);

    //            // Number of logical processors
    //            //-----------------------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"NumberOfLogicalProcessors",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
    //            {
    //                m_pModelProcessors->setLogicalProcessorsCount(static_cast<unsigned int>(udxObj),static_cast<unsigned int>(vtProp.intVal));
    //            }
    //            VariantClear(&vtProp);

    //            // Release class object
    //            //---------------------

    //            pIWbemClassObject->Release();
    //            pIWbemClassObject = nullptr;

    //            // Next processor (if any)
    //            //------------------------

    //            udxObj++;

    //        } // while( true )

    //        pIEnumWbemClassObject->Release();
    //        pIEnumWbemClassObject = nullptr;

    //    } // if( pIEnumWbemClassObject != nullptr )

    //    emit processorChanged();

    //} // if( m_pIWbemServices != nullptr )

} // updateProcessor

//------------------------------------------------------------------------------
void CPCResourceMonitor::startAutoUpdateProcessor()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startAutoUpdateProcessor",
        /* strAddInfo   */ "" );

    //if( m_stateAutoUpdateProcessor == EStateIdle )
    //{
    //    if( m_autoUpdateIntervals.m_iProcessor_ms > 0 && !m_pTimerAutoUpdateProcessor->isActive() )
    //    {
    //        m_pTimerAutoUpdateProcessor->start(m_autoUpdateIntervals.m_iProcessor_ms);
    //    }
    //    m_stateAutoUpdateProcessor = EStateRunning;
    //    emit autoUpdateProcessorStateChanged();
    //}

} // startAutoUpdateProcessor

//------------------------------------------------------------------------------
void CPCResourceMonitor::stopAutoUpdateProcessor()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopAutoUpdateProcessor",
        /* strAddInfo   */ "" );

    //if( m_pTimerAutoUpdateProcessor->isActive() )
    //{
    //    m_pTimerAutoUpdateProcessor->stop();
    //}
    //if( m_stateAutoUpdateProcessor == EStateRunning )
    //{
    //    m_stateAutoUpdateProcessor = EStateIdle;
    //    emit autoUpdateProcessorStateChanged();
    //}

} // stopAutoUpdateProcessor

//------------------------------------------------------------------------------
CPCResourceMonitor::EState CPCResourceMonitor::getAutoUpdateProcessorState() const
//------------------------------------------------------------------------------
{
    return m_stateAutoUpdateProcessor;
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::setAutoUpdateIntervalProcessorInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    //if( m_autoUpdateIntervals.m_iProcessor_ms != i_iInterval_ms )
    //{
    //    if( m_pTimerAutoUpdateProcessor->isActive() )
    //    {
    //        m_pTimerAutoUpdateProcessor->stop();
    //    }

    //    m_autoUpdateIntervals.m_iProcessor_ms = i_iInterval_ms;
    //    saveAutoUpdateIntervals();
    //    emit autoUpdateIntervalProcessorChanged();

    //    if( m_stateAutoUpdateProcessor == EStateRunning && m_autoUpdateIntervals.m_iProcessor_ms > 0 )
    //    {
    //        m_pTimerAutoUpdateProcessor->start(m_autoUpdateIntervals.m_iProcessor_ms);
    //    }
    //}

} // setAutoUpdateIntervalProcessorInMs

//------------------------------------------------------------------------------
int CPCResourceMonitor::getAutoUpdateIntervalProcessorInMs() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals.m_iProcessor_ms;
}

/* Processor Values
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::updateProcessorValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateProcessorValues",
        /* strAddInfo   */ "" );

    //M_WBEM_INIT();

    //if( m_pIWbemRefresher != nullptr && m_pIWbemHiPerfEnumOSProc != nullptr && m_pModelProcessors != nullptr && m_pModelProcessorsValues != nullptr )
    //{
    //    HRESULT hRes;
    //    ULONG   udxObj;
    //    DWORD   dwVal;

    //    mthTracer.trace( "-> IWbemRefresher->Refresh()" );
    //    hRes = m_pIWbemRefresher->Refresh(0);
    //    mthTracer.trace( "<- IWbemRefresher->Refresh()" );

    //    if( FAILED(hRes) )
    //    {
    //        if( m_pErrLog != nullptr )
    //        {
    //            m_pErrLog->addEntry( SErrResultInfo(
    //                /* severity    */ EResultSeverityError,
    //                /* iErrNr      */ EResultRequestRefused,
    //                /* strErrNr    */ ZS::result2Str(EResultRequestRefused),
    //                /* strAddInfo  */ "IWbemRefresher->Refresh() failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + hResult2Str(hRes)) + ")",
    //                /* strSource   */ m_strObjName ) );
    //        }
    //        if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //        {
    //            mthTracer.trace("IWbemRefresher->Refresh() failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + hResult2Str(hRes)) + ")");
    //        }
    //    }

    //    if( SUCCEEDED(hRes) ) // Refresh successfull
    //    {
    //        ULONG uObjCount = 0;

    //        hRes = m_pIWbemHiPerfEnumOSProc->GetObjects(
    //            /* lFlags      */ 0L,
    //            /* uNumObjects */ m_uObjAccessOSProcCount,
    //            /* apObj       */ m_arpIWbemObjAccessOSProc,
    //            /* puReturned  */ &uObjCount );

    //        // On initial call m_uObjAccessOSProcCount was 0 and thats less than uObjCount ...
    //        if( hRes == WBEM_E_BUFFER_TOO_SMALL && m_uObjAccessOSProcCount < uObjCount )
    //        {
    //            m_arpIWbemObjAccessOSProc = new IWbemObjectAccess*[uObjCount];

    //            if( nullptr == m_arpIWbemObjAccessOSProc )
    //            {
    //                hRes = E_OUTOFMEMORY;
    //            }
    //            else
    //            {
    //                SecureZeroMemory( m_arpIWbemObjAccessOSProc, uObjCount*sizeof(IWbemObjectAccess*) );
    //                m_uObjAccessOSProcCount = uObjCount;

    //                // Please note that there is an additional entry for the total processor data
    //                // and therefore OSProcCount equals to ProcessorCount+1.
    //                hRes = m_pIWbemHiPerfEnumOSProc->GetObjects(
    //                    /* lFlags      */ 0L,
    //                    /* uNumObjects */ m_uObjAccessOSProcCount,
    //                    /* apObj       */ m_arpIWbemObjAccessOSProc,
    //                    /* puReturned  */ &uObjCount );
    //            }
    //        }

    //        if( FAILED(hRes) )
    //        {
    //            if( m_pErrLog != nullptr )
    //            {
    //                m_pErrLog->addEntry( SErrResultInfo(
    //                    /* severity    */ EResultSeverityError,
    //                    /* iErrNr      */ EResultRequestRefused,
    //                    /* strErrNr    */ ZS::result2Str(EResultRequestRefused),
    //                    /* strAddInfo  */ "IWbemHiPerfEnumOSProc->GetObjects() failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + hResult2Str(hRes)) + ")",
    //                    /* strSource   */ m_strObjName ) );
    //            }
    //            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //            {
    //                mthTracer.trace("IWbemHiPerfEnumOSProc->GetObjects() failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + hResult2Str(hRes)) + ")");
    //            }
    //        }

    //        if( SUCCEEDED(hRes) && m_arpIWbemObjAccessOSProc != nullptr && m_arpIWbemObjAccessOSProc[0] != nullptr )
    //        {
    //            if( !m_bIWbemHiPerfEnumOSProcGotHandles )
    //            {
    //                hRes = m_arpIWbemObjAccessOSProc[0]->GetPropertyHandle(
    //                    /* wszPropertyName */ L"PercentProcessorTime",
    //                    /* pType           */ &m_cimTypeOSProcLoad,
    //                    /* plHandle        */ &m_lhndOSProcLoad );
    //                m_bIWbemHiPerfEnumOSProcGotHandles = true;
    //            }

    //            for( udxObj = 0; udxObj < m_pModelProcessors->getLogicalProcessorsCount(); udxObj++ )
    //            {
    //                if( m_arpIWbemObjAccessOSProc[udxObj] != nullptr )
    //                {
    //                    // Processor load
    //                    //---------------

    //                    hRes = m_arpIWbemObjAccessOSProc[udxObj]->ReadDWORD(
    //                        /* lhnd  */ m_lhndOSProcLoad,
    //                        /* pdw   */ &dwVal );

    //                    m_pModelProcessorsValues->appendLoadInPerCent(static_cast<unsigned int>(udxObj),static_cast<double>(dwVal));

    //                    // Release class object
    //                    //----------------------

    //                    m_arpIWbemObjAccessOSProc[udxObj]->Release();
    //                    m_arpIWbemObjAccessOSProc[udxObj] = nullptr;
    //                }
    //            }

    //            emit processorValuesChanged();

    //        } // if( SUCCEEDED(hRes) && m_arpIWbemObjAccessOS != nullptr )
    //    } // if( SUCCEEDED(hRes) ) // Refresh successfull
    //} // if( m_pIWbemRefresher != nullptr && m_pIWbemHiPerfEnumOSProc != nullptr && m_pModelProcessors != nullptr && m_pModelProcessorsValues != nullptr )

} // updateProcessorValues

//------------------------------------------------------------------------------
void CPCResourceMonitor::startAutoUpdateProcessorValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startAutoUpdateProcessorValues",
        /* strAddInfo   */ "" );

    //if( m_stateAutoUpdateProcessorValues == EStateIdle )
    //{
    //    if( m_autoUpdateIntervals.m_iProcessorValues_ms > 0 && !m_pTimerAutoUpdateProcessorValues->isActive() )
    //    {
    //        m_pTimerAutoUpdateProcessorValues->start(m_autoUpdateIntervals.m_iProcessorValues_ms);
    //    }
    //    m_stateAutoUpdateProcessorValues = EStateRunning;
    //    emit autoUpdateProcessorValuesStateChanged();
    //}

} // startAutoUpdateProcessorValues

//------------------------------------------------------------------------------
void CPCResourceMonitor::stopAutoUpdateProcessorValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopAutoUpdateProcessorValues",
        /* strAddInfo   */ "" );

    //if( m_pTimerAutoUpdateProcessorValues->isActive() )
    //{
    //    m_pTimerAutoUpdateProcessorValues->stop();
    //}
    //if( m_stateAutoUpdateProcessorValues == EStateRunning )
    //{
    //    m_stateAutoUpdateProcessorValues = EStateIdle;
    //    emit autoUpdateProcessorValuesStateChanged();
    //}

} // stopAutoUpdateProcessorValues

//------------------------------------------------------------------------------
CPCResourceMonitor::EState CPCResourceMonitor::getAutoUpdateProcessorValuesState() const
//------------------------------------------------------------------------------
{
    return m_stateAutoUpdateProcessorValues;
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::setAutoUpdateIntervalProcessorValuesInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    //if( m_autoUpdateIntervals.m_iProcessorValues_ms != i_iInterval_ms )
    //{
    //    if( m_pTimerAutoUpdateProcessorValues->isActive() )
    //    {
    //        m_pTimerAutoUpdateProcessorValues->stop();
    //    }

    //    m_autoUpdateIntervals.m_iProcessorValues_ms = i_iInterval_ms;
    //    saveAutoUpdateIntervals();
    //    emit autoUpdateIntervalProcessorValuesChanged();

    //    if( m_stateAutoUpdateProcessorValues == EStateRunning && m_autoUpdateIntervals.m_iProcessorValues_ms > 0 )
    //    {
    //        m_pTimerAutoUpdateProcessorValues->start(m_autoUpdateIntervals.m_iProcessorValues_ms);
    //    }
    //}

} // setAutoUpdateIntervalProcessorValuesInMs

//------------------------------------------------------------------------------
int CPCResourceMonitor::getAutoUpdateIntervalProcessorValuesInMs() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals.m_iProcessorValues_ms;
}

/* Memory
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::updateMemory()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateMemory",
        /* strAddInfo   */ "" );

    M_WBEM_INIT();

    if( m_pIWbemServices != nullptr )
    {
        HRESULT               hRes;
        ULONG                 uReturn;
        VARIANT               vtProp;
        QString               strVal;
        double                fVal;
        bool                  bOk;
        IEnumWbemClassObject* pIEnumWbemClassObject = nullptr;
        IWbemClassObject*     pIWbemClassObject;

        double fTotalPhysicalMemorySize_B = 0.0;
        double fTotalVirtualMemorySize_B = 0.0;
        double fTotalVisibleMemorySize_B = 0.0;

        // FROM Computer System
        //---------------------

        hRes = m_pIWbemServices->ExecQuery(
            /* strQueryLanguage */ bstr_t("WQL"),
            /* strQuery         */ bstr_t("SELECT TotalPhysicalMemory FROM Win32_ComputerSystem"),
            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            /* pCtx             */ nullptr,
            /* ppEnum           */ &pIEnumWbemClassObject );

        if( FAILED(hRes) )
        {
            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
            if( hRes == WBEM_E_SHUTTING_DOWN )
            {
                resultSeverity = EResultSeverityInfo;
            }
            if( m_pErrLog != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IWbemServices->ExecQuery(SELECT TotalPhysicalMemory FROM Win32_ComputerSystem) failed with error code ";
                strAddErrInfo += QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")";
                SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateOperatingSystem", EResultRequestRefused, resultSeverity, strAddErrInfo);
                m_pErrLog->addEntry(errResultInfo);
            }
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("IWbemServices->ExecQuery(SELECT TotalPhysicalMemory FROM Win32_ComputerSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
            }
        }
        else if( pIEnumWbemClassObject != nullptr )
        {
            hRes = pIEnumWbemClassObject->Next(
                /* lTimeout   */ WBEM_INFINITE,
                /* uCount     */ 1,
                /* apObjects  */ &pIWbemClassObject,
                /* puReturned */ &uReturn );

            if( uReturn > 0 && pIWbemClassObject != nullptr )
            {
                // Total physical memory
                //----------------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"TotalPhysicalMemory",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
                {
                    fTotalPhysicalMemorySize_B = static_cast<double>(vtProp.intVal);
                }
                else if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    fVal = strVal.toDouble(&bOk);
                    if( bOk )
                    {
                        fTotalPhysicalMemorySize_B = fVal;
                    }
                }
                VariantClear(&vtProp);

                if( fTotalPhysicalMemorySize_B > 0.0 )
                {
                    m_pModelMemory->setPhysicalMemorySizeInB(fTotalPhysicalMemorySize_B);
                }

                // Release class object
                //---------------------

                pIWbemClassObject->Release();
                pIWbemClassObject = nullptr;
            }

            pIEnumWbemClassObject->Release();
            pIEnumWbemClassObject = nullptr;

        } // if( pIEnumWbemClassObject != nullptr )

        // FROM Operating System
        //----------------------

        // "TotalVirtualMemorySize" may not return the correct value. E.g. on a WinXP machine
        // 2 GB were returned even if the virtual memory size was more than 4 GB.
        // Also calculating the total virtual memory size by adding the amount of total RAM to the
        // amount of paging space did not return the correct value (8 GB instead of approx. 4GB).
        // So I decided not to monitor the virtual memory usage if the reported "TotalVirtualVemorySize"
        // is less than the "TotalVisibleMemorySize" - which should not never be the case as the
        // virtual memory should include the visible RAM memory plus some space on disk.

        hRes = m_pIWbemServices->ExecQuery(
            /* strQueryLanguage */ bstr_t("WQL"),
            /* strQuery         */ bstr_t("SELECT TotalVirtualMemorySize, TotalVisibleMemorySize FROM Win32_OperatingSystem"),
            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            /* pCtx             */ nullptr,
            /* ppEnum           */ &pIEnumWbemClassObject );

        if( FAILED(hRes) )
        {
            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
            if( hRes == WBEM_E_SHUTTING_DOWN )
            {
                resultSeverity = EResultSeverityInfo;
            }
            if( m_pErrLog != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IWbemServices->ExecQuery(SELECT TotalVirtualMemorySize, TotalVisibleMemorySize FROM Win32_ComputerSystem) failed with error code ";
                strAddErrInfo += QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")";
                SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateMemory", EResultRequestRefused, resultSeverity, strAddErrInfo);
                m_pErrLog->addEntry(errResultInfo);
            }
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("IWbemServices->ExecQuery(SELECT TotalVirtualMemorySize, TotalVisibleMemorySize FROM Win32_OperatingSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
            }
        }
        else if( pIEnumWbemClassObject != nullptr )
        {
            hRes = pIEnumWbemClassObject->Next(
                /* lTimeout   */ WBEM_INFINITE,
                /* uCount     */ 1,
                /* apObjects  */ &pIWbemClassObject,
                /* puReturned */ &uReturn );

            if( uReturn > 0 && pIWbemClassObject != nullptr )
            {
                // Total virtual memory size
                //--------------------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"TotalVirtualMemorySize",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
                {
                    fTotalVirtualMemorySize_B = 1024.0*static_cast<double>(vtProp.intVal);
                }
                else if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    fVal = strVal.toDouble(&bOk);
                    if( bOk )
                    {
                        fTotalVirtualMemorySize_B = 1024.0*fVal;
                    }
                }
                VariantClear(&vtProp);

                // Size stored in paging files
                //----------------------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"TotalVisibleMemorySize",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
                {
                    fTotalVisibleMemorySize_B = 1024.0*static_cast<double>(vtProp.intVal);
                }
                else if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    fVal = strVal.toDouble(&bOk);
                    if( bOk )
                    {
                        fTotalVisibleMemorySize_B = 1024.0*fVal;
                    }
                }
                VariantClear(&vtProp);

                // Release class object
                //---------------------

                pIWbemClassObject->Release();
                pIWbemClassObject = nullptr;
            }

            if( fTotalVisibleMemorySize_B > fTotalVirtualMemorySize_B )
            {
                if( m_pErrLog != nullptr )
                {
                    QString strAddErrInfo;
                    strAddErrInfo  = "OS reported wrong value (" + QString::number(fTotalVirtualMemorySize_B/1024.0,'f',0) + " kB) for total virtual memory size. Assuming PC with more than 4GB virtual memory.";
                    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateMemory", EResultInvalidValue, EResultSeverityWarning, strAddErrInfo);
                    m_pErrLog->addEntry(errResultInfo, "Updating the OS with the appropriate hot fix package may help");
                }
                fTotalVirtualMemorySize_B = 4.0e9;
                if( fTotalVisibleMemorySize_B > fTotalVirtualMemorySize_B )
                {
                    fTotalVirtualMemorySize_B = fTotalVisibleMemorySize_B;
                }
            }
            m_pModelMemory->setVirtualMemorySizeInB(fTotalVirtualMemorySize_B);

            pIEnumWbemClassObject->Release();
            pIEnumWbemClassObject = nullptr;

        } // if( pIEnumWbemClassObject != nullptr )

        emit memoryChanged();

    } // if( m_pIWbemServices != nullptr )

} // updateMemory

//------------------------------------------------------------------------------
void CPCResourceMonitor::startAutoUpdateMemory()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startAutoUpdateMemory",
        /* strAddInfo   */ "" );

    if( m_stateAutoUpdateMemory == EStateIdle )
    {
        if( m_autoUpdateIntervals.m_iMemory_ms > 0 && !m_pTimerAutoUpdateMemory->isActive() )
        {
            m_pTimerAutoUpdateMemory->start(m_autoUpdateIntervals.m_iMemory_ms);
        }
        m_stateAutoUpdateMemory = EStateRunning;
        emit autoUpdateMemoryStateChanged();
    }

} // startAutoUpdateMemory

//------------------------------------------------------------------------------
void CPCResourceMonitor::stopAutoUpdateMemory()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopAutoUpdateMemory",
        /* strAddInfo   */ "" );

    if( m_pTimerAutoUpdateMemory->isActive() )
    {
        m_pTimerAutoUpdateMemory->stop();
    }
    if( m_stateAutoUpdateMemory == EStateRunning )
    {
        m_stateAutoUpdateMemory = EStateIdle;
        emit autoUpdateMemoryStateChanged();
    }

} // stopAutoUpdateMemory

//------------------------------------------------------------------------------
CPCResourceMonitor::EState CPCResourceMonitor::getAutoUpdateMemoryState() const
//------------------------------------------------------------------------------
{
    return m_stateAutoUpdateMemory;
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::setAutoUpdateIntervalMemoryInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    if( m_autoUpdateIntervals.m_iMemory_ms != i_iInterval_ms )
    {
        if( m_pTimerAutoUpdateMemory->isActive() )
        {
            m_pTimerAutoUpdateMemory->stop();
        }

        m_autoUpdateIntervals.m_iMemory_ms = i_iInterval_ms;
        saveAutoUpdateIntervals();
        emit autoUpdateIntervalMemoryChanged();

        if( m_stateAutoUpdateMemory == EStateRunning && m_autoUpdateIntervals.m_iMemory_ms > 0 )
        {
            m_pTimerAutoUpdateMemory->start(m_autoUpdateIntervals.m_iMemory_ms);
        }
    }

} // setAutoUpdateIntervalMemoryInMs

//------------------------------------------------------------------------------
int CPCResourceMonitor::getAutoUpdateIntervalMemoryInMs() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals.m_iMemory_ms;
}

/* Memory Values
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::updateMemoryValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateMemoryValues",
        /* strAddInfo   */ "" );

    M_WBEM_INIT();

    if( m_pIWbemServices != nullptr )
    {
        HRESULT               hRes;
        ULONG                 uReturn;
        VARIANT               vtProp;
        QString               strVal;
        double                fVal;
        bool                  bOk;
        IEnumWbemClassObject* pIEnumWbemClassObject = nullptr;
        IWbemClassObject*     pIWbemClassObject;

        // FROM Operating System
        //----------------------

        hRes = m_pIWbemServices->ExecQuery(
            /* strQueryLanguage */ bstr_t("WQL"),
            /* strQuery         */ bstr_t("SELECT FreePhysicalMemory, FreeVirtualMemory FROM Win32_OperatingSystem"),
            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            /* pCtx             */ nullptr,
            /* ppEnum           */ &pIEnumWbemClassObject );

        if( FAILED(hRes) )
        {
            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
            if( hRes == WBEM_E_SHUTTING_DOWN )
            {
                resultSeverity = EResultSeverityInfo;
            }
            if( m_pErrLog != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IWbemServices->ExecQuery(SELECT FreePhysicalMemory, FreeVirtualMemory FROM Win32_OperatingSystem) failed with error code ";
                strAddErrInfo += QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")";
                SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "updateMemoryValues", EResultRequestRefused, resultSeverity, strAddErrInfo);
                m_pErrLog->addEntry(errResultInfo);
            }
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("IWbemServices->ExecQuery(SELECT FreePhysicalMemory, FreeVirtualMemory FROM Win32_OperatingSystem) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
            }
        }
        else if( pIEnumWbemClassObject != nullptr )
        {
            hRes = pIEnumWbemClassObject->Next(
                /* lTimeout   */ WBEM_INFINITE,
                /* uCount     */ 1,
                /* apObjects  */ &pIWbemClassObject,
                /* puReturned */ &uReturn );

            if( uReturn > 0 && pIWbemClassObject != nullptr )
            {
                // Free Physical Memory
                //---------------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"FreePhysicalMemory",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
                {
                    m_pModelMemoryValues->appendPhysicalMemoryFreeSizeInB(1024.0*static_cast<double>(vtProp.intVal));
                }
                else if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    fVal = strVal.toDouble(&bOk);
                    if( bOk )
                    {
                        m_pModelMemoryValues->appendPhysicalMemoryFreeSizeInB(1024.0*fVal);
                    }
                }
                VariantClear(&vtProp);

                // Free Virtual Memory
                //---------------------

                hRes = pIWbemClassObject->Get(
                    /* wszName  */ L"FreeVirtualMemory",
                    /* lFlags   */ 0,
                    /* pVal     */ &vtProp,
                    /* pType    */ 0,
                    /* plFlavor */ 0 );
                if( SUCCEEDED(hRes) && vtProp.vt == VT_I4 )
                {
                    m_pModelMemoryValues->appendVirtualMemoryFreeSizeInB(1024.0*static_cast<double>(vtProp.intVal));
                }
                else if( SUCCEEDED(hRes) && vtProp.vt == VT_BSTR )
                {
                    strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
                    fVal = strVal.toDouble(&bOk);
                    if( bOk )
                    {
                        m_pModelMemoryValues->appendVirtualMemoryFreeSizeInB(1024.0*fVal);
                    }
                }
                VariantClear(&vtProp);

                // Release class object
                //---------------------

                pIWbemClassObject->Release();
                pIWbemClassObject = nullptr;
            }

            pIEnumWbemClassObject->Release();
            pIEnumWbemClassObject = nullptr;

        } // if( pIEnumWbemClassObject != nullptr )

        emit memoryValuesChanged();

    } // if( m_pIWbemServices != nullptr )

} // updateMemoryValues

//------------------------------------------------------------------------------
void CPCResourceMonitor::startAutoUpdateMemoryValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startAutoUpdateMemoryValues",
        /* strAddInfo   */ "" );

    if( m_stateAutoUpdateMemoryValues == EStateIdle )
    {
        if( m_autoUpdateIntervals.m_iMemoryValues_ms > 0 && !m_pTimerAutoUpdateMemoryValues->isActive() )
        {
            m_pTimerAutoUpdateMemoryValues->start(m_autoUpdateIntervals.m_iMemoryValues_ms);
        }
        m_stateAutoUpdateMemoryValues = EStateRunning;
        emit autoUpdateMemoryValuesStateChanged();
    }

} // startAutoUpdateMemoryValues

//------------------------------------------------------------------------------
void CPCResourceMonitor::stopAutoUpdateMemoryValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopAutoUpdateMemoryValues",
        /* strAddInfo   */ "" );

    if( m_pTimerAutoUpdateMemoryValues->isActive() )
    {
        m_pTimerAutoUpdateMemoryValues->stop();
    }
    if( m_stateAutoUpdateMemoryValues == EStateRunning )
    {
        m_stateAutoUpdateMemoryValues = EStateIdle;
        emit autoUpdateMemoryValuesStateChanged();
    }

} // stopAutoUpdateMemoryValues

//------------------------------------------------------------------------------
CPCResourceMonitor::EState CPCResourceMonitor::getAutoUpdateMemoryValuesState() const
//------------------------------------------------------------------------------
{
    return m_stateAutoUpdateMemoryValues;
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::setAutoUpdateIntervalMemoryValuesInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    if( m_autoUpdateIntervals.m_iMemoryValues_ms != i_iInterval_ms )
    {
        if( m_pTimerAutoUpdateMemoryValues->isActive() )
        {
            m_pTimerAutoUpdateMemoryValues->stop();
        }

        m_autoUpdateIntervals.m_iMemoryValues_ms = i_iInterval_ms;
        saveAutoUpdateIntervals();
        emit autoUpdateIntervalMemoryValuesChanged();

        if( m_stateAutoUpdateMemoryValues == EStateRunning && m_autoUpdateIntervals.m_iMemoryValues_ms > 0 )
        {
            m_pTimerAutoUpdateMemoryValues->start(m_autoUpdateIntervals.m_iMemoryValues_ms);
        }
    }

} // setAutoUpdateIntervalMemoryValuesInMs

//------------------------------------------------------------------------------
int CPCResourceMonitor::getAutoUpdateIntervalMemoryValuesInMs() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals.m_iMemoryValues_ms;
}

/* Logical Disks
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::updateLogicalDisks()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateLogicalDisks",
        /* strAddInfo   */ "" );

    //M_WBEM_INIT();

    //if( m_pIWbemServices != nullptr )
    //{
    //    HRESULT               hRes;
    //    ULONG                 uReturn;
    //    VARIANT               vtProp;
    //    QString               strVal;
    //    double                fVal;
    //    int                   iVal;
    //    bool                  bOk;
    //    ULONG                 udxObj;
    //    IEnumWbemClassObject* pIEnumWbemClassObject = nullptr;
    //    IWbemClassObject*     pIWbemClassObject;

    //    // FROM Logical Disk
    //    //------------------

    //    // Detect number of logical disk if not yet detected ..
    //    if( m_pModelLogicalDisks->getDisksCount() == 0 )
    //    {
    //        ULONG uObjCount = 0;

    //        hRes = m_pIWbemServices->ExecQuery(
    //            /* strQueryLanguage */ bstr_t("WQL"),
    //            /* strQuery         */ bstr_t("SELECT BlockSize FROM Win32_LogicalDisk"),
    //            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
    //            /* pCtx             */ nullptr,
    //            /* ppEnum           */ &pIEnumWbemClassObject );

    //        if( FAILED(hRes) )
    //        {
    //            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
    //            if( hRes == WBEM_E_SHUTTING_DOWN )
    //            {
    //                resultSeverity = EResultSeverityInfo;
    //            }
    //            if( m_pErrLog != nullptr )
    //            {
    //                m_pErrLog->addEntry( SErrResultInfo(
    //                    /* severity    */ resultSeverity,
    //                    /* iErrNr      */ EResultRequestRefused,
    //                    /* strAddInfo  */ "IWbemServices->ExecQuery(SELECT BlockSize FROM Win32_LogicalDisk) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")",
    //                    /* strSource   */ m_strObjName ) );
    //            }
    //            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //            {
    //                mthTracer.trace("IWbemServices->ExecQuery(SELECT BlockSize FROM Win32_LogicalDisk) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
    //            }
    //        }
    //        else if( pIEnumWbemClassObject != nullptr )
    //        {
    //            while( true )
    //            {
    //                hRes = pIEnumWbemClassObject->Next(
    //                    /* lTimeout   */ WBEM_INFINITE,
    //                    /* uCount     */ 1,
    //                    /* apObjects  */ &pIWbemClassObject,
    //                    /* puReturned */ &uReturn );

    //                if( uReturn == 0 || pIWbemClassObject == nullptr )
    //                {
    //                    break;
    //                }
    //                uObjCount++;

    //                pIWbemClassObject->Release();
    //                pIWbemClassObject = nullptr;

    //            } // while( true )

    //            if( uObjCount > 0 )
    //            {
    //                m_pModelLogicalDisks->setDisksCount(uObjCount);

    //                if( m_pModelLogicalDisksValues == nullptr )
    //                {
    //                    m_pModelLogicalDisksValues = new CModelLogicalDisksValues(m_pModelLogicalDisks);
    //                }
    //            }

    //            pIEnumWbemClassObject->Release();
    //            pIEnumWbemClassObject = nullptr;

    //        } // if( pIEnumWbemClassObject != nullptr )

    //    } // if( m_pModelLogicalDisks->getDisksCount() == 0 )

    //    // FROM Logical Disk
    //    //------------------

    //    hRes = m_pIWbemServices->ExecQuery(
    //        /* strQueryLanguage */ bstr_t("WQL"),
    //        /* strQuery         */ bstr_t("SELECT DriveType, Caption, Description, DeviceID, Name, VolumeName, VolumeSerialNumber, BlockSize, Size FROM Win32_LogicalDisk"),
    //        /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
    //        /* pCtx             */ nullptr,
    //        /* ppEnum           */ &pIEnumWbemClassObject );

    //    if( FAILED(hRes) )
    //    {
    //        ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
    //        if( hRes == WBEM_E_SHUTTING_DOWN )
    //        {
    //            resultSeverity = EResultSeverityInfo;
    //        }
    //        if( m_pErrLog != nullptr )
    //        {
    //            m_pErrLog->addEntry( SErrResultInfo(
    //                /* severity    */ resultSeverity,
    //                /* iErrNr      */ EResultRequestRefused,
    //                /* strAddInfo  */ "IWbemServices->ExecQuery(SELECT DriveType, Caption, Description, DeviceID, Name, VolumeName, VolumeSerialNumber, BlockSize, Size FROM Win32_LogicalDisk) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")",
    //                /* strSource   */ m_strObjName ) );
    //        }
    //        if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //        {
    //            mthTracer.trace("IWbemServices->ExecQuery(SELECT DriveType, Caption, Description, DeviceID, Name, VolumeName, VolumeSerialNumber, BlockSize, Size FROM Win32_LogicalDisk) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
    //        }
    //    }
    //    else if( pIEnumWbemClassObject != nullptr )
    //    {
    //        udxObj = 0;

    //        while( true )
    //        {
    //            hRes = pIEnumWbemClassObject->Next(
    //                /* lTimeout   */ WBEM_INFINITE,
    //                /* uCount     */ 1,
    //                /* apObjects  */ &pIWbemClassObject,
    //                /* puReturned */ &uReturn );

    //            if( uReturn == 0 || pIWbemClassObject == nullptr )
    //            {
    //                break;
    //            }

    //            // Drive Type
    //            //-----------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"DriveType",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_I4 )
    //                {
    //                    if( vtProp.intVal >= 0 && vtProp.intVal < CPCResourceMonitor::EDriveTypeCount )
    //                    {
    //                        CPCResourceMonitor::EDriveType driveType = static_cast<CPCResourceMonitor::EDriveType>(vtProp.intVal);
    //                        m_pModelLogicalDisks->setDriveType( udxObj, driveType );
    //                    }
    //                }
    //                else if( vtProp.vt == VT_BSTR )
    //                {
    //                    strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
    //                    iVal = strVal.toInt(&bOk);
    //                    if( bOk )
    //                    {
    //                        CPCResourceMonitor::EDriveType driveType = static_cast<CPCResourceMonitor::EDriveType>(iVal);
    //                        m_pModelLogicalDisks->setDriveType( udxObj, driveType );
    //                    }
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Caption
    //            //--------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"Caption",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_BSTR )
    //                {
    //                    m_pModelLogicalDisks->setCaption( udxObj, QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) ) );
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Description
    //            //------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"Description",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_BSTR )
    //                {
    //                    m_pModelLogicalDisks->setDescription( udxObj, QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) ) );
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Device Id
    //            //----------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"DeviceID",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_BSTR )
    //                {
    //                    m_pModelLogicalDisks->setDeviceId( udxObj, QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) ) );
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Name
    //            //-----

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"Name",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_BSTR )
    //                {
    //                    m_pModelLogicalDisks->setName( udxObj, QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) ) );
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Volume Name
    //            //------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"VolumeName",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_BSTR )
    //                {
    //                    m_pModelLogicalDisks->setVolumeName( udxObj, QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) ) );
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Volume Serial Number
    //            //---------------------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"VolumeSerialNumber",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_BSTR )
    //                {
    //                    m_pModelLogicalDisks->setVolumeSerialNumber( udxObj, QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) ) );
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Block Size
    //            //-----------

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"BlockSize",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_I4 )
    //                {
    //                    m_pModelLogicalDisks->setBlockSize( udxObj, CPhysVal( static_cast<double>(vtProp.intVal), DataQuantity().Byte() ) );
    //                }
    //                else if( vtProp.vt == VT_BSTR )
    //                {
    //                    strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
    //                    fVal = strVal.toDouble(&bOk);
    //                    if( bOk )
    //                    {
    //                        m_pModelLogicalDisks->setBlockSize( udxObj, CPhysVal( fVal, DataQuantity().Byte() ) );
    //                    }
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Size
    //            //-----

    //            hRes = pIWbemClassObject->Get(
    //                /* wszName  */ L"Size",
    //                /* lFlags   */ 0,
    //                /* pVal     */ &vtProp,
    //                /* pType    */ 0,
    //                /* plFlavor */ 0 );
    //            if( SUCCEEDED(hRes) )
    //            {
    //                if( vtProp.vt == VT_I4 )
    //                {
    //                    m_pModelLogicalDisks->setSize( udxObj, CPhysVal( static_cast<double>(vtProp.intVal), DataQuantity().Byte() ) );
    //                }
    //                else if( vtProp.vt == VT_BSTR )
    //                {
    //                    strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
    //                    fVal = strVal.toDouble(&bOk);
    //                    if( bOk )
    //                    {
    //                        m_pModelLogicalDisks->setSize( udxObj, CPhysVal( fVal, DataQuantity().Byte() ) );
    //                    }
    //                }
    //            }
    //            VariantClear(&vtProp);

    //            // Release class object
    //            //---------------------

    //            pIWbemClassObject->Release();
    //            pIWbemClassObject = nullptr;

    //            // Next disk (if any)
    //            //-------------------

    //            udxObj++;

    //        } // while( true )

    //        pIEnumWbemClassObject->Release();
    //        pIEnumWbemClassObject = nullptr;

    //    } // if( pIEnumWbemClassObject != nullptr )

    //    emit logicalDisksChanged();

    //} // if( m_pIWbemServices != nullptr )

} // updateLogicalDisks

//------------------------------------------------------------------------------
void CPCResourceMonitor::startAutoUpdateLogicalDisks()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startAutoUpdateLogicalDisks",
        /* strAddInfo   */ "" );

    //if( m_stateAutoUpdateLogicalDisks == EStateIdle )
    //{
    //    if( m_autoUpdateIntervals.m_iLogicalDisks_ms > 0 && !m_pTimerAutoUpdateLogicalDisks->isActive() )
    //    {
    //        m_pTimerAutoUpdateLogicalDisks->start(m_autoUpdateIntervals.m_iLogicalDisks_ms);
    //    }
    //    m_stateAutoUpdateLogicalDisks = EStateRunning;
    //    emit autoUpdateLogicalDisksStateChanged();
    //}

} // startAutoUpdateLogicalDisks

//------------------------------------------------------------------------------
void CPCResourceMonitor::stopAutoUpdateLogicalDisks()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopAutoUpdateLogicalDisks",
        /* strAddInfo   */ "" );

    //if( m_pTimerAutoUpdateLogicalDisks->isActive() )
    //{
    //    m_pTimerAutoUpdateLogicalDisks->stop();
    //}
    //if( m_stateAutoUpdateLogicalDisks == EStateRunning )
    //{
    //    m_stateAutoUpdateLogicalDisks = EStateIdle;
    //    emit autoUpdateLogicalDisksStateChanged();
    //}

} // stopAutoUpdateLogicalDisks

//------------------------------------------------------------------------------
CPCResourceMonitor::EState CPCResourceMonitor::getAutoUpdateLogicalDisksState() const
//------------------------------------------------------------------------------
{
    return m_stateAutoUpdateLogicalDisks;
}

/* Logical Disks Values Values
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::updateLogicalDisksValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateLogicalDisksValues",
        /* strAddInfo   */ "" );

    //M_WBEM_INIT();

    //if( m_pIWbemServices != nullptr )
    //{
    //    HRESULT               hRes;
    //    ULONG                 uReturn;
    //    VARIANT               vtProp;
    //    QString               strVal;
    //    double                fVal;
    //    bool                  bOk;
    //    ULONG                 udxObj;
    //    IEnumWbemClassObject* pIEnumWbemClassObject = nullptr;
    //    IWbemClassObject*     pIWbemClassObject;

    //    // FROM Logical Disk
    //    //------------------

    //    // Detect number of logical disk if not yet detected ..
    //    if( m_pModelLogicalDisks->getDisksCount() == 0 )
    //    {
    //        ULONG uObjCount = 0;

    //        hRes = m_pIWbemServices->ExecQuery(
    //            /* strQueryLanguage */ bstr_t("WQL"),
    //            /* strQuery         */ bstr_t("SELECT FreeSpace FROM Win32_LogicalDisk"),
    //            /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
    //            /* pCtx             */ nullptr,
    //            /* ppEnum           */ &pIEnumWbemClassObject );

    //        if( FAILED(hRes) )
    //        {
    //            ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
    //            if( hRes == WBEM_E_SHUTTING_DOWN )
    //            {
    //                resultSeverity = EResultSeverityInfo;
    //            }
    //            if( m_pErrLog != nullptr )
    //            {
    //                m_pErrLog->addEntry( SErrResultInfo(
    //                    /* severity    */ resultSeverity,
    //                    /* iErrNr      */ EResultRequestRefused,
    //                    /* strAddInfo  */ "IWbemServices->ExecQuery(SELECT FreeSpace FROM Win32_LogicalDisk) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")",
    //                    /* strSource   */ m_strObjName ) );
    //            }
    //            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //            {
    //                mthTracer.trace("IWbemServices->ExecQuery(SELECT FreeSpace FROM Win32_LogicalDisk) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
    //            }
    //        }
    //        else if( pIEnumWbemClassObject != nullptr )
    //        {
    //            while( true )
    //            {
    //                uObjCount++;

    //                hRes = pIEnumWbemClassObject->Next(
    //                    /* lTimeout   */ WBEM_INFINITE,
    //                    /* uCount     */ 1,
    //                    /* apObjects  */ &pIWbemClassObject,
    //                    /* puReturned */ &uReturn );

    //                if( uReturn == 0 || pIWbemClassObject == nullptr )
    //                {
    //                    break;
    //                }

    //                // Release class object
    //                //---------------------

    //                pIWbemClassObject->Release();
    //                pIWbemClassObject = nullptr;

    //            } // while( true )

    //            if( uObjCount > 0 )
    //            {
    //                m_pModelLogicalDisks->setDisksCount(uObjCount);

    //                if( m_pModelLogicalDisksValues == nullptr )
    //                {
    //                    m_pModelLogicalDisksValues = new CModelLogicalDisksValues(m_pModelLogicalDisks);
    //                }
    //            }

    //            pIEnumWbemClassObject->Release();
    //            pIEnumWbemClassObject = nullptr;

    //        } // if( pIEnumWbemClassObject != nullptr )

    //    } // if( m_pModelLogicalDisks->getDisksCount() == 0 )

    //    // FROM Logical Disk
    //    //------------------

    //    hRes = m_pIWbemServices->ExecQuery(
    //        /* strQueryLanguage */ bstr_t("WQL"),
    //        /* strQuery         */ bstr_t("SELECT FreeSpace FROM Win32_LogicalDisk"),
    //        /* lflags           */ WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
    //        /* pCtx             */ nullptr,
    //        /* ppEnum           */ &pIEnumWbemClassObject );

    //    if( FAILED(hRes) )
    //    {
    //        ZS::System::EResultSeverity resultSeverity = EResultSeverityError;
    //        if( hRes == WBEM_E_SHUTTING_DOWN )
    //        {
    //            resultSeverity = EResultSeverityInfo;
    //        }
    //        if( m_pErrLog != nullptr )
    //        {
    //            m_pErrLog->addEntry( SErrResultInfo(
    //                /* severity    */ resultSeverity,
    //                /* iErrNr      */ EResultRequestRefused,
    //                /* strAddInfo  */ "IWbemServices->ExecQuery(SELECT FreeSpace FROM Win32_LogicalDisk) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")",
    //                /* strSource   */ m_strObjName ) );
    //        }
    //        if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    //        {
    //            mthTracer.trace("IWbemServices->ExecQuery(SELECT FreeSpace FROM Win32_LogicalDisk) failed with error code " + QString::number(static_cast<unsigned int>(hRes),16) + " (" + wbemErrCode2Str(hRes) + ")");
    //        }
    //    }
    //    else if( pIEnumWbemClassObject != nullptr )
    //    {
    //        udxObj = 0;

    //        while( true )
    //        {
    //            hRes = pIEnumWbemClassObject->Next(
    //                /* lTimeout   */ WBEM_INFINITE,
    //                /* uCount     */ 1,
    //                /* apObjects  */ &pIWbemClassObject,
    //                /* puReturned */ &uReturn );

    //            if( uReturn == 0 || pIWbemClassObject == nullptr )
    //            {
    //                break;
    //            }

    //            // Free Space
    //            //-----------

    //            if( m_pModelLogicalDisksValues != nullptr )
    //            {
    //                hRes = pIWbemClassObject->Get(
    //                    /* wszName  */ L"FreeSpace",
    //                    /* lFlags   */ 0,
    //                    /* pVal     */ &vtProp,
    //                    /* pType    */ 0,
    //                    /* plFlavor */ 0 );
    //                if( SUCCEEDED(hRes) )
    //                {
    //                    if( vtProp.vt == VT_I4 )
    //                    {
    //                        m_pModelLogicalDisksValues->appendFreeSpace( udxObj, CPhysVal( static_cast<double>(vtProp.intVal), DataQuantity().Byte() ) );
    //                    }
    //                    else if( vtProp.vt == VT_BSTR )
    //                    {
    //                        strVal = QString( (QChar*)vtProp.bstrVal, static_cast<int>(wcslen(vtProp.bstrVal)) );
    //                        fVal = strVal.toDouble(&bOk);
    //                        if( bOk )
    //                        {
    //                            m_pModelLogicalDisksValues->appendFreeSpace( udxObj, CPhysVal( fVal, DataQuantity().Byte() ) );
    //                        }
    //                    }
    //                }
    //                VariantClear(&vtProp);
    //            }

    //            // Release class object
    //            //---------------------

    //            pIWbemClassObject->Release();
    //            pIWbemClassObject = nullptr;

    //            // Next disk (if any)
    //            //-------------------

    //            udxObj++;

    //        } // while( true )

    //        pIEnumWbemClassObject->Release();
    //        pIEnumWbemClassObject = nullptr;

    //    } // if( pIEnumWbemClassObject != nullptr )

    //    emit logicalDisksValuesChanged();

    //} // if( m_pIWbemServices != nullptr )

} // updateLogicalDisksValues

//------------------------------------------------------------------------------
void CPCResourceMonitor::startAutoUpdateLogicalDisksValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startAutoUpdateLogicalDisksValues",
        /* strAddInfo   */ "" );

    //if( m_stateAutoUpdateLogicalDisksValues == EStateIdle )
    //{
    //    if( m_autoUpdateIntervals.m_iLogicalDisksValues_ms > 0 && !m_pTimerAutoUpdateLogicalDisksValues->isActive() )
    //    {
    //        m_pTimerAutoUpdateLogicalDisksValues->start(m_autoUpdateIntervals.m_iLogicalDisksValues_ms);
    //    }
    //    m_stateAutoUpdateLogicalDisksValues = EStateRunning;
    //    emit autoUpdateLogicalDisksValuesStateChanged();
    //}

} // startAutoUpdateLogicalDisksValues

//------------------------------------------------------------------------------
void CPCResourceMonitor::stopAutoUpdateLogicalDisksValues()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopAutoUpdateLogicalDisksValues",
        /* strAddInfo   */ "" );

    //if( m_pTimerAutoUpdateLogicalDisksValues->isActive() )
    //{
    //    m_pTimerAutoUpdateLogicalDisksValues->stop();
    //}
    //if( m_stateAutoUpdateLogicalDisksValues == EStateRunning )
    //{
    //    m_stateAutoUpdateLogicalDisksValues = EStateIdle;
    //    emit autoUpdateLogicalDisksValuesStateChanged();
    //}

} // stopAutoUpdateLogicalDisksValues

//------------------------------------------------------------------------------
CPCResourceMonitor::EState CPCResourceMonitor::getAutoUpdateLogicalDisksValuesState() const
//------------------------------------------------------------------------------
{
    return m_stateAutoUpdateLogicalDisksValues;
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::setAutoUpdateIntervalLogicalDisksInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    //if( m_autoUpdateIntervals.m_iLogicalDisks_ms != i_iInterval_ms )
    //{
    //    if( m_pTimerAutoUpdateLogicalDisks->isActive() )
    //    {
    //        m_pTimerAutoUpdateLogicalDisks->stop();
    //    }

    //    m_autoUpdateIntervals.m_iLogicalDisks_ms = i_iInterval_ms;
    //    saveAutoUpdateIntervals();
    //    emit autoUpdateIntervalLogicalDisksChanged();

    //    if( m_stateAutoUpdateLogicalDisks == EStateRunning && m_autoUpdateIntervals.m_iLogicalDisks_ms > 0 )
    //    {
    //        m_pTimerAutoUpdateLogicalDisks->start(m_autoUpdateIntervals.m_iLogicalDisks_ms);
    //    }
    //}

} // setAutoUpdateIntervalLogicalDisksInMs

//------------------------------------------------------------------------------
int CPCResourceMonitor::getAutoUpdateIntervalLogicalDisksInMs() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals.m_iLogicalDisks_ms;
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::setAutoUpdateIntervalLogicalDisksValuesInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    //if( m_autoUpdateIntervals.m_iLogicalDisksValues_ms != i_iInterval_ms )
    //{
    //    if( m_pTimerAutoUpdateLogicalDisksValues->isActive() )
    //    {
    //        m_pTimerAutoUpdateLogicalDisksValues->stop();
    //    }

    //    m_autoUpdateIntervals.m_iLogicalDisksValues_ms = i_iInterval_ms;
    //    saveAutoUpdateIntervals();
    //    emit autoUpdateIntervalLogicalDisksValuesChanged();

    //    if( m_stateAutoUpdateLogicalDisksValues == EStateRunning && m_autoUpdateIntervals.m_iLogicalDisksValues_ms > 0 )
    //    {
    //        m_pTimerAutoUpdateLogicalDisksValues->start(m_autoUpdateIntervals.m_iLogicalDisksValues_ms);
    //    }
    //}

} // setAutoUpdateIntervalLogicalDisksValuesInMs

//------------------------------------------------------------------------------
int CPCResourceMonitor::getAutoUpdateIntervalLogicalDisksValuesInMs() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals.m_iLogicalDisksValues_ms;
}

/*==============================================================================
protected:
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::saveAutoUpdateIntervals()
//------------------------------------------------------------------------------
{
    //if( m_pSettingsFile != nullptr )
    //{
    //    m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalComputerSystemInMs", m_autoUpdateIntervals.m_iComputerSystem_ms );
    //    m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalOperatingSystemInMs", m_autoUpdateIntervals.m_iOperatingSystem_ms );
    //    m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalProcessorInMs", m_autoUpdateIntervals.m_iProcessor_ms );
    //    m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalProcessorValuesInMs", m_autoUpdateIntervals.m_iProcessorValues_ms );
    //    m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalMemoryInMs", m_autoUpdateIntervals.m_iMemory_ms );
    //    m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalMemoryValuesInMs", m_autoUpdateIntervals.m_iMemoryValues_ms );
    //    m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalLogicalDisksInMs", m_autoUpdateIntervals.m_iLogicalDisks_ms );
    //    m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalLogicalDisksValuesInMs", m_autoUpdateIntervals.m_iLogicalDisksValues_ms );
    //    m_pSettingsFile->sync();
    //}

} // saveAutoUpdateIntervals

//------------------------------------------------------------------------------
void CPCResourceMonitor::readAutoUpdateIntervals()
//------------------------------------------------------------------------------
{
    //if( m_pSettingsFile != nullptr )
    //{
    //    m_pSettingsFile->sync();

    //    SAutoUpdateIntervals autoUpdateIntervalsCurr = m_autoUpdateIntervals;

    //    autoUpdateIntervalsCurr.m_iComputerSystem_ms = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalComputerSystemInMs", autoUpdateIntervalsCurr.m_iComputerSystem_ms ).toInt();
    //    autoUpdateIntervalsCurr.m_iOperatingSystem_ms = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalOperatingSystemInMs", autoUpdateIntervalsCurr.m_iOperatingSystem_ms ).toInt();
    //    autoUpdateIntervalsCurr.m_iProcessor_ms = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalProcessorInMs", autoUpdateIntervalsCurr.m_iProcessor_ms ).toInt();
    //    autoUpdateIntervalsCurr.m_iProcessorValues_ms = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalProcessorValuesInMs", autoUpdateIntervalsCurr.m_iProcessorValues_ms ).toInt();
    //    autoUpdateIntervalsCurr.m_iMemory_ms   = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalMemoryInMs", autoUpdateIntervalsCurr.m_iMemory_ms ).toInt();
    //    autoUpdateIntervalsCurr.m_iMemoryValues_ms   = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalMemoryValuesInMs", autoUpdateIntervalsCurr.m_iMemoryValues_ms ).toInt();
    //    autoUpdateIntervalsCurr.m_iLogicalDisks_ms   = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalLogicalDisksInMs", autoUpdateIntervalsCurr.m_iLogicalDisks_ms ).toInt();
    //    autoUpdateIntervalsCurr.m_iLogicalDisksValues_ms   = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalLogicalDisksValuesInMs", autoUpdateIntervalsCurr.m_iLogicalDisksValues_ms ).toInt();

    //    if( autoUpdateIntervalsCurr.m_iOperatingSystem_ms != m_autoUpdateIntervals.m_iOperatingSystem_ms )
    //    {
    //        setAutoUpdateIntervalOperatingSystemInMs(autoUpdateIntervalsCurr.m_iOperatingSystem_ms);
    //    }
    //    if( autoUpdateIntervalsCurr.m_iComputerSystem_ms != m_autoUpdateIntervals.m_iComputerSystem_ms )
    //    {
    //        setAutoUpdateIntervalComputerSystemInMs(autoUpdateIntervalsCurr.m_iComputerSystem_ms);
    //    }
    //    if( autoUpdateIntervalsCurr.m_iProcessor_ms != m_autoUpdateIntervals.m_iProcessor_ms )
    //    {
    //        setAutoUpdateIntervalProcessorInMs(autoUpdateIntervalsCurr.m_iProcessor_ms);
    //    }
    //    if( autoUpdateIntervalsCurr.m_iProcessorValues_ms != m_autoUpdateIntervals.m_iProcessorValues_ms )
    //    {
    //        setAutoUpdateIntervalProcessorValuesInMs(autoUpdateIntervalsCurr.m_iProcessorValues_ms);
    //    }
    //    if( autoUpdateIntervalsCurr.m_iMemory_ms != m_autoUpdateIntervals.m_iMemory_ms )
    //    {
    //        setAutoUpdateIntervalMemoryInMs(autoUpdateIntervalsCurr.m_iMemory_ms);
    //    }
    //    if( autoUpdateIntervalsCurr.m_iMemoryValues_ms != m_autoUpdateIntervals.m_iMemoryValues_ms )
    //    {
    //        setAutoUpdateIntervalMemoryValuesInMs(autoUpdateIntervalsCurr.m_iMemoryValues_ms);
    //    }
    //    if( autoUpdateIntervalsCurr.m_iLogicalDisks_ms != m_autoUpdateIntervals.m_iLogicalDisks_ms )
    //    {
    //        setAutoUpdateIntervalLogicalDisksInMs(autoUpdateIntervalsCurr.m_iLogicalDisks_ms);
    //    }
    //    if( autoUpdateIntervalsCurr.m_iLogicalDisksValues_ms != m_autoUpdateIntervals.m_iLogicalDisksValues_ms )
    //    {
    //        setAutoUpdateIntervalLogicalDisksValuesInMs(autoUpdateIntervalsCurr.m_iLogicalDisksValues_ms);
    //    }
    //}

} // readAutoUpdateIntervals

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CPCResourceMonitor::onAutoUpdateComputerSystemTimeout()
//------------------------------------------------------------------------------
{
    //if( m_stateAutoUpdateComputerSystem == EStateRunning )
    //{
    //    updateComputerSystem();
    //}
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::onAutoUpdateOperatingSystemTimeout()
//------------------------------------------------------------------------------
{
    //if( m_stateAutoUpdateOperatingSystem == EStateRunning )
    //{
    //    updateOperatingSystem();
    //}
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::onAutoUpdateProcessorTimeout()
//------------------------------------------------------------------------------
{
    //if( m_stateAutoUpdateProcessor == EStateRunning )
    //{
    //    updateProcessor();
    //}
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::onAutoUpdateProcessorValuesTimeout()
//------------------------------------------------------------------------------
{
    //if( m_stateAutoUpdateProcessorValues == EStateRunning )
    //{
    //    updateProcessorValues();
    //}
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::onAutoUpdateMemoryTimeout()
//------------------------------------------------------------------------------
{
    if( m_stateAutoUpdateMemory == EStateRunning )
    {
        updateMemory();
    }
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::onAutoUpdateMemoryValuesTimeout()
//------------------------------------------------------------------------------
{
    if( m_stateAutoUpdateMemoryValues == EStateRunning )
    {
        updateMemoryValues();
    }
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::onAutoUpdateLogicalDisksTimeout()
//------------------------------------------------------------------------------
{
    //if( m_stateAutoUpdateLogicalDisks == EStateRunning )
    //{
    //    updateLogicalDisks();
    //}
}

//------------------------------------------------------------------------------
void CPCResourceMonitor::onAutoUpdateLogicalDisksValuesTimeout()
//------------------------------------------------------------------------------
{
    //if( m_stateAutoUpdateLogicalDisksValues == EStateRunning )
    //{
    //    updateLogicalDisksValues();
    //}
}

/*==============================================================================
public: // instance methods exploring the host
==============================================================================*/

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getManufacturerUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValComputerSystemManufacturerUpdSysTime;
}

//------------------------------------------------------------------------------
QString CPCResourceMonitor::getManufacturer() const
//------------------------------------------------------------------------------
{
    return m_strComputerSystemManufacturer;
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getModelUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValComputerSystemModelUpdSysTime;
}

//------------------------------------------------------------------------------
QString CPCResourceMonitor::getModel() const
//------------------------------------------------------------------------------
{
    return m_strComputerSystemModel;
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getSerialNrUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValComputerSerialNrUpdSysTime;
}

//------------------------------------------------------------------------------
QString CPCResourceMonitor::getSerialNr() const
//------------------------------------------------------------------------------
{
    return m_strComputerSerialNr;
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getDNSHostNameUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValComputerSystemDNSHostNameUpdSysTime;
}

//------------------------------------------------------------------------------
QString CPCResourceMonitor::getDNSHostName() const
//------------------------------------------------------------------------------
{
    return m_strComputerSystemDNSHostName;
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getDomainUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValComputerSystemDomainUpdSysTime;
}

//------------------------------------------------------------------------------
QString CPCResourceMonitor::getDomain() const
//------------------------------------------------------------------------------
{
    return m_strComputerSystemDomain;
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getOSUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValOSCaptionUpdSysTime;
}

//------------------------------------------------------------------------------
QString CPCResourceMonitor::getOS() const
//------------------------------------------------------------------------------
{
    return m_strOSCaption;
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getOSServicePackUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValOSServicePackUpdSysTime;
}

//------------------------------------------------------------------------------
QString CPCResourceMonitor::getOSServicePack() const
//------------------------------------------------------------------------------
{
    return m_strOSServicePack;
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getProcessorsCountUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValComputerSystemNumberOfProcessorsUpdSysTime;
}

//------------------------------------------------------------------------------
unsigned int CPCResourceMonitor::getProcessorsCount() const
//------------------------------------------------------------------------------
{
    return m_uComputerSystemNumberOfProcessors;
}

//------------------------------------------------------------------------------
ZS::PhysVal::CPhysVal CPCResourceMonitor::getLogicalProcessorsCountUpdSysTime() const
//------------------------------------------------------------------------------
{
    return m_physValComputerSystemNumberOfLogicalProcessorsUpdSysTime;
}

//------------------------------------------------------------------------------
unsigned int CPCResourceMonitor::getLogicalProcessorsCount() const
//------------------------------------------------------------------------------
{
    return m_uComputerSystemNumberOfLogicalProcessors;
}

//------------------------------------------------------------------------------
CModelProcessors* CPCResourceMonitor::getModelProcessors()
//------------------------------------------------------------------------------
{
    return m_pModelProcessors;
}

//------------------------------------------------------------------------------
CModelProcessorsValues* CPCResourceMonitor::getModelProcessorsValues()
//------------------------------------------------------------------------------
{
    return m_pModelProcessorsValues;
}

//------------------------------------------------------------------------------
CModelMemory* CPCResourceMonitor::getModelMemory()
//------------------------------------------------------------------------------
{
    return m_pModelMemory;
}

//------------------------------------------------------------------------------
CModelMemoryValues* CPCResourceMonitor::getModelMemoryValues()
//------------------------------------------------------------------------------
{
    return m_pModelMemoryValues;
}

//------------------------------------------------------------------------------
CModelLogicalDisks* CPCResourceMonitor::getModelLogicalDisks()
//------------------------------------------------------------------------------
{
    return m_pModelLogicalDisks;
}

//------------------------------------------------------------------------------
CModelLogicalDisksValues* CPCResourceMonitor::getModelLogicalDisksValues()
//------------------------------------------------------------------------------
{
    return m_pModelLogicalDisksValues;
}
