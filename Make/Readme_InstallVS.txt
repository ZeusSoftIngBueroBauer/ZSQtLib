Building fails in VS2019 with Toolset VS2017: can't find atlbase.h

- see https://developercommunity.visualstudio.com/content/problem/625902/vc-includepath-incorrect-building-fails-in-vs2019.html

  This issue is caused by the fact that the V141 ATL component is not installed.
  There is no atlmfc folder in "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.16.27023" path.
  Open Visual Studio installer, install the "C++ ATL for c141 build tools (x86 & x64)" component under "Individual components" page and recheck the issue.
  Select the Desktop workload and requested v141 build tools.



