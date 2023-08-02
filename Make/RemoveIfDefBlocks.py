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

This script walks through the directory passed with argument '--gitRepoDir',
searches for all '*.cpp', '*.h' and '*.qml' files and removes the code blocks starting
with '#ifdef <directive>' (in qml files with '//#ifdef <directive>') to the next line
containing either '#else' or '#endif' (in qml either '//#else' or '//#endif').
If an '#else' is found after the '#ifdef <directive>' the lines between '#else'
end the next '#endif' are kept.

In addition all files in which the '#ifdef <directive>' has been found are added to zip
archives including path information:

- A zip file containing the unmodified files (still including '#ifdef <directive>') and
- a zip file containing the modified files (without the '#ifdef <directive>' blocks).

@param gitRepoDir
    Directory to be recursively scanned for files with extension '.cpp', '.h' and '.qml'.
    Pass this argument by inserting -- at the beginning.

@param directive
    Directive for the #ifdef blocks to be removed. You may omit '#ifdef'.
    Pass this argument by inserting -- at the beginning.

@param zipOutDir
    Directory in which the zip archives have to be created. If omitted the current directory
    (the directory from which this python script is called) is used.
    Pass this argument by inserting -- at the beginning.

Usage Examples
--------------

python RemoveIfDefBlocks.py --gitRepoDir='SourcePath' --directive='USE_ZS_IPCTRACE_DLL_IF' --zipOutDir='OutputPath'
python RemoveIfDefBlocks.py --gitRepoDir='/home/devel/display_application' --directive='USE_ZS_QTLIBS' --zipOutDir='/home/devel/display_application.bak'
'''

import fnmatch, getopt, os, sys, subprocess, zipfile

def getGitRevisionHash(repoRootDir) -> str:
    currDir = os.getcwd()
    os.chdir(repoRootDir)
    hash = subprocess.check_output(['git', 'rev-parse', 'HEAD']).decode('ascii').strip()
    os.chdir(currDir)
    return hash

def getGitRevisionShortHash(repoRootDir) -> str:
    currDir = os.getcwd()
    os.chdir(repoRootDir)
    currDir = os.getcwd()
    hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode('ascii').strip()
    os.chdir(currDir)
    return hash

def main():
    gitRepoDir = ''
    directive = '#ifdef USE_ZS_IPCTRACE_DLL_IF'
    zipOutDir = ''
    try:
        opts, args = getopt.getopt(sys.argv[1:], '', ['gitRepoDir=', 'directive=', 'zipOutDir='])
    except getopt.GetoptError:
        print('Invalid usage')
        print('RemoveIfDefBlocks.py --gitRepoDir=<directory> --directive=<directive>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-gitRepoDir', '--gitRepoDir'):
            gitRepoDir = arg
        elif opt in ('-directive', '--directive'):
            directive = arg
        elif opt in ('-zipOutDir', '--zipOutDir'):
            zipOutDir = arg
    zipOutDir = os.path.abspath(zipOutDir)
    gitHash = getGitRevisionShortHash(gitRepoDir)
    print('gitRepoDir = ' + gitRepoDir)
    print('zipOutDir = ' + zipOutDir)
    if not '#ifdef' in directive:
        directive = '#ifdef ' + directive
    print('directive = ' + directive)
    print('gitHash = ' + gitHash)
    print('')
    filePathsWithDirective = []
    for dirPath, dirNames, fileNames in os.walk(gitRepoDir):
        for fileName in fileNames:
            sourceFilePath = ''
            filePath = os.path.join(dirPath, fileName)
            if fnmatch.fnmatch(fileName, '*.qml'):
                sourceFilePath = filePath
            elif fnmatch.fnmatch(fileName, '*.cpp'):
                sourceFilePath = filePath
            elif fnmatch.fnmatch(fileName, '*.h'):
                sourceFilePath = filePath
            if len(sourceFilePath) > 0:
                #print('Checking File: ', sourceFilePath)
                directiveInFile = False
                with open(sourceFilePath, 'r', newline='') as f:
                    try:
                        lines = f.readlines()
                        for idxLine in range(len(lines)):
                            line = lines[idxLine]
                            if directive in line:
                                directiveInFile = True
                                break
                    except:
                        print('Cannot read: ', sourceFilePath)
                if directiveInFile:
                    #print('Processing File: ', sourceFilePath)
                    filePathsWithDirective.append(sourceFilePath)
                #else:
                #    print('Ignoring File: ', sourceFilePath)
    if not os.path.exists(zipOutDir):
        os.makedirs(zipOutDir)
    zipFileName = 'With_' + directive + '_IfDefBlocks_' + gitHash
    zipFileName = zipFileName.replace('#ifdef ', '')
    zipFilePath = os.path.join(zipOutDir, zipFileName)
    zipFile = zipfile.ZipFile(zipFilePath, "w" )
    print('')
    for sourceFilePath in filePathsWithDirective:
        print('Archiving File "', sourceFilePath, '" in "', zipFilePath, '"')
        zipFile.write(sourceFilePath, compress_type=zipfile.ZIP_DEFLATED)
    print('')
    for sourceFilePath in filePathsWithDirective:
        print('Processing File: ', sourceFilePath)
        with open(sourceFilePath, 'r', newline='') as f:
            try:
                lines = f.readlines()
            except:
                print('Cannot read: ', sourceFilePath)
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
        targetFile = sourceFilePath # + '.tst'
        with open(targetFile, 'w', newline='') as f:
            for line in newLines:
                f.write(line)
    zipFileName = 'Without_' + directive + '_IfDefBlocks_' + gitHash
    zipFileName = zipFileName.replace('#ifdef ', '')
    zipFilePath = os.path.join(zipOutDir, zipFileName)
    zipFile = zipfile.ZipFile(zipFilePath, "w" )
    print('')
    for sourceFilePath in filePathsWithDirective:
        print('Archiving File "', sourceFilePath, '" in "', zipFilePath, '"')
        zipFile.write(sourceFilePath, compress_type=zipfile.ZIP_DEFLATED)
    print('')

if __name__ == '__main__':
    main()
