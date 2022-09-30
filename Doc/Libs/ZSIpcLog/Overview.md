@page _PAGE_Libs_ZSIpcLog_Overview Overview

![Overview](ZSIpcLog/Overview.png)

In the Master Application an instance of class `CIpcLogServer` has to be created.
This class aggregates an instance of an Ipc Server to which the log client can connect to.
`CIpcLogServer` is derived from `CLogServer` which again aggregates an instance of class `CLogFile`.

On servers site the `CLogFile` class outputs the log entries to a log file using an instance of
class `CLogFile`. The logger instance may add additional information like the current thread
name, class name and time informations before writing adding an entry line to the log file if
the the logger has been configured correspondingly.

On clients site the `CWdgtLog` class outputs the received log entries to a `QTextEdit` instance.

In contrast to the server-side output in a log file, the output on the client side can be tracked almost
in real time (online tracer). As an additional feature, if the current thread name is include in the
log entries - each thread is shown in his own color.

The logger instances are added to an index tree and can be hierachically organized according to name spaces,
class names and instance names. Logging may be activated, enabled or disabled through the logger instances.

Logging may also be enabled or disabled during runtime for the whole log server or for all logger instances
belonging to a name space. This allows you to start the application without any logging. If you reach the
point of interest you can enable logging.

If the log connects to to the log tracer the tree of logger instances is transmitted as an XML data stream
from the server to the client. The client will indicate the index tree of the logger instances allowing
the user to activate, enable or disable logging for each module, class or instance.

In order to clarify the facts described, you can take a closer look at the code of the test application
`ZSAppTestIpcLog`. Within this test not only an instance of `CIpcLogServer` with logger instances for
filtering modules, classes and instances to be logged is created, but also a complete log client
is created to test the communication between client and server.

In order to use logging, three main tasks have to be completed which are

- creating and starting the log server,
- creating logger instances and
- use a logger to create log entries.

These three tasks are described in more details in the next chapters.

