The controls implemented for ZSStyleWindows are based on Qt's Fusion Style.

What to do when adding a new control:

- Create the qml file by copying the qml file from %QT_INSTALL_DIR%\Src\qtquickcontrols2\src\imports\controls\fusion
- Add the file name to ..\ZSQuickControlsQml.qrc
- Add the file name to ..\..\..\..\Make\Libs\ZSQuickControls\CMakeLists.txt
- Add the control for visualization in ..\..\..\..\Qml\Libs\ZSQuickControls\ZSStyleWindows\ZSQuickControlsStyleWdgt.qml
