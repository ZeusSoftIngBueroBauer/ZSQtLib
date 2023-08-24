'''

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

'''

'''
Description
-----------

This script reads an input file passed with argument '--filePath'
and tries to remove each file listed in this file.

@param inputFilePath
    Path to the file containing the list of files to be removed.

Usage Examples
--------------

python RemoveFiles.py --inputFilePath='InputFilePath'
python RemoveFiles.py --inputFilePath='/home/devel/display_application.bak/ITS-5437-DeleteCodingData/RemovedFiles-3b8da022e-fac5073c'
'''

import fnmatch, getopt, os, sys, subprocess, zipfile

def main():
    inputFilePath = ''
    try:
        opts, args = getopt.getopt(sys.argv[1:], '', ['inputFilePath='])
    except getopt.GetoptError:
        print('Invalid usage')
        print('RemoveFiles.py --inputFilePath=<InputFilePath>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-inputFilePath', '--inputFilePath'):
            inputFilePath = arg
    print('inputFilePath = ' + inputFilePath)
    print('')

    filePaths = []
    with open(inputFilePath, 'r', newline='') as f:
        try:
            filePaths = f.readlines()
        except:
            print('  Cannot read: ', inputFilePath)
    if len(filePaths) > 0:
        for filePath in filePaths:
            filePath = filePath.strip()
            print('  Deleting file: ', filePath)
            os.remove(filePath)

if __name__ == '__main__':
    main()
