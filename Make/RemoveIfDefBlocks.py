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
Usage Example:
--------------

This script walks through the directory passed with argument '--directory',
searches for all '*.cpp' and '*.h' files and removes the code blocks starting
with '#ifdef <directive>' to the next line containing '#endif'.

    python RemoveIfDefBlocks.py --directory='SourcePath' --directive='USE_ZS_IPCTRACE_DLL_IF'
'''

import fnmatch, getopt, os, sys

def main():
    directory = ''
    directive = '#ifdef USE_ZS_IPCTRACE_DLL_IF'
    try:
        opts, args = getopt.getopt(sys.argv[1:], '', ['directory=', 'directive='])
    except getopt.GetoptError:
        print('Invalid usage')
        print('RemoveIfDefBlocks.py -directory=<directory> -directive=<directive>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-dir', '--directory'):
            directory = arg
        elif opt in ('-directive', '--directive'):
            directive = arg
    directory = os.path.abspath(directory)
    print('directory = ' + directory)
    if not '#ifdef' in directive:
        directive = '#ifdef ' + directive
    print('directive = ' + directive)
    for root, subdirs, files in os.walk(directory):
        for filename in files:
            sourceFile = ''
            filepath = os.path.join(root, filename)
            if fnmatch.fnmatch(filename, '*.cpp'):
                sourceFile = filepath
            elif fnmatch.fnmatch(filename, '*.h'):
                sourceFile = filepath
            if len(sourceFile) > 0:
                print('Processing File: ', sourceFile)
                with open(sourceFile, 'r', newline='') as f:
                    lines = f.readlines()
                blockLeft = False
                inBlock = False
                newLines = []
                prevLine = ''
                nextLine = ''
                line = ''
                lineBeforeBlock = ''
                lineAfterBlock = ''
                for idxLine in range(len(lines)):
                    if idxLine > 0:
                        prevLine = line
                    if idxLine < len(lines) - 1:
                        nextLine = lines[idxLine+1]
                    line = lines[idxLine]
                    if directive in nextLine:
                        lineBeforeBlock = line
                        if len(line) > 1:
                            newLines.append(line)
                    elif directive in line:
                        inBlock = True
                    elif inBlock and '#endif' in line:
                        inBlock = False
                        blockLeft = True
                        if idxLine < len(lines) - 1:
                            lineAfterBlock = lines[idxLine+1]
                    elif not inBlock:
                        if blockLeft:
                            if len(line) > 1:
                                newLines.append(line)
                            elif len(lineBeforeBlock) <= 1:
                                # Newly added block may start and end with an empty line.
                                # Not both lines will be removed but only the empty line
                                # before the block starts.
                                newLines.append(line)
                            blockLeft = False
                        else:
                            newLines.append(line)
                        lineBeforeBlock = ''
                        lineAfterBlock = ''
                with open(sourceFile, 'w', newline='') as f:
                    for line in newLines:
                        f.write(line)

if __name__ == '__main__':
    main()
