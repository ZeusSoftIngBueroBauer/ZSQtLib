set MakeDir = 0
if not exist ..\Windows goto CLEAN
if not exist ..\..\Make goto CLEAN
set MakeDir = 1
cd ..
cd ..

:CLEAN

if not exist .\Make goto EOF

del *.msi /s
del *.exe /s
del *.obj /s
del *.o /s
del *.moc /s
del *.lib /s
del *.pch /s
del *.exp /s
del *.dll /s
del *.idb /s
del *.pdb /s
del *.ncb /s
del *.bsc /s
del *.ilk /s
del *.sbr /s
del *.res /s
del *.rsp /s
del *.aps /s
del *.log /s
del *.tmp /s
del *.contrib /s
del *.contrib.1 /s
del *.contrib.2 /s
del *.contrib.3 /s
del *.contrib.4 /s
del *.contrib.5 /s
del *.keep /s
del *.keep.1 /s
del *.keep.2 /s
del *.keep.3 /s
del *.keep.4 /s
del *.keep.5 /s
del *.bauer_c.user /s
del *.vcxproj.user /s
del *.sln.ccexclude /s
del "*.Christian Bauer.user" /s

del makefile.debug /s
del makefile.release /s

del bat00000*.bat /s
del moc_*.cpp /s
del emb_*.cpp /S
del build_*.html /s
del *.manifest /s
del *.manifest.res /s
del mt.dep /s
del UpgradeLog*.* /s

del vssver.scc /s
attrib -r mssccprj.scc /s
del mssccprj.scc /s

del .mvfs_* /s

attrib -h *.suo /s
attrib -r *.suo /s
del *.suo /s
del buildlog.htm /s

attrib -h *.sdf /s
attrib -r *.sdf /s
del *.sdf /s

attrib -h *.opensdf /s
attrib -r *.opensdf /s
del *.opensdf /s

attrib -h *.pro.user /s
attrib -r *.pro.user /s
del *.pro.user /s

attrib -h *.pro.user.* /s
attrib -r *.pro.user.* /s
del *.pro.user.* /s

rd /s /q Make\Windows\_UpgradeReport_Files
rd /s /q Make\Windows\Backup
rd /s /q Make\Windows\Backup1
rd /s /q Make\Windows\Backup2
rd /s /q Make\Windows\Backup3
rd /s /q Make\Windows\Backup4
rd /s /q Make\Windows\Backup5
rd /s /q Make\Windows\Backup6

rd /s /q Make\Windows\ZSData\Win32
rd /s /q Make\Windows\ZSData\x64

rd /s /q Make\Windows\ZSDataGUI\Win32
rd /s /q Make\Windows\ZSDataGUI\x64

rd /s /q Make\Windows\ZSDiagram\Win32
rd /s /q Make\Windows\ZSDiagram\x64

rd /s /q Make\Windows\ZSDraw\Win32
rd /s /q Make\Windows\ZSDraw\x64

rd /s /q Make\Windows\ZSIpc\Win32
rd /s /q Make\Windows\ZSIpc\x64

rd /s /q Make\Windows\ZSIpcGUI\Win32
rd /s /q Make\Windows\ZSIpcGUI\x64

rd /s /q Make\Windows\ZSPhysSizes\Atomic\Win32
rd /s /q Make\Windows\ZSPhysSizes\Atomic\x64

rd /s /q Make\Windows\ZSPhysSizes\Electricity\Win32
rd /s /q Make\Windows\ZSPhysSizes\Electricity\x64

rd /s /q Make\Windows\ZSPhysSizes\Geometry\Win32
rd /s /q Make\Windows\ZSPhysSizes\Geometry\x64

rd /s /q Make\Windows\ZSPhysSizes\Kinematics\Win32
rd /s /q Make\Windows\ZSPhysSizes\Kinematics\x64

rd /s /q Make\Windows\ZSPhysSizes\Mechanics\Win32
rd /s /q Make\Windows\ZSPhysSizes\Mechanics\x64

rd /s /q Make\Windows\ZSPhysSizes\Nuclionics\Win32
rd /s /q Make\Windows\ZSPhysSizes\Nuclionics\x64

rd /s /q Make\Windows\ZSPhysSizes\Photometry\Win32
rd /s /q Make\Windows\ZSPhysSizes\Photometry\x64

rd /s /q Make\Windows\ZSPhysSizes\Thermodynamic\Win32
rd /s /q Make\Windows\ZSPhysSizes\Thermodynamic\x64

rd /s /q Make\Windows\ZSPhysVal\Win32
rd /s /q Make\Windows\ZSPhysVal\x64

rd /s /q Make\Windows\ZSPhysValGUI\Win32
rd /s /q Make\Windows\ZSPhysValGUI\x64

rd /s /q Make\Windows\ZSRemCmd\Win32
rd /s /q Make\Windows\ZSRemCmd\x64

rd /s /q Make\Windows\ZSRemCmdGUI\Win32
rd /s /q Make\Windows\ZSRemCmdGUI\x64

rd /s /q Make\Windows\ZSSNMP\Win32
rd /s /q Make\Windows\ZSSNMP\x64

rd /s /q Make\Windows\ZSSNMPGUI\Win32
rd /s /q Make\Windows\ZSSNMPGUI\x64

rd /s /q Make\Windows\ZSSys\Win32
rd /s /q Make\Windows\ZSSys\x64

rd /s /q Make\Windows\ZSSysGUI\Win32
rd /s /q Make\Windows\ZSSysGUI\x64

rd /s /q Make\Windows\ZSTest\Win32
rd /s /q Make\Windows\ZSTest\x64

rd /s /q Make\Windows\ZSTestGUI\Win32
rd /s /q Make\Windows\ZSTestGUI\x64

rd /s /q Make\Windows\ZSTrace\Win32
rd /s /q Make\Windows\ZSTrace\x64

rd /s /q Make\Windows\ZSTraceGUI\Win32
rd /s /q Make\Windows\ZSTraceGUI\x64

rd /s /q Apps\Products\ZSDataClient\Make\Windows\Win32
rd /s /q Apps\Products\ZSDataClient\Make\Windows\x64

rd /s /q Apps\Products\ZSDraw\Make\Windows\Win32
rd /s /q Apps\Products\ZSDraw\Make\Windows\x64

rd /s /q Apps\Products\ZSRemCmdClient\Make\Windows\Win32
rd /s /q Apps\Products\ZSRemCmdClient\Make\Windows\x64

rd /s /q Apps\Products\ZSSNMPExplorer\Make\Windows\Win32
rd /s /q Apps\Products\ZSSNMPExplorer\Make\Windows\x64

rd /s /q Apps\Products\ZSTrcCtxClient\Make\Windows\Win32
rd /s /q Apps\Products\ZSTrcCtxClient\Make\Windows\x64

rd /s /q Apps\Products\ZSTrcMsgClient\Make\Windows\Win32
rd /s /q Apps\Products\ZSTrcMsgClient\Make\Windows\x64

rd /s /q Apps\Products\ZSTrcMthClient\Make\Windows\Win32
rd /s /q Apps\Products\ZSTrcMthClient\Make\Windows\x64

rd /s /q Apps\Products\ZSAnalyseAcousticSignals\Db\Make\Windows\Win32
rd /s /q Apps\Products\ZSAnalyseAcousticSignals\Db\Make\Windows\x64

rd /s /q Apps\Products\ZSAnalyseAcousticSignals\DbClientCapture\Make\Windows\Win32
rd /s /q Apps\Products\ZSAnalyseAcousticSignals\DbClientCapture\Make\Windows\x64

rd /s /q Apps\Products\ZSAnalyseAcousticSignals\DbClientGUI\Make\Windows\Win32
rd /s /q Apps\Products\ZSAnalyseAcousticSignals\DbClientGUI\Make\Windows\x64

rd /s /q Apps\Products\ZSAnalyseAcousticSignals\DbClientSigGen\Make\Windows\Win32
rd /s /q Apps\Products\ZSAnalyseAcousticSignals\DbClientSigGen\Make\Windows\x64

rd /s /q Apps\Products\ZSAnalyseAcousticSignals\App\Make\Windows\Win32
rd /s /q Apps\Products\ZSAnalyseAcousticSignals\App\Make\Windows\x64

rd /s /q Apps\Test\ZSData\Make\Windows\Win32
rd /s /q Apps\Test\ZSData\Make\Windows\x64

rd /s /q Apps\Test\ZSDiagram\Make\Windows\Win32
rd /s /q Apps\Test\ZSDiagram\Make\Windows\x64

rd /s /q Apps\Test\ZSErrLogModel\Make\Windows\Win32
rd /s /q Apps\Test\ZSErrLogModel\Make\Windows\x64

rd /s /q Apps\Test\ZSObjPoolModel\Make\Windows\Win32
rd /s /q Apps\Test\ZSObjPoolModel\Make\Windows\x64

rd /s /q Apps\Test\ZSPhysVal\Make\Windows\Win32
rd /s /q Apps\Test\ZSPhysVal\Make\Windows\x64

rd /s /q Apps\Test\ZSRemCmdServer\Make\Windows\Win32
rd /s /q Apps\Test\ZSRemCmdServer\Make\Windows\x64

rd /s /q Apps\Test\ZSTrcServer\Make\Windows\Win32
rd /s /q Apps\Test\ZSTrcServer\Make\Windows\x64

rd /s /q Lib
rd /s /q Bin
rd /s /q var/log
rd /s /q var
rd /s /q var
rd /s /q var
rd /s /q etc

if /i "%MakedDir%" == "0" goto EOF

cd ./Make
cd ./Windows

:EOF
