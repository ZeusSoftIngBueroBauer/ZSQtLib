.. _zsqtlib_pythonbindings:

Python Bindings
===============

In order to allow access to the remote command server via the remote command client class of the
ZSQtLib it was necessary to provide python bindings for the ZSQtLib classes which should be
accessible from within the python scripts.

As the ZSQtLib is based on Qt providing not just Ansi C but C++ classes the classes and types of
the ZSQtLib framework may not just simply exposed by means of providing cpython extensions.
Instead it was necessary to use a by far more complicated way to generate the bindings.

Qt is exposed to python by the PySide project. The PySide project provides LGPL-licensed Python
bindings for the Qt. It also includes complete toolchain for rapidly generating bindings for any
Qt-based C++ class hierarchies. PySide Qt bindings allow both free open source and proprietary
software development and ultimately aim to support Qt platforms.

PySide are CPython Qt bindings generated using the Shiboken generator.

As PySide is generated using shiboken it has been decided to also use shiboken and together with
PySide also shiboken and all type system XML files are installed in the pythons site package
directory it has been decided also to use shiboken to generate python language bindings of the
C+ classes of the ZSQtLib.


.. image:: zsqtlib_pythonbindings.png


.. toctree::
    :maxdepth: 7

    zsqtlib_pythonbindings_build_binaries
    zsqtlib_pythonbindings_build_setups
    zsqtlib_pythonbindings_install
