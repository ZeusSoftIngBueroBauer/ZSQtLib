@page _PAGE_Libs_ZSIpcTrace_Overview Overview

![Overview](ZSIpcTrace/Overview.png)

In the Master Application an instance of class `CIpcTrcServer` has to be created.
This class aggregates an instance of an Ipc Server to which the trace methdod client can connect to.
`CIpcTrcServer` is derived from `CTrcServer` which again aggregates an instance of class `CTrcMthFile`.

On servers site the `CTrcMthFile` class outputs the method trace to a log file using an instance of
class `CLogFile`. `CTrcMthFile` formats the method trace by indenting the method calls according to the
call stack for each thread in which they are processed and adds additional information like thread name
and time informations before writing adding an entry line to the log file.

On clients site the `CWdgtTrcMthList` class outputs the method trace client to a `QTextEdit` instance.
`CWdgtTrcMthList` formats the method trace by indenting the method calls according to the
call stack for each thread in which they are processed and adds additional information like thread name
and time informations before writing adding an entry line to the log file.

In contrast to the server-side output in a log file, the output on the client side can be tracked almost
in real time (online tracer). As an additional feature, each thread is shown in his own color, which greatly
simplifies the assignment of method calls to their thread call stack and significantly improves readability.

Filtering of modules, classes, instances of classes is realized through instance of class `CTrcAdminObj`.
The trace admin objects are added to an index tree and are hierachically organized according to name spaces,
class names and instance names. Logging may be activated, enabled or disabled through those trace admin objects.

Logging may also be enabled or disabled during runtime for the whole trace server or for all trace admin objects
belonging to a name space. This allows you to start the application without any tracing. If you reach the
point of interest you can enable tracing.

If the trace method client connects to to the trace tracer the tree of trace admin objects is queried by the
client and sent as an XML data stream from the server to the client. The client will indicate the index tree
of trace admin objects allowing the user to activate, enable or disable logging for each module, class or
instance.

In order to clarify the facts described, you can take a closer look at the code of the test application
`ZSAppTestIpcTrace`. Within this test not only an instance of `CIpcTrcServer` with trace admin objects for
filtering modules, classes and instances to be traced is created, but also a complete trace method client
is created to test the communication between client and server.

In order to use method tracing, three main tasks have to be completed which are

- creating and starting the trace server,
- creating trace admin objects and
- use a method trace to output the trace messages.

These three tasks are described in more details in the next chapters.

