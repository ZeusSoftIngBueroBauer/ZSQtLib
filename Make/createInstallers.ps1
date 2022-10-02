Param (
    $AppName="all",
    $Compiler="all",
    $ConfigType="all"
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
#     .\createInstallers.ps1 -AppName TrcMthClient -Compiler msvc2019 -ConfigType Debug
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
# @Param ConfigType
#     Range [Debug, Release]
#     Config type to be used.
#     If omitted the libraries and applications are built for both release and debug.

$AppNames = @($AppName)
if( $AppName -ieq "all" ) {
    $AppNames = @( "TrcMthClient", "LogClient" )
}

$Compilers = @($Compiler)
if( $Compiler -ieq "all" ) {
    $Compilers = @("msvc2019", "mingw81")
}

$ConfigTypes = @($ConfigType)
if( $ConfigType -ieq "all" ) {
    $ConfigTypes = @("Debug", "Release")
}

# Function buildAndInstall
# ------------------------
#
# - builds the libraries, the product applications and the test applications,
# - runs the automated test applications (the test applications requiring user
#   input got to be started separately) and
# - creates an installer for the defined product applications.
#
# @Param Generator
#     Range ["Visual Studio 16 2019", "MinGW Makefiles""]
#     Defines the generator passed to CMake to create the build tree.
#     Depends on the compiler to be used.
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

function buildAndInstall {

    Param (
        $Generator,
        $Compiler,
        $Platform,
        $ConfigType,
        $ZSQtLibVersion
    )

    #clear error variable
    $error.clear()

    $BinDir="..\Bin\$Compiler`_$Platform"
    $BuildDir="..\Build\$Compiler`_$Platform`_$ConfigType"

    echo ""
    echo "====================================================================="
    echo "Function buildAndInstall"
    echo "====================================================================="
    echo "PSScriptRoot:   $PSScriptRoot"
    echo "Generator:      $Generator"
    echo "Compiler:       $Compiler"
    echo "Platform:       $Platform"
    echo "ConfigType:     $ConfigType"
    echo "ZSQtLibVersion: $ZSQtLibVersion"
    echo "BinDir:         $BinDir"
    echo "BuildDir:       $BuildDir"
    echo ""

    if($Generator -eq "Visual Studio 16 2019") {
        if(-Not $env:Path.Contains("C:\Qt\5.15.2\msvc2019_64\bin")) {
            if($env:Path.Contains("C:\Qt\5.15.2\mingw81_64\bin")) {
                $env:Path = $env:Path.Replace("C:\Qt\5.15.2\mingw81_64\bin", "C:\Qt\5.15.2\msvc2019_64\bin")
            }
            else {
                $env:Path = $env:Path + ";C:\Qt\5.15.2\msvc2019_64\bin"
            }
        }

        echo ""
        echo "cmake -G $Generator . -A $Platform -B $BuildDir -DCMAKE_CONFIGURATION_TYPES=$ConfigType -DCMAKE_BUILD_TYPE=$ConfigType -DCMAKE_INSTALL_PREFIX=$BinDir"
        cmake -G "$Generator" . -A "$Platform" -B "$BuildDir" -DCMAKE_CONFIGURATION_TYPES="$ConfigType" -DCMAKE_BUILD_TYPE="$ConfigType" -DCMAKE_INSTALL_PREFIX="$BinDir"
    }

    if($Generator -eq "MinGW Makefiles") {
        if(-Not $env:Path.Contains("C:\Qt\5.15.2\mingw81_64\bin")) {
            if($env:Path.Contains("C:\Qt\5.15.2\msvc2019_64\bin")) {
                $env:Path = $env:Path.Replace("C:\Qt\5.15.2\msvc2019_64\bin", "C:\Qt\5.15.2\mingw81_64\bin")
            }
            else {
                $env:Path = $env:Path + ";C:\Qt\5.15.2\mingw81_64\bin"
            }
        }

        # Generator MinGW Makefiles does not support platform specification
        echo ""
        echo "cmake -G $Generator . -B $BuildDir -DCMAKE_BUILD_TYPE=$ConfigType -DCMAKE_INSTALL_PREFIX=$BinDir"
        cmake -G "$Generator" . -B "$BuildDir" -DCMAKE_BUILD_TYPE="$ConfigType" -DCMAKE_INSTALL_PREFIX="$BinDir"
    }

    echo ""
    echo "cmake --build $BuildDir --config=$ConfigType -j6"
    cmake --build "$BuildDir" --config="$ConfigType" -j6
    if(!$?) {
        echo "Error: cmake --build failed"
        Exit 1;
    }

    echo ""
    echo "cmake --install $BuildDir"
    cmake --install "$BuildDir"
    if(!$?) {
        echo "Error: cmake --install failed"
        Exit 1;
    }

    echo ""
    echo "Run automated test..."
    cd $BuildDir
    ### ctest -T test
    if(!$?) {
        echo "Error: Automated tests failed"
        cd $PSScriptRoot
        Exit 1;
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
        $ZSQtLibVersion
    )

    #clear error variable
    $error.clear()

    $BinDir="..\Bin\$Compiler`_$Platform"
    $DeployDir="..\Deploy\$Compiler`_$Platform`_$ConfigType\$AppName"

    cd $PSScriptRoot

    echo ""
    echo "====================================================================="
    echo "Function createInstaller"
    echo "====================================================================="
    echo "PSScriptRoot:   $PSScriptRoot"
    echo "AppName:        $AppName"
    echo "Compiler:       $Compiler"
    echo "Platform:       $Platform"
    echo "ConfigType:     $ConfigType"
    echo "ZSQtLibVersion: $ZSQtLibVersion"
    echo "BinDir:         $BinDir"
    echo "DeployDir:      $DeployDir"
    echo ""

    if (Test-Path -path $DeployDir) {
        echo "rm -r -Force $DeployDir\*"
        rm -r -Force $DeployDir\*
        echo "rmdir -Force $DeployDir"
        rmdir -Force $DeployDir
    }
    echo "mkdir -p $DeployDir"
    mkdir -p $DeployDir

    echo "Copy apps and libs and resources to $DeployDir"
    if($ConfigType -eq "Debug") {
        echo "cp $BinDir\ZSApp$AppName`d.exe $DeployDir"
        cp $BinDir\ZSApp$AppName`d.exe $DeployDir
    }
    else {
        echo "cp $BinDir\ZSApp$AppName.exe $DeployDir"
        cp $BinDir\ZSApp$AppName.exe $DeployDir
    }
    echo "cp $BinDir\Qt*.dll $DeployDir"
    cp $BinDir\Qt*.dll $DeployDir
    echo "cp $BinDir\ZS*.dll $DeployDir"
    cp $BinDir\ZS*.dll $DeployDir
    echo "cp -r $BinDir\plugins $DeployDir\plugins"
    cp -r $BinDir\plugins $DeployDir\plugins
    echo "cp ..\Resources\Apps\Products\ZS$AppName\ZSApp$AppName.ico $DeployDir"
    cp ..\Resources\Apps\Products\ZS$AppName\ZSApp$AppName.ico $DeployDir

    echo ""
    echo "windeployqt --debug $DeployDir"
    windeployqt --debug $DeployDir
    if($error.Count -ne 0)
    {
        echo "Error: windeployqt failed";
        Exit 1;
    }

    echo ""
    if (Test-Path -path $Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data) {
        echo "rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*"
        rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*
    }
    if (!(Test-Path $Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data)) {
        New-Item -Path $Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data -ItemType Directory
    }
    echo "cp -r $DeployDir\* Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data"
    cp -r $DeployDir\* Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data
    echo "binarycreator -f -c Apps\Products\ZS$AppName\Installer\config\config.xml -p Apps\Products\ZS$AppName\Installer\packages $DeployDir\$AppName-$ZSQtLibVersion-Installer.exe"
    binarycreator -f -c Apps\Products\ZS$AppName\Installer\config\config.xml -p Apps\Products\ZS$AppName\Installer\packages $DeployDir\$AppName-$ZSQtLibVersion-Installer.exe
    if($error.Count -ne 0)
    {
        echo "Error: binarycreator failed";
        Exit 1;
    }
    echo "rm -r -Force $DeployDir\* -exclude $AppName-$ZSQtLibVersion-Installer.exe"
    rm -r -Force $DeployDir\* -exclude $AppName-$ZSQtLibVersion-Installer.exe
    echo "rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*"
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

cd ..
$ZSQtLibVersion=git describe --always --tags --long
cd $PSScriptRoot

for( $idxCompiler=0; $idxCompiler -lt $Compilers.length; $idxCompiler++ ) {
    $Compiler = $Compilers[$idxCompiler]
    for( $idxCfgType=0; $idxCfgType -lt $ConfigTypes.length; $idxCfgType++ ) {
        $ConfigType = $ConfigTypes[$idxCfgType]
        if($Compiler -eq "msvc2019") {
            buildAndInstall -Generator "Visual Studio 16 2019" -Compiler $Compiler -Platform "x64" -ConfigType $ConfigType -ZSQtLibVersion $ZSQtLibVersion
        }
        if($Compiler -eq "mingw81") {
            # Not yet supported (TODO: libQt.. missing in installer packages)
            #buildAndInstall -Generator "MinGW Makefiles" -Compiler $Compiler -Platform "x64" -ConfigType $ConfigType -ZSQtLibVersion $ZSQtLibVersion
        }
    }
}

for( $idxAppName=0; $idxAppName -lt $AppNames.length; $idxAppName++ ) {
    $AppName = $AppNames[$idxAppName]
    for( $idxCompiler=0; $idxCompiler -lt $Compilers.length; $idxCompiler++ ) {
        $Compiler = $Compilers[$idxCompiler]
        # Compiler "mingw81" not yet supported (TODO: libQt.. missing in installer packages)
        if($Compiler -eq "msvc2019") {
            createInstaller -AppName $AppName -Compiler $Compiler -Platform "x64" -ConfigType $ConfigType -ZSQtLibVersion $ZSQtLibVersion
        }
    }
}

Exit 0
