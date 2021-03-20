#-------------------------------------------------------------------------------
# Copyright (c) 2015 Dialog Semiconductor GmbH, All rights reserved.
# 
# Created on 20.07.2015
#
# Author: Christian Bauer
#
#-------------------------------------------------------------------------------
'''
'''

import argparse
import sys, socket, os, traceback

from PySide import QtGui

from app import CApp


#*******************************************************************************
def main():
#*******************************************************************************
    '''
    See docstring of the module above.
    '''

    theApp = None

    strOrgName   = "Dialog Semiconductor"
    strOrgDomain = "www.dialog-semiconductor.com"
    strAppName = 'RemCmdClientExample'
    strMainWindowTitle = "Remote Command Client Example (using bindings created with shiboken)"

    strConfigDir = 'C:/' + strOrgName + '/RemCmdClient/Examples/Config'
    strLogDir = 'C:/' + strOrgName + '/RemCmdClient/Examples/Log'

    try:
        if not os.path.exists(strConfigDir):
            os.makedirs(strConfigDir)
        if not os.path.exists(strLogDir):
            os.makedirs(strLogDir)

        theApp = CApp(
            sys.argv,
            strOrgName,
            strOrgDomain,
            strAppName,
            strMainWindowTitle,
            strConfigDir,
            strLogDir )
        theApp.exec_() # pylint: disable=no-member

    except SystemExit:
        pass
    except Exception: # pylint: disable=broad-except
        exc_type, exc_value = sys.exc_info()[0:2]
        strExc = traceback.format_exception_only(exc_type,exc_value)
        strExc = "".join(strExc)
        strExc = strExc.replace('\n',' ').replace('\r',' ')
        if theApp is None:#
            theApp = QtGui.QApplication.instance()  # pylint: disable=no-member
        if theApp is None:
            theApp = QtGui.QApplication(sys.argv)       # pylint: disable=no-member
        QtGui.QMessageBox.critical( None, "Startup", "Failed to start application\n\n%s" % strExc ) # pylint: disable=no-member
        traceback.print_exc()


#*******************************************************************************
if __name__ == '__main__':
#*******************************************************************************
    main()
