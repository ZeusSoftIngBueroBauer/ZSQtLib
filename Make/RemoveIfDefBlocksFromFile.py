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

This script removes the code blocks starting with '#ifdef <directive>'
(in qml files with '//#ifdef <directive>') to the next line containing either
'#else' or '#endif' (in qml either '//#else' or '//#endif').
If an '#else' is found after the '#ifdef <directive>' the lines between '#else'
end the next '#endif' are kept.

@param filePath
    Complete file path from which the blocks have to be removed.
    Pass this argument by inserting -- at the beginning.

@param directive
    Directive for the #ifdef blocks to be removed. You may omit '#ifdef'.
    Pass this argument by inserting -- at the beginning.

Usage Examples
--------------

python RemoveIfDefBlocksFromFile.py --filePath='SourcePath'
python RemoveIfDefBlocksFromFile.py --directive='USE_ZS_QTLIBS' --filePath='/home/devel/display_application/IsobusBase/test_IsobusBase/addressClaimStateMachine/TestAddressClaimStateMachine.cpp'
'''

import fnmatch, getopt, os, sys, subprocess

def removeDirectiveFromFile(filePath, directive):
    print('')
    print('-> removeDirectiveFromFile(' + filePath + ', ' + directive + ')')
    lines = []
    try:
        with open(filePath, 'r', newline='') as f:
            try:
                lines = f.readlines()
            except:
                print('    Cannot read: ', filePath)
    except:
        print('    Cannot open: ', filePath)
    if len(lines) > 0:
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
        targetFile = filePath # + '.tst'
        with open(targetFile, 'w', newline='') as f:
            for line in newLines:
                f.write(line)
    print('<- removeDirectiveFromFile')

def main():
    filePath = ''
    directive = '#ifdef USE_ZS_IPCTRACE_DLL_IF'
    try:
        opts, args = getopt.getopt(sys.argv[1:], '', ['filePath=', 'directive='])
    except getopt.GetoptError:
        print('Invalid usage')
        print('RemoveIfDefBlocksFromFile.py --filePath=<filePath> --directive=<directive>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-filePath', '--filePath'):
            filePath = arg
        elif opt in ('-directive', '--directive'):
            directive = arg
    if not '#ifdef' in directive:
        directive = '#ifdef ' + directive
    print('filePath = ' + filePath)
    print('directive = ' + directive)
    removeDirectiveFromFile(filePath, directive)
    print('')

if __name__ == '__main__':
    main()
