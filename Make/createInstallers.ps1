Param (
    $AppName="all",
    $Compiler="msvc2019",
    $ConfigType="all",
    $QTDIR=$null
)
# @brief Creates an installer for the defined application using the specified compiler and config type.
#
# You need to call this script from the visual studio command prompt which may be opened via:
#
#     Apps/Visual Studio 2019/Developer PowerShell for VS 2019
#
# Change to the directory containing this power shell script. E.g.
#     > cd C:\Projekte\ZeusSoft\ZSQtLib\Make
#
# Set the execution policy to unrestricted for the current user in the command prompt with:
#     C:\Projekte\ZeusSoft\ZSQtLib\Make> Set-ExecutionPolicy Unrestricted -Scope CurrentUser
#
# @Examples
#
#     .\createInstallers.ps1
#     .\createInstallers.ps1 -AppName TrcMthClient -QTDIR "C:\Qt\5.15.2\msvc2019_64" -Compiler msvc2019 -ConfigType Debug
#
# @Param AppName
#     Range [TrcMthClient, LogClient]
#     Name of the product application for which an installer should be created.
#     If omitted installers for all product applications will be created.
#
# @Param Compiler
#     Range [msvc2017, msvc2019, mingw81]
#     Defines the compiler to be used for building the libraries and applications.
#     If omitted the libraries and applications are built for "msvc2019".
#
# @Param ConfigType
#     Range [Debug, Release]
#     Config type to be used.
#     If omitted the libraries and applications are built for both release and debug.
#
# @Param QTDIR
#     Range [valid path]
#     If QTDIR is not set as an environment variable,m or if a different path should be
#     used as defined in the environment, the path may be explicitly passed here.
#
$AppNames = @($AppName)
if( $AppName -ieq "all" ) {
    $AppNames = @( "TrcMthClient", "LogClient" )
}

$ConfigTypes = @($ConfigType)
if( $ConfigType -ieq "all" ) {
    $ConfigTypes = @("Debug", "Release")
}

if($QTDIR -eq $null) {
    $QTDIR=$Env:QTDIR
}
if($QTDIR -eq $null) {
    Write-Host "Error: QTDIR not defined"
    Exit 1
}
$QTDIR=$QTDIR.replace("\bin", "")

$QtDlls = @("Qt5Core", "Qt5Network", "Qt5Xml", "Qt5Qml", "Qt5Gui", "Qt5Widgets")
$ZSQtLibDlls = @("ZSSysQt5", "ZSSysGUIQt5", "ZSIpcQt5", "ZSIpcGUIQt5", "ZSIpcTraceQt5", "ZSIpcTraceGUIQt5")

# Function buildAndInstall
# ------------------------
#
# - builds the libraries, the product applications and the test applications,
# - runs the automated test applications (the test applications requiring user
#   input got to be started separately) and
# - creates an installer for the defined product applications.
#
# @Param Compiler
#     Range [msvc2017, msvc2019, mingw81]
#     Defines the compiler to be used for building the libraries and applications.
#     If omitted the libraries and applications are built for all supported compilers.
#
# @Param Platform
#     Range [x64]
#     Platform for which the libraries and applications should be built.
#     Provided for future use if Win32 should also be supported.
#
# @Param ConfigType
#     Range [Debug, Release]
#     Config type to be used.
#     If omitted the libraries and applications are built for both release and debug.
#
# @Param QTDIR

function buildAndInstall {

    Param (
        $Compiler,
        $Platform,
        $ConfigType,
        $ZSQtLibVersion,
        $QTDIR
    )

    #clear error variable
    $error.clear()

    $BinDir="..\Bin\$Compiler`_$Platform"
    $BuildDir="..\Build\$Compiler`_$Platform`_$ConfigType"

    Write-Host ""
    Write-Host "====================================================================="
    Write-Host "Function buildAndInstall"
    Write-Host "====================================================================="
    Write-Host "PSScriptRoot:   $PSScriptRoot"
    Write-Host "Generator:      $Generator"
    Write-Host "Compiler:       $Compiler"
    Write-Host "Platform:       $Platform"
    Write-Host "ConfigType:     $ConfigType"
    Write-Host "ZSQtLibVersion: $ZSQtLibVersion"
    Write-Host "QTDIR:          $QTDIR"
    Write-Host "BinDir:         $BinDir"
    Write-Host "BuildDir:       $BuildDir"
    Write-Host ""

    if($Compiler.Contains("msvc")) {
        if($Compiler -eq "msvc2017") {
            $Generator="Visual Studio 15 2017"
        }
        if($Compiler -eq "msvc2019") {
            $Generator="Visual Studio 16 2019"
        }
        Write-Host ""
        Write-Host "cmake -G $Generator . -A $Platform -B $BuildDir -DCMAKE_CONFIGURATION_TYPES=$ConfigType -DCMAKE_BUILD_TYPE=$ConfigType -DCMAKE_PREFIX_PATH="$QTDIR" -DCMAKE_INSTALL_PREFIX=$BinDir"
        cmake -G "$Generator" . -A "$Platform" -B "$BuildDir" -DCMAKE_CONFIGURATION_TYPES="$ConfigType" -DCMAKE_BUILD_TYPE="$ConfigType" -DCMAKE_PREFIX_PATH="$QTDIR" -DCMAKE_INSTALL_PREFIX="$BinDir"
    }

    if($Compiler -eq "mingw81") {
        $Generator="MinGW Makefiles"
        if(-Not $env:Path.Contains("C:\Qt\5.15.2\msvc2019_64\bin")) {
            if($env:Path.Contains("C:\Qt\5.15.2\mingw81_64\bin")) {
                $env:Path = $env:Path.Replace("C:\Qt\5.15.2\mingw81_64\bin", "C:\Qt\5.15.2\msvc2019_64\bin")
            }
            else {
                $env:Path = $env:Path + ";C:\Qt\5.15.2\msvc2019_64\bin"
            }
        }
        # Generator MinGW Makefiles does not support platform specification
        Write-Host ""
        Write-Host "cmake -G $Generator . -B $BuildDir -DCMAKE_BUILD_TYPE=$ConfigType -DCMAKE_INSTALL_PREFIX=$BinDir"
        cmake -G "$Generator" . -B "$BuildDir" -DCMAKE_BUILD_TYPE="$ConfigType" -DCMAKE_INSTALL_PREFIX="$BinDir"
    }

    Write-Host ""
    Write-Host "cmake --build $BuildDir --config $ConfigType -j6"
    cmake --build "$BuildDir" --config $ConfigType -j6
    if(!$?) {
        Write-Host "Error: cmake --build failed"
        Exit 1
    }

    Write-Host ""
    Write-Host "cmake --install $BuildDir"
    cmake --install "$BuildDir"
    if(!$?) {
        Write-Host "Error: cmake --install failed"
        Exit 1
    }

    Write-Host ""
    Write-Host "Run automated test..."
    cd $BuildDir
    ### ctest -T test
    if(!$?) {
        Write-Host "Error: Automated tests failed"
        cd $PSScriptRoot
        Exit 1
    }
    cd $PSScriptRoot
}

# Function createInstaller
# ------------------------
#
# - builds the libraries, the product applications and the test applications,
# - runs the automated test applications (the test applications requiring user
#   input got to be started separately) and
# - creates an installer for the defined product applications.
#
# @Param AppName
#     Range [TrcMthClient, LogClient]
#     Name of the product application for which an installer should be created.
#     If omitted installers for all product applications will be created.
#
# @Param Compiler
#     Range [msvc2019, mingw81]
#     Defines the compiler to be used for building the libraries and applications.
#     If omitted the libraries and applications are built for all supported compilers.
#
# @Param Platform
#     Range [x64]
#     Platform for which the libraries and applications should be built.
#     Provided for future use if Win32 should also be supported.
#
# @Param ConfigType
#     Range [Debug, Release]
#     Config type to be used.
#     If omitted the libraries and applications are built for both release and debug.

function createInstaller {

    Param (
        $AppName,
        $Compiler,
        $Platform,
        $ConfigType,
        $ZSQtLibVersion,
        $QTDIR
    )

    #clear error variable
    $error.clear()

    $BinDir="..\Bin\$Compiler`_$Platform"
    $DeployDir="..\Deploy\$Compiler`_$Platform`_$ConfigType\$AppName"

    cd $PSScriptRoot

    Write-Host ""
    Write-Host "====================================================================="
    Write-Host "Function createInstaller"
    Write-Host "====================================================================="
    Write-Host "PSScriptRoot:   $PSScriptRoot"
    Write-Host "AppName:        $AppName"
    Write-Host "Compiler:       $Compiler"
    Write-Host "Platform:       $Platform"
    Write-Host "ConfigType:     $ConfigType"
    Write-Host "ZSQtLibVersion: $ZSQtLibVersion"
    Write-Host "BinDir:         $BinDir"
    Write-Host "DeployDir:      $DeployDir"
    Write-Host "QTDIR:          $QTDIR"
    Write-Host ""

    if (Test-Path -path $DeployDir) {
        Write-Host "rm -r -Force $DeployDir\*"
        rm -r -Force $DeployDir\*
        Write-Host "rmdir -Force $DeployDir"
        rmdir -Force $DeployDir
    }
    Write-Host "mkdir -p $DeployDir"
    mkdir -p $DeployDir

    Write-Host "Copy apps and libs and resources to $DeployDir"
    if($ConfigType -eq "Debug") {
        Write-Host "cp $BinDir\ZSApp$AppName`d.exe $DeployDir"
        cp $BinDir\ZSApp$AppName`d.exe $DeployDir
    }
    else {
        Write-Host "cp $BinDir\ZSApp$AppName.exe $DeployDir"
        cp $BinDir\ZSApp$AppName.exe $DeployDir
    }
    for( $idxZQQtLibDll=0; $idxZQQtLibDll -lt $ZSQtLibDlls.length; $idxZQQtLibDll++ ) {
        $ZSQtLibDll = $ZSQtLibDlls[$idxZQQtLibDll]
        if($ConfigType -eq "Debug") {
            Write-Host "cp $BinDir\$ZSQtLibDll`d.dll $DeployDir"
            cp $BinDir\$ZSQtLibDll`d.dll $DeployDir
        }
        else {
            Write-Host "cp $BinDir\$ZSQtLibDll.dll $DeployDir"
            cp $BinDir\$ZSQtLibDll.dll $DeployDir
        }
    }
    for( $idxQtDll=0; $idxQtDll -lt $QtDlls.length; $idxQtDll++ ) {
        $QtDll = $QtDlls[$idxQtDll]
        if($ConfigType -eq "Debug") {
            Write-Host "cp $QTDIR\bin\$QtDll`d.dll $DeployDir"
            cp $QTDIR\bin\$QtDll`d.dll $DeployDir
        }
        else {
            Write-Host "cp $QTDIR\bin\$QtDll.dll $DeployDir"
            cp $QTDIR\bin\$QtDll.dll $DeployDir
        }
    }

    Write-Host ""
    Write-Host "$QTDIR\bin\windeployqt --compiler-runtime --debug $DeployDir"
    if($ConfigType -eq "Debug") {
        & "$QTDIR\bin\windeployqt" --compiler-runtime --debug $DeployDir
    }
    else {
        & "$QTDIR\bin\windeployqt" --compiler-runtime $DeployDir
    }
    if($error.Count -ne 0)
    {
        Write-Host "Error: windeployqt failed";
        Exit 1
    }

    Write-Host ""
    if (Test-Path -path Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data) {
        Write-Host "rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*"
        rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*
    }
    if (!(Test-Path Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data)) {
        New-Item -Path Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data -ItemType Directory
    }
    Write-Host "cp -r $DeployDir\* Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data"
    cp -r $DeployDir\* Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data
    Write-Host "binarycreator -f -c Apps\Products\ZS$AppName\Installer\config\config.xml -p Apps\Products\ZS$AppName\Installer\packages $DeployDir\$AppName-$ZSQtLibVersion-Installer.exe"
    binarycreator -f -c Apps\Products\ZS$AppName\Installer\config\config.xml -p Apps\Products\ZS$AppName\Installer\packages $DeployDir\$AppName-$ZSQtLibVersion-Installer.exe
    if($error.Count -ne 0)
    {
        Write-Host "Error: binarycreator failed";
        Exit 1;
    }
    Write-Host "rm -r -Force $DeployDir\* -exclude $AppName-$ZSQtLibVersion-Installer.exe"
    rm -r -Force $DeployDir\* -exclude $AppName-$ZSQtLibVersion-Installer.exe
    Write-Host "rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*"
    rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*
}


# Script createInstallers
# -----------------------
#

echo ""
echo "*********************************************************************"
echo "Script createInstallers"
echo "*********************************************************************"
echo ""
echo "AppNames:    $AppNames"
echo "Compiler:    $Compiler"
echo "ConfigTypes: $ConfigTypes"
echo "QTDIR:       $QTDIR"
echo ""

cd ..
echo "git describe --always --tags --long"
$ZSQtLibVersion=git describe --always --tags --long
echo "ZSQtLibVersion: $ZSQtLibVersion"
cd $PSScriptRoot

for( $idxCfgType=0; $idxCfgType -lt $ConfigTypes.length; $idxCfgType++ ) {
    $ConfigType = $ConfigTypes[$idxCfgType]
    # "mingw81" not yet supported (TODO: libQt.. missing in installer packages)
    if($Compiler -ne "mingw81") {
        buildAndInstall -Compiler $Compiler -Platform "x64" -ConfigType $ConfigType -ZSQtLibVersion $ZSQtLibVersion -QTDIR $QTDIR
    }
}

for( $idxAppName=0; $idxAppName -lt $AppNames.length; $idxAppName++ ) {
    $AppName = $AppNames[$idxAppName]
    for( $idxCfgType=0; $idxCfgType -lt $ConfigTypes.length; $idxCfgType++ ) {
        $ConfigType = $ConfigTypes[$idxCfgType]
        # "mingw81" not yet supported (TODO: libQt.. missing in installer packages)
        if($Compiler -ne "mingw81") {
            createInstaller -AppName $AppName -Compiler $Compiler -Platform "x64" -ConfigType $ConfigType -ZSQtLibVersion $ZSQtLibVersion -QTDIR $QTDIR
        }
    }
}

Exit 0
