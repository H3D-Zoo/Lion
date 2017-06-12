if not exist build mkdir build
cd build

if not exist vs2008 mkdir vs2008
cd vs2008
cmake ../../ -G "Visual Studio 9 2008"

REM vs2015
cd ..
if not exist vs2015 mkdir vs2015
cd vs2015
cmake ../../ -G "Visual Studio 14 2015" 

pause
