How to use the test template project creating a new test:

The following explanation text uses "<TestModule>" as the name of your new test module. Replace it with the name
of your new test module (e.g. "SysLogFile").

- Copy the "Make/Apps/Test/ZSTestTemplate" directory to a new test subdirectory with the name of your new test module.
  E.g. if the name of your new test module would be "SysLogFile":
  "Make/Apps/Test/ZSSysLogFile"
- Delete hidden ".vs" subdirectory (could also be deleted before copying the Template directory).
- Delete "ReadMe.txt" in Target Directory (otherwise the clone of this "ReadMe.txt" file may become part of the git repository).
- Delete "ZSAppTestTemplate_VS2017.vcxproj.user" (will be created later on by studio).
- Rename project files to the new test name:
  "ZSAppTestTemplate.pro" to "ZSAppTestSysLogFile.pro",
  "ZSAppTestTemplate_VS2017.vcxproj" to "ZSAppTestSysLog_VS2017.vcxproj".
  "ZSAppTestTemplate_VS2017.vcxproj.filters" to "ZSAppTestSysLog_VS2017.vcxproj.filters".
- Edit the project files (*.pro; *.vcxproj; *.filters) and replace all occurences of "Template" with the name of your new test module.
  !! Don't replace TEMPLATE = app in the Qt project file !!
- Copy the "Include/Apps/Test/ZSTestTemplate" directory to a new test subdirectory with the name of your new test module.
  E.g. if the name of your new test module would be "SysLogFile":
  "Include/Apps/Test/ZSSysLogFile"
- Rename the file "TSGrpTemplate.h" into "TSGrp*.h". E.g. to "TSGrpSysLogFile.h".
- Copy the "Source/Apps/Test/ZSTestTemplate" directory to a new test subdirectory with the name of your new test module.
  E.g. if the name of your new test module would be "SysLogFile":
  "Source/Apps/Test/ZSSysLogFile"
- Rename the file "TSGrpTemplate.cpp" into "TSGrp*.cpp". E.g. to "TSGrpSysLogFile.cpp".
- If you use visual studio as development environment:
  - Add the new test project to the solution "ZSQtLibs_<VSVersion>.sln" (e.g. to "ZSQtLibs_VS2017").
  - Move the files in the project to the corresponding filters.
  - Replace all occurences of "Template" in all header and source files of the new test module project with "<TestModule>".
  - Build and start the new test module project. You should see a main window with a tree like this:
    + ZS::System::<TestModule>
      + <TestModule>
        + TestStep1
  - Edit the file "TSGrp<TestModule>.cpp" by adding the test steps you need.
    For further information see documentation of the test subsystem of ZSQtLib.
