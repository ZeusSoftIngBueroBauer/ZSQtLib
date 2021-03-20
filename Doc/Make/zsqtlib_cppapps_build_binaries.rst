.. _zsqtlib_cppapps_build_setups:

Build Binaries of the ZSQtLib
-----------------------------

The following tools have to be installed as prerequisites:

+------------------------------------+---------------------------------------------------------------------------------+
| :ref:`Appendix-Tools-7-Zip`        | To unzip the Visual Studio 2008 iso image an appropriate Zip program is needed. |
+------------------------------------+---------------------------------------------------------------------------------+
| :ref:`Appendix-Tools-C++ Compiler` | To compile and link the C++ source code into binaries.                          |
+------------------------------------+---------------------------------------------------------------------------------+
| :ref:`Appendix-Tools-Qt Framework` | The ZSQtLib is based on the Qt framework.                                       |
+------------------------------------+---------------------------------------------------------------------------------+
| :ref:`Appendix-Tools-GNU Make`     | Beside the Visual Studio project files also GUI GNU make files are provided.    |
+------------------------------------+---------------------------------------------------------------------------------+

There are two ways to build the dlls and the applications of the ZSQtLib.

- You may build the library and applications using the Visual Studio GUI by opening the solution
  file "ZSQtLibsRemCmd_9.0.sln" (located in "..\ZSQtLib\Make\Windows") with Visual Studio 2008
  (or "ZSQtLibsRemCmd_10.0.sln" with Visual Studio 2010). You must select Release and Win32 to
  build the solution.
- You may also build the library using GNU make from the command line.
  As the make files are using the C++ compiler of Visual Studio 2008 some VS 2008 specific
  environment variables have to be set before the make command may be executed. For this you
  may open the "Visual Studio Command Prompt" via the Windows Start Menu Entry
  "Microsoft Visual C++ 2008 Express Edition - Visual Studio Tools - Visual Studio 2008 Command Prompt".
  To setup the VS 2008 environment variables you may also invoke the batch command "vcvarsall_9.0_x86"
  located in "..\ZSQtLib\Make\Windows" and afterwards "make" the library and applications. Change
  the current working directory to be "C:\...\ZSQtLib\Make" and then invoke::

    C:\...\ZSQtLib\Make>Windows\vcvarsall_9.0_x86
    C:\...\ZSQtLib\Make>make -f ZSQtLib.mk

No matter whether you build with the visual studio solution file or via the make files the generated
binaries will be output to the directory ".\ZSQtLib\Bin\Win32".

.. Note::
   By building via the make files also the Python Bindings will be created. By building via the
   Visual Studio solution "ZSQtLibsRemCmd_9.0.sln" the Bindings will not be created. If you also
   want to create the Bindings using the Visual Studio GUI you have to open the solution
   "ZSQtLibsRemCmdBindings4Python_9.0.sln". But using the solution projects is not the recommended
   way to create the bindings as it was not possible to define rules to only generate and compile
   the bindings if the corresponding source and header files have been changed.
   Using the Visual Studio solution the bindings will alyways be completely rebuild - and this
   takes a while. In addition in order to compile the binding wrapper modules

   - first you have to generate the wrapper modules using shiboken,
   - second you have to enter the generated wrapper modules to the visual studio project
   - and third you have to build the project.

   This means that each time the list of generated wrapper modules changes (because a new class
   have been implemented) it is necessary to manually add or remove the wrapper module from the
   visual studio project. And this is error prone.

.. Note::
   As default only the Release versions are created. If you also need the Debug versions you would
   also need to install (or compile on your own) Debug versions of Python, PySide and other site
   packages. This is a lot of work and is described in a separated section.
