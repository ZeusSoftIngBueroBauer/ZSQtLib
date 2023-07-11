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
searches for all '*.cpp', '*.h' and '*.qml' files and removes the code blocks starting
with '#ifdef <directive>' to the next line containing either '#else' or '#endif'.
If an '#else' is found after the '#ifdef <directive>' the lines between '#else'
end the next '#endif' are kept.

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
            if fnmatch.fnmatch(filename, '*.qml'):
                sourceFile = filepath
            elif fnmatch.fnmatch(filename, '*.cpp'):
                sourceFile = filepath
            elif fnmatch.fnmatch(filename, '*.h'):
                sourceFile = filepath
            if len(sourceFile) > 0:
                print('Checking File: ', sourceFile)
                directiveInFile = False
                with open(sourceFile, 'r', newline='') as f:
                    try:
                        lines = f.readlines()
                        for idxLine in range(len(lines)):
                            line = lines[idxLine]
                            if directive in line:
                                directiveInFile = True
                                break
                    except:
                        print('Cannot read: ', sourceFile)
                if not directiveInFile:
                    print('Ignoring File: ', sourceFile)
                else:
                    print('Processing File: ', sourceFile)
                    newLines = []
                    inIfBlock = False
                    inElseBlock = False
                    blockLeft = False
                    for idxLine in range(len(lines)):
                        prevLine = ''
                        nextLine = ''
                        newLine = ''
                        if idxLine > 0:
                            prevLine = line
                        if idxLine < len(lines) - 1:
                            nextLine = lines[idxLine+1]
                        line = lines[idxLine]
                        isSpaceLine = line.isspace()
                        #print('-----------------------------')
                        #print('Line: ', idxLine)
                        #print('Line     [{}]: {}'.format(len(line), line))
                        #print('PrevLine [{}]: {}'.format(len(prevLine), prevLine))
                        #print('NextLine [{}]: {}'.format(len(nextLine), nextLine))
                        #print('-> isSpaceLine:     ', isSpaceLine)
                        #print('-> inIfBlock:       ', inIfBlock)
                        #print('-> inElseBlock:     ', inElseBlock)
                        #print('-> blockLeft:       ', blockLeft)
                        if directive in line:
                            inIfBlock = True
                            blockLeft = False
                        elif inIfBlock and '#else' in line:
                            inElseBlock = True
                            blockLeft = False
                        elif inIfBlock and '#endif' in line:
                            inIfBlock = False
                            inElseBlock = False
                            blockLeft = True
                        elif inElseBlock:
                            newLine = line
                            blockLeft = False
                        elif not inIfBlock:
                            if isSpaceLine:
                                if not blockLeft: # or directive in nextLine):
                                    newLine = line
                            else:
                                newLine = line
                            blockLeft = False
                        #print('')
                        #print('<- inIfBlock:       ', inIfBlock)
                        #print('<- inElseBlock:     ', inElseBlock)
                        #print('<- blockLeft:       ', blockLeft)
                        #print('newLine [{}]: {}'.format(len(newLine), newLine))
                        if len(newLine) > 0:
                            newLines.append(line)
                    targetFile = sourceFile # + '.tst'
                    with open(targetFile, 'w', newline='') as f:
                        for line in newLines:
                            f.write(line)

if __name__ == '__main__':
    main()
