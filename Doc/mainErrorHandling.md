@page _PAGE_Main_ErrorHandling Error Handling

Description of an error
=======================

Error numbers are defined by the enum ZS :: System :: EResult.
This error number can be converted into a readable string using the "result2Str" method.

How to react to an error code depends on various circumstances. For this reason, an error code
is usually accompanied by a weighting that is defined via the enum ZS :: System :: EResultSeverity.

On the one hand, the error code can have a purely informative character and can only say that the
attempted operation is currently not possible. Calling "canAdd" and found that the object already
exists in the list, the result code "ObjAlreadyInList" has an informative character.

On the other hand, the fault code can also be understood as a warning to inform the user that the
operation is changing something that he or she may not have wanted. Calling "add" and find that
the object is already in the list the result code "ObjAlreadyInList" can be issued to the user
as a warning and inform him that an already existing object would be overwritten.

An error code can mean a real error and indicate that an operation could not be performed because
e.g. certain conditions are not met. Calling "add" and find that the object is already in the list
the operation can also simply be rejected with the error message "ObjAlreadyInList".

An error code can also mean a serious error if something completely unexpected has happened
indicating a programming error that can cause the program to crash. If the "add" method was called
internally and it should not happen that one and the same object already appears in the list, this
would have to be treated as a critical error and an exception might be thrown.

It is helpful for locating program errors if, in addition to the error codes and their weighting,
the cause of the error is included. So to speak, the source of the error, the module, namespace,
class, instance name and method name and or also file name and line number in which the error was
recognized. Further information such as call parameters or the output of status variables can also
be helpful.

In order to be able to save all this information, the structure ZS :: System :: SErrResultInfo was
introduced, in which the error code, weighting of the error, the source of the error and additional
information can be stored.

Logging Errors
==============

Once filled with the appropriate values, the structure SErrResultInfo can be stored resistant in an
error log instance, which stores the error in a log file as an XML data structure. The contents of
the error log instance can be displayed in a table using a pre-made GUI control, the errors can be
examined and also removed from the error log instance.

So that the error log file does not overflow if the same error occurs again and again, a counter is
counted for each error log entry. If the same occurs again, a new entry is not generated, only the
counter is incremented.

A completed ErrResultInfo structure can also be passed to the constructor of the ZS::System::CException
class. The CException structure automatically adds the error to the error log. If this is not desired,
the "Add2ErrLog" flag, which is to be passed as an argument to the CException structure, can be set to false.

Below is an example of how the Error Log can be used.

First of all, it is necessary to determine the storage location and the file name for the error log file.
The method ZS::System::getAppLogDir returns the default path in which log files should normally be stored.
The file name can be the name of the application extended by "-ErrLog". The file type is xml. Then you come
up with a meaningful name for the error log instance and can create it with the static method CreateInstance.
The first transfer parameter of type bool specifies whether all Qt error messages should be caught and
stored in the log file.

Normally one will call CErrLog::CreateInstance when constructing the application, i.e. immediately before
starting the application.

@code
QString strAppName = QCoreApplication::applicationName();

QString strIniFileScope = "System";

QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);

QString strErrLogFileBaseName = strAppNameNormalized + "-ErrLog.";
QString strErrLogFileSuffix = "xml";

QString strErrLogFileAbsFilePath = strAppLogDir + "/" + strErrLogFileBaseName + "." + strErrLogFileSuffix;

CErrLog::CreateInstance(true, strErrLogFileAbsFilePath, "MyErrLogFile");
@endcode

If only one error log instance is required in the system and is satisfied with the default settings,
calling CreateInstance without transfer parameters is sufficient.

@code
CErrLog::CreateInstance();
@endcode

The following default settings are then used:

- store Qt error messages in the log file,
- File Path: "<ProgramData> / <OrganizationName> / <ApplicationName> / Log",
- File Name: "<ApplicationName> -ErrLog.xml" and
- Object Name: "ZS :: System :: ErrLog"

The ErrLog instance must be released again when the application is closed. On the one hand, so that all
error messages are saved in the ErrLog file, and on the other hand, of course, because you should release
all generated instances during a clean shutdown, so that no MemoryLeaks are left behind. If DestroyInstance
is called without an ErrLog name, the name "ZSErrLog" is used as the default. If you have used
your own name, this must be passed as an argument to DestroyInstance.

@code
CErrLog::DestroyInstance();
@endcode

You can access the ErrLog instance from anywhere in the program using the static method GetInstance.
The object name of the ErrLog file must be transferred. If GetInstance is called without an argument,
the default object name "ZSErrLog" is used.

Error messages are added at runtime of the program with the call addEntry, as shown in the following example.

@code
if( CErrLog::GetInstance() != nullptr )
{
    SErrResultInfo errResultInfo(
        /* errSource  */ "ZS::System", "CTest", "ErrLogTest", "testMethod",
        /* result     */ EResultListIsFull,
        /* severity   */ EResultSeverityError,
        /* strAddInfo */ "Maximum number of entries for reached." );
    CErrLog::GetInstance()->addEntry(errResultInfo);
}
@endcode

With the GUI Control ZS::System::GUI::CWdgtErrLog, which contains the TableView ZS::System::GUI::CTableViewErrLog
and buttons for deleting and resizing the table, the content of the error log instance can be displayed on the screen.
In addition, the MainWindow can also connect with the entryAdded signal of the CErrLog class, evaluate the content of
the ErrLog instance and output an icon in the status line, as shown in the following figure.

![ErrLogTableView](Libs\ZSSys\ErrLogWdgt.bmp)

