@page _PAGE_Libs_ZSIpcLog_Usage_Loggers Creating Logger Instances

Logger instances control what is logged and how.

A logger instance can be associated with a context (database accesses, user logings, etc.),
one or more modules, one or more classes, or one or all instances of a class.

Usually you want to control log outputs for a certain context and if logging
for the context is enabled, how detailed these outputs should be.

To do this, a logger instance must be created and registered with the Log Server.
The logger instance can be globally accessed throughout all source modules by
accessing it by a unique name.

In order not to lose the overview if many logger instances are created during runtime you may
logically group the instances by their names. E.g. you may name an instance
'MainPath::SubPath::ObjectName' (e.g. 'Database::Table:Customers').

If you use a different separator for the object path (e.g. "/") you need to replace
this separator by the separator used by the loggers index tree ("::").

To get a logger instance simply call the method CLogServer::GetLogger() by passing
a descriptive name. If a logger with the name is already existing the reference to this
logger instance is returned. If a logger with th ename is not yet existing a logger with
the given name is created.

    CLogger* pLoggerDb = CLogServer::GetLogger("DatabaseAccesses");
    CLogger* pLoggerUE = CLogServer::GetLogger("UserEvents::Logins");
    CLogger* pLoggerIpcSrv = CLogServer::GetLogger("ZS::Ipc::CServer");
    CLogger* pLoggerSrvInst1 = CLogServer::GetLogger("ZS::Ipc::CServer::Inst1");


**Summary**

If you have created all trace admin objects as described in the examples above, you will get a tree with
the following structure.

