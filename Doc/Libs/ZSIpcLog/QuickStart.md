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

    CIpcLogServer* pLogServer = CIpcLogServer::CreateInstance();
    pLogServer->recallLoggers();
    pLogServer->startup();

Either at the end of 'main.cpp' or in the destructor in class derived from 'CApplication'
stop and destroy the log server.

    CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();
    pLogServer->shutdown();
    pLogServer->saveLoggers();
    CIpcLogServer::ReleaseInstance();
    pLogServer = nullptr;

**Create log entries via Logger Instances**

Log entries are created using a logger instance.

To get a logger instance you need to invoke the log server's method 'getLogger'.

    CLogger* pLogger = pLogServer->getLogger("MyLoggerName");

As the log server is a singleton class a logger may also be retrieved using the
class method 'GetLogger'.

    CLogger* pLogger = CLogServer::GetLogger("MyLoggerName");

Log entries are created by invoking the Logger's method "log".
As first argument the method expects the log level for which the log
entry should be created. If the passed log level is less than or equal
to the current log level of the logger the entry will be added.

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
    pLogger->setLogLevel(ELogDetailLevel::Info);

    // The following call will not create a log entry:
    pLogger->log(ELogDetailLevel::Debug, "Debug Message");

    // The following calls will create log entries:
    pLogger->log(ELogDetailLevel::Info, "Info Message");
    pLogger->log(ELogDetailLevel::Notice, "Notification Message");

    // The log server should be used to output detailed debug messages.
    pLogger->setLogLevel(ELogDetailLevel::DebugDetailed);

    QString strLogEntry;
    if( pLogger->getLogLevel() >= ELogDetailLevel::DebugVerbose ) { // false
        strLogEntry = "Verbose debug message containing many details";
    }
    else if( pLogger->getLogLevel() >= ELogDetailLevel::DebugDetailed ) { // true
        strLogEntry = "Detailed debug message containing some details";
    }
    else if( pLogger->getLogLevel() >= ELogDetailLevel::Debug ) {
        strLogEntry = "Debug message";
    }
    pLogger->log(ELogDetailLevel::Debug, strLogEntry); // Output of Detailed debug message

The log server itself uses an internally created root logger which will be created
when instantiating the log server. To get a reference to this root logger call
'getLogger' and 'GetLogger' without passing a name.

If you don't want special formatting of log entries depending on modules, classes, instances
of classes or contexts like "database accesses", "user logins" etc. the root logger is all you need.

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

