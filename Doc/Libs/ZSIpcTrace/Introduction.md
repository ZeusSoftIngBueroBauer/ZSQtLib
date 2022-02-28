@page _PAGE_Libs_ZSIpcTrace_Introduction Introduction

Since debugging code is often not sufficient to detect errors in a program or is not possible at all because
the error only occurs in the release version of the software, for example, log outputs are usually used.

When troubleshooting concurrency problems in multi-threaded applications, reading log output is often the
only way to track down the cause of the error.

Logging can also to be used to uncover weak points and to optimize the runtime of the application.

These log outputs are often output via "printf" instructions either on the screen or in a file.
However, especially in embedded software development, these "printf" outputs are not sufficient if,
for example, the devices do not have a screen or a suitable storage medium. As a rule, the devices
only have a USB, parallel, serial, TCP/IP or another type of interface.

In addition, there is often a lack of the ability to specify what exactly should be loogged.
Debug output consoles often offer the option of using a filter so that the log output is not only filled
with information that is irrelevant for the current troubleshooting and you can no longer see the forest
because there ar so many trees.

However, this type of filtering is usually implemented in an inconvenient manner and is often anything
but intuitive to use.

An additional weak point with applying the filter to the debug output console is that the generation of
unnecessary and irrelevant debug outputs is not prevented if the filter is only applied afterwards and
processor time is unnecessary wasted to generate the debug outputs.

It is better to apply the filter before generating the debug outputs.

The trace subsystem of the ZSQtLib should help here by providing an online debug output console realised
as the Trace Method Client connected via TCP/IP to a trace server created within the master application
to be logged and whose methods should be traced.

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

If the trace method client connects to to the trace tracer the tree of trace admin objects is queried by the
client and sent as an XML data stream from the server to the client. The client will indicate the index tree
of trace admin objects allowing the user to activate, enable or disable logging for each module, class or
instance.

In order to clarify the facts described, you can take a closer look at the code of the test application
`ZSAppTestIpcTrace`. Within this test not only an instance of `CIpcTrcServer` with trace admin objects for
filtering modules, classes and instances to be traced is created, but also a complete trace method client
is created to test the communication between client and server.

@subpage _PAGE_Libs_ZSIpcTrace_Usage_TrcServer

@subpage _PAGE_Libs_ZSIpcTrace_Usage_TrcAdminObjs

@subpage _PAGE_Libs_ZSIpcTrace_Usage_MethodTracer

