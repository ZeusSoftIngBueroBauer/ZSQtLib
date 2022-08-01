@page _PAGE_Libs_ZSIpcLog_QuickStart Quick Start

In order to use logging, three main tasks have to be completed.
These three tasks are described in more details in the next chapters.

**Create and Start Log Server**

Note: If you don't want to use online logging via TCP/IP to the log client just use
      CLogServer instead of CIpcLogServer in the following code examples. In addition
      there is no need to startup or shutdown the log server if no TCP/IP connections
      should be used.

Before creating the log server you need to set the organization and application name for
the Qt Application. The log server will use this information to setup a default location
for reading and saving settings.

    QCoreApplication::setOrganizationName("MyOrganization");
    QCoreApplication::setApplicationName("MyAppName");

On windows machines the default absolute file paths for saving settings and the log file is:

    C:/ProgramData/<MyOrganization>/<MyAppName>/ZSLogServer-Loggers.xml
    C:/ProgramData/<MyOrganization>/<MyAppName>/log/ZSLogServer.log

The file paths may be retrieved with:

    CIpcLogServer::GetLoggerFileAbsoluteFilePath()
    CIpcLogServer::GetLocalLogFileAbsoluteFilePath()

If you don't want to use the default file paths you may set the paths as desired
before creating the log server instance using the following methods:

    CIpcLogServer::SetLoggerFileAbsoluteFilePath()
    CIpcLogServer::SetLocalLogFileAbsoluteFilePath()

Either at the beginning of 'main.cpp' or in the constructor in class derived from 'CApplication'
after setting the organization and application name create and start the log server.

    CIpcLogServer* pTrcServer = CIpcLogServer::CreateInstance();
    pLogServer->recallLoggers();
    pLogServer->startup();

Either at the end of 'main.cpp' or in the destructor in class derived from 'CApplication'
stop and destroy the log server.

    CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();
    pLogServer->shutdown();
    pLogServer->saveLoggers();
    CIpcLogServer::ReleaseInstance();
    pLogServer = nullptr;

**Create log entries via Log Server**

Log entries are created by invoking the Log Server's method "log".
As first argument the method expects the log level for which the log
entry should be created. If the passed log level is less than or equal
to the current log level of the log server the entry will be added.

The following log levels exist:

Level| Description
| --- | --- |
0: None | Log outputs are disabled.
1: Fatal | Log fatal errors (critical errors, critical exceptions).
2: Error | At this level all error conditions should be logged.
3: Warning | At the Warning level all events that could potentially<br>
             become an error should be logged.
4: Notice | At this level all events should be logged which are<br>
            considered to be notable but are not an error.
5: Info | The Info level is usually used to log all actions that<br>
          are user-driven or system specific.
6: Debug | The Debug level is usually used to log anything that<br>
           happens in the program for debugging purposes.<br>
           The "normal" debug level may be used to create debug output<br>
           messages which should be as short as possible.<br>
           For very long strings you may want to truncate the string<br>
           with ellipsis in the middle<br>
           (method ZS::System::truncateStringWithEllipsisInTheMiddle).
7: DebugDetailed | Used for "detailed" debug messages.<br>
                   For very long strings though you still may want<br>
                   to truncate the string with ellipsis in the middle<br>
                   but show more details than in the normal debug log level.
8: DebugVerbose | Used for "verbose" debug messages. In this verbose log<br>
                  level you may want to write the whole long string to the log file.

Example:

    // The log server should be used to just show normal info messages.
    pLogServer->setLogLevel(ELogDetailLevel::Info);

    // The following call will not create a log entry:
    pLogServer->log(ELogDetailLevel::Debug, "Debug Message");

    // The following calls will create log entries:
    pLogServer->log(ELogDetailLevel::Info, "Info Message");
    pLogServer->log(ELogDetailLevel::Notice, "Notification Message");

    // The log server should be used to output detailed debug messages.
    pLogServer->setLogLevel(ELogDetailLevel::DebugDetailed);

    QString strLogEntry;
    if( pLogServer->getLogLevel() >= ELogDetailLevel::DebugVerbose ) { // false
        strLogEntry = "Verbose debug message containing many details";
    }
    else if( pLogServer->getLogLevel() >= ELogDetailLevel::DebugDetailed ) { // true
        strLogEntry = "Detailed debug message containing some details";
    }
    else if( pLogServer->getLogLevel() >= ELogDetailLevel::Debug ) {
        strLogEntry = "Debug message";
    }
    pLogServer->log(ELogDetailLevel::Debug, strLogEntry); // Output of Detailed debug message

If you don't want to add additional informations to the log entries that's all you have to do.

If you wish to add additional informations like the date time the log entry has been created,
the name of the current thread from which context the log entry was created or the name space,
class name or object name, you need to create, modify and access loggers.

One logger is always existing which is used inernally as a "root" logger by class
ZS::System::CLogServer to store the settings used if no explicit logger instance has been created.

CLogServer::setLogLevel for example does not store the current log level as a property of
class CLogServer but forwards the setting to the root logger.

**Create log entries via Logger Instances**

The log server itself uses an internally created root logger.
This root logger will store the current log level of the server.
The root logger already allows to format log entries without the
need to create your own logger instances.

If you don't want special formatting of log entries depending on modules, classes, instances of classes
or contexts like "database accesses", "user logins" etc. the root logger is all you need.

Get the root logger by calling "CLogServer::GetLogger" with no arguments.

If you want special formatting create your own logger instances by calling
"CLogServer::GetLogger(MyLoggerName)".

As "MyLoggerName" you should pass a useful string. The node separator "::" may be used
to group the logger instances in the tree of logger instances. This is useful if you
use several logger instances.

Examples:

    CLogger* loggerDbAccess = CLogServer::GetLogger("DatabaseAccesses");
    CLogger* loggerUserLogins = CLogServer::GetLogger("UserEvents::Logins");
    CLogger* myClassLogger = CLogServer::GetLogger("ZS::Ipc::CServer");
    CLogger* myInstanceLogger = CLogServer::GetLogger("ZS::Ipc::CServer::" + objectName());

A logger instance allows you to format the log entries as following.

- Adding the name of the current thread: CLogger::setAddThreadName(bool)
- Add the current date time: CLogger::setAddDateTime(bool)
- Add the current process time of the application: CLogger::addSystemTime(bool)
- Add a name space to the log entry: CLogger::setNameSpace(const QString&)
- Add a class name to the log entry: CLogger::setClassName(const QString&)
- Add an object name to the log entry: CLogger::setObjectName(const QString&)
- Enable or disable logging for the logger: CLogger::setEnabled(EEnabled)
- Set the loggers detail level with: CLogger::setDetailLevel(ELogDetailLevel)
- Set a data filter at the logger with: CLogger::setDataFilter(const QString&)

For bool arguments true activates, false deactivates the feature.
For string arguments a non empty string activates, an empty string deactivates the feature.


**Install Log Client Application**

Create the installer following @ref _PAGE_Main_BuildInstructions.

