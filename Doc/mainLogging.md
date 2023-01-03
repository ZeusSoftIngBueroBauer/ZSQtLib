@page _PAGE_Main_Logging Logging

To capture the flow of software execution persistently, the flow of the software execution may be logged
into a file. Reading log files could be very helpful to detect problems during the implementation phase
but also after releasing the software to the customer.

But as a requirement not also the software execution flow but also some important events migth have to
be logged. For example it might be necessary to log who changed what and when.

This is where a logger comes into place.

A logger should have the following features:

- Support different levels of logging
  - Debug
    This level is used to log almost anything that happens in the program. This is mostly used during
    debugging and should be suppressed before entering the production stage. But it should be possible
    to activate them again for troubleshooting.
  - Info
    This level is used to log all actions that are user-driven, or system specific like regularly
    scheduled operations.
  - Warning
    At this level all events that could potentially become an error should be logged.
  - Error
    At this level all error conditions should be logged.
  - CriticalError
    Use this level to log critical errors like unhandled exceptions.
- It should be possible to change the logging level during runtime without the need of restarting the
  application. Changing the logging level should be as intuitive and simple as possible without the
  need of changing entries in hard to read ini files or in the registry.
- Online logging should be possible where the log outputs are not just written to a file but to a
  viewer which immediately shows what is currently being logged.
- It should be possible to define several categories of logging events. For example you may want to
  define categories like "DatabaseAccess", "TCP/IP Events", "UserLogin", etc.
- Ideally a logger should have no effect on performance in the released product.
  In addition it should be possible to filter the log outputs. For performance reasons this filtering
  should already take place within the appliation and not at the debug output console so that logging
  output strings are not created if not needed.

The logging subsystem of the ZSQtLib should help here by providing an online debug output console realised
as the Log Client connected via TCP/IP to a thread save log server created within the master application
to be logged.

NEXT

- @subpage _PAGE_Libs_ZSIpcLog_QuickStart
- @subpage _PAGE_Libs_ZSIpcLog_Overview
- @subpage _PAGE_Libs_ZSIpcLog_Usage_LogServer
- @subpage _PAGE_Libs_ZSIpcLog_Usage_Loggers
- @subpage _PAGE_Libs_ZSIpcLog_DllInterface

