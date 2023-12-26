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

Because the python script 'RemoveIfDefBlockFromFile' is imported, you must call
this python script from the directory where the imported python script is located.

This script walks through the directory passed with argument '--path', searches for
all 'CMakeList.txt', '*.cpp', '*.h' and '*.qml' files and removes the code blocks
starting with '#ifdef <directive>' to the next line containing either '#else' or '#endif'.
If an '#else' is found after the '#ifdef <directive>' the lines between '#else' and
the next '#endif' are kept.

In qml files the directives are commented like '//#ifdef <directive>' and '//#else' or '//#endif'.
In the CMakeLists.txt files the blocks are not removed but only added the archive containing

In addition all files in which the '#ifdef <directive>' has been found are added to zip
archives including path information:

- A zip file containing the unmodified files (still including '#ifdef <directive>') and
- a zip file containing the modified files (without the '#ifdef <directive>' blocks).

@param path (optional)
    Default: Current working directory
    Directory to be recursively scanned for files with extension '.cpp', '.h' and '.qml'.
    Pass this argument by inserting -- at the beginning.

@param gitChanges (optional)
    Default: False
    If you pass this argument the list of changed files of the current commit is scanned.

@param sinceCommit (optional)
    Default: Empty string
    If you pass this argument not only the changed files of the current commit but all
    files changed since the defined git commit hash are scanned.

@param directive (optional)
    Default: '#ifdef USE_ZS_IPCTRACE_DLL_IF'
    Directive for the #ifdef blocks to be removed. You may omit '#ifdef'.
    Pass this argument by inserting -- at the beginning.

@param zipOutDir
Default: Current working directory
    Directory in which the zip archives have to be created. If omitted the current directory
    (the directory from which this python script is called) is used.
    Pass this argument by inserting -- at the beginning.

Usage Examples
--------------

python RemoveIfDefBlocks.py --path='/home/devel/display_application' --directive='USE_ZS_QTLIBS' --zipOutDir='/mnt/hgfs/vm-share/display_application.zstrace/ITS-5808-ReactionToProfileChangeVECUService'
python RemoveIfDefBlocks.py --path='/home/devel/display_application' --gitChanges --sinceCommit='fac5073c' --directive='USE_ZS_QTLIBS' --zipOutDir='/mnt/hgfs/vm-share/display_application.zstrace/ITS-5808-ReactionToProfileChangeVECUService
'''

import argparse, datetime, fnmatch, getopt, os, sys, subprocess, zipfile
import RemoveIfDefBlocksFromFile

def getGitRevisionHash(repoRootDir) -> str:
    print('')
    print('-> getGitRevisionHash(' + repoRootDir + ')')
    currDir = os.getcwd()
    os.chdir(repoRootDir)
    hash = subprocess.check_output(['git', 'rev-parse', 'HEAD']).decode('ascii').strip()
    os.chdir(currDir)
    print('<- getGitRevisionHash: ' + hash)
    print('')
    return hash

def getGitRevisionShortHash(repoRootDir) -> str:
    print('')
    print('-> getGitRevisionShortHash: ' + repoRootDir)
    currDir = os.getcwd()
    os.chdir(repoRootDir)
    currDir = os.getcwd()
    hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode('ascii').strip()
    os.chdir(currDir)
    print('<- getGitRevisionShortHash: ' + hash)
    print('')
    return hash

def getFilePathsOfChangedFilesSinceCommit(repoRootDir, gitHash) -> []:
    print('')
    print('-> getFilePathsOfChangedFilesSinceCommit(' + repoRootDir + ', ' + gitHash + ')')
    filePaths = []
    currDir = os.getcwd()
    os.chdir(repoRootDir)
    currDir = os.getcwd()
    filePaths = subprocess.check_output(['git', 'diff', '--name-only', gitHash]).decode('ascii').split()
    os.chdir(currDir)
    for sourceFilePath in filePaths:
        print('  ' + sourceFilePath)
    print('<- getFilePathsOfChangedFilesSinceCommit')
    print('')
    return filePaths

def getFilePathsWithDirective(repoRootDir, directive) -> []:
    print('')
    print('-> getFilePathsWithDirective(' + repoRootDir + ', ' + directive + ')')
    filePathsWithDirective = []
    for dirPath, dirNames, fileNames in os.walk(repoRootDir):
        for fileName in fileNames:
            sourceFilePath = ''
            filePath = os.path.join(dirPath, fileName)
            directiveInFiles = directive
            if fnmatch.fnmatch(fileName, 'CMakeLists.txt'):
                directiveInFiles = directiveInFiles.removeprefix('#ifdef ')
                sourceFilePath = filePath
            elif fnmatch.fnmatch(fileName, '*.qml'):
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
                            if directiveInFiles in line:
                                directiveInFile = True
                                break
                    except:
                        print('Cannot read: ', sourceFilePath)
                if directiveInFile:
                    print('Found File: ', sourceFilePath)
                    filePathsWithDirective.append(sourceFilePath)
                #else:
                #    print('Ignoring File: ', sourceFilePath)
    print('<- getFilePathsWithDirective')
    print('')
    return filePathsWithDirective

def getFilePathsOfRemovedFiles(filePaths) -> []:
    print('')
    print('-> getFilePathsOfRemovedFiles')
    filePathsOfRemovedFiles = []
    for sourceFilePath in filePaths:
        if not os.path.exists(sourceFilePath):
            filePathsOfRemovedFiles.append(sourceFilePath)
    for sourceFilePath in filePathsOfRemovedFiles:
        print('  ' + sourceFilePath)
    print('<- getFilePathsOfRemovedFiles')
    print('')
    return filePathsOfRemovedFiles

def addToArchive(zipOutDir, zipFileName, filePaths):
    print('')
    print('-> addToArchive(' + zipOutDir + ', ' + zipFileName + ')')
    if not os.path.exists(zipOutDir):
        os.makedirs(zipOutDir)
    zipFilePath = os.path.join(zipOutDir, zipFileName)
    zipFile = zipfile.ZipFile(zipFilePath, "w" )
    for sourceFilePath in filePaths:
        if os.path.exists(sourceFilePath):
            print('  Archiving File "', sourceFilePath, '" in "', zipFilePath, '"')
            zipFile.write(sourceFilePath, compress_type=zipfile.ZIP_DEFLATED)
        else:
            print('  Ignoring Not Existing File "', sourceFilePath, '"')
    print('<- addToArchive')
    print('')

def writeFilePathsToFile(zipOutDir, fileName, filePaths):
    print('')
    print('-> writeFilePathsToFile(' + zipOutDir + ', ' + fileName + ')')
    if not os.path.exists(zipOutDir):
        os.makedirs(zipOutDir)
    filePath = os.path.join(zipOutDir, fileName)
    with open(filePath, 'w') as f:
        for sourceFilePath in filePaths:
            print('  ' + sourceFilePath)
            f.write(sourceFilePath)
            f.write('\n')
    print('<- writeFilePathsToFile')
    print('')

def removeDirectiveFromFiles(filePaths, directive):
    print('')
    print('-> removeDirectiveFromFiles(' + directive + ')')
    for sourceFilePath in filePaths:
        RemoveIfDefBlocksFromFile.removeDirectiveFromFile(sourceFilePath, directive)
    print('<- removeDirectiveFromFiles')

def main():
    parser = argparse.ArgumentParser(description='Removes ifdef blocks from files')
    parser.add_argument('-p', '--path', help='Directory to be recursively scanned for files with extension .cpp, .h and .qml.')
    parser.add_argument('-g', '--gitChanges', type=bool, default=False, help='Check files changed in git commit.')
    parser.add_argument('-s', '--sinceCommit', default=None, help='Include files since this git commit hash.')
    parser.add_argument('-d', '--directive', default='USE_ZS_IPCTRACE_DLL_IF', help='Directive for the #ifdef blocks to be removed.')
    parser.add_argument('-z', '--zipOutDir', help='Directory in which the zip archives have to be created.')
    args = parser.parse_args()
    path = args.path
    gitChanges = args.gitChanges
    directive = args.directive
    if not '#ifdef' in directive:
        directive = '#ifdef ' + directive
    zipOutDir = args.zipOutDir
    print('path           = ' + path)
    print('gitChanges     = ' + str(gitChanges))
    print('zipOutDir      = ' + zipOutDir)
    print('directive      = ' + directive)
    if gitChanges:
        gitHashCurrent = getGitRevisionShortHash(path)
        if args.sinceCommit is None:
            gitHashBase = gitHashCurrent
        else:
            gitHashBase = args.sinceCommit
        print('gitHashBase    = ' + gitHashBase)
        print('gitHashCurrent = ' + gitHashCurrent)
    print('')

    dateTimeString = datetime.datetime.now().strftime("%Y-%m-%d-%H-%M-%S")
    if gitChanges:
        filePaths = getFilePathsOfChangedFilesSinceCommit(path, gitHashBase)
        zipFileName = 'ChangedFiles-' + gitHashCurrent + '-' + gitHashBase
        addToArchive(zipOutDir, zipFileName, filePaths)
        filePathsRemovedSinceCommit = getFilePathsOfRemovedFiles(filePaths)
        fileName = 'RemovedFiles-' + gitHashCurrent + '-' + gitHashBase
        writeFilePathsToFile(zipOutDir, fileName, filePathsRemovedSinceCommit)
    else:
        zipFileName = 'ChangedFiles-' + dateTimeString
        filePaths = getFilePathsWithDirective(path, directive)
        addToArchive(zipOutDir, zipFileName, filePaths)

    removeDirectiveFromFiles(filePaths, directive)

    if gitChanges:
        zipFileName = 'ChangedFiles-' + gitHashCurrent + '-' + gitHashBase + '-Without' + directive
    else:
        zipFileName = 'ChangedFiles-' + dateTimeString + '-Without' + directive
    zipFileName = zipFileName.replace('#ifdef ', '')
    addToArchive(zipOutDir, zipFileName, filePaths)

if __name__ == '__main__':
    main()
