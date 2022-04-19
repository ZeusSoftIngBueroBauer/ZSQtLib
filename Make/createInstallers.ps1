param ($AppName=$null)

# You need to call this script from the visual studio command prompt.
# The command prompt may be opened via:
# - Apps/Visual Studio 2019/Developer PowerShell for VS 2019
# Change to the directory containing this power shell script. E.g.
# > cd C:\Projekte\ZeusSoft\ZSQtLib\Make
# Set the execution policy to unrestricted for the current user in the command prompt with:
# C:\Projekte\ZeusSoft\ZSQtLib\Make> Set-ExecutionPolicy Unrestricted -Scope CurrentUser

if($AppName -eq $null) {
    $AppName="All"
}

function buildAndInstall {

    Param (
        $AppName,
        $Generator,
        $Compiler,
        $Platform,
        $ConfigType
    )

    #clear error variable
    $error.clear()

    $BinDir="..\Bin\$Compiler`_$Platform"
    $BuildDir="..\Build\$Compiler`_$Platform`_$ConfigType"
    $DeployDir="..\Deploy\$Compiler`_$Platform`_$ConfigType"

    cd ..
    $ZSQtLibVersion=git describe --always --tags --long
    cd $PSScriptRoot

    echo ""
    echo "PSScriptRoot:   $PSScriptRoot"
    echo "AppName:        $AppName"
    echo "Generator:      $Generator"
    echo "Compiler:       $Compiler"
    echo "Platform:       $Platform"
    echo "ConfigType:     $ConfigType"
    echo "ZSQtLibVersion: $ZSQtLibVersion"
    echo "BinDir:         $BinDir"
    echo "BuildDir:       $BuildDir"
    echo "DeployDir:      $DeployDir"
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
    }

    echo ""
    echo "cmake -G $Generator -A $Platform -B $BuildDir -DCMAKE_CONFIGURATION_TYPES=$ConfigType -DCMAKE_BUILD_TYPE=$ConfigType -DCMAKE_INSTALL_PREFIX=$BinDir"
    cmake -G "$Generator" -A "$Platform" -B "$BuildDir" -DCMAKE_CONFIGURATION_TYPES="$ConfigType" -DCMAKE_BUILD_TYPE="$ConfigType" -DCMAKE_INSTALL_PREFIX="$BinDir"

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

    #echo ""
    #echo "Run automated test..."
    #cd $BuildDir
    #ctest -T test
    #if(!$?) {
    #    echo "Error: Automated tests failed"
    #    cd $PSScriptRoot
    #    Exit 1;
    #}
    #cd $PSScriptRoot

    echo ""
    echo "PSScriptRoot:   $PSScriptRoot"
    echo "AppName:        $AppName"
    echo "Generator:      $Generator"
    echo "Compiler:       $Compiler"
    echo "Platform:       $Platform"
    echo "ConfigType:     $ConfigType"
    echo "ZSQtLibVersion: $ZSQtLibVersion"
    echo "BinDir:         $BinDir"
    echo "BuildDir:       $BuildDir"
    echo "DeployDir:      $DeployDir"
    echo ""

    if (Test-Path -path $DeployDir) {
        rm -r -Force $DeployDir\*
    }

    echo "Copy apps and libs and resources to $DeployDir"
    if($ConfigType -eq "Debug") {
        cp $BinDir\ZSApp$AppName`d.exe $DeployDir
    }
    else {
        cp $BinDir\ZSApp$AppName.exe $DeployDir
    }
    cp $BinDir\Qt*.dll $DeployDir
    cp $BinDir\ZS*.dll $DeployDir
    cp -r $BinDir\plugins $DeployDir\plugins
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
    rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*
    cp -r "$DeployDir\*" "Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data"
    echo "binarycreator -f -c Apps\Products\ZS$AppName\Installer\config\config.xml -p Apps\Products\ZS$AppName\Installer\packages $DeployDir\$AppName-$ZSQtLibVersion-Installer.exe"
    binarycreator -f -c Apps\Products\ZS$AppName\Installer\config\config.xml -p Apps\Products\ZS$AppName\Installer\packages $DeployDir\$AppName-$ZSQtLibVersion-Installer.exe
    if($error.Count -ne 0)
    {
        echo "Error: binarycreator failed";
        Exit 1;
    }
    rm -r -Force $DeployDir\* -exclude $AppName-$ZSQtLibVersion-Installer.exe
    rm -r -Force Apps\Products\ZS$AppName\Installer\packages\de.zeussoft.$AppName\data\*
}

buildAndInstall -AppName "TrcMthClient" -Generator "Visual Studio 16 2019" -Compiler "msvc2019" -Platform "x64" -ConfigType "Debug"

Exit 0
