@mainpage Overview

Welcome to the user guide of the ZSQtLib.
The user guide provides you with an introduction and overview of the modules within this framework.

ZSQtLib is a C++ class library based on Qt.

The class library provides you with classes which may help you to realize your C++ projects.

For most of the classes also python bindings may be generated and therefore the ZSQtLib classes may also be
used from within python scripts.

The classes are logically organized in namespace packages. The packages are deployed as Dlls. For most of the
packages two Dlls are provided. One Dll summarizes all classes building up the logical application or model layer
without any GUI components (without any viewer functionality). Another Dll provides the viewer classes which may be
used to control the application layer classes or indicate the content of the data model classes.

![Packages_Overview](packages_overview.bmp)

The ZSQtLib is devided into the following areas:
- @subpage _PAGE_BasicConcepts
- @subpage _PAGE_Libs
- @subpage _PAGE_Apps
