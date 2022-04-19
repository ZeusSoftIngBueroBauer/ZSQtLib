@page _PAGE_Main_MethodTracing Method Tracing / Logging

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

Using the trace subsystem of the ZSQtLib helps you in different phases of coding:

- During the implementation phase by investigating the trace output.<br/>
  Are the methods called as intended?
  Optimizing the code by detecting unnecessary calls?
  Are there unneccessary recursions?
  Detect concurrency problems in multithread applications.
- Finding bugs in already implemented classes.
- Getting to know insufficiently documented source code.
- Getting an overview of the system by evaluation the tree of trace admin objects.

NEXT

- @subpage _PAGE_Libs_ZSIpcTrace_QuickStart
- @subpage _PAGE_Libs_ZSIpcTrace_Overview
- @subpage _PAGE_Libs_ZSIpcTrace_Usage_TrcServer
- @subpage _PAGE_Libs_ZSIpcTrace_Usage_TrcAdminObjs
- @subpage _PAGE_Libs_ZSIpcTrace_Usage_MethodTracer
- @subpage _PAGE_Libs_ZSIpcTrace_UseCase_ReadingTraceOutput
- @subpage _PAGE_Libs_ZSIpcTrace_DllInterface

