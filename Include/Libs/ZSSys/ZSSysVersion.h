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

#ifndef ZSSys_Version_h
#define ZSSys_Version_h

#include <QtCore/qstring.h>

namespace ZS
{
namespace System
{
/*==============================================================================

Version Number Format:
----------------------

<MajorVersion>.<MinorVersion>.<ReleaseId>

<MajorVersion> .. Major version number of the ZSQtLib. This version number
                  will only be changed on radical changes concerning the whole
                  library. On changing the QtMajorVersion the ZSQtLibMajorVersion
                  will not be changed.

<MinorVersion> .. Minor version number of the ZSQtLib. This version number
                  will be changed if significant changes have been made within
                  already existing and released sub systems of the ZSQtLib
                  concerning the architecture, class design or interfaces.
                  Will be reset to 1 if the ZSQtLibMajorVersion will be
                  incremented or reset.

<ReleaseId>       Will be incremented each time a new version of the library
                  will be released after bugfixes or improvements.
                  Will be reset to 1 if the FeatureId will be incremented
                  or reset.

The version number of the Qt framework the sources have been compiled and tested
with should be added to the version history.


Version History:
----------------

4.1.4.1   First release of the SMSysDll containing the trace modules.
--------------------------------------------------------------------------------
4.1.5.1   Added the Dll with physical units, physical sizes and physical values.
          Separated trace modules in separated Dll.
--------------------------------------------------------------------------------
4.1.6.1   Renamed Network Dll into Ipc Dll. Added functionality to communicate
          with other processes via shared memory.
--------------------------------------------------------------------------------
4.1.6.2   Corrected assignment of port numbers to shared memory sockets.
--------------------------------------------------------------------------------
4.1.6.3   Added "addLogItem" for IPC connection widget.
--------------------------------------------------------------------------------
4.2.1     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Revised Version Number Format:
            As Linux only supports three digits for versioning shared objects the
            digits for the version numbers have been reduced to three digits and
            the <QtMajorVersionNumber> has been removed. To avoid duplicate version
            numbers the new range of version numbers starts from 4.2.1 (but not
            from 1.6.3).
--------------------------------------------------------------------------------
4.2.2     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Added Qt-Dll directories to include directory path.
          Added error log class.
--------------------------------------------------------------------------------
4.2.3     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Added Qt-Dll directories to include directory path.
          Limited number of entries in log edit widgets (trace and IPC connections).
          Trace server writes local trace file directly (not via local trace client).
--------------------------------------------------------------------------------
4.2.4     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Using global error log and trace files.
--------------------------------------------------------------------------------
4.2.5     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Error manager integrated.
          Introduced SNMP-Explorer and SNMP-Standard-PC-Client.
--------------------------------------------------------------------------------
4.2.6     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Error manager not as singleton class.
          Global dialogs not as singleton classes.
--------------------------------------------------------------------------------
4.2.7     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Improvements and bug fixes in SNMP modules.
          IPC Connection watch dog timer.
--------------------------------------------------------------------------------
4.2.8     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
--------------------------------------------------------------------------------
4.2.9     Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          GCC compiler warnings.
--------------------------------------------------------------------------------
4.2.10    Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Introduced "DetailLevel" for tracing and distinguishing between class
          and instance tracing.
--------------------------------------------------------------------------------
4.2.11    Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Minor bug fixes.
--------------------------------------------------------------------------------
4.2.12    Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2
          Minor bug fixes (deleting rows from ErrLogModel).
--------------------------------------------------------------------------------
4.2.13    Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2, 4.7.0
          Minor bug fixes (File- and LineCount of TraceServers LogFile).
--------------------------------------------------------------------------------
4.2.14    Compiled and tested with Qt versions 4.2.3, 4.5.1, 4.5.2, 4.7.0
          Minor bug fixes (call previously installed qt message handler).
--------------------------------------------------------------------------------
4.2.15    Compiled and tested with Qt versions 4.7.0 (Windows), 4.7.1 (Linux Debian Lenny)
          Porting issues for Qt 4.7.1 and Linux Debian Lenny
--------------------------------------------------------------------------------
4.2.16    Compiled and tested with Qt versions 4.7.0 (Windows), 4.7.1 (Linux Debian Lenny)
          Object pool model class revised.
--------------------------------------------------------------------------------
4.2.17    Compiled and tested with Qt versions 4.7.0 (Windows), 4.7.1 (Linux Debian Lenny)
          Bug fixes for trace method client activating admin objects recursively.
--------------------------------------------------------------------------------
4.2.18    Compiled and tested with Qt versions 4.7.3 (Windows), 4.7.1 (Linux Debian Lenny)
          Added package "ZSDraw" with application "ZSAppDraw".
--------------------------------------------------------------------------------
4.2.19    Compiled and tested with Qt versions 4.7.3 (Windows), 4.7.1 (Linux Debian Lenny)
          Trace admin objects emits signal "changed".
          Added dll interface  module TrcDllIf to dynamically load the trace dll.
--------------------------------------------------------------------------------
4.2.20    Compiled and tested with Qt versions 4.7.0 and 4.7.4 (Windows and Linux)
--------------------------------------------------------------------------------
4.2.21    Compiled and tested with Qt versions 4.7.0 and 4.7.4 (Windows and Linux)
          Minor changes and bugfixes.
          Allowing to specify "ActivatedAsDefault" flag for trace admin objects.
          Allowing to transfer default trace and host settings to trace server.
--------------------------------------------------------------------------------
4.3.1     Compiled and tested with Qt versions 4.7.0 and 4.7.4 (Windows and Linux)
          Minor changes and bugfixes.
          ErrResultInfo in signals/slots not const to allow setting the flag AddedToErrLogFile.
          Using namespace ZS::Ipc in signature of connected/disconnected signals of IpcClient and IpcServer.
          ZS::Data namespace with data set, data tables, data columns, data rows
          and data provider classes.
--------------------------------------------------------------------------------
4.4.1     Compiled and tested with Qt versions 4.8.5
          Using CRequest for all asynchronous request methods.
          Separated sub systems into Dlls without GUI elements and Dlls providing
          GUI elements.
--------------------------------------------------------------------------------
4.5.1     Compiled and tested with Qt versions 4.8.5
          ZSDraw using Qt graphics view framework.
--------------------------------------------------------------------------------
4.6.1     Compiled and tested with Qt versions 4.8.5
          Remote command sub system revised and improved.
--------------------------------------------------------------------------------
4.6.2     Use ClientHostSettings instead of SocketDscr to setup Ipc client connection.
--------------------------------------------------------------------------------
4.6.3     Compiled and tested with Qt versions 4.8.7
          Use WaitCondition to indicate that thread.run method has been started.
          WaitCondition for start thread with valid timeouts.
          Message Id counters protected by mutex.
          Synchronizing with start and running threads using startup confirmation messages.
          Aborting IpcClient and IpcServer connect and startup requests.
          Protect admin objects methods with mutex. Send trace admin object node info
          before trace method data (signal/slot with DirectConnection).
          Request Queue and Request Sequencer as children of parent objects to be included
          in moveToThread calls.
          ErroResultDialog added.
          IpcClient with virtual methods to create own implementation of gateway threads.
          Don't add "ConnectionRefused" result into err log model.
          QThread::wait() with valid timeouts.
          No request continue timer for request sequencer.
          Method to return detail level of admin object.
==============================================================================*/

#include "ZSSys/ZSSysDllMain.h"

#define _ZSSYS_STRINGIFY(x) #x
#define _ZSSYS_TOSTRING(x) _ZSSYS_STRINGIFY(x)

#define ZSLIB_VERSION_MAJOR QT_VERSION_MAJOR
#define ZSLIB_VERSION_MINOR 6
#define ZSLIB_VERSION_PATCH 3

#define ZSVersionNr "4.6.3"

// Please note !!!
// The version number definition is read by the build process to update the
// version number in resource files, setup projects and so on.
// !! Therefore: don't change the format of the version number definition !!
//#ifdef _WINDOWS
//#define ZSVersionNr _ZSSYS_TOSTRING(ZSLIB_VERSION_MAJOR) "." _ZSSYS_TOSTRING(ZSLIB_VERSION_MINOR) "." _ZSSYS_TOSTRING(ZSLIB_VERSION_PATCH)
//#else
//#endif

const QString c_strSysVersionNr(ZSVersionNr);

const QString c_strSysVersionNrDateTime = c_strSysVersionNr + ": " + __DATE__ + " " + __TIME__;

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Version_h
