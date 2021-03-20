.. _zsqtlib_cppapps_build_binaries:

Build Setups of the ZSQtLib Applications
----------------------------------------

The following tools have to be installed as prerequisites:

+------------------------------------+--+
| :ref:`Appendix-Tools-Inno Setup`   |  |
+------------------------------------+--+

After you have build the ZSQtLib binaries you may create setups for the applications of the
ZSQtLib. This time the only application distributed by the ZSQtLib is the method trace client.

To build the setup for the method trace client the Inno Setup Script "ZSAppTrcMthClient.iss"
located in directory ".\ZSQtLib\Apps\Products\ZSTrcMthClient\Make\Windows" has been implemented.
You may open this setup script using the Inno Setup Ide and compile the script.
You may also pass the script file on the command line to the ISCC (Inno Setup Command Line Compiler)
as follows::

    C:\...>cd .\ZSQtLib\Apps\Products\ZSTrcMthClient\Make\Windows
    C:\...\ZSQtLib\Apps\Products\ZSTrcMthClient\Make\Windows>ISCC ZSAppTrcMthClient.iss

Either way a the setup executable "ZSAppTrcMthClientSetup.exe" will be created in directory
".\ZSQtLib\Setup\Win32\Apps\Products\ZSTrcMthClient".
