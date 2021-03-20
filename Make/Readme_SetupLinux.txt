1. Setup Linux Machine
**********************

1.1. Oracle VM VirtualBox
=========================

Create a virtual machine for Ubuntu with at least 16 GByte disk space.
Please use the newest version of Virtual Box as there might be some
incompatibilities between Virtual Box and Linux Versions.

1.2. Install Operating System (Red Hat Linux 7.4 64 Bit)
========================================================

After starting the virtual machine for the first time select folder containing
the linux installation package "rhel-server-7.4-x86_64-dvd.iso".

Please see https://developers.redhat.com/products/rhel/hello-world in order to setup
the Red Hat Linux server version (package: rhel-server-7.4-x86_64-dvd.iso).

Install the latest updates and enable additional software repositories using
the subscription manager as described.

As superuser download and install any available updates by running yum update:

- yum -y update

and reboot the system:

- reboot

Enable additional software repositories

In order to install VirtualBoxGuestAdditions:

- Install gcc, g++, kernel-devel and bzip2 as follows:
  yum install gcc kernel-devel bzip2
  yum install gcc-c++
- Mount the VirtualBox Guest Additions ISO (if not yet done)
- Run the installer: sh VBoxLinuxAdditions.run
- Unmount
- Restart Linux

1.3. Installing Qt for Linux
============================

Download Qt Online Installer for Linux from "https://www.qt.io/download-qt-installer

Make the downloaded "qt-unified-linux-x64-3.0.2-online.run" file an executable file for linux
and start the run file by double click.

1.3. Adaptions to VM and Linux
==============================

1.3.1. Shared Folders
---------------------

In order to use shared folders the Guest Additions must have been succesfully
installed. After creating the shared folder it might be necessary to restart
linux (icon on the top right corner). If creating a shared folder is successfull
this folder must be available from linux below "/media/sf_<ShareName>".

To grant access to the shared directory execute "sudo adduser <username> vboxsf"
to add you as a user to the vboxsf group. Please note that you may need to enter
the user name in small letters. After changing the group you need to logout and
login to linux.

Red Hat Linux:

You can access the share by making the user, or group id of 1000, a member of
group vboxsf. This is done by changing the vboxsf line in the /etc/group file
(e.g. change vboxsf:x:490: to vboxsf:x:1000:). May require reboot.


1.3.2. Time Synchronisation between virtual box host and guest machine
----------------------------------------------------------------------

If you run linux in a virtual box the time synchronization between the host and
guest OS may lack accuracy (drifting away) ending up in a make file warning
"Clock skew detected" or in a message saying that the make file has a time stamp
from the future.
To solve the time drift you may execute "/usr/sbin/VBoxService --timesync-set-start"
before running make from command prompt.
Create and start shell script with following content:
  while:
  do
  /usr/sbin/VBoxService --timesync-set-start
  sleep 60
  done

Or see https://vilimpoc.org/blog/2014/02/09/better-time-synchronization-with-virtualbox-guest-additions/.

2. Installing Qt for Linux
**************************

The way Qt has to be installed depend on the used Linux version.

2.1. Linux Mint Cinnamon
========================

For Linux Cinnamon its easy to install Qt. You just have to start the Sofware Manager
below the System Administration menu and below the Programming category you can
install the Qt-sdk.

If you

2.2. Linux Mint KDE
===================

To install Qt in this Linux version is more complicated.

2.2. Prerequisites
==================

2.2.1. Installing ZLib Linux
-----------------------------

- sudo apt-get install zlibc zliblg zliblg-dev

2.2.2 Qt
--------

2.2.1. Via Archive with Qt Sources
''''''''''''''''''''''''''''''''''

- Download and unpack the archive:

Download "qt-everywhere-opensource-src-4.8.6.tar.gz" from
https://download.qt.io/archive/qt/4.8/4.8.6/.

In linux terminal unpack the archive

    gunzip qt-everywhere-opensource-src-4.8.6.tar.gz
    tar xf qt-everywhere-opensource-src-4.8.6.tar

- Building the Library:

To configure the Qt library for your machine type, run the ./configure script in the package directory.
By default, Qt is configured for installation in the /usr/local/Trolltech/Qt-%VERSION% directory.

!!!! Problem with the configure script in the shared directory !!!

The configure script wants to create symbolic links. In the shared directory this might not be possible.
If the symbolic link cannot be created not an error but only a warning is issued on starting the
configure script. Anyway on compiling Qt from the shared directory was not possible and
allways zlip link errors have been reported. Neither adding the configure option -qt-zlib to use
the zlib bundled with Qt nore -no-webkit to exclude the WebKit solved the problem.
But after copying the opensource package to "/usr/local/Trolltech", opening the terminal with
the package directory as the working directory configuring and making Qt was successfull.

To create the library and compile all the demos, examples, tools, and tutorials, type "make".
Once everything is built, you mut run "make install".
Qt will be installed into /usr/local/Trolltech/Qt-4.8.7.
To reconfigure, run "make confclean" and "configure" again.

- Set QTDIR environment variable:

Create shell script "qt.sh" in "/etc/profile.d" with following content:

    #!/bin/sh
    # Environment variables for the Qt package:
    if [ -d /usr/lib/qt3 ]; then
      QTDIR=/usr/lib/qt3
    else
      exit
    fi
    if [ ! "$CPLUS_INCLUDE_PATH" = "" ]; then
      CPLUS_INCLUDE_PATH=$QTDIR/include:$CPLUS_INCLUDE_PATH
    else
      CPLUS_INCLUDE_PATH=$QTDIR/include
    fi
    MANPATH="$MANPATH:$QTDIR/doc/man"
    PATH="$PATH:$QTDIR/bin"
    export QTDIR
    export CPLUS_INCLUDE_PATH
    export MANPATH

In order for the environment variable to be exported and set you need to logout and login.
To logout type "su -".

- Compile Qt:

To compile Qt (and other libraries or executables) you may need to login as root in order to get
sufficient rights to read the content of the Qt include directories.
To login as root type "su -" in the terminal.

2.2.3 Qt-Creator
----------------

Download installer "qt-creator-opensource-linux-x86_64-4.0.0.run" from
https://www.heise.de/download/product/qt-creator-63227/download.

or "wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run"

Adjust permission:
    chmod +x qt-opensource-linux-x64-5.7.0.run
run the installer:
    ./qt-opensource-linux-x64-5.7.0.run
and follow the instruction to complete the installation.

3. Build and Run LTT and ZSQtLib Packages
*****************************************

3.1. Setup
==========

3.1.1. Environment Variables
----------------------------

3.1.1.1. ZSQTLIB_DIR
''''''''''''''''''''

Assuming that a virtual machine is used with a shared directory mapped to "/media/sf_LTT" you
need to create a shell script "zsqtlib.sh" in "/etc/profile.d" with following content:

    #!/bin/sh
    # Environment variables for the ZSQtLib package:
    if [ -d /media/sf_LTT/ZSQtLib ]; then
      ZSQTLIB_DIR=/media/sf_LTT/ZSQtLib
    else
      exit
    fi
    if [ ! "$CPLUS_INCLUDE_PATH" = "" ]; then
      CPLUS_INCLUDE_PATH=$ZSQTLIB_DIR/Include:$CPLUS_INCLUDE_PATH
    else
      CPLUS_INCLUDE_PATH=$ZSQTLIB_DIR/Include
    fi
    PATH="$PATH:$ZSQTLIB_DIR/Bin/Linux"
    export ZSQTLIB_DIR
    export CPLUS_INCLUDE_PATH

3.1.1.2. LTTBASELIB_DIR
'''''''''''''''''''''''

Assuming that a virtual machine is used with a shared directory mapped to "/media/sf_LTT" you
need to create a shell script "lttbaselib.sh" in "/etc/profile.d" with following content:

    #!/bin/sh
    # Environment variables for the LTTBaseLib package:
    if [ -d /media/sf_LTT/ltt_baselib_c ]; then
      LTTBASELIB_DIR=/media/sf_LTT/ltt_baselib_c
    else
      exit
    fi
    if [ ! "$CPLUS_INCLUDE_PATH" = "" ]; then
      CPLUS_INCLUDE_PATH=$LTTBASELIB_DIR/Include:$CPLUS_INCLUDE_PATH
    else
      CPLUS_INCLUDE_PATH=$LTTBASELIB_DIR/Include
    fi
    PATH="$PATH:$LTTBASELIB_DIR/Bin/Linux"
    export LTTBASELIB_DIR
    export CPLUS_INCLUDE_PATH

3.1.2. Build Settings for Qt-Creator
====================================

Build directories for the projects:

    /tmp/DiaSemi/LTT/Int/<ProjectName>/Linux/<Debug|Release>

    ProjectName [ZSSys, ZSSysGUI, .., LTTBase, LTTBaseGUI, LTTTargetServerDemoGUI]

Don't use the shared folder of the virtual box as in that case the libs may be corrupted and linux
can't read them (I don't know why but on linking those libs g++ indicated the error
"unknown file system, cannot open libZS.., libLTT.. and so on).

Instead compile everything e.g. into "/tmp/DiaSemi/LTT" and output the libraries to "/usr/local/DiaSemi".
But in order to run the application you need to "install" the libs e.g. to "usr/lib".


4. FAQs
*******

4.1. Compilation and Link Errors
================================

4.1.1. "failed to create symbolic link: read only file system virtualbox"
-------------------------------------------------------------------------

same as "failed to create symbolic link: protocol error"

4.1.2. "failed to create symbolic link: protocol error"
-------------------------------------------------------------------------

For linux machines symbolic links need to be created for the shared libraries.
E.g. for "libZSSys.so.4.6.3" the symbolic links "libZSSys.so", "libZSSys.so.4"
and "libZSSys.so.4.6" need to be created. You may not be able to create those
symbolic links ang get the error message above if running linux as a virtual
machine on a windows host and trying to create the links in a shared folder.

To solve this error:

1. Run the Local Security Policy (Win+R, type "secpol.msc", confirm UAC).
   Then navigate to "Local Policies->User Rights Assignment".

2. Find the permission called "Create symbolic links" and double-click it.
   Add your user to the list of objects having this permission.
   Or you can add the "Users" group. Log off and log on. You can check the
   success by running in console: "whoami /priv"
   If you see the privilege SeCreateSymbolicLinkPrivilege enabled, you've done well.

3. Make sure your user has proper access to the shared folder on host system.

4. Execute "VBoxManage setextradata VM_NAME VBoxInternal2/SharedFoldersEnableSymlinksCreate/SHARE_NAME 1"
   in a console from the C:\Program Files\VirtualBox folder.
   Don't forget to substitute VM_NAME with the name of your VM and SHARE_NAME
   with the name of the shared folder (as it appears in the VM settings).

If you happen to own one of the "lesser" version of windows (e.g. 7 Home),
you don't have the "secpol.msc" utility. See How do I grant SeCreateSymbolicLink on Windows
Vista Home edition for help.

But if you happen to run a windows system where you are not allowed to run
the Local Security Policy" tool you can't solve the error as described above.
Instead you need to created the symbolic links somewhere else but not in a
shared folder.

4.1. Linux Shell Commands
=========================

4.1.1. Setting environment variable for current shell
-----------------------------------------------------

Enter e.g. "TALKATIVE_MAKEFILE=true".

4.1.2. Change date and time
---------------------------

To change the date and time use command "date -s "Mon Sep 25 15:04:00 CEST 2017".
You may omit the date and just type the new time like "date -s "15:04".


4.1.3. Clear screen
-------------------

Clear screen in linux terminal is: "clear"

4.1.4. Start File Explorer with Admin Rights
--------------------------------------------

In terminal enter sudo nautilus
